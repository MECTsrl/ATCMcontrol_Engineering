#if !defined(AFX_EVENTSELECTION_H__D459AA78_2F39_4B96_8C37_BAD3BEBB0AD2__INCLUDED_)
#define AFX_EVENTSELECTION_H__D459AA78_2F39_4B96_8C37_BAD3BEBB0AD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventSelection.h : header file
//

#include "resource.h"

// NFTASKHANDLING 30.05.05 SIS >>
#include "extern.h"
#include "CEProjInfo.h"
#include "AfxTempl.h"

class CEventDescription
{
public:
	CEventDescription() {m_iIdx = 0;};

	int			m_iIdx;				// Event-Index
	CString		m_strName;			// Event-Name
	CString		m_strDescription;	// Event-Description
};

typedef CList<CEventDescription, CEventDescription&> TEventList;
// NFTASKHANDLING 30.05.05 SIS <<

/////////////////////////////////////////////////////////////////////////////
// CEventSelection dialog

class CEventSelection : public CDialog
{
// Construction
public:
	CEventSelection(CWnd* pParent = NULL);   // standard constructor

    void SetEventString(const CString& crstrEvent);
    CString GetEventString();

	// NFTASKHANDLING 30.05.05 SIS >>
	void SetProjInfo(ICEProjInfo* pICEProjectInfo);
	void SetResource(const CString& crstrResource);
	// NFTASKHANDLING 30.05.05 SIS <<

// Dialog Data
	//{{AFX_DATA(CEventSelection)
	enum { IDD = IDD_EVENTSELECT };
	CListCtrl	m_tEventList;
	CComboBox	m_tTargetCombo;							// NFTASKHANDLING 30.05.05 SIS
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventSelection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventSelection)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickEventList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeTargetCombo();		// NFTASKHANDLING 30.05.05 SIS
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    CString BuildEventString();
    void ParseEventString();
    void UpdateStates();

	// NFTASKHANDLING 30.05.05 SIS >>
	void FillTargetCombo();
	void FillEventList();
	BOOL ReadEventDescription(TEventList& rtEventList);
	BOOL ParseEventDescription(CEventDescription& rtEventDescription, const CString& crstrLine);
	void SkipWhiteSpace(const CString& crstrLine, int& riChar);
	BOOL CheckEventDescription(const CString& crstrTargetType);
	CString GetTargetExtensionFromType(const CString& crstrTargetType);
	CString GetAttributeOfTarget(const CString& crstrTargetIdPath, const CString& crstrAttribute);
	// NFTASKHANDLING 30.05.05 SIS <<

protected:
    CImageList		m_tStateImages;
    CString			m_strEvent;
	// NFTASKHANDLING 30.05.05 SIS >>
    ICEProjInfo*	m_pICEProjectInfo;
	CString			m_strResource;
	BOOL			m_bEventListFilled;
	// NFTASKHANDLING 30.05.05 SIS <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTSELECTION_H__D459AA78_2F39_4B96_8C37_BAD3BEBB0AD2__INCLUDED_)
