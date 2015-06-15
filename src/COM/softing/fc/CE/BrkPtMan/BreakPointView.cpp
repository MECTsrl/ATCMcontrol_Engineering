

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// BreakPointView.cpp : Implementation of CBreakPointView

#include "stdafx.h"
#include <windowsx.h>
#include "fc_todebug\fc_assert.h"
#include "BrkPtMan.h"
#include "BreakPointView.h"


/////////////////////////////////////////////////////////////////////////////
// local defines

#ifndef ListView_SetCheckState
   #define ListView_SetCheckState(hwndLV, i, fCheck) \
      ListView_SetItemState(hwndLV, i, \
      INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif


/////////////////////////////////////////////////////////////////////////////
// CBreakPointView

// codes for bitmap asignment
#define BP_ENABLED  0
#define BP_DISABLED 1
#define BP_ENABLEDUNSET 2
#define BP_STOPPED  3

#define BP_STOPPED_OVERLAY  4


CBreakPointView::~CBreakPointView()
{
    RemoveAllItems();
}


HWND CBreakPointView::Create(HWND hWndParent, RECT& rcPos, LPARAM dwInitParam/* = NULL*/) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HWND hWnd = CComCompositeControl<CBreakPointView>::Create(hWndParent, rcPos, dwInitParam);

    if(hWnd != NULL)
    {
        m_BPListHWnd = GetDlgItem(IDC_BPLIST);
        m_RemoveBtnHWnd = GetDlgItem(IDC_REMOVE);
        m_RemoveAllBtnHWnd = GetDlgItem(IDC_REMOVEALL);
        m_GotoBtnHWnd = GetDlgItem(IDC_GOTO);

        m_hAccelerators = ::LoadAccelerators (_Module.GetResourceInstance (), MAKEINTRESOURCE (IDR_BREAKPOINTVIEW));

        int iSize = sizeof(m_SysMetrics);
        ZeroMemory(&m_SysMetrics, iSize);
        m_SysMetrics.cbSize = iSize;
        if(!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, iSize, &m_SysMetrics, 0)) {
            return(NULL);
        }

        ListView_SetExtendedListViewStyle(m_BPListHWnd, LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

        // attach the ImageList to the tree
	    ListView_SetImageList(m_BPListHWnd, m_ImageList.m_hImageList, LVSIL_SMALL);

        TEXTMETRIC  tm;
        HDC         hDC = ::GetDC(m_BPListHWnd);
        if(GetTextMetrics(hDC, &tm)) {
            m_iCtrlHeight = tm.tmHeight + 4;
        } else {
            m_iCtrlHeight = 8;
        }
        ::ReleaseDC(m_BPListHWnd, hDC);

        // set the button icons
        HICON hIcon;
        hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_REMOVE)); 
        ::SendMessage(m_RemoveBtnHWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hIcon);
        hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_REMOVEALL)); 
        ::SendMessage(m_RemoveAllBtnHWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hIcon);
        hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_GOTO)); 
        ::SendMessage(m_GotoBtnHWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hIcon);
		
		DisplayColumnHeadings();

        m_ToolTipHWnd = ::CreateWindowEx (0,
                                          TOOLTIPS_CLASS,
    		                              NULL,
                                          WS_POPUP | TTS_ALWAYSTIP,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
    		                              hWnd,
                                          0,
                                          _Module.GetModuleInstance (),
                                          NULL);

	    if (m_ToolTipHWnd != NULL)
        {
            ::SendMessage (m_ToolTipHWnd, TB_SETPARENT, (WPARAM)hWnd, 0L);

            AddTool (m_RemoveBtnHWnd, IDS_REMOVE_BREAKPOINT);
            AddTool (m_RemoveAllBtnHWnd, IDS_REMOVE_ALL_BREAKPOINTS);
            AddTool (m_GotoBtnHWnd, IDS_GOTO_SOURCE_CODE);

            ::SendMessage (m_ToolTipHWnd, TTM_ACTIVATE, TRUE, 0);
        };
    }

    return(hWnd);
}


void CBreakPointView::AddTool (HWND hwndTool, UINT uiText)
{
	TOOLINFO ti;

    memset (&ti, 0, sizeof (ti));

    ti.cbSize = sizeof (ti);
    ti.hwnd = m_hWnd;
    ti.uFlags = TTF_IDISHWND;
    ti.uId = (UINT)hwndTool;
    ti.hinst = _Module.GetResourceInstance ();
    ti.lpszText = (LPTSTR)MAKEINTRESOURCE (uiText);

    ::SendMessage (m_ToolTipHWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}


LRESULT CBreakPointView::OnSetCursor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    HWND hWnd;
    MSG msg;
    POINT pt;

    bHandled = FALSE;

    if (m_ToolTipHWnd == NULL)
    {
        return (0);
    };

    hWnd = (HWND)wParam;
    if (hWnd == NULL)
    {
        return (0);
    };

    if (hWnd != m_RemoveBtnHWnd && hWnd != m_RemoveAllBtnHWnd && hWnd != m_GotoBtnHWnd)
    {
        return (0);
    };

    if (HIWORD (lParam) < WM_MOUSEFIRST || HIWORD (lParam) > WM_MOUSELAST)
    {
        return (0);
    };

    ::GetCursorPos (&pt);

    msg.message = HIWORD (lParam);
    msg.hwnd = hWnd;
    msg.wParam = 0;
    msg.lParam = MAKELPARAM (pt.x, pt.y);     
    msg.time = 0;
    msg.pt = pt;

    ::SendMessage (m_ToolTipHWnd, TTM_RELAYEVENT, 0, (LPARAM)&msg);

    return (0);
}


HRESULT CBreakPointView::FinalConstruct() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// create ImageList
	m_ImageList.Create(IDB_BPSYMBOL, 16, 0, RGB(255, 0, 255));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
    m_ImageList.SetOverlayImage(BP_STOPPED,BP_STOPPED_OVERLAY);

    m_iActItemIndex = -1;   // no Item selected

    return(S_OK);
}

void CBreakPointView::FinalRelease() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    m_ImageList.Detach();
    m_ImageList.DeleteImageList();
	return;
}

void CBreakPointView::CalcRects(const RECT* pClientRect) {
    RECT    r = *pClientRect;

    CSize BtnSize(22, 19);

    m_rRemoveBtn = r;
    m_rRemoveBtn.bottom = m_rRemoveBtn.top + BtnSize.cy;
    m_rRemoveBtn.right = m_rRemoveBtn.left + BtnSize.cx;

    m_rRemoveAllBtn = m_rRemoveBtn;
    m_rRemoveAllBtn.left = m_rRemoveBtn.left + BtnSize.cx;
    m_rRemoveAllBtn.right = m_rRemoveAllBtn.left + BtnSize.cx;

    m_rGotoBtn = m_rRemoveBtn;
    m_rGotoBtn.left = m_rRemoveAllBtn.left + BtnSize.cx;
    m_rGotoBtn.right = m_rGotoBtn.left + BtnSize.cx;

    m_rBPList = r;
    m_rBPList.top += 21;
    m_rBPList.bottom -= 0;
    return;
}

void CBreakPointView::DisplayColumnHeadings(void) {
	CString strHeadings;
	strHeadings.LoadString(IDS_COLUMNS);

	int iPos;
	int iCount = 0;

	EraseColumns();
	EraseList();
	while((iPos = strHeadings.Find(_T(","))) != -1) {
		CString strItem;
		strItem = strHeadings.Left(iPos);
		AddColumn(strItem,iCount++);
		strItem = strHeadings.Mid(iPos + 1);
		strHeadings = strItem;
	}
	AddColumn(strHeadings,iCount);
	return;
}

void CBreakPointView::EraseColumns(void) {
	while(ListView_DeleteColumn(m_BPListHWnd, 0));
	UpdateWindow();
    ASSERT(_CrtCheckMemory());
	return;
}

void CBreakPointView::EraseList(void) {
	ListView_DeleteAllItems(m_BPListHWnd);
	return;
}

BOOL CBreakPointView::AddColumn(LPCTSTR strItem,int nItem,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = ListView_GetStringWidth(m_BPListHWnd, lvc.pszText) + 15;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return(ListView_InsertColumn(m_BPListHWnd, nItem, &lvc)); 
}

BOOL CBreakPointView::AddItemRaw(int nItem, int nSubItem, LPCTSTR strItem, int nImageIndex) {
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}
	if(nSubItem == 0)
		return(ListView_InsertItem(m_BPListHWnd, &lvItem));
	return(ListView_SetItem(m_BPListHWnd, &lvItem));
}


void CBreakPointView::AddItem (int iIndex, BPListInfo *pListInfo)
{
	bool bActive;
    int iImage;
    bool bStoppedOverlay;
	CString strPosition;
    
	ASSERT (pListInfo != NULL);
    if (pListInfo == NULL)
        return;

	m_bNoUpdate = TRUE;

    iImage = GetItemImage (pListInfo, bStoppedOverlay);
	AddItemRaw (iIndex, 0, pListInfo->m_strInstanceName, iImage);
    if (bStoppedOverlay)
    {
        ListView_SetItemState (m_BPListHWnd, iIndex, INDEXTOOVERLAYMASK (BP_STOPPED_OVERLAY), LVIS_OVERLAYMASK);
    };

    BuildPosText (pListInfo, strPosition);
    AddItemRaw (iIndex, 1, strPosition);

    bActive = pListInfo->m_BPState == BPM_BPACTIVE || pListInfo->m_BPState == BPM_BPACTIVEUNSET;
	ListView_SetCheckState (m_BPListHWnd, iIndex, bActive);

	m_bNoUpdate = FALSE;
}


void CBreakPointView::RefreshItemState (int iIndex, BPListInfo *pListInfo)
{
    bool bStoppedOverlay;
    bool bActive;

    m_bNoUpdate = TRUE;

	LV_ITEM lvItem;
	lvItem.mask = LVIF_IMAGE | LVIF_STATE;
	lvItem.iItem = iIndex;
	lvItem.iSubItem = 0;
	lvItem.iImage = GetItemImage (pListInfo, bStoppedOverlay);
    lvItem.stateMask = LVIS_OVERLAYMASK;
    lvItem.state = bStoppedOverlay ? INDEXTOOVERLAYMASK (BP_STOPPED_OVERLAY) : 0;
	ListView_SetItem (m_BPListHWnd, &lvItem);

    bActive = pListInfo->m_BPState == BPM_BPACTIVE || pListInfo->m_BPState == BPM_BPACTIVEUNSET;
	ListView_SetCheckState (m_BPListHWnd, iIndex, bActive);

    m_bNoUpdate = FALSE;
}


int CBreakPointView::GetItemImage (BPListInfo *pListInfo, bool &bStoppedOverlay)
{
    int iImage;

    switch (pListInfo->m_BPState)
    {
    case BPM_BPACTIVE:
        iImage = BP_ENABLED;
        bStoppedOverlay = pListInfo->m_StopState == BPM_STOPPED;
        break;

    case BPM_BPACTIVEUNSET:
        iImage = BP_ENABLEDUNSET;
        bStoppedOverlay = pListInfo->m_StopState == BPM_STOPPED;
        break;

    case BPM_BPINACTIVE:
        iImage = BP_DISABLED;
        bStoppedOverlay = pListInfo->m_StopState == BPM_STOPPED;
        break;

    case BPM_NOBP:
        iImage = pListInfo->m_StopState == BPM_STOPPED ? BP_STOPPED : 0;
        bStoppedOverlay = false;
        break;
    };

    return (iImage);
}



//------------------------------------------------------------------*
/**
 * remove item.
 *
 * @param           [in] nItem: number of item
 * @return          successful?
*/
BOOL CBreakPointView::RemoveItem(int nItem)
{
    BOOL    bReturn = TRUE;
    // SIS: it is a good idea to delete the object before
    // removing the pointer
    BPListInfo* pInfo = m_Breakpoints[nItem];
    if(pInfo)
    {
        delete pInfo;
    }
	m_Breakpoints.RemoveAt(nItem);
    if(::IsWindow(m_BPListHWnd))
    {
        bReturn = ListView_DeleteItem(m_BPListHWnd, nItem);
    }
	return(bReturn);		
}

//------------------------------------------------------------------*
/**
 * remove all items (break points).
 *
*/
void CBreakPointView::RemoveAllItems()
{
    int iIndex;
    int iBPNum = m_Breakpoints.GetSize();
    for(iIndex = iBPNum-1; iIndex >= 0; --iIndex)
    {
        RemoveItem(iIndex);
    }
}



void CBreakPointView::BuildPosText(BPListInfo* pListInfo, CString& strPosition)
{
    CString strTemp;

    switch(pListInfo->m_PosQual) {
        case EDP_STLINE:
            strTemp.LoadString(IDS_STLINE_POS);
            strPosition.Format(strTemp, pListInfo->m_lY);
            break;
        case EDP_GRBEGIN:
            strPosition.LoadString(IDS_GRBEGIN_POS);
            break;
        case EDP_GREND:
            strPosition.LoadString(IDS_GREND_POS);
            break;
        case EDP_GRBOX:
            strTemp.LoadString(IDS_GRBOX_POS);
			strPosition.Format(strTemp, pListInfo->m_lY, pListInfo->m_lX); 
            break;
        case EDP_GRSTBOX:
            strTemp.LoadString(IDS_GRSTBOX_POS);
			strPosition.Format(strTemp, pListInfo->m_lY, pListInfo->m_lX, pListInfo->m_lCY); 
            break;
        case EDP_GRREGION:
            assert(!"not implem");
            strTemp = _T("BP 'region' not implemented!!");
            break;
        default:
            strPosition.Empty();
            break;
    }
    return;
}

void CBreakPointView::ShowBPList(int iSelIndex)
{
    int iIndex;

	m_bNoUpdate = TRUE;
    for(iIndex = 0; iIndex < m_Breakpoints.GetSize (); ++iIndex) {
		AddItem(iIndex, m_Breakpoints[iIndex]);
    }
    if(iIndex != 0) { 
        if((iSelIndex > -1) && (iSelIndex < iIndex)) {
            ListView_SetItemState(m_BPListHWnd, iSelIndex, LVIS_SELECTED, LVIS_SELECTED);
        } else {
            ListView_SetItemState(m_BPListHWnd, --iIndex, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
	m_bNoUpdate = FALSE;
	return;
}

LRESULT CBreakPointView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO : Add Code for message handler. Call DefWindowPoc if necessary.
    RECT r;
    r.top = r.left = 0;
    r.right = LOWORD(lParam);  // width of client area
    r.bottom = HIWORD(lParam); // height of client area
    CalcRects(&r);

    ::MoveWindow(m_RemoveBtnHWnd,
		m_rRemoveBtn.left, m_rRemoveBtn.top, 
        m_rRemoveBtn.right - m_rRemoveBtn.left, 
        m_rRemoveBtn.bottom - m_rRemoveBtn.top, TRUE);

	::MoveWindow(m_RemoveAllBtnHWnd,
		m_rRemoveAllBtn.left, m_rRemoveAllBtn.top, 
        m_rRemoveAllBtn.right - m_rRemoveAllBtn.left, 
        m_rRemoveAllBtn.bottom - m_rRemoveAllBtn.top, TRUE);

    ::MoveWindow(m_GotoBtnHWnd,
		m_rGotoBtn.left, m_rGotoBtn.top, 
        m_rGotoBtn.right - m_rGotoBtn.left, 
        m_rGotoBtn.bottom - m_rGotoBtn.top, TRUE);

    ::MoveWindow(m_BPListHWnd,
		m_rBPList.left, m_rBPList.top, 
        m_rBPList.right - m_rBPList.left, 
        m_rBPList.bottom - m_rBPList.top, TRUE);
    return 0;
}

void CBreakPointView::RemoveBP(int iItem) {
	m_pIBreakPoint->ResetBP(CComBSTR(m_Breakpoints[iItem]->m_strInstanceName), 
							m_Breakpoints[iItem]->m_PosQual, 
                            m_Breakpoints[iItem]->m_lX, 
                            m_Breakpoints[iItem]->m_lY,
                            m_Breakpoints[iItem]->m_lCX, 
                            m_Breakpoints[iItem]->m_lCY
                         );
	return;
}

LRESULT CBreakPointView::OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // to avoid handling of the m_Breakpoints array during deletion use a 
    // additional temp array RemoveList
	int	iListItemCount;
	int iListItem;
	bool bUpdate=FALSE;
    CArray<BPListInfo*, BPListInfo*> RemoveList;

	if(ListView_GetSelectedCount(m_BPListHWnd) <= 0) {
        return(S_OK);
    }
	iListItemCount = ListView_GetItemCount(m_BPListHWnd); 
	for(iListItem = 0; iListItem < iListItemCount; ++iListItem) {
		if(ListView_GetItemState(m_BPListHWnd, iListItem, LVIS_SELECTED)) {
            RemoveList.Add(m_Breakpoints[iListItem]);
			bUpdate = TRUE;
		}
    }
	if(bUpdate) {
	    while(RemoveList.GetSize() != 0) {
	        m_pIBreakPoint->ResetBP(CComBSTR(RemoveList[0]->m_strInstanceName), 
							        RemoveList[0]->m_PosQual, 
                                    RemoveList[0]->m_lX, 
                                    RemoveList[0]->m_lY,
                                    RemoveList[0]->m_lCX, 
                                    RemoveList[0]->m_lCY
            );
		    RemoveList.RemoveAt(0);
	    };
        EraseList();
		ShowBPList(iListItem);
	};
	bHandled = TRUE;
	return(S_OK);
}

LRESULT CBreakPointView::OnClickedRemoveAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // to avoid handling of the m_Breakpoints array during deletion use a 
    // additional temp array RemoveList
	int	iListItemCount;
	int iListItem;
    CArray<BPListInfo*, BPListInfo*> RemoveList;
	iListItemCount = ListView_GetItemCount(m_BPListHWnd); 

	for(iListItem = 0; iListItem < iListItemCount; ++iListItem) {
        RemoveList.Add(m_Breakpoints[iListItem]);
    }
	while(RemoveList.GetSize() != 0) {
	    m_pIBreakPoint->ResetBP(CComBSTR(RemoveList[0]->m_strInstanceName), 
							    RemoveList[0]->m_PosQual, 
                                RemoveList[0]->m_lX, 
                                RemoveList[0]->m_lY,
                                RemoveList[0]->m_lCX, 
                                RemoveList[0]->m_lCY
        );
		RemoveList.RemoveAt(0);
	};
    // now update the BPView
    m_pIBreakPoint->UpdateAllBPs(CComBSTR());
	bHandled = TRUE;
	return 0;
}

LRESULT CBreakPointView::OnItemchangedBPList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if(m_bNoUpdate)
    {
        bHandled = TRUE;
        return(0);
    }

    NM_LISTVIEW*    pNMListview = (NM_LISTVIEW*)pnmh;
    m_iActItemIndex = pNMListview->iItem;

    if(pNMListview->uChanged == LVIF_STATE) 
    {
	    if((m_iActItemIndex > -1) && (m_iActItemIndex < m_Breakpoints.GetSize())) 
        {
            UINT    uiStateNew = pNMListview->uNewState >> 12;
            UINT    uiStateOld = pNMListview->uOldState >> 12;

            CComBSTR sInstanceName(m_Breakpoints[m_iActItemIndex]->m_strInstanceName);

            switch(uiStateNew)
            {
            case 1:     // uncheck
                if((m_Breakpoints[m_iActItemIndex]->m_BPState == BPM_BPACTIVE) ||
                    (m_Breakpoints[m_iActItemIndex]->m_BPState == BPM_BPACTIVEUNSET))
                {
	                m_pIBreakPoint->DeactivateBP(sInstanceName, 
                                            m_Breakpoints[m_iActItemIndex]->m_PosQual, 
                                            m_Breakpoints[m_iActItemIndex]->m_lX, 
                                            m_Breakpoints[m_iActItemIndex]->m_lY,
                                            m_Breakpoints[m_iActItemIndex]->m_lCX, 
                                            m_Breakpoints[m_iActItemIndex]->m_lCY
                    );
                }
                break;
            case 2:     // check
                if(m_Breakpoints[m_iActItemIndex]->m_BPState == BPM_BPINACTIVE) 
                {
	                m_pIBreakPoint->ActivateBP(sInstanceName, 
                                                m_Breakpoints[m_iActItemIndex]->m_PosQual, 
                                                m_Breakpoints[m_iActItemIndex]->m_lX, 
                                                m_Breakpoints[m_iActItemIndex]->m_lY,
                                                m_Breakpoints[m_iActItemIndex]->m_lCX, 
                                                m_Breakpoints[m_iActItemIndex]->m_lCY
                    );
                } 
                else if (m_Breakpoints[m_iActItemIndex]->m_BPState == BPM_NOBP) 
                {
            		ListView_SetCheckState(m_BPListHWnd, m_iActItemIndex, FALSE);
				}
                break;
            }
        }
    }
    
    bHandled = TRUE;
	return(0);
}

LRESULT CBreakPointView::OnDblclkBpList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if(m_iActItemIndex != -1) {
		GotoSelected ();
/*  code to toggle the activation of a selected BP
        CComBSTR sInstanceName(m_Breakpoints[m_iActItemIndex]->m_strInstanceName);
        BOOL bCheck = Button_GetCheck(m_ActivateBoxHWnd);
        if(bCheck) {
	        m_pIBreakPoint->DeactivateBP(sInstanceName, 
                                        m_Breakpoints[m_iActItemIndex]->m_Qualifier, 
                                        m_Breakpoints[m_iActItemIndex]->m_lXPos, 
                                        m_Breakpoints[m_iActItemIndex]->m_lYPos);
            Button_SetCheck(m_ActivateBoxHWnd, FALSE);
        } else {
	        m_pIBreakPoint->ActivateBP(sInstanceName, 
                                        m_Breakpoints[m_iActItemIndex]->m_Qualifier, 
                                        m_Breakpoints[m_iActItemIndex]->m_lXPos, 
                                        m_Breakpoints[m_iActItemIndex]->m_lYPos);
            Button_SetCheck(m_ActivateBoxHWnd, TRUE);
        }
*/
    }
    bHandled = TRUE;
	return 0;
}

LRESULT CBreakPointView::OnClickedGoto(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	GotoSelected ();
	bHandled = TRUE;
	return(S_OK);
}

void CBreakPointView::GotoSelected ()
{
	int	iListItemCount;
	int iListItem;

	if(ListView_GetSelectedCount(m_BPListHWnd) <= 0) {
        return;
    }
	iListItemCount = ListView_GetItemCount(m_BPListHWnd); 
	for(iListItem = 0; iListItem < iListItemCount; ++iListItem) {
		if(ListView_GetItemState(m_BPListHWnd, iListItem, LVIS_SELECTED)) {
            m_pIBreakPoint->ShowBP(CComBSTR (m_Breakpoints[iListItem]->m_strInstanceName), m_Breakpoints[iListItem]->m_lNumber);
			break;
		}
    }
}


LRESULT CBreakPointView::OnContextMenu (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());

    PopupContextMenu (LOWORD (lParam), HIWORD (lParam));

	bHandled = TRUE;
	return (0);
}


LRESULT CBreakPointView::OnContextMenuCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CPoint point (0, 0);


    // Generally want to handle two cases in command handlers.
    //
    // 1) hWndCtl == NULL && wNotifyCode == 1 means the command resulted from an accelerator keystroke
    //
    // 2) hWndCtl != NULL && wNotifyCode == BN_CLICKED means the command resulted from a button click

    if (!(hWndCtl == NULL && wNotifyCode == 1 || hWndCtl != NULL && wNotifyCode == BN_CLICKED))
    {
        bHandled = FALSE;
        return (0);
    };

    ::ClientToScreen (m_hWnd, &point);
    PopupContextMenu ((WORD)point.x, (WORD)point.y);

    bHandled = TRUE;
    return (0);
}


BOOL CBreakPointView::PreTranslateAccelerator(LPMSG pMsg, HRESULT &hRet)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_hAccelerators != NULL && ::TranslateAccelerator (m_hWnd, m_hAccelerators, pMsg))
    {
        hRet = S_OK;
        return (TRUE);
    };

    return (CComCompositeControl<CBreakPointView>::PreTranslateAccelerator (pMsg, hRet));
}


void CBreakPointView::PopupContextMenu (WORD wX, WORD wY)
{
	CMenu menu;
	CMenu *pPopup;
	bool bHaveBPs=false;
	bool bHaveSelectedBPs=false;
	bool bHaveSelectedSPs=false;
	int iSelectedCount;
	int iCount=0;
	int iIndex;

    if (!menu.LoadMenu (IDR_BPVIEW_POPUP))
	{
		return;
	};

	pPopup = menu.GetSubMenu (0);
	ASSERT (pPopup != NULL);

	iSelectedCount = ListView_GetSelectedCount (m_BPListHWnd);
	iCount = ListView_GetItemCount (m_BPListHWnd); 

	for (iIndex = 0; iIndex < iCount; ++iIndex)
	{
		if (m_Breakpoints[iIndex]->m_BPState != BPM_NOBP)
		{
			bHaveBPs = true;
		};

		if (ListView_GetItemState (m_BPListHWnd, iIndex, LVIS_SELECTED))
		{
			if (m_Breakpoints[iIndex]->m_StopState == BPM_STOPPED)
			{
				bHaveSelectedSPs = true;
			};

			if (m_Breakpoints[iIndex]->m_BPState != BPM_NOBP)
			{
				bHaveSelectedBPs = true;
			};
		};
	};

	pPopup->EnableMenuItem (IDM_GOTO_SOURCE_CODE, iSelectedCount == 1 ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_REMOVE_BREAKPOINT, bHaveSelectedBPs ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_REMOVE_ALL_BREAKPOINTS, bHaveBPs ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_TOGGLE_BREAKPOINT_ENABLED, iSelectedCount == 1 && bHaveSelectedBPs ? MF_ENABLED : MF_GRAYED);
	pPopup->EnableMenuItem (IDM_CONTINUE_TASK, bHaveSelectedSPs ? MF_ENABLED : MF_GRAYED);

	::TrackPopupMenu (pPopup->m_hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, wX, wY, 0, m_hWnd, NULL);
}


LRESULT CBreakPointView::OnRemoveBreakpoint (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());

	if (ListView_GetSelectedCount (m_BPListHWnd) > 0)
	{
		int iCount;
		int iIndex;

		iCount = ListView_GetItemCount (m_BPListHWnd); 

		for (iIndex = 0; iIndex < iCount; ++iIndex)
		{
			if (ListView_GetItemState (m_BPListHWnd, iIndex, LVIS_SELECTED))
			{
				BPListInfo *pBPInfo;

				pBPInfo = m_Breakpoints[iIndex];

				if (pBPInfo->m_BPState != BPM_NOBP)
				{
					m_pIBreakPoint->ResetBP (CComBSTR (pBPInfo->m_strInstanceName), 
                        pBPInfo->m_PosQual, pBPInfo->m_lX, pBPInfo->m_lY, pBPInfo->m_lCX, pBPInfo->m_lCY);
				};
			};
		};
	};

	return (0);
}

LRESULT CBreakPointView::OnToggleBreakpointEnabled (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());

	if (ListView_GetSelectedCount (m_BPListHWnd) > 0)
	{
		int iCount;
		int iIndex;

		iCount = ListView_GetItemCount (m_BPListHWnd); 

		for (iIndex = 0; iIndex < iCount; ++iIndex)
		{
			if (ListView_GetItemState (m_BPListHWnd, iIndex, LVIS_SELECTED))
			{
				BPListInfo *pBPInfo;

				pBPInfo = m_Breakpoints[iIndex];

				if (pBPInfo->m_BPState == BPM_BPINACTIVE)
				{
					m_pIBreakPoint->ActivateBP (CComBSTR (pBPInfo->m_strInstanceName), 
                        pBPInfo->m_PosQual, pBPInfo->m_lX, pBPInfo->m_lY, pBPInfo->m_lCX, pBPInfo->m_lCY);
				}
				else
				{
					m_pIBreakPoint->DeactivateBP (CComBSTR (pBPInfo->m_strInstanceName), 
                        pBPInfo->m_PosQual, pBPInfo->m_lX, pBPInfo->m_lY, pBPInfo->m_lCX, pBPInfo->m_lCY);
				};
			};
		};
	};

	return (0);
}

LRESULT CBreakPointView::OnContinueTask (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());

	ContinueSelected (false);
	return (0);
}

void CBreakPointView::ContinueSelected (bool bSingleStep)
{
	if (ListView_GetSelectedCount (m_BPListHWnd) > 0)
	{
		int iCount;
		int iIndex;

		iCount = ListView_GetItemCount (m_BPListHWnd); 

		for (iIndex = 0; iIndex < iCount; ++iIndex)
		{
			if (ListView_GetItemState (m_BPListHWnd, iIndex, LVIS_SELECTED))
			{
				BPListInfo *pBPInfo;

				pBPInfo = m_Breakpoints[iIndex];

				if (pBPInfo->m_StopState == BPM_STOPPED)
				{
					m_pIBreakPoint->ContinueTask (CComBSTR (pBPInfo->m_strInstanceName), bSingleStep ? 1: 0);
				};
			};
		};
	};
}


HRESULT CBreakPointView::OnRemoveBP (LPCTSTR pszInstanceName, IBreakPointInfo *pBPInfo)
{
    HRESULT hr;
    int iIndex;

    hr = GetListInfo (pszInstanceName, pBPInfo, false, iIndex);
    if (FAILED (hr))
    {
        return (hr);
    };

    if (hr == S_OK)
    {
        ASSERT(iIndex != -1);
        BPListInfo *pListInfo;

        pListInfo = m_Breakpoints[iIndex];
	    ASSERT (pListInfo != NULL);
	    if (pListInfo == NULL)
	    {
		    return (E_FAIL);
	    };

        if (pListInfo->m_StopState == BPM_STOPPED)
        {
            pListInfo->m_BPState = BPM_NOBP;
            RefreshItemState (iIndex, pListInfo);
        }
        else
        {
            RemoveItem (iIndex);
        }
    }

    return (S_OK);
}


HRESULT CBreakPointView::OnActivateBP (LPCTSTR pszInstanceName, IBreakPointInfo *pBPInfo, BOOL bStopped, BOOL bUnset)
{
	HRESULT hr;
    int iIndex;
	bool bNewBP;
    BPListInfo *pListInfo;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	hr = GetListInfo (pszInstanceName, pBPInfo, true, iIndex);
	if (FAILED (hr))
	{
		return (hr);
	};

    ASSERT (iIndex != -1);
	bNewBP = hr == S_FALSE;

	pListInfo = m_Breakpoints[iIndex];
	ASSERT (pListInfo != NULL);
	if (pListInfo == NULL)
	{
		return (E_FAIL);
	};

	if (bStopped)
    {
	    pListInfo->m_StopState = BPM_STOPPED;
    }
    else
    {
		pListInfo->m_BPState = bUnset ? BPM_BPACTIVEUNSET : BPM_BPACTIVE;
    };

    if (bNewBP)
    {
        AddItem (iIndex, pListInfo);
    }
    else
    {
        RefreshItemState (iIndex, pListInfo);
    };

    return (S_OK);
}


HRESULT CBreakPointView::OnDeactivateBP (LPCTSTR pszInstanceName, IBreakPointInfo *pBPInfo)
{
	HRESULT hr;
    int iIndex;
	bool bNewBP;
    BPListInfo *pListInfo;

	hr = GetListInfo (pszInstanceName, pBPInfo, true, iIndex);
	if (FAILED (hr))
	{
		return (hr);
	};

    ASSERT (iIndex != -1);
	bNewBP = hr == S_FALSE;

	pListInfo = m_Breakpoints[iIndex];
	ASSERT (pListInfo != NULL);
	if (pListInfo == NULL)
	{
		return (E_FAIL);
	};

	pListInfo->m_BPState = BPM_BPINACTIVE;

    if (bNewBP)
    {
        AddItem (iIndex, pListInfo);
    }
    else
    {
        RefreshItemState (iIndex, pListInfo);
    };

    return (S_OK);
}


HRESULT CBreakPointView::OnContinueBP (LPCTSTR pszInstanceName, IBreakPointInfo *pBPInfo)
{
    HRESULT hr;
    int iIndex;
    BPListInfo *pListInfo;

    hr = GetListInfo (pszInstanceName, pBPInfo, false, iIndex);
    if (FAILED (hr))
    {
        return (hr);
    };

    if(hr == S_OK) 
    {
        ASSERT (iIndex != -1);
        pListInfo = m_Breakpoints[iIndex];
        ASSERT (pListInfo != NULL);
        if (pListInfo == NULL)
        {
            return (E_FAIL);
        };

	    if (pListInfo->m_BPState == BPM_NOBP)
        {
    	    RemoveItem (iIndex);
	    }
        else
        {
		    pListInfo->m_StopState = BPM_CONTINUED;
            RefreshItemState (iIndex, pListInfo);
	    };
    }

    return (S_OK);
}


HRESULT CBreakPointView::GetListInfo (CString strInstanceName, IBreakPointInfo* pBPInfo, bool bCreate, int &iIndex)
{
	HRESULT             hr;
	long	            lX;
	long                lY;
	long	            lCX;
	long                lCY;
    EDP_POS_QUALIFIER   posQual;
    CString             strInstanceFromList;
	BPListInfo*         pListInfo;

    //set index invalid
    iIndex = -1;
    strInstanceName.MakeLower();


	hr = pBPInfo->get_X(&lX);
    if(FAILED(hr)) {
        return(hr);
    }

	hr = pBPInfo->get_Y(&lY);
    if(FAILED(hr)) {
        return(hr);
    }

	hr = pBPInfo->get_CX(&lCX);
    if(FAILED(hr)) {
        return(hr);
    }

	hr = pBPInfo->get_CY(&lCY);
    if(FAILED(hr)) {
        return(hr);
    }


	hr = pBPInfo->get_PosQualifier(&posQual);
	if(FAILED(hr)) {
		return(hr);
	}

    for(iIndex = 0; iIndex < m_Breakpoints.GetSize (); ++iIndex) 
    {
        pListInfo = m_Breakpoints[iIndex];
        strInstanceFromList = pListInfo->m_strInstanceName;
        strInstanceFromList.MakeLower();

        if(strInstanceFromList == strInstanceName && pListInfo->m_PosQual==posQual) 
        {
            if (posQual == EDP_GRBEGIN || posQual == EDP_GREND)
            {
				// BP already in list
                return (S_OK);
            }
            else if(posQual == EDP_STLINE)
            {
			    if (pListInfo->m_lY == lY)
                {
				    // BP already in list
				    return (S_OK);
                }
			}
            else if(posQual == EDP_GRBOX)
            {
			    if (pListInfo->m_lX == lX && pListInfo->m_lY == lY)
                {
				    // BP already in list
				    return (S_OK);
                }
			}
            else if(posQual == EDP_GRSTBOX)
            {
			    if (pListInfo->m_lX == lX && pListInfo->m_lY == lY && pListInfo->m_lCY == lCY)
                {
				    // BP already in list
				    return (S_OK);
                }
			}
            else if(posQual == EDP_GRREGION)
            {
			    if (pListInfo->m_lX == lX && pListInfo->m_lY == lY && pListInfo->m_lCX == lCX && pListInfo->m_lCY == lCY)
                {
				    // BP already in list
				    return (S_OK);
                }
			}
            else
            {
                assert(!"bad BP PosQualifier");
            }

		}
	}

    //set the index invalid
    iIndex = -1;

    if (bCreate)
    {
	    // create new entry
	    pListInfo = new BPListInfo;
	    pListInfo->m_BPState = BPM_NOBP;
	    pListInfo->m_StopState = BPM_CONTINUED;
	    pListInfo->m_strInstanceName = strInstanceName;
        pListInfo->m_PosQual = posQual;
	    pListInfo->m_lX      = lX;
	    pListInfo->m_lY      = lY;
	    pListInfo->m_lCX     = lCX;
	    pListInfo->m_lCY     = lCY;
	    hr = pBPInfo->get_Number(&pListInfo->m_lNumber);
	    if(FAILED(hr)) {
		    delete(pListInfo);
		    return(hr);
	    }

        //Add returns index of added item.
        iIndex = m_Breakpoints.Add(pListInfo);
    };

	return (S_FALSE);
}

STDMETHODIMP CBreakPointView::AttachToBPMan(LPUNKNOWN pUnk)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
    assert(m_pIBreakPoint == NULL);
    if(m_pIBreakPoint != NULL) {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: already attached\n"));
        return(E_FAIL);
    }
	assert(pUnk != NULL);
    if(pUnk == NULL) {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: try to attach to NULL pointer\n"));
		return(E_FAIL);
    }
    
    // clear the BPList
    m_Breakpoints.RemoveAll();
	if(IsWindow()) {
		DisplayColumnHeadings ();
	}
    CComQIPtr<IBreakPoint, &IID_IBreakPoint> pBPMan(pUnk);   
    if(pBPMan != NULL) {
        m_pIBreakPoint = pBPMan;
	    HRESULT hr = AtlAdvise(m_pIBreakPoint, GetUnknown(), IID__IBreakPointNotify, &m_dwBPNotifyCookie);
	    if(FAILED (hr)) {
		    FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to advise breakpoint manager FrontEnd (hr=0x%08lx)\n"), (long)hr);
		    return (hr);
	    };
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: attach to BPMan succeeded\n"));
        // now update the BPView
        m_pIBreakPoint->UpdateAllBPs(CComBSTR());
        return(S_OK);
    }
	return(S_FALSE);
}

STDMETHODIMP CBreakPointView::DetachFromBPMan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
    assert(m_dwBPNotifyCookie != NULL);
    if(m_dwBPNotifyCookie == NULL) {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: there is no attachement\n"));
        return(E_FAIL);
    }

	HRESULT hr = AtlUnadvise (m_pIBreakPoint, IID__IBreakPointNotify, m_dwBPNotifyCookie);
	if(FAILED (hr)) {
		FC_DEBUGPRINT1(_T ("CEBPMAN> Failed to unadvise breakpoint manager FrontEnd (hr=0x%08lx)\n"), (long)hr);
	}
	m_dwBPNotifyCookie = NULL;
    m_pIBreakPoint = NULL;
	FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: detach from BPMan succeeded\n"));
	return S_OK;
}


// _IBreakPointNotify
STDMETHODIMP CBreakPointView::OnBPStateChanged(tagBPM_BREAKPOINT_STATE newState, BSTR sInstance, IUnknown* pUnk, BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    assert (pUnk != NULL);
    if(pUnk == NULL) {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: try notify using a NULL pointer\n"));
	    return(E_POINTER);
    }

    CComQIPtr<IBreakPointInfo, &IID_IBreakPointInfo> pBPInfo(pUnk);   
    if(pBPInfo != NULL) {
        switch(newState) {
            case BPM_NOBP:
                hr = OnRemoveBP(CString(sInstance), pBPInfo);
	            break;
            case BPM_BPACTIVE:
                hr = OnActivateBP(CString(sInstance), pBPInfo);
	            break;
            case BPM_BPACTIVEUNSET:
                hr = OnActivateBP(CString(sInstance), pBPInfo, FALSE, TRUE);
	            break;
            case BPM_BPINACTIVE:
                hr = OnDeactivateBP(CString(sInstance), pBPInfo);
	            break;
            default:
        		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: unknown notification\n"));
	            hr = S_FALSE;
	            break;
        }
    } else {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: try notify without IID_IBreakPointInfo-object\n"));
    }
    return(hr);
}

STDMETHODIMP CBreakPointView::OnSPStateChanged(tagBPM_STOPPOINT_STATE newState, BSTR sInstance, IUnknown * pUnk, BOOL bUpdate)
{
    HRESULT hr = E_FAIL;
    IBreakPointInfo *pBPInfo;

    assert (pUnk != NULL);
    if(pUnk == NULL) {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: try notify using a NULL pointer\n"));
	    return(E_POINTER);
    }

    hr = pUnk->QueryInterface (IID_IBreakPointInfo, (LPVOID *)&pBPInfo);
    if (FAILED (hr))
        return (hr);

    ASSERT (pBPInfo != NULL);
    if (pBPInfo == NULL)
    {
		FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: try notify without IID_IBreakPointInfo-object\n"));
        return (E_FAIL);
    };


    // Must not do UI concerning actions in here because there
    // is the danger of entering a message loop which may demand
    // a continue or single step action which is rejected by the
    // control while inside this notification. Anyway should re-
    // turn as fast as possible.

    PostMessage (DELAYED_SP_STATE_CHANGE, (WPARAM)newState, (LPARAM)pBPInfo);

    return (S_OK);
}


LRESULT CBreakPointView::OnDelayedSPStateChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    HRESULT hr;
    tagBPM_STOPPOINT_STATE newState=(tagBPM_STOPPOINT_STATE)wParam;
    IBreakPointInfo *pBPInfo=(IBreakPointInfo *)lParam;
    CComBSTR sInstance;

    bHandled = TRUE;

    hr = pBPInfo->get_Instance (&sInstance);
    if (FAILED (hr))
    {
        pBPInfo->Release ();
        return (0);
    };

    switch(newState) {
        case BPM_STOPPED:
            OnActivateBP(CString(sInstance), pBPInfo, TRUE);
	        break;
        case BPM_CONTINUED:
            // now remove BP from list
            OnContinueBP(CString(sInstance), pBPInfo);
	        break;
        default:
        	FC_DEBUGPRINT(_T ("CEBPMAN> breakpoint manager FrontEnd: unknown notification\n"));
	        break;
    }
    
    pBPInfo->Release ();

    return (0);
}


STDMETHODIMP CBreakPointView::OnShowBP(BSTR sInstance, IUnknown * pUnk)
{
	return (S_OK);
}

LRESULT CBreakPointView::OnCommandHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	lParam = HID_BASE_RESOURCE + IDD_BREAKPOINTVIEW;

	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
	{
		pApp->WinHelp(lParam);
		return TRUE;
	}

	return FALSE;
}

LRESULT CBreakPointView::OnToolTipNfy (int id, LPNMHDR pNMHDR, BOOL &bHandled)
{
    TOOLTIPTEXT *pTTT=(TOOLTIPTEXT *)pNMHDR;
    HWND hWnd;

    bHandled = FALSE;

	if (pTTT == NULL)
    {
		return (0);
	};
   
	if (!(pTTT->uFlags & TTF_IDISHWND))
    {
        return (0);
    };

	// idFrom is actually the HWND of the tool
	hWnd = (HWND)pNMHDR->idFrom;

    if (hWnd != m_RemoveBtnHWnd && hWnd != m_RemoveAllBtnHWnd && hWnd != m_GotoBtnHWnd)
    {
        return (0);
    };

    ::SendMessage (hWnd, WM_GETTEXT, sizeof (pTTT->szText) / sizeof (pTTT->szText[0]), (LPARAM)pTTT->szText);
    //_tcsncpy (pTTT->szText, ???, ARRAY_LEN (pTTT->szText));
    pTTT->hinst = NULL;
    pTTT->lpszText = pTTT->szText;

    bHandled = TRUE;
    return (0);
}

