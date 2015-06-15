
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

#include "stdafx.h"
#include "KADTypeInfo.h"

//----  Static Initializations:   ----------------------------------*

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


CKADTypeInfo::CKADTypeInfo()
{
}

CKADTypeInfo::~CKADTypeInfo()
{
    Reset();
}

//------------------------------------------------------------------*
/**
 * reset type info object.
 *
*/
void CKADTypeInfo::Reset()
{
    // delete state objects
    CKADStatusData*     pStateTmp;
    int                 iNumStates = m_tStateArray.GetSize();

    for(int iStateIdx = 0; iStateIdx < iNumStates; ++iStateIdx)
    {
        pStateTmp = m_tStateArray[iStateIdx];
        ASSERT(pStateTmp);
        if(pStateTmp)
        {
            delete pStateTmp;
        }
    }
    m_tStateArray.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * insert state values.
 *
 * @param           [in] crstrStateVals: state values as string with 
 *                          semicolons as separators
 * @param           [in] iStateIdx: 1 based state index
 * @return          
 * @exception       -
 * @see             
*/
int CKADTypeInfo::InsertStateValues(const CString& crstrStateVals, int iStateIdx)
{
    // StateValue string looks like this: "2;4;7;13"
    int         iTmp;
    CString     strStateVals = crstrStateVals;
    CString     strVal;
    int         iCount = 0;

    iTmp = strStateVals.Find(_T(';'));
    while(iTmp != -1)
    {
        strVal = strStateVals.Left(iTmp);
        if(InsertStateValue(strVal, iStateIdx))
        {
            ++iCount;
        }
        strStateVals = strStateVals.Right(strStateVals.GetLength() - iTmp -1);
        iTmp = strStateVals.Find(_T(';'));
    }
    strVal = strStateVals;
    if(InsertStateValue(strVal, iStateIdx))
    {
        ++iCount;
    }
    return iCount;
}


//------------------------------------------------------------------*
/**
 * insert state value.
 *
 * @param           [in] crstrStateVal: state value as string
 * @param           [in] iStateIdx: 1 based state index
 * @return          successful?
 * @see             InsertStateValues()
*/
BOOL CKADTypeInfo::InsertStateValue(const CString& crstrStateVal, int iStateIdx)
{
    int iStateVal = atoi(crstrStateVal);

    if(iStateVal != 0)
    {
        m_tStateValMap.SetAt(iStateVal, iStateIdx);
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * get state from state value.
 *
 * @param           [in] iStateVal: state value
 * @param           [out] riImageId: image id
 * @param           [out] riOverlayState: overlay state or 0 if not defined
 * @see             
*/
void CKADTypeInfo::GetStateInfoFromStateVal(int iStateVal, int& riImageId, int& riOverlayState)
{
    CKADStatusData* pKADState = NULL;
    riImageId = 0;          // default is zero
    riOverlayState = 0;     // default is zero
    int iStateIdx;          // state index
    int iNumStates = m_tStateArray.GetSize();

    if(m_tStateValMap.Lookup(iStateVal, iStateIdx))
    {
        ASSERT(iStateIdx >= 0 && iStateIdx < iNumStates);
        pKADState = m_tStateArray[iStateIdx];
        ASSERT(pKADState);
        if(pKADState)
        {
            riOverlayState = pKADState->m_iOverlayState;
            riImageId = pKADState->m_iImageId;
        }
    }
    else
    {
        riImageId = m_tData.m_iImageIdx;
    }
}


