
#ifndef __POUVARSELCTRL_H_
#define __POUVARSELCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "PouVarSelection.h"

#include "VarSelDlg.h"
#include "PouSelDlg.h"
#include "PouVarSelData.h"
#include "CEMsg.h"

#include "PouVarSelectionCP.h"


//------------------------------------------------------------------*
/**
 * CPouVarSelCtrl: The PouVarSel ActiveX Control.
 * It contains a Tab Control and two dialog pages 
 * with the pou selection and the variable selection
 *
 * @see             
*/
class ATL_NO_VTABLE CPouVarSelCtrl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IPouVarSelCtrl, &IID_IPouVarSelCtrl, &LIBID_POUVARSELECTIONLib>,
	public CComControl<CPouVarSelCtrl>,
	public IPersistStreamInitImpl<CPouVarSelCtrl>,
	public IOleControlImpl<CPouVarSelCtrl>,
	public IOleObjectImpl<CPouVarSelCtrl>,
	public IOleInPlaceActiveObjectImpl<CPouVarSelCtrl>,
	public IViewObjectExImpl<CPouVarSelCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CPouVarSelCtrl>,
	public IConnectionPointContainerImpl<CPouVarSelCtrl>,
	public CComCoClass<CPouVarSelCtrl, &CLSID_PouVarSelCtrl>,
	public CProxy_IPouVarSelCtrlEvents< CPouVarSelCtrl >,
	public CProxy_ICEMessageEvent< CPouVarSelCtrl >
{
public:
	

DECLARE_REGISTRY_RESOURCEID(IDR_POUVARSELCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()


BEGIN_COM_MAP(CPouVarSelCtrl)
	COM_INTERFACE_ENTRY(IPouVarSelCtrl)
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
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()


BEGIN_PROP_MAP(CPouVarSelCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_CONNECTION_POINT_MAP(CPouVarSelCtrl)
CONNECTION_POINT_ENTRY(IID__IPouVarSelCtrlEvents)
CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()



BEGIN_MSG_MAP(CPouVarSelCtrl)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
    NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnSelChanged)
ALT_MSG_MAP(1)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)  
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocusTabControl)
END_MSG_MAP()


// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


public:
    CPouVarSelCtrl();
    ~CPouVarSelCtrl();
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    HRESULT IOleInPlaceObject_UIDeactivate();

	LRESULT OnSetFocusTabControl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)


// IPouVarSelCtrl
public:
	STDMETHOD(InitPouVarSelection)();
	STDMETHOD(ShowVarSelection)(/*[in, string]*/BSTR sPouName);
	STDMETHOD(ShowPouSelection)();
    STDMETHOD(SetVarSelectionHint)(/*[in, string]*/ BSTR sPouName,
                                   /*[in, string]*/ BSTR sVarNamePrefix,
                                   /*[in, string]*/ BSTR sDataType);
    STDMETHOD(LookupUserHelp)(/*[in, string]*/  BSTR sPou,
                              /*[out, string]*/ BSTR* psFileName);
    STDMETHOD(LookupHeader)(/*[in, string]*/  BSTR sPou,
                            /*[out, string]*/ BSTR* psHeaderFile);
    STDMETHOD(GetLRUPous)(/*[out]*/ SAFEARRAY** parrPous);
    STDMETHOD(GetPouXML)(/*[in, string]*/  BSTR szPouName, 
                         /*[out, string]*/ BSTR* pszPouDef);
    STDMETHOD(GetVarList)(/*[out]*/ SAFEARRAY** parrVars);
    STDMETHOD(GetPouList)(/*[out]*/ SAFEARRAY** parrPous);
    STDMETHOD(GetPouDefinition)(/*[in, string]*/  BSTR szPouName, 
                                /*[out, string]*/ BSTR* pszPouDef);
    STDMETHOD(GetPouMember)(/*[in, string]*/ BSTR szPouName, 
                            /*[out]*/ SAFEARRAY** parrMembers);
    STDMETHOD(ShowVarSelectionBox)(/*[in]*/LONG lXPos, /*[in]*/LONG lYPos,
                                   /*[in, string]*/BSTR szLine, /*[in]*/LONG lColumn,
                                   /*[in, string]*/BSTR szTypeHint,
                                   /*[in, string]*/BSTR szPouName,
                                   /*[out, string]*/BSTR* pszSelection,
                                   /*[out]*/LONG* plStartColumn,
                                   /*[out]*/LONG* plEndColumn);
    STDMETHOD(DisablePouVarSelection)(/*[in]*/BOOL bDisable);

	HRESULT OnDraw(ATL_DRAWINFO& di);


public:
    void ChangeSelectedTab(int sel);
    CPouVarSelData* GetPouVarSelData() {return &m_pouVarData;}
    CPouSelDlg*     GetPouSelDlg() {return &m_pouSelDlg;}
    CVarSelDlg*     GetVarSelDlg() {return &m_varSelDlg;}
    BOOL            IsDisabled() {return m_bDisabled;}


private:
    CContainedWindow m_ctlSysTabControl32;
    CPouSelDlg m_pouSelDlg;
    CVarSelDlg m_varSelDlg;
    CPouVarSelData m_pouVarData;
    BOOL m_bDisabled;
	
	void InitTab();
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif //__POUVARSELCTRL_H_


