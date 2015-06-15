
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "KADFilterItem.h"

//----  Static Initializations:   ----------------------------------*

CKADFilterItem::CKADFilterItem()
{
}

CKADFilterItem::~CKADFilterItem()
{
}

//------------------------------------------------------------------*
/**
 * add type to filter map
*/
void CKADFilterItem::AddType(const CString& rstrType)
{
    m_tTypeSet.insert(rstrType);
}

//------------------------------------------------------------------*
/**
 * set filter text (text in filter category combo box of view)
*/
void CKADFilterItem::SetFilterText(const CString& rstrText)
{
    m_strText = rstrText;
}

//------------------------------------------------------------------*
/**
 * get filter text (text in filter category combo box of view)
*/
CString& CKADFilterItem::GetFilterText()
{
    return(m_strText);
}

//------------------------------------------------------------------*
/**
 * set filter expression (wildcard expression)
*/
void CKADFilterItem::SetFilterExpr(const CString& rstrExpr)
{
    m_strExpr = rstrExpr;
}

//------------------------------------------------------------------*
/**
 * get filter expression (wildcard expression)
*/
CString& CKADFilterItem::GetFilterExpr()
{
    return(m_strExpr);
}

//------------------------------------------------------------------*
/**
 * is passing type filter <-> is type contained in type set?
 *
 *  if type set is empty -> category filter is not used -> return TRUE
 *
 * @param           [in] rstrType: type id name
 * @return          is passing filter?
*/
BOOL CKADFilterItem::IsPassingTypeFilter(const CString& rstrType)
{
    if(!m_tTypeSet.empty())
    {
        if(m_tTypeSet.find(rstrType) == m_tTypeSet.end())
        {
            return FALSE;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * reset filter
*/
void CKADFilterItem::Reset()
{
    m_tTypeSet.clear();
}
