
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <stdio.h>
//To use the fc_tools.lib we must supply the memory allocation 
//functions that are called from fc_tools.lib, use the fc_todebug.dll:
#include "helper.h"

static bool ReadVarSyncRaw(
    IOPCItemMgt*        pItemMgt, 
    IOPCSyncIO*         pSyncIO,
    const wchar_t*      pszItem,
    BL_IEC_TYP          want,
    BINVAL_TYP*         pResult
);






class CImpBPNotify : 
	public CComObjectRoot,
	public IDispatchImpl<
        _FCBreakpointNotification, 
        &IID__FCBreakpointNotification, 
        &LIBID_CSC_ONLINELib>
{
public:
	BEGIN_COM_MAP(CImpBPNotify)
   	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(_FCBreakpointNotification)
	END_COM_MAP()

    CConnection* pCnt;


    STDMETHOD (OnBreakpointReached) (BSTR sInstanceName, long lBreakpointNumber,
                                     long bpQuality, BSTR sTaskName)
    {
        
        return S_OK;
    }
    STDMETHOD (OnConfigurationChanged) (BSTR sAddress)
    {
        if(pCnt->m_pCBocch)
            return pCnt->m_pCBocch(pCnt, pCnt->m_pCBocchCtx);
        return S_OK;
    }
};






CConnection::CConnection(
    IFCSessions*   pISessions,
    const TCHAR*   pszTrgName,
    const TCHAR*   pszControlAddr,
    const TCHAR*   pszPassWd
)
{
    FC_CString     jot(MAX_PATH);
    FC_CString     strClsid;
    const TCHAR*   psz;

    m_pISessions = pISessions;
    
    m_strTrgName.load(pszTrgName);
    m_strControlAddr.load(pszControlAddr);
    
    psz = _tgetenv(_T("USERNAME"));
    if(!psz)
        psz = _T("");
    m_strUser.load(psz);

    psz = pszPassWd;
    if(!psz)
        psz = _T("JhrbKH785A1B09B285");
    m_strPassWd.load(psz);
    
    m_sUser            = FC_StringToBSTR(m_strUser);
    m_sPassWd          = FC_StringToBSTR(m_strPassWd);
    m_sTrgName         = FC_StringToBSTR(m_strTrgName);
    m_sControlAddr     = FC_StringToBSTR(m_strControlAddr);
    m_pIConnect        = NULL;
    m_pICPBPNotify     = NULL;
    m_pIBPNotifyObj    = NULL;
    m_dwBPNotifyCookie = 0;



    psz = _tgetenv(_T("COMPUTERNAME"));
    if(!psz)
        psz = _T("localhost");
    m_sCompName = FC_StringToBSTR(psz);

    psz = m_strControlAddr;
    psz = jot.clear().appendUpTo(psz, _T(","));
    m_sServerPCName = jot.toBSTR();

    psz = jot.clear().appendUpTo(psz, _T(","));
    if(!jot.isEmpty())
        strClsid<<_T("{")<<jot<<_T("}");

    m_sCustCLSID    = strClsid.toBSTR();
    m_sCustInit     = FC_StringToBSTR(psz);
    
    if(m_sCustCLSID[0]==0)
    {
        memset(&m_custCLSID, 0, sizeof(m_custCLSID));
    }
    else
    {
        if(FAILED(IIDFromString(m_sCustCLSID, &m_custCLSID)))
        {
            _tprintf(_T("4dl: error: invalid connect string '%s': CLSID syntax\n"), strClsid.psz());
            g_bForceShutdown = true;
        }
    }

    m_pOSMgt     = NULL;
    m_lOSKey     = 0;
    m_pOSOPCSrv  = NULL;
    m_pITransfer = NULL;

    m_pSyncIO    = NULL;
    m_pItemMgt   = NULL;
    m_pICtrlMgt  = NULL;
    m_lCtrlMgtKey= 0;
    m_bCtrlLoggedIn = false;
    m_pCBocch    = NULL;
    m_pCBocchCtx = NULL;
}
CConnection::~CConnection()
{
    closeRawCtrlMgt();
    closeCommissioningSession();
    closeRawVarSession();
    closeRawComSession();

    FC_FREE_BSTR(m_sUser);
    FC_FREE_BSTR(m_sPassWd);
    FC_FREE_BSTR(m_sTrgName);
    FC_FREE_BSTR(m_sControlAddr);
    assert(!m_pIConnect && !m_pICPBPNotify && !m_pIBPNotifyObj && m_dwBPNotifyCookie==0);
    assert(!m_pOSMgt && m_lOSKey==0 && !m_pOSOPCSrv && !m_pITransfer && 
        !m_pICtrlMgt && m_lCtrlMgtKey==0 && !m_pSyncIO && !m_pItemMgt);

    FC_FREE_BSTR(m_sServerPCName);
    FC_FREE_BSTR(m_sCustCLSID);
    FC_FREE_BSTR(m_sCustInit);
}




bool CConnection::openCommissioningSession()
{
    HRESULT                     hr;
    int                         nTry;
    LPUNKNOWN                   pUnk = NULL;
    CSC_SESSION_STATE           state;
    IConnectionPointContainer*  pICPC = NULL;


    if(m_pIConnect || m_pICPBPNotify || m_pIBPNotifyObj || m_dwBPNotifyCookie!=0)
    {
        _tprintf("4dl: fatal internal error: openCommissioningSession for '%s' already done\n", m_strControlAddr);
	    return false;
    }

    _tprintf("4dl: connect to %s at {%s}\n", m_strTrgName, m_strControlAddr);
    

    if(FAILED(hr = m_pISessions->OpenCommissioningSession(m_sControlAddr, &pUnk))) 
    {
        _tprintf("4dl: fatal error: OpenCommissioningSession for '%s' failed: hr=0x%x\n", m_strControlAddr, hr);
	    return false;
    }
    hr = pUnk->QueryInterface(IID_IFCConnect,(void **)&m_pIConnect);
    FC_RELEASE_PTR(pUnk);
    if(FAILED(hr)) 
    {
        _tprintf("4dl: fatal error: QueryInterface IID_IFCConnect failed for '%s': hr=0x%x\n", m_strControlAddr, hr);
        FC_RELEASE_PTR(m_pIConnect);
	    return false;
    }




    hr = m_pIConnect->Connect(m_sUser, m_sPassWd);
    if(FAILED(hr)) 
    {
        _tprintf("4dl: fatal error: IID_IFCConnect Connect failed for '%s': hr=0x%x\n", m_strControlAddr, hr);
        FC_RELEASE_PTR(m_pIConnect);
	    return false;
    }
    //give up connecting after 60 seconds. (consider slow RAS connection!)
    //try 600 times and sleep 100ms:
    for(nTry=1, state=not_connected; nTry<601 && state!=exclusive_established && !g_bForceShutdown; nTry++)
    {
        hr = m_pIConnect->get_SessionState(&state);
        if(FAILED(hr)) 
        {
            _tprintf("4dl: fatal error: IID_IFCConnect::get_SessionState failed for '%s': hr=0x%x\n", m_strControlAddr, hr);
            break;
        }
        
        if(nTry>10)
        {
            if(nTry%50==0)
                _tprintf("\n");
            _tprintf(".");
        }
        Sleep(100);
    }

    if(nTry>10)
        _tprintf("\n");


    if(state!=exclusive_established) 
    {
        if(!g_bForceShutdown)
            _tprintf("4dl: error: cannot debug connect to %s (other user logged in?)\n", m_strControlAddr);
        FC_RELEASE_PTR(m_pIConnect);
	    return false;
    }

   //   sResStateVar = FC_StringToBSTR(_T("DLoadGUID"));


    //add a BP-Notify IF sink:
    hr = m_pIConnect->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pICPC);
    if(SUCCEEDED(hr))
    {
      	hr = pICPC->FindConnectionPoint(IID__FCBreakpointNotification, &m_pICPBPNotify);
        if(SUCCEEDED(hr))
        {
            hr = CComObject<CImpBPNotify>::CreateInstance(&m_pIBPNotifyObj);
            if(SUCCEEDED(hr))
            {
                m_pIBPNotifyObj->AddRef();
                m_pIBPNotifyObj->pCnt = this;
                hr = m_pICPBPNotify->Advise(m_pIBPNotifyObj, &m_dwBPNotifyCookie);
            }
        }
    }
    FC_RELEASE_PTR(pICPC);
    if(FAILED(hr)) 
    {
         FC_RELEASE_PTR(m_pIBPNotifyObj);
         FC_RELEASE_PTR(m_pICPBPNotify);
         FC_RELEASE_PTR(m_pIConnect);
         m_dwBPNotifyCookie = 0;
        _tprintf("4dl: fatal internal error: failed to set a BP-Notify connection point interface for '%s': hr=0x%x\n", m_strControlAddr, hr);
	    return false;
    }

    return true;
}


void CConnection::closeCommissioningSession()
{
    HRESULT hr;

    if(!m_pIConnect)
    {
        assert(!m_pICPBPNotify && !m_pIBPNotifyObj && m_dwBPNotifyCookie==0);
        return;
    }


   	if(m_pICPBPNotify)
        m_pICPBPNotify->Unadvise(m_dwBPNotifyCookie);

    m_dwBPNotifyCookie = 0;
    FC_RELEASE_PTR(m_pICPBPNotify);
    FC_RELEASE_PTR(m_pIBPNotifyObj);

    hr = m_pIConnect->Disconnect();
    if(FAILED(hr)) 
        _tprintf("4dl: error: IID_IFCConnect Disconnect failed for '%s': hr=0x%x\n", m_strControlAddr, hr);

    FC_RELEASE_PTR(m_pIConnect);
}




bool CConnection::openRawComSession()
{
    HRESULT           hr;
    MULTI_QI          queue[1];
    COSERVERINFO      serverInfo;
	DWORD             clsctx;
    IUnknown*         pUnk;

    
    if(!openRawVarSession())//HACK just to poll the resourcestate
        return false;

    if(m_pOSMgt && m_pITransfer)
        return true;

    assert(!m_pOSMgt && !m_pITransfer);
    m_pOSMgt = NULL;
    m_pITransfer = NULL;


    queue[0].pItf = NULL;
    queue[0].hr = 0;
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;
	serverInfo.pwszName = m_sServerPCName;
	serverInfo.pAuthInfo = 0;
	clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;

    if(m_sCustCLSID[0]==0)
    {
        static const CLSID CLSID_CommissioningMgt = {0x3E99950A,0x9502,0x4a7c,{0xB0,0xAE,0x6B,0x3C,0x4C,0x01,0x1D,0xF4}};
        static const CLSID CLSID_OPCServer        = {0x5425B8A4,0x754D,0x496a,{0xAF,0x31,0x7D,0x78,0xA5,0x30,0xE5,0x8D}};

        //Domain Transfer IF:
        queue[0].pIID = &IID_ICommissioningMgt;
        hr = ::CoCreateInstanceEx(CLSID_CommissioningMgt, NULL, 
            clsctx, &serverInfo, FC_ARRAY_LEN(queue), queue);

        if(SUCCEEDED(hr) && SUCCEEDED(queue[0].hr))
            m_pOSMgt = (ICommissioningMgt*) queue[0].pItf;
    }
    else
    {
        //custom connect case
        queue[0].pIID = &IID_ICustomConnect;
   
        hr = ::CoCreateInstanceEx(m_custCLSID, NULL, 
            clsctx, &serverInfo,FC_ARRAY_LEN(queue), queue);

        if(SUCCEEDED(hr))
        {
            ICustomConnect* pCustConn = (ICustomConnect*)queue[0].pItf;

            //Domain Transfer IF:
            hr = pCustConn->CreateCommissioningMgt(m_sCustInit, &pUnk);
            if(SUCCEEDED(hr))
            {
                hr = pUnk->QueryInterface(IID_ICommissioningMgt,(void**)&m_pOSMgt);
                FC_RELEASE_PTR(pUnk);
            }
            FC_RELEASE_PTR(pCustConn);
        }
    }


    if(FAILED(hr))
    {
        _tprintf("4dl: error: DCOM connection to OPC-Server failed '%s': hr=0x%x\n", m_strControlAddr, hr);
        return false;
    }

    hr = m_pOSMgt->Login(m_sUser, m_sPassWd, m_sCompName, &m_lOSKey, &pUnk);
    if(FAILED(hr)) 
    {
        _tprintf("4dl: error: Login failed for '%s': hr=0x%x\n", m_strControlAddr, hr);
        FC_RELEASE_PTR(m_pOSMgt);
        return false;
    }

    hr = pUnk->QueryInterface(IID_ITransfer, (LPVOID*)&m_pITransfer);
    pUnk->Release();
    if(FAILED(hr)) 
    {
        _tprintf("4dl: error: login failed for '%s': hr=0x%x\n", m_strControlAddr, hr);
        FC_RELEASE_PTR(m_pOSMgt);
        return false;
    }

    return true;
}



bool CConnection::openRawVarSession()
{
    HRESULT           hr;
    MULTI_QI          queue[1];
    COSERVERINFO      serverInfo;
	DWORD             clsctx;

    if(m_pOSOPCSrv)
    {
        assert(m_pSyncIO && m_pItemMgt && m_pICtrlMgt);
        return true;
    }

    queue[0].pItf = NULL;
    queue[0].hr = 0;
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;
	serverInfo.pwszName = m_sServerPCName;
	serverInfo.pAuthInfo = 0;
	clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;

    if(m_sCustCLSID[0]==0)
    {
        static const CLSID CLSID_OPCServer        = {0x5425B8A4,0x754D,0x496a,{0xAF,0x31,0x7D,0x78,0xA5,0x30,0xE5,0x8D}};

        //Variable Access IF:
        queue[0].pIID = &IID_IOPCServer;
	    hr = CoCreateInstanceEx(CLSID_OPCServer, NULL, 
            clsctx, &serverInfo, FC_ARRAY_LEN(queue), queue);

        if(SUCCEEDED(hr) && SUCCEEDED(queue[0].hr))
        {
            m_pOSOPCSrv = (IOPCServer*) queue[0].pItf;
        }
    }
    else
    {
        //custom connect case
        queue[0].pIID = &IID_ICustomConnect;
   
        hr = ::CoCreateInstanceEx(m_custCLSID, NULL, 
            clsctx, &serverInfo,FC_ARRAY_LEN(queue), queue);

        if(SUCCEEDED(hr))
        {
            IUnknown*       pUnk;
            ICustomConnect* pCustConn = (ICustomConnect*)queue[0].pItf;

            //Variable Access IF:
            hr = pCustConn->CreateVarSession(m_sCustInit, &pUnk);
            if(SUCCEEDED(hr)) 
            {
                hr = pUnk->QueryInterface(IID_IOPCServer,(void**)&m_pOSOPCSrv);
                FC_RELEASE_PTR(pUnk);
            }
            if(SUCCEEDED(hr)) 
            {
    			hr = pCustConn->CreateControlManagement(m_sCustInit, &pUnk);
                if(SUCCEEDED(hr)) 
                {
                    hr = pUnk->QueryInterface(IID_IControlMgt,(void**)&m_pICtrlMgt);
                    FC_RELEASE_PTR(pUnk);
                }
            }

            FC_RELEASE_PTR(pCustConn);
        }
    }

    if(FAILED(hr))
    {
        _tprintf("4dl: error: DCOM connection to OPC-Server failed '%s': hr=0x%x\n", m_strControlAddr, hr);
        return false;
    }

    if(!createSyncVarIF(L"4DL", &m_pSyncIO, &m_pItemMgt))
        FC_RELEASE_PTR(m_pOSOPCSrv);

    if(!checkResourceState())
    {
        closeRawVarSession();
        return false;
    }

    return true;
}


IOPCServer* CConnection::getRawIOPCServer(
    IOPCSyncIO** ppSyncIO, IOPCItemMgt** ppItemMgt)const
{
    assert(!m_pOSOPCSrv || (m_pSyncIO && m_pItemMgt));
    if(ppSyncIO)
       *ppSyncIO = m_pSyncIO;
    if(ppItemMgt)
       *ppItemMgt = m_pItemMgt;

    return m_pOSOPCSrv;
}
ITransfer* CConnection::getRawOSTransfer(long* plKey)const
{
    *plKey = m_lOSKey; 
    return m_pITransfer;
}

IControlMgt* CConnection::getCtrlMgt(long* plKey)
{
    HRESULT hr;
    openRawVarSession();
    if(!m_pICtrlMgt)
        return NULL;
    *plKey = 0;
    if(!m_bCtrlLoggedIn)
    {
        hr = m_pICtrlMgt->Login(m_sUser, m_sPassWd, &m_lCtrlMgtKey);
        if(FAILED(hr))
        {
           _tprintf("4dl: error: login for start/stop resouce/task failed: hr=0x%x\n", hr);
           return NULL;
        }
    }

    m_bCtrlLoggedIn = true;
    *plKey = m_lCtrlMgtKey;
    return m_pICtrlMgt;
}




bool CConnection::createSyncVarIF(
    const wchar_t* pszGroupName,
    IOPCSyncIO**   ppSyncIO,
    IOPCItemMgt**  ppItemMgt 
)
{
    HRESULT             hr;
    LONG                timeBias = 0;
    FLOAT               fltDeadband = 0.0;
    DWORD               dwLCID = MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), SORT_DEFAULT);
    OPCHANDLE           hGroup = 0;
    DWORD               dwRevUpdateRate = 0;
    

    *ppSyncIO = NULL;
    *ppItemMgt= NULL;

    if(!m_pOSOPCSrv)
    {
        assert(!"CConnection::createSyncVarIF: not connected");
        return false;
    }


    hr = m_pOSOPCSrv->AddGroup(
            pszGroupName,         //[in, string]        LPCWSTR     szName,                     
            FALSE,                //[in]                BOOL        bActive,                    
            0,                    //[in]                DWORD       dwRequestedUpdateRate,
            NULL,                 //[in]                OPCHANDLE   hClientGroup,
            &timeBias,            //[unique, in]        LONG*       pTimeBias,
            &fltDeadband,         //[unique, in]        FLOAT*      pPercentDeadband,
            dwLCID,               //[in]                DWORD       dwLCID,
            &hGroup,              //[out]               OPCHANDLE*  phServerGroup,
            &dwRevUpdateRate,     //[out]               DWORD*      pRevisedUpdateRate,
            IID_IOPCItemMgt,      //[in]                REFIID      riid, 
            (LPUNKNOWN*)ppItemMgt //[out, iid_is(riid)] LPUNKNOWN * ppUnk
    );

    if(FAILED(hr))
    {
        _tprintf(_T("4dl: error: 'AddGroup' failed for OPC-Server at '%s': hr=0x%x\n"),
            getsControlAddr(), hr);
        return false;
    }

    hr = (*ppItemMgt)->QueryInterface(IID_IOPCSyncIO, (void**)ppSyncIO);
    if(FAILED(hr))
    {
        FC_RELEASE_PTR(*ppItemMgt);
        _tprintf(_T("4dl: error: QueryInterface(IID_IOPCSyncIO) failed for OPC-Server at '%s': hr=0x%x\n"),
            getsControlAddr(), hr);
    }

    return true;
}









void CConnection::closeRawVarSession()
{
    FC_RELEASE_PTR(m_pSyncIO);
    FC_RELEASE_PTR(m_pItemMgt);
    FC_RELEASE_PTR(m_pOSOPCSrv);
}
void CConnection::closeRawCtrlMgt()
{
    HRESULT hr;
    if(m_pICtrlMgt && m_bCtrlLoggedIn)
    {
        hr = m_pICtrlMgt->Logout(m_lCtrlMgtKey);
        assert(SUCCEEDED(hr));
    }

    m_bCtrlLoggedIn = false;
    m_lCtrlMgtKey   = 0;
    FC_RELEASE_PTR(m_pICtrlMgt);
}

void CConnection::closeRawComSession()
{
    HRESULT hr;
    FC_RELEASE_PTR(m_pITransfer);
    if(m_pOSMgt)
    {
        hr = m_pOSMgt->Logout(m_lOSKey);
        assert(SUCCEEDED(hr));
        FC_RELEASE_PTR(m_pOSMgt);
    }
    m_lOSKey = 0;
}

bool CConnection::checkResourceState()
{
    bool          bOk;
    int           nTry;
    BINVAL_TYP    val;
    InitBinVal(&val);

    if(!m_pItemMgt || !m_pSyncIO)
        return false;

    do
    {
        bOk = ReadVarSyncRaw(m_pItemMgt, m_pSyncIO, L"__srt0.__system.ResourceState", BL_DINT, &val);
        bOk = bOk && (val.aDINT > -4);
        if(!bOk)
        {
            nTry++;
            Sleep(300);
        }
    }
    while(!bOk && nTry<40);

    if(!bOk)
    {
        _tprintf(_T("4dl: error: failed to connect to control '%s'\n"),
            getControlAddr());
    }

    return bOk;
}


static bool ReadVarSyncRaw(
    IOPCItemMgt*        pItemMgt, 
    IOPCSyncIO*         pSyncIO,
    const wchar_t*      pszItem,
    BL_IEC_TYP          want,
    BINVAL_TYP*         pResult
)
{
    HRESULT              hr;
    OPCITEMDEF           opcItems;
    OPCHANDLE            hServer;

    FreeBinVal(pResult);


    opcItems.szAccessPath = NULL;
    opcItems.szItemID = (wchar_t*)pszItem;
    opcItems.bActive = false;
    opcItems.dwBlobSize = 0;
    opcItems.pBlob = NULL;
    opcItems.vtRequestedDataType = BLTypToVariantTyp(want);
    opcItems.wReserved = 0;
    opcItems.hClient = NULL;

    {
        HRESULT*       pErrorAI  = NULL;
        OPCITEMRESULT* pResultAI = NULL;

        hr = pItemMgt->AddItems(1, &opcItems, &pResultAI, &pErrorAI);
        if(FAILED(hr))
            return false;

        hr = pErrorAI[0];
        hServer = pResultAI[0].hServer;
        CoTaskMemFree(pErrorAI);
        CoTaskMemFree(pResultAI);
        if(FAILED(hr))
            return FALSE;
    }


    {
        OPCITEMSTATE* pItemVAL = NULL;
        HRESULT*      pErrorR = NULL;
        hr = pSyncIO->Read(OPC_DS_DEVICE, 1, &hServer, &pItemVAL, &pErrorR);

        if(SUCCEEDED(hr) && SUCCEEDED(pErrorR[0]) && pItemVAL[0].wQuality>=192)
            VariantToBinVal(&pItemVAL[0].vDataValue, want, pResult);

        if(pErrorR)
            CoTaskMemFree(pErrorR);
        if(pItemVAL)
        {
            VariantClear(&pItemVAL[0].vDataValue);
            CoTaskMemFree(pItemVAL);
        }
    }


    {
        HRESULT* pErrorRI = NULL;    
        hr = pItemMgt->RemoveItems(1, &hServer, &pErrorRI);
        //ignore this hr...
        if(pErrorRI)
            CoTaskMemFree(pErrorRI);
    }    
    return true;
}

