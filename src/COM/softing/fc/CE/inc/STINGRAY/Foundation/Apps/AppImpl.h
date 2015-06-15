/////////////////////////////////////////////////////////////////////////////
// AppImpl.h : OTL Application implementation
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//

#pragma once


#include <Foundation\Apps\Application.h>


namespace stingray {
namespace foundation {



template <typename _WindowClass, typename _Base>
void CCreateWindowMessageLoop<_WindowClass, _Base>::CreateMainWindow (
	int nCmdShow, 
	void* lpParam
)
{
	_WindowClass* pwnd = new _WindowClass;
	pwnd->Create(0, CWindow::rcDefault, NULL, 0, 0, 0, lpParam);
	pwnd->CenterWindow();
	pwnd->ShowWindow(nCmdShow);
	pwnd->UpdateWindow();
	m_pwndMain = pwnd;
}


template <typename _WindowClass, typename _Base>
void CCreateWindowMessageLoop<_WindowClass, _Base>::DestroyMainWindow (
)
{
	if (m_pwndMain->m_hWnd != NULL) {
		m_pwndMain->DestroyWindow();
	}
	delete m_pwndMain;
}

template <typename _DialogClass, typename _Base>
void CCreateDialogMessageLoop<_DialogClass, _Base>::CreateMainWindow (
		int nCmdShow,
		void* lpParam)
{
	_DialogClass* pwnd = new _DialogClass;
	pwnd->Create(0, CWindow::rcDefault, reinterpret_cast<LPARAM>(lpParam));
	pwnd->ShowWindow(nCmdShow);
	m_pwndMain = pwnd;
}

template <typename _DialogClass, typename _Base>
void CCreateDialogMessageLoop<_DialogClass, _Base>::DestroyMainWindow ()
{
	if (m_pwndMain->m_hWnd != NULL) {
		m_pwndMain->DestroyWindow();
	}
	delete m_pwndMain;
}



};
};


