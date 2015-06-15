/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/FormatException.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: FormatException.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/FormatException.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"

#include "FormatException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CFormatException::CFormatException(const char* comment):CException(0) 
{
	strcpy(m_message, comment);
}

CFormatException::CFormatException(const CFormatException& e):CException(0) 
{
	strcpy(m_message, e.m_message);
}

BOOL CFormatException::GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	strncpy(lpszError, m_message, nMaxError-1);
	lpszError[nMaxError-1] = 0;
	return 1;
}

/* ---------------------------------------------------------------------------- */
