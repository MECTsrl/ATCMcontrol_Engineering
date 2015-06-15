/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Setup.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/Setup.h $
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
 */

#if !defined(AFX_SETUP4CE_H__CF433CA3_FEF3_4BB7_A7C3_4B018987D235__INCLUDED_)
#define AFX_SETUP4CE_H__CF433CA3_FEF3_4BB7_A7C3_4B018987D235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSetupApp:
// See SetUp4Ce.cpp for the implementation of this class
//

class CSetupApp : public CWinApp
{
public:
	CSetupApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSetupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUP4CE_H__CF433CA3_FEF3_4BB7_A7C3_4B018987D235__INCLUDED_)

/* ---------------------------------------------------------------------------- */
