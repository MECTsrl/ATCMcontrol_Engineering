

#ifndef COMOBJ_H
#define COMOBJ_H

#include "KadAccess.h"

class CXmlNodeRef;

template <class attributeClass> 
HRESULT TFGetSubNodeAttribute(BSTR nodeName, BSTR attributeName, 
    attributeClass attribute)
// gets the value of an attribute of a sub-node (of the root)
{
	HRESULT hrResult;                                                       

    if (attribute != NULL) {                                                
		hrResult = kadAccess.GetSubNodeAttribute(nodeName,attributeName,    
            attribute);                                                     
	} else {                                                                
		hrResult = E_POINTER;                                               
	}                                                                       
	return hrResult;
}

template <class attributeClass> 
HRESULT TFGetAttribute(CXmlNodeRef *pXmlNodeRef, BSTR attributeName, 
    attributeClass attribute)
// gets the value of an attribute of a node
{
	HRESULT hrResult;                                                       

    if (attribute != NULL) {                                                
		hrResult = kadAccess.GetAttribute(attributeName,                    
            pXmlNodeRef->GetXmlNode(),attribute);                           
	} else {                                                                
		hrResult = E_POINTER;                                               
	}                                                                       
	return hrResult;
}

template <class comReturnObjClass, class interfaceClass>
HRESULT TFCreateAndInitReturnObject(GUID interfaceId, IXMLDOMNode *pNode, 
    interfaceClass **ppInterface, BOOL bReleaseNode)
// creates a new object and sets its reference to the XML node
{
    HRESULT hrResult;
    IUnknown *pUnknown;
    CComObject<comReturnObjClass> *pReturnObject;

    hrResult = CComObject<comReturnObjClass>::CreateInstance(&pReturnObject);
    if (hrResult == S_OK) {
        pReturnObject->QueryInterface(IID_IUnknown,(void **) &pUnknown);
        if (hrResult ==S_OK) {
            hrResult = kadAccess.InitAndQueryInterface(pUnknown,pNode,interfaceId,
                (void **) ppInterface);
        }
    }
    if (bReleaseNode) {
        pNode->Release();
    }
    return hrResult;
}

template <class enumClass, class enumInterface>
HRESULT TFCreateAndInitEnumObject(GUID enumInterfaceId, GUID enumElementInterfaceId,
    BSTR sSubNodeName, IXMLDOMNode *pNode, enumInterface **ppEnum)
// creates and initializes an enumeration object
{
    HRESULT hrResult;
    IInitEnum *pInitEnum;
    CComObject<enumClass> *pEnumObject;

    *ppEnum = NULL;
    hrResult = CComObject<enumClass>::CreateInstance(&pEnumObject);
    if (hrResult == S_OK) {
        hrResult = pEnumObject->QueryInterface(IID_IInitEnum,(void **) &pInitEnum);
        if (hrResult == S_OK) {
            hrResult = pInitEnum->InitEnum(enumElementInterfaceId,pNode,
                sSubNodeName);
            if (hrResult == S_OK) {
                hrResult = pEnumObject->QueryInterface(enumInterfaceId,
                    (void **) ppEnum);
            }
            pInitEnum->Release();
        }
    }
    return hrResult;
}

template <class enumClass, class enumInterface, class enumElementClass, 
    class enumElementInterface> 
HRESULT TFGetSubSubNodeEnum(GUID enumInterfaceId, GUID enumElementInterfaceId,
    BSTR sSubNodeName, BSTR sSubSubNodeName, enumInterface **ppEnum)
// gets an enumeration object that can be used to access the sub nodes
// of a sub node, the nodes are identified by the corresponding node names
{
    HRESULT hrResult;
    IXMLDOMNode *pNode;

    if (ppEnum == NULL) {
		hrResult = E_POINTER;
    } else {
        hrResult = kadAccess.GetSubNode(sSubNodeName,&pNode);
        if (hrResult == S_OK) {
            hrResult = TFCreateAndInitEnumObject<enumClass,enumInterface>(
                enumInterfaceId,enumElementInterfaceId,sSubSubNodeName,pNode,ppEnum);
            pNode->Release();
        }
    }
    return hrResult;
}

template <class comReturnObjClass, class interfaceClass> 
HRESULT TFGetSubSubNode(CXmlNodeRef *pXmlNodeRef, GUID interfaceId,
    BSTR sSubNodeName, BSTR sSubSubNodeName, interfaceClass **ppInterface)
// gets an object encapsulating a reference to the sub-node
// specified by the corresponding node name
{
    HRESULT hrResult;
    IXMLDOMNode *pNode;
    IXMLDOMNode *pSubSubNode;

    *ppInterface = NULL;
    pNode = NULL;
    if (ppInterface == NULL) 
    {
		hrResult = E_POINTER;
    }
    else 
    {
        if(pXmlNodeRef)
        {
            hrResult = kadAccess.GetFirstSubNode(sSubNodeName,pXmlNodeRef->GetXmlNode(),&pNode);
        }
        else
        {
            hrResult = kadAccess.GetFirstSubNode(sSubNodeName, NULL, &pNode);
        }
        if(hrResult == S_OK)
        {
            hrResult = kadAccess.GetFirstSubNode(sSubSubNodeName, pNode, &pSubSubNode);
            pNode->Release();
        }
    }	
    if (hrResult == S_OK) 
    {
        hrResult = TFCreateAndInitReturnObject<comReturnObjClass,interfaceClass>(
            interfaceId,pSubSubNode,ppInterface,TRUE);
    }
	return hrResult;
}

template <class comReturnObjClass, class interfaceClass> 
HRESULT TFGetSubNode(CXmlNodeRef *pXmlNodeRef, GUID interfaceId,
    BSTR sSubNodeName, interfaceClass **ppInterface)
// gets an object encapsulating a reference to the sub-node
// specified by the corresponding node name
{
    HRESULT hrResult;
    IXMLDOMNode *pNode;

    *ppInterface = NULL;
    pNode = NULL;
    if (ppInterface == NULL) {
		hrResult = E_POINTER;
    } else {
        hrResult = kadAccess.GetFirstSubNode(sSubNodeName,pXmlNodeRef->GetXmlNode(),&pNode);
    }	
    if (hrResult == S_OK) {
        hrResult = TFCreateAndInitReturnObject<comReturnObjClass,interfaceClass>(
            interfaceId,pNode,ppInterface,TRUE);
    }
	return hrResult;
}

template <class comReturnObjClass, class interfaceClass> 
HRESULT TFGetSubSubNodeByAttribute(GUID interfaceId, BSTR sSubNodeName, 
    BSTR sSubSubNodeName, BSTR sAttributeName, BSTR sAttributeValue, 
    interfaceClass **ppInterface)
// gets an object encapsulating a reference to the a sub-sub-node
// specified by the corresponding node and attribute names / values
{
    HRESULT hrResult;
    IUnknown *pUnknown;
    IXMLDOMNode *pNode;

    *ppInterface = NULL;
    pNode = NULL;
    pUnknown = NULL;
    if (ppInterface == NULL) {
		hrResult = E_POINTER;
    } else {
        hrResult = kadAccess.GetSubSubNodeByAttribute(sSubNodeName,
            sSubSubNodeName,sAttributeName,sAttributeValue,&pNode);
    }
    if (hrResult == S_OK) {
        hrResult = TFCreateAndInitReturnObject<comReturnObjClass,interfaceClass>(
            interfaceId,pNode,ppInterface,TRUE);
    }
    return hrResult;
}

template <class comReturnObjClass, class interfaceClass> 
HRESULT TFGetSubNodeByAttribute(CXmlNodeRef *pXmlNodeRef, GUID interfaceId,
    BSTR sSubNodeName, BSTR sAttributeName, BSTR sAttributeValue, 
    interfaceClass **ppInterface)
// gets an object encapsulating a reference to the a sub-sub-node
// specified by the corresponding node and attribute names / values
{
    HRESULT hrResult;
    IUnknown *pUnknown;
    IXMLDOMNode *pNode;

    *ppInterface = NULL;
    pNode = NULL;
    pUnknown = NULL;
    if (ppInterface == NULL) {
		hrResult = E_POINTER;
    } else {
        hrResult = kadAccess.GetSubNodeByAttribute(pXmlNodeRef->GetXmlNode(),
            sSubNodeName,sAttributeName,sAttributeValue,&pNode);
    }
    if (hrResult == S_OK) {
        hrResult = TFCreateAndInitReturnObject<comReturnObjClass,interfaceClass>(
            interfaceId,pNode,ppInterface,TRUE);
    }
    return hrResult;
}

#endif
