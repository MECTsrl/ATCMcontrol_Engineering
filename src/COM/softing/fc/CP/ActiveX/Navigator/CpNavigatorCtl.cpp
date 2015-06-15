// CpNavigatorCtl.cpp : Implementation of the CCpNavigatorCtrl OLE control class.

#include "stdafx.h"
#include "CpNavigator.h"
#include "CpNavigatorCtl.h"
#include "CpNavigatorPpg.h"
#include "CpNavTreeCtrl.h"
#include "CpNavTab.h"
#include "CpObjectList.h"
#include "CpInternet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCpNavigatorCtrl, COleControl)

/////////////////////////////////////////////////////////////////////////////
// Interface map for IObjectSafety
BEGIN_INTERFACE_MAP( CCpNavigatorCtrl, COleControl )
   INTERFACE_PART(CCpNavigatorCtrl, IID_IObjectSafety, ObjSafe)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IObjectSafety member functions
// Delegate AddRef, Release, QueryInterface
ULONG FAR EXPORT CCpNavigatorCtrl::XObjSafe::AddRef()
{
    METHOD_PROLOGUE(CCpNavigatorCtrl, ObjSafe)
    return pThis->ExternalAddRef();
}
ULONG FAR EXPORT CCpNavigatorCtrl::XObjSafe::Release()
{
    METHOD_PROLOGUE(CCpNavigatorCtrl, ObjSafe)
    return pThis->ExternalRelease();
}
HRESULT FAR EXPORT CCpNavigatorCtrl::XObjSafe::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(CCpNavigatorCtrl, ObjSafe)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

const DWORD dwSupportedBits =       INTERFACESAFE_FOR_UNTRUSTED_CALLER |
      INTERFACESAFE_FOR_UNTRUSTED_DATA;
const DWORD dwNotSupportedBits = ~ dwSupportedBits;
/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::XObjSafe::GetInterfaceSafetyOptions
// Allows container to query what interfaces are safe for what. We're
// optimizing significantly by ignoring which interface the caller is
// asking for.
HRESULT STDMETHODCALLTYPE CCpNavigatorCtrl::XObjSafe::GetInterfaceSafetyOptions( 
	/* [in] */ REFIID riid,
    /* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,
    /* [out] */ DWORD __RPC_FAR *pdwEnabledOptions)
{
   METHOD_PROLOGUE(CCpNavigatorCtrl, ObjSafe)
   HRESULT retval = ResultFromScode(S_OK);   // Does interface exist?
   IUnknown FAR* punkInterface;   retval = pThis->ExternalQueryInterface(&riid, 
               (void * *)&punkInterface);
   if (retval != E_NOINTERFACE) 
   { // interface exists
      punkInterface->Release(); // release it—just checking!   
   }   
   // We support both kinds of safety and have always both set,
   // regardless of interface.
   *pdwSupportedOptions = *pdwEnabledOptions = dwSupportedBits;
   return retval; // E_NOINTERFACE if QI failed
}

/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::XObjSafe::SetInterfaceSafetyOptions
// Since we're always safe, this is a no-brainer—but we do check to make
// sure the interface requested exists and that the options we're asked to
// set exist and are set on (we don't support unsafe mode).
HRESULT STDMETHODCALLTYPE CCpNavigatorCtrl::XObjSafe::SetInterfaceSafetyOptions( 
        /* [in] */ REFIID riid,
        /* [in] */ DWORD dwOptionSetMask,
        /* [in] */ DWORD dwEnabledOptions)
{
    METHOD_PROLOGUE(CCpNavigatorCtrl, ObjSafe)
   
   // Does interface exist?
   IUnknown FAR* punkInterface;
   pThis->ExternalQueryInterface(&riid, (void * *)&punkInterface);
   if (punkInterface) 
   { // interface exists
      punkInterface->Release(); // release it—just checking!
   }
   else 
   { // Interface doesn't exist.
      return ResultFromScode(E_NOINTERFACE);
   }
   // Can't set bits we don't support.
   if (dwOptionSetMask & dwNotSupportedBits) 
   { 
      return ResultFromScode(E_FAIL);
   }
   
   // Can't set bits we do support to zero
   dwEnabledOptions &= dwSupportedBits;
   // (We already know there are no extra bits in mask. )
   if ((dwOptionSetMask & dwEnabledOptions) != dwOptionSetMask) 
   {
      return ResultFromScode(E_FAIL);
   }                                    
   
   // Don't need to change anything since we're always safe.
   return ResultFromScode(S_OK);
}

/////////////////////////////////////////////////////////////////////////////
// Message map
BEGIN_MESSAGE_MAP(CCpNavigatorCtrl, COleControl)
	//{{AFX_MSG_MAP(CCpNavigatorCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
    ON_MESSAGE(WM_FCNAV_NEWURL, OnNewURL)
    ON_MESSAGE(WM_FCNAV_LOADSTATUS, OnLoadStatus)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCpNavigatorCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCpNavigatorCtrl)
	DISP_FUNCTION(CCpNavigatorCtrl, "GetSelectedUrl", GetSelectedUrl, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCpNavigatorCtrl, "GetSelectedServer", GetSelectedServer, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCpNavigatorCtrl, "LoadObjectList", LoadObjectList, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CCpNavigatorCtrl, "ShowObjectList", ShowObjectList, VT_ERROR, VTS_I2 VTS_I4)
	DISP_FUNCTION(CCpNavigatorCtrl, "GetObjectUrl", GetObjectUrl, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CCpNavigatorCtrl, "GetLoaderUrl", GetLoaderUrl, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CCpNavigatorCtrl, "IsObjectListLoaded", IsObjectListLoaded, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCpNavigatorCtrl, "GetObjetUrl2", GetObjetUrl2, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CCpNavigatorCtrl, "SetFocus", SetFocus, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCpNavigatorCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCpNavigatorCtrl)
	EVENT_CUSTOM("NewUrl", FireNewUrl, VTS_BSTR  VTS_BOOL  VTS_BOOL)
	EVENT_CUSTOM("LoadingReady", FireLoadingReady, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCpNavigatorCtrl, 1)
	PROPPAGEID(CCpNavigatorPropPage::guid)
END_PROPPAGEIDS(CCpNavigatorCtrl)


/////////////////////////////////////////////////////////////////////////////
// Klassenerzeugung und GUID initialisieren

// {2CF26AE0-EE63-11d1-A512-00A024363DFC}
IMPLEMENT_OLECREATE_EX(CCpNavigatorCtrl, "CPNAVIGATOR.CpNavigatorCtrl.1", 
0x2cf26ae0, 0xee63, 0x11d1, 0xa5, 0x12, 0x0, 0xa0, 0x24, 0x36, 0x3d, 0xfc);

/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCpNavigatorCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCpNavigator =
		{ 0xb3380722, 0x7157, 0x11d1, { 0xa4, 0xa8, 0, 0xa0, 0x24, 0x36, 0x3d, 0xfc } };
const IID BASED_CODE IID_DCpNavigatorEvents =
		{ 0xb3380723, 0x7157, 0x11d1, { 0xa4, 0xa8, 0, 0xa0, 0x24, 0x36, 0x3d, 0xfc } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCpNavigatorOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCpNavigatorCtrl, IDS_CPNAVIGATOR, _dwCpNavigatorOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::CCpNavigatorCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCpNavigatorCtrl

BOOL CCpNavigatorCtrl::CCpNavigatorCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CPNAVIGATOR,
			IDB_CPNAVIGATOR,
			afxRegInsertable | afxRegApartmentThreading,
			_dwCpNavigatorOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::CCpNavigatorCtrl - Constructor

CCpNavigatorCtrl::CCpNavigatorCtrl()
{
	InitializeIIDs(&IID_DCpNavigator, &IID_DCpNavigatorEvents);

	m_pObjectList = new CCpObjectList;
	
	m_pStatusLine = new CStatic;

	m_pSheet = new CCpNavTab;
	m_pTreeCtrl[0] = new CCpNavTreeCtrl;
	m_pTreeCtrl[1] = new CCpNavTreeCtrl;
//	m_pTreeCtrl[2] = new CCpNavTreeCtrl;

	m_bServerUrl = FALSE;
	m_sUrl.Empty();

	m_bObjectListLoaded=FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::~CCpNavigatorCtrl - Destructor

CCpNavigatorCtrl::~CCpNavigatorCtrl()
{
	// TODO: Cleanup your control's instance data here.
//	ASSERT_VALID(m_pTreeCtrl[2]);
//	delete m_pTreeCtrl[2];
	
	ASSERT_VALID(m_pTreeCtrl[1]);
	delete m_pTreeCtrl[1];
	
	ASSERT_VALID(m_pTreeCtrl[0]);	
	delete m_pTreeCtrl[0];

	ASSERT_VALID(m_pSheet);	
	delete m_pSheet;

	ASSERT_VALID(m_pStatusLine);	
	delete m_pStatusLine;

	if (m_pObjectList != NULL)
		delete m_pObjectList;
}

/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::DoPropExchange - Persistence support

void CCpNavigatorCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl::OnResetState - Reset control to default state

void CCpNavigatorCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl message handlers

//*****************************************************************************	
LRESULT CCpNavigatorCtrl::OnLoadStatus(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	CString msg;
	switch (wParam)
	{
	case FCNAV_LOADSTATUS_READY:
		FireLoadingReady();
		if (lParam != NULL)
		{
			CCpObjectList *pO = (CCpObjectList *)lParam;
			msg.Format(IDS_STATUS_READY, pO->GetObjectCount());
		}
		else
		{
			msg.Format(IDS_STATUS_READY, 0);
		}
		break;

	case FCNAV_LOADSTATUS_LOADING:
		if (lParam != NULL)
		{
			CCpObjectList *pO = (CCpObjectList *)lParam;
			msg.Format(IDS_STATUS_LOADING, (LPCTSTR)pO->GetName(), pO->GetObjectCount());
		}
		break;

	case FCNAV_LOADSTATUS_ABORTED:
		FireLoadingReady();
		if (lParam != NULL)
		{
			CCpObjectList *pO = (CCpObjectList *)lParam;
			msg.Format(IDS_STATUS_ABORTED, pO->GetObjectCount());
		}
		break;
	}
	
	m_pStatusLine->SetWindowText(" " + msg);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Called by the tree control if a new hyperlink has been selected
//*****************************************************************************	
LRESULT CCpNavigatorCtrl::OnNewURL(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	BOOL bNewWindow = (wParam & PARAM_NEW_WINDOW) != 0;

	m_bServerUrl = FALSE;
	m_sUrl.Empty();

//	HTREEITEM hItem = m_pTreeCtrl[m_pSheet->GetCurSel()]->GetSelectedItem();
	HTREEITEM hItem = (HTREEITEM)lParam;
	if (hItem != NULL)
	{
		const CCpObjectList* pObj = m_pTreeCtrl[m_pSheet->GetCurSel()]->GetObject(hItem);

		if (!pObj->GetChildrenFile().IsEmpty() && !pObj->ChildrenLoaded())
		{
			ASSERT(pObj->GetServer().GetLength() > 0);
			m_bServerUrl = TRUE;
			m_sUrl = CString("http://") + pObj->GetServer() + CString("/4c");
		}
		else
		{
			m_sUrl = pObj->GetAspUrl();
			if (!m_sUrl.IsEmpty())
			{
				CString NextChar = "?";
				const CString ConfFile = pObj->GetConfFile();
				if (ConfFile.GetLength() > 0)
				{
					m_sUrl += NextChar + "ConfFile=";

					// complete path it is an xml file
					CString testExt = ConfFile.Right(4);
					testExt.MakeUpper();
					if (ConfFile.GetLength() > 4 && testExt.Find(".XML") == 0)
						m_sUrl += CString("http://") + pObj->GetServer();

					m_sUrl += "/4c/" + pObj->GetConfFile();
					NextChar = "&";
				}
				if (pObj->GetName().GetLength() > 0)
				{
					m_sUrl += NextChar + "ObjName=" + pObj->GetName();
					NextChar = "&";
				}
				if (pObj->GetClassName().GetLength() > 0)
				{
					m_sUrl += NextChar + "Class=" + pObj->GetClassName();
					NextChar = "&";
				}
			}
		}
	}
	
	if (m_sUrl.GetLength() > 0)		
		FireNewUrl(m_sUrl, m_bServerUrl, bNewWindow);
	
	return 0;
}

SCODE CCpNavigatorCtrl::LoadObjectList(LPCTSTR sUrl) 
{
	m_bObjectListLoaded=FALSE;
	m_pObjectList->LoadAsync(sUrl, this);
	m_bObjectListLoaded=TRUE;
	return S_OK;
}

SCODE CCpNavigatorCtrl::ShowObjectList(short iPage, long iFilter) 
{
	ASSERT(iPage < sizeof(m_pTreeCtrl));
	if (iPage < sizeof(m_pTreeCtrl))
	{
		if (m_pObjectList != NULL)
		{
			ASSERT_VALID(m_pTreeCtrl[iPage]);

			unsigned InitialDisplayLevel = 3;
			if (iFilter != 0)
				InitialDisplayLevel = 2;

			m_pTreeCtrl[iPage]->ShowObjectList(m_pObjectList, InitialDisplayLevel, iFilter);
			return S_OK;
		}
	}
	return S_FALSE;
}

BSTR CCpNavigatorCtrl::GetSelectedServer() 
{
	HTREEITEM hItem = m_pTreeCtrl[m_pSheet->GetCurSel()]->GetSelectedItem();
	if (hItem != NULL)
	{
		const CCpObjectList* pObj = m_pTreeCtrl[m_pSheet->GetCurSel()]->GetObject(hItem);
		if (pObj != NULL && pObj->GetServer().GetLength() > 0)
		{
			return pObj->GetServer().AllocSysString();
		}
	}
	return CString("").AllocSysString();
}

BSTR CCpNavigatorCtrl::GetSelectedUrl() 
{
	return m_sUrl.AllocSysString();
}

int CCpNavigatorCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	ASSERT_VALID(m_pSheet);
	
	CRect Rect, RectStatus, RectTree;
	GetClientRect(Rect);

	m_pSheet->Create(WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | TCS_TABS, Rect, this, 1);
	m_pSheet->AddItem(m_pTreeCtrl[0], IDS_CPNAVIGATOR_TAB_PROJECT);
	m_pSheet->AddItem(m_pTreeCtrl[1], IDS_CPNAVIGATOR_TAB_FB);	
//	m_pSheet->AddItem(m_pTreeCtrl[2], IDS_CPNAVIGATOR_TAB_VAR);	

	TEXTMETRIC tm;
    CDC	*pDC = m_pSheet->GetDC();
    pDC->GetTextMetrics(&tm);
	ReleaseDC(pDC);

	int h = tm.tmHeight;
	RectStatus.SetRect(0, Rect.Height()-h, Rect.Width(), Rect.Height());
	RectTree.SetRect(0, 0, Rect.Width(), Rect.Height() - h);
	
	m_pSheet->SetWindowPos(NULL, 0, 0, RectTree.Width(), RectTree.Height(), SWP_NOMOVE); 

	m_pStatusLine->Create("", WS_EX_STATICEDGE | SS_LEFTNOWORDWRAP , RectStatus, this);
	m_pStatusLine->SetFont(m_pSheet->GetFont());


	m_pSheet->ShowWindow(SW_SHOW);	
	m_pStatusLine->ShowWindow(SW_SHOW);

	CString msg;
	msg.LoadString(IDS_STATUS_INIT);	
	m_pStatusLine->SetWindowText(" " + msg);


//	LPCSTR pstrFile  = "http://d-s-1/intranet/CpGenTestMk/config/objects.txt";
//	LoadObjectList(pstrFile, 0, 0);
//	CCpInternet in;
//	in.WriteFile("d:/cp/test.txt", "ftp://d-s-1/test.txt");

	return 0;
}

void CCpNavigatorCtrl::OnDestroy() 
{
	// stop load thread
	m_pObjectList->StopLoadAsync();

	COleControl::OnDestroy();
	
	// TODO: Add your message handler code here
	ASSERT_VALID(m_pSheet);	
	m_pSheet->DeleteAllItems();
	m_pSheet->DestroyWindow();
}

void CCpNavigatorCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (IsWindow(m_pSheet->m_hWnd) && IsWindow(m_pStatusLine->m_hWnd))
	{
		CRect RectStatus;
		m_pStatusLine->GetWindowRect(RectStatus);
		int h = RectStatus.Height();
		m_pStatusLine->SetWindowPos(NULL, 0, cy-h, cx, h, 0); 
		m_pSheet->SetWindowPos(NULL, 0, 0, cx, cy-h, SWP_NOMOVE); 
	}
}




BSTR CCpNavigatorCtrl::GetObjectUrl(LPCTSTR sName) 
{
	CString strReturn("");
	if (m_pObjectList==0)
		return strReturn.AllocSysString();

	//
	// search object
	//
	CString strName(sName);
	CCpObjectList* pObj=m_pObjectList->FindObjectByPath(strName);
	
	//
	// get servername
	//
	CString strServer;
	CCpObjectList* pRoot=m_pObjectList->GetRootObject();
	if (pRoot)
	{
		strServer=pRoot->GetServer();
	}

	//
	// make url string
	//
	if (pObj)
	{
		CString strAsp=strServer+"/4c/" + pObj->GetAspFile();
		CString strConfFile="/4c/" + pObj->GetConfFile();
		CString strName=pObj->GetName();
		CString strClassName=pObj->GetClassName();
		strReturn = strAsp + "?" + "ConfFile="+strConfFile + "&ObjName="+strName + "&ClassName="+strClassName;
		strReturn.Replace('\\','/');
	}
	return strReturn.AllocSysString();
}

BSTR CCpNavigatorCtrl::GetLoaderUrl(LPCTSTR sName) 
{
	CString strReturn("");
	if (m_pObjectList==0)
		return strReturn.AllocSysString();

	//
	// search object
	//
	CString strName(sName);
	CCpObjectList* pObj=m_pObjectList->FindObjectByPath(strName);
	
	//
	// get servername
	//
	CString strServer;
	CCpObjectList* pRoot=m_pObjectList->GetRootObject();
	if (pRoot)
	{
		strServer=pRoot->GetServer();
	}

	//
	// make loader url string
	//
	if (pObj)
	{
		CString strLoader=strServer+"/4c/CP_Loader.asp";
		CString strAsp="/4c/" + pObj->GetAspFile();
		CString strConfFile="/4c/" + pObj->GetConfFile();
		CString strName=pObj->GetName();
		CString strClassName=pObj->GetClassName();
		strReturn = strLoader +"?AspFile="+strAsp + "&" + "ConfFile="+strConfFile + "&ObjName="+strName + "&ClassName="+strClassName;
		strReturn.Replace('\\','/');
	}
	return strReturn.AllocSysString();
}

BOOL CCpNavigatorCtrl::IsObjectListLoaded() 
{
	if ( m_bObjectListLoaded==FALSE)
		return FALSE;
	
	if (!m_pObjectList)
		return FALSE;
	else
		return !m_pObjectList->IsLoading();

}

BSTR CCpNavigatorCtrl::GetObjetUrl2(LPCTSTR sName) 
{
	CString strReturn("");
	if (m_pObjectList==0)
		return strReturn.AllocSysString();

	//
	// search object
	//
	CString strName(sName);
	CCpObjectList* pObj=m_pObjectList->FindObjectByPath(strName);
	
	//
	// make url string
	//
	if (pObj)
	{
		CString strAsp=pObj->GetAspFile();
		CString strConfFile="/4c/" + pObj->GetConfFile();
		CString strName=pObj->GetName();
		CString strClassName=pObj->GetClassName();
		strReturn = strAsp + "?" + "ConfFile="+strConfFile + "&ObjName="+strName + "&ClassName="+strClassName;
		strReturn.Replace('\\','/');
	}
	return strReturn.AllocSysString();
}

/*
void CCpNavigatorCtrl::DeleteJVMTmpFiles() 
{
	_TCHAR tszBuf[MAX_PATH];
	if (0 == GetTempPath(MAX_PATH, tszBuf))
	{
		ASSERT(FALSE);
		return;
	}

	_tcscat(tszBuf, _T("//JVM*.tmp"));

	WIN32_FIND_DATA FileData;
	HANDLE hff = FindFirstFile(tszBuf, &FileData);
	if (hff != INVALID_HANDLE_VALUE)
	{
		do
		{
			remove(FileData.cFileName);
		}
		while (FindNextFile(hff, &FileData));
	}
	FindClose(hff);
	return;
}
*/

void CCpNavigatorCtrl::SetFocus() 
{
	CWnd *pWnd = m_pTreeCtrl[m_pSheet->GetCurSel()];
//	CWnd *pWnd = m_pSheet;
	if (pWnd != NULL && IsWindow(pWnd->m_hWnd))
		pWnd->SetFocus();
}
