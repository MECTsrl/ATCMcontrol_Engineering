#if !defined(AFX_XMLNODE_H__AEFE2263_3CFA_4A8A_AAF8_3D16E9974E1F__INCLUDED_)
#define AFX_XMLNODE_H__AEFE2263_3CFA_4A8A_AAF8_3D16E9974E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <atlbase.h>

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;



class CXMLNodeList;
class CXMLDocument;


//------------------------------------------------------------------*
/**
 *  class CXMLNode
 *
 *  @see    ....
*/
class CXMLNode  
{
public:
	CXMLNode();
	virtual ~CXMLNode();

    BOOL GetTagName(CString& strTagName) const;

    BOOL GetParentNode(CXMLNode& parentNode) const;
    
    // work with the children in the tree
    BOOL GetChildNodeList(CXMLNodeList& nodeList) const;
    BOOL AppendChild(const CXMLNode& childNode, 
                     BOOL bSorted = FALSE, 
                     BOOL bIgnoreCase = FALSE);
    BOOL InsertChildBefore(const CXMLNode& childNode, const CXMLNode& beforeChild);
    BOOL ReplaceChild(const CXMLNode& newChild, const CXMLNode& oldChild);
    BOOL RemoveChild(const CXMLNode& child);
    
    // work with attributes
    BOOL GetAttribute(const CString& strName, CString& strValue, BOOL bRecursive = FALSE) const;
    BOOL SetAttribute(const CString& strName, const CString& strValue);
    BOOL RemoveAttribute(const CString& strName);

    BOOL GetChildWithAttr(const CString& strAttrName, const CString& strAttrValue, CXMLNode& child) const;

    // get first child with tag name
    BOOL GetFirstChildWithTagName(const CString& crstrTagName, CXMLNode& child) const;

    BOOL RemoveAllChilds();

    // SIS, 01.02.01: get id path
    BOOL GetIdPath(CString& strPath);
    // SIS, 01.02.01: get node from id path
    BOOL GetNodeFromIdPath(const CString& strPath, CXMLNode& node);
    // SIS, 01.02.01: get document (used by GetNodeFromIdPath)
    BOOL GetDocument(CXMLDocument& document);
    // SIS: 01.02.01: get node type
    BOOL GetNodeType(DOMNodeType& tNodeType); 

    // SIS: 08.02.01: get parent node with attribute
    BOOL GetParentWithAttr(const CString& strName, const CString& strAttrValue, CXMLNode& rtNode, BOOL bIncludeThis = FALSE);

    // only internal for CXMLDocument
    BOOL SetIXMLNode(IXMLDOMNode* pNode);
    BOOL SetIXMLNode(IUnknown* pNode);
    BOOL GetIXMLNode(IXMLDOMNode** ppNode);
    BOOL GetIXMLNodeUnknown(IUnknown** ppNode);

    BOOL GetXMLString(CString &strXMLString) const;

    BOOL GetText(CString& strText);
    BOOL SetText(const CString& strText);
private:
    CComQIPtr<IXMLDOMNode, &IID_IXMLDOMNode> m_node;
};

#endif // !defined(AFX_XMLNODE_H__AEFE2263_3CFA_4A8A_AAF8_3D16E9974E1F__INCLUDED_)