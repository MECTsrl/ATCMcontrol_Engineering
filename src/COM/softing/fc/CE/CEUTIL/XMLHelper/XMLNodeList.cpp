


//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "XMLNode.h"
#include "XMLNodeList.h"


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
CXMLNodeList::CXMLNodeList()
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
CXMLNodeList::~CXMLNodeList()
{

}


//------------------------------------------------------------------*
/**
 * GetLength
 *
 * @param           
 * @return          returns the number of nodes in this list
 *                  0 if list is empty or an error occured
 * @exception       -
 * @see             
*/
long CXMLNodeList::GetLength()
{
    //HRESULT get_length(long *listLength);
    long ret = 0;

    if (!m_nodeList)
    {
        return 0;
    }

    HRESULT hr = m_nodeList->get_length(&ret);
    if (hr != S_OK)
    {
        return 0;
    }
    return ret;
}


//------------------------------------------------------------------*
/**
 * Item
 * get one node of that list, address that node with a position in the list
 *
 * @param           [in]  pos:      the position of the node
 *                  [out] node:     the xml node
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNodeList::Item(long pos, CXMLNode& node) const
{
    //HRESULT item(long index,IXMLDOMNode **listItem);
    CComPtr<IXMLDOMNode> listItem;

    if (!m_nodeList)
    {
        return FALSE;
    }

    HRESULT hr = m_nodeList->get_item(pos, &listItem);
    if (hr != S_OK)
    {
        return FALSE;
    }

    node.SetIXMLNode(listItem);
    return TRUE;
}
 

//------------------------------------------------------------------*
/**
 * Reset
 * reset node iteration, next will return the first node after this
 *
 * @param           
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNodeList::Reset()
{
    //HRESULT reset(void);
    if (!m_nodeList)
    {
        return FALSE;
    }

    HRESULT hr = m_nodeList->reset();
    if (hr != S_OK)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * Next
 * return the next node in the node iteration
 *
 * @param           [out] node:  the xml node
 * @return          TRUE:  on success
 *                  FALSE: else
 * @exception       -
 * @see             
*/
BOOL CXMLNodeList::Next(CXMLNode& node)
{
    //HRESULT nextNode(IXMLDOMNode **nextItem);
    CComPtr<IXMLDOMNode> nextItem;

    if (!m_nodeList)
    {
        return FALSE;
    }

    HRESULT hr = m_nodeList->nextNode(&nextItem);
    if (hr != S_OK || !nextItem)
    {
        return FALSE;
    }

    if (!nextItem)
    {
        return FALSE;
    }

    node.SetIXMLNode(nextItem);
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * SetIXMLNodeList
 * initializes the node list with an interface pointer
 * !This function may only be used by other xml helper classes!
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CXMLNodeList::SetIXMLNodeList(IXMLDOMNodeList* pNodeList)
{
    if (m_nodeList)
    {
        m_nodeList.Release();
    }
    m_nodeList = pNodeList;
    return TRUE;
}
