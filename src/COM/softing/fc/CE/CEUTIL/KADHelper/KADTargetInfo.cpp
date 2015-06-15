
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "KADTargetInfo.h"
#include "KADTargetData.h"
#include "KadManager.h"
#include <afxtempl.h>
#include <afxcmn.h>
#include <atlbase.h>

//----  Static Initializations:   ----------------------------------*

CKADTargetInfo::CKADTargetInfo()
{
}

//------------------------------------------------------------------*
/**
 * destructor.
 *
 *  deletes target data objects
 *
*/
CKADTargetInfo::~CKADTargetInfo()
{
    // delete type info
    POSITION        tPosition;
    CString         strTypeName;
    CKADTargetData*   pTargetData;

    tPosition = m_tTargetMap.GetStartPosition();
    while(tPosition)
    {
        m_tTargetMap.GetNextAssoc(tPosition, strTypeName, pTargetData);
        delete pTargetData;
    }
}

//------------------------------------------------------------------*
/**
 * load target info from KAD.
 *
 *  uses LoadTargetData() and LoadTargetOnlineData()
 *
 * @param           [in] pIKadMan: interface pointer to KAD-Manager
 * @return          successful?
 * @see             LoadTargetData(), LoadTargetOnlineData()
*/
BOOL CKADTargetInfo::Load(CComPtr<IUnknown>& pIKadMan)
{
    HRESULT hr;
    BOOL    bReturn = TRUE;

    CComQIPtr<ITargets>     pIKadTargets = pIKadMan;
    if(!pIKadMan)
    {
        return FALSE;
    }

    CComPtr<IEnumTargets>   pIEnumTargets;
    hr = pIKadTargets->GetTargetEnumerator(&pIEnumTargets);

    if(hr != S_OK || !pIEnumTargets)
    {
        return FALSE;
    }

    ULONG           n;
    CComPtr<ITarget>  pIKadTarget;

    hr = pIEnumTargets->Next(1, &pIKadTarget, &n);
    while (hr == S_OK && pIKadTarget)
    {
        if(!LoadTargetData(pIKadTarget))
            bReturn = FALSE;
        pIKadTarget.Release();
        hr = pIEnumTargets->Next(1, &pIKadTarget, &n);
        if(hr != S_OK)
            bReturn = FALSE;
    }
    return bReturn;
}

//------------------------------------------------------------------*
/**
 * load target data.
 *
 *  uses LoadTargetOnlineData()
 *
 * @param           [in] pITarget: interface pointer to KAD Target
 * @return          successful?
 * @see             LoadTargetOnlineData()
*/
BOOL CKADTargetInfo::LoadTargetData(CComPtr<ITarget>& pITarget)
{
    HRESULT         hr;
    CComBSTR        bstrTmp;
    CString         strType;
    BOOL            bReturn = TRUE;
    WORD            wTmp;
    CKADTargetData* pTargetData;

    hr = pITarget->GetTypeString(&bstrTmp);
    strType = bstrTmp;
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    bstrTmp.Empty();

    // create new target data object
    pTargetData = new CKADTargetData;
    hr = pITarget->GetFacilityCode(&wTmp);
    if(hr == S_OK)
    {
        pTargetData->m_lFacilityCode = wTmp;
    }
    else
    {
        bReturn = FALSE;
    }

    hr = pITarget->GetExt(&bstrTmp);
    ASSERT(hr == S_OK);
    if(hr == S_OK)
    {
        pTargetData->m_strExtension = bstrTmp;
    }

    CComPtr<ITargetOnline>  pITargetOnline;
    hr = pITarget->GetTargetOnline(&pITargetOnline);
    if(hr == S_OK && pITargetOnline)
    {
        if(!LoadTargetOnlineData(pITargetOnline, pTargetData))
        {
            bReturn = FALSE;
        }
    }
    m_tTargetMap.SetAt(strType, pTargetData);
    return bReturn;
}

//------------------------------------------------------------------*
/**
 * load target online information from KAD
 *
 * @param           - pITargetOnline* pITargetOnline - pointer to target online interface           
 * @param           - CKADTargetData* pTargetData - pointer to target online data object           
 * @return          - TRUE, if method was successful, else FALSE
*/
BOOL CKADTargetInfo::LoadTargetOnlineData(CComPtr<ITargetOnline>& pITargetOnline, CKADTargetData* pTargetData)
{
    HRESULT         hr;
    GUID            tGuidTmp;
    BOOL            bReturn = TRUE;
    BOOL            bConsoleFlag;
    BOOL            bDebugFlag;
    BOOL            bMultitaskFlag;
    BOOL            bWarmstartFlag;
    BOOL            bInitClearFlag;
    BOOL            bDLChangesFlag;

    hr = pITargetOnline->GetCustomConnect(&tGuidTmp);
    if(hr == S_OK)
    {
        pTargetData->SetCustomConnectGuid(tGuidTmp);
    }
    else
    {
        bReturn = FALSE;
    }
    
    hr = pITargetOnline->GetDownload(&tGuidTmp);
    if(hr == S_OK)
    {
        pTargetData->SetDownloadGuid(tGuidTmp);
    }

//    hr = pITargetOnline->GetDownloadChanges(&tGuidTmp);
//    if(hr == S_OK)
//    {
//        pTargetData->SetDlChangesGuid(tGuidTmp);
//    }
//
//    hr = pITargetOnline->GetDownloadSelection(&tGuidTmp);
//    if(hr == S_OK)
//    {
//        pTargetData->SetDlSelectionGuid(tGuidTmp);
//    }
    hr = pITargetOnline->GetDLChangesFlag(&bDLChangesFlag);
    if(hr == S_OK)
    {
        pTargetData->SetDLChangesFlag(bDLChangesFlag);
    }

    hr = pITargetOnline->GetConsoleFlag(&bConsoleFlag);
    if(hr == S_OK)
    {
        pTargetData->SetConsoleFlag(bConsoleFlag);
    }

    hr = pITargetOnline->GetDebugFlag(&bDebugFlag);
    if(hr == S_OK)
    {
        pTargetData->SetDebugFlag(bDebugFlag);
    }

    hr = pITargetOnline->GetMultitaskFlag(&bMultitaskFlag);
    if(hr == S_OK)
    {
        pTargetData->SetMultitaskFlag(bMultitaskFlag);
    }

    hr = pITargetOnline->GetWarmstartFlag(&bWarmstartFlag);
    if(hr == S_OK)
    {
        pTargetData->SetWarmstartFlag(bWarmstartFlag);
    }

    hr = pITargetOnline->GetInitClearFlag(&bInitClearFlag);
    if(hr == S_OK)
    {
        pTargetData->SetInitClearFlag(bInitClearFlag);
    }

    hr = pITargetOnline->GetPreDLStep(&tGuidTmp);
    if(hr == S_OK)
    {
        pTargetData->SetPreDLStepGuid(tGuidTmp);
    }

    hr = pITargetOnline->GetPostDLStep(&tGuidTmp);
    if(hr == S_OK)
    {
        pTargetData->SetPostDLStepGuid(tGuidTmp);
    }

    hr = pITargetOnline->GetPreConnectStep(&tGuidTmp);
    if(hr == S_OK)
    {
        pTargetData->SetPreConnectStepGuid(tGuidTmp);
    }
    return(bReturn);
}


//------------------------------------------------------------------*
/**
 * get target data for type.
 *
 * @param           [in] crstrType: type id string
 * @return          pointer to target data object or NULL if not found
 *
*/
CKADTargetData* CKADTargetInfo::GetTargetDataForType(const CString& crstrType)
{
    CKADTargetData* pTargetData = NULL;

    m_tTargetMap.Lookup(crstrType, pTargetData);
    return(pTargetData);
}

