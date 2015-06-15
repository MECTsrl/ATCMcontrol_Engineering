//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DP_ExtParam.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDP_ExtUserParam
//------------------------------------------------------------------*
CDP_ExtUserParam::CDP_ExtUserParam()
  : m_dataType(octets),
    m_paramType(CExtParamDefinition::constantValue),
    m_lOffset(0),
    m_lDefaultValue(0),
    m_lMinValue(0),
    m_lMaxValue(0),
    m_lAreaStart(0),
    m_lAreaLength(0)
{
    m_lRefNum = NOT_SPECIFIED;
}

//------------------------------------------------------------------*
CDP_ExtUserParam::~CDP_ExtUserParam()
{
}

//------------------------------------------------------------------*
HRESULT
    CDP_ExtUserParam::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void
    CDP_ExtUserParam::FinalRelease()
{
}


//------------------------------------------------------------------*
BOOL
    CDP_ExtUserParam::CopyDefinitionValues(CExtParamDefinition *pDefinition)
{
    ASSERT(pDefinition != NULL);
    if (pDefinition == NULL) {
        return FALSE;
    }
    m_strName       = pDefinition->m_strName;
    m_dataType      = pDefinition->m_dataType;
    m_paramType     = pDefinition->m_paramType;

//  Do not copy offset, it always has a random value and overwrites probably a good value
//  m_lOffset       = pDefinition->m_lOffset;  

    m_lDefaultValue = pDefinition->m_lDefaultValue;
    m_lMinValue     = pDefinition->m_lMinValue;
    m_lMaxValue     = pDefinition->m_lMaxValue;
    m_lAreaStart    = pDefinition->m_lAreaStart;
    m_lAreaLength   = pDefinition->m_lAreaLength;

    // copy enum values !!
    m_textDef       = pDefinition->m_textDef;
    POSITION pos;
    for (pos = pDefinition->m_listEnumValues.GetHeadPosition(); pos != NULL; )
    {
        m_listEnumValues.AddTail(pDefinition->m_listEnumValues.GetNext(pos));
    }

    return TRUE;
}

void CDP_ExtUserParam::Check(CStationCallback *pcb)
{
    _ASSERTE(pcb != NULL);

    if(m_textDef.GetRefNumber() == 0) {
        return; //no real text definitions.
    }

    //check whether possible values might be out of range
    m_textDef.Check(m_lMinValue,m_lMaxValue,pcb);
}

//=============================================================================
//      IExtUserParam
//=============================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_Offset(LONG *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_lOffset;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_Description(BSTR *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_strName.AllocSysString();
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_IsConstantParam(/*[out, retval]*/ BOOL *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = (m_paramType == CExtParamDefinition::constantValue);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_IsVariableEnumParam(BOOL *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = (m_paramType == CExtParamDefinition::variableEnum);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_IsVariableMinMaxParam(BOOL *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = (m_paramType == CExtParamDefinition::variableRange);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_IsBitArea(BOOL *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = (m_dataType == bitArea);
    return S_OK;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_BitAreaStart(LONG *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (m_dataType != bitArea) {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    *pVal = m_lAreaStart;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_BitAreaLength(LONG *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (m_dataType != bitArea) {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    *pVal = m_lAreaLength;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_DefaultValue(LONG *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (m_paramType == CExtParamDefinition::constantValue) {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    *pVal = m_lDefaultValue;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_MinValue(LONG *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (m_paramType != CExtParamDefinition::variableRange) {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    *pVal = m_lMinValue;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_MaxValue(LONG *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (m_paramType != CExtParamDefinition::variableRange) {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    *pVal = m_lMaxValue;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::get_ConstPrmBytes(VARIANT *pVal)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }
    if (m_paramType != CExtParamDefinition::constantValue) {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    pVal->vt = VT_UI1 | VT_ARRAY;   // array of bytes
    SAFEARRAYBOUND boundData;
    boundData.lLbound = 0;
    boundData.cElements = m_usrParamBytes.GetSize();

    SAFEARRAY   *psaData = ::SafeArrayCreate(VT_UI1, 1, &boundData);
    if (psaData == NULL) {
        return E_OUTOFMEMORY;
    }
    
    // copy UsrParam bytes to SAFEARRAY
    HRESULT hr;
    if (boundData.cElements > 0)
    {
        const BYTE *pUsrParamBytes = m_usrParamBytes.GetData();
        _ASSERTE(pUsrParamBytes != NULL);
        BYTE *pRawData;
        hr = ::SafeArrayAccessData(psaData, (void HUGEP* FAR*)&pRawData);
        ::memcpy(pRawData, pUsrParamBytes, m_usrParamBytes.GetSize());
        ::SafeArrayUnaccessData(psaData);
    }
    pVal->parray = psaData;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::NamedEnumValues(VARIANT *pValues, VARIANT *pNames)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (   (pValues == NULL)
        || (pNames == NULL))
    {
        return E_POINTER;
    }
    if (   (m_paramType != CExtParamDefinition::variableEnum)
        && (m_paramType != CExtParamDefinition::variableRange))
    {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    return m_textDef.GetValueTextLists(pValues, pNames);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CDP_ExtUserParam::EnumValues(VARIANT *pValues)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (pValues == NULL)
    {
        return E_POINTER;
    }
    if (   (m_paramType != CExtParamDefinition::variableEnum)
        && (m_paramType != CExtParamDefinition::variableRange))
    {
        return E_FAIL;      // type of ExtParam mismatch !!!
    }

    pValues->vt = VT_I4 | VT_ARRAY;   // array of long
    SAFEARRAYBOUND boundData;
    boundData.lLbound = 0;
    boundData.cElements = m_listEnumValues.GetCount();
    SAFEARRAY   *psaValues = ::SafeArrayCreate(VT_I4, 1, &boundData);
    if (psaValues == NULL) {
        return E_OUTOFMEMORY;
    }
    
    // copy values to SAFEARRAY
    HRESULT hr;
    POSITION pos = m_listEnumValues.GetHeadPosition();
    for (long i = boundData.lLbound; pos != NULL; i++)
    {
        LONG lValue = m_listEnumValues.GetNext(pos);
        _ASSERTE((ULONG) i <= boundData.cElements);
        hr = ::SafeArrayPutElement(psaValues, &i, &lValue);
        _ASSERTE(SUCCEEDED(hr));
        if (FAILED(hr)) {
            continue;
        }
    }
    pValues->parray = psaValues;
    
    return S_OK;
}

//=============================================================================
//      IExtUserParam2
//=============================================================================

STDMETHODIMP CDP_ExtUserParam::get_Datatype(tagGSDDatatype * pVal)
{
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_dataType;		
	return S_OK;
}
/////////////////////////////////////////////////////////////////////
// CEnumExtUserParam


//------------------------------------------------------------------*
CEnumExtUserParam::CEnumExtUserParam()
    : m_listExtUserParams()
{
	m_pos = 0;
    m_count = 0;
}

//------------------------------------------------------------------*
CEnumExtUserParam::~CEnumExtUserParam()
{
    m_listExtUserParams.RemoveAll();
    m_pos = 0;
    m_count = 0;
}

//------------------------------------------------------------------*
void CEnumExtUserParam::Initialize(listExtUserParamObject& listExtUserParams)
{
    CDPExtUserParamObject* pParam = NULL;
    m_count = listExtUserParams.GetCount();

    for(ULONG ul= 0; ul< m_count; ul++) {
        //operator [] makes implicit addref
        pParam = listExtUserParams[ul];
        if(pParam) {
            m_listExtUserParams.Add(pParam);
        }
    }

    Reset();
}

//------------------------------------------------------------------*
//Implementation for IEnumUnknown
STDMETHODIMP CEnumExtUserParam::Next(ULONG celt, LPUNKNOWN* prgpUnk, ULONG* pceltFetched)
{
    ULONG ulFetched;
    CDPExtUserParamObject* pParam = NULL;

    if(prgpUnk == NULL) {
        return E_POINTER;
    }

    ::ZeroMemory(prgpUnk,sizeof(*prgpUnk) * celt);

    if(pceltFetched == NULL && celt != 1) {
        //this it not allowed
        return E_INVALIDARG;
    }

    for(ulFetched = 0;(m_pos < m_count && ulFetched < celt); m_pos++) {
        //[] operator performs implicit addref
        pParam = m_listExtUserParams[m_pos];
        if(pParam) {
            prgpUnk[ulFetched] = pParam->GetUnknown();
            ulFetched++;
        }
    }

    if(pceltFetched) {
        *pceltFetched = ulFetched;
    }

    return (ulFetched == celt) ? S_OK : S_FALSE ;
}

//------------------------------------------------------------------*
STDMETHODIMP CEnumExtUserParam::Skip(ULONG  celt)
{
    if(m_pos + celt >= m_count) {
        m_pos = m_count-1;
        return S_FALSE;
    }

    m_pos += celt;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CEnumExtUserParam::Reset(void)
{
	m_pos = 0;
	return S_OK;	
}

//------------------------------------------------------------------*
// Implementation for IEnumUnknown
STDMETHODIMP CEnumExtUserParam::Clone(IEnumUnknown **ppEnumUnknown)
{
    if (ppEnumUnknown == NULL) {
        return E_POINTER;
    }
    *ppEnumUnknown = NULL;

    CComObject<CEnumExtUserParam>   *pEnumExtUserParams = NULL;
    HRESULT hr = CComObject<CEnumExtUserParam>::CreateInstance(&pEnumExtUserParams);
    if (FAILED(hr)) {
        return hr;
    }

    pEnumExtUserParams->Initialize(m_listExtUserParams);
    // this QI also increments the missing AddRef from CreateInstance
    return pEnumExtUserParams->QueryInterface(IID_IEnumUnknown, (LPVOID*)ppEnumUnknown);
} 

