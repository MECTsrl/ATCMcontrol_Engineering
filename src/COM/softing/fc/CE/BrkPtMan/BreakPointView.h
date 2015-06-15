
#ifndef __BREAKPOINTVIEW_H_
#define __BREAKPOINTVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


// BreakPointView.h : Declaration of the CBreakPointView
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "BrkPtMan.h"
#include "BreakPointMan.h"

#include "afxpriv.h"

/////////////////////////////////////////////////////////////////////////////
// CBreakPointView

#define DELAYED_SP_STATE_CHANGE (WM_USER + 1000)

class ATL_NO_VTABLE CBreakPointView : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IBreakPointView, &IID_IBreakPointView, &LIBID_BRKPTMANLib>,
	public CComCompositeControl<CBreakPointView>,
	public IPersistStreamInitImpl<CBreakPointView>,
	public IOleControlImpl<CBreakPointView>,
	public IOleObjectImpl<CBreakPointView>,
	public IOleInPlaceActiveObjectImpl<CBreakPointView>,
	public IViewObjectExImpl<CBreakPointView>,
	public IOleInPlaceObjectWindowlessImpl<CBreakPointView>,
	public IConnectionPointContainerImpl<CBreakPointView>,
	public IPersistStorageImpl<CBreakPointView>,
	public ISpecifyPropertyPagesImpl<CBreakPointView>,
	public IQuickActivateImpl<CBreakPointView>,
	public IDataObjectImpl<CBreakPointView>,
	public IProvideClassInfo2Impl<&CLSID_BreakPointView, NULL, &LIBID_BRKPTMANLib>,
	public IPropertyNotifySinkCP<CBreakPointView>,
	public _IBreakPointNotify, 
	public CComCoClass<CBreakPointView, &CLSID_BreakPointView>
{
public:
    CBreakPointView()
	{
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
        m_dwBPNotifyCookie = NULL;
        m_pIBreakPoint = NULL;
		m_bNoUpdate = FALSE;
        m_ToolTipHWnd = NULL;
	}

    virtual ~CBreakPointView();


DECLARE_REGISTRY_RESOURCEID(IDR_BREAKPOINTVIEW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBreakPointView)
	COM_INTERFACE_ENTRY(IBreakPointView)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(_IBreakPointNotify)
END_COM_MAP()

BEGIN_PROP_MAP(CBreakPointView)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CBreakPointView)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CBreakPointView)
	CHAIN_MSG_MAP(CComCompositeControl<CBreakPointView>)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	COMMAND_HANDLER(IDC_REMOVE, BN_CLICKED, OnClickedRemove)
	COMMAND_HANDLER(IDC_REMOVEALL, BN_CLICKED, OnClickedRemoveAll)
	NOTIFY_HANDLER(IDC_BPLIST, LVN_ITEMCHANGED, OnItemchangedBPList)
	NOTIFY_HANDLER(IDC_BPLIST, NM_DBLCLK, OnDblclkBpList)
	COMMAND_HANDLER(IDC_GOTO, BN_CLICKED, OnClickedGoto)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	COMMAND_HANDLER(IDM_REMOVE_BREAKPOINT, BN_CLICKED, OnClickedRemove)
	COMMAND_HANDLER(IDM_REMOVE_ALL_BREAKPOINTS, BN_CLICKED, OnClickedRemoveAll)
	COMMAND_HANDLER(IDM_TOGGLE_BREAKPOINT_ENABLED, BN_CLICKED, OnToggleBreakpointEnabled)
	COMMAND_HANDLER(IDM_GOTO_SOURCE_CODE, BN_CLICKED, OnClickedGoto)
	COMMAND_HANDLER(IDM_CONTINUE_TASK, BN_CLICKED, OnContinueTask)
	MESSAGE_HANDLER(WM_COMMANDHELP, OnCommandHelp)
	NOTIFY_HANDLER(IDD_BREAKPOINTVIEW, TTN_NEEDTEXT, OnToolTipNfy)
    MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
	COMMAND_ID_HANDLER(IDM_CONTEXT_MENU, OnContextMenuCommand)
    MESSAGE_HANDLER(DELAYED_SP_STATE_CHANGE, OnDelayedSPStateChange)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CBreakPointView)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CBreakPointView>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IBreakPointView
private:
    HWND    m_BPListHWnd;
    HWND    m_RemoveBtnHWnd;
    HWND    m_RemoveAllBtnHWnd;
	HWND    m_GotoBtnHWnd;
    HWND    m_ToolTipHWnd;

    RECT    m_rBPList;
    RECT    m_rRemoveBtn;
    RECT    m_rRemoveAllBtn;
	RECT    m_rGotoBtn;

    CImageList  m_ImageList;

    HACCEL m_hAccelerators;

    NONCLIENTMETRICS    m_SysMetrics;
    int     m_iCtrlHeight;

    struct BPListInfo {
        CString           m_strInstanceName;
	    EDP_POS_QUALIFIER m_PosQual;
	    long              m_lX;
	    long              m_lY;
	    long              m_lCX;
	    long              m_lCY;
        BPM_BRKPT_STATE   m_BPState;
        BPM_STPPT_STATE   m_StopState;
		long              m_lNumber;
    };

	BOOL m_bNoUpdate;

    CArray<BPListInfo*, BPListInfo*> m_Breakpoints;
    void DisplayColumnHeadings(void);
	void EraseColumns(void);
	void EraseList(void);
	BOOL AddColumn(
		LPCTSTR strItem,int nItem,int nSubItem = -1,
		int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
		int nFmt = LVCFMT_LEFT);
    BOOL AddItemRaw(int nItem, int nSubItem, LPCTSTR strItem, int nImageIndex = -1);
	void AddItem(int iIndex, BPListInfo* pListInfo);
    void RefreshItemState (int iIndex, BPListInfo *pListInfo);
    int GetItemImage (BPListInfo *pListInfo, bool &bStoppedOverlay);
	BOOL RemoveItem(int nItem);
    void RemoveAllItems();
    void BuildPosText(BPListInfo* pListInfo, CString& strPosition);
    void ShowBPList(int iSelIndex = -1);
	void RemoveBP(int iItem);

    CComPtr<IBreakPoint> m_pIBreakPoint;
    DWORD   m_dwBPNotifyCookie;

    int m_iActItemIndex;
    void CalcRects(const RECT* pClientRect);

    HRESULT OnRemoveBP(LPCTSTR pszInstanceName, IBreakPointInfo* pBPInfo);
    HRESULT OnActivateBP(LPCTSTR pszInstanceName, IBreakPointInfo* pBPInfo, BOOL bStopped = FALSE, BOOL bUnset = FALSE);
    HRESULT OnDeactivateBP(LPCTSTR pszInstanceName, IBreakPointInfo* pBPInfo);
    HRESULT OnContinueBP(LPCTSTR pszInstanceName, IBreakPointInfo* pBPInfo);

	HRESULT GetListInfo (CString strInstanceName, IBreakPointInfo* pBPInfo, bool bCreate, int &iIndex);
	void GotoSelected ();
	void ContinueSelected (bool bSingleStep);

    void AddTool (HWND hwndTool, UINT uiText);
    LRESULT OnSetCursor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnToolTipNfy (int id, LPNMHDR pNMHDR, BOOL &bHandled);

    void PopupContextMenu (WORD wX, WORD wY);

public:
	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT &hRet);

    HWND Create(HWND hWndParent, RECT& rcPos, LPARAM dwInitParam = NULL); 
    HRESULT FinalConstruct();
	void FinalRelease ();

    STDMETHOD(AttachToBPMan)(LPUNKNOWN pUnk);
	STDMETHOD(DetachFromBPMan)();

	enum { IDD = IDD_BREAKPOINTVIEW };
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnClickedRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedRemoveAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnItemchangedBPList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblclkBpList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClickedGoto(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRemoveBreakpoint (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnToggleBreakpointEnabled (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnContinueTask (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCommandHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnContextMenuCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnDelayedSPStateChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

// _IBreakPointNotify
	STDMETHOD(OnBPStateChanged)(tagBPM_BREAKPOINT_STATE newState, BSTR sInstance, IUnknown* pUnk, BOOL bUpdate);
    STDMETHOD(OnSPStateChanged)(tagBPM_STOPPOINT_STATE newState, BSTR sInstance, IUnknown* pUnk, BOOL bUpdate);
	STDMETHOD(OnShowBP)(BSTR sInstance, IUnknown *pUnk);
};

#endif // __BREAKPOINTVIEW_H_


