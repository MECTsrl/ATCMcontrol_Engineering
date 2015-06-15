



#include "stdafx.h"
#include "KadManager.h"
#include "IECFeaturesComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CIECFeatures

XML_NODE_REF_IMPL(CIECFeaturesComObj)


STDMETHODIMP CIECFeaturesComObj::GetDataTypes(BSTR *psDataTypes)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"DATA_TYPES",psDataTypes);
}

STDMETHODIMP CIECFeaturesComObj::GetLanguages(BSTR *psLanguages)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"LANGUAGES",psLanguages);
}

STDMETHODIMP CIECFeaturesComObj::GetRetain(BOOL *pbRetain)
{
    return TFGetAttribute<BOOL *>(&m_XmlNodeRef,L"RETAIN",pbRetain);
}

STDMETHODIMP CIECFeaturesComObj::GetAutoDecl(
    GUID* pAutoDeclComponent,
    BSTR* pINIFileWithFullPath
)
{
    HRESULT hr = TFGetAttribute<GUID*>(&m_XmlNodeRef,L"CUSTOM_DECLARATION", pAutoDeclComponent);
    if(hr!=S_OK)
        return hr;
    return TFGetAttribute<BSTR*>(&m_XmlNodeRef,L"CUSTOM_DECL_CONFIG", pINIFileWithFullPath);
}

