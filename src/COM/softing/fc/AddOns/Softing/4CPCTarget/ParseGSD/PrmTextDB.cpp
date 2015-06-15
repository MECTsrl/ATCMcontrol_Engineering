//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "PrmTextDB.h"
#include "gsdmessage.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrmTextItem

//------------------------------------------------------------------*
CPrmTextItem::CPrmTextItem(long lValue, LPCTSTR pszDescription)
{
    m_lValue = lValue;
    ASSERT(pszDescription != NULL);
    m_strDescr = pszDescription;
}

//------------------------------------------------------------------*
CPrmTextItem::~CPrmTextItem()
{
}

//------------------------------------------------------------------*
// copy ctor
CPrmTextItem::CPrmTextItem(CPrmTextItem const & other)
{
    *this = other;  // simply call assignment operator
}
                                
//------------------------------------------------------------------*
// assignment
CPrmTextItem const &
    CPrmTextItem::operator = (CPrmTextItem const & other)
{
    if (this == &other) {
        return *this;       // copying myself, I'm done
    }

    // copy object data:
    m_lValue   = other.m_lValue;
    m_strDescr = other.m_strDescr;

    return *this;
}
                                



/////////////////////////////////////////////////////////////////////////////
// CPrmTextDef

//------------------------------------------------------------------*
CPrmTextDef::CPrmTextDef(long lReference)
  : m_lReference(0)
{
    m_lReference = lReference;
}

//------------------------------------------------------------------*
CPrmTextDef::~CPrmTextDef()
{
    Clear();
}


//------------------------------------------------------------------*
// copy ctor
CPrmTextDef::CPrmTextDef(CPrmTextDef const & other)
{
    *this = other;  // simply call assignment operator
}
                                
//------------------------------------------------------------------*
/**
 * operator=
 *  assignment
 *
 * @param           
 * @return          
 * @exception       CMemoryException
 * @see             
*/
CPrmTextDef const &
    CPrmTextDef::operator = (CPrmTextDef const & other)
{
    if (this == &other) {
        return *this;       // copying myself, I'm done
    }

    // copy object data:
    m_lReference = other.m_lReference;
    // deep copy list and map:
    Clear();

    POSITION    pos;
    for (pos = other.m_listTextItem.GetHeadPosition(); pos != NULL; )
    {
        CPrmTextItem *pItem = other.m_listTextItem.GetNext(pos);
        if (pItem != NULL)
        {
            CPrmTextItem *pNewItem = new CPrmTextItem(*pItem);
            if (pNewItem != NULL) {
                AddItem(pNewItem);
            }
            else {
                // should throw mem exception!
                AfxThrowMemoryException();
            }
        }
    }

    return *this;
}
                                

//------------------------------------------------------------------*
void
    CPrmTextDef::Clear()
{
    CPrmTextItem *pItem = NULL;
    while (!m_listTextItem.IsEmpty())
    {
        pItem = m_listTextItem.RemoveTail();
        if (pItem != NULL) {
            delete pItem;
        }
    }
    m_listTextItem.RemoveAll();
    m_mapTextItem.RemoveAll();
}

//------------------------------------------------------------------*
BOOL
    CPrmTextDef::GetDescription(long lValue, CString &strDescr)
{
    CPrmTextItem *pItem = NULL;
    if (m_mapTextItem.Lookup(lValue, pItem)) {
        strDescr = pItem->GetDescription();
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
BOOL
    CPrmTextDef::AddItem(CPrmTextItem *pItem)
{
    if (pItem == NULL) {
        return FALSE;
    }
    long lValue = pItem->GetValue();
    CPrmTextItem *pOld = NULL;
    BOOL bExists = m_mapTextItem.Lookup(lValue, pOld);
    if (pOld != NULL) {
        // clean it up before overwriting it!
        POSITION pos = m_listTextItem.Find(pOld);
        if (pos != NULL) {
            m_listTextItem.RemoveAt(pos);
        }
        delete pOld;
    }

    m_listTextItem.AddTail(pItem);
    m_mapTextItem.SetAt(lValue, pItem);
    return bExists;
}


//------------------------------------------------------------------*
HRESULT
    CPrmTextDef::GetValueTextLists(VARIANT *pValues, VARIANT *pNames)
{
    if (   (pValues == NULL)
        || (pNames == NULL))
    {
        return E_POINTER;
    }

    HRESULT hr;
    // build array of values:
    pValues->vt = VT_I4 | VT_ARRAY;   // array of long
    SAFEARRAYBOUND boundData;
    boundData.lLbound = 0;
    boundData.cElements = m_listTextItem.GetCount();
    SAFEARRAY   *psaValues = ::SafeArrayCreate(VT_I4, 1, &boundData);
    if (psaValues == NULL) {
        return E_OUTOFMEMORY;
    }
    
    pNames->vt = VT_BSTR | VT_ARRAY;   // array of BSTR
    SAFEARRAYBOUND boundData2;
    boundData2.lLbound = 0;
    boundData2.cElements = m_listTextItem.GetCount();
    SAFEARRAY   *psaNames = ::SafeArrayCreate(VT_BSTR, 1, &boundData2);
    if (psaNames == NULL) {
        ::SafeArrayDestroy(psaValues);
        return E_OUTOFMEMORY;
    }
    
    // copy values to SAFEARRAY
    CString strDescr;
    POSITION pos = m_listTextItem.GetHeadPosition();
    for (long i = boundData.lLbound; pos != NULL; i++)
    {
        CPrmTextItem *pItem = m_listTextItem.GetNext(pos);
        _ASSERTE(pItem != NULL);
        LONG    lValue = pItem->GetValue();
        _ASSERTE((ULONG) i <= boundData.cElements);
        hr = ::SafeArrayPutElement(psaValues, &i, &lValue);
        _ASSERTE(SUCCEEDED(hr));
        if (FAILED(hr)) {
            continue;
        }
        strDescr = pItem->GetDescription();
        BSTR sDescr = strDescr.AllocSysString();
        hr = ::SafeArrayPutElement(psaNames, &i, sDescr);
        ::SysFreeString(sDescr);
        _ASSERTE(SUCCEEDED(hr));
        if (FAILED(hr)) {
            continue;
        }
    }
    pValues->parray = psaValues;
    pNames->parray  = psaNames;
    return S_OK;

}

void CPrmTextDef::Check(long lMinValue ,long lMaxValue, CStationCallback* pcb)
{
    POSITION pos = NULL;
    HRESULT hr;
    long l;
    CString strDescr,str;

    for (pos = m_listTextItem.GetHeadPosition(); pos != NULL;)
    {
        CPrmTextItem *pItem = m_listTextItem.GetNext(pos);
        l = pItem->GetValue();

        if(l < lMinValue || l > lMaxValue) {
            strDescr = pItem->GetDescription();
            hr = W_GSD_PRMTEXT_VALUE;
            GSD_FormatMessage(str,hr,l,(LPCTSTR)strDescr);
            pcb->Message(hr,str);
        }
    }

}

