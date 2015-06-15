


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DllInterface.h"
#include "PropTask.h"
#include "DialogTask.h"
#include "CEContainer.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPropTask::CPropTask(CPropertySheetBase *Dialog,
                                       BSTR sSourceFile,
                                       BSTR sId,
                                       ICEProjInfo *pICEProjectInfo,
                                       BSTR sAdditionalInfo) :
    COperation(Dialog, sSourceFile, sId, pICEProjectInfo, sAdditionalInfo)
{
    m_Resource = NULL;
    m_iPos = 0;
    m_iLength = 0;
    m_bUseAttribute = FALSE;
}

CPropTask::~CPropTask()
{
    if(m_Resource)
    {
        delete m_Resource;
        m_Resource = NULL;
    }
}

BOOL CPropTask::Execute()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    Init();
    GenTask(m_TaskText);
    InsertTask();
    WriteFile(m_ResourceText);
    InformProjectManager();

    return TRUE;
}

void CPropTask::SetFileName()
{
    //File allready exists
    m_sFileName = m_sSourceFile;
}

void CPropTask::Init()
{
    if(!m_bAutomationMode)
    {
        m_sAlias          = GetDialog()->GetAlias();
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

}


void CPropTask::InformProjectManager()
{
    HRESULT hRes = 0;
    BSTR strFileName = m_sFileName.AllocSysString();
    hRes = m_pICEProjectInfo->fileChangedHint(strFileName);
    SysFreeString(strFileName);
   	ASSERT(hRes==S_OK);
}


CDialogTask* CPropTask::GetDialog()
{
    return (CDialogTask*) m_pDialog;
}

void CPropTask::FillDialog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    SetFileName();
    string buf;
    ReadFileContent(CString(m_ProjectPath) + _T("\\") + m_sFileName, buf);
    m_ResourceText = buf.c_str();
    m_sName = GetNameFromId();
    ASSERT(!m_ResourceText.IsEmpty());
    m_Resource = (Resource *)ParsePou(m_ResourceText, m_Resource);
    InitFromFile(m_Resource);
    if(!m_bAutomationMode)
    {
        COperation::FillDialog();
        GetDialog()->SetCreateUser(m_sCreateUser);
        GetDialog()->SetCreateDate(m_sCreateDate);
        GetDialog()->SetLastModUser(m_sLastModUser);
        GetDialog()->SetLastModDate(m_sLastModDate);
        GetDialog()->SetAlias(m_sAlias);
        GetDialog()->SetName(m_sName);
        GetDialog()->SetText1(m_sUsrText[0]);
	    GetDialog()->SetText2(m_sUsrText[1]);
	    GetDialog()->SetText3(m_sUsrText[2]);
	    GetDialog()->SetHelpUrl(m_sHelpUrl);
	    GetDialog()->SetUrl(m_sUrl);
        BOOL bAutoStart = IsAutoStart();
        GetDialog()->SetAutoStart(bAutoStart);
		GetDialog()->SetLoaded(IsLoaded());		// NFTASKHANDLING 19.05.05 SIS
        GetDialog()->SetInterval(m_sInterval);
        GetDialog()->SetIoSync(m_sIOSync);
        GetDialog()->SetPriority(m_sPriority);
        GetDialog()->SetEventString(m_sEvent);
        GetDialog()->SetWatchdog(m_sWatchdog);
    }
}

BOOL CPropTask::IsAutoStart()
{
	if(m_sAutoStart.IsEmpty() || !m_sAutoStart.CompareNoCase(_T("TRUE"))) 
    {
		return TRUE;
	}
	return FALSE;
}

void CPropTask::SetAutoStart(BOOL bAuto)
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

BOOL CPropTask::IsLoaded()
{
	if(m_sLoaded.IsEmpty() || !m_sLoaded.CompareNoCase(_T("TRUE"))) 
    {
		return TRUE;
	}
	return FALSE;
}

void CPropTask::SetLoaded(BOOL bLoaded)
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


// SIS: event driven tasks >>
CString CPropTask::GetEventString()
{
    return m_sEvent;
}

void CPropTask::SetEventString(const CString& crstrEvent)
{
    m_sEvent = crstrEvent;
}
// SIS: event driven tasks <<


void CPropTask::InitFromFile(Resource* Pou)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    ASSERT(Pou);
    COperation::Init();
    SetPouDT(FALSE); //TRUE for Insert (FALSE for Prop)

    const task_configurations_type TaskConfigs = Pou->getTaskConfigurations();
    if(TaskConfigs.size()!= 0)
    {
        task_configurations_citer iter;
        for (iter = TaskConfigs.begin(); iter < TaskConfigs.end(); iter++)
        {
            ASSERT(iter);
            string Name = (*iter)->getName();
            CString sName = Name.c_str();
            if(!sName.CompareNoCase(m_sName))
            {
                const SysComment* pSysCmt = (*iter)->GetFileSystemComment();
                if(pSysCmt)
                {
                    CString Dummy;
                    string dummy;
                    Dummy.LoadString(IDS_KEY_ALIAS1);
                    dummy = Dummy;
                    m_sAlias          = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_USERTEXT1);
                    dummy = Dummy;
                    m_sUsrText[0]     = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_USERTEXT2);
                    dummy = Dummy;
                    m_sUsrText[1]     = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_USERTEXT3);
                    dummy = Dummy;
                    m_sUsrText[2]     = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_HELPURL);
                    dummy = Dummy;
                    m_sHelpUrl        = (pSysCmt->avm_get(dummy)).c_str();
                    Dummy.LoadString(IDS_KEY_URL);
                    dummy = Dummy;
                    m_sUrl            = (pSysCmt->avm_get(dummy)).c_str();
                }
                StringPosRange Range = (*iter)->getRange();
                m_iPos = Range.getFirst() - 1;
                m_iLength = Range.getLength() + 1;

                m_sInterval = ((*iter)->getInterval()).c_str();
                m_sPriority = ((*iter)->getPriority()).c_str();
                m_sIOSync   = ((*iter)->getIosync()).c_str();
                m_sAutoStart = (*iter)->getAutostart().c_str();
				m_sLoaded = (*iter)->getLoaded().c_str();	// NFTASKHANDLING 19.05.05 SIS

                m_sEvent = (*iter)->getEvent().c_str(); // SIS: event driven tasks
                m_sEvent.Replace(_T("'"), _T(""));      // strip quotation marks
                m_sEvent.Replace(_T("\""), _T(""));     // strip quotation marks

                m_sWatchdog = (*iter)->getWatchdog().c_str();
            }
        }
    }
}

void CPropTask::GenTask(CString & StrOut)
{
    CString StrHelp, Strco;
    CString StrAttr = _T("");
    CString Strlf = NEWLINE;
    CString StrIndent = SPACETAB;

    Strco.LoadString(IDS_KEY_COMMA);
    StrOut = _T("");

    if(!m_sObjGUID.IsEmpty() || !m_sUsrText[0].IsEmpty() || !m_sUsrText[1].IsEmpty()
        || !m_sUsrText[2].IsEmpty() || !m_sAlias.IsEmpty() || !m_sUrl.IsEmpty())
    {
        StrOut += Strlf;
        GetStdAttribString(StrOut, 4);
        StrOut.Delete((StrOut.GetLength() -1), 1);
        StrOut += StrIndent;
        StrOut += StrIndent;
        StrOut += StrIndent;
    }

    if (!m_bUseAttribute)
    {
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
		if(!m_sLoaded.IsEmpty())	// #301 06.02 06 SIS: removed if clause that prevented showing default value "TRUE"
		{
			StrAttr += _T(", LOADED:=");
			StrAttr += m_sLoaded;
		}
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

    if (m_strNewTaskName.IsEmpty())
    {
        StrHelp.Format(IDS_KEY_TASK, m_sName, StrOut, StrAttr);
    }
    else
    {
        StrHelp.Format(IDS_KEY_TASK, m_strNewTaskName, StrOut, StrAttr);
    }
    StrOut = StrHelp;
}

void CPropTask::InsertTask()
{
    ASSERT(!m_ResourceText.IsEmpty());
    m_ResourceText.Delete(m_iPos, m_iLength);
    m_ResourceText.Insert(m_iPos, m_TaskText);
}

void CPropTask::SetTaskName(const CString& crstrTaskName)
{
    m_strNewTaskName = crstrTaskName;
}

void CPropTask::SetUseAttributes()
{
    m_bUseAttribute = TRUE;
}


void CPropTask::SetAttribute(const CString& crstrAttribute)
{
    m_arrAttributes.Add(crstrAttribute);
}
