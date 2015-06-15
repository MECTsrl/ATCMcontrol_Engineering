/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/FormatException.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: FormatException.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/FormatException.h $
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

#if !defined(AFX_FORMATEXCEPTION_H__CEBEBAC4_2A64_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_FORMATEXCEPTION_H__CEBEBAC4_2A64_11D5_A05C_0050BF490551__INCLUDED_

class CFormatException : public CException  
{
protected:
	char		m_message[128];
public:

	CFormatException() {}
	CFormatException(const char* comment);
	CFormatException(const CFormatException&);
	virtual BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL );
};

#endif

/* ---------------------------------------------------------------------------- */
