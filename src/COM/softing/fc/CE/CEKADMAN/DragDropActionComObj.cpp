


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// DragDropActionComObj.cpp : Implementation of CDragDropActionComObj
#include "stdafx.h"
#include "KadManager.h"
#include "DragDropActionComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDragDropActionComObj

XML_NODE_REF_IMPL(CDragDropActionComObj)

STDMETHODIMP CDragDropActionComObj::GetDragType(BSTR* psDragType)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"DRAGTYPE",psDragType);
}

STDMETHODIMP CDragDropActionComObj::GetDropType(BSTR *psDropType)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"DROPTYPE",psDropType);
}

STDMETHODIMP CDragDropActionComObj::GetHandler(GUID *pHandlerId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"HANDLER",pHandlerId);
}

STDMETHODIMP CDragDropActionComObj::GetEnableExpression(BSTR *psEnableExpression)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ENABLE_EXPRESSION",psEnableExpression);
}


