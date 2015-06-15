
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedlerror.h"
#include <direct.h>
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "dlstlist.h"
#include "utilif.h"
#include "DLUpdateDlg.h"
#include "DLStateDlg.h"
#include "cedl.h"

/////////////////////////////////////////////////////////////////////////////
// CImpCEDL

CImpCEDL::CImpCEDL(void)
{
	m_hEvntAbort = NULL;
	m_pStationList = NULL;
	m_bInit = FALSE;
    m_pEvents = NULL;
    m_dwEvents = 0;
}

HRESULT CImpCEDL::FinalConstruct(void)
{
	return S_OK;
}

void CImpCEDL::FinalRelease(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

	Cleanup();
    AfxDaoTerm();
}

void CImpCEDL::CreateEvntSink(void) throw()
{

    if(m_pEvents) {//already an event interface created ?
        return; //yes, don't worry just use the existing one.
    }
    //initalize Event interface now
	HRESULT hr = CComObject<CImpDLEvents>::CreateInstance(&m_pEvents);
	if(FAILED(hr)) {
        ComThrow(GetObjectCLSID(),__uuidof(IDLEvents),hr);
	}

	m_pEvents->AddRef(); //stay with one reference

    hr = AtlAdvise(GetUnknown(),m_pEvents->GetUnknown(),
		IID_IDLEvents, &m_dwEvents);
	
    if(FAILED(hr)) {
		FC_DEBUGPRINT1(_T("CEDL> Advise failed hr=0x%x\n"),hr);
        m_dwEvents = 0;
		m_pEvents->Release();
        m_pEvents = NULL;
        ComThrow(GetObjectCLSID(),__uuidof(IDLEvents),hr);
	}

}

void CImpCEDL::DestroyEvntSink(void)
{
    if(m_pEvents) {
        HRESULT hr = AtlUnadvise(GetUnknown(),IID_IDLEvents, m_dwEvents);
        
        m_pEvents->Release();
        m_pEvents = NULL;            
    }

}


STDMETHODIMP CImpCEDL::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDL,&IID_IDLEx,&IID_IDLProperties
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CImpCEDL::DownloadSelect(long lHWND,BSTR sUser)
{
   	HRESULT hr = S_OK;
    HWND hwndParent = NULL;
    CWnd *pWnd = NULL;
	CStation *pStation = NULL;
	CDLUpdateDlg* pDlg = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_DEBUGPRINT1(_T("CEDL> DownloadSelect: thread id=0x%x\n"),::GetCurrentThreadId());

	try {
	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> DownloadSelect: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);

	    }
		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> DownloadSelect: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}

        pWnd = new CWnd();
        if(!pWnd) {
            AfxThrowMemoryException();
        }
        
        hwndParent = reinterpret_cast<HWND>(lHWND);
        if(!::IsWindow(hwndParent)) {
            hwndParent = NULL;
        }

        pWnd->Attach(hwndParent);
        pDlg = new CDLUpdateDlg(pWnd);
		if(!pDlg) {
			AfxThrowMemoryException();
		}
    
		pDlg->SetStation(pStation);
		assert(m_pEvents != NULL);
		pDlg->SetEventPtr(m_pEvents);
		pDlg->DoModal();
		
	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    if(pWnd) {
        pWnd->Detach();
        delete pWnd;
    }
    
    if(pDlg) {
		delete(pDlg);
	}

    FC_RELEASE_PTR(pStation);
	return hr;
}

STDMETHODIMP CImpCEDL::Init(BSTR sTargetName, BSTR sGenPath,BOOL bSilent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr = S_OK;
	CString strPath,strSrc,strDest,strTarget;
	CFileStatus rStatus;
    PIDLEvents pEvntSink;
    PICEMsg pMsgSink;
    IErrorInfo* pInfo = NULL;
    DWORD dwErr = 0UL;
	
	// Enforce using of DAO36 and Jet40 by mfc42.dll
	AfxGetModuleState()->m_dwVersion = 0x0601;
    //EF don't call this: causes an zombie process! AfxDaoInit();

	FC_DEBUGPRINT1(_T("CEDL> Init: thread id=0x%x\n"),::GetCurrentThreadId());

    try { 
        if(!sTargetName || !sGenPath) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_POINTER);
        }

        strTarget = sTargetName;
        strPath = sGenPath;
	    strSrc = strPath + _T('\\') + strTarget;

        //create own event sink - not created if already done
        CreateEvntSink();

        //check for the target specific gen path first
	    if(!CFile::GetStatus(strSrc,rStatus)) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOGENPATH,strSrc,strTarget);
        }

        //create downloader directory within target specific gen path if
        //not already created
        strSrc = strSrc + _T('\\') + CEDL_DOMDIR;

	    if(!CFile::GetStatus(strSrc,rStatus)) {
		    //create a new dir
            if(_tmkdir(strSrc)) {
                //on error throw file exception with path
                dwErr = GetLastError();
                CFileException::ThrowOsError(dwErr,strSrc);
		    }
        }

        CStationList::CopyDatabase(strTarget,sGenPath,false);
        //get the path of the current database
        CStationList::GetDBPath(strDest,strTarget,sGenPath);
    

        pEvntSink = dynamic_cast <CProxyIDLEvents<CImpCEDL>*>(this);
        assert(pEvntSink);
        pMsgSink = dynamic_cast <CProxy_ICEMessageEvent<CImpCEDL>*>(this);    
        assert(pMsgSink);

        /* TODO CHECK OBSOLETE
        if(m_pStationList) { //station list there - reinitialization
            //station list there but represents not the same project ?
            if(strGUID != m_pStationList->GetProjectGuid()) {
                /*  not the same project return downloader already inited.
                    a different project requires a call to Exit before reinit
                    downloader 
                return E_CE_DL_ALREADY_INITED;
            }

            /*  iff any download is pending and state not CEDL_STAT_IDLE
                reinitialization is not possible

            if(m_pStationList->IsDownloadPending()) {
                return E_CE_DL_PENDING;
            }

            //check wether downloader database is open.
            if(!m_pStationList->IsDBOpen()) { //reopen it iff not open
	            m_pStationList->OpenDB();
            }

            if(!m_pStationList->CheckDBVersion()) {
                //close db first
                m_pStationList->CloseDB();
                CStationList::CopyDatabase(sGenPath,true);
                //reopen
                m_pStationList->OpenDB();
            }

        }
        */

	    m_pStationList = new CStationList(strPath,strDest);
	    if(!m_pStationList) {
		    AfxThrowMemoryException();
	    }

	    m_pStationList->OpenDB();

        if(!m_pStationList->CheckDBVersion()) {
            //close db first
            m_pStationList->CloseDB();
            CStationList::CopyDatabase(strTarget,sGenPath,true);
            //reopen
            m_pStationList->OpenDB();
        }
         
        m_pStationList->BuildStationList(sTargetName,pEvntSink,pMsgSink, bSilent?true:false);
    }
    catch(_com_error err) {
        Fire_OnNewMessage(NULL,E_FACILITY_CEDL,err.Error(),err.Description());
        ComCatch(err);
		hr = err.Error();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
	catch(CDaoException* e) {
        hr = e->m_scode;

        strDest.Empty();
        if(e->m_pErrorInfo) {
            strDest = e->m_pErrorInfo->m_strDescription;
        }

        if(strDest.IsEmpty()) {
            strDest.Format(IDS_CEDL_DEFERROR,hr);
        }

        ComErrorInfo(GetObjectCLSID(),__uuidof(IDL),&pInfo,strDest);
        _com_error err(hr,pInfo);
        
        Fire_OnNewMessage(NULL,E_FACILITY_CEDL,err.Error(),err.Description());
        e->Delete();
        ComCatch(err);
    }
	catch(CFileException * e) {
		hr = HRESULT_FROM_WIN32(e->m_lOsError);
        strDest.Empty();
        strSrc.Empty();

        FormatMsg(strSrc.GetBuffer(_MAX_PATH),hr);
        strSrc.ReleaseBuffer();

        if(strSrc.IsEmpty()) {
            strSrc.Format(IDS_CEDL_DEFERROR,hr);
        }
        strSrc.TrimRight(_T("\r\n"));

        FormatMsg(strDest.GetBuffer(_MAX_PATH),E_CE_DL_FILE_ERROR,strTarget,
            strSrc,e->m_strFileName);
        strDest.ReleaseBuffer();

        ComErrorInfo(GetObjectCLSID(),__uuidof(IDL),&pInfo,strDest);
        _com_error err(hr,pInfo);
        
        Fire_OnNewMessage(NULL,E_FACILITY_CEDL,err.Error(),err.Description());
        e->Delete();
        ComCatch(err);
	}

    if(FAILED(hr)) {
        Cleanup();
        m_bInit = FALSE;
        return hr;
    }
    
    m_bInit = TRUE;
	return hr;
}

STDMETHODIMP CImpCEDL::Exit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if(!m_bInit) {
        return S_OK;
    }

    Cleanup();
    m_bInit = FALSE;
	return S_OK;
}

void CImpCEDL::Cleanup()
{
	DestroyEvntSink();

	if(m_pStationList) {
		delete m_pStationList;
		m_pStationList = NULL;
	}
}

STDMETHODIMP CImpCEDL::DownloadTarget(/*[in]*/BSTR sUser,/*[in]*/BSTR sPassWord,
                                      /*[in]*/CEDL_DOWNLD_TYPE Type,/*[in]*/CEDL_DOWNLD_OPTIONS options)
{
	HRESULT hr = S_OK;
	CStation *pStation = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	try {
        if(Type == CEDL_DOWNLD_INVALID || Type == CEDL_DOWNLD_SELECT) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_INVALIDARG);
        }

        if(sUser == NULL || sPassWord == NULL) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_POINTER);
        }

	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> Attach: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);
	    }

		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> DownloadTarget: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}

        if (pStation->IsRequestPending()) {
            FC_DEBUGPRINT(_T("CEDL> DownloadControl: Download Pending\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_PENDING,"");
        }

		pStation->SetSignal(DOWN_LD);
        pStation->ResetSignal(BUILD_DL_LIST);
		pStation->ResetSignal(CHANGED);
		pStation->ResetSignal(PRJ_NEW);
		pStation->ResetSignal(PRJ_CHANGED);
        pStation->SetOptions(options);


		if(Type == CEDL_DOWNLD_CHANGES) { //only iff changes not iff download all
            pStation->ResetSignal(DOWN_LD_ALL);
		}
		else { //type == CEDL_DOWNLD_ALL
			if(!pStation->IsOptionSet(DOWNLD_FLASH_ONLY)) {
				pStation->SetSignal(DOWN_LD_ALL);
				pStation->MarkDomainsNew();
			}
		}

        pStation->FireStartDownload();
        pStation->SetState(CEDL_STAT_WAIT_CONNECT);
        hr = pStation->DoStateMachine();
	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        FC_DEBUGPRINT1(_T("CEDL> DownloadControl: Cycle 1 failed 0x%x\n"),hr);
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    FC_RELEASE_PTR(pStation);
	return hr;
}

STDMETHODIMP CImpCEDL::Abort()
{
	HRESULT hr = S_OK;
	CStation *pStation = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE (AfxGetStaticModuleState ())
	FC_DEBUGPRINT1(_T("CEDL> Abort: thread id=0x%x\n"),::GetCurrentThreadId());

	try{
	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> Abort: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);

	    }
		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> Abort: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}

		hr = pStation->Abort(); //adaption cycle 1 and 2
		if(FAILED(hr)) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),hr);
		}

	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    FC_RELEASE_PTR(pStation);
    return hr;
}


STDMETHODIMP CImpCEDL::ShowDownloadState (/*[in]*/long lHWND)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ())
	HRESULT hr = S_OK;
    CComBSTR sDefault = _T("");
	
    try {
	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> ShowDownloadState: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);
	    }

    	CDLStateDlg dialog (m_pStationList, m_pEvents, AfxGetMainWnd());
    	dialog.DoModal ();
    }
    catch(_com_error err) {
        Fire_OnNewMessage(sDefault,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }
	return hr;
}

STDMETHODIMP CImpCEDL::Attach(LPUNKNOWN pUnk)
{
	HRESULT hr = S_OK;
	CStation *pStation = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	try {
        if(!pUnk){
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_POINTER);
        }

	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> Attach: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);

	    }

		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> Attach: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}

		pStation->Attach(pUnk);

	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    FC_RELEASE_PTR(pStation);
	return hr;
}

STDMETHODIMP CImpCEDL::Detach()
{
	HRESULT hr = S_OK;
	CStation *pStation = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	try {
	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> Detach: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);
	    }

		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> Detach: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}

        pStation->Detach();

	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    FC_RELEASE_PTR(pStation);
	return hr;
}

STDMETHODIMP CImpCEDL::GetTargetDLVersion(/*[out]*/BSTR* psVersion,/*[out]*/BSTR* psProjectName)
{
	HRESULT hr = S_OK;
	CString str;
	CStation *pStation = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	try {
        if(!psVersion || !psProjectName) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_INVALIDARG);
        }

        *psVersion = NULL;
        *psProjectName = NULL;

	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> Detach: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);
	    }

		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> Detach: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}


        pStation->GetDLGUID(CEDL_VERSIONDOMAIN,str);
        if(!str.IsEmpty()) {
            *psVersion = str.AllocSysString();
        }
        str = pStation->GetProjectName();
        
        if(!str.IsEmpty()) {
            *psProjectName = str.AllocSysString();
        }
	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    FC_RELEASE_PTR(pStation);
	return hr;
}

STDMETHODIMP CImpCEDL::get_Address(BSTR *psAddress)
{
    //TODO sMachine is the real address of control
	HRESULT hr = S_OK;
	CString str;
	CStation *pStation = NULL;
    CComBSTR sTargetName = _T("");

	AFX_MANAGE_STATE (AfxGetStaticModuleState ())

	try {
        if(!psAddress) {
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_INVALIDARG);
        }

        *psAddress = NULL;

	    if (!m_bInit) {
		    FC_DEBUGPRINT (_T ("CEDL> Detach: not initialized\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_NOT_INITED);
	    }

		pStation = m_pStationList->FindStationByIndex(0,true);
		if(!pStation) {
			FC_DEBUGPRINT(_T("CEDL> Detach: Station not found\n"));
            ComThrow(GetObjectCLSID(),__uuidof(IDL),E_CE_DL_STATION_NOT_FOUND);
		}

        str = pStation->GetAddress();
        *psAddress = str.AllocSysString();

	}
	catch(CMemoryException* e) {
		hr = E_OUTOFMEMORY;
		e->Delete();
	}
    catch(_com_error err) {
        Fire_OnNewMessage(sTargetName,E_FACILITY_CEDL,err.Error(),err.Description());
        hr = ComCatch(err);
    }

    FC_RELEASE_PTR(pStation);
	return hr;
}

