/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTest.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlTest.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/OnlTest.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_ONLCOMMTEST_H__D51BA315_8D56_4F64_9BCE_3C9567AB85FB__INCLUDED_)
#define AFX_ONLCOMMTEST_H__D51BA315_8D56_4F64_9BCE_3C9567AB85FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COnlTestApp:
// See OnlTest.cpp for the implementation of this class
//

class COnlTestApp : public CWinApp
{
public:
	COnlTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnlTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COnlTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLCOMMTEST_H__D51BA315_8D56_4F64_9BCE_3C9567AB85FB__INCLUDED_)

/* ---------------------------------------------------------------------------- */
