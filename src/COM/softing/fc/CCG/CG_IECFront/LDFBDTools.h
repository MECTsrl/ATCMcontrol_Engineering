
#ifndef __LDFBDTOOLS_H_
#define __LDFBDTOOLS_H_



//----  Aggregate Includes:   --------------------------------------*
#include "Hash4GR.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*


//----  Prototypes:    ---------------------------------------------*




bool SortPinArray(
    CGT_CDiagMsg*             pdm,
    const CG_Edipos*          pEdp,
    FC_CGArray<PinElement*>&  pins,
    int*                      pnInputs,
    int*                      pnOutputs,
    PinElement**              ppEN,
    PinElement**              ppENO
);

const CG_SymEntry* GetFirstLDFBDInEntry (const CG_Symtab*   pSym);
const CG_SymEntry* GetNextLDFBDInEntry  (const CG_SymEntry* pLast);
const CG_SymEntry* GetFirstLDFBDOutEntry(const CG_Symtab*   pSym);
const CG_SymEntry* GetNextLDFBDOutEntry (const CG_SymEntry* pLast);



FC_CStr CreatePinErrorMsgName(
    HINSTANCE         hInst, 
    const BoxElement* pBox, 
    const PinElement* pPin
);

bool CheckPinUseAgainstHeader(
    CGT_CDiagMsg*        pdm, 
    const CG_Edipos*     pBoxEdp,
    const BoxElement*    pBox, 
    const PinElement*    pPin, 
    const ST_CSymtabPrv* pSymOfEntry,
    const CG_SymEntry*   pEntry
);


void BoxTypeToDiagString(BOX_TYPE boxType, FC_CString& text, HMODULE hmodRes);


//check if pExpr is written more than once in a network
//an expr is considered to be 'the same' if it's ST dump
//is equal in ignorecase mode (see CGT_DumpSTExpr):
void WarnMultibleWriteInNW(
    CGT_CDiagMsg*    pdm,
    const CG_STStmt* pNW
);




#endif // __LDFBDTOOLS_H_

