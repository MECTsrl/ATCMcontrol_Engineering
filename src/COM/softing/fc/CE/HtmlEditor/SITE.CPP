

#include "stdafx.h"
#include <afxpriv.h>

#include "FcHtmlEd.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"

#include "FcIDispatch.h"
#include "FcIOleClientSite.h"
#include "FcIOleDocumentSite.h"
#include "FcIOleInPlaceSite.h"
#include "FcIDocHostUIHandler.h"
#include "FcIDocHostShowUI.h"

#include <INITGUID.H>

#include "site.h"
#include <TRIEDIID.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
 * CSite::CSite
 * CSite::~CSite
 *
 * Constructor Parameters:
 *  hWnd            HWND of the window associated with the site
 *  pFR             PCFrame to the parent structure.
 */
CSite::CSite(CFcHtmlEdView* pFR )
{
	m_dwDAdviseConnection = 0;
    
	m_cRef = 1;
    m_pFR = pFR;

    m_pObj = NULL;
    
    m_pIOleClientSite= new CFcIOleClientSite(this);
    m_pIOleDocumentSite = new CFcIOleDocumentSite(this);
    m_pIOleInPlaceSite = new CFcIOleInPlaceSite(this);
	m_pIOleInPlaceSite->SetWindowContext(pFR);
    m_pIDocHostUIHandler = new CFcIDocHostUIHandler(this);
    m_pIDocHostShowUI = new CFcIDocHostShowUI(this);
    m_pIDispatch = new CFcIDispatch(this);
}


CSite::~CSite(void)
{
	ASSERT(m_cRef == 0);

	while (m_pIDispatch->Release() > 0);
	while (m_pIDocHostShowUI->Release() > 0);
	while (m_pIDocHostUIHandler->Release() > 0);
	while (m_pIOleInPlaceSite->Release() > 0);
	while (m_pIOleClientSite->Release() > 0);
	while (m_pIOleDocumentSite->Release() > 0);
}


/*
 * CSite::QueryInterface
 * CSite::AddRef
 * CSite::Release
 *
 * Purpose:
 *  IUnknown members for CSite object.
 */
STDMETHODIMP CSite::QueryInterface( REFIID riid, void **ppv )
{
	if (riid == IID_IOleWindow || riid == IID_IOleInPlaceSite)
		*ppv = m_pIOleInPlaceSite;
	else if (riid == IID_IOleDocumentSite)
		*ppv = m_pIOleDocumentSite;
	else if (riid == IID_IDocHostUIHandler)
		*ppv = m_pIDocHostUIHandler;
	else if (riid == IID_IDocHostShowUI)
		*ppv = m_pIDocHostShowUI;
	else if (riid == IID_IDispatch)
		*ppv = m_pIDispatch;
	else
		*ppv = NULL;

    if ( NULL != *ppv )
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }

	// Try the frame instead
	HRESULT hr = m_pFR->QueryInterface( riid, ppv );
	if ( SUCCEEDED(hr))
		return hr;

    if ( IID_IOleDocument == riid || IID_IOleInPlaceObject == riid)
	{
		if (m_pObj != NULL)
		{
			hr = m_pObj->QueryInterface( riid, ppv );
			if ( SUCCEEDED(hr))
				return hr;
		}
	}

	return hr;
}


STDMETHODIMP_(ULONG) CSite::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CSite::Release(void)
{
    if ( 0 != --m_cRef )
        return m_cRef;

    delete this;
    return 0;
}

/*
 * CSite::Create
 *
 * Purpose:
 *  Asks the site to instantiate the MSHTML.DLL object.
 *  
 *
 * Parameters:
 *  pIStorage       IStorage * of the parent storage in which we're
 *                  to create an IStorage for the new object.
 *  pchPath         Path of what to load..
 *
 * Return Value:
 *  BOOL            Result of the creation.
 */
BOOL CSite::Create( TCHAR* pchPath )
{
    HRESULT   hr;

	// Create TriEditDocument
	hr = CoCreateInstance( CLSID_TriEditDocument, NULL,								
			CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&m_pObj );
    
	if (SUCCEEDED(hr))
	{
		LPOLEOBJECT pIOleObject;
	    //We need an IOleObject most of the time, so get one here.
		hr = m_pObj->QueryInterface( IID_IOleObject, (void **)&pIOleObject );
		if (SUCCEEDED(hr))
		{
			//SetClientSite is critical for DocObjects
			hr = pIOleObject->SetClientSite(m_pIOleClientSite);
			if (SUCCEEDED(hr))
			{
				// Put the object in the running state
				OleRun(pIOleObject);
			}
		}
	}

	// Establish "data change" event handling 
	IDataObject *pIDataObject;
	hr = m_pObj->QueryInterface( IID_IDataObject, (void **)&pIDataObject );
	if (SUCCEEDED(hr))
	{
		IAdviseSink *pIAdviseSink;
		hr = m_pFR->QueryInterface( IID_IAdviseSink, (void **)&pIAdviseSink );
		if (SUCCEEDED(hr))
		{
			FORMATETC format;
			format.cfFormat = 0;
			format.dwAspect = 0;
			format.lindex = -1;
			format.ptd = NULL;
			format.tymed = TYMED_NULL;
			
			hr = pIDataObject->DAdvise(&format, ADVF_NODATA, pIAdviseSink, &m_dwDAdviseConnection );
			ASSERT(SUCCEEDED(hr));
			pIAdviseSink->Release();
		}
		pIDataObject->Release();
	}

	if (SUCCEEDED(hr))
	    hr = Load(pchPath);


	return SUCCEEDED(hr);
}

/*
 * CSite::Close
 *
 * Purpose:
 *  Possibly commits the storage, then releases it, afterwards
 *  frees alls the object pointers.
 *
 * Parameters:
 *  fCommit         BOOL indicating if we're to commit.
 *
 * Return Value:
 *  None
 */
void CSite::Close()
{
	// Remove "data change" event handling 
	IDataObject *pIDataObject = NULL;
	HRESULT hr = m_pObj->QueryInterface( IID_IDataObject, (void **)&pIDataObject );
	if (SUCCEEDED(hr))
	{
		ASSERT(pIDataObject != NULL);
		pIDataObject->DUnadvise(m_dwDAdviseConnection);
		pIDataObject->Release();
	}
	m_dwDAdviseConnection = 0;

	
	// OnInPlaceDeactivate releases this pointer.
	LPOLEINPLACEOBJECT pIOleInPlaceObject = NULL;
	hr = m_pObj->QueryInterface(IID_IOleInPlaceObject, (void**)&pIOleInPlaceObject);
	if (SUCCEEDED(hr))
	{
		ASSERT(pIOleInPlaceObject != NULL);
        pIOleInPlaceObject->InPlaceDeactivate();
	    pIOleInPlaceObject->Release();
	}


	LPOLEOBJECT pIOleObject = NULL;
	hr = m_pObj->QueryInterface( IID_IOleObject, (void **)&pIOleObject );
	if (SUCCEEDED(hr))
	{
		ASSERT(pIOleObject != NULL);
        pIOleObject->Close(OLECLOSE_NOSAVE );
	    pIOleObject->Release();
	}

    m_pObj->Release();
	m_pObj = NULL;
}



/*
 * CSite::Activate
 *
 * Purpose:
 *  Activates a verb on the object living in the site.
 *
 * Parameters:
 *  iVerb           LONG of the verb to execute.
 *
 * Return Value:
 *  None
 */

void CSite::Activate( LONG iVerb )
{
    CWaitCursor *pWait;
    RECT        rc;
            
    pWait = new CWaitCursor;

    GetClientRect(m_pFR->m_hWnd, &rc );

	LPOLEOBJECT pIOleObject = NULL;
	HRESULT hr = m_pObj->QueryInterface( IID_IOleObject, (void **)&pIOleObject );
	if (SUCCEEDED(hr))
	{
		ASSERT(pIOleObject != NULL);
		pIOleObject->DoVerb( iVerb, NULL, m_pIOleClientSite, 0, m_pFR->m_hWnd, &rc );
	    pIOleObject->Release();
	}

    delete pWait;
}

/*
 *
 * Purpose:
 *  Informs the site that the client area window was resized and
 *  that the site needs to also tell the DocObject of the resize.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  None
 */

void CSite::UpdateDocumentViewRect()
{	
	ASSERT(m_pIOleDocumentSite != NULL);
	if (m_pIOleDocumentSite != NULL)
	{
		IOleDocumentView *pDocView = m_pIOleDocumentSite->GetDocumentView();
		if (pDocView != NULL)
		{
			RECT rc;
			pDocView->AddRef();
		    GetClientRect(m_pFR->m_hWnd, &rc);
			pDocView->SetRect(&rc);
			pDocView->Release();
		}
	}
}


/*
 * CSite::Load
 *
 * Purpose:
 *  Loads the path provided using IPersistMoniker or IPersistFile. If no path
 *	was provided it simply does an InitNew
 *
 * Parameters:
 *  pchPath - The path
 *
 * Return Value:
 */
HRESULT CSite::Load(TCHAR* pchPath)
{
    HRESULT   hr = S_FALSE;
	if (m_pObj != NULL)
	{
		if ( pchPath != NULL && *pchPath != 0)
		{
			OLECHAR  szwName[256];
			MultiByteToWideChar(CP_ACP, 0, pchPath, -1, szwName, 256);    
			IPersistFile*  pPFile;
			hr = m_pObj->QueryInterface(IID_IPersistFile, (void **) &pPFile);
			if ( SUCCEEDED(hr) )
			{
				// Call Load on the IPersistFile
				hr = pPFile->Load((LPWSTR)szwName, 0);
				pPFile->Release();
			}
		}
		else
		{
			// No path provided so just do an InitNew on the Stream 
			IPersistStreamInit* pPStm;
			hr = m_pObj->QueryInterface(IID_IPersistStreamInit, (void **)&pPStm);
			if ( SUCCEEDED(hr) )
			{
				hr = pPStm->InitNew();
				pPStm->Release();
			}
		}
	}

	return hr;
}


HRESULT CSite::Save(TCHAR* pchPath)
{
    HRESULT   hr = S_OK;

    OLECHAR  szwName[256];
	LPOLESTR pszwName = szwName;
	LPOLESTR *ppszwName = &pszwName;

	IPersistFile*  pPFile = NULL;
    hr = m_pObj->QueryInterface(IID_IPersistFile, (void **) &pPFile);
	if ( SUCCEEDED(hr) && pPFile)
	{
		if ( pchPath != NULL && *pchPath != 0)
		{
			MultiByteToWideChar(CP_ACP, 0, pchPath, -1, szwName, 256);    
		}
		else 
		{
			hr = pPFile->GetCurFile(ppszwName);
		}
		if ( SUCCEEDED(hr) && ppszwName)
		{
			// Call Save on the IPersistFile
			hr = pPFile->Save(*ppszwName, TRUE);
			if ( SUCCEEDED(hr) )
			{
				hr = pPFile->SaveCompleted(*ppszwName);
			}
	    }
	    pPFile->Release();
    }

	return hr;
}

IOleCommandTarget* CSite::GetCommandTarget()
{ 
	return m_pIDocHostUIHandler->GetIOleCommandTarget(); 
}