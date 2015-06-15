
#ifndef __TARGETSET_H_
#define __TARGETSET_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "KADHelper\KADTargetInfo.h"
#include "download.h"
#define MFCMAPHASHKEY
#include "mfcmap.h"

//----  Forward Class Definitions:   -------------------------------*
class CDownload;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CTargetOnlBase;

class CTargetSet : public CObject
{
    CKADTargetInfo m_Info;
public:
    CTargetSet();

    CTargetOnlBase* AddTarget(  const CString& crstrId,
                                const CString& crstrIdPath,
                                const CString& crstrType,
                                const CString& crstrAssignedResource,
                                const CString& crstrAddr = _T(""),
                                const CString& crstrUserAddr = _T(""),
                                BOOL bNoDebugTarget = FALSE);

    BOOL RemoveTarget (const CString &strId);
    BOOL RemoveAllTargets ();

    CTargetOnlBase *GetTarget (const CString &strId);
    CDownload* GetDownloader(const CString &strId);

    POSITION GetFirstTargetPosition();
    CTargetOnlBase *GetNextTarget (POSITION &pPosition);

    BOOL RenameTarget (const CString &strId, const CString &strNewId);

    BOOL DisconnectAllTargets();

    HRESULT LoadFromKAD();

    // Helper functions for main menu and toolbar actions
    int GetTargetCount();
    int GetTargetCountDisconnected();
    int GetTargetCountConnected();
    int GetTargetCountDisconnectedEx();
    int GetTargetCountConnectedEx();
    int GetTargetCountWarmstartReady();
    int GetTargetCountInitClearReady();
    int GetTargetCountDLChanges();

    int GetAllTargetIds(CStringArray& rastrTargetIds);
    int GetTargetIdsDisconnected(CStringArray& rastrTargetIds);
    int GetTargetIdsConnected(CStringArray& rastrTargetIds);
    int GetTargetIdsDisconnectedEx(CStringArray& rastrTargetIds);
    int GetTargetIdsConnectedEx(CStringArray& rastrTargetIds);
    int GetTargetIdsDLChanges(CStringArray& rastrTargetIds);

protected:
    CMap<CString, LPCTSTR, CTargetOnlBase*, CTargetOnlBase*> m_Targets;
    CMFCObjMan<CString,CDownload> m_DownloadList;
private: 
    void AddDownloader(const CString& crstrId, const CString& strType);
    void RemoveDownloader(const CString& crstrId);
};

#endif // __TARGETSET_H_

