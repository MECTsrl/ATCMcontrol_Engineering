// PMPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "PMPropPage.h"
#include "MainPropertySheet.h"
//#include "SelectTargetDlg.h"
#include "defs.h"
#include "utilif.h"

#include "fc_tools\fc_registry.h"
#include "ComHelper.h"

#include "cedl.h"
#include "cedl_i.c"

#include "vmSharedDef.h"

#include "resource.h"

#define ALLOC(x)	(BYTE *)HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x)	HeapFree(GetProcessHeap(), 0, (x));x=NULL


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPMPropPage property page

IMPLEMENT_DYNCREATE(CPMPropPage, CPropertyPage)

CPMPropPage::CPMPropPage(UINT uiId /*=0*/ , CMainPropertySheet* pMainSheet /*= NULL*/)
 : CPropertyPage(uiId),
   m_pMainSheet(pMainSheet)
{
    ASSERT(m_pMainSheet);

    // remove help button
    m_psp.dwFlags &= ~PSP_HASHELP;

    m_bSupportProgressCallback = FALSE;
}

CPMPropPage::~CPMPropPage()
{
    ResetTargetInfoList();
}

void CPMPropPage::InitializeFromRegistry(BOOL bGeneral /*=FALSE*/)
{
    // base version does nothing
}

void CPMPropPage::SaveToRegistry(BOOL bGeneral /*=FALSE*/)
{
    // base version does nothing
}

// base class returns empty string
CString CPMPropPage::GetProjectPath()
{
    return _T("");
}

//------------------------------------------------------------------*
/**
 * TryLoadTargetInfo
 *
 * @param           crstrProjectPath: project path
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPMPropPage::TryLoadTargetInfo(const CString& crstrProjectPath)
{
    ResetTargetInfoList();

    CFileStatus tStatus;
    if(CFile::GetStatus(crstrProjectPath, tStatus) && (tStatus.m_attribute & CFile::directory))
    {
        CString strProjectFile;
        if(FindProjectFile(crstrProjectPath, strProjectFile) == ST_OK)
        {
            return LoadTargetInfoList(crstrProjectPath, TRUE);
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * LoadTargetInfoList
 *
 * @param           crstrProjectPath: project path
 * @param           bForceReload: force reload of target info list?
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPMPropPage::LoadTargetInfoList(const CString& crstrProjectPath, BOOL bForceReload /*=FALSE*/)
{
    BOOL bLoaded = FALSE;
    if(m_strLoadedProject.CompareNoCase(crstrProjectPath) != 0 || bForceReload)
    {
        if(LoadProjectTargetInfo(crstrProjectPath) == ST_OK)
        {
            m_strLoadedProject = crstrProjectPath;
            bLoaded = TRUE;
        }
        else
        {
            m_strLoadedProject.Empty();
        }
    }
    return bLoaded;
}


//------------------------------------------------------------------*
/**
 * ResetTargetInfoList
 *
 *  delete all target info objects, empty and destroy list
 *
 * @return          -
 * @exception       -
 * @see             -
*/
void CPMPropPage::ResetTargetInfoList()
{
    POSITION pos = m_tTargetInfoList.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* pTInfo = m_tTargetInfoList.GetNext(pos);
        if (pTInfo)
        {
            delete pTInfo;
        }
    }
    m_tTargetInfoList.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * Launch4C
 *
 *  launches the engineering
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPMPropPage::Launch4C()
{
    // get installation path: install path + Engineering\bin
    CString strCmdLine;
    CString strInstallPath;
    size_t sizeInstDir = MAX_PATH;
    if(S_OK != UTIL_GetInstallPath(strInstallPath.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        return FALSE;
    }
    strInstallPath.ReleaseBuffer();
    strInstallPath += _T("Engineering\\bin");

    CString strProjectPath = GetProjectPath();
    CString strProjectFile;
    FindProjectFile(strProjectPath, strProjectFile);

    // build command line
    if(!strProjectFile.IsEmpty())
    {
        strProjectPath += _T("\\") + strProjectFile;
        strCmdLine.Format(_T("\"%s\\4ceng.exe\" \"%s\""), strInstallPath, strProjectPath);
    }
    else
    {
        strCmdLine.Format(_T("\"%s\\4ceng.exe\""), strInstallPath);
    }

    // execute command
    TRACE("%s", strCmdLine);
    if(ExecuteCommand(strCmdLine, TRUE, FALSE) != ST_OK)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * FindProjectFile
 *
 *   search for 4cp file in project directory
 *   if more than one file is found the first file is
 *   returned and the return value is ST_MULTIPLE_PROJECT_FILES
 *
 * @param           crstrProjectPath: full project path
 * @param           rstrProjFile: file name of 4cp file without path
 * @return          successful? (error codes see defs.h)
 * @exception       -
 * @see             -
*/
int CPMPropPage::FindProjectFile(const CString& crstrProjectPath, CString& rstrProjFile)
{
    CString strProjFile;
    BOOL bFoundOne = FALSE;
    BOOL bFound = TRUE;

    HANDLE hfile;
    WIN32_FIND_DATA findFileData;
    char szFilePattern[MAX_PATH];
    strcpy(szFilePattern, crstrProjectPath);
    strcat(szFilePattern, "\\*.4cp");
    hfile = FindFirstFile(szFilePattern, &findFileData);
    
    if (hfile == INVALID_HANDLE_VALUE)
    {
        return ST_ERROR;
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
                    return ST_MULTIPLE_PROJECT_FILES;
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

    rstrProjFile = strProjFile;

    return ST_OK;
}



//------------------------------------------------------------------*
/**
 * LoadProjectTargetInfo
 *
 *  calls GetProjectTargets() to get all targets contained in the project
 *  for each target a target info object is created
 *  calls FillTargetInfo for each target info object
 *
 * @param           crstrProjectPath: full project path
 * @return          successful? (error codes see defs.h)
 * @exception       -
 * @see             GetProjectTargets(), FillProjectTargetInfo()
*/
int CPMPropPage::LoadProjectTargetInfo(const CString& crstrProjectPath)
{
    ResetTargetInfoList();

    int ires = GetProjectTargets(crstrProjectPath);
    if (ires!=ERR_OK)
    {
        m_pMainSheet->SetStatusText(_T("No project file"));
        return ires;
    }

    POSITION pos = m_tTargetInfoList.GetHeadPosition();
    while(pos)
    {
        TTargetInfo* pTInfo = m_tTargetInfoList.GetNext(pos);
        if (pTInfo)
        {
            ires = FillProjectTargetInfo(pTInfo, crstrProjectPath);
        }
    }
    return ires;
}
    

//------------------------------------------------------------------*
/**
 * GetProjectTargets
 *
 *  read project file and determine contained targets
 *  for each target a target info object is created and added to the 
 *  target info list
 *  for retrieving data on a target call FillProjectTargetInfo()
 *
 * @param           crstrProjectPath: full project path
 * @return          success? (for return codes see defs.h)
 * @exception       -
 * @see             -
*/
int CPMPropPage::GetProjectTargets(const CString& crstrProjectPath)
{
    CString strProjFilePath;
    CString strProjFile;
    int iRes = FindProjectFile(crstrProjectPath, strProjFile);
    if(iRes != ST_OK)
    {
        m_pMainSheet->SetStatusText(_T("No project file"));
        return iRes;
    }

    strProjFilePath = crstrProjectPath + _T("\\") + strProjFile;

    // parse projetc file 
    // search [TARGETS] line
    // look at all following lines until new [...] or end of file
    // if they match "file=***" --> *** is a target file.
    CStdioFile file;
    if (!file.Open(strProjFilePath, CFile::modeRead|CFile::typeText))
    {
        m_pMainSheet->SetStatusText(_T("Could not read project file"));
        return ST_ERROR;
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
                                        return ST_ERROR;
                                    }

                                    pTInfo->strTargetName = strFileName.Left(ppos);
                                    pTInfo->strTargetFile = strFileName;
                                    pTInfo->strTargetType = strFileName.Mid(ppos+1);
                                    m_tTargetInfoList.AddTail(pTInfo);
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
        m_pMainSheet->SetStatusText(_T("Could not read project file"));
        return ST_ERROR;
    }
    
    return ST_OK;
}



//------------------------------------------------------------------*
/**
 * FillProjectTargetInfo
 *
 *  read target file and fill target info object
 *
 * @param           pTInfo: target info object
 * @param           crstrProjectPath: full project path
 * @return          successful? (error codes see defs.h)
 * @exception       -
 * @see             -
*/
int CPMPropPage::FillProjectTargetInfo(TTargetInfo* pTInfo, const CString& crstrProjectPath)
{
    CString strProjFilePath = crstrProjectPath + _T("\\") + pTInfo->strTargetFile;

    CXMLDocument doc;
 
    if (!doc.Load(strProjFilePath))
    {
        CString strMessage;
        strMessage.Format(IDS_ERR_READINGTARGETFILE, strProjFilePath);
        ::AfxMessageBox(strMessage, MB_OK | MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(_T("Could not read target file"));
        return ST_ERROR;
    }

    CXMLNode node;
    if (!doc.GetRootNode(node))
    {
        CString strMessage;
        strMessage.Format(IDS_ERR_READINGTARGETFILE, strProjFilePath);
        ::AfxMessageBox(strMessage, MB_OK | MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(_T("Could not read target file"));
        return ST_ERROR;
    }

    CString strControl;
    if (!node.GetAttribute("CONTROL", strControl))
    {
        CString strMessage;
        strMessage.Format(IDS_ERR_READINGTARGETFILE, strProjFilePath);
        ::AfxMessageBox(strMessage, MB_OK | MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(_T("Could not read target file"));
        return ST_ERROR;
    }
    pTInfo->strControl = strControl;

    CString strOPCServer;
	CString	strCustomConnect;	// #245 14.09.05 SIS
    CString strOPCInstName;
	CString strCommChannel;
	CString strConnectPar;

    HRESULT hr = UTIL_ParseControlString(strControl,
                                        strOPCServer.GetBuffer(UTIL_MAX_OPC_SERVER_LEN), 
                                        strCustomConnect.GetBuffer(UTIL_MAX_GUID_LEN),			// #245 14.09.05 SIS
                                        strOPCInstName.GetBuffer(UTIL_MAX_OPC_INST_NAME_LEN),
                                        strCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
                                        strConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN),
                                        NULL,
                                        NULL,
                                        NULL);

    strOPCServer.ReleaseBuffer();
	strCustomConnect.ReleaseBuffer();	// #245 14.09.05 SIS
    strOPCInstName.ReleaseBuffer();
	strCommChannel.ReleaseBuffer();
	strConnectPar.ReleaseBuffer();

    pTInfo->strTargetOPCInstName = strOPCInstName;
	pTInfo->strCustomConnect = strCustomConnect;	// #245 14.09.05 SIS
    pTInfo->strTargetAddress = strConnectPar;
    pTInfo->strTargetOPCServer = strOPCServer;

    CString strResource;
    if (node.GetAttribute("RESOURCE", strResource))
    {
        pTInfo->strTargetResource = strResource;
    }

    return ST_OK;
}



BOOL CPMPropPage::DownloadProject(const CString& crstrFilePath, TTargetInfo* pTargetInfo)
{
    CWaitCursor tWaitCur;

    BOOL bReturn = FALSE;
    CString strTargetControl = GetTargetControlString(pTargetInfo->strControl);

    // load onl comm library
    HMODULE hModule = LoadOnlCommLibraryForTarget(pTargetInfo->strTargetType);
    if (hModule == NULL)
    {
        AfxMessageBox("Error: Loading library OnlComm.dll.", MB_OK | MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(_T("Could not load online dll"));
        return FALSE;
    }
    // connect to target
    LPCONSTRUCT4CLAYER lpConstruct4CLayer = (LPCONSTRUCT4CLAYER)GetProcAddress(hModule, "Construct4CLayer");
    if (lpConstruct4CLayer == NULL)
    {
        CString strMessage(_T("Error: Getting ProcAddress Construct4CLayer."));
        AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(strMessage);
        FreeLibrary(hModule);
        hModule = NULL;
        m_pMainSheet->SetStatusText(_T("Could not get proc address for Construct4CLayer"));
        return FALSE;
    }

    C4CLayer* pLayer = lpConstruct4CLayer(strTargetControl);
    if (pLayer != NULL)
    {
        if(ConnectToTarget(pLayer))
        {
            BYTE byCommand	= CMD_SAVE_PROJECT;
	        UINT uLen		= 0;
	        BYTE *pData 	= ALLOC(uLen);
            BOOL bOK = TRUE;
	        if (!LoadFile(crstrFilePath, &pData, &uLen))
	        {
		        m_pMainSheet->SetStatusText(_T("Error: Unable to read from file"));
                DisconnectFromTarget(pLayer);
                delete pLayer;
                FreeLibrary(hModule);
                return FALSE;
	        }

            if(m_bSupportProgressCallback)
            {
                pLayer->SetProgressCallBack(&ProgressCB);
            }

	        bOK = pLayer->Transfer(&byCommand, &pData, &uLen);

            if(m_bSupportProgressCallback)
            {
                pLayer->SetProgressCallBack(NULL);
            }

	        if (! bOK)
	        {
		        m_pMainSheet->SetStatusText(_T("Error: Store Project. No answer"));
	        }
	        else if (bOK && byCommand != CMD_SAVE_PROJECT)
	        {
                CString strErrorMessage;
                short   sErrNum;
                if(pData)
                {
                    sErrNum = *(short*)pData;
                    strErrorMessage.Format(_T("Error: Store Project. Error number 0x%X"), sErrNum);
                }
                else
                {
                    strErrorMessage = _T("Error: Store Project");
                }
		        m_pMainSheet->SetStatusText(strErrorMessage);
                bOK = FALSE;
	        }
            DisconnectFromTarget(pLayer);
            bReturn = bOK;
        }
        delete pLayer;
    }

    FreeLibrary(hModule);
    hModule = NULL;
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * DownloadFile
 *
 * @param           crstrFilePath: file path
 * @param           pLayer: pointer to communication layer (already connected)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPMPropPage::DownloadFile(const CString& crstrFilePath, C4CLayer* pLayer)
{
    CWaitCursor tWaitCur;

    ASSERT(pLayer);

    if (pLayer == NULL)
    {
        return FALSE;
    }
    // TODO:
    // IsCommOpen() and IsConnected() are inversely implemented
    // i.e. if IsConnected == TRUE means not connected
    // and IsCommOpen == TRUE means comm not open
    // this may be changed later
    if(pLayer->IsCommOpen() || pLayer->IsConnected())
    {
        m_pMainSheet->SetStatusText(_T("Error: Not connected"));
        return FALSE;
    }

    BYTE byCommand	= CMD_SAVE_FILE;
	UINT uLen		= 0;
	BYTE *pData 	= ALLOC(uLen);
    BOOL bOK = TRUE;
	if (!LoadFile(crstrFilePath, &pData, &uLen))
	{
		m_pMainSheet->SetStatusText(_T("Error: Unable to read from file"));
        return FALSE;
	}

    CString sFile(crstrFilePath);

	int iPos = sFile.ReverseFind('\\');
	if (iPos != -1)
	{
		sFile = sFile.Right(sFile.GetLength() - iPos - 1);
	}

	BYTE *pFile = ALLOC(sFile.GetLength() + 1 + uLen);
	memcpy(pFile, sFile, sFile.GetLength());
	pFile[sFile.GetLength()] = 0;
	memcpy(pFile + sFile.GetLength() + 1, pData, uLen);

	FREE(pData);

	pData = pFile;
	uLen += 1 + sFile.GetLength();
    
    if(m_bSupportProgressCallback)
    {
        pLayer->SetProgressCallBack(&ProgressCB);
    }

	bOK = pLayer->Transfer(&byCommand, &pData, &uLen);

    if(m_bSupportProgressCallback)
    {
        pLayer->SetProgressCallBack(NULL);
    }

	if (! bOK)
	{
		m_pMainSheet->SetStatusText(_T("Error: Store File. No answer"));
	}
	else if (bOK && byCommand != CMD_SAVE_FILE)
	{
        CString strErrorMessage;
        short   sErrNum;
        if(pData)
        {
            sErrNum = *(short*)pData;
            strErrorMessage.Format(_T("Error: Store File. Error number 0x%X"), sErrNum);
        }
        else
        {
            strErrorMessage = _T("Error: Store File");
        }
		m_pMainSheet->SetStatusText(strErrorMessage);
        bOK = FALSE;
	}
    return bOK;
}

bool ProgressCB(int iCurrent, int iMax)
{
    CMainPropertySheet* pMainSheet = (CMainPropertySheet*)::AfxGetMainWnd();
    if(iCurrent == 0)
    {
        pMainSheet->InitProgress(iCurrent, iMax, TRUE);

        pMainSheet->SetProgress(iCurrent, iMax, TRUE);
    }
	
    pMainSheet->SetProgress(iCurrent, iMax, TRUE);
    
    if(iCurrent == iMax)
    {
        pMainSheet->StopProgress(TRUE);
    }
    return true;
}


//------------------------------------------------------------------*
/**
 * UploadFile
 *
 * @param           crstrFilePath: destination file path
 * @param           pLayer: pointer to communication layer (already connected)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPMPropPage::UploadFile(const CString& crstrFilePath, C4CLayer* pLayer)
{
    CWaitCursor tWaitCur;
    CString     strFileName;    // file name

    int iFound = crstrFilePath.ReverseFind(_T('\\'));
    if(iFound == -1)
    {
        CString strMessage;
        strMessage.Format(_T("Incorrect file path: %s"), crstrFilePath);
        m_pMainSheet->SetStatusText(strMessage);
        return FALSE;
    }
    strFileName = crstrFilePath.Mid(iFound + 1);

    ASSERT(pLayer);

    if(!pLayer)
    {
        return FALSE;
    }
    // TODO:
    // IsCommOpen() and IsConnected() are inversely implemented
    // i.e. if IsConnected == TRUE means not connected
    // and IsCommOpen == TRUE means comm not open
    // this may be changed later
    if(pLayer->IsCommOpen() || pLayer->IsConnected())
    {
        m_pMainSheet->SetStatusText(_T("Error: Not connected"));
        return FALSE;
    }

    BYTE byCommand	= CMD_LOAD_FILE;
	UINT uLen		= strFileName.GetLength() + 1;
	BYTE *pData 	= ALLOC(uLen);
    strcpy((char *)pData, strFileName);

    if(m_bSupportProgressCallback)
    {
        pLayer->SetProgressCallBack(&ProgressCB);
    }

	BOOL bOK = pLayer->Transfer(&byCommand, &pData, &uLen);
    
    if(m_bSupportProgressCallback)
    {
        pLayer->SetProgressCallBack(NULL);
    }

	if (! bOK)
	{
		m_pMainSheet->SetStatusText(_T("Error: Upload File. No answer"));
	}
	else if (bOK && byCommand != CMD_LOAD_FILE)
	{
        CString strErrorMessage;
        short   sErrNum;
        if(pData)
        {
            sErrNum = *(short*)pData;
            strErrorMessage.Format(_T("Error: Upload File. Error number 0x%X"), sErrNum);
        }
        else
        {
            strErrorMessage = _T("Error: Upload File");
        }
		m_pMainSheet->SetStatusText(strErrorMessage);
        bOK = FALSE;
	}
	if (bOK && !StoreFile(crstrFilePath, pData, uLen))
	{
		m_pMainSheet->SetStatusText(_T("Error: Unable to store to file"));
        bOK = FALSE;
	}

    return bOK;
}


BOOL CPMPropPage::UploadDirectoryContent(CStringArray& rastrFiles, C4CLayer* pLayer)
{
    CWaitCursor tWaitCur;

    ASSERT(pLayer);

    if(!pLayer)
    {
        return FALSE;
    }
    // TODO:
    // IsCommOpen() and IsConnected() are inversely implemented
    // i.e. if IsConnected == TRUE means not connected
    // and IsCommOpen == TRUE means comm not open
    // this may be changed later
    if(pLayer->IsCommOpen() || pLayer->IsConnected())
    {
        m_pMainSheet->SetStatusText(_T("Error: Not connected"));
        return FALSE;
    }

	BYTE byCommand	= CMD_DIR_CONTENT;
	UINT uLen		= 0;
	BYTE *pData		= ALLOC(uLen);
	BOOL bOK;
    
	bOK = pLayer->Transfer(&byCommand, &pData, &uLen);

	if (! bOK)
	{
        m_pMainSheet->SetStatusText(_T("Error: Directory content. No answer."));
        bOK = FALSE;
	}
	else if (bOK && byCommand != CMD_DIR_CONTENT)
	{
        CString strErrorMessage;
        short   sErrNum;
        if(pData)
        {
            sErrNum = *(short*)pData;
            strErrorMessage.Format(_T("Error: Directory content. Error number 0x%X"), sErrNum);
        }
        else
        {
            strErrorMessage = _T("Error: Directory content");
        }
		m_pMainSheet->SetStatusText(strErrorMessage);
        bOK = FALSE;
	}

	if (uLen != 0)
	{
		for (unsigned i = 0; i < uLen - 1; i += MAX_PATH)
		{
			rastrFiles.Add((LPCTSTR)pData + i);
		}
	}
    return bOK;
}


//------------------------------------------------------------------*
/**
 * DeleteControlFile
 *
 *  deletes a file on the control
 *
 * @param           crstrFilePath: up to now only file name
 * @param           pLayer: pointer to communication layer (already connected)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPMPropPage::DeleteControlFile(const CString& crstrFilePath, C4CLayer* pLayer)
{
    CWaitCursor tWaitCur;
    CString     strFileName(crstrFilePath);

    ASSERT(pLayer);

    if(!pLayer)
    {
        return FALSE;
    }
    // TODO:
    // IsCommOpen() and IsConnected() are inversely implemented
    // i.e. if IsConnected == TRUE means not connected
    // and IsCommOpen == TRUE means comm not open
    // this may be changed later
    if(pLayer->IsCommOpen() || pLayer->IsConnected())
    {
        m_pMainSheet->SetStatusText(_T("Error: Not connected"));
        return FALSE;
    }

    BYTE byCommand	= CMD_DEL_FILE;
	UINT uLen		= strFileName.GetLength() + 1;
	BYTE *pData 	= ALLOC(uLen);
    strcpy((char *)pData, strFileName);

	BOOL bOK = pLayer->Transfer(&byCommand, &pData, &uLen);
    
	if (! bOK)
	{
		m_pMainSheet->SetStatusText(_T("Error: Delete File. No answer"));
	}
	else if (bOK && byCommand != CMD_DEL_FILE)
	{
        CString strErrorMessage;
        short   sErrNum;
        if(pData)
        {
            sErrNum = *(short*)pData;
            strErrorMessage.Format(_T("Error: Delete File. Error number 0x%X"), sErrNum);
        }
        else
        {
            strErrorMessage = _T("Error: Delete File");
        }
		m_pMainSheet->SetStatusText(strErrorMessage);
        bOK = FALSE;
	}

    return bOK;
}


BOOL CPMPropPage::ConnectToTarget(C4CLayer* pLayer)
{
    BOOL bReturn = TRUE;
    if(pLayer->IsCommOpen())                // caution: inverse implementation of IsCommOpen() (TODO)
    {
        bReturn = pLayer->OpenComm();
    }
    if(bReturn && pLayer->IsConnected())    // caution: inverse implementation of IsConnected() (TODO)
    {
        bReturn = pLayer->Connect();
    }
    if(!bReturn)
    {
        m_pMainSheet->SetStatusText(_T("Error: Could not connect to target"));
        pLayer->CloseComm();
    }
    SetConnectStatus(!pLayer->IsConnected());	// TODO: inverse implementation of IsConnected()
    return bReturn;
}


void CPMPropPage::DisconnectFromTarget(C4CLayer* pLayer)
{
    if(pLayer)
    {
        if(!pLayer->IsConnected())      // caution: inverse implementation (TODO)
        {
            pLayer->Disconnect();
        }
        if(!pLayer->IsCommOpen())       // caution: inverse implementation (TODO)
        {
            pLayer->CloseComm();
        }
    }
    SetConnectStatus(FALSE);
}


BOOL CPMPropPage::UploadProject(const CString& crstrFilePath, const CString& crstrTargetControl, const CString& crstrTargetType)
{
    CWaitCursor tWaitCur;
    
    BOOL bReturn = FALSE;
    // load onl comm library
    // TODO:
    // let user specify target type
    // try to load onl comm library in local directory
    HMODULE hModule = LoadOnlCommLibraryForTarget(crstrTargetType);
    //HMODULE hModule = LoadLocalOnlCommLibrary();
    if(hModule == NULL)
    {
        // use win target if not found locally
        hModule = LoadOnlCommLibraryForTarget(_T("4CWin"));
    }
    if (hModule == NULL)
    {
        AfxMessageBox("Error: Loading library OnlComm.dll.", MB_OK | MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(_T("Could not load online dll"));
        return FALSE;
    }
    // connect to target
    LPCONSTRUCT4CLAYER lpConstruct4CLayer = (LPCONSTRUCT4CLAYER)GetProcAddress(hModule, "Construct4CLayer");
    if (lpConstruct4CLayer == NULL)
    {
        CString strMessage(_T("Error: Getting ProcAddress Construct4CLayer."));
        AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
        m_pMainSheet->SetStatusText(strMessage);
        FreeLibrary(hModule);
        hModule = NULL;
        m_pMainSheet->SetStatusText(_T("Could not get proc address for Construct4CLayer"));
        return FALSE;
    }

    C4CLayer* pLayer = lpConstruct4CLayer(crstrTargetControl);
    if (pLayer != NULL)
    {
        if(ConnectToTarget(pLayer))
        {
            BYTE byCommand	= CMD_LOAD_PROJECT;
	        UINT uLen		= 0;
	        BYTE *pData 	= ALLOC(uLen);
	        BOOL bOK;
            
            if(m_bSupportProgressCallback)
            {
                pLayer->SetProgressCallBack(&ProgressCB);
            }

	        bOK = pLayer->Transfer(&byCommand, &pData, &uLen);

            if(m_bSupportProgressCallback)
            {
                pLayer->SetProgressCallBack(NULL);
            }

	        if (! bOK)
	        {
		        m_pMainSheet->SetStatusText(_T("Error: Upload Project. No answer."));
	        }
	        else if (bOK && byCommand != CMD_LOAD_PROJECT)
	        {
                CString strErrorMessage;
                short   sErrNum;
                if(pData)
                {
                    sErrNum = *(short*)pData;
                    strErrorMessage.Format(_T("Error: Upload Project. Error number 0x%X"), sErrNum);
                }
                else
                {
                    strErrorMessage = _T("Error: Upload Project");
                }
		        m_pMainSheet->SetStatusText(strErrorMessage);
                bOK = FALSE;
	        }
	        if (bOK && !StoreFile(crstrFilePath, pData, uLen))
	        {
		        m_pMainSheet->SetStatusText(_T("Error: Unable to store to file"));
                bOK = FALSE;
	        }
            DisconnectFromTarget(pLayer);
            bReturn = bOK;
        }
        delete pLayer;
    }

    FreeLibrary(hModule);
    hModule = NULL;
    return bReturn;
}



// create target control string
CString CPMPropPage::GetTargetControlString(const CString& crstrControl)
{
    HRESULT hr;
    CString strOPCInstName;
    CString strCommChannel;
    CString strConnectPar;
    CString strProtocol;
    CString strProtocolPar;
    CString strFirmwareVersion;

    hr = UTIL_ParseControlString(crstrControl,
                                    NULL, 
                                    NULL, 
                                    strOPCInstName.GetBuffer(UTIL_MAX_OPC_INST_NAME_LEN),
                                    strCommChannel.GetBuffer(UTIL_MAX_COMM_CHANNEL_LEN), 
                                    strConnectPar.GetBuffer(UTIL_MAX_CONNECT_PAR_LEN),
                                    strProtocol.GetBuffer(UTIL_MAX_PROTOCOL_LEN),
                                    strProtocolPar.GetBuffer(UTIL_MAX_PROTOCOL_PAR_LEN),
                                    strFirmwareVersion.GetBuffer(UTIL_MAX_FIRMWARE_VERSION_LEN));
    strOPCInstName.ReleaseBuffer();
    strCommChannel.ReleaseBuffer();
    strConnectPar.ReleaseBuffer();
    strProtocol.ReleaseBuffer();
    strProtocolPar.ReleaseBuffer();
    strFirmwareVersion.ReleaseBuffer();

    ASSERT(hr == S_OK);

    // construct target connect string
    CString strTargetControl;
    CString strProtocolTmp;
    CString strFirmwareVersionTmp;
    CString strConnectStringTmp;

    strProtocolTmp.Format("%s(%s)", strProtocol, strProtocolPar);

    if(!strFirmwareVersion.IsEmpty())
    {
        strFirmwareVersionTmp.Format("%s(%s)", UTIL_VERSION_STRING, strFirmwareVersion);
    }

    strConnectStringTmp.Format("%s(%s)", strCommChannel, strConnectPar);
    
    if(strFirmwareVersionTmp.IsEmpty())
    {
        strTargetControl.Format("%s,%s,%s", strOPCInstName, strConnectStringTmp, strProtocolTmp);
    }
    else
    {
        strTargetControl.Format("%s,%s,%s,%s", strOPCInstName, strConnectStringTmp, strProtocolTmp, strFirmwareVersionTmp);
    }
    return strTargetControl;
}




HMODULE CPMPropPage::LoadOnlCommLibraryForTarget(const CString& crstrTargetType)
{
    CString strTargetInstallPath;
    CString strLibPath;
    if(!GetTargetInstallPath(crstrTargetType, strTargetInstallPath))
    {
        m_pMainSheet->SetStatusText(_T("Could not get ATCMControl install path"));
        return NULL;
    }

    strLibPath.Format(_T("%s\\OnlComm.dll"), strTargetInstallPath);

    HMODULE hModule = LoadLibrary(strLibPath);

    if(hModule == NULL)
    {
        m_pMainSheet->SetStatusText(_T("Could not load online dll"));
    }

    if(GetProcAddress(hModule, "SupportProgressCallBack"))
    {
        m_bSupportProgressCallback = TRUE;
    }
    else
    {
        DWORD dwLastErr = GetLastError();
        m_bSupportProgressCallback = FALSE;
    }

    return hModule;
}


HMODULE CPMPropPage::LoadLocalOnlCommLibrary()
{
    CString strAppPath;
    CString strLibPath;

    GetModuleFileName(NULL, strAppPath.GetBuffer(_MAX_PATH), _MAX_PATH);
    strAppPath.ReleaseBuffer();

    int iFound = strAppPath.ReverseFind(_T('\\'));
    ASSERT(iFound >= 0);
    
    strLibPath.Format(_T("%s\\OnlComm.dll"), strAppPath.Left(iFound));
    HMODULE hModule = LoadLibrary(strLibPath);

    return hModule;
}

//------------------------------------------------------------------*
/**
 * CopyOPCServerConfig
 *
 *  copy opc server configuration files from project to opc config
 *  directory
 *
 * @param           pTInfo: target info object
 * @param           crstrProjectPath: full project path
 * @return          
 * @exception       -
 * @see             
*/
int CPMPropPage::CopyOPCServerConfig(TTargetInfo* pTInfo, const CString& crstrProjectPath)
{
    int iErr = ST_OK;
    CString strSourceDir;
    CString strDestDir;
    CString strInstallDir;

    size_t sizeInstDir = MAX_PATH;

    if (S_OK != UTIL_GetInstallPath(strInstallDir.GetBuffer(MAX_PATH), &sizeInstDir))
    {
//        if (!m_bSilent)
//        {
            CString strMessage;
            strMessage = _T("Error: Could not find install path");
            m_pMainSheet->SetStatusText(strMessage);
//        }
        return ST_ERROR;
    }
    strInstallDir.ReleaseBuffer();

    CString strConfigDir = strInstallDir + _T("Engineering\\bin\\") + pTInfo->strTargetType + _T("\\config\\");
    strDestDir = strConfigDir;

    if (!CreateDirectory(strDestDir, NULL))
    {
        DWORD dword = GetLastError();
        if (dword != ERROR_ALREADY_EXISTS)
        {
//            if (!m_bSilent)
//            {
                CString strMessage;
                strMessage = _T("Error: Could not create config directory");
                m_pMainSheet->SetStatusText(strMessage);
//            }
            return ST_ERROR;
        }
    }

    CString strRelPath = _T("_") + pTInfo->strTargetOPCInstName + _T("_");
    strDestDir += strRelPath;
    if (!CreateDirectory(strDestDir, NULL))
    {
        DWORD dword = GetLastError();
        if (dword != ERROR_ALREADY_EXISTS)
        {
//            if (!m_bSilent)
//            {
                CString strMessage;
                strMessage = _T("Error: Could not create target config directory");
                m_pMainSheet->SetStatusText(strMessage);
//            }
            return ST_ERROR;
        }
    }
    strDestDir += _T("\\");

    CString strOrigDir = crstrProjectPath +_T("\\gen\\") + pTInfo->strTargetName + _T("\\");
    CString strFindPattern = strOrigDir + _T("*.*");
    WIN32_FIND_DATA findFileData;
    HANDLE hfind;
    BOOL bFound = TRUE;

    hfind = FindFirstFile(strFindPattern, &findFileData);
    if (hfind == INVALID_HANDLE_VALUE)
    {
//        if (!m_bSilent)
//        {
            CString strMessage;
            strMessage = _T("Error: Invalid handle value");
            m_pMainSheet->SetStatusText(strMessage);
//        }
        return ST_ERROR;
    }
    
    while(bFound)
    {
        CString strOrigFileName = strOrigDir + findFileData.cFileName;
        CString strDestFileName = strDestDir + findFileData.cFileName;

        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            if (!CopyFile(strOrigFileName, strDestFileName, FALSE))
            {
//                if (!m_bSilent)
//                {
                    CString strMessage;
                    strMessage.Format(_T("Error: Copy file: %s -> %s"), strOrigFileName, strDestFileName);
                    m_pMainSheet->SetStatusText(strMessage);
//                }
                iErr = ST_ERROR;
            }   
        }
        bFound = FindNextFile(hfind, &findFileData);
    }

    FindClose(hfind);

    return ST_OK;
}


//------------------------------------------------------------------*
/**
 * PatchOPCServerRegistry
 *
 *   patches opc server registry
 *
 * @param           pTInfo: target info object
 * @param           crstrProjectPath: full project path
 * @return          successful? (error codes see defs.h)
 * @exception       -
 * @see             -
*/
int CPMPropPage::PatchOPCServerRegistry(TTargetInfo* pTInfo, const CString& crstrProjectPath)
{
    CString strMessage;
    CString strKey = _T("SOFTWARE\\Softing\\4CONTROLV2\\");

    CString strOPCServerExe = FindOPCServer(pTInfo);
    if (strOPCServerExe.IsEmpty())
    {
        strMessage.Format(_T("Error: Missing registry entry for OPC server exe for %s"), pTInfo->strTargetName);
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;    
    }

    strKey += strOPCServerExe + _T("\\SysVars\\") + pTInfo->strTargetOPCInstName;

    CString strVal = pTInfo->strControl;
    int ipos = strVal.Find(_T(','));
    if (ipos<0)
    {
        strMessage.Format(_T("Error: Invalid connect string for target %s"), pTInfo->strTargetName);
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }
    ipos = strVal.Find(_T(','), ipos+1);
    if (ipos<0)
    {
        strMessage.Format(_T("Error: Invalid connect string for target %s"), pTInfo->strTargetName);
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }
    strVal = strVal.Mid(ipos+1);

    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("CommParam"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: CommParam"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    FindProjectFile(crstrProjectPath, strVal);
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
        strMessage.Format(_T("Error: Could not write to registry: projectname"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }
       
    strVal = pTInfo->strTargetResource;
    strVal.MakeLower();
    
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("resourcename0"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: resourcename0"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    strKey += _T("\\softRT\\0\\DLoad");

    strVal = _T("0");
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("cleared"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: cleared"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
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
        strMessage.Format(_T("Error: Could not write to registry: date"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    strVal = GetDLGuid(pTInfo, crstrProjectPath);
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("dloadguid"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: dloadguid"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    strVal = GetLocalMachineName();
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("host"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: host"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    strVal = _T("");
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("prjguid"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: prjguid"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    strVal = GetCurrentUser();
    if (ERROR_SUCCESS != FC_RegSetStringVal(HKEY_LOCAL_MACHINE, 
                                           strKey, 
                                           _T("user"), 
                                           strVal))
    {
        strMessage.Format(_T("Error: Could not write to registry: user"));
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    return ST_OK;
}


// #245 14.09.05 SIS >>
// find opc server name from registry instead of file system
// naming convention changed -> previously "*SCS.exe" now "OnlServ4C*.exe"
//------------------------------------------------------------------*
/**
 * FindOPCServer
 *
 *  searches in HKEY_CLASSES_ROOT\CLSID\ for installed opc server
 *
 * @param           pTInfo: target info object
 * @return          name of server
 * @exception       -
 * @see             -
*/
CString CPMPropPage::FindOPCServer(TTargetInfo* pTInfo)
{
	CString strName;	// OPC server name to be returned
	CString strKey;		// registry key to opc server
	CString	strOPCServerExePathName;	// opc server exe path name

	strKey.Format(_T("CLSID\\{%s}\\LocalServer32"), (LPCTSTR)pTInfo->strCustomConnect);

    CString strMessage;
    if (ERROR_SUCCESS != FC_RegGetStringVal(HKEY_CLASSES_ROOT, 
                                           strKey,
                                           NULL,
                                           strOPCServerExePathName.GetBuffer(MAX_PATH),
										   MAX_PATH))
	{
        strMessage = _T("Error: Could not get opc server name from registry.");
        m_pMainSheet->SetStatusText(strMessage);
	}
	else
	{
		strOPCServerExePathName.ReleaseBuffer();

		GetLongPathName(strOPCServerExePathName, strOPCServerExePathName.GetBuffer(MAX_PATH), MAX_PATH);
		strOPCServerExePathName.ReleaseBuffer();

		int iFound = strOPCServerExePathName.ReverseFind(_T('\\'));
		strName = strOPCServerExePathName.Mid(iFound+1);

		int ipos = strName.ReverseFind(_T('.'));
		if (ipos>0)
		{
			strName = strName.Left(ipos);
		}
	}
    return strName;
}
// #245 14.09.05 SIS <<


BOOL CPMPropPage::GetTargetInstallPath(const CString& crstrTargetType, CString& rstrTargetInstDir)
{
    CString strInstallDir;
    CString strMessage;
    size_t sizeInstDir = MAX_PATH;

    if (S_OK != UTIL_GetInstallPath(strInstallDir.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        strMessage = _T("Error: Could not get install path");
        m_pMainSheet->SetStatusText(strMessage);
        return FALSE;
    }
    strInstallDir.ReleaseBuffer();
    rstrTargetInstDir = strInstallDir + _T("Engineering\\bin\\") + crstrTargetType;
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * GetDLGuid
 *
 *  uses downloader and IDLProperties interface to get download guid
 *
 * @param           pTInfo: target info object
 * @param           crstrProjectPath: full project path
 * @return          download guid
 * @exception       -
 * @see             -
*/
CString CPMPropPage::GetDLGuid(TTargetInfo* pTInfo, const CString& crstrProjectPath)
{
    CString strDLGuid;
    CString strMessage;

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
        strMessage = _T("Error: Could not create downloader");
        m_pMainSheet->SetStatusText(strMessage);
        return ST_ERROR;
    }

    CComBSTR sTargetName = pTInfo->strTargetName;
    CComBSTR sGenPath = (CString)(crstrProjectPath +_T("\\gen"));
    hr =  pDL->Init(sTargetName,sGenPath, TRUE);
    if (hr != S_OK)
    {
        strMessage = _T("Error: Init downloader failed");
        m_pMainSheet->SetStatusText(strMessage);
        pDL->Release();
        return ST_ERROR;
    }

    IDLProperties* pDLProperties = NULL;
    hr = pDL->QueryInterface(IID_IDLProperties, (LPVOID *)&pDLProperties);
    if (hr != S_OK)
    {
        strMessage = _T("Error: Could not get IDLProperties interface");
        m_pMainSheet->SetStatusText(strMessage);
        pDL->Release();
        return ST_ERROR;
    }

    pDL->Release();

    CComBSTR sVersion;
    CComBSTR sProjName;
    hr = pDLProperties->GetTargetDLVersion(&sVersion, &sProjName);
    if (hr != S_OK)
    {
        strMessage = _T("Error: Could not get target download version");
        m_pMainSheet->SetStatusText(strMessage);
        pDLProperties->Release();
        return ST_ERROR;
    }
    pDLProperties->Release();

    strDLGuid = sVersion;

    return strDLGuid;
}



//------------------------------------------------------------------*
/**
 * ExecuteCommand
 *
 * @param           crstrCmdLine: command line
 * @param           bVisible: window of started program visible?
 * @param           bWait: wait for termination of started program?
 * @param           bSilent: silent mode?
 * @return          return code of started program or ST_CREATE_PROC_FAILED
 * @exception       -
 * @see             -
*/
int CPMPropPage::ExecuteCommand(const CString& crstrCmdLine, BOOL bVisible /*=FALSE*/, BOOL bWait /*=TRUE*/, BOOL bSilent /*=FALSE*/)
{
    CWaitCursor tWaitCursor;
    CString strCmdLine(crstrCmdLine);
    int         iReturnCode = ST_OK;

    // now create command thread
    PROCESS_INFORMATION tProcInfo;
    STARTUPINFO         siStartInfo;
    BOOL                bResult;
    SECURITY_ATTRIBUTES saInherit = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = FALSE;
    saAttr.lpSecurityDescriptor = NULL;

    ::ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);

    siStartInfo.hStdInput = 0;
    siStartInfo.hStdOutput = 0;
    siStartInfo.hStdError = 0;
    siStartInfo.wShowWindow = bVisible ? SW_SHOW : SW_HIDE;


    siStartInfo.dwFlags = STARTF_USESHOWWINDOW;

    bResult = ::CreateProcess
                    (NULL,          // application name       
                    strCmdLine.GetBuffer(strCmdLine.GetLength() + 10),   // command line
                    NULL,           // process security attributes
                    NULL,           // primary thread security attributes
                    FALSE,          // handles are not inherited
                    0,              // creation flags
                    NULL,           // use parent's environment
                    NULL,           // use parent's current directory
                    &siStartInfo,   // STARTUPINFO pointer
                    &tProcInfo);    // receives PROCESS_INFORMATION

    // create process may have modified buffer:
    strCmdLine.ReleaseBuffer();

    if(!bResult)
    {
        DWORD   dwLastError = ::GetLastError();
        CString strMessage;
        strMessage.Format(IDS_COMMAND_FAILED, strCmdLine);
        if(!bSilent)
        {
            ::AfxMessageBox(strMessage, MB_ICONEXCLAMATION);
        }
        strMessage.Format(_T("Command could not be executed. Last error: 0x%08X"), dwLastError);
        m_pMainSheet->SetStatusText(strMessage);
        iReturnCode = ST_CREATE_PROC_FAILED;
    }
    else
    {
        DWORD   dwResult;
        DWORD   dwReturnCode;
        // this is an example of how to use an exit event
        // that may be set from a button
        //HANDLE  atHandles[2];
        //atHandles[0] = m_hExitEvent;
        //atHandles[1] = tProcInfo.hProcess;
        //
        //dwResult = ::WaitForMultipleObjects(2, atHandles, FALSE, 5000);
        //::ResetEvent(m_hExitEvent);
        //// thread is to be exited:
        //if(dwResult == WAIT_OBJECT_0)
        //{
        //    StopThread(tProcInfo.hProcess);
        //    return thread_break;
        //}
        //else
        //if timeout occurred: stop thread and exit
        //if(dwResult == WAIT_TIMEOUT)
        //{
        //}
        if(bWait)
        {
            dwResult = ::WaitForSingleObject(tProcInfo.hProcess, INFINITE);
            ::GetExitCodeProcess(tProcInfo.hProcess, &dwReturnCode);
            if(dwReturnCode != 0)
            {
                bResult = FALSE;
                iReturnCode = (int) dwReturnCode;
                if(iReturnCode != ST_OK)
                {
                    CString strMessage;
                    strMessage.Format(IDS_COMMAND_FAILED_RETCODE, iReturnCode);
                    if(!bSilent)
                    {
                        ::AfxMessageBox(strMessage, MB_ICONEXCLAMATION);
                    }
                    strMessage.Format(_T("Command failed. Tool returned 0x%X"), dwReturnCode);
                    m_pMainSheet->SetStatusText(strMessage);
                }
            }
        }
    }    
    return iReturnCode;
}

//------------------------------------------------------------------*
/**
 * GetCurrentUser
 *
 *  get environment variable USERNAME
 *
 * @param           -
 * @return          user name
 * @exception       -
 * @see             -
*/
CString CPMPropPage::GetCurrentUser()
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


CString CPMPropPage::GetLocalMachineName()
{
    CString strLocMachineName;
    if (S_OK != UTIL_GetLocalMachineName(strLocMachineName.GetBuffer(MAX_PATH), MAX_PATH))
    {
        CString strMessage;
        strMessage = _T("Error: Could not get local machine name");
        m_pMainSheet->SetStatusText(strMessage);
    }
    strLocMachineName.ReleaseBuffer();

    return strLocMachineName;
}

/* LoadFile
 * ----------------------------------------------------------------------------
 * if function succeeds memory is allocated
 * this memory must be freed by the caller
 *
 */
BOOL CPMPropPage::LoadFile(LPCTSTR szFile, BYTE **ppData, UINT *upLen)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	*ppData = ALLOC(dwSize);
	if(*ppData == NULL)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	if (! ReadFile(hFile, *ppData, dwSize, (DWORD *)upLen, NULL))
	{
        FREE(*ppData);
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	if (*upLen != dwSize)
	{
		FREE(*ppData);
		*upLen = 0;
		return FALSE;
	}

	return TRUE;
}

/* StoreFile
 * ----------------------------------------------------------------------------
 */
BOOL CPMPropPage::StoreFile(LPCTSTR szFile, BYTE *pData, UINT uLen)
{
	HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD dwWritten = 0;

	if (! WriteFile(hFile, pData, uLen, &dwWritten, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	return uLen == dwWritten;
}



BOOL CPMPropPage::OnKillActive()
{
    m_pMainSheet->SetStatusText(_T(""));    // clear status text

    return CPropertyPage::OnKillActive();
}


CString CPMPropPage::GetStandardProjectPath()
{
    CString strProjectPath;

    size_t sizeInstDir = MAX_PATH;
    UTIL_GetInstallPath(strProjectPath.GetBuffer(MAX_PATH), &sizeInstDir);
    strProjectPath.ReleaseBuffer();

    ASSERT(!strProjectPath.IsEmpty());
    strProjectPath += _T("Projects");
    return strProjectPath;
}


void CPMPropPage::OnCommSettingsChanged()
{
    if(GetSafeHwnd())
    {
        CWnd*   pWnd = GetDlgItem(IDC_CONNECT_INFO_EDIT);
        if(pWnd && pWnd->GetSafeHwnd())
        {
            pWnd->SetWindowText(m_pMainSheet->GetConnectInfo());
            pWnd->UpdateWindow();
        }
    }
}


void CPMPropPage::SetConnectStatus(BOOL bConnected)
{
    if(GetSafeHwnd())
    {
        CWnd*   pWnd = GetDlgItem(IDC_CONNECT_STATUS_EDIT);
        if(pWnd && pWnd->GetSafeHwnd())
        {
            CString strStatus(_T("not connected"));
            if(bConnected)
            {
                strStatus = _T("connected");
            }
            pWnd->SetWindowText(strStatus);
            pWnd->UpdateWindow();
        }
    }
}



/////////////////////////////////////////////////////////////////////////////
// CPMPropPage message handlers
