

// ClassComObj.cpp : Implementation of CClassComObj
#include "stdafx.h"
#include "KadManager.h"
#include "ClassComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CClassComObj

XML_NODE_REF_IMPL(CClassComObj)

STDMETHODIMP CClassComObj::GetSection(BSTR *psSection)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"SECTION",psSection);
}

STDMETHODIMP CClassComObj::GetGroupText(BSTR *psGroupText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TEXT",psGroupText);
}

STDMETHODIMP CClassComObj::GetClassType(BSTR *psClassType)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"CLASS_TYPE",psClassType);
}

STDMETHODIMP CClassComObj::GetTypeString(BSTR *psType)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TYPE",psType);
}

STDMETHODIMP CClassComObj::GetType(IType **ppIType)
{
    return kadAccess.GetNodeType(m_XmlNodeRef.GetXmlNode(),ppIType);
}

STDMETHODIMP CClassComObj::GetPouSel(BOOL *pbPouSelFlag)
{    
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"POU_SEL",
        pbPouSelFlag);

}


