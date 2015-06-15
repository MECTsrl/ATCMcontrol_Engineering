/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/visLib/visAdapt.c 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: visAdapt.c $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/visLib/visAdapt.c $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			visLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:10 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Local Defines:   ----------------------------------------------------- */

#define __4CFILE__	"visAdapt.c"

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdInc.h"

/* ----  Target Specific Includes:	 ------------------------------------------ */
#include "osSocket.h"

/* ----  Local Defines:   ----------------------------------------------------- */

/* ----  Global Variables:	 -------------------------------------------------- */

/* ----  Local Functions:	--------------------------------------------------- */

/* ----  Implementations:	--------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * domVisuCommInit	
 *
 */
IEC_UINT domVisuCommInit(void)
{
	static IEC_BOOL bOnce = FALSE;
	
	if (bOnce == FALSE)
	{
		WSADATA WSAData;

		WSAData.wVersion		= MAKEWORD(2,0);
		WSAData.wHighVersion	= 2;
		WSAData.iMaxSockets 	= 10;
		WSAData.iMaxUdpDg		= 0;
		WSAData.lpVendorInfo	= NULL;

		if (WSAStartup(MAKEWORD(2,0), &WSAData) != 0)
		{
			TR_ERROR("[TCP] ERROR(%d): WSAStartup failed.\r\n", VMS_ERRNO);
			RETURN(ERR_CREATE_TASK);
		}

		bOnce = TRUE;
	}
	
	RETURN(OK);
}


/* ---------------------------------------------------------------------------- */
