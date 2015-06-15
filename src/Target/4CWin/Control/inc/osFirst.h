/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osFirst.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osFirst.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osFirst.h $
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

#ifndef _OSFIRST_H_
#define _OSFIRST_H_

/* osFirst.h
 * ----------------------------------------------------------------------------
 * Adaptation header file. (This file is included before all other header 
 * files.)
 */

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif

#if defined(_BACNET_INCLUDED_)
#if WIN32|WINNT
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif
#endif

/* Adaptation specific code ... 
 */

/* Customer Libraries
 * ----------------------------------------------------------------------------
 */


/* Autotest support
 * ----------------------------------------------------------------------------
 */
#undef _SOFTING_AUTOTEST_


#endif /* _OSFIRST_H_ */

/* ---------------------------------------------------------------------------- */
