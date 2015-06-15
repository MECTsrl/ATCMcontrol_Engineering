// CompManEventsImpl.cpp: implementation of the CCompManEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestProjManDlg1.h"

#include "CompManEventsImpl.h"
#include "cecompmanev_i.c"
#include "kadmanager_i.c"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompManEventsImpl::CCompManEventsImpl()
{
    HRESULT hr; 

    m_pKadManag = NULL;
    hr = CoCreateInstance(CLSID_KadManagerComObj,
		                  NULL,
		                  CLSCTX_INPROC_SERVER,
		                  IID_IUnknown,
						  (LPVOID *)&m_pKadManag);

	if (hr!=S_OK)
    {
        ATLTRACE("error: InitKADManager: CoCreateInstance\n");
        return;
    }   
}

CCompManEventsImpl::~CCompManEventsImpl()
{
    m_pKadManag->Release();
}

STDMETHODIMP CCompManEventsImpl::OnComponentByCLSID(BSTR sCLSID, IUnknown** ppUnk)
{
    *ppUnk = m_pKadManag;
    (*ppUnk)->AddRef();
    return S_OK;
}

STDMETHODIMP CCompManEventsImpl::OnComponentById(BSTR sId, IUnknown** ppUnk)
{
    return E_NOTIMPL;
}