


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DeleteTask.h"
#include "resource.h"
//----  Static Initializations:   ----------------------------------*


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::CDeleteTask
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
CDeleteTask::CDeleteTask(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo) : 
COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Resource = NULL;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::~CDeleteTask
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CDeleteTask::~CDeleteTask()
{
    delete m_Resource;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::Execute
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CDeleteTask::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    BOOL bRes = FALSE;

    SetFileName();
    if(ParseResource())
    {
        bRes = DeleteTaskConfig();
        if(bRes)
        {
            WriteFile(m_ResourceText);
            InformProjectManager();     //Parse resource file
        }
    }
    return bRes;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteTask::SetFileName()
{
    m_sFileName = m_sSourceFile;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteTask::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::ParseResource
 * Description			
 * @return			successful? 
 * @exception			
 * @see			
*/
BOOL CDeleteTask::ParseResource()
{
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    buf);
    ASSERT(m_Resource == NULL);
    parse_string(buf, &m_Resource);
    m_ResourceText = buf.c_str();
    if(!m_Resource)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::GetTaskRange
 * Description			
 * @param			CString strTaskName
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteTask::GetTaskRange(CString strTaskName)
{
    const task_configurations_type TaskConfigs = m_Resource->getTaskConfigurations();
    if(TaskConfigs.size()!= 0)
    {
        task_configurations_citer iter;
        for (iter = TaskConfigs.begin(); iter < TaskConfigs.end(); iter++)
        {
            string Name = (*iter)->getName();
            CString sName = Name.c_str();
            if(!sName.CompareNoCase(strTaskName))
            {
                StringPosRange Range = (*iter)->getRange();
                m_iPos = Range.getFirst() - 1;
                m_iLength = Range.getLength() + 1;
            }
        }
    }        
}


//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::DeleteTaskConfig
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CDeleteTask::DeleteTaskConfig()
{
    BOOL    bDelete = TRUE;
    m_sName = GetNameFromId();

    if(!m_bSilent)
    {
        CString errorMsg;
        errorMsg.Format(IDS_REALLY_DELETE_FILE3, m_sName);
        int iRes = ::AfxMessageBox(errorMsg, MB_YESNO|MB_ICONWARNING);
        if(iRes != IDYES)
        {
            bDelete = FALSE;
        }
    }
    if(bDelete)
    {
        GetTaskRange(m_sName);
        m_ResourceText.Delete(m_iPos, m_iLength);
        //m_ResourceText was ganged !!!
        string buf = (LPCTSTR)m_ResourceText;
        
        // this is only a hack
        if (m_Resource)
        {
            delete m_Resource;
            m_Resource = NULL;
        }

        parse_string(buf, &m_Resource);
        DeleteProgramInstances(m_sName);
        return TRUE;
    }
    return FALSE;
}



//------------------------------------------------------------------*
/**
 * Function name			CDeleteTask::DeleteProgramInstances
 * Description			
 * @param			CString strTaskName
 * @return			void 
 * @exception			
 * @see			
*/
void CDeleteTask::DeleteProgramInstances(CString strTaskName)
{
    program_configurations_citer iter1;
    const program_configurations_type ProgInstConfigs = m_Resource->getProgramConfigurations();
    if(ProgInstConfigs.size() != 0)
    {
        for(iter1 = (ProgInstConfigs.end()-1); iter1 >= ProgInstConfigs.begin(); iter1--)
        {
            CString  strTaskNameFromList = ((*iter1)->getTaskName()).c_str();
            if(strTaskName.CompareNoCase((LPCTSTR)strTaskNameFromList) == 0)
            {
                StringPosRange Range = (*iter1)->getRange();
                int iPos = Range.getFirst() - 1;
                int iLength = Range.getLength() + 1;
                m_ResourceText.Delete(iPos, iLength);
            }
        }
    }
}

