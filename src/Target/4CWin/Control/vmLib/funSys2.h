/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funSys2.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: funSys2.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funSys2.h $
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

									EVT_GetException		,	/* 135 */
									EVT_Set 				,	/* 136 */
									NULL					,	/* 137 */
									MSG_SendToEng			,	/* 138 */
									MSG_Trace				,	/* 139 */
									NULL /* inline !*/		,	/* 140 */
									TIM_Get 				,	/* 141 */
									TSK_ClearError			,	/* 142 */
									TSK_Exception			,	/* 143 */
									TSK_GetCount			,	/* 144 */
									TSK_GetError			,	/* 145 */
									TSK_GetInfo 			,	/* 146 */
									TSK_GetMyNumber 		,	/* 147 */
									TSK_GetName 			,	/* 148 */
									TSK_GetState			,	/* 149 */
									TSK_Start				,	/* 150 */
									TSK_Stop				,	/* 151 */
									WD_Disable				,	/* 152 */
									WD_Enable				,	/* 153 */
									NULL					,	/* 154 */
									NULL					,	/* 155 */
									EVT_GetEvent			,	/* 156 */
									WD_IsEnabled			,	/* 157 */
									TSK_GetMyName			,	/* 158 */
									SYS_Coldstart			,	/* 159 */

#else	/* RTS_CFG_SYSTEM_LIB_NT */

									NULL					,	/* 135 */
									NULL					,	/* 136 */
									NULL					,	/* 137 */
									NULL					,	/* 138 */
									NULL					,	/* 139 */
									NULL					,	/* 140 */
									NULL					,	/* 141 */
									NULL					,	/* 142 */
									NULL					,	/* 143 */
									NULL					,	/* 144 */
									NULL					,	/* 145 */
									NULL					,	/* 146 */
									NULL					,	/* 147 */
									NULL					,	/* 148 */
									NULL					,	/* 149 */
									NULL					,	/* 150 */
									NULL					,	/* 151 */
									NULL					,	/* 152 */
									NULL					,	/* 153 */
									NULL					,	/* 154 */
									NULL					,	/* 155 */
									NULL					,	/* 156 */
									NULL					,	/* 157 */
									NULL					,	/* 158 */
									NULL					,	/* 159 */

#endif	/* RTS_CFG_SYSTEM_LIB_NT */

/* ---------------------------------------------------------------------------- */

