


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// CntrItem.cpp : implementation of the CGenEdCntrItem class
//

#include "stdafx.h"
#include "CEMain.h"

#include "GenEdDoc.h"
#include "GenEdView.h"
#include "GenEdCntrItem.h"
#include "StdEditorIF.h"
#include "mainfrm.h"
#include "MsgWnd.h"
#include "CECompMan.h"
#include "CEProjInfo.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if _MFC_VER == 0x0421

/////////////////////////////////////////////////////////////////////////////
// COleDocObjectItem


// nice things from oleimpl2.h

LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID riid);
#define QUERYINTERFACE(lpUnknown, iface) \
	(iface*)_AfxQueryInterface(lpUnknown, IID_##iface)

#define RELEASE(lpUnk) do \
	{ if ((lpUnk) != NULL) { (lpUnk)->Release(); (lpUnk) = NULL; } } while (0)

HMENU AFXAPI AfxMergeMenus(HMENU hMenuShared, HMENU hMenuSource,
	LONG* lpMenuWidths, int iWidthIndex, BOOL bMergeHelpMenus = FALSE);
void AFXAPI AfxUnmergeMenus(HMENU hMenuShared, HMENU hMenuSource,
	HMENU hHelpMenuPopup = NULL);

DVTARGETDEVICE* AFXAPI _AfxOleCreateTargetDevice(LPPRINTDLG lpPrintDlg);
DVTARGETDEVICE* AFXAPI _AfxOleCreateTargetDevice(LPDEVNAMES pDN, LPDEVMODE pDM);

//only iff AFXDLL defined
#ifdef _AFXDLL

typedef LPVOID* LPLP;
#define DEVNAMEPART(pdn, x) (pdn->x == 0 ? NULL : (LPCTSTR)pdn + pdn->x)

LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID iid)
{
ASSERT(lpUnknown != NULL);

LPUNKNOWN lpW = NULL;
if (lpUnknown->QueryInterface(iid, (LPLP)&lpW) != S_OK)
return NULL;

return lpW;
}


DVTARGETDEVICE* AFXAPI _AfxOleCreateTargetDevice(LPDEVNAMES pDN, LPDEVMODE
pDM)
{
USES_CONVERSION;

DVTARGETDEVICE* ptd = NULL;
DWORD dwDevNamesSize, dwDevModeSize, dwPtdSize;

LPCTSTR lpszDriverName = DEVNAMEPART(pDN, wDriverOffset);
LPCTSTR lpszDeviceName = DEVNAMEPART(pDN, wDeviceOffset);
LPCTSTR lpszPortName = DEVNAMEPART(pDN, wOutputOffset);

LPCOLESTR lpszDriverNameOle = T2COLE(lpszDriverName);
LPCOLESTR lpszDeviceNameOle = T2COLE(lpszDeviceName);
LPCOLESTR lpszPortNameOle = T2COLE(lpszPortName);
int nDriverNameSize = (lpszDriverNameOle == NULL) ? 0 :
(ocslen(lpszDriverNameOle)+1)*sizeof(OLECHAR);
int nDeviceNameSize = (lpszDeviceNameOle == NULL) ? 0 :
(ocslen(lpszDeviceNameOle)+1)*sizeof(OLECHAR);
int nPortNameSize = (lpszPortNameOle == NULL) ? 0 :
(ocslen(lpszPortNameOle)+1)*sizeof(OLECHAR);

LPDEVMODEOLE lpDevModeOle = DEVMODET2OLE(pDM);

dwDevNamesSize = nDriverNameSize + nDeviceNameSize + nPortNameSize;
dwDevModeSize = (DWORD)(lpDevModeOle->dmSize + lpDevModeOle->dmDriverExtra);

dwPtdSize = sizeof(DVTARGETDEVICE) + dwDevNamesSize + dwDevModeSize;

if ((ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(dwPtdSize)) != NULL)
{
// copy in the info
ptd->tdSize = (UINT)dwPtdSize;

ptd->tdDriverNameOffset = sizeof(DVTARGETDEVICE);
ocscpy((LPOLESTR)((BYTE*)ptd + ptd->tdDriverNameOffset), lpszDriverNameOle);
ptd->tdDeviceNameOffset = (WORD)(ptd->tdDriverNameOffset + nDriverNameSize);
ocscpy((LPOLESTR)((BYTE*)ptd + ptd->tdDeviceNameOffset), lpszDeviceNameOle);
ptd->tdPortNameOffset = (WORD)(ptd->tdDeviceNameOffset + nDeviceNameSize);
ocscpy((LPOLESTR)((BYTE*)ptd + ptd->tdPortNameOffset), lpszPortNameOle);
ptd->tdExtDevmodeOffset = (WORD)(ptd->tdPortNameOffset + nPortNameSize);
memcpy((BYTE*)ptd + ptd->tdExtDevmodeOffset, lpDevModeOle,
sizeof(DEVMODEOLE)+lpDevModeOle->dmDriverExtra);
}
return ptd;
}

#endif //AFXDLL

BOOL AFXAPI AfxConnectionAdvise(LPUNKNOWN pUnkSrc, REFIID iid,
	LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD* pdwCookie)
{
	ASSERT_POINTER(pUnkSrc, IUnknown);
	ASSERT_POINTER(pUnkSink, IUnknown);
	ASSERT_POINTER(pdwCookie, DWORD);

	BOOL bSuccess = FALSE;

	LPCONNECTIONPOINTCONTAINER pCPC;

	if (SUCCEEDED(pUnkSrc->QueryInterface(
					IID_IConnectionPointContainer,
					(LPVOID*)&pCPC)))
	{
		ASSERT_POINTER(pCPC, IConnectionPointContainer);

		LPCONNECTIONPOINT pCP;

		if (SUCCEEDED(pCPC->FindConnectionPoint(iid, &pCP)))
		{
			ASSERT_POINTER(pCP, IConnectionPoint);

			if (SUCCEEDED(pCP->Advise(pUnkSink, pdwCookie)))
				bSuccess = TRUE;

			pCP->Release();

			// The connection point just AddRef'ed us.  If we don't want to
			// keep this reference count (because it would prevent us from
			// being deleted; our reference count wouldn't go to zero), then
			// we need to cancel the effects of the AddRef by calling
			// Release.

			if (bSuccess && !bRefCount)
				pUnkSink->Release();
		}

		pCPC->Release();
	}

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
// AfxConnectionUnadvise

BOOL AFXAPI AfxConnectionUnadvise(LPUNKNOWN pUnkSrc, REFIID iid,
	LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD dwCookie)
{
	ASSERT_POINTER(pUnkSrc, IUnknown);
	ASSERT_POINTER(pUnkSink, IUnknown);

	// When we call Unadvise, the connection point will Release us.  If we
	// didn't keep the reference count when we called Advise, we need to
	// AddRef now, to keep our reference count consistent.  Note that if
	// the Unadvise fails, then we need to undo this extra AddRef by
	// calling Release before we return.

	if (!bRefCount)
		pUnkSink->AddRef();

	BOOL bSuccess = FALSE;

	LPCONNECTIONPOINTCONTAINER pCPC;

	if (SUCCEEDED(pUnkSrc->QueryInterface(
					IID_IConnectionPointContainer,
					(LPVOID*)&pCPC)))
	{
		ASSERT_POINTER(pCPC, IConnectionPointContainer);

		LPCONNECTIONPOINT pCP;

		if (SUCCEEDED(pCPC->FindConnectionPoint(iid, &pCP)))
		{
			ASSERT_POINTER(pCP, IConnectionPoint);

			if (SUCCEEDED(pCP->Unadvise(dwCookie)))
				bSuccess = TRUE;

			pCP->Release();
		}

		pCPC->Release();
	}

	// If we failed, undo the earlier AddRef.

	if (!bRefCount && !bSuccess)
		pUnkSink->Release();

	return bSuccess;
}


IMPLEMENT_DYNAMIC(COleDocObjectItem, COleClientItem)

BEGIN_INTERFACE_MAP(COleDocObjectItem, COleClientItem)
	INTERFACE_PART(COleDocObjectItem, IID_IOleDocumentSite, OleDocumentSite)
END_INTERFACE_MAP()


COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc)
	: COleClientItem(pContainerDoc)
{
	m_pHelpPopupMenu = NULL;
	m_pActiveView = NULL;
	m_pIPrint = NULL;
	m_bInHelpMenu = FALSE;
}

COleDocObjectItem::~COleDocObjectItem()
{
	if (m_pHelpPopupMenu != NULL)
		m_pHelpPopupMenu->RemoveMenu(0, MF_BYPOSITION);
	delete m_pHelpPopupMenu;
}


/////////////////////////////////////////////////////////////////////////////
// IOleDocumentSite interface

STDMETHODIMP_(ULONG) COleDocObjectItem::XOleDocumentSite::AddRef()
{
	METHOD_PROLOGUE_EX(COleDocObjectItem, OleDocumentSite)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleDocObjectItem::XOleDocumentSite::Release()
{
	METHOD_PROLOGUE_EX(COleDocObjectItem, OleDocumentSite)
	return pThis->ExternalRelease();
}

STDMETHODIMP COleDocObjectItem::XOleDocumentSite::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX(COleDocObjectItem, OleDocumentSite)
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleDocObjectItem::XOleDocumentSite::ActivateMe(
	LPOLEDOCUMENTVIEW pViewToActivate)
{
	METHOD_PROLOGUE_EX(COleDocObjectItem, OleDocumentSite)

	LPOLEDOCUMENT lpDocument;
	LPOLECLIENTSITE lpClientSite = pThis->GetClientSite();
	LPOLEINPLACESITE lpInPlaceSite =
		(LPOLEINPLACESITE) pThis->GetInterface(&IID_IOleInPlaceSite);

	if (lpClientSite == NULL || lpInPlaceSite == NULL)
		return E_FAIL;

	// if we've gotten a NULL view, we're to create one ourselves

	if (pViewToActivate == NULL)
	{
		// if we already have a view, we can simply activate it

		if (pThis->m_pActiveView != NULL && pThis->m_pView != NULL)
		{
			pThis->ActivateAndShow();
			return NOERROR;
		}

		ASSERT(pThis->m_lpObject != NULL);
		if (pThis->m_lpObject == NULL)
			return E_FAIL;

		lpDocument = QUERYINTERFACE(pThis->m_lpObject, IOleDocument);
		if (lpDocument == NULL)
			return E_FAIL;

		if (FAILED(
			lpDocument->CreateView(lpInPlaceSite, NULL, 0, &pViewToActivate)))
		{
			lpDocument->Release();
		return E_OUTOFMEMORY;
		}

		// we're done with the document pointer
		lpDocument->Release();
	}
	else if (pThis->m_pActiveView != NULL && pThis->m_pActiveView == pViewToActivate)
	{
		// we already own this view, so no need to addref
		// simply make it visible and resize it

		pThis->ActivateAndShow();
		return NOERROR;
	}
	else
	{
		// set the in-place site
		pViewToActivate->SetInPlaceSite(lpInPlaceSite);
		pViewToActivate->AddRef();
	}

	// it must've created
	ASSERT(pThis->m_pView != NULL);

	// if we had an old one, release it
	if (pThis->m_pActiveView != NULL)
	{
		pThis->m_pActiveView->Show(FALSE);
		pThis->m_pActiveView->UIActivate(FALSE);
		pThis->m_pActiveView->Release();
		RELEASE(pThis->m_pIPrint);
	}

	// remember it for later
	pThis->m_pActiveView = pViewToActivate;

	// activate and position it
	pThis->ActivateAndShow();

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
// IOleDocumentSite implementation helper

void COleDocObjectItem::ActivateAndShow()
{
	// set up toolbars and menus for the object
	m_pActiveView->UIActivate(TRUE);

	// set the window size, avoiding new toolbars
	RECT rc;
	m_pView->GetClientRect(&rc);
	m_pActiveView->SetRect(&rc);

	// make everything visible
	m_pActiveView->Show(TRUE);

	return;
}

void COleDocObjectItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	COleClientItem::Serialize(ar);
}

LPOLEDOCUMENTVIEW COleDocObjectItem::GetActiveView() const
{
	return m_pActiveView;
}

void COleDocObjectItem::Release(OLECLOSE dwCloseOption)
{
	RELEASE(m_pActiveView);
	RELEASE(m_pIPrint);
	COleClientItem::Release(dwCloseOption);
}

HRESULT COleDocObjectItem::ExecCommand(DWORD nCmdID, DWORD nCmdExecOpt,
	const GUID* pguidCmdGroup)
{
	LPOLECOMMANDTARGET lpCt = QUERYINTERFACE(m_lpObject, IOleCommandTarget);
	HRESULT hr = E_NOTIMPL;

	if (lpCt != NULL)
		lpCt->Exec(pguidCmdGroup, nCmdID, nCmdExecOpt, NULL, NULL);

	RELEASE(lpCt);
	return hr;
}

BOOL COleDocObjectItem::SupportsIPrint()
{
	if (m_pIPrint != NULL)
		return TRUE;

	m_pIPrint = QUERYINTERFACE(m_lpObject, IPrint);
	return (m_pIPrint != NULL);
}

BOOL COleDocObjectItem::GetPageCount(LPLONG pnFirstPage, LPLONG pcPages)
{
	if (m_pIPrint == NULL && !SupportsIPrint())
		return FALSE;

	//WINBUG: The proxy in DOCOBJ.DLL is broken; it doesn't allow
	// NULL parameters to IPrint::GetPageInfo(), even though the spec
	// says it should.

	LONG lPages;
	LONG lFirstPage;

	HRESULT hr = m_pIPrint->GetPageInfo(&lFirstPage, &lPages);

	if (pnFirstPage != NULL)
		*pnFirstPage = lFirstPage;
	if (pcPages != NULL)
		*pcPages = lPages;

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}


CMenu* COleDocObjectItem::GetHelpMenu(UINT& nPosition)
{
	CFrameWnd* pFrame = m_pView->GetTopLevelFrame();
	CMenu* pMenuFrame = CMenu::FromHandle(pFrame->m_hMenuDefault);

	if (pMenuFrame != NULL)
		nPosition = pMenuFrame->GetMenuItemCount() -1;

	return pMenuFrame;
}

void COleDocObjectItem::OnInsertMenus(CMenu* pMenuShared,
	LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pMenuShared);
	ASSERT(AfxIsValidAddress(lpMenuWidths, sizeof(OLEMENUGROUPWIDTHS)));

	// initialize the group widths array
	lpMenuWidths->width[0] = 1;
	lpMenuWidths->width[2] = 0;
	lpMenuWidths->width[4] = 0;

	// get menu from document template
	CDocTemplate* pTemplate = GetDocument()->GetDocTemplate();
	HMENU hMenuOLE = pTemplate->m_hMenuInPlace;

	// only copy the popups if there is a menu loaded
	if (hMenuOLE == NULL)
		return;

	UINT nItem;
	CMenu* pMenuFrame = GetHelpMenu(nItem);

	if (pMenuFrame != NULL)
	{
		CString strHelpMenuName;
		int nSeparator = pMenuFrame->GetMenuString(nItem,
				strHelpMenuName, MF_BYPOSITION);
		if (nSeparator == 0)
		{
			TRACE0("Error: COleDocObjectItem::OnInsertMenus() found no help menu!\n");
			return;
		}

#if 0
		CString strTearOffName;
		strTearOffName.Format(_T("%s %s"), AfxGetAppName(), strHelpMenuName);
		strTearOffName.Remove('&');

		// get the normal frame menu
		int nCount = pMenuFrame->GetMenuItemCount();
		HMENU hMenu = GetSubMenu(pMenuFrame->m_hMenu, nCount-1);

		// clean up old menu and allocate a new one
		if (m_pHelpPopupMenu == NULL)
		{
			m_pHelpPopupMenu = new CMenu;

			// create new sub-popup menu and add container's Help tearoff
			// then add help menu from main window
			m_pHelpPopupMenu->CreateMenu();
			m_pHelpPopupMenu->InsertMenu((UINT) -1, MF_BYPOSITION | MF_POPUP,
				(UINT) hMenu, strTearOffName);
		}

		pMenuShared->InsertMenu(1, MF_BYPOSITION | MF_POPUP,
			(UINT) m_pHelpPopupMenu->m_hMenu, strHelpMenuName);

		// tell the object we added our Help menu
		lpMenuWidths->width[5] = 1;
#else
		HMENU hMenu;

		hMenu = GetSubMenu (pMenuFrame->m_hMenu, nItem);
		pMenuShared->InsertMenu (1, MF_BYPOSITION | MF_POPUP, (UINT)hMenu, strHelpMenuName);
#endif
	}

	// insert our menu items and adjust group widths array
	AfxMergeMenus(pMenuShared->GetSafeHmenu(), hMenuOLE,
		&lpMenuWidths->width[0], 0);
}

void COleDocObjectItem::OnRemoveMenus(CMenu *pMenuShared)
{
	int cItemsShared = pMenuShared->GetMenuItemCount();
	if (cItemsShared != 0)
	{
#if 0
		CMenu *pMenuHelp = pMenuShared->GetSubMenu(cItemsShared - 1);

		int cItemsHelp = pMenuHelp->GetMenuItemCount();
		int nItem;
		for (nItem = cItemsHelp-1; nItem > 0; nItem--)
			pMenuHelp->DeleteMenu(nItem, MF_BYPOSITION);
#endif
		pMenuShared->RemoveMenu(cItemsShared - 1, MF_BYPOSITION);
	}

	COleClientItem::OnRemoveMenus(pMenuShared);
}

BOOL COleDocObjectItem::OnPreparePrinting(CView* pCaller, CPrintInfo* pInfo, BOOL bPrintAll)
{
	LONG lDocObjectPages = 0;

	CDocument* pDoc = pCaller->GetDocument();
	COleDocument* pOleDoc = DYNAMIC_DOWNCAST(COleDocument, pDoc);
	if (pOleDoc == NULL)
		return FALSE;

	POSITION pos = pOleDoc->GetStartPosition();
	while (pos != NULL)
	{
		COleClientItem* pItem = pOleDoc->GetNextClientItem(pos);
		COleDocObjectItem* pDocItem = DYNAMIC_DOWNCAST(COleDocObjectItem, pItem);
		if (pDocItem == NULL)
			continue;

		// if this isn't the view, continue
		if (!bPrintAll)
		{
			if (pItem->m_pView == NULL || pItem->m_pView->m_hWnd != pCaller->m_hWnd)
				continue;
		}

		if (pDocItem->SupportsIPrint())
		{
			LONG lThisObjectPages;
			if (pDocItem->GetPageCount(NULL, &lThisObjectPages))
				lDocObjectPages += lThisObjectPages;
			break;
		}
		else
		{
			lDocObjectPages = 1;
			break;
		}
	}

	if (lDocObjectPages > 0)
	{
		UINT nMaxPage = pInfo->GetMaxPage();
		pInfo->SetMaxPage(nMaxPage + lDocObjectPages);
		pInfo->m_bDocObject = TRUE;
	}

	return TRUE;
}

void COleDocObjectItem::OnPrint(CView* pCaller, CPrintInfo* pInfo,
	BOOL bPrintAll)
{
	CDocument* pDoc = pCaller->GetDocument();
	COleDocument* pOleDoc = DYNAMIC_DOWNCAST(COleDocument, pDoc);
	if (pOleDoc == NULL)
		return;

	POSITION pos = pOleDoc->GetStartPosition();
	while (pos != NULL)
	{
		COleClientItem* pItem = pOleDoc->GetNextClientItem(pos);
		COleDocObjectItem* pDocItem = DYNAMIC_DOWNCAST(COleDocObjectItem, pItem);
		if (pDocItem == NULL)
			continue;

		// if this isn't the view, continue
		if (!bPrintAll)
		{
			if (pItem->m_pView == NULL || pItem->m_pView->m_hWnd != pCaller->m_hWnd)
				continue;
		}

		HRESULT hrThisPage = E_UNEXPECTED;

		if (pDocItem->SupportsIPrint())
		{
			DVTARGETDEVICE* pTargetDevice = NULL;
			LPDEVNAMES lpDevNames = NULL;
			LPDEVMODE lpDevMode = NULL;

			lpDevNames = (LPDEVNAMES) GlobalLock(pInfo->m_pPD->m_pd.hDevNames);
			if (lpDevNames != NULL)
			{
				lpDevMode = (LPDEVMODE) GlobalLock(pInfo->m_pPD->m_pd.hDevMode);
				if (lpDevMode != NULL)
				{
					pTargetDevice = _AfxOleCreateTargetDevice(lpDevNames, lpDevMode);
					if (pTargetDevice != NULL)
					{
						PAGESET* pps = (PAGESET*) CoTaskMemAlloc(sizeof(PAGESET));
						if (pps != NULL)
						{
							pps->cbStruct = sizeof(PAGESET);
							ASSERT((pps->cbStruct % 4) == 0);
							pps->fOddPages = TRUE;
							pps->fEvenPages = TRUE;
							pps->cPageRange = 1;
							pps->rgPages[0].nFromPage = pInfo->m_nCurPage;
							pps->rgPages[0].nToPage = pInfo->m_nCurPage;

							LONG lLastPage = pInfo->m_nCurPage;
							LONG lPagesPrinted;

							hrThisPage =
							pDocItem->m_pIPrint->Print(PRINTFLAG_RECOMPOSETODEVICE,
								&pTargetDevice, &pps, NULL, NULL,
								pInfo->m_nCurPage, &lPagesPrinted, &lLastPage);

							if (!SUCCEEDED(hrThisPage))
								TRACE1("IPrint::Print() returned %8.8X\n", hrThisPage);
							CoTaskMemFree(pps);
						}
						CoTaskMemFree(pTargetDevice);
					}
					GlobalUnlock(pInfo->m_pPD->m_pd.hDevMode);
				}
				GlobalUnlock(pInfo->m_pPD->m_pd.hDevNames);
			}
		}
		else
		{
			// try through IOleCommandTarget

			hrThisPage = pDocItem->ExecCommand(OLECMDID_PRINT);
			if (!SUCCEEDED(hrThisPage))
				TRACE1("IOleCommandTarget::Exec() returned %8.8X\n", hrThisPage);
		}

		if (!SUCCEEDED(hrThisPage))
		{
			pInfo->m_bContinuePrinting = FALSE;
			break;
		}
	}

	return;
}

#endif // _MFC_VER == 0x0421



/////////////////////////////////////////////////////////////////////////////
// CGenEdCntrItem implementation

IMPLEMENT_SERIAL(CGenEdCntrItem, COleDocObjectItem, 0)

BEGIN_DISPATCH_MAP(CGenEdCntrItem, COleDocObjectItem)
	//{{AFX_DISPATCH_MAP(CGenEdCntrItem)
	DISP_FUNCTION(CGenEdCntrItem, "GetDocument", GetGenEdDoc, VT_ERROR, VTS_PDISPATCH)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {9DBFD080-5875-11d2-B9C9-006008749B3D}
static const IID IID_ICEGenEdItem =
{ 0x9dbfd080, 0x5875, 0x11d2, { 0xb9, 0xc9, 0x00, 0x60, 0x08, 0x74, 0x9b, 0x3d } };

BEGIN_INTERFACE_MAP(CGenEdCntrItem, COleDocObjectItem)
	INTERFACE_PART(CGenEdCntrItem, IID_ICEGenEdItem, Dispatch)
	INTERFACE_PART(CGenEdCntrItem, IID_ICEEditNotify, CEEditNotifyObj)
END_INTERFACE_MAP()


CGenEdCntrItem::CGenEdCntrItem(CGenEdDoc* pContainer)
: COleDocObjectItem(pContainer),
  m_StateChangeCB (this, OnStateChange),
  m_bStandardEditor (false),
  m_bProgramEditor (false),
  m_PosQual(EDP_STLINE),
  m_lX  (-1),
  m_lY  (-1),
  m_lCX (-1),
  m_lCY (-1),
  m_dwEditNotifyCookie (0),
  m_dwMessageCookie (0),
  m_bDelayPositioning (false),
  m_bDelayedPosition (false),
  m_bDelayedLocation (false),
  m_bKeepOffServer (false)
{
    EnableAutomation ();

    CESMRegisterCallback (&m_StateChangeCB);
}


CGenEdCntrItem::~CGenEdCntrItem()
{
    CESMUnregisterCallback (&m_StateChangeCB);
}


void CGenEdCntrItem::Release (OLECLOSE dwCloseOption)
{
    m_pICEEdit = NULL;
    m_pICEPrgEdit = NULL;
    m_pICEMonitor = NULL;
    m_pICEDebug = NULL;

    // If a server is inplace active when an editor is closed it is
    // already codisconnected at this point and unadvising would not
    // work here. So we also unadvise in the OLE_CLOSED notification
    // (see OnChange). If the server is not inplace active when an
    // editor is closed OnChange is NOT called for server closing.
    // However the server seems to be not codisconnected at this point.
    // So we try unadvising both here and in OnChange.

    UnadviseServer ();

    m_pIConnectionPoint = NULL;

    COleDocObjectItem::Release (dwCloseOption);
}


void CGenEdCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
    ASSERT_VALID(this);
    CDocument *pDocument;

    if (nCode == OLE_CLOSED)
    {

        // see comment in Release

        m_pICEEdit = NULL;
        m_pICEPrgEdit = NULL;
        m_pICEMonitor = NULL;
        m_pICEDebug = NULL;

        UnadviseServer ();
        m_pIConnectionPoint = NULL;
    }
    else if (nCode == OLE_SAVED)
    {
        CGenEdDoc* pDoc = GetDocument();
        if (pDoc)
        {
            pDoc->ResetCheckedFileTime();
        }
    }


    COleDocObjectItem::OnChange (nCode, dwParam);

    pDocument = GetDocument ();
    pDocument->SetModifiedFlag (FALSE);
    pDocument->UpdateAllViews (NULL);
}

void CGenEdCntrItem::OnDeactivate ()
{
    HRESULT hr;
    LPOLEDOCUMENTVIEW pView;

    COleDocObjectItem::OnDeactivate ();

    // It seems as if MFC forgets to call CloseView which leads to our
    // IOleInPlaceSite interface not being released by the server. The
    // effect is that our destructor is not called (missing final re-
    // lease). However DCOM seems to automatically release all outstan-
    // ding references as soon as the server application dies and then
    // our destructor is called (a little late but better than never).
    // The solution is to call CloseView manually (which does not harm
    // if MFC is patched at some future time because CloseView can cope
    // with duplicate invocation). The problem is I've got not the sligh-
    // test idea whether CloseView was intended to be called by the con-
    // tainer or server so I rolled the dice and it clearly said container.

    hr = m_lpObject->QueryInterface (IID_IOleDocumentView, (LPVOID *)&pView);
    if (SUCCEEDED (hr))
    {
        ASSERT (pView != NULL);
        if (pView != NULL)
        {
            pView->CloseView (0);
            pView->Release ();
        }
    }
}

BOOL CGenEdCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
    ASSERT_VALID(this);

    // During in-place activation CGenEdCntrItem::OnChangeItemPosition
    //  is called by the server to change the position of the in-place
    //  window.  Usually, this is a result of the data in the server
    //  document changing such that the extent has changed or as a result
    //  of in-place resizing.
    //
    // The default here is to call the base class, which will call
    //  COleClientItem::SetItemRects to move the item
    //  to the new position.

    if (!COleDocObjectItem::OnChangeItemPosition(rectPos))
        return FALSE;

    // TODO: update any cache you may have of the item's rectangle/extent

    return TRUE;
}

void CGenEdCntrItem::OnGetItemPosition(CRect& rPosition)
{
    ASSERT_VALID(this);

    // During in-place activation, CGenEdCntrItem::OnGetItemPosition
    //  will be called to determine the location of this item.  The default
    //  implementation created from AppWizard simply returns a hard-coded
    //  rectangle.  Usually, this rectangle would reflect the current
    //  position of the item relative to the view used for activation.
    //  You can obtain the view by calling CGenEdCntrItem::GetActiveView.

    // TODO: return correct rectangle (in pixels) in rPosition
    m_pView->GetClientRect(rPosition);
}

void CGenEdCntrItem::OnActivate()
{
    // Allow only one inplace activate item per frame
    CGenEdView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
    {
        CGenEdView *pOtherView;
        CRect rect;

        pOtherView = ((CGenEdCntrItem *)pItem)->GetActiveView ();
        pOtherView->GetClientRect (rect);
        ((CGenEdCntrItem *)pItem)->SetClientExtent (CSize (rect.right - rect.left, rect.bottom - rect.top));

        pItem->Deactivate();
    }

    COleDocObjectItem::OnActivate();
}

void CGenEdCntrItem::OnDeactivateUI(BOOL bUndoable)
{
    COleDocObjectItem::OnDeactivateUI(bUndoable);

    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}


BOOL CGenEdCntrItem::IsBlank () const
{
    // There was a problem with loading the client item from
    // the temporary compound file when calling OnSaveDocument
    // and OnOpenDocument consecutively. For some reason opening
    // the stream designated to the server data failed with
    // denial of access. I did not have the time to investigate.
    // For now I prevent writing the client item to the compound
    // file which as a consequence prevents writing the server
    // data. Where no data is written no data will be loaded and
    // for the moment there is no problem. However the illness
    // is not healed. Should do more investigations in this.
    // How can I reproduce the problem? Remove this method, open
    // an ST file, modify it without saving, add a local variable
    // with a wizard and oops CGenEdCntrItem::Serialize fails.

    return (TRUE);
}


BOOL CGenEdCntrItem::OnShowControlBars (CFrameWnd *pFrameWnd, BOOL bShow)
{
//    CMainFrame *pMainFrame;

    COleDocObjectItem::OnShowControlBars (pFrameWnd, bShow);

//    pMainFrame = dynamic_cast<CMainFrame *> (pFrameWnd);
//    if (pMainFrame != NULL)
//    {
//        pMainFrame->OnShowInplaceToolbars (bShow == TRUE);
//    }

    return (TRUE);
}


bool CGenEdCntrItem::CreateServer (const GUID &rtGUID)
{
    HRESULT hr;
    CComPtr<_ICEMessageEvent> pICEMessageEvent;

    if (!CreateNewItem(rtGUID))
    {
        return (false);
    }

    ASSERT (m_lpObject != NULL);
    if (m_lpObject == NULL)
    {
        Release ();
        return (false);
    }

    hr = CheckICEEdit ();
    if (hr == S_OK)
    {
        m_pICEEdit->Lock (TRUE);
    }

    //::OleLockRunning (m_lpObject, TRUE, TRUE);

    hr = CheckICEEdit ();
    if (SUCCEEDED (hr))
    {
        m_bStandardEditor = true;
    }

    hr = CheckICEPrgEdit ();
    if (SUCCEEDED (hr))
    {
        m_bProgramEditor = true;
    }

    if (m_bStandardEditor)
    {
        hr = SetModified (false);
        assert (SUCCEEDED (hr));
    }

    hr = GetServerConnectionPoint (IID_ICEEditNotify);
    if (FAILED (hr))
    {
        // INCOMPLETE: Informational output
    }

    if (m_pIConnectionPoint != NULL)
    {
        hr = AdviseServer ();

        if (FAILED (hr))
        {
            Release ();
            return (false);
        }
    }

    if (m_bStandardEditor)
    {
        hr = GetPosition (m_PosQual, m_lX, m_lY, m_lCX, m_lCY);
        assert (SUCCEEDED (hr));
    }

    if (m_bProgramEditor)
    {
        HRESULT hr;
        CComQIPtr<ICEProjInfo>          pProjInfo;
        CComPtr<IUnknown>               pTmp;
        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjInfo = pTmp;

        if(pProjInfo)
        {
            CComBSTR    sGenPath;
            CString     strGenPath;
            pProjInfo->getProjectGenPath(&sGenPath);
            strGenPath = sGenPath;
            hr = SetSymbolFile (strGenPath + _T ("\\instances.txt"));
            assert (SUCCEEDED (hr));
        }
    }

    return (true);
}


HRESULT CGenEdCntrItem::GetServerConnectionPoint (const IID &iidSink)
{
    HRESULT hr;
    CComPtr<IConnectionPointContainer> pConnPtContainer;

    hr = m_lpObject->QueryInterface (IID_IConnectionPointContainer, (LPVOID *)&pConnPtContainer);
    if (FAILED (hr))
    {
        return (hr);
    }
    assert (pConnPtContainer != NULL);
    if (pConnPtContainer == NULL)
    {
        return (E_FAIL);
    }

    hr = pConnPtContainer->FindConnectionPoint (iidSink, &m_pIConnectionPoint);
    if (FAILED (hr))
    {
        return (hr);
    }
    assert (m_pIConnectionPoint != NULL);
    if (m_pIConnectionPoint == NULL)
    {
        return (E_FAIL);
    }

    return (S_OK);
}


HRESULT CGenEdCntrItem::AdviseServer ()
{
    RegisterMessageSource ();

    if (m_pIConnectionPoint != NULL && m_bStandardEditor)
    {
        HRESULT hr;
        ICEEditNotify *pICEEditNotify;

        pICEEditNotify = (ICEEditNotify *)GetInterface ((LPVOID)&IID_ICEEditNotify);   // does not AddRef
        ASSERT (pICEEditNotify != NULL);
        if (pICEEditNotify == NULL)
        {
            return (E_FAIL);
        }

        hr = m_pIConnectionPoint->Advise (pICEEditNotify, &m_dwEditNotifyCookie);
        if (FAILED (hr))
        {
            m_dwEditNotifyCookie = 0;
            return (hr);
        }
    }

    return (S_OK);
}


HRESULT CGenEdCntrItem::UnadviseServer ()
{
    UnregisterMessageSource ();

    if (m_pIConnectionPoint != NULL && m_dwEditNotifyCookie != NULL)
    {
        HRESULT hr;

        hr = m_pIConnectionPoint->Unadvise (m_dwEditNotifyCookie);
        if (FAILED (hr))
        {
            return (hr);
        }

        m_dwEditNotifyCookie = NULL;
    }

    return (S_OK);
}


void CGenEdCntrItem::RegisterMessageSource ()
{
    CMsgWnd *pMessageWindow;

    pMessageWindow = dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", false));
    if (pMessageWindow != NULL)
    {
        ASSERT (m_lpObject != NULL);
        if (m_lpObject != NULL)
        {
            pMessageWindow->RegisterMsgSource (m_lpObject, &m_dwMessageCookie);
        }
    }
}


void CGenEdCntrItem::UnregisterMessageSource ()
{
    CMsgWnd *pMessageWindow;

    if (m_dwMessageCookie != NULL)
    {
        pMessageWindow = dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View"));
        if (pMessageWindow != NULL)
        {
            ASSERT (m_lpObject != NULL);
            if (m_lpObject != NULL)
            {
                pMessageWindow->UnregisterMsgSource (m_lpObject, m_dwMessageCookie);
            }
        }

        m_dwMessageCookie = NULL;
    }
}


void CGenEdCntrItem::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    if (stateNotify == CEComponentAccessibleNotify)
    {
        if (CESMGetComponentName () == _T ("Message View"))
        {
            RegisterMessageSource ();
        }
    }
    else if (stateNotify == CEHandsOffComponentNotify)
    {
        if (CESMGetComponentName () == _T ("Message View"))
        {
            UnregisterMessageSource ();
        }
    }
}


bool CGenEdCntrItem::IsStandardEditor ()
{
    return (m_bStandardEditor);
}


bool CGenEdCntrItem::IsProgramEditor ()
{
    return (m_bProgramEditor);
}


void CGenEdCntrItem::KeepOffServer ()
{
    if (m_lpObject != NULL)
    {
        HRESULT hr;

        //::OleLockRunning (m_lpObject, FALSE, TRUE);

        hr = CheckICEEdit ();
        if (hr == S_OK)
        {
            m_pICEEdit->Lock (FALSE);
        }
    }

    m_bKeepOffServer = true;
}


void CGenEdCntrItem::SetDelayedPositioning (bool bDelay)
{
    if (bDelay == m_bDelayPositioning)
    {
        return;
    }

    m_bDelayPositioning = bDelay;

    if (!bDelay)
    {
        if (m_bDelayedPosition)
        {
            m_bDelayedPosition = false;
            GotoPosition (m_DelayedPosQual, m_lDelayedX, m_lDelayedY, m_lDelayedCX, m_lDelayedCY);
        }

        if (m_bDelayedLocation)
        {
            m_bDelayedLocation = false;
            GotoLocation (m_strDelayedLocation);
        }
    }
}


ICEEdit *CGenEdCntrItem::GetICEEdit ()
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (NULL);
    }

    return (m_pICEEdit);
}


ICEPrint *CGenEdCntrItem::GetICEPrint()
{
    HRESULT hr;

    hr = CheckICEEdit();
    if (hr != S_OK)
    {
        return (NULL);
    }

    ICEPrint* pICEPrint;
    hr = m_pICEEdit->QueryInterface(IID_ICEPrint, (void**) &pICEPrint);
    if(hr != S_OK)
    {
        return (NULL);
    }

    return (pICEPrint);
}


HRESULT CGenEdCntrItem::CheckICEEdit ()
{
    HRESULT hr;
    CComPtr<ICEEdit> pICEEdit;

    if (m_bKeepOffServer)
    {
        return (S_FALSE);
    }

    if (m_pICEEdit != NULL)
    {
        return (S_OK);
    }

    ASSERT (m_lpObject != NULL);
    if (m_lpObject == NULL)
    {
        return (E_FAIL);
    }

    hr = m_lpObject->QueryInterface (IID_ICEEdit, (LPVOID *)&pICEEdit);
    if (hr == E_NOINTERFACE)
    {
        return (E_NOINTERFACE);
    }
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pICEEdit != NULL);
    if (pICEEdit == NULL)
    {
        return (E_UNEXPECTED);
    }

    m_pICEEdit = pICEEdit;
    return (S_OK);
}


HRESULT CGenEdCntrItem::CheckICEPrgEdit ()
{
    HRESULT hr;
    CComPtr<ICEPrgEdit> pICEPrgEdit;

    if (m_bKeepOffServer)
    {
        return (S_FALSE);
    }

    if (m_pICEPrgEdit != NULL)
    {
        return (S_OK);
    }

    ASSERT (m_lpObject != NULL);
    if (m_lpObject == NULL)
    {
        return (E_FAIL);
    }

    hr = m_lpObject->QueryInterface (IID_ICEPrgEdit, (LPVOID *)&pICEPrgEdit);
    if (hr == E_NOINTERFACE)
    {
        return (E_NOINTERFACE);
    }
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pICEPrgEdit != NULL);
    if (pICEPrgEdit == NULL)
    {
        return (E_UNEXPECTED);
    }

    m_pICEPrgEdit = pICEPrgEdit;
    return (S_OK);
}


HRESULT CGenEdCntrItem::CheckICEMonitor ()
{
    HRESULT hr;
    CComPtr<ICEMonitor> pICEMonitor;

    if (m_bKeepOffServer)
    {
        return (S_FALSE);
    }

    if (m_pICEMonitor != NULL)
    {
        return (S_OK);
    }

    ASSERT (m_lpObject != NULL);
    if (m_lpObject == NULL)
    {
        return (E_FAIL);
    }

    hr = m_lpObject->QueryInterface (IID_ICEMonitor, (LPVOID *)&pICEMonitor);
    if (hr == E_NOINTERFACE)
    {
        return (E_NOINTERFACE);
    }
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pICEMonitor != NULL);
    if (pICEMonitor == NULL)
    {
        return (E_UNEXPECTED);
    }

    m_pICEMonitor = pICEMonitor;
    return (S_OK);
}


HRESULT CGenEdCntrItem::CheckICEDebug ()
{
    HRESULT hr;
    CComPtr<ICEDebug> pICEDebug;

    if (m_bKeepOffServer)
    {
        return (S_FALSE);
    }

    if (m_pICEDebug != NULL)
    {
        return (S_OK);
    }

    ASSERT (m_lpObject != NULL);
    if (m_lpObject == NULL)
    {
        return (E_FAIL);
    }

    hr = m_lpObject->QueryInterface (IID_ICEDebug, (LPVOID *)&pICEDebug);
    if (hr == E_NOINTERFACE)
    {
        return (E_NOINTERFACE);
    }
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pICEDebug != NULL);
    if (pICEDebug == NULL)
    {
        return (E_UNEXPECTED);
    }

    m_pICEDebug = pICEDebug;
    return (S_OK);
}


HRESULT CGenEdCntrItem::LoadDocument (const CString &strFileName)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->LoadDocument (CComBSTR (strFileName));
    if (FAILED (hr))
    {
        return (hr);
    }

    m_strDocumentFile = strFileName;

    return (S_OK);
}


HRESULT CGenEdCntrItem::SaveDocument ()
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->SaveDocument (CComBSTR (m_strDocumentFile));
    if (FAILED (hr))
    {
        return (hr);
    }

    return (S_OK);
}


HRESULT CGenEdCntrItem::SaveDocumentAs (const CString &strFileName)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->SaveDocument (CComBSTR (strFileName));
    if (FAILED (hr))
    {
        return (hr);
    }

    return (S_OK);
}


HRESULT CGenEdCntrItem::SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->SetInstance (CComBSTR (strInstanceName), CComBSTR (strMachineName), CComBSTR (strMachineAddress));

    return (hr);
}


HRESULT CGenEdCntrItem::GetModified ()
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->GetModified ();

    return (hr);
}


HRESULT CGenEdCntrItem::SetModified (bool bModified)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->SetModified (bModified);

    return (hr);
}


HRESULT CGenEdCntrItem::ForceReadOnly (bool bOn)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->ForceReadOnly (bOn);

    return (hr);
}


HRESULT CGenEdCntrItem::GotoPosition (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    HRESULT hr;

    if (m_bDelayPositioning)
    {
        m_bDelayedPosition = true;
        m_DelayedPosQual = posQual;
        m_lDelayedX = lX;
        m_lDelayedY = lY;
        m_lDelayedCX= lCX;
        m_lDelayedCY= lCY;

        m_bDelayedLocation = false;
    }

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->GotoPosition (posQual, lX, lY, lCX, lCY);

    return (hr);
}


HRESULT CGenEdCntrItem::GetPosition (EDP_POS_QUALIFIER& posQual, long& lX, long& lY, long& lCX, long& lCY)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->GetPosition (&posQual, &lX, &lY, &lCX, &lCY);
    return (hr);
}


HRESULT CGenEdCntrItem::SetClientExtent (CSize size)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->SetClientExtent (size.cx, size.cy);
    return (hr);
}


HRESULT CGenEdCntrItem::GotoLocation (const CString &strLocation)
{
    HRESULT hr;

    if (m_bDelayPositioning)
    {
        m_bDelayedLocation = true;
        m_strDelayedLocation = strLocation;

        m_bDelayedPosition = false;
    }

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->GotoLocation (CComBSTR (strLocation));

    return (hr);
}


HRESULT CGenEdCntrItem::FindFirst (const CString &strStringToSearch, UINT uiOption, CString &strLocation)
{
    HRESULT hr;
    BSTR sLocation;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        strLocation = "";
        return (hr);
    }

    hr = m_pICEEdit->FindFirst (CComBSTR (strStringToSearch), uiOption, &sLocation);
    if (FAILED (hr))
    {
        strLocation = "";
        return (hr);
    }

    strLocation = sLocation;
    ::SysFreeString (sLocation);

    return (hr);
}


HRESULT CGenEdCntrItem::FindNext (CString &strLocation)
{
    HRESULT hr;
    BSTR sLocation;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        strLocation = "";
        return (hr);
    }

    hr = m_pICEEdit->FindNext (&sLocation);
    if (FAILED (hr))
    {
        strLocation = "";
        return (hr);
    }

    strLocation = sLocation;
    ::SysFreeString (sLocation);

    return (hr);
}


HRESULT CGenEdCntrItem::SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->SetPrintInfo (CComBSTR (strPrinterName), lWidth, lHeight);
    return (hr);
}


HRESULT CGenEdCntrItem::GetPageCount (long &lCount)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->GetPageCount (&lCount);
    return (hr);
}


HRESULT CGenEdCntrItem::GetPages (CStringArray &pages)
{
    HRESULT hr;
    SAFEARRAY *pPages=NULL;
    long lLowerBound;
    long lUpperBound;
    long lIndex;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->GetPages (&pPages);
    if (FAILED (hr))
    {
        return (hr);
    }

    ::SafeArrayGetLBound (pPages, 1, &lLowerBound);
    ::SafeArrayGetUBound (pPages, 1, &lUpperBound);

    pages.SetSize (lUpperBound - lLowerBound + 1);

    for (lIndex = lLowerBound; lIndex <= lUpperBound; ++lIndex)
    {
        BSTR sPage;

        ::SafeArrayGetElement (pPages, &lIndex, (void *)&sPage);
        pages.SetAt (lIndex, sPage);
        ::SysFreeString (sPage);
    }

    ::SafeArrayDestroy (pPages);

    return (S_OK);
}


HRESULT CGenEdCntrItem::SetBaseFile (const CString &strBaseFile)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEEdit->SetBaseFile (CComBSTR (strBaseFile));

    return (hr);
}


HRESULT CGenEdCntrItem::SetSymbolFile (const CString &strSymbolFile)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->SetSymbolFile (CComBSTR (strSymbolFile));

    return (hr);
}


HRESULT CGenEdCntrItem::NotifyConnect (const CString &strMachineAddress)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->NotifyConnect (CComBSTR (strMachineAddress));

    return (hr);
}


HRESULT CGenEdCntrItem::NotifyDisconnect (const CString &strMachineAddress)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->NotifyDisconnect (CComBSTR (strMachineAddress));

    return (hr);
}


HRESULT CGenEdCntrItem::NotifyDebugState (CSC_SESSION_STATE state)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->NotifyDebugState ((short)state);

    return (hr);
}


HRESULT CGenEdCntrItem::StoreWatchpoints (CStringArray &watchpoints)
{
    HRESULT hr;
    SAFEARRAY *pSafeArray=NULL;
    long lLowerBound;
    long lUpperBound;
    long lIndex;

    hr = CheckICEMonitor ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEMonitor->StoreWatchpoints (&pSafeArray);
    if (FAILED (hr))
    {
        return (hr);
    }

    watchpoints.RemoveAll ();

    if (pSafeArray == NULL)
    {
        return (S_OK);
    }

    ::SafeArrayGetLBound (pSafeArray, 1, &lLowerBound);
    ::SafeArrayGetUBound (pSafeArray, 1, &lUpperBound);

    for (lIndex = lLowerBound; lIndex <= lUpperBound; ++lIndex)
    {
        BSTR sWatchpoint;

        ::SafeArrayGetElement (pSafeArray, &lIndex, &sWatchpoint);
        watchpoints.Add (sWatchpoint);
        ::SysFreeString (sWatchpoint);
    }

    ::SafeArrayDestroy (pSafeArray);

    return (S_OK);
}


HRESULT CGenEdCntrItem::RestoreWatchpoints (const CStringArray &watchpoints)
{
    HRESULT hr;
    SAFEARRAYBOUND bound;
    SAFEARRAY *pSafeArray;

    hr = CheckICEMonitor ();
    if (hr != S_OK)
    {
        return (hr);
    }

    bound.lLbound = 0;
    bound.cElements = watchpoints.GetSize ();

    pSafeArray = ::SafeArrayCreate (VT_BSTR, 1, &bound);

    if (bound.cElements > 0)
    {
        long lIndex=0;

        if (pSafeArray == NULL)
        {
            return (E_FAIL);
        }

        for (lIndex = 0; (unsigned long)lIndex < bound.cElements; ++lIndex)
        {
            CString strWP;

            strWP = watchpoints[lIndex];

            ::SafeArrayPutElement (pSafeArray, &lIndex, (void *)(BSTR)CComBSTR (strWP));
        }
    }

    hr = m_pICEMonitor->LoadWatchpoints (pSafeArray);

    ::SafeArrayDestroy (pSafeArray);

    return (hr);
}


HRESULT CGenEdCntrItem::Resubscribe ()
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->Resubscribe ();

    return (hr);
}


HRESULT CGenEdCntrItem::InsertFBCall (const CString &strFB)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->InsertFBCall (CComBSTR (strFB));

    return (hr);
}

HRESULT CGenEdCntrItem::InsertVariable(const CString &strVar)
{
    HRESULT hr;

    hr = CheckICEPrgEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEPrgEdit->InsertVariable (CComBSTR (strVar));

    return (hr);
}



HRESULT CGenEdCntrItem::ShowBreakpoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY, short sState)
{
    HRESULT hr;

    hr = CheckICEDebug ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEDebug->ShowBreakpoint (posQual, lX, lY, lCX, lCY, sState);

    return (hr);
}


HRESULT CGenEdCntrItem::HideBreakpoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    HRESULT hr;

    hr = CheckICEDebug ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEDebug->HideBreakpoint (posQual, lX, lY, lCX, lCY);

    return (hr);
}


HRESULT CGenEdCntrItem::ShowStoppoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    HRESULT hr;

    hr = CheckICEDebug ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEDebug->ShowStoppoint (posQual, lX, lY, lCX, lCY);

    return (hr);
}


HRESULT CGenEdCntrItem::HideStoppoint ()
{
    HRESULT hr;

    hr = CheckICEDebug ();
    if (hr != S_OK)
    {
        return (hr);
    }

    hr = m_pICEDebug->HideStoppoint ();

    return (hr);
}


HRESULT CGenEdCntrItem::GetSelectedText(CString &rstrText)
{
    HRESULT hr;

    hr = CheckICEEdit ();
    if (hr != S_OK)
    {
        return (hr);
    }

    CComBSTR sText;
    hr = m_pICEEdit->GetSelectedText(&sText);
    if (FAILED (hr))
    {
        return (hr);
    }

    rstrText = sText;
    return (S_OK);
}

CString CGenEdCntrItem::GetDocumentFile ()
{
    return (m_strDocumentFile);
}


void CGenEdCntrItem::SetDocumentFile (const CString &strDocumentFile)
{
    m_strDocumentFile = strDocumentFile;
}


void CGenEdCntrItem::OnPositionChanged (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    m_PosQual = posQual;
    m_lX = lX;
    m_lY = lY;
    m_lCX= lCX;
    m_lCY= lCY;
}


CString CGenEdCntrItem::GetPositionText ()
{
    CString strText;

    strText.Format ("Line %d, Col %d", m_lY, m_lX);//EF TODO
    
    return (strText);
}


void CGenEdCntrItem::Serialize(CArchive& ar)
{
    ASSERT_VALID(this);

    // Call base class first to read in COleDocObjectItem data.
    // Since this sets up the m_pDocument pointer returned from
    //  CGenEdCntrItem::GetDocument, it is a good idea to call
    //  the base class Serialize first.
    COleDocObjectItem::Serialize(ar);

    // now store/retrieve data specific to CGenEdCntrItem
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}


SCODE CGenEdCntrItem::GetGenEdDoc (LPDISPATCH *pDocumentDispatch)
{
    CGenEdDoc *pDocument;

    pDocument = GetDocument ();
    assert (pDocument != NULL);
    if (pDocument == NULL)
    {
        return (E_UNEXPECTED);
    }

    *pDocumentDispatch = pDocument->GetIDispatch (TRUE);
    assert (*pDocumentDispatch != NULL);
    if (*pDocumentDispatch == NULL)
    {
        return (E_UNEXPECTED);
    }

    return (S_OK);
}

BOOL CGenEdCntrItem::OnGetWindowContext(
	CFrameWnd** ppMainFrame, CFrameWnd** ppDocFrame,
	LPOLEINPLACEFRAMEINFO pFrameInfo)
{
    COleClientItem::OnGetWindowContext(ppMainFrame, ppDocFrame, pFrameInfo);

	if (pFrameInfo != NULL)
	{
		// get accelerator table from mainframe
        CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		HACCEL hAccel = pMainFrame->GetAccelerators();
		pFrameInfo->cAccelEntries =
			hAccel != NULL ? CopyAcceleratorTable(hAccel, NULL, 0) : 0;
		pFrameInfo->haccel = pFrameInfo->cAccelEntries != 0 ? hAccel : NULL;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGenEdCntrItem diagnostics

#ifdef _DEBUG
void CGenEdCntrItem::AssertValid() const
{
    COleDocObjectItem::AssertValid();
}

void CGenEdCntrItem::Dump(CDumpContext& dc) const
{
    COleDocObjectItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface ICEEditNotify

STDMETHODIMP_(ULONG) CGenEdCntrItem::XCEEditNotifyObj::AddRef ()
{
    METHOD_PROLOGUE_EX (CGenEdCntrItem, CEEditNotifyObj)
  
    return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CGenEdCntrItem::XCEEditNotifyObj::Release ()
{
    METHOD_PROLOGUE_EX (CGenEdCntrItem, CEEditNotifyObj)

    return (pThis->ExternalRelease ());
}

STDMETHODIMP CGenEdCntrItem::XCEEditNotifyObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
    METHOD_PROLOGUE_EX (CGenEdCntrItem, CEEditNotifyObj)

    return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}


STDMETHODIMP CGenEdCntrItem::XCEEditNotifyObj::OnPositionChanged (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    METHOD_PROLOGUE_EX (CGenEdCntrItem, CEEditNotifyObj)

    pThis->OnPositionChanged (posQual, lX, lY, lCX, lCY);
    return (S_OK);
}


