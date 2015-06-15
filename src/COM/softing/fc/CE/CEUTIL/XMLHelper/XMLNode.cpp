

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "XMLNode.h"
#include "XMLNodeList.h"
#include "XMLDocument.h"
#include "IdPathHelp.h"
#include "CESysDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//------------------------------------------------------------------*
/**
 * Constructor
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CXMLNode::CXMLNode()
{

}


//------------------------------------------------------------------*
/**
 * Destructor
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CXMLNode::~CXMLNode()
{

}


//------------------------------------------------------------------*
/**
 * GetTagName
 *
 * @param            [out] strTagName: the name of the xml node
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetTagName(CString& strTagName) const
{
    CComBSTR sName;

    if (m_node)
    {
        HRESULT hr =  m_node->get_nodeName(&sName);
        if (hr != S_OK)
        {
            return FALSE;
        }
        strTagName = sName;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
 

//------------------------------------------------------------------*
/**
 * GetParentNode
 * returns the parent node of that xml node
 *
 * @param           [out] parentNode: the parent xml node
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetParentNode(CXMLNode& parentNode) const
{
    CComPtr<IXMLDOMNode> node;

    if (!m_node)
    {
        return FALSE;
    }

    HRESULT hr = m_node->get_parentNode(&node);

    if (hr != S_OK || !node)
    {
        return FALSE;
    }

    parentNode.SetIXMLNode(node);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetChildNodeList
 * get the list of all child nodes of that xml node
 *
 * @param           [out] nodeList: the list of child nodes
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetChildNodeList(CXMLNodeList& nodeList) const
{
    //HRESULT get_childNodes(IXMLDOMNodeList **childList);
    CComPtr<IXMLDOMNodeList> childList;

    if (!m_node)
    {
        return FALSE;
    }

    HRESULT hr = m_node->get_childNodes(&childList);
    if (hr != S_OK)
    {
        return FALSE;
    }

    nodeList.SetIXMLNodeList(childList);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * AppendChild
 * add a new child node to the children of this node, the node will be
 * appended behind all other child nodes
 *
 * @param           [in] childNode: the node to append
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::AppendChild(const CXMLNode& childNode,
                           BOOL bSorted /*= FALSE*/, 
                           BOOL bIgnoreCase /*= FALSE*/)
{
    CComPtr<IXMLDOMNode> outNode;
    if (!bSorted)
    {
        HRESULT hr = m_node->appendChild(childNode.m_node, &outNode);
        if (hr != S_OK)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    // insert child sorted

    // get all children
    CXMLNodeList            tChildList;
    CXMLNode                tNodeChild;
    CXMLNode                tBeforeNode;
    CString                 strChildName;
    CString                 strTextInsert;

    if (childNode.GetAttribute(CE_XMLATTR_ID, strTextInsert))
    {
        if(GetChildNodeList(tChildList))
        {
            long lNumChildren = tChildList.GetLength();
            for(long lChild = 0; lChild < lNumChildren; ++lChild)
            {
                tChildList.Item(lChild, tNodeChild);
                tNodeChild.GetAttribute(CE_XMLATTR_ID, strChildName);

                // if child name alphabetically after file/folder name, insert before
                if (bIgnoreCase)
                {
                    if(strChildName.CompareNoCase(strTextInsert)>0)
                    {
                        tBeforeNode = tNodeChild;
                        break;
                    }
                }
                else
                {
                    if(strChildName.Compare(strTextInsert)>0)
                    {
                        tBeforeNode = tNodeChild;
                        break;
                    }
                }
            }
        }
    }

    return InsertChildBefore(childNode, tBeforeNode);
}


//------------------------------------------------------------------*
/**
 * InsertChildBefore
 * insert a new child node to this node before another child node
 *
 * @param           [in] childNode:  the new node to insert
 * @param           [in] beforeNode: the node before which the new node 
 *                                   should be inserted
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::InsertChildBefore(const CXMLNode& childNode, const CXMLNode& beforeChild)
{
    //HRESULT insertBefore(IXMLDOMNode *newChild,VARIANT refChild,IXMLDOMNode **outNewChild);
    CComPtr<IXMLDOMNode> outNewChild;
    CComVariant refChild;

    if (!m_node)
    {
        return FALSE;
    }

    refChild = (IXMLDOMNode*)beforeChild.m_node;

    HRESULT hr = m_node->insertBefore(childNode.m_node, refChild, &outNewChild);
    if (hr != S_OK)
    {
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * ReplaceChild
 * replace an existing child of this node with a new node
 *
 * @param           [in] newChild:  the new child node
 * @param           [in] oldChild:  the child node that should be replaced
 * @return          
 * @exception       -
 * @see             
*/
BOOL CXMLNode::ReplaceChild(const CXMLNode& newChild, const CXMLNode& oldChild)
{
    //HRESULT replaceChild(IXMLDOMNode *newChild,IXMLDOMNode *oldChild,IXMLDOMNode **outOldChild);
    CComPtr<IXMLDOMNode> outOldChild;

    if (!m_node)
    {
        return FALSE;
    }

    HRESULT hr = m_node->replaceChild(newChild.m_node, oldChild.m_node, &outOldChild);
    if (hr != S_OK)
    {
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * RemoveChild
 * remove a child of this node
 *
 * @param           [in] child: the child node
 * @return          
 * @exception       -
 * @see             
*/
BOOL CXMLNode::RemoveChild(const CXMLNode& child)
{
    //HRESULT removeChild(IXMLDOMNode *childNode,IXMLDOMNode **outOldChild);
    CComPtr<IXMLDOMNode> outOldChild;

    if (!m_node)
    {
        return FALSE;
    }

    HRESULT hr = m_node->removeChild(child.m_node, &outOldChild);
    if (hr != S_OK)
    {
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetAttribute
 * get a value for an attribute with a known name
 *
 * @param           [in] strName:   the name of the attribute
 * @param           [out] strValue: the value of the attribute
 * @param           [in] bRecursive: if attribute not found function is called for parent node
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetAttribute(const CString& strName, CString& strValue, BOOL bRecursive /*=FALSE*/) const
{
    // element : HRESULT getAttribute(BSTR name,VARIANT *value);
    CComVariant attrText;
    CComBSTR name = strName;
    strValue = "";

    CComQIPtr<IXMLDOMElement, &IID_IXMLDOMElement> element;
    element = m_node;
    if (!element)
    {
        return FALSE;
    }

    HRESULT hr = element->getAttribute(name, &attrText);
    if (hr != S_OK)
    {
        if(bRecursive)
        {
            // get parent node
            CXMLNode    tParent;
            if(GetParentNode(tParent))
            {
                if(tParent.GetAttribute(strName, strValue, bRecursive))
                {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    if (attrText.vt!=VT_BSTR)
    {
        return FALSE;
    }

    strValue = attrText.bstrVal;

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * SetAttribute
 * set the value of an attribute:
 * if a attribute with the name does already exist, the value will be
 * changed, if not a new attribute will be created
 *
 * @param           [in] strName: the name of the attribute
 * @param           [in] strValue: the value of the attribute
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::SetAttribute(const CString& strName, const CString& strValue)
{
    // element : HRESULT setAttribute(BSTR name,VARIANT value);
    CComVariant attrText = strValue;
    CComBSTR name = strName;

    CComQIPtr<IXMLDOMElement, &IID_IXMLDOMElement> element;
    element = m_node;
    if (!element)
    {
        return FALSE;
    }

    HRESULT hr = element->setAttribute(name, attrText);
    if (hr != S_OK)
    {
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * RemoveAttribute
 * remove an attribute of the node
 *
 * @param           [in] strName: the name of the attribute
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::RemoveAttribute(const CString& strName)
{
    // element : HRESULT removeAttribute(BSTR name);
    CComBSTR name = strName;

    CComQIPtr<IXMLDOMElement, &IID_IXMLDOMElement> element;
    element = m_node;
    if (!element)
    {
        return FALSE;
    }

    HRESULT hr = element->removeAttribute(name);
    if (hr != S_OK)
    {
        return FALSE;
    }
    return TRUE;

}


//------------------------------------------------------------------*
/**
 * GetChildWithAttr
 * searches all childs of this node and returns the first child that 
 * contains an attribute with the given name and value
 *
 * @param           [in] strAttrName:    the name of the attribute
 * @param           [in] strAttrValue:   the value of the attribute
 * @param           [out] child:         the child node containing the
 *                                       attribute, if found
 * @return          TRUE: if a child with the attribute has been found
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetChildWithAttr(const CString& strAttrName, const CString& strAttrValue, CXMLNode& child) const
{
    BOOL ret = FALSE;
    CXMLNodeList nodeList;
    CXMLNode enumChild;

    GetChildNodeList(nodeList);
    nodeList.Reset();

    while(nodeList.Next(enumChild))
    {
        CString attrVal;
        enumChild.GetAttribute(strAttrName, attrVal);
        if (attrVal.Compare(strAttrValue)==0)
        {
            ret = TRUE;
            break;
        }
    }

    if (ret)
    {
        child = enumChild;
    }
    return ret;
}

//------------------------------------------------------------------*
/**
 * GetFirstChildWithTagName
 * searches all childs of this node and returns the first child
 * with the given tag name
 *
 * @param           [in] crstrTagName:   the tag name
 * @param           [out] child:         the found child node
 * @return          TRUE: if a child with the tag name has been found
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetFirstChildWithTagName(const CString& crstrTagName, CXMLNode& child) const
{
    BOOL ret = FALSE;
    CXMLNodeList nodeList;
    CXMLNode enumChild;

    GetChildNodeList(nodeList);
    nodeList.Reset();

    while(nodeList.Next(enumChild))
    {
        CString strTagName;
        enumChild.GetTagName(strTagName);
        if (strTagName.Compare(crstrTagName)==0)
        {
            ret = TRUE;
            break;
        }
    }

    if (ret)
    {
        child = enumChild;
    }
    return ret;
}


//------------------------------------------------------------------*
/**
 * RemoveAllChilds
 * removes all child nodes under this node
 *
 * @param           
 * @return          TRUE: on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::RemoveAllChilds()
{
    CXMLNodeList nodeList;
    CXMLNode child;

    GetChildNodeList(nodeList);
    nodeList.Reset();

    while(nodeList.Next(child))
    {
        RemoveChild(child);
    }
    return TRUE;
}



   
//------------------------------------------------------------------*
/**
 * SetIXMLNode
 * initialize the node with an xml interface pointer
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CXMLNode::SetIXMLNode(IXMLDOMNode* pNode)
{
    if (m_node)
    {
        m_node.Release();
    }
    m_node = pNode;
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * SetIXMLNode
 * initialize the node with an xml interface pointer as IUnknown*
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CXMLNode::SetIXMLNode(IUnknown* pNode)
{
    if (m_node)
    {
        m_node.Release();
    }
    m_node = pNode;
    if (!m_node)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Get the dom node as xml interface pointer.
 *
 * @param           - IXMLDOMNode** ppNode - interface pointer to node
 * @return          - TRUE if successful, else FALSE
*/
BOOL CXMLNode::GetIXMLNode(IXMLDOMNode** ppNode)
{
    ASSERT(ppNode);

    if (!m_node)
    {
        return FALSE;
    }

    *ppNode = (IXMLDOMNode*)m_node;
    (*ppNode)->AddRef();
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Get the dom node as xml interface pointer.
 *
 * @param           - IUnknown** ppNode - interface pointer to node
 * @return          - TRUE if successful, else FALSE
*/
BOOL CXMLNode::GetIXMLNodeUnknown(IUnknown** ppNode)
{
    ASSERT(ppNode);
    
    if (!m_node)
    {
        return FALSE;
    }

    *ppNode = (IUnknown*)((IXMLDOMNode*)m_node);
    (*ppNode)->AddRef();
    return TRUE;
}







// SIS, 01.02.01: 
//------------------------------------------------------------------*
/**
 * get id path
 *
 * @param           [in] strPath:   id path to node       
 * @return          TRUE if successful else FALSE         
*/
BOOL CXMLNode::GetIdPath(CString& strPath)
{
    CXMLNode        tParentNode;
    CXMLNode        tParentNodeTmp;
    CString         strId;
    int             iCount = 0;
    DOMNodeType     nodeType;

    strPath = "";
    GetAttribute(CE_XMLATTR_ID, strPath);
    strPath = CE_XML_IDPATH_SEP + strPath;

    if (!GetParentNode(tParentNode))
    {
        return TRUE;
    }
    tParentNode.GetNodeType(nodeType);

    while(nodeType == NODE_ELEMENT)
    {
        tParentNode.GetAttribute(CE_XMLATTR_ID, strId);
        strPath = CE_XML_IDPATH_SEP + strId + strPath;
        if(!tParentNode.GetParentNode(tParentNodeTmp))
            break;//sometimes returns FALSE and tParentNodeTmp==NULL ? d-2136
        tParentNode = tParentNodeTmp;
        tParentNode.GetNodeType(nodeType);
    }

    /*for(int iId = iCount-1; iId >= 0; --iId)
    {
        strPath += "/" + csaId[iId];
    }*/
    return TRUE;
}

// SIS, 01.02.01: 
//------------------------------------------------------------------*
/**
 * get node from id path
 *
 * if id path is absolute the search will start from root node
 * else from this node is started
 *
 * @param           [in] strPath:   id path to node  
 * @param           [out] node:     node with this id path  
 * @return          TRUE if successful else FALSE         
*/
BOOL CXMLNode::GetNodeFromIdPath(const CString& strPath, CXMLNode& node)
{
    CXMLNode    node_child;             // child node
    BOOL        bFound;                 // next child with given id found?
    CString     csPath = strPath;       // working copy of strPath
    CString     csId;                   // id to search

    // if path is empty -> return root node
    if(!StripIdFromIdPath(csPath, csId))
    {
        CXMLDocument    tXMLDoc;
        if(!GetDocument(tXMLDoc))
            return FALSE;
        tXMLDoc.GetRootNode(node);
        return TRUE;
    }

    // if absolute path is given, start from root node
    if(csId == "")
    {
        CXMLDocument    tXMLDoc;
        if(!GetDocument(tXMLDoc))
            return FALSE;
        tXMLDoc.GetDocNode(node);
        if(!StripIdFromIdPath(csPath, csId))
            return(TRUE);
    }
    else    // start from this node
        node.SetIXMLNode(m_node);

    // find all ids in path
    do
    {
        bFound = node.GetChildWithAttr(CE_XMLATTR_ID, csId, node_child);
        if(!bFound)
        {
            return FALSE;
        }
        node = node_child;
    } while(StripIdFromIdPath(csPath, csId));
    return TRUE;
}


// SIS, 01.02.01: 
//------------------------------------------------------------------*
/**
 * get document
 *
 * @param           [out] document:     XML-document    
 * @return          TRUE if successful else FALSE
*/
BOOL CXMLNode::GetDocument(CXMLDocument& document)
{
    CComPtr<IXMLDOMDocument>    tDOMDocPtr;
    HRESULT hr = m_node->get_ownerDocument(&tDOMDocPtr);
    if(hr != S_OK)
        return FALSE;
    document.SetIXMLDocument(tDOMDocPtr);
    return TRUE;
}

// SIS: 01.02.01: 
//------------------------------------------------------------------*
/**
 * get node type
 *
 * @param           [out] tNodeType:    XML node type     
 * @return          TRUE if successful else FALSE
*/
BOOL CXMLNode::GetNodeType(DOMNodeType& tNodeType)
{
    HRESULT hr;
    hr = m_node->get_nodeType(&tNodeType);
    if(SUCCEEDED(hr))
        return TRUE;
    return FALSE;
}

// SIS: 08.02.01: get parent node with attribute
//------------------------------------------------------------------*
/**
 * searches all childs of this node and returns the first child that 
 * contains an attribute with the given name and value
 *
 * @param           [in] strName:       the name of the attribute
 * @param           [in] strValue:      the value of the attribute
 *                                      if this is empty, value is not regarded
 * @param           [out] rtParent:     the parent node containing the
 *                                      attribute, if found
 * @param           [in] bIncludeThis:  the node itself is included in search
 *                                      default is FALSE
 * @return          TRUE: if a child with the attribute has been found
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetParentWithAttr(const CString& strName, const CString& strValue, CXMLNode& rtParent, BOOL bIncludeThis /*=FALSE*/)
{
    BOOL        ret = FALSE;
    BOOL        bValueRegardless = FALSE;
    CString     attrVal;

    if(strValue == "")
    {
        bValueRegardless = TRUE;
    }

    if(bIncludeThis)
    {
        if(GetAttribute(strName, attrVal))
        {
            if(bValueRegardless || attrVal.Compare(strValue)==0)
            {
                ret = TRUE;
                rtParent = *this;
                return(ret);
            }
        }
    }

    CXMLNode    tNode = *this;

    while(tNode.GetParentNode(rtParent))
    {
        if(rtParent.GetAttribute(strName, attrVal))
        {
            if(bValueRegardless || attrVal.Compare(strValue)==0)
            {
                ret = TRUE;
                break;
            }
        }
        tNode = rtParent;
    }

    return ret;
}

//------------------------------------------------------------------*
/**
 * GetXMLString
 * get the xml node as xml string representation.
 *
 * @param           [out] strXMLString : the xml representation of the document     
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNode::GetXMLString(CString &strXMLString) const
{
    CComBSTR sXML;

    HRESULT hr = m_node->get_xml(&sXML);

    if (hr != S_OK)
    {
        return FALSE;
    }

    strXMLString = sXML;
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * GetText: get the text set for an xml node.
 *
 * @param           strText: the set text will be returned
 * @return          GetText failed: TRUE/FALSE
 */
BOOL CXMLNode::GetText(CString& strText)
{
    CComBSTR sText;

    if (m_node)
    {
      HRESULT hr = m_node->get_text(&sText);
      if (hr != S_OK)
      {
          return FALSE;
      }
    
      strText = sText;

      return TRUE;
    }
    else
    {
      return FALSE;
    }
}



//------------------------------------------------------------------*
/**
 * SetText: set the text for an xml node.
 *
 * @param           strText: the text to set.
 * @return          Set text failed: TRUE/FALSE
 */
BOOL CXMLNode::SetText(const CString& strText)
{
    CComBSTR sText(strText);

    if (m_node)
    {
      HRESULT hr = m_node->put_text(sText);
      if (hr != S_OK)
      {
          return FALSE;
      }
    
      return TRUE;
    }
    else
    {
      return FALSE;
    }
}

