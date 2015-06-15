
//
#if !defined(AFX_DLSTATEDLG_H__F45CBEC1_1C08_11D2_B9AB_006008749B3D__INCLUDED_)
#define AFX_DLSTATEDLG_H__F45CBEC1_1C08_11D2_B9AB_006008749B3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "resource.h"

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CStationList;
class CStation;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CDLStateDlg dialog

class CDLStateDlg : public CDialog
{
    int  m_iSelection;  //selection index
// Construction
public:
	CDLStateDlg (CStationList *pStationList, CComObject<CImpDLEvents> *pEvents, CWnd *pParent=NULL);

	//{{AFX_DATA(CDLStateDlg)
	enum { IDD = IDD_DOWNLOADSTATE };
	CButton	m_Details;
	CListCtrl	m_ControlList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDLStateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void PrepareList(void);
	void AddColumn (const CString &strHeading, int iColumn);
    void CheckDownloadState(int iIndex, CStation *pStation);
	void ShowDetails(void);
	bool IsConnected(void);

protected:
	CStationList                     *m_pStationList;
    CComObject<CImpDLEvents>         *m_pEvents;

	//{{AFX_MSG(CDLStateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDetails();
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LONG OnVarChanged( UINT, LONG );
	virtual void OnCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_DLSTATEDLG_H__F45CBEC1_1C08_11D2_B9AB_006008749B3D__INCLUDED_)
