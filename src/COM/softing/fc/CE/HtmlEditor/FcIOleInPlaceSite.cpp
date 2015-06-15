

#include "stdafx.h"
#include "FcHtmlEd.h"
#include "FcIOleInPlaceSite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFcIOleInPlaceSite::CFcIOleInPlaceSite(IUnknown *pUnkownObj/*=NULL*/)
{
 	m_pUnkownObj = pUnkownObj;
	m_cRef = 1;

	m_pSiteWnd = NULL;
	m_pIOleInPlaceObject = NULL;
}

CFcIOleInPlaceSite::~CFcIOleInPlaceSite()
{
	if (m_pIOleInPlaceObject != NULL)
	{
		m_pIOleInPlaceObject->Release();
		m_pIOleInPlaceObject = NULL;
	}
	ASSERT(m_cRef == 0);
}


STDMETHODIMP CFcIOleInPlaceSite::QueryInterface(REFIID iid, void **ppvObject)
{
	*ppvObject = NULL;
	if (IID_IOleInPlaceSite == iid || IID_IUnknown == iid)
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

STDMETHODIMP_(ULONG) CFcIOleInPlaceSite::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcIOleInPlaceSite::Release()
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
 * CImpIOleInPlaceActiveObject::GetWindow
 *
 * Purpose:
 *  Retrieves the handle of the window associated with the object
 *  on which this interface is implemented.
 *
 * Parameters:
 *  phWnd           HWND * in which to store the window handle.
 *
 * Return Value:
 *  HRESULT         NOERROR if successful, E_FAIL if there is no
 *                  window.
 */
STDMETHODIMP CFcIOleInPlaceSite::GetWindow( HWND *phWnd )
{
    //This is the client-area window in the frame
	ASSERT(m_pSiteWnd != NULL);
    *phWnd = m_pSiteWnd->m_hWnd;
    return NOERROR;
}


/*
 * CImpIOleInPlaceActiveObject::ContextSensitiveHelp
 *
 * Purpose:
 *  Instructs the object on which this interface is implemented to
 *  enter or leave a context-sensitive help mode.
 *
 * Parameters:
 *  fEnterMode      BOOL TRUE to enter the mode, FALSE otherwise.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */

STDMETHODIMP CFcIOleInPlaceSite::ContextSensitiveHelp( 
											BOOL /*fEnterMode*/ )
{
    return NOERROR;
}


/*
 * CFcIOleInPlaceSite::CanInPlaceActivate
 *
 * Purpose:
 *  Answers the server whether or not we can currently in-place
 *  activate its object.  By implementing this interface we say
 *  that we support in-place activation, but through this function
 *  we indicate whether the object can currently be activated
 *  in-place.  Iconic aspects, for example, cannot, meaning we
 *  return S_FALSE.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  HRESULT         NOERROR if we can in-place activate the object
 *                  in this site, S_FALSE if not.
 */
STDMETHODIMP CFcIOleInPlaceSite::CanInPlaceActivate( void )
{    
    /*
     * We can always in-place activate--no restrictions for DocObjects.
     * We don't worry about other cases since CSite only ever creates
     * embedded files.
     */
    return NOERROR;
}


/*
 * CFcIOleInPlaceSite::OnInPlaceActivate
 *
 * Purpose:
 *  Informs the container that an object is being activated in-place
 *  such that the container can prepare appropriately.  The
 *  container does not, however, make any user interface changes at
 *  this point.  See OnUIActivate.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  HRESULT         NOERROR or an appropriate error code.
 */
STDMETHODIMP CFcIOleInPlaceSite::OnInPlaceActivate( void )
{
    return m_pUnkownObj->QueryInterface(IID_IOleInPlaceObject, (void**)&m_pIOleInPlaceObject);
}



/*
 * CFcIOleInPlaceSite::OnInPlaceDeactivate
 *
 * Purpose:
 *  Notifies the container that the object has deactivated itself
 *  from an in-place state.  Opposite of OnInPlaceActivate.  The
 *  container does not change any UI at this point.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  HRESULT         NOERROR or an appropriate error code.
 */

STDMETHODIMP CFcIOleInPlaceSite::OnInPlaceDeactivate( void )
{
    /*
     * Since we don't have an Undo command, we can tell the object
     * right away to discard its Undo state.
     */
//@@    m_pSite->Activate( OLEIVERB_DISCARDUNDOSTATE );
	if (m_pIOleInPlaceObject != NULL)
	{
		m_pIOleInPlaceObject->Release();
		m_pIOleInPlaceObject = NULL;
	}

    return NOERROR;
}




/*
 * CFcIOleInPlaceSite::OnUIActivate
 *
 * Purpose:
 *  Informs the container that the object is going to start munging
 *  around with user interface, like replacing the menu.  The
 *  container should remove any relevant UI in preparation.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  HRESULT         NOERROR or an appropriate error code.
 */

STDMETHODIMP CFcIOleInPlaceSite::OnUIActivate( void )
{
    //No state we have to set up here.
    return NOERROR;
}




/*
 * CFcIOleInPlaceSite::OnUIDeactivate
 *
 * Purpose:
 *  Informs the container that the object is deactivating its
 *  in-place user interface at which time the container may
 *  reinstate its own.  Opposite of OnUIActivate.
 *
 * Parameters:
 *  fUndoable       BOOL indicating if the object will actually
 *                  perform an Undo if the container calls
 *                  ReactivateAndUndo.
 *
 * Return Value:
 *  HRESULT         NOERROR or an appropriate error code.
 */
STDMETHODIMP CFcIOleInPlaceSite::OnUIDeactivate( BOOL /*fUndoable*/ )
{
    return NOERROR;
}


/*
 * CFcIOleInPlaceSite::DeactivateAndUndo
 *
 * Purpose:
 *  If immediately after activation the object does an Undo, the
 *  action being undone is the activation itself, and this call
 *  informs the container that this is, in fact, what happened.
 *  The container should call IOleInPlaceObject::UIDeactivate.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  HRESULT         NOERROR or an appropriate error code.
 */
STDMETHODIMP CFcIOleInPlaceSite::DeactivateAndUndo( void )
{
	// Tell the object we are deactivating
	if (m_pIOleInPlaceObject != NULL)
		m_pIOleInPlaceObject->InPlaceDeactivate();
    return NOERROR;
}




/*
 * CFcIOleInPlaceSite::DiscardUndoState
 *
 * Purpose:
 *  Informs the container that something happened in the object
 *  that means the container should discard any undo information
 *  it currently maintains for the object.
 *
 * Parameters:
 *  None
 *
 * Return Value:
 *  HRESULT         NOERROR or an appropriate error code.
 */

STDMETHODIMP CFcIOleInPlaceSite::DiscardUndoState( void )
{
    return E_NOTIMPL;
}




/*
 * CFcIOleInPlaceSite::GetWindowContext
 *
 * Purpose:
 *  Provides an in-place object with pointers to the frame and
 *  document level in-place interfaces (IOleInPlaceFrame and
 *  IOleInPlaceUIWindow) such that the object can do border
 *  negotiation and so forth.  Also requests the position and
 *  clipping rectangles of the object in the container and a
 *  pointer to an OLEINPLACEFRAME info structure which contains
 *  accelerator information.
 *
 *  Note that the two interfaces this call returns are not
 *  available through QueryInterface on IOleInPlaceSite since they
 *  live with the frame and document, but not the site.
 *
 * Parameters:
 *  ppIIPFrame      LPOLEINPLACEFRAME * in which to return the
 *                  AddRef'd pointer to the container's
 *                  IOleInPlaceFrame.
 *  ppIIPUIWindow   LPOLEINPLACEUIWINDOW * in which to return
 *                  the AddRef'd pointer to the container document's
 *                  IOleInPlaceUIWindow.
 *  prcPos          LPRECT in which to store the object's position.
 *  prcClip         LPRECT in which to store the object's visible
 *                  region.
 *  pFI             LPOLEINPLACEFRAMEINFO to fill with accelerator
 *                  stuff.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcIOleInPlaceSite::GetWindowContext(
						LPOLEINPLACEFRAME* ppIIPFrame,
						LPOLEINPLACEUIWINDOW* ppIIPUIWindow,
						LPRECT prcPos,
						LPRECT prcClip,
						LPOLEINPLACEFRAMEINFO pFI )
{
	ASSERT(m_pSiteWnd != NULL);
    
	*ppIIPUIWindow = NULL;
    m_pUnkownObj->QueryInterface(IID_IOleInPlaceFrame, (void **)ppIIPFrame);
    
    if (NULL != prcPos && m_pSiteWnd != NULL)
        m_pSiteWnd->GetClientRect(prcPos);

    *prcClip = *prcPos;

    pFI->cb = sizeof(OLEINPLACEFRAMEINFO);
    pFI->fMDIApp = FALSE;


	CFrameWnd* pFrame = m_pSiteWnd->GetParentFrame();
	if ( pFrame != NULL )
		pFI->hwndFrame = pFrame->m_hWnd;

    pFI->haccel = NULL;
    pFI->cAccelEntries = 0;

    return NOERROR;
}


/*
 * CFcIOleInPlaceSite::Scroll
 *
 * Purpose:
 *  Asks the container to scroll the document, and thus the object,
 *  by the given amounts in the sz parameter.
 *
 * Parameters:
 *  sz              SIZE containing signed horizontal and vertical
 *                  extents by which the container should scroll.
 *                  These are in device units.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcIOleInPlaceSite::Scroll( SIZE /*sz*/ )
{
    //Not needed for DocObjects
    return E_NOTIMPL;
}


/*
 * CFcIOleInPlaceSite::OnPosRectChange
 *
 * Purpose:
 *  Informs the container that the in-place object was resized.
 *  The container must call IOleInPlaceObject::SetObjectRects.
 *  This does not change the site's rectangle in any case.
 *
 * Parameters:
 *  prcPos          LPCRECT containing the new size of the object.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcIOleInPlaceSite::OnPosRectChange( LPCRECT /*prcPos*/ )
{
    //Not needed for DocObjects
    return E_NOTIMPL;
}
