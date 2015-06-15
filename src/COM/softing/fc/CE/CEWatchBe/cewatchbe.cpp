

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// cewatchbe.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f cewatchbeps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "cewatchbe.h"
#include "subscription.h"
#include "cewatchbe_i.c"
#include "csc_online_i.c"
#include "cewatch.h"
#include "fcpump.h"

CComModule _Module;

static CExpressionFactory *theExpressionFactory;


CExpressionFactory *CEGetExpressionFactory ()
{
    ASSERT (theExpressionFactory != NULL);
    return (theExpressionFactory);
}


BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_CEWatchBE, CCEWatch)
  OBJECT_ENTRY(CLSID_CEWatchAdminBE, CCEWatchAdmin)
END_OBJECT_MAP()


class CCewatchbeApp : public CWinApp
{
public:
    CCewatchbeApp ();
    HRESULT LockManagers ();
    HRESULT UnlockManagers ();
    CSubscriptionManager *GetSubscriptionManager ();
    CCESessionManager *GetSessionManager ();
    void AddDelayedAdapt (CExpressionNode *pExpression);
    void RemoveDelayedAdapt (CExpressionNode *pExpression);
    void TriggerResubscribe ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCewatchbeApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCewatchbeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    unsigned int m_uiSMLockCount;
    CFCPump m_Pump;
    FC_CALLBACK (FCPumpCB, CCewatchbeApp) m_PumpCB;
    CList<CExpressionNode *, CExpressionNode *> m_AdaptExpressions;
    CSubscriptionManager m_SubscriptionManager;
    CCESessionManager *m_pSessionManager;
    bool m_bResubscribe;

    void OnPump ();
};


BEGIN_MESSAGE_MAP(CCewatchbeApp, CWinApp)
	//{{AFX_MSG_MAP(CCewatchbeApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CCewatchbeApp theApp;


void CELockManagers ()
{
    HRESULT hr;

    hr = theApp.LockManagers ();
    ASSERT (SUCCEEDED (hr));
}


void CEUnlockManagers ()
{
    (void)theApp.UnlockManagers ();
}


CSubscriptionManager *CEGetSubscriptionManager ()
{
    CSubscriptionManager *pSubscriptionManager;

    pSubscriptionManager = theApp.GetSubscriptionManager ();
    ASSERT (pSubscriptionManager != NULL);
    return (pSubscriptionManager);
}


CCESessionManager *CEGetSessionManager ()
{
    CCESessionManager *pSessionManager;

    pSessionManager = theApp.GetSessionManager ();
    ASSERT (pSessionManager != NULL);
    return (pSessionManager);
}


void CEAddDelayedAdapt (CExpressionNode *pExpression)
{
    theApp.AddDelayedAdapt (pExpression);
}


void CERemoveDelayedAdapt (CExpressionNode *pExpression)
{
    theApp.RemoveDelayedAdapt (pExpression);
}


void CETriggerResubscribe ()
{
    theApp.TriggerResubscribe ();
}


CCewatchbeApp::CCewatchbeApp ()
: m_uiSMLockCount (0),
  m_PumpCB (this, OnPump),
  m_pSessionManager (NULL),
  m_bResubscribe (false)
{
}


BOOL CCewatchbeApp::InitInstance()
{
    HRESULT hr;

    _Module.Init(ObjectMap, m_hInstance, &LIBID_CEWATCHBELib);
    CRect rect (0, 0, 0, 0);

    m_Pump.RegisterCB (&m_PumpCB);
    hr = m_Pump.Create (_T ("ATCM Watch Backend Pump"));
    ASSERT (SUCCEEDED (hr));

    theExpressionFactory = new CExpressionFactory;

    return CWinApp::InitInstance();
}


int CCewatchbeApp::ExitInstance()
{
    delete theExpressionFactory;

    ASSERT (m_uiSMLockCount == 0);

    m_Pump.Destroy ();
    m_Pump.UnregisterCB (&m_PumpCB);

    _Module.Term();
    return CWinApp::ExitInstance();
}


HRESULT CCewatchbeApp::LockManagers ()
{
    if (m_uiSMLockCount == 0)
    {
        HRESULT hr;

        m_pSessionManager = new CCESessionManager (false);

        hr = m_SubscriptionManager.Start ();
        if (FAILED (hr))
        {
            return (hr);
        };
    };

    ++m_uiSMLockCount;

    return (S_OK);
}


HRESULT CCewatchbeApp::UnlockManagers ()
{
    ASSERT (m_uiSMLockCount != 0);
    if (m_uiSMLockCount == 0)
    {
        return (E_FAIL);
    };

    --m_uiSMLockCount;

    if (m_uiSMLockCount == 0)
    {
        HRESULT hr;

        hr = m_SubscriptionManager.Stop ();
        ASSERT (SUCCEEDED (hr));

        delete m_pSessionManager;
        m_pSessionManager = NULL;

        return (hr);
    };

    return (S_OK);
}


CSubscriptionManager *CCewatchbeApp::GetSubscriptionManager ()
{
    return (&m_SubscriptionManager);
}


CCESessionManager *CCewatchbeApp::GetSessionManager ()
{
    return (m_pSessionManager);
}


void CCewatchbeApp::AddDelayedAdapt (CExpressionNode *pExpression)
{
    HRESULT hr;

    if (m_AdaptExpressions.Find (pExpression) == NULL)
    {
        m_AdaptExpressions.AddTail (pExpression);
    };

    hr = m_Pump.Trigger ();
    ASSERT (SUCCEEDED (hr));
}


void CCewatchbeApp::RemoveDelayedAdapt (CExpressionNode *pExpression)
{
    POSITION pPosition;

    pPosition = m_AdaptExpressions.Find (pExpression);
    if (pPosition != NULL)
    {
        m_AdaptExpressions.RemoveAt (pPosition);
    };
}


void CCewatchbeApp::TriggerResubscribe ()
{
    HRESULT hr;

    m_bResubscribe = true;

    hr = m_Pump.Trigger ();
    ASSERT (SUCCEEDED (hr));
}


void CCewatchbeApp::OnPump ()
{
    AFX_MANAGE_STATE (AfxGetStaticModuleState ());
    CExpressionNode *pExpression;

    if (m_bResubscribe)
    {
        if (m_SubscriptionManager.IsInitialized ())
        {
            m_SubscriptionManager.ResubscribeAll ();
        };

        m_bResubscribe = false;
    };

    while (!m_AdaptExpressions.IsEmpty ())
    {
        pExpression = m_AdaptExpressions.RemoveHead ();
        pExpression->OnDelayedAdapt ();
    };
}


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}



