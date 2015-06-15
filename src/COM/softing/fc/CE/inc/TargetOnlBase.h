#ifndef __TARGETONLBASE_H_
#define __TARGETONLBASE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CSC_Online.h"
#include "implsessnot.h"
#include "fccallback.h"
#include "Subscription.h"
#include "KADHelper/KADTargetData.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define E_CE_TARGETONL_GENERAL                  1
#define E_CE_TARGETONL_ALREADYCONNECTED         2
#define E_CE_TARGETONL_ADVISEFAILED             3
#define E_CE_TARGETONL_NOTCONNECTED             4
#define E_CE_TARGETONL_UNADVISEFAILED           5
#define E_CE_TARGETONL_CONNECTFAILED            6
#define E_CE_TARGETONL_DISCONNECTFAILED         7
#define E_CE_TARGETONL_UNKNOWNVARIABLE          8
#define E_CE_TARGETONL_ALREADYSUBSCR            9
#define E_CE_TARGETONL_FORCEDISCONNECTFAILED    10

enum TargetConnectAction
{
    TargetPreConnect,
    TargetPostConnect,
    TargetConnectError,
    TargetPreDisconnect,
    TargetPostDisconnect,
    TargetDisconnectError,
    TargetPreExclusiveConnect,
    TargetPostExclusiveConnect,
    TargetExclusiveConnectError,
    TargetPreExclusiveDisconnect,
    TargetPostExclusiveDisconnect,
    TargetExclusiveDisconnectError
};

// values of resource state variable
typedef enum E_ResourceState
{
    eRS_not_connected = -4,     // -4: target is not connected
    eRS_version_conflict,       // -3: download version not up to date
    eRS_service_stopped,        // -2: PC service not running
    eRS_general_error,          // -1: not specified error
    eRS_on_creation,            // 0: not yet used
    eRS_stopped,                // 1: resource is stopped
    eRS_running                 // 2: resource is running
};

// values of target online connect state (TOCS)
enum E_TargetOnlConnectState
{
    eTOCS_exception = -5,       // -5: this state should never be reached
    eTOCS_targ_con_ex_error,    // -4: variable and exclusive conn OK, target not connected
    eTOCS_targ_con_error,       // -3: variable conn OK, no exclusive and target not connected
    eTOCS_csc_con_ex_error,     // -2: variable conn OK, exclusive conn interrupted after beeing establ.
    eTOCS_csc_con_error,        // -1: variable conn interrupted, no exclusive
    eTOCS_not_connected,        // 0: variable conn not connected, exclusive not connected
    eTOCS_connected,            // 1: variable conn OK, exclusive not connected, target connected
    eTOCS_connected_ex          // 2: variable and exclusive conn OK, target connected
};

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CImplSessionNotify;
class CTargetOnlBase;

FC_CALLBACK_DEF3 (TargetOnlSessionCB, CTargetOnlBase*, CSC_SESSION_STATE, HRESULT)
FC_CALLBACK_DEF2 (TargetOnlConnectActionCB, CTargetOnlBase*, TargetConnectAction)

FC_CALLBACK_DEF1 (TargetOnlConnectStateCB, CTargetOnlBase*)
FC_CALLBACK_DEF1 (TargetOnlResourceStateCB, CTargetOnlBase*)


//------------------------------------------------------------------*
/**
 *  class CTargetOnlBase
 *
 *  @see    ....
 */
class CTargetOnlBase
{
//friend CImplSessionNotify;
public:
    CTargetOnlBase::CTargetOnlBase( const CString& crstrTargetId = "",
                                    const CString& crstrInstIdPath = "",
                                    const CString& crstrType = "",
                                    const CString& crstrAssignedResource = "");

    virtual ~CTargetOnlBase();

    // set identification string
    void SetId(const CString& crstrId);
    CString GetId();
    // set id-path in instance view
    void SetIdPath(const CString& crstrInstIdPath);
    CString GetIdPath();
    // set type
    void SetType(const CString& crstrType);
    CString GetType();

    void SetControlAddress(const CString& crstrAddress);
    CString GetControlAddress();
    // connected control address may differ from control address:
    CString GetConnectedControlAddress();

    void SetUserControlAddress(const CString& crstrUserAddress);
    CString GetUserControlAddress();

    void SetAssignedResource(const CString& crstrAssignedResource);
    CString GetAssignedResource();

    //get the user name from the system
    LPCTSTR GetUserName() const
    { return m_strUserName; }

    //get the settings password
    LPCTSTR GetPassword() const
    { return m_strPassword; }

    BOOL GetWaitForTargetConnection() const { return m_bWaitForTargetConnection;}
    void SetWaitForTargetConnection(const BOOL bWaitForTargetConnection);

    // initialization
    virtual BOOL Init();

    // connect
    HRESULT Connect();
    virtual HRESULT Disconnect();
    BOOL    IsConnected();
    BOOL    IsTargetConnected();

    HRESULT EstablishConnection(BOOL bExclusive);

    CSC_SESSION_STATE GetSessionState();

    // virtual methods for connections
    virtual BOOL IsConnectedExclusive();
    virtual BOOL IsTargetConnectedExclusive();
    virtual CSC_SESSION_STATE GetExclusiveSessionState();

    virtual HRESULT ConnectExclusive();
    virtual HRESULT DisconnectExclusive(BOOL bForce = FALSE);

    BOOL DoPreConnectStep();

    // variable connection
    HRESULT SubscribeVariable (const CString &strVariableName, FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB, CSubscription *&pSubscription);
    HRESULT UnsubscribeVariable (CSubscription *pSubscription, FC_CALLBACK_BASE (SubscriptionModifiedCB) *pModifiedCB);
    HRESULT ReadVariable (CSubscription *pSubscription, short& sQuality, CComVariant &varValue);

    // get the current user from CSC Online
    HRESULT GetCurrentUser(CString& rstrUser);
    HRESULT GetMachineName(CString& rstrMachineName);
    
    // resource actions
    HRESULT StartResource();
    HRESULT StopResource();
    HRESULT ColdStartResource();
    HRESULT WarmStartResource();

    // task actions
    HRESULT StartTask(LPCTSTR pszTask);
    HRESULT StopTask(LPCTSTR pszTask);
    HRESULT StartAllTasks();
    HRESULT StopAllTasks();

    // custom online commands
    HRESULT CustomOnlCmd(SAFEARRAY **ppDataDown, SAFEARRAY **ppDataUp);
    HRESULT ProjectInfoUpload(BSTR* psProjVers,
	                          BSTR* psProjName,
	                          BSTR* psModified,
	                          BSTR* psCreated,
                              BSTR* psOwner,
	                          BSTR* psComment1,
	                          BSTR* psComment2,
	                          BSTR* psComment3);
    HRESULT ProjectDownload(SAFEARRAY **ppDataDown);
    HRESULT ProjectUpload(SAFEARRAY **ppDataUp);
    
    HRESULT ClearFlash();
    HRESULT CustomCommandSimple(unsigned char ucCommand, unsigned char ucTrials = 1);

    HRESULT CustCmd1(SAFEARRAY **ppDataDown, 
                     SAFEARRAY **ppDataUp);  
    HRESULT CustCmd2(SAFEARRAY **ppDataDown, 
                     SAFEARRAY **ppDataUp); 
    HRESULT CustCmd3(SAFEARRAY **ppDataDown, 
                     SAFEARRAY **ppDataUp);  
    HRESULT CustCmd4(SAFEARRAY **ppDataDown, 
                     SAFEARRAY **ppDataUp); 
    HRESULT CustCmd5(SAFEARRAY **ppDataDown, 
                     SAFEARRAY **ppDataUp); 
    HRESULT LoadFile(BSTR sName, 
                     SAFEARRAY **ppData); 
    HRESULT SaveFile(BSTR sName, 
                     SAFEARRAY **ppData);   
    HRESULT Dir(SAFEARRAY **ppFileNames);

    // state queries
    bool    IsResourceStarted();
    bool    IsResourceStopped();
    bool    IsResourceError();
    bool    IsServiceStopped();
    
    E_ResourceState GetResourceState();
    E_TargetOnlConnectState GetConnectState();

    virtual CString GetConnectStateText();
    virtual CString GetDiagnosisText();

    CString GetSessionStateText(CSC_SESSION_STATE state);
    CString GetResourceStateText();

    // register / unregister callbacks
    HRESULT RegisterSessionCB(FC_CALLBACK_BASE(TargetOnlSessionCB) *pCallback);
    HRESULT UnregisterSessionCB(FC_CALLBACK_BASE(TargetOnlSessionCB) *pCallback);

    HRESULT RegisterConnectActionCB (FC_CALLBACK_BASE (TargetOnlConnectActionCB) *pCallback);
    HRESULT UnregisterConnectActionCB (FC_CALLBACK_BASE (TargetOnlConnectActionCB) *pCallback);

    HRESULT RegisterConnectStateCB (FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pCallback);
    HRESULT UnregisterConnectStateCB (FC_CALLBACK_BASE (TargetOnlConnectStateCB) *pCallback);

    HRESULT RegisterResourceStateCB (FC_CALLBACK_BASE (TargetOnlResourceStateCB) *pCallback);
    HRESULT UnregisterResourceStateCB (FC_CALLBACK_BASE (TargetOnlResourceStateCB) *pCallback);

    virtual void OnUserLoggedIn (const CString &strUserName, const CString &strHostName, bool bLoggedIn);

    HRESULT ShowConnectDialog(CString & strErrorMessage, BOOL bExclusive = FALSE);
    int GetConnectTimeout();

    BOOL IsConsoleType();
    BOOL IsDebugEnabled();
    BOOL IsMultitaskEnabled();
    BOOL IsWarmstartEnabled();
    BOOL IsInitClearEnabled();
    BOOL IsDLChangesEnabled();

    CKADTargetData* GetKADTargetData();

protected:
    BOOL LoadKADInfo();
    HRESULT MakeHResult(long lCode);
    
    HRESULT OpenVarSession();
    HRESULT CloseVarSession();

    HRESULT SubscribeForState ();
    HRESULT UnsubscribeForState ();
    void OnResourceStateChanged (CSubscription *pSubscription);

    virtual HRESULT SubscribeForDownloadInfo();
    virtual HRESULT UnsubscribeForDownloadInfo();

    void OnSessionNotifyCB(CSC_SESSION_STATE sessionState, HRESULT hrError);

    static void LockSubscriptionManager ();
    static void UnlockSubscriptionManager ();

    HRESULT SetPassword (const CString &strPassword);

    void GetNewConnectState();

    virtual E_TargetOnlConnectState CalculateConnectState();

    void ExecuteConnectStateChangeCallbacks();
    void ExecuteResourceConnectChangeCallbacks();

protected:
    CString     m_strId;                    // id of the target
    CString     m_strIdPath;                // id-path to instance node
    CString     m_strType;                  // target type (KAD)

    CString     m_strUserName;              // user name from system
    CString     m_strPassword;              // password from settings

    CString     m_strAssignedResource;      // assigned resource
    CString     m_strControlAddress;        // address to connect to control
    CString     m_strControlAddressConn;    // address of currently connected control
    CString     m_strUserControlAddress;    // address to connect to control shown to user

    BOOL        m_bWaitForTargetConnection;

    BOOL        m_bConnected;               // TRUE if connected

    CKADTargetData  m_tKADTargetData;       // KAD target data object

    // interface pointer to variable session
    CComPtr<IFCVarSession>                          m_pIFCVarSession;
    CComObject<CImplSessionNotify>*                 m_pSessionNotify;
    FC_CALLBACK(SessionNotifyCB, CTargetOnlBase)    m_SessionNotifyCB;
    
    typedef CList<FC_CALLBACK_BASE (TargetOnlSessionCB) *, FC_CALLBACK_BASE (TargetOnlSessionCB) *> SessionCBList;
    SessionCBList       m_SessionCBs;
    FC_CALLBACK_HOLDER (TargetOnlConnectActionCB) m_ConnectActionCBs;

    FC_CALLBACK_HOLDER (TargetOnlConnectStateCB)  m_ConnectStateCBs;
    FC_CALLBACK_HOLDER (TargetOnlResourceStateCB) m_ResourceStateCBs;
    
    DWORD	            m_dwSessionCookie;
    CSC_SESSION_STATE   m_SessionState;

    CSubscription*                              m_pStateSubscription;
//    long                                        m_lState;
    FC_CALLBACK(SubscriptionModifiedCB, CTargetOnlBase)    m_StateCB;

    CSubscription*                              m_pProjectGUIDSubscription;
    bool                                        m_bProjectGUIDValid;
    CString                                     m_strProjectGUID;
    CSubscription*                              m_pProjectNameSubscription;
    bool                                        m_bProjectNameValid;
    CString                                     m_strProjectName;

    E_TargetOnlConnectState         m_eConnectState;
    E_TargetOnlConnectState         m_eOldConnectState;
    E_ResourceState                 m_eResourceState;
    E_ResourceState                 m_eOldResourceState;

    static CSubscriptionManager*    m_pSubscriptionManager;
    static unsigned int             m_uiSMLockCount;

};

#endif // __TARGETONLBASE_H_