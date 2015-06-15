
#include "ObjPropDeclBase.h"

#if !defined(AFX_MODIFYFBMEMBER_H__E392A688_CF67_11D4_9623_00A024399A66__INCLUDED_)
#define AFX_MODIFYFBMEMBER_H__E392A688_CF67_11D4_9623_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModifyFbMember : public CDialog
{
// Construction
public:
	CModifyFbMember();
    CModifyFbMember(LPCTSTR pszSClasses[]);
	CModifyFbMember(LPCTSTR pszSClasses[],
                    CString strName,
                    CString strAlias,
                    CString strComment,
                    CString strType,
                    CString strInitVal,
                    CString strSclass);
	~CModifyFbMember();
	void GetMemberDecl(CString& strName,
                       CString& strAlias,
                       CString& strComment,
                       CString& strType,
                       CString& strInitVal,
                       CString& strSClass);
    void SetParent(CObjPropDeclBase* pParent);

    CObjPropDeclBase* m_pParent;

// Dialog Data
	//{{AFX_DATA(CModifyFbMember)
	enum { IDD = IDD_MODIFY_FBMEMBER };
	CButton	m_Ok;
	CButton	m_Cancel;
	CString	m_FbMemberInitVal;
	CString	m_FbMemberName;
	CString	m_FbMemberType;
	int		m_FbMemberSclass;
	CString	m_FbMemberAlias;
	CString	m_FbMemberComment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CModifyFbMember)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CModifyFbMember)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	//}}AFX_MSG
    CString m_caption;
    void SetCaption(BOOL add);

	DECLARE_MESSAGE_MAP()

private:
	const LPCTSTR * m_pszSClasses;
	void FillComboBox(int iCBId, const char * const pszItems[], int iDefault = -1, BOOL bIsRetainSupp = TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYFBMEMBER_H__E392A688_CF67_11D4_9623_00A024399A66__INCLUDED_)

