

// TargetComObj.cpp : Implementation of CTargetComObj
#include "stdafx.h"
#include "KadManager.h"
#include "TargetComObj.h"
#include "TargetOnlineComObj.h"
#include "IECFeaturesComObj.h"
#include "ComObj.h"

/////////////////////////////////////////////////////////////////////////////
// CTargetComObj

XML_NODE_REF_IMPL(CTargetComObj)

STDMETHODIMP CTargetComObj::GetTypeString(BSTR *psType)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"TYPE",psType);
}

STDMETHODIMP CTargetComObj::GetExt(BSTR* psExt)
{
    return TFGetAttribute<BSTR *>(&m_XmlNodeRef,L"EXT",psExt);
}


STDMETHODIMP CTargetComObj::GetType(IType **ppIType)
{
    return kadAccess.GetNodeType(m_XmlNodeRef.GetXmlNode(),ppIType);
}

STDMETHODIMP CTargetComObj::GetFacilityCode(WORD *pFacilityCode)
{
    return TFGetAttribute<WORD *>(&m_XmlNodeRef,L"FACILITYCODE",pFacilityCode);
}

STDMETHODIMP CTargetComObj::GetTargetOnline(ITargetOnline **ppITargetOnline)
{
    return TFGetSubNode<CTargetOnlineComObj,ITargetOnline>(
        &m_XmlNodeRef,IID_ITargetOnline,L"TARGETONLINE",ppITargetOnline);
}

STDMETHODIMP CTargetComObj::GetIECFeatures(IIECFeatures **ppIIECFeatures)
{
    return TFGetSubNode<CIECFeaturesComObj,IIECFeatures>(
        &m_XmlNodeRef,IID_IIECFeatures,L"IEC_FEATURES",ppIIECFeatures);
}

