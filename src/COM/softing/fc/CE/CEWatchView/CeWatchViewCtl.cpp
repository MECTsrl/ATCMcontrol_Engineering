


#include "stdafx.h"
#include "CeWatchView.h"
#include "CeWatchViewCtl.h"
#include "CeWatchViewPpg.h"
#include "XMLHelper/XMLDocument.h"
#include "XMLHelper/XMLNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCeWatchViewCtrl, COleControl)


#define CE_PSET_ELEM_PANE          _T("PANE")

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCeWatchViewCtrl, COleControl)
	//{{AFX_MSG_MAP(CCeWatchViewCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCeWatchViewCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCeWatchViewCtrl)
	DISP_FUNCTION(CCeWatchViewCtrl, "SetProjectPath", SetProjectPath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CCeWatchViewCtrl, "Store", Store, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CCeWatchViewCtrl, "Load", Load, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CCeWatchViewCtrl, "ForceResubscribe", ForceResubscribe, VT_EMPTY, NULL)
	DISP_FUNCTION(CCeWatchViewCtrl, "SaveToXMLString", SaveToXMLString, VT_ERROR, VTS_PBSTR)
	DISP_FUNCTION(CCeWatchViewCtrl, "LoadFromXMLString", LoadFromXMLString, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CCeWatchViewCtrl, "LoadFromXMLString2", LoadFromXMLString2, VT_ERROR, VTS_PBSTR)
    DISP_FUNCTION(CCeWatchViewCtrl, "Deactivate", Deactivate, VT_EMPTY, NULL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCeWatchViewCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCeWatchViewCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCeWatchViewCtrl, 1)
	PROPPAGEID(CCeWatchViewPropPage::guid)
END_PROPPAGEIDS(CCeWatchViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCeWatchViewCtrl, "4C_20_CE.WatchView.1",
	0xD735F1A0, 0xA452, 0x4cde, 0x83, 0x5F, 0xA8, 0x08,0xC1, 0x04, 0x7A, 0x6A)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCeWatchViewCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCeWatchView =
        { 0xB6FB4AD1, 0xA1A7, 0x47d0, { 0xB3, 0x39, 0xFC, 0xA0, 0xAF, 0x92, 0xF0, 0x20 } };
const IID BASED_CODE IID_DCeWatchViewEvents =
        { 0xD40CB30D, 0x80EB, 0x4d27, { 0x82, 0x7A, 0xA6, 0x50, 0x57, 0x6F, 0xBA, 0xBE } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCeWatchViewOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCeWatchViewCtrl, IDS_CEWATCHVIEW, _dwCeWatchViewOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl::CCeWatchViewCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCeWatchViewCtrl

BOOL CCeWatchViewCtrl::CCeWatchViewCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_CEWATCHVIEW,
			IDB_CEWATCHVIEW,
			afxRegInsertable | afxRegApartmentThreading,
			_dwCeWatchViewOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl::CCeWatchViewCtrl - Constructor

CCeWatchViewCtrl::CCeWatchViewCtrl()
{
	InitializeIIDs(&IID_DCeWatchView, &IID_DCeWatchViewEvents);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl::~CCeWatchViewCtrl - Destructor

CCeWatchViewCtrl::~CCeWatchViewCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl::OnDraw - Drawing function

void CCeWatchViewCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{

}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl::DoPropExchange - Persistence support

void CCeWatchViewCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl::OnResetState - Reset control to default state

void CCeWatchViewCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl message handlers

int CCeWatchViewCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Dlg.Create(IDD_MAINDLG, this);
	return 0;
}

void CCeWatchViewCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);
//	rc.right -= 10;
	m_Dlg.MoveWindow(rc);
}

void CCeWatchViewCtrl::OnSetFocus (CWnd *pOldWnd)
{
    COleControl::OnSetFocus (pOldWnd);

    m_Dlg.SetFocus ();
}

void CCeWatchViewCtrl::SetProjectPath(LPCTSTR path) 
{
	m_Dlg.SetProjectPath(path);
}

BOOL CCeWatchViewCtrl::Store(LPCTSTR file) 
{
	return m_Dlg.Store(file);
}

BOOL CCeWatchViewCtrl::Load(LPCTSTR file) 
{
	return m_Dlg.Load(file);
}

void CCeWatchViewCtrl::ForceResubscribe()
{
    m_Dlg.ForceResubscribe();
}


SCODE CCeWatchViewCtrl::SaveToXMLString(BSTR* psXMLString) 
{
    CString strXMLString;
    CXMLDocument    tXMLDoc;
    CXMLNode        tNodeTmp;
    CXMLNode        tDocNode;

    if(tXMLDoc.GetDocNode(tDocNode))
    {
        if(tXMLDoc.CreateNode(tNodeTmp, CE_PSET_ELEM_PANE))
        {
            tDocNode.AppendChild(tNodeTmp);
            if(m_Dlg.SaveToNode(tNodeTmp, tXMLDoc))
            {
                tXMLDoc.GetXMLString(strXMLString);
                *psXMLString = strXMLString.AllocSysString();
            }
        }
    }
	return S_OK;
}


SCODE CCeWatchViewCtrl::LoadFromXMLString(LPCTSTR sXMLString) 
{
    CString strXMLString(sXMLString);
    CXMLDocument    tXMLDoc;
    tXMLDoc.LoadXMLString(strXMLString);
    CXMLNode        tNodeTmp;
    if(tXMLDoc.GetRootNode(tNodeTmp))
    {
        if(!m_Dlg.LoadFromNode(tNodeTmp))
        {
            return E_FAIL;
        }
    }
	return S_OK;
}


SCODE CCeWatchViewCtrl::LoadFromXMLString2(BSTR* psXMLString) 
{
    CString strXMLString(*psXMLString);
    CXMLDocument    tXMLDoc;
    tXMLDoc.LoadXMLString(strXMLString);
    CXMLNode        tNodeTmp;
    if(tXMLDoc.GetRootNode(tNodeTmp))
    {
        if(!m_Dlg.LoadFromNode(tNodeTmp))
        {
            return E_FAIL;
        }
    }
	return S_OK;
}


void CCeWatchViewCtrl::Deactivate() 
{
    m_Dlg.Deactivate();
}

BOOL CCeWatchViewCtrl::PreTranslateMessage(MSG* pMsg) 
{
    HWND hWnd;
    HWND hParentWnd;

    // search for window below control which is a parent of pMsg->hwnd
    hWnd = pMsg->hwnd;
    while ((hParentWnd = ::GetParent (hWnd)) != NULL && hParentWnd != m_hWnd)
    {
        hWnd = hParentWnd;
    }

    if (hParentWnd == NULL)
    {
        return (FALSE);
    }

    if (WalkPreTranslateTree(hWnd, pMsg))
    {
        return (TRUE);
    }

    return COleControl::PreTranslateMessage(pMsg);
}
