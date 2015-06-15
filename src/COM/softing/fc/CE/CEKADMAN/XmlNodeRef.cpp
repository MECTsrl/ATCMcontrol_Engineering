#include "stdafx.h"

#include "XmlNodeRef.h"

CXmlNodeRef::CXmlNodeRef()
// creates a new object containing a reference to a XML node
{
    m_pXmlNode = NULL;
}

CXmlNodeRef::~CXmlNodeRef()
// deletes an object containing a reference to a XML node
{
    if (m_pXmlNode != NULL) {
        m_pXmlNode->Release();
    }
}

void CXmlNodeRef::SetXmlNode(IXMLDOMNode *pXmlNode)
// sets the reference to a XML node
{
    KADMANAGER_ASSERT((m_pXmlNode == NULL) && (pXmlNode != NULL));
    m_pXmlNode = pXmlNode;
    pXmlNode->AddRef();
}

IXMLDOMNode *CXmlNodeRef::GetXmlNode()
// returns the referenced XML node
{
    KADMANAGER_ASSERT(m_pXmlNode != NULL);
    return m_pXmlNode;
}

