#ifndef __WATCHTABCTRL_H_
#define __WATCHTABCTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

// Message sent to context menu owner on right mouse click inside tab control
// WPARAM: not used
// LPARAM: pointer to WatchTabHitInfo
// result: not used
#define WM_WATCHTAB_CONTEXTMENU (WM_APP + 1)
#define WM_WATCHTAB_DBLCLCK     (WM_APP + 2)
#define WM_WATCHTAB_RENAME      (WM_APP + 3)

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


// Describes position (m_Point) of a tab control hit and which tab was hit (m_iTabIndex).
// Position is in screen coordinates (need these for TrackPopupMenu).
// Tab index is as used by tab control and -1 if no tab was hit.

struct WatchTabHitInfo
{
    CPoint m_Point;
    int    m_iTabIndex;
};


class CWatchTabControl : public SECTabControl
{
public:
    CWatchTabControl ();
    virtual ~CWatchTabControl ();

    virtual void Initialize (DWORD dwStyle);

    void DeleteTab (int iIndex);

    void SetContextMenuOwner (CWnd *pOwner);

protected:
    CWnd *m_pContextMenuOwner;

public:
    DECLARE_DYNAMIC (CWatchTabControl)

protected:
    //{{AFX_MSG(SECTabControl)
    afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};


#endif // __WATCHTABCTRL_H_
