

#include "stdafx.h"
#include "fchtmled.h"
#include "FcIDocHostUIHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CFcIDocHostUIHandler::CFcIDocHostUIHandler(IUnknown *pUnkownObj/*=NULL*/)
{
	m_pUnkownObj = pUnkownObj;
    m_cRef = 0;

	m_pIOleCommandTarget = NULL;
}

CFcIDocHostUIHandler::~CFcIDocHostUIHandler()
{
	if (m_pIOleCommandTarget != NULL)
	{
		m_pIOleCommandTarget->Release();
		m_pIOleCommandTarget = NULL;
	}
}

STDMETHODIMP CFcIDocHostUIHandler::QueryInterface(REFIID iid, void **ppvObject)
{
	*ppvObject = NULL;
	if (IID_IOleClientSite == iid || IID_IUnknown == iid)
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}

	if (m_pUnkownObj != NULL)
		return m_pUnkownObj->QueryInterface(iid, ppvObject);
	else
		return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CFcIDocHostUIHandler::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcIDocHostUIHandler::Release()
{
	ASSERT(m_cRef > 0);
    --m_cRef;

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
    return m_cRef;
}

// * CFcIDocHostUIHandler::GetHostInfo
// *
// * Purpose: Called at initialization
// *
STDMETHODIMP CFcIDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
    pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;
    pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

    return S_OK;
}

// * CFcIDocHostUIHandler::ShowUI
STDMETHODIMP CFcIDocHostUIHandler::ShowUI(
				DWORD dwID, 
				IOleInPlaceActiveObject * /*pActiveObject*/,
				IOleCommandTarget * pCommandTarget,
				IOleInPlaceFrame * /*pFrame*/,
				IOleInPlaceUIWindow * /*pDoc*/)
{

	// store the CommandTraget interface
	if (m_pIOleCommandTarget != NULL)
	{
		m_pIOleCommandTarget->Release();
		m_pIOleCommandTarget = NULL;
	}
	if (pCommandTarget != NULL)
	{
		m_pIOleCommandTarget = pCommandTarget;
		m_pIOleCommandTarget->AddRef();

//@@	m_pSite->GetFrame()->SetOptions();	
	}


	// We've already got our own UI in place so just return S_OK
    return S_OK;
}

// * CFcIDocHostUIHandler::HideUI
STDMETHODIMP CFcIDocHostUIHandler::HideUI(void)
{
    return S_OK;
}

// * CFcIDocHostUIHandler::UpdateUI
STDMETHODIMP CFcIDocHostUIHandler::UpdateUI(void)
{
	// MFC is pretty good about updating it's UI in it's Idle loop so don't do anything here
	return S_OK;
}

// * CFcIDocHostUIHandler::EnableModeless
// *
// * Purpose: Called from IOleInPlaceActiveObject::EnableModeless
// *
STDMETHODIMP CFcIDocHostUIHandler::EnableModeless(BOOL /*fEnable*/)
{
    return E_NOTIMPL;
}

// * CFcIDocHostUIHandler::OnDocWindowActivate
// *
// * Purpose: Called from IOleInPlaceActiveObject::OnDocWindowActivate
// *
STDMETHODIMP CFcIDocHostUIHandler::OnDocWindowActivate(BOOL /*fActivate*/)
{
    return E_NOTIMPL;
}

// * CFcIDocHostUIHandler::OnFrameWindowActivate
// *
// * Purpose: Called from IOleInPlaceActiveObject::OnFrameWindowActivate
// *
STDMETHODIMP CFcIDocHostUIHandler::OnFrameWindowActivate(BOOL /*fActivate*/)
{
    return E_NOTIMPL;
}

// * CFcIDocHostUIHandler::ResizeBorder
// *
// * Purpose: Called from IOleInPlaceActiveObject::ResizeBorder
// *
STDMETHODIMP CFcIDocHostUIHandler::ResizeBorder(
				LPCRECT /*prcBorder*/, 
				IOleInPlaceUIWindow* /*pUIWindow*/,
				BOOL /*fRameWindow*/)
{
    return E_NOTIMPL;
}

// * CFcIDocHostUIHandler::ShowContextMenu
// *
// * Purpose: Called when it would normally display its context menu
// *
STDMETHODIMP CFcIDocHostUIHandler::ShowContextMenu(
				DWORD /*dwID*/, 
				POINT* /*pptPosition*/,
				IUnknown* /*pCommandTarget*/,
				IDispatch* /*pDispatchObjectHit*/)
{
    return E_NOTIMPL;
}

// * CFcIDocHostUIHandler::TranslateAccelerator
// *
// * Purpose: Called from TranslateAccelerator routines
// *
STDMETHODIMP CFcIDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID)
{
    return S_FALSE;
}

// * CFcIDocHostUIHandler::GetOptionKeyPath
// *
// * Purpose: Called to find where the host wishes to store 
// *	its options in the registry
// *
STDMETHODIMP CFcIDocHostUIHandler::GetOptionKeyPath(LPOLESTR* pchKey, DWORD)
{

	CFcHtmlEdApp* pApp = (CFcHtmlEdApp*)AfxGetApp();
	CString strPath = pApp->m_pszRegistryKey;
	BSTR sKey = strPath.AllocSysString();
	if (sKey == NULL)
		return E_OUTOFMEMORY;
	*pchKey = (LPOLESTR)CoTaskMemAlloc((wcslen(sKey)+1)*sizeof(OLECHAR));
	if (*pchKey == NULL) {
		SysFreeString(sKey);
		return E_OUTOFMEMORY;
	}

	wcscpy(*pchKey, sKey);
	SysFreeString(sKey);

	return S_OK;
}

STDMETHODIMP CFcIDocHostUIHandler::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
    return E_NOTIMPL;
}

STDMETHODIMP CFcIDocHostUIHandler::GetExternal( 
    /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
    return E_NOTIMPL;
}

STDMETHODIMP CFcIDocHostUIHandler::TranslateUrl( 
    /* [in] */ DWORD dwTranslate,
    /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
    /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
    return E_NOTIMPL;
}

STDMETHODIMP CFcIDocHostUIHandler::FilterDataObject( 
    /* [in] */ IDataObject __RPC_FAR *pDO,
    /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
    return E_NOTIMPL;
}
