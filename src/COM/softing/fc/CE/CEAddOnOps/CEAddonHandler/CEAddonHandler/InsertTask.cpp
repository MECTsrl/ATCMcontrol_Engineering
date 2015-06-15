


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <fstream>
#include "DllInterface.h"
#include "InsertTask.h"
#include "DialogTask.h"
#include "CEContainer.h"
#include "resource.h"
#include "Operation.h"
#include "SBLLexTree.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::CInsertTask
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
CInsertTask::CInsertTask(CPropertySheetBase *Dialog,
                                           BSTR sSourceFile,
                                           BSTR sId,
                                           ICEProjInfo *pICEProjectInfo,
                                           BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Resource = NULL;
    m_sName = _T("Task");
    m_bPosFirst = FALSE;
    m_bUseAttribute = FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::~CInsertTask
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CInsertTask::~CInsertTask()
{
    if (m_Resource)
    {
        delete m_Resource;
    }
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::Execute
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
BOOL CInsertTask::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    Init();
    SetFileName();
    if(!ParseResource())
    {
        return FALSE;
    }
    BOOL Res = !DoesTaskExist(m_sName);
    if(Res)
    {
        GenTask(m_TaskText);
        InsertTask();
        CString ResourceText = CString(m_ResourceText.c_str());
        WriteFile(ResourceText);
        InformProjectManager();
    }
    if (m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
    m_ResourceText.empty();
    m_aVarList.RemoveAll();
    return Res;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::ParseResource
 * Description			
 * @return			successful? 
 * @exception			
 * @see			
*/
BOOL CInsertTask::ParseResource()
{
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + CString(m_sFileName), 
                    m_ResourceText);
    ASSERT(m_Resource == NULL);
    ASSERT(m_ResourceText.length() != 0);
    parse_string(m_ResourceText, &m_Resource);
    ASSERT(m_Resource);
    if(!m_Resource)
    {
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::GenTask
 * Description			
 * @param			CString & StrOut
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::GenTask(CString & StrOut)
{
    CString StrHelp, Strco;
    CString StrAttr;
    CString Strlf = NEWLINE;
    CString StrIndent = SPACETAB;
    CString strComment;

    Strco.LoadString(IDS_KEY_COMMA);

    if(!m_sObjGUID.IsEmpty() || !m_sUsrText[0].IsEmpty() || !m_sUsrText[1].IsEmpty()
        || !m_sUsrText[2].IsEmpty() || !m_sAlias.IsEmpty() || !m_sUrl.IsEmpty())
    {
        strComment += Strlf;
        GetStdAttribString(strComment, 4);
        strComment.Delete((strComment.GetLength() -1), 1);
        strComment += StrIndent;
        strComment += StrIndent;
        strComment += StrIndent;
    }

    if (!m_bUseAttribute)
    {
        ASSERT(!m_sAutoStart.IsEmpty());

        if (m_sEvent.IsEmpty())
        {
            if (m_sIOSync.IsEmpty())
            {
                StrAttr.Format(IDS_KEY_TASK_ATTR1, m_sInterval, m_sPriority, m_sAutoStart);
            }
            else
            {
                StrAttr.Format(IDS_KEY_TASK_ATTR2, m_sIOSync);
            }
        }
        else
        {
            StrAttr.Format(IDS_KEY_TASK_ATTR3, m_sEvent, m_sPriority, m_sAutoStart);
        }

        if(!m_sWatchdog.IsEmpty())
        {
            StrAttr += _T(", WATCHDOG:=");
            StrAttr += m_sWatchdog;
        }

#ifdef DSC346_TODO_FOR_OSAI //EF: workaround for all non osai targets, todo for next osai delivery!
		// 13.12.06 SIS >>
		if(!m_sLoaded.IsEmpty())
		{
			StrAttr += _T(", LOADED:=");
			StrAttr += m_sLoaded;
		}
		// 13.12.06 SIS <<
#endif 
    }
    else
    {
        BOOL bFirst = TRUE;
        int num = m_arrAttributes.GetSize();
        for (int i=0; i<num; i++)
        {
            if (!bFirst)
            {
                StrAttr += _T(", ");
            }

            StrAttr += m_arrAttributes.GetAt(i);
            bFirst = FALSE;
        }
    }


    StrHelp.Format(IDS_KEY_TASK, m_sName, strComment, StrAttr);
    StrOut = Strlf;
    StrOut += StrIndent;
    StrOut += StrIndent;
    StrOut += StrHelp;
    //StrOut += Strlf;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::IsAutoStart
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CInsertTask::IsAutoStart()
{
	if(m_sAutoStart.IsEmpty() || !m_sAutoStart.CompareNoCase(_T("TRUE"))) 
    {
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::InsertTask
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::InsertTask()
{
    ASSERT(m_Resource);
    StringPos insertpos;
    if (m_bPosFirst)
    {
        insertpos = GetNextInsertPosition(m_ResourceText, m_Resource->getNextImpDirInsertPos());
    }
    else
    {
        insertpos = GetNextInsertPosition(m_ResourceText, m_Resource->getNextTaskConfInsertPos());
    }
    m_ResourceText.insert(insertpos, m_TaskText);
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::InformProjectManager
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}
//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::ReadFileContent
 * Description			
 * @param			const CString fileName
 * @param			string& buf
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::ReadFileContent(const CString fileName, string& buf)
{
    CStdioFile cFile(fileName, CFile::modeRead);
    int iLength = cFile.GetLength();
    char* pbuf;
    pbuf = new char[iLength+1];
    int iCount;
    iCount = cFile.Read(pbuf, iLength);
    pbuf[iCount]=0;
    ASSERT(iCount > 0);
    buf = pbuf;
    delete pbuf;
    cFile.Close();
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::SetFileName
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::SetFileName()
{
    m_sFileName = m_sSourceFile;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::GetDialog
 * Description			
 * @return			CDialogTask* 
 * @exception			
 * @see			
*/
CDialogTask* CInsertTask::GetDialog()
{
    return (CDialogTask*) m_pDialog;
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::Init
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::Init()
{
    if(!m_bAutomationMode)
    {
        m_sAlias          = GetDialog()->GetAlias();
        m_sName           = GetDialog()->GetName();
        m_sUsrText[0]     = GetDialog()->GetText1();
	    m_sUsrText[1]     = GetDialog()->GetText2();
	    m_sUsrText[2]     = GetDialog()->GetText3();
	    m_sHelpUrl        = GetDialog()->GetHelpUrl();
	    m_sUrl            = GetDialog()->GetUrl();
        m_sInterval       = GetDialog()->GetInterval();
        m_sPriority       = GetDialog()->GetPriority();
        m_sIOSync         = GetDialog()->GetIoSync();
        m_sEvent          = GetDialog()->GetEventString();
        m_sWatchdog       = GetDialog()->GetWatchdog();
        SetAutoStart(GetDialog()->GetAutoStart());
		SetLoaded(GetDialog()->GetLoaded());	// NFTASKHANDLING 19.05.05 SIS
    }

    COperation::Init();
    SetPouDT(TRUE); //TRUE for Insert (FALSE for Prop)
}

//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::SetAutoStart
 * Description			
 * @param			BOOL bAuto
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::SetAutoStart(BOOL bAuto)
{
	if(bAuto) 
    {
		m_sAutoStart = _T("TRUE");
	}
	else 
    {
		m_sAutoStart = _T("FALSE");
	}
}

// NFTASKHANDLING 19.05.05 SIS >>
//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::IsLoaded
 * Description			
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CInsertTask::IsLoaded()
{
	if(m_sLoaded.IsEmpty() || !m_sLoaded.CompareNoCase(_T("TRUE"))) 
    {
		return TRUE;
	}
	return FALSE;
}

void CInsertTask::SetLoaded(BOOL bLoaded)
{
	if(bLoaded) 
    {
		m_sLoaded = _T("TRUE");
	}
	else 
    {
		m_sLoaded = _T("FALSE");
	}
}
// NFTASKHANDLING 19.05.05 SIS <<
 
// SIS: event driven tasks >>
CString CInsertTask::GetEventString()
{
    return m_sEvent;
}

void CInsertTask::SetEventString(const CString& crstrEvent)
{
    m_sEvent = crstrEvent;
}
// SIS: event driven tasks <<


//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::FillDialog
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CInsertTask::FillDialog()
{
    SetFileName();
    if(!ParseResource())
    {
        return;
    }
    //Standart name
    CString sName = GetNameProposal(m_sName);
    
    GetDialog()->SetCreateUser(m_sCreateUser);
    GetDialog()->SetCreateDate(m_sCreateDate);
    GetDialog()->SetLastModUser(m_sLastModUser);
    GetDialog()->SetLastModDate(m_sLastModDate);
    
    GetDialog()->SetName(sName);

    if (m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::DoesTaskExist
 * Description			
 * @param			CString strTaskName
 * @return			does task exist?
 * @exception			
 * @see			
*/
BOOL CInsertTask::DoesTaskExist(CString strTaskName)
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
                    errorMsg.Format(IDS_TASK_EXISTS, strTaskName);
                    ::AfxMessageBox(errorMsg, MB_OK|MB_ICONWARNING);
                }
                return TRUE; //it exists already
            }
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * Function name			CInsertTask::GetNameProposal
 * Description			
 * @param			CString strBasis
 * @return			CString 
 * @exception			
 * @see			
*/
CString CInsertTask::GetNameProposal(CString strBasis)
{
    CSBLLexTree<bool>   tAssoc;             // association memory for names
    bool*               pbTmp = (bool*) 1;  // dummy element for insertion

    int         iIndex = 1;
    CString     strTaskName;
    strTaskName.Format("%s%d", strBasis, iIndex);
        
    task_configurations_citer iter;
    if(!m_Resource)
    {
        return strTaskName;
    }

    const task_configurations_type TaskConfigs = m_Resource->getTaskConfigurations();
    if(TaskConfigs.size() != 0)
    {
        for(iter = TaskConfigs.begin(); iter < TaskConfigs.end(); iter++)
        {
            strTaskName = ((*iter)->getName()).c_str();
            tAssoc.insertData(strTaskName, pbTmp);
        }
    }

    for(iIndex = 1; iIndex < MAX_PROPOSAL_COUNT; ++iIndex)
    {    
        strTaskName.Format("%s%d", strBasis, iIndex);
        if(!tAssoc.getData(strTaskName))
        {
            break;
        }
    }

    return strTaskName;
}



void CInsertTask::SetTaskName(const CString& crstrTaskName)
{
    m_sName = crstrTaskName;
}


void CInsertTask::SetInterval(const CString& crstrInterval)
{
    m_sInterval = crstrInterval;
}


void CInsertTask::SetPriority(const CString& crstrInterval)
{
    m_sPriority = crstrInterval;
}


void CInsertTask::SetIOSync(const CString& crstrIOSync)
{
    m_sIOSync = crstrIOSync;
}


void CInsertTask::SetGenPos(BOOL bPosFirst)
{
    m_bPosFirst = bPosFirst;
}

void CInsertTask::SetUseAttributes()
{
    m_bUseAttribute = TRUE;
}


void CInsertTask::SetAttribute(const CString& crstrAttribute)
{
    m_arrAttributes.Add(crstrAttribute);
}



