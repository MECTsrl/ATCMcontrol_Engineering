/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/SysVar.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: SysVar.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/SysVar.cpp $
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
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "OpcError.h"
#include "CSC_OnlineErr.h"
#include "NSpaceHolder.h"
#include "iecResource.h"
#include "srtResource.h"
#include "controlMgt.h"     // used to access license info
#include "opcDynServer.h"
#include "Creator.h"
#include "connection.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CSysVarRegistry
/////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
CSysVar::CSysVar(LPCTSTR pszName /* = NULL*/, DWORD rights /* = OPC_READABLE*/, VARTYPE datatype /*= VT_BSTR*/)
{
    if (pszName != NULL)
    {
        CString strName = pszName;
        strName.MakeLower();
        setName(strName);
    }
    setAccessRights(rights);
    setNativeDatatype(datatype);
    setUserData(sysVarType);

	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	GenericPointer<GenericCreator> creator = engine->getCreator();
    GenericCreator *pCreator = creator;
    Creator *pSpecificCreator = dynamic_cast<Creator *>(pCreator);
    if (pSpecificCreator != NULL)
        pSpecificCreator->Bless(this);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSysVar::~CSysVar()
{
}


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
CSysVarRegistry::CSysVarRegistry(LPCTSTR pszName)
  : CSysVar(pszName)
{
    if (_tcsicmp(pszName, g_csSVarCleared) == 0)
    {
        setNativeDatatype(VT_BOOL);
    }
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSysVarRegistry::~CSysVarRegistry()
{
}


//------------------------------------------------------------------*
/**
 * read value from device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarRegistry::updateReadCache(IN GenericCache *cache, IN GenericItem *item)
{
    // is a simple variable
    // it's not a QVT, add quality and timestamp here
	cache->setValue(&m_vtMutexedValue);
	cache->setQuality(OPC_QUALITY_GOOD);
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * write value to device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarRegistry::writeToVar(IN VARIANT /* vtNew*/, IN GenericItem * /* item */)
{
    return OPC_E_BADRIGHTS;
}

//------------------------------------------------------------------*
/**
 * update registry with new value
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarRegistry::Update(LPCTSTR pszNewValue)
{
    HRESULT hr = E_FAIL;
    CSysVarPool *pPool = (CSysVarPool*) getParentBranch();
    if (pPool == NULL) {
        return E_FAIL;
    }
    // %TBD% downcast is not checked, hope it is a var pool object.... !!!
    CSysVarPool::m_cs.Lock();
    m_vtMutexedValue = pszNewValue;

    // update cached value
    GenericPointer<GenericCache>    pCache = getCache();
    if (!(!pCache)) {
        pCache->setValue(&m_vtMutexedValue);
    }
    CSysVarPool::m_cs.Unlock();
    HKEY    hKey = pPool->OpenSysVarPath(TRUE);
    pPool->release();
    if (hKey != NULL)
    {
        hr = UpdateToRegistry(hKey);
    }

    return hr;
}

//------------------------------------------------------------------*
/**
 * reread current value from registry
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarRegistry::Reread()
{
    HRESULT hr = S_FALSE;
    CSysVarPool *pPool = (CSysVarPool*) getParentBranch();
    if (pPool == NULL) {
        return E_FAIL;
    }
    // %TBD% downcast is not checked, hope it is a var pool object.... !!!
    HKEY    hKey = pPool->OpenSysVarPath(FALSE);
    pPool->release();
    if (hKey != NULL)
    {
        hr = InitFromRegistry(hKey);
    }

    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CSysVarRegistry::InitFromRegistry(HKEY hkNode)
{
#define _MAX_SYS_VAR_LENGTH 300
    CRegKey key;
    key.Attach(hkNode);
    CString strValue;
    LPTSTR psz = strValue.GetBuffer(_MAX_SYS_VAR_LENGTH);
    DWORD   dwCount = _MAX_SYS_VAR_LENGTH - 1;
    LONG ret = key.QueryValue(psz, getName(), &dwCount);
    if (dwCount >= _MAX_SYS_VAR_LENGTH) {
        return E_FAIL;
    }
    strValue.ReleaseBuffer(dwCount - 1);
    CSysVarPool::m_cs.Lock();
    m_vtMutexedValue  = strValue;
    // initialize cached value
    GenericPointer<GenericCache>    pCache = getCache();
    if (!(!pCache)) {
        pCache->setValue(&m_vtMutexedValue);
    }
    CSysVarPool::m_cs.Unlock();
    return (ret == ERROR_SUCCESS) ? S_OK : S_FALSE;
}

//------------------------------------------------------------------*
HRESULT
    CSysVarRegistry::UpdateToRegistry(HKEY hkNode)
{
    CRegKey key;
    HRESULT hr;
    CSysVarPool::m_cs.Lock();
    CComVariant vtNew = m_vtMutexedValue;
    CSysVarPool::m_cs.Unlock();
    hr = vtNew.ChangeType(VT_BSTR);
    if (FAILED(hr)) {
        return hr;
    }
    CString strNewValue = vtNew.bstrVal;
    key.Attach(hkNode);
    LONG ret = key.SetValue(strNewValue, getName());
    if (ret == ERROR_SUCCESS) {
        return S_OK;
    }
    else {
        return E_FAIL;
    }
}


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CSysVarSecurity
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
CSysVarSecurity::CSysVarSecurity(LPCTSTR pszName, SecurityType type)
  : CSysVar(pszName),
    m_bIsEnabled(FALSE),
    m_rights(CSecurity::NoSecurity)
{
    m_securityType = type;
    switch (m_securityType) {
    case isEnabled:
        setNativeDatatype(VT_BOOL);
        break;
    case Password:
        setAccessRights(OPC_WRITEABLE);
        setNativeDatatype(VT_BSTR);
        break;
    case Rights:
        setNativeDatatype(VT_I2);
        break;
    default:
        break;
    }
    Reread();
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSysVarSecurity::~CSysVarSecurity()
{
}


//------------------------------------------------------------------*
/**
 * read value from device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarSecurity::updateReadCache(IN GenericCache *cache, IN GenericItem *item)
{
    HRESULT hr = S_OK;

    CComVariant value;
    switch (m_securityType) {
    case isEnabled:
        value = g_security.IsSecurityEnabled();
        break;
    case Rights:
        {
            value = (int) GetCurrentAccessRights(item);
        }
        break;
    default:
        hr = OPC_E_BADRIGHTS;
        break;
    }
    // is a simple variable
    // it's not a QVT, add quality and timestamp here
    if (SUCCEEDED(hr))
    {
        cache->setValue(&value);
        cache->setQuality(OPC_QUALITY_GOOD);
    }
    else {
        cache->setQuality(OPC_QUALITY_BAD);
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * write value to device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarSecurity::writeToVar(IN VARIANT vtNew, IN GenericItem *item)
{
    HRESULT hr = S_OK;
    CComVariant vtNewVal = vtNew;
    hr = vtNewVal.ChangeType(VT_BSTR);
    if (FAILED(hr)) return hr;

    switch (m_securityType) {
    case Password:
        assert(vtNewVal.vt == VT_BSTR);
        m_strPassword = vtNewVal.bstrVal;
        // check if password correct and determine rights
        m_rights = g_security.GetUserRights(m_strPassword);
        SavePassword(m_strPassword, item);
        hr = SetAccessRights(m_rights, item);
        break;
    default:
        hr = OPC_E_BADRIGHTS;
        break;
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * update registry with new value
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarSecurity::Update(LPCTSTR /*pszNewValue*/)
{
    return Reread();
}

//------------------------------------------------------------------*
/**
 * reread current value from registry
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarSecurity::Reread()
{
    HRESULT hr = S_OK;

    switch (m_securityType) {
    case isEnabled:
        hr = UpdateSecurity();
        break;
    case Rights:
    default:
        hr = S_FALSE;
        break;
    }

    return hr;
}

//------------------------------------------------------------------*
/**
 * UpdateSecurity for all sessions
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarSecurity::UpdateSecurity()
{
    HRESULT hr = S_OK;
    BOOL bChanged = FALSE;
    
    CSecurity::SECURITY_RIGHTS  rights;
    // get any new security from downloaded domains:
    hr = g_security.ReReadSecurityInfo();
    if (FAILED(hr)) {
        ::TrcPrint(TRC_ERROR, _T("CSysVarSecurity::UpdateSecurity(): failed to update security info, hr=0x%08X\n"), hr);
        m_bIsEnabled = FALSE;
        rights    = CSecurity::NoSecurity;
        bChanged = TRUE;
    }
    else
    {
        // has security info changed?
        if (hr == S_OK) {
            // yes, have new security settings
            bChanged = TRUE;
            ::TrcPrint(TRC_INTERFACE, _T("CSysVarSecurity::UpdateSecurity(): change in update security info!\n"));
        }
    }
    // propagate to all sessions!
    if (bChanged)
    {
        GenericPointer<OPCEngine>   pEngine = ::getOPCEngine();
        GenericPointer<OPCRootOPC>   pRoot;
        pRoot = pEngine->getOPCRoot();
        
        // count items in all servers
        if (!!pRoot)
        {
            GenericList<GenericBranch> sessions;
            sessions = pRoot->getBranchList();
            if (!sessions.isEmpty())
            {
                // search thru all groups of server and get total item count
                for (POSITION pos = sessions.getStartPosition(); pos != NULL; )
                {
                    CDynamicOpcServer *pSession;
                    pSession = dynamic_cast<CDynamicOpcServer*> (sessions.getNext(pos));
                    assert(pSession != NULL);
                    ::TrcPrint(TRC_INTERFACE, _T("CSysVarSecurity::UpdateSecurity(): change in security settings for session 0x%08X, reapplying prev. PW...\n"), pSession);
                    TRACE(_T("CSysVarSecurity::UpdateSecurity(): change in security settings for session 0x%08X, reapplying prev. PW...\n"), pSession);
                    CSecurity::SECURITY_RIGHTS newRights = g_security.GetUserRights(pSession->GetSavedPassword());
                    pSession->SetSessionRights(newRights);
                    TrcPrint(TRC_INTERFACE, _T("CSysVarSecurity::UpdateSecurity(): session 0x%08X changed to '%s'\n"),
                        pSession, CSecurity::GetSecurityText(newRights));
                    TRACE(_T("CSysVarSecurity::UpdateSecurity(): session 0x%08X changed to '%s'\n"),
                        pSession, CSecurity::GetSecurityText(newRights));
                }
            }
        }
    }
    
    m_bIsEnabled = g_security.IsSecurityEnabled();
    return hr;
}

/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CSysVarDebug
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
CSysVarDebug::CSysVarDebug(LPCTSTR pszName, DbgType type)
  : CSysVar(pszName),
    m_bIsEnabled(FALSE)
{
    m_debugType = type;
    switch (m_debugType) {
    case isEnabled:
        setNativeDatatype(VT_BOOL);
        break;
    case startedDate:
    case currUser:
    case currHost:
        setNativeDatatype(VT_BSTR);
        break;
    default:
        break;
    }
    Reread();
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSysVarDebug::~CSysVarDebug()
{
}


//------------------------------------------------------------------*
/**
 * read value from device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarDebug::updateReadCache(IN GenericCache *cache, IN GenericItem *item)
{
    HRESULT hr = S_OK;

    CComVariant value;
    switch (m_debugType) {
    case isEnabled:
        value = m_bIsEnabled;
        break;
    case startedDate:
        value = m_strVal;
        break;
    case currUser:
        value = m_strVal;
        break;
    case currHost:
        value = m_strVal;
        break;
    default:
        hr = OPC_E_BADRIGHTS;
        break;
    }
    // is a simple variable
    // it's not a QVT, add quality and timestamp here
    if (SUCCEEDED(hr))
    {
        cache->setValue(&value);
        cache->setQuality(OPC_QUALITY_GOOD);
    }
    else {
        cache->setQuality(OPC_QUALITY_BAD);
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * write value to device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarDebug::writeToVar(IN VARIANT /* vtNew*/, IN GenericItem * /* item */)
{
    return OPC_E_BADRIGHTS;
}

//------------------------------------------------------------------*
/**
 * update registry with new value
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarDebug::Update(LPCTSTR pszNewValue)
{
    HRESULT hr = S_OK;
    switch (m_debugType) {
    case isEnabled:
        m_bIsEnabled = (_ttoi(pszNewValue) != 0);
        break;
    case startedDate:
        m_strVal = pszNewValue;
        break;
    case currUser:
        m_strVal = pszNewValue;
        break;
    case currHost:
        m_strVal = pszNewValue;
        break;
    default:
        hr = OPC_E_BADRIGHTS;
        break;
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * reread current value from registry
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarDebug::Reread()
{
    HRESULT hr = S_FALSE;

    m_bIsEnabled = FALSE;

    return hr;
}


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CSysVarResource
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
CSysVarResource::CSysVarResource(LPCTSTR pszName, ResVarType type)
  : CSysVar(pszName)
{
    m_resVarType = type;
    switch (m_resVarType) {
    case state:
    case dnload:
        setNativeDatatype(VT_I4);
        break;
    default:
        break;
    }
    Reread();
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSysVarResource::~CSysVarResource()
{
}


//------------------------------------------------------------------*
/**
 * read value from device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarResource::updateReadCache(IN GenericCache *cache, IN GenericItem *item)
{
    HRESULT hr = E_FAIL;

    CComVariant value;
    switch (m_resVarType) {
    case state:
        value = GetResourceState();
        hr = S_OK;
        break;
    case dnload:
        {
            GenericPointer<GenericBranch> pool = getParentBranch();
            if (!!pool) {
                GenericPointer<GenericBranch> resource = pool->getParentBranch();
                GenericBranch *pResource = resource;
                SRT_Resource *pSRT_Resource = dynamic_cast<SRT_Resource *>(pResource);
                if (pSRT_Resource != NULL) {
                    value = pSRT_Resource->GetCachedDownloadCounter();
                    hr = S_OK;
                }
            }
        }
        break;
    default:
        hr = OPC_E_BADRIGHTS;
        break;
    }
    // is a simple variable
    // it's not a QVT, add quality and timestamp here
    if (SUCCEEDED(hr))
    {
        cache->setValue(&value);
        cache->setQuality(OPC_QUALITY_GOOD);
    }
    else {
        cache->setQuality(OPC_QUALITY_BAD);
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * write value to device
 *
 * @param           req     request
 * @return          void
 * @exception       -
 */
HRESULT
    CSysVarResource::writeToVar(IN VARIANT /* vtNew*/, IN GenericItem * /* item */)
{
    return OPC_E_BADRIGHTS;
}

//------------------------------------------------------------------*
/**
 * update registry with new value
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarResource::Update(LPCTSTR /*pszNewValue*/)
{
    return Reread();
}

//------------------------------------------------------------------*
/**
 * reread current value from registry
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CSysVarResource::Reread()
{
    HRESULT hr = S_FALSE;

    return hr;
}

//------------------------------------------------------------------*
/**
 * reread current value from registry
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CSysVarResource::ControlState
    CSysVarResource::GetResourceState()
{
	BOOL bOK = myConnection.IsConnected();
    if (!bOK)
        return NoConnect;

    BOOL bLogin = myConnection.IsLogon();
    LONG lResourceState = myConnection.GetResourceState();

    if (!bLogin)
        return BadVersion;
    else
        return (ControlState)lResourceState;
}

/* ---------------------------------------------------------------------------- */
