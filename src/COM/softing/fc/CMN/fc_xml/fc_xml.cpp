/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml/fc_xml.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_xml.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml/fc_xml.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_xml
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  ??.??.2001  SCD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/




#include "fc_xml.h"
#include "fc_xml_error.h"

#define nobreak

#include <atlbase.h>



HRESULT XML_LoadDocument(BSTR sSourceFile, IXMLDOMDocument **ppDocument)
{
	HRESULT hrResult;
	VARIANT_BOOL vbIsSuccessful;
    CComVariant vXMLSource = sSourceFile;

    try {
	    hrResult = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		                            IID_IXMLDOMDocument, (void**)ppDocument);
	    if (hrResult != S_OK) 
        {
            *ppDocument = NULL;
        } 
        else 
        {
		    hrResult = (*ppDocument)->load(vXMLSource, &vbIsSuccessful);
        }
	    if ((hrResult == S_OK) && (!vbIsSuccessful)) 
        {
		    hrResult = E_LOAD_XML_DOCUMENT_FAILED;
	    }
    }
    catch (...) 
    {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }
	if (hrResult != S_OK) 
    {
        if (*ppDocument != NULL) 
        {
		    (*ppDocument)->Release();
            *ppDocument = NULL;
        }
	}
	return hrResult;
}




HRESULT XML_HasNodeName(IXMLDOMNode *pNode, BSTR sNodeName,
                        BOOL *bHasNodeName)
// checks whether the node has the given name (and has the type 'element')
{
	HRESULT hrResult;
    DOMNodeType nodeType;
	BSTR sName;

    try {
        sName = NULL;
        *bHasNodeName = FALSE;

	    hrResult = pNode->get_nodeType(&nodeType);
	    if ((hrResult == S_OK) && (nodeType == NODE_ELEMENT)) 
        {
    	    hrResult = pNode->get_nodeName(&sName);
		    if (hrResult == S_OK) 
            {
			    if (wcscmp(sNodeName,sName) == 0) 
                {
                    *bHasNodeName = TRUE;
			    }
		    }
	    }
    }
    catch (...) 
    {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }

    if (sName != NULL) 
    {
        SysFreeString(sName);
        sName = NULL;
    }
    
	return hrResult;
}




HRESULT XML_GetChildNodes(IXMLDOMNode *pParentNode, 
	                      IXMLDOMNodeList **ppChildNodes, long *plNumberChildNodes)
// returns the child nodes of a given node as well as the number
// of these nodes (if any exist)
{
	HRESULT hrResult;
	VARIANT_BOOL vbDummy;

    try 
    {
        *ppChildNodes = NULL;
	    if (pParentNode == NULL) 
        {
		    hrResult = E_MISSING_PARENT_XML_NODE;
        } 
        else 
        {
            hrResult = S_OK;
        }
        if (hrResult == S_OK) 
        {
		    hrResult = pParentNode->hasChildNodes(&vbDummy);
            if (hrResult == S_FALSE) 
            {
			    hrResult = S_XML_NODE_NOT_FOUND;
            }
        }
		if (hrResult == S_OK) 
        {
	        hrResult = pParentNode->get_childNodes(ppChildNodes);
            if (hrResult != S_OK) 
            {
                *ppChildNodes = NULL;
            }
        }
		if (hrResult == S_OK) 
        {
			hrResult = (*ppChildNodes)->get_length(plNumberChildNodes);
	    }
    }
    catch (...) 
    {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }

	if (hrResult != S_OK) 
    {
        if (*ppChildNodes != NULL) 
        {
		    (*ppChildNodes)->Release();
            *ppChildNodes = NULL;
        }
	}
	return hrResult;
}




HRESULT XML_GetFirstSubNode(BSTR sNodeName, IXMLDOMNode *pParentNode,
                            IXMLDOMNode **ppChildNode)
// returns the first sub-node with the given name
{
	HRESULT hrResult;
    BOOL bHasNodeName;
	long lNumberChildren,lChildNumber;
    IXMLDOMNode *pChildNode;
    IXMLDOMNodeList *pChildNodes;

    try 
    {
        pChildNode = NULL;
   	    *ppChildNode = NULL;
	    hrResult = XML_GetChildNodes(pParentNode, &pChildNodes, &lNumberChildren);
	    if (hrResult == S_OK) 
        {
		    lChildNumber = 0;
		    while ((lChildNumber < lNumberChildren) && 
				    (*ppChildNode == NULL) && 
				    (hrResult == S_OK)) 
            {
                hrResult = pChildNodes->get_item(lChildNumber, &pChildNode);
			    if (hrResult != S_OK) 
                {
                    pChildNode = NULL;
                } 
                else 
                {
                    hrResult = XML_HasNodeName(pChildNode, sNodeName, &bHasNodeName);
                    if ((hrResult == S_OK) && bHasNodeName) 
                    {
                        *ppChildNode = pChildNode;
                    }
			        else
                    {
				        pChildNode->Release();
                        pChildNode = NULL;
			        }
                }
	    	    lChildNumber++;
		    }
		    if (*ppChildNode == NULL) 
            {
			    hrResult = S_XML_NODE_NOT_FOUND;
		    }
	    }
    }
    catch (...) 
    {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }

    if (pChildNodes != NULL) 
    {
		pChildNodes->Release();
    }

/*	if (hrResult != S_OK)
    {
        if (pChildNode != NULL) 
        {
		    pChildNode->Release();
        }
        if (*ppChildNode != NULL) 
        {
            (*ppChildNode)->Release();
            *ppChildNode = NULL;
        }
	}*/
	return hrResult;
}







HRESULT XML_GetNextSiblingNode(IXMLDOMNode *pNode, BSTR sNodeName,
                               IXMLDOMNode **ppSiblingNode)
// gets the next sibling node with the specified node name
{
    BOOL done,bHasNodeName;
    HRESULT hrResult;
    IXMLDOMNode *pActualNode;
    IXMLDOMNode *pSiblingNode;

    try 
    {
        *ppSiblingNode = NULL;
        pSiblingNode = NULL;
        pActualNode = pNode;
        done = FALSE;
        while (! done) 
        {
            hrResult = pActualNode->get_nextSibling(&pSiblingNode);
            switch (hrResult) 
            {
                case S_OK:
                    if (pActualNode != pNode) 
                    {
                        pActualNode->Release();
                    }
                    pActualNode = pSiblingNode;
                    hrResult = XML_HasNodeName(pSiblingNode,sNodeName,&bHasNodeName);
                    if ((hrResult == S_OK) && bHasNodeName) 
                    {
                        *ppSiblingNode = pSiblingNode;
                        done = TRUE;
                    }
                    break;
                case S_FALSE:
                    hrResult = S_XML_NODE_NOT_FOUND;
                    nobreak;
                default:
                    done = TRUE;    
                    break;
            }
            if (hrResult != S_OK) 
            {
                done = TRUE;
            }
        }
        if (pSiblingNode != *ppSiblingNode) 
        {
            pSiblingNode->Release();
            pSiblingNode = NULL;
        }

    }
    catch (...) 
    {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }

    if (hrResult != S_OK) 
    { 
        if (pSiblingNode != NULL) 
        {
            pSiblingNode->Release();
        }
    }

    return hrResult;
}





HRESULT XML_GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
                         BSTR *psAttributeValue)
// returns the value of an attribute
{
	HRESULT hrResult;
	VARIANT vValue;
	IXMLDOMNode *pAttributeNode = NULL;
	IXMLDOMNamedNodeMap *pAttributeMap = NULL;

    try 
    {
        *psAttributeValue = NULL;
	    hrResult = pNode->get_attributes(&pAttributeMap);
	    if (hrResult == S_OK) 
        {
		    hrResult = pAttributeMap->getNamedItem(sAttributeName, &pAttributeNode);
        }
		if (hrResult == S_OK) 
        {
		    hrResult = pAttributeNode->get_nodeValue(&vValue);
        }
		if (hrResult == S_OK) 
        {
            if (vValue.vt == VT_BSTR) 
            {
    			*psAttributeValue = vValue.bstrVal;
            } 
            else 
            {
                hrResult = S_FALSE;
            }
	    }
    }
    catch (...) 
    {
        hrResult = E_XML_EXCEPTION_OCCURED;
    }
	
    if (pAttributeNode!=NULL)
    {
        pAttributeNode->Release();
    }
    if (pAttributeMap!=NULL)
    {
        pAttributeMap->Release();
    }
    

    return hrResult;

}







/*
 *----------------------------------------------------------------------------*
 *  $History: fc_xml.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_xml
 * 
 * *****************  Version 4  *****************
 * User: Jd           Date: 26.04.01   Time: 14:58
 * Updated in $/4Control/COM/softing/fc/CMN/fc_xml
 * Removed some memory leaks.
 *==
 *----------------------------------------------------------------------------*
*/
