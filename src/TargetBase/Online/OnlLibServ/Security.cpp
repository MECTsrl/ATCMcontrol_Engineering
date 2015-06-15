/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/Security.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Security.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/Security.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Security.h"
#include "CSC_OnlineErr.h"

//----  Local Defines:   -------------------------------------------*
#define _MAX_LINE_LEN       500
#define SEC_FILE_NAME       _T("security.txt")
#define WHO_FILE_NAME       _T("who.list")
#define WHO_ENGINEERING     _T("Engineering")

//----  Globals:    ------------------------------------------------*
CSecurity    g_security;

/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CSecurity
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSecurity::CSecurity()
  : m_bSecEnabled(FALSE)
{
    // save path to config directory
    TCHAR szTrcFile[_MAX_PATH + 1];
    ::GetModuleFileName(_Module.GetModuleInstance(), szTrcFile, _MAX_PATH);
    LPTSTR  psz = _tcsrchr(szTrcFile, _T('\\'));
    if (psz != NULL) {
        *psz = _T('\0');
    }

    m_strBasePath = szTrcFile;
    m_strConfigPath = m_strBasePath + _T("\\config\\");
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSecurity::~CSecurity()
{
}



//------------------------------------------------------------------*
/**
 * InitSecurityInfo
 *      read current engineering password from who.list
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CSecurity::InitSecurityInfo()
{
    m_strEngineeringPW.Empty();

    CString strWhoFile = m_strBasePath + _T("\\");
    strWhoFile += WHO_FILE_NAME;
    
    // test if security file exists
    FILE *hfWho = _tfopen(strWhoFile, _T("r"));
    if (hfWho == NULL) {
        // no who file!
        return S_FALSE;
//        return HRESULT_FROM_WIN32(GetLastError());
    }

    CString strLine;
    LPTSTR  szLine = strLine.GetBuffer(_MAX_LINE_LEN + 1);
    
    BOOL bFound = FALSE;
    while (!feof(hfWho))
    {
        if (_fgetts(szLine, _MAX_LINE_LEN, hfWho) == NULL) {
            // unexpected end of file while reading line!
            fclose(hfWho);
            return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
        }
        // is it security file line?
        LPTSTR  pszField = _tcstok(szLine, _T("="));
        if (pszField != NULL)
        {
            if (_tcsicmp(pszField, WHO_ENGINEERING) == NULL)
            {
                // got line with enigneering password, isolate PW
                // second field holds password (V1.1)
                pszField = _tcstok(NULL, _T("=;\n\r"));
                if (pszField != NULL)
                {
                    m_strEngineeringPW = pszField;
                    bFound = TRUE;
                    TrcPrint(TRC_ERROR, _T("Found Engineering PW in file '%s'\n"), (LPCTSTR) strWhoFile);
                }
                break;
            }
        }
    }
    
    fclose(hfWho);
    
    return bFound ? S_OK : E_FAIL;
}


//------------------------------------------------------------------*
/**
 * ReReadSecurityInfo
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CSecurity::ReReadSecurityInfo()
{
    HRESULT hr = S_FALSE;
    CSingleLock lock(&m_csSecurity, TRUE);

    CString strSecurityFile, strVersionGUID;
    hr = GetSecurityFile(strSecurityFile, strVersionGUID);
    if (FAILED(hr) || hr == S_FALSE) {
        // cannot access security file!!!
        if (FAILED(hr)) {
            TrcPrint(TRC_ERROR, _T("Error opening security file, hr=0x%08x\n"), hr);
        }
        else {
            TrcPrint(TRC_WARNING, _T("No security file found, security disabled!\n"));
        }
        m_strLastVersionGUID.Empty();
        m_strReadOnlyPW.Empty();
        m_strReadWritePW.Empty();
        m_bSecEnabled = FALSE;
        return hr;
    }

    if (   strVersionGUID.IsEmpty()
        || (m_strLastVersionGUID == strVersionGUID))
    {
        // no change, no update of security info
        return S_FALSE; 
    }
    // save version GUID
    m_strLastVersionGUID = strVersionGUID;

    // extract passwords
    hr = ExtractPasswords(strSecurityFile, m_strReadOnlyPW, m_strReadWritePW);
    if (FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("Error extracting passwords from security file, hr=0x%08x\n"), hr);
    }
    
    // ..and determine if security is enabled or not
    m_bSecEnabled = !(m_strReadOnlyPW.IsEmpty() && m_strReadWritePW.IsEmpty());

    return hr;
}


//------------------------------------------------------------------*
/**
 * GetUserRights
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
CSecurity::SECURITY_RIGHTS
    CSecurity::GetUserRights(LPCTSTR pszPassword)
{
    CSingleLock lock(&m_csSecurity, TRUE);
    if (!IsSecurityEnabled()) {
        return NoSecurity;
    }

    assert(pszPassword!=NULL);

    SECURITY_RIGHTS rights = NoRights;
    
    if (!m_strEngineeringPW.IsEmpty() && m_strEngineeringPW == pszPassword)
    {
        rights = Engineering;
    }
    else {
        if (m_strReadWritePW == pszPassword)
        {
            rights = ReadAndWrite;
        }
        else {
            if (m_strReadOnlyPW == pszPassword)
            {
                rights = ReadOnly;
            }
        }
    }

    ::TrcPrint(TRC_INTERFACE, _T("GetUserRights() returned '%s' for PW='%s'\n"),
        CSecurity::GetSecurityText(rights), pszPassword);
    return rights;
}


//------------------------------------------------------------------*
LPCTSTR
    CSecurity::GetSecurityText(SECURITY_RIGHTS right)
{
    LPCTSTR pszRet;
    switch (right) {
    case NoSecurity:
        pszRet = _T("NoSecurity");
        break;
    case NoRights:
        pszRet = _T("NoRights");
        break;
    case ReadOnly:
        pszRet = _T("ReadOnly");
        break;
    case ReadAndWrite:
        pszRet = _T("ReadAndWrite");
        break;
    case Engineering:
        pszRet = _T("Engineering");
        break;
    default:
        pszRet = _T("???");
        break;
    }

    return pszRet;
}


//------------------------------------------------------------------*
/**
 * GetSecurityFile
 *      get path to downloaded security file and its version GUID
 *
 * @param            
 * @return           S_OK, error code if not present
 * @exception        -
 * @see              
*/
HRESULT
    CSecurity::GetSecurityFile(OUT CString &strFileName, OUT CString &strVersionGUID)
{
    HRESULT hr = S_OK;

    CString strSecFile = m_strConfigPath + SEC_FILE_NAME;
    CString strListFile = m_strConfigPath + _T("__.list");
    
    // test if security file exists
    FILE *hfTest = _tfopen(strSecFile, _T("r"));
    if (hfTest == NULL) {
        // is no real error, security is disabled !!!!!!!!!!!!
        return S_FALSE;
//        return HRESULT_FROM_WIN32(GetLastError());
    }
    fclose(hfTest);

    // look for __.list file to extract the security file's version GUID
    FILE *hfList = _tfopen(strListFile, _T("r"));
    if (hfList == NULL) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    hr = ExtractVersionGUID(hfList, strVersionGUID);
    fclose(hfList);
    
    strFileName =  strSecFile;

    return hr;
}


//------------------------------------------------------------------*
/**
 * ExtractVersionGUID
 *      extract version GUID for security file
 *
 * @param            
 * @return           S_OK, error code
 * @exception        -
 * @see              
*/
HRESULT
    CSecurity::ExtractVersionGUID(FILE *hfFile, OUT CString &strVersionGUID)
{
    CString strLine;
    LPTSTR  szLine = strLine.GetBuffer(_MAX_LINE_LEN + 1);
    
    BOOL bFound = FALSE;
    while (!feof(hfFile))
    {
        if (_fgetts(szLine, _MAX_LINE_LEN, hfFile) == NULL) {
            // unexpected end of file while reading line!
            return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
        }
        // is it security file line?
        LPTSTR  pszField = _tcstok(szLine, _T(";"));
        if (pszField != NULL)
        {
            if (_tcsicmp(pszField, SEC_FILE_NAME) == NULL)
            {
                // got line with security file, isolate version GUID
                // second field holds version GUID (V1.1)
                pszField = _tcstok(NULL, _T(";"));
                if (pszField != NULL)
                {
                    strVersionGUID = pszField;
                    bFound = TRUE;
                }
                break;
            }
        }
    }
    return bFound ? S_OK : S_FALSE;
}


//------------------------------------------------------------------*
/**
 * ExtractPasswords
 *      extract readonly and read/write password from security file
 *
 * @param            
 * @return           S_OK, error code
 * @exception        -
 * @see              
*/
HRESULT
    CSecurity::ExtractPasswords(LPCTSTR pszSecFile, OUT CString &strReadOnlyPW, CString &strReadWritePW)
{
    HRESULT hr;
    CComPtr<IXMLDocument> pXMLDocument;
    hr = ParseXML(pszSecFile, &pXMLDocument);
    if (FAILED(hr)) {
        return hr;
    }

    CComPtr<IXMLElement> pRoot;
    hr = pXMLDocument->get_root(&pRoot);
    if (FAILED(hr)) return hr;

    CComBSTR    sName;
    hr = pRoot->get_tagName(&sName);
    if (FAILED(hr)) return hr;

    // make sure we are on user list entry
    CString strName = sName;
    if (strName.CompareNoCase(_T("USERLIST")) != 0) {
        // bad XML syntax !!
        return E_CSC_BAD_SECURITY_FILE_SYNTAX;
    }

    // start iterating thru user/password list
    CComPtr<IXMLElementCollection> pColl;
    hr = pRoot->get_children(&pColl);
    if (FAILED(hr)) return hr;
    long lCnt = 0;
    hr = pColl->get_length(&lCnt);
    if (FAILED(hr)) return hr;
    
    CComPtr<IXMLElement> pUser;
    CComVariant vtVal;
    CString strRights, strPW;
    long lFound = 0;
    for (long lNext = 0; lNext < lCnt; lNext = lFound + 1)
    {
        pUser.Release();
        hr = GetElemWithMatchingName(pColl, _T("USER"), lNext, lFound, &pUser);
        if (FAILED(hr)) return hr;
        // save password corresponding to rights
        hr = pUser->getAttribute(CComBSTR(_T("Rights")), &vtVal);
        if (FAILED(hr)) return hr;
        
        hr = vtVal.ChangeType(VT_BSTR);
        if (FAILED(hr)) return hr;
        strRights = vtVal.bstrVal;
        vtVal.Clear();

        hr = pUser->getAttribute(CComBSTR(_T("Password")), &vtVal);
        if (FAILED(hr)) return hr;

        hr = vtVal.ChangeType(VT_BSTR);
        if (FAILED(hr)) return hr;
        strPW = vtVal.bstrVal;
        vtVal.Clear();

        // which password?
        if (strRights.CompareNoCase(_T("rw")) == 0)
        {
            // have ReadAndWrite password!
            strReadWritePW = strPW;
        }
        else {
            if (strRights.CompareNoCase(_T("r")) == 0)
            {
                // have ReadOnly password!
                strReadOnlyPW = strPW;
            }
        }
    }
    return S_OK;
}





//------------------------------------------------------------------*
/**
 * ParseXML
 *      open and parse XML file 
 *
 * @param            OUT
 * @return           S_OK, error code
 * @exception        -
 * @see              
*/
HRESULT
    CSecurity::ParseXML(LPCTSTR pszSecFile, IXMLDocument **ppDoc)
{
    if (ppDoc == NULL) {
        return E_POINTER;
    }
    *ppDoc = NULL;
    HRESULT hr;
    CComPtr<IXMLDocument> pXMLDocument;
    // create control; get XML 1.0 document ptr
    IUnknown    *pUnk = NULL;
    hr = ::CoCreateInstance(CLSID_XMLDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDocument, (void**) &pUnk);
    if (FAILED(hr)) {
        return hr;
    }
    pXMLDocument = (IXMLDocument*) pUnk;
    pUnk->Release();

    // Synchronously create a stream on a URL.
    CComPtr<IStream> pStm;
    hr = ::URLOpenBlockingStream(0, pszSecFile, &pStm, 0,0);
    if (FAILED(hr)) return hr;

    // Get the IPersistStreamInit interface to the XML doc.
    CComPtr<IPersistStreamInit> pPSI;
    hr = pXMLDocument->QueryInterface(IID_IPersistStreamInit, (void **)&pPSI);
    if (!pPSI) return E_NOINTERFACE;
    
    // Init the XML doc from the stream.
    hr = pPSI->Load(pStm);
    
    if (SUCCEEDED(hr))
    {
        *ppDoc = pXMLDocument;
        (*ppDoc)->AddRef();
    }
    else
    {
        // Get error information !
        CComQIPtr<IXMLError>  pXMLError;
        XML_ERROR xmle;
        
        pXMLError = pPSI;
        assert(pXMLError);
        
        pXMLError->GetErrorInfo(&xmle);
        CString strMsg, strError;
        strMsg = _T("Security.txt: Error on line %d\nFound '%s'\nExpected:\n\n%s");
        strError.Format(strMsg, xmle._nLine, (LPCTSTR)CString(xmle._pszFound), (LPCTSTR)CString(xmle._pszExpected));
        
        SysFreeString(xmle._pszFound);
        SysFreeString(xmle._pszExpected);
        SysFreeString(xmle._pchBuf);
        TrcPrint(TRC_ERROR, strError);
    }
    return hr;
}


//------------------------------------------------------------------*
/**
 * GetElemWithMatchingName
 *      return lInstance-th element in collection with matching name
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
HRESULT
    CSecurity::GetElemWithMatchingName(IXMLElementCollection *pColl,
                    LPCTSTR pszName, long lStart, OUT long &lFound, OUT IXMLElement **ppElem)
{
    if (ppElem == NULL) {
        return E_POINTER;
    }
    *ppElem = NULL;

    HRESULT hr;
    long lCnt = 0;
    assert(pColl);
    hr = pColl->get_length(&lCnt);
    if (FAILED(hr)) throw hr;
    // loop thru collection to check for a element with requested name:
    for (long l = lStart; l < lCnt; l++)
    {
        CComVariant vIndex = (long) l;
        CComVariant vEmpty;
        IDispatch   *pDisp = NULL;
        hr = pColl->item(vIndex, vEmpty, &pDisp);
        if (FAILED(hr)) throw hr;
        if (pDisp == NULL) throw E_NOINTERFACE;

        // QI to check if we have a element
        CComQIPtr<IXMLElement> pKid = pDisp;
        pDisp->Release();
        if (pKid)
        {
            // check for matching type:
            long    lType;
            hr = pKid->get_type(&lType);
            if (FAILED(hr)) throw hr;
            XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
            if (type == XMLELEMTYPE_ELEMENT)
            {
                // check for requested tag name
                CComBSTR sBuf;
                hr = pKid->get_tagName(&sBuf);
                if (SUCCEEDED(hr)) {
                    CString strName = sBuf;
                    if (strName.CompareNoCase(pszName) == 0)
                    {
                        *ppElem = pKid;
                        (*ppElem)->AddRef();
                        lFound = l;     // set also sibling instance !
                        hr = S_OK;
                        break;
                    }
                    
                }
            }
        }
    }
    hr = (*ppElem != NULL) ? S_OK : S_FALSE;
    return hr;
}

/* ---------------------------------------------------------------------------- */
