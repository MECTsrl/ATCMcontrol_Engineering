/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funSys22.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: funSys22.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funSys22.h $
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

#if defined(RTS_CFG_SYSTEM_LIB_NT)

									SYS_Reboot				,	/* 180 */
									SYS_RetainWrite 		,	/* 181 */
									SYS_Warmstart			,	/* 182 */
									LD_Get					,	/* 183 */
									WD_SetTrigger			,	/* 184 */
									WD_GetTrigger			,	/* 185 */
									TSK_GetStatistic		,	/* 186 */
									TSK_ClearStatistic		,	/* 187 */
									NULL					,	/* 188 */
									NULL					,	/* 189 */
									NULL					,	/* 190 */
									NULL					,	/* 191 */
									NULL					,	/* 192 */
									NULL					,	/* 193 */
									NULL					,	/* 194 */
									NULL					,	/* 195 */
									NULL					,	/* 196 */
									NULL					,	/* 197 */
									NULL					,	/* 198 */
									NULL					,	/* 199 */
									
#else	/* RTS_CFG_SYSTEM_LIB_NT */

									NULL					,	/* 180 */
									NULL					,	/* 181 */
									NULL					,	/* 182 */
									NULL					,	/* 183 */
									NULL					,	/* 184 */
									NULL					,	/* 185 */
									NULL					,	/* 186 */
									NULL					,	/* 187 */
									NULL					,	/* 188 */
									NULL					,	/* 189 */
									NULL					,	/* 190 */
									NULL					,	/* 191 */
									NULL					,	/* 192 */
									NULL					,	/* 193 */
									NULL					,	/* 194 */
									NULL					,	/* 195 */
									NULL					,	/* 196 */
									NULL					,	/* 197 */
									NULL					,	/* 198 */
									NULL					,	/* 199 */

#endif	/* RTS_CFG_SYSTEM_LIB_NT */

/* ---------------------------------------------------------------------------- */

