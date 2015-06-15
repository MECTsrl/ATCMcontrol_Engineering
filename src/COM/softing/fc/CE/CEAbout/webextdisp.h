
#pragma once


// --------------------------------------------------------------------------------
//
// IWebBrowserExternalDispatchImpl
//
// --------------------------------------------------------------------------------
//
// This class is used to provide an implementation of external IDispatch
// when hosting a WebBrowser control (the thumbnail view).
//
// It accepts any method listed in the WDISPATCH_MAP() of the class. From
// whithin the HTML page, these methods can be accessed via the 
// window.external object, e.g:
// 
// <A HREF='vbscript:window.external.ReadCurrentValues'>Read value</A>  
// 
// The methods may have no parameters.
//

template <class T>
struct _WDISPATCH_MAP_ENTRY 
{
	HRESULT (_stdcall T::*pfn)();	  // method to invoke
  LPOLESTR szDispName;            // method's name
};

#define BEGIN_WDISPATCH_MAP(_class) \
  static const _WDISPATCH_MAP_ENTRY<_class>* GetDispatchMap()\
  {\
    static const _WDISPATCH_MAP_ENTRY<_class> _entries[] = {

#define WDISPATCH_METHOD_ENTRY(name)  \
  { name, OLESTR(#name) },

#define END_WDISPATCH_MAP() \
  {0,0}}; \
  return _entries; } 


template <class T>
class ATL_NO_VTABLE IWebBrowserExternalDispatchImpl : public IDispatch
{
public:
// IDispatch
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
	{      
    T* pT = static_cast<T*>(this);
    HRESULT hRes = S_OK;
    for( UINT i = 0 ; i <  cNames ; i++ )
    {
      rgdispid[i] = DISPID_UNKNOWN;

      const _WDISPATCH_MAP_ENTRY<T>* pMap = pT->GetDispatchMap();    
      const _WDISPATCH_MAP_ENTRY<T>* pEntry = pMap;
      
      for( pEntry ; pEntry && 0 != pEntry->szDispName ; pEntry++ )
      {
        if ( 0 == wcscmp(rgszNames[i], pEntry->szDispName)  )
        { 
          rgdispid[i] = (pEntry - pMap); 
          break;
        }
      }

      if ( DISPID_UNKNOWN == rgdispid[i] )
        hRes = DISP_E_UNKNOWNNAME;
    }
		return hRes;
	}

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr)
	{
    T* pT = static_cast<T*>(this);
    const _WDISPATCH_MAP_ENTRY<T>* pEntry = pT->GetDispatchMap();
    (pT->*pEntry[dispidMember].pfn)(); 
    return S_OK;
	}
};

