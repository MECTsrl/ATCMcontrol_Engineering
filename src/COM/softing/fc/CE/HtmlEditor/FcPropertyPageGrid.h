#if !defined(AFX_FCPROPERTYPAGEGRID_H__D75978B6_D56E_11D2_A002_00A024363DFC__INCLUDED_)
#define AFX_FCPROPERTYPAGEGRID_H__D75978B6_D56E_11D2_A002_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcPropertyPageGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcPropertyPageGrid dialog

class CFcPropertyPageGrid : public CPropertyPage
{
	DECLARE_DYNCREATE(CFcPropertyPageGrid)

// Construction
public:
	CFcPropertyPageGrid(CFcHtmlEdView* pParent = NULL);
	~CFcPropertyPageGrid();

// Dialog Data
	//{{AFX_DATA(CFcPropertyPageGrid)
	enum { IDD = IDD_GRID };
	UINT	m_Horz;
	UINT	m_Vert;
	BOOL	m_bGridOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFcPropertyPageGrid)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFcPropertyPageGrid)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFcHtmlEdView* m_pParent;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCPROPERTYPAGEGRID_H__D75978B6_D56E_11D2_A002_00A024363DFC__INCLUDED_)
