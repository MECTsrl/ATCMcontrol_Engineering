//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "defs.h"
#include "DP_Module.h"
#include "gsdmessage.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDP_Module
//------------------------------------------------------------------*

CDP_Module::CDP_Module()
  : m_lModuleReference(NOT_SPECIFIED),
    m_lExtModParamLen(0),
    m_strInfoText(),
    m_listDiag()
{
}

//------------------------------------------------------------------*
CDP_Module::~CDP_Module()
{

}

//------------------------------------------------------------------*
HRESULT CDP_Module::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void CDP_Module::FinalRelease()
{
    m_listExtUserParamObject.RemoveAll();
    m_listDiag.RemoveAll();
}

void CDP_Module::Check(long lRevision, CStationCallback* pcb)
{
    CString strMsg;
    int i,iCount;
    //check existence of module reference if revision >= 3
    if(lRevision >= 3 && m_lModuleReference == NOT_SPECIFIED) {
        //issue error - Module reference required as of GSD Revision 3
        GSD_FormatMessage(strMsg,E_GSD_MODREF_MISSING,(LPCTSTR)m_strName);
        pcb->Message(E_GSD_MODREF_MISSING,strMsg);
    }

    //Check PrmText values agains boundaries of ExtUserPrmData
    iCount = m_listExtUserParamObject.GetCount();
    for(i= 0; i< iCount; i++) {
        CDPExtUserParamObject * p = m_listExtUserParamObject[i]; //makes an addref.
        if(!p) {
            _ASSERTE(p);
            continue;
        }
        
        p->Check(pcb);
        FC_RELEASE_PTR(p);
    }
}

//=============================================================================
//      IModule
//=============================================================================

//------------------------------------------------------------------*
STDMETHODIMP CDP_Module::get_LenUserParams(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
	*pVal = m_lExtModParamLen;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Module::get_HasExtUserParam(/*[out, retval]*/ BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_listExtUserParamObject.GetCount() > 0);
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Module::get_ModName(/*[out, retval]*/ BSTR *pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    return ::AllocSysString(pVal,m_strName);
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Module::get_ConfigBytes(/*[out, retval]*/ VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);

    pVal->vt = VT_UI1 | VT_ARRAY;   // array of bytes
    SAFEARRAYBOUND boundData;
    boundData.lLbound = 0;
    boundData.cElements = m_configBytes.GetSize();

    SAFEARRAY   *psaData = ::SafeArrayCreate(VT_UI1, 1, &boundData);
    if (psaData == NULL) {
        return E_OUTOFMEMORY;
    }
    
    // copy UsrParam bytes to SAFEARRAY
    HRESULT hr;
    if (boundData.cElements > 0)
    {
        const BYTE *pConfigBytes = m_configBytes.GetData();
        _ASSERTE(pConfigBytes != NULL);
        BYTE *pRawData;
        hr = ::SafeArrayAccessData(psaData, (void HUGEP* FAR*)&pRawData);
        ::memcpy(pRawData, pConfigBytes, m_configBytes.GetSize());
        ::SafeArrayUnaccessData(psaData);
    }
    pVal->parray = psaData;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CDP_Module::CreateExtUserParamEnumerator(LPUNKNOWN *ppUnk)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(ppUnk,E_POINTER);

    *ppUnk = NULL;

	CEnumExtUserParamObject *pEnumUserParams = new CEnumExtUserParamObject;
    if (pEnumUserParams == NULL) {
		return E_OUTOFMEMORY;
    }
	pEnumUserParams->Initialize(m_listExtUserParamObject);
	return pEnumUserParams->QueryInterface(IID_IEnumUnknown, (void**)ppUnk);
}

//=============================================================================
//      IModule2
//=============================================================================

STDMETHODIMP CDP_Module::get_ModuleReference(LONG * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_lModuleReference;		
	return S_OK;
}

STDMETHODIMP CDP_Module::get_InfoText(BSTR * psVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(psVal,E_POINTER);
    return ::AllocSysString(psVal,m_strInfoText);
}

STDMETHODIMP CDP_Module::get_ExtUserParamCollection(IExtUserParamCollection** pp)
{
    int iCount,i;
    CComObject<ExtUserParamCollection> *pc = NULL;
    IExtUserParam *pParam;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<ExtUserParamCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listExtUserParamObject.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDPExtUserParamObject * p = m_listExtUserParamObject[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            pParam = dynamic_cast<IExtUserParam*>(p);
            _ASSERTE(pParam);

            hrLoop = pc->Insert(i,pParam);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pParam);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

STDMETHODIMP CDP_Module::get_DiagnosticCollection(IDiagnosticInfoCollection** pp)
{
    int iCount,i;
    CComObject<DiagCollection> *pc = NULL;
    IDiagnosticInfo *pInfo;
    HRESULT hr = S_OK,hrLoop;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pp,E_POINTER);
    *pp = NULL;

    try {
        hr = CComObject<DiagCollection>::CreateInstance(&pc);
        if(FAILED(hr)) {
            throw hr;
        }

        iCount = m_listDiag.GetCount();

        hr = pc->Initialize(iCount); 
        if(FAILED(hr)) {
            throw hr;
        }

        for(i= 0; i< iCount; i++) {
            CDP_UnitDiagObject * p = m_listDiag[i]; //makes an addref.
            if(!p) {
                _ASSERTE(p);
                continue;
            }
            pInfo = dynamic_cast<IDiagnosticInfo*>(p);
            _ASSERTE(pInfo);

            hrLoop = pc->Insert(i,pInfo);
            if(FAILED(hrLoop)) {
                hr = hrLoop;
            }

            FC_RELEASE_PTR(pInfo);
        }

        if(FAILED(hr)) {
            throw hr;
        }

        hr = pc->QueryInterface(pp);
    }
    catch(HRESULT hrRes) {
        if(pc) {
            delete pc;
        }
        hr = hrRes;
    }
    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CEnumModules

//------------------------------------------------------------------*
CEnumModules::CEnumModules()
    : m_listModules()
{
	m_pos = 0;
    m_count = 0;
}

//------------------------------------------------------------------*
CEnumModules::~CEnumModules()
{

    m_listModules.RemoveAll();
	m_pos = 0;
    m_count = 0;
}

//------------------------------------------------------------------*
void CEnumModules::Initialize(listModuleObject& listModules)
{
	CDPModuleObject* pModule = NULL;
    m_count = listModules.GetCount();

    for(ULONG ul= 0; ul< m_count; ul++) {
        //operator [] makes implicit addref
        pModule = listModules[ul];
        if(pModule) {
            m_listModules.Add(pModule);
        }
    }

	Reset();
}

//------------------------------------------------------------------*
//Implementation for IEnumUnknown
STDMETHODIMP CEnumModules::Next(ULONG celt, LPUNKNOWN* prgpUnk, ULONG* pceltFetched)
{
    ULONG ulFetched;
    CDPModuleObject* pModule = NULL;

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
        pModule = m_listModules[m_pos];
        if(pModule) {
            prgpUnk[ulFetched] = pModule->GetUnknown();
            ulFetched++;
        }
    }

    if(pceltFetched) {
        *pceltFetched = ulFetched;
    }

    return (ulFetched == celt) ? S_OK : S_FALSE ;
}

//------------------------------------------------------------------*
STDMETHODIMP CEnumModules::Skip(ULONG celt)
{
    if(m_pos + celt >= m_count) {
        m_pos = m_count-1;
        return S_FALSE;
    }

    m_pos += celt;
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CEnumModules::Reset(void)
{
	m_pos = 0;
	return S_OK;	
}

//------------------------------------------------------------------*
// Implementation for IEnumUnknown
STDMETHODIMP CEnumModules::Clone(IEnumUnknown **ppEnumUnknown)
{
    if (ppEnumUnknown == NULL) {
        return E_POINTER;
    }
    *ppEnumUnknown = NULL;

	CEnumModuleObject *pEnumModules = new CEnumModuleObject;
    if (pEnumModules == NULL) {
		return E_OUTOFMEMORY;
    }
	pEnumModules->Initialize(m_listModules);
	return pEnumModules->QueryInterface(IID_IEnumUnknown, (LPVOID*)ppEnumUnknown);
} 


