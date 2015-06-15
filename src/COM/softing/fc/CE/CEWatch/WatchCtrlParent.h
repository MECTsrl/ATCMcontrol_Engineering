#ifndef __WATCHCTRLPARENT_H_
#define __WATCHCTRLPARENT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "WatchCtrlWrapper.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CWatchControlParent : public CWnd
{
public:
    CWatchControlParent ();
    virtual ~CWatchControlParent ();

    CWatchControlWrapper *GetWrapper ();

protected:
	virtual void PostNcDestroy ();

	CWatchControlWrapper m_ControlWrapper;

public:
	DECLARE_DYNCREATE(CWatchControlParent)

    //{{AFX_VIRTUAL(CWatchControlParent)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CWatchControlParent)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __WATCHCTRLPARENT_H_

