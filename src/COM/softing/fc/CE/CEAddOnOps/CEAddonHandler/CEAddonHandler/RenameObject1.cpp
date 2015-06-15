


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "RenameObject1.h"
#include "utilif.h"
#include "CEAddonActionComObj.h"
#include "resource.h"

//----  Static Initializations:   ----------------------------------*

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::CRenameObject
 * Description			
 * @param			CPropertySheetBase *Dialog
 * @param			BSTR sSourceFile
 * @param			BSTR sId
 * @param			ICEProjInfo *pICEProjectInfo
 * @param			BSTR sAdditionalInfo
 * @return			
 * @exception			
 * @see			
*/
CRenameObject::CRenameObject(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler) :
  COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo),
  m_pAddonHandler(pAddonHandler)
{
    m_sNewName = m_sAdditionalInfo;
    m_pPouObject = NULL;
    m_sName = GetNameFromId();
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::~CRenameObject
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CRenameObject::~CRenameObject()
{
    if(m_pPouObject)
    {
        delete m_pPouObject;
        m_pPouObject = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::Execute
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CRenameObject::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    CString strHelp = _T("");
    strHelp.LoadString(IDS_SOFTING_ADDON1);
    LPCTSTR g_MessageBoxCaption = (LPCTSTR)strHelp;

    // Check, if identifier is IEC compatible
    HRESULT hr = UTIL_CheckIdentifier(m_sNewName);
    if(hr != S_OK)
    {
        CString strMessage;
        switch(hr)
        {
        case E_UTIL_ID_SYNTAX:
            strMessage.LoadString(IDS_ERROR_IECSYNTAX);
            break;
        case E_UTIL_ID_IS_KEYWORD:
            strMessage.LoadString(IDS_ERROR_KEYWORD);
            break;
        case E_UTIL_ID_TOO_LONG:
            strMessage.LoadString(IDS_ERROR_TOOLONG);
            break;
        }
        if(!m_bSilent)
        {
            CString strHelp1 = _T("");
            strHelp1.Format(IDS_ERROR_INVALID, m_sNewName);
            ::AfxMessageBox(strHelp1 + strMessage, MB_OK|MB_ICONWARNING);
        }
    }
    else if(m_sName != m_sNewName)
    {
        SetFileName();
        CString strNewFileName = m_sNewName + GetExtentionFromFileName(m_sFileName);
        BOOL bRes = !DoesFileExist(m_sNewName, FALSE);
        if(bRes)
        {
            RenameInText();
            WriteHeader(m_sObjectText);
            RenameFile(m_sFileName, strNewFileName);
            InformProjectManager();//Rename the link
        }
        return bRes;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameObject::InformProjectManager()
{
    HRESULT hRes = 0;
    ICEContainer *pICEContainer = NULL;
    BSTR sSF;
    hRes = m_pICEProjectInfo->getProjectName(&sSF);
    ASSERT(hRes==S_OK);
    CString SourceFile(sSF); 
    SysFreeString(sSF);

    CString StrId = SourceFile;
    SourceFile += _T(".4cp");
    sSF = SourceFile.AllocSysString();
    hRes = m_pICEProjectInfo->getContainerIFforFile(sSF, &pICEContainer);
   	ASSERT(hRes==S_OK);

    BSTR strFileName = m_sFileName.AllocSysString();
    CString strNewFileName = m_sNewName + GetExtentionFromFileName(m_sFileName);
    BSTR strNewName = strNewFileName.AllocSysString();
    hRes = pICEContainer->renameLink(sSF,     
                                     m_sId, 
                                     strFileName,
                                     strNewName);
    ASSERT(hRes==S_OK);

    pICEContainer->Release();

    SysFreeString(strFileName);
    SysFreeString(strNewName);
    SysFreeString(sSF);
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::RenameFile
 * Description			
 * @param			CString strFileName
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameObject::RenameFile(CString strFileName, CString strNewName)
{
    try
    {
        CString strProjectPath(m_ProjectPath);
        CString strOldFilePath(strProjectPath + _T("\\") + strFileName);
        CString strNewFilePath(strProjectPath + _T("\\") + strNewName);

        CFileStatus fileStatus;
        if (CFile::GetStatus(strNewFilePath, fileStatus))
        {
            // file already exists, delete it
            CFile::Remove(strNewFilePath);   
        }
        CFile::Rename(strOldFilePath,strNewFilePath);
        // trigger file close and reopen
        ASSERT(m_pAddonHandler);
        if(m_pAddonHandler)
        {
            CComBSTR    sOldFilePath(strFileName);
            CComBSTR    sNewFilePath(strNewName);
            m_pAddonHandler->Fire_OnFileRenamed(sOldFilePath, sNewFilePath);
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->ReportError();
        pFileException->Delete();
    }   
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameObject::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * Function name			GetExtentionFromFileName
 * Description			
 * @param			CString strFileName
 * @return			CString 
 * @exception			
 * @see			
*
CString CRenameObject::GetExtentionFromFileName(CString strFileName)
{
    CString strFileExtention;
    int iPos = strFileName.ReverseFind('.');
    strFileExtention = strFileName.Right(strFileName.GetLength() - iPos);
    return strFileExtention;
}*/


//------------------------------------------------------------------*
/**
 * Function name			CRenameObject::RenameInText
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameObject::RenameInText()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 

    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_sObjectText = buf.c_str();
    m_pPouObject = ParsePou(m_sObjectText, m_pPouObject);
    const string Name = m_pPouObject->getName();
    StringPosRange NameRange = m_pPouObject->getNameRange();
    m_sName = Name.c_str();
    int iPos = NameRange.getFirst() - 1;
    int iLength = NameRange.getLength();
    m_sObjectText.Delete(iPos, iLength);
    m_sObjectText.Insert(iPos, m_sNewName);
}




