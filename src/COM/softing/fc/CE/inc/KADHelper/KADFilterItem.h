#ifndef __KADFILTERITEM_H_
#define __KADFILTERITEM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"
#include <set>


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
typedef std::set<CString>   T_StringSet;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADFilterItem
 *
 *  data for category filter in navigator
 *  @see    ....
 */
class CKADFilterItem
{
// public members
public:
    CKADFilterItem();
    ~CKADFilterItem();

    // add type to filter map
    void AddType(const CString& rstrType);

    // set filter text (text in combo box)
    void SetFilterText(const CString& rstrText);

    // get filter text (text in combo box)
    CString& GetFilterText();

    // set filter expression (wildcard expression)
    void SetFilterExpr(const CString& rstrExpr);

    // get filter expression (wildcard expression)
    CString& GetFilterExpr();

    // is passing type filter <-> is type contained in type set?
    BOOL IsPassingTypeFilter(const CString& rstrType);

    // reset filter
    void Reset();

// protected members
protected:

    CString         m_strText;      // text for combo-box
    CString         m_strExpr;      // wildcard expression
    T_StringSet     m_tTypeSet;     // set of types
};

#endif // __KADFILTERITEM_H_