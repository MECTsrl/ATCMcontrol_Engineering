////////////////////////////////////////////////////////////////////////////////
// BarViewport.h: interface for the SECBarViewport class.
//
// Stingray Software Extension Classes
// Copyright © 1999 Stingray Software Inc,
// All rights reserved.
//
// Author:      Prakash Surendra
// Description:  ShortcutBar - MVCViewport implementation
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __TABBAR_VPORT_H__
#define __TABBAR_VPORT_H__

#include "Foundation\StructuredGraphics\VisualWindow.h"
#include "Foundation\mvc\mvcviewport.h"
#include "TabBarmodel.h"
#include "TabBarcontroller.h"

namespace stingray {

using namespace foundation;

// TLS index used by the viewport instance for caching a reference to itself
DWORD __declspec(selectany) g_TlsSlot = 0;

#define ID_BARVPTIMER		0x1001

class CHitTest
{
public:
	enum { htItem = 0, htClient, htOther };
	enum { WMLBtnDown = 0, WMLBtnUp, WMMouseMove, WMRBtnDown, WMRBtnUp };
	virtual void HitTest(UINT nFlags, CPoint pt, UINT& nHitTestCode, int& nHitIndex) = 0;
};

static const UINT nButtonHt	= 23;
static const UINT TEXT_X_OFFSET = 4;

// Forward declarations
class SECTabBarController;

class SECBarViewport : public CMvcViewport<Visual2D, SECTabBarPresModel, SECTabBarController>, public CHitTest
{
public:
	SECBarViewport()
	{
		m_pHost = NULL;
		m_nHitIndex = -1;
		m_bPressed = FALSE;
		m_bHilight = FALSE;
		m_nCurrActv = -1;
		m_nAnimationInt = 7;
		m_nAnimationStep = 10;
		m_nTimerID = 0;
		m_bInAnimate = FALSE;

		m_hWndEdit = NULL;
		m_nEditIndex = -1;
	}

	virtual ~SECBarViewport()
	{
		if(g_TlsSlot)
		{
			TlsFree(g_TlsSlot);
			g_TlsSlot=0;
		}
	}

	virtual CSize SetSize(int cx, int cy)
	{
		CSize szRet = Visual2D::SetSize(cx, cy);
		if(GetModel())
		{
			UpdateClientRect();
			::InvalidateRect(m_pHost->GetWindowHandle(), LPCRECT(GetBounds()), TRUE);
			UpdateActiveClient();
		}
		return szRet; 
	}

	virtual CPoint SetOrigin(int x, int y)
	{
		CPoint ptRet = Visual2D::SetOrigin(x,y);
		if(GetModel())
		{
			UpdateClientRect();
			::InvalidateRect(m_pHost->GetWindowHandle(), LPCRECT(GetBounds()), TRUE);
			UpdateActiveClient();
		}
		return ptRet;
	}

	virtual CPoint MoveOrigin(int xOff,int yOff)
	{
		CPoint ptRet = Visual2D::MoveOrigin(xOff, yOff); 
		if(GetModel())
		{
			UpdateClientRect();
			::InvalidateRect(m_pHost->GetWindowHandle(), LPCRECT(GetBounds()), TRUE);
			UpdateActiveClient();
		}
		return ptRet;
	}

	virtual void Draw(CGraphicsContext* pCGC)
	{
		if((m_pModel->GetTabBarCount() <= 0) || (m_pModel->GetActiveIndex() < 0))
		return;

		// Get DC from the IVisualWindow implementation
		CGraphicsContext CGC;
		if(pCGC == NULL)
		{
			CGC.Attach(GetDC(m_pHost->GetWindowHandle()));	
			pCGC = &CGC;
		}

		CRect rcClip;
		::GetUpdateRect(m_pHost->GetWindowHandle(), &rcClip, TRUE);
		// If the update rect is empty, then it is very likely that ::BeginPaint() was called earlier from within
		// the hosting window validating the update rect. We still need to paint the shortcut bar, so do not select
		// a clipping region for the dc.
		if(!rcClip.IsRectEmpty())
		{
			HRGN hRgnClip = CreateRectRgnIndirect(&rcClip);
			::SelectClipRgn(pCGC->GetSafeHdc(), hRgnClip);
			DeleteObject(hRgnClip);
		}

		if(!m_bInAnimate)
			DrawBars(pCGC);

		// If an active MvcVisualComponent exists, then invoke Draw_() on this component
		int nActive = m_pModel->GetActiveIndex();
		SECTabBarBase* pActiveBar = m_pModel->GetTabBarAt(nActive);
		if(pActiveBar->IsVisualComp())
		{
			IVisual* pVisual = NULL;
			pActiveBar->QueryGuid(__uuidof(IVisual), (void**)&pVisual);
			assert(pVisual);
#ifdef _SFL_MFC_SUPPORT
			pVisual->Draw(CDC::FromHandle(pCGC->GetSafeHdc()));
#else	//_SFL_ATL_SUPPORT
			pVisual->Draw((CDC*)pCGC);
#endif	//_SFL_MFC_SUPPORT
		}		

		if(CGC.GetSafeHdc())
			::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
	}

	virtual void EraseBkgndVP(CGraphicsContext* pCGC)
	{
		// Exclude the Shortcutbar's client rect from the dc's update area and allow 
		// the default erasebkgnd processing to take place
		if( m_pModel->GetTabBarCount() > 0 )
		{
			/*CGDIBrush bkBrush;
			bkBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
			pCGC->FillRect( rcClip, &bkBrush);
			bkBrush.DeleteObject();*/

			CRect rcBounds = GetBounds();
			pCGC->ExcludeClipRect(&rcBounds);
		}
	}

	virtual void OnUpdate(ISubject* pSubject, IMessage* pMsg)
	{
		pSubject;
		pMsg;

		// The model has changed, so update the client rect
		if(!pMsg)
			return;
		
		// Ideally, we have to call IMessage::GetTypeID() and perform the static cast only if the type is the same.
		// However, we can get away without this check as SECTabBarPresModel, at present, sends only 1 type of message.
		SECTabBarMessage* pTabBarMsg = static_cast<SECTabBarMessage*> (pMsg);
		switch(pTabBarMsg->m_nTabBarVerb)
		{
		case SECTabBarMessage::TABBAR_ADD:
			break;

		case SECTabBarMessage::TABBAR_INSERT:
			break;

		case SECTabBarMessage::TABBAR_REMOVE:
			RemoveBar( GetModel()->GetTabBarIndex(pTabBarMsg->m_pTabBar), pTabBarMsg->m_dwExData);
			break;
		
		case SECTabBarMessage::TABBAR_ACTIVATE:		
			ActivateBar( GetModel()->GetTabBarIndex(pTabBarMsg->m_pTabBar), pTabBarMsg->m_dwExData );
			break;

		case SECTabBarMessage::TABBAR_UPDATE:
		{	
			m_nCurrActv = m_pModel->GetActiveIndex();
			UpdateClientRect();
			if(m_nCurrActv >= 0)
				UpdateActiveClient();		
			::InvalidateRect(m_pHost->GetWindowHandle(), LPCRECT(GetBounds()), TRUE);
		}

		default:
			break;
		}
	}

	virtual void HitTest(UINT nFlags, CPoint pt, UINT& nHitTestCode, int& nHitIndex)
	{
		CRect rcBounds = GetBounds();
		if(!rcBounds.PtInRect(pt))
		{
			nHitTestCode = CHitTest::htOther;
			return;
		}
		CGraphicsContext CGC;
		CGC.Attach(GetDC(m_pHost->GetWindowHandle()));

		nHitIndex = -1;

		// If bar is currently pressed or hilighted, then restore state if the cursor is no longer in the bar rect
		if(m_bPressed || m_bHilight)
		{
			CRect rcBar;
			GetBarRect(m_nHitIndex, rcBar);
			if(m_bPressed && nFlags == CHitTest::WMLBtnUp)
			{
				m_bPressed = FALSE;
				SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(m_nHitIndex);
				DrawButton(&CGC, pTabBar, &rcBar);
				DrawIcon(&CGC, pTabBar, &rcBar);		
				DrawText(&CGC, pTabBar, &rcBar);
			}
			else
			if( nFlags == CHitTest::WMMouseMove && !rcBar.PtInRect(pt) )
			{
				m_bPressed = FALSE;
				m_bHilight = FALSE;

				SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(m_nHitIndex);
				DrawButton(&CGC, pTabBar, &rcBar);
				DrawIcon(&CGC, pTabBar, &rcBar);		
				DrawText(&CGC, pTabBar, &rcBar);
				m_nHitIndex = -1;
			}			
		}

		// If point lies within the viewport's client area then return the appropriate code
		if(m_rcClient.PtInRect(pt))
		{
			nHitTestCode = CHitTest::htClient;
			nHitIndex = m_pModel->GetActiveIndex();
			::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
			return;
		}
		// Ok a bar has been hit. Which one?
		int nCount = m_pModel->GetTabBarCount();
		for(int i=0; i<nCount; i++)
		{
			CRect rcBar;
			GetBarRect(i, rcBar);
			if(rcBar.PtInRect(pt))
			{
				if(!m_bHilight)
				{
					m_nHitIndex = i;
					if(m_pModel->GetTabBarStyle() & SEC_TABBAR_BARCURSOR)
						::SetCursor(m_pModel->GetTabBarCursor());
					if(!m_nTimerID)
						m_nTimerID = ::SetTimer(m_pHost->GetWindowHandle(), ID_BARVPTIMER, 100, NULL);
				}

				// If the bar is disabled, then just set the cursor and return without any UI/Controller-hitIndex feedback
				if(!m_pModel->IsTabBarEnabled(i))
					break;

				nHitTestCode = CHitTest::htItem;
				nHitIndex = i;

				// Provide the UI feedback
				switch(nFlags)
				{
				case 0:		// CHitTest::WMLBtnDown
				{
					m_bPressed = TRUE;
					SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(m_nHitIndex);
					DrawButton(&CGC, pTabBar, &rcBar, TRUE);
					DrawIcon(&CGC, pTabBar, &rcBar, TRUE);		
					DrawText(&CGC, pTabBar, &rcBar, TRUE);
					break;
				}
				case 2:		// CHitTest::WMMouseMove
					if(!m_bHilight)
					{
						if(m_pModel->GetTabBarStyle() & SEC_TABBAR_NOHILIGHT)
							break;
						m_bHilight = TRUE;
						CRect rcTemp = rcBar; 
						rcTemp.bottom -= 1;
						SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(m_nHitIndex);
						DrawButton(&CGC, pTabBar, &rcBar, FALSE);
						DrawIcon(&CGC, pTabBar, &rcBar, FALSE);
						DrawText(&CGC, pTabBar, &rcBar, FALSE);
						::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
					}
					break;
				default:
					break;
				}	
				::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
				return;
			}
		}	

		::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
		nHitTestCode = CHitTest::htOther;
	}

	void OnViewportTimer(UINT nIDEvent)
	{
		if(nIDEvent != m_nTimerID)
		return;

		if( m_nHitIndex != -1)
		{
			CPoint ptCur;
			::GetCursorPos(&ptCur);
			::ScreenToClient(m_pHost->GetWindowHandle(), &ptCur);

			CRect rcBar;
			GetBarRect(m_nHitIndex, &rcBar);
			if(rcBar.PtInRect(ptCur))
				return;

			// The cursor is no longer over the bars. Reset the VP's internal state and redraw the active bar.
			m_bPressed = FALSE;
			m_bHilight = FALSE;

			CGraphicsContext CGC;
			CGC.Attach(GetDC(m_pHost->GetWindowHandle()));//m_pHost->GetCanvas();

			SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(m_nHitIndex);
			m_nHitIndex = -1;

			DrawButton(&CGC, pTabBar, &rcBar);
			DrawIcon(&CGC, pTabBar, &rcBar);		
			DrawText(&CGC, pTabBar, &rcBar);

			::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
		}
		::KillTimer(m_pHost->GetWindowHandle(), m_nTimerID);
		m_nTimerID = 0;
	}

	virtual void OnViewportSetCursor( HWND hWnd, UINT nHitTest, UINT message )
	{
		if( (m_nHitIndex != -1) && (m_pModel->GetTabBarStyle() & SEC_TABBAR_BARCURSOR) )
			::SetCursor(m_pModel->GetTabBarCursor());
		else
			::DefWindowProc( m_pHost->GetWindowHandle(), WM_SETCURSOR, WPARAM(hWnd), MAKELPARAM(nHitTest, message) );
	}

	int GetAnimationInterval()	{ return m_nAnimationInt; }

	void SetAnimationInterval(int nInterval)	{ m_nAnimationInt = nInterval; }

	int GetAnimationStep()	{ return m_nAnimationStep; }

	void SetAnimationStep(int nStep)	{ m_nAnimationStep = nStep; }

	void EditLabel(int nBarIndex)
	{
		// Create the edit control, locate it at the tabbar position and set the tabbar label as the initial text
		SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(nBarIndex);

		HWND hWndHost = m_pHost->GetWindowHandle();
		m_hWndEdit = ::CreateWindow(_T("EDIT"), pTabBar->GetLabel(), WS_CHILD|WS_BORDER, 0,0,0,0,
						hWndHost, (HMENU)0x0111, hAppInstance, NULL);
		assert(m_hWndEdit);
		m_nEditIndex = nBarIndex;

		// Allocate a TLS index and store a reference to the 'this' pointer
		if(g_TlsSlot)
		{
			TlsFree(g_TlsSlot); 
			g_TlsSlot = 0;
		}
		g_TlsSlot = TlsAlloc();
		TlsSetValue(g_TlsSlot,this);
		// Subclass the edit control
		m_OldWndProc = (WNDPROC)::SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG)EditWndProc);

		HFONT hFont = m_pModel->GetTabBarFont(nBarIndex);
		::SendMessage(m_hWndEdit, WM_SETFONT, WPARAM(hFont), 0);
		CRect rcBar;
		GetBarRect(nBarIndex, &rcBar);
		::SetWindowPos(m_hWndEdit, HWND_TOP, rcBar.left, rcBar.top, rcBar.Width(), rcBar.Height(), SWP_SHOWWINDOW);
		::SetFocus(m_hWndEdit);
		::SetCapture(m_hWndEdit);
	}

	void SetHostImpl(IVisualWindow* pHost)
	{
		assert(pHost);
		m_pHost = pHost;
	}

	IVisualWindow* GetHostImpl()	{ return m_pHost; }

protected:
	virtual void DrawBars(CGraphicsContext* pCGC)
	{
		// Iterate the model's bar collection and draw each bar
		CRect rcBounds = GetBounds();
		CPoint pt = GetOrigin();

		SECTabBarPresModel* pBarModel = GetModel();
		int nCount = pBarModel->GetTabBarCount();
		int nActive = pBarModel->GetActiveIndex();
		CRect rcDraw(pt.x, pt.y, rcBounds.Width(), pt.y+nButtonHt);
		for(int i=0; i<nCount; i++)
		{
			SECTabBarBase* pTabBar = pBarModel->GetTabBarAt(i);

			BOOL bPressed = FALSE;
			if(m_bPressed && m_nHitIndex == i)
				bPressed = TRUE;

			DrawButton(pCGC, pTabBar, &rcDraw, bPressed);
			DrawIcon(pCGC, pTabBar, &rcDraw, bPressed);		
			DrawText(pCGC, pTabBar, &rcDraw, bPressed);

			// Adjust the draw rect
			if(i != nActive)
				rcDraw.top = rcDraw.bottom;
			else	// Offset by the client rect
				rcDraw.top = m_rcClient.bottom;
			
			rcDraw.bottom = rcDraw.top+nButtonHt;
		}
	}

	virtual void DrawButton(CGraphicsContext* pCGC, SECTabBarBase* pTabBar, LPRECT lpRect, BOOL bPressed = FALSE)
	{
		CRect rcBtn(*lpRect);

		UINT nBtnStyle = DFCS_BUTTONPUSH;
		if( bPressed )
			nBtnStyle |= DFCS_PUSHED;
		
		int nTabIndex = m_pModel->GetTabBarIndex(pTabBar);

		if(!m_pModel->IsTabBarEnabled(nTabIndex))
			nBtnStyle |= DFCS_INACTIVE;

		pCGC->DrawFrameControl( rcBtn, DFC_BUTTON, nBtnStyle);
		
		// If a background color or brush has been specified for the bar, then fill the button rect with this brush.
		if( !m_pModel->GetTabBarBrush(nTabIndex) )
		{
			if(m_pModel->GetTabBarBkColor(nTabIndex) != GetSysColor(COLOR_BTNFACE))
			{
				CGDIBrush bkBrush;
				bkBrush.CreateSolidBrush(m_pModel->GetTabBarBkColor(nTabIndex));
				CRect rcBkFill = rcBtn;
				rcBkFill.DeflateRect(1,1,2,2);
				pCGC->FillRect( rcBkFill, &bkBrush );
			}
		}
		else
		{
			CGDIBrush bkBrush;
			bkBrush.Attach(m_pModel->GetTabBarBrush(nTabIndex));
			CRect rcBkFill = rcBtn;
			rcBkFill.DeflateRect(1,1,2,2);
			pCGC->FillRect(rcBkFill, &bkBrush);
			bkBrush.Detach();
		}

		if(m_bHilight && !m_bPressed && (m_nHitIndex == m_pModel->GetTabBarIndex(pTabBar)) )
		{
			rcBtn.bottom -= 1;
			pCGC->DrawEdge(&rcBtn, EDGE_RAISED, BF_BOTTOMRIGHT);
		}

	}

	virtual void DrawIcon(CGraphicsContext* pCGC, SECTabBarBase* pTabBar, LPRECT lpRect, BOOL bPressed = FALSE)
	{
		if(pTabBar->GetIcon() == NULL)
		return;

		CRect rcBar(*lpRect);
		if(bPressed)
			rcBar.OffsetRect(1,1);
		rcBar.right -= TEXT_X_OFFSET;

		CRect rcIcon;
		CalcTextRect(pCGC, pTabBar, rcBar, &rcIcon);

		int nTabIndex = m_pModel->GetTabBarIndex(pTabBar);
		if(m_pModel->IsTabBarEnabled(nTabIndex))
			::DrawIconEx(pCGC->GetSafeHdc(), rcIcon.left-20, rcIcon.top, pTabBar->GetIcon(), 16, 16, NULL, NULL, DI_NORMAL);
		else	// Draw the icon in the disabled state
			::DrawState(pCGC->GetSafeHdc(), NULL, NULL, MAKELPARAM(pTabBar->GetIcon(),0), 0, rcIcon.left-20, rcIcon.top,
					16, 16, DST_ICON|DSS_DISABLED);
	}

	virtual void DrawText(CGraphicsContext* pCGC, SECTabBarBase* pTabBar, LPRECT lpRect, BOOL bPressed = FALSE)
	{
		CRect rcBar(*lpRect);
		if(bPressed)
			rcBar.OffsetRect(1,1);
		rcBar.right -= TEXT_X_OFFSET;	

		CRect rcText;
		CalcTextRect(pCGC, pTabBar, rcBar, &rcText);

		int nTabIndex = m_pModel->GetTabBarIndex(pTabBar);

		HFONT hfOld = (HFONT)::SelectObject(pCGC->GetSafeHdc(), m_pModel->GetTabBarFont(nTabIndex));
		int nOldBkMode = pCGC->SetBkMode(TRANSPARENT);
		// Set the text color for the bar
		COLORREF clrOld;
		if(m_pModel->IsTabBarEnabled(nTabIndex))
			clrOld = pCGC->SetTextColor(m_pModel->GetTabBarTextColor(nTabIndex));
		else
			clrOld = pCGC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));

		LPCTSTR lpszLabel = pTabBar->GetLabel();
		pCGC->ExtTextOut( rcText.left, rcText.top, ETO_CLIPPED, rcText, lpszLabel, _tcslen(lpszLabel), NULL );

		pCGC->SetTextColor(clrOld);	
		pCGC->SetBkMode(nOldBkMode);
		::SelectObject(pCGC->GetSafeHdc(), hfOld);
	}

	virtual void ActivateBar(int nNewActv, int nPrevActv)
	{
		if(nNewActv == nPrevActv)
			return;

		// Depending on the styles, may or may not animate the scroll
		if( (nPrevActv >= 0) && !(m_pModel->GetTabBarStyle() & SEC_TABBAR_NOANIMATE))
		{
				AnimateActivate(nNewActv, nPrevActv);
				::InvalidateRect(m_pHost->GetWindowHandle(), LPCRECT(GetBounds()), FALSE);
		}
		else	// Reset all viewport & window states and update the client rect
		{
			UpdateClientRect();
			UpdateActiveClient();
			::InvalidateRect(m_pHost->GetWindowHandle(), LPCRECT(GetBounds()), TRUE);
		}	


		m_nCurrActv = nNewActv;
		if(nPrevActv >= 0)
		{
			SECTabBarBase* pPrevBar = m_pModel->GetTabBarAt(nPrevActv);
			if(!pPrevBar->IsVisualComp())
			{
				HWND hWndClient = (static_cast<CTabBarWnd*> (pPrevBar))->GetClient();
				::ShowWindow(hWndClient, SW_HIDE);
			}
		}	
			
		CPoint ptCur;
		::GetCursorPos(&ptCur);
		::ScreenToClient(m_pHost->GetWindowHandle(), &ptCur);
		CRect rcBar;
		GetBarRect(nNewActv, rcBar);
		if(!rcBar.PtInRect(ptCur))
		{
			m_bHilight = FALSE;
			m_nHitIndex = -1;
		}
		m_bPressed = FALSE;
	}

	virtual void RemoveBar(int nRemoveIndex, int nActivate)
	{
		if(nActivate >= 0)
			ActivateBar(nActivate, nRemoveIndex);
		else	// This is the only bar. Simply hide the client. 
		{
			SECTabBarBase* pPrevBar = m_pModel->GetTabBarAt(nRemoveIndex);
			if(!pPrevBar->IsVisualComp())
			{
				HWND hWndClient = (static_cast<CTabBarWnd*> (pPrevBar))->GetClient();
				::ShowWindow(hWndClient, SW_HIDE);
			}
		}
	}

	virtual void AnimateActivate(int nNewActv, int nPrevActv)
	{
		SECTabBarBase* pNewBar = m_pModel->GetTabBarAt(nNewActv);
		SECTabBarBase* pOldBar = m_pModel->GetTabBarAt(nPrevActv);

		int nOldPane = m_rcClient.Height();
		int nNewPane = 0;

		// Get number of clock cycles to wait between animation
		int iAnimate = GetAnimationInterval();

		// Compute the number of pixels to move per frame animation
		int iStep = GetAnimationStep(); 
		if (iStep <= 0)
		   iStep = 1;
		int iAmountToChange = nOldPane / iStep;
		if (iAmountToChange == 0)
		   iAmountToChange = 1;

		DWORD dwNextTick = GetTickCount() + iAnimate;

		CGraphicsContext CGC;
		CGC.Attach(GetDC(m_pHost->GetWindowHandle()));

		BOOL bScrlDown = FALSE;
		int nBegin, nEnd;
		if(nNewActv < nPrevActv)
		{
			bScrlDown = TRUE;
			nBegin = nNewActv;
			nEnd = nPrevActv;
		}
		else
		{
			nBegin = nPrevActv;
			nEnd = nNewActv;
		}

		int nCount= m_pModel->GetTabBarCount();

		// Allocate an array of rects equal to the number of bars and init the array with the current bar rects
		CRect* pRcBars = new CRect[nCount];	
		for(int i=0; i<=nCount-1; i++)
			GetBarRect(i, pRcBars[i]);

		int nTop = pRcBars[nBegin].bottom;
		int nBottom = (nEnd+1 < nCount) ? pRcBars[nEnd+1].top : GetBounds().bottom;

		m_bInAnimate = TRUE;
		while( (nOldPane - iAmountToChange) >= 1 )
		{
		  // Check if it is time to animate
		  if(GetTickCount() < dwNextTick)
			continue;

		  // Adjust the pane sizes.
			nOldPane -= iAmountToChange;
			nNewPane += iAmountToChange;
			// Compensate for the last partial animation cycle
			if(nOldPane <= iAmountToChange)
				iAmountToChange += nOldPane;

			for(int i=nBegin+1; i<=nEnd; i++)
			{
				if(bScrlDown)
					pRcBars[i].top += iAmountToChange;
				else
					pRcBars[i].top -= iAmountToChange;
				pRcBars[i].bottom = pRcBars[i].top+nButtonHt;
			}

			CRect rcTopPane = CRect(m_rcClient.left, nTop, m_rcClient.right, pRcBars[nBegin+1].top);
			CRect rcBottomPane = CRect(m_rcClient.left, pRcBars[nEnd].bottom, m_rcClient.right, nBottom);
			
			HDWP hdwp = NULL;
			int nWndCount = 0;
			if(!pOldBar->IsVisualComp()) nWndCount++;
			if(!pNewBar->IsVisualComp()) nWndCount++;
			if(nWndCount)
				hdwp = BeginDeferWindowPos(nWndCount);
			InternalAnimateBarClient(pOldBar, &rcBottomPane, &rcTopPane, bScrlDown, hdwp);
			InternalAnimateBarClient(pNewBar, &rcTopPane, &rcBottomPane, bScrlDown, hdwp);
			if(hdwp)
				EndDeferWindowPos(hdwp);
			// Redraw the new positions for the clients
			HWND hWndHost = m_pHost->GetWindowHandle();
			if(!pNewBar->IsVisualComp())
			{
				HWND hWndClient = ((CTabBarWnd*)pNewBar)->GetClient();
				::GetWindowRect(hWndClient, &m_rcClient);
				::ScreenToClient(hWndHost, (LPPOINT)&m_rcClient);
				::ScreenToClient(hWndHost, ((LPPOINT)&m_rcClient)+1);
				::RedrawWindow(hWndClient, NULL, NULL, RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
			}
			else
			{
				IBounds2D* pBounds = NULL;
				pNewBar->QueryGuid(__uuidof(IBounds2D), (void**)&pBounds);
				assert(pBounds);
				m_rcClient = pBounds->GetBounds();
				CRect rcTemp = m_rcClient;
				if(bScrlDown)
					rcTemp.top = m_rcClient.bottom - iAmountToChange;
				else
					rcTemp.bottom = m_rcClient.top + iAmountToChange;
				::RedrawWindow(hWndHost, rcTemp, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
			}

			for(int j=nBegin+1; j<=nEnd; j++)
			{
				SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(j);
				DrawButton(&CGC, pTabBar, pRcBars[j]);
				DrawIcon(&CGC, pTabBar, pRcBars[j]);		
				DrawText(&CGC, pTabBar, pRcBars[j]);
			}

		  // Compute the next animation time
		  dwNextTick = GetTickCount() + iAnimate;
		}
		m_bInAnimate = FALSE;

		delete [] pRcBars;

		::ReleaseDC(m_pHost->GetWindowHandle(), CGC.Detach());
	}

	virtual void InternalAnimateBarClient(SECTabBarBase* pTabBar, CRect* lpRC1, CRect* lpRC2, BOOL bScroll, HDWP hdwp = NULL)
	{
		if(pTabBar->IsVisualComp())
		{
			IBounds2D* pBounds = NULL;
			pTabBar->QueryGuid(__uuidof(IBounds2D), (void**)&pBounds);
			assert(pBounds);
			if(bScroll)
			{
				pBounds->SetOrigin(lpRC1->left, lpRC1->top);
				pBounds->SetSize(lpRC1->Width(), lpRC1->Height());
			}
			else
			{
				pBounds->SetOrigin(lpRC2->left, lpRC2->top);
				pBounds->SetSize(lpRC2->Width(), lpRC2->Height());
			}
		}
		else
		{
			HWND hWndClient = (static_cast<CTabBarWnd*> (pTabBar))->GetClient();
			HWND hWndHost = m_pHost->GetWindowHandle();
			int nFlags = SWP_NOACTIVATE|SWP_NOREDRAW|(::IsWindowVisible(hWndClient) ? 0:SWP_SHOWWINDOW);
			if(bScroll)
			{
				if(hdwp)
					::DeferWindowPos(hdwp, hWndClient, HWND_TOP,lpRC1->left, lpRC1->top, lpRC1->Width(), 
							lpRC1->Height(), nFlags);
				else
					::SetWindowPos(hWndClient, HWND_TOP, lpRC1->left, lpRC1->top, lpRC1->Width(), 
							lpRC1->Height(), nFlags);
			}
			else
			{
				if(hdwp)
					::DeferWindowPos(hdwp, hWndClient, HWND_TOP, lpRC2->left, lpRC2->top, lpRC2->Width(), 
							lpRC2->Height(), nFlags);
				else
					::SetWindowPos(hWndClient, HWND_TOP, lpRC2->left, lpRC2->top, lpRC2->Width(), 
							lpRC2->Height(), nFlags);
			}
		}
	}

	void CalcTextRect(CGraphicsContext* pCGC, SECTabBarBase* pTabBar, CRect rcBar, CRect* lpRcText)
	{
		int nTabIndex = m_pModel->GetTabBarIndex(pTabBar);

		HFONT hfOld = (HFONT)::SelectObject(pCGC->GetSafeHdc(), m_pModel->GetTabBarFont(nTabIndex));
		LPCTSTR lpszLabel = pTabBar->GetLabel();
		CSize sizeText = pCGC->GetTextExtent(lpszLabel, _tcslen(lpszLabel));
		::SelectObject(pCGC->GetSafeHdc(), hfOld);

		CPoint ptOrg = GetOrigin();
		// Calculate the left point of the text depending on the label alignment
		switch(m_pModel->GetLabelAlignment())
		{
		case SEC_TABBAR_LBLALIGNLEFT:
			lpRcText->left = ptOrg.x + TEXT_X_OFFSET + (pTabBar->GetIcon()==NULL ? 0:20);	// offset + icon width
			break;

		case SEC_TABBAR_LBLALIGNCENTER:
			lpRcText->left = ptOrg.x + (rcBar.Width()-sizeText.cx)/2;
			break;

		case SEC_TABBAR_LBLALIGNRIGHT:
			lpRcText->left = ptOrg.x + (rcBar.Width()-TEXT_X_OFFSET-sizeText.cx);
			break;

		default:
			assert(0);
		}	
		lpRcText->top	 = (rcBar.Height() - sizeText.cy) / 2;
		lpRcText->right  = lpRcText->left + sizeText.cx;
		lpRcText->bottom = lpRcText->top  + sizeText.cy;
		
		//lpRcText now holds the rect for the text...but now we need to line it up with the button.
		int height = lpRcText->Height();
		lpRcText->top += rcBar.top;
		lpRcText->bottom = lpRcText->top + height;
	}

	BOOL GetBarRect(int nIndex, LPRECT lpRect)
	{
		SECTabBarPresModel* pModel = GetModel();
		int nCount = pModel->GetTabBarCount();
		int nActive = m_nCurrActv;

		CPoint ptOrg = GetOrigin();
		CRect rcBounds = GetBounds();

		*lpRect = CRect(ptOrg.x, ptOrg.y, rcBounds.Width(), ptOrg.y+nButtonHt);
		for(int i=0; i<nCount; i++)
		{
			if(i == nIndex)
				return TRUE;

			if(i != nActive)
				lpRect->top = lpRect->bottom;
			else	
				lpRect->top = m_rcClient.bottom;
			lpRect->bottom = lpRect->top+nButtonHt;
		}
		return FALSE;
	}

	void UpdateClientRect()
	{
		SECTabBarPresModel* pModel = GetModel();
		int nCnt = pModel->GetTabBarCount();
		if(nCnt <= 0)
		{
			m_rcClient = CRect(0,0,0,0);
			return;
		}
		int nActv = pModel->GetActiveIndex();
		CRect rcBounds = GetBounds();
		CPoint pt = GetOrigin();

		m_rcClient = CRect(pt.x, pt.y, rcBounds.Width(), rcBounds.Height());
		for(int i=0; i<nCnt; i++)
		{
			m_rcClient.top += nButtonHt;
			if(i == nActv)
				break;		
		}
		m_rcClient.bottom = m_rcClient.top + (rcBounds.Height() - (nCnt * nButtonHt));
	}

	// In case of a viewport simply resize the bounds rect. The _Draw method takes care of invoking the viewport's rendering functions. 
	// If the active bar hosts a HWND, then position this window now. In case of a viewport, just specify the coords.
	void UpdateActiveClient()
	{
		if((m_pModel->GetTabBarCount() <= 0) || (m_pModel->GetActiveIndex() < 0))
		return;

		SECTabBarBase* pActiveBar = m_pModel->GetTabBarAt(m_pModel->GetActiveIndex());
		if(pActiveBar->IsVisualComp())
		{
			IBounds2D* pBounds = NULL;
			pActiveBar->QueryGuid(__uuidof(IBounds2D), (void**)&pBounds);
			assert(pBounds);
			pBounds->SetOrigin(m_rcClient.left, m_rcClient.top);
			pBounds->SetSize(m_rcClient.Width(), m_rcClient.Height());		
		}
		else
		{
			HWND hWndClient = (static_cast<CTabBarWnd*> (pActiveBar))->GetClient();
			HWND hWndHost = m_pHost->GetWindowHandle();
			::SetWindowPos( hWndClient, HWND_TOP, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(),
								SWP_SHOWWINDOW);		
		}	
	}

	void ProcessEditing(BOOL bSave)
	{
		if(bSave)
		{
			int nLen = ::GetWindowTextLength(m_hWndEdit);
			TCHAR* pszLabel = new TCHAR[nLen+1];
			::GetWindowText(m_hWndEdit, pszLabel, nLen+1);

			SECTabBarBase* pTabBar = m_pModel->GetTabBarAt(m_nEditIndex);
			pTabBar->SetLabel(pszLabel);
			delete pszLabel;
		}
		m_nEditIndex = -1;
		::ReleaseCapture();
		::SetFocus(m_pHost->GetWindowHandle());

		// Unsubclass the edit control and destroy it
		::SetWindowLong(m_hWndEdit, GWL_WNDPROC, (LONG)m_OldWndProc);
		::DestroyWindow(m_hWndEdit);

		// Free the TLS slot
		if(g_TlsSlot)
		{
			TlsFree(g_TlsSlot); 
			g_TlsSlot = 0;
		}
	}

	static LRESULT CALLBACK EditWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SECBarViewport* _this = (SECBarViewport*)TlsGetValue(g_TlsSlot);
		assert(hWnd == _this->m_hWndEdit);

		switch(uMsg)
		{
		case WM_KEYDOWN:
			if(wParam == VK_RETURN)
			{
				_this->ProcessEditing(TRUE);
				return 0;
			}
			else if(wParam == VK_ESCAPE)
			{
				_this->ProcessEditing(FALSE);		
				return 0;
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				CPoint ptCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				CRect rcEdit;
				::GetClientRect(hWnd, &rcEdit);
				if(!rcEdit.PtInRect(ptCursor))
					_this->ProcessEditing(FALSE);
				break;
			}
		case WM_CAPTURECHANGED:
			if(_this->m_nEditIndex != -1)
				::SetCapture(_this->m_hWndEdit);
			break;

		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;
			break;

		default:
			break;
		}

		return CallWindowProc(_this->m_OldWndProc, hWnd, uMsg, wParam, lParam);
	}	

protected:
	IVisualWindow* m_pHost;
	CRect m_rcClient;
	int m_nHitIndex;
	BOOL m_bPressed;
	BOOL m_bHilight;
	UINT m_nTimerID;

	HWND m_hWndEdit;
	int  m_nEditIndex;

	// Animation helper members
	int m_nAnimationInt;
	int m_nAnimationStep;
	int m_nCurrActv;
	BOOL m_bInAnimate;

	WNDPROC m_OldWndProc;
};

};	// namespace stingray

#endif //__TABBAR_VPORT_H__

