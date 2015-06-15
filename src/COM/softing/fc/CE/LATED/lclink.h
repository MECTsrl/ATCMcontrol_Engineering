#ifndef __LCLINK_H_
#define __LCLINK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "lcconfig.h"
//----  Forward Class Definitions:   -------------------------------*
class CDialogTarget;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CObjPropTarget dialog

class CObjPropTarget : public CPropertyPage, public /*implements*/ CLCConfigListener
{
	DECLARE_DYNCREATE(CObjPropTarget)

// Construction
public:
    enum {
        Col_Name = 0,
        Col_Type,
        Col_IP,
        Col_Resource
    };

	CObjPropTarget();
	~CObjPropTarget();

    void SetParent(CDialogTarget* pParent)
    { m_pParent = pParent; }

    void OnTarget(int id, CTargetInfo & info);

// Dialog Data
	//{{AFX_DATA(CObjPropTarget)
	enum { IDD = IDD_OBJPROP_TARGET };
	CListCtrl	m_TargetList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CObjPropTarget)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual void OnCancel();
	virtual void OnOK();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjPropTarget)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTargets(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    void GetTargets();
    void InitColumnHeaders();
    void InsertItem(CTargetInfo & info);
    void InsertPropertyLink();
    void Cleanup();
    bool m_bInit;
    int  m_iColumns;
    int  m_iSel;
    CString m_strInvalid;
    CString m_strInvalidHost;
    CDialogTarget* m_pParent;
};

/////////////////////////////////////////////////////////////////////////////
// CDialogTarget

class CDialogTarget : public CPropertySheet
{
	DECLARE_DYNAMIC(CDialogTarget)

// Construction
public:
	CDialogTarget(CLCConfig*, 
                  CLCLink*, 
                  CLCDestLink*,
                  bool bProperty,
                  UINT nIDCaption, 
                  CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
    CLCConfig* GetConfig()
    { 
        return m_pConfig;
    }

    CLCLink* GetLink()
    {
        return m_pLink;
    }

    CLCDestLink* GetDestLink()
    {
        return m_pDestLink;
    }

    bool IsDestLink()
    { return m_pDestLink != NULL; }

    bool IsProperty()
    { return m_bProperty; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTarget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDialogTarget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialogTarget)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    void InitPages();
    bool            m_bProperty;
    CLCConfig*      m_pConfig;
    CLCLink*        m_pLink;
    CLCDestLink*    m_pDestLink;
    CObjPropTarget  m_TargetDecl;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CDlgAssignType dialog

class CDlgAssignType : public CDialog , public /*implements*/ CLCConfigListener
{
// Construction
public:
	CDlgAssignType(CLCConfig*, CLCLink*, CWnd* pParent = NULL);   // standard constructor
    void OnDataType(int id, CLCDataType *);

// Dialog Data
	//{{AFX_DATA(CDlgAssignType)
	enum { IDD = IDD_ASSIGN_DATATYPE };
	CListBox	m_lb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAssignType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAssignType)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDClkDatatypeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    bool DoOK();
    CLCConfig*  m_pConfig;
    CLCLink*    m_pLink;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __LCLINK_H_

