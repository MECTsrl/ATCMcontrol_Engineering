
// MenuComObj.cpp : Implementation of CMenuComObj
#include "stdafx.h"
#include "EnumMenuItemsComObj.h"
#include "MenuComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuComObj

XML_NODE_REF_IMPL(CMenuComObj)

STDMETHODIMP CMenuComObj::GetId(BSTR *psId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ID",psId);
}

STDMETHODIMP CMenuComObj::GetTitle(BSTR *psTitle)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TITLE",psTitle);
}

STDMETHODIMP CMenuComObj::GetHandler(GUID *pHandlerId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"HANDLER",pHandlerId);
}

STDMETHODIMP CMenuComObj::GetMenuItemEnumerator(IEnumMenuItems **ppIEnumMenuItems)
{
    return TFCreateAndInitEnumObject<CEnumMenuItemsComObj,IEnumMenuItems>(
        IID_IEnumMenuItems,IID_IMenuItem,L"MENUITEM",
        m_XmlNodeRef.GetXmlNode(),ppIEnumMenuItems);
}

STDMETHODIMP CMenuComObj::GetItemByText(BSTR sText, IMenuItem **ppIMenuItem)
{
    return TFGetSubNodeByAttribute<CMenuItemComObj,IMenuItem>(
        &m_XmlNodeRef,IID_IMenuItem,L"MENUITEM",L"TEXT",sText,ppIMenuItem);
}
