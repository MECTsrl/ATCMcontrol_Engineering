
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "KADTargetData.h"

//----  Static Initializations:   ----------------------------------*

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

CKADTargetData::CKADTargetData()
{
    m_bCustomConnect    = FALSE;
    m_bDownload         = FALSE;
    m_bDlChanges        = FALSE;
    m_bDlSelection      = FALSE;
    m_bConsoleFlag      = FALSE;
    m_bDebugFlag        = TRUE;
    m_bMultitaskFlag    = TRUE;
    m_bWarmstartFlag    = TRUE;
    m_bInitClearFlag    = TRUE;
    m_bDLChangesFlag    = FALSE;
    m_bPreDLStep        = FALSE;
    m_bPostDLStep       = FALSE;
    m_bPreConnectStep   = FALSE;
}


CKADTargetData::~CKADTargetData()
{
}


void CKADTargetData::SetCustomConnectGuid(const GUID& tGuid)
{
    m_tCustomConnect = tGuid;
    m_bCustomConnect = TRUE;
}

BOOL CKADTargetData::GetCustomConnectGuid(GUID& tGuid)
{
    if(m_bCustomConnect)
    {
        tGuid = m_tCustomConnect;
    }
    return(m_bCustomConnect);
}

void CKADTargetData::SetDownloadGuid(const GUID& tGuid)
{
    m_tDownload = tGuid;
    m_bDownload = TRUE;
}

BOOL CKADTargetData::GetDownloadGuid(GUID& tGuid)
{
    if(m_bDownload)
    {
        tGuid = m_tDownload;
    }
    return(m_bDownload);
}

//void CKADTargetData::SetDlChangesGuid(const GUID& tGuid)
//{
//    m_tDlChanges = tGuid;
//    m_bDlChanges = TRUE;
//}
//
//BOOL CKADTargetData::GetDlChangesGuid(GUID& tGuid)
//{
//    if(m_bDlChanges)
//    {
//        tGuid = m_tDlChanges;
//    }
//    return(m_bDlChanges);
//}


//void CKADTargetData::SetDlSelectionGuid(const GUID& tGuid)
//{
//    m_tDlSelection = tGuid;
//    m_bDlSelection = TRUE;
//}
//
//BOOL CKADTargetData::GetDlSelectionGuid(GUID& tGuid)
//{
//    if(m_bDlSelection)
//    {
//        tGuid = m_tDlSelection;
//    }
//    return(m_bDlSelection);
//}

void CKADTargetData::SetConsoleFlag(BOOL bConsoleFlag)
{
    m_bConsoleFlag = bConsoleFlag;
}

BOOL CKADTargetData::GetConsoleFlag()
{
    return m_bConsoleFlag;
}

void CKADTargetData::SetDebugFlag(BOOL bDebugFlag)
{
    m_bDebugFlag = bDebugFlag;
}

BOOL CKADTargetData::GetDebugFlag()
{
    return m_bDebugFlag;
}

void CKADTargetData::SetMultitaskFlag(BOOL bMultitaskFlag)
{
    m_bMultitaskFlag = bMultitaskFlag;
}

BOOL CKADTargetData::GetMultitaskFlag()
{
    return m_bMultitaskFlag;
}

void CKADTargetData::SetWarmstartFlag(BOOL bWarmstartFlag)
{
    m_bWarmstartFlag = bWarmstartFlag;
}

BOOL CKADTargetData::GetWarmstartFlag()
{
    return m_bWarmstartFlag;
}

void CKADTargetData::SetInitClearFlag(BOOL bInitClearFlag)
{
    m_bInitClearFlag = bInitClearFlag;
}

BOOL CKADTargetData::GetInitClearFlag()
{
    return m_bInitClearFlag;
}

void CKADTargetData::SetDLChangesFlag(BOOL bDLChangesFlag)
{
    m_bDLChangesFlag = bDLChangesFlag;
}

BOOL CKADTargetData::GetDLChangesFlag()
{
    return m_bDLChangesFlag;
}


void CKADTargetData::SetPreDLStepGuid(const GUID& tGuid)
{
    m_tPreDLStep = tGuid;
    m_bPreDLStep = TRUE;
}

BOOL CKADTargetData::GetPreDLStepGuid(GUID& tGuid)
{
    if(m_bPreDLStep)
    {
        tGuid = m_tPreDLStep;
    }
    return(m_bPreDLStep);
}

void CKADTargetData::SetPostDLStepGuid(const GUID& tGuid)
{
    m_tPostDLStep = tGuid;
    m_bPostDLStep = TRUE;
}

BOOL CKADTargetData::GetPostDLStepGuid(GUID& tGuid)
{
    if(m_bPostDLStep)
    {
        tGuid = m_tPostDLStep;
    }
    return(m_bPostDLStep);
}

void CKADTargetData::SetPreConnectStepGuid(const GUID& tGuid)
{
    m_tPreConnectStep = tGuid;
    m_bPreConnectStep = TRUE;
}

BOOL CKADTargetData::GetPreConnectStepGuid(GUID& tGuid)
{
    if(m_bPreConnectStep)
    {
        tGuid = m_tPreConnectStep;
    }
    return(m_bPreConnectStep);
}
