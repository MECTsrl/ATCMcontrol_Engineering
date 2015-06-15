#ifndef DECLCHECK_H_
#define DECLCHECK_H_

#include "helper.h"
#include "expr.h"

void ST_CheckNewDeclScope(
    CG_CIECFront*    pFront,
    const CG_Edipos* pEdp, 
    const CG_Symtab* pSym,
    CG_DECLSCOPE*    pS, 
    WORD*            pdeclMod
);

bool ST_CheckAndAppendDecl(
    CG_CIECFront*    pFront,
    ST_CSymtabPrv*   pSymPrv,
    CG_DECLSCOPE     declScope,
    WORD             declMod,

    const TCHAR*     pszId, 
    const CG_Edipos* pEdpId, 

    const CG_DType*  pDType,
    const CG_Edipos* pEdpTyp,

    const TCHAR*     pszAddr, 
	const TCHAR*     pszSysAddr, //forosai
    const CG_Edipos* pEdpAdr, 

    const CG_AttribList*   pAttr, 
    const CG_Initialiser*  pInit
);



#endif //DECLCHECK_H_
