/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osTarget.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osTarget.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osTarget.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			osKernel
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

#ifndef _OSTARGET_H_
#define _OSTARGET_H_

/* osTarget.h
 * ----------------------------------------------------------------------------
 * Header file for additional, target specific definitions. This file is 
 * included after the main configuration header file (osDef.h).
 */

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif

/* Global Include Files
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/* Target specific definitions
 * ----------------------------------------------------------------------------
 */

#pragma warning (disable:4100)	/* warning C4100: 'parameter' : unreferenced formal parameter					*/
#pragma warning (disable:4115)	/* warning C4115: 'definition' : named type definition in parentheses			*/
#pragma warning (disable:4103)	/* warning C4103: 'include file' : used #pragma pack to change alignment		*/
#pragma warning (disable:4200)	/* warning C4200: nonstandard extension used : zero-sized array in struct/union */
#pragma warning (disable:4057)	/* warning C4057: 'function' : 'char *' differs in indirection to slightly different base types from 'unsigned char [522]'	*/
#pragma warning (disable:4505)	/* warning C4505: 'sysByteToHex' : unreferenced local function has been removed */

#define MS_VCC_60

#endif /* _OSTARGET_H_ */

/* ---------------------------------------------------------------------------- */
