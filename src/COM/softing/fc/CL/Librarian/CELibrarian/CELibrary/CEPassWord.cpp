/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CEPassWord.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CEPassWord.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CEPassWord.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
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
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CEPassWord.h"
//----  Static Initializations:   ----------------------------------*


int CEPassWord::key[] = { 3, 29, 5, 23, 7, 19, 11, 17, 13, 31 };

int CEPassWord::Encode(const CString& passWord, CString& encPassWord)
{	
	encPassWord=""; 
	//
	// password must not contain ' '
	//

	if ( passWord == "" )
	{
		encPassWord = "";
		return 1;
	}

	//
	// make password longer
	//
	CString pass=passWord;
	pass = pass + " ";
	while ( pass.GetLength() < CEL_MAX_PWLEN)
	{
		pass = pass + passWord;
	}
	pass=pass.Left(CEL_MAX_PWLEN);

	//
	// code password
	//
	for (int ii=0;ii<CEL_MAX_PWLEN;ii++)
	{
		int cc = (key[ii] + (((int) pass[ii]) - 32) ) % 95;
		cc=cc+32;
		encPassWord=encPassWord + (TCHAR) cc;
	}

	return 1; 
}

/*
 *----------------------------------------------------------------------------*
 *  $History: CEPassWord.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/
