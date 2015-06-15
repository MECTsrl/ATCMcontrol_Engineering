


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "cecompman.h"
#include "utilif.h"
#include "cemainerror.h"
#include "TargetSet.h"
#include "TargetOnlBase.h"
#include "TargetOnl.h"
#include "KADHelper\KADTargetData.h"
#include "mainfrm.h"

CTargetSet::CTargetSet() : m_Targets(), m_Info() , m_DownloadList()
{
}

//------------------------------------------------------------------*
/**
 * add target.
 *
 * @param            [in] crstrId: id of target
 * @param            [in] crstrIdPath: id path of target in instance view
 * @param            [in] crstrType: target type (KAD type)
 * @param            [in] crstrAssignedResource: id of assigned resource
 * @param            [in] crstrAddr: control address string used to connect
 * @return           pointer to newly created target online object
 * @see              
*/
CTargetOnlBase* CTargetSet::AddTarget
(
    const CString& crstrId,
    const CString& crstrIdPath,
    const CString& crstrType,
    const CString& crstrAssignedResource,
    const CString& crstrAddr /*=""*/,
    const CString& crstrUserAddr /*=""*/,
    BOOL bNoDebugTarget /*=FALSE*/
)
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        CString strStatusText;
        strStatusText.Format(IDS_STATUS_ADD_TARGET, crstrId);
        pMainFrame->UpdateStatusBar(strStatusText);
    }

    CString strIdUpper = crstrId;
    CCEStateManager*    pStateManager;
    CTargetOnlBase*     pTarget = NULL;

    if(bNoDebugTarget)
    {
        pTarget = new CTargetOnlBase(crstrId, crstrIdPath, crstrType, crstrAssignedResource);
    }
    else
    {
        pTarget = new CTargetOnl(crstrId, crstrIdPath, crstrType, crstrAssignedResource);
    }

    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        return (pTarget);
    }
    
    pTarget->SetControlAddress(crstrAddr);
    pTarget->SetUserControlAddress(crstrUserAddr);
    //set the address in the wago object
    strIdUpper.MakeUpper();
    m_Targets.SetAt (strIdUpper, pTarget);

    AddDownloader(crstrId,crstrType);

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerAddTarget(pTarget);
        //TODO trigger Downloader creation
    }
    if (pMainFrame != NULL)
    {
        pMainFrame->UpdateStatusBar(_T(""));
    }
    return(pTarget);
}


//------------------------------------------------------------------*
/**
 * remove target.
 *
 * @param           [in] crstrId: id of target (name)
 * @return          successfuly removed?
 * @see             
*/
BOOL CTargetSet::RemoveTarget (const CString& crstrId)
{
    CString strIdUpper;
    CTargetOnlBase* pTarget = NULL;
    CCEStateManager* pStateManager;
    HRESULT hr;

    strIdUpper = crstrId;
    strIdUpper.MakeUpper ();

    if (!m_Targets.Lookup (strIdUpper, pTarget))
    {
        return (FALSE);
    }

    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        return (FALSE);
    }


    if(pTarget->IsConnectedExclusive())
    {
        CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        hr = pTarg->DisconnectExclusive();
        // if not successful, force disconnect
        if(hr != S_OK)
        {
            hr = pTarg->DisconnectExclusive(TRUE);
        }
    }

    if(pTarget->IsConnected())
    {
        hr = pTarget->Disconnect();
    }

    m_Targets.RemoveKey (strIdUpper);
    RemoveDownloader(crstrId);

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerRemoveTarget (pTarget);
    }

    delete pTarget;
    return (FALSE);
}


//------------------------------------------------------------------*
/**
 * remove all targets.
 *
 * @return          successfully removed all targets?
 * @see             
*/
BOOL CTargetSet::RemoveAllTargets ()
{
    POSITION    pPosition;
    BOOL        bReturn = TRUE;

    while ((pPosition = m_Targets.GetStartPosition ()) != NULL)
    {
        CString strId;
        CTargetOnlBase *pTarget;

        m_Targets.GetNextAssoc (pPosition, strId, pTarget);
        if(!RemoveTarget (strId))
        {
            bReturn = FALSE;
        }
    }
    return (bReturn);
}


//------------------------------------------------------------------*
/**
 * get target with certain id (name).
 *
 * @param           [in] strId: id of target
 * @return          pointer to target online object
 * @see             
*/
CTargetOnlBase *CTargetSet::GetTarget (const CString &strId)
{
    CString strIdUpper;
    CTargetOnlBase *pTarget;

    strIdUpper = strId;
    strIdUpper.MakeUpper ();

    if (!m_Targets.Lookup (strIdUpper, pTarget))
    {
        return (NULL);
    }

    ASSERT (pTarget != NULL);
    return (pTarget);
}


//------------------------------------------------------------------*
/**
 * get first target position.
 *
 * @return          position of first target
 * @see             GetNextTarget()
*/
POSITION CTargetSet::GetFirstTargetPosition()
{
    return (m_Targets.GetStartPosition ());
}


//------------------------------------------------------------------*
/**
 * get next target.
 *
 * @param           [in] rPosition: position from where to start
 * @return          pointer to target online object or NULL if not found
 * @see             
*/
CTargetOnlBase *CTargetSet::GetNextTarget(POSITION& rPosition)
{
    CString strId;
    CTargetOnlBase *pTarget;

    m_Targets.GetNextAssoc(rPosition, strId, pTarget);
    return (pTarget);
}


//------------------------------------------------------------------*
/**
 * rename target online object.
 *
 * @param           [in] strId: old id of target
 * @param           [in] strNewId: new id of target
 * @return          successful?
 * @see             
*/
BOOL CTargetSet::RenameTarget (const CString &strId, const CString &strNewId)
{
    CString strIdUpper;
    CString strNewIdUpper;
    CString strType;
    CTargetOnlBase *pTarget;
    CDownload* pDL = NULL;
    CCEStateManager *pStateManager;

    strIdUpper = strId;
    strIdUpper.MakeUpper ();

    if (!m_Targets.Lookup (strIdUpper, pTarget))
    {
        return (FALSE);
    }
    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
    {
        return (FALSE);
    }
    m_Targets.RemoveKey (strIdUpper);
    strNewIdUpper = strNewId;
    strNewIdUpper.MakeUpper ();
    CString strTemp=strNewId;
    pTarget->SetId(strTemp);
    m_Targets.SetAt(strNewIdUpper, pTarget);
    strType = pTarget->GetType();

    RemoveDownloader(strId);
    AddDownloader(strId,strType);

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerRenameTarget (pTarget, strId);
    }
    return(TRUE);
}



//------------------------------------------------------------------*
/**
 * Loads target type information into CKADTargetInfo m_Info member.
 * Used to associate the downloader GUID or custom connect GUID.
 * @return			HRESULT - E_CECOMPMAN_COMPONENT_NOT_ALIVE
 *                  - KAD Manager not loaded with component manager.
*/
HRESULT CTargetSet::LoadFromKAD()
{
    HRESULT hr = S_OK;
    CComPtr<IUnknown> pKADManager;

    pKADManager.Attach(CEGetCOM(CE_COMPMAN_KADMAN));
    if(!pKADManager) {
        return E_CECOMPMAN_COMPONENT_NOT_ALIVE;
    }

    m_Info.Load(pKADManager);
    return S_OK;
}




//------------------------------------------------------------------*
/**
 * Creates and adds a CDownload downloader wrapper class to the 
 * map of downloader objects of the target set. Each CDownload
 * object will be identified by the target instance name passed with
 * argument crstrId. The target type will be used to get the target
 * type specific downloader GUID from the target type information
 * (m_Info) in order to create the associated downloader component.
 * No downloader object will be created if no target type data has
 * been loaded with LoadFromKAD(), if the downloader GUID is a NULL_GUID
 * or in case of an out of memory situation.
 * @param			const CString& crstrId - target instance name
 * @param			const CString& strType - target type from KAD.
 * @see	    RemoveDownloader(), LoadFromKAD()		
*/
void CTargetSet::AddDownloader(const CString& crstrId, const CString& strType)
{
    CDownload* pDL = NULL;
    CString str;
    CGuid   guid;
    CKADTargetData* pData = m_Info.GetTargetDataForType(strType);

    if(!pData) {
        UTIL_FormatMessage(AfxGetResourceHandle(),str.GetBuffer(_MAX_PATH),
            _MAX_PATH,E_CESYSTEM_NO_TARGETDATA,crstrId,strType);
        str.ReleaseBuffer();
        CESystemMessage(str);
        return;
    }

    pData->GetDownloadGuid(guid);

    if(guid.IsEmpty()) {
        //No downloader GUID for the target type in the 4CE KAD
        UTIL_FormatMessage(AfxGetResourceHandle(),str.GetBuffer(_MAX_PATH),
            _MAX_PATH,E_CESYSTEM_INV_DOWNLOADER_GUID,(LPCTSTR) guid,strType);
        str.ReleaseBuffer();
        CESystemMessage(str);
        return;
    }

    
    str = crstrId;
    pDL = new CDownload(guid,str);
    if(!pDL) {
        return;
    }

    // SIS: TODO preliminary
    pDL->SetConsoleFlag(pData->GetConsoleFlag());
    
    str.MakeUpper();
    //Initial reference count after creation is 1 due to CCmdTarget base class
    //Hence add the 1st reference to the the list
    m_DownloadList.Assoc(str,pDL);
    return;
}



//------------------------------------------------------------------*
/**
 * Removes CDownload downloader wrapper class from the map of downloader 
 * objects of the target set. The CDownload object will be identified by
 * the target instance name. Calls CDownload::Destroy() to destroy the 
 * the associated downloader component prior to removal.
 * @param	const CString& crstrId - target instance name
 * @see		AddDownloader()
*/
void CTargetSet::RemoveDownloader(const CString& crstrId)
{
    CString str = crstrId;
    str.MakeUpper();
    CDownload* pDL = m_DownloadList.Get(str);
    if(pDL) {
        pDL->Destroy();
        m_DownloadList.Forget(str);
    }

    FC_RELEASE_PTR(pDL);
}

//------------------------------------------------------------------*
/**
 * Gets the CDownload downloader wrapper class from the map of downloader 
 * objects of the target set. The CDownload object will be identified by
 * the target instance name. If a CDownload object is returned it has an
 * additional reference.<br><b>The caller must call CDownload::Release() to
 * release the returned reference.</b>
 * @param			const CString &strId - target instance name
 * @return			CDownload* object or NULL, if no CDownload object can
 *                  be identified by by the target instance name.
 * @see	            RemoveDownloader(),AddDownloader()		
*/
CDownload* CTargetSet::GetDownloader(const CString &strId)
{
    CString strIdUpper=strId;
    strIdUpper.MakeUpper();
    return m_DownloadList.Get(strIdUpper);
}


//------------------------------------------------------------------*
/**
 * get number of target online objects.
 *
 * @return          number of target online objects
*/
int CTargetSet::GetTargetCount()
{
    return(m_Targets.GetCount());
}

//------------------------------------------------------------------*
/**
 * get target count of all targets that are disconnected.
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountDisconnected()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(!pTargetOnl->IsConnected())
        {
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target count of all targets that are connected.
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountConnected()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsConnected())
        {
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target count of all targets that are not exclusively connected.
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountDisconnectedEx()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(!pTargetOnl->IsConnectedExclusive())
        {
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target count of all targets that are exclusively connected.
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountConnectedEx()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsConnectedExclusive())
        {
            ++iCount;
        }
    }
    return iCount;
}


//------------------------------------------------------------------*
/**
 * get target count of all targets that can be warmstarted.
 *
 *  this are all targets that are connected and warmstart enabled
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountWarmstartReady()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsWarmstartEnabled() && pTargetOnl->IsConnected())
        {
            ++iCount;
        }
    }
    return iCount;
}


//------------------------------------------------------------------*
/**
 * get target count of all targets that can be initialized / cleared.
 *
 *  this are all targets that are connected exclusive and init / clear enabled
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountInitClearReady()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsInitClearEnabled() && pTargetOnl->IsConnectedExclusive())
        {
            ++iCount;
        }
    }
    return iCount;
}


//------------------------------------------------------------------*
/**
 * get target count of all targets that are download changes enabled.
 *
 * @return          number of targets
*/
int CTargetSet::GetTargetCountDLChanges()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsDLChangesEnabled())
        {
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get all target ids (names).
 *
 * @param           [out] rastrTargetIds: target ids
 * @return          number of target ids returned
*/
int CTargetSet::GetAllTargetIds(CStringArray& rastrTargetIds)
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        rastrTargetIds.Add(strTargetId);
        ++iCount;
    }
    return iCount;
}


//------------------------------------------------------------------*
/**
 * disconnect all targets for shutdown.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CTargetSet::DisconnectAllTargets()
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    HRESULT         hr;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl)
        {
            if(pTargetOnl->IsConnectedExclusive())
            {
                hr = pTargetOnl->DisconnectExclusive();
                if(hr == S_FALSE)
                {
                    return FALSE;
                }
                if(hr != S_OK)
                {
                    hr = pTargetOnl->DisconnectExclusive(TRUE);
                    if(hr != S_OK)
                    {
                        return FALSE;
                    }
                }
            }
            if(pTargetOnl->IsConnected())
            {
                hr = pTargetOnl->Disconnect();
                if(hr != S_OK)
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get target ids of all targets that are disconnected.
 *
 * @param           [out] rastrTargetIds: target ids
 * @return          number of target ids returned
*/
int CTargetSet::GetTargetIdsDisconnected(CStringArray& rastrTargetIds)
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(!pTargetOnl->IsConnected())
        {
            rastrTargetIds.Add(strTargetId);
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target ids of all targets that are connected.
 *
 * @param           [out] rastrTargetIds: target ids
 * @return          number of target ids returned
*/
int CTargetSet::GetTargetIdsConnected(CStringArray& rastrTargetIds)
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsConnected())
        {
            rastrTargetIds.Add(strTargetId);
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target ids of all targets that are not exclusively connected.
 *
 * @param           [out] rastrTargetIds: target ids
 * @return          number of target ids returned
*/
int CTargetSet::GetTargetIdsDisconnectedEx(CStringArray& rastrTargetIds)
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(!pTargetOnl->IsConnectedExclusive())
        {
            rastrTargetIds.Add(strTargetId);
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target ids of all targets that are exclusively connected.
 *
 * @param           [out] rastrTargetIds: target ids
 * @return          number of target ids returned
*/
int CTargetSet::GetTargetIdsConnectedEx(CStringArray& rastrTargetIds)
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsConnectedExclusive())
        {
            rastrTargetIds.Add(strTargetId);
            ++iCount;
        }
    }
    return iCount;
}

//------------------------------------------------------------------*
/**
 * get target ids of all targets that can perform a "download changes" operation.
 *
 * @param           [out] rastrTargetIds: target ids
 * @return          number of target ids returned
*/
int CTargetSet::GetTargetIdsDLChanges(CStringArray& rastrTargetIds)
{
    CString         strTargetId;
    CTargetOnlBase* pTargetOnl;
    int             iCount = 0;

    POSITION    pos = m_Targets.GetStartPosition();
    while(pos)
    {
        m_Targets.GetNextAssoc(pos, strTargetId, pTargetOnl);
        if(pTargetOnl->IsDLChangesEnabled())
        {
            rastrTargetIds.Add(strTargetId);
            ++iCount;
        }
    }
    return iCount;
}
