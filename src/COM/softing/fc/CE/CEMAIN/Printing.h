#if !defined(AFX_PRINTING_H__67FCDDB3_4178_461B_B4C7_067BF70D5C7C__INCLUDED_)
#define AFX_PRINTING_H__67FCDDB3_4178_461B_B4C7_067BF70D5C7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPrintUserData
{
    protected:

    int     m_iPageNum;
    CString m_strObjName;
    CString m_strProjectName;
    CString m_strProjectPath;
    int     m_iPageCount;

    public:

    CPrintUserData()
    {
        m_iPageNum = 0;
        m_iPageCount = -1;
    }

	virtual ~CPrintUserData()
    {
    }

    int GetPageNum() 
    {
        return m_iPageNum;
    }

    int IncrementPageNum() 
    {
        return ++m_iPageNum;
    }

    void ResetPageNum() 
    {
        m_iPageNum = 0;
    }

    int GetPageCount()
    {
        return m_iPageCount;
    }

    void SetPageCount(int iPageCount)
    {
        m_iPageCount = iPageCount;
    }

    LPCTSTR GetObjectName() 
    {
        return m_strObjName;
    }

    void SetObjectName(LPCTSTR pszObjName) 
    {
        m_strObjName = pszObjName;
    }

    LPCTSTR GetProjectName() 
    {
        return m_strProjectName;
    }

    void SetProjectName(LPCTSTR pszProjectName) 
    {
        m_strProjectName = pszProjectName;
    }

    LPCTSTR GetProjectPath() 
    {
        return m_strProjectPath;
    }

    void SetProjectPath(LPCTSTR pszProjectPath) 
    {
        m_strProjectPath = pszProjectPath;
    }
};


class CPrjPrintDialog : public CDialog
{
public:
	//{{AFX_DATA(CPrjPrintDialog)
	enum { IDD = AFX_IDD_PRINTDLG };
	//}}AFX_DATA

	
	CPrjPrintDialog::CPrjPrintDialog(CWnd* pParent);
		
	virtual ~CPrjPrintDialog() { }

	virtual BOOL OnInitDialog();
	virtual void OnCancel();

protected:
#ifdef _MAC
	CWnd*   m_pView;        // the view being printed

	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
#endif

#ifdef _MAC
	//{{AFX_MSG(CPrjPrintDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#endif
};

BOOL GetPrintInfo(CString &strPrinterName, long* plWidth, long* plHeight);

BOOL CALLBACK PrjPrintAbortProc(HDC, int);


#endif // !defined(AFX_PRINTING_H__67FCDDB3_4178_461B_B4C7_067BF70D5C7C__INCLUDED_)
