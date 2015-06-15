/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTestDlg.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlTestDlg.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTestDlg.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_ONLCOMMTESTDLG_H__0729613C_B875_4A45_8F34_3F2EA4894C95__INCLUDED_)
#define AFX_ONLCOMMTESTDLG_H__0729613C_B875_4A45_8F34_3F2EA4894C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _ONLCOMM __declspec(dllimport)

#include "4CLayer.h"
#include "CommBase.h"

#define CB_MEMORY 0
#define CB_STRING 1
#define CB_DINT   2
#define CB_UDINT  3
#define CB_INT    4
#define CB_UINT   5
#define CB_BYTE   6
#define CB_BOOL   7


/////////////////////////////////////////////////////////////////////////////
// COnlTestDlg dialog

class COnlTestDlg : public CDialog
{
// Construction
public:
	COnlTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COnlTestDlg)
	enum { IDD = IDD_ONLCOMMTEST_DIALOG };
	CComboBox	m_cbType;
	CComboBox	m_cbPort;
	CListBox	m_List;
	CString	m_sConnectParam;
	UINT	m_uBit;
	UINT	m_uLength;
	UINT	m_uOffset;
	UINT 	m_uSegment;
	BOOL	m_bCheckSwap;
	CString	m_strPath;
	CString	m_sFile;
	CString	m_sGetSet;
	CString	m_sDBI;
	CString	m_sCommDLL;
	BOOL	m_bPortScan;
	CString	m_sTCPAddr;
	int		m_iConnType;
	CString	m_sState;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnlTestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	public:
		virtual ~COnlTestDlg();
		
// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(COnlTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnUnload();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnLogout();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnDisconnect();
	afx_msg void OnCheckSwap();
	afx_msg void OnBtnState();
	afx_msg void OnBtnGetValue();
	afx_msg void OnBtnSetValue();
	afx_msg void OnBtnDbiGetChild();
	afx_msg void OnBtnDbiGetAddr();
	afx_msg void OnDblclkList();
	afx_msg void OnSelchangeList();
	afx_msg void OnBtnDbiRes();
	afx_msg void OnBtnSelCommDLL();
	afx_msg void OnRadioOnlComm();
	afx_msg void OnRadioVisuComm();
	afx_msg void OnCheckPort();
	afx_msg void OnBtnDbiGetParent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
	void EnableControls();
	BOOL LoadFile(LPCTSTR szFile, BYTE **ppData, UINT *upLen);
	BOOL StoreFile(LPCTSTR szFile, BYTE *pData, UINT uLen);



LPCTSTR COnlTestDlg::GetTypeStr(UINT uType);

	// ########################################################################
	
	protected:
		CCommBase	*m_pComm;

	protected:
		BYTE	*m_pDBIVar;
		BYTE	*m_pChildren;
		BYTE	*m_pAddress;

		USHORT	m_uDBIVar;
		USHORT	m_uChildren;
		USHORT	m_uAddress;

		CString m_sDBICurrent;

	protected:
		void	CreateComm();
		void	CreateDBIVar();
		void	AddDBIVar(BYTE *pVar);
		void	DelDBIVar();
		void	Free();
		void	FreeListBox();

	protected:
		void	ShowChildren();

	private:
		void	Init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLCOMMTESTDLG_H__0729613C_B875_4A45_8F34_3F2EA4894C95__INCLUDED_)

/* ---------------------------------------------------------------------------- */
