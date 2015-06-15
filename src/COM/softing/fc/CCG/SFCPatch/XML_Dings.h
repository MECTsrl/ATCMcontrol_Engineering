
#ifndef __ATLBASE_H__
    #error XML Wrapper library requires ATL! Include <atlbase.h> before including this header!
#endif

#ifndef __CXMLDINGS_H_
#define __CXMLDINGS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <msxml.h>      // this includes header of IE4 delivered msxml control !!
#include "msXML_2.h"      // must include this local MIDL compiled header!!

//----  Forward Class Definitions:   -------------------------------*
class   CXML_Dings;

//----  Defines:    ------------------------------------------------*
// #define _USE_IE5_MSXML

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//----  Typedefs:    -----------------------------------------------*
typedef CList<long, long&>  stackSiblings;

//------------------------------------------------------------------*
/**
 *  class CXML_Dings
 *
 *  @see    ....
*/
class CXML_Dings 
{
    friend class NodeIter;

public:
	CXML_Dings(CFile* pFile, CFile* pWriteFile=NULL);	// constructor, load also msXML.dll via CoCreateInstance()
											// parameters as in <b>CArchive</b>
    virtual ~CXML_Dings();

private:
    CXML_Dings(CXML_Dings const & other);	// copy ctor
    CXML_Dings const &  operator = (CXML_Dings const & other);	// assignment

public:
    enum NumericFormat {
        decimal,
        hex,
        bin,
        oct
    };

//
// Access
//
public:
	// return string of last XML parser error
	HRESULT GetLastParseError(CString &strLastError)
			{
				strLastError = m_strError;
				return (m_strError.IsEmpty()) ? S_FALSE : S_OK;
			}
	
	// return ptr to current XML node element node in msXML.dll tree
    // caller must Release() when no more in use !!!
	HRESULT GetCurrentElement(IXMLElement2 **ppElem)
			{
				if (ppElem == NULL) 
				{
					return E_POINTER;
				}
				*ppElem = m_pCurrentElem;
				if (*ppElem)
				{
					(*ppElem)->AddRef();
				}
				return S_OK;
			}

	// return ptr to current XML document in msXML.dll tree
	// caller must Release() when no more in use !!!
	HRESULT GetXMLDocument(IXMLDocument2 **ppDoc)
			{
				if (ppDoc == NULL)
				{
					return E_POINTER;
				}
				*ppDoc = m_pXMLDocument;
				if (*ppDoc)
				{
					(*ppDoc)->AddRef();
				}
				return S_OK;
			}

    // when loading/writing, return current active node name
	HRESULT GetCurrentNodeName(CString &strNodeName)
			{
				strNodeName = m_strCurrentNodeName;
				return S_OK;
	}

// =Operations
public:
	//
    // all operations throw HRESULT exceptions !!!!!!!
    // to/from XML file
	//

	void Load();	// parse XML file and prepare for walking thru tree
	void Flush();	// write tree in memory to XML file

	//
    // work on tree in memory, good for read and write
	//
	void Clear();	// cleanup XML element tree in memory
	BOOL MoveDown(LPCTSTR pszElemName); // walk down one elem hierarchy to element
										// will create new node, throws when not found in Class Map !!
										// works for read and write !!
										// returns TRUE if node exists (loading) or node was created (writing)
	void MoveUp();	// walk up one elem node level
	void MoveToRoot();	// go to root elem
	BOOL MoveToSibling(LPCTSTR pszAttrName, LPCTSTR pszAttrValue);	// walk to siblings in same level,
																	// matching the given attribute and value
																	// valid in read mode only!
	BOOL MoveNext();	// walk to siblings in same level,
						// assume identical name from previous MoveDown()
						// adds sibling elem node when in writing node
						// writing to XML:
	void SetDocType(LPCTSTR pszDocType);	// call once to set doc type
											// <b>must</b> be called once before <b>Flush</b>()ing
	void SetDTD(LPCTSTR pszDTD); // call once to set DTD URL
	void SetComment(LPCTSTR pszText); // set comment, create as node
	void SetAttribute(LPCTSTR pszAttrName, LPCTSTR pszAttrValue); // set attribute value of current elem node
	void SetAttribute(LPCTSTR pszAttrName, long iAttrValue, enum NumericFormat fmt = decimal); // set attribute value of current elem node
	void SetAttribute(LPCTSTR pszAttrName, ULONG iAttrValue, enum NumericFormat fmt = decimal); // set attribute value of current elem node
	void SetAttribute(LPCTSTR pszAttrName, CPoint ptAttrValue, enum NumericFormat fmt = decimal); // set attribute value of current elem node
	void SetCData(LPCTSTR pszText); // set text element with CDATA handling
									// honor whitespaces by wrapping in &lt;![CDATA[
	void SetPCData(LPCTSTR pszText); // set text element with PCDATA handling
									// strip whitespaces (when parsed, not when saving)

	//
	// reading from XML
	//
	void GetDocType(CString &strType) // return parsed doc type, can be empty!
		{
			strType = m_strDocType;
		}
	void GetDTD(CString &strDTD) // return parsed DTD URL, can be empty!
	{
		strDTD = m_strDTD;
	}

	BOOL GetAttribute(LPCTSTR pszAttrName, CString &strAttrValue); // get attribute value of current elem node
	BOOL GetAttribute(LPCTSTR pszAttrName, long &iAttrValue, enum NumericFormat fmt = decimal);	// get attribute value of current elem node, format is ignored, compatibility only!
	BOOL GetAttribute(LPCTSTR pszAttrName, ULONG &iAttrValue, enum NumericFormat fmt = decimal); // get attribute value of current elem node
	BOOL GetAttribute(LPCTSTR pszAttrName, CPoint &ptAttrValue, enum NumericFormat fmt = decimal);	// get attribute value of current elem node;
																									// value is a ';' separated point tupel
	void GetCData(CString &strText); // get text elemtent
   
	static HRESULT          EncodeString(LPCTSTR pszText, CString &strEncoded);
							 // encode/decode special characters like umlauts, <, >, ... etc
    static HRESULT          DecodeString(BSTR sText, CString &strDecoded);

// =Overrides
public:

protected:
	HRESULT SetCurrentElement(IXMLElement2 *pElem); // set new current XML node in msXML.dll tree

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

    LPCTSTR                 Encode(TCHAR ch);
    TCHAR                   Decode(LPCTSTR pszNamed);
    CString                 EscapeQuote(BSTR sVal);
    void                    WriteIndent();
    void                    WriteIndented(LPCTSTR pszText);
	CFile*					GetFile();
	CFile*					GetWriteFile();
	void					WriteEncoded(CFile* pFile, const CString& strText);	

// =Attributes
protected:
	CFile*					m_pFile;
	CFile*					m_pWriteFile;
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

    CString                 m_strCurrentNodeName;
                                // when loading/writing, holds current active node name

};

//------------------------------------------------------------------*
class NodeIter
{
public:
                            NodeIter(CXML_Dings &ar);
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
    CXML_Dings            *m_pArchive;
    CString                 m_strSaveNodeName;
    CComPtr<IXMLElement2>   m_pCurrSaved;
    CComPtr<IXMLElementCollection>   m_pColl;
    POSITION                m_pos;
    long                    m_lCnt;

};

#endif __CXMLDINGS_H_

