#if !defined(AFX_CEWATCHVIEWCTL_H__FA85C92D_E5B6_11D2_A00A_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHVIEWCTL_H__FA85C92D_E5B6_11D2_A00A_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CeWatchDlg.h"
// CeWatchViewCtl.h : Declaration of the CCeWatchViewCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CCeWatchViewCtrl : See CeWatchViewCtl.cpp for implementation.

class CCeWatchViewCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCeWatchViewCtrl)

// Constructor
public:
	CCeWatchViewCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCeWatchViewCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCeWatchViewCtrl();

	DECLARE_OLECREATE_EX(CCeWatchViewCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCeWatchViewCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCeWatchViewCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCeWatchViewCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCeWatchViewCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCeWatchViewCtrl)
	afx_msg void SetProjectPath(LPCTSTR path);
	afx_msg BOOL Store(LPCTSTR file);
	afx_msg BOOL Load(LPCTSTR file);
    afx_msg void ForceResubscribe();
	afx_msg SCODE SaveToXMLString(BSTR* psXMLString);
	afx_msg SCODE LoadFromXMLString(LPCTSTR sXMLString);
	afx_msg SCODE LoadFromXMLString2(BSTR* psXMLString);
    afx_msg void Deactivate();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CCeWatchViewCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCeWatchViewCtrl)
	dispidSetProjectPath = 1L,
	dispidStore = 2L,
	dispidLoad = 3L,
	dispidForceResubscribe = 4L,
	dispidSaveToXMLString = 5L,
	dispidLoadFromXMLString = 6L,
	dispidLoadFromXMLString2 = 7L,
	//}}AFX_DISP_ID
	};

private:
	CCeWatchDlg m_Dlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHVIEWCTL_H__FA85C92D_E5B6_11D2_A00A_00A024363DFC__INCLUDED)
