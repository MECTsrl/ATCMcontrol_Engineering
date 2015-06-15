// StateComObj.cpp : Implementation of CStateComObj
#include "stdafx.h"
#include "KadManager.h"
#include "StateComObj.h"
#include "EnumComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CStateComObj

XML_NODE_REF_IMPL(CStateComObj)

STDMETHODIMP CStateComObj::GetText(BSTR *psText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TEXT",psText);
}

STDMETHODIMP CStateComObj::GetIcon(BSTR *psIcon)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ICON",psIcon);
}

STDMETHODIMP CStateComObj::GetOverlay(BSTR *psOverlay)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"OVERLAY",psOverlay);
}

STDMETHODIMP CStateComObj::GetStateValue(BSTR *psStateValue)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"STATE_VAL",psStateValue);
}

