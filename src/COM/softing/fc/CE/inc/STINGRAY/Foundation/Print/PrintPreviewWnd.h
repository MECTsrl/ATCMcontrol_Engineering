/////////////////////////////////////////////////////////////////////////////
// PrintPreviewWnd.h : Print preview window classes.
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Print preview window classes.
//

#pragma once

#if defined(_SFL_ATL_SUPPORT)

#include <atlcom.h>

#include <Foundation\sflres.h>

#include <Foundation\Print\PrintPreviewModel.h>
#include <Foundation\Print\PrintPreviewViewport.h>
#include <Foundation\Print\PrintPreviewController.h>
#include <Foundation\Print\PrintJob.h>

#include <Foundation\Controls\AtlControls.h>
#include <Foundation\MVC\MvcClientViewport.h>

#include <Foundation\Events\EventRouter.h>
#include <Foundation\Events\EventRouterMap.h>
#include <Foundation\Events\WindowListener.h>
#include <Foundation\Layout\BorderClient.h>
#include <Foundation\MVC\MvcZoomCommand.h>
#include <Foundation\Debug.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc CPrtPreviewClientWnd

//@class CPrtPreviewClientWnd | This class implements a print preview
// client window.  It is a mixin class of CClientWindowImpl,
// CPrtPreviewScrollViewport, and CPrtPreviewModel.

class CPrtPreviewClientWnd : public CMvcClientViewport< CPrtPreviewClientWnd, CPrtPreviewScrollViewport >,
                             public CPrtPreviewModel
{
public:
	typedef CMvcClientViewport< CPrtPreviewClientWnd, CPrtPreviewScrollViewport > _BaseClass;

	CPrtPreviewClientWnd()
	{
		SetModel(this);
	}

	BEGIN_MSG_MAP(CPrtPreviewDlgBar)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		CHAIN_MSG_MAP(_BaseClass)
	END_MSG_MAP()

	BEGIN_GUID_MAP(CPrtPreviewClientWnd) 
		GUID_CHAIN_ENTRY(_BaseClass)
		GUID_CHAIN_ENTRY(CPrtPreviewModel)
	END_GUID_MAP

	virtual BOOL CreateController()
	{
		m_pCtlr = new CPrtPreviewController();

		if (m_pCtlr != NULL)
		{
			m_pCtlr->SetModel(this);
		}

		return TRUE;
	}

    LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// To get rid of warnings
		uMsg;
		wParam;
		lParam;
		bHandled;

		return 0L;
	}
};

//////////////////////////////////////////////////////////////////////////
//@doc CPrtPreviewDlgBar

//@class This class implements the print preview dialog control bar. It
// is a modeless dialog that contains controls for manipulating a
// print preview window. It is loosely coupled from the print preview
// window to allow different controls bars to be used easily. In other
// words, the print preview window has no direct coupling to this class
// so this class can be replaced without affecting the core print preview
// logic.  This class maintains an IQueryGuid pointer to the print
// preview window. That IQueryGuid pointer is used to get other
// interfaces for manipulating the print preview window. An example of
// a print preview window that is queried for is IZoom, which is used
// when the magnification value changes in the zoom combo box.

class CPrtPreviewDlgBar : public CDialogImpl<CPrtPreviewDlgBar>,
                          public IEventRouterImpl, 
                          public CEventRouterMap<CPrtPreviewDlgBar>
{
// Embedded types
public:
	enum { IDD = IDD_PRINTPREVIEW_BAR };
	typedef CMvcZoomCommand<10, IQueryGuid> _ZoomCommand;

// Constructors/destructor
public:
	CPrtPreviewDlgBar(IQueryGuid* pICmdRecvr = NULL) :
		m_pICmdRecvr(pICmdRecvr)
	{
	}

// Attributes
protected:
	ATLControls::CComboBox m_zoomCombo;
	IQueryGuid* m_pICmdRecvr;

// Operations
public:

	BEGIN_MSG_MAP(CPrtPreviewDlgBar)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CEventRouterMap<CPrtPreviewDlgBar>)
		COMMAND_HANDLER(IDC_SFL_PPV_ZOOM, CBN_EDITCHANGE, OnEditChangeZoomCombo)
		COMMAND_HANDLER(IDC_SFL_PPV_ZOOM, CBN_SELCHANGE, OnSelchangeZoomCombo)
	END_MSG_MAP()

	void SetCommandReceiver(IQueryGuid* pICmdRecvr)
	{
		m_pICmdRecvr = pICmdRecvr;
		InitZoomEdit();
	}

	void InitZoomEdit()
	{
		if (!IsWindow())
			return;

		IZoom* pIZoom = guid_cast<IZoom*>(m_pICmdRecvr);
		if (pIZoom != NULL)
		{
			CSize szZoom(pIZoom->GetMagnification());
			// only allow symmetric zoom factors
			ASSERT(szZoom.cx == szZoom.cy);

			HWND zoomWnd;
			zoomWnd = GetDlgItem(IDC_SFL_PPV_ZOOM);

			if (zoomWnd)
			{
				TCHAR convertNumToString[12];
				_itot(szZoom.cx,convertNumToString,10);
				
				int length = _tcslen(convertNumToString);
				convertNumToString[length] = _T('%');
				convertNumToString[length+1] = _T('\0');

				ATLControls::CEdit edit = zoomWnd;
				edit.SetWindowText(convertNumToString);
			}
		}
	}

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// To get rid of warnings
		bHandled;
		lParam;
		wParam;
		uMsg;

		HWND zoomWnd;
		zoomWnd = GetDlgItem(IDC_SFL_PPV_ZOOM);
		if (zoomWnd)
		{
			m_zoomCombo = zoomWnd;

			// insert your favorite magnification
			// allowable 0 <= PERCENT <= INT_MAX
			m_zoomCombo.AddString(_T("10%"));
			m_zoomCombo.AddString(_T("25%"));
			m_zoomCombo.AddString(_T("50%"));
			m_zoomCombo.AddString(_T("75%"));
			m_zoomCombo.AddString(_T("100%"));
			m_zoomCombo.AddString(_T("125%"));
			m_zoomCombo.AddString(_T("150%"));
			m_zoomCombo.AddString(_T("175%"));
			m_zoomCombo.AddString(_T("200%"));
		}

		InitZoomEdit();

		return FALSE;
	}

	LRESULT OnEditChangeZoomCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// To get rid of warnings...
		wNotifyCode;
		wID, 
		hWndCtl;
		bHandled;

		return 0L;
	}

	LRESULT OnSelchangeZoomCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// To get rid of warnings
		bHandled;
		hWndCtl;
		wID;
		wNotifyCode;


		TCHAR szMag[20];
		int nCurSel = m_zoomCombo.GetCurSel();
		m_zoomCombo.GetLBText(nCurSel, szMag);
		int nLen = _tcslen(szMag);
		szMag[nLen-1] = _T('\0');
		int nMag = _ttoi(szMag);

		_ZoomCommand* pZoomCmd = new _ZoomCommand(m_pICmdRecvr);
		ASSERT(0 != pZoomCmd);
		if(0 != pZoomCmd)
		{
			pZoomCmd->SetMagnification(nMag, nMag);
			pZoomCmd->Execute();
			delete pZoomCmd;
		}

		return 0L;
	}
};

/////////////////////////////////////////////////////////////////////////////
//@doc CPrintPreviewFrameImpl

//@class CPrintPreviewFrameImpl | This class provides an implementation of
// print preview for a frame window.  The base class is passed in as a
// template parameter and is assumed to be a frame window derived class.

//@mfunc bool | CPrintPreviewFrameImpl | DoPrintPreview | Show the print
// preview window inside the client area of the frame window.
//@rdesc true if successful, false if error occurred
//@parm IPrintable* | pIPrintable | Interface to object to be printed.
//@comm This function shows the print preview window in the client area
// of the frame and puts the print preview dialog control bar at the
// top of the client area of the frame. This function calls the
// OnBeginPreview() callback function to give derived classes an
// opportunity to inject some processing before the print print
// preview window is shown.

//@mfunc void | CPrintPreviewFrameImpl | EndPrintPreview | Close the
// print preview window and restore normal view inside frame.
//@rdesc none
//@comm Closes the print preview window and restores the previous
// state of the frame window. Calls the OnEndPreview() callback
// function to give derived classes an opportunity to cleanup
// and restore things after print preview has been closed.

//@mfunc void | CPrintPreviewFrameImpl | OnBeginPreview | Called by
// the framework just before the print preview window is displayed.
//@rdesc none
//@comm Derived classes override this method to close any windows
// and toolbars that are not applicable to print preview.

//@mfunc void | CPrintPreviewFrameImpl | OnEndPreview | Called by
// the framework just after the print preview window is closed.
//@rdesc none
//@comm Derived classes override this method to restore the state
// of the frame window after print preview is finished.

//@mfunc ILayoutNode* | CPrintPreviewFrameImpl | SetLayoutRoot | Set the root layout node.
//@rdesc Previous root layout node.
//@parm ILayoutNode* | pNewRootNode | Pointer to new root layout node.
//@comm This function replaces the current layout root node
// with a new layout root node.

template <class _Base, UINT _PrintID = IDC_SFL_FILE_PRINT, UINT _PrintPreviewID = IDC_SFL_FILE_PRINT_PREVIEW>
class CPrintPreviewFrameImpl : public _Base
{
// Embedded types
public:
	typedef CPrintPreviewFrameImpl<_Base> _ThisClass;
	typedef _Base _BaseClass;

protected:
	/////////////////////////////////////////////////////////////////////////
	// CControlBarListener - This nested class listens to the print preview
	// control bar for commands.

    class CControlBarListener : public CCommandAdapter
	{
	public:
		CControlBarListener(_ThisClass* pFrame) : m_pFrame(pFrame)
		{
		}

		virtual bool OnCommand(UINT nID, int nNotifyCode)
		{
			if (nID == IDC_SFL_PPV_PRINT)
			{
				BOOL bHandled;
				m_pFrame->OnFilePrint((WORD) nNotifyCode, 
										(WORD) nID, 
										0, bHandled);
				return true;
			}
			else if (nID == IDC_SFL_PPV_CLOSE)
			{
				m_pFrame->PostMessage(WM_CLOSE_PRINT_PREVIEW, 0, 0);
			}
			return false;
		}

		BEGIN_GUID_MAP(CControlBarListener)
			GUID_CHAIN_ENTRY(CCommandAdapter)
		END_GUID_MAP

		_ThisClass* m_pFrame;
	};

// Constructors/destructor
public:
	//@cmember
	/* Default constructor for print preview frame window. */
	CPrintPreviewFrameImpl() :
		m_pPreviewView(NULL),
		m_pPreviewDlgBar(NULL),
		m_pPreviewLayout(NULL),
		m_pPrevLayoutRoot(NULL),
		m_previewListener(this),
		m_hPrevFocusWnd(NULL)
	{
	}

	//@cmember
	/* Free memory used by print preview. */
	virtual ~CPrintPreviewFrameImpl()
	{
#if 0
		// Destroy the print preview layout nodes
		DestroyPreviewLayout();
		// Destroy print preview client window
		DestroyPreviewView();
		// Destroy print preview dialog bar
		DestroyPreviewDialogBar();
#endif
	}

// Attributes
protected:
	//@cmember
	/* Print document object which acts as the output target. */
	CPrintDoc m_printDoc;
	//@cmember
	/* Client window that displays the print preview view. */
	CPrtPreviewClientWnd* m_pPreviewView;
	//@cmember
	/* Modeless dialog used to send commands to the print preview window. */
	CPrtPreviewDlgBar* m_pPreviewDlgBar;
	//@cmember
	/* Root layout node used during print preview. */
	IBorderClientLayout* m_pPreviewLayout;
	//@cmember
	/* Used to restore layout manager to previous state when print preview is done. */
	ILayoutNode* m_pPrevLayoutRoot;
	//@cmember
	/* Listens to the dialog bar for certain events. */
	CControlBarListener m_previewListener;
	//@cmember
	/* Handle of window that had input focus before print preview window shown. */
	HWND m_hPrevFocusWnd;

// Operations
public:
	//@cmember
	/* Returns a pointer to object to be printed. */
	virtual IPrintable* GetCurrentPrintable() = 0;

	//@cmember
	/* Show the print preview window inside the client area of the frame window. */
	virtual bool BeginPrintPreview(IPrintable* pIPrintable)
	{
		bool bSuccess = false;

		CPrinterConfig* pPrinterConfig = _Module.GetPrinterConfig();
		if ((pPrinterConfig != NULL && pPrinterConfig->PrinterExists()) && pIPrintable != NULL)
		{
			m_printDoc.SetPrinterConfig(_Module.GetPrinterConfig());

			OnBeginPreview();

			// Create print preview view
			ASSERT(CreatePreviewView());
			m_pPreviewView->ShowWindow(SW_SHOWNORMAL);

			// Create the print preview dialog bar
			ASSERT(CreatePreviewDialogBar());
			m_pPreviewDlgBar->ShowWindow(SW_SHOWNORMAL);

			ASSERT(CreatePreviewLayout());

			// Change the layout root node to use the print preview layout
			m_pPrevLayoutRoot = SetLayoutRoot(m_pPreviewLayout);

			// Execute the print job
			m_hPrevFocusWnd = m_pPreviewView->SetFocus();
			bSuccess = m_pPreviewView->Start(pIPrintable, &m_printDoc);
		}

		return bSuccess;
	}

	//@cmember
	/* Close the print preview window and restore normal view inside frame. */
	virtual void EndPrintPreview()
	{
		// Restore previous root layout node
		if (m_pPrevLayoutRoot != NULL)
		{
			SetLayoutRoot(m_pPrevLayoutRoot);
			m_pPrevLayoutRoot = NULL;
		}

		// Destroy the print preview layout nodes
		DestroyPreviewLayout();
		// Destroy print preview dialog bar
		DestroyPreviewDialogBar();
		// Destroy print preview client window
		DestroyPreviewView();

		// Restore previous focus window
		if (m_hPrevFocusWnd != NULL)
		{
			::SetFocus(m_hPrevFocusWnd);
			m_hPrevFocusWnd = NULL;
		}

		// Callback for derived classes
		OnEndPreview();
	}

	//@cmember
	/* Called by the framework just before the print preview window is displayed. */
	virtual void OnBeginPreview()
	{
	}

	//@cmember
	/* Called by the framework just after the print preview window is closed. */
	virtual void OnEndPreview()
	{
	}

// Message handlers
public:
	BEGIN_MSG_MAP(_ThisClass)
		COMMAND_ID_HANDLER(_PrintID, OnFilePrint)
		COMMAND_ID_HANDLER(_PrintPreviewID, OnFilePrintPreview)
		MESSAGE_HANDLER(WM_CLOSE_PRINT_PREVIEW, OnPrintPreviewClose)
		CHAIN_MSG_MAP(_BaseClass)
	END_MSG_MAP()

	LRESULT OnFilePrint(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// To get rid of the warnings...
		wNotifyCode;
		wID;
		hWndCtl;
		bHandled;

		CPrintJob printJob;
		CPrintDoc printDoc;
		printDoc.SetPrinterConfig(_Module.GetPrinterConfig());
		printJob.Start(GetCurrentPrintable(), &printDoc);
		return 0L;
	}

	LRESULT OnFilePrintPreview(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// To get rid of the warnings...
		wNotifyCode;
		wID;
		hWndCtl;
		bHandled;

		BeginPrintPreview(GetCurrentPrintable());
		return 0L;
	}

	LRESULT OnPrintPreviewClose(UINT, WPARAM, LPARAM, BOOL&)
	{
		EndPrintPreview();
		return 0L;
	}

// Implementation
private:
	//@cmember
	/* Set the root layout node. */
	ILayoutNode* SetLayoutRoot(ILayoutNode* pNewRootNode)
	{
		ILayoutNode* pPrevRootNode = m_pLayoutRoot;
		m_pLayoutRoot = pNewRootNode;

		CRect rcInitial;
		GetClientRect(rcInitial);

		if (m_pLayoutRoot)
		{
			m_pLayoutRoot->SetInitialRect(rcInitial);
			m_pLayoutRoot->Init(GetHWND());

			// Recursively recalculate the appropriate Z-Order (necessary to 
			// avoid stepping on child control tab order, while still 
			// allowing nested splitters to coeexist)
			m_pLayoutRoot->RecalcZOrder();

			if (m_pLayoutRoot->GetChildrenCount() == 0)
			{
				AutoPopulate();
			}

			// Initial recalculation
			RecalcLayout(rcInitial, true);
		}

		return pPrevRootNode;
	}

	//@cmember
	/* Create the print preview view. */
	bool CreatePreviewView()
	{
		if (m_pPreviewView == NULL)
		{
			CRect rcClient;
			GetClientRect(rcClient);

			m_pPreviewView = new CPrtPreviewClientWnd();
			if (m_pPreviewView->Create(m_hWnd, rcClient))
			{
				return true;
			}
		}
		return false;
	}

	//@cmember
	/* Destroy the print preview window. */
	void DestroyPreviewView()
	{
		if (m_pPreviewView != NULL)
		{
			if (m_pPreviewView->IsWindow())
			{
				m_pPreviewView->DestroyWindow();
			}
			delete m_pPreviewView;
			m_pPreviewView = NULL;
		}
	}

	//@cmember
	/* Create the print preview dialog bar. */
	bool CreatePreviewDialogBar()
	{
		if (m_pPreviewDlgBar == NULL)
		{
			m_pPreviewDlgBar = new CPrtPreviewDlgBar();

			if (m_pPreviewDlgBar->Create(m_hWnd))
			{
				m_pPreviewDlgBar->SetCommandReceiver(guid_cast<IQueryGuid*>(m_pPreviewView));
				m_pPreviewDlgBar->AddListener(guid_cast<IEventListener*>(m_pPreviewView));
				m_pPreviewDlgBar->AddListener(static_cast<CCommandAdapter*>(&m_previewListener));
				return true;
			}
			else
			{
				delete m_pPreviewDlgBar;
				m_pPreviewDlgBar = NULL;
			}
		}

		return false;
	}

	//@cmember
	/* Destroy the print preview dialog bar. */
	void DestroyPreviewDialogBar()
	{
		if (m_pPreviewDlgBar != NULL)
		{
			if (m_pPreviewDlgBar->IsWindow())
			{
 				m_pPreviewDlgBar->DestroyWindow();
				m_pPreviewDlgBar->m_hWnd = NULL;
			}
			delete m_pPreviewDlgBar;
			m_pPreviewDlgBar = NULL;
		}
	}

	//@cmember
	/* Create the print preview layout nodes. */
	bool CreatePreviewLayout()
	{
		bool bSuccess = false;

		if (m_pPreviewLayout == NULL &&
		    m_pPreviewView != NULL && m_pPreviewView->IsWindow() &&
		    m_pPreviewDlgBar != NULL && m_pPreviewDlgBar->IsWindow())
		{
			// Create a border client layout to use for root of print preview layout tree
			ILayoutNode* pPreviewRoot = CreateLayoutNode(__uuidof(foundation::CBorderClientLayout));
			m_pPreviewLayout = guid_cast<foundation::IBorderClientLayout*>(pPreviewRoot);

			// Create a window layout node for the print preview window
			ILayoutNode* pClientNode = CreateLayoutNode(__uuidof(foundation::CWindowLayoutNode));
			pClientNode->Init(m_hWnd, *m_pPreviewView);
			m_pPreviewLayout->AddLayoutNode(pClientNode, foundation::IBorderClientLayout::BorderPosition::Client);

			// Create a window layout node for the dialog bar
			ILayoutNode* pDlgBarNode = CreateLayoutNode(__uuidof(foundation::CWindowLayoutNode));
			pDlgBarNode->Init(m_hWnd, *m_pPreviewDlgBar);
			m_pPreviewLayout->AddLayoutNode(pDlgBarNode, foundation::IBorderClientLayout::BorderPosition::Top);

			bSuccess = true;
		}

		return bSuccess;
	}
 
	//@cmember
	/* Free memory used by print preview for layout management. */
	void DestroyPreviewLayout()
	{
		if (m_pPreviewLayout != NULL)
		{
			DestroyLayoutNode(m_pPreviewLayout);
			m_pPreviewLayout = NULL;
		}
	}
};

}; // namespace stingray::foundation
}; // namespace stingray

#endif // #if defined(_SFL_ATL_SUPPORT)
