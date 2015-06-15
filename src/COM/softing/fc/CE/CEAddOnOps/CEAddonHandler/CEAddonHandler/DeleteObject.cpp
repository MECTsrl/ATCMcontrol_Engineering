


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DeleteObject.h"
#include "resource.h"
#include "CEAddonActionComObj.h"

//----  Static Initializations:   ----------------------------------*

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::CDeleteObject
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
CDeleteObject::CDeleteObject(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler) : 
  COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo),
  m_pAddonHandler(pAddonHandler)
{

}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::~CDeleteObject
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CDeleteObject::~CDeleteObject()
{

}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::Execute
 * Description		called when the user clicks on the OK button
 * @return			BOOL: TRUE if succesful else FALSE 
 * @exception			
 * @see			    DeleteFile, InformProjectManager, SetFileName
*/
BOOL CDeleteObject::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    SetFileName();
    BOOL Res = DeleteFile(m_sFileName);
    if(Res)
    {
        InformProjectManager();//Delete the link
    }
    return Res;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteObject::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::InformProjectManager
 * Description		invoke the project manager to delete the link associated
                    to this object from the 4cp file
 * @return			void 
 * @exception			
 * @see			    ICEProjInfo
*/
void CDeleteObject::InformProjectManager()
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
    hRes = pICEContainer->deleteLink(sSF,     
                                     m_sId, 
                                     strFileName);
    SysFreeString(strFileName);
    SysFreeString(sSF);
    ASSERT(hRes==S_OK);

    pICEContainer->Release();
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::DeleteFile
 * Description		Asks the user if he really wants to delete the file. if so
	                the file is deleted
 * @param			CString strFileName
 * @return			file deleted? 
 * @exception		file exeption catchen internally	
 * @see			    GetNameFromFileName, 
*/
BOOL CDeleteObject::DeleteFile(CString strFileName)
{
    CString strAddInfo = m_sAdditionalInfo;

    CString strFilePathName;
    strFilePathName = (CString)m_ProjectPath + _T("\\") + strFileName;
    CFileStatus tFileStatus;
    if(!CFile::GetStatus(strFilePathName, tFileStatus))
    {
        if(!m_bSilent)
        {
            CString strMessage;
            strMessage.Format(IDS_FILE_ALREADY_REMOVED, strFilePathName);
            ::AfxMessageBox(strMessage, MB_OK | MB_ICONWARNING);
        }
        return TRUE;
    }
    CString errorMsg;
    CString errorTitle;

    m_sName = GetNameFromFileName(strFileName);
    if(!m_bSilent)
    {
        errorMsg.Format(IDS_REALLY_DELETE_FILE1, m_sName, strFilePathName);
        int Res = ::AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING);
        if(Res != IDYES)
        {
            return FALSE;
        }
    }
    if(!::DeleteFile(strFilePathName))
    {
        if(!m_bSilent)
        {
            errorMsg.Format(IDS_ERROR_DELETE_FILE, strFilePathName);
            ::AfxMessageBox(errorMsg, MB_OK);
        }
        return FALSE;
    }
    else
    {
        // file has been removed, now trigger close editor, if open
        ASSERT(m_pAddonHandler);
        if(m_pAddonHandler)
        {
            CComBSTR    sFilePathName(strFilePathName);
            m_pAddonHandler->Fire_OnFileRemoved(sFilePathName);
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteObject::GetNameFromFileName
 * Description		Parses the file name to get the object name from it	
 * @param			CString strFileName: file name 
 * @return			CString : object name
 * @exception			
 * @see			
*/
CString CDeleteObject::GetNameFromFileName(CString strFileName)
{
    CString strName;
    int iPos = strFileName.ReverseFind('.');
    strName = strFileName.Left(iPos);
    return strName;
}


