//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedlerror.h"
#include "CSC_OnlineErr.h"
#include "dlext.h"
#include "dlcsc.h"
#include "csc_online_i.c"
#include "downld.h"
#include "dlstat.h"

CImpTransferNotify::CImpTransferNotify()
{
	m_pStation = NULL;
}

CImpTransferNotify::~CImpTransferNotify()
{
	if(m_pStation) {
		m_pStation->Release();
		m_pStation = NULL;
	}
}


STDMETHODIMP CImpTransferNotify::OnUserLoggedIn (BSTR sUserName, BSTR sHostName, BOOL bLoggedIn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strUser = sUserName;

    if(bLoggedIn) {
	    FC_DEBUGPRINT1(_T("CEDL> User logged in = %s\n"),strUser);
	    m_pStation->FireErrorMessage(S_CE_DL_LOGIN,strUser);
    }
    else {
	    FC_DEBUGPRINT1(_T("CEDL> User logged out = %s\n"),strUser);
	    m_pStation->FireErrorMessage(S_CE_DL_LOGOUT,strUser);
    }

	return S_OK;
}

STDMETHODIMP CImpTransferNotify::OnTransferError (DWORD dwCookie, BSTR sCurrentDomainName, CSC_TRANSFER_STATE state, HRESULT hError)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    m_pStation->CheckThreadId(GetCurrentThreadId());

    CString str = sCurrentDomainName;

    switch(m_pStation->m_State) {
        case CEDL_STAT_GETDV: //get __.guid,__.list domains
			FC_DEBUGPRINT1(_T("CEDL> Upload domain %s - error\n"),str);
            m_pStation->SetSignal(PRJ_NEW);
            break;
        case CEDL_STAT_DLCC: //control domain download
            FC_DEBUGPRINT1(_T("CEDL> Download domain %s - error\n"),str);
            m_pStation->FireTransferError(str,state,hError);
            m_pStation->Abort();
            break;
        case CEDL_STAT_UPCC: //control domain upload
            FC_DEBUGPRINT1(_T("CEDL> Upload domain %s - error\n"),str);
            m_pStation->FireTransferError(str,state,hError);
            m_pStation->SetSignal(PRJ_NEW);
            break;
        case CEDL_STAT_DELCC:
            FC_DEBUGPRINT1(_T("CEDL> Delete domain %s - error\n"),str);
            m_pStation->FireTransferError(str,state,hError);
            break;
    }
    
    return S_OK;
}

STDMETHODIMP CImpTransferNotify::OnTransferStateChanged (DWORD dwCookie, BSTR sCurrentDomainName, CSC_TRANSFER_STATE state,float fltPercentDone)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(!m_pStation) {
		return S_OK;
    }

	m_pStation->CheckThreadId(GetCurrentThreadId());

	switch(state) {
		case initiating:
			break;
		case transfering_domain:
            OnTransferDomain(sCurrentDomainName,fltPercentDone);
			break;
        case deleted_domain:
            OnDeleteDomain(sCurrentDomainName,fltPercentDone);
            break;
		case aborting:
			OnAborting(dwCookie);
            break;
		case cleanup:
            OnCleanup();
			break;
	}

	return S_OK;
}

void CImpTransferNotify::OnDeleteDomain(BSTR sDomain, float fPercentage)
{
    CString strDomain = sDomain;

    DUMP_STATIONSTATE(_T("OnDeleteDomain"),m_pStation->m_State);

    switch(m_pStation->m_State) {
        case CEDL_STAT_DELCC:
            FC_DEBUGPRINT1(_T("CEDL> Delete domain %s\n"),strDomain);
            m_pStation->FireDLState(sDomain,fPercentage,DOWNLD_DEL);
            break;
        default:
            break;
    }
}


void CImpTransferNotify::OnTransferDomain(BSTR sDomain, float fPercentage)
{
    CString strDomain = sDomain;

    DUMP_STATIONSTATE(_T("OnTransferDomain"),m_pStation->m_State);

    switch(m_pStation->m_State) {
        case CEDL_STAT_GETDV:
            FC_DEBUGPRINT1(_T("CEDL> Upload domain %s\n"),strDomain);
            m_pStation->FireDLState(sDomain,fPercentage,DOWNLD_UPLOAD);
		    break;
        case CEDL_STAT_UPCC: //domain upload
            FC_DEBUGPRINT1(_T("CEDL> Upload domain %s\n"),strDomain);
            m_pStation->FireDLState(sDomain,fPercentage,DOWNLD_UPLOAD);
            break;
        case CEDL_STAT_DLCC:
            FC_DEBUGPRINT1(_T("CEDL> Download domain %s\n"),strDomain);
            m_pStation->FireDLState(sDomain,fPercentage,DOWNLD_TRANSFER);
            break;
        default:
            break;
    }

}

void CImpTransferNotify::OnCleanup(void)
{
    DUMP_STATIONSTATE(_T("OnCleanup"),m_pStation->m_State);

    m_pStation->DoStateMachine();
}

bool CImpTransferNotify::IsRequestPending(void)
{
    return (m_pStation->IsRequestPending());
}


void  CImpTransferNotify::OnAborting(DWORD dwCookie)
{
	HRESULT hr = m_pStation->OnAborting();

	if(FAILED(hr)) {
		m_pStation->FireErrorMessage(hr);
	}
}
