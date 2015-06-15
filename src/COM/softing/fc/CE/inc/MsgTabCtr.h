// MsgTabCtr.h: interface for the CMsgTabCtr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTABCTR_H__1B67E3EE_70B8_4057_82FD_3BF779078302__INCLUDED_)
#define AFX_MSGTABCTR_H__1B67E3EE_70B8_4057_82FD_3BF779078302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Describes position (m_Point) of a tab control hit and which tab was hit (m_iTabIndex).
// Position is in screen coordinates (need these for TrackPopupMenu).
// Tab index is as used by tab control and -1 if no tab was hit.

struct MsgTabHitInfo
{
    CPoint m_Point;
    int    m_iTabIndex;
};


class CMsgTabCtr : public SECTabControl  
{
public:
	CMsgTabCtr();
	virtual ~CMsgTabCtr();
    virtual void Initialize (DWORD dwStyle);
    void    DeleteTab (int iIndex);
    void    SetTabIcon(int nIndex, HICON hIcon);
    void    SetTabColor(int nIndex, COLORREF Color);
    SECTab* CreateNewTab() const;
    void    DrawTab(CDC *pDC, int nTab, CRgn &tabRgn);
    void    RecalcLayout();
    void    SetContextMenuOwner (CWnd *pOwner);
    int     GetTabIndexFromScreenPosition(const CPoint& rtPoint);
    CString GetTabToolTipText(int iTab = -1);

protected:
    CWnd*   m_pContextMenuOwner;

public:
    DECLARE_DYNAMIC (CMsgTabCtr)

protected:
    //{{AFX_MSG(SECTabControl)
    afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
    afx_msg BOOL OnToolTipNfy (UINT id, NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

#endif // !defined(AFX_MSGTABCTR_H__1B67E3EE_70B8_4057_82FD_3BF779078302__INCLUDED_)



