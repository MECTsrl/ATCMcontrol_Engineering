/* $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropBase.h 1     28.02.07 19:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PropBase.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/PropBase.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_PROPBASE_H__C1CA3D63_9361_4113_B687_81D01589A2B7__INCLUDED_)
#define AFX_PROPBASE_H__C1CA3D63_9361_4113_B687_81D01589A2B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropBase.h : header file
//

class CPropSheet;
class CInstall;

/////////////////////////////////////////////////////////////////////////////
// CPropBase dialog

class CPropBase : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropBase)

// Construction
public:
	CPropBase(UINT nIDTemplate, UINT nIDCaption = 0);
	~CPropBase();

// Dialog Data
	//{{AFX_DATA(CPropBase)
	enum { IDD = IDD_PP_DUMMY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropBase)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropBase)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	protected:
		static BOOL m_bInitialized;

	public:
		CPropSheet *GetPropSheet();
		CInstall   *GetInstall();

		void EnableClose(BOOL bEnable);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPBASE_H__C1CA3D63_9361_4113_B687_81D01589A2B7__INCLUDED_)

/* ---------------------------------------------------------------------------- */
