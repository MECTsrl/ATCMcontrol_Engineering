
#ifndef __PRJDBI_H_
#define __PRJDBI_H_

//----  Aggregate Includes:   --------------------------------------*
#include "helper.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


extern "C" void SortTargetsByName(FC_CGArray<TargetAndType>*  pTargets);    // 23.08.06 SIS

extern "C" void DumpProjectDBI(
    CGT_CDiagMsg*               pdm,
    ICG_IECFront*               pIFront,
    FC_CGArray<TargetAndType>*  pTargets
);

#endif // __PRJDBI_H_


