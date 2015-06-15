
#include "ObjPropDutDecl.h"

#if !defined(AFX_MODIFYDUTMEMBER_H__70ADB8F3_F080_11D4_B8E7_002018641833__INCLUDED_)
#define AFX_MODIFYDUTMEMBER_H__70ADB8F3_F080_11D4_B8E7_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModifyDutMember : public CDialog
{
// Construction
public:
    CModifyDutMember();
    CModifyDutMember(CString strName,
                     CString strAlias,
                     CString strComment,
                     CString strType,
                     CString strInitVal);
    ~CModifyDutMember();
    void GetMemberDecl(CString& strName,
                       CString& strAlias,
                       CString& strComment,
                       CString& strType,
                       CString& strInitVal);

    void SetParent(CObjPropDutDecl* pParent);

    CObjPropDutDecl* m_pParent;

// Dialog Data
    //{{AFX_DATA(CModifyDutMember)
    enum { IDD = IDD_MODIFY_DUTMEMBER };
    CString	m_MemberAlias;
    CString	m_MemberComment;
    CString	m_MemberInitVal;
    CString	m_MemberName;
    CString	m_MemberType;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CModifyDutMember)
    public:
    virtual BOOL OnInitDialog();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CModifyDutMember)
    afx_msg void OnOk();
    afx_msg void OnCancel();
    //}}AFX_MSG
    CString m_caption;
    void SetCaption(BOOL add);

    DECLARE_MESSAGE_MAP()

private:
    void FillComboBox(int iCBId, const char * const pszItems[], int iDefault = -1);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYDUTMEMBER_H__70ADB8F3_F080_11D4_B8E7_002018641833__INCLUDED_)

