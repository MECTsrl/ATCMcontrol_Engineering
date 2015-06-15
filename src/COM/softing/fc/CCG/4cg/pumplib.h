
#ifndef __PUMPLIB_H_
#define __PUMPLIB_H_


//----  Aggregate Includes:   --------------------------------------*
#include "cgt_CDiagMsg.h"
#include "helper.h"

//----  Forward Class Definitions:   -------------------------------*
class CMDL_OPTIONSTYP;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



extern "C" void MakeLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const FC_CGArray<TargetAndType>* pTargets,
    const CMDL_OPTIONSTYP*           pOpts
);
extern "C" void MakeLibraryStub(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpts
);
extern "C" void ListLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpts
);
extern "C" void ExtractLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpt
);
extern "C" void RemoveLibTarget(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpts
);
extern "C" void AppendLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpts
);

#endif // __PUMPLIB_H_


