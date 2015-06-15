// SECEditCtrlOleDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "SECEditCtrlOleDropTarget.h"
#include "SECEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SECEditCtrlOleDropTarget

SECEditCtrlOleDropTarget::SECEditCtrlOleDropTarget()
{
}

SECEditCtrlOleDropTarget::~SECEditCtrlOleDropTarget()
{
}


BEGIN_MESSAGE_MAP(SECEditCtrlOleDropTarget, SECEditCtrlOleDropTarget)
	//{{AFX_MSG_MAP(SECEditCtrlOleDropTarget)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SECEditCtrlOleDropTarget message handlers
DROPEFFECT SECEditCtrlOleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
	DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

if (!pWnd->IsKindOf(RUNTIME_CLASS(SECEditCtrl)))
		return DROPEFFECT_NONE;

	SECEditCtrl* pSECEditCtrl = (SECEditCtrl*)pWnd;
	return pSECEditCtrl->OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT SECEditCtrlOleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
	DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(SECEditCtrl)))
		return DROPEFFECT_NONE;

	SECEditCtrl* pSECEditCtrl = (SECEditCtrl*)pWnd;
	return pSECEditCtrl->OnDragOver(pDataObject, dwKeyState, point);
}

BOOL SECEditCtrlOleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
	DROPEFFECT dropEffect, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(SECEditCtrl)))
		return DROPEFFECT_NONE;

	SECEditCtrl* pSECEditCtrl = (SECEditCtrl*)pWnd;
	return pSECEditCtrl->OnDrop(pDataObject, dropEffect, point);
}

void SECEditCtrlOleDropTarget::OnDragLeave(CWnd* pWnd)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(SECEditCtrl)))
		return;

	SECEditCtrl* pSECEditCtrl = (SECEditCtrl*)pWnd;
	pSECEditCtrl->OnDragLeave();
	return;
}

// Update this later. For now, bail out (via the base class)
DROPEFFECT SECEditCtrlOleDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
	DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
	return COleDropTarget::OnDropEx(pWnd, pDataObject, dropEffect, dropEffectList, point);
}

// Update this later. For now, bail out (via the base class)
DROPEFFECT SECEditCtrlOleDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState,
	CPoint point)
{
	//return COleDropTarget::OnDragScroll(pWnd, dwKeyState, point);
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(SECEditCtrl)))
		return DROPEFFECT_NONE;

	SECEditCtrl* pSECEditCtrl = (SECEditCtrl*)pWnd;

	DROPEFFECT dropEffect = 0;

	// get client rectangle of destination window
	CRect rectClient;
	pWnd->GetClientRect(&rectClient);
	CRect rect = rectClient;

	// hit-test against inset region
	UINT nTimerID = MAKEWORD(-1, -1);
	rect.InflateRect(-nScrollInset, -nScrollInset);
	if (rectClient.PtInRect(point) && !rect.PtInRect(point))
	{
		// determine which way to scroll along both X & Y axis
		if (point.x < rect.left)
			nTimerID = MAKEWORD(SB_LINEUP, HIBYTE(nTimerID));
		else if (point.x >= rect.right)
			nTimerID = MAKEWORD(SB_LINEDOWN, HIBYTE(nTimerID));
		if (point.y < rect.top)
			nTimerID = MAKEWORD(LOBYTE(nTimerID), SB_LINEUP);
		else if (point.y >= rect.bottom)
			nTimerID = MAKEWORD(LOBYTE(nTimerID), SB_LINEDOWN);
		ASSERT(nTimerID != MAKEWORD(-1, -1));

		BOOL bEnableScroll = FALSE;
		bEnableScroll = pSECEditCtrl->OnScroll(nTimerID, 0, FALSE);

		if (!bEnableScroll)
			nTimerID = MAKEWORD(-1, -1);
	}

	if (nTimerID == MAKEWORD(-1, -1))
	{
		if (m_nTimerID != MAKEWORD(-1, -1))
		{
			// send fake OnDragEnter when transition from scroll->normal
			COleDataObject dataObject;
			dataObject.Attach(m_lpDataObject, FALSE);
			OnDragEnter(pWnd, &dataObject, dwKeyState, point);
			m_nTimerID = MAKEWORD(-1, -1);
		}
		return DROPEFFECT_NONE;
	}

	// save tick count when timer ID changes
	DWORD dwTick = GetTickCount();
	if (nTimerID != m_nTimerID)
	{
		m_dwLastTick = dwTick;
		m_nScrollDelay = nScrollDelay;
	}

	// scroll if necessary
	if (dwTick - m_dwLastTick > m_nScrollDelay)
	{
		pSECEditCtrl->OnScroll(nTimerID, 0, TRUE);
		m_dwLastTick = dwTick;
		m_nScrollDelay = nScrollInterval;
	}
	if (m_nTimerID == MAKEWORD(-1, -1))
	{
		// send fake OnDragLeave when transitioning from normal->scroll
		OnDragLeave(pWnd);
	}

	m_nTimerID = nTimerID;
	// check for force link
	if ((dwKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_LINK;
	// check for force copy
	else if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_COPY;
	// check for force move
	else if ((dwKeyState & MK_ALT) == MK_ALT ||
		(dwKeyState & MK_SHIFT) == MK_SHIFT)
		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_MOVE;
	// default -- recommended action is move
	else
		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_MOVE;
	return dropEffect;
}
