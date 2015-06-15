/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/fbMBus2.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: fbMBus2.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/fbMBus2.h $
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

#if defined(RTS_CFG_MBUS2_LIB)

									fb_mbus2_meter_init			,	/* 30 */
									fb_mbus2_meter_list_slaves	,	/* 31 */
									fb_mbus2_meter_read			,	/* 32 */
									NULL						,	/* 33 */
									NULL						,	/* 34 */
									NULL						,	/* 35 */
									NULL						,	/* 36 */
									NULL						,	/* 37 */
									NULL						,	/* 38 */
									NULL						,	/* 39 */

#else	/* RTS_CFG_MBUS2_LIB */

									NULL						,	/* 30 */
									NULL						,	/* 31 */
									NULL						,	/* 32 */
									NULL						,	/* 33 */
									NULL						,	/* 34 */
									NULL						,	/* 35 */
									NULL						,	/* 36 */
									NULL						,	/* 37 */
									NULL						,	/* 38 */
									NULL						,	/* 39 */

#endif	/* RTS_CFG_MBUS2_LIB */

/* ---------------------------------------------------------------------------- */

