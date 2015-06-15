
#ifndef __STEPDBI_H_
#define __STEPDBI_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDBIContext;
class CTypeDBI;
class CXMLArchive;

#include "DbgInfo.h"
#include "AnyDBI.h"


/////////////////////////////////////////////////////////////////////////////
// CStepDBI

class ATL_NO_VTABLE CStepDBI : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CStepDBI, &CLSID_StepDBI>,
	public IDispatchImpl<IStepDBI, &IID_IStepDBI, &LIBID_DBGINFOLib>,
    public CAnyDBI
{
public:
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStepDBI)
	COM_INTERFACE_ENTRY(IStepDBI)
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

    CStepDBI ();
    ~CStepDBI ();
};


class CStepList
{
public:
    ~CStepList ();

    HRESULT Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent);
    void Unload (CAnyDBI *pParent);

    unsigned long GetCount ();
    CStepDBI *GetStep (unsigned long ulIndex);

    CStepDBI *LookupByNumber (long lNumber);
    CStepDBI *LookupByName (const CString &strName);
    CStepDBI *LookupByPosition (long lX, long lY);

protected:
    CArray<CStepDBI *, CStepDBI *> m_Steps;
};

#endif
