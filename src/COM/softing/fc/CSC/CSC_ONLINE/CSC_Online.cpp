/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CSC_Online.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CSC_Online.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/CSC_Online.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  14.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "resource.h"
//  CLSID declarations:
#include "CSC_Online.h"
#include "CSC_Online_i.c"

#include "CSC_OnlineServer.h"
#include "CSC_OnlineServer_i.c"
#include "CSC_OnlineServerExt.h"
#include "CSC_OnlineServerExt_i.c"

#include "CEMsg.h"
#include "CEMsg_i.c"

#include "CSC_OnlSrv.h"
#include "CSC_OnlSrv_i.c"

#include "4CVariables.h"
#include "4CVariable.h"
#include "FCSession.h"

// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!

static struct {
    CSC_SESSION_STATE key;
    LPCTSTR szDescr; 
}   StateDescTbl[] = {
	{not_connected,_T("not connected")},
	{waiting_for_connect,_T("waiting for connect")},
    {RW_established,_T("RW established")},
    {interrupted,_T("interrupted")},
    {waiting_for_exclusive,_T("waiting for exclusive")},
    {exclusive_established,_T("exclusive_established")},
    {regular_shutdown,_T("regular_shutdown")},
};

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_FCVariableCollection, CFCVariableCollection)
	OBJECT_ENTRY(CLSID_FCSessions, CFCSessions)
END_OBJECT_MAP()


const TCHAR gs_chMachineNameLeadIn  = _T('{');
const TCHAR gs_chMachineNameLeadOut = _T('}');
const TCHAR gs_szMachineNameLeadIn[]  = _T("{[");
const TCHAR gs_szMachineNameLeadOut[] = _T("}]");

//------------------------------------------------------------------*
class CCSC_OnlineApp : public CWinApp
{
public:
	virtual BOOL            InitInstance();
	virtual int             ExitInstance();
};

//------------------------------------------------------------------*
CCSC_OnlineApp theApp;

//------------------------------------------------------------------*
BOOL
    CCSC_OnlineApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance);
    ::InitTrace(m_hInstance,_T("SOFTWARE\\SOFTING\\4ControlV2\\"),_T("CSC_ONLINE"));
	return CWinApp::InitInstance();
}

//------------------------------------------------------------------*
int
    CCSC_OnlineApp::ExitInstance()
{
	_Module.Term();
    TrcTerminate();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

//------------------------------------------------------------------*
STDAPI
    DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
//------------------------------------------------------------------*
STDAPI
    DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
//------------------------------------------------------------------*
STDAPI
    DllRegisterServer(void)
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
//------------------------------------------------------------------*
STDAPI
    DllUnregisterServer(void)
{
	_Module.UnregisterServer(TRUE);
	return S_OK;
}


//------------------------------------------------------------------*
/**
 * GetLocalComputerName
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    GetLocalComputerName(CString &strName)
{
    HRESULT hr;
    DWORD   dwSize = _MAX_PATH;
    BOOL bOK = ::GetComputerName(strName.GetBuffer(dwSize), &dwSize);
    if (bOK) {
        strName.ReleaseBuffer(dwSize);
        hr = S_OK;
    }
    else {
        strName.ReleaseBuffer();
        strName.Empty();
        hr = E_FAIL;
    }
    return hr;
}


//------------------------------------------------------------------*
/**
 * SplitMachineName
 *  isolate a escaped machine name, return pure machine name with
 *  square brackets removed removed !
 *
 * @param            pszVarName     either a full path variable name or simple machine name
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    SplitMachineName(LPCTSTR pszVarName, CString &strPureMachineName, CString &strRemainder)
{
    CString strTmp;
    _ASSERTE(pszVarName != NULL);
    strTmp = pszVarName;
    strRemainder = pszVarName;

    LPTSTR  pszScan = strTmp.GetBuffer(strTmp.GetLength());

    // overread any leading whitespaces
    while ((*pszScan != EOS) && _istspace(*pszScan)) {
        pszScan++;
    }

    // must be the escape char!
    if (   (*pszScan == EOS)
        || (_tcschr(gs_szMachineNameLeadIn, *pszScan) == NULL))
    {
        return E_FAIL;
    }
    pszScan++;
    LPCTSTR pszStart = pszScan; // save ptr

    // now read on until ending escape character is found:
    while ((*pszScan != EOS) && (_tcschr(gs_szMachineNameLeadOut, *pszScan) == NULL)) {
        pszScan++;
    }
    if (*pszScan == EOS)
    {
        return E_FAIL;
    }
    *pszScan = EOS;

    strPureMachineName = pszStart;
    pszScan++;
    if (*pszScan != EOS) {
        if (*pszScan == _T('.'))
          pszScan++;
        strRemainder = pszScan;
    }
    
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * DecorateConfigName
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
CString
    DecorateConfigName(LPCTSTR pszMachineName)
{
    CString strConfig;
    strConfig  = gs_chMachineNameLeadIn;
    strConfig += pszMachineName;
    strConfig += gs_chMachineNameLeadOut;
    return strConfig;
}


//------------------------------------------------------------------*
/**
 * GetSessionStateString
 *
 * @param            state
 * @return           descriptive text for session state
 * @exception        -
 * @see              
*/
LPCTSTR GetSessionStateString(CSC_SESSION_STATE state)
{
    static TCHAR sz[] = _T("???");
    int nEntries  = FC_ARRAY_LEN(StateDescTbl);

    for(int i = 0;i<nEntries;i++) {
        if(state == StateDescTbl[i].key)
            return StateDescTbl[i].szDescr;
    }
    return sz;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: CSC_Online.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 10.03.03   Time: 9:55
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Enhanced traces.
 * 
 * *****************  Version 6  *****************
 * User: Jd           Date: 4.02.03    Time: 9:21
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Changes for new callback interface into engineering (custom online
 * commands).
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 10.04.01   Time: 18:19
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 27.03.01   Time: 16:17
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 15.03.01   Time: 19:36
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header, removed private trace files, replaced by tracea.lib
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:48
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 *==
 *----------------------------------------------------------------------------*
*/
