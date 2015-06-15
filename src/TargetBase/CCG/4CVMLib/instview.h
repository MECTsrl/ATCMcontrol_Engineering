/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/instview.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: instview.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/instview.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
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

#ifndef _INSTVIEW_H_
#define _INSTVIEW_H_

#include "cgt_CDiagMsg.h"

extern "C" long DumpInstView(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //front end IF
    const TCHAR*       pszTrgTyp, //target type
    const TCHAR*       pszTrgId,
    const TCHAR*       pszTrgAddr,
    const CG_Resource* pRes     //called resource
);

#endif

/* ---------------------------------------------------------------------------- */
