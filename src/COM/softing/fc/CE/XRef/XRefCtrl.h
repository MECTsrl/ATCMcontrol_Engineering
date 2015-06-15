#ifndef __XREFCTRL_H_
#define __XREFCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "cemsg.h"
#include "XRefCP.h"


//------------------------------------------------------------------*
/**
 * CXRefCtrl
 *
 * @see             
*/
class ATL_NO_VTABLE CXRefCtrl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXRefCtrl, &IID_IXRefCtrl, &LIBID_XREFLib>,
	public CComCompositeControl<CXRefCtrl>,
	public IPersistStreamInitImpl<CXRefCtrl>,
	public IOleControlImpl<CXRefCtrl>,
	public IOleObjectImpl<CXRefCtrl>,
	public IOleInPlaceActiveObjectImpl<CXRefCtrl>,
	public IViewObjectExImpl<CXRefCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CXRefCtrl>,
	public IConnectionPointContainerImpl<CXRefCtrl>,
	public IPersistStorageImpl<CXRefCtrl>,
	public ISpecifyPropertyPagesImpl<CXRefCtrl>,
	public IQuickActivateImpl<CXRefCtrl>,
	public IDataObjectImpl<CXRefCtrl>,
	//public IProvideClassInfo2Impl<&CLSID_XRefCtrl, &IID__IXRefCtrlEvents, &LIBID_XREFLib>,
	public IPropertyNotifySinkCP<CXRefCtrl>,
	public CComCoClass<CXRefCtrl, &CLSID_XRefCtrl>,
	public CProxy_IXRefCtrlEvents< CXRefCtrl >,
	public CProxy_ICEMessageEvent< CXRefCtrl >
{
public:
	CXRefCtrl();
	~CXRefCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_XREFCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXRefCtrl)
	COM_INTERFACE_ENTRY(IXRefCtrl)
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
	//COM_INTERFACE_ENTRY(IProvideClassInfo)
	//COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_PROP_MAP(CXRefCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CXRefCtrl)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(IID__IXRefCtrlEvents)
	CONNECTION_POINT_ENTRY(IID__ICEMessageEvent)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CXRefCtrl)
	CHAIN_MSG_MAP(CComCompositeControl<CXRefCtrl>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH, BN_CLICKED, OnClickedButton_search)
	COMMAND_HANDLER(IDC_BUTTON_PRINT, BN_CLICKED, OnClickedButton_print)	// XREFPRINT 20.07.05 SIS
	COMMAND_HANDLER(IDC_LIST_NAMES, LBN_SELCHANGE, OnSelchangeList_names)
	COMMAND_HANDLER(IDC_LIST_REFERENCES, LBN_DBLCLK, OnDblclkList_references)
	COMMAND_HANDLER(IDC_ACCESS_READ, BN_CLICKED, OnClickedAccess_read)
	COMMAND_HANDLER(IDC_ACCESS_WRITE, BN_CLICKED, OnClickedAccess_write)
    MESSAGE_HANDLER(WM_HELP, OnShowHelp)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CXRefCtrl)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CXRefCtrl>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IXRefCtrl
public:
	HRESULT ShowMessage(HRESULT hrMessageCode);
	STDMETHOD(InitXRef)();
	STDMETHOD(SetActiveQuery)(/*[in,string]*/ BSTR sVariableName);
	STDMETHOD(ChangeRefPosition)(BOOL bForward);
	STDMETHOD(LinkXRef)();
	// OSAI feature print XRef 24.03.04 SIS >>
	STDMETHOD(GetPrintString)(/*[out,string]*/ BSTR* psContent, /*[in]*/ int iLineLength, /*[in]*/ int iMode);  // XREFPRINT 21.07.05 SIS
	// OSAI feature print XRef 24.03.04 SIS <<

	enum { IDD = IDD_XREFCTRL };

protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedButton_search(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedButton_print(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);	// XREFPRINT 20.07.05 SIS
	LRESULT OnSelchangeList_names(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDblclkList_references(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedAccess_read(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedAccess_write(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


protected:
	LRESULT FillReferences(void);
	LRESULT SearchDefinitions(void);
	LRESULT PrintSelection(void);		// XREFPRINT 20.07.05 SIS
	BOOL ShowReference(int iOffset);
	void UpdateXRefData();
	void ClearDialog();
	BOOL Link(void);
	CXref * m_XrefData;
	BOOL GetLinkDirectory(CString &sDirectory);
	void FillDummy(void);
	CComboBox	m_CBVariableScope;
	CComboBox	m_CBDeclScope;
	CComboBox	m_CBTypeScope;
	CComboBox	m_CBFileScope;
	CListBox	m_LBNameList;
	CListBox	m_LBReferenceList;
	CStatic		m_STHitText;
	CStatic		m_STAccess;
	CStatic		m_STFilter;	// XREFPRINT 21.07.05 SIS
	CButton		m_BUAccessRead;
	CButton		m_BUAccessWrite;

	// XREFPRINT 21.07.05 SIS >>
	CString		m_strLastSearchName;
	CString		m_strLastSearchScope;
	CString		m_strLastSearchType;
	CString		m_strLastSearchFile;
	BOOL		m_bLastSearchRead;
	BOOL		m_bLastSearchWrite;
	// XREFPRINT 21.07.05 SIS <<
};

#endif //__XREFCTRL_H_


