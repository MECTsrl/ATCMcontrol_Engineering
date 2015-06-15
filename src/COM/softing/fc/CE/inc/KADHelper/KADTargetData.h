#ifndef __KADTARGETDATA_H_
#define __KADTARGETDATA_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CKADTargetData - class for KAD-target data
 *
 *  @see CKADTypeInfo
 */
class CKADTargetData
{
public:
    CKADTargetData();
    ~CKADTargetData();

    // set xxx guid: creates a new GUID and make a copy
    void SetCustomConnectGuid(const GUID& tGuid);
    void SetDownloadGuid(const GUID& tGuid);
//    void SetDlChangesGuid(const GUID& tGuid);
//    void SetDlSelectionGuid(const GUID& tGuid);

    BOOL GetCustomConnectGuid(GUID& tGuid);
    BOOL GetDownloadGuid(GUID& tGuid);
//    BOOL GetDlChangesGuid(GUID& tGuid);
//    BOOL GetDlSelectionGuid(GUID& tGuid);

    void SetConsoleFlag(BOOL bConsoleFlag);
    BOOL GetConsoleFlag();

    void SetDLChangesFlag(BOOL bDLChangesFlag);
    BOOL GetDLChangesFlag();

    void SetDebugFlag(BOOL bDebugFlag);
    BOOL GetDebugFlag();
    void SetMultitaskFlag(BOOL bMultitaskFlag);
    BOOL GetMultitaskFlag();
    void SetWarmstartFlag(BOOL bWarmstartFlag);
    BOOL GetWarmstartFlag();
    void SetInitClearFlag(BOOL bInitClearFlag);
    BOOL GetInitClearFlag();

    void SetPreDLStepGuid(const GUID& tGuid);
    BOOL GetPreDLStepGuid(GUID& tGuid);

    void SetPostDLStepGuid(const GUID& tGuid);
    BOOL GetPostDLStepGuid(GUID& tGuid);

    void SetPreConnectStepGuid(const GUID& tGuid);
    BOOL GetPreConnectStepGuid(GUID& tGuid);

public:
    CString m_strType;          // target type
    CString m_strExtension;     // extension
    long    m_lFacilityCode;    // facility code

protected:
    GUID    m_tCustomConnect;   // handler for custom connection
    BOOL    m_bCustomConnect;   // is handler set?
    
    GUID    m_tDownload;        // handler for download
    BOOL    m_bDownload;        // is handler set?
    
    GUID    m_tDlChanges;       // handler for download changes
    BOOL    m_bDlChanges;       // is handler set?
    
    GUID    m_tDlSelection;     // handler for download selection
    BOOL    m_bDlSelection;     // is handler set?
    
    BOOL    m_bConsoleFlag;     // target with console?
    BOOL    m_bDebugFlag;       // target is debug enabled?
    BOOL    m_bMultitaskFlag;   // target is multi task enabled?
    BOOL    m_bWarmstartFlag;   // target is warmstart enabled?
    BOOL    m_bInitClearFlag;   // target is init / clear enabled?
    BOOL    m_bDLChangesFlag;   // target is download changes enabled?

    GUID    m_tPreDLStep;       // pre download step handler
    BOOL    m_bPreDLStep;       // is pre download step handler set?

    GUID    m_tPostDLStep;      // post download step handler
    BOOL    m_bPostDLStep;      // is post download step handler set?

    GUID    m_tPreConnectStep;  // pre connect step handler
    BOOL    m_bPreConnectStep;  // is pre connect step handler set?
};



#endif // __KADTARGETDATA_H_
