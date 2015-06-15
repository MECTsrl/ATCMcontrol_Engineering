/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/helper.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: helper.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/helper.h $
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

#ifndef _HELPER_H_
#define _HELPER_H_

#include <tchar.h>

#include "cgt_misc.h"
#include "defines.h"
#include "parseKAD.h"

//defined in 4cpc.cpp:
const TCHAR* GetModuleName();


void SetFeatures(ICG_IECFront* pIFront, const CKADInfo* pKAD, LPCTSTR trg);

void DumpResponseFile(
    CGT_CDiagMsg*              pdm, 
    const TCHAR*               pszTrgType,
    ICG_IECFront*              pIFront,
    const CG_POU4CPInfo*const* pzpProcessed
);
bool GetTaskInitVal(const CG_Task* pTask, LPCTSTR attr, __int64* pVal);
bool GetTaskInitLit(const CG_Task* pTask, LPCTSTR attr, const CG_Literal** pLit);


//TODO make a core feature of sysgen:
void LCT_RecoredHit(const char* pszFile, long lineNo);
void LCT_DumpResult(const TCHAR* pszHint);
//TODO better hash for pszFeat better than __FILE__, __LINE__ !!
#define LCT(pszFeat)
//#define LCT(pszFeat)  LCT_RecoredHit(__FILE__, __LINE__)



bool CheckGlobalFBLocalRetainRec(CGT_CDiagMsg*    pdm, const CG_Symtab* pSym, bool bInFB);
bool CheckGlobalFBLocalRetain(CGT_CDiagMsg*    pdm, const CG_Symtab* pSym);

#endif

/* ---------------------------------------------------------------------------- */
