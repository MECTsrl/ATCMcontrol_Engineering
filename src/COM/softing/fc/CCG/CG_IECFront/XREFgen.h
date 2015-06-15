
#ifndef __XREFGEN_H_
#define __XREFGEN_H_



//----  Aggregate Includes:   --------------------------------------*
#include <stdio.h>
#include "helper.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


bool ST_ConsiderXREFDump(
    CG_CIECFront*   pFront,
    ST_CSymtabPrv*  pSymPrv,
    const ST_CBody* pBody
);



//type safe handel
class ACCESS_TREE;
//create a Simple Member Access info struct:
ACCESS_TREE* CreateSMAHash(const ST_CBody* pBody);
//free memory:
void FreeSMAHash(ACCESS_TREE* pSMA);
//look up if x.y.z got written:
bool GetsSMAWritten(
    ACCESS_TREE*             pSMA, 
    long                     nPath, 
    const CG_SymEntry*const* pPath/*[nPath]*/
);
//helper for debug dump:
void SMATreeDump(ACCESS_TREE* pTree, FILE* pFile);


#endif // __XREFGEN_H_

