

#include "stdafx.h"
#include "FcHtmlEd.h"
#include "FcIOleDocumentSite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFcIOleDocumentSite::CFcIOleDocumentSite(IUnknown *pUnkownObj/*=NULL*/)
{
 	m_pUnkownObj = pUnkownObj;
	m_cRef = 1;
	m_pDocView = NULL;
}

CFcIOleDocumentSite::~CFcIOleDocumentSite()
{
}

STDMETHODIMP CFcIOleDocumentSite::QueryInterface(REFIID iid, void **ppvObject)
{
	*ppvObject = NULL;
	if (IID_IOleDocumentSite == iid || IID_IUnknown == iid)
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

STDMETHODIMP_(ULONG) CFcIOleDocumentSite::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcIOleDocumentSite::Release()
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

/*
 * CImpIOleDocumentsite::ActivateMe
 *
 * Purpose:
 *  Instructs the container to activate the object in this site as
 *  a document object.
 *
 * Parameters:
 *  pView           IOleDocumentView * of the object to activate.
 *
 * Return Value:
 *  HRESULT         NOERROR if successful, error code otherwise.
 */
STDMETHODIMP CFcIOleDocumentSite::ActivateMe(IOleDocumentView *pView)
{
    /*
     * If we're passed a NULL view pointer, then try to get one from
     * the document object (the object within us).
     */

	if (m_pUnkownObj == NULL)
		return E_FAIL;

	IOleInPlaceSite* pInPlace = NULL;
	if (FAILED(m_pUnkownObj->QueryInterface(IID_IOleInPlaceSite, (void **)&pInPlace)))
		return E_FAIL;
	
	HRESULT hr = E_FAIL;
	if ( NULL == pView )
    {
		IOleDocument*		pDoc = NULL;
		if (SUCCEEDED(m_pUnkownObj->QueryInterface(IID_IOleDocument, (void **)&pDoc)))
		{
			if (SUCCEEDED(pDoc->CreateView(pInPlace, NULL, 0, &m_pDocView)))
				hr = S_OK;
			else
				hr = E_OUTOFMEMORY;
		}

		if (pDoc != NULL)
			pDoc->Release();
    }        
    else
    {
		m_pDocView = pView;

		//Make sure that the view has our client site
		m_pDocView->SetInPlaceSite(pInPlace);

        //We're holding onto the pointer, so AddRef it.
        m_pDocView->AddRef();
		hr = S_OK;
    }

	pInPlace->Release();
	if (FAILED(hr))
		return hr;


    // Activation steps, now that we have a view:
    
    //This sets up toolbars and menus first    
    m_pDocView->UIActivate(TRUE);

    //Set the window size sensitive to new toolbars
/*
    RECT rc;
    m_pSite->GetFrame()->GetClientRect( &rc );
    m_pDocView->SetRect( &rc );
*/
	//Makes it all active
    m_pDocView->Show( TRUE );    
    return NOERROR;
}
