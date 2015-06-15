/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/OnlServError.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: OnlServError.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/OnlServError.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
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

#ifndef __ONLSERVERROR_H_
#define __ONLSERVERROR_H_

#include "Connection.h"

bool GetCommErrorMessage(BSTR * psMessage, LPBYTE lpData, UINT cbData,...);
bool GetCommErrorMessage(BSTR * psMessage, IEC_UINT uErrNo,...);

#endif

/* ---------------------------------------------------------------------------- */
