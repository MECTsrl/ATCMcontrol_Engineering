


#include "stdafx.h"
#include "cemain.h"
#include "Printing.h"





/////////////////////////////////////////////////////////////////////////////
// Printing Dialog

static BOOL g_bUserAbort;

CPrjPrintDialog::CPrjPrintDialog(CWnd* pParent)

{
#ifdef _MAC
			// Note! set m_pView *before* CDialog::Create so that
			// CPrjPrintDialog::OnInitDialog can use it.
			m_pView = pParent;
#endif
			Create(CPrjPrintDialog::IDD, pParent);      // modeless !
			g_bUserAbort = FALSE;

			//SU _afxWinState->m_bUserAbort = FALSE;
}

#ifdef _MAC
BEGIN_MESSAGE_MAP(CPrjPrintDialog, CDialog)
	//{{AFX_MSG_MAP(CPrjPrintDialog)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif

BOOL CALLBACK PrjPrintAbortProc(HDC, int)
{
	//SU _AFX_WIN_STATE* pWinState = _afxWinState;

#ifdef _MAC
	EventRecord er;
	BOOL fNewMessages;

	// Look for an event, and if we find one, see if WLM cares about it.
	// If so, remove it from the queue (and then ignore it, since we've
	// already told WLM about it by calling QueueEvent). We also remove
	// the event from the queue if it's an activateEvt for a non-WLM
	// window, since this will probably be an activate for the printer
	// driver's status window, which we want to flush from the queue so
	// that it doesn't block events for WLM windows.

	while (EventAvail(everyEvent, &er) &&
		(QueueEvent(&er, &fNewMessages) || er.what == activateEvt))
	{
		GetNextEvent(everyEvent, &er);
	}

	// It's harder for us to depend on a cmd-. or escape keypress getting
	// picked up by the EventAvail, since the user could easily click in
	// the printer status window, producing a non-WLM event that would hide
	// the keypress. Therefore we also explicitly check for the keypress.

	if (GetAsyncKeyState(VK_CANCEL))
	{
		TRACE0("saw an async VK_CANCEL\n");
		g_bUserAbort = TRUE;
		//SU pWinState->m_bUserAbort = TRUE;
	}
#endif

	MSG msg;
	while (!g_bUserAbort &&
#ifndef _MAC
		::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
#else
		// don't grab any new messages from the Mac event queue
		::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE | PM_NOEVENTS))
#endif
	{
		if (!AfxGetThread()->PumpMessage())
			return FALSE;   // terminate if WM_QUIT received
	}
	return !g_bUserAbort;
	//SU return !pWinState->m_bUserAbort;
}

BOOL CPrjPrintDialog::OnInitDialog()
{
#ifdef _MAC
	// prime the state of the VK_CANCEL key
	GetAsyncKeyState(VK_CANCEL);
#endif
	SetWindowText(AfxGetAppName());
#ifndef _MAC
	CenterWindow();
#else
	CenterWindow(m_pView->GetParentFrame());
#endif
	return CDialog::OnInitDialog();
}

void CPrjPrintDialog::OnCancel()
{
	g_bUserAbort = TRUE;
	//SU _afxWinState->m_bUserAbort = TRUE;  // flag that user aborted print
	CDialog::OnCancel();
}

#ifdef _MAC
int CPrjPrintDialog::OnMouseActivate(CWnd* pDesktopWnd, UINT, UINT)
{
	UNUSED(pDesktopWnd); // not used in release build
	ASSERT(pDesktopWnd == this);

	// if the printer driver has opened its own status window, we don't
	// want to activate our printing status window when it's clicked

	return MA_NOACTIVATE;
}
#endif
