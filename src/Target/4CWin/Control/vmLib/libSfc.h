/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/libSfc.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: libSfc.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/libSfc.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			vmLib
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

#ifndef _INTSFC_H_
#define _INTSFC_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if defined(RTS_CFG_SFC)

/* Don't use! Just to replace former OS_BOOL (16 bit) until it is safe
 * to replace it with IEC_BOOL (8 bit).
 */
#define SFC_BOOL IEC_INT


/* Function Declarations
 * ----------------------------------------------------------------------------
 */

void sfc_calcdotrans (STDLIBFUNCALL);		 /* ordinal 72 */
void sfc_calcdoact	 (STDLIBFUNCALL);		 /* ordinal 73 */
void sfc_finalise	 (STDLIBFUNCALL);		 /* ordinal 74 */

#endif	/* RTS_CFG_SFC */

#endif	/* _INTSFC_H_ */

/* ---------------------------------------------------------------------------- */

