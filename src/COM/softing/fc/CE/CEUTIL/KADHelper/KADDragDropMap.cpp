
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "KADDragDropMap.h"

//----  Static Initializations:   ----------------------------------*


CKADDragDropMap::CKADDragDropMap()
{
}

CKADDragDropMap::~CKADDragDropMap()
{
    // delete all drop trees
    POSITION        tPosition;
    CString         strTypeName;
    TDropTree*      pDropTree;

    tPosition = m_tMap.GetStartPosition();
    while(tPosition)
    {
        m_tMap.GetNextAssoc(tPosition, strTypeName, pDropTree);
        delete pDropTree;
    }

}

//------------------------------------------------------------------*
/**
 * add drag drop action.
 *
 * @param           [in] crstrDragType: drag type
 * @param           [in] crstrDropType: drop type
 * @param           [in] pGUID: drag drop action handler GUID
 * @param           [in] crstrEnableExpression: enable expression
 * @return          successfully added?
*/
BOOL CKADDragDropMap::AddDragDropAction(const CString& crstrDragType, const CString& crstrDropType, CGuid tGUID, const CString& crstrEnableExpression) 
{
    try
    {
        // if drag type not existing, create new drop tree in owner mode
        TDropTree* pDropTree = NULL;
        m_tMap.Lookup(crstrDragType, pDropTree);

        if(!pDropTree)
        {
            pDropTree = new TDropTree;
            pDropTree->setModusOwner();
            m_tMap.SetAt(crstrDragType, pDropTree);
        }
        // if drop type is not empty
        if(!crstrDropType.IsEmpty())
        {
            // insert drop type into drop tree
            CKADDragDropAction* pDDAction = new CKADDragDropAction;
            pDDAction->m_tHandler = tGUID;
            pDDAction->m_strEnableExpr = crstrEnableExpression;
            if(pDropTree->insertData(crstrDropType, pDDAction))
            {
                return TRUE;
            }
        }
    }
    catch (CMemoryException* e)
    {
        e->Delete();
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * is drag type?.
 *
 * @param           [in] crstrType: type name string
 * @return          is drag type?
*/
BOOL CKADDragDropMap::IsDragType(const CString& crstrType)
{
    // if drag type exists, return TRUE else FALSE
    TDropTree* pDropTree = NULL;
    m_tMap.Lookup(crstrType, pDropTree);
    if(pDropTree)
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * is drop type?.
 *
 * @param           [in] crstrDragType: drag type name string
 * @param           [in] crstrType: type name string
 * @return          can drag type be dropped on this type?
*/
BOOL CKADDragDropMap::IsDropType(const CString& crstrDragType, const CString& crstrType)
{
    // if drag type exists
    TDropTree* pDropTree = NULL;
    m_tMap.Lookup(crstrDragType, pDropTree);
    if(pDropTree)
    {
        CKADDragDropAction*   pCKADDragDropAction = pDropTree->getData(crstrType);
        // if drop type exists in drop tree, return TRUE else return FALSE
        if(pCKADDragDropAction)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get drop info.
 *
 * @param           [in] crstrDragType: drag type name string
 * @param           [in] crstrType: type name string
 * @param           [out] rtGUID: guid of drag drop handler if found
 * @param           [out] rstrEnableExpression: enable expression
 * @return          drop handler found?
*/
BOOL CKADDragDropMap::GetDropInfo(const CString& crstrDragType, const CString& crstrDropType, GUID& rtGUID, CString& rstrEnableExpression)
{
    // if drag type exists
    TDropTree* pDropTree = NULL;
    m_tMap.Lookup(crstrDragType, pDropTree);
    if(pDropTree)
    {
        CKADDragDropAction*   pDDAction = pDropTree->getData(crstrDropType);
        // if drop type exists in drop tree, return TRUE else return FALSE
        if(pDDAction)
        {
            rtGUID = pDDAction->m_tHandler;
            rstrEnableExpression = pDDAction->m_strEnableExpr;
            return TRUE;
        }
    }
    return FALSE;
}
