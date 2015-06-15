/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/XML_Wrapper.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: XML_Wrapper.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/XML_Wrapper.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CMN
 *
 * =COMPONENT           XML DOM Wrapper and MFC integration
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
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

#ifndef __XML_WRAPPER_H_
#define __XML_WRAPPER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef __ATLBASE_H__
    #error XML Wrapper library requires ATL! Include <atlbase.h> before including this header!
#endif

#ifndef __CXMLOBJECT_H_
#define __CXMLOBJECT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <msxml.h>      // this includes header of IE4 delivered msxml control !!
#include "msXML_2.h"      // must include this local MIDL compiled header!!

//----  Forward Class Definitions:   -------------------------------*
class   CXMLArchive;
class   CXMLObject;

//----  Defines:    ------------------------------------------------*
// #define _USE_IE5_MSXML

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//----  Typedefs:    -----------------------------------------------*
typedef CMap<CString, LPCTSTR, CRuntimeClass *, CRuntimeClass *&>   classMap;
typedef CMap<CString, LPCTSTR, CString, LPCTSTR>                    classNameMap;
typedef CList<long, long&>                                          stackSiblings;

//------------------------------------------------------------------*
/**
 *  class CXMLArchive
 *
 *  @see    ....
*/
class CXMLArchive :
            public CArchive
{
    friend class NodeIter;
// =Construction
public:
                            CXMLArchive(CFile* pFile, UINT nMode);
                                // constructor, load also msXML.dll via CoCreateInstance()
                                // parameters as in <b>CArchive</b>
    virtual                 ~CXMLArchive();
                                // dtor
private:
                            CXMLArchive(CXMLArchive const & other);
                                // copy ctor
    CXMLArchive const &    operator = (CXMLArchive const & other);
                                // assignment

public:
    enum NumericFormat {
        decimal,
        hex,
        bin,
        oct
    };
// =Access
public:
    HRESULT                 GetLastParseError(CString &strLastError)
                                // return string of last XML parser error
                            {
                                strLastError = m_strError;
                                return (m_strError.IsEmpty()) ? S_FALSE : S_OK;
                            }
    HRESULT                 GetCurrentElement(IXMLElement2 **ppElem)
                                // return ptr to current XML node element node in msXML.dll tree
                                // caller must Release() when no more in use !!!
                            {
                                if (ppElem == NULL) {
                                    return E_POINTER;
                                }
                                *ppElem = m_pCurrentElem;
                                if (*ppElem) {
                                    (*ppElem)->AddRef();
                                }
                                return S_OK;
                            }
    HRESULT                 GetXMLDocument(IXMLDocument2 **ppDoc)
                                // return ptr to current XML document in msXML.dll tree
                                // caller must Release() when no more in use !!!
                            {
                                if (ppDoc == NULL) {
                                    return E_POINTER;
                                }
                                *ppDoc = m_pXMLDocument;
                                if (*ppDoc) {
                                    (*ppDoc)->AddRef();
                                }
                                return S_OK;
                            }
    HRESULT                 GetCurrentNodeName(CString &strNodeName)
                                // when loading/writing, return current active node name
                            {
                                strNodeName = m_strCurrentNodeName;
                                return S_OK;
                            }

// =Operations
public:
    // all operations throw HRESULT exceptions !!!!!!!
    // to/from XML file
    void                    Load();
                                // parse XML file and prepare for walking thru tree
    void                    Flush();
                                // write tree in memory to XML file

    void                    AddToClassMap(CXMLObject *pObj, LPCTSTR pszNodeName);
    void                    AddToClassMap(CRuntimeClass *pRT, LPCTSTR pszNodeName);
                                // add runtime class info of pObj and XML node name to map
                                // <b>must be called prior to serialize object for</b>
                                // <b>both storing and loading !! </b>
    void                    WriteObject(const CXMLObject *pObj);
                                // do not call directly, commonly used from within stream operators !!
    CXMLObject *            ReadObject(CXMLObject *pExistingObj = NULL);
                                // do not call directly, commonly used from within stream operators !!


    // work on tree in memory, good for read and write
    void                    Clear();
                                // cleanup XML element tree in memory
    BOOL                    MoveDown(LPCTSTR pszElemName);
                                // walk down one elem hierarchy to element
                                // will create new node, throws when not found in Class Map !!
                                // works for read and write !!
                                // returns TRUE if node exists (loading) or node was created (writing)
    void                    MoveUp();
                                // walk up one elem node level
    BOOL                    MoveToSibling(LPCTSTR pszAttrName, LPCTSTR pszAttrValue);
                                // walk to siblings in same level,
                                // matching the given attribute and value
                                // valid in read mode only!
    BOOL                    MoveNext();
                                // walk to siblings in same level,
                                // assume identical name from previous MoveDown()
                                // adds sibling elem node when in writing node
    BOOL                    IsElementExisting(LPCTSTR pszElemName);

    // writing to XML:
    void                    SetDocType(LPCTSTR pszDocType);
                                // call once to set doc type
                                // <b>must</b> be called once before <b>Flush</b>()ing
    void                    SetDTD(LPCTSTR pszDTD);
                                // call once to set DTD URL
    void                    SetComment(LPCTSTR pszText);
                                // set comment, create as node
    void                    SetAttribute(LPCTSTR pszAttrName, LPCTSTR pszAttrValue);
                                // set attribute value of current elem node

    // SIS: use this function, if string is generated from external and may contain non ASCII characters:
    void                    SetStringAttributeUTF8(LPCTSTR pszAttrName, LPCTSTR pszAttrValue);
                                // set string attribute value of current elem node using UTF-8 coding

    void                    SetAttribute(LPCTSTR pszAttrName, long iAttrValue, enum NumericFormat fmt = decimal);
                                // set attribute value of current elem node
    void                    SetAttribute(LPCTSTR pszAttrName, ULONG iAttrValue, enum NumericFormat fmt = decimal);
                                // set attribute value of current elem node
    void                    SetAttribute(LPCTSTR pszAttrName, CPoint ptAttrValue, enum NumericFormat fmt = decimal);
                                // set attribute value of current elem node
    void                    SetCData(LPCTSTR pszText);
                                // set text element with CDATA handling
                                // honor whitespaces by wrapping in &lt;![CDATA[
    void                    SetPCData(LPCTSTR pszText);
                                // set text element with PCDATA handling
                                // strip whitespaces (when parsed, not when saving)

    // reading from XML
    void                    GetDocType(CString &strType)
                                // return parsed doc type, can be empty!
                            {
                                strType = m_strDocType;
                            }
    void                    GetDTD(CString &strDTD)
                                // return parsed DTD URL, can be empty!
                            {
                                strDTD = m_strDTD;
                            }
    BOOL                    GetAttribute(LPCTSTR pszAttrName, CString &strAttrValue);
                                // get attribute value of current elem node
    BOOL                    GetAttribute(LPCTSTR pszAttrName, long &iAttrValue, enum NumericFormat fmt = decimal);
                                // get attribute value of current elem node, format is ignored, compatibility only!
    BOOL                    GetAttribute(LPCTSTR pszAttrName, ULONG &iAttrValue, enum NumericFormat fmt = decimal);
                                // get attribute value of current elem node
    BOOL                    GetAttribute(LPCTSTR pszAttrName, CPoint &ptAttrValue, enum NumericFormat fmt = decimal);
                                // get attribute value of current elem node;
                                // value is a ';' separated point tupel
    void                    GetCData(CString &strText);
                                // get text elemtent


// =Overrides
public:

protected:
    HRESULT                 SetCurrentElement(IXMLElement2 *pElem);
                                // set new current XML node in msXML.dll tree
//    void                    SetCurrentNodeName(LPCTSTR pszNodeName)
//                            {
//                                m_strCurrentNodeName = pszNodeName;
//                            }

// =Helpers
private:
    HRESULT                 CreateXmlControl(BOOL bEmptyRoot = TRUE);
    HRESULT                 DropXmlControl();
    HRESULT                 WriteXML();

    HRESULT                 CreateNewElem(LPCTSTR pszElemName, XMLELEM_TYPE type, IXMLElement2 **ppElem);
    HRESULT                 GetRootElem(IXMLElement2 **ppElem);
    HRESULT                 GetElemOfType(IXMLElementCollection *pColl, XMLELEM_TYPE reqType,
                                        long lInstance, IXMLElement2 **ppElem);
    HRESULT                 GetElemWithMatchingAttr(IXMLElementCollection *pColl, LPCTSTR pszAttrName, LPCTSTR pszAttrValue, IXMLElement2 **ppElem);
                                // return element in collection with matching value of requested attribute
    HRESULT                 GetElemWithMatchingName(IXMLElementCollection *pColl,
                                        LPCTSTR pszName, long lInstance, IXMLElement2 **ppElem);
                                // return lInstance-th element in collection with matching name
    HRESULT                 CreateRootNode();
    void                    WriteNode(IXMLElement2 *pElem, LPCTSTR pszNodeName); // throws file exception or HRESULT
    void                    WriteAttributes(IXMLElementCollection *pColl);
    HRESULT                 SetTextElement(LPCTSTR pszText);
    HRESULT                 EncodeString(LPCTSTR pszText, CString &strEncoded);
                                // encode/decode special characters like umlauts, <, >, ... etc
    HRESULT                 DecodeString(BSTR sText, CString &strDecoded);
    LPCTSTR                 Encode(TCHAR ch);
    TCHAR                   Decode(LPCTSTR pszNamed);
    CString                 EscapeQuote(BSTR sVal);
    void                    WriteIndent();
    void                    WriteIndented(LPCTSTR pszText);

// =Attributes
protected:
    CComPtr<IXMLDocument2>  m_pXMLDocument;
    CComPtr<IXMLElement2>   m_pCurrentElem;
    long                    m_lSibling;
                                // instance of sibling, 0-based!
    stackSiblings           m_stackSiblings;
                                // stack of previous level siblings
    CComPtr<IXMLElement2>   m_pRootElem;
                                // only set if storing; to circumvent missing method
                                // in IE4's msXML to add elements to empty document!
    CString                 m_strDocType;
    CString                 m_strDTD;

    CString                 m_strError;
                                // last parser error
    int                     m_iIndent;

    classNameMap            m_mapClassToNode;
                                // map c++ class names to XML node names
                                // used when storing classes into XML file
    classMap                m_mapNodeToClass;
                                // map XML node names to C++ class names (CRuntimeClass *)
                                // used when loading classes from XML file
    CString                 m_strCurrentNodeName;
                                // when loading/writing, holds current active node name

private:

};

//------------------------------------------------------------------*
/**
 *  class CXMLObject
 *
 *  @see    ....
*/
class CXMLObject :
            public CObject
{
// =Serialization
protected:
                            CXMLObject();
                                // create when serializing

    DECLARE_DYNCREATE(CXMLObject);

// =Construction
public:
    virtual                 ~CXMLObject();
                                // dtor
private:
                            CXMLObject(CXMLObject const & other);
                                // copy ctor
    CXMLObject const &    operator = (CXMLObject const & other);
                                // assignment

// =Access
public:

// =Operations
public:

// =Overrides
public:
    virtual void            SerializeXML(CXMLArchive &ar);

protected:

// =Helpers
private:

// =Attributes
protected:

private:

};

//------------------------------------------------------------------*
/**
 *  class CXMLDocument
 *
 *  @see    ....
*/
template <class T_derivingCDocument>
class CXMLDocument :
    public T_derivingCDocument
{
// =Construction
public:
//                            CXMLDocument() : T_derivingCDocument;
                            CXMLDocument() {};
                                // default ctor

// =Access
public:

// =Operations
public:

// =Overrides
public:
	virtual void            SerializeXML(CXMLArchive& ar) = 0;
	virtual BOOL            OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL            OnSaveDocument(LPCTSTR lpszPathName);

protected:

// =Helpers
private:

// =Attributes
protected:

private:

};


//------------------------------------------------------------------*
class NodeIter
{
public:
                            NodeIter(CXMLArchive &ar);
                            ~NodeIter();

// =Operations
public:
    POSITION                begin(void);
    IXMLElement2 *          next(POSITION &pos);
    long                    getMaxCount(void)
                            {
                                return  m_lCnt;
                            }

// =Helpers
private:
    POSITION                advanceToNextElement(CComQIPtr<IXMLElement2> &pKid, POSITION &posCurr);

// =Attribute
private:
    CXMLArchive            *m_pArchive;
    CString                 m_strSaveNodeName;
    CComPtr<IXMLElement2>   m_pCurrSaved;
    CComPtr<IXMLElementCollection>   m_pColl;
    POSITION                m_pos;
    long                    m_lCnt;

};

//------------------------------------------------------------------*
template <class T_derivingCDocument>
BOOL
    CXMLDocument<T_derivingCDocument>::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

    CXMLArchive *pLoadArchive = NULL;
    try {
        pLoadArchive = new CXMLArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
    }
    catch (HRESULT hr)
    {
        // could not create archive
        TRACE(_T("cannot open CXMLArchive for loading, fired hr=0x%08X\n"), hr);
		ReleaseFile(pFile, TRUE);
        if (pLoadArchive) {
            delete pLoadArchive;
        }
        return FALSE;
    }
    catch (CException  *e)
    {
        e->Delete();
    }
	pLoadArchive->m_pDocument = this;
	pLoadArchive->m_bForceFlat = FALSE;
    try
    {
        CWaitCursor wait;
        if (pFile->GetLength() != 0)
        {
            // catch any XML-parse errors !!!!!!!
            try {
                pLoadArchive->Load();
            }
            catch (HRESULT hr)
            {
                CString strError;
                hr = pLoadArchive->GetLastParseError(strError);
                if (!strError.IsEmpty())
                {
                    CString strMsg = _T("Error while parsing XML file:\n");
                    strMsg += lpszPathName;
                    strMsg += _T("\n\n");
                    strMsg += strError;
                    AfxMessageBox(strMsg, MB_ICONSTOP);
                }
                pLoadArchive->Close();
                AfxThrowArchiveException(CArchiveException::badIndex, lpszPathName);
            }
            
            // catch any unhandled serialize errors !!!!!!!
            try {
                SerializeXML(*pLoadArchive);     // load me
            }
            catch (HRESULT hr)
            {
#define ERROR_UNHANDLED_EXCEPTION_WHILE_READING_XML 0
                assert(ERROR_UNHANDLED_EXCEPTION_WHILE_READING_XML);
                TRACE(_T("ERROR: unhandled exception while reading XML-file %s, hr=0x%08X\n"), lpszPathName, hr);
                pLoadArchive->Close();
                AfxThrowArchiveException(CArchiveException::badIndex, lpszPathName);
            }
            
            pLoadArchive->Close();
            ReleaseFile(pFile, TRUE);
        }	
    }
    catch (CException *e)
    {
        ReleaseFile(pFile, TRUE);
        DeleteContents();   // remove failed contents
        
        try
        {
            ReportSaveLoadException(lpszPathName, e,
                FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        }
        catch (...)
        {
        }
        e->Delete();
        delete pLoadArchive;
        return FALSE;
    }
    
    delete pLoadArchive;
	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}

//------------------------------------------------------------------*
template <class T_derivingCDocument>
BOOL
    CXMLDocument<T_derivingCDocument>::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFileException fe;
	CFile* pFile = NULL;
	pFile = GetFile(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);

	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

    CXMLArchive *pSaveArchive = NULL;
    try {
        pSaveArchive = new CXMLArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
    }
    catch (HRESULT hr)
    {
        // could not create archive
        TRACE(_T("cannot create CXMLArchive, fired hr=0x%08X\n"), hr);
		ReleaseFile(pFile, TRUE);
        if (pSaveArchive) {
            delete pSaveArchive;
        }
        return FALSE;
    }
    catch (CException  *e)
    {
        e->Delete();
    }
	pSaveArchive->m_pDocument = this;
	pSaveArchive->m_bForceFlat = FALSE;
    try
    {
		CWaitCursor wait;
        // catch any unhandled serialize errors !!!!!!!
        try
        {
            SerializeXML(*pSaveArchive);     // save me
        }
        catch (HRESULT hr)
        {
#define ERROR_UNHANDLED_EXCEPTION_WHILE_WRITING_XML 0
            assert(ERROR_UNHANDLED_EXCEPTION_WHILE_WRITING_XML);
            TRACE(_T("ERROR: unhandled exception while saving XML-file %s, hr=0x%08X\n"), lpszPathName, hr);
            pSaveArchive->Close();
            AfxThrowArchiveException(CArchiveException::generic, lpszPathName);
        }
        // regular save of file
        pSaveArchive->Flush();
        pSaveArchive->Close();
        ReleaseFile(pFile, FALSE);
    }
	catch (CException *e)
	{
		ReleaseFile(pFile, TRUE);

		try
		{
			ReportSaveLoadException(lpszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		}
        catch (...) { }
        e->Delete();
        delete pSaveArchive;
		return FALSE;
	}

    delete pSaveArchive;
	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
}


//------------------------------------------------------------------*
inline CXMLArchive& 
    operator<<(CXMLArchive& ar, const CXMLObject* pOb)
{
    assert(pOb);
    ar.WriteObject(pOb);
    return ar;
}

//------------------------------------------------------------------*
inline CXMLArchive& 
    operator>>(CXMLArchive& ar, CXMLObject*& pOb)
{
	pOb = ar.ReadObject();
    return ar;
}

//------------------------------------------------------------------*
inline CXMLArchive& 
    operator>>(CXMLArchive& ar, const CXMLObject*& pOb)
{
	pOb = ar.ReadObject();
    return ar;
}

//------------------------------------------------------------------*
// support for MFC's container templates
#ifndef __AFXTEMPL_H__
#pragma message("WARNING: To use stream operator templates, <afxtempl.h> must be included first")

#else   // __AFXTEMPL_H__

//------------------------------------------------------------------*
template<class TYPE, class ARG_TYPE>
inline CXMLArchive&
    operator<<(CXMLArchive& ar, const CList<TYPE, ARG_TYPE> * pList)
{
    assert(pList);
    for (POSITION pos = pList->GetHeadPosition(); pos != NULL; )
    {
        CXMLObject *pObj = pList->GetNext(pos);
        assert(pObj != NULL);
        ASSERT_KINDOF(CXMLObject, pObj);

        ar << pObj;
    }
    return ar;
}

//------------------------------------------------------------------*
inline CXMLArchive&
    operator<<(CXMLArchive& ar, const CPtrList * pPtrList)
{
    assert(pPtrList);
    for (POSITION pos = pPtrList->GetHeadPosition(); pos != NULL; )
    {
        CXMLObject *pObj = (CXMLObject *)pPtrList->GetNext(pos);
        assert(pObj != NULL);
        ASSERT_KINDOF(CXMLObject, pObj);

        ar << pObj;
    }
    return ar;
}

//------------------------------------------------------------------*
template<class BASE_CLASS, class TYPE >
inline CXMLArchive&
    operator<<(CXMLArchive& ar, const CTypedPtrList<BASE_CLASS, TYPE> * pPtrList)
{
    assert(pPtrList);
    for (POSITION pos = pPtrList->GetHeadPosition(); pos != NULL; )
    {
        CXMLObject *pObj = (CXMLObject *)pPtrList->GetNext(pos);
        assert(pObj != NULL);
        ASSERT_KINDOF(CXMLObject, pObj);

        ar << pObj;
    }
    return ar;
}

//------------------------------------------------------------------*
template<class TYPE, class ARG_TYPE>
inline CXMLArchive&
    operator<<(CXMLArchive& ar, const CArray<TYPE, ARG_TYPE> * pArray)
{
    assert(pArray);
    int iCnt = pArray->GetSize();
    for (int i = 0; i < iCnt; i++)
    {
        CXMLObject *pObj = pArray->GetAt(i);
        assert(pObj != NULL);
        ASSERT_KINDOF(CXMLObject, pObj);

        ar << pObj;
    }
    return ar;
}

//------------------------------------------------------------------*
template<class BASE_CLASS, class TYPE >
inline CXMLArchive&
    operator<<(CXMLArchive& ar, const CTypedPtrArray<BASE_CLASS, TYPE> * pTypedPtrArray)
{
    assert(pTypedPtrArray);
    int iCnt = pTypedPtrArray->GetSize();
    for (int i = 0; i < iCnt; i++)
    {
        CXMLObject *pObj = pTypedPtrArray->GetAt(i);
        assert(pObj != NULL);
        ASSERT_KINDOF(CXMLObject, pObj);

        ar << pObj;
    }
    return ar;
}

/*  // %TBD%    need to define how map is serialized to XML:
    //          are keys also associated with (different) XML node ?????
//------------------------------------------------------------------*
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline CXMLArchive&
    operator<<(CXMLArchive& ar, const CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> * pMap)
{
    assert(pList);
    for (POSITION pos = pMap->GetHeadPosition(); pos != NULL; )
    {
        KEY& rKey, VALUE& rValue
        pMap->GetNextAssoc(pos, key, rValue);
        assert(rValue != NULL);
        ASSERT_KINDOF(CXMLObject, rValue);

        ar << pObj;
    }
    return ar;
}
*/

//------------------------------------------------------------------*
template<class TYPE, class ARG_TYPE>
inline CXMLArchive&
    operator>>(CXMLArchive& ar, CList<TYPE, ARG_TYPE> * pList)
{
    assert(pList);
    assert(ar.IsLoading());
    
    NodeIter    iter(ar);
    POSITION    pos;

    for (pos = iter.begin(); pos != NULL; )
    {
        iter.next(pos);
//        TYPE    newObj;
        CXMLObject *pObj = NULL;
        try {
            pObj = ar.ReadObject();
        }
        catch (HRESULT hr) {
            // cannot create!
            throw hr;
        }
        
        assert(pObj != NULL);
        pList->AddTail((ARG_TYPE)pObj);
    }
    return ar;
}

//------------------------------------------------------------------*
inline CXMLArchive&
    operator>>(CXMLArchive& ar, CPtrList *pList)
{
    assert(pList);
    assert(ar.IsLoading());
    
    NodeIter    iter(ar);
    POSITION    pos;

    for (pos = iter.begin(); pos != NULL; )
    {
        iter.next(pos);
        CXMLObject *pObj = NULL;
        try {
            pObj = ar.ReadObject();
        }
        catch (HRESULT hr) {
            // cannot create!
            throw hr;
        }
        
        assert(pObj != NULL);
        pList->AddTail(pObj);
    }
    return ar;
}

//------------------------------------------------------------------*
template<class BASE_CLASS, class TYPE >
inline CXMLArchive&
    operator>>(CXMLArchive& ar, CTypedPtrList<BASE_CLASS, TYPE> *pList)
{
    assert(pList);
    assert(ar.IsLoading());
    
    NodeIter    iter(ar);
    POSITION    pos;

    for (pos = iter.begin(); pos != NULL; )
    {
        iter.next(pos);
        CXMLObject *pObj = NULL;
        try {
            pObj = ar.ReadObject();
        }
        catch (HRESULT hr) {
            // cannot create!
            throw hr;
        }
        
        assert(pObj != NULL);
        pList->AddTail((TYPE)pObj);
    }
    return ar;
}

//------------------------------------------------------------------*
template<class TYPE, class ARG_TYPE>
inline CXMLArchive&
    operator>>(CXMLArchive& ar, CArray<TYPE, ARG_TYPE> * pArray)
{
    assert(pArray);
    assert(ar.IsLoading());
    
    NodeIter    iter(ar);
    POSITION    pos = iter.begin();
    for ( ; pos != NULL; )
    {
        iter.next(pos);
        CXMLObject *pObj = NULL;
        try {
            pObj = ar.ReadObject();
        }
        catch (HRESULT hr) {
            // cannot create!
            throw hr;
        }
        
        assert(pObj != NULL);
        pArray->Add(pObj);
    }
    return ar;
}

//------------------------------------------------------------------*
template<class BASE_CLASS, class TYPE >
inline CXMLArchive&
    operator>>(CXMLArchive& ar, CTypedPtrArray<BASE_CLASS, TYPE> * pTypedPtrArray)
{
    assert(pTypedPtrArray);
    assert(ar.IsLoading());
    
    NodeIter    iter(ar);
    POSITION    pos = iter.begin();
    for ( ; pos != NULL; )
    {
        iter.next(pos);
//        TYPE    newObj;
        CXMLObject *pObj = NULL;
        try {
            pObj = ar.ReadObject();
        }
        catch (HRESULT hr) {
            // cannot create!
            throw hr;
        }
        
        assert(pObj != NULL);
        pTypedPtrArray->Add((TYPE)pObj);
    }
    return ar;
}

/*  // %TBD%    need to define how map is serialized to XML:
    //          are keys also associated with (different) XML node ?????
//------------------------------------------------------------------*
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline CXMLArchive&
    operator>>(CXMLArchive& ar, const CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> * pMap)
{
    assert(pList);
    for (POSITION pos = pMap->GetHeadPosition(); pos != NULL; )
    {
        KEY& rKey, VALUE& rValue
        pMap->GetNextAssoc(pos, key, rValue);
        assert(rValue != NULL);
        ASSERT_KINDOF(CXMLObject, rValue);

        ar << pObj;
    }
    return ar;
}
*/



#endif  // __AFXTEMPL_H__


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CXMLOBJECT_H_


#endif // __XML_WRAPPER_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: XML_Wrapper.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 29.09.03   Time: 14:01
 * Updated in $/4Control/COM/softing/fc/inc
 * bug fix d-2109: SetStringAttributeUTF8()
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 12.12.01   Time: 22:00
 * Updated in $/4Control/COM/softing/fc/inc
 * Added IsElementExisting() to cope with problems encounted with
 * MoveDown().
 * 
 * *****************  Version 2  *****************
 * User: Scd          Date: 31.01.01   Time: 12:24
 * Updated in $/4Control/COM/softing/fc/inc
 * new headers added
 *==
 *----------------------------------------------------------------------------*
*/
