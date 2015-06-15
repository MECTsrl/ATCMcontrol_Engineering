
#ifndef __BREAKPOINTDBI_H_
#define __BREAKPOINTDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDBIContext;
class CXMLArchive;

#include "DbgInfo.h"
#include "AnyDBI.h"


/////////////////////////////////////////////////////////////////////////////
// CProjectDBI

class ATL_NO_VTABLE CBreakpointDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBreakpointDBI, &CLSID_BreakpointDBI>,
	public IDispatchImpl<IBreakpointDBI, &IID_IBreakpointDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBreakpointDBI)
	COM_INTERFACE_ENTRY(IBreakpointDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, long lNumber, EDP_POS_QUALIFIER qualifier, long lX, long lY, long lCX, long lCY);
    HRESULT Init (CDBIContext *pContext, CXMLArchive &ar);

    long GetNumber ();
    EDP_POS_QUALIFIER GetQualifier ();
    long GetX ();
    long GetY ();
    long GetCX();
    long GetCY();

	STDMETHOD(get_Number)(/*[out, retval]*/ long *plNumber);
	STDMETHOD(get_Qualifier)(/*[out, retval]*/ EDP_POS_QUALIFIER *pQualifier);
	STDMETHOD(get_X)(/*[out, retval]*/ long *plX);
	STDMETHOD(get_Y)(/*[out, retval]*/ long *plY);
	STDMETHOD(get_CX)(/*[out, retval]*/ long *plCX);
	STDMETHOD(get_CY)(/*[out, retval]*/ long *plCY);

protected:
    long              m_lNumber;
    EDP_POS_QUALIFIER m_Qualifier;
    long              m_lX;
    long              m_lY;
    long              m_lCX;
    long              m_lCY;

    CBreakpointDBI ();
    ~CBreakpointDBI ();
};


class CBreakpointList
{
public:
    ~CBreakpointList ();

    HRESULT Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent);
    void Unload (CAnyDBI *pParent);

    unsigned long GetCount ();
    CBreakpointDBI *GetBreakpoint (unsigned long ulIndex);

    CBreakpointDBI *LookupByNumber (long lNumber);
    CBreakpointDBI *LookupByPosition (EDP_POS_QUALIFIER qualifier, long lX, long lY, long lCX, long lCY);

protected:
    CArray<CBreakpointDBI *, CBreakpointDBI *> m_Breakpoints;
};

#endif
