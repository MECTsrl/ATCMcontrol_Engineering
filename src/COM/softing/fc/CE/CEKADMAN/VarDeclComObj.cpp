




#include "stdafx.h"
#include "KadManager.h"
#include "VarDeclComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CVarDeclComObj

XML_NODE_REF_IMPL(CVarDeclComObj)


STDMETHODIMP CVarDeclComObj::GetHandler(GUID *pHandlerId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"HANDLER",pHandlerId);
}

STDMETHODIMP CVarDeclComObj::GetRetain(BOOL *pbRetainFlag)
{
     return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"RETAIN",
        pbRetainFlag);
}

STDMETHODIMP CVarDeclComObj::GetInput(BOOL *pbInputFlag)
{
     return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"INPUT",
        pbInputFlag);
}

STDMETHODIMP CVarDeclComObj::GetOutput(BOOL *pbOutpuFlag)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"OUTPUT",
        pbOutpuFlag);
}

STDMETHODIMP CVarDeclComObj::GetInOut(BOOL *pbInOutFlag)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"INOUT",
        pbInOutFlag);
}

