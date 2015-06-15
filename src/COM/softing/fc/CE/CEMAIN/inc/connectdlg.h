
#ifndef __CONNECTDLG_H_
#define __CONNECTDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CTargetOnlBase;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConnectDlg 

class CConnectDlg : public CDialog
{
    UINT            m_TimerId;
    CTargetOnlBase* m_pTarget;
    BOOL            m_bExclusive;
    int             m_iConnectTimeOut;
    BOOL            m_bForced;
    BOOL            m_bWasConnected;
    BOOL            m_bWaitForTargetConnection;

// Konstruktion
public:
	CConnectDlg( CTargetOnlBase * pTarget, int iConnectTimeOut, BOOL bExclusive = FALSE, CWnd* pParent = NULL);   // Standardkonstruktor
    void GetStateText(CString & strStateText);

// Dialogfelddaten
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_CONNECT };
	CProgressCtrl	m_ProgrCtrl;
    CString         m_strStateText;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CConnectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancel();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    HRESULT InitiateMonitoringConnection ();
    HRESULT InitiateDebugConnection ();
    void RevertConnectState ();
};

/////////////////////////////////////////////////////////////////////////////
// CConnErrDlg dialog

class CConnErrDlg : public CDialog
{
// Construction
public:
	CConnErrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConnErrDlg)
	enum { IDD = IDD_CONNECT_ERROR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnErrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnErrDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // __CONNECTDLG_H_

