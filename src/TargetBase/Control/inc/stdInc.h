/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/stdInc.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: stdInc.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/stdInc.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			vmKernel
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

#ifndef _STDINC_H_
#define _STDINC_H_

/* This file must be included before all other include files in the VM. The 
 * following header files must be included in the the given order!
 */

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

/* Included in the run time system
 */
#define INC_RTS

#include "BuildNr.h"

#ifndef _OSFIRST_H_
#include "osFirst.h"
#endif

#ifndef _OSDEF_H_
#include "osDef.h"
#endif

#ifndef _OSTARGET_H_
#include "osTarget.h"
#endif

#ifndef _VMSHARED_H_
#include "vmShared.h"
#endif

#ifndef _VMMDEF_H_
#include "vmmDef.h"
#endif

#ifndef _INTDEF_H_
#include "intDef.h"
#endif

#ifndef _LIBDEF_H_
#include "libDef.h"
#endif

#ifndef _VMMMAIN_H_
#include "vmmMain.h"
#endif

#ifndef _OSLAST_H_
#include "osLast.h"
#endif

#endif	/* _STDINC_H_ */

/* ---------------------------------------------------------------------------- */
