#ifndef ENUM_DATA_H
#define ENUM_DATA_H

class CEnumData {
// this class contains all the elements needed to implement
// one of the enumeration COM object
    private:
        BOOL m_bReset;
        GUID m_elementInterfaceId;
        IXMLDOMNode *m_pNode; 
        IXMLDOMNode *m_pActualSubNode; 
		BSTR m_sSubNodeName;
    public:
        CEnumData();
        ~CEnumData();
        void Init(GUID elementInterfaceId, IXMLDOMNode *pNode,
            BSTR sSubNodeName);
        void Reset();
        GUID GetElementInterfaceId();
        HRESULT GetNextNode(IXMLDOMNode **ppNextNode);
};

#endif