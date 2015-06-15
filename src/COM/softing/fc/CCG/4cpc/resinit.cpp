

//----  Includes:   -------------------------------------------*
#include "cgt_tools.h"
#include "cgt_address.h"
#include "cgt_misc.h"
#include "helper.h"
#include "resinit.h"
#include "defines.h"
#include "globctx.h"
#include "parse4CH.h"
#include "PC_InitFiles.h"




//local helper:
static bool PC_DumpRetainFileRec(
    const CG_Symtab*   pSym,
    bool               bOuterRetain, //<-the instance is retain
    long*              pNRetainItems,
    CGT_CDmp&           dmp,
    CGT_CInstPath&     instpath
);
static void PC_DumpTaskInitFile(
    CGlobCtx*      pgctx,
    CTarget*       pTrg,
    const CG_Task* pTsk
);







extern "C" void PC_DumpRetainFile(
    CGlobCtx*  pgctx,
    CTarget*   pTrg
)
{
    int                i;
    CGT_CInstPath      instpath;
    long               nRetainItems;
    CGT_CDmp           dmp;
    CGT_CDiagMsg*      pdm;
    const CG_Resource* pRes;
    FC_CString         genRel(MAX_PATH);

    pdm  = &pgctx->m_dm;
    pRes = pTrg->getCGRes();
    assert(pRes);

    
    //add to domain list of target:
    genRel.load(pTrg->getTrgName()).appendDir(PC_RETAIN_ITEM_FILE);
    genRel.toLower(); //must all be lower case.
    pTrg->addDomain(genRel);



    nRetainItems = 0;

    PC_DumpRetainFileRec(pRes->pGVL, false,  &nRetainItems, dmp, instpath);
    assert(instpath.getDepth()==0);
    for(i=0; i<pRes->pGVL->nGVLImp; i++)
        PC_DumpRetainFileRec(pRes->pGVL->pGVLImp[i].pSym, false,  &nRetainItems, dmp, instpath);

    pTrg->setUsedRetain(nRetainItems);
    if(nRetainItems>PC_MAX_RETAIN_ITEMS)
    {
       pdm->msg3(CG_W_TOO_MANY_RETAIN_ITEMS, &pRes->edp,
           pRes->pszId, pdm->jot1()<<nRetainItems, pdm->jot2()<<PC_MAX_RETAIN_ITEMS);
    }

    CGT_DmpToFile(pdm, pgctx->getGenPath(), genRel, dmp, false);
}


/*
resource init file example:

version;1.20
resource_name;resource1
__state;system.FcResourceState;;;
__memused;system.FcResourceState;;;
__projectname;system.StringVar;;dd;
__iomanager;system.IOManager;;{88EF2AA0BF1011D1B14C06097E1BE2F},1,iodp.data|{88EF2AA1BF1011D1B14C06097E1BE2F },4,opcserv.data,opcgrp.data|{EF01C8D12EF711D3BB2D0608993FF},5,ioff.data;
task1;system.FcTask;;;__task1.init
program1;user.program1;;;program1.init
*/
extern "C" void PC_DumpTargetInitFile(
    CGlobCtx*  pgctx,
    CTarget*   pTrg
)
{
    int                i;
    int                j;
    CGT_CDmp           dmp;
    const CG_Resource* pRes;
    const CG_SymEntry* pEntry;
    FC_CString         genRel(MAX_PATH);


    pRes = pTrg->getCGRes();
    assert(pRes);


    genRel.load(pTrg->getTrgName()).appendDir(PC_TARGET_INIT_FILE);
    genRel.toLower(); //must all be lower case.
    //add to domain list of target:
    pTrg->addDomain(genRel);


    //version id:
    dmp<<_T("version;")BIN_CODE_VERSION _T("\n");

    //resource name:
    (dmp<<_T("resource_name;")).lo(pRes->pszId)<<_T('\n');
   
    //special variables at resource level:
    dmp<<_T("__state;system.FcResourceState;;;\n");
    dmp<<_T("__memused;system.FcResourceState;;;\n");
    dmp<<_T("__projectname;system.StringVar;;");
    //CVS escape: prj name isn't an identifier..
    dmp.dumpAsCSV(pgctx->m_pIF->FCPgetPrjName(),-1,true)<<_T(";\n");


    //append all domains from all Field Bus Masters:
    const FC_CGArray<PC_CIOComp*>* pFBMList = pTrg->getMasters();
    int   nIOLayers = pFBMList->getUsed();
    if(nIOLayers>0 && !pgctx->getLicData()->bDemo)//load IO-Layer only if not demo mode
    {
        for(i=0; i<nIOLayers; i++)
        {
            const PC_CIOComp*  pFBM = pFBMList->get(i);
            const FC_CStrList* pHWDomList = pFBM->getDomainList();

            dmp<<(i==0 ? _T("__iomanager;system.IOManager;;") : _T("|"));

            dmp<<pFBM->getIOLayerClsid()<<_T(",");
            dmp<<pFBM->getChannel();

            for(j=0; j<pHWDomList->getUsed(); j++)
            {
                const TCHAR* pszHWDom = pHWDomList->get(j);
                pTrg->addDomain(pszHWDom);

                dmp<<_T(",");
                dmp.lo(FC_StringGetFilePart(pszHWDom));   
            }
        }
        dmp<<_T(";\n");   
    }

    
    for(j=0; j<pRes->nTasks; j++)
    {
        //write task object into resource.init:
        const CG_Task* pTsk = &pRes->pTasks[j];
        
        //ignore IOSYNC Tasks:
        if(pgctx->m_pIF->lookUpTaskInit(pTsk, CG_TASK_IOSYNC))
            continue;

        dmp.lo(pTsk->pszId)<<_T(';')<<_T("system.FcTask")<<_T(";;;");
        //task init filename
        (dmp<<PC_TASK_FILE_PRFIX).lo(pTsk->pszId)<<_T(".init\n");
        
        //TODO check: done by front:  E_4CCG_WARN_NO_PROGRAM_AT_TASK ??
        PC_DumpTaskInitFile(pgctx, pTrg, pTsk);
    }


    for(pEntry=pRes->pGVL->pFirst ;pEntry; pEntry=pEntry->pNext)
    {
        assert(pEntry->declScope==CG_VAR_GLOBAL);
        ST_DumpInitFileEntry(pgctx, pTrg, genRel, NULL, pEntry, dmp);
    }
    for(j=0; j<pRes->pGVL->nGVLImp; j++)
    {
        for(pEntry=pRes->pGVL->pGVLImp[j].pSym->pFirst ;pEntry; pEntry=pEntry->pNext)
        {
            assert(pEntry->declScope==CG_VAR_GLOBAL || pEntry->declScope==CG_CONST);
            if (pEntry->declScope==CG_VAR_GLOBAL)
                ST_DumpInitFileEntry(pgctx, pTrg, genRel, NULL, pEntry, dmp);
        }
    }

    //Dump to disk:
    CGT_DmpToFile(&pgctx->m_dm, pgctx->getGenPath(), genRel, dmp, false);
}





















static bool PC_DumpRetainFileRec(
    const CG_Symtab*   pSym,
    bool               bOuterRetain, //<-the instance is retain
    long*              pNRetainItems,
    CGT_CDmp&          dmp,
    CGT_CInstPath&     instpath
)
{
    const CG_SymEntry*  pEntry;

    assert(pSym);
    if(!pSym)
        return false;

    //special sfc __retain if FB or Prg inst is sfc:
    if(bOuterRetain && pSym->flags & CG_STAB_BDY_IS_SFC)
    {
        assert(pSym->pouTyp==CG_POU_PRG || pSym->pouTyp==CG_POU_FB);
        (*pNRetainItems)++;
        if(instpath.getDepth()>0)
            dmp.lo(instpath)<<_T('.');

        dmp<<_T("__retain\n");
    }


    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        switch(pEntry->declScope)
        {
        case CG_VAR_EXTERNAL:
        case CG_VAR_IN_OUT:
        case CG_CONST:
            continue;
        }

        CG_DTYPES  type      = pEntry->pType->type;
        const TCHAR* pszId  = pEntry->pszId;

        if(pEntry->pType->bIsArray)
        {
            if((pEntry->declMod & CG_DMOD_RETAIN) ||
               (bOuterRetain && (pEntry->declMod & CG_DMOD_NON_RETAIN)==0)
              )
            {
                //no retain of IO Inputs!
                if(!pEntry->pszAddr || 
                     CGT_GetAddrLocation(pEntry->pszAddr)!=_T('I'))
                {
                    if(pEntry->pType->pSym)
                    {
                        for(long kk=0; kk<pEntry->pType->nElems; kk++)
                        {
                            instpath.append(pszId);
                            instpath.appendInt(kk);
                            PC_DumpRetainFileRec(pEntry->pType->pSym, true, 
                                              pNRetainItems, dmp, instpath);
            
                            //cut instpath to it's old value:
                            instpath.stripLast();
                            instpath.stripLast();
                        }
                    }
                    else
                    {
                        for(long kk=0; kk<pEntry->pType->nElems; kk++)
                        {
                            (*pNRetainItems)++;
                            if(instpath.getDepth()>0)
                                dmp.lo(instpath)<<_T('.');
                            
                            dmp.lo(pszId)<<_T('.')<<kk<<_T('\n');
                        }
                    }
                }
            }
        }
        else if(pEntry->pType->pSym)
        {
            bool bPropagateRetain = bOuterRetain;
            if(bPropagateRetain && (type==CG_INSTANCE))
            {
                //AMEND german comments 65b/323/cd
                //if retain/non_retain attributes are not
                //inherited for FunctionBlocks.
                //(but use inheritence for nested structs and Program lokal FBs!)
                //So, if the instance we are dumping is retain (bOuterRetain==true)
                //and this member is a FBI, set bPropagateRetain false.
                bPropagateRetain = false;
            }

            //check the retain quali of this declaration:
            if(bPropagateRetain && (pEntry->declMod & CG_DMOD_NON_RETAIN)!=0)
            {
                bPropagateRetain = false;
            }
            else if(pEntry->declMod & CG_DMOD_RETAIN)
            {
                bPropagateRetain = true;
            }

            instpath.append(pEntry->pszId);
            
            //check max nesting:

            PC_DumpRetainFileRec(pEntry->pType->pSym, bPropagateRetain, 
                                 pNRetainItems, dmp, instpath);
            
            //cut instpath to it's old value:
            instpath.stripLast();
        }
        else if(CG_IsElementary(type) || CG_IsAnyQVT(type) || CG_IsAnyEdge(type))
        {
            if((pEntry->declMod & CG_DMOD_RETAIN) ||
               (bOuterRetain && (pEntry->declMod & CG_DMOD_NON_RETAIN)==0)
              )
            {
                //no retain of IO Inputs!
                if(!pEntry->pszAddr || 
                    CGT_GetAddrLocation(pEntry->pszAddr)!=_T('I'))
                {
                    (*pNRetainItems)++;
                    //dump lower case:
                    if(instpath.getDepth()>0)
                        dmp.lo(instpath)<<_T('.');

                    dmp.lo(pszId)<<_T('\n');
                }
            }
        }
        else
        {
            assert(!"illegal datatype");
            return false;
        }
    }

    return true;
}



static void PC_DumpTaskInitFile(
    CGlobCtx*      pgctx,
    CTarget*       pTrg,
    const CG_Task* pTsk
)
{
    int                i; 
    CGT_CDmp            dmp;
    const CG_TaskInit* pTI;
    __int64            i64Priority;
    __int64            i64Interval;
    bool               bAutostart;
    FC_CString         genRel(MAX_PATH);
    bool               bVerHintGiven = false;

    //"<targetname>\__<taskname>.init"
    genRel<<pTrg->getTrgName()<<_T("\\")PC_TASK_FILE_PRFIX
          <<pTsk->pszId<<_T(".")PC_INIT_EXT;
    genRel.toLower(); //must all be lower case.
    //add to target loader.list:
    pTrg->addDomain(genRel);



    //get task inits, they must have been checked already !!:
    pTI = pgctx->m_pIF->lookUpTaskInit(pTsk,  CG_TASK_PRIORITY);
    if(pTI && pTI->val.extyp==CG_LITERAL && pTI->val.lit.pLit->ut==CG_LIT_I64)
        i64Priority = pTI->val.lit.pLit->i64;
    else
        CGT_IERR2(&pgctx->m_dm, pTI? &pTI->edp : NULL, _T("invalid task init after passed front end"));

    
    pTI = pgctx->m_pIF->lookUpTaskInit(pTsk,  CG_TASK_INTERVAL);
    if(pTI)
    {
        if(pTI && pTI->val.extyp==CG_LITERAL && pTI->val.lit.pLit->type==CG_TIME)
            i64Interval = pTI->val.lit.pLit->i64;
        else
            CGT_IERR2(&pgctx->m_dm, pTI? &pTI->edp : NULL, _T("invalid task init after passed front end"));
    }
    else if(pgctx->getControlVer()>=22000)
    {
        //non cyclic task:
        i64Interval = 0; 
    }
    else
    {
        i64Interval = 100; 
        pgctx->m_dm.errNS1(&pTsk->edp, _T("acyclic tasks (no 'INTERVAL:=' specified)"));
        if(!bVerHintGiven)
        {
            bVerHintGiven = true;
            pgctx->m_dm.userMsg(_T("    Note: to enable acyclic task features you must set firmware version >=22000\n"));
            pgctx->m_dm.userMsg(_T("          and be sure to download only to controls with version >=22000.\n"));
        }
    }


    pTI = pgctx->m_pIF->lookUpTaskInit(pTsk,  CG_TASK_AUTOSTART);
    if(!pTI)
        bAutostart = true;
    else if(pTI->val.extyp==CG_LITERAL)
        bAutostart = pTI->val.lit.pLit->i64 ? true : false;
    else
        CGT_IERR2(&pgctx->m_dm, pTI? &pTI->edp : NULL, _T("invalid task init after passed front end"));
    

    //task attributes:
    dmp<<_T("prio;")<<i64Priority<<'\n';
    dmp<<_T("interval_ms;")<<i64Interval<<'\n';
    dmp<<_T("autostart;")<<(bAutostart ? _T('1') : _T('0')) <<'\n';

    

    //list of prgs to run:
    dmp<<_T("run;");
    for(i=0; i<pTsk->nPrgs; i++)
    {
        if(i>0)
            dmp<<_T('|');
        dmp.lo(pTsk->pPrgs[i]->pszId);
    }
    dmp<<_T('\n');


    //predefined system variable:
    dmp<<_T("__state")<<_T(";system.FcTaskState;;;\n");
    dmp<<_T("__prio") <<_T(";system.FcTaskState;;;\n");
    dmp<<_T("__cycle")<<_T(";system.FcTaskState;;;\n");
    dmp<<_T("__etmax")<<_T(";system.FcTaskState;;;\n");
    dmp<<_T("__errno")<<_T(";system.FcTaskState;;;\n");


    if(pgctx->getControlVer()>=22000)
    {
        dmp<<_T("__usrcommand")<<_T(";system.FcTaskStateString;;;\n");
        dmp<<_T("__statetext0")<<_T(";system.FcTaskStateString;;;\n");
        dmp<<_T("__statetext1")<<_T(";system.FcTaskStateString;;;\n");
        dmp<<_T("__statetext2")<<_T(";system.FcTaskStateString;;;\n");
        dmp<<_T("__statetext3")<<_T(";system.FcTaskStateString;;;\n");
        dmp<<_T("__statetext4")<<_T(";system.FcTaskStateString;;;\n");
    }

    //Dump to disk:  
    CGT_DmpToFile(&pgctx->m_dm, pgctx->getGenPath(), genRel, dmp, false);
}



