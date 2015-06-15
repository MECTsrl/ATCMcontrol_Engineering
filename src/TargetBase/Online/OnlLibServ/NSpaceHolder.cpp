/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/NSpaceHolder.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: NSpaceHolder.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/NSpaceHolder.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include <gensrv.h>
#include <genItem.h>
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "NSpaceHolder.h"

#include "iecConfiguration.h"
#include "iecResource.h"
#include "srtResource.h"
#include "conResource.h"
#include "iecVar.h"
#include "creator.h"
#include "OPCerror.h"
#include "BuildNr.h"
#include "Product.h"
#include "connection.h"
#include "ControlMgt.h"

#include <genFunctions.h>
#include "opcDynGroup.h"

//----  Globals:    ------------------------------------------------*
//CComAutoCriticalSection     tagCS;

//----  Local Defines:   -------------------------------------------*
const TCHAR g_szSystemFolderName[] = _T("__system");

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----  Globals:    ----------------------------------------------------------*
static NameSpaceHolder    *g_theNamespace = NULL;
CCriticalSection    g_csNamespace;

CSysVarPool     *NameSpaceHolder::sm_pGlobalSysVar = NULL;
CSysVarPool     *NameSpaceHolder::sm_pGlobalSecuritySysVar = NULL;
CSysVarPool     *NameSpaceHolder::sm_pConsoleDLoadSysVar = NULL;

int             NameSpaceHolder::sm_cntResource = 0;
CSysVarPool     *NameSpaceHolder::sm_pResSysVar[MAX_RESOURCE];
CSysVarPool     *NameSpaceHolder::sm_pResDLoadSysVar[MAX_RESOURCE];
CSysVarPool     *NameSpaceHolder::sm_pResDebugSysVar[MAX_RESOURCE];

CCriticalSection NameSpaceHolder::sm_csDynRes;
BranchList      NameSpaceHolder::sm_dynResList;


//TCHAR            NameSpaceHolder::sm_cSeparator = _T('.');
TCHAR           NameSpaceHolder::sm_cStdSeparatorChar  = _T('.');
LPCTSTR         NameSpaceHolder::sm_pszSeparatorList = _T(".!\\/");


DWORD               NameSpaceHolder::sm_dwItemsPerSecond = 0;


//------------------------------------------------------------------*
/**
 * get global namespace, protected by critical section
 *  this call increments the ref. count, so caller must release
 *
 * @param           pNamespace
 * @return          TRUE if any namespace present; FALSE on error
 * @exception       -
*/
BOOL
    GetGlobalNamespace(NameSpaceHolder **ppNamespace)
{
    if (ppNamespace == NULL) {
        return FALSE;       // ptr is null, cannot return anything
    }
    *ppNamespace = NULL;
//    TRACE(_T("GetGlobalNamespace: aquire namespace lock...\n"));
    CSingleLock     lock(&g_csNamespace, TRUE);
    if (g_theNamespace != NULL) {
        g_theNamespace->addRef();
        *ppNamespace = g_theNamespace;
//        TRACE(_T("GetGlobalNamespace: got namespace lock...\n"));
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * set new global namespace, protected by critical section
 *
 * @param           pNewNamespace
 * @return          TRUE if any namespace present; FALSE on error
 * @exception       -
*/
BOOL
    SetGlobalNamespace(NameSpaceHolder *pNewNamespace)
{
    NameSpaceHolder *pOld = NULL;
    {
        // lock temporarily previous namespace
//        TRACE(_T("SetGlobalNamespace: aquire lock to old namespace...\n"));
        CSingleLock     lock(&g_csNamespace, TRUE);
        pOld = g_theNamespace;
    }
    if (pOld != NULL) {
        TRACE(_T("Switching namespace old = 0x%08x, new = 0x%08x...\n"), pOld, pNewNamespace);
        pOld->MarkAllTags(OPC_QUALITY_BAD | OPC_QUALITY_OUT_OF_SERVICE, TRUE);
        // since lock is removed, have a chance to clean up old namespace
        TRACE(_T("SetGlobalNamespace: releasing old namespace ...\n"));
        pOld->release();    // this eventually also closes down namespace thread
    }
    // now set new namespace
    {
        TRACE(_T("SetGlobalNamespace: aquire lock to set new namespace...\n"));
        CSingleLock     lock(&g_csNamespace, TRUE);
        g_theNamespace = pNewNamespace;
        TRACE(_T("SetGlobalNamespace: set new namespace...\n"));
        if (pNewNamespace != NULL) {
            pNewNamespace->addRef();
        }
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * CreateSystemVariables
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    NameSpaceHolder::CreateSystemVariables(void)
{
    HRESULT hr = S_OK;
    CSysVar  *pVar = NULL;
    CSysVarPool *pPool = NULL;

    // create namespaces for config and resource system variables
    // a) create global system variables:
    pPool = new CSysVarPool(_T(""), g_csSystem, CSysVarPool::configGlobal);
    if (pPool != NULL)
    {
        sm_pGlobalSysVar = pPool;
        pVar = new CSysVarRegistry(g_csSVarProjectName);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarRes0Name);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
    }

    // c) create global security system variables:
    pPool = new CSysVarPool(g_csSVarSecSecurity, g_csSVarSecSecurity, CSysVarPool::configGlobal, 0);
    if (pPool != NULL)
    {
        sm_pGlobalSecuritySysVar = pPool;
        pVar = new CSysVarSecurity(g_csSVarSecurityIsEnabled, CSysVarSecurity::isEnabled);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarSecurity(g_csSVarSecurityPassword, CSysVarSecurity::Password);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarSecurity(g_csSVarSecurityRights, CSysVarSecurity::Rights);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
    }
    
    // g) create console system variables:
    pPool = new CSysVarPool(g_csSVarSecDLoad, g_csSVarSecDLoad, CSysVarPool::consoleGlobal, 0);
    if (pPool != NULL)
    {
        sm_pConsoleDLoadSysVar = pPool;
        pVar = new CSysVarRegistry(g_csSVarDate);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarUser);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarHost);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarPrjGUID);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarDLoadGUID);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarCleared);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
    }
    return hr;
}


//------------------------------------------------------------------*
/**
 * CreateResourceSystemVariables
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    NameSpaceHolder::CreateResourceSystemVariables(int iEnumRes)
{
    HRESULT hr = S_OK;
    CSysVar  *pVar = NULL;
    CSysVarPool *pPool = NULL;
    
    if (iEnumRes >= MAX_RESOURCE) {
        return E_INVALIDARG;
    }
    sm_cntResource++;
    
    // d) create general resource system variables:
    pPool = new CSysVarPool(_T(""), g_csSystem, CSysVarPool::srtGlobal, 0);
    if (pPool != NULL)
    {
        assert(sm_pResSysVar[iEnumRes] == NULL);
        sm_pResSysVar[iEnumRes] = pPool;
        pVar = new CSysVarResource(g_csResourceState, CSysVarResource::state);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();

        pVar = new CSysVarResource(g_csDownloadCounter, CSysVarResource::dnload);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
    }
    
    // e) create resource download system variables:
    pPool = new CSysVarPool(g_csSVarSecDLoad, g_csSVarSecDLoad, CSysVarPool::srtGlobal, 0);
    if (pPool != NULL)
    {
        assert(sm_pResDLoadSysVar[iEnumRes] == NULL);
        sm_pResDLoadSysVar[iEnumRes] = pPool;
        pVar = new CSysVarRegistry(g_csSVarDate);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarUser);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarHost);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarPrjGUID);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarDLoadGUID);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarRegistry(g_csSVarCleared);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
    }
    
    // f) create resource debug system variables:
    pPool = new CSysVarPool(g_csSVarSecDebug, g_csSVarSecDebug, CSysVarPool::srtGlobal, 0);
    if (pPool != NULL)
    {
        assert(sm_pResDebugSysVar[iEnumRes] == NULL);
        sm_pResDebugSysVar[iEnumRes] = pPool;
        pVar = new CSysVarDebug(g_csSVarDebugIsEnabled, CSysVarDebug::isEnabled);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarDebug(g_csSVarDebugStartedDate, CSysVarDebug::startedDate);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarDebug(g_csSVarDebugUser, CSysVarDebug::currUser);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
        
        pVar = new CSysVarDebug(g_csSVarDebugHost, CSysVarDebug::currHost);
        if (pVar == NULL) return E_OUTOFMEMORY;
        pPool->AddSysVar(pVar);
        pVar->release();
    }
    return hr;
}


//------------------------------------------------------------------*
/**
* DeleteResourceSystemVariables
*
* @param           
* @return          
* @exception       -
* @see             
*/
HRESULT
    NameSpaceHolder::DeleteResourceSystemVariables(int iEnumRes)
{
    if (iEnumRes >= sizeof(sm_pResSysVar)) {
        return E_INVALIDARG;
    }
    if (sm_cntResource > 0) {
        sm_cntResource--;
    }
    if (sm_pResSysVar[iEnumRes] != NULL) {
        sm_pResSysVar[iEnumRes]->removeAllLeafs();
        sm_pResSysVar[iEnumRes]->removeAllBranches();
        sm_pResSysVar[iEnumRes]->release();
        sm_pResSysVar[iEnumRes] = NULL;
    }
    if (sm_pResDLoadSysVar[iEnumRes] != NULL) {
        sm_pResDLoadSysVar[iEnumRes]->removeAllLeafs();
        sm_pResDLoadSysVar[iEnumRes]->removeAllBranches();
        sm_pResDLoadSysVar[iEnumRes]->release();
        sm_pResDLoadSysVar[iEnumRes] = NULL;
    }
    if (sm_pResDebugSysVar[iEnumRes] != NULL) {
        sm_pResDebugSysVar[iEnumRes]->removeAllLeafs();
        sm_pResDebugSysVar[iEnumRes]->removeAllBranches();
        sm_pResDebugSysVar[iEnumRes]->release();
        sm_pResDebugSysVar[iEnumRes] = NULL;
    }
    return S_OK;
}
    
//------------------------------------------------------------------*
/**
 * DeleteSystemVariables
 *
 * @param           
 * @return          
 * @exception       -
*/
void 
    NameSpaceHolder::DeleteSystemVariables()
{
    if (sm_pGlobalSysVar != NULL) {
        sm_pGlobalSysVar->removeAllLeafs();
        sm_pGlobalSysVar->removeAllBranches();
        sm_pGlobalSysVar->release();
        sm_pGlobalSysVar = NULL;
    }
    if (sm_pGlobalSecuritySysVar != NULL) {
        sm_pGlobalSecuritySysVar->removeAllLeafs();
        sm_pGlobalSecuritySysVar->removeAllBranches();
        sm_pGlobalSecuritySysVar->release();
        sm_pGlobalSecuritySysVar = NULL;
    }
    if (sm_pConsoleDLoadSysVar != NULL) {
        sm_pConsoleDLoadSysVar->removeAllLeafs();
        sm_pConsoleDLoadSysVar->removeAllBranches();
        sm_pConsoleDLoadSysVar->release();
        sm_pConsoleDLoadSysVar = NULL;
    }
}



/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS NameSpaceHolder
/////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
NameSpaceHolder::NameSpaceHolder()
  : GenericObject(GENOBJ_TYPE_NS_ROOT)      // %TBD% ok ????
{
    m_CheckRate = 5000;

    m_pEngine = ::getOPCEngine();
    assert(!!m_pEngine);

    sm_cStdSeparatorChar = m_pEngine->getTreeDelimiter(&sm_pszSeparatorList, NULL);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
NameSpaceHolder::~NameSpaceHolder()
{
    Cleanup();
}


//------------------------------------------------------------------*
/**
 * InitInstance
 *
 * @param           void
 * @return          HRESULT
 * @exception       -
*/
HRESULT
    NameSpaceHolder::InitInstance()
{
    HRESULT hr = S_OK;
    for (int i = 0; i < MAX_RESOURCE; i++)
    {
        sm_pResSysVar[i] = NULL;
        sm_pResDLoadSysVar[i] = NULL;
        sm_pResDebugSysVar[i] = NULL;
    }
    assert(!!m_pEngine);
    if (!m_pEngine) {
        return E_FAIL;
    }
    GenericPointer<GenericCreator> creator = new Creator(m_workEvent, &m_readList, &m_writeList, &m_readListLock, &m_writeListLock);
    if (!creator) {
        return E_OUTOFMEMORY;
    }
    m_pEngine->setCreator(creator);
    m_pEngine->setCheckParameter(3*60*1000); //check connections every 3 minutes

    //-----------       Prepare Configuration
    CSC_InitHashTables();

    //-----------       Prepare security
    hr = g_security.InitSecurityInfo();
    if (FAILED(hr)) return hr;
    
    hr = CreateSystemVariables();
    if (FAILED(hr)) return hr;

    if (!CreateAndConnectNamespace(_T(""))) {
        return E_FAIL;
    }

    // set vendor specific OPC server info
    m_pEngine->setVersionInfo(PROD_VER_MAJOR, PROD_VER_MINOR, PROD_VER_BUILD);
    CString vendor;
    vendor.LoadString(IDS_VENDOR_INFO);
    m_pEngine->setVendorInfo(vendor);

    hr = createDeviceThread();
    if (FAILED(hr)) return hr;

    // now get OPC server running:
    m_pEngine->initialize(OPCEngine::OutProc, CLSID_OPCServer, IDR_DUMMY_REGSCRIPT, _Module.GetModuleInstance(), COINIT_MULTITHREADED);
    //m_pEngine->start();

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * StartEngine
 *
 * @param           void
 * @return          HRESULT
 * @exception       -
*/
HRESULT
    NameSpaceHolder::StartEngine()
{
    m_pEngine->start();
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * ExitInstance
 *
 * @param           void
 * @return          HRESULT
 * @exception       -
*/
HRESULT
    NameSpaceHolder::ExitInstance()
{
    m_pEngine->stop();      // withdraw from COM

	m_endEvent.SetEvent();
    WaitForSingleObject(m_deviceThread, 30000);
    CloseHandle(m_deviceThread);
    m_deviceThread = NULL;

    CSingleLock lock(&sm_csDynRes, TRUE);
    while (!sm_dynResList.IsEmpty())
    {
        GenericBranch* pIecRes = sm_dynResList.RemoveHead();
        if (pIecRes) {
            pIecRes->removeAllLeafs();
            pIecRes->removeAllBranches();
            pIecRes->release();
        }
    }
    lock.Unlock();

    assert(!!m_pEngine);
    if (!!m_pEngine) {
        GenericPointer<OPCRootNS> nsroot;
        nsroot = m_pEngine->getNamespaceRoot();
        // remove all tags
        nsroot->removeAllLeafs();		
        // remove all nodes
        nsroot->removeAllBranches();	
        m_pEngine->terminate();
    }

    DeleteSystemVariables();
    int cnt = sm_cntResource;
    for (int i = 0; i < cnt; i++) {
        DeleteResourceSystemVariables(i);
    }

    CSC_CleanupHashTables();
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * create and connect local namespace from information of 4C control in given configuration
 *
 * @param           pszConfiguration    name of configuration to look for control
 *                          if empty string, look on local machine
 * @return          TRUE if found and load; FALSE on error
 * @exception       -
*/
BOOL
    NameSpaceHolder::CreateAndConnectNamespace(LPCTSTR pszConfiguration)
{
    TRACE(_T("Reconnect namespace with control...\n"));

    HRESULT hr;
    // start building namespace:
    // CD2: we currently have a simple 1:1 mapping of config- to machine-name!!!
    // later I must retreive real IEC config name from downloaded domains !

    //-----------       Create global system variables as static OPC namespace :
    assert(!!m_pEngine);
	GenericPointer<OPCRootNS> pRoot;
	pRoot = m_pEngine->getNamespaceRoot();

    // add configuration global system variables:
    CSysVarPool *pGlobalSys = NULL;
    CSysVarPool *pPool = (CSysVarPool*) NameSpaceHolder::GetGlobalSysVars();
    if (pPool != NULL)
    {
        pGlobalSys = pPool;
    }
    
    // add configuration global security system variables:
    pPool = (CSysVarPool*) NameSpaceHolder::GetGlobalSecuritySysVars();
    if (pPool != NULL)
    {
        if (pGlobalSys != NULL) {
            pGlobalSys->addBranch(pPool);
        }
        pPool->release();
    }
    
    if (pGlobalSys != NULL) {
        // add global system path
        pRoot->addBranch(pGlobalSys);
        pGlobalSys->release();
    }
    
    
    // create all available resources; currently we only have two resources: SoftRT with JavaRT, Console

    //-------------     Resource 1: ------------------------------------------------------
    // create mirror of the soft-realtime resource (a JAVA NT service)
    // this is the default resource and therefor added at head of resource node list!!!!
    // add resources to namespace:
    SRT_Resource* pIecRes = new SRT_Resource(pszConfiguration, 0);
    assert(pIecRes != NULL);
    if (pIecRes != NULL)
    {
        hr = pIecRes->CreateSysVars();
        if (FAILED(hr)) {
            TRACE(_T("cannot add __system variables to __SRT0, hr=0x%08X\n" ), hr);
        }
        
        // add to dynamic resource list:
        CSingleLock lock(&sm_csDynRes, TRUE);
        sm_dynResList.AddTail(pIecRes);
        lock.Unlock();

        hr = pIecRes->OpenResourceInstance();
        if (FAILED(hr)) {
            TRACE(_T("ERROR NameSpaceHolder::CreateAndConnectNamespace: Cannot open IResource on control, hr=0x%08X\n" ), hr);
        }
    }
    else {
        TRACE(_T("ERROR NameSpaceHolder::CreateAndConnectNamespace: Cannot create SRT_Resource, out of memory!\n" ));
    }


    //-------------     Resource 2: ------------------------------------------------------
    // console resource
    CON_Resource    *pConRes = new CON_Resource(pszConfiguration);
    assert(pConRes != NULL);
    if (pConRes != NULL)
    {
        // add download variables
        GenericPointer<GenericBranch>    pSystem = pConRes->findBranch(g_csSystem);
        if (!!pSystem)
        {
            pPool = (CSysVarPool*) GetConsoleDLoadSysVars();
            if (pPool != NULL) {
                pSystem->addBranch(pPool);
                pPool->release();
            }
        }
        else {
            TRACE(_T("cannot add __system variables to __Console\n"));
        }
        pRoot->addBranch(pConRes);  // increments refCount too !!
        pConRes->release();     
    }
    else {
        TRACE(_T("ERROR NameSpaceHolder::CreateAndConnectNamespace: Cannot create CON_Resource, out of memory!\n" ));
    }
    
    return TRUE;
}


VOID CConnection::InitGUID()
{
    CSysVarPool *pPool = (CSysVarPool*) NameSpaceHolder::GetResourceDLoadSysVars(0);
    if (pPool != NULL)
    {
        // lookup in project and resource names
        GenericPointer<CSysVar> pVar = pPool->LookupSysVar(g_csSVarDLoadGUID);
        pPool->release();
        if (!!pVar)
        {
            GenericPointer<GenericCache> pCache = pVar->getCache();
            CComVariant Val;
            if (pCache->getValue(&Val))
            {
                if (SUCCEEDED(Val.ChangeType(VT_BSTR)))
                {
                    CLSIDFromString(Val.bstrVal, &m_GUID);
                }
            }
        }
    }

}




//-----------------------------------------------------------------------------
// deviceThreadMain
//
// - thread main routine of device working thread
//
// returns:
//		thread end state
//
DWORD WINAPI
    NameSpaceHolder::deviceThreadMain(IN LPVOID context)
{
    if (context == NULL) {
        assert(0);
        return 1;
    }

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	return ((NameSpaceHolder*)context)->deviceThread();
} // deviceThreadMain


//-----------------------------------------------------------------------------
// createDeviceThread
//
// - create device working thread
//
HRESULT
    NameSpaceHolder::createDeviceThread(void)
{
	DWORD tid;
	m_deviceThread = ::CreateThread(NULL, 0, deviceThreadMain, (void *)this, 0, &tid);

    HRESULT hr = S_OK;
    if (m_deviceThread == NULL) {
        hr = HRESULT_FROM_WIN32(::GetLastError());
    }
    return hr;
} // createDeviceThread


//-----------------------------------------------------------------------------
// deviceThread
//
// - thread routine of device working thread
//
// returns:
//		thread end state
//
DWORD
    NameSpaceHolder::deviceThread(void)
{
	BOOL end = FALSE;
	DWORD ret;
	HANDLE eventWait[2];

    myConnection.StartConnectThread();

	eventWait[WORK_EVENT] = m_workEvent;
	eventWait[END_EVENT] = m_endEvent;
    DWORD   dwLastCheck = ::GetTickCount();
    DWORD   dwItemCount = 0;
	while(!end)
	{
		ret = ::WaitForMultipleObjects(2, eventWait, FALSE, m_CheckRate);

		switch (ret - WAIT_OBJECT_0)
		{
        case END_EVENT:
            end = TRUE;
            break;
        case WORK_EVENT:
            {

            //-- write requests --
            RequestList delayedWriteList;
            BYTE *request = NULL;
            UINT requestLen = 0;
            for(;;)
            {   // while request list is not empty
                m_writeListLock.Lock();
                if (m_writeList.IsEmpty())
                {
                    m_writeListLock.Unlock();
                    break;
                }
                GenericPointer<GenericRequest> req = m_writeList.RemoveHead();
                m_writeListLock.Unlock();

                dwItemCount++;
                assert(req->isStarted() && !req->isPending());

                if (!req->isCanceled())
                {
					// request was started and not yet completed
                    GenericPointer<GenericItemTag>  tag   = req->getItemTag();
                    GenericPointer<GenericCache>    cache = req->getCache();
                    GenericPointer<GenericItem>     item  = req->getItem();
                    if (!tag || !cache) {
                        assert(0);
                        continue;
                    }
                    HRESULT hr = E_FAIL;
                    CComVariant vtWrite;
                    if (req->getWriteValue(&vtWrite)) {
                        GenericItemTag *pTag = tag;
                        if (pTag->getUserData() == iecVarType)
                          {
                            IEC_Variable *pIecVar = dynamic_cast<IEC_Variable *>(pTag);
                            assert(pIecVar != NULL);
                            if (pIecVar != NULL) {
                                hr = pIecVar->BuildWriteRequest(vtWrite, item, &request, &requestLen);
                                if (SUCCEEDED(hr)) {
                                    req.addRef();
                                    delayedWriteList.AddTail(req);
                                    continue;
                                }
                            }
                          }
                        else
                          {
                            COpcTag *pOpcTag = dynamic_cast<COpcTag *>(pTag);
                            assert(pOpcTag != NULL);
                            if (pOpcTag != NULL) {
                                hr = pOpcTag->writeToVar(vtWrite, item);
                            }
                          }
                    }

                    req->complete(hr);
				}

            }

            if (!delayedWriteList.IsEmpty()) 
			{
				HRESULT hr = myConnection.TransferData(TRUE, CMD_SET_VALUE, &request, &requestLen);

				if (FAILED(hr)) {
                    requestLen = 0;
                }

                while(!delayedWriteList.IsEmpty()) {
                    GenericPointer<GenericRequest> req = delayedWriteList.RemoveHead();
                    assert(req->isStarted() && !req->isPending());

                    req->complete(hr);
                }

                HeapFree(GetProcessHeap(), 0, request);
            }

		    //-- read requests --
            RequestList delayedReadList;
            request = NULL;
            requestLen = 0;
            for(;;)
            { // while request list is not empty
                m_readListLock.Lock();
                if (m_readList.IsEmpty())
                {
                    m_readListLock.Unlock();
                    if (delayedReadList.GetCount()==1)
                    {
                        Sleep(50);
                        if (!m_readList.IsEmpty())
                            continue;
                    }
                    break;
                }
                GenericPointer<GenericRequest> req = m_readList.RemoveHead();
                m_readListLock.Unlock();

                dwItemCount++;
                assert(req->isStarted() && !req->isPending());

                if (!req->isCanceled())
                {
                    GenericPointer<GenericItemTag>  tag   = req->getItemTag();
                    GenericPointer<GenericCache>    cache = req->getCache();
                    GenericPointer<GenericItem>     item  = req->getItem();
                    if (!tag || !cache) {
                        assert(0);
                        continue;
                    }

                    HRESULT hr = E_FAIL;
                    GenericItemTag *pTag = tag;
                    if (pTag->getUserData() == iecVarType)
                      {
                        IEC_Variable *pIecVar = dynamic_cast<IEC_Variable *>(pTag);
                        assert(pIecVar != NULL);
                        if (pIecVar != NULL) {
                            hr = pIecVar->BuildReadRequest(cache, item, &request, &requestLen);
                            if (SUCCEEDED(hr)) {
                                req.addRef();
                                delayedReadList.AddTail(req);
                                continue;
                            }
                        }
                      }
                    else
                      {
                        COpcTag *pOpcTag = dynamic_cast<COpcTag *>(pTag);
                        assert(pOpcTag != NULL);
                        if (pOpcTag != NULL) {
                            hr = pOpcTag->updateReadCache(cache, item);
                        }

                      }

                    req->complete(hr);
                }
            }

            if (!delayedReadList.IsEmpty()) 
			{
				HRESULT hr = myConnection.TransferData(TRUE, CMD_GET_VALUE, &request, &requestLen);

				if (FAILED(hr)) {
					requestLen = 0;
				}

                UINT readPos = 0;
                while(!delayedReadList.IsEmpty()) {
                    GenericPointer<GenericRequest> req = delayedReadList.RemoveHead();
                    assert(req->isStarted() && !req->isPending());
                    
                    checkRequestTime(req, SUCCEEDED(hr));
                    

                    GenericPointer<GenericItemTag>  tag   = req->getItemTag();
                    GenericPointer<GenericCache>    cache = req->getCache();
                    GenericPointer<GenericItem>     item  = req->getItem();
                    if (!tag || !cache) {
                        assert(0);
                        continue;
                    }

                    HRESULT hr = E_FAIL;
                    GenericItemTag *pTag = tag;
                    IEC_Variable *pOpcTag = dynamic_cast<IEC_Variable *>(pTag);
                    assert(pOpcTag != NULL);
                    if (pOpcTag != NULL) {
                        hr = pOpcTag->updateReadCache(cache, item, request, requestLen, &readPos);
                    }

                    req->complete(hr);
                }

                HeapFree(GetProcessHeap(), 0, request);
            }

            }
            // directly fall thru
            // break;
        default:
            DWORD dwTickCount = GetTickCount();
            if ((dwTickCount - dwLastCheck) > m_CheckRate)
            {
                sm_dwItemsPerSecond = (1000 * dwItemCount) / (dwTickCount - dwLastCheck);
                dwLastCheck = dwTickCount;
                dwItemCount = 0;
            }
            break;
        }
    }

    myConnection.StopConnectThread();
	return 0;
} // deviceThread


void 
    NameSpaceHolder::checkRequestTime(GenericRequest* req, BOOL bSucceeded)
{
    DWORD dwTS = getTimeSpan(req->getStartTime(), ::GetTickCount());
    
    GenericPointer<GenericBranch> pBranch;
    pBranch = req->getItem()->getParentBranch();
    if (!pBranch)
    {
        TRACE(_T("ERROR: NameSpaceHolder::checkRequestTime : findBranch == NULL\n" ));
        return;
    }
    while(!(pBranch->is(GENOBJ_TYPE_GROUP)))
    {
        pBranch = pBranch->getParentBranch();
        if (!pBranch)
        {
            TRACE(_T("ERROR: NameSpaceHolder::checkRequestTime : findBranch == NULL\n" ));
            return;
        }
    }
    
    GenericBranch* pBranchGroup = pBranch;
    GenericGroup* pGroup =  dynamic_cast<GenericGroup *>(pBranchGroup);
    if (!pGroup)
    {
        TRACE(_T("ERROR: NameSpaceHolder::checkRequestTime : pGroup == NULL\n" ));
        return;
    } 
    CDynamicOpcGroup* pDynGroup =  dynamic_cast<CDynamicOpcGroup *>(pBranchGroup);
    if (!pDynGroup)
    {
        TRACE(_T("ERROR: NameSpaceHolder::checkRequestTime : pDynGroup == NULL\n" ));
        return;
    }
      
      
    if (req->isCanceled())
    {
        DWORD dwNewTS = dwTS + 100;
        if (dwNewTS>m_CheckRate)  // maximum 5 sec.
        {
            dwNewTS = m_CheckRate;
        }
 
        pDynGroup->changeUpdateRate(dwNewTS);
    }
    else if (bSucceeded)
    {
        pDynGroup->checkUpdateRate(dwTS);
    }
}



//------------------------------------------------------------------*
/**
 * GetDefaultResource
 *      return default/first resource, which is the SoftRT/JavaVM resource
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    NameSpaceHolder::GetDefaultResource(IEC_Resource **ppRes)
{
    if (ppRes == NULL) {
        return E_POINTER;
    }
    *ppRes = NULL;

    HRESULT hr = E_FAIL;
    CSingleLock lock(&sm_csDynRes, TRUE);
    if (!sm_dynResList.IsEmpty()) {
        *ppRes = (IEC_Resource *) sm_dynResList.GetHead();
        if (*ppRes != NULL) {
            (*ppRes)->addRef();
            hr = S_OK;
        }
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * GetDynResourceList
 *      return list of dynamic namespace resources, currently only one: SRT0
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    NameSpaceHolder::GetDynResourceList(BranchList *pDynList)
{
    if (pDynList == NULL) {
        return E_POINTER;
    }

    HRESULT hr = S_FALSE;
    CSingleLock lock(&sm_csDynRes, TRUE);
    for (POSITION pos = sm_dynResList.GetHeadPosition(); pos != NULL; )
    {
        IEC_Resource *pRes = (IEC_Resource *) sm_dynResList.GetNext(pos);
        if (pRes != NULL) {
            pRes->addRef();
            hr = S_OK;
        }
        pDynList->AddTail(pRes);
    }
    return hr;
}


//------------------------------------------------------------------*
/**
 * GetNamedResource
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    NameSpaceHolder::GetNamedResource(LPCTSTR pszResourceName, IEC_Resource **ppRes)
{
    if (ppRes == NULL) {
        return E_POINTER;
    }
    *ppRes = NULL;

    assert(!!m_pEngine);
	GenericPointer<OPCRootNS> pRoot;
	pRoot = m_pEngine->getNamespaceRoot();
    // search for given resource name
    *ppRes = (IEC_Resource *) pRoot->findBranch(pszResourceName);
    if (*ppRes != NULL) {
       return S_OK;
    }

    CString strName = pszResourceName;
    CSingleLock lock(&sm_csDynRes, TRUE);
    for (POSITION pos = sm_dynResList.GetHeadPosition(); pos != NULL; )
    {
        IEC_Resource *pRes = (IEC_Resource *) sm_dynResList.GetNext(pos);
        if (pRes != NULL) {
            if (   (strName.CompareNoCase(pRes->getName()) == 0)
                || (strName.CompareNoCase(pRes->GetSystemName()) == 0))
            {
                pRes->addRef();
                *ppRes = pRes;
                return S_OK;
            }
        }
    }
    
    return E_FAIL;
}

//------------------------------------------------------------------*
void
    NameSpaceHolder::ReopenResources()
{
    GenericPointer<IEC_Resource>    pRes;
    HRESULT hr = GetDefaultResource(&pRes);
    if (FAILED(hr) || !pRes) {
        return;
    }
    
    // need only to reconnect with control!
    hr = pRes->OpenResourceInstance();
    
}

//------------------------------------------------------------------*
void
    NameSpaceHolder::MarkAllTags(WORD /*quality*/, BOOL /*bMarkDeleted*/ /*= FALSE*/)
{
//    assert(FALSE);      // %TBD% not implemented yet !!!!!!!!!
}




// CD1: used to update namespace after new download
//------------------------------------------------------------------*
BOOL
    NameSpaceHolder::CheckForNamespaceUpdate(void)
{
    GenericPointer<IEC_Resource>    pRes;
    HRESULT hr = GetDefaultResource(&pRes);
    if (FAILED(hr) || !pRes) {
        return FALSE;
    }
    return pRes->CheckForNamespaceUpdate();
}


//------------------------------------------------------------------*
/**
 * SplitVariableName
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL
    NameSpaceHolder::SplitVariableName(LPCTSTR pszFullPathName, CStringList& listParsedName)
{
    CString strFullPathName = pszFullPathName;

    if (!listParsedName.IsEmpty()) {
        listParsedName.RemoveAll();
    }
    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strFullPathName, NameSpaceHolder::sm_pszSeparatorList);
    while (pszSplit != NULL) {
        listParsedName.AddTail(pszSplit);
        pszSplit = _tcstok(NULL, NameSpaceHolder::sm_pszSeparatorList);
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * GetResourceName
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL
    NameSpaceHolder::GetResourceName(LPCTSTR pszFullPathName, CString &strResourceName)
{
    CString strFullPathName = pszFullPathName;

    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strFullPathName, NameSpaceHolder::sm_pszSeparatorList);
    BOOL bFound;
    if (pszSplit != NULL) {
        strResourceName = pszSplit;
        bFound = TRUE;
    }
    else {
        strResourceName.Empty();
        bFound = FALSE;
    }
    return bFound;
}


//------------------------------------------------------------------*
/**
 * IsSystemBranchName
 *
 * @param           
 * @return          TRUE if branch name matches system folder name
 * @exception       -
 * @see             
*/
BOOL
    NameSpaceHolder::IsSystemBranchName(LPCTSTR pszBranchName)
{
    return (_tcsicmp(pszBranchName, g_csSystem) == NULL);
}

//------------------------------------------------------------------*
/**
 * IsConsoleBranchName
 *
 * @param           
 * @return          TRUE if branch name matches console folder name
 * @exception       -
 * @see             
*/
BOOL
    NameSpaceHolder::IsConsoleBranchName(LPCTSTR pszBranchName)
{
    return (_tcsicmp(pszBranchName, g_csResCon) == NULL);
}

//------------------------------------------------------------------*
/**
 * IsDynamicResource
 *
 * @param           listParsedPath  path parsed into its pathnames
 * @param           ppRes           resource to which name points; NULL if no dynamic resource
 * @return          TRUE if path points to some dynamic resource
 * @exception       -
 * @see             
*/
BOOL
    NameSpaceHolder::IsDynamicResource(CStringList &listParsedPath, OUT IEC_Resource **ppRes)
{
    assert(ppRes != NULL);
    *ppRes = NULL;
    CString strTop = listParsedPath.GetHead();
    if (   IsSystemBranchName(strTop)
        || IsConsoleBranchName(strTop))
    {
        return FALSE;    // static
    }

    CSingleLock lock(&sm_csDynRes, TRUE);
    for (POSITION pos = sm_dynResList.GetHeadPosition(); pos != NULL; )
    {
        IEC_Resource *pRes = (IEC_Resource *) sm_dynResList.GetNext(pos);
        if (pRes != NULL) {
            if (   (strTop.CompareNoCase(pRes->getName()) == 0)
                || (strTop.CompareNoCase(pRes->GetSystemName()) == 0))
            {
                pRes->addRef();
                *ppRes = pRes;
                break;
            }
        }
    }

    return (*ppRes != NULL);
}



//------------------------------------------------------------------*
void
    NameSpaceHolder::Cleanup()
{
}

/* ---------------------------------------------------------------------------- */
