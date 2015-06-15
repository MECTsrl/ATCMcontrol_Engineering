#ifndef __KADDRAGDROPMAP_H_
#define __KADDRAGDROPMAP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "SBLLexTree.h"
#include "ComHelper.h"


//----  Forward Class Definitions:   -------------------------------*
//------------------------------------------------------------------*
/**
 *  class CKADDragDropAction
 *
 *  contains drag drop action information
 *
 *  @see
 */
class CKADDragDropAction
{
public:
    CKADDragDropAction() {};
    virtual ~CKADDragDropAction() {};

    CGuid       m_tHandler;         // COM-handler
    CString     m_strEnableExpr;    // enable expression    

};

//----  Defines:    ------------------------------------------------*
typedef CSBLLexTree<CKADDragDropAction>                 TDropTree;
typedef CMap<CString, LPCTSTR, TDropTree*, TDropTree*>  TDDMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



//------------------------------------------------------------------*
/**
 *  class CKADDragDropMap
 *
 *  contains drag drop action information in form of a map
 *  drag type -> drop type tree (list) + drop action data
 *
 *  @see
 */
class CKADDragDropMap
{
public:
    CKADDragDropMap();
    virtual ~CKADDragDropMap();

    BOOL AddDragDropAction(const CString& crstrDragType, const CString& crstrDropType, CGuid tGUID, const CString& crstrEnableExpression); 

    BOOL IsDragType(const CString& crstrType);
    BOOL IsDropType(const CString& crstrDragType, const CString& crstrType);

    BOOL GetDropInfo(const CString& crstrDragType, const CString& crstrDropType, GUID& rtGUID, CString& rstrEnableExpression);

protected:
    TDDMap      m_tMap;     // map drag type -> drop type lex tree
};
#endif // __KADDRAGDROPMAP_H_