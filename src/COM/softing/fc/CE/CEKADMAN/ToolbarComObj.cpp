

// ToolbarComObj.cpp : Implementation of CToolbarComObj
#include "stdafx.h"
#include "EnumToolbarItemsComObj.h"
#include "ToolbarComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CToolbarComObj

XML_NODE_REF_IMPL(CToolbarComObj)

STDMETHODIMP CToolbarComObj::GetId(BSTR *psId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ID",psId);
}

STDMETHODIMP CToolbarComObj::GetHandler(GUID *pHandlerId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"HANDLER",pHandlerId);
}

STDMETHODIMP CToolbarComObj::GetToolbarItemEnumerator(IEnumToolbarItems **ppEnumToolbarItems)
{
    return TFCreateAndInitEnumObject<CEnumToolbarItemsComObj,IEnumToolbarItems>(
        IID_IEnumToolbarItems,IID_IToolbarItem,L"TOOLBARITEM",
        m_XmlNodeRef.GetXmlNode(),ppEnumToolbarItems);
}

