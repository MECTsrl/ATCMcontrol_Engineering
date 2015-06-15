



//----  Includes:   -------------------------------------------*
#include "cgt_tools.h"
#include "fc_tools\fc_hash.h"
#include "cgt_address.h"





//local helper:
static void CheckVarExternalsRec(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //used to look up the gv
    const CG_Resource* pRes,    //calling resource
    const CG_Symtab*   pSym,    //Prg,FB (or Fun?) to check
    FC_CHashPtr<
      const CG_Symtab*, 
      bool>*           pChecked, //hash table for already checked POUs
    FC_CHashStr<
      const CG_SymEntry*
      >*               pHashAutoExt //if not NULL get all auto decls
);

static void CheckACCallingRestrictions(
    CGT_CDiagMsg*                pdm,         //for err msg
    ICG_IECFront*                pIF,         //used to look attribs
    const CG_Task*               pCallingTask,//the calling task 
    const CG_Symtab*             pCallee,     //the POU called
    FC_CGArray<const CG_Symtab*>&calltree     //the calling POUs
);




long CGT_CheckVarExternalsRec(
    CGT_CDiagMsg*        pdm,        //for err msg
    ICG_IECFront*        pIF,        //front end IF
    const CG_Resource*   pRes,       //calling resource
    const CG_SymEntry*** ppzpAutoExt //can be NULL 
)
{
    long                                    nErrBeg;
    const CG_Symtab*const*                  ppSym;
    //hash table for already checked POUs:
    FC_CHashPtr<const CG_Symtab*,  bool>    checked; 
    //hash of auto extern decls:
    FC_CHashStr<const CG_SymEntry*>*        pHashAutoExt;


    if(!pRes->bNoError)
        return -1; //don't call this function on error...
    
    if(ppzpAutoExt)
        pHashAutoExt = new FC_CHashStr<const CG_SymEntry*>(true, false);
    else
        pHashAutoExt = NULL;//caller does not need this info

    //remember err count on entry:
    nErrBeg = pdm->getTotalErrors();

    //loop all programs and all global FBs of this resource
    //call helper routine that recurses into pSym and checks
    //the var_externals and #import files against the resource gvl.
    for(ppSym = pRes->pzGVLRefSyms; *ppSym; ppSym++)
    {
        if((*ppSym)->pouTyp != CG_POU_SDT)
            CheckVarExternalsRec(pdm, pIF, pRes, *ppSym, &checked, pHashAutoExt);
    }

    if(ppzpAutoExt)
    {
        *ppzpAutoExt = pHashAutoExt->allocValArray(NULL);
        delete(pHashAutoExt);
    }

    //loopp all tasks and those that have INTERVAL attrib must not reference any wait functions:
    FC_CGArray<const CG_Symtab*> calltree(100);
    for(int i=0; i<pRes->nTasks; i++)
    {
        const CG_Task* pTsk = &pRes->pTasks[i]; 
        if(pIF->lookUpTaskInit(pTsk, CG_TASK_INTERVAL))
        {
            for(int j=0; j<pTsk->nPrgs; j++)
                CheckACCallingRestrictions(pdm, pIF, pTsk, pTsk->pPrgs[j]->pType->pSym, calltree);
        }//else: task is acyclic, dont' need to check
    }

    return pdm->getTotalErrors() - nErrBeg;
}




static void CheckVarExternalsRec(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //used to look up the gv
    const CG_Resource* pRes,    //calling resource
    const CG_Symtab*   pSym,    //Prg,FB and Fun to check var external use
    FC_CHashPtr<
      const CG_Symtab*, 
      bool>*           pChecked, //hash table for already checked POUs
    FC_CHashStr<
      const CG_SymEntry*
      >*               pHashAutoExt //if not NULL get all auto decls
)
{
    int                    j;
    bool                   bImportError;
    const CG_Symtab*const* ppSym;
    const CG_SymEntry*     pEX;
    const CG_SymEntry*     pGV;
    bool                   bTryToLoad;
    ICG_ExtBodyInfo*       pBdxExtInf;


    assert(pSym->pouTyp != CG_POU_GVL);
    
    bTryToLoad   = false;
    bImportError = false;
    pBdxExtInf   = NULL;

    //check if imported gvl files are also imported in resource:
    for(j=0; j<pSym->nGVLImp; j++)
    {
        const CG_Symtab* pGVL = pSym->pGVLImp[j].pSym;
        bool  bImpFound       = false;

        for(int i=0; i<pRes->pGVL->nGVLImp; i++)
        {
            if(pRes->pGVL->pGVLImp[i].pSym==pGVL)
            {
                bImpFound = true;
                break;
            }
        }

        if(!bImpFound)
        {
            bImportError = true;
            pdm->msg2(CG_E_GV_IMPORT_NOT_IN_RES, &pSym->pGVLImp[j].edp,
                pGVL->edp.pszFile, pRes->pszId);
        }
    }

    if(!bImportError && pSym->nExternal>0)
    {
        for(pEX = pSym->pFirst; pEX; pEX = pEX->pNext)
        {
            if(pEX->declScope != CG_VAR_EXTERNAL)
                continue;

            if(pEX->declMod & CG_DMOD_SYS_VAR)
            {
                if(pHashAutoExt)
                    pHashAutoExt->insert(pEX->pszId, pEX);

                //in any case don't validate this auto decl. 
                continue;
            }


            pGV = pIF->lookUpGlobVar(pRes, pEX->pszId);
            if(!pGV)
            {
                pdm->msg2(CG_E_GV_UNDEFINED, &pEX->edp,
                    pEX->pszId, pRes->pszId);
                continue;
            }
            if(pGV->declScope == CG_CONST)
            {
                //TODO: better diag msg:
                pdm->msg2(CG_E_GV_UNDEFINED, &pEX->edp, pEX->pszId, pRes->pszId);
                continue;
            }

            if(!pIF->dtypeFullEqual(pEX->pType, pGV->pType))
            {
                pdm->msg2(CG_E_GV_TYPE_MISMATCH, &pEX->edp,
                    pEX->pszId, pdm->edpToStr(&pGV->edp, pdm->jot1()));
                continue;
            }
            if( (pGV->declMod & CG_DMOD_CONSTANT) && 
               !(pEX->declMod & CG_DMOD_CONSTANT)
              )//otherway round is ok!
            {
                pdm->msg2(CG_E_GV_CONSTANT_MISMATCH, &pEX->edp, 
                    pEX->pszId, pdm->edpToStr(&pGV->edp, pdm->jot1()));
                continue;
            }

            //special problem: GV is AT %I but not declared CONSTANT,
            //special diagnostic for every write access:
            //bConstQuali must be true if address is not writable.
            //Need to do this check only if var external 
            //CG_SymEnrty::accFlags's W bit is set:
            if(pGV->pszAddr && (pEX->accFlags & CG_ACCESS_ANY_W)!=0 &&
               CGT_GetAddrLocation(pGV->pszAddr)==_T('I')
              )
            {
                const CG_Edipos* pEdps;
                long             nWrt;

                //if not already done get the extended body info IF:
                if(!bTryToLoad)
                {
                    bTryToLoad = true;
                    pBdxExtInf = pIF->loadExtBodyInfo(pSym);
                    //assert: should not fail since the CG_SymEnrty::accFlags 
                    //was set, this means that there is some body info avail 
                    //maybe also from a lib POU's .link file.
                    //don't call this function if errors in body !
                    assert(pBdxExtInf || !"pIF->loadExtBodyInfo failed");
                }
             
                if(!pBdxExtInf)
                    continue;

                nWrt = pBdxExtInf->getVarExternalWrites(pEX, &pEdps);
                assert(nWrt>0);//assert: must be at least one!
                for(j=0; j<nWrt; j++)
                {
					const TCHAR* pszAddrDisplay = pGV->pszAddr; //forosai
					if(pGV->declMod & CG_DMOD_SYS_ADDR)
					{
						if(pGV->pszSysAddr)
							pszAddrDisplay = pGV->pszSysAddr; //CAUTION: only access member pszSysAddr if  
								                              //CG_DMOD_SYS_ADDR bit is set6, maybe old version of frontend IF !
					}
                    pdm->msg2(CG_E_GV_INPUT_WRITE, 
                        &pEdps[j], pGV->pszId, pszAddrDisplay);
                }
            }
        }
    }

    FC_RELEASE_PTR(pBdxExtInf);


    if(pSym->pzRefSyms)
    {
        for(ppSym=pSym->pzRefSyms; *ppSym; ppSym++)
        {
            if(pChecked->insert(*ppSym, true))
                CheckVarExternalsRec(pdm, pIF, pRes, *ppSym, pChecked, pHashAutoExt);
            //else: was already in hash and therefore already checked too.
        }
    }
    else
    {
        assert(!"body not loaded?");
    }
}

static void DumpCallTree(
    CGT_CDiagMsg*                pdm,         //for err msg
    const CG_Task*               pCallingTask,//the calling task 
    FC_CGArray<const CG_Symtab*>&calltree     //the calling POUs
)
{
    pdm->userMsg(_T("    "));
    pdm->userMsg(pCallingTask->pszId);

    for(int i=0; i<calltree.getUsed(); i++)
    {
        pdm->userMsg(_T("->"));
        pdm->userMsg(calltree.get(i)->pszId);
    }

    pdm->userMsg(_T("\n\n"));
}

static void CheckACCallingRestrictions(
    CGT_CDiagMsg*                pdm,         //for err msg
    ICG_IECFront*                pIF,         //used to look attribs
    const CG_Task*               pCallingTask,//the calling task 
    const CG_Symtab*             pCallee,     //the POU called
    FC_CGArray<const CG_Symtab*>&calltree     //the calling POUs
)
{
    int                     i;
    const CG_Symtab*        pSym;
    const CG_KeyValPair*    pAttrCtx;

    //printf("task: '%s' callee: '%s' caller: '%s'\n", pCallingTask->pszId, pCallee->pszId, pCaller?pCaller->pszId:"NULL");

    calltree.push(pCallee);


    if(pCallee->flags & CG_STAB_FORCEACTASK)
    {
        pAttrCtx = pIF->lookUpAttrib(pCallee->pAttribs, _T("forcecontext"));//TODO use pSym->flags ??
        assert(pAttrCtx);
        if(pAttrCtx)
        {
            pdm->msg1(CG_E_TEXT, &pAttrCtx->edpVal, pdm->jot1()<<
                _T("acyclic function called from cyclic task '")<<pCallingTask->pszId<<_T("' call tree:"));
            DumpCallTree(pdm, pCallingTask, calltree);
        }
    }

    if(pCallee->pzRefSyms)
    {
        for(i=0; (pSym = pCallee->pzRefSyms[i])!=NULL; i++)
            CheckACCallingRestrictions(pdm, pIF, pCallingTask, pSym, calltree);
    }//else body not loaded, ignore ...

    calltree.pop(pCallee);
}

