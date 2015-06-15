/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/4C_Inst3.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: 4C_Inst3.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/4C_Inst3.cpp $
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


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


// 4C_Inst3.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include "4C_Inst3.h"
#include "InstCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LONG g_Dummy;

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CMy4C_Inst3App

BEGIN_MESSAGE_MAP(CMy4C_Inst3App, CWinApp)
	//{{AFX_MSG_MAP(CMy4C_Inst3App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4C_Inst3App construction

CMy4C_Inst3App::CMy4C_Inst3App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMy4C_Inst3App object

CMy4C_Inst3App theApp;


/*
 *----------------------------------------------------------------------------*
 *  $History: 4C_Inst3.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 20.11.01   Time: 16:13
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 2.03.01    Time: 12:25
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added sourcesafe header
 *==
 *----------------------------------------------------------------------------*
*/