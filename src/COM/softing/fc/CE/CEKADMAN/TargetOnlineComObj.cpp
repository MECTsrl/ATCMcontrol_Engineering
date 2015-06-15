

// TargetOnlineComObj.cpp : Implementation of CTargetOnlineComObj
#include "stdafx.h"
#include "KadManager.h"
#include "TargetOnlineComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetOnlineComObj

XML_NODE_REF_IMPL(CTargetOnlineComObj)

STDMETHODIMP CTargetOnlineComObj::GetCustomConnect(GUID *pCustomConnectId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"CUSTOM_CONNECT",pCustomConnectId);
}

STDMETHODIMP CTargetOnlineComObj::GetDownload(GUID *pDownloadId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"DOWNLOAD",pDownloadId);
}

//STDMETHODIMP CTargetOnlineComObj::GetDownloadChanges(GUID *pDownloadChangesId)
//{
//    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"DLCHANGES",pDownloadChangesId);
//}
//
//STDMETHODIMP CTargetOnlineComObj::GetDownloadSelection(GUID *pDownloadSelectionId)
//{
//    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"DLSELECTION",pDownloadSelectionId);
//}

STDMETHODIMP CTargetOnlineComObj::GetConsoleFlag(BOOL* pbConsoleFlag)
{
    return TFGetAttribute<BOOL*>(&m_XmlNodeRef,L"CONSOLE",pbConsoleFlag);
}

STDMETHODIMP CTargetOnlineComObj::GetDebugFlag(BOOL* pbDebugFlag)
{
    return TFGetAttribute<BOOL*>(&m_XmlNodeRef,L"DEBUG",pbDebugFlag);
}

STDMETHODIMP CTargetOnlineComObj::GetMultitaskFlag(BOOL* pbMultitaskFlag)
{
    return TFGetAttribute<BOOL*>(&m_XmlNodeRef,L"MULTITASK",pbMultitaskFlag);
}

STDMETHODIMP CTargetOnlineComObj::GetWarmstartFlag(BOOL* pbWarmstartFlag)
{
    return TFGetAttribute<BOOL*>(&m_XmlNodeRef,L"WARMSTART",pbWarmstartFlag);
}

STDMETHODIMP CTargetOnlineComObj::GetInitClearFlag(BOOL* pbInitClearFlag)
{
    return TFGetAttribute<BOOL*>(&m_XmlNodeRef,L"INITCLEAR",pbInitClearFlag);
}

STDMETHODIMP CTargetOnlineComObj::GetDLChangesFlag(BOOL* pbDLChangesFlag)
{
    return TFGetAttribute<BOOL*>(&m_XmlNodeRef,L"DLCHANGES",pbDLChangesFlag);
}

STDMETHODIMP CTargetOnlineComObj::GetPreDLStep(GUID* pPreDLStepId)
{
    return TFGetAttribute<GUID*>(&m_XmlNodeRef,L"PREDLSTEP",pPreDLStepId);
}

STDMETHODIMP CTargetOnlineComObj::GetPostDLStep(GUID* pPostDLStepId)
{
    return TFGetAttribute<GUID*>(&m_XmlNodeRef,L"POSTDLSTEP",pPostDLStepId);
}

STDMETHODIMP CTargetOnlineComObj::GetPreConnectStep(GUID* pPreConnectStepId)
{
    return TFGetAttribute<GUID*>(&m_XmlNodeRef,L"PRECONNECTSTEP",pPreConnectStepId);
}

