
#ifndef __CALLSTACKVIEW_H_
#define __CALLSTACKVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#include "resource.h"       // main symbols
#include <atlctl.h>


/////////////////////////////////////////////////////////////////////////////
// CCallStackView
class ATL_NO_VTABLE CCallStackView : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<ICallStackView, &IID_ICallStackView, &LIBID_BRKPTMANLib>,
	public CComCompositeControl<CCallStackView>,
	public IPersistStreamInitImpl<CCallStackView>,
	public IOleControlImpl<CCallStackView>,
	public IOleObjectImpl<CCallStackView>,
	public IOleInPlaceActiveObjectImpl<CCallStackView>,
	public IViewObjectExImpl<CCallStackView>,
	public IOleInPlaceObjectWindowlessImpl<CCallStackView>,
	public IConnectionPointContainerImpl<CCallStackView>,
	public IPersistStorageImpl<CCallStackView>,
	public ISpecifyPropertyPagesImpl<CCallStackView>,
	public IQuickActivateImpl<CCallStackView>,
	public IDataObjectImpl<CCallStackView>,
	public IPropertyNotifySinkCP<CCallStackView>,
	public CComCoClass<CCallStackView, &CLSID_CallStackView>
{
public:
	CCallStackView()
	{
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CALLSTACKVIEW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCallStackView)
	COM_INTERFACE_ENTRY(ICallStackView)
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
END_COM_MAP()

BEGIN_PROP_MAP(CCallStackView)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CCallStackView)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CCallStackView)
	CHAIN_MSG_MAP(CComCompositeControl<CCallStackView>)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CCallStackView)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CCallStackView>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// ICallStackView
public:
	STDMETHOD(ShowCallStack)();

	enum { IDD = IDD_CALLSTACKVIEW };
};

#endif // __CALLSTACKVIEW_H_

