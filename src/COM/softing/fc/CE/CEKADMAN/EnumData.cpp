#include "stdafx.h"

#include "EnumData.h"
#include "KadAccess.h"

CEnumData::CEnumData()
// creates an object encapsulating the enumeration data
{
    m_bReset = TRUE;
    m_sSubNodeName = NULL;
    m_pNode = NULL;
    m_pActualSubNode = NULL;
}

CEnumData::~CEnumData()
// destroys an object encapsulating the enumeration data
{
    if (m_sSubNodeName != NULL) {
        SysFreeString(m_sSubNodeName);
    }
    if (m_pNode != NULL) {
        m_pNode->Release();
    }
    if (m_pActualSubNode != NULL) {
        m_pActualSubNode->Release();
    }
}

void CEnumData::Init(GUID elementInterfaceId, IXMLDOMNode *pNode,
    BSTR sSubNodeName)
// initializes the enumeration data
{
    KADMANAGER_ASSERT(m_pNode == NULL);
    KADMANAGER_ASSERT(m_sSubNodeName == NULL);
    m_elementInterfaceId = elementInterfaceId;
    m_pNode = pNode;
    m_pNode->AddRef();
    m_sSubNodeName = SysAllocString(sSubNodeName);
    
}

HRESULT CEnumData::GetNextNode(IXMLDOMNode **ppNextNode)
// gets the next node of the enumeration
{
    HRESULT hrResult;
    IXMLDOMNode *pNextSubNode;

    *ppNextNode = NULL;
    if (m_bReset) {
        m_bReset = FALSE;
        hrResult = kadAccess.GetFirstSubNode(m_sSubNodeName,m_pNode,
            &pNextSubNode);
    } else {
        if (m_pActualSubNode != NULL) {
            hrResult = kadAccess.GetNextSiblingNode(m_pActualSubNode,
                m_sSubNodeName,&pNextSubNode);
        } else {
            hrResult = E_POINTER;
        }
    }
    if (m_pActualSubNode != NULL) {
        m_pActualSubNode->Release();
        m_pActualSubNode = NULL;
    }
    if (hrResult == S_OK) {
        m_pActualSubNode = pNextSubNode;
        *ppNextNode = pNextSubNode;
        pNextSubNode->AddRef();
    }
    return hrResult;
}

GUID CEnumData::GetElementInterfaceId()
// gets the interface ID of enumeration element
{
    return m_elementInterfaceId;
}

void CEnumData::Reset()
// resets the enumeration
{
    m_bReset = TRUE;
}

