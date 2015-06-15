/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/4C_Inst3.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: 4C_Inst3.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/4C_Inst3.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
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


// 4C_Inst3.h : main header file for the 4C_Inst3 DLL
//

#if !defined(AFX_4C_INST3_H__09A4E0BE_F13D_11D4_B8EA_002018641833__INCLUDED_)
#define AFX_4C_INST3_H__09A4E0BE_F13D_11D4_B8EA_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy4C_Inst3App
// See 4C_Inst3.cpp for the implementation of this class
//

class CMy4C_Inst3App : public CWinApp
{
public:
	CMy4C_Inst3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy4C_Inst3App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMy4C_Inst3App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_4C_INST3_H__09A4E0BE_F13D_11D4_B8EA_002018641833__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 *  $History: 4C_Inst3.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 2.03.01    Time: 12:25
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added sourcesafe header
 *==
 *----------------------------------------------------------------------------*
*/