
#include "stdafx.h"
#include "fcdatatip.h"


BEGIN_MESSAGE_MAP(CFCDataTip, CToolTipCtrl)
	ON_NOTIFY_REFLECT(TTN_NEEDTEXT, OnNeedText)
	ON_NOTIFY_REFLECT(TTN_POP, OnPop)
END_MESSAGE_MAP()


CFCDataTip::CFCDataTip ()
: m_pPopupCB (NULL),
  m_pActionQueryCB (NULL),
  m_bCreated (false),
  m_bActive (false),
  m_bInCallback (false),
  m_pTool (NULL),
  m_PopupState (Sleeping),
  m_bDelayedDelTool (false)
{
}


CFCDataTip::~CFCDataTip ()
{
	Destroy ();
}


void CFCDataTip::SetPopupCB (FC_CALLBACK_BASE (FCDataTipPopupCB) *pPopupCB)
{
	m_pPopupCB = pPopupCB;
}


void CFCDataTip::SetActionQueryCB (FC_CALLBACK_BASE (FCDataTipActionQueryCB) *pActionQueryCB)
{
	m_pActionQueryCB = pActionQueryCB;
}


HRESULT CFCDataTip::Create (CWnd *pParent, DWORD dwStyle, CWnd *pTool)
{
	if (m_bCreated)
		return (S_FALSE);

	m_pTool = pTool;

	if (!CToolTipCtrl::Create (pParent, dwStyle))
		return (E_FAIL);

	m_bCreated = true;

	SendMessage (TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	SendMessage (TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
	SendMessage (TTM_SETDELAYTIME, TTDT_INITIAL, 200);
	SendMessage (TTM_SETDELAYTIME, TTDT_RESHOW, 200);

	return (S_OK);
}


HRESULT CFCDataTip::Destroy ()
{
	if (!m_bCreated)
		return (S_FALSE);

	m_bCreated = false;

	if (!DestroyWindow ())
		return (E_FAIL);

	return (S_OK);
}


bool CFCDataTip::IsCreated ()
{
	return (m_bCreated);
}


HRESULT CFCDataTip::Activate ()
{
	if (!m_bCreated)
		return (E_FAIL);

	if (m_bActive)
		return (S_FALSE);

	CToolTipCtrl::Activate (TRUE);
	m_bActive = true;

	AddTool (m_pTool);
	m_PopupState = Triggered;

	return (S_OK);
}


HRESULT CFCDataTip::Deactivate ()
{
	if (!m_bCreated)
		return (E_FAIL);

	if (!m_bActive)
		return (S_FALSE);

	if (m_PopupState != Sleeping || m_bDelayedDelTool)
	{
		if (m_pTool != NULL)
			DelTool (m_pTool);

		m_bDelayedDelTool = false;
	};

	m_PopupState = Sleeping;

	CToolTipCtrl::Activate (FALSE);
	m_bActive = false;

	return (S_OK);
}


bool CFCDataTip::IsActive ()
{
	return (m_bActive);
}


HRESULT CFCDataTip::SetText (const CString &strText)
{
	m_strText = strText;

	if (!m_bCreated || !m_bActive || m_PopupState == Sleeping)
		return (S_FALSE);

	if (m_pTool == NULL)
		return (E_UNEXPECTED);

	if (m_bInCallback)
		return (S_OK);

	PropagateText ();

	return (S_OK);
}


void CFCDataTip::PropagateText ()
{
	if (m_strText.IsEmpty ())
		UpdateTipText (_T (" "), m_pTool);
	else
		UpdateTipText (m_strText, m_pTool);
}


void CFCDataTip::RelayMessage (LPMSG pMessage)
{
	FCDataTipAction action;

	if (!m_bCreated || !m_bActive)
		return;

	if (pMessage->message == WM_MOUSEMOVE && m_pActionQueryCB != NULL)
	{
		POINTS points;

		points = MAKEPOINTS (pMessage->lParam);

		m_bInCallback = true;
		m_pActionQueryCB->Execute (CPoint (points.x, points.y), action);
		m_bInCallback = false;

		switch (action)
		{
		case FCHideDataTip: Hide (); break;
		case FCMoveDataTip: Move (); break;
		case FCShowDataTip: Show (); break;
		};
	};

	CToolTipCtrl::RelayEvent (pMessage);
}


HRESULT CFCDataTip::Hide ()
{
	if (m_PopupState == Sleeping)
	{
		if (m_bDelayedDelTool)
		{
			DelTool (m_pTool);
			m_bDelayedDelTool = false;
		};

		return (S_FALSE);
	};

	m_PopupState = Sleeping;

	if (m_pTool == NULL)
		return (E_UNEXPECTED);

	UpdateTipText (_T(""), m_pTool);

	DelTool (m_pTool);

	return (S_OK);
}


HRESULT CFCDataTip::Show ()
{
	HRESULT hr;

	hr = Hide ();
	if (FAILED (hr))
		return (hr);

	AddTool (m_pTool);

	m_PopupState = Triggered;

	SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);

	return (S_OK);
}


HRESULT CFCDataTip::Move ()
{
	if (m_PopupState != PoppedUp)
		return (S_FALSE);

	PropagateText ();

	return (S_OK);
}


CFCDataTip::PopupState CFCDataTip::GetPopupState ()
{
	return (m_PopupState);
}


bool CFCDataTip::IsSleeping ()
{
	return (m_PopupState == Sleeping);
}


bool CFCDataTip::IsTriggered ()
{
	return (m_PopupState != Sleeping);
}


bool CFCDataTip::IsPoppedUp ()
{
	return (m_PopupState == PoppedUp);
}


void CFCDataTip::OnNeedText (NMHDR *pNMHDR, LRESULT *pResult)
{
	TOOLTIPTEXT *pTTT=(TOOLTIPTEXT *)pNMHDR;    

	*pResult = 0;

	if (pTTT == NULL)
		return;

	pTTT->hinst = NULL;
	pTTT->szText[0] = _T ('\0');
	pTTT->lpszText = pTTT->szText;

	if (!(pTTT->uFlags & TTF_IDISHWND))
		return;

	if (m_PopupState == Sleeping)
		return;

	if (m_pPopupCB != NULL)
	{
		bool bAllow=true;

		m_bInCallback = true;
		m_pPopupCB->Execute (bAllow);
		m_bInCallback = false;

		if (!bAllow)
		{
			if (m_pTool != NULL)
				UpdateTipText (_T(""), m_pTool);

			return;
		};
	};

	if (m_strText.IsEmpty ())
		pTTT->lpszText = (LPTSTR)_T (" ");
	else
		pTTT->lpszText = (LPTSTR)(LPCTSTR)m_strText;

	m_PopupState = PoppedUp;
}


void CFCDataTip::OnPop (NMHDR *pNMHDR, LRESULT *pResult)
{
	m_PopupState = Sleeping;
	m_bDelayedDelTool = true;
}
