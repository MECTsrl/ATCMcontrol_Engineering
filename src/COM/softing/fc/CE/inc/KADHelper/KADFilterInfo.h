#ifndef __KADFILTERINFO_H_
#define __KADFILTERINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>
#include "KADFilterItem.h"

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
typedef std::vector<CKADFilterItem*>    T_ItemPtrVect;
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADFilterInfo
 *  
 *  contains category filter information for a view
 *  first filter (index 0) is always "All" and is equivalent to no filtering
 */
class CKADFilterInfo
{
public:
    CKADFilterInfo();
    ~CKADFilterInfo();

    // initialization
    void Init();

    // get number of filters
    int GetSize();

    // add new filter item with text and expression
    int AddNewFilterItem(const CString& rstrText, const CString& rstrExpr = "");

    // get index for filter item with certain text
    int GetFilterItemIndex(const CString& rstrText);

    // add type to filter item with certain index
    void AddTypeToFilterItem(const CString& rstrType, int iIndex);

    // get all filter texts
    void GetFilterTexts(CStringArray& rastrTexts);

    // get filter expression of item iIndex
    CString GetFilterExpr(int iIndex);

    // get pointer to filter object with index iIndex
    CKADFilterItem* GetFilterItem(int iIndex);

    // is passing filter with index iIndex?
    BOOL IsPassingTypeFilter(const CString& rstrType, int iIndex);

protected:
    // reset filter item vector
    void Reset();

protected:
     T_ItemPtrVect      m_apFilters;    // vector of pointers to filters
};

#endif // __KADFILTERINFO_H_

