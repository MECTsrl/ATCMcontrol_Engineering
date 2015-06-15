


//----  Includes:   -------------------------------------------*
#include "helper.h"
#include "cedl.h"
#include "CEMsg.h"


//----  Static Initializations:   ----------------------------------*
//TODO  use default dl , should be read from 4ce's KAD
//{88AD4524-E2B2-4853-A081-5CDBF5732E09}
static const IID CLSID_DL = 
    { 0x88AD4524, 0xE2B2, 0x4853, { 0xA0, 0x81, 0x5C, 0xDB, 0xF5, 0x73, 0x2E, 0x09} };





class CImpDLEvents : 
	public CComObjectRoot,
	public IDispatchImpl<IDLEvents, &IID_IDLEvents, &LIBID_CEDLLib>
{
public:
	BEGIN_COM_MAP(CImpDLEvents)
   	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDLEvents)
	END_COM_MAP()

    STDMETHOD (OnDownloadState) (BSTR sMachine, BSTR sDomain, float fPercentage, CEDL_DOWNLD_STATE State)
    {
        m_DLstate = State;
        return S_OK;
    }
    STDMETHOD (OnDownloadError) (BSTR sMachine, HRESULT hr, BSTR sDescr)
    {
        g_bForceShutdown = TRUE;
        _tprintf(_T("4dl: error '%S' : reason '%S'\n"), sMachine, sDescr);
        return S_OK;
    }
private:
    CEDL_DOWNLD_STATE m_DLstate;

public:
    void clearDLState() {m_DLstate = DOWNLD_DEL;}
    CEDL_DOWNLD_STATE getDLState()const{return m_DLstate;}
};

class CImpDLMsgEvents : 
	public CComObjectRoot,
	public IDispatchImpl<_ICEMessageEvent, &IID__ICEMessageEvent, &LIBID_CEDLLib>
{
public:
	BEGIN_COM_MAP(CImpDLMsgEvents)
   	COM_INTERFACE_ENTRY(IDispatch)
   	COM_INTERFACE_ENTRY(_ICEMessageEvent)
	END_COM_MAP()

    int m_nVerbose;

    CImpDLMsgEvents() : m_nVerbose(0)   
    {
    }

    //Msg Event:
    HRESULT STDMETHODCALLTYPE OnNewMessage(BSTR sMachine, WORD wScrFacility,  HRESULT hr, BSTR sDescr)
    {
        if(hr == 0xc22003ed)//E_CSC_NO_CONNECT_TO_CONTROL HACK cannot use csc_onlineerr.mc 
            return S_OK; //HACK for d-1841

        if(FAILED(hr) || (sDescr && wcsstr(sDescr, L"error D")!=NULL))
        {
            g_bForceShutdown = true;
            _tprintf(_T("4dl: %S"), sDescr);
            _tprintf(_T("  (hr=0x%x)"), hr);
            if(sDescr[0] && sDescr[wcslen(sDescr)-1] != _T('\n'))
                _tprintf(_T("\n"));
        }
        else if(m_nVerbose>=1)
        {
            _tprintf(_T("4dl: %S"), sDescr);
            if(sDescr[0] && sDescr[wcslen(sDescr)-1] != _T('\n'))
                _tprintf(_T("\n"));
        }
        return S_OK;
    }

};


struct OnCHCBParam
{
    IDL*               pIDL;
    const FC_CStrList* p4PL;
    bool               bErr;
    int                nVerbose;
    int                nMaxGS;
};

static HRESULT OnConfigChangedCBImpl(CConnection* pCnt, void* pCtx)
{
    OnCHCBParam*      pPar = (OnCHCBParam*)pCtx;
    IDL*              pIDL = pPar->pIDL;
    const FC_CStrList*p4PL = pPar->p4PL;

    if(g_bForceShutdown)
    {
        pPar->bErr = true;
        return E_UNEXPECTED;
    }
    if(!p4PL)
        return S_OK;

    for(int i=0; i<p4PL->getUsed(); i++)
    {
        if(!DownloadParameter(pCnt, p4PL->get(i), pPar->nVerbose, pPar->nMaxGS))
        {
            g_bForceShutdown = true;
            pPar->bErr = true;
            //pIDL->Abort();  TODO does not work ...
            return E_UNEXPECTED;
        }
    }

    return S_OK;
}



        
bool DownloadControl(
    CConnection*          pcnt,
    const CMDL_INPUT*     pDLO,
    const FC_CStrList*    p4PL,
    int                   nVerbose,
    int                   nMaxGS
)
{
    HRESULT                         hr;
    int                             iRetry;
    bool                            bResult;
    IDL*                            pIDL= NULL;
    CComObject<CImpDLEvents>*       pDLEventObj = NULL;
    CComObject<CImpDLMsgEvents>*    pImpMsgEvents = NULL;
    DWORD                           dwDLEvents =0;
    DWORD                           dwDLMessages=0;
	IConnectionPoint*               pCPEvents = NULL;
	IConnectionPoint*               pCPMessages = NULL;
    IConnectionPointContainer*      pCPC = NULL;
    OnCHCBParam                     CBPar;
    

    CBPar.pIDL = NULL;
    CBPar.p4PL = NULL;
    CBPar.bErr = false;
    CBPar.nVerbose = nVerbose;
    CBPar.nMaxGS   = nMaxGS;
    bResult = false;


    //get a downloader DL interface and advise DL Event interface:
    hr = CoCreateInstance(CLSID_DL, NULL, CLSCTX_SERVER, IID_IDL,(LPVOID*)&pIDL);
    if(FAILED(hr)) 
    {
        _tprintf("\n4dl: fatal error: cannot create downloader instance: hr=%x\n", hr);
	    goto error;
    }
   
    hr = CComObject<CImpDLEvents>::CreateInstance(&pDLEventObj);
    if(FAILED(hr))
    {
        _tprintf("\n4dl: fatal error: CComObject<CImpDLEvents>::CreateInstance failed: hr=%x\n" , hr);
        goto error;
    }
    hr = CComObject<CImpDLMsgEvents>::CreateInstance(&pImpMsgEvents);
    if(FAILED(hr))
    {
        _tprintf("\n4dl: fatal error: CComObject<CImpDLMsgEvents>::CreateInstance failed: hr=%x\n" , hr);
        goto error;
    }
    pImpMsgEvents->m_nVerbose = nVerbose;


    hr = pIDL->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
    if(SUCCEEDED(hr))
      	hr = pCPC->FindConnectionPoint(IID_IDLEvents, &pCPEvents);
    if(SUCCEEDED(hr))
        hr = pCPEvents->Advise(pDLEventObj, &dwDLEvents);

    if(SUCCEEDED(hr))
        hr = pCPC->FindConnectionPoint(IID__ICEMessageEvent, &pCPMessages);
	if(SUCCEEDED(hr))
	    hr = pCPMessages->Advise(pImpMsgEvents, &dwDLMessages);

    hr = pIDL->Init(pcnt->getsTrgName(), pDLO->sGenPath, true);
    if(!SUCCEEDED(hr))
    {
        _tprintf("\n4dl: fatal error: download Init failed: hr=%x\n", hr);
        goto error;
    }

    pDLEventObj->clearDLState();


    hr = pIDL->Attach(pcnt->getIFCConnect());
    if(FAILED(hr)) 
    {
        _tprintf("4dl: fatal error: CEDL Attach failed: hr=%x\n", hr);
	    goto error;
    }


    if(p4PL && p4PL->getUsed()>0)
    {
        CBPar.pIDL = pIDL;
        CBPar.p4PL = p4PL;
        CBPar.bErr = false;
        pcnt->setOnConfigChangedCallback(OnConfigChangedCBImpl, (void*)&CBPar);
    }



    iRetry = 0;//d-1841: workaraound: problem solved in 4CE by polling 
    do         //RESOURCESTAE variable.
    {
        hr = pIDL->DownloadTarget(
                     pcnt->getsUser(), 
                     pcnt->getsPassWd(),  
                     pDLO->bDLChanges ? CEDL_DOWNLD_CHANGES : CEDL_DOWNLD_ALL, 
                     pDLO->bDLDoFlash ? DOWNLD_FLASH        : DOWNLD_NO_OPTIONS
        );
        if(hr == 0xc22003ed)//E_CSC_NO_CONNECT_TO_CONTROL HACK cannot use csc_onlineerr.mc 
           Sleep(100);
        else
            break;
    }
    while(++iRetry<50);

    if(FAILED(hr))
    {
        _tprintf("4dl: fatal error: download DownloadControl failed: hr=%x\n", hr);
	    goto error;
    }


    while(1)
    {
        if(pDLEventObj->getDLState()==DOWNLD_READY   || 
           pDLEventObj->getDLState()==DOWNLD_ABORTED ||
           pDLEventObj->getDLState()==DOWNLD_ERROR   ||
           pDLEventObj->getDLState()==DOWNLD_INTR
          )
          break;
       
        if(g_bForceShutdown)
        {
          pIDL->Abort();
          pIDL->Detach();
          goto error;
        }
        Sleep(100);
    }
    
    if(pDLEventObj->getDLState()!=DOWNLD_READY)
    {
        _tprintf("4dl: error: download for '%s' failed\n", pcnt->getTrgName());
	    goto error;
    }

    hr = pIDL->Detach();
    if(FAILED(hr)) 
    {
        _tprintf("4dl: fatal error: CEDL Detach failed: hr=%x\n", hr);
	    goto error;
    }

    hr = pIDL->Exit();
    assert(SUCCEEDED(hr));
    

    bResult = true;
error: 
    pcnt->setOnConfigChangedCallback(NULL, NULL);
    
    //cleanup:
    if(pCPEvents)
    {
        pCPEvents->Unadvise(dwDLEvents);
        pCPEvents->Release();
    }
    if(pCPMessages)
    {
        pCPMessages->Unadvise(dwDLMessages);
        pCPMessages->Release();
    }
    if(pCPC)
        pCPC->Release();

    if(pIDL)
        pIDL->Release();

    if(bResult && !CBPar.bErr)
    {
        FC_CString semf(MAX_PATH);//HACK sem file but works fine ..
        semf<<pDLO->genPath<<_T("\\project.sem");
        WritePrivateProfileString(_T("downloaded"),pcnt->getTrgName(),_T("1"), semf);
        return true;
    }
    return false;
}




int UploadBinaryDomain(
    const CConnection* pCnt,
    const TCHAR*       pszDomain,
    BYTE**             ppData,
    DWORD*             pnBytes,
    const TCHAR*       pszDestFile
)
{
    HRESULT        hr;
    ITransfer*     pIT;
    long           lKey;
    BSTR           sDomain;
    SAFEARRAY*     psa;
    void*          pSABytes;

    if(ppData)
        *ppData  = NULL;
    if(pnBytes)
        *pnBytes = 0;

    pIT = pCnt->getRawOSTransfer(&lKey);
    assert(pIT && pszDomain);
    if(!pIT || !pszDomain)
    {
        _tprintf(_T("4dl: internal error: invalid arguments to 'UploadBinaryDomain'\n"));
        return -1;
    }
    
    sDomain = FC_StringToBSTR(pszDomain);
    hr = pIT->UploadDomain(lKey, ProgramDomains, sDomain, &psa);
    FC_FREE_BSTR(sDomain);
    if(hr==0x80070002)//HACK where are the hrs defined?
        return 0;  //no error does simply not exist..

    if(FAILED(hr))
    {
        _tprintf(_T("4dl: error: upload '%s' failed: hr=0x%x\n"),pszDomain, hr);
        return -1;
    }


    // check on safearrays format:
    if (   (psa->cDims != 1)
        || (psa->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
        || (psa->cbElements != sizeof(BYTE))
        || (psa->rgsabound[0].lLbound != 0))
    {
        _tprintf(_T("4dl: internal error: upload '%s' failed: invalid SAFEARRAY format\n"),pszDomain);
        SafeArrayDestroy(psa);
        return -1;
    }


    hr = ::SafeArrayAccessData(psa, &pSABytes);
    if(FAILED(hr)) 
    {
        _tprintf(_T("4dl: internal error: upload '%s': SafeArrayAccessData failed: hr=0x%x\n"),pszDomain,hr);
        SafeArrayDestroy(psa);
        return -1;
    }

    hr = S_OK;
    if(pnBytes && ppData)
    {
        *pnBytes = psa->rgsabound[0].cElements;
        *ppData = new BYTE[*pnBytes];
        if(*ppData)
        {
            memcpy(*ppData, pSABytes, *pnBytes);
        }
        else
        {
            _tprintf(_T("4dl: fatal error: out of memory\n"));
            hr = E_FAIL;
        }
    }
    else if(pszDestFile)
    {
        FILE* pFile = _tfopen(pszDestFile, _T("wb"));
        if(!pFile)
        {
            //try to create path on first fopen error:
            FC_CString jot(MAX_PATH);
            jot.load(pszDestFile).stripFilePart();
            FC_CreateSubDir(jot);
            pFile = _tfopen(pszDestFile, _T("wb"));
        }

        if(pFile)
        {
            fwrite(pSABytes, psa->rgsabound[0].cElements, 1, pFile);
            fclose(pFile);
        }
        else
        {
            //second error:
            _tprintf(_T("4dl: error: cannot open file '%s' for writing\n"), pszDestFile);
            hr = E_FAIL;
        }
    }

    SafeArrayUnaccessData(psa);
    SafeArrayDestroy(psa);

    return hr==S_OK ? 1 : -1;
}

