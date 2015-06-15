/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/fbIec.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: fbIec.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/fbIec.h $
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

#if defined(IP_CFG_BOOL)
									rs, 				/*0*/	/*RS_TYP*/
#else
									NULL, 				/*0*/
#endif
#if defined(IP_CFG_BOOL)
									sr, 				/*1*/	/*SR_TYP*/
#else
									NULL, 				/*1*/
#endif
#if defined(IP_CFG_BOOL) && defined(IP_CFG_TIME)
									tp, 				/*2*/	/*TP_TON_TOF_PAR*/
#else
									NULL, 				/*2*/
#endif
#if defined(IP_CFG_BOOL) && defined(IP_CFG_TIME)
									ton,				/*3*/	/*TP_TON_TOF_PAR*/
#else
									NULL, 				/*3*/
#endif
#if defined(IP_CFG_BOOL) && defined(IP_CFG_TIME)
									tof,				/*4*/	/*TP_TON_TOF_PAR*/
#else
									NULL, 				/*4*/
#endif
#if defined(IP_CFG_BYTE) && defined(IP_CFG_INT)
									ctu,				/*5*/	/*CTU_PAR*/
#else
									NULL, 				/*5*/
#endif
#if defined(IP_CFG_BYTE) && defined(IP_CFG_INT)
									ctd,				/*6*/	/*CTD_PAR*/
#else
									NULL, 				/*6*/
#endif
#if defined(IP_CFG_BYTE) && defined(IP_CFG_INT)
									ctud,				/*7*/	/*CTUD_PAR*/
#else
									NULL, 				/*7*/
#endif
#if defined(IP_CFG_BYTE)
									r_trig, 			/*8*/	/*TRIG_PAR*/
#else
									NULL, 				/*8*/
#endif
#if defined(IP_CFG_BYTE) && defined(IP_CFG_INT)
									f_trig, 			 /*9*/	 /*TRIG_PAR*/
#else
									NULL, 				 /*9*/
#endif
									NULL, 				/*10*/
									NULL, 				/*11*/
									NULL, 				/*12*/
									NULL, 				/*13*/
									NULL, 				/*14*/
									NULL, 				/*15*/
									NULL, 				/*16*/
									NULL, 				/*17*/
									NULL, 				/*18*/
									NULL, 				/*19*/

/* ---------------------------------------------------------------------------- */

