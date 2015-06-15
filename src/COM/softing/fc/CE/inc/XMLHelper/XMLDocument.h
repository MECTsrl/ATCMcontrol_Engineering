#if !defined(AFX_XMLDOCUMENT_H__0C5C85FD_90D5_48DE_A878_0EB063071045__INCLUDED_)
#define AFX_XMLDOCUMENT_H__0C5C85FD_90D5_48DE_A878_0EB063071045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;


class CXMLNode;


//------------------------------------------------------------------*
/**
 *  This class is a wrapper for IXMLDOMDocument
 *
 *  @see    ....
*/
class CXMLDocument  
{
public:
	CXMLDocument();
	virtual ~CXMLDocument();

    BOOL Load(const CString& strFileName);
    BOOL Save(const CString& strFileName) const;
    BOOL LoadXMLString(const CString& strXMLString);
    BOOL GetXMLString(CString& strXMLString) const;
    BOOL CreateNode(CXMLNode& node, const CString& strTagName);
    BOOL GetDocNode(CXMLNode& node) const;
    BOOL GetRootNode(CXMLNode& node) const;

    // SIS, 01.02.01: set IXMLDOMDocument pointer
    BOOL SetIXMLDocument(IXMLDOMDocument* p_domDocument);
    BOOL SetIXMLDocument(IUnknown* p_domDocument);
    BOOL GetIXMLDocument(IXMLDOMDocument** pp_domDocument);
    BOOL GetIXMLDocumentUnknown(IUnknown** pp_domDocument);

private:
    CComQIPtr<IXMLDOMDocument, &IID_IXMLDOMDocument> m_domDocument;

};

#endif // !defined(AFX_XMLDOCUMENT_H__0C5C85FD_90D5_48DE_A878_0EB063071045__INCLUDED_)
