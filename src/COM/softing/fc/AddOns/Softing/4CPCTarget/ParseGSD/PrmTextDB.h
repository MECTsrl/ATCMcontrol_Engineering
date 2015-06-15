
#ifndef __PRMTEXTDB_H_
#define __PRMTEXTDB_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "defs.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
*  class CPrmTextItem
*
*  @see    ....
*/
class CPrmTextItem
{
// =Construction
public:
                            CPrmTextItem(long lValue, LPCTSTR pszDescription);
                                // default ctor
    virtual                 ~CPrmTextItem();
                                // dtor
                            CPrmTextItem(CPrmTextItem const & other);
                                // copy ctor
    CPrmTextItem const &    operator = (CPrmTextItem const & other);
                                // assignment

// =Access
public:
    long                    GetValue(void) const
                            {
                                return m_lValue;
                            }
    CString const &         GetDescription()
                            {
                                return m_strDescr;
                            }

// =Operations
public:

// =Overrides
public:

protected:

// =Helpers
private:

// =Attributes
protected:

private:
    long                    m_lValue;
                                // value of parameter
    CString                 m_strDescr;
                                // textual description to this value

};

//----  Typedefs:    -----------------------------------------------*
typedef CMap<long, long&, CPrmTextItem*, CPrmTextItem*&>    mapTextItem;
typedef CList<CPrmTextItem*, CPrmTextItem*&>                listTextItem;




//------------------------------------------------------------------*
/**
*  class CPrmTextDef
*
*  @see    ....
*/
class CPrmTextDef
{
// =Construction
public:
                            CPrmTextDef(long lReference = 0);
                                // default ctor
    virtual                 ~CPrmTextDef();
                                // dtor
                            CPrmTextDef(CPrmTextDef const & other);
                                // copy ctor
    CPrmTextDef const &    operator = (CPrmTextDef const & other);
                                // assignment

// =Access
public:
    long                    GetRefNumber(void) const
                            {
                                return m_lReference;
                            }
    void                    PutRefNumber(long lRef)
                            {
                                m_lReference = lRef;
                            }
    BOOL                    GetDescription(long lValue, CString &strDescr);

// =Operations
public:
    BOOL                    AddItem(CPrmTextItem *pItem);
    HRESULT                 GetValueTextLists(VARIANT *pValues, VARIANT *pNames);
    void                    Check(long,long, CStationCallback*);

// =Overrides
public:

protected:

// =Helpers
private:
    void                    Clear();

// =Attributes
protected:

private:
    long                    m_lReference;
                                // is referenced from ExtUserPrmData block by this value
    listTextItem            m_listTextItem;
                                // main list of text items
    mapTextItem             m_mapTextItem;
                                // index map of values to textual description

};

//----  Typedefs:    -----------------------------------------------*
typedef CMap<LONG, LONG, CPrmTextDef*, CPrmTextDef*&>       mapTextDefDB;


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __PRMTEXTDB_H_

