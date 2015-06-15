#ifndef __WATCHTABWND_H_
#define __WATCHTABWND_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CWatchTabWindow : public SECTabWnd
{
public:
    CWatchTabWindow ();
    virtual ~CWatchTabWindow ();

    void SetContextMenuOwner (CWnd *pOwner);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECTabWnd)
    //}}AFX_VIRTUAL

protected:
    virtual BOOL CreateTabCtrl (DWORD dwStyle, UINT uiID);

    //{{AFX_MSG(CWatchTabWindow)
    afx_msg LRESULT OnTabSelect(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP();

    CWnd *m_pContextMenuOwner;

public:
    DECLARE_DYNCREATE(CWatchTabWindow)
};

#endif // __WATCHTABWND_H_
