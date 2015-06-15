#ifndef __HELPER_H_
#define __HELPER_H_

#include <tchar.h>
#include "defines.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_system.h"
#include "CG_IECFront.h"
#include "cgt_CDiagMsg.h"

class CMDL_OPTIONSTYP;


const TCHAR* GetModuleName();


extern "C" struct TargetAndType
{
    const TCHAR*  pszTrg;
    int           iType;
};
extern "C" void GetAllTargets(
    CGT_CDiagMsg*               pdm,
    ICG_IECFront*               pIFront,
    FC_CGArray<TargetAndType>*  pTargets,
    const TCHAR*                pszTrgTypeOnly
);

extern "C" void CleanUpGenDir(CGT_CDiagMsg* pdm, ICG_IECFront* pIFront);

bool FC_GetFileVersion(
    const TCHAR* pszBackEnd,
    TCHAR        szFileVer[/*dwBytes*/],
    unsigned int dwBytes
);

extern "C" void ExecCustomBuildSteps(
    CGT_CDiagMsg*        pdm,
    ICG_IECFront*        pIFront,
    int                  nVerbose,
    bool                 bPre //else post
);

extern "C" void Check4CPNeedVersion(
    CGT_CDiagMsg*  pdm,
    ICG_IECFront*  pIFront
);
extern "C" void Check4CPCompilerSec(
    CGT_CDiagMsg*    pdm,
    ICG_IECFront*    pIFront,
    CMDL_OPTIONSTYP* pOpt
);

extern "C" void AutoBuildLibs(
    CGT_CDiagMsg*    pdm,
    ICG_IECFront*    pIFront,
    CMDL_OPTIONSTYP* pOpt
);

extern "C" void CheckFile(
    CGT_CDiagMsg*          pdm,
    ICG_IECFront*          pIFront,
    const CMDL_OPTIONSTYP* pOpts
);

extern "C" void BuildTargets(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const FC_CGArray<TargetAndType>& targets,
    const CMDL_OPTIONSTYP*           pOpts
);


bool CallBackend(
    CGT_CDiagMsg* pdm,
    int           nVerbose,
    const TCHAR*  pszExePath,
    const TCHAR*  pszCmdArgs,
    const TCHAR*  pszGenPath,
    const TCHAR*  pszTrgType
);




#endif // __HELPER_H_


