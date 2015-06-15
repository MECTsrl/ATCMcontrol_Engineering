#if !defined(AFX_MSGFRAME_H__02DC42F9_8E66_11D1_AE09_02608C7C2DDE__INCLUDED_)
#define AFX_MSGFRAME_H__02DC42F9_8E66_11D1_AE09_02608C7C2DDE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MsgFrame.h : header file
//
#include "MsgTabWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMsgFrame frame


static const int INDICATOR_BITMAP_WIDTH = 13;

static const int INDICATOR_TRANSPARENT_COLOR = RGB (255, 0, 255);

static const int IDI_ESTABLISHED      = 0;
static const int IDI_ALL_ESTABLISHED  = 1;
static const int IDI_INTERRUPTED      = 2;
static const int IDI_COMM_INTERRUPTED = 3;
static const int IDI_COMM_ESTABLISHED = 4;

class CMsgFrame : public SECMDIFrameWnd
{
    DECLARE_DYNCREATE(CMsgFrame)
public:
	CMsgFrame();           // protected constructor used by dynamic creation
    
// Attributes
public:
    CMsgTabWnd   m_tabWnd;

private:
    CImageList  m_ImageList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMsgFrame();

	// Generated message map functions
	//{{AFX_MSG(CMsgFrame)
	afx_msg void OnDropFiles(HDROP hDropInfo);
 	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    // set image for tab
    void SetImage(int iTabIdx, int iImageIdx);

    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

    void InsertTargetTab(int iIndex, const CString& crstrTargetName);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGFRAME_H__02DC42F9_8E66_11D1_AE09_02608C7C2DDE__INCLUDED_)