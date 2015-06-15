/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/AsmParse.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AsmParse.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/AsmParse.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _ASMPARSE_H_
#define _ASMPARSE_H_

class FileContext_C;
//call yacc:
bool ParseFile(FileContext_C* pfc);

#endif

/* ---------------------------------------------------------------------------- */
