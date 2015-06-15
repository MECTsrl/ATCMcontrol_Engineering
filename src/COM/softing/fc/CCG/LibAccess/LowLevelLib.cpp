#include "stdafx.h"
#include "CG_4CLAccMsg.h"//mc


#define CL_4CL_VERSION _T("ATCMControl V2.0 Library")

static FC_ARSTAT NodeListToSafeArray(
    FC_CGArray<FC_CArNode*>* pNList, 
    SAFEARRAY**              ppsa
);






/////////////////////////////////////////////////////////////////////
// I4CLOpen
/////////////////////////////////////////////////////////////////////
STDMETHODIMP C4CLAccessLib::Open(
    /*[in]*/         BOOL          bWrite, 
    /*[in, string]*/ BSTR          sFileName,
    /*[out,string]*/ BSTR*         psErrorDescr,
    /*[out]*/        I4CLAccess**  ppIAcc
)
{
    USES_CONVERSION;
    HRESULT                 hr;
    CComObject<CLibObject>* pLib;

    *psErrorDescr = NULL;
    *ppIAcc       = NULL;


    // create instance of library-accessor COM-object and open the archive:
    hr = pLib->CreateInstance(&pLib);
    pLib->AddRef();

    if(SUCCEEDED(hr))
    {
        hr = pLib->openAr(OLE2T(sFileName), bWrite?true:false, psErrorDescr);
        if(hr == S_OK)
            *ppIAcc = pLib;
        else
            pLib->Release();
    }
    else
    {
        FC_CString jot;

        assert(!"CreateInstance for I4CLAccess failed");
        jot.fmtSysErr(hr);
        *psErrorDescr = jot.toBSTR();
    }
    return hr;
}

STDMETHODIMP C4CLAccessLib::Close(I4CLAccess* pIAccess)
{
    CLibObject* pLib;

    pLib = (CLibObject*)(pIAccess);
    return pLib->closeAr(NULL);
}




















HRESULT CLibObject::getPouDir(
    BSTR        sPOUName, 
    BSTR        sTargetType, 
    FC_CArDir** ppPOU,
    BSTR*       psErrDescr,
    bool        bForceNew
)
{
    USES_CONVERSION;
    FC_ARSTAT   ret;
    FC_CArDir* pPOU;
    
    *ppPOU = NULL;
    
    if((ret = m_pRoot->getSubDir(OLE2T(sPOUName), ppPOU)) != FC_AR_OK)
        goto ArErr;
    
    if(!*ppPOU && bForceNew)
    {
        if((ret = m_pRoot->createDir(OLE2T(sPOUName), ppPOU)) != FC_AR_OK)
            goto ArErr;
    }


    if(*ppPOU && sTargetType && sTargetType[0])
    {    
        pPOU = *ppPOU;
        if((ret = pPOU->getSubDir(OLE2T(sTargetType), ppPOU)) != FC_AR_OK)
            goto ArErr;

        if(!*ppPOU && bForceNew)
        {
            if((ret = pPOU->createDir(OLE2T(sTargetType), ppPOU)) != FC_AR_OK)
                goto ArErr;
        }
    }    

    return *ppPOU ? S_OK : S_FALSE;
ArErr:
    return onArError(ret, psErrDescr);
}


HRESULT CLibObject::onArError(
    FC_ARSTAT    ret, 
    BSTR*        psErrDescr
)
{
    HMODULE    hmod;
    HRESULT    hrIOLastErr;
    FC_CString jot(MAX_PATH);


    if(psErrDescr)
        *psErrDescr = NULL;

    hmod = _Module.GetResourceInstance();
   
    switch(ret)
    {
    case FC_AR_OK:
    return S_OK;
    
    case FC_AR_OUTOFMEM:
        if(psErrDescr)
        {
            jot.fmtMsg(CL_E_OUTOFMEM, hmod);
            *psErrDescr = jot.toBSTR();
        }
    return E_OUTOFMEMORY;

//generall IO errors
    case FC_AR_IO_READ_FILE:
    case FC_AR_IO_WRITE_FILE:
        hrIOLastErr = GetLastError();
        hrIOLastErr = HRESULT_FROM_WIN32(hrIOLastErr);
        assert(FAILED(hrIOLastErr));

        if(psErrDescr)
        {
            FC_CString jot2(MAX_PATH);
            jot2.fmtSysErr(hrIOLastErr);
            jot.fmtMsg2(ret==FC_AR_IO_READ_FILE ? CL_E_READ_FILE : CL_E_WRITE_FILE, 
                hmod, m_ar.getLastIOErrorFileName(), jot2);

            *psErrDescr = jot.toBSTR();
        }
    return FAILED(hrIOLastErr) ? hrIOLastErr : E_UNEXPECTED;
    
    case FC_AR_CIM_NO_SUCH_FILE:
        hrIOLastErr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        if(psErrDescr)
        {
            jot.fmtMsg1(CL_E_CIM_NO_FILE_FOUND, hmod, m_ar.getLastIOErrorFileName());
            *psErrDescr = jot.toBSTR();
        }
    return hrIOLastErr;

    //version error:
    case FC_AR_BADMAGIC:
        //extra treatment on open!:
        assert(!"call CLibObject::onArError: for FC_AR_BADMAGIC?");
    return E_UNEXPECTED;


    //binary format errors while reading:
    case FC_AR_CORRUPTED:
        if(psErrDescr)
        {
            jot.fmtMsg(CL_E_CORRUPTED, hmod);
            *psErrDescr = jot.toBSTR();
        }
    return HRESULT_FROM_WIN32(ERROR_FILE_CORRUPT);

    //bad arguments or illegal calls:
    case FC_AR_NOT_OPENED_WRITE:
        if(psErrDescr)
        {
            jot.fmtMsg(CL_E_NOT_OPEN_FOR_WRITE, hmod);
            *psErrDescr = jot.toBSTR();
        }
    return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);

    case FC_AR_BAD_FILE_NAME:
        if(psErrDescr)
        {
            jot.fmtMsg1(CL_E_BAD_FILE_NAME, hmod, "todo");
            *psErrDescr = jot.toBSTR();
        }
    return HRESULT_FROM_WIN32(ERROR_INVALID_NAME);

    case FC_AR_BUFFER_TOO_SMALL:
        if(psErrDescr)
        {
            jot.fmtMsg(CL_E_BUFFER_TO_SMALL, hmod);
            *psErrDescr = jot.toBSTR();
        }
    return HRESULT_FROM_WIN32(TYPE_E_BUFFERTOOSMALL);

    case FC_AR_ERR_INERROR:
        if(psErrDescr)
        {
            jot.fmtMsg(CL_E_IN_ERROR, hmod);
            *psErrDescr = jot.toBSTR();
        }
    return HRESULT_FROM_WIN32(ERROR_CAN_NOT_COMPLETE);

    default:
        assert(!"bad FC_ARSTAT enum");
        return E_UNEXPECTED;
    }
}







HRESULT CLibObject::openAr(
    const TCHAR*    pszArchive, 
    bool            bWrite,
    BSTR*           psErrorDescr
)
{
    USES_CONVERSION;
    FC_ARSTAT  ret;
    HRESULT    hr;
    TCHAR      szVerFound[FC_AR_MAX_VER];

    assert(!m_pRoot);

    *psErrorDescr = NULL;


    hr = S_OK;
    ret = m_ar.open(pszArchive, bWrite, szVerFound);

    //special error handling on open:
    if(ret == FC_AR_BADMAGIC)
    {
        FC_CString    jot(MAX_PATH);
        HMODULE       hmod = _Module.GetResourceInstance();
        
        if(*szVerFound)
        {
            FC_CString   szFound;
            const TCHAR* pszExpect;
            
            pszExpect = szFound.appendUpTo(szVerFound, _T(","));
            jot.fmtMsg2(CL_E_VERSION_BINARY, hmod, pszExpect, szFound);
        }
        else if(StgIsStorageFile(T2OLE(pszArchive))==S_OK)
        {
            jot.fmtMsg(CL_E_VERSION_V1X, hmod);
        }
        else
        {
            jot.fmtMsg(CL_E_READ_MAGIC, hmod);
        }

        *psErrorDescr = jot.toBSTR();
        hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
    }
    else if(ret != FC_AR_OK)
    {
        //all other errors (should be only IO errors):
        hr = onArError(ret, psErrorDescr);
    }
    else if(*szVerFound && _tcscmp(CL_4CL_VERSION, szVerFound))
    {
        FC_CString    jot(MAX_PATH);
        HMODULE       hmod = _Module.GetResourceInstance();
        
        //4CL internal version, independet of archive binary format:
        jot.fmtMsg2(CL_E_VERSION_4CL, hmod, CL_4CL_VERSION, szVerFound);
        *psErrorDescr = jot.toBSTR();
        hr = HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

        //close it:
        m_ar.close(false, NULL);
    }
    //else: version ok.

    if(hr == S_OK)
    {
        assert(*szVerFound || bWrite);//<-must be new archive
        m_pRoot = m_ar.getRoot();
        assert(m_pRoot);
    }

    return hr;
}

HRESULT CLibObject::closeAr(
    BSTR* psErrDescr
)
{
    FC_ARSTAT   ret;

    assert(m_pRoot);
    ret = m_ar.close(m_ar.isOpenWrite(), CL_4CL_VERSION);
    return ret==FC_AR_OK ? S_OK : onArError(ret, psErrDescr);
}









/////////////////////////////////////////////////////////////////////
// I4CLAccess
/////////////////////////////////////////////////////////////////////

STDMETHODIMP CLibObject::GetLibFileName(/*[out, string]*/ BSTR* sFileName)
{
    *sFileName = FC_StringToBSTR(m_ar.getFileName());
    return S_OK;
}

STDMETHODIMP CLibObject::GetLastModifiedFileTime(/*[out]*/ __int64* pftLastModified)
{
    LARGE_INTEGER li;
    FILETIME      ft;
    
    ft = m_ar.getLastModTime();
    li.LowPart  = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    *pftLastModified = li.QuadPart;
    return S_OK;
}



STDMETHODIMP CLibObject::GetAllPOUs(
    /*[out]*/ SAFEARRAY** parrPOUs
)
{
    FC_ARSTAT               ret;
    FC_CGArray<FC_CArNode*> nList;
    
    *parrPOUs= NULL;

    ret = m_pRoot->getNodes(NULL, false, true, &nList);
    
    if(ret==FC_AR_OK)
        ret =  NodeListToSafeArray(&nList, parrPOUs);

    return ret==FC_AR_OK ? S_OK : onArError(ret, NULL);
}


STDMETHODIMP CLibObject::GetTargetTypesOfPOU(
    /*[in, string]*/ BSTR        sPOUName, 
    /*[out]*/        SAFEARRAY** pasTrgTypes
)
{
    USES_CONVERSION;
    FC_ARSTAT               ret;
    FC_CArDir*              pPOU;
    FC_CGArray<FC_CArNode*> nList;
    
    *pasTrgTypes = NULL;

    if((ret = m_pRoot->getSubDir(OLE2T(sPOUName), &pPOU)) != FC_AR_OK)
        return onArError(ret, NULL);

    if(!pPOU)
        return S_FALSE;

    ret = pPOU->getNodes(NULL, false, true, &nList);
    if(ret==FC_AR_OK)
        ret = NodeListToSafeArray(&nList, pasTrgTypes);

    return ret==FC_AR_OK ? S_OK : onArError(ret, NULL);
}



STDMETHODIMP CLibObject::ExistsPOU(
    /*[in, string]*/ BSTR sPOUName, 
    /*[in, string]*/ BSTR sTargetType
)
{
    FC_CArDir* pPOU;
    return getPouDir(sPOUName, sTargetType, &pPOU, NULL, false);
}



STDMETHODIMP CLibObject::GetFileNames(
    /*[in, string]*/ BSTR        sPOUName,
    /*[in, string]*/ BSTR        sTargetType,
    /*[out]*/        SAFEARRAY** parrFiles
)
{
    HRESULT                 hr;
    FC_ARSTAT               ret;
    FC_CArDir*              pPOU;
    FC_CGArray<FC_CArNode*> nList;
    
    *parrFiles = NULL;

    hr = getPouDir(sPOUName, sTargetType, &pPOU, NULL, false);
    if(hr!=S_OK) //return S_FALSE if POU does not exits
        return hr;
    
    //assert: S_OK: POU must exist
    assert(pPOU);

    ret = pPOU->getNodes(NULL, true, false, &nList);
    if(ret==FC_AR_OK)
        ret = NodeListToSafeArray(&nList, parrFiles);

    return ret==FC_AR_OK ? S_OK : onArError(ret, NULL);
}



STDMETHODIMP CLibObject::ExistsFile(
    /*[in, string]*/BSTR   sPOUName,
    /*[in, string]*/BSTR   sTargetType,
    /*[in, string]*/BSTR   sFileName,
    /*[full][out] */DWORD* plFileByteSize
)
{
    USES_CONVERSION;
    HRESULT     hr;
    FC_ARSTAT   ret;
    FC_CArDir*  pPOU;
    FC_CArFile* pFile;
    
    if(plFileByteSize)
        *plFileByteSize = 0;

    hr = getPouDir(sPOUName, sTargetType, &pPOU, NULL, false);
    if(hr != S_OK)
        return hr;

    if((ret = pPOU->getFile(OLE2T(sFileName), &pFile)) != FC_AR_OK)
        return onArError(ret, NULL);

    if(!pFile)
        return S_FALSE;

    if(plFileByteSize)
    {
        *plFileByteSize = pFile->getSize();
        assert(*plFileByteSize>=0);
    }
    return S_OK;
}

  

STDMETHODIMP CLibObject::GetFileRaw(
    /*[in, string]*/ BSTR   sPOUName, 
    /*[in, string]*/ BSTR   sTargetType,
    /*[in, string]*/ BSTR   sFileName,
    /*[out,ptr]   */ byte*  pBytes,
    /*[in]        */ DWORD  nBytes
)
{
    USES_CONVERSION;
    HRESULT     hr;
    FC_ARSTAT   ret;
    FC_CArDir*  pPOU;
    FC_CArFile* pFile;
    

    hr = getPouDir(sPOUName, sTargetType, &pPOU, NULL, false);
    if(hr!=S_OK) //return S_FALSE if POU or file does not exits
        return hr;
    //assert: S_OK: POU must exist
    assert(pPOU);

    if((ret = pPOU->getFile(OLE2T(sFileName), &pFile)) != FC_AR_OK)
        return onArError(ret, NULL);
    if(!pFile)
        return S_FALSE;

    if((ret = pFile->getData(nBytes, pBytes)) != FC_AR_OK)
        return onArError(ret, NULL);
    
    return S_OK;
}


STDMETHODIMP CLibObject::PutFileRaw(
    /*[in, string]*/ BSTR  sPOUName, 
    /*[in, string]*/ BSTR  sTargetType,
    /*[in, string]*/ BSTR  sFileName,
    /*[in, ptr]*/    BYTE* pBytes,
    /*[in, ptr]*/    DWORD nBytes
)
{
    USES_CONVERSION;
    HRESULT     hr;
    FC_ARSTAT   ret;
    FC_CArDir*  pPOU;
              
    if(!m_ar.isOpenWrite())
        return E_ACCESSDENIED;
    
    hr = getPouDir(sPOUName, sTargetType, &pPOU, NULL, true);
    if(hr != S_OK)
    {
        assert(hr != S_FALSE);
        return hr;
    }

    ret = pPOU->createFileFromMem(OLE2T(sFileName), nBytes, pBytes);

    return ret==FC_AR_OK ? S_OK : onArError(ret, NULL);
}

STDMETHODIMP CLibObject::GetFileAsBSTR(
    /* [string][in] */ BSTR     sPOUName,
    /* [string][in] */ BSTR     sTargetType,
    /* [string][in] */ BSTR     sFileName,
    /* [out] */ BSTR __RPC_FAR* sTextFile
)
{
    USES_CONVERSION;
    HRESULT     hr;
    FC_ARSTAT   ret;
    FC_CArDir*  pPOU;
    FC_CArFile* pFile;
    
    *sTextFile = NULL;

    hr = getPouDir(sPOUName, sTargetType, &pPOU, NULL, false);
    if(hr!=S_OK) //return S_FALSE if POU or file does not exits
        return hr;
    //assert: S_OK: POU must exist
    assert(pPOU);
    
    if((ret = pPOU->getFile(OLE2T(sFileName), &pFile)) != FC_AR_OK)
        return onArError(ret, NULL);

    if(!pFile)
        return S_FALSE;

#ifdef UNICODE 
#error "rework and rethink this code"
#endif

    DWORD dwBytes = pFile->getSize();
    char* psz     = new char[dwBytes+1];
    if(!psz)
    {
        assert(psz);
        return E_OUTOFMEMORY;
    }

    if((ret = pFile->getData(dwBytes, psz)) != FC_AR_OK)
        return onArError(ret, NULL);
    
    psz[dwBytes] = 0;
    *sTextFile = FC_StringToBSTR(psz);
    delete(psz);
    return S_OK;
}



STDMETHODIMP CLibObject::CheckOut(
    /*[in, string]*/   BSTR          sPOUName, 
    /*[in, string]*/   BSTR          sTargetType,
    /*[in, string]*/   BSTR          sFileSpec,
    /*[in]*/           BSTR          sCheckOutDir,
    /*[in, out,ptr]*/  SAFEARRAY**   parrFiles,
    /*[out]*/          BSTR*         sErrorDescr
)
{
    HRESULT                 hr;
    FC_ARSTAT               ret;
    FC_CString              dest(MAX_PATH);
    FC_CString              files(MAX_PATH);
    FC_CArDir*              pPOU;
    FC_CGArray<FC_CArNode*> nList;


    *sErrorDescr = NULL;
    if(parrFiles)
        *parrFiles = NULL;

    hr = getPouDir(sPOUName, sTargetType, &pPOU, sErrorDescr, false);
    if(hr!=S_OK) //return S_FALSE if POU or file does not exits
        return hr;
    //assert: S_OK: POU must exist
    assert(pPOU);

    dest.load(sCheckOutDir);
    files.load(sFileSpec);

    ret = pPOU->getFilesToDisk(dest, files, true,  parrFiles ? &nList : NULL);
    
    if(ret==FC_AR_OK)
        ret = NodeListToSafeArray(&nList, parrFiles);

    return ret==FC_AR_OK ? S_OK : onArError(ret, sErrorDescr);
}


STDMETHODIMP CLibObject::CheckIn(
    /*[in, string]*/ BSTR  sPOUName,
    /*[in, string]*/ BSTR  sTargetType,
    /*[in, string]*/ BSTR  sFileSpec,
    /*[out]*/        BSTR* sErrorDescr
)
{
    HRESULT      hr;
    FC_CArDir*   pPOU;
    FC_CString   fileSpec(MAX_PATH);
    FC_ARSTAT    ret;

    *sErrorDescr = NULL;

    hr = getPouDir(sPOUName, sTargetType, &pPOU, sErrorDescr, true);
    if(hr != S_OK)
    {
        assert(hr != S_FALSE);
        return hr;
    }

    fileSpec.load(sFileSpec);
    ret = pPOU->createFilesFromDisk(fileSpec, false);
    return ret==FC_AR_OK ? S_OK : onArError(ret, sErrorDescr);
}


STDMETHODIMP CLibObject::DeleteFiles(
    /*[in, string]*/ BSTR  sPOUName,
    /*[in, string]*/ BSTR  sTargetType,
    /*[in, string]*/ BSTR  sFileSpec
) 
{
    USES_CONVERSION;
    HRESULT     hr;
    FC_ARSTAT   ret;
    FC_CArDir*  pDir;


    hr = getPouDir(sPOUName, sTargetType, &pDir, NULL, false);
    if(hr != S_OK && hr!=S_FALSE)
        return hr;//must be fatal error..
    if(!pDir)
        return S_OK;//retutn S_OK if POU does not exists

    if(sFileSpec)
    {
        ret = pDir->remove(OLE2T(sFileSpec), NULL);
    }
    else
    {
        FC_CStr dirname(pDir->getName());
        pDir = pDir->getParent();
        assert(pDir);
        ret = pDir->remove(dirname, NULL);
    }

    return ret==FC_AR_OK ? S_OK : onArError(ret, NULL);
}






//local helpers:
static FC_ARSTAT NodeListToSafeArray(FC_CGArray<FC_CArNode*>* pNList, SAFEARRAY** ppsa)
{
    HRESULT       hr;
    SAFEARRAY*    psa;
    long          i;
    long          nNodes;
    FC_CArNode*   pNode;
    const TCHAR*  pId; 
    BSTR          bstr;
        
    if(!ppsa)
        return FC_AR_OK;
    
    *ppsa = NULL;

    nNodes = pNList->getUsed();
    psa = SafeArrayCreateVector(VT_BSTR, 0, nNodes);
    if(!psa)
        return FC_AR_OUTOFMEM;

    for(i=0; i<nNodes; i++)
    {
        pNode = pNList->get(i);
        pId   = pNode->getName();
#ifdef UNICODE 
        bstr = SysAllocString(pId);
        hr = SafeArrayPutElement(psa, &i, bstr);
        SysFreeString(bstr);
#else
        wchar_t wBuffer[3*MAX_PATH];
        int iOk = FC_StringToWideChar(pId, wBuffer, FC_ARRAY_LEN(wBuffer));
        assert(iOk>0);
        bstr = SysAllocString(wBuffer);
        hr = SafeArrayPutElement(psa, &i, bstr);
        SysFreeString(bstr);
#endif
        if(FAILED(hr))
        {
            SafeArrayDestroy(psa);
            return FC_AR_OUTOFMEM;
        }
    }

    *ppsa = psa;
    return FC_AR_OK;
}

