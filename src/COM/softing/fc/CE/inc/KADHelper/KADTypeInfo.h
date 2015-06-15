#ifndef __KADTYPEINFO_H_
#define __KADTYPEINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "KADMenu.h"

#include <vector>

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADTypeData - class for holding KAD-type data
 *      - used like a structure
 *
 *  @see CKADTypeInfo
 */
class CKADTypeData
{
public:
    CKADTypeData() { m_bVisible = TRUE; m_iImageIdx = -1; m_bListView = FALSE; };
    ~CKADTypeData() {};

public:
    CString     m_strID;        // type id (= type name)
    CString     m_strBitmap;    // bitmap file name
    BOOL        m_bVisible;     // visible in tree view?
    BOOL        m_bListView;    // to be shown in list view?
    CString     m_strText;      // text to be shown in list view ...
    int         m_iImageIdx;    // image index in image list
};


//------------------------------------------------------------------*
/**
 *  class CKADStatusData
 *
 *  @see    ....
*/
class CKADStatusData
{
public:

    CKADStatusData() { m_iOverlayState = 0;};
    ~CKADStatusData() {};

public:
    CString     m_strText;              // status text information
    int         m_iOverlayState;        // index of overlay image
    int         m_iImageId;             // image id
};



typedef CArray<CKADStatusData*, CKADStatusData*>    T_StateArray;
typedef CMap<int, int, int, int>                    T_IntIntMap;
//------------------------------------------------------------------*
/**
 *  class CKADTypeInfo - class for KAD-type information (used like a structure)
 *
 *  @see    ....
 */
class CKADTypeInfo
{
public:
    CKADTypeInfo();
    ~CKADTypeInfo();

    void GetStateInfoFromStateVal(int iStateVal, int& riImageId, int& riOverlayState);
    // insert one or more state values to the state val map
    int InsertStateValues(const CString& crstrStateVals, int iStateIdx);

protected:
    void Reset();
    // insert a single state value to the state val map
    BOOL InsertStateValue(const CString& crstrStateVal, int iStateIdx);


public:
    CKADTypeData        m_tData;            // type data
    CKADMenu            m_tMenu;            // context menu of type
    T_StateArray        m_tStateArray;      // m_tStateArray[i] contains CKADStatusData*
                                            // belonging to state index i-1
    T_IntIntMap         m_tStateValMap;     // mapping of state values to state indices
};


#endif // __KADTYPEINFO_H_
