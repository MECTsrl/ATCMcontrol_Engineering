
// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f DbgInfops.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "DbgInfo.h"
#include "DBIHolder.h"

#include "DbgInfo_i.c"
#include "DebugInfoServer.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
    OBJECT_ENTRY(CLSID_DebugInfoServer, CDebugInfoServer)
END_OBJECT_MAP()

class CDbgInfoApp : public CWinApp
{
public:
    void AddDBIHolder (CDBIHolder *pDBIHolder);
    void RemoveDBIHolder (CDBIHolder *pDBIHolder);
    CDBIHolder *LookupDBIHolder (const CString &strDebugInfoPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbgInfoApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDbgInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    CList<CDBIHolder *, CDBIHolder *> m_DBIHolders;
};

BEGIN_MESSAGE_MAP(CDbgInfoApp, CWinApp)
	//{{AFX_MSG_MAP(CDbgInfoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDbgInfoApp theApp;


void DBIAddHolder (CDBIHolder *pDBIHolder)
{
    theApp.AddDBIHolder (pDBIHolder);
}


void DBIRemoveHolder (CDBIHolder *pDBIHolder)
{
    theApp.RemoveDBIHolder (pDBIHolder);
}


CDBIHolder *DBILookupHolder (const CString &strDebugInfoPath)
{
    return (theApp.LookupDBIHolder (strDebugInfoPath));
}


BOOL CDbgInfoApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_DBGINFOLib);
    return CWinApp::InitInstance();
}

int CDbgInfoApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}


void CDbgInfoApp::AddDBIHolder (CDBIHolder *pDBIHolder)
{
    m_DBIHolders.AddTail (pDBIHolder);
}


void CDbgInfoApp::RemoveDBIHolder (CDBIHolder *pDBIHolder)
{
    POSITION pPosition;

    pPosition = m_DBIHolders.Find (pDBIHolder);
    if (pPosition != NULL)
    {
        m_DBIHolders.RemoveAt (pPosition);
    };
}


CDBIHolder *CDbgInfoApp::LookupDBIHolder (const CString &strDebugInfoPath)
{
    POSITION pPosition;

    pPosition = m_DBIHolders.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CDBIHolder *pHolder;

        pHolder = m_DBIHolders.GetNext (pPosition);
        ASSERT (pHolder != NULL);
        if (pHolder == NULL)
            continue;

        if (strDebugInfoPath.CompareNoCase (pHolder->GetDebugInfoPath ()) == 0)
        {
            pHolder->Reference ();
            return (pHolder);
        };
    };

    return (NULL);
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
    HRESULT hr = Create4C_CATIDs();
    ASSERT(SUCCEEDED(hr));
    if (FAILED(hr)) {
        return hr;
    }
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}