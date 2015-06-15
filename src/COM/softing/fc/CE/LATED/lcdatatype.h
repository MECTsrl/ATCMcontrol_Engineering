#ifndef __LCDATATYPE_H_
#define __LCDATATYPE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "lcconfig.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
// indices of column indices in m_aiCol
// i.e. column for name has index m_aiCol[COL_NAME]
#define COL_NAME            0
#define COL_TYPE            1
#define COL_INITVAL         2
#define COL_ALIAS           3
#define COL_COMMENT         4
#define COL_STORAGECLASS    5
#define NUM_COLS            6
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
void GetCurrentUserName(CString &);

/////////////////////////////////////////////////////////////////////////////
// CObjPropWiz dialog

class CObjPropWiz : public CPropertyPage
{
	DECLARE_DYNCREATE(CObjPropWiz)

// Construction
public:
	CObjPropWiz();
	~CObjPropWiz();

    void AddWizardName(LPCTSTR WizardName, LPCTSTR Language);
    void SetDataType(CLCDataType* pDataType)
    { m_pDataType = pDataType;}

    void SetConfig(CLCConfig* pConfig)
    { m_pConfig = pConfig;}

// Dialog Data
	//{{AFX_DATA(CObjPropWiz)
	enum { IDD = IDD_OBJPROP_WIZ };
	CListCtrl	m_Wizards;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CObjPropWiz)
	public:
	virtual BOOL OnKillActive();
	protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjPropWiz)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CString m_strWizName;
    CString m_strLanguage;
    CImageList m_ImageList;
    CLCDataType* m_pDataType;
    CLCConfig* m_pConfig;
};

/////////////////////////////////////////////////////////////////////////////
// CObjPropGen dialog

class CObjPropGen : public CPropertyPage
{
	DECLARE_DYNCREATE(CObjPropGen)
// Construction
public:
	CObjPropGen();   // standard constructor
    CObjPropGen(bool);
    ~CObjPropGen()
    {}
    void SetDataType(CLCDataType* pDataType)
    { m_pDataType = pDataType;}

    void SetConfig(CLCConfig* pConfig)
    { m_pConfig = pConfig;}
    
// Dialog Data
	//{{AFX_DATA(CObjPropGen)
	enum { IDD = IDD_OBJPROP_GEN };
	CString	m_strAlias;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjPropGen)
	public:
	virtual BOOL OnKillActive();
	protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjPropGen)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CLCDataType*    m_pDataType;
    CLCConfig*      m_pConfig;
    bool m_bProperty;
};

/////////////////////////////////////////////////////////////////////////////
// CObjPropInfo dialog

class CObjPropInfo : public CPropertyPage
{
	DECLARE_DYNCREATE(CObjPropInfo)

// Construction
public:
	CObjPropInfo();
	~CObjPropInfo();

// Dialog Data
	//{{AFX_DATA(CObjPropInfo)
	enum { IDD = IDD_OBJPROP_INFO };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CObjPropInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjPropInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CObjPropDutDecl dialog

class CObjPropDutDecl : public CPropertyPage , public /*implements*/ CLCConfigListener
{
	DECLARE_DYNCREATE(CObjPropDutDecl)

// Construction
public:
    CObjPropDutDecl();
	CObjPropDutDecl(bool,int);
	~CObjPropDutDecl();
    
    void SetDataType(CLCDataType* pDataType)
    { m_pDataType = pDataType;}

    void SetConfig(CLCConfig* pConfig)
    { m_pConfig = pConfig;}

    CLCConfig* GetConfig()
    { 
        return m_pConfig; 
    }

    void OnMember(int id, CLCDataType *, CLCMember*);
    bool OnCheckDuplicate(CString & strName);

// Dialog Data
	//{{AFX_DATA(CObjPropDutDecl)
	enum { IDD = IDD_OBJPROP_DUTDECL };
	CListCtrl	m_FbMembers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CObjPropDutDecl)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	virtual void OnCancel();
	protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjPropDutDecl)
	afx_msg void OnAddmember();
	afx_msg void OnClickMembers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoMembers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModifymember();
	afx_msg void OnDeletemember();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    void InitColumnHeaders();
    void EnableButtons();
    void SetMembers();
    void InsertMember(CLCMember*);

    int m_iColumnStrId;
	int m_iColumns;
	BOOL m_bInit;
    int m_aiCol[NUM_COLS];
    bool m_bProperty;
    CLCDataType* m_pDataType;
    CLCConfig* m_pConfig;
};

class CModifyDutMember : public CDialog , public /*implements*/ CLCConfigListener
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

    void OnIECType(int id, BL_IEC_TYP Type, CString &);

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
    CObjPropDutDecl* m_pParent;
};


/////////////////////////////////////////////////////////////////////////////
// CDialogDataType

class CDialogDataType : public CPropertySheet
{
	DECLARE_DYNAMIC(CDialogDataType)

// Construction
public:
    CDialogDataType(CLCConfig* pConfig, 
                    CLCDataType* pDataType, 
                    UINT nIDCaption, 
                    CWnd* pParentWnd, UINT iSelectPage, 
                    bool bProperty, int iColumnStrId);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDataType)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDialogDataType();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialogDataType)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//members
    CObjPropWiz         m_WizPage;
    CObjPropGen         m_GenPage;
    CObjPropDutDecl     m_DeclPage;
    bool                m_bProperty;
//methods
    void InitPages(CLCDataType*,CLCConfig*);
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __LCDATATYPE_H_