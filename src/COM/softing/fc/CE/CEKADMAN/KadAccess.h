#ifndef KAD_ACCESS_H
#define KAD_ACCESS_H

struct IType;

//------------------------------------------------------------------*
/**
 *  class CKadAccess - provided access to the 4CE KAD (Kernel addon
 *  description. Handles all direct access to the KAD XML file.
 *
 *  @see    KAD description - section ATCM KAD in 4C_Interfaces.doc
*/
class CKadAccess {
	private:
        BOOL m_bInitialized;
		IXMLDOMNode *m_pTreeRoot;
        HRESULT StringToGuid(BSTR sGuidString, GUID *pGuid);
	public:
		CKadAccess();
		~CKadAccess();
        HRESULT Init();
        HRESULT InitAndQueryInterface(IUnknown *pUnknown, IXMLDOMNode *pNode,
            IID interfaceId, void **ppInterface);
		HRESULT GetFirstSubNode(BSTR sNodeName, IXMLDOMNode *pParentNode,
            IXMLDOMNode **ppChildNode);
        HRESULT GetSubNode(BSTR sSubNodeName, IXMLDOMNode **ppSubNode);
        HRESULT GetNextSiblingNode(IXMLDOMNode *pNode, BSTR sNodeName,
            IXMLDOMNode **ppSiblingNode);
        HRESULT GetSubNodeByAttribute(IXMLDOMNode *pNode, BSTR sSubNodeName,
            BSTR sAttributeName, BSTR sAttributeValue, IXMLDOMNode **ppSubSubNode);
        HRESULT GetSubSubNodeByAttribute(BSTR sSubNodeName, BSTR sSubSubNodeName,
            BSTR sAttributeName, BSTR sAttributeValue, 
            IXMLDOMNode **ppSubSubNode);
		HRESULT GetSubNodeAttribute(BSTR sNodeName, BSTR sAttributeName,
            BSTR *psAttributeValue);
        HRESULT GetSubNodeAttribute(BSTR sNodeName, BSTR sAttributeName,
            GUID *pAttributeValue);
		HRESULT GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
			BSTR *pAttributeValue);
		HRESULT GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
			GUID *pAttributeValue);
		HRESULT GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
			BOOL *pbAttributeValue);
        HRESULT GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
			WORD *pbAttributeValue);
        HRESULT GetSubNodeType(BSTR nodeName, IType **ppIType);
        HRESULT GetNodeType(IXMLDOMNode *pNode, IType **ppIType);
        
        HRESULT RemergeKAD();
};

extern CKadAccess kadAccess;

#endif 
