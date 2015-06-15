/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmergeDLL.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: KADmergeDLL.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmergeDLL.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  01.03.2000  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

// KADmergeDLL.h : main header file for the KADMERGEDLL DLL
//

#if !defined(AFX_KADMERGEDLL_H__DD626796_1244_11D5_B90E_002018641833__INCLUDED_)
#define AFX_KADMERGEDLL_H__DD626796_1244_11D5_B90E_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKADmergeDLLApp
// See KADmergeDLL.cpp for the implementation of this class
//

class CKADmergeDLLApp : public CWinApp
{
public:
	CKADmergeDLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKADmergeDLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CKADmergeDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMERGEDLL_H__DD626796_1244_11D5_B90E_002018641833__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 *  $History: KADmergeDLL.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 15.03.01   Time: 14:16
 * Created in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * New Project
 * 
 *----------------------------------------------------------------------------*
*/
