


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "RenameTask.h"
#include "utilif.h"
#include "resource.h"
//----  Static Initializations:   ----------------------------------*


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::CRenameTask
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
CRenameTask::CRenameTask(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler) : 
  CRenameObject(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo, pAddonHandler)
{
    m_sName = GetNameFromId();
    m_Resource = NULL;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::~CRenameTask
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CRenameTask::~CRenameTask()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::Execute
 * Description			
 * @return			successful? 
 * @exception			
 * @see			
*/
BOOL CRenameTask::Execute()
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
            strHelp1.Format(IDS_ERROR_INVALID,m_sNewName);
            ::AfxMessageBox(strHelp1 + strMessage, MB_OK|MB_ICONWARNING);
        }
    }
    else if(m_sName != m_sNewName)
    {
        BOOL bRes = FALSE;
        SetFileName();
        if(ParseResource())
        {
            bRes = !DoesTaskExist(m_sNewName);
            if(bRes)
            {
                RenameInText();
                WriteFile(m_ResourceText);
                InformProjectManager();//Delete the link
            }
        }
        return bRes;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameTask::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::DoesTaskExist
 * Description			
 * @param			CString strTaskName
 * @return			does task exist? 
 * @exception			
 * @see			
*/
BOOL CRenameTask::DoesTaskExist(CString strTaskName)
{
    task_configurations_citer iter;
    const task_configurations_type TaskConfigs = m_Resource->getTaskConfigurations();
    if(TaskConfigs.size() != 0)
    {
        for(iter = TaskConfigs.begin(); iter < TaskConfigs.end(); iter++)
        {
            CString  strTaskNameFromList = ((*iter)->getName()).c_str();
            if(strTaskName.CompareNoCase((LPCTSTR)strTaskNameFromList) == 0)
            {
                if(!m_bSilent)
                {
                    CString errorMsg;
                    errorMsg.Format(IDS_OBJECT_EXISTS);
                    ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                }
                return TRUE; //it does exists already
            }
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::ParseResource
 * Description			
 * @return			successful? 
 * @exception			
 * @see			
*/
BOOL CRenameTask::ParseResource()
{
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    buf);
    ASSERT(m_Resource == NULL);
    parse_string(buf, &m_Resource);
    m_ResourceText = buf.c_str();
    if(m_Resource)
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * Function name			CRenameTask::RenameInText
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CRenameTask::RenameInText()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
    task_configurations_citer iter;
    const task_configurations_type TaskConfigs = m_Resource->getTaskConfigurations();
    if(TaskConfigs.size() != 0)
    {
        for(iter = TaskConfigs.begin(); iter < TaskConfigs.end(); iter++)
        {
            CString  strTaskNameFromList = ((*iter)->getName()).c_str();
            if(!m_sName.CompareNoCase(strTaskNameFromList))
            {
                StringPosRange Range = (*iter)->getNameRange();
                m_ResourceText.Delete(Range.getFirst()-1, Range.getLength());
                m_ResourceText.Insert(Range.getFirst()-1, m_sNewName);
            }
        }
    }
    //m_ResourceText was ganged !!!
    string buf = (LPCTSTR)m_ResourceText;
    ASSERT(m_Resource == NULL);
    parse_string(buf, &m_Resource);

    program_configurations_citer iter1;
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    if(ProgInstConfigs.size() != 0)
    {
        for(iter1 = (ProgInstConfigs.end()-1); iter1 >= ProgInstConfigs.begin(); iter1--)
        {
            CString  strProgInstTaskNameFromList = ((*iter1)->getTaskName()).c_str();
            if(m_sName.CompareNoCase((LPCTSTR)strProgInstTaskNameFromList) == 0)
            {
                StringPosRange Range1 = (*iter1)->getTaskNameRange();
                m_ResourceText.Delete(Range1.getFirst()-1, Range1.getLength());
                m_ResourceText.Insert(Range1.getFirst()-1, m_sNewName);
            }
        }
    }
}

