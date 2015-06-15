

#include "stdafx.h"
#include "XMLNodeIter.h"

#include "XMLNode.h"
#include "XMLNodelist.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor with xml node.
 *
 * @param           [in] rtNode: starting node
*/
CXMLNodeIter::CXMLNodeIter(CXMLNode& rtNode)
{
    m_tNodeStack.empty();
    m_tNodeStack.push(rtNode);
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
*/
CXMLNodeIter::~CXMLNodeIter()
{
    Reset();
}


//------------------------------------------------------------------*
/**
 * get next node.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CXMLNodeIter::GetNext(CXMLNode& rtNode)
{
    if(!m_tNodeStack.pop(rtNode))
    {
        return FALSE;
    }

    CXMLNodeList    tChildList;
    if(rtNode.GetChildNodeList(tChildList))
    {
        CXMLNode    tNodeChild;
        long        lNumChildren = tChildList.GetLength();
        long        lChild;
        // add children in reverse order to stack to get correct pop order
        for(lChild = lNumChildren - 1; lChild >= 0; --lChild)
        {
            if(tChildList.Item(lChild, tNodeChild))
            {
                m_tNodeStack.push(tNodeChild);
            }
        }
    }
    return TRUE;
}

void CXMLNodeIter::Reset()
{
    m_tNodeStack.empty();
}

