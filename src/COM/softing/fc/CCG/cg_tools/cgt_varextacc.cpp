

//----  Includes:   -------------------------------------------*
#include "cgt_tools.h"
#include "fc_tools\fc_hash.h"

//define the typesafe 'HANDLE' struct, 
//only foreward decl in cgt_tools.h
struct _CGT_VarExtAccessInfo
{
    FC_CHashPtr<const CG_SymEntry*,  WORD>    simpleUsedGV; 


    //iterator set by 
    const FC_CHashPtr<const CG_SymEntry*,WORD>::Pair* pPair;

    CGT_VarExtAccessInfo()
    {
        pPair = NULL;
    }
};


//local helper:
static bool GetVarExtAccessInfoOfPOU
(
    CGT_CDiagMsg*           pdm,          //for err msg
    ICG_IECFront*           pIF,          //used to look up the gv
    const CG_Resource*      pRes,         //calling resource
    FC_CHashStr<
      const CG_SymEntry*
      >*                    pHashAutoExt, //sample auto GV decls
    const CG_Symtab*        pSym,         //current Prg,FB,Fun
    CGT_VarExtAccessInfo*   pInfo
);



CGT_VarExtAccessInfo* CGT_GetVarExtAccessInfo(
    CGT_CDiagMsg*      pdm,      //for err msg
    ICG_IECFront*      pIF,      //front end IF
    const CG_Resource* pRes,     //calling resource
    const CG_Task*     pTask     //calling task
)
{
    int                             j;
    FC_CHashStr<const CG_SymEntry*> autoGVs(true, false);
    CGT_VarExtAccessInfo*           pInfo;
    const CG_Symtab**               pzpSym;

    pzpSym= CGT_EnumRefSymtabsOfTask(pdm, pIF, pTask);
    if(!pzpSym)
    {
        assert(pdm->hasErr());
        return NULL;
    }

    pInfo = new CGT_VarExtAccessInfo;
    //loop all references POUs of this tasks:
    for(j=0; pzpSym[j]; j++)
    {
        if(pzpSym[j]->nExternal==0)
            continue;

        if(!GetVarExtAccessInfoOfPOU(pdm, pIF, pRes, &autoGVs, pzpSym[j], pInfo))
        {
            delete(pInfo);
            pInfo = NULL;
            break;
        }
    }

    delete(pzpSym);
    return pInfo;
}
void CGT_FreeVarExtAccessInfo(CGT_VarExtAccessInfo* pInfo)
{
    delete(pInfo);
}

long CGT_InitVarExtAccInfoList(CGT_VarExtAccessInfo* pInfo)
{
    pInfo->pPair = pInfo->simpleUsedGV.firstPair();
    return pInfo->simpleUsedGV.countPairs();
}
int CGT_GetNextVarExtAccInfo(
    CGT_VarExtAccessInfo* pInfo,
    CGT_AccInfo*          pAcc
)
{
    if(!pInfo->pPair)
        return 0;
    
    pAcc->pEntry    = pInfo->pPair->pKey;
    pAcc->bRead     = (pInfo->pPair->pVal & CG_ACCESS_ANY_R) != 0;
    pAcc->bWritten  = (pInfo->pPair->pVal & CG_ACCESS_ANY_W) != 0;

    pInfo->pPair = pInfo->pPair->pNext;
    return 1;
}

void CGT_DumpVarExtAccessInfo(
    FILE*                 pFile, 
    CGT_VarExtAccessInfo* pInfo
)
{
    CGT_AccInfo acc;   

    if(!pFile)
        pFile = stdout;

    CGT_InitVarExtAccInfoList(pInfo);
    while(CGT_GetNextVarExtAccInfo(pInfo, &acc))
    {
        _ftprintf(pFile, _T("%s = %s%s\n"), 
            acc.pEntry->pszId, 
            acc.bRead   ? "R" : "",
            acc.bWritten ? "W" : ""
        );
    }
    
}
const TCHAR* CGT_ExprAccFlagsToString(WORD accFlags, TCHAR szBuffer[/*32*/])
{
    int i = 0;

    if(accFlags & CG_ACCESS_R)
        szBuffer[i++] = _T('R');
    if(accFlags & CG_ACCESS_r)
        szBuffer[i++] = _T('r');

    if(accFlags & CG_ACCESS_W)
        szBuffer[i++] = _T('W');
    if(accFlags & CG_ACCESS_w)
        szBuffer[i++] = _T('w');

    if(accFlags & CG_ACCESS_O)
        szBuffer[i++] = _T('O');
    if(accFlags & CG_ACCESS_o)
        szBuffer[i++] = _T('o');

    if(accFlags & CG_ACCESS_I)
        szBuffer[i++] = _T('I');
    if(accFlags & CG_ACCESS_i)
        szBuffer[i++] = _T('i');

    if(accFlags & CG_ACCESS_X)
        szBuffer[i++] = _T('X');
    if(accFlags & CG_ACCESS_x)
        szBuffer[i++] = _T('x');

    if(accFlags & CG_ACCESS_C)
        szBuffer[i++] = _T('C');
    if(accFlags & CG_ACCESS_c)
        szBuffer[i++] = _T('c');

    if(accFlags & CG_ACCESS_A)
        szBuffer[i++] = _T('A');
    if(accFlags & CG_ACCESS_a)
        szBuffer[i++] = _T('a');

    if(accFlags & CG_ACCESS_B)
        szBuffer[i++] = _T('B');

    if(accFlags & CG_ACCESS_N)
        szBuffer[i++] = _T('N');

    szBuffer[i++] = 0;
    return szBuffer;
}







//local helper:
static bool GetVarExtAccessInfoOfPOU
(
    CGT_CDiagMsg*           pdm,          //for err msg
    ICG_IECFront*           pIF,          //used to look up the gv
    const CG_Resource*      pRes,         //calling resource
    FC_CHashStr<
      const CG_SymEntry*
      >*                    pHashAutoExt, //sample auto GV decls
    const CG_Symtab*        pSym,         //current Prg,FB,Fun
    CGT_VarExtAccessInfo*   pInfo
)
{
    const CG_SymEntry*     pEX;
    const CG_SymEntry*     pGV;
    
    assert(pSym->pouTyp==CG_POU_PRG || pSym->pouTyp==CG_POU_FB || pSym->pouTyp==CG_POU_FUN);
    assert(pSym->flags & CG_STAB_HDR_NOERR); 
    assert(!(pSym->flags & CG_STAB_HAS_BDY) || 
              (pSym->flags & CG_STAB_BDY_LOADED) && (pSym->flags & CG_STAB_BDY_NOERR));

    for(pEX = pSym->pFirst; pEX; pEX = pEX->pNext)
    {
        if(pEX->declScope != CG_VAR_EXTERNAL)
            continue;

        //get the GV entry , either an auto declared ext or from resource's gvl:
        pGV =NULL;
        if(pEX->declMod & CG_DMOD_SYS_VAR)
        {
            //if aout decl var we must megre all access bits to the same
            //CG_SymEntry object, we use simply the first VAR_EXTERNAL decl
            //that was inserted into the pHashAutoExt by name.
            //TODO: maybe a better idea to update also the resource gvl
            //if "AddAutoDecl" ???
            if(pHashAutoExt->insertEx(pEX->pszId, pEX, false, &pGV))
                pGV = pEX;
            //else: was already inserted, insertEx has set 
            //      pGV to it's previously inserted value.
        }
        else
        {
            pGV = pIF->lookUpGlobVar(pRes, pEX->pszId);
        }

        if(!pGV)
        {
            assert(!"should not happen since called after check var ext with 0 erors");
            pdm->msg2(CG_E_GV_UNDEFINED, &pEX->edp,
                pEX->pszId, pRes->pszId);
            return false;
        }

        if(pEX->accFlags != 0)
        {
            //somehow used, merge new accInfo with possible previous accInfo:
            WORD accFlags = 0;
            pInfo->simpleUsedGV.get(pGV, &accFlags);
            pInfo->simpleUsedGV.insert(pGV, accFlags | pEX->accFlags);
        }
        //else: var ext unused
    }

    return true;
}


