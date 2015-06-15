
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "KADFilterInfo.h"
#include "..\..\CeMain\resource.h"

//----  Static Initializations:   ----------------------------------*


//------------------------------------------------------------------*
/**
 * construct CKADFilterInfo object
 *
*/
CKADFilterInfo::CKADFilterInfo()
{
    Init();
}


//------------------------------------------------------------------*
/**
 * destroy CKADFilterInfo object
 * just call Reset member function
*/
CKADFilterInfo::~CKADFilterInfo()
{
    Reset();
}


//------------------------------------------------------------------*
/**
 * insert default: filter with filter text "All"
 *  this filter is not used and no types are added
 *
*/
void CKADFilterInfo::Init()
{
    CString strAll;
    strAll.LoadString(IDS_KAD_HELPER_FILTER_ALL);
    AddNewFilterItem(strAll);
}

//------------------------------------------------------------------*
/**
 * reset filter vector
 *
 *  first delete all filters then clear filter vector
 *
*/
void CKADFilterInfo::Reset()
{
    T_ItemPtrVect::iterator     itFilter;
    CKADFilterItem*             pFilterTmp;

    for(itFilter = m_apFilters.begin(); itFilter != m_apFilters.end(); ++itFilter)
    {
        pFilterTmp = *itFilter;
        if(pFilterTmp)
        {
            delete pFilterTmp;
            *itFilter = 0;
        }
    }
    m_apFilters.clear();
}

//------------------------------------------------------------------*
/**
 * get number of filters
 *
*/
int CKADFilterInfo::GetSize()
{
    return(m_apFilters.size());
}

//------------------------------------------------------------------*
/**
 * add new filter item with text and expression
 *
 * @param           [in] rstrText: text to be shown in filter category combo of view
 * @param           [in] rstrExpr: expression to be used for text filter
 *                                 default is ""
 * @return          index of new filter in array or -1 if it already exists
*/
int CKADFilterInfo::AddNewFilterItem(const CString& rstrText, const CString& rstrExpr)
{
    CKADFilterItem* pFilterItemTmp;

    // if filter item with text already exists -> return -1
    if(GetFilterItemIndex(rstrText) != -1)
    {
        return(-1);
    }
    pFilterItemTmp = new CKADFilterItem;
    pFilterItemTmp->SetFilterText(rstrText);
    pFilterItemTmp->SetFilterExpr(rstrExpr);
    m_apFilters.push_back(pFilterItemTmp);
    return(m_apFilters.size()-1);
}


//------------------------------------------------------------------*
/**
 * get index of filter item with certain text
 *
 * @param           [in] rstrText: text of filter item
 * @return          index of filter item or -1 if not found
 * @see             AddTypeToFilterItem()
*/
int CKADFilterInfo::GetFilterItemIndex(const CString& rstrText)
{
    T_ItemPtrVect::iterator     itFilter;
    CKADFilterItem*             pFilterTmp;
    CString                     strText;
    int                         iIndex = 0;
    int                         iFoundIndex = -1;

    for(itFilter = m_apFilters.begin(); itFilter != m_apFilters.end(); ++itFilter)
    {
        pFilterTmp = *itFilter;
        if(pFilterTmp)
        {
            strText = pFilterTmp->GetFilterText();
            if(strText == rstrText)
            {
                iFoundIndex = iIndex;
                break;
            }
        }
        ++iIndex;
    }
    return(iFoundIndex);
}


//------------------------------------------------------------------*
/**
 * add type to filter item
 *
 *  to get the index of a filter item with a certain text use
 *  GetFilterItemIndex()
 *
 * @param           [in] rstrType: type to add
 * @param           [in] iIndex: index of filter item
 * @see             GetFilterItemIndex()
*/
void CKADFilterInfo::AddTypeToFilterItem(const CString& rstrType, int iIndex)
{
    CKADFilterItem* pFilterItem = GetFilterItem(iIndex);

    if(pFilterItem)
    {
        pFilterItem->AddType(rstrType);
    }
}


//------------------------------------------------------------------*
/**
 * get pointer to filter object with certain index
 *
 * @param           [in] iIndex: index of filter in vector
 * @return          pointer to filter object or NULL if index beyond valid range
*/
CKADFilterItem* CKADFilterInfo::GetFilterItem(int iIndex)
{
    if(iIndex < 0 || iIndex < m_apFilters.size())
    {
        return(m_apFilters[iIndex]);
    }
    return(0);
}

//------------------------------------------------------------------*
/**
 * get all filter texts
 *
 *  this is to be used to fill up category filter combo in view
 *
 * @param           [out] rastrTexts: string array with filter texts
*/
void CKADFilterInfo::GetFilterTexts(CStringArray& rastrTexts)
{
    T_ItemPtrVect::iterator     itFilter;
    CKADFilterItem*             pFilterTmp;
    CString                     strText;

    rastrTexts.RemoveAll();
    for(itFilter = m_apFilters.begin(); itFilter != m_apFilters.end(); ++itFilter)
    {
        pFilterTmp = *itFilter;
        if(pFilterTmp)
        {
            strText = pFilterTmp->GetFilterText();
            rastrTexts.Add(strText);
        }
    }
}

//------------------------------------------------------------------*
/**
 * is passing filter with index iIndex?
 * get filter item with index iIndex and delegate work
 *
 * @param           [in] rstrType: type id text
 * @param           [in] iIndex: index of cat filter
 * @return          is passing filter?
 * @see             CKADFilterItem::IsPassingTypeFilter()
*/
BOOL CKADFilterInfo::IsPassingTypeFilter(const CString& rstrType, int iIndex)
{
    CKADFilterItem* pFilterItem = GetFilterItem(iIndex);
    if(pFilterItem)
    {
        return(pFilterItem->IsPassingTypeFilter(rstrType));
    }
    return(TRUE);
}

// get filter expression of item iIndex
CString CKADFilterInfo::GetFilterExpr(int iIndex)
{
    CKADFilterItem* pFilterItem = GetFilterItem(iIndex);
    if(pFilterItem)
    {
        return(pFilterItem->GetFilterExpr());
    }
    return("");
}
