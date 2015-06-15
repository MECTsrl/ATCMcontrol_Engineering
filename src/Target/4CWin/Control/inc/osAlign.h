/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osAlign.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osAlign.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc/osAlign.h $
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

/* osAlign.h
 * ----------------------------------------------------------------------------
 * Switch compiler alignmnet to 1 byte for shared communication protocoll
 * structures and IEC function structures.
 */

#if defined(RTS_PRAGMA_PACK_1)

	/* Set compiler alignment to 1 byte
	 */
	#pragma pack(1)

#elif defined(RTS_PRAGMA_PACK_DEF)

	/* Set compiler aligment back to default
	 */
	#pragma pack()

#else

	#error	Alignment type not defined!

#endif

/* ---------------------------------------------------------------------------- */
