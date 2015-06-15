

#include "stdafx.h"
#include "AFXPRIV.H"
#include "utilif.h"
#include "htmlhelp.h"
#include "cewatchview.h"
#include "CeWatchBackEnd.h"
#include "CeWatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCeWatchDlg dialog



CCeWatchDlg::CCeWatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCeWatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCeWatchDlg)
	m_strSelVarName = _T("");
	//}}AFX_DATA_INIT

	m_pBackEnd = NULL;
	m_pWriteVar = NULL;
	
	for (int ii = 0; ii < sizeof(m_ToolWndArray)/sizeof(CWnd*); ii++)
		m_ToolWndArray[ii] = NULL;

	m_bHistoryFocus = false;

    m_hAccelerators = ::LoadAccelerators (AfxGetResourceHandle (), MAKEINTRESOURCE (IDR_ACCELERATORS));
}

CCeWatchDlg::~CCeWatchDlg()
{
	if (m_pBackEnd != NULL)
	{
		m_VarList.SetBackEnd(NULL); 
		delete m_pBackEnd;
		m_pBackEnd = NULL;
	}
}


void  CCeWatchDlg::SetProjectPath(LPCTSTR file) 
{ 
    m_ProjectPath = file; 
	if (m_pBackEnd != NULL)
	{
		m_VarList.SetBackEnd(NULL); 
		delete m_pBackEnd;
		m_pBackEnd = NULL;
	}
	if (m_ProjectPath.GetLength() > 0)
		m_pBackEnd = new CCeWatchBackEnd(m_ProjectPath + _T ("\\gen"), this);

    ASSERT(m_pBackEnd);
	m_VarList.SetBackEnd(m_pBackEnd);
    UpdateButtons();
}

BOOL CCeWatchDlg::Store(LPCTSTR file)
{ 
    ASSERT(file);
	if(!file)
	{
        return FALSE;
	}

	CString strFile(file);
	if(strFile.IsEmpty())
    {
		return FALSE;
    }
    return m_VarList.Store(strFile);
}


BOOL CCeWatchDlg::Load(LPCTSTR file) 
{
    ASSERT(file);
	if(!file)
	{
        return FALSE;
	}

    CString strFile(file);
    if(strFile.IsEmpty())
    {
        return FALSE;
    }
    return m_VarList.Load(strFile); 
}

void CCeWatchDlg::ForceResubscribe()
{
    m_VarList.OnForceResubscribe();
}


void CCeWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCeWatchDlg)
	DDX_Control(pDX, IDC_STATIC_SELVAR, m_SelVarNameCtrl);
	DDX_Control(pDX, IDC_LISTCTRL_VARLIST, m_VarList);
	DDX_Control(pDX, IDC_CB_HISTORY, m_HistoryListCtrl);
	DDX_Control(pDX, IDM_REMOVE_SELECTED_VARIABLES, m_DelButton);
	DDX_Control(pDX, IDM_REMOVE_ALL_VARIABLES, m_DelAllButton);
	DDX_Control(pDX, IDC_BTN_WRITE, m_WriteButton);
	DDX_Control(pDX, IDM_ADD_VARIABLE_FROM_LIST, m_InsertButton);
	DDX_Text(pDX, IDC_STATIC_SELVAR, m_strSelVarName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCeWatchDlg, CDialog)
	//{{AFX_MSG_MAP(CCeWatchDlg)
    ON_MESSAGE(WM_CEWATCH_VARLISTCHANGED, OnVarListChanged)
    ON_MESSAGE(WM_CEWATCH_VARCHANGED, OnVarChanged)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()	// SHIFT_F10 14.12.04 SIS
	ON_BN_CLICKED(IDC_BTN_WRITE, OnBtnWrite)
	ON_BN_CLICKED(IDM_REMOVE_ALL_VARIABLES, OnRemoveAllVariables)
	ON_BN_CLICKED(IDM_REMOVE_SELECTED_VARIABLES, OnRemoveSelectedVariables)
	ON_BN_CLICKED(IDM_ADD_VARIABLE_FROM_LIST, OnAddVariableFromList)
	ON_CBN_EDITCHANGE(IDC_CB_HISTORY, OnEditchangeCbHistory)
	ON_CBN_SELCHANGE(IDC_CB_HISTORY, OnSelchangeCbHistory)
	ON_WM_SETCURSOR()
	ON_WM_CLOSE()
	ON_CBN_SETFOCUS(IDC_CB_HISTORY, OnSetFocusCbHistory)
	ON_CBN_KILLFOCUS(IDC_CB_HISTORY, OnKillFocusCbHistory)
	ON_WM_HELPINFO()
    ON_COMMAND(ID_COPY_ITEMS, OnCopyItems)
    ON_COMMAND(ID_CUT_ITEMS, OnCutItems)
    ON_COMMAND(ID_PASTE_ITEMS, OnPasteItems)
	// 14.12.05 SIS >>
	ON_COMMAND(IDM_INC_RANGE, OnIncRange)
	ON_COMMAND(IDM_DEC_RANGE, OnDecRange)
	// 14.12.05 SIS >>
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCeWatchDlg message handlers

BOOL CCeWatchDlg::PreTranslateMessage (MSG *pMsg)
{
    if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg) != 0)
        return (TRUE);

    return (CDialog::PreTranslateMessage (pMsg));
}


BOOL CCeWatchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bHistoryFocus = false;

	m_InsertButton.SetIcons(IDI_INSERT, IDI_INSERT_DISABLE);
	m_DelButton.SetIcons(IDI_DEL, IDI_DEL_DISABLE);
	m_DelAllButton.SetIcons(IDI_DELALL, IDI_DELALL_DISABLE);

	VERIFY(m_ToolTipCtrl.Create(this, TTS_ALWAYSTIP));
	m_ToolTipCtrl.AddTool(&m_InsertButton  , IDS_TIP_INSERT);
	m_ToolTipCtrl.AddTool(&m_DelButton     , IDS_TIP_DEL);
	m_ToolTipCtrl.AddTool(&m_DelAllButton  , IDS_TIP_DELALL);
	m_ToolTipCtrl.AddTool(&m_WriteButton   , IDS_TIP_WRITE);
	m_ToolTipCtrl.AddTool(&m_HistoryListCtrl, IDS_TIP_HISTORY);
	m_ToolTipCtrl.Activate(TRUE);

	m_ToolWndArray[0] = &m_InsertButton;
	m_ToolWndArray[1] = &m_DelButton;
	m_ToolWndArray[2] = &m_DelAllButton;
	m_ToolWndArray[3] = &m_WriteButton;
	m_ToolWndArray[4] = &m_HistoryListCtrl;

	m_pWriteVar = NULL;
	m_VarList.Init();
	UpdateHistoryList();
	UpdateButtons();

	// SHIFT_F10 15.12.04 SIS >>
	// set focus to list control
	m_VarList.SetFocus();

	return FALSE;
	// SHIFT_F10 15.12.04 SIS <<
}

// SHIFT_F10 14.12.04 SIS >>
void CCeWatchDlg::OnSetFocus(CWnd* pOldWnd)
{
	m_VarList.SetFocus();
}
// SHIFT_F10 14.12.04 SIS <<

void CCeWatchDlg::OnClose() 
{
	CDialog::OnClose();
}

void CCeWatchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (!IsWindow(m_VarList.GetSafeHwnd()) || 
	    !IsWindow(m_WriteButton.GetSafeHwnd()) ||
	    !IsWindow(m_HistoryListCtrl.GetSafeHwnd()) ||
	    !IsWindow(m_SelVarNameCtrl.GetSafeHwnd()))
	return;

	cx -= 2;
	cy -= 4;
	CRect rcPar, rc1, rc2, rc3, rc4;
	GetWindowRect(rcPar);

	m_WriteButton.GetWindowRect(rc1);
	m_HistoryListCtrl.GetWindowRect(rc2);
	m_VarList.GetWindowRect(rc3);
	m_SelVarNameCtrl.GetWindowRect(rc4);
	
	// distance between single controls
	int dc = rc1.left - rc2.right;
	// distance to border
	int db = rc3.left - rcPar.left;

	// move list control
	ScreenToClient(rc3);
	m_VarList.MoveWindow(db, rc3.top, cx - 2 * db, cy - rc3.top - db);
	
	// move var name text field
	ScreenToClient(rc4);
	int w = cx - rc4.left - dc - rc2.Width() - dc - rc1.Width() - db;
	w = max(w, 60);
	m_SelVarNameCtrl.MoveWindow(
		rc4.left, rc4.top, w, rc4.Height());
	m_SelVarNameCtrl.GetWindowRect(rc4);
	ScreenToClient(rc4);

	// move history list
	ScreenToClient(rc2);
	m_HistoryListCtrl.MoveWindow(
			rc4.right + dc, rc2.top, rc2.Width(), rc2.Height());
	m_HistoryListCtrl.GetWindowRect(rc2);
	ScreenToClient(rc2);

	// move write button
	ScreenToClient(rc1);
	m_WriteButton.MoveWindow(
		rc2.right + dc, rc1.top, rc1.Width(), rc1.Height());
}

//*****************************************************************************	
LRESULT CCeWatchDlg::OnVarListChanged(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	CCeWatchElement* pWriteVar = NULL; 
	if (IsWindow(m_VarList.GetSafeHwnd()))
		pWriteVar = m_VarList.GetSelectedVar();


	if (pWriteVar != NULL && !pWriteVar->GetType().IsSimpleType())
		pWriteVar = NULL;

	m_pWriteVar = pWriteVar;

	if (pWriteVar != NULL)
		m_SelVarNameCtrl.SetWindowText(pWriteVar->GetName());
	else
		m_SelVarNameCtrl.SetWindowText("");

	UpdateHistoryList();
	if (pWriteVar != NULL)
		m_HistoryListCtrl.SetWindowText(CCeWatchBackEnd::CleanValue(m_pWriteVar, m_pWriteVar->GetValue()));

	UpdateButtons();
	return 0;
}
//*****************************************************************************	
LRESULT CCeWatchDlg::OnVarChanged(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	CCeDisplayElement* pD = (CCeDisplayElement*)lParam;
	if (pD != NULL)
	{
		m_VarList.UpdateItem(pD);
		if (pD->GetWatchElement() == m_pWriteVar)
		{
			if (!m_bHistoryFocus)
			{
				UpdateWriteButton();
				m_HistoryListCtrl.SetWindowText(CCeWatchBackEnd::CleanValue(m_pWriteVar, m_pWriteVar->GetValue()));
			}
		}
	}

	return 0;
}


void CCeWatchDlg::OnBtnWrite() 
{
	if (CheckWrite())
	{
		CString strValue;
		m_HistoryListCtrl.GetWindowText(strValue);
		if (m_pBackEnd->WriteVar(m_pWriteVar, strValue))
		{
			m_pWriteVar->AddToHistory(strValue);
			m_HistoryListCtrl.ResetContent();
			for (int ii = 0; ii < m_pWriteVar->GetHistoryCount(); ii++)
				m_HistoryListCtrl.AddString(m_pWriteVar->GetHistory(ii));
		}
        else
        {
	        CString strMessage;
	        strMessage.LoadString(IDS_ERROR_WRITE_FAILED);
            ::AfxMessageBox(strMessage, MB_ICONSTOP);
        }
	}
}

void CCeWatchDlg::OnRemoveAllVariables() 
{
	if (IsWindow(m_VarList.GetSafeHwnd()))
		m_VarList.PostMessage(WM_COMMAND, IDM_REMOVE_ALL_VARIABLES, 0L);	
}

void CCeWatchDlg::OnRemoveSelectedVariables() 
{
	if (IsWindow(m_VarList.GetSafeHwnd()))
		m_VarList.PostMessage(WM_COMMAND, IDM_REMOVE_SELECTED_VARIABLES, 0L);		
}

void CCeWatchDlg::OnAddVariableFromList() 
{
	if (IsWindow(m_VarList.GetSafeHwnd()))
		m_VarList.PostMessage(WM_COMMAND, IDM_ADD_VARIABLE_FROM_LIST, 0L);
}

void CCeWatchDlg::UpdateWriteButton(LPCTSTR pNewValue/*=NULL*/) 
{
	if (IsWindow(m_WriteButton.GetSafeHwnd()))
		m_WriteButton.EnableWindow(CheckWrite(pNewValue));
}

void CCeWatchDlg::UpdateDelButton() 
{
	if (IsWindow(m_DelButton.GetSafeHwnd()) && IsWindow(m_VarList.GetSafeHwnd()))
		m_DelButton.EnableWindow(m_VarList.CheckVariableDelete());
}

void CCeWatchDlg::UpdateDelAllButton() 
{
	if (IsWindow(m_DelAllButton.GetSafeHwnd()) && IsWindow(m_VarList.GetSafeHwnd()))
		m_DelAllButton.EnableWindow(m_VarList.GetItemCount() > 0);
}
void CCeWatchDlg::UpdateInsertButton() 
{
	if (IsWindow(m_InsertButton.GetSafeHwnd()) && IsWindow(m_VarList.GetSafeHwnd()))
		m_InsertButton.EnableWindow(m_VarList.CheckVariableInsert());
}

void CCeWatchDlg::UpdateHistoryList() 
{
	if (IsWindow(m_HistoryListCtrl.GetSafeHwnd()))
	{
		if (m_pWriteVar != NULL && m_pWriteVar != NULL)
		{
			CCeWatchElement* pWriteVar = m_pWriteVar;
			ASSERT(IsValidWatchElement(pWriteVar));
			m_HistoryListCtrl.EnableWindow(TRUE);
			m_HistoryListCtrl.ResetContent();
			for (int ii = 0; ii < pWriteVar->GetHistoryCount(); ii++)
				m_HistoryListCtrl.AddString(pWriteVar->GetHistory(ii));
			m_HistoryListCtrl.SetWindowText("");
		}
		else
		{
			m_HistoryListCtrl.ResetContent();
			m_HistoryListCtrl.EnableWindow(FALSE);
		}
	}
}


void CCeWatchDlg::UpdateButtons() 
{
	UpdateWriteButton();
	UpdateDelButton();
	UpdateDelAllButton();
	UpdateInsertButton();
}

bool CCeWatchDlg::CheckWrite(LPCTSTR pNewValue/*=NULL*/) 
{
	bool bEnable = true;
	if (bEnable)
		bEnable = bEnable && (m_pBackEnd != NULL);
	if (bEnable)
		bEnable = bEnable && (m_pWriteVar != NULL);
	if (bEnable)
		bEnable = bEnable && m_pWriteVar->IsConnected();		
	if (bEnable)
	{
		CString strNewValue;
		if (pNewValue == NULL)
			m_HistoryListCtrl.GetWindowText(strNewValue);
		else
			strNewValue = pNewValue;

		bEnable = bEnable && CCeWatchBackEnd::IsValidValue(m_pWriteVar, strNewValue);
	}
	return bEnable;
}

void CCeWatchDlg::OnEditchangeCbHistory() 
{
	UpdateWriteButton();
}

void CCeWatchDlg::OnSelchangeCbHistory() 
{
	CString strValue;
	if (m_HistoryListCtrl.GetCurSel() >= 0)
	{
		m_HistoryListCtrl.GetLBText(m_HistoryListCtrl.GetCurSel(), strValue);
		m_HistoryListCtrl.SetWindowText(strValue);
	}

	UpdateWriteButton(strValue);	
}

BOOL CCeWatchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (pWnd != NULL && IsWindow(m_ToolTipCtrl.GetSafeHwnd()))	
	{
		POINT pt;
		CRect rc;
		::GetCursorPos(&pt);

		for (int ii = 0; ii < sizeof(m_ToolWndArray)/sizeof(CWnd*); ii++)
		{
			if (m_ToolWndArray[ii] != NULL && IsWindow(m_ToolWndArray[ii]->GetSafeHwnd()))
			{
				m_ToolWndArray[ii]->GetWindowRect(rc);
				if (rc.PtInRect(pt))
				{
					m_ToolWndArray[ii]->ScreenToClient(&pt);

					MSG msg;
					switch(message)
					{
					case WM_MOUSEMOVE:
					case WM_LBUTTONDOWN:
					case WM_MBUTTONDOWN:
					case WM_RBUTTONDOWN:
					case WM_LBUTTONUP:
					case WM_MBUTTONUP:
					case WM_RBUTTONUP:
						msg.message = message;            
						break;
					default:
						msg.message = WM_MOUSEMOVE;            					
						break;
					}
					msg.hwnd    = m_ToolWndArray[ii]->m_hWnd;
					msg.wParam  = 0;
					msg.lParam  = MAKELPARAM(pt.x, pt.y);     
					msg.time    = 0;
					msg.pt      = pt;
					m_ToolTipCtrl.RelayEvent(&msg);
					break;
				}
			}
		}
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CCeWatchDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (wParam == IDOK || wParam == IDCANCEL)
	{
		if (m_bHistoryFocus)
		{
			if (wParam == IDOK)
				OnBtnWrite();
			else
				m_HistoryListCtrl.ResetContent();
		}
		else
		{
			m_VarList.PostMessage(WM_CEWATCH_ENDEDIT, wParam, NULL);
		}
		return TRUE;
	}
	else
		return CDialog::OnCommand(wParam, lParam);
}

void CCeWatchDlg::OnSetFocusCbHistory() 
{
	m_bHistoryFocus = true;

}

void CCeWatchDlg::OnKillFocusCbHistory() 
{
	m_bHistoryFocus = false;
	
}

BOOL CCeWatchDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	WinHelp(HID_BASE_RESOURCE + IDD_WATCH);
	return TRUE;

//	return CDialog::OnHelpInfo(pHelpInfo);
}

void CCeWatchDlg::WinHelp(DWORD dwData, UINT nCmd) 
{
	HWND hWnd = ::GetActiveWindow();

	CString strManualPath;
	UINT    nStrLen = _MAX_PATH;

	HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
	strManualPath.ReleaseBuffer();
	if(SUCCEEDED(hr)) 
	{
		strManualPath += CE_MANUAL_FILENAME;
		switch(nCmd) 
		{
		case HELP_CONTEXT:
			if(dwData >= 0x30000) 
			{
				dwData -= 0x30000;
			} 
			else if(dwData >= 0x20000) 
			{
				dwData -= 0x20000;
			} 
			else if(dwData >= 0x10000) 
			{
				dwData -= 0x10000;
			}
			HtmlHelp(hWnd, strManualPath, HH_HELP_CONTEXT, dwData);
			break;
		case HELP_INDEX:
			break;
		
		case HELP_FINDER:
			HtmlHelp(hWnd, strManualPath, HH_DISPLAY_TOPIC, NULL);
			break;
		}
	}
	
//	CDialog::WinHelp(dwData, nCmd);
}


void CCeWatchDlg::OnCopyItems ()
{
    CopyItems ();
}


void CCeWatchDlg::OnCutItems ()
{
    if (CopyItems ())
        m_VarList.DeleteSelectedVar ();
}


bool CCeWatchDlg::CopyItems ()
{
    CArray<CCeDisplayElement *, CCeDisplayElement *> displays;
    int i;
    CString strNames;
    HGLOBAL memoryHandle;
    TCHAR *memoryBlock;

    m_VarList.GetSelectedDisplayElements (displays);

    for (i = 0; i < displays.GetSize (); ++i)
    {
        CCeDisplayElement *pDisplay;
        CCeWatchElement *pElement;
        CString strName;

        pDisplay = displays[i];
        ASSERT (pDisplay != NULL);
        if (pDisplay == NULL)
            continue;

        pElement = pDisplay->GetWatchElement ();
        ASSERT (pElement != NULL);
        if (pElement == NULL)
            continue;

        strName = pElement->GetName ();

        if (pElement->GetLevel () > 1)
        {
            int iPos;
        
            iPos = strName.ReverseFind ('.');
		    if (iPos > 0 && iPos < strName.GetLength () - 1)
                strName = strName.Mid (iPos + 1);

        };

        if (pElement->GetType ().IsArray ())
            strName += _T (", [") + pDisplay->GetWatchIndices ().AsString () + _T ("]");

        if (!strNames.IsEmpty ())
            strNames += (_T ("\n"));

        strNames += strName;
    };

    if (strNames.GetLength () == 0)
        return (true);

    memoryHandle = ::GlobalAlloc (GMEM_MOVEABLE | GMEM_DDESHARE, strNames.GetLength () + 1);
    if (memoryHandle == NULL)
        return (false);

    memoryBlock = (TCHAR *)::GlobalLock (memoryHandle);
    if (memoryBlock == NULL)
    {
        ::GlobalFree (memoryHandle);
        return (false);
    };

    _tcscpy (memoryBlock, strNames);

    ::GlobalUnlock (memoryHandle);

    if (!::OpenClipboard (m_hWnd))
    {
        ::GlobalFree (memoryHandle);
        return (false);
    };

    if (!::EmptyClipboard ())
    {
        ::GlobalFree (memoryHandle);
        return (false);
    };

    if (::SetClipboardData (CF_TEXT, memoryHandle) == NULL)
    {
        ::GlobalFree (memoryHandle);
        return (false);
    };

    ::CloseClipboard ();

    return (true);
}


void CCeWatchDlg::OnPasteItems ()
{
    if (m_bHistoryFocus)
    {
        m_HistoryListCtrl.Paste();
        return;
    }

    HGLOBAL memoryHandle;
    LPCTSTR memoryBlock;
    CString strNames;

    if (!::OpenClipboard (m_hWnd))
        return;

    memoryHandle = ::GetClipboardData (CF_TEXT);
    if (memoryHandle == NULL)
    {
        ::CloseClipboard ();
        return;
    };

    memoryBlock = (LPCTSTR)::GlobalLock (memoryHandle);
    if (memoryBlock == NULL)
    {
        ::CloseClipboard ();
        return;
    };

    strNames = memoryBlock;

    ::GlobalUnlock (memoryHandle);
    ::CloseClipboard ();

    while (!strNames.IsEmpty ())
    {
        int iPos;
        CString strName;
        CCeWatchElement *pInsertedElement=NULL;

        iPos = strNames.Find (_T ('\n'));
        if (iPos == -1)
        {
            strName = strNames;
            strNames = _T ("");
        }
        else
        {
            strName = strNames.Left (iPos);
            strNames = strNames.Mid (iPos + 1);
        };

        strName.TrimLeft ();
        strName.TrimRight ();

        if (!strName.IsEmpty ())
            m_VarList.InsertExpressionWithStructInfo(strName, pInsertedElement);
    };
}


// 14.12.05 SIS >>
void CCeWatchDlg::OnIncRange()
{
	m_VarList.IncRange(FALSE);
}

void CCeWatchDlg::OnDecRange()
{
	m_VarList.IncRange(TRUE);
}
// 14.12.05 SIS <<


//------------------------------------------------------------------*
/**
 * save to node.
 *
 *  delegates call to CCeWatchList::SaveToNode()
 *
 * @param           [in] rtNode: xml node representing watch view page
 * @return          successful?
 * @exception       -
 * @see             CCeWatchList::SaveToNode()
*/
BOOL CCeWatchDlg::SaveToNode(CXMLNode& rtNode, CXMLDocument& rtDocument)
{
    if(m_VarList.SaveToNode(rtNode, rtDocument))
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * load from node.
 *
 *  delegates call to CCeWatchList::LoadFromNode()
 *
 * @param           [in] rtNode: xml node representing watch view page
 * @return          successful?
 * @exception       -
 * @see             CCeWatchList::LoadFromNode()
*/
BOOL CCeWatchDlg::LoadFromNode(CXMLNode& rtNode)
{
    if(m_VarList.LoadFromNode(rtNode))
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * deactiate
 *
 *  is delegated to var list.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CCeWatchDlg::Deactivate()
{
    m_VarList.Deactivate();
}

