/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/XML_Wrapper/XMLObject.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: XMLObject.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/XML_Wrapper/XMLObject.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CMN
 *
 * =COMPONENT           XML DOM Wrapper and MFC integration
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  31.01.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


#include "stdafx.h"
#include "XML_Wrapper.h"
#include "UtilIf.h"

//----  Local Defines:   -------------------------------------------*
#define COMMENT_INTRO   "<!-- "
#define COMMENT_TRAIL   " -->\r\n"
#define CDATA_INTRO     "<![CDATA["
#define CDATA_TRAIL     "]]>"

#define INDENT      4   // numer of blanks to indent per level


//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "msXML_2_i.c"



/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CXMLObject
/////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CXMLObject, CObject);

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CXMLObject::CXMLObject()
{
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CXMLObject::~CXMLObject()
{
}


//------------------------------------------------------------------*
/**
 * SerializeXML
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
void
    CXMLObject::SerializeXML(CXMLArchive & /*ar*/)
{
    _ASSERTE(FALSE);    // must be overriden!!!
    return;
}


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CXMLArchive
/////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       throws HRESULT if failed
*/
CXMLArchive::CXMLArchive(CFile* pFile, UINT nMode)
  : CArchive(pFile, nMode),
    m_iIndent(0),
    m_lSibling(0)
{

    HRESULT hr;
    hr = CreateXmlControl(FALSE);
    if (FAILED(hr)) throw hr;
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CXMLArchive::~CXMLArchive()
{
    DropXmlControl();
}

//------------------------------------------------------------------*
/**
 * Load
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::Load()
                            // parse XML file and prepare for walking thru tree
{
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    HRESULT hr;
    hr = DropXmlControl();
    if (FAILED(hr)) throw hr;
    hr = CreateXmlControl();
    if (FAILED(hr)) throw hr;

    CFile   *pFile = GetFile();
    assert(pFile);
    if (pFile == NULL) throw E_FAIL;

    // create a IStream with contents of CFile;
    // to allow use of all CFile derivates (CMemFile etc.),
    // copy contents of CFile into a stream
    CComPtr<IStorage>   pStore;
    // create temporary storage to have a stream to fill with CFile contents
    hr = ::StgCreateDocfile(NULL, 
            STGM_CREATE | STGM_READWRITE
          | STGM_SHARE_EXCLUSIVE | STGM_DELETEONRELEASE, 0, &pStore);
    if (FAILED(hr)) throw hr;
    CComPtr<IStream> pStm;
    hr = pStore->CreateStream(L"", STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0,0, &pStm);
    if (FAILED(hr)) throw hr;

    DWORD dwLen = pFile->GetLength();
    BYTE *pBuffer = new BYTE[dwLen];
    // %TBD% would be nicer to allocate a 4K to 16K buffer and loop until completely copied into stream
    if (pBuffer == NULL) throw E_OUTOFMEMORY;

    try {
        pFile->Read(pBuffer, dwLen);
    }
    catch (CFileException *e)
    {
        delete [] pBuffer;
        e->Delete();
        throw E_FAIL;   // %TBD% should be a more descriptive exception!!!
    }
    pFile->Close();

    ULONG ulWritten = 0;
    hr = pStm->Write(pBuffer, dwLen, &ulWritten);
    delete [] pBuffer;
    if (FAILED(hr)) throw hr;
    assert(dwLen == ulWritten);

    pStm.Release(); // close stream and reopen
    hr = pStore->OpenStream(L"", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStm);
    if (FAILED(hr)) throw hr;
    
    // Get the IPersistStreamInit interface to the XML doc.
    CComPtr<IPersistStreamInit> pPSI;
    hr = m_pXMLDocument->QueryInterface(IID_IPersistStreamInit, (void **)&pPSI);
    if (!pPSI) throw E_NOINTERFACE;
    
    // Init the XML doc from the stream.
    hr = pPSI->Load(pStm);
    
    if (SUCCEEDED(hr))
    {
        m_strError.Empty();
        // get doc type and other info
        m_strDocType.Empty();
        BSTR sString;
        hr = m_pXMLDocument->get_doctype(&sString);
        if (FAILED(hr)) throw hr;
        if (sString != NULL) {
            m_strDocType = sString;
            ::SysFreeString(sString);
        }

        m_strDTD.Empty();
        hr = m_pXMLDocument->get_dtdURL(&sString);
        if (FAILED(hr) && (hr != E_NOTIMPL)) throw hr;
        if (SUCCEEDED(hr))
        {
            m_strDTD = sString;
            ::SysFreeString(sString);
        }
        

        hr = GetRootElem(&m_pRootElem);
        if (SUCCEEDED(hr)) {
            VERIFY(SUCCEEDED(SetCurrentElement(m_pRootElem)));
        }
    }
    else
    {
        // Get error information !
        CComQIPtr<IXMLError>  pXMLError;
        CComQIPtr<IXMLError2>  pXMLError2;
        XML_ERROR xmle;
        
        pXMLError = pPSI;
        pXMLError2 = pPSI;

        // use the improved error interface if available
        if (pXMLError2)
        {
            BSTR    sReason;
            hr = pXMLError2->get_reason(&sReason);
            if (FAILED(hr)) throw hr;
            m_strError = sReason;
            SysFreeString(sReason);
        }
        else
        {
            assert(pXMLError);
            
            pXMLError->GetErrorInfo(&xmle);
            CString strMsg;
            strMsg = _T("Error on line %d\nFound '%s'\nExpected:\n\n%s");
            m_strError.Format(strMsg, xmle._nLine, (LPCTSTR)CString(xmle._pszFound), (LPCTSTR)CString(xmle._pszExpected));
            
            SysFreeString(xmle._pszFound);
            SysFreeString(xmle._pszExpected);
            SysFreeString(xmle._pchBuf);
        }
        throw S_FALSE;
    }
    
    return;
}

//------------------------------------------------------------------*
/**
 * Flush
 *      write tree in memory to XML file
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::Flush()
{
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    WriteXML();
}

//------------------------------------------------------------------*
/**
 * AddToClassMap
 *      add runtime class info of pObj and XML node name to map
 *      <b>must be called prior to serialize object for</b>
 *      <b>both storing and loading !! </b>
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::AddToClassMap(CXMLObject *pObj, LPCTSTR pszNodeName)
{
    assert(pObj != NULL);
    ASSERT_KINDOF(CXMLObject, pObj);
    CRuntimeClass   *pRT = pObj->GetRuntimeClass();
    assert(pRT != NULL);
    if (pRT == NULL) {
        throw E_FAIL;   // %TBD% should be specific error code: not a valid XML object, must have IMPLEMENT_SERIAL!!
    }

    AddToClassMap(pRT, pszNodeName);
}

//------------------------------------------------------------------*
/**
 * AddToClassMap
 *      add runtime class info of pObj and XML node name to map
 *      <b>must be called prior to serialize object for</b>
 *      <b>both storing and loading !! </b>
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::AddToClassMap(CRuntimeClass *pRT, LPCTSTR pszNodeName)
{
    assert(pRT != NULL);
    // enter into maps, overwriting any name clashes!
    m_mapClassToNode.SetAt(pRT->m_lpszClassName, pszNodeName);
    m_mapNodeToClass.SetAt(pszNodeName, pRT);
}


//------------------------------------------------------------------*
/**
 * WriteObject
 *      do not call directly, commonly use from within stream operators !!
 *      lookup of node name in Class Map, create XML node and serialize object
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::WriteObject(const CXMLObject *pObj)
{
    assert(IsStoring());
    assert(pObj != NULL);
    ASSERT_KINDOF(CXMLObject, pObj);
    CRuntimeClass   *pRT = pObj->GetRuntimeClass();
    assert(pRT != NULL);
    if (pRT == NULL) {
        throw E_FAIL;   // %TBD% should be specific error code: not a valid XML object, must have IMPLEMENT_SERIAL!!
    }
    CString strNodeName;
    if (!m_mapClassToNode.Lookup(pRT->m_lpszClassName, strNodeName)) {
        throw E_FAIL;  // %TBD% throw more specific exception: unknown node/class name!!
    }
    MoveDown(strNodeName);
    ((CXMLObject *)pObj)->SerializeXML(*this);
    MoveUp();
}

//------------------------------------------------------------------*
/**
 * ReadObject
 *      do not call directly, commonly use from within stream operators !!
 *      lookup of node name in Class Map, create C++ XML obj and serialize it
 *
 * @param           void
 * @return          ptr to newly created XM object
 * @exception       HRESULT
*/
CXMLObject *
    CXMLArchive::ReadObject(CXMLObject *pExistingObj)
{
    assert(IsLoading());
    CRuntimeClass   *pRT = NULL;
    if (!m_mapNodeToClass.Lookup(m_strCurrentNodeName, pRT)) {
        throw E_FAIL;   // %TBD% throw more specific exception: unknown node name!!
    }

    CXMLObject *pObj = NULL;
    if (!pExistingObj)
    {
        pObj = DYNAMIC_DOWNCAST(CXMLObject, pRT->CreateObject());
        if (pObj == NULL) throw E_OUTOFMEMORY;
    }
    else {
        pObj = pExistingObj;
    }

    try {
        pObj->SerializeXML(*this);
    }
    catch (HRESULT hr) {
        // cannot serialize!
        if (!pExistingObj) {
            delete pObj;
        }
        throw hr;
    }

    return pObj;
}

//------------------------------------------------------------------*
/**
 * Clear
 *      cleanup XML element tree in memory
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::Clear()
{
    // clear by simply recreating a new instance of XML control
    VERIFY(DropXmlControl());
    CreateXmlControl(FALSE);
}


//------------------------------------------------------------------*
/**
 * SetDocType
 *      call once to set doc type
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetDocType(LPCTSTR pszDocType)
{
    if (!IsStoring()) throw E_FAIL;     // use separate error code: wrong mode!!
    assert(m_strDocType.IsEmpty());     // should only be called once !!!!
    assert(m_pRootElem == NULL);
    if (m_pRootElem) throw E_FAIL;  // already set, cannot set again!!
        
    m_strDocType = pszDocType;
    m_pRootElem = NULL;
    HRESULT hr;
    hr = GetRootElem(&m_pRootElem);
    if (FAILED(hr)) throw hr;

    VERIFY(SUCCEEDED(SetCurrentElement(m_pRootElem)));
}

//------------------------------------------------------------------*
/**
 * SetDocType
 *      call once to set doc type
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetDTD(LPCTSTR pszDTD)
{
    if (!IsStoring()) throw E_FAIL;     // use separate error code: wrong mode!!
        
    m_strDTD = pszDTD;
}


//------------------------------------------------------------------*
/**
 * MoveDown
 *      walk down one elem hierarchy to element
 *      will create new node, throws when not found in Class Map !!
 *      works for read and write !!
 *
 * @param           void
 * @return          TRUE if node exists (loading) or node was created (writing)
 * @exception       HRESULT
*/
BOOL
    CXMLArchive::MoveDown(LPCTSTR pszElemName)
{
    if (!m_pXMLDocument) throw E_NOINTERFACE;   // have no XML control loaded !!
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;   // have no current element, for now, considered as error!

    HRESULT hr = E_FAIL;
    CComPtr<IXMLElement2>    pElem;
    if (IsStoring())
    {
        // create elements when moving down
        hr = CreateNewElem(pszElemName, XMLELEMTYPE_ELEMENT, &pElem);
        if (FAILED(hr)) throw hr;

        assert(m_pCurrentElem);
        hr = m_pCurrentElem->addChild(pElem, -1, -1);    // add to end of child list
        if (FAILED(hr)) throw hr;
    }
    else
    {
        // seek to first matching child element of current element
        CComPtr<IXMLElementCollection>   pColl;
        assert(m_pCurrentElem);
        hr = m_pCurrentElem->get_children(&pColl);
        if (FAILED(hr)) throw hr;
        if (hr == S_FALSE) {
            return FALSE;    // have no child nodes at all!
        }
        
        // stack old sibling instance number !!!
        m_stackSiblings.AddTail(m_lSibling);
        m_lSibling = 0;     // start at first sibling
        hr = GetElemWithMatchingName(pColl, pszElemName, m_lSibling, &pElem);
        if (FAILED(hr) || !pElem) {
            /* TODO SU I think this is a bug and if sibling is not restored
               But I retain the old implementation due deadline issues.
            if (!m_stackSiblings.IsEmpty()) {
                m_lSibling = m_stackSiblings.RemoveTail();
            }
            */
            return FALSE;   // not found
        }
    }

    // set current elem to new eleme
    VERIFY(SUCCEEDED(SetCurrentElement(pElem)));
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * MoveUp
 *      walk up one elem node level
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::MoveUp()
{
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!
    
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    if (IsLoading())
    {
        // try to restore previous sibling #
        if (!m_stackSiblings.IsEmpty()) {
            m_lSibling = m_stackSiblings.RemoveTail();
        }
    }

    CComPtr<IXMLElement2>    pParent;
    HRESULT hr;
    hr = m_pCurrentElem->get_parent(&pParent);
    if (FAILED(hr)) throw hr;

    VERIFY(SUCCEEDED(SetCurrentElement(pParent)));
}

//------------------------------------------------------------------*
/**
 * MoveNext
 *      walk to siblings in same level,
 *      assume identical name from previous MoveDown()
 *      adds sibling elem node when in writing node
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
BOOL
    CXMLArchive::MoveNext()
{
    if (!m_pXMLDocument) throw E_NOINTERFACE;   // have no XML control loaded !!
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;   // have no current element, for now, considered as error!

    HRESULT hr;
    CComPtr<IXMLElement2>    pParent;
    hr = m_pCurrentElem->get_parent(&pParent);
    if (FAILED(hr)) throw hr;
    if (!pParent) throw E_FAIL;
    CComPtr<IXMLElementCollection>   pColl;
    hr = pParent->get_children(&pColl);
    if (FAILED(hr)) throw hr;

    CComPtr<IXMLElement2>    pElem;
    assert(!m_strCurrentNodeName.IsEmpty());    // name must not be empty !!
    if (IsStoring())
    {
        // create elements when moving down
        hr = CreateNewElem(m_strCurrentNodeName, XMLELEMTYPE_ELEMENT, &pElem);
        if (FAILED(hr)) throw hr;

        hr = pParent->addChild(pElem, -1, -1);    // add to end of child list
        if (FAILED(hr)) throw hr;
    }
    else
    {
        hr = GetElemWithMatchingName(pColl, m_strCurrentNodeName, m_lSibling + 1, &pElem);
        if (FAILED(hr)) throw hr;
        if ((hr == S_FALSE) || !pElem) {
            return FALSE;
        }
    }
    
    VERIFY(SUCCEEDED(SetCurrentElement(pElem)));
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * MoveToSibling
 *      walk to siblings in same level,
 *      matching the given attribute and value
 *      valid in read mode only!
 *
 * @param           void
 * @return          TRUE if found, FALSE if not
 * @exception       HRESULT on errors
*/
BOOL
    CXMLArchive::MoveToSibling(LPCTSTR pszAttrName, LPCTSTR pszAttrValue)
{
    if (IsStoring())    throw E_NOTIMPL;        // %TBD% bad state !!

    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!
    HRESULT hr;
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    CComPtr<IXMLElement2>    pParent;
    hr = m_pCurrentElem->get_parent(&pParent);
    if (FAILED(hr)) throw hr;
    if (!pParent) throw E_FAIL;
    CComPtr<IXMLElementCollection>   pColl;
    hr = pParent->get_children(&pColl);
    if (FAILED(hr)) throw hr;

    CComPtr<IXMLElement2>    pElem;
    hr = GetElemWithMatchingAttr(pColl, pszAttrName, pszAttrValue, &pElem);
    if (FAILED(hr)) throw hr;
    if (hr == S_FALSE) {
        return FALSE;
    }
    
    VERIFY(SUCCEEDED(SetCurrentElement(pElem)));
    return TRUE;
}


BOOL CXMLArchive::IsElementExisting(LPCTSTR pszElemName)
{
    if (!m_pXMLDocument) throw E_NOINTERFACE;   // have no XML control loaded !!
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;   // have no current element, for now, considered as error!

    HRESULT hr = E_FAIL;
    CComPtr<IXMLElement2>    pElem;

    if (IsStoring())
    {
        throw E_FAIL;
    }
    else
    {
        // seek to first matching child element of current element
        CComPtr<IXMLElementCollection>   pColl;
        assert(m_pCurrentElem);
        hr = m_pCurrentElem->get_children(&pColl);
        if (FAILED(hr)) throw hr;
        if (hr == S_FALSE) {
            return FALSE;    // have no child nodes at all!
        }

        long lStart = 0L;
        //cache old value
        long lSibling = m_lSibling;
        //check for element existence
        hr = GetElemWithMatchingName(pColl, pszElemName, lStart, &pElem);
        m_lSibling = lSibling; //restore old value
        if (FAILED(hr) || !pElem) {
            return FALSE;   // not found
        }
    }

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * SetAttribute
 *      set attribute value of current elem node
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetAttribute(LPCTSTR pszAttrName, LPCTSTR pszAttrValue)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    // encode special characters like umlauts, <, >, ... etc !!!!!!!
    HRESULT hr;
    CString strEncoded;
    hr = EncodeString(pszAttrValue, strEncoded);
    if (FAILED(hr)) throw hr;

    CComVariant vValue = strEncoded;
    hr = m_pCurrentElem->setAttribute(CComBSTR(pszAttrName), vValue);
    if (FAILED(hr)) throw hr;
}

//------------------------------------------------------------------*
/**
 * SetStringAttributeUTF8
 *      set string attribute value of current elem node
 *      uses UTIL_String2UTF8 to convert non ascii characters to
 *      UTF-8 format.
 *
 * @param           pszAttrName: attribute name
 * @param           pszAttrValue: attribute value
 * @return          void
 * @exception       HRESULT
*/
void CXMLArchive::SetStringAttributeUTF8(LPCTSTR pszAttrName, LPCTSTR pszAttrValue)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    // encode special characters like umlauts, <, >, ... etc !!!!!!!
    HRESULT hr;
    CString strEncoded;
    //hr = EncodeString(pszAttrValue, strEncoded);
    //if (FAILED(hr)) throw hr;
    size_t uiSize = strlen(pszAttrValue)*6 + 1; 
    if(!UTIL_String2UTF8(pszAttrValue, strEncoded.GetBuffer(uiSize), uiSize))
    {
        hr = E_FAIL;
    }
    strEncoded.ReleaseBuffer();

    CComVariant vValue = strEncoded;
    hr = m_pCurrentElem->setAttribute(CComBSTR(pszAttrName), vValue);
    if (FAILED(hr)) throw hr;
}

//------------------------------------------------------------------*
/**
 * SetAttribute
 *      set attribute value of current elem node
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetAttribute(LPCTSTR pszAttrName, long lAttrValue, enum NumericFormat fmt /*= decimal*/)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CString strVal;
    CString strBuf;
    LPTSTR pszBuf = strBuf.GetBuffer(50);
    switch (fmt) {
    case decimal:
        strVal =  _ltoa(lAttrValue, pszBuf, 10);
        break;
    case hex:
        strVal.Format(_T("0x%s"), _ltoa(lAttrValue, pszBuf, 16));
        break;
    case bin:
        strVal.Format(_T("%%%s"), _ltoa(lAttrValue, pszBuf, 2));
        break;
    case oct:
        strVal.Format(_T("0%s"), _ltoa(lAttrValue, pszBuf, 8));
        break;
    }
    strVal.ReleaseBuffer();
    SetAttribute(pszAttrName, strVal);
}

//------------------------------------------------------------------*
/**
 * SetAttribute
 *      set attribute value of current elem node
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetAttribute(LPCTSTR pszAttrName, ULONG ulAttrValue, enum NumericFormat fmt /*= decimal*/)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CString strVal;
    CString strBuf;
    LPTSTR pszBuf = strBuf.GetBuffer(50);
    switch (fmt) {
    case decimal:
        strVal =  _ultoa(ulAttrValue, pszBuf, 10);
        break;
    case hex:
        strVal.Format(_T("0x%s"), _ultoa(ulAttrValue, pszBuf, 16));
        break;
    case bin:
        strVal.Format(_T("%%%s"), _ultoa(ulAttrValue, pszBuf, 2));
        break;
    case oct:
        strVal.Format(_T("0%s"), _ultoa(ulAttrValue, pszBuf, 8));
        break;
    }
    strVal.ReleaseBuffer();
    SetAttribute(pszAttrName, strVal);
}

//------------------------------------------------------------------*
/**
 * SetAttribute
 *      set attribute value of current elem node
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetAttribute(LPCTSTR pszAttrName, CPoint ptAttrValue, enum NumericFormat fmt /*= decimal*/)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CString strVal;
    CString strBuf;
    LPTSTR pszBuf = strBuf.GetBuffer(50);
    switch (fmt) {
    case decimal:
        strVal  = _ltoa(ptAttrValue.x, pszBuf, 10);
        strVal += ";";
        strVal += _ltoa(ptAttrValue.y, pszBuf, 10);
        break;
    case hex:
        strVal  = "0x";
        strVal += _ltoa(ptAttrValue.x, pszBuf, 16);
        strVal += ";0x";
        strVal += _ltoa(ptAttrValue.y, pszBuf, 16);
        break;
    case bin:
        strVal  = "%%";
        strVal += _ltoa(ptAttrValue.x, pszBuf, 2);
        strVal += ";%%";
        strVal += _ltoa(ptAttrValue.y, pszBuf, 2);
        break;
    case oct:
        strVal  = "0";
        strVal += _ltoa(ptAttrValue.x, pszBuf, 8);
        strVal += ";0";
        strVal += _ltoa(ptAttrValue.y, pszBuf, 8);
        break;
    }
    strVal.ReleaseBuffer();
    SetAttribute(pszAttrName, strVal);
}

//------------------------------------------------------------------*
/**
 * GetAttribute
 *      get attribute value of current elem node
 *
 * @param           void
 * @return          TRUE if attribute found
 * @exception       HRESULT
*/
BOOL
    CXMLArchive::GetAttribute(LPCTSTR pszAttrName, CString &strAttrValue)
                            // get attribute value of current elem node
{
    if (!IsLoading()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CComVariant vValue;
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    HRESULT hr = m_pCurrentElem->getAttribute(CComBSTR(pszAttrName), &vValue);
    if (FAILED(hr))         throw hr;
    if (hr == S_FALSE) {
        strAttrValue.Empty();
        return FALSE;
    }

    assert(vValue.vt == VT_BSTR);
    if (vValue.vt != VT_BSTR) {
        hr = vValue.ChangeType(VT_BSTR);
        if (FAILED(hr)) throw hr;
    }
    // getAttribute() seems to return the string already decoded !!
    strAttrValue = vValue.bstrVal;
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * GetAttribute
 *      get attribute value of current elem node
 *
 * @param           void
 * @return          TRUE if attribute found
 * @exception       HRESULT
*/
BOOL
    CXMLArchive::GetAttribute(LPCTSTR pszAttrName, long &lAttrValue, enum NumericFormat /*fmt = decimal*/)
{
    if (!IsLoading()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CString strAttrValue;
    lAttrValue = 0;
    BOOL bFound = GetAttribute(pszAttrName, strAttrValue);
    if (!bFound) {
        return FALSE;
    }

    lAttrValue = ::_tcstol(strAttrValue, NULL, 0);     // should be baselib call !!!!
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * GetAttribute
 *      get attribute value of current elem node
 *
 * @param           void
 * @return          TRUE if attribute found
 * @exception       HRESULT
*/
BOOL
    CXMLArchive::GetAttribute(LPCTSTR pszAttrName, ULONG &ulAttrValue, enum NumericFormat /*fmt = decimal*/)
{
    if (!IsLoading()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CString strAttrValue;
    ulAttrValue = 0;
    BOOL bFound = GetAttribute(pszAttrName, strAttrValue);
    if (!bFound) {
        return FALSE;
    }

    ulAttrValue = ::_tcstoul(strAttrValue, NULL, 0);     // should be baselib call !!!!
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * GetAttribute
 *      get attribute value of current elem node
 *
 * @param           void
 * @return          TRUE if attribute found
 * @exception       HRESULT
*/
BOOL
    CXMLArchive::GetAttribute(LPCTSTR pszAttrName, CPoint &ptAttrValue, enum NumericFormat /*fmt = decimal*/)
{
    LPTSTR pszStop;

    if (!IsLoading()) throw E_FAIL;      // use separate error code: wrong mode!!
    if (!m_pXMLDocument) throw E_NOINTERFACE;       // have no XML control loaded !!

    CString strAttrValue;
    ptAttrValue.x = ptAttrValue.y = 0;
    BOOL bFound = GetAttribute(pszAttrName, strAttrValue);
    if (!bFound) {
        return FALSE;
    }

    ptAttrValue.x = ::_tcstol(strAttrValue, &pszStop, 0);     // %TBD% should be baselib call !!!!
    pszStop = _tcschr(pszStop, _T(';') );
    if( pszStop )
    {
       pszStop++;
       ptAttrValue.y = ::_tcstol(pszStop, NULL, 0);     // %TBD% should be baselib call !!!!
    } else {
       assert(FALSE);
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * SetCData
 *      set text elemtent with CDATA handling (honor whitespaces)
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetCData(LPCTSTR pszText)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    HRESULT hr;
    CString strCData;
    // encode special characters like umlauts, <, >, ... etc !!!!!!!
    CString strEncoded;
    //hr = EncodeString(pszText, strEncoded);
    //if (FAILED(hr)) throw hr;
    size_t uiSize = strlen(pszText)*6 + 1; 
    if(!UTIL_String2UTF8(pszText, strEncoded.GetBuffer(uiSize), uiSize))
    {
        hr = E_FAIL;
    }
    strEncoded.ReleaseBuffer();

    strCData.Format(CDATA_INTRO "%s" CDATA_TRAIL, (LPCTSTR)strEncoded);
    hr = SetTextElement(strCData);

    if (FAILED(hr)) throw hr;
}

//------------------------------------------------------------------*
/**
 * SetPCData
 *      set text element with PCDATA handling (strip whitespaces)
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetPCData(LPCTSTR pszText)
{
    if (!IsStoring()) throw E_FAIL;      // use separate error code: wrong mode!!
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    HRESULT hr;
    // encode special characters like umlauts, <, >, ... etc !!!!!!!
    CString strEncoded;
    hr = EncodeString(pszText, strEncoded);
    if (FAILED(hr)) throw hr;
    hr = SetTextElement(strEncoded);

    if (FAILED(hr)) throw hr;
}

//------------------------------------------------------------------*
/**
 * GetCData
 *      set text element handling
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::GetCData(CString &strText)
{
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;
    strText.Empty();

    HRESULT hr = S_FALSE;
    // check if there already is a text child:
    CComPtr<IXMLElementCollection>   pColl;
    hr = m_pCurrentElem->get_children(&pColl);
    if (FAILED(hr)) throw hr;

    CComPtr<IXMLElement2>    pTextElem;
    // if any children at all, search thru kiddy list:
    if (hr != S_FALSE)  // get_children() would have returned S_FALSE if collection were empty
    {
        // look for a already existing text element as child

        try {
            hr = GetElemOfType(pColl, XMLELEMTYPE_TEXT, 1, &pTextElem);
        }
        catch (HRESULT hr) {
            throw hr;
        }
    }

    // if we have not found a text elem child, return empty string, no error
    if (pTextElem)
    {
        BSTR sText;
        hr = pTextElem->get_text(&sText);
        if (FAILED(hr)) throw hr;
        // decode special characters like umlauts, <, >, ... etc !!!!!!!
        hr = DecodeString(sText, strText);
        SysFreeString(sText);
        if (FAILED(hr)) throw hr;
    }
}

//------------------------------------------------------------------*
/**
 * SetComment
 *      set comment, create as node
 *
 * @param           void
 * @return          void
 * @exception       HRESULT
*/
void
    CXMLArchive::SetComment(LPCTSTR pszText)
{
    assert(m_pCurrentElem);
    if (!m_pCurrentElem) throw E_NOINTERFACE;

    // create new comment element
    HRESULT hr;
    CComPtr<IXMLElement2>    pComment;
    hr = CreateNewElem(_T(""), XMLELEMTYPE_COMMENT, &pComment);
    if (FAILED(hr)) throw hr;

    // and add to child collection of current element
    hr = m_pCurrentElem->addChild(pComment, -1, -1);    // add to end of child list
    if (FAILED(hr)) throw hr;
    
    hr = pComment->put_text(CComBSTR(pszText));
    if (FAILED(hr)) throw hr;
}



//------------------------------------------------------------------*
//      Helpers:
//------------------------------------------------------------------*

//------------------------------------------------------------------*
/**
 * CreateXmlControl
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CXMLArchive::CreateXmlControl(BOOL bEmptyRoot /*= TRUE*/)
{
    HRESULT hr;
    SetCurrentElement(NULL);
    m_pXMLDocument = NULL;
    m_pRootElem = NULL;

    // create control; get XML 1.0 document ptr
    IUnknown    *pUnk = NULL;
    hr = ::CoCreateInstance(CLSID_XMLDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDocument2, (void**) &pUnk);
    if (FAILED(hr)) {
        return hr;
    }
    m_pXMLDocument = (IXMLDocument2*) pUnk;
    pUnk->Release();
    if (!bEmptyRoot) {
        hr = CreateRootNode();
    }

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * CreateRootNode
 *      only way to "insert" a root node in IXMLDocument
 *      w/o reading a real XML file
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CXMLArchive::CreateRootNode()
{
    HRESULT hr;
    CComPtr<IStorage>   pStore;
    // create temporary storage to have a stream to fill with tiny XML script
    hr = ::StgCreateDocfile(NULL, 
            STGM_CREATE | STGM_READWRITE
          | STGM_SHARE_EXCLUSIVE | STGM_DELETEONRELEASE, 0, &pStore);
    if (FAILED(hr)) {
        return hr;
    }
    CComPtr<IStream> pStm;
    hr = pStore->CreateStream(L"fifi", STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0,0, &pStm);
    if (FAILED(hr)) {
        return hr;
    }
    CHAR szEmpty[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
//                     "<!DOCTYPE xyz SYSTEM \"xyz.dtd\">\n"
                     "<xyz/>\n";
    ULONG ulWritten = 0;
    hr = pStm->Write(szEmpty, (sizeof(szEmpty) - 1) * sizeof(CHAR), &ulWritten);
    if (FAILED(hr)) {
        return hr;
    }

    pStm.Release(); // close stream and reopen
    hr = pStore->OpenStream(L"fifi", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStm);
    if (FAILED(hr)) {
        return hr;
    }

    // Get the IPersistStreamInit interface to the XML doc.
    CComQIPtr<IPersistStreamInit> pPSI;
    pPSI = m_pXMLDocument;
    if (!pPSI) {
        return E_NOINTERFACE;
    }
    
    // Init the XML doc from the stream.
    hr = pPSI->Load(pStm);
    if (FAILED(hr))
    {
        // Get error information !
        CComQIPtr<IXMLError>  pXMLError;
        XML_ERROR xmle;
        
        pXMLError = pPSI;
        
        assert(pXMLError);
        
        pXMLError->GetErrorInfo(&xmle);
        
        SysFreeString(xmle._pszFound);
        SysFreeString(xmle._pszExpected);
        SysFreeString(xmle._pchBuf);
        return hr;
    }
    return hr;
}


//------------------------------------------------------------------*
/**
 * DropXmlControl
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CXMLArchive::DropXmlControl()
{
    SetCurrentElement(NULL);
    m_pXMLDocument = NULL;
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * CreateNewElem
 *      create new element
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
 */
HRESULT
    CXMLArchive::CreateNewElem(LPCTSTR pszElemName, XMLELEM_TYPE type, IXMLElement2 **ppElem)
{
    CComVariant vType = (long) type;    // force to be a VT_I4
    assert(vType.vt == VT_I4);
    CComVariant vName;
    // must not have name set for text or element types !!!!
    if (   (type != XMLELEMTYPE_TEXT)
        && (type != XMLELEMTYPE_COMMENT))
    {
        vName = pszElemName;
    }

    assert(ppElem != NULL);
    *ppElem = NULL;
    assert(m_pXMLDocument);
    return m_pXMLDocument->createElement(vType, vName, ppElem);
}

//------------------------------------------------------------------*
/**
 * GetRootElem
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
 */
HRESULT
    CXMLArchive::GetRootElem(IXMLElement2 **ppElem)
{
    if (m_pXMLDocument) {
        return m_pXMLDocument->get_root(ppElem);
    }
    *ppElem = NULL;
    return E_FAIL;
}

//------------------------------------------------------------------*
/**
 * SetCurrentElement
 *      set new current XML node in msXML.dll tree
 * @param            
 * @return           
 * @exception        -
 * @see              
 */
HRESULT
    CXMLArchive::SetCurrentElement(IXMLElement2 *pElem)
{
    HRESULT hr = S_FALSE;
    if (pElem)
    {
        BSTR sBuf;
        hr = pElem->get_tagName(&sBuf);
        if (SUCCEEDED(hr)) {
            m_strCurrentNodeName = sBuf;
            ::SysFreeString(sBuf);
        }
        else {
            m_strCurrentNodeName.Empty();
        }
    }
    else {
        m_strCurrentNodeName.Empty();
        m_stackSiblings.RemoveAll();
    }
    m_pCurrentElem = pElem;
    return hr;
}

//------------------------------------------------------------------*
/**
 * GetElemOfType
 *      return lInstance-th (1-based) of element in collection of matching type
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
HRESULT
    CXMLArchive::GetElemOfType(IXMLElementCollection *pColl, XMLELEM_TYPE reqType, long lInstance, IXMLElement2 **ppElem)
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
    // loop thru collection to check for a element of requested type:
    for (long l = 0; l < lCnt; l++)
    {
        // IXMLElementCollection::item() returns IXMLElement2
        CComVariant vIndex = (long) l;
        CComVariant vEmpty;
        IDispatch   *pDisp = NULL;
        hr = pColl->item(vIndex, vEmpty, &pDisp);
        if (FAILED(hr)) throw hr;
        if (pDisp == NULL) throw E_NOINTERFACE;

        // QI to check if we have a element
        CComQIPtr<IXMLElement2> pKid = pDisp;
        pDisp->Release();
        if (pKid)
        {
            // check for matching type:
            long    lType;
            hr = pKid->get_type(&lType);
            if (FAILED(hr)) throw hr;
            XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
            if (type == reqType)
            {
                lInstance--;
                if (lInstance <= 0) {
                    *ppElem = pKid;
                    (*ppElem)->AddRef();
                    m_lSibling = l;     // save as current sibling instance
                    hr = S_OK;
                    break;
                }
            }
        }
    }
    hr = (*ppElem != NULL) ? S_OK : E_INVALIDARG;
    return hr;
}

//------------------------------------------------------------------*
/**
 * GetElemWithMatchingAttr
 *      return element in collection with matching value of requested attribute
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
HRESULT
    CXMLArchive::GetElemWithMatchingAttr(IXMLElementCollection *pColl,
                    LPCTSTR pszAttrName, LPCTSTR pszAttrValue, IXMLElement2 **ppElem)
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
    // loop thru collection to check for a element of requested type:
    for (long l = 0; l < lCnt; l++)
    {
        CComVariant vIndex = (long) l;
        CComVariant vEmpty;
        IDispatch   *pDisp = NULL;
        hr = pColl->item(vIndex, vEmpty, &pDisp);
        if (FAILED(hr)) throw hr;
        if (pDisp == NULL) throw E_NOINTERFACE;

        // QI to check if we have a element
        CComQIPtr<IXMLElement2> pKid = pDisp;
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
                // check for its attribute!
                CComVariant vValue;
                hr = pKid->getAttribute(CComBSTR(pszAttrName), &vValue);
                if (FAILED(hr)) throw hr;
                if (vValue.vt != VT_BSTR) {
                    hr = vValue.ChangeType(VT_BSTR);
                    if (FAILED(hr)) throw hr;
                }
                CString strValue = vValue.bstrVal;
                if (strValue.CompareNoCase(pszAttrValue) == 0)
                {
                    *ppElem = pKid;
                    (*ppElem)->AddRef();
                    m_lSibling = l;     // set also sibling instance !
                    hr = S_OK;
                    break;
                }
            }
        }
    }
    hr = (*ppElem != NULL) ? S_OK : S_FALSE;
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
    CXMLArchive::GetElemWithMatchingName(IXMLElementCollection *pColl,
                    LPCTSTR pszName, long lStart, IXMLElement2 **ppElem)
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
        CComQIPtr<IXMLElement2> pKid = pDisp;
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
                BSTR sBuf;
                hr = pKid->get_tagName(&sBuf);
                if (SUCCEEDED(hr)) {
                    CString strName = sBuf;
                    ::SysFreeString(sBuf);
                    if (strName.CompareNoCase(pszName) == 0)
                    {
                        *ppElem = pKid;
                        (*ppElem)->AddRef();
                        m_lSibling = l;     // set also sibling instance !
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


//------------------------------------------------------------------*
/**
 * SetTextElement
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
 */
HRESULT
    CXMLArchive::SetTextElement(LPCTSTR pszText)
{
    HRESULT hr;
    
    // check if there already is a text child:
    CComPtr<IXMLElementCollection>   pColl;
    hr = m_pCurrentElem->get_children(&pColl);
    if (FAILED(hr)) throw hr;

    CComPtr<IXMLElement2>    pTextElem;
    // if any children at all, search thru kiddy list:
    if (hr != S_FALSE)  // get_children() would have returned S_FALSE if collection were empty
    {
        // look for a already existing text element as child

        try {
            hr = GetElemOfType(pColl, XMLELEMTYPE_TEXT, 1, &pTextElem);
        }
        catch (HRESULT hr) {
            throw hr;
        }
    }

    // if we have not found a text elem child, create a new one!
    if (!pTextElem)
    {
        // create new text element
        hr = CreateNewElem(_T(""), XMLELEMTYPE_TEXT, &pTextElem);
        if (FAILED(hr)) throw hr;
        // and add to child collection of current element
        hr = m_pCurrentElem->addChild(pTextElem, -1, -1);    // add to end of child list
        if (FAILED(hr)) throw hr;
    }
    
    // set text to new elem
    hr = E_FAIL;
    if (pTextElem)
    {
        hr = pTextElem->put_text(CComBSTR(pszText));
    }

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * WriteXML
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
 */
HRESULT
    CXMLArchive::WriteXML()
{
    CFile   *pFile = GetFile();
    assert(pFile);
    if (!pFile) {
        return E_FAIL;
    }
    // write XML version string and char set
    try
    {
        m_iIndent = 0;
        CString strBuf;
        const char cchXML_Tag[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
        pFile->Write(cchXML_Tag, (sizeof(cchXML_Tag)-1) * sizeof(char));
        
        // write DOCTYPE
        if (   !m_strDocType.IsEmpty()
            && !m_strDTD.IsEmpty())
        {
            // %TBD% in version 1.1 we never write DOCTYPE to avoid unsolved problem of location
            // of DTDs (especially for clipboard files in temp directory!!!)
//            strBuf.Format("<!DOCTYPE %s SYSTEM \"%s\">\r\n", (LPCTSTR) m_strDocType, (LPCTSTR) m_strDTD);
//            pFile->Write((LPCSTR) strBuf, strBuf.GetLength());
        }
        
        // write root node; same as doctype!
        // start dump at top; walk thru tree recursively
        WriteNode(m_pRootElem, m_strDocType);
        
    }
    catch (HRESULT hr)
    {
        return hr;
    }
    catch (CFileException *e)
    {
        e->Delete();
        return E_FAIL;
    }

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * WriteNode
 *      write nndes recursivly
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
void
    CXMLArchive::WriteNode(IXMLElement2 *pElem, LPCTSTR pszNodeName)
{
    CFile   *pFile = GetFile();
    assert(pFile);
    assert(pElem);

    CString strBuf;

    // print attributes if any:
    // check if there already is a text child:
    CComPtr<IXMLElementCollection>   pCollAttr;
    CComPtr<IXMLElementCollection>   pCollNodes;
    HRESULT hr;

    // tag intro
    strBuf.Format("<%s", pszNodeName);
    WriteIndent();
    pFile->Write(strBuf, strBuf.GetLength());

    // get_children/get_attributes returned S_FALSE if collection were empty
    hr = pElem->get_attributes(&pCollAttr);
    if (FAILED(hr)) throw hr;
    hr = pElem->get_children(&pCollNodes);
    if (FAILED(hr)) throw hr;
    BOOL bNoChildNodes = ((hr == S_FALSE) | (pCollNodes == NULL));

    // write attr list:
    if (pCollAttr)
    {
        WriteAttributes(pCollAttr);
    }
    // complete intro tag
    if (bNoChildNodes) {
        strBuf = " />\r\n";
    }
    else {
        strBuf = ">\r\n";
    }
    pFile->Write(strBuf, strBuf.GetLength());

    // write any child nodes
    if (pCollNodes)
    {
        // print child nodes and walk down the tree!
        long lCnt = 0;
        hr = pCollNodes->get_length(&lCnt);
        if (FAILED(hr)) throw hr;
        // loop thru collection and write all children
        for (long l = 0; l < lCnt; l++)
        {
            // IXMLElementCollection::item() returns IXMLElement2
            CComVariant vIndex = (long) l;
            CComVariant vEmpty;
            IDispatch   *pDisp = NULL;
            hr = pCollNodes->item(vIndex, vEmpty, &pDisp);
            if (SUCCEEDED(hr))
            {
                // QI to check if we have a element
                CComQIPtr<IXMLElement2> pKid = pDisp;
                pDisp->Release();
                if (pKid)
                {
                    // check for type: only decend if element
                    long    lType;
                    hr = pKid->get_type(&lType);
                    if (FAILED(hr)) throw hr;
                    XMLELEM_TYPE type = (XMLELEM_TYPE) lType;
                    if (type == XMLELEMTYPE_ELEMENT)
                    {
                        // walk down tree
                        BSTR    sName;
                        hr = pKid->get_tagName(&sName);
                        if (FAILED(hr)) throw hr;
                        m_iIndent++;
                        WriteNode(pKid, CString(sName));
                        m_iIndent--;
                        SysFreeString(sName);
                    }
                    else if (type == XMLELEMTYPE_TEXT)
                    {
                        // print text of element
                        BSTR    sText;
                        hr = pKid->get_text(&sText);
                        if (FAILED(hr)) throw hr;
                        strBuf = sText;
                        SysFreeString(sText);
                        // check if need to indent
                        if (strBuf.Find(CDATA_INTRO) >= 0) {
                            // CDATA is not indented
                            pFile->Write((LPCTSTR) strBuf, strBuf.GetLength());
                        }
                        else {
                            m_iIndent++;
                            WriteIndented(strBuf);
                            m_iIndent--;
                        }
                    }
                    else if (type == XMLELEMTYPE_COMMENT)
                    {
                        // print comment
                        BSTR    sText;
                        hr = pKid->get_text(&sText);
                        if (FAILED(hr)) throw hr;
                        strBuf = sText;
                        SysFreeString(sText);
                            m_iIndent++;
                        WriteIndent();
                            m_iIndent--;
                        pFile->Write(COMMENT_INTRO, (sizeof(COMMENT_INTRO)-1)*sizeof(char));
                        pFile->Write(strBuf, strBuf.GetLength());
                        pFile->Write(COMMENT_TRAIL, (sizeof(COMMENT_TRAIL)-1)*sizeof(char));
                    }
                }
            }
        }
        if (lCnt > 0)
        {
            // terminating tag to "close" this element
            strBuf = "\r\n";
            pFile->Write(strBuf, strBuf.GetLength());
            WriteIndent();
            strBuf.Format("</%s>\r\n", pszNodeName);
            pFile->Write(strBuf, strBuf.GetLength());
        }
    }
}


//------------------------------------------------------------------*
/**
 * WriteAttributes
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
void
    CXMLArchive::WriteAttributes(IXMLElementCollection *pColl)
{
    long lCnt = 0;
    assert(pColl);
    HRESULT hr = pColl->get_length(&lCnt);
    if (FAILED(hr)) throw hr;

    CFile   *pFile = GetFile();
    CString strBuf;
    // loop thru collection and write all attributes
    for (long l = 0; l < lCnt; l++)
    {
        // IXMLElementCollection::item() returns IXMLAttribute
        CComVariant vIndex = (long) l;
        CComVariant vEmpty;
        IDispatch   *pDisp = NULL;
        hr = pColl->item(vIndex, vEmpty, &pDisp);
        if (SUCCEEDED(hr) && (pDisp != NULL))
        {
            // QI to check if we have a element
            CComQIPtr<IXMLAttribute> pAttr = pDisp;
            pDisp->Release();
            if (!pAttr) throw E_NOINTERFACE;
            // have child attribute, write name and value
            BSTR sName, sVal;
            hr = pAttr->get_name(&sName);
            if (FAILED(hr)) throw hr;
            hr = pAttr->get_value(&sVal);
            if (FAILED(hr)) throw hr;

            //EF: don't call EscapeQuote since this is done 
            //in UTIL_XMLEncode/Decode anyway:
            CString strVal = /*EscapeQuote*/(sVal); 
            strBuf.Format(" %s=\"%s\"", (LPCSTR)CString(sName), (LPCSTR)strVal);
            
            
            pFile->Write((LPCSTR) strBuf, strBuf.GetLength());
            SysFreeString(sVal);
            SysFreeString(sName);
        }
    }
}

//------------------------------------------------------------------*
void
    CXMLArchive::WriteIndent()
{
    if (m_iIndent > 0)
    {
        CString strIndent(' ', m_iIndent * INDENT);
        CFile   *pFile = GetFile();
        pFile->Write((LPCSTR) strIndent, strIndent.GetLength());
    }
}

//------------------------------------------------------------------*
void
    CXMLArchive::WriteIndented(LPCTSTR pszText)
{
    CFile   *pFile = GetFile();
    LPCTSTR psz = pszText;
    while (*psz != '\0')
    {
        LPCTSTR pszBreak = strpbrk(psz, "\r\n");
        if (pszBreak != NULL)
        {
            LPCTSTR pszNext = pszBreak + 1;
            if (   (*pszNext == '\r')
                || (*pszNext == '\n'))
            {
                pszBreak++;
            }
            CString strLine = CString(psz, pszBreak - psz);
            WriteIndent();
            pFile->Write((LPCSTR) strLine, strLine.GetLength());
            psz = pszBreak + 1;
        }
        else
        {
            WriteIndent();
            pFile->Write(psz, strlen(psz));
            LPCTSTR pszLast = psz - 1;
            if (   (*pszLast != '\r')
                && (*pszLast != '\n'))
            {
                pFile->Write("\r\n", strlen("\r\n"));
            }

            break;
        }
    }
}


//------------------------------------------------------------------*
/**
 * EncodeString
 *      encode special characters like umlauts, <, >, ... etc
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
HRESULT
    CXMLArchive::EncodeString(LPCTSTR pszText, CString &strEncoded)
{
    CComBSTR sIn = pszText;
    BSTR sEncoded;
    HRESULT hr = ::UTIL_XMLEncodeString(sIn, &sEncoded, XML_ENCODE_DEFAULT);
    if (FAILED(hr)) throw hr;
    strEncoded = sEncoded;
    SysFreeString(sEncoded);
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * DecodeString
 *      decode special characters like umlauts, <, >, ... etc
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
HRESULT
    CXMLArchive::DecodeString(BSTR sRaw, CString &strDecoded)
{
    BSTR sDecoded;
    HRESULT hr = ::UTIL_XMLDecodeString(sRaw, &sDecoded, XML_ENCODE_DEFAULT);
    if (FAILED(hr)) throw hr;
    strDecoded = sDecoded;
    SysFreeString(sDecoded);
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * EscapeQuote
 *      escape double quotes
 *
 * @param            
 * @return           
 * @exception        HRESULT
 * @see              
 */
CString
    CXMLArchive::EscapeQuote(BSTR sVal)
{
    CString strIn = sVal;
    LPCTSTR pszStart = strIn;
    LPCTSTR psz = pszStart;
    CString strEncoded;
    while (*psz != _T('\0'))
    {
        if (*psz == _T('\"'))
        {
            if ((psz - pszStart) > 0) {
                strEncoded += CString(pszStart, psz - pszStart);
            }
            strEncoded += _T("&#34;");      // escaped quote
            pszStart = ++psz;
        }
        else {
            psz++;
        }
    }
    if (psz > pszStart)
    {
        strEncoded += CString(pszStart, psz - pszStart);
    }

    return strEncoded;
}

//------------------------------------------------------------------*
/**
 * ctor
 *
 * @param            void
 * @return           void
 * @exception        -
 * @see              
 */
NodeIter::NodeIter(CXMLArchive &ar)
  : m_pArchive(NULL),
    m_pos(NULL),
    m_lCnt(0)
{
    m_pArchive = &ar;
    assert(m_pArchive != NULL);
}


//------------------------------------------------------------------*
/**
 * dtor
 *
 * @param            void
 * @return           void
 * @exception        -
 * @see              
 */
NodeIter::~NodeIter()
{
    assert(m_pArchive != NULL);
    if (m_pArchive != NULL)
    {
//        m_pArchive->SetCurrentNodeName(m_strSaveNodeName);
        m_pArchive->SetCurrentElement(m_pCurrSaved);
    }
    m_pArchive = NULL;
}


//------------------------------------------------------------------*
/**
 * begin
 *      set current position to first element in child collection
 *      of current XML node
 *
 * @param            void
 * @return           void
 * @exception        HRESULT
 * @see              
 */
POSITION
    NodeIter::begin()
{
    m_pos = NULL;
    m_lCnt = 0;
    HRESULT hr;
    assert(m_pArchive != NULL);
    hr = m_pArchive->GetCurrentNodeName(m_strSaveNodeName);
    if (FAILED(hr)) throw hr;
    hr = m_pArchive->GetCurrentElement(&m_pCurrSaved);
    if (FAILED(hr)) throw hr;

    hr = m_pCurrSaved->get_children(&m_pColl);
    if (FAILED(hr)) throw hr;
    if (hr == S_FALSE) {
        // empty collection!
        return NULL;
    }

    hr = m_pColl->get_length(&m_lCnt);
    if (FAILED(hr)) throw hr;
    CComQIPtr<IXMLElement2> pKid;
    m_pos = (POSITION) 1;
    advanceToNextElement(pKid, m_pos);

    return m_pos;
}

//------------------------------------------------------------------*
/**
 * next
 *      move forward to next cild
 *
 * @param            void
 * @return           ptr to current XML element; DO NOT RELEASE, IS RELEASED INTERNALLY!
 * @exception        HRESULT
 * @see              
 */
IXMLElement2 *
    NodeIter::next(POSITION &pos)
{
    CComQIPtr<IXMLElement2> pKid;
    pos = advanceToNextElement(pKid, pos);
    if (pKid)
    {
        HRESULT hr;
        // get node name:
        BSTR sNodeName;
        hr = pKid->get_tagName(&sNodeName);
        if (FAILED(hr)) throw hr;
        assert(m_pArchive != NULL);
        SysFreeString(sNodeName);
    }
    m_pArchive->SetCurrentElement(pKid);
    IXMLElement2 * pRet = pKid;
    return pRet;
}

//------------------------------------------------------------------*
/**
 * advanceToNextElement
 *      move forward to next cild
 *
 * @param            posCurr    current valid pos
 * @return           next valid pos
 * @exception        HRESULT
 * @see              
 */
POSITION
    NodeIter::advanceToNextElement(CComQIPtr<IXMLElement2> &pKid, POSITION &posCurr)
{
    long lCurr = ((long) posCurr) - 1;
    BOOL bDone = FALSE;
    POSITION posNext = NULL;
    do
    {
        // check current pos
        CComVariant vIndex = lCurr;
        if (lCurr < m_lCnt) {
            posCurr = (POSITION) lCurr + 1;
        }
        else {
            posCurr = NULL;
            posNext = NULL;
            break;
        }
        lCurr++;
        
        // determine next valid pos:
        if ((lCurr + 1) <= m_lCnt) {
            posNext = (POSITION) lCurr + 1;
        }
        else {
            posNext = NULL;
        }
        
        CComVariant vEmpty;
        IDispatch   *pDisp = NULL;
        HRESULT hr;
        hr = m_pColl->item(vIndex, vEmpty, &pDisp);
        if (FAILED(hr)) throw hr;
        if (pDisp == NULL) throw E_NOINTERFACE;
        
        // QI to check if we have a element
        pKid = pDisp;
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
                // get node name:
                bDone = TRUE;
                break;
            }
        }
    } while (!bDone);
    return posNext;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: XMLObject.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/XML_Wrapper
 * 
 * *****************  Version 6  *****************
 * User: Sis          Date: 1.10.03    Time: 14:56
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * CString not allowed in header file (2)
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 1.10.03    Time: 14:29
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 1.10.03    Time: 9:50
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * bug fix: UTIL_String2UTF8 and CString
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 29.09.03   Time: 18:08
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * bug fix d-76: Euro sign problem in GR-Editor
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 29.09.03   Time: 14:00
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * bug fix d-2109: SetStringAttributeUTF8()
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 12.12.01   Time: 22:01
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * Added IsElementExisting() to cope with problems encounted with
 * MoveDown().
 * 
 * *****************  Version 2  *****************
 * User: Scd          Date: 31.01.01   Time: 14:16
 * Updated in $/4Control/COM/softing/fc/CMN/XML_Wrapper
 * new headers added
 *==
 *----------------------------------------------------------------------------*
*/
