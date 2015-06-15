/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/StdAfx.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: StdAfx.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/StdAfx.h $
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


// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__09A4E0C0_F13D_11D4_B8EA_002018641833__INCLUDED_)
#define AFX_STDAFX_H__09A4E0C0_F13D_11D4_B8EA_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#endif // _AFX_NO_OLE_SUPPORT


#define FCINST_API extern "C" __declspec(dllexport)




// ---->>>>>>> Debugging in msi
// normally no debug step boxes will be displayed
// to be able to see what the dll does in custom actions
// comment the line #define MB(a,b)
// and uncomment the other #define line.

//#ifdef DEBUG

   #define MB(a,b) 

//#else

//   #define MB(a,b) ::AfxMessageBox(a,b)

//#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__09A4E0C0_F13D_11D4_B8EA_002018641833__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 *  $History: StdAfx.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 5  *****************
 * User: Jd           Date: 28.11.01   Time: 10:21
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * Correct console service starting and stopping
 * 
 * *****************  Version 4  *****************
 * User: Jd           Date: 20.11.01   Time: 16:13
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 15.11.01   Time: 14:20
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * Check if mfc.dlls have been copied, or a reboot is necessary.
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 2.03.01    Time: 12:26
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added sourcesafe header
 *==
 *----------------------------------------------------------------------------*
*/