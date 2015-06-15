// TestProjManDlg1Dlg.h : header file
//

#if !defined(AFX_TESTPROJMANDLG1DLG_H__7B12599D_B899_4D30_AB92_6EBBCD4871AC__INCLUDED_)
#define AFX_TESTPROJMANDLG1DLG_H__7B12599D_B899_4D30_AB92_6EBBCD4871AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEProjMan.h"
#include "CEContainer.h"
#include "ProjManEventsImpl.h"
#include "MsgEventsImpl.h"
#include "CompManEventsImpl.h"

#import <msxml.dll> raw_interfaces_only named_guids
using namespace MSXML;
#include <atlbase.h>

/////////////////////////////////////////////////////////////////////////////
// CTestProjManDlg1Dlg dialog

class CTestProjManDlg1Dlg : public CDialog
{
// Construction
public:
	CTestProjManDlg1Dlg(CWnd* pParent = NULL);	// standard constructor
    ~CTestProjManDlg1Dlg();

    void ChangeChangeState(BOOL state);
    void AddMsgString(const CString& str);
    void AddMsgViewString(const CString& str);
    void AddTestOutString(const CString& str);
    void BuildTree(const CXMLNode& node, int einr, CString& text);

// Dialog Data
	//{{AFX_DATA(CTestProjManDlg1Dlg)
	enum { IDD = IDD_TESTPROJMANDLG1_DIALOG };
	CEdit	m_edit_test_2;
	CEdit	m_edit_test_ClassType;
	CEdit	m_edit_testError;
	CEdit	m_edit_testOut;
	CEdit	m_edit_cont_Error;
	CEdit	m_edit_cont_OldLink;
	CEdit	m_edit_cont_NewLink;
	CEdit	m_edit_cont_Location;
	CEdit	m_edit_cont_FileName;
	CEdit	m_edit_cont_ParentLocation;
	CEdit	m_edit_cont_ParentFile;
	CEdit	m_editCPOutput;
	CEdit	m_editValue;
	CEdit	m_editSection;
	CEdit	m_editKey;
	CEdit	m_editIMsg;
	CEdit	m_editMsg;
	CStatic	m_staticChange;
	CEdit	m_editTrees;
	CEdit	m_editProjName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestProjManDlg1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CComPtr <ICEProjectManager> pProjMan;

    CComObject<CProjManEventsImpl> *m_pProjEventHandler;
	DWORD                           m_dwProjEventHandlerCookie;
    CComObject<CMsgEventsImpl>     *m_pMsgEventHandler;
	DWORD                           m_dwMsgEventHandlerCookie;
    CComObject<CCompManEventsImpl> *m_pCompManEventHandler;
	DWORD                           m_dwCompManEventHandlerCookie;


    ICEContainer* m_pContainer;

public:
    HRESULT AdviseProjEventHandler();
	void UnadviseProjEventHandler();

    HRESULT AdviseMsgEventHandler();
	void UnadviseMsgEventHandler();

    HRESULT AdviseCompManEventHandler();
	void UnadviseCompManEventHandler();

protected:
	// Generated message map functions
	//{{AFX_MSG(CTestProjManDlg1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenProj();
	afx_msg void OnCloseProj();
	afx_msg void OnShowClass();
	afx_msg void OnShowTarget();
	afx_msg void OnReparse();
	afx_msg void OnSave();
	afx_msg void OnClassTree();
	afx_msg void OnTargetTree();
	afx_msg void OnCPInsert();
	afx_msg void OnCPDeleteKey();
	afx_msg void OnCPDeleteKeyVal();
	afx_msg void OnCPGetVal();
	afx_msg void OnCPGetAllVal();
	afx_msg void OnContGetContainer();
	afx_msg void OnContCreate();
	afx_msg void OnContRename();
	afx_msg void OnContDelete();
	afx_msg void OnContGetContainerFull();
	afx_msg void OnTestGetLibPath();
	afx_msg void OnTestInstPath();
	afx_msg void OnTestGetAllClassTypes();
	afx_msg void OnTestGetAllTargetTypes();
	afx_msg void OnTestTextForClassType();
	afx_msg void OnTestFileChangeHint();
	afx_msg void OnTestFileParseNow();
	afx_msg void OnTestGetXMLNodePtr();
	afx_msg void OnTestGetNameProp();
	afx_msg void OnTestGetAllClasses();
    afx_msg void OnTestGetAllClassesIdPaths();
	afx_msg void OnTestGetAllClassesOfType();
    afx_msg void OnTestGetAllClassesOfTypeIdPaths();
	afx_msg void OnTestGetAllTargets();
    afx_msg void OnTestGetAllTargetsIdPaths();
	afx_msg void OnTestGetAllFilesOfCat();
	afx_msg void OnTestGetFileCat();
	afx_msg void OnTestAddFVFile();
	afx_msg void OnTestRemoveFVFile();
	afx_msg void OnTestGetFileFromIdPath();
	afx_msg void OnTestGetIdPathFromFile();
	afx_msg void OnTestGetTypeOfTarget();
	afx_msg void OnGetAddressStringForTarget();
	afx_msg void OnGetUserAddrStringForTarget();
	afx_msg void OnGetNameForAddr();
	afx_msg void OnGetResourceForTarget();
	afx_msg void OnGetFreeChannelNrForPC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPROJMANDLG1DLG_H__7B12599D_B899_4D30_AB92_6EBBCD4871AC__INCLUDED_)
