
#include "stdafx.h"
#include "Dialogs.h"
#include "ObjPropGenTask.h"
#include "SyntaxCheck.h"
#include "EventSelection.h"     // SIS: event driven tasks

// NFTASKHANDLING 30.05.05 SIS >>
#include "extern.h"
#include "operation.h"
// NFTASKHANDLING 30.05.05 SIS <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenTask::CObjPropGenTask
 * Description      constructor for class CObjPropGenTask
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenTask::CObjPropGenTask(DIALOGTYPE DialogType, 
                                 CPropertySheetBase *PropertySheet) : 
    CObjPropGenBase(DialogType,
                    IDD_OBJPROP_GEN_TASK,
                    PropertySheet)
{
	m_AutoStart = TRUE;
	m_bLoaded = TRUE;	// NFTASKHANDLING 19.05.05 SIS
	m_Interval = _T("");
	m_IoSync = _T("");
	m_Priority = _T("");
    //m_EventString.Empty();    // do not need this
    m_eType = eTaskTypeCyclic;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenTask::~CObjPropGenTask
 * Description      destructor for class CObjPropGenTask
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropGenTask::~CObjPropGenTask()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenTask::DoDataExchange
 * Description      exchanges and validates dialog data
 * @param           CDataExchange* pDX
 * @return          -
 * @exception       -
 * @see             -
*/
void CObjPropGenTask::DoDataExchange(CDataExchange* pDX)
{
	CObjPropGenBase::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_AUTOSTART, m_AutoStart);
	DDX_Check(pDX, IDC_LOADED, m_bLoaded);
	DDX_Text(pDX, IDC_INTERVAL, m_Interval);
	DDX_Text(pDX, IDC_IOSYNC, m_IoSync);
	DDX_Text(pDX, IDC_PRIORITY, m_Priority);
    DDX_Text(pDX, IDC_EVENTSTRING, m_EventString);
	DDX_Radio(pDX, IDC_TASKTYPE_CYCLIC, (int&)m_eType);
	DDX_Text(pDX, IDC_WATCHDOG, m_Watchdog);
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenTask::OnInitDialog
 * Description      message handler for the WM_INITDIALOG message
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenTask::OnInitDialog() 
{
	CObjPropGenBase::OnInitDialog();

    InitializeMissingData();

    // if io-sync is not empty -> IO-Sync type
    if(!m_IoSync.IsEmpty())
    {
        m_eType = eTaskTypeIoSync;
    }

    // if event string is not empty -> event driven type
    if(!m_EventString.IsEmpty())
    {
        m_eType = eTaskTypeEventDriven;
    }
  
    UpdateData(FALSE);

    EnableControls();
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CObjPropGenTask::InitializeMissingData()
{
    switch(m_eType)
    {
    case eTaskTypeCyclic:
        if (m_Interval == _T(""))
        {
            m_Interval = _T("T#100ms");
        }                               // fall through
    case eTaskTypeEventDriven:
        if (m_Priority == _T(""))
        {
            m_Priority = _T("5");
        }
        break;
    case eTaskTypeIoSync:
        if (m_IoSync == _T(""))
        {
            m_IoSync = _T("Channel Number");
        }
        break;
    }
}


//------------------------------------------------------------------*
/**
 * Function name    CObjPropGenTask::OnKillActive
 * Description      is called when this property page is no longer the active page
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenTask::OnKillActive()
{
    UpdateData(TRUE);
   	Trim(m_Interval);
   	Trim(m_Priority);
   	Trim(m_IoSync);
   	Trim(m_Watchdog);
    UpdateData(FALSE);

    CString ErrorMessage;
    int     iPriority;
	CWnd*   pWnd = NULL;

    switch(m_eType)
    {
    case eTaskTypeCyclic:
        // 
        if (CheckTypeOfLiteral(_T("TIME"), m_Interval, ErrorMessage) == FALSE)
        {
            pWnd = GetDlgItem(IDC_INTERVAL);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_TASK_TIME_INVALID, MB_ICONWARNING);
            return FALSE;
        }
        if(m_Priority.IsEmpty())
        {
            pWnd = GetDlgItem(IDC_PRIORITY);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_TASK_PRIORITY_EMPTY, MB_ICONWARNING);
            return FALSE;
        }
        iPriority = atoi(m_Priority);
        if(m_Priority.IsEmpty() || iPriority < 0 || iPriority > 9)
        {
            pWnd = GetDlgItem(IDC_PRIORITY);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_INVALID_TASKPRIORITY, MB_ICONWARNING);
            return FALSE;
        }
		pWnd = GetDlgItem(IDC_IOSYNC);
		if(pWnd)
		{
			pWnd->SetWindowText(_T(""));
		}
		pWnd = GetDlgItem(IDC_EVENTSTRING);
		if(pWnd)
		{
			pWnd->SetWindowText(_T(""));
		}
        
/* EF 11.05.06: do not check datatype anymore, if version >= 2.1.5 is now ANY_INT older version is TIME
   since there is no target version at this time we can't ckeck any thing...
        if(!m_Watchdog.IsEmpty() && CheckTypeOfLiteral(_T("TIME"), m_Watchdog, ErrorMessage) == FALSE)
        {
            pWnd = GetDlgItem(IDC_WATCHDOG);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_WATCHDOG_INVALID, MB_ICONWARNING);
            return FALSE;
        }
*/
        break;
    case eTaskTypeEventDriven:
        if(m_EventString.IsEmpty())
        {
            pWnd = GetDlgItem(IDC_EVENTSTRING);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_EMPTY_EVENTSTRING, MB_ICONWARNING);
            return FALSE;
        }
        if(!CheckEventString())
        {
            pWnd = GetDlgItem(IDC_EVENTSTRING);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_INVALID_EVENTSTRING, MB_ICONWARNING);
            return FALSE;
        }
/* EF 11.05.06: do not check datatype anymore, if version >= 2.1.5 is now ANY_INT older version is TIME
   since there is no target version at this time we can't ckeck any thing...
        if(!m_Watchdog.IsEmpty() && CheckTypeOfLiteral(_T("TIME"), m_Watchdog, ErrorMessage) == FALSE)
        {
            pWnd = GetDlgItem(IDC_WATCHDOG);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_WATCHDOG_INVALID, MB_ICONWARNING);
            return FALSE;
        }
*/
        break;
    case eTaskTypeIoSync:
        break;
        if(m_IoSync.IsEmpty())
        {
            pWnd = GetDlgItem(IDC_IOSYNC);
            if(pWnd)
            {
                GotoDlgCtrl(pWnd);
            }
            ::AfxMessageBox(IDS_EMPTY_IOSYNC, MB_ICONWARNING);
            return FALSE;
        }
    }

    return CObjPropGenBase::OnKillActive();
}



//------------------------------------------------------------------*
/**
 * CheckEventString.
 *
 *  remove invalid characters and check event number range
 *
 * @param           -
 * @return          event string valid?
 * @exception       -
 * @see             -
*/
BOOL CObjPropGenTask::CheckEventString()
{
    if(m_EventString.IsEmpty())
    {
        return TRUE;
    }
    CString strEventNew;
    CString strTmp;
    int     iStrLen = m_EventString.GetLength();
    int     iEventNum;
    TCHAR   cChar;
    TCHAR   cLastChar(0);   // last character except for '," or space
    
    for(int iChar = 0; iChar < iStrLen; ++iChar)
    {
        cChar = m_EventString[iChar];
        switch(cChar)
        {
        case _T('\''):      // skip single quotation mark
        case _T('"'):       // skip double quotation mark
            break;
        case _T(' '):
            if(cLastChar == _T(','))
            {
                break;      // skip space after comma
            }
            strEventNew += cChar;
            cLastChar = cChar;
            break;
        case _T(','):
            iEventNum = _ttoi(strTmp);
            if(iEventNum < MIN_EVENT_NUM || iEventNum > MAX_EVENT_NUM)
            {
                return FALSE;
            }
            strTmp.Empty();
            strEventNew += cChar;
            cLastChar = cChar;
            break;
        default:
            // if character is not a digit or if last character was space without preceeding comma
            if(!_istdigit(cChar) || cLastChar == _T(' '))
            {
                return FALSE;
            }
            strTmp += cChar;
            strEventNew += cChar;
            cLastChar = cChar;
        }
    }
    strEventNew.TrimLeft();
    strEventNew.TrimRight();
    
    if(!strTmp.IsEmpty())
    {
        iEventNum = _ttoi(strTmp);
        if(iEventNum < MIN_EVENT_NUM || iEventNum > MAX_EVENT_NUM)
        {
            return FALSE;
        }
    }
    
    if(m_EventString.CompareNoCase(strEventNew) != 0)
    {
        m_EventString = strEventNew;
        UpdateData(FALSE);
    }
    return TRUE;
}


void CObjPropGenTask::OnTypeChanged()
{
    UpdateData();
    InitializeMissingData();
    UpdateData(FALSE);
    EnableControls();

	// d-177 01.02.05 SIS >>
	/*
    // goto dialog control
    switch(m_eType)
    {
    case eTaskTypeCyclic:
        GotoDlgCtrl(GetDlgItem(IDC_INTERVAL));
    case eTaskTypeEventDriven:
        GotoDlgCtrl(GetDlgItem(IDC_EVENTSTRING));
    case eTaskTypeIoSync:
        GotoDlgCtrl(GetDlgItem(IDC_IOSYNC));
        break;
    }*/
	// d-177 01.02.05 SIS <<
}


void CObjPropGenTask::EnableControls()
{
    switch(m_eType)
    {
    case eTaskTypeCyclic:
        GetDlgItem(IDC_AUTOSTART)->EnableWindow(TRUE);
        GetDlgItem(IDC_INTERVAL)->EnableWindow(TRUE);
        GetDlgItem(IDC_PRIORITY)->EnableWindow(TRUE);
        GetDlgItem(IDC_EVENTSTRING)->EnableWindow(FALSE);
        GetDlgItem(IDC_IOSYNC)->EnableWindow(FALSE);
        GetDlgItem(IDC_TASK_SELECTEVENTS_BTN)->EnableWindow(FALSE);
        GetDlgItem(IDC_WATCHDOG)->EnableWindow(TRUE);
        break;
    case eTaskTypeEventDriven:
        GetDlgItem(IDC_AUTOSTART)->EnableWindow(TRUE);
        GetDlgItem(IDC_INTERVAL)->EnableWindow(FALSE);
        GetDlgItem(IDC_PRIORITY)->EnableWindow(TRUE);
        GetDlgItem(IDC_EVENTSTRING)->EnableWindow(TRUE);
        GetDlgItem(IDC_IOSYNC)->EnableWindow(FALSE);
        GetDlgItem(IDC_TASK_SELECTEVENTS_BTN)->EnableWindow(TRUE);
        GetDlgItem(IDC_WATCHDOG)->EnableWindow(TRUE);
        break;
    case eTaskTypeIoSync:
        GetDlgItem(IDC_AUTOSTART)->EnableWindow(FALSE);
        GetDlgItem(IDC_INTERVAL)->EnableWindow(FALSE);
        GetDlgItem(IDC_PRIORITY)->EnableWindow(FALSE);
        GetDlgItem(IDC_EVENTSTRING)->EnableWindow(FALSE);
        GetDlgItem(IDC_IOSYNC)->EnableWindow(TRUE);
        GetDlgItem(IDC_TASK_SELECTEVENTS_BTN)->EnableWindow(FALSE);
        GetDlgItem(IDC_WATCHDOG)->EnableWindow(TRUE);
        break;
    }
}


void CObjPropGenTask::OnEventsSelect()
{
    // select events
    CEventSelection tEventSelectDlg;
    tEventSelectDlg.SetEventString(m_EventString);
	// NFTASKHANDLING 30.05.05 SIS >>
	ASSERT(m_PropertySheet);
	CString	strResource = m_PropertySheet->m_pOperation->m_sFileName;
	int		iFound = strResource.ReverseFind(_T('.'));
	ASSERT(iFound != -1);
	strResource = strResource.Left(iFound);
	tEventSelectDlg.SetResource(strResource);
	tEventSelectDlg.SetProjInfo(m_PropertySheet->m_pOperation->GetProjInfo());
	// NFTASKHANDLING 30.05.05 SIS <<

    if(tEventSelectDlg.DoModal() == IDOK)
    {
        m_EventString = tEventSelectDlg.GetEventString();
        UpdateData(FALSE);
    }
}


BEGIN_MESSAGE_MAP(CObjPropGenTask, CObjPropGenBase)
	ON_BN_CLICKED(IDC_TASKTYPE_CYCLIC, OnTypeChanged)
	ON_BN_CLICKED(IDC_TASKTYPE_EVENT, OnTypeChanged)
	ON_BN_CLICKED(IDC_TASKTYPE_IOSYNC, OnTypeChanged)
	ON_BN_CLICKED(IDC_TASK_SELECTEVENTS_BTN, OnEventsSelect)
END_MESSAGE_MAP()


