

#ifndef __HELPER_H_
#define __HELPER_H_

#include <tchar.h>
#include "cgt_misc.h"
#include "parse4ch.h"


const CG_BITSET SUPPORTED_BASIC_TYPES = 
    CG_TBS_ANY_BIT|CG_TBS_ANY_DATE|CG_TBS_TIME|
    CG_TBS_LREAL|CG_TBS_WSTRING|CG_TBS_DINT|CG_TBS_LINT|
    CG_TBS_QVT_BOOL|CG_TBS_QVT_BYTE|CG_TBS_QVT_WORD |CG_TBS_QVT_DWORD|CG_TBS_QVT_LWORD|
    CG_TBS_QVT_DINT|CG_TBS_QVT_LINT|CG_TBS_QVT_LREAL|CG_TBS_QVT_WSTRING; 
                             


//defined in 4cpc.cpp:
const TCHAR* GetModuleName();
const TCHAR* Get4CCopyRightSourceHeader();
const TCHAR* GetGeneratorVersionString();

bool checkTaskSettings(
    CGT_CDiagMsg*              pdm, 
	ICG_IECFront*			   pIF,
	const CG_Resource*		   pRes, 
	const CTarget*			   pTrg
);

void SetFeatures(ICG_IECFront* pIFront);


void DumpResponseFile(
    CGT_CDiagMsg*              pdm, 
    const TCHAR*               pszTrgType,
    ICG_IECFront*              pIFront,
    const CG_POU4CPInfo*const* pzpProcessed
);

void ExecuteJvc(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszOpt,
    const TCHAR*  pszJavaFiles,
    int           nVerbose
);


enum CG_OS_TYPES
{
	E_WIN_UNKNOWN,
	E_WIN_98_95_ME,
	E_WIN_NT35,
	E_WIN_NT40,
	E_WIN_2000,
	E_WIN_XP,
	E_WIN_2003_SERVER,
	E_WIN_VISTA,
};

CG_OS_TYPES CG_GetOSVersion();


#endif // __HELPER_H_

