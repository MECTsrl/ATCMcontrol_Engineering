/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funSys.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: funSys.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funSys.h $
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

#if defined(RTS_CFG_SYSTEM_LIB)

									ClearLocalTaskErrno 	,	/*120*/
									ClearTaskErrno			,	/*121*/
									NULL					,	/*122*/
									GetLocalTaskErrno		,	/*123*/
									GetLocalTaskInfo		,	/*124*/
									GetLocalTaskState		,	/*125*/
									GetTaskErrno			,	/*126*/
									GetTimeSinceSystemBoot	,	/*127*/
									OutputDebugString		,	/*128*/
									StartLocalTask			,	/*129*/
									StopLocalTask			,	/*130*/
									ThrowException			,	/*131*/
									NULL, 						/*132*/
									NULL, 						/*133*/
									NULL, 						/*134*/

#else	/* RTS_CFG_SYSTEM_LIB */

									NULL, 						/*120*/
									NULL, 						/*121*/
									NULL, 						/*122*/
									NULL, 						/*123*/
									NULL, 						/*124*/
									NULL, 						/*125*/
									NULL, 						/*126*/
									NULL, 						/*127*/
									NULL, 						/*128*/
									NULL, 						/*129*/
									NULL, 						/*130*/
									NULL, 						/*131*/
									NULL, 						/*132*/
									NULL, 						/*133*/
									NULL, 						/*134*/

#endif	/* RTS_CFG_SYSTEM_LIB */

/* ---------------------------------------------------------------------------- */

