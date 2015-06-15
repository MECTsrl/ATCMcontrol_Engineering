// ToolbarItemComObj.cpp : Implementation of CToolbarItemComObj
#include "stdafx.h"
#include "KadManager.h"
#include "ToolbarItemComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CToolbarItemComObj

XML_NODE_REF_IMPL(CToolbarItemComObj)

STDMETHODIMP CToolbarItemComObj::GetMainMenuId(BSTR *psDispatchId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"MAINMENUID",psDispatchId);
}

STDMETHODIMP CToolbarItemComObj::GetIcon(BSTR *psIcon)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ICON",psIcon);
}

STDMETHODIMP CToolbarItemComObj::GetToolTip(BSTR *psToolTip)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TOOLTIP",psToolTip);
}

