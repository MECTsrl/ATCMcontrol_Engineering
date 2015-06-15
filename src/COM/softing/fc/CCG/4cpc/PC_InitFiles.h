
#ifndef PC_INITFILES_H_
#define PC_INITFILES_H_
#include "globctx.h"
class CGT_CDmp;

void ST_DumpInitFileEntry(
    CGlobCtx*            pgctx,
    CTarget*             pTrg,
    const TCHAR*         pszGenRelInitFile,
    const TCHAR*         pszInstanceAddr,
    const CG_SymEntry*   pEntry, 
    CGT_CDmp&            dmp
);


#endif //PC_INITFILES_H_




