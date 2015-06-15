/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/projectpatcher.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: projectpatcher.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/projectpatcher.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CDEPLOY
 *
 * =COMPONENT           Deploy
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  09.07.2002  JD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/





#include "stdafx.h"
#include "Deploy.h"
#include "ProjectPatcher.h"
#include "utilif.h"
#include "fc_tools\fc_registry.h"
#include "ComHelper.h"

#include "cedl.h"
#include "cedl_i.c"

// d-188 02.02.05 SIS >>
#include <winsvc.h>
// d-188 02.02.05 SIS <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectPatcher::CProjectPatcher()
{
    m_iAction = ACT_UNDEF;
    m_bSilent = FALSE;
    m_bPatchOneTarget = FALSE;
    m_strLocalMachine = _T("localhost");
}

CProjectPatcher::~CProjectPatcher()
{
    POSITION pos = m_listPatchTargetInfos.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* ptinfo = m_listPatchTargetInfos.GetNext(pos);
        delete ptinfo;
    }
    m_listPatchTargetInfos.RemoveAll();
    pos = m_listProjectTargetInfos.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* ptinfo = m_listProjectTargetInfos.GetNext(pos);
        delete ptinfo;
    }
    m_listPatchTargetInfos.RemoveAll();

}


int CProjectPatcher::DoAction()
{
    int iRet = -1;
    switch(m_iAction)
    {
    case ACT_SHOW:
        iRet = FindProjectTargetInfo();
        if (iRet==ERR_OK)
        {
            ShowProjectTargetInfo();
        }
        break;
    case ACT_PATCH:
        iRet = FindProjectTargetInfo();
        if (iRet==ERR_OK)
        {
            iRet = UpdatePatchInfo();
            if (iRet!=ERR_OK)
            {
                return iRet;
            }
            if (PatchFileViewXML()!=ERR_OK)
            {
                iRet = ERR_ERROR;
            }
            if (PatchAllTargets()!=ERR_OK)
            {
                iRet = ERR_ERROR;
            }
            if (PatchProjectSem()!=ERR_OK)
            {
                iRet = ERR_ERROR;
            }
        }
        break;
    }

    return iRet;
}



void CProjectPatcher::SetAction(const int iAct)
{
    m_iAction = iAct;
}



void CProjectPatcher::SetSilentMode(const BOOL bSilent)
{
    m_bSilent = bSilent;
}



void CProjectPatcher::SetProjectDir(const TCHAR* pszProjDir)
{
    CString strProjDir(pszProjDir);
    if (strProjDir.GetLength()>4 && strProjDir.Right(4).CompareNoCase(_T(".4cp"))==0)
    {
        CString strProjFile;
        int pos = strProjDir.ReverseFind(_T('\\'));
        if (pos>0)
        {
            strProjFile = strProjDir.Mid(pos);
            strProjDir = strProjDir.Left(pos);
            m_strProjFile = strProjFile;
        }
    }


    m_strProjDir = strProjDir;
}



BOOL CProjectPatcher::AddPatchTargetInfo(const TCHAR* pszTargetInfo)
{
    if (m_bPatchOneTarget)
    {
        return FALSE;
    }

    CString strTInfo = pszTargetInfo;
    int i = strTInfo.Find(':');
    if (i<0)
    {
        return FALSE;
    }
    TTargetInfo* ptinfo = new TTargetInfo;
    if (!ptinfo)
    {
        return FALSE;
    }
	CString strTargetAddress = strTInfo.Mid(i+1);
    ptinfo->strTargetName = strTInfo.Left(i);

	// if new target address is not of form CommChannel(CommParam)
	// assume TCP
	if(strTargetAddress.Find(_T('(')) == -1)
	{
		ptinfo->strTargetNewAddress.Format(_T("TCP(%s)"), (LPCTSTR)strTargetAddress);
	}
	else
	{
		ptinfo->strTargetNewAddress = strTargetAddress;
	}

    if (ptinfo->strTargetName.Compare("*")==0)
    {
        if (m_listPatchTargetInfos.GetCount()>0)
        {
            return FALSE;
        }
        m_bPatchOneTarget = TRUE;
    }

    m_listPatchTargetInfos.AddTail(ptinfo);

    return TRUE;
}



int CProjectPatcher::FindProjectTargetInfo()
{
    int ires = ERR_OK;
    ires = GetProjectTargets();
    if (ires!=ERR_OK)
    {
        return ires;
    }
    POSITION pos = m_listProjectTargetInfos.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* pTInfo = m_listProjectTargetInfos.GetNext(pos);
        if (pTInfo)
        {
            ires = FillProjectTargetInfo(pTInfo);
        }
    }
    return ires;
}


void CProjectPatcher::ShowProjectTargetInfo()
{
    int iNum = m_listProjectTargetInfos.GetCount();
    printf("The project %s contains %i targets:\n", m_strProjDir, iNum);
    POSITION pos = m_listProjectTargetInfos.GetHeadPosition();
    while (pos)
    {
        TTargetInfo* ptinfo = m_listProjectTargetInfos.GetNext(pos);
        printf(" %s\t:\t%s\n", ptinfo->strTargetName, ptinfo->strTargetAddress);
    }
}


int CProjectPatcher::GetProjectTargets()
{
    CString strProjFile;
    strProjFile = FindProjectFile();
    if (strProjFile.IsEmpty())
    {
       return ERR_ERROR;
    }

    strProjFile = m_strProjDir + _T("\\") + strProjFile;

    // parse projetc file 
    // search [TARGETS] line
    // look at all following lines until new [...] or end of file
    // if they match "file=***" --> *** is a target file.
    CStdioFile file;
    if (!file.Open(strProjFile, CFile::modeRead|CFile::typeText))
    {
        return ERR_ERROR;
    }

    try
    {
        BOOL bStart = FALSE;
        BOOL bFinished = FALSE;
        CString strLine;
        while(!bFinished && file.ReadString(strLine))
        {
            strLine.TrimLeft();
            strLine.TrimRight();
            if (strLine.IsEmpty())
            {
                continue;
            }
            if (strLine.CompareNoCase("[TARGETS]")==0)
            {
                bStart = TRUE;
            }
            else if (bStart)
            {
                if (strLine[0] == '[')
                {
                    bFinished = TRUE;
                }
                else
                {
                    if (strLine[0]!=';')
                    {
                        CString strFileName;
                        int gpos = strLine.Find('=');
                        if (gpos>0 && gpos+1<strLine.GetLength())
                        {
                            strFileName = strLine.Mid(gpos+1);
                            strFileName.TrimLeft();
                            strFileName.TrimRight();

                            if (!strFileName.IsEmpty())
                            {
                                int ppos = strFileName.Find('.');
                                if (ppos>0 && ppos+1<strFileName.GetLength())
                                {
                                    TTargetInfo* pTInfo = new TTargetInfo;
                                    if (!pTInfo)
                                    {
                                        return ERR_ERROR;
                                    }

                                    pTInfo->strTargetName = strFileName.Left(ppos);
                                    pTInfo->strTargetFile = strFileName;
                                    pTInfo->strTargetType = strFileName.Mid(ppos+1);
                                    m_listProjectTargetInfos.AddTail(pTInfo);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch(CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
        return ERR_ERROR;
    }
    
    return ERR_OK;
}



int CProjectPatcher::FillProjectTargetInfo(TTargetInfo* pTInfo)
{
    CString strProjFile = m_strProjDir + _T("\\") + pTInfo->strTargetFile;

    CXMLDocument doc;
 
    if (!doc.Load(strProjFile))
    {
        if (!m_bSilent)
        {
            printf("Error: Error reading target file: %s!", strProjFile);
        }
        return ERR_ERROR;
    }

    CXMLNode node;
    if (!doc.GetRootNode(node))
    {
        if (!m_bSilent)
        {
            printf("Error: Error reading target file: %s!", strProjFile);
        }
        return ERR_ERROR;
    }

    CString strControl;
    if (!node.GetAttribute("CONTROL", strControl))
    {
        if (!m_bSilent)
        {
            printf("Error: Error reading target file: %s!", strProjFile);
        }
        return ERR_ERROR;
    }

    CString strOPCServer;
    CString strOPCInstName;
	CString strCommChannel;
	CString strConnectPar;

    HRESULT hr = UTIL_ParseControlString(strControl,
                                        strOPCServer.GetBuffer(UTIL_MAX_OPC_SERVER_LEN), 
                                        NULL,
                                        strOPCInstName.GetBuffer(UTIL_MAX_OPC_INST_NAME_LEN),
                                        strCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
                                        strConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN),
                                        NULL,
                                        NULL,
                                        NULL);

    strOPCServer.ReleaseBuffer();
    strOPCInstName.ReleaseBuffer();
	strCommChannel.ReleaseBuffer();
	strConnectPar.ReleaseBuffer();

    pTInfo->strTargetOPCInstName = strOPCInstName;
    pTInfo->strTargetAddress.Format(_T("%s(%s)"), (LPCTSTR)strCommChannel, (LPCTSTR)strConnectPar);
    pTInfo->strTargetOPCServer = strOPCServer;

    CString strResource;
    if (node.GetAttribute("RESOURCE", strResource))
    {
        pTInfo->strTargetResource = strResource;
    }

    return ERR_OK;
}



CString CProjectPatcher::FindProjectFile()
{
    if (!m_strProjFile.IsEmpty())
    {
        return m_strProjFile;
    }

    CString strProjFile;
    BOOL bFoundOne = FALSE;
    BOOL bFound = TRUE;

    HANDLE hfile;
    WIN32_FIND_DATA findFileData;
    char szFilePattern[MAX_PATH];
    strcpy(szFilePattern, m_strProjDir);
    strcat(szFilePattern, "\\*.4cp");
    hfile = FindFirstFile(szFilePattern, &findFileData);
    
    if (hfile == INVALID_HANDLE_VALUE)
    {
        if (!m_bSilent)
        {
            printf("Error: No ATCMControl project file found.\n");
        }
    }
    else
    {
        while(bFound)
        {
            CString strFileExt;
            CString strProjFileTemp = findFileData.cFileName;
            int i = strProjFileTemp.Find('.');
            if (i>0 && i+1<strProjFileTemp.GetLength())
            {
                strFileExt = strProjFileTemp.Mid(i+1);
            }
            if (strFileExt.CompareNoCase("4cp")==0)
            {
                if (bFoundOne == TRUE)
                {
                    if (!m_bSilent)
                    {
                        printf("Error: More than one ATCMControl project file found.\n");
                    }
                    strProjFile.Empty();
                }
                else
                {
                    bFoundOne = TRUE;
                    strProjFile = strProjFileTemp;
                }
            }
            bFound = FindNextFile(hfile, &findFileData);
        }
    }

    FindClose(hfile);
    return strProjFile;
}


CString CProjectPatcher::GetLocalMachineName()
{
    CString strLocMachineName;
    if (S_OK != UTIL_GetLocalMachineName(strLocMachineName.GetBuffer(MAX_PATH), MAX_PATH))
    {
        if (!m_bSilent)
        {
            printf("Error: Getting local machine name!\n");
        }
    }
    strLocMachineName.ReleaseBuffer();

    return strLocMachineName;
}


int CProjectPatcher::UpdatePatchInfo()
{
    // todo: handle * as one and only target
   

    POSITION posPatch = m_listPatchTargetInfos.GetHeadPosition();
    while(posPatch)
    {
        TTargetInfo* pPatchTarget = m_listPatchTargetInfos.GetNext(posPatch);
        if (!pPatchTarget)
        {
            continue;
        }

        // handle * as one and only target
        if (pPatchTarget->strTargetName.Compare(_T("*"))==0)
        {
            if (m_listPatchTargetInfos.GetCount()>1)
            {
                if (!m_bSilent)
                {
                    printf("Error: If * is given as target name, only one target can be patched!\n");
                }
                return ERR_ERROR;
            }
            if (m_listProjectTargetInfos.GetCount()!=1)
            {
                if (!m_bSilent)
                {
                    printf("Error: If * is given as target name, the project may contain only one target!\n");
                }
                return ERR_ERROR;
            }
            TTargetInfo* pProjHead = m_listProjectTargetInfos.GetHead();
            pPatchTarget->strTargetName = pProjHead->strTargetName;
        }
        
        BOOL bFound = FALSE;
        POSITION posProj = m_listProjectTargetInfos.GetHeadPosition();
        while(!bFound && posProj)
        {
            TTargetInfo* pProjTarget = m_listProjectTargetInfos.GetNext(posProj);
            if (!pProjTarget)
            {
                continue;
            }
            if (pPatchTarget->strTargetName.CompareNoCase(pProjTarget->strTargetName)==0)
            {
                bFound = TRUE;
                pPatchTarget->strTargetAddress = pProjTarget->strTargetAddress;
                pPatchTarget->strTargetFile = pProjTarget->strTargetFile;
                pPatchTarget->strTargetOPCServer = pProjTarget->strTargetOPCServer;
				pPatchTarget->strTargetOPCInstName = pProjTarget->strTargetOPCInstName;
                pPatchTarget->strTargetResource = pProjTarget->strTargetResource;
                pPatchTarget->strTargetType = pProjTarget->strTargetType;
            }
        }

        if (!bFound)
        {
            if (!m_bSilent)
            {
                printf("Error: Target %s not found in project!\n", pPatchTarget->strTargetName);
            }
            return ERR_ERROR;
        }
    }
   
    return ERR_OK;
}


int CProjectPatcher::PatchFileViewXML()
{
    CXMLDocument doc;
    CString strFileName = m_strProjDir + "\\fileview.xml";
    if (!doc.Load(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file fileview.xml!\n");
        }
        return ERR_ERROR;
    }

    CXMLNode node;
    if (!doc.GetRootNode(node))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file fileview.xml!\n");
        }
        return ERR_ERROR;
    }

    CString strNewText = m_strProjDir + _T("\\");
    CString strProjFile = FindProjectFile();
    int i = strProjFile.ReverseFind('.');
    if (i>0)
    {
        strProjFile = strProjFile.Left(i);
    }
    strNewText = strNewText + strProjFile;

    if (!node.SetAttribute("TEXT", strNewText))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file fileview.xml!\n");
        }
    }

    if (!node.SetAttribute("ID", strNewText))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file fileview.xml!\n");
        }
    }

    doc.Save(strFileName);

    return ERR_OK;
}

int CProjectPatcher::PatchAllTargets()
{
    int iRet = ERR_OK;

    POSITION pos = m_listPatchTargetInfos.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* pTInfo = m_listPatchTargetInfos.GetNext(pos);
        if (pTInfo==NULL)
        {
            continue;
        }
        if (PatchTarget(pTInfo) != ERR_OK)
        {
            iRet = ERR_ERROR;
        }
    }
    if (PatchProjectDBI()!=ERR_OK)
    {
        iRet = ERR_ERROR;
    }
    return iRet;
}

int CProjectPatcher::PatchTarget(TTargetInfo* pTInfo)
{
    int iRet = ERR_OK;
    if (PatchTargetFile(pTInfo) != ERR_OK)
    {
        iRet = ERR_ERROR;
    }
    if (PatchTargetLoaderList(pTInfo) != ERR_OK)
    {
        iRet = ERR_ERROR;
    }
    if (PatchTargetTRG(pTInfo) != ERR_OK)
    {
        iRet = ERR_ERROR;
    }
    if (PatchTargetINST(pTInfo) != ERR_OK)
    {
        iRet = ERR_ERROR;
    }
    if (PatchOPCServer(pTInfo) != ERR_OK)
    {
        iRet = ERR_ERROR;
    }

    return ERR_OK;
}

int CProjectPatcher::PatchTargetFile(TTargetInfo* pTInfo)
{
    CString strFileName = m_strProjDir;
    strFileName += _T("\\") + pTInfo->strTargetFile;

    CXMLDocument doc;
    
    if (!doc.Load(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching target file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    CXMLNode node;

    if (!doc.GetRootNode(node))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching target file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    CString strControl;

    if (!node.GetAttribute(_T("CONTROL"), strControl))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching target file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    CString strOPCServer;
    CString strCustomConnectGuid;
    CString strOPCInstName;
    CString strCommChannel;
    CString strConnectPar;
    CString strProtocol;
    CString strProtocolPar;
    CString strFirmwareVersion;

    if (S_OK != UTIL_ParseControlString(strControl,
                                        strOPCServer.GetBuffer(UTIL_MAX_OPC_SERVER_LEN), 
                                        strCustomConnectGuid.GetBuffer(UTIL_MAX_GUID_LEN), 
                                        strOPCInstName.GetBuffer(UTIL_MAX_OPC_INST_NAME_LEN),
                                        strCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
                                        strConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN),
                                        strProtocol.GetBuffer(UTIL_MAX_PROTOCOL_LEN),
                                        strProtocolPar.GetBuffer(UTIL_MAX_PROTOCOL_PAR_LEN),
                                        strFirmwareVersion.GetBuffer(UTIL_MAX_FIRMWARE_VERSION_LEN)))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching target file %s!\n", strFileName);
        }

        strOPCServer.ReleaseBuffer();
        strCustomConnectGuid.ReleaseBuffer();
        strOPCInstName.ReleaseBuffer();
        strCommChannel.ReleaseBuffer();
        strConnectPar.ReleaseBuffer();
        strProtocol.ReleaseBuffer();
        strProtocolPar.ReleaseBuffer();
        strFirmwareVersion.ReleaseBuffer();

        return ERR_ERROR;
    }

    strOPCServer.ReleaseBuffer();
    strCustomConnectGuid.ReleaseBuffer();
    strOPCInstName.ReleaseBuffer();
    strCommChannel.ReleaseBuffer();
    strConnectPar.ReleaseBuffer();
    strProtocol.ReleaseBuffer();
    strProtocolPar.ReleaseBuffer();
    strFirmwareVersion.ReleaseBuffer();

    // change relevant things
    strOPCServer = m_strLocalMachine;

	CString	strNewCommChannel;
	CString	strNewConnectPar;
	if(!EvaluateBracketedTerm(pTInfo->strTargetNewAddress, strNewCommChannel, strNewConnectPar))
	{
		strNewCommChannel = strCommChannel;
		strNewConnectPar = strConnectPar;
	}
	if(strNewCommChannel == _T("RS232"))
	{
		int iFound = strNewConnectPar.Find(_T(':'));
		ASSERT(iFound != -1);
		strOPCInstName = (iFound > 0) ? strNewConnectPar.Left(iFound) : strNewConnectPar;
	}
	else
	{
		strOPCInstName = strNewConnectPar;
	}
	pTInfo->strNewOPCInstName = strOPCInstName;		// 18.10.06 SIS

    CString strNewControl;
    
    if (S_OK != UTIL_BuildControlString(strOPCServer, 
                                        strCustomConnectGuid,
                                        strOPCInstName,
                                        strNewCommChannel, 
                                        strNewConnectPar,
                                        strProtocol,
                                        strProtocolPar,
                                        strFirmwareVersion,
                                        strNewControl.GetBuffer(UTIL_MAX_CONTROL_STRING_LEN)))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching target file %s!\n", strFileName);
        }
        strNewControl.ReleaseBuffer();
        return ERR_ERROR;
    }

    strNewControl.ReleaseBuffer();
    pTInfo->strNewConnectString = strNewControl;

    if (!node.SetAttribute(_T("CONTROL"), strNewControl))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching target file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    doc.Save(strFileName);

    return ERR_OK;
}

int CProjectPatcher::PatchProjectDBI()
{
    int iErr = ERR_OK;

    CString strFileName = m_strProjDir + _T("\\gen\\project.dbi");
    CXMLDocument doc;
    if (!doc.Load(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file project.dbi!\n");
        }
        return ERR_ERROR;
    }

    CXMLNode nodePous;
    CXMLNode nodeProject;

    if (!doc.GetRootNode(nodePous))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file project.dbi!\n");
        }
        return ERR_ERROR;
    }

    if (!nodePous.GetFirstChildWithTagName(_T("PROJECT"), nodeProject))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file project.dbi!\n");
        }
        return ERR_ERROR;
    }

    CXMLNodeList nodeList;
    nodeProject.GetChildNodeList(nodeList);
    nodeList.Reset();

    POSITION pos = m_listPatchTargetInfos.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* pTInfo = m_listPatchTargetInfos.GetNext(pos);
        if (!pTInfo)
        {
            continue;
        }   
        
        nodeList.Reset();
        BOOL bFound = FALSE;
        CXMLNode node;

        while(nodeList.Next(node))
        {
            CString attrVal;
            node.GetAttribute(_T("ID"), attrVal);
            if (attrVal.CompareNoCase(pTInfo->strTargetName)==0)
            {
                bFound = TRUE;
                break;
            }   
        }

        if (!bFound)
        {
            if (!m_bSilent)
            {
                printf("Error: Patching file project.dbi!\n");
            }
            iErr = ERR_ERROR;
            continue;
        }

        if (!node.SetAttribute(_T("PCADDR"), pTInfo->strNewConnectString))
        {
            if (!m_bSilent)
            {
                printf("Error: Patching file project.dbi!\n");
            }
            iErr = ERR_ERROR;
        }
    }

    if (!doc.Save(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file project.dbi!\n");
        }
        iErr = ERR_ERROR;
    }

    return iErr;
}

int CProjectPatcher::PatchTargetLoaderList(TTargetInfo* pTInfo)
{
    CString strFileName = m_strProjDir + _T("\\gen\\") + pTInfo->strTargetName + _T("\\loader.list");

    CStdioFile file;

    try
    {
        if (!file.Open(strFileName, CFile::modeRead|CFile::typeText))
        {
            if (!m_bSilent)
            {
                printf("Error: Patching loader.list of target %i!\n", pTInfo->strTargetName);
            }
            return ERR_ERROR;
        }

        CList<CString, CString&> listLines;

        CString strLine;
        BOOL bFound = FALSE;
        while(file.ReadString(strLine))
        {
            int i = strLine.Find(_T(';'));
            if (i>0)
            {
                if (strLine.Left(i).CompareNoCase(_T("control"))==0)
                {
                    strLine = _T("control;") + pTInfo->strNewConnectString;
                    bFound = TRUE;
                }
            }
            listLines.AddTail(strLine);
        }

        if (!bFound)
        {
            if (!m_bSilent)
            {
                printf("Error: Patching loader.list of target %i!\n", pTInfo->strTargetName);
            }
            return ERR_ERROR;
        }

        file.Close();

        if (!file.Open(strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
        {
            if (!m_bSilent)
            {
                printf("Error: Patching loader.list of target %i!\n", pTInfo->strTargetName);
            }
            return ERR_ERROR;
        }

        POSITION pos = listLines.GetHeadPosition();
        while(pos)
        {
            CString strLine = listLines.GetNext(pos);
            strLine += _T("\n");
            file.WriteString(strLine);
        }

        file.Close();
    }
    catch(CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();
        if (!m_bSilent)
        {
            printf("Error: Patching loader.list of target %i!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    return ERR_OK;
}

int CProjectPatcher::PatchTargetTRG(TTargetInfo* pTInfo)
{
    CString strFileName = m_strProjDir + _T("\\gen\\") + pTInfo->strTargetName 
                          + _T("\\") + pTInfo->strTargetName + _T(".trg");

    CXMLDocument doc;

    if (!doc.Load(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    CXMLNode node;
    
    if (!doc.GetRootNode(node))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    if (!node.SetAttribute(_T("PCADDR"), pTInfo->strNewConnectString))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    if (!doc.Save(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    return ERR_OK;
}

int CProjectPatcher::PatchTargetINST(TTargetInfo* pTInfo)
{
    int iErr = ERR_OK;

    CString strFileName = m_strProjDir + _T("\\gen\\") + pTInfo->strTargetName + _T("\\")
                          + pTInfo->strTargetName + _T(".inst");

    CXMLDocument doc;

    if (!doc.Load(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    CXMLNode node;

    if (!doc.GetRootNode(node))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    iErr = ChangeStateVarInNodes(node, pTInfo);

    if (!doc.Save(strFileName))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file %s!\n", strFileName);
        }
        return ERR_ERROR;
    }

    return iErr;
}

int CProjectPatcher::ChangeStateVarInNodes(CXMLNode& node, TTargetInfo* pTInfo)
{
    int iErr = ERR_OK;
    CString strStateVar;

    if (node.GetAttribute(_T("STATE_VAR"), strStateVar))
    {
        CString strRestStateVar;
        int i = strStateVar.ReverseFind(_T('}'));
        if (i>0)
        {
            strRestStateVar = strStateVar.Mid(i);
        }

        CString strNewStateVar = _T("{") + pTInfo->strNewConnectString + strRestStateVar;

        if (!node.SetAttribute(_T("STATE_VAR"), strNewStateVar))
        {
            iErr = ERR_ERROR;
        }
    }

    CXMLNodeList nodeList;

    if (node.GetChildNodeList(nodeList))
    {
        nodeList.Reset();

        CXMLNode childNode;
        while(nodeList.Next(childNode))
        {
            int iChildErr = ChangeStateVarInNodes(childNode, pTInfo);
            if (iChildErr!=ERR_OK)
            {
                iErr = iChildErr;
            }
        }
    }

    return iErr;

}

int CProjectPatcher::PatchProjectSem()
{
    CTime time;

    CString strFileName = m_strProjDir + _T("\\gen\\project.sem");
    CFileStatus fstatus;

    if (!CFile::GetStatus(strFileName, fstatus))
    {
        if (!m_bSilent)
        {
            printf("Error: Patching file project.sem!\n");
        }
        return ERR_ERROR;
    }

    time = CTime::GetCurrentTime();

    CTimeSpan timeSpan(0, 0, 5, 0);
    time += timeSpan;

    fstatus.m_mtime = time;

    try
    {
        CFile::SetStatus(strFileName, fstatus);
    }
    catch (CFileException* e)
    {
        HRESULT hr;
        hr = HRESULT_FROM_WIN32(e->m_lOsError);
        e->Delete();

        if (!m_bSilent)
        {
            printf("Error: Patching file project.sem!\n");
        }
        return ERR_ERROR;
    }

    return ERR_OK;
}

int CProjectPatcher::PatchOPCServer(TTargetInfo* pTInfo)
{
    int iErr = ERR_OK;
    if (PatchOPCServerConfig(pTInfo)!=ERR_OK)
    {
        iErr = ERR_ERROR;
    }
    if (PatchOPCServerRegistry(pTInfo)!=ERR_OK)
    {
        iErr = ERR_ERROR;
    }
    return iErr;
}

int CProjectPatcher::PatchOPCServerConfig(TTargetInfo* pTInfo)
{
    int iErr = ERR_OK;

    CString strInstallDir;
    size_t sizeInstDir = MAX_PATH;

    if (S_OK != UTIL_GetInstallPath(strInstallDir.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPC Server config failed!\n");
        }
        return ERR_ERROR;
    }
    strInstallDir.ReleaseBuffer();

    CString strConfigDir = strInstallDir + _T("Engineering\\bin\\") + pTInfo->strTargetType + _T("\\config\\");
    
    if (!CreateDirectory(strConfigDir, NULL))
    {
        DWORD dword = GetLastError();
        if (dword != ERROR_ALREADY_EXISTS)
        {
            if (!m_bSilent)
            {
                printf("Error: Creating OPC Server config failed!\n");
            }
            return ERR_ERROR;
        }
    }

    strConfigDir = strConfigDir + _T("_") + pTInfo->strNewOPCInstName + _T("_");		// 18.10.06 SIS
    if (!CreateDirectory(strConfigDir, NULL))
    {
        DWORD dword = GetLastError();
        if (dword != ERROR_ALREADY_EXISTS)
        {
            if (!m_bSilent)
            {
                printf("Error: Creating OPC Server config failed!\n");
            }
            return ERR_ERROR;
        }
    }
    strConfigDir = strConfigDir + _T("\\");

    CString strOrigDir = m_strProjDir +_T("\\gen\\") + pTInfo->strTargetName + _T("\\");
    CString strFindPattern = strOrigDir + _T("*.*");
    WIN32_FIND_DATA findFileData;
    HANDLE hfind;
    BOOL bFound = TRUE;

    hfind = FindFirstFile(strFindPattern, &findFileData);
    if (hfind == INVALID_HANDLE_VALUE)
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPC Server config failed!\n");
        }
        return ERR_ERROR;
    }
    
    while(bFound)
    {
        CString strOrigFileName = strOrigDir + findFileData.cFileName;
        CString strDestFileName = strConfigDir + findFileData.cFileName;

        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            if (!CopyFile(strOrigFileName, strDestFileName, FALSE))
            {
                if (!m_bSilent)
                {
                    printf("Error: Creating OPC Server config failed!\n");
                }
                iErr = ERR_ERROR;
            }   
        }
        bFound = FindNextFile(hfind, &findFileData);
    }

    FindClose(hfind);

    return ERR_OK;
}


// d-188 02.02.05 SIS >>
BOOL CProjectPatcher::IsWindowsNT(void)
{
    OSVERSIONINFO VI;

    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

    return VI.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


/* IsAdmin
 * ----------------------------------------------------------------------------
 */
BOOL CProjectPatcher::IsAdmin(void)
{
  SC_HANDLE hSC;

  if (IsWindowsNT())
  {
      hSC = OpenSCManager(NULL, NULL, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE);
      if (!hSC)
      {
        return FALSE;
      }

      CloseServiceHandle(hSC);
  }

  return TRUE;
}
// d-188 02.02.05 SIS <<

int CProjectPatcher::PatchOPCServerRegistry(TTargetInfo* pTInfo)
{
	// d-188 02.02.05 SIS >>
	if(!IsAdmin())
	{
        if (!m_bSilent)
        {
            printf("Error: User has no administrator rights. OPC server registry patching failed!\n");
        }
		return ERR_ERROR;
	}
	// d-188 02.02.05 SIS <<
    CString strKey = _T("SOFTWARE\\Softing\\4CONTROLV2\\");

    CString strOPCServerExe = FindOPCServer(pTInfo);
    if (strOPCServerExe.IsEmpty())
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;    
    }

    strKey += strOPCServerExe + _T("\\SysVars\\") + pTInfo->strNewOPCInstName;		// 18.10.06 SIS

    CString strVal = pTInfo->strNewConnectString;
    int ipos = strVal.Find(_T(','));
    if (ipos<0)
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }
    ipos = strVal.Find(_T(','), ipos+1);
    if (ipos<0)
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }
    strVal = strVal.Mid(ipos+1);

    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("CommParam"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    strVal = FindProjectFile();
    ipos = strVal.ReverseFind(_T('.'));
    if (ipos>0)
    {
        strVal = strVal.Left(ipos);
    }

    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("projectname"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }
       
    strVal = pTInfo->strTargetResource;
    strVal.MakeLower();
    
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("resourcename0"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    strKey += _T("\\softRT\\0\\DLoad");

    strVal = _T("0");
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("cleared"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    CTime time;
    time = CTime::GetCurrentTime();

    strVal.Format(_T("dt#%i-%02i-%02i-%02i:%02i:%02i.000"), 
                    time.GetYear(),
                    time.GetMonth(),
                    time.GetDay(),
                    time.GetHour(),
                    time.GetMinute(),
                    time.GetSecond());
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("date"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    strVal = GetDLGuid(pTInfo);
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("dloadguid"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    strVal = _T("localhost");
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("host"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    strVal = _T("");
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("prjguid"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    strVal = GetCurrentUser();
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("user"), 
                                           strVal))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPCServer registry values for target %s!\n", pTInfo->strTargetName);
        }
        return ERR_ERROR;
    }

    return ERR_OK;
}


CString CProjectPatcher::FindOPCServer(TTargetInfo* pTInfo)
{
    CString strInstallDir;
    size_t sizeInstDir = MAX_PATH;

    if (S_OK != UTIL_GetInstallPath(strInstallDir.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        if (!m_bSilent)
        {
            printf("Error: Creating OPC Server config failed!\n");
        }
        return ERR_ERROR;
    }
    strInstallDir.ReleaseBuffer();

    CString strConfigDir = strInstallDir + _T("Engineering\\bin\\") + pTInfo->strTargetType + _T("\\");
    
 
    CString strFindPattern = strConfigDir + _T("*SCS.exe");
    WIN32_FIND_DATA findFileData;
    HANDLE hfind;

	// 18.10.06 SIS >>
	// new opc server are named OnlServ*..
    hfind = FindFirstFile(strFindPattern, &findFileData);
    if (hfind == INVALID_HANDLE_VALUE)
    {
		// alternatively
		strFindPattern = strConfigDir + _T("OnlServ*.exe");
		hfind = FindFirstFile(strFindPattern, &findFileData);
		if (hfind == INVALID_HANDLE_VALUE)
		{
			return _T("");
		}
	}
	// 18.10.06 SIS <<

    CString strName = findFileData.cFileName;
    int ipos = strName.ReverseFind(_T('.'));
    if (ipos>0)
    {
        strName = strName.Left(ipos);
    }

    return strName;
}

CString CProjectPatcher::GetCurrentUser()
{
    TCHAR* pUserName;
    pUserName = _tgetenv(_T("USERNAME"));

    CString strUserName;
    if (pUserName)
    {
        strUserName = pUserName;
    }

    return strUserName;
}

CString CProjectPatcher::GetDLGuid(TTargetInfo* pTInfo)
{
    CString strDLGuid;

    CString strClsidDL = _T("{88AD4524-E2B2-4853-A081-5CDBF5732E09}");

    IDL* pDL = NULL;

    CGuid guid(strClsidDL);

    HRESULT hr = CoCreateInstance(guid,
		                          NULL,
		                          CLSCTX_SERVER,
		                          __uuidof(IDL), 
						          (LPVOID *)&pDL);
    if (hr!=S_OK)
    {
        if (!m_bSilent)
        {
            printf("Error: Retrieving download GUID\n");
        }
        return ERR_ERROR;
    }

    CComBSTR sTargetName = pTInfo->strTargetName;
    CComBSTR sGenPath = (CString)(m_strProjDir +_T("\\gen"));
    hr =  pDL->Init(sTargetName,sGenPath, TRUE);
    if (hr != S_OK)
    {
        if (!m_bSilent)
        {
            printf("Error: Retrieving download GUID\n");
        }
        pDL->Release();
        return ERR_ERROR;
    }

    IDLProperties* pDLProperties = NULL;
    hr = pDL->QueryInterface(IID_IDLProperties, (LPVOID *)&pDLProperties);
    if (hr != S_OK)
    {
        if (!m_bSilent)
        {
            printf("Error: Retrieving download GUID\n");
        }
        pDL->Release();
        return ERR_ERROR;
    }

    pDL->Release();

    CComBSTR sVersion;
    CComBSTR sProjName;
    hr = pDLProperties->GetTargetDLVersion(&sVersion, &sProjName);
    if (hr != S_OK)
    {
        if (!m_bSilent)
        {
            printf("Error: Retrieving download GUID\n");
        }
        pDLProperties->Release();
        return ERR_ERROR;
    }
    pDLProperties->Release();

    strDLGuid = sVersion;

    return strDLGuid;
}

//------------------------------------------------------------------*
/**
 * evaluate bracketed term.
 *
 *  bracketed term: XXXX(YYYY)
 *  XXXX: prefix
 *  YYYY: contents
 *
 * @param           crstrInput: input string (in/out)
 * @param           rstrPrefix: prefix of term
 * @param           rstrContents: contents of term
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CProjectPatcher::EvaluateBracketedTerm(const CString& crstrInput, CString& rstrPrefix, CString& rstrContents)
{
	CString strInput(crstrInput);
    CString strExtracted;   // extracted string
    int     iStart = -1;
    int     iStop = -1;
    int     iBracketsOpen = 0;
    int     iNumChars = strInput.GetLength();
    int     iChar;
    TCHAR   cCurrent;

    // search for first occurrence of bracket open
    for(iChar = 0; iChar < iNumChars; ++iChar)
    {
        if(strInput[iChar] == UTIL_BRACKET_OPEN)
        {
            ++iBracketsOpen;
            iStart = iChar + 1;
            break;
        }
    }

    if(!iBracketsOpen)
    {
        return FALSE;    // no bracket open found -> return FALSE
    }

    // search for corresponding bracket close
    for(iChar = iStart; iChar < iNumChars; ++iChar)
    {
        cCurrent = strInput[iChar];
        if(cCurrent == UTIL_BRACKET_OPEN)
        {
            ++iBracketsOpen;
        }
        if(cCurrent == UTIL_BRACKET_CLOSE)
        {
            --iBracketsOpen;
            if(iBracketsOpen == 0)
            {
                iStop = iChar;
                break;
            }
        }
    }

    if(iStop >= iStart)
    {
        strExtracted = strInput.Mid(iStart, iStop - iStart);
        rstrPrefix = strInput.Left(iStart - 1);
        strInput = strInput.Mid(iStop + 1);
        rstrContents = strExtracted;
        return TRUE;
    }

    return FALSE;
}


/*
 *----------------------------------------------------------------------------*
 * $History: projectpatcher.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 18.10.06   Time: 19:00
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 18.10.06 SIS bug fix new opc config
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 2.02.05    Time: 17:07
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 16.12.04   Time: 18:53
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * support RS232
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 26.05.04   Time: 12:20
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 24.09.02   Time: 15:30
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * FIxed patching loader.list.
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 17.09.02   Time: 13:30
 * Updated in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 17.09.02   Time: 11:08
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * Moved from Bruneck Target to product.
 * 
 *==
 *----------------------------------------------------------------------------*
*/
