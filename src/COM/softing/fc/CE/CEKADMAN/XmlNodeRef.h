#ifndef XMLNODEREF_H
#define XMLNODEREF_H

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;


// declares the necessary members / functions to implement
// an object containing a reference to a XML node           
#define XML_NODE_REF_DECL                                  \
    private:                                               \
        CXmlNodeRef m_XmlNodeRef;                          \
    public:                                                \
        STDMETHOD(SetXmlNode)(IUnknown *pXmlNode);

// defines the necessary functions to implement
// an object containing a reference to a XML node           
#define XML_NODE_REF_IMPL(class)                           \
    STDMETHODIMP class::SetXmlNode(IUnknown *pXmlNode)     \
    {                                                      \
        m_XmlNodeRef.SetXmlNode((IXMLDOMNode *) pXmlNode); \
        return S_OK;                                       \
    }

class CXmlNodeRef {
// this object encapsulates a reference to a XML node
// it performs the necessary AddRef / Release operations inside
    private:
        IXMLDOMNode *m_pXmlNode;
    public:
        CXmlNodeRef();
        ~CXmlNodeRef();
        void SetXmlNode(IXMLDOMNode *pXmlNode);
        IXMLDOMNode *GetXmlNode();
};

#endif

