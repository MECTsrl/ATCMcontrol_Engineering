

#include "stdafx.h"
#include "fchtmled.h"
#include "FcIDispatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFcIDispatch::CFcIDispatch(IUnknown *pUnkownObj/*=NULL*/)
{
 	m_pUnkownObj = pUnkownObj;
	m_cRef = 1;
}

CFcIDispatch::~CFcIDispatch()
{
	ASSERT(m_cRef == 0);
}


STDMETHODIMP CFcIDispatch::QueryInterface(REFIID iid, void **ppvObject)
{
	*ppvObject = NULL;
	if (IID_IDispatch == iid || IID_IUnknown == iid)
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}
	if (m_pUnkownObj != NULL)
		return m_pUnkownObj->QueryInterface(iid, ppvObject);
	else
		return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CFcIDispatch::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcIDispatch::Release()
{
	ASSERT(m_cRef > 0);
    --m_cRef;

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
    return m_cRef;
}


/*
 * CFcIDispatch::GetTypeInfoCount
 * CFcIDispatch::GetTypeInfo
 * CFcIDispatch::GetIDsOfNames
 *
 * Unimplemented members, not needed for ambient properties.
 */

STDMETHODIMP CFcIDispatch::GetTypeInfoCount(UINT *pctInfo)
{
    *pctInfo=0;
    return NOERROR;
}

STDMETHODIMP CFcIDispatch::GetTypeInfo(UINT itinfo
    , LCID lcid, ITypeInfo **pptInfo)
{
    *pptInfo=NULL;
    return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CFcIDispatch::GetIDsOfNames(REFIID riid
    , OLECHAR **rgszNames, UINT cNames, LCID lcid, DISPID *rgDispID)
{
    *rgszNames=NULL;
    *rgDispID=NULL;
    return ResultFromScode(E_NOTIMPL);
}


/*
 * CFcIDispatch::Invoke
 *
 * Purpose:
 *  Calls a method in the dispatch interface or manipulates a
 *  property.
 *
 * Parameters:
 *  dispIDMember    DISPID of the method or property of interest.
 *  riid            REFIID reserved, must be NULL.
 *  lcid            LCID of the locale.
 *  wFlags          USHORT describing the context of the invocation.
 *  pDispParams     DISPPARAMS * to the array of arguments.
 *  pVarResult      VARIANT * in which to store the result.  Is
 *                  NULL if the caller is not interested.
 *  pExcepInfo      EXCEPINFO * to exception information.
 *  puArgErr        UINT * in which to store the index of an
 *                  invalid parameter if DISP_E_TYPEMISMATCH
 *                  is returned.
 *
 * Return Value:
 *  HRESULT         NOERROR or a general error code.
 */


STDMETHODIMP CFcIDispatch::Invoke(DISPID dispIDMember, REFIID riid
    , LCID lcid, unsigned short wFlags, DISPPARAMS *pDispParams
    , VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    HRESULT     hr;
    VARIANT     varResult;

    if (IID_NULL!=riid)
        return ResultFromScode(E_INVALIDARG);

    if(NULL==pVarResult)
      pVarResult=&varResult;

    VariantInit(pVarResult);

    //The most common case is boolean, use as an initial type
    V_VT(pVarResult)=VT_BOOL;

    /*
     * Process the requested ambient property.  Anything but a
     * request for a property is invalid, so we can check that
     * before looking at the specific ID.  We can only get away
     * with this because all properties are read-only.
     */

    if (!(DISPATCH_PROPERTYGET & wFlags))
        return ResultFromScode(DISP_E_MEMBERNOTFOUND);

    hr=NOERROR;

    switch (dispIDMember)
	{
        case DISPID_AMBIENT_USERMODE:
            V_BOOL(pVarResult)= VARIANT_FALSE;
            break;

        default:
            hr=ResultFromScode(DISP_E_MEMBERNOTFOUND);
            break;
	}

    return hr;
}
