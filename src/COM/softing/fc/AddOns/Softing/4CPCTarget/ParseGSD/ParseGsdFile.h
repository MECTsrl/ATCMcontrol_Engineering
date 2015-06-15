
#ifndef __PARSEGSDFILE_H_
#define __PARSEGSDFILE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // main symbols
#include "ParseGSD.h"   // interfaces
#include "pgsd.h"       // coclasses definition
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CParseGsdFile
class ATL_NO_VTABLE CParseGsdFile :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CParseGsdFile,&CLSID_ParseGsdFile>,
	public CComControl<CParseGsdFile>,
	public IDispatchImpl<IParseGsdFile, &IID_IParseGsdFile, &LIBID_PARSEGSDLib,2,0>,
	public IPersistStreamInitImpl<CParseGsdFile>,
	public IOleControlImpl<CParseGsdFile>,
	public IOleObjectImpl<CParseGsdFile>,
	public IOleInPlaceActiveObjectImpl<CParseGsdFile>,
	public IViewObjectExImpl<CParseGsdFile>,
	public IConnectionPointContainerImpl<CParseGsdFile>,
	public IOleInPlaceObjectWindowlessImpl<CParseGsdFile>
{
public:
	                        CParseGsdFile();
	virtual                 ~CParseGsdFile();
    HRESULT                 FinalConstruct();
    void                    FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_PARSEGSDFILE)

//--------------------------
// =Interface map
BEGIN_COM_MAP(CParseGsdFile) 
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IParseGsdFile)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IViewObject, IViewObjectEx)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IViewObject2, IViewObjectEx)
	COM_INTERFACE_ENTRY_IMPL(IViewObjectEx)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL_IID(IID_IOleInPlaceObject, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IMPL(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY_IMPL(IOleControl)
	COM_INTERFACE_ENTRY_IMPL(IOleObject)
	COM_INTERFACE_ENTRY_IMPL(IPersistStreamInit)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

//--------------------------
// =Property map
BEGIN_PROPERTY_MAP(CParseGsdFile)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROPERTY_MAP()


//--------------------------
// =Connection point map
BEGIN_CONNECTION_POINT_MAP(CParseGsdFile)
END_CONNECTION_POINT_MAP()

//--------------------------
// =Message map
BEGIN_MSG_MAP(CParseGsdFile)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
END_MSG_MAP()


// IViewObjectEx
	STDMETHOD(GetViewStatus)(DWORD* pdwStatus)
	{
		ATLTRACE(_T("IViewObjectExImpl::GetViewStatus\n"));
		*pdwStatus = 0;
		return S_OK;
	}

//--------------------------
// =Interface
// IParseGsdFile
public:
	STDMETHOD(get_sTraceFilePath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_sTraceFilePath)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_bTraceParsing)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_bTraceParsing)(/*[in]*/ BOOL newVal);
	STDMETHOD(ParseFile)(/*[in, string]*/ BSTR sFileGSD, /*[out]*/ LPUNKNOWN *pGsdObj);
	HRESULT OnDraw(ATL_DRAWINFO& di);

// =Helpers
private:
    LPCTSTR                 GetTempDir(void);

// =Attributes:
private:
    BOOL                    m_bTraceParsing;
    CString                 m_sTraceFilePath;
};

#endif //__PARSEGSDFILE_H_

