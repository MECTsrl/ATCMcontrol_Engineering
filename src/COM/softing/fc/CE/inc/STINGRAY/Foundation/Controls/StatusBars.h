/////////////////////////////////////////////////////////////////////////////
// sflstatusbars.h SFL Status support
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       George Shepherd
// Description:  Status Support.
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <Foundation\Controls\AtlControls.h>

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// StatusBar

const int nStatusBarID	= 100;//also the string res for status text eventually

struct STATUSPANEINFO
{
	int m_nWidth;
	int m_ID;
};

template <typename _Base = CStatusBarCtrl, typename _WinTraits = CStatusBarWinTraits>
class CStatusBarImpl : 
	public CWindowImpl<CStatusBarImpl,_Base,_WinTraits>
{

	typedef CStatusBarImpl<_Base,_WinTraits >					thisClass;
	typedef CWindowImpl<CStatusBarImpl, _Base, _WinTraits>		baseClass;

public:

	DECLARE_WND_SUPERCLASS(_T("OTLStatusBarClass"), _Base::GetWndClassName());
    BEGIN_MSG_MAP(thisClass)
	END_MSG_MAP()


	CStatusBarImpl()
	{
		m_pStatusPanes = 0;
		m_nPanes = 0;
	}

	virtual ~CStatusBarImpl()
	{
		if (m_pStatusPanes) {
			delete [] m_pStatusPanes;
		}
	}

	int IDToPaneIndex(int nID)
	{
		int n = -1;
		for (int i = 0; i < m_nPanes; i++) {
			if(nID == m_pStatusPanes[i].m_ID) {
				return i; 
			}
		}		
		return n;
	};

	
	HWND Create (
		HWND hWndParent, 
		RECT& rcPos,
		LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, 
		DWORD dwExStyle = 0,
		UINT nID = nStatusBarID, 
		LPVOID lpCreateParam = NULL
	)
	{
		if (GetWndClassInfo().m_lpszOrigName == NULL)
			GetWndClassInfo().m_lpszOrigName = GetWndClassName();
		ATOM atom = GetWndClassInfo().Register(&m_pfnSuperWindowProc);

		dwStyle = GetWndStyle(dwStyle);
		dwExStyle = GetWndExStyle(dwExStyle);

		HWND hWnd = CWindowImplBaseT<_Base,_WinTraits>::Create(hWndParent, rcPos, szWindowName,dwStyle, dwExStyle, nID,atom,lpCreateParam);
		ATLASSERT(::IsWindow(hWnd));
		
		return hWnd;
	}


	void SetPanes(int nPanes, STATUSPANEINFO* pStatusPanes)
	{
		if (m_pStatusPanes) {
			delete [] m_pStatusPanes;
			m_nPanes = 0;
		}

		m_pStatusPanes = new STATUSPANEINFO[nPanes];
		if (m_pStatusPanes) {
			memcpy(m_pStatusPanes, pStatusPanes, sizeof(STATUSPANEINFO) * nPanes);
			m_nPanes = nPanes;
		}

		RECT rect;

		GetClientRect(&rect);
		int* rgParts = 0;

		rgParts = new int[nPanes];

		for (int x = 0; x < nPanes; x++) { 
			int nCume = 0;
			for(int y = x; y < nPanes; y++) {
				nCume += pStatusPanes[y].m_nWidth;
			}

			if(pStatusPanes[x].m_nWidth == -1) {
				rgParts[x] = -1;
			} 
			else {
				rgParts[x] = rect.right - nCume;
			}
		}

		SetParts(nPanes, rgParts);	
		
		if (rgParts) {
			delete [] rgParts;
		}
	}

	STATUSPANEINFO* m_pStatusPanes;
	int m_nPanes;

};

typedef CWinTraits<	WS_CHILD | WS_VISIBLE | 
					WS_CLIPCHILDREN |
					WS_CLIPSIBLINGS | 
					CCS_BOTTOM | 
					SBARS_SIZEGRIP, 0> CStatusBarWinTraits;

typedef CWinTraits<WS_CHILD | WS_VISIBLE> CNothingWinTraits;

typedef CStatusBarImpl<ATLControls::CStatusBarCtrl, CStatusBarWinTraits> CStatusBar;



};	// namespace stingray::foundation
};	// namespace stingray
