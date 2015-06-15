//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "dp_unitdiag.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

CDiagVal::CDiagVal()
: m_strDiagText()
{
    m_lValue = NOT_SPECIFIED;
}

CDiagVal::CDiagVal(long lValue, LPCTSTR pszDiagText)
: m_strDiagText(pszDiagText)
{
    m_lValue = lValue;
}

CDiagVal::CDiagVal(const CDiagVal & other)
{
    CopyInternal(other);
}

CDiagVal & CDiagVal::operator =(const CDiagVal &other)
{
    CopyInternal(other);
    return *this;
}


void CDiagVal::CopyInternal(const CDiagVal &other)
{
    if(this == &other) {
        return;
    }

    m_lValue = other.m_lValue;
    m_strDiagText = other.m_strDiagText;
}

CDP_UnitDiag::CDP_UnitDiag()
: m_Values()
{
    m_Type = DiagBit;
    m_lBitAreaStart = NOT_SPECIFIED;
    m_lBitAreaEnd = NOT_SPECIFIED;
}

CDP_UnitDiag::~CDP_UnitDiag()
{
    m_Values.RemoveAll();
}

void CDP_UnitDiag::AddValue(long lValue, LPCTSTR pszDiagText)
{
    if(pszDiagText == NULL) {
        return;
    }

    CDiagVal val(lValue,pszDiagText);
    m_Values.AddTail(val);
}

STDMETHODIMP CDP_UnitDiag::get_DiagnosticType(tagDiagType * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_Type;
	return S_OK;
}

STDMETHODIMP CDP_UnitDiag::get_BitAreaStart(LONG * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);

    if(m_Type == DiagChannel) { 
        return E_INVALIDARG; 
    }
    *pVal = m_lBitAreaStart;
	return S_OK;
}

STDMETHODIMP CDP_UnitDiag::get_BitAreaEnd(LONG * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);

    if(m_Type == DiagChannel) { 
        return E_INVALIDARG; 
    }

    *pVal = m_lBitAreaEnd;
	return S_OK;
}

STDMETHODIMP CDP_UnitDiag::get_Values(VARIANT * pValues, VARIANT * pText)
{
    HRESULT hr;
    SAFEARRAYBOUND boundData,boundData2;
    SAFEARRAY *psaValues = NULL;
    SAFEARRAY *psaText = NULL;
    BSTR s = NULL;
    long lValue;
    
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pValues,E_POINTER);
    FC_PCHECK_HR(pText,E_POINTER);

    //iff something is in the variant - resource leak ?
    ::VariantInit(pValues);
    ::VariantInit(pText);

    try {
        CString str;
        // build array of values:
        pValues->vt = VT_I4 | VT_ARRAY;   // array of long
        boundData.lLbound = 0;
        boundData.cElements = m_Values.GetCount();
        psaValues = ::SafeArrayCreate(VT_I4, 1, &boundData);
        if (psaValues == NULL) {
            AfxThrowMemoryException();
        }
    
        pText->vt = VT_BSTR | VT_ARRAY;   // array of BSTR
        boundData2.lLbound = 0;
        boundData2.cElements = m_Values.GetCount();
        psaText = ::SafeArrayCreate(VT_BSTR, 1, &boundData2);
        if (psaText == NULL) {
            AfxThrowMemoryException();
        }
    
        // copy values to SAFEARRAY
        POSITION pos = m_Values.GetHeadPosition();
        for (long i = boundData.lLbound; pos != NULL; i++)
        {
            CDiagVal val = m_Values.GetNext(pos);
            _ASSERTE((ULONG) i <= boundData.cElements);

            lValue = val.m_lValue;
            str = val.m_strDiagText;

            hr = ::SafeArrayPutElement(psaValues, &i, &lValue);
            _ASSERTE(SUCCEEDED(hr));
            if (FAILED(hr)) {
                continue;
            }
            s = str.AllocSysString();
            hr = ::SafeArrayPutElement(psaText, &i, s);
            FC_FREE_BSTR(s);
            _ASSERTE(SUCCEEDED(hr));
            if (FAILED(hr)) {
                continue;
            }
        }
    }
    catch(CMemoryException * e) {
        e->Delete();
        
        if(psaValues != NULL) {
            ::SafeArrayDestroy(psaValues);
        }

        if(psaText != NULL) {
            ::SafeArrayDestroy(psaText);
        }

        FC_FREE_BSTR(s);
        return E_OUTOFMEMORY;
    }

    pValues->parray = psaValues;
    pText->parray = psaText;
    return S_OK;
}

