

// MenuItemComObj.cpp : Implementation of CMenuItemComObj
#include "stdafx.h"
#include "KadManager.h"
#include "MenuItemComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuItemComObj

XML_NODE_REF_IMPL(CMenuItemComObj)

STDMETHODIMP CMenuItemComObj::GetId(BSTR *psId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ID",psId);
}

STDMETHODIMP CMenuItemComObj::GetText(BSTR *psText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TEXT",psText);
}

STDMETHODIMP CMenuItemComObj::GetHandler(GUID *pHandlerId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"HANDLER",pHandlerId);
}

STDMETHODIMP CMenuItemComObj::GetExeHandler(BSTR *psHandler)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"EXEHANDLER",psHandler);
}

STDMETHODIMP CMenuItemComObj::GetDispatchId(BSTR *psDispatchId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"DISPID",psDispatchId);
}

//------------------------------------------------------------------*
/**
 * Returns the STANDARDOP attribute associated with a menu item entry
 * of the KAD (Kernel Addon Description). The value of the attribute 
 * STANDARDOP identifies an standard operation as string - associated
 * with the menu item entry.
 *
 * @param           BSTR * - pointer to BSTR receiving the string value of
 *                  the attribute STANDARDOP. Pointer shall not be NULL.
 * @return          HRESULT - of error, S_OK if successful.
 *
 * @see     KAD description - section ATCM KAD in 4C_Interfaces.doc
*/
STDMETHODIMP CMenuItemComObj::GetStandardOperation(BSTR *psStandardOperation)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"STANDARDOP",psStandardOperation);

}

STDMETHODIMP CMenuItemComObj::GetStatusBarText(BSTR *psStatusBarText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"STBARTXT",psStatusBarText);
}

STDMETHODIMP CMenuItemComObj::GetOrderKey(BSTR *psOrderKey)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ORDERKEY",psOrderKey);
}

//------------------------------------------------------------------*
/**
 * Returns the KEYMOUSEEVENT attribute associated with a menu item entry
 * of the KAD (Kernel Addon Description). The value of the attribute 
 * KEYMOUSEEVENT identifies an operation as string
 *
 * @param           BSTR * - pointer to BSTR receiving the string value of
 *                  the attribute KEYMOUSEEVENT. Pointer shall not be NULL.
 * @return          HRESULT - of error, S_OK if successful.
 *
 * @see     KAD description - section ATCM KAD in 4C_Interfaces.doc
*/
STDMETHODIMP CMenuItemComObj::GetKeyMouseEvent(BSTR * psKeyMouseEvent)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"KEYMOUSEEVENT",psKeyMouseEvent);
}

STDMETHODIMP CMenuItemComObj::GetEnableExpression(BSTR *psEnableExpression)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ENABLE_EXPRESSION",
        psEnableExpression);
}

STDMETHODIMP CMenuItemComObj::GetShowExpression(BSTR *psShowExpression)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"SHOW_EXPRESSION",
        psShowExpression);
}

STDMETHODIMP CMenuItemComObj::GetAccelerator(BSTR *psAccelerator)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ACCELERATOR",
        psAccelerator);
}

