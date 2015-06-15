
#if !defined(AFX_XMLNODEITER_H__D053AE41_B0D2_4A8F_99C2_5A0436C062EE__INCLUDED_)
#define AFX_XMLNODEITER_H__D053AE41_B0D2_4A8F_99C2_5A0436C062EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XMLNode.h"
#include "SBLStack.h"

class CXMLNodeIter  
{
public:
    CXMLNodeIter(CXMLNode& rtNode);
    virtual ~CXMLNodeIter();
    BOOL GetNext(CXMLNode& rtNode);
    void Reset();

private:
    CSBLStack<CXMLNode>  m_tNodeStack;
};

#endif // !defined(AFX_XMLNODEITER_H__D053AE41_B0D2_4A8F_99C2_5A0436C062EE__INCLUDED_)