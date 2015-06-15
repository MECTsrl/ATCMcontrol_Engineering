
#ifndef __INSTVIEW_H_
#define __INSTVIEW_H_



//----  Aggregate Includes:   --------------------------------------*
#include "cgt_CDiagMsg.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


extern "C" long DumpInstView(
    CGT_CDiagMsg*      pdm,       //for err msg
    int                controlVer,
    ICG_IECFront*      pIF,       //front end IF
    const TCHAR*       pszTrgTyp, //target type
    const TCHAR*       pszTrgId,  //PC1
    const TCHAR*       pszTrgAddr,//w-ef 
    const CG_Resource* pRes       //called resource
);

#endif // __INSTVIEW_H_


