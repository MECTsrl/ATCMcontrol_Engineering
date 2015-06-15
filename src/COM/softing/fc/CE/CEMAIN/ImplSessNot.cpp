
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "cesys.h"
#include "TargetOnlBase.h"
#include "ImplSessNot.h"
#include "CESysDef.h"
#include "traceif.h"

STDMETHODIMP_(HRESULT) CImplSessionNotify::OnChanged()
{
    TrcPrint(TRC_COMM_EVENT, _T("CImplSessionNotify::ReverOnChangedtConnectState \n"));
	return S_OK;
}

STDMETHODIMP_(HRESULT) CImplSessionNotify::OnStateChanged(/*[in]*/ LPUNKNOWN pSession, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError)
{
	AFX_MANAGE_STATE (AfxGetAppModuleState ());

    TrcPrint(TRC_COMM_EVENT, _T("CImplSessionNotify::OnStateChanged : newState=%i ;  hrError=0x%08X!\n"), 
                newState, hrError);


	if (m_pCallback == NULL)
	{
		return E_FAIL;
	}

	m_pCallback->Execute (newState, hrError);
    return S_OK;
}

