#if !defined(AFX_XMLNODELIST_H__1999D7A9_309A_4D82_8FB2_308551DA564B__INCLUDED_)
#define AFX_XMLNODELIST_H__1999D7A9_309A_4D82_8FB2_308551DA564B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <atlbase.h>

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;



class CXMLNode;


//------------------------------------------------------------------*
/**
 *  class CXMLNodeList
 *
 *  @see    ....
*/
class CXMLNodeList  
{
public:
	CXMLNodeList();
	virtual ~CXMLNodeList();

    long GetLength();
    BOOL Item(long pos, CXMLNode& node) const;
    BOOL Reset();
    BOOL Next(CXMLNode& node);

    friend CXMLNode;
private:
    // only internal for CXMLNode
    BOOL SetIXMLNodeList(IXMLDOMNodeList* pNodeList);

private:
    CComPtr<IXMLDOMNodeList> m_nodeList;

};

#endif // !defined(AFX_XMLNODELIST_H__1999D7A9_309A_4D82_8FB2_308551DA564B__INCLUDED_)