

#include "stdafx.h"
#include <AFXPRIV.H>
#include "FcHtmlEd.h"
#include "FcFontBar.h"

#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"
#include "site.h"


#include <TriEdCID.h>
#include <TRIEDIID.h>
#include <dhtmled.h>

#include "FcHtmlColor.h"
#include "FcStyleDlg.h"
#include "FcInsertTableDlg.h"
#include "FcDlgInsertHtml.h"
#include "FcDlgPageProperties.h"
#include "FcPropertyPageGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdView

IMPLEMENT_DYNCREATE(CFcHtmlEdView, CView)

BEGIN_MESSAGE_MAP(CFcHtmlEdView, CView)
	//{{AFX_MSG_MAP(CFcHtmlEdView)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_CBN_SELENDOK(ID_FORMAT_FONTNAME, OnFontNameChange)
	ON_CBN_SELENDOK(ID_FORMAT_FONTSIZE, OnFontSizeChange)
	ON_CBN_SELENDOK(ID_FORMAT_BLOCKFORMAT, OnHtmlTagChange)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO , OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO , OnEditRedo)
	ON_COMMAND(ID_EDIT_COPY , OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT  , OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_FIND , OnEditFind)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_INSERT_PICTURE, OnInsertPicture)
	ON_COMMAND(ID_INSERT_ANCHOR, OnInsertAnchor)
	ON_COMMAND(ID_INSERT_HTML, OnInsertHtml)
	ON_COMMAND(ID_FORMAT_ALIGN_LEFT, OnFormatAlignLeft)
	ON_COMMAND(ID_FORMAT_ALIGN_CENTER, OnFormatAlignCenter)
	ON_COMMAND(ID_FORMAT_ALIGN_RIGHT, OnFormatAlignRight)
	ON_COMMAND(ID_FORMAT_FONT, OnFormatFont)
	ON_COMMAND(ID_FORMAT_STYLE, OnFormatStyle)
	ON_COMMAND(ID_FORMAT_BOLD, OnFormatBold)
	ON_COMMAND(ID_FORMAT_ITALIC, OnFormatItalic)
	ON_COMMAND(ID_FORMAT_UNDERLINE, OnFormatUnderline)
	ON_COMMAND(ID_FORMAT_BULLETS, OnFormatBullets)
	ON_COMMAND(ID_FORMAT_NUMBER, OnFormatNumber)
	ON_COMMAND(ID_FORMAT_PAGE, OnFormatPage)
	ON_COMMAND(ID_FORMAT_INCREASEINDENT, OnFormatIncreaseIndent)
	ON_COMMAND(ID_FORMAT_DECREASEINDENT, OnFormatDecreaseIndent)
	ON_COMMAND(ID_FORMAT_TEXT_COLOR, OnFormatTextColor)
	ON_COMMAND(ID_FORMAT_ABS_POSITION, OnFormatAbsolute)
	ON_COMMAND(ID_FORMAT_LOCK, OnFormatLock)
	ON_COMMAND(ID_FORMAT_GRID, OnFormatGrid)
	ON_COMMAND(ID_ZORDER_BACKWARD, OnZorderBackward)
	ON_COMMAND(ID_ZORDER_BEHINDTEXT, OnZorderBehindtext)
	ON_COMMAND(ID_ZORDER_FORWARD, OnZorderForward)
	ON_COMMAND(ID_ZORDER_FRONTOFTEXT, OnZorderFrontoftext)
	ON_COMMAND(ID_ZORDER_TO_BACK, OnZorderToBack)
	ON_COMMAND(ID_ZORDER_TO_FRONT, OnZorderToFront)
	ON_COMMAND(ID_TABLE_INSERTTABLE, OnTableInsertTable)
	ON_COMMAND(ID_TABLE_INSERTCELL, OnTableInsertCell)
	ON_COMMAND(ID_TABLE_INSERTCOL, OnTableInsertCol)
	ON_COMMAND(ID_TABLE_INSERTROW, OnTableInsertRow)
	ON_COMMAND(ID_TABLE_MERGECELLS, OnTableMergeCells)
	ON_COMMAND(ID_TABLE_SPLITCELL, OnTableSplitCell)
	ON_COMMAND(ID_TABLE_DELETECELLS, OnTableDeleteCells)
	ON_COMMAND(ID_TABLE_DELETECOLS, OnTableDeleteCols)
	ON_COMMAND(ID_TABLE_DELETEROWS, OnTableDeleteRows)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT           , OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR           , OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO            , OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO            , OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY            , OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT             , OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE           , OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND            , OnUpdateEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL      , OnUpdateEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_INSERT_PICTURE       , OnUpdateInsertPicture)
	ON_UPDATE_COMMAND_UI(ID_INSERT_ANCHOR        , OnUpdateInsertAnchor)
	ON_UPDATE_COMMAND_UI(ID_INSERT_HTML          , OnUpdateInsertHtml)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_STYLE         , OnUpdateFormatStyle)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_FONT          , OnUpdateFormatFont)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_DECREASEINDENT, OnUpdateFormatDecreaseindent)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_INCREASEINDENT, OnUpdateFormatIncreaseindent)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_BOLD          , OnUpdateFormatBold)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_CENTER  , OnUpdateFormatAlignCenter)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_LEFT    , OnUpdateFormatAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_RIGHT   , OnUpdateFormatAlignRight)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_BULLETS       , OnUpdateFormatBullets)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ITALIC        , OnUpdateFormatItalic)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_NUMBER        , OnUpdateFormatNumber)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_UNDERLINE     , OnUpdateFormatUnderline)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TEXT_COLOR    , OnUpdateFormatColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ABS_POSITION  , OnUpdateFormatAbsolute)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_LOCK          , OnUpdateFormatLock)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_GRID          , OnUpdateFormatGrid)
	ON_UPDATE_COMMAND_UI(ID_ZORDER_BACKWARD      , OnUpdateZorderBackward)
	ON_UPDATE_COMMAND_UI(ID_ZORDER_BEHINDTEXT    , OnUpdateZorderBehindtext)
	ON_UPDATE_COMMAND_UI(ID_ZORDER_FORWARD       , OnUpdateZorderForward)
	ON_UPDATE_COMMAND_UI(ID_ZORDER_FRONTOFTEXT   , OnUpdateZorderFrontoftext)
	ON_UPDATE_COMMAND_UI(ID_ZORDER_TO_BACK       , OnUpdateZorderToBack)
	ON_UPDATE_COMMAND_UI(ID_ZORDER_TO_FRONT      , OnUpdateZorderToFront)
	ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTTABLE    , OnUpdateTableInsertTable)
	ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTCELL     , OnUpdateTableInsertCell )
	ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTCOL      , OnUpdateTableInsertCol  )
	ON_UPDATE_COMMAND_UI(ID_TABLE_INSERTROW      , OnUpdateTableInsertRow  )
	ON_UPDATE_COMMAND_UI(ID_TABLE_MERGECELLS     , OnUpdateTableMergeCells )
	ON_UPDATE_COMMAND_UI(ID_TABLE_SPLITCELL      , OnUpdateTableSplitCell  )
	ON_UPDATE_COMMAND_UI(ID_TABLE_DELETECELLS    , OnUpdateTableDeleteCells)
	ON_UPDATE_COMMAND_UI(ID_TABLE_DELETECOLS     , OnUpdateTableDeleteCols )
	ON_UPDATE_COMMAND_UI(ID_TABLE_DELETEROWS     , OnUpdateTableDeleteRows )
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdView construction/destruction

CFcHtmlEdView::CFcHtmlEdView()
{
	m_cRef = 0;
	m_hWndObj = NULL;
    m_pSite = NULL;
    m_pIOleIPActiveObject = NULL;
	m_ChangeCount = 0;
    m_bLoaded = FALSE;
}

CFcHtmlEdView::~CFcHtmlEdView()
{
	// Debug check that nobody is trying to hang onto us!
	ASSERT( m_cRef == 0 );
}

BOOL CFcHtmlEdView::CreateDocObject( TCHAR* pchPath )
{    
	//Create and initialize the site for the ActiveX Document
	m_pSite = new CSite(this);
    if ( NULL == m_pSite )
        return FALSE;

    /*
     * Now tell the site to create an object in it using the filename
     * and the storage we opened.  The site will create a sub-storage
     * for the doc object's use.
     */

	// Ask the Site to Create the Activex Document
    if ( !m_pSite->Create(pchPath))
        return FALSE;

    //We created the thing, now activate it with "Show"
    m_pSite->Activate( OLEIVERB_SHOW );


    return TRUE;        
}

LPUNKNOWN CFcHtmlEdView::GetTriEditObject()
{ 
	ASSERT(m_pSite != NULL);
	return m_pSite->GetObjectUnknown(); 
}

BOOL CFcHtmlEdView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdView drawing

void CFcHtmlEdView::OnDraw(CDC* pDC)
{
	CFcHtmlEdDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdView printing

BOOL CFcHtmlEdView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFcHtmlEdView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFcHtmlEdView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CFcHtmlEdView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdView diagnostics

#ifdef _DEBUG
void CFcHtmlEdView::AssertValid() const
{
	CView::AssertValid();
}

void CFcHtmlEdView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFcHtmlEdDoc* CFcHtmlEdView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFcHtmlEdDoc)));
	return (CFcHtmlEdDoc*)m_pDocument;
}
#endif //_DEBUG




/*
 * IUnknown implementation
 */
/*
 * CFcHtmlEdView::QueryInterface
 * CFcHtmlEdView::AddRef
 * CFcHtmlEdView::Release
 */
STDMETHODIMP CFcHtmlEdView::QueryInterface( REFIID riid, void **ppv )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    /*
     * We provide IOleInPlaceFrame and IOleCommandTarget
	 *   interfaces here for the ActiveX Document hosting
	 */
    *ppv = NULL;

    if ( IID_IUnknown == riid || IID_IOleInPlaceUIWindow == riid ||
         IID_IOleWindow == riid || IID_IOleInPlaceFrame == riid  ||
		 IID_IAdviseSink == riid)
	{
        *ppv = (IOleInPlaceFrame *)this;
	}

    if (  IID_IAdviseSink == riid)
	{
        *ppv = (IAdviseSink *)this;
	}
    if (  IID_IOleCommandTarget == riid)
	{
        *ppv = (IOleCommandTarget *)this;
	}

    if ( NULL != *ppv )
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return NOERROR;
    }

    return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CFcHtmlEdView::AddRef( void )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcHtmlEdView::Release( void )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    //Nothing special happening here-- life if user-controlled.
	// Debug check to see we don't fall below 0
	ASSERT( m_cRef != 0 );
    return --m_cRef;
}


/*
 * IOleInPlaceFrame implementation
 */
/*
 * CFcHtmlEdView::GetWindow
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
STDMETHODIMP CFcHtmlEdView::GetWindow( HWND* phWnd )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
	CFrameWnd* pFrame = GetParentFrame();
	if ( pFrame != NULL )
	{
		*phWnd = pFrame->m_hWnd;
	}
	return NOERROR;
}



/*
 * CFcHtmlEdView::ContextSensitiveHelp
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
STDMETHODIMP CFcHtmlEdView::ContextSensitiveHelp( BOOL fEnterMode )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    return NOERROR;
}



/*
 * CFcHtmlEdView::GetBorder
 *
 * Purpose:
 *  Returns the rectangle in which the container is willing to
 *  negotiate about an object's adornments.
 *
 * Parameters:
 *  prcBorder       LPRECT in which to store the rectangle.
 *
 * Return Value:
 *  HRESULT         NOERROR if all is well, INPLACE_E_NOTOOLSPACE
 *                  if there is no negotiable space.
 */
STDMETHODIMP CFcHtmlEdView::GetBorder( LPRECT prcBorder )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    if ( NULL == prcBorder )
	{
        return E_INVALIDARG;
	}

    //We return all the client area space
    GetClientRect( prcBorder );
    return NOERROR;
}


/*
 * CFcHtmlEdView::RequestBorderSpace
 *
 * Purpose:
 *  Asks the container if it can surrender the amount of space
 *  in pBW that the object would like for it's adornments.  The
 *  container does nothing but validate the spaces on this call.
 *
 * Parameters:
 *  pBW             LPCBORDERWIDTHS containing the requested space.
 *                  The values are the amount of space requested
 *                  from each side of the relevant window.
 *
 * Return Value:
 *  HRESULT         NOERROR if we can give up space,
 *                  INPLACE_E_NOTOOLSPACE otherwise.
 */
STDMETHODIMP CFcHtmlEdView::RequestBorderSpace( LPCBORDERWIDTHS /*pBW*/ )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    // We have no border space restrictions
    return NOERROR;
}


/*
 * CFcHtmlEdView::SetBorderSpace
 *
 * Purpose:
 *  Called when the object now officially requests that the
 *  container surrender border space it previously allowed
 *  in RequestBorderSpace.  The container should resize windows
 *  appropriately to surrender this space.
 *
 * Parameters:
 *  pBW             LPCBORDERWIDTHS containing the amount of space
 *                  from each side of the relevant window that the
 *                  object is now reserving.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcHtmlEdView::SetBorderSpace( LPCBORDERWIDTHS /*pBW*/ )
{
	AFX_MANAGE_STATE(this->m_pModuleState);

    return NOERROR;
}




/*
 * CFcHtmlEdView::SetActiveObject
 *
 * Purpose:
 *  Provides the container with the object's IOleInPlaceActiveObject
 *  pointer
 *
 * Parameters:
 *  pIIPActiveObj   LPOLEINPLACEACTIVEOBJECT of interest.
 *  pszObj          LPCOLESTR naming the object.  Not used.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcHtmlEdView::SetActiveObject( LPOLEINPLACEACTIVEOBJECT pIIPActiveObj,
											LPCOLESTR /*pszObj*/)
{
	AFX_MANAGE_STATE(this->m_pModuleState);
	// If we already have an active Object then release it.
    if ( NULL != m_pIOleIPActiveObject )
	{
        m_pIOleIPActiveObject->Release();
	}

    //NULLs m_pIOleIPActiveObject if pIIPActiveObj is NULL
    m_pIOleIPActiveObject = pIIPActiveObj;

    if ( NULL != m_pIOleIPActiveObject )
	{
        m_pIOleIPActiveObject->AddRef();
		m_pIOleIPActiveObject->GetWindow( &m_hWndObj );
	}
    return NOERROR;
}



/*
 * CFcHtmlEdView::InsertMenus
 *
 * Purpose:
 *  Instructs the container to place its in-place menu items where
 *  necessary in the given menu and to fill in elements 0, 2, and 4
 *  of the OLEMENUGROUPWIDTHS array to indicate how many top-level
 *  items are in each group.
 *
 * Parameters:
 *  hMenu           HMENU in which to add popups.
 *  pMGW            LPOLEMENUGROUPWIDTHS in which to store the
 *                  width of each container menu group.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcHtmlEdView::InsertMenus( HMENU /*hMenu*/, LPOLEMENUGROUPWIDTHS /*pMGW*/ )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
	// We've turned off the MSHTML.DLL Menus so we don't expect any merging to go on!
	return E_NOTIMPL;
}


/*
 * CFcHtmlEdView::SetMenu
 *
 * Purpose:
 *  Instructs the container to replace whatever menu it's currently
 *  using with the given menu and to call OleSetMenuDescritor so OLE
 *  knows to whom to dispatch messages.
 *
 * Parameters:
 *  hMenu           HMENU to show.
 *  hOLEMenu        HOLEMENU to the menu descriptor.
 *  hWndObj         HWND of the active object to which messages are
 *                  dispatched.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcHtmlEdView::SetMenu( HMENU /*hMenu*/, HOLEMENU /*hOLEMenu*/, HWND /*hWndObj*/ )
{
	// We've turned off the MSHTML.DLL Menus so we don't expect any merging to go on!
	return E_NOTIMPL;
}



/*
 * CFcHtmlEdView::RemoveMenus
 *
 * Purpose:
 *  Asks the container to remove any menus it put into hMenu in
 *  InsertMenus.
 *
 * Parameters:
 *  hMenu           HMENU from which to remove the container's
 *                  items.
 *
 * Return Value:
 *  HRESULT         NOERROR
 */
STDMETHODIMP CFcHtmlEdView::RemoveMenus( HMENU /*hMenu*/ )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
	// We've turned off the MSHTML.DLL Menus so we don't expect any merging to go on!
	return E_NOTIMPL;
}




/*
 * CFcHtmlEdView::SetStatusText
 *
 * Purpose:
 *  Asks the container to place some text in a status line, if one
 *  exists.  If the container does not have a status line it
 *  should return E_FAIL here in which case the object could
 *  display its own.
 *
 * Parameters:
 *  pszText         LPCOLESTR to display.
 *
 * Return Value:
 *  HRESULT         NOERROR if successful, S_TRUNCATED if not all
 *                  of the text could be displayed, or E_FAIL if
 *                  the container has no status line.
 */
STDMETHODIMP CFcHtmlEdView::SetStatusText( LPCOLESTR pszText )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
	CFrameWnd* pFrame = GetTopLevelFrame();
	if ( pFrame != NULL )
	{
		USES_CONVERSION;
		pFrame->SetMessageText( OLE2T(pszText) );
	}
    return NOERROR;
}



/*
 * CFcHtmlEdView::EnableModeless
 *
 * Purpose:
 *  Instructs the container to show or hide any modeless popup
 *  windows that it may be using.
 *
 * Parameters:
 *  fEnable         BOOL indicating to enable/show the windows
 *                  (TRUE) or to hide them (FALSE).
 *
 * Return Value:
 *  HRESULT         NOERROR
 */

STDMETHODIMP CFcHtmlEdView::EnableModeless( BOOL /*fEnable*/ )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    return NOERROR;
}


/*
 * CFcHtmlEdView::TranslateAccelerator
 *
 * Purpose:
 *  When dealing with an in-place object from an EXE server, this
 *  is called to give the container a chance to process accelerators
 *  after the server has looked at the message.
 *
 * Parameters:
 *  pMSG            LPMSG for the container to examine.
 *  wID             WORD the identifier in the container's
 *                  accelerator table (from IOleInPlaceSite
 *                  ::GetWindowContext) for this message (OLE does
 *                  some translation before calling).
 *
 * Return Value:
 *  HRESULT         NOERROR if the keystroke was used,
 *                  S_FALSE otherwise.
 */
STDMETHODIMP CFcHtmlEdView::TranslateAccelerator( LPMSG /*pMSG*/, WORD /*wID*/ )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
	//We should probably forward this to the top level frame!
    return S_FALSE;
}


/*
 * IAdviseSink implementation
 */

STDMETHODIMP_(void) CFcHtmlEdView::OnDataChange(
	LPFORMATETC /*pFEIn*/,
	LPSTGMEDIUM /*pSTM*/)
{
	m_ChangeCount++;
	if (m_bLoaded)
	{
		GetDocument()->SetModifiedFlag();
	}
}

STDMETHODIMP_(void) CFcHtmlEdView::OnViewChange(DWORD /*dwAspect*/,
												LONG /*lindex*/)
{    
}

STDMETHODIMP_(void) CFcHtmlEdView::OnRename( LPMONIKER /*pmk*/ )
{
}

STDMETHODIMP_(void) CFcHtmlEdView::OnSave()
{
}

STDMETHODIMP_(void) CFcHtmlEdView::OnClose()
{
}


/*
 * IOleCommandTarget methods.
 */

STDMETHODIMP CFcHtmlEdView::QueryStatus( const GUID* pguidCmdGroup, ULONG cCmds,
				OLECMD* prgCmds, OLECMDTEXT* pCmdText )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    if ( pguidCmdGroup != NULL )
	{
		// It's a nonstandard group!!
        return OLECMDERR_E_UNKNOWNGROUP;
	}

    MSOCMD*     pCmd;
    INT         c;
    HRESULT     hr = S_OK;

    // By default command text is NOT SUPPORTED.
    if ( pCmdText && ( pCmdText->cmdtextf != OLECMDTEXTF_NONE ) )
	{
        pCmdText->cwActual = 0;
	}

    // Loop through each command in the ary, setting the status of each.
    for ( pCmd = prgCmds, c = cCmds; --c >= 0; pCmd++ )
    {
        // By default command status is NOT SUPPORTED.
        pCmd->cmdf = 0;

        switch ( pCmd->cmdID )
        {
			case OLECMDID_SETPROGRESSTEXT:
			case OLECMDID_SETTITLE:
//			case OLECMDID_UPDATECOMMANDS:
				pCmd->cmdf = OLECMDF_SUPPORTED;
				break;

			case OLECMDID_NEW:
			case OLECMDID_OPEN:
			case OLECMDID_SAVE:
				pCmd->cmdf = (MSOCMDF_SUPPORTED | MSOCMDF_ENABLED);
				break;
        }
    }

    return hr;
}
        

STDMETHODIMP CFcHtmlEdView::Exec( const GUID* pguidCmdGroup, DWORD nCmdID,
    DWORD nCmdexecopt, VARIANTARG* pvaIn, VARIANTARG*  )
{
	AFX_MANAGE_STATE(this->m_pModuleState);
    HRESULT hr = S_OK;

    if ( pguidCmdGroup == NULL )
    {
		USES_CONVERSION;

        switch (nCmdID)
        {
		default:
		case OLECMDID_SAVE:
			hr = OLECMDERR_E_NOTSUPPORTED;
			break;

		case OLECMDID_SETPROGRESSTEXT:
			if ( pvaIn && V_VT(pvaIn) == VT_BSTR )
			{
				CFrameWnd* pFrame = GetTopLevelFrame();
				if ( pFrame != NULL )
					pFrame->SetMessageText( OLE2T(V_BSTR(pvaIn)) );
				else
					hr = OLECMDERR_E_NOTSUPPORTED;
			}
			break;

		case OLECMDID_UPDATECOMMANDS:
			UpdateFontBar();
			break;
/*
		case OLECMDID_SETTITLE:
			if (pvaIn && V_VT(pvaIn) == VT_BSTR)
			{
				CFcHtmlEdDoc* pDoc = GetDocument();
				ASSERT_VALID(pDoc);
				pDoc->SetTitle(OLE2T(V_BSTR(pvaIn)));
			}
			else
			{
				hr = OLECMDERR_E_NOTSUPPORTED;
			}
			break;
*/

        }
    }
    else
    {
        hr = OLECMDERR_E_UNKNOWNGROUP;
    }
    return hr;
}


DWORD CFcHtmlEdView::QueryTriEditCommand( ULONG ucmdID )
{
	if ( m_pSite != NULL )
	{
		LPOLECOMMANDTARGET pCommandTarget = m_pSite->GetCommandTarget();
		if (pCommandTarget != NULL)
		{
			MSOCMD msocmd;
			msocmd.cmdID = ucmdID;
			msocmd.cmdf  = 0;

			HRESULT hr = pCommandTarget->QueryStatus(&GUID_TriEditCommandGroup, 1, &msocmd, NULL);
			if (SUCCEEDED(hr))
				return msocmd.cmdf;
		}
	}
	return 0;
}


HRESULT CFcHtmlEdView::ExecTriEditCommand(ULONG ucmdID, VARIANT* pVarIn, VARIANT* pVarOut, BOOL bPromptUser)
{
	HRESULT hr = S_FALSE;
	if (m_pSite != NULL)
	{
		LPOLECOMMANDTARGET pCommandTarget = m_pSite->GetCommandTarget();
		ASSERT(pCommandTarget != NULL);
		if (pCommandTarget != NULL)
		{
			DWORD dwCmdOpt = MSOCMDEXECOPT_DONTPROMPTUSER;
			if (bPromptUser)
				dwCmdOpt = MSOCMDEXECOPT_PROMPTUSER;

			hr = pCommandTarget->Exec(
				&GUID_TriEditCommandGroup,
				ucmdID,
				dwCmdOpt,
				pVarIn,
				pVarOut);
		}
	}
	return hr;
}



/////////////////////////////////////////////////////////////////////////////
// CFcHtmlEdView message handlers

void CFcHtmlEdView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_pSite != NULL)
		m_pSite->UpdateDocumentViewRect();
	
}

void CFcHtmlEdView::OnInitialUpdate() 
{
	if (m_pSite == NULL)
	{
		if ( !CreateDocObject(NULL))
		{
			// Destroy this window
			MessageBox( "Path Name Invalid", "Error", MB_ICONEXCLAMATION );
			GetParent()->PostMessage( WM_CLOSE, 0, 0);
		}
	}

	CView::OnInitialUpdate();
}

void CFcHtmlEdView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	//Load();	
}

void CFcHtmlEdView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate && pActivateView == this)
	{
		InitHtmlTagNames();
		UpdateFontBar();

		// set alignmnet grid to current values
		CFcHtmlEdOptions* pO = ((CFcHtmlEdApp*)AfxGetApp())->GetOptions();
		SetGrid(pO->GetGridHorizontal(), pO->GetGridVertical(), !pO->IsGridActive());
	}
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CFcHtmlEdView::Load() 
{
	m_ChangeCount = 0;
	if (m_pSite != NULL)
    {
        HRESULT hr;

		hr = m_pSite->Load((TCHAR*)LPCTSTR(GetDocument()->GetEdDocPathName()));
        ::Sleep(500);
        m_bLoaded = TRUE;
        return (SUCCEEDED (hr));
    }
	else
		return FALSE;
}

BOOL CFcHtmlEdView::Save() 
{
	m_ChangeCount = 0;
	if (m_pSite != NULL)
		return SUCCEEDED(m_pSite->Save((TCHAR*)LPCTSTR(GetDocument()->GetEdDocPathName())));
	else
		return FALSE;
}

void CFcHtmlEdView::OnDestroy() 
{
	if ( m_pSite != NULL )
	{
		CSite *pSite = m_pSite;			//Prevents reentry
		m_pSite = NULL;

        pSite->Close();	// Cleans up the storage and closes object
		pSite->Release();
		pSite = NULL;
	}
	CView::OnDestroy();
}

void CFcHtmlEdView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	// Give the focus to the ActiveX Document window
    if ( m_hWndObj != NULL )
	{
		::SetFocus( m_hWndObj );
	}
	
}


BOOL CFcHtmlEdView::PreTranslateMessage(MSG* pMsg) 
{
    if ( NULL != m_pIOleIPActiveObject )
    {
        HRESULT hr = m_pIOleIPActiveObject->TranslateAccelerator( pMsg );

        //If the object translated the accelerator, we're done
        if ( NOERROR == hr )
            return TRUE;
    }	
	return CView::PreTranslateMessage(pMsg);
}

void CFcHtmlEdView::OnFontNameChange() 
{
	CFcFontBar* pFontBar = GetFontBar();
	if (pFontBar != NULL)
	{
		CString fontName = pFontBar->GetSelectedFontName();
		ASSERT(fontName.GetLength() > 0);
		if (fontName.GetLength() > 0)
		{
			VARIANT va;
			VariantInit(&va);
			va.vt = VT_BSTR;
			va.bstrVal = fontName.AllocSysString();

			ExecTriEditCommand(IDM_TRIED_FONTNAME, &va, NULL, FALSE);

            VariantClear(&va);
		}
	}
}

void CFcHtmlEdView::OnFontSizeChange() 
{
	CFcFontBar* pFontBar = GetFontBar();
	if (pFontBar != NULL)
	{
		int fontSize = pFontBar->GetSelectedFontSize();
		ASSERT(fontSize > 0);
		if (fontSize > 0)
		{
			VARIANT va;
			VariantInit(&va);
			va.vt = VT_I4;
			va.lVal = fontSize;  // -2 through +4

			ExecTriEditCommand(IDM_TRIED_FONTSIZE, &va, NULL, FALSE);

            VariantClear(&va);
		}
	}
}
void CFcHtmlEdView::OnHtmlTagChange() 
{
	CFcFontBar* pFontBar = GetFontBar();
	if (pFontBar != NULL)
	{
		CString HtmlTag = pFontBar->GetSelectedHtmlTag();
		ASSERT(HtmlTag.GetLength() > 0);
		if (HtmlTag.GetLength() > 0)
		{
			VARIANT va;
			VariantInit(&va);
			va.vt = VT_BSTR;
			va.bstrVal = HtmlTag.AllocSysString();

			ExecTriEditCommand(IDM_TRIED_BLOCKFMT, &va, NULL, FALSE);

            VariantClear(&va);
		}
	}
}


void CFcHtmlEdView::InitHtmlTagNames()
{
	CFcFontBar* pFontBar = GetFontBar();
	if (pFontBar != NULL)
	{
		VARIANTARG varRange;
		VariantInit(&varRange);
		varRange.vt = VT_ARRAY;
		varRange.parray = NULL;

		HRESULT hr = ExecTriEditCommand(IDM_TRIED_GETBLOCKFMTS, NULL, &varRange, FALSE);
		if (SUCCEEDED(hr))
		{
			LONG iLBound, iUBound;
			SAFEARRAY* psa = V_ARRAY(&varRange);
			SafeArrayGetLBound(psa, 1, &iLBound);
			SafeArrayGetUBound(psa, 1, &iUBound);

			BSTR  bstr = NULL;
			for (LONG ii = iLBound; ii <= iUBound; ii++)
			{
				SafeArrayGetElement(psa, &ii, &bstr);
				if(bstr != NULL)
				{
					pFontBar->AddHtmlTag(bstr, ii == iLBound);
					SysFreeString(bstr);
					bstr = NULL;
				}
			}
			//SafeArrayDestroy(psa);
		}
		VariantClear(&varRange);
	}
}

void CFcHtmlEdView::UpdateFontBar()
{
	CFcFontBar* pFontBar = GetFontBar();
	if (pFontBar != NULL)
	{
		HRESULT hr;
		VARIANT va;
		VariantInit(&va);

		// 	Update Html tag combo
		DWORD dwHtmlTagStatus = QueryTriEditCommand(IDM_TRIED_BLOCKFMT);
		CString HtmlTag;
		if (dwHtmlTagStatus & OLECMDF_ENABLED)
		{
			va.vt = VT_BSTR;
			va.bstrVal = NULL;
			hr = ExecTriEditCommand(IDM_TRIED_BLOCKFMT, NULL, &va, FALSE);
			if (SUCCEEDED(hr) && va.vt == VT_BSTR && va.bstrVal != NULL)
			{
				HtmlTag = va.bstrVal;
			}
			VariantClear(&va);
		}
		pFontBar->SetSelectedHtmlTag(HtmlTag, dwHtmlTagStatus & OLECMDF_ENABLED); 
			
		// 	Update font name combo
		DWORD dwFontNameStatus = QueryTriEditCommand(IDM_TRIED_FONTNAME);
		CString FontName;
		if (dwFontNameStatus & OLECMDF_ENABLED)
		{
			hr = ExecTriEditCommand(IDM_TRIED_FONTNAME, NULL, &va, FALSE);
			if (SUCCEEDED(hr) && va.vt == VT_BSTR && va.bstrVal != NULL)
			{
				FontName = va.bstrVal;
			}
			VariantClear(&va);
		}
		pFontBar->SetSelectedFontName(FontName, dwFontNameStatus & OLECMDF_ENABLED); 

		// 	Update font size combo
		DWORD dwFontSizeStatus = QueryTriEditCommand(IDM_TRIED_FONTSIZE);
		int FontSize = -1;
		if (dwFontSizeStatus & OLECMDF_ENABLED)
		{
			hr = ExecTriEditCommand(IDM_TRIED_FONTSIZE, NULL, &va, FALSE);
			if (SUCCEEDED(hr) && va.vt == VT_I4 && va.lVal >= 1)
			{
				FontSize = va.lVal;
			}
			VariantClear(&va);
		}
		pFontBar->SetSelectedFontSize(FontSize, dwFontSizeStatus & OLECMDF_ENABLED); 
    }
}

CFcFontBar* CFcHtmlEdView::GetFontBar()
{
	CWnd* pFrame = this;
	while ((pFrame = pFrame->GetParentFrame()) != NULL)
	{
		CControlBar* pBar = ((CFrameWnd*)pFrame)->GetControlBar(IDD_FONT_BAR);
		if (pBar != NULL)
			return (CFcFontBar *)pBar;
	}
	return NULL;
}

void CFcHtmlEdView::OnFormatFont() 
{
	ExecTriEditCommand(IDM_TRIED_FONT, NULL, NULL, TRUE);
}

void CFcHtmlEdView::OnFormatStyle() 
{
	CFcDlgStyle dlg(this);
	dlg.DoModal();
}


void CFcHtmlEdView::OnFormatAlignLeft()      { ExecTriEditCommand(IDM_TRIED_JUSTIFYLEFT  , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatAlignCenter()    { ExecTriEditCommand(IDM_TRIED_JUSTIFYCENTER, NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatAlignRight()     { ExecTriEditCommand(IDM_TRIED_JUSTIFYRIGHT , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatBold()           { ExecTriEditCommand(IDM_TRIED_BOLD         , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatItalic()         { ExecTriEditCommand(IDM_TRIED_ITALIC       , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatUnderline()      { ExecTriEditCommand(IDM_TRIED_UNDERLINE    , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatBullets()        { ExecTriEditCommand(IDM_TRIED_UNORDERLIST  , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatNumber()         { ExecTriEditCommand(IDM_TRIED_ORDERLIST    , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatIncreaseIndent() { ExecTriEditCommand(IDM_TRIED_INDENT       , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatDecreaseIndent() { ExecTriEditCommand(IDM_TRIED_OUTDENT      , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnFormatAbsolute()       { ExecTriEditCommand(IDM_TRIED_MAKE_ABSOLUTE, NULL, NULL, TRUE ); }	
void CFcHtmlEdView::OnFormatLock()           { ExecTriEditCommand(IDM_TRIED_LOCK_ELEMENT , NULL, NULL, FALSE); }


void CFcHtmlEdView::OnFormatPage() 
{
	CFcDlgPageProperties  dlg(this);
	dlg.DoModal();
}

void CFcHtmlEdView::OnFormatTextColor() 
{
	CColorDialog* pColorDlg = new CColorDialog;
	if ( pColorDlg->DoModal() == IDOK )
	{
		VARIANT va;
		VariantInit(&va);
		va.vt = VT_BSTR;
		va.bstrVal = FcColorRef2HtmlColor(pColorDlg->GetColor()).AllocSysString();
		ExecTriEditCommand(IDM_TRIED_FORECOLOR, &va, NULL, FALSE);
        VariantClear(&va);
	}
	delete pColorDlg;		
}

void CFcHtmlEdView::OnZorderToBack()         { ExecTriEditCommand(IDM_TRIED_SEND_TO_BACK  , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnZorderToFront()        { ExecTriEditCommand(IDM_TRIED_SEND_TO_FRONT , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnZorderBackward()       { ExecTriEditCommand(IDM_TRIED_SEND_BACKWARD , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnZorderForward()        { ExecTriEditCommand(IDM_TRIED_SEND_FORWARD  , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnZorderBehindtext()     { ExecTriEditCommand(IDM_TRIED_SEND_BEHIND_1D, NULL, NULL, FALSE); }
void CFcHtmlEdView::OnZorderFrontoftext()    { ExecTriEditCommand(IDM_TRIED_SEND_FRONT_1D , NULL, NULL, FALSE); }


void CFcHtmlEdView::OnTableInsertTable() 
{
	CFcDlgInsertTable dlg(this);
	dlg.DoModal();
}
void CFcHtmlEdView::OnTableInsertCell()      { ExecTriEditCommand(IDM_TRIED_INSERTCELL   , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableInsertCol()       { ExecTriEditCommand(IDM_TRIED_INSERTCOL    , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableInsertRow()       { ExecTriEditCommand(IDM_TRIED_INSERTROW    , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableMergeCells()      { ExecTriEditCommand(IDM_TRIED_MERGECELLS   , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableSplitCell()       { ExecTriEditCommand(IDM_TRIED_SPLITCELL    , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableDeleteCells()     { ExecTriEditCommand(IDM_TRIED_DELETECELLS  , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableDeleteCols()      { ExecTriEditCommand(IDM_TRIED_DELETECOLS   , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnTableDeleteRows()      { ExecTriEditCommand(IDM_TRIED_DELETEROWS   , NULL, NULL, FALSE); }

void CFcHtmlEdView::OnFilePrint()            { ExecTriEditCommand(IDM_TRIED_PRINT        , NULL, NULL, TRUE);  }	

void CFcHtmlEdView::OnEditClear()            { ExecTriEditCommand(IDM_TRIED_DELETE        , NULL, NULL, FALSE); }
void CFcHtmlEdView::OnEditUndo()             { ExecTriEditCommand(IDM_TRIED_UNDO         , NULL, NULL, FALSE); }	
void CFcHtmlEdView::OnEditRedo()             { ExecTriEditCommand(IDM_TRIED_REDO         , NULL, NULL, FALSE); }	
void CFcHtmlEdView::OnEditCut()              { ExecTriEditCommand(IDM_TRIED_CUT          , NULL, NULL, FALSE); }	
void CFcHtmlEdView::OnEditCopy()             { ExecTriEditCommand(IDM_TRIED_COPY         , NULL, NULL, FALSE); }	
void CFcHtmlEdView::OnEditPaste()            { ExecTriEditCommand(IDM_TRIED_PASTE        , NULL, NULL, FALSE); }	
void CFcHtmlEdView::OnEditFind()             { ExecTriEditCommand(IDM_TRIED_FIND         , NULL, NULL, TRUE ); }	
void CFcHtmlEdView::OnEditSelectAll()        { ExecTriEditCommand(IDM_TRIED_SELECTALL    , NULL, NULL, TRUE ); }	

void CFcHtmlEdView::OnInsertPicture()        { ExecTriEditCommand(IDM_TRIED_IMAGE        , NULL, NULL, TRUE ); }
void CFcHtmlEdView::OnInsertAnchor()         { ExecTriEditCommand(IDM_TRIED_HYPERLINK    , NULL, NULL, TRUE ); }
void CFcHtmlEdView::OnInsertHtml() 
{
	CFcDlgInsertHtml dlg(this);
	if (dlg.DoModal() == IDOK)
		InsertHtml(dlg.m_HtmlText);
}

void CFcHtmlEdView::OnUpdateFilePrint           (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_PRINT        ); }
void CFcHtmlEdView::OnUpdateEditClear           (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_DELETE       ); }
void CFcHtmlEdView::OnUpdateEditUndo            (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_UNDO         ); }
void CFcHtmlEdView::OnUpdateEditRedo            (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_REDO         ); }
void CFcHtmlEdView::OnUpdateEditCopy            (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_COPY         ); }
void CFcHtmlEdView::OnUpdateEditCut             (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_CUT          ); }
void CFcHtmlEdView::OnUpdateEditPaste           (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_PASTE        ); }
void CFcHtmlEdView::OnUpdateEditFind            (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_FIND         ); }
void CFcHtmlEdView::OnUpdateEditSelectAll       (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SELECTALL    ); }

void CFcHtmlEdView::OnUpdateInsertPicture       (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_IMAGE        ); }
void CFcHtmlEdView::OnUpdateInsertAnchor        (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_HYPERLINK    ); }
void CFcHtmlEdView::OnUpdateInsertHtml          (CCmdUI* pCmdUI) { pCmdUI->Enable(InsertHtml(CString(""))      ); }

void CFcHtmlEdView::OnUpdateTableInsertTable    (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_INSERTTABLE  ); }
void CFcHtmlEdView::OnUpdateTableInsertCell     (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_INSERTCELL   ); }
void CFcHtmlEdView::OnUpdateTableInsertCol      (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_INSERTCOL    ); }
void CFcHtmlEdView::OnUpdateTableInsertRow      (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_INSERTROW    ); }
void CFcHtmlEdView::OnUpdateTableMergeCells     (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_MERGECELLS   ); }
void CFcHtmlEdView::OnUpdateTableSplitCell      (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SPLITCELL    ); }
void CFcHtmlEdView::OnUpdateTableDeleteCells    (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_DELETECELLS  ); }
void CFcHtmlEdView::OnUpdateTableDeleteCols     (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_DELETECOLS   ); }
void CFcHtmlEdView::OnUpdateTableDeleteRows     (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_DELETEROWS   ); }
  
void CFcHtmlEdView::OnUpdateFormatStyle         (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_BLOCKFMT     ); }
void CFcHtmlEdView::OnUpdateFormatFont          (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_FONT         ); }
void CFcHtmlEdView::OnUpdateFormatDecreaseindent(CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_OUTDENT      ); }
void CFcHtmlEdView::OnUpdateFormatIncreaseindent(CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_INDENT       ); }
void CFcHtmlEdView::OnUpdateFormatAlignLeft     (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_JUSTIFYLEFT  ); }
void CFcHtmlEdView::OnUpdateFormatAlignCenter   (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_JUSTIFYCENTER); }
void CFcHtmlEdView::OnUpdateFormatAlignRight    (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_JUSTIFYRIGHT ); }
void CFcHtmlEdView::OnUpdateFormatBold          (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_BOLD         ); }
void CFcHtmlEdView::OnUpdateFormatItalic        (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_ITALIC       ); }
void CFcHtmlEdView::OnUpdateFormatUnderline     (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_UNDERLINE    ); }
void CFcHtmlEdView::OnUpdateFormatNumber        (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_ORDERLIST    ); }
void CFcHtmlEdView::OnUpdateFormatBullets       (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_UNORDERLIST  ); }
void CFcHtmlEdView::OnUpdateFormatColor         (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_FORECOLOR    ); }
void CFcHtmlEdView::OnUpdateFormatAbsolute      (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_MAKE_ABSOLUTE); }
void CFcHtmlEdView::OnUpdateFormatLock          (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_LOCK_ELEMENT ); }
void CFcHtmlEdView::OnUpdateFormatGrid          (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SET_ALIGNMENT); }

void CFcHtmlEdView::OnUpdateZorderToBack        (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SEND_TO_BACK   ); }
void CFcHtmlEdView::OnUpdateZorderToFront       (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SEND_TO_FRONT  ); }
void CFcHtmlEdView::OnUpdateZorderBackward      (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SEND_BACKWARD  ); }
void CFcHtmlEdView::OnUpdateZorderBehindtext    (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SEND_FORWARD   ); }
void CFcHtmlEdView::OnUpdateZorderForward       (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SEND_BEHIND_1D ); }
void CFcHtmlEdView::OnUpdateZorderFrontoftext   (CCmdUI* pCmdUI) { UpdateUIItem(pCmdUI, IDM_TRIED_SEND_FRONT_1D  ); }

void CFcHtmlEdView::OnViewOptions() 
{
	CString str1, str2;
	str1.LoadString(IDR_MAINFRAME);
	str2.LoadString(ID_OPTIONS);
	str1 += CString(" ") + str2;
	CPropertySheet sheet(str1);

	CFcPropertyPageGrid GridPage(this);
	sheet.AddPage(&GridPage);
	sheet.DoModal();
}

void CFcHtmlEdView::OnFormatGrid() 
{
	CFcHtmlEdOptions* pO = ((CFcHtmlEdApp*)AfxGetApp())->GetOptions();
	SetGrid(pO->GetGridHorizontal(), pO->GetGridVertical(), !pO->IsGridActive());
}

void CFcHtmlEdView::UpdateUIItem(CCmdUI* pCmdUI, DWORD nCmdID) 
{
	DWORD dwStatus;
	switch (nCmdID)
	{
	default :
		dwStatus = QueryTriEditCommand(nCmdID);
		pCmdUI->SetCheck((dwStatus & OLECMDF_LATCHED) == OLECMDF_LATCHED ? 1 : 0);
		pCmdUI->Enable  ((dwStatus & OLECMDF_ENABLED) == OLECMDF_ENABLED);		
		break;

	case IDM_TRIED_SET_ALIGNMENT:
	dwStatus = QueryTriEditCommand(IDM_TRIED_SET_ALIGNMENT);
	pCmdUI->Enable  ((dwStatus & OLECMDF_ENABLED) == OLECMDF_ENABLED);		
	pCmdUI->SetCheck(((CFcHtmlEdApp*)AfxGetApp())->GetOptions()->IsGridActive() ? 1 : 0);
		break;

	// The QueryStatus()-function of these 2 commands causes MSHTML 
	// to fire an OLECMDID_UPDATECOMMANDS event. This is probably a bug in MSHTML.
	// Because this function is called from the idle loop, the user interface elements 
	// are updated all the time. The result is that the Combo - boxes inside the toolbar 
	// not usable. To avoid this problem, the check for absolute positionning will only 
	// take place, when a moveable object is selected. In this case font selection is
	// is disabled.
	case IDM_TRIED_MAKE_ABSOLUTE:
	case IDM_TRIED_LOCK_ELEMENT :
		dwStatus = QueryTriEditCommand(IDM_TRIED_FONTNAME);
		if ((dwStatus & OLECMDF_ENABLED) != OLECMDF_ENABLED)
		{
			dwStatus = QueryTriEditCommand(nCmdID);
			pCmdUI->SetCheck((dwStatus & OLECMDF_LATCHED) == OLECMDF_LATCHED ? 1 : 0);
			pCmdUI->Enable  ((dwStatus & OLECMDF_ENABLED) == OLECMDF_ENABLED);		
		}
		else
		{
			pCmdUI->SetCheck(0);
			pCmdUI->Enable  (FALSE);		
		}
		break;
	}
}


BOOL CFcHtmlEdView::SetGrid(unsigned Horz, unsigned Vert, BOOL bActive) 
{
	// store grid settings in option object
	CFcHtmlEdOptions* pOptions =  ((CFcHtmlEdApp*)AfxGetApp())->GetOptions();
	pOptions->SetGridHorizontal(Horz);
	pOptions->SetGridVertical(Vert);
	pOptions->SetGridActive(bActive);

	POINT point;
	point.x = 0;
	point.y = 0;
	if (pOptions->IsGridActive())
	{
		point.x = pOptions->GetGridHorizontal();
		point.y = pOptions->GetGridVertical();
	}

	VARIANT va;
	VariantInit(&va);
	va.vt = VT_BYREF;
	V_BYREF(&va) = &point;

	return SUCCEEDED(ExecTriEditCommand(IDM_TRIED_SET_ALIGNMENT, &va, NULL, FALSE));		
}

BOOL CFcHtmlEdView::InsertHtml(LPCSTR pHtmlString)
{
    HRESULT hr = E_FAIL;
	IHTMLTxtRange* pRange = NULL;

	IHTMLDocument2* pDoc = NULL;
	IUnknown* lpUnk = m_pSite->GetObjectUnknown();
	if ( lpUnk != NULL )
	{
		hr = lpUnk->QueryInterface(IID_IHTMLDocument2, (void **)&pDoc);
		if (SUCCEEDED(hr))
		{
			IHTMLSelectionObject* pSel = NULL;
			pDoc->get_selection(&pSel);
			if (pSel)
			{
				IDispatch* pID = NULL;
				pSel->createRange(&pID);
				if (pID)
				{
					hr = pID->QueryInterface(IID_IHTMLTxtRange, (LPVOID *)&pRange);
					if (SUCCEEDED(hr))
					{
						if (pHtmlString != NULL && strlen(pHtmlString) > 0)
						{
							BSTR bstrContent = CString(pHtmlString).AllocSysString();
							pRange->pasteHTML(bstrContent);
							::SysFreeString(bstrContent);
						}
						pRange->Release();
					}
					pID->Release();
				}
				pSel->Release();
			}
			pDoc->Release();
		}
	}
	return SUCCEEDED(hr);
}


