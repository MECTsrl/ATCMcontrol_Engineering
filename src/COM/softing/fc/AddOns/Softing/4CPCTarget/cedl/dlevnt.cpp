
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedl.h"
#include "dlevnt.h"
#include "resource.h"
#include "utilif.h"

CImpDLEvents::CImpDLEvents(void)
{
    m_hwndListClient = NULL;
}

CImpDLEvents::~CImpDLEvents(void)
{
}

STDMETHODIMP CImpDLEvents::OnDownloadState(BSTR sAddress, BSTR sDomain, float fPercentage,CEDL_DOWNLD_STATE State)
{
	HWND hwndNameCtrl;
    HWND hwndProgressCtrl;
	CString strOut;

    if(!m_hwndListClient) {
        /*on normal download the event methods will also be invoked
          but the dialog handle is invalid 
        */
        return S_OK; 
    }
	
	switch(State) {
        case DOWNLD_READY:
            hwndNameCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_DOMNAME);
            if(hwndNameCtrl != NULL) {
				strOut.LoadString(IDS_DL_DONE);
				::SetWindowText(hwndNameCtrl, strOut);
            }
            hwndProgressCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_PROGRESS);
            if(hwndProgressCtrl != NULL) {
                ::PostMessage(hwndProgressCtrl, PBM_SETPOS,  0, 0);
            }
			::PostMessage(m_hwndListClient, CEDL_MSG_DOWNLOAD_READY, 0, 0);
            break;
		case DOWNLD_ABORTED:
            ::PostMessage(m_hwndListClient,CEDL_MSG_DOWNLOAD_ERROR, 0, 0);
            break;
		case DOWNLD_INTR:
            hwndNameCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_DOMNAME);
            if(hwndNameCtrl != NULL) {
				strOut.LoadString(IDS_CONN_INTR);
				::SetWindowText(hwndNameCtrl, strOut);
			}
			::PostMessage(m_hwndListClient,CEDL_MSG_DOWNLOAD_ERROR, 0, 0);
			break;
		case DOWNLD_UPLOAD:
        case DOWNLD_TRANSFER:
            hwndNameCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_DOMNAME);
            if(hwndNameCtrl != NULL) {
				int iIDS;
                CString strDomainName = sDomain;
				iIDS = (State == DOWNLD_UPLOAD) ? IDS_UPLD : IDS_DOWNLD;
				strOut.LoadString(iIDS);
				strOut += strDomainName;
                ::SetWindowText(hwndNameCtrl, strOut);
            }
            hwndProgressCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_PROGRESS);
            if(hwndProgressCtrl != NULL) {
                ::PostMessage(hwndProgressCtrl, PBM_SETPOS,  (WPARAM)(100 * fPercentage), 0);
            }
            break;
        case DOWNLD_LIST_READY:
            hwndNameCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_DOMNAME);
            if(hwndNameCtrl != NULL) {
		        strOut.LoadString(IDS_DL_DONE);
		        ::SetWindowText(hwndNameCtrl, strOut);
            }
            hwndProgressCtrl = ::GetDlgItem(m_hwndListClient, IDC_UPDATE_PROGRESS);
            if(hwndProgressCtrl != NULL) {
                ::PostMessage(hwndProgressCtrl, PBM_SETPOS,  0, 0);
            }

            ::PostMessage(m_hwndListClient, CEDL_MSG_DLLIST_READY, 0, 0);
            break;
        default:
            break;
    }
    return S_OK;
}

STDMETHODIMP CImpDLEvents::OnDownloadError(BSTR sTarget, HRESULT hr, BSTR sDescr)
{
    FC_DEBUGPRINT2(_T("CEDL> Error: hr = 0x%x %s\n"),hr, CString(sDescr));
    return S_OK; 
}


