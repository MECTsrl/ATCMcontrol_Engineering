
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "resource.h"
#include "TargetOnl.h"
#include "connectdlg.h"
#include "csc_onlineErr.h"
#include "CESysDef.h"
#include "traceif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TARGET_NAME_ADDR    _T("%s (%s)")
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConnectDlg 


CConnectDlg::CConnectDlg( CTargetOnlBase* pTarget, int iConnectTimeOut, BOOL bExclusive, CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
    ASSERT(pTarget != NULL);

    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::CConnectDlg : (TargetId: %s) Exclusive:%s : iConnectTimeOut:%i \n"), 
        pTarget->GetId(), bExclusive?_T("TRUE"):_T("FALSE"), iConnectTimeOut);

    m_TimerId = 0;
    m_pTarget = pTarget;
    m_bExclusive = bExclusive;
    m_bForced = FALSE;
    m_iConnectTimeOut = max(iConnectTimeOut, CE_MIN_CONN_TIMEOUT);
    m_strStateText.LoadString(IDS_CONNECT_CONNECT);
    m_bWasConnected = false;
    m_bWaitForTargetConnection = TRUE;
    m_bWaitForTargetConnection = m_pTarget->GetWaitForTargetConnection();
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	DDX_Control(pDX, IDC_CONNECT_PROGRESS, m_ProgrCtrl);
	DDX_Text(pDX, IDC_CONNECT_STATE, m_strStateText);
	//}}AFX_DATA_MAP
}

void CConnectDlg::GetStateText(CString & strStateText)
{
    strStateText = m_strStateText;
}

BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CConnectDlg 

BOOL CConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    CString strCaption,str;

    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnInitDialog : (TargetId: %s) Exclusive:%s \n"), 
        m_pTarget->GetId(), m_bExclusive?_T("TRUE"):_T("FALSE"));


    if(m_bExclusive)
    {
        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (m_pTarget);
        if(pTarg)
        {
            if( ( m_bWaitForTargetConnection && pTarg->IsTargetConnectedExclusive()) ||
                (!m_bWaitForTargetConnection && pTarg->IsConnectedExclusive())       ||
                InitiateDebugConnection() != S_OK)
            {
                TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnInitDialog : (TargetId: %s) EndDialog (already connected or initiate error)\n"), 
                        m_pTarget->GetId());

                EndDialog(IDOK);
                return(TRUE);
            }
        }
    }
    else
    {
        if(( m_bWaitForTargetConnection && m_pTarget->IsTargetConnected()) ||
           (!m_bWaitForTargetConnection && m_pTarget->IsConnected()) ||
            InitiateMonitoringConnection() != S_OK)
        {
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnInitDialog : (TargetId: %s) EndDialog (already connected or initiate error)\n"), 
                        m_pTarget->GetId());

            EndDialog(IDOK);
            return(TRUE);
        }
    }

    m_ProgrCtrl.SetRange(0, m_iConnectTimeOut);
    m_ProgrCtrl.SetPos(0);
    m_ProgrCtrl.SetStep(1);
    m_TimerId = SetTimer(1, 1000, NULL);
    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnInitDialog : (TargetId: %s) Timer started\n"), 
                        m_pTarget->GetId());

    if(m_pTarget)
    {
        str.Format(TARGET_NAME_ADDR,m_pTarget->GetId(),m_pTarget->GetControlAddress());
        strCaption.Format(IDS_CONNECT_TO_CONTROL,str );
        SetWindowText(strCaption);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CConnectDlg::OnTimer(UINT nIDEvent) 
{
    int iLower;
    int iUpper;
    int iPos;
    CSC_SESSION_STATE ConnectState;
    CSC_SESSION_STATE ExclusiveConnectState = not_connected;
    BOOL bStop = FALSE;
    BOOL bForced = FALSE;
    BOOL bUpdate = FALSE;
    CString strTargetName;
    BOOL bIsTargetConnected = FALSE;
    BOOL bIsTargetConnectedExclusive = FALSE;
    BOOL bIsConnected = FALSE;
    BOOL bIsConnectedExclusive = FALSE;

	CDialog::OnTimer(nIDEvent);

    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s)\n"), 
                        m_pTarget->GetId());

    if(m_pTarget)
    {
        ConnectState        = m_pTarget->GetSessionState();
        bIsTargetConnected  = m_pTarget->IsTargetConnected();
        bIsConnected        = m_pTarget->IsConnected();
        
        // SIS 30.06.04 SUPER SPECIAL HACK FOR OSAI >>
        // even if target is not connected, end dialog
        // this is required if OSAI target is in emergency mode
        // download is possible since download can be done
        // from the OPC server by copying into the file system
        if(m_pTarget->GetType().Compare(_T("OSAI")) == 0)
        {
            bIsTargetConnected = TRUE;
        }
        // SIS 30.06.04 SUPER SPECIAL HACK FOR OSAI <<

        TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s) : SessionState:%s ; IsConnected: %s ; IsTargetConnected: %s\n"), 
                        m_pTarget->GetId(), m_pTarget->GetSessionStateText(ConnectState), 
                        bIsConnected?_T("TRUE"):_T("FALSE"),
                        bIsTargetConnected?_T("TRUE"):_T("FALSE"));

        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (m_pTarget);
        if(pTarg)
        {
            ExclusiveConnectState       = pTarg->GetExclusiveSessionState();
            bIsTargetConnectedExclusive = pTarg->IsTargetConnectedExclusive();
            bIsConnectedExclusive       = pTarg->IsConnectedExclusive(); 
            
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s) : ExclusiveSessionState:%s ; IsConnectedExclusive: %s ; IsTargetConnectedExclusive: %s\n"), 
                        m_pTarget->GetId(), m_pTarget->GetSessionStateText(ExclusiveConnectState),
                        bIsConnectedExclusive?_T("TRUE"):_T("FALSE"),
                        bIsTargetConnectedExclusive?_T("TRUE"):_T("FALSE"));
        }

        if( ( m_bWaitForTargetConnection && 
              bIsTargetConnected && 
             (!m_bExclusive || bIsTargetConnectedExclusive) 
            ) 
          ||
            ( !m_bWaitForTargetConnection &&
              bIsTargetConnected && bIsConnected && 
             (!m_bExclusive || bIsConnectedExclusive) 
            ) 
          )
        {
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s) : Target connected, close dialog.\n"), 
                        m_pTarget->GetId());
            OnOK();
        }

        strTargetName.Format(TARGET_NAME_ADDR,m_pTarget->GetId(),m_pTarget->GetControlAddress());
        m_ProgrCtrl.GetRange(iLower, iUpper);
        iPos = m_ProgrCtrl.GetPos();

        if(interrupted == ConnectState || interrupted == ExclusiveConnectState)
        {
            m_strStateText.Format(IDS_CONNECT_INTERRUPT, strTargetName);
            bUpdate = TRUE;
        }

        if(iPos >= iUpper)
        {
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s) : Timeout.\n"), 
                        m_pTarget->GetId());
            m_strStateText.Format(IDS_CONNECT_TIMEOUT, strTargetName);
            bUpdate = TRUE;
            bStop = TRUE;
        }

        if(!m_bForced && m_bExclusive
            && RW_established == ConnectState
            && waiting_for_exclusive == ExclusiveConnectState
            && E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED == pTarg->GetExclusiveSessionHResult() )
        {
                TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s) : Force Logout Dialog.\n"), 
                        m_pTarget->GetId());

                m_bForced = TRUE;
                CConnErrDlg* pDlg = new CConnErrDlg(this);
                if(pDlg->DoModal() == IDOK) {
                    if(pTarg) {
                        TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnTimer : (TargetId: %s) : User forced Logout.\n"), 
                                m_pTarget->GetId());
                        // user forced connection abort
                        pTarg->DisconnectExclusive(TRUE);
                    }
                }
                delete pDlg;
        }

        if(bUpdate) {
            UpdateData(FALSE);
        }

        if(bStop)
        {
            KillTimer(m_TimerId);
            m_TimerId = 0;
//            RevertConnectState();
            return;
        }

    }

    m_ProgrCtrl.StepIt();
}

void CConnectDlg::OnCancel() 
{
    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::OnCancel : (TargetId: %s) \n"), 
                        m_pTarget->GetId());

    if(m_TimerId > 0)
    {
        if(m_pTarget)
        {
            // user aborted connection before timeout
            CString strTargetName = m_pTarget->GetId();
            m_strStateText.Format(IDS_CONNECT_ABORTED, strTargetName);
            UpdateData(FALSE);
        }

        KillTimer(m_TimerId);
    }

    RevertConnectState();

	CDialog::OnCancel();
}

void CConnectDlg::OnOK() 
{
    KillTimer(m_TimerId);
    m_TimerId = 0;
	CDialog::OnOK();
}


HRESULT CConnectDlg::InitiateMonitoringConnection()
{
    HRESULT hr;
    
    ASSERT(m_pTarget != NULL);
    if(m_pTarget == NULL)
        return(E_FAIL);

    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::InitiateMonitoringConnection : (TargetId: %s) \n"), 
                        m_pTarget->GetId());

    hr = m_pTarget->Connect();
    if(hr != S_OK)
    {
        TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::InitiateMonitoringConnection : (TargetId: %s) Target->Connect returned hr=0x%08X!\n"), 
            m_pTarget->GetId(), hr);
        return(hr);
    }

    return(S_OK);
}


HRESULT CConnectDlg::InitiateDebugConnection()
{
    HRESULT hr;
    CString strMessage;

    ASSERT(m_pTarget != NULL);
    if(m_pTarget == NULL)
        return(E_FAIL);

    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::InitiateDebugConnection : (TargetId: %s) \n"), 
                        m_pTarget->GetId());

    m_bWasConnected = m_pTarget->IsConnected();

    if(!m_bWasConnected)
    {
        hr = m_pTarget->Connect();
        if(hr != S_OK)
        {
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::InitiateDebugConnection : (TargetId: %s) Target->Connect returned hr=0x%08X!\n"), 
                m_pTarget->GetId(), hr);
            return(hr);
        }
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (m_pTarget);
    if(pTarg)
    {
        hr = pTarg->ConnectExclusive();
        if(hr != S_OK)
        {
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::InitiateDebugConnection : (TargetId: %s) Target->ConnectExclusive returned hr=0x%08X!\n"), 
                m_pTarget->GetId(), hr);
            if(!m_bWasConnected)
            {
                m_pTarget->Disconnect();
            }

            return(hr);
        }
    }
    else
    {
        TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::InitiateDebugConnection : (TargetId: %s) GetTargetOnl failed.\n"), 
                m_pTarget->GetId());
        return(E_FAIL);
    }

    return(S_OK);
}


void CConnectDlg::RevertConnectState()
{
    TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::RevertConnectState : (TargetId: %s) \n"), 
                m_pTarget->GetId());

    if(m_bExclusive)
    {
        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (m_pTarget);
        if(pTarg)
        {
            TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::RevertConnectState : (TargetId: %s) DisconnectExclusive\n"), 
                        m_pTarget->GetId());
            pTarg->DisconnectExclusive();

            if(!m_bWasConnected)
            {
                TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::RevertConnectState : (TargetId: %s) Disconnect\n"), 
                        m_pTarget->GetId());
                m_pTarget->Disconnect();
            }
        }
    }
    else
    {
        TrcPrint(TRC_COMM_EVENT, _T("CConnectDlg::RevertConnectState : (TargetId: %s) Disconnect\n"), 
                        m_pTarget->GetId());
        m_pTarget->Disconnect();
    }
}


/////////////////////////////////////////////////////////////////////////////
// CConnErrDlg dialog


CConnErrDlg::CConnErrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnErrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnErrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConnErrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnErrDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnErrDlg, CDialog)
	//{{AFX_MSG_MAP(CConnErrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnErrDlg message handlers
