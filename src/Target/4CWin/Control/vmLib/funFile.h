/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funFile.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: funFile.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/vmLib/funFile.h $
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

#if defined(RTS_CFG_FILE_LIB)

									fa_sync_openFile		,	/* 160 */
									fa_sync_readFile		,	/* 161 */
									fa_sync_renameFile		,	/* 162 */
									fa_sync_writeFile		,	/* 163 */
									fa_sync_closeFile		,	/* 164 */
									fa_sync_createDirectory ,	/* 165 */
									fa_sync_deleteFile		,	/* 166 */
									fa_sync_existsFile		,	/* 167 */
									fa_sync_getSize 		,	/* 168 */
									fa_get_diskFreeSpace	,	/* 169 */
									fa_error_string 		,	/* 170 */
									fa_sync_arrayReadFile	,	/* 171 */
									fa_sync_arrayWriteFile	,	/* 172 */
									fa_flush				,	/* 173 */
									NULL					,	/* 174 */
									NULL					,	/* 175 */
									NULL					,	/* 176 */
									NULL					,	/* 177 */
									NULL					,	/* 178 */
									NULL					,	/* 179 */

#else	/* RTS_CFG_FILE_LIB */

									NULL					,	/* 160 */
									NULL					,	/* 161 */
									NULL					,	/* 162 */
									NULL					,	/* 163 */
									NULL					,	/* 164 */
									NULL					,	/* 165 */
									NULL					,	/* 166 */
									NULL					,	/* 167 */
									NULL					,	/* 168 */
									NULL					,	/* 169 */
									NULL					,	/* 170 */
									NULL					,	/* 171 */
									NULL					,	/* 172 */
									NULL					,	/* 173 */
									NULL					,	/* 174 */
									NULL					,	/* 175 */
									NULL					,	/* 176 */
									NULL					,	/* 177 */
									NULL					,	/* 178 */
									NULL					,	/* 179 */


#endif	/* RTS_CFG_FILE_LIB */

/* ---------------------------------------------------------------------------- */

