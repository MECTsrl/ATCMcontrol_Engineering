
#ifndef __ACTIONDBI_H_
#define __ACTIONDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDBIContext;
class CTypeDBI;
class CXMLArchive;

#include "DbgInfo.h"
#include "AnyDBI.h"


/////////////////////////////////////////////////////////////////////////////
// CActionDBI

class ATL_NO_VTABLE CActionDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CActionDBI, &CLSID_ActionDBI>,
	public IDispatchImpl<IActionDBI, &IID_IActionDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CActionDBI)
	COM_INTERFACE_ENTRY(IActionDBI)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
    HRESULT Init (CDBIContext *pContext, const CString &strName, long lNumber, long lX, long lY);
    HRESULT Init (CDBIContext *pContext, CXMLArchive &ar);

    CString GetName ();
    CTypeDBI *GetType ();
    long GetNumber ();
    long GetX ();
    long GetY ();

	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *psName);
	STDMETHOD(get_Type)(/*[out, retval]*/ IUnknown **ppType);
	STDMETHOD(get_Number)(/*[out, retval]*/ long *plNumber);
	STDMETHOD(get_X)(/*[out, retval]*/ long *plX);
	STDMETHOD(get_Y)(/*[out, retval]*/ long *plY);

protected:
    CTypeDBI    *m_pType;
    CString     m_strName;
    long        m_lNumber;
    long        m_lX;
    long        m_lY;

    CActionDBI ();
    ~CActionDBI ();
};


class CActionList
{
public:
    ~CActionList ();

    HRESULT Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent);
    void Unload (CAnyDBI *pParent);

    unsigned long GetCount ();
    CActionDBI *GetAction (unsigned long ulIndex);

    CActionDBI *LookupByNumber (long lNumber);
    CActionDBI *LookupByName (const CString &strName);
    CActionDBI *LookupByPosition (long lX, long lY);

protected:
    CArray<CActionDBI *, CActionDBI *> m_Actions;
};

#endif
