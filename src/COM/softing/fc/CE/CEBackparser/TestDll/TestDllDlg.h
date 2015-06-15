// TestDllDlg.h : header file
//
#pragma warning(disable: 4786)

#if !defined(AFX_TESTDLLDLG_H__83816583_F84C_11D4_A09E_006008736ABF__INCLUDED_)
#define AFX_TESTDLLDLG_H__83816583_F84C_11D4_A09E_006008736ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "POU.h"
#include "FunctionBlock.h"
#include "lex_yacc\Grammar.h"


/////////////////////////////////////////////////////////////////////////////
// CTestDllDlg dialog

class CTestDllDlg : public CDialog
{
// Construction
public:
	CTestDllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDllDlg)
	enum { IDD = IDD_TESTDLL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


using namespace std;

typedef vector<string> ListOfFileNames;

void readListOfFileNames(const string& fileNameListFile,
                         ListOfFileNames& fileNameList);
void getAbsolutePath(const string& relPath, string& absPath);
void readFileContent(const string& fileName, string& buf);
void test_cvd_gen(const string& absFilePath);
void test_ops(const string& absFilePath);
void test_resource_positions(const string& absFilePath);
void test_configuration_positions(const string& absFilePath);
void test_globvarconstantlist_positions(const string& absFilePath);
void test_struct_positions(const string& absFilePath);
void test_get_var_name(const string& absFilePath);
void test_dates(const string& absFilePath);
void test_task_configuration(const string& absFilePath);

#endif // !defined(AFX_TESTDLLDLG_H__83816583_F84C_11D4_A09E_006008736ABF__INCLUDED_)
