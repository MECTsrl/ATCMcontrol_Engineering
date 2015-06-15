
#if !defined(AFX_OBJPROPFBMDECL_H__E392A685_CF67_11D4_9623_00A024399A66__INCLUDED_)
#define AFX_OBJPROPFBMDECL_H__E392A685_CF67_11D4_9623_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjPropFbmDecl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjPropFbmDecl dialog

class CObjPropFbmDecl : public CPropertyPage
{
// Construction
public:
	CObjPropFbmDecl();
	~CObjPropFbmDecl();

// Dialog Data
	//{{AFX_DATA(CObjPropFbmDecl)
	enum { IDD = IDD_OBJPROP_FBMDECL };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CObjPropFbmDecl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjPropFbmDecl)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJPROPFBMDECL_H__E392A685_CF67_11D4_9623_00A024399A66__INCLUDED_)
