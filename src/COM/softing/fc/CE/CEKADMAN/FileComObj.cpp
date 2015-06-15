

// FileComObj.cpp : Implementation of CFileComObj
#include "stdafx.h"
#include "KadManager.h"
#include "FileComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CFileComObj

XML_NODE_REF_IMPL(CFileComObj)

STDMETHODIMP CFileComObj::GetExtension(BSTR *psExtension)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"EXT",psExtension);
}

STDMETHODIMP CFileComObj::GetIcon(BSTR *psIcon)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ICON",psIcon);
}

STDMETHODIMP CFileComObj::GetParser(GUID *pParserId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"PARSER",pParserId);
}

STDMETHODIMP CFileComObj::GetStandardEditor(GUID *pEditorId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"STDEDITOR",pEditorId);
}

STDMETHODIMP CFileComObj::GetProgramEditor(GUID *pEditorId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"PRGEDITOR",pEditorId);
}

STDMETHODIMP CFileComObj::GetExeEditor(BSTR *psEditor)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"EXEEDITOR",psEditor);
}

STDMETHODIMP CFileComObj::GetContainer(GUID *pContainerId)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"CONTAINER",pContainerId);
}

STDMETHODIMP CFileComObj::GetFilterText(BSTR *psFilterText)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"FILTERTEXT",psFilterText);
}

STDMETHODIMP CFileComObj::GetHandler(GUID *pHandler)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"HANDLER",pHandler);
}

STDMETHODIMP CFileComObj::GetDispatchId(BSTR *psDispatchId)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"DISPID",psDispatchId);
}

STDMETHODIMP CFileComObj::GetConfigurator(GUID *pConfigurator)
{
    return TFGetAttribute<GUID *>(&m_XmlNodeRef,L"CONFIGURATOR",
        pConfigurator);
}

STDMETHODIMP CFileComObj::GetVisibilityFlag(BOOL* pbVisible)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"VISIBILITY", pbVisible);
}

STDMETHODIMP CFileComObj::GetFindInFilesFlag(BOOL* pbFindInFiles)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"FINDINFILES", pbFindInFiles);
}

STDMETHODIMP CFileComObj::GetOrderKey(BSTR *psOrderKey)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"ORDERKEY",psOrderKey);
}

STDMETHODIMP CFileComObj::GetPrintFlag(BOOL* pbPrint)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"PRINT", pbPrint);
}

