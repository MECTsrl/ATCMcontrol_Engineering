/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/dump4asm.cpp 2     12.07.07 14:34 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: dump4asm.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/dump4asm.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 12.07.07 14:34 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "cgt_misc.h"
#include "globctx.h"
#include "dump4asm.h"
#include "vmasm.h"
#include "VmConstants.h"
#include "helper.h"
#include "defines.h"
#include "Parse4CH.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_hash.h"
#include "resource.h"
#include "cgt_address.h"

//local helper:
static bool VMDumpHeaderOfPOU(
    CGT_CDmp&         dmp,
    CGlobCtx*         pgctx,
    const CG_Symtab*  pSym
);
static void SFC_dumpHeader(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS
);
static void SFC_dumpStaticSfcStructure(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS
);
static void SFC_dumpDoTransAct(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS
);
static void SFC_dumpTransActAttributes(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS
);
static void SFC_dumpInterStructure(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS
);
static void SFC_dumpVisData(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS
);
static void SFC_dumpBody(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS,
    BODY_CTX_INFO*      pctx
);
static void VMDumpFunctionInits(
    const BODY_CTX_INFO* pctx,
    CGT_CDiagMsg*        pdm,
    CGT_CDmp&            dmp,
    const CG_Symtab*     pSym
);                             


static BP_OPTION GetBPOptOfPOU(CGlobCtx* pgctx, const CG_Symtab* pSym)
{
    BP_OPTION            bpoptCls   = pgctx->getBPOption();
    const CG_KeyValPair* pAttr;
    
    // get optional class attributes 
    pAttr = pgctx->m_pIF->lookUpAttrib(pSym->pAttribs, CG_VM_CLASS_OPT_BP);
    if (pAttr)
    {
        if ( ! _tcsicmp(pAttr->pszVal, _T("0")))
            bpoptCls = BP_OPT0;
        else if ( ! _tcsicmp(pAttr->pszVal, _T("1")))
            bpoptCls = BP_OPT1;
        else if ( ! _tcsicmp(pAttr->pszVal, _T("2")))
            bpoptCls = BP_OPT2;
        else 
        {
            pgctx->m_dm.msg2(CG_W_SYNTAX_EXPECTED_FOUND, &pAttr->edpVal, 
                CG_VM_CLASS_OPT_BP _T("=\"[0|1|2]\""), 
                pgctx->m_dm.jot1()<<CG_VM_CLASS_OPT_BP _T("=")<<_T("\"")<<pAttr->pszVal<<_T("\"")
            );
        }
    }
    return bpoptCls;
}


//get precompiled code from lib fun and fb,prg pass here:
//this function must only be called for library POUs that dont have a body
//(Note: library POUs that have a body are source code)
//and are not pure inline. 
void GenCodeLibPOU(
    CGlobCtx*               pgctx,
    const CG_Symtab*        pSym,
    CCodeInfo*              pInfo,
    FC_CGArray<CGT_BP_TYP>* pbpList
)
{
    assert(pSym->pLibCtx);
    assert(!(pSym->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR)));
    assert(!(pSym->flags & CG_STAB_HAS_BDY));
    
    CGT_IERR2(&pgctx->m_dm, &pSym->edp, _T("not supported: lib POU that is not pure inline"));
}






//entry function for this file: 
extern "C" void GenCodeStruct(
    CGlobCtx*                 pgctx,
    CCodeInfo*                pInfo
)
{
    const CG_SymEntry*  pEntry;
    CGT_CDmp*           pDmp = new CGT_CDmp;
    CGT_CDmp&           dmp = *pDmp;


    // class definition:
    (dmp<<_T(".struct ")).lo(pInfo->m_pSym->pszId)<<'\n';
    dmp.incrIndent();

    //TODO: don't use target so early! lib build?? we have only one target at the moment, may be NULL if build libs:
    CTarget* pTrg = pgctx->getTarget();
    for(pEntry = pInfo->m_pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        CG_VM_DumpOneVariable(dmp, pgctx, pTrg, pEntry, false);
    }

    dmp.decrIndent();

    //close class definition:
    dmp<<_T(".end\n\n");

    // remember dumper
    pInfo->m_pDmp = pDmp;

    //dump to disk:
    FC_CString asmfile(MAX_PATH);
    asmfile<<pgctx->getTrgTypeGenPath();  // at the moment we have only one target type
    asmfile.appendDir(pInfo->m_pSym->pszId) << CG_VM_ASM_TEXT_EXT;
    CGT_DmpToFile(&pgctx->m_dm, NULL, asmfile, dmp, false);
}
    



//entry function for this file: 
extern "C" void GenCodeSFCPrgFB(
    CGlobCtx*                 pgctx,
    const CG_Resource*        pRes, //may be NULL if building a library
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
)
{
    const CG_Symtab*        pSym;
    CGT_CDmp*               pDmp = new CGT_CDmp;
    CGT_CDmp&               dmp = *pDmp;
    const CG_SFCStruct*     pSFCS;
    BODY_CTX_INFO           bdyctx;
    CBodyInfo               bdyinf(pbpList, pgctx);
    
    //init local variables:
    pSym = pIBdy->getHeader();
    assert(pSym->pouTyp != CG_POU_SDT && pSym->pouTyp != CG_POU_FUN);

    //copy right and packages:
    dmp<<_T("\n\n");

    //class definition:
    if(pSym->pouTyp==CG_POU_PRG)
        dmp<<_T(".prg ");
    else if(pSym->pouTyp==CG_POU_FB)
        dmp<<_T(".fb ");
    else
        assert(!"bad POU typ");

    dmp.lo(pSym->pszId)<<'\n';
    dmp.incrIndent();

    // class id
    dmp<<_T("clssid ")<<pgctx->getNextFreeClassId()<<'\n';
    
    bdyctx.pgctx            = pgctx;
    bdyctx.pTrg             = pgctx->getTarget();
    bdyctx.ulTrgVer         = bdyctx.pTrg ? bdyctx.pTrg->getTrgVer() : 0;
    bdyctx.pRes             = pRes;
    bdyctx.pSym             = pSym;
    bdyctx.pIF              = pgctx->m_pIF;
    bdyctx.pdm              = &pgctx->m_dm;
    bdyctx.pIBdy            = pIBdy;
    bdyctx.bTransi          = false;
    bdyctx.bAction          = false;
    bdyctx.bExceptionAction = false;
    bdyctx.pInf             = &bdyinf;
    bdyctx.BPoption         = GetBPOptOfPOU(pgctx, pSym);


    pSFCS = pIBdy->getSFCStruct();
    SFC_dumpHeader(dmp, pSFCS);
    dmp << _T(";///////////////////// SFC visualisation data\n");
    SFC_dumpVisData(dmp, pSFCS);
    dmp << _T(";///////////////////// Static SFC struct\n");
    SFC_dumpStaticSfcStructure(dmp, pSFCS);
    dmp << _T(";///////////////////// SFC internal structures\n");
    SFC_dumpInterStructure(dmp, pSFCS);
    dmp << _T(";///////////////////// doTransAct\n");
    SFC_dumpDoTransAct(dmp, pSFCS);
    dmp << _T(";///////////////////// Step / Action Attributes\n");
    SFC_dumpTransActAttributes(dmp, pSFCS);
    dmp << _T("var int __count       := int#0  ;used for while loops\n");
    
    dmp << _T(";///////////////////// user variables\n");
    if(VMDumpHeaderOfPOU(dmp, pgctx, pSym))
    {
        dmp<<_T(";///////////////////// SFC body\n");
        SFC_dumpBody(dmp, pSFCS, &bdyctx);
    }

    //close class definition:
    dmp<<_T("RETN\n");
    dmp.decrIndent();
    dmp<<_T(".end\n\n");
    
    // remember dumper
    pInfo->m_pDmp = &dmp;
    
    //dump the 4asm file to disk:
    FC_CString asmfile(MAX_PATH);
    asmfile << pgctx->getTrgTypeGenPath();  // at the moment we have only one target type
    asmfile.appendDir(pSym->pszId) << CG_VM_ASM_TEXT_EXT;
    CGT_DmpToFile(&pgctx->m_dm, NULL, asmfile, dmp, false);
}


//entry function for this file: 
extern "C" void GenCodeSTPrgFB(
    CGlobCtx*                 pgctx,
    const CG_Resource*        pRes, //may be NULL if building a library
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
)
{
    const CG_Symtab*        pSym;
    CGT_CDmp*               pDmp = new CGT_CDmp;
    CGT_CDmp&               dmp = *pDmp;
    BODY_CTX_INFO           bdyctx;
    CBodyInfo               bdyinf(pbpList, pgctx);

    //init local variables:
    pSym = pIBdy->getHeader();
    assert(pSym->pouTyp != CG_POU_SDT);

    dmp<<_T("\n\n");

    // class definition:
    if(pSym->pouTyp==CG_POU_PRG)
        dmp<<_T(".prg ");
    else if(pSym->pouTyp==CG_POU_FB)
        dmp<<_T(".fb ");
    else
        assert(!"bad pou type");

    dmp.lo(pSym->pszId)<<'\n';
    dmp.incrIndent();
    dmp<<_T("clssid ")<<pgctx->getNextFreeClassId()<<'\n';
    VMDumpHeaderOfPOU(dmp, pgctx, pSym);

    // BODY
    bdyctx.pgctx            = pgctx;
    bdyctx.pTrg             = pgctx->getTarget();
    bdyctx.ulTrgVer         = bdyctx.pTrg ? bdyctx.pTrg->getTrgVer() : 0;
    bdyctx.pRes             = pRes;
    bdyctx.pSym             = pSym;
    bdyctx.pIF              = pgctx->m_pIF;
    bdyctx.pdm              = &pgctx->m_dm;
    bdyctx.pIBdy            = pIBdy;
    bdyctx.bTransi          = false;
    bdyctx.bAction          = false;
    bdyctx.bExceptionAction = false;
    bdyctx.pInf             = &bdyinf;
    bdyctx.BPoption         = GetBPOptOfPOU(pgctx, pSym);
    assert(bdyctx.BPoption != BP_OPT_INVAL);


    //"begin of POU" BP function use special BP number 0:
    if(bdyctx.BPoption != BP_OPT0)
    {
        DumpBPFun(dmp, &bdyctx, pIBdy->edpBeg(), 0, false);
        //only for NCC:
        DumpBPFun(dmp, &bdyctx, pIBdy->edpBeg(), bdyctx.pInf->m_lastBPNum++, true);
    }

    VMDmpStmt(dmp, pIBdy->getSTStmts(), &bdyctx);

    //only for NCC to seperate the RET from the last stmt (ret cannot be converted to native):
    if(bdyctx.BPoption == BP_OPT0)
        DumpBPFun(dmp, &bdyctx, pIBdy->edpBeg(), bdyctx.pInf->m_lastBPNum++, true);

    dmp<<_T(":")CG_VM_LABEL_POUEND _T("\n");

    //"end of POU" BP function use special BP number 1:
    if(bdyctx.BPoption != BP_OPT0)
        DumpBPFun(dmp, &bdyctx, pIBdy->edpEnd(), 1, false);

 
    dmp<<_T("RETN\n");
    dmp.decrIndent();
    dmp<<_T(".end\n\n\n");

    // remember dumper
    pInfo->m_pDmp = pDmp;

    //dump the 4asm file to disk:
    FC_CString asmfile(MAX_PATH);
    asmfile << pgctx->getTrgTypeGenPath();  // at the moment we have only one target type
    asmfile.appendDir(pSym->pszId) << CG_VM_ASM_TEXT_EXT;
    CGT_DmpToFile(&pgctx->m_dm, NULL, asmfile, dmp, false);
}


//entry function for this file: 
extern "C" void GenCodeFun(
    CGlobCtx*               pgctx,
    const CG_Resource*      pRes, //may be NULL if building a library
    ICG_Body*               pIBdy,
    CCodeInfo*              pInfo,
    FC_CGArray<CGT_BP_TYP>* pbpList
)
{
    FC_CString              jot;
    const CG_Symtab*        pSym;
    const CG_SymEntry*      pEntry;
    CGT_CDmp                tmpBdy;
    BODY_CTX_INFO           bdyctx;
    CBodyInfo               bdyinf(pbpList, pgctx);

    // remember dumper in pInfo and dmp as ref:
    pInfo->m_pDmp     = new CGT_CDmp;
    CGT_CDmp& dmp = *pInfo->m_pDmp;

    pSym = pIBdy->getHeader();
    assert(pInfo->m_pSym==pSym);

    bdyctx.pgctx            = pgctx;
    bdyctx.pTrg             = pgctx->getTarget();
    bdyctx.ulTrgVer         = bdyctx.pTrg ? bdyctx.pTrg->getTrgVer() : 0;
    bdyctx.pRes             = pRes;
    bdyctx.pSym             = pSym;
    bdyctx.pIF              = pgctx->m_pIF;
    bdyctx.pdm              = &pgctx->m_dm;
    bdyctx.pIBdy            = pIBdy;
    bdyctx.bTransi          = false;
    bdyctx.bAction          = false;
    bdyctx.bExceptionAction = false;
    bdyctx.pInf             = &bdyinf;
    bdyctx.BPoption         = pgctx->getBPOption();

    tmpBdy.incrIndent();

    VMDmpStmt(tmpBdy, pIBdy->getSTStmts(), &bdyctx);

    //class definition:
    dmp<<_T("\n\n");
    (dmp<<_T(".fun ")).lo(pSym->pszId)<<'\n';
    dmp.incrIndent();
    // class id
    dmp<<_T("clssid ")<<pgctx->getNextFreeClassId()<<'\n';

    //function locals:
    bool bError = false;
    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(!CG_VM_DumpOneVariable(dmp, pgctx, bdyctx.pTrg, pEntry, false))
            bError = true;
    }
    
    if(bError)
        return;

    // dump init value for local vars function return
    VMDumpFunctionInits(&bdyctx, &pgctx->m_dm, dmp, pSym);

    dmp<<_T("\n; function body\n");
    dmp.decrIndent();
    dmp.append(tmpBdy);

    // end of FUN
    dmp<< _T(":") CG_VM_LABEL_POUEND _T("\n");

    dmp.incrIndent();
        //only for NCC to seperate the RET from the last stmt (ret cannot be converted to native):
        DumpBPFun(dmp, &bdyctx, pIBdy->edpBeg(), 1, true);
        (dmp<<_T("CLST            leavebytes(")).lo(pSym->pszId)<<_T(")\n");
        dmp<<_T("RETN\n");
    dmp.decrIndent();

    dmp<<_T(".end\n\n\n");


    //dump the 4asm file to disk:
    //set asm file domain to code info, path must be
    //relative to gen dir, will be needed for lib create:
    jot.load(pgctx->getTrgTypeGenPath());  // at the moment we have only one target type
    jot.appendDir(pSym->pszId)<<CG_VM_ASM_TEXT_EXT;
    pInfo->m_asmFile.load(jot);

    CGT_DmpToFile(&pgctx->m_dm, NULL, jot, dmp, false);
}


// return -1 if not succesful: fatal error
static long VMGetIdOfMacroFB(
    CGlobCtx*            pgctx,
    const CG_Symtab*     pFBSmy,
    bool*                pbExtLib,
    long*                pMinVersion, //output: min control version, 0 if all versions are ok
    long*                pMaxVersion  //output: max control version, 99999 if all versions are ok
)
{
    TCHAR               funopid[MAX_PATH];
    const CG_MacroBody* pMac;
    VM_MACRO_TYP        mtyp = VM_MAC_UNDEF;

    *pMinVersion = 0;
    *pMaxVersion = 99999;
    *pbExtLib = false;

    pMac = pgctx->m_pIF->getNonPolyMacroBody(pFBSmy);
    assert(pMac);
    if(pMac)
    {
        assert(pFBSmy->pLibCtx == pMac->pSym->pLibCtx);
        mtyp = VMClassifyMacroText(&pgctx->m_dm, &pMac->edp, pMac->pszMac, funopid);
    }

    switch(mtyp)
    {
    case VM_MAC_ERROR:      
        return -1;
    case VM_MAC_EXTLIB:     
         *pbExtLib = true;
    case VM_MAC_STDLIB:   
        return pgctx->getIdOfLibFunFb(pFBSmy->pLibCtx, &pMac->edp, funopid, pMinVersion, pMaxVersion);
    default:
        pgctx->m_dm.errNS1(&pMac->edp, pgctx->m_dm.res1(CG_RES_STR_WRONG_INLINE_FB));
    }

    return -1;
}

static bool VMDumpHeaderOfPOU(
    CGT_CDmp&         dmp,
    CGlobCtx*         pgctx,
    const CG_Symtab*  pSym 
)
{
    bool bOk = true;
    const CG_SymEntry* pEntry;
    const CG_SymEntry* pEntry_last = NULL;
    bool  bInRetain = false;   //TODO: sort the header !!
    bool  bInConstRetain = false;   
    bool  bInRetain_last = false;   
    bool  bInConstRetain_last = false;   
    int   nRetainSections = 0;
    int   nConstRetainSections = 0;

    //TODO use VMCheckDecl for all diags, but be carefull that maybe 
    //io counters are called twice ?

    //TODO: don't use target so early! lib build?? we have only one target at the moment, may be NULL if build libs:
    CTarget* pTrg = pgctx->getTarget();
    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        bInRetain      = (pEntry->declMod & CG_DMOD_RETAIN) && !(pEntry->declMod & CG_DMOD_CONSTANT);
        bInConstRetain = (pEntry->declMod & CG_DMOD_RETAIN) &&  (pEntry->declMod & CG_DMOD_CONSTANT);
        bool bConsiderByteAlign = false;

        if(bInRetain && !bInRetain_last)
        {
            dmp<<_T(";retain:\n");
            bConsiderByteAlign = true;
            nRetainSections++;
            if(nRetainSections==2)
                pgctx->m_dm.errNS1(&pEntry->edp, pgctx->m_dm.res1(CG_RES_STR_NS_MULTI_RETSEC));
        }
        if(!bInRetain && bInRetain_last)
        {
            bConsiderByteAlign = true;
            dmp<<_T(";end retain\n");
        }
        if(bInConstRetain && !bInConstRetain_last)
        {
            dmp<<_T(";constant retain:\n");
            bConsiderByteAlign = true;
            nConstRetainSections++;
            if(nConstRetainSections==2)
                pgctx->m_dm.errNS1(&pEntry->edp, pgctx->m_dm.res1(CG_RES_STR_NS_MULTI_CRETSEC));
        }
        if(!bInConstRetain && bInConstRetain_last)
        {
            bConsiderByteAlign = true;
            dmp<<_T(";end constant retain\n");
        }

        if(bConsiderByteAlign)
            if(pEntry_last && pEntry_last->pType->type==CG_BOOL && pEntry->pType->type==CG_BOOL)
                dmp<<_T("align8\n");

        pEntry_last = pEntry;
        bInRetain_last = bInRetain;
        bInConstRetain_last = bInConstRetain;

        if(!CG_VM_DumpOneVariable(dmp, pgctx, pTrg, pEntry, false))
            bOk = false;
        

        //HACK?? Header for library FBs, only to give the ASM the offsets:
        if(pEntry->pType->type == CG_INSTANCE && !(pEntry->pType->pSym->flags & CG_STAB_HAS_BDY))
        {
            CCodeInfo* pPouInfo = pgctx->getPOUInfoBySym(pEntry->pType->pSym);
            assert(pPouInfo);
            if(!pPouInfo->m_pDmp)
            {
                long                fbid;
                long                minVer;
                long                maxVer;
                bool                bExtLib;
                const CG_SymEntry*  pE2;

                pPouInfo->m_pDmp = new CGT_CDmp;

                fbid = VMGetIdOfMacroFB(pgctx, pEntry->pType->pSym, &bExtLib, &minVer, &maxVer);

                if(pTrg && minVer!=0 && pTrg->getTrgVer()<(unsigned long)minVer)
                {
                    pgctx->m_dm.errNS1(
                        &pEntry->edp, pgctx->m_dm.res1(VM_RES_STR_NS_FEATURE_VER_MIN).fmtStr3(
                                NULL, 
                                pgctx->m_dm.jot1()<<"'"<<pEntry->pType->pSym->pszId<<"'", 
                                pgctx->m_dm.jot2()<<pTrg->getTrgVer(), 
                                pgctx->m_dm.jot3()<<minVer
                              ) 
                    );
                }

                if(pTrg && pTrg->getTrgVer()>(unsigned long)maxVer)
                {
                    pgctx->m_dm.errNS1(
                        &pEntry->edp, pgctx->m_dm.res1(VM_RES_STR_NS_FEATURE_VER_MAX).fmtStr3(
                                NULL, 
                                pgctx->m_dm.jot1()<<"'"<<pEntry->pType->pSym->pszId<<"'", 
                                pgctx->m_dm.jot2()<<pTrg->getTrgVer(), 
                                pgctx->m_dm.jot3()<<maxVer
                              ) 
                    );
                }

                (*pPouInfo->m_pDmp)<<_T(".fb ");
                pPouInfo->m_pDmp->lo(pEntry->pType->pSym->pszId)<<' '<<fbid<<'\n';

                pPouInfo->m_pDmp->incrIndent();
                    for(pE2=pEntry->pType->pSym->pFirst; pE2; pE2=pE2->pNext)
                        CG_VM_DumpOneVariable(*pPouInfo->m_pDmp, pgctx, pTrg, pE2, false);
                pPouInfo->m_pDmp->decrIndent();
                (*pPouInfo->m_pDmp)<<_T(".end\n\n");
            }
            //else: type name already seen
        }
    }

    //force byte alignmet for any RETAIN decl section in Prgs, FBs:
    //ignore NON_RETAIN here because FBI instance retain is not sup any way..
    //this is only needed if this var is a BOOL and the next is also a BOOL
    //with different RETAIN CONSTANT modifiers
    if(bInRetain)
    {
        dmp<<_T(";end retain\n");
        if(pEntry_last && pEntry_last->pType->type==CG_BOOL)
            dmp<<_T("align8\n");
    }
    else if(bInConstRetain)
    {
        dmp<<_T(";end constant retain\n");
        if(pEntry_last && pEntry_last->pType->type==CG_BOOL)
            dmp<<_T("align8\n");
    }


    return bOk;
}




static void SFC_dumpHeader(CGT_CDmp&           dmp,
                           const CG_SFCStruct* pSFCS)
{
    dmp << _T("var int __nsteps      := int#") << pSFCS->nSteps << '\n';
    dmp << _T("var int __ntrans      := int#") << pSFCS->nTrans << '\n';
    dmp << _T("var int __nactions    := int#") << pSFCS->nActions << '\n';
    dmp << _T("var int __nnextsteps  := int#") << pSFCS->nNextSteps << '\n';
    dmp << _T("var int __nprevsteps  := int#") << pSFCS->nPrevSteps << '\n';
    dmp << _T("var int __nstepaction := int#") << pSFCS->nStepAction << '\n';
    dmp << _T("var int __initstep    := int#") << pSFCS->initStep << '\n';
    
    dmp << _T(";offsets\n");
    // dotransact
    dmp << _T("var int off__dotransact                     := offsetof(") SFC_DOTRANSACT _T("0)\n");
    // first step__t
    dmp << _T("var int off__step_t                         := offsetof(");
    dmp.lo(pSFCS->pStepNames[0]) << _T("__t)") CRS;
    // first step__x
    dmp << _T("var int off__step_x                         := offsetof("); 
    dmp.lo(pSFCS->pStepNames[0]) << _T("__x)") CRS;
    // first action__a
    dmp << _T("var int off__action_a                       := offsetof(");
    if (pSFCS->nActions == 0)
        dmp << _T("__count) ") << _T("; this is a dummy because there no actions") << CRS;
    else
        dmp.lo(pSFCS->pActionNames[0]) << _T("__a)") CRS;
    // first action__q
    dmp << _T("var int off__action_q                       := offsetof(");
    if (pSFCS->nActions == 0)
        dmp << _T("__count) ") << _T("; this is a dummy because there no actions") << CRS;
    else
        dmp.lo(pSFCS->pActionNames[0]) << _T("__q)") CRS;
    // first followTrans
    dmp << _T("var int off__followtrans                    := offsetof(__followtrans0)") CRS;
    // first AltTrans
    dmp << _T("var int off__alttrans                       := offsetof(__alttrans0)") CRS;
    // first NextSteps
    dmp << _T("var int off__nextsteps                      := offsetof(__nextsteps0)") CRS;
    // first NextStepsOffset
    dmp << _T("var int off__nextstepsoffset                := offsetof(__nextstepsoffset0)") CRS;
    // first PrevSteps
    dmp << _T("var int off__prevsteps                      := offsetof(__prevsteps0)") CRS;
    // first PrevStepsOffset
    dmp << _T("var int off__prevstepsoffset                := offsetof(__prevstepsoffset0)") CRS;
    // first StepActionBlocks
    dmp << _T("var int off__stepactionblocks               := offsetof(__stepactionblocks0)") CRS;
    // first StepActionBlockOffset
    dmp << _T("var int off__stepactionblockoffset          := offsetof(__stepactionblockoffset0)") CRS;
    // first StepActionBlockQualifier
    dmp << _T("var int off__stepactionblockqualifier       := offsetof(__stepactionblockqualifier0)") CRS;  
    // first StepActionBlockTimeParameter
    dmp << _T("var int off__stepactionblocktimeparameter   := offsetof(__stepactionblocktimeparameter0)") CRS;  
    // StepActionBlockOffsetParameter
    dmp << _T(";var int off__stepactionblockoffsetparameter := offsetof(");
    if (pSFCS->nActions == 0)
        dmp << _T("__count) ") << _T("; this is a dummy because there no actions") << CRS;
    else
        dmp.lo(pSFCS->pActionNames[0]) << _T("__q)") CRS;
    // private__xx
    dmp << _T("var int off__private_xx                     := offsetof(__private0__xx)") CRS;
}

static void SFC_dumpStaticSfcStructure(CGT_CDmp&           dmp,
                                       const CG_SFCStruct* pSFCS)
{
    // followTrans
    for (int i=0; i<pSFCS->nSteps; i++)
        dmp << _T("var int  __followtrans") << i << _T("                  := int#") 
            << pSFCS->pFollowTrans[i] << CRS;

    // AltTrans
    for (i=0; i<pSFCS->nTrans; i++)
        dmp << _T("var int  __alttrans") << i << _T("                     := int#") 
            << pSFCS->pAltTrans[i] << CRS;

    // NextSteps
    for (i=0; i<pSFCS->nNextSteps; i++)
        dmp << _T("var int  __nextsteps") << i << _T("                    := int#") 
            << pSFCS->pNextSteps[i] << CRS;

    // NextStepsOffset
    for (i=0; i<pSFCS->nTrans; i++)
        dmp << _T("var int  __nextstepsoffset") << i << _T("              := int#") 
            << pSFCS->pNextStepsOffset[i] << CRS;

    // PrevSteps
    for (i=0; i<pSFCS->nPrevSteps; i++)
        dmp << _T("var int  __prevsteps") << i << _T("                    := int#") 
            << pSFCS->pPrevSteps[i] << CRS;

    // PrevStepsOffset
    for (i=0; i<pSFCS->nTrans; i++)
        dmp << _T("var int  __prevstepsoffset") << i << _T("              := int#") 
            << pSFCS->pPrevStepsOffset[i] << CRS;

    // StepActionBlocks
    for (i=0; i<pSFCS->nStepAction; i++)
        dmp << _T("var int  __stepactionblocks") << i << _T("             := int#") 
            << pSFCS->pStepActionBlocks[i] << CRS;

    // StepActionBlockOffset
    for (i=0; i<pSFCS->nSteps; i++)
        dmp << _T("var int  __stepactionblockoffset") << i << _T("        := int#") 
            << pSFCS->pStepActionBlockOffset[i] << CRS;

    // StepActionBlockQualifier
    for (i=0; i<pSFCS->nStepAction; i++)
        dmp << _T("var int  __stepactionblockqualifier") << i << _T("     := int#") 
            << pSFCS->pStepActionBlockQualifier[i] << CRS;

    // StepActionBlockTimeParameter
    for (i=0; i<pSFCS->nStepAction; i++)
        dmp << _T("var time __stepactionblocktimeparameter") << i << _T(" := time#") 
            << pSFCS->pStepActionBlockTimeParameter[i] << _T("ms") CRS;
}

static void SFC_dumpInterStructure(CGT_CDmp&           dmp,
                                   const CG_SFCStruct* pSFCS)
{
    // private_xx
    // Gesamtlänge
    int iUserSpace =  sizeof(short) * (3 * pSFCS->nTrans + 4 * pSFCS->nActions)
                    + sizeof(short) * (3 * pSFCS->nSteps + pSFCS->nTrans + pSFCS->nActions)
                    + sizeof(long)  * (pSFCS->nSteps + pSFCS->nActions);

    for (int i=0; i<iUserSpace; i++)
    {
        dmp << _T("var int __private") << i << _T("__xx := int#") << 0 << CRS;
    }
}

static void SFC_dumpVisData(CGT_CDmp&           dmp,
                            const CG_SFCStruct* pSFCS)
{
    dmp << _T("var string(255)   __sfcvis\n");
    dmp << _T("var byte          __dummy_sfcvis\n");
    dmp << _T("var string(255)   __onceforcedtransitions\n");
    dmp << _T("var byte          __dummy_onceforcedtransitions\n");
    dmp << _T("var string(255)   __forcedtransitions\n");
    dmp << _T("var byte          __dummy_forcedtransitions\n");
    dmp << _T("var string(255)   __blockedtransitions\n");
    dmp << _T("var byte          __dummy_blockedtransitions\n");
    dmp << _T("var string(255)   __onceforcedactions\n");
    dmp << _T("var byte          __dummy_onceforcedactions\n");
    dmp << _T("var string(255)   __forcedactions\n");
    dmp << _T("var byte          __dummy_forcedactions\n");
    dmp << _T("var string(255)   __blockedactions\n");
    dmp << _T("var byte          __dummy_blockedactions\n");
    dmp << _T("var string(255)   __mancmd\n");
    dmp << _T("var byte          __dummy_mancmd\n");
    dmp << _T("var bool          __manualmode\n");
    dmp << _T("var bool          __donext\n");
    dmp << _T("var bool          __alltransitionson\n");
    dmp << _T("var bool          __allactionsoff\n");
    dmp << _T("var bool          __resetSFC:=true\n");  // should be implem. in private data in future
    dmp << _T("var time          __actTime\n");   // should be implem. in private data in future
    for (int i=0; i<29; i++)
        dmp << _T("var dword       __ptr_") << i << _T(" := dword#0 ;private pointer\n");  // should be implem. in private data in future
}

static void SFC_dumpDoTransAct(CGT_CDmp&           dmp,
                               const CG_SFCStruct* pSFCS)
{
    for (int i=0; i<max(pSFCS->nActions, pSFCS->nTrans) + 1; i++)
        dmp << _T("var int __dotransact") << i << CRS;
}

static void SFC_dumpTransActAttributes(CGT_CDmp&           dmp,
                                       const CG_SFCStruct* pSFCS)
{
    // STEPS
    // time qualifier
    for (int i=0; i<pSFCS->nSteps; i++)
    {
        dmp << _T("var time "); 
        dmp.lo(pSFCS->pStepNames[i]) << _T("__t") CRS;
    }

    // exec qualifier
    for (i=0; i<pSFCS->nSteps; i++)
    {
        dmp << _T("var byte ");
        dmp.lo(pSFCS->pStepNames[i]) << _T("__x") CRS;
    }

    // ACTIONS
    // ??????????? exec qualifier
    for (i=0; i<pSFCS->nActions; i++)
    {
        dmp << _T("var byte ");
        dmp.lo(pSFCS->pActionNames[i]) << _T("__a") CRS;
    }

    // ??????????? exec qualifier
    for (i=0; i<pSFCS->nActions; i++)
    {
        dmp << _T("var byte ");
        dmp.lo(pSFCS->pActionNames[i]) << _T("__q") CRS;
    }
}

static void SFC_dumpBody(
    CGT_CDmp&           dmp,
    const CG_SFCStruct* pSFCS,
    BODY_CTX_INFO*      pctx
)
{
    const CG_STStmt* pStmts; // NULL terminated list
    CG_Edipos        edp;
    CG_ExprMem       mem(&edp, pctx->pSym);
    edp.typ     = CG_EDP_ST;
    edp.pszFile = _T(":!gen");//TODO HACK 
    edp.lc.c = edp.lc.l =0;

    if (pctx->BPoption != BP_OPT0)
    {
        DumpBPFun(dmp, pctx, pctx->pIBdy->edpBeg(), 0, false);
        DumpBPFun(dmp, pctx, pctx->pIBdy->edpBeg(), pctx->pInf->m_lastBPNum++, true);//<-only for NCC
    }

	pStmts = pctx->pIBdy->getExceptionAction();
	if(pStmts)
	{
		dmp << _T(";///////////////////// exception action\n");
		pctx->bTransi = false;
		pctx->bAction = false;
		pctx->bExceptionAction = true;
		VMDmpStmt(dmp, pctx->pIBdy->getExceptionAction(), pctx);
	}


    dmp << _T(";///////////////////// calcdotrans\n");
    //TODO sfc_calcdotrans should not return a bool value for the jmpcn any longer
    //but in order to NOT change the firmware simply remove the jmpcn and clear the stack using
    // "CALF_SL       72 6" instead of "CALF_SL       72 4" that will clear the stack.
    VMDmpOPC(dmp, _T("PSHC_00_16"))<<'\n';
    VMDmpOPC(dmp, _T("LEAD_IN"))<<_T("0 ; &instance;\n");
    VMDumpInternalSyslibCall1(dmp, pctx, _T("sfc_finalise"), 72, true, CG_INT, CG_STRUCT);

    pStmts = createDoTransStmts(&edp, &mem, pSFCS, pctx);
    pctx->bTransi = true;
    pctx->bAction = false;
    pctx->bExceptionAction = false;
    VMDmpStmt(dmp, pStmts, pctx);

    dmp << _T(";///////////////////// calcdoact\n");
    VMDmpOPC(dmp, _T("LEAD_IN"))<<_T("0 ; &instance;\n");
    VMDumpInternalSyslibCall1(dmp, pctx, _T("sfc_calcdoact"), 73, false, CG_VOID, CG_STRUCT);

    dmp << _T(";///////////////////// finalise\n");
    VMDmpOPC(dmp, _T("LEAD_IN"))<<_T("0 ; &instance;\n");
    VMDumpInternalSyslibCall1(dmp, pctx, _T("sfc_finalise"), 74, false, CG_VOID, CG_STRUCT);

    pStmts = createDoActStmts(&edp, &mem, pSFCS, pctx);
    pctx->bTransi = false;
    pctx->bAction = true;
    pctx->bExceptionAction = false;
    VMDmpStmt(dmp, pStmts, pctx);

    //only for NCC to seperate the RET from the last stmt (ret cannot be converted to native):
    if (pctx->BPoption == BP_OPT0)
        DumpBPFun(dmp, pctx, pctx->pIBdy->edpBeg(), pctx->pInf->m_lastBPNum++, true);
    dmp << _T(":") << CG_VM_LABEL_POUEND _T("\n");

    if (pctx->BPoption != BP_OPT0)
    {
        DumpBPFun(dmp, pctx, pctx->pIBdy->edpEnd(), 1, false);
    }
}


bool handleLicenseForDirectAddress(
    const CG_SymEntry*  pEntry,
    CGlobCtx*           pgctx,
    CTarget*            pTrg
)
{
    if(pgctx->getLicData()->bIsStartKit)
    {
        // allow only 6 boolean I/O
        switch(pEntry->pType->type)
        {
        case CG_BOOL:
        case CG_BYTE:
        case CG_WORD:
        case CG_DWORD:
        case CG_INT:
        case CG_UINT:
        case CG_DINT:
        case CG_UDINT:
        case CG_REAL:
        case CG_LREAL:
        case CG_TIME:

            if (pEntry->pType->bIsArray)
                pTrg->incNumIOBits(VMSimpleTypeBitSize(pEntry->pType->type) * pEntry->pType->nElems);
            else
                pTrg->incNumIOBits(VMSimpleTypeBitSize(pEntry->pType->type));
        break;
        case CG_STRING:
            pTrg->incNumIOBits(pEntry->pType->lStrLen*8);
        break;
        case CG_STRUCT:
        {
            const CG_SymEntry* pE2;
            long			   lIECBits = 0;

            for(pE2=pEntry->pType->pSym->pFirst; pE2; pE2=pE2->pNext)
            {
                if(pE2->pType->pSym || pE2->pType->bIsArray)
                {
			        pgctx->m_dm.errNS1(&pEntry->edp, pgctx->m_dm.jot1().loadRes(VM_RES_STR_NS_STRUCTAT_NOSIMPLE));
                    return false;
                }
                lIECBits += VMSimpleDTypeBitSize(pE2->pType);
            }
            pTrg->incNumIOBits(lIECBits);
        }
        break;
        default:
            CGT_IERR(&pgctx->m_dm, _T("invalid data type"));
        }
    }
    return true;
}


static const TCHAR* VMGetStorageClassString(CG_DECLSCOPE store)
{
    switch(store)
    {
    case CG_VAR:             return _T("var    ");
    case CG_VAR_INPUT:       return _T("in     ");
    case CG_VAR_OUTPUT:      return _T("out    ");
    case CG_VAR_IN_OUT:      return _T("inout  ");
    case CG_VAR_GLOBAL:      return _T("");
    case CG_VAR_EXTERNAL:    return _T("");
    case CG_CONST:           return _T("const  ");
    case CG_VAR_TEMP:        return _T("temp   ");
    case CG_VAR_IN_EXT:      return _T("inext  ");
    case CG_DECLSCOPE_UNDEF:
    default:
        assert(!"bad store class");
        return _T("internal error");
    }
}



static CGT_CDmp& CG_VM_DumpBinVal(CGT_CDmp& dmp, const CG_SymEntry* pEntry)
{
    if (pEntry->pInit->ut == CG_INIT_SIMPLE)
    {
        const CG_Literal* pLit = pEntry->pInit->sim.pLit;
        VMDumpLitValue(dmp, pEntry->pInit->sim.pLit);
    }
    else if (pEntry->pInit->ut == CG_INIT_ARRAY)
    {
        assert(pEntry->pInit->arr.pInits->ut != CG_INIT_STRUCT);
        VMDumpArrayInit(dmp, &pEntry->pInit->arr);
    }
    return dmp;
}

static CGT_CDmp& VMDmpAddr(
    CGT_CDmp&         dmp,
    const VM_Address* pVMAddr 
)
{
    assert(pVMAddr->ulByteOffs != -1);


    dmp<<'%'<<pVMAddr->chSize<<pVMAddr->vmSegEnum<<'.'<<pVMAddr->ulByteOffs;

    if(pVMAddr->chSize=='X')
        dmp<<'.'<<(unsigned long)pVMAddr->ucBitPos;

    return dmp;
}


static bool VMCheckMaxArraySize(CGlobCtx* pgctx, const CG_SymEntry* pEntry)
{
    if(pEntry->pType->bIsArray && pEntry->pType->nElems>0)
    {
        long lBytes = VMDTypeFullByteSize(pgctx, &pEntry->edp, pEntry->pType);
        //total size must be < seg max size:
        if(lBytes > VM_SEGMENT_SIZE-1)//must -1 because the VM stores the segm size in 
                                      //a WORD(!) so 0xffff+1==65536 cannot be represented
        {
            pgctx->m_dm.msg2(CG_E_TOO_MANY_ARRAY_ELEMS, &pEntry->edp, 
                pgctx->m_dm.jot1()<<lBytes<<_T(" bytes"), 
                pgctx->m_dm.jot2()<<VM_SEGMENT_SIZE-1<<_T(" bytes")
            );
            return false;
        }

        //not more than a Word of elems:
        if(pEntry->pType->nElems > (1<<VM_ARRAY_LIMIT_SIZE)+1)
        {
            pgctx->m_dm.msg2(CG_E_TOO_MANY_ARRAY_ELEMS, &pEntry->edp, 
                pgctx->m_dm.jot1()<<lBytes, pgctx->m_dm.jot2()<<(1<<VM_SEGMENT_SIZE)+1
            );
            return false;
        }
    }
    return true;
}

bool CG_VM_DumpOneVariable(
    CGT_CDmp&           dmp,
    CGlobCtx*           pgctx,
    CTarget*            pTrg, //can be NULL if build libs !!
    const CG_SymEntry*  pEntry,
    bool                bDumpExternals
)
{
    CG_DECLSCOPE    store   = pEntry->declScope;
    const CG_DType* pType   = pEntry->pType;
    CG_DTYPES       typ     = pType->type;
    const TCHAR*    pszId   = pEntry->pszId;
    LPCTSTR         pszAddr = NULL;
    CGT_CDiagMsg*   pdm     = &pgctx->m_dm;
    bool            bIsFunction = pEntry->pParent->pouTyp==CG_POU_FUN;


    // void variables are no-variables
    if (typ == CG_VOID)
        return true;

    // check whether EGDE featerer is use outside of library
    if (pEntry->pType->type == CG_BOOL_R_EDGE || pEntry->pType->type == CG_BOOL_F_EDGE)
    {
        if (pEntry->pParent->flags & CG_STAB_HAS_BDY)
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_EDGE_IN_FB));
    }

    //select action:
    switch(store)
    {
        case CG_VAR:
        case CG_VAR_INPUT:
        case CG_VAR_IN_EXT:
        case CG_VAR_OUTPUT:
        case CG_VAR_GLOBAL:
        case CG_VAR_IN_OUT:   break;
        case CG_VAR_EXTERNAL:   
        break;
        case CG_CONST:         return true;
        default:
            assert(!"bad enum");
    }

    
    // CHECK RESTRICTIONS
    if(pEntry->pType->bIsArray)
        VMCheckMaxArraySize(pgctx, pEntry);

    if(pEntry->declMod & CG_DMOD_RETAIN)
    {
        if(pgctx->m_kad.getVmSettings()->szGMRSegSize.isEmpty())
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_RETAIN_VARS));
        }
        else if (store == CG_VAR_GLOBAL || store == CG_VAR_EXTERNAL)
        {
            //Global Retain restrictions:
            assert(store ==CG_VAR_GLOBAL); //var external should not have a declMod !!
            if((pEntry->pType->pSym || pEntry->pType->bIsArray) && 
                (pTrg && pTrg->getTrgVer() < 20500)
              )
            {
                pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_CMPLX_RETAIN));
                return false;
            }
        }
        else
        {
            //Local Retain restrictions:
            //if ver<20500: only simple global variables can be retain
            if(!pgctx->m_kad.getLocalRetain() || (pTrg && pTrg->getTrgVer() < 20500))
            {
                pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_LOCAL_RETAIN));
                return false;
            }
        }



        if(pEntry->pType->type==CG_INSTANCE)
        {
            //TODO: FBI retain declaration
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_NS_FBI_RETAIN));
            return false;
        }

        // no direct address for RETAIN in all current versions 
        if(pEntry->pszAddr)
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_RETAIN_DIRADDR));
            return false;
        }
    }
    else if(pEntry->declMod & CG_DMOD_NON_RETAIN)
    {
        if (pEntry->pParent->flags & CG_STAB_HAS_BDY)
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_NS_NON_RETAIN));
    }



    if (   pEntry->pszAddr != NULL
        && (pEntry->declMod & CG_DMOD_SYS_VAR) == 0
        && pEntry->pParent->pouTyp != CG_POU_GVL)
    {
        pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_AT_IN_PRG));
        return false;
    }
    
    
    if(bIsFunction)
    {
        if((pEntry->pParent->flags & CG_STAB_HAS_BDY)!=0 && CG_IsMultiElemType(pEntry->pType) &&
            (store!=CG_VAR_IN_OUT && store!=CG_VAR_EXTERNAL && store!=CG_VAR_INPUT && store!=CG_VAR_IN_EXT)
          )
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_LOC_CPLX_TYPE));
            return false;
        }

        if(pTrg && pTrg->getTrgVer() < 20700 && pEntry != pEntry->pParent->pFirst
                && (store==CG_VAR_OUTPUT || store==CG_VAR_IN_OUT)
                && pEntry->pType->type==CG_BOOL && ! pEntry->pType->bIsArray
          )
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_BOOL_FUN_VARIO));
            return false;
        }
    }
    else
    {
        if (pTrg && pTrg && pTrg->getTrgVer() < 20700 
            && store==CG_VAR_IN_OUT 
            && pEntry->pType->type==CG_BOOL && !pEntry->pType->bIsArray)
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_BOOL_WITH_INOUT));
            return false;
        }
    }





   
    switch(store)
    {
    case CG_VAR:
        dmp << _T("var    ");
        VMDmpType(dmp, pType) << _T(' ');
        dmp.lo(pEntry->pszId);
        
        if(pEntry->pszAddr && ! pgctx->getLicData()->bDemo)
        {
            VM_Address  vmAddr(pEntry);

            if (!pTrg)//HACK
            {
                pgctx->m_dm.msg2(CG_E_INVALID_ADDR, &pEntry->edp, pEntry->pszAddr, 
                    pgctx->m_dm.res1(CG_RES_STR_ADR_NO_TRG));
            }
            else if (pTrg->checkAddr(pgctx, pEntry, &vmAddr))
            {
                if (   pgctx->getLicData()->bConsiderLic
                    && ! handleLicenseForDirectAddress(pEntry, pgctx, pTrg))
                {
                    return false;
                }

                if (pgctx->getLicData()->bConsiderLic && pgctx->getLicData()->bDemo)
                {
                    // only non demo versions can have % addresses
                    if ( ! pTrg->isDEMOErrMsgDisplayed())
                    {
                        pgctx->m_dm.msg1(CG_W_DEMO_USED_IO, 
                            &pEntry->edp, 
                            pTrg->getTrgName());

                        pTrg->setDEMOerrMsgDisplayed();
                    }
                }
                else
                {
                    VMDmpAddr(dmp<<_T(" at "), &vmAddr);
                }
            }
        }
        
        if(!bIsFunction && pEntry->pInit && !pEntry->pType->pSym)
        {
            dmp << _T(":=");
            CG_VM_DumpBinVal(dmp, pEntry);
        }//else: instances or function locals will be initialised elsewhere
        
        dmp << '\n';
        break;
    case CG_VAR_EXTERNAL:
        if (!bDumpExternals)
            break;
        // else fall through
    case CG_VAR_GLOBAL:
        VMDmpType(dmp, pType) << _T(' ');
        dmp.lo(pEntry->pszId);
        
        if(pEntry->pszAddr && ! pgctx->getLicData()->bDemo)
        {
			const VM_Address* pVMAddr = pgctx->getDirectVars()->get(pEntry);
            VMDmpAddr(dmp<<_T(" at "), pVMAddr)<<' ';
        }
        
        if(pEntry->pInit)
        {
            if (pEntry->pszAddr && pEntry->pType->type==CG_STRING)
            {
                if(pTrg && pTrg->getTrgVer() < 22000)
                {
                    pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_INIT_VAR_DADDR));
                    return false;
                }
            }
           
            if (!bIsFunction && !pEntry->pType->pSym)
            {
                dmp << _T(":=");
                CG_VM_DumpBinVal(dmp, pEntry);
            }//else: instances or function locals will be initialised elsewhere
        }

        dmp << '\n';
        break;
    case CG_CONST:
        return true;
    case CG_VAR_INPUT:
    case CG_VAR_OUTPUT:
    case CG_VAR_IN_OUT:
    case CG_VAR_TEMP:
    case CG_VAR_IN_EXT:
        dmp << VMGetStorageClassString(store);
        VMDmpType(dmp, pType) << _T(' ');
        dmp.lo(pEntry->pszId);
        
        pszAddr = pEntry->pszAddr;
        if(pszAddr)
        {
            pdm->msg2(CG_E_ADDR_ILL_USE, &pEntry->edp, 
                pdm->jot1()<<pEntry->pszId<<_T(" AT ")<<pszAddr,
                CGT_StoreClassName(store));
            return false;
        }
        
        if(!bIsFunction && pEntry->pInit && !pEntry->pType->pSym)
        {
            dmp << _T(":=");
            CG_VM_DumpBinVal(dmp, pEntry);
        }//else: instances or function locals will be initialised elsewhere
        
        dmp << '\n';
        break;
    default:
        assert( ! _T("invalid decl scope"));
        CGT_IERR(pdm, pdm->jot1() << _T("4VM: fatal: unknown storage class\r\n"));
    }

    return true;
}




bool VMCheckDecl(
    CGlobCtx*           pgctx,
    CTarget*            pTrg, //can be NULL if build libs !!
    const CG_SymEntry*  pEntry
)
{
    CG_DECLSCOPE    store   = pEntry->declScope;
    const CG_DType* pType   = pEntry->pType;
    CG_DTYPES       typ     = pType->type;
    const TCHAR*    pszId   = pEntry->pszId;
    CGT_CDiagMsg*   pdm     = &pgctx->m_dm;
    bool            bIsFunction = pEntry->pParent->pouTyp==CG_POU_FUN;


    // void variables are no-variables
    if (typ == CG_VOID)
        return true;

    // check whether EGDE featerer is use outside of library
    if (pEntry->pType->type == CG_BOOL_R_EDGE || pEntry->pType->type == CG_BOOL_F_EDGE)
    {
        if (pEntry->pParent->flags & CG_STAB_HAS_BDY)
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_EDGE_IN_FB));
    }

    //select action:
    switch(store)
    {
        case CG_VAR:
        case CG_VAR_INPUT:
        case CG_VAR_IN_EXT:
        case CG_VAR_OUTPUT:
        case CG_VAR_GLOBAL:
        case CG_VAR_IN_OUT:   break;
        case CG_VAR_EXTERNAL:   
        break;
        case CG_CONST:         return true;
        default:
            assert(!"bad enum");
    }
    
    // CHECK RESTRICTIONS
    if(pEntry->pType->bIsArray)
        VMCheckMaxArraySize(pgctx, pEntry);


    if(pEntry->declMod & CG_DMOD_RETAIN)
    {
        if(pgctx->m_kad.getVmSettings()->szGMRSegSize.isEmpty())
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_RETAIN_VARS));
        }
        else if (store == CG_VAR_GLOBAL || store == CG_VAR_EXTERNAL)
        {
            //Global Retain restrictions:
            assert(store ==CG_VAR_GLOBAL); //var external should not have a declMod !!
            if((pEntry->pType->pSym || pEntry->pType->bIsArray) && 
                (pTrg && pTrg->getTrgVer() < 20500)
              )
            {
                pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_CMPLX_RETAIN));
                return false;
            }
        }
        else
        {
            //Local Retain restrictions:
            //if ver<20500: only simple global variables can be retain
            if(!pgctx->m_kad.getLocalRetain() || (pTrg && pTrg->getTrgVer() < 20500))
            {
                pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_LOCAL_RETAIN));
                return false;
            }
        }



        if(pEntry->pType->type==CG_INSTANCE)
        {
            //TODO: FBI retain declaration
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_NS_FBI_RETAIN));
            return false;
        }

        // no direct address for RETAIN in all current versions 
        if(pEntry->pszAddr)
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_NS_RETAIN_DIRADDR));
            return false;
        }
    }
    else if(pEntry->declMod & CG_DMOD_NON_RETAIN)
    {
        if (pEntry->pParent->flags & CG_STAB_HAS_BDY)
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_NS_NON_RETAIN));
    }



    if (   pEntry->pszAddr != NULL
        && (pEntry->declMod & CG_DMOD_SYS_VAR) == 0
        && pEntry->pParent->pouTyp != CG_POU_GVL)
    {
        pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_AT_IN_PRG));
        return false;
    }
    
    
    if(bIsFunction)
    {
        if((pEntry->pParent->flags & CG_STAB_HAS_BDY)!=0 && CG_IsMultiElemType(pEntry->pType) &&
            (store!=CG_VAR_IN_OUT && store!=CG_VAR_EXTERNAL && store!=CG_VAR_INPUT && store!=CG_VAR_IN_EXT)
          )
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_LOC_CPLX_TYPE));
            return false;
        }

        if(pTrg && pTrg->getTrgVer() < 20700 && pEntry != pEntry->pParent->pFirst
                && (store==CG_VAR_OUTPUT || store==CG_VAR_IN_OUT)
                && pEntry->pType->type==CG_BOOL && ! pEntry->pType->bIsArray
          )
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(VM_RES_STR_BOOL_FUN_VARIO));
            return false;
        }
    }
    else
    {
        if (pTrg && pTrg->getTrgVer() < 20700 
            && store==CG_VAR_IN_OUT 
            && pEntry->pType->type==CG_BOOL && !pEntry->pType->bIsArray)
        {
            pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_BOOL_WITH_INOUT));
            return false;
        }
    }

   
    switch(store)
    {
    case CG_VAR:
        if(pEntry->pszAddr && ! pgctx->getLicData()->bDemo)
        {
            VM_Address  vmAddr(pEntry);

            if (!pTrg)//HACK
            {
                pgctx->m_dm.msg2(CG_E_INVALID_ADDR, &pEntry->edp, pEntry->pszAddr, 
                    pgctx->m_dm.res1(CG_RES_STR_ADR_NO_TRG));
            }
            else if (pTrg->checkAddr(pgctx, pEntry, &vmAddr))
            {
                if (   pgctx->getLicData()->bConsiderLic
                    && ! handleLicenseForDirectAddress(pEntry, pgctx, pTrg))
                {
                    return false;
                }

                if (pgctx->getLicData()->bConsiderLic && pgctx->getLicData()->bDemo)
                {
                    // only non demo versions can have % addresses
                    if ( ! pTrg->isDEMOErrMsgDisplayed())
                    {
                        pgctx->m_dm.msg1(CG_W_DEMO_USED_IO, 
                            &pEntry->edp, 
                            pTrg->getTrgName());

                        pTrg->setDEMOerrMsgDisplayed();
                    }
                }
            }
        }  
        break;
    case CG_VAR_EXTERNAL:
    case CG_VAR_GLOBAL:
        if(pEntry->pInit)
        {
            if (pEntry->pszAddr && pEntry->pType->type==CG_STRING)
            {
                if(pTrg && pTrg->getTrgVer() < 22000)
                {
                    pdm->errNS1(&pEntry->edp, pdm->res1(CG_RES_STR_INIT_VAR_DADDR));
                    return false;
                }
            }
        }

    break;
    case CG_CONST:
        return true;
    case CG_VAR_INPUT:
    case CG_VAR_OUTPUT:
    case CG_VAR_IN_OUT:
    case CG_VAR_TEMP:
    case CG_VAR_IN_EXT:
        if(pEntry->pszAddr)
        {
            pdm->msg2(CG_E_ADDR_ILL_USE, &pEntry->edp, 
                pdm->jot1()<<pEntry->pszId<<_T(" AT ")<<pEntry->pszAddr,
                CGT_StoreClassName(store));
            return false;
        }
    break;
    default:
        assert( ! _T("invalid decl scope"));
        CGT_IERR(pdm, pdm->jot1() << _T("4VM: fatal: unknown storage class\r\n"));
    }

    return true;
}

static void VMDumpFunctionInits(
    const BODY_CTX_INFO* pctx,
    CGT_CDiagMsg*        pdm,
    CGT_CDmp&            dmp,
    const CG_Symtab*     pSym
)                             
{
    const CG_SymEntry*  pEntry;
    const CG_DType*     pType;
    CG_DECLSCOPE        ds;
    long                lSLen;
    const TCHAR*        pszCmnt = _T("; init local variables:\n");

    const FC_CHashPtr<
        const CG_ExprTree*,
        char*>::Pair*           pPair;


    //fun ret val first:
    pEntry = pSym->pFirst;
    pType = pEntry->pType;
    if(pType->bIsArray || pType->pSym)
    {
        assert(!"complex type in fun not supported");
        dmp<<"internal error";
    }
    else if(pType->type==CG_STRING)
    {
        if(pszCmnt)
        {
            dmp<<pszCmnt;
            pszCmnt = NULL;    
        }

        //fun return var string must have been 'len-field-initialized' by 
        //caller, now only set act len to 0: TODO set len-file directly without using ":=''"?
        VMDmpOPC(dmp, _T("PSHC_TX"))<<_T("''\n");
        VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16")).lo(pEntry->pszId)<<'\n';
        VMDmpOPC(dmp, _T("MOVE_STRING"))<<'\n';
    }
    //else: simple type (or void): need not to be init since we always PUSHIX_0 for the fun 
    //ret val on call. This may change if the calling convention for the ret val changes
    //or if only a LEAV -X would be used ...


    for(pEntry=pEntry->pNext; pEntry; pEntry=pEntry->pNext)
    {
        ds    = pEntry->declScope;
        pType = pEntry->pType;

        // only VAR_OUTPUT and VAR are initialised:
        if(!(ds==CG_VAR_OUTPUT || ds==CG_VAR))
            continue;   

        if(pszCmnt)
        {
            dmp<<pszCmnt;
            pszCmnt = NULL;    
        }
        //don't use VMAssign(dmp, pctx, &varExpr, &litExpr); reason: strings must be 
        //INIT_STRING and not MOVE_STRING in order do initialize the len field.
        //TODO: better memset local space to zero with one call and then use
        //a memset(list) opcode.?
        //Example code:
        //push          byte#0
        //pop           b
        //push          ''
        //lea           s
        //INIT_STRING   30
        //assert: currently only simple (and string) supported as function output/local:

        //simple pop or init_string or move_string:
        if(pType->pSym || pType->bIsArray)
		{
			assert(!"complex type in fun not supported");
            dmp<<"internal error";
		}
        else if(pType->type == CG_STRING)
        {
            //push value: init or empty string
            const TCHAR* pszInitStr = _T("''");
            if(pEntry->pInit)
            {
                assert(pEntry->pInit->ut==CG_INIT_SIMPLE);
                pszInitStr = pEntry->pInit->sim.pLit->psz;
            }

            VMDmpOPC(dmp, _T("PSHC_TX"))<<pszInitStr<<'\n';

            if(ds==CG_VAR)
            {
                lSLen = pType->lStrLen != -1 ? pType->lStrLen : VM_MAX_STRLEN;
                VMDmpOPC(dmp, _T("LEAD_IN")).lo(pEntry->pszId)<<'\n';
                VMDmpOPC(dmp, _T("INIT_STRING"))<<lSLen<<'\n';
            }
            else
            {
                //VAR_OUTPUT must have been 'len-field-initialized' by caller, 
                //now only set act len to 0: TODO set len-file directly without using ":=''"?
                VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16")).lo(pEntry->pszId)<<'\n';
                VMDmpOPC(dmp, _T("MOVE_STRING"))<<'\n';
            }
        }
		else
        {
            //push value: init or 0
            const CG_Literal* pLitInit;
            if(pEntry->pInit)
            {
                assert(pEntry->pInit->ut==CG_INIT_SIMPLE);
                pLitInit = pEntry->pInit->sim.pLit;
            }
            else
                pLitInit = pctx->pIF->getDefaultInit(pType->type);
            
            
            VMPushLitValue(dmp, pctx, pLitInit);

            //normal simple values gets popped, outputs with popind, unless the fun ret: 
            if(ds==CG_VAR) //instance data
                VMDmpSizedOPC(dmp, _T("POPD_IN_"), pEntry->pType->type).lo(pEntry->pszId)<<'\n';
            else //output is a ptr:
                VMDmpSizedOPC(dmp, _T("POPI_IN_"), pEntry->pType->type).lo(pEntry->pszId)<<'\n';
        }
    }

    // initialise STRING temp vars
    for(pPair = pctx->pInf->m_hashTmpExpr.firstPair(); pPair; pPair = pPair->pNext)
    {
        assert(pPair->pKey->extyp == CG_TMPVAR);
        
        pType = pPair->pKey->pType;
        if(pType->type != CG_STRING)
            continue;

        if(pszCmnt)
        {
            dmp<<pszCmnt;
            pszCmnt = NULL;    
        }

        lSLen = pType->lStrLen != -1 ? pType->lStrLen : VM_MAX_STRLEN;
        assert(lSLen>0);
        
        VMDmpOPC(dmp, _T("PSHC_TX"))<<_T("''\n");
        VMDmpOPC(dmp, _T("LEAD_IN"));
        VMDmpTmpVarName(dmp, pctx->pgctx, pType, pPair->pKey->tmp.lTmpNum, false)<<'\n';
        VMDmpOPC(dmp, _T("INIT_STRING"))<<lSLen<<'\n';
    }
}

        
bool VMCalcDirectAddress(
	CGlobCtx*		    pgctx,
	CTarget*            pTrg,
	const CG_SymEntry*  pEntry,
	VM_Address*         pVMAddr
)
{
    const CG_DType* pType   = pEntry->pType;
    const TCHAR*    pszAddr = pEntry->pszAddr;
	const CG_Edipos*pEdp    = &pEntry->edpAddr;
	CGT_CDiagMsg*   pdm     = &pgctx->m_dm;

    // void variables are no-variables
    assert(pszAddr && pType->type!=CG_BOOL_R_EDGE || pType->type!=CG_BOOL_F_EDGE);

	if((pEntry->declMod & CG_DMOD_RETAIN))
	{
		pdm->errNS1(pEdp, pdm->res1(VM_RES_STR_NS_RETAIN_DIRADDR));
		return false;
	}

    if((pEntry->declMod & CG_DMOD_SYS_VAR) == 0 && pEntry->pParent->pouTyp != CG_POU_GVL)
    {
        pdm->errNS1(pEdp, pdm->res1(CG_RES_STR_AT_IN_PRG));
        return false;
    }


    if(pType->bIsArray)
    {
        if(pType->pSym)
        {
            pdm->errNS1(pEdp, pdm->res1(VM_RES_STR_NS_CMPLX_ARRAY_AT));
            return false;
        }

// OSAI 13/10/2008 - Eliminato test target, vedi anche case CG_STRING e l'"else".
//        bool bOsai  = ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("4cosai"));
//        bool bOsaiCE= ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("4COsaiCE"));//d-226
//        bool bWin   = ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("4cwin"));
//        bool bCmz   = ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("cmz"));
//        bool bCmz2  = ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("fct200"));
//        bool bCmz3  = ! _tcsicmp(pgctx->m_popt->szCurTrgTyp, _T("fct241"));
//        if(bOsai || bOsaiCE || bWin || bCmz || bCmz2 || bCmz3)
        {
            // check the data type against the %-address
            TCHAR cAddrBitSize = pszAddr[2];
            bool bError = false;
            if(cAddrBitSize != 'B') //d-2159
            {
                switch(pEntry->pType->type)
                {
                case CG_BOOL:    bError = cAddrBitSize != _T('X'); break;
                case CG_BYTE:    bError = cAddrBitSize != _T('B'); break;
                case CG_WORD:    bError = cAddrBitSize != _T('W'); break;
                case CG_DWORD:   bError = cAddrBitSize != _T('D'); break;
                case CG_INT:     bError = cAddrBitSize != _T('W'); break;
                case CG_UINT:    bError = cAddrBitSize != _T('W'); break;
                case CG_DINT:    bError = cAddrBitSize != _T('D'); break;
                case CG_UDINT:   bError = cAddrBitSize != _T('D'); break;
                case CG_REAL:    bError = cAddrBitSize != _T('D'); break;
                case CG_LREAL:   bError = cAddrBitSize != _T('L'); break;
                case CG_TIME:    bError = cAddrBitSize != _T('D'); break;
                case CG_STRING:  bError = cAddrBitSize != _T('S'); break;  //// if(!bOsai || !bOsaiCE) break; // else do default
                default: 
                    pdm->errNS1(pEdp, pdm->res1(VM_RES_STR_NS_TYPE_AT_ADDR)<<pEntry->pType->pszName);
                    return false;
                }
            }
            if(bError)
            {
                TCHAR strSizeSpec[2];
                strSizeSpec[0] = cAddrBitSize;
                strSizeSpec[1] = 0;
                pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszAddr, 
                    pdm->jot1().fmtStr2(pdm->res1(VM_RES_STR_WRONG_ARRAY_ADDR), strSizeSpec, pType->pszName));

                return false;
            }
        }
// OSAI 13/10/2008 - Eliminato test target, vedi sopra.
//        else
//        {
//            pdm->errNS1(pEdp, pdm->res1(VM_RES_STR_NS_ARRAY_AT_ADDR));
//            return false;
//        }
    }
    else
    {
        TCHAR cAddrBitSize = pszAddr[2];
        bool bErrorSize = false;
        bool bError = false;
        switch(pEntry->pType->type)
        {
        case CG_BOOL:    bErrorSize = cAddrBitSize != _T('X'); break;
        case CG_BYTE:    bErrorSize = cAddrBitSize != _T('B'); break;
        case CG_WORD:    bErrorSize = cAddrBitSize != _T('W'); break;
        case CG_DWORD:   bErrorSize = cAddrBitSize != _T('D'); break;
        case CG_INT:     bErrorSize = cAddrBitSize != _T('W'); break;
        case CG_UINT:    bErrorSize = cAddrBitSize != _T('W'); break;
        case CG_DINT:    bErrorSize = cAddrBitSize != _T('D'); break;
        case CG_UDINT:   bErrorSize = cAddrBitSize != _T('D'); break;
        case CG_REAL:    bErrorSize = cAddrBitSize != _T('D'); break;
        case CG_LREAL:   bErrorSize = cAddrBitSize != _T('L'); break;
        //case CG_TIME:
        case CG_STRING:  bErrorSize = cAddrBitSize != _T('S'); break;
        case CG_STRUCT:  bErrorSize = cAddrBitSize != _T('B'); break;
        default: 
            pdm->errNS1(pEdp, pdm->res1(VM_RES_STR_NS_TYPE_AT_ADDR)<<pEntry->pType->pszName);
            return false;
        }
        if(bErrorSize)
        {
            TCHAR strSizeSpec[2];
            strSizeSpec[0] = cAddrBitSize;
            strSizeSpec[1] = 0;
            pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszAddr, 
                pdm->jot1().fmtStr2(pdm->res1(VM_RES_STR_WRONG_ARRAY_ADDR), strSizeSpec, pType->pszName));
            return false;
        }
    }

    //d-203 HACK: error msg should be done in IO-Configurator if any ...
    //at the moment we do not support any !<modifier>
    const TCHAR* pszModifier = _tcschr(pszAddr, '!');
    if(pszModifier)
        pdm->errNS1(pEdp, pszModifier);


	if(!pTrg)//HACK
	{
		pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszAddr, pdm->res1(CG_RES_STR_ADR_NO_TRG));
		return false;
	}

    //check license stuff
	if(pgctx->getLicData()->bConsiderLic)
	{
		if(!handleLicenseForDirectAddress(pEntry, pgctx, pTrg))
			return true;

		if(pgctx->getLicData()->bDemo)
		{
			if(!pTrg->isDEMOErrMsgDisplayed())
			{
				pdm->msg1(CG_W_DEMO_USED_IO, pEdp, pTrg->getTrgName());
				pTrg->setDEMOerrMsgDisplayed();
			}
			return true;
		}
	}


	// CHECK ADDRESS
	if(!pTrg->checkAddr(pgctx, pEntry, pVMAddr))
		return true;

    return true;
}


const CG_STStmt* createDoTransStmts(CG_Edipos* pEdp, CG_ExprMem* pExprMem, const CG_SFCStruct* pSFCS, BODY_CTX_INFO* pctx)
{
    const CG_STStmt*    pFirstStmt;
    CG_STStmt*          pAsnStmt;
    CG_STStmt*          pWhileStmt;
    CG_STStmt*          pCaseStmt;
    CG_STStmt*          pExitStmt;
    CG_ExprTree*        countVar;
    CG_ExprTree*        countPlusOne;
    CG_ExprTree*        cArray;
    CG_STCaseList*      pCase;
    CG_STCaseList*      pNewCase;

    countVar               = pExprMem->registerCGExpr(CG_VARREF, pEdp, pExprMem->pIntType);
    countVar->var.accFlags = CG_ACCESS_R|CG_ACCESS_W;
    countVar->var.pEntry   = pExprMem->registerSymEntry(pEdp, _T("__count"), pExprMem->pIntType);
    
    countPlusOne           = pExprMem->registerCGExpr(CG_ADD, pEdp, pExprMem->pIntType);
    countPlusOne->ops.pLhs = countVar;
    countPlusOne->ops.pRhs = pExprMem->lit1;
    
    // i := 0; (* loop tmp 'i' *)
    pAsnStmt            = pExprMem->registerStmt();
    pFirstStmt          = pAsnStmt;
    pAsnStmt->styp      = CG_ASSIGN;
    pAsnStmt->edp       = *pEdp;
    pAsnStmt->asns.pLhs = countVar;
    pAsnStmt->asns.pRhs = pExprMem->lit0;
    
    // doTransAct[i] 
    cArray = pExprMem->registerCGExpr(CG_USEREXPR, pEdp, pExprMem->pIntType);
    cArray->usr.ldata = UE_SFC_INSTANCE_ARRAY;
    cArray->usr.psz1  = pExprMem->registerString (_T("__doTransAct0")); // base address of doTransAct array
    cArray->usr.psz2  = countVar->var.pEntry->pszId;
    
    // WHILE true DO
    pWhileStmt              = pExprMem->registerStmt();
    pAsnStmt->pNext         = pWhileStmt;
    pWhileStmt->styp        = CG_WHILE;
    pWhileStmt->edp         = *pEdp;
    pWhileStmt->whls.pWhile = pExprMem->litTrue;

    // CASE doTransAct[i] OF
    pCaseStmt               = pExprMem->registerStmt();
    pWhileStmt->whls.pStmts = pCaseStmt;
    pCaseStmt->styp         = CG_CASE;
    pCaseStmt->edp          = *pEdp;
    pCaseStmt->cases.pExpr  = cArray;

    //0: doTransAct[i] := bool_to_int(t1);
    //1: doTransAct[i] := bool_to_int(t2);
    //2: doTransAct[i] := bool_to_int(t3);
    //3: doTransAct[i] := bool_to_int(t4);
    //4: doTransAct[i] := bool_to_int(t5);
    //5: doTransAct[i] := bool_to_int(t6);
    //6: doTransAct[i] := bool_to_int(true);
    for (int i = 0; i < pSFCS->nTrans; i++)
    {
        pNewCase = pExprMem->registerCaseList(i, pctx->pIBdy->getTransi(i));
        if (i == 0)
            pCaseStmt->cases.pCases = pNewCase;
        else 
            pCase->pNextCase        = pNewCase;

        pCase      = pNewCase;
        pCase->edp = *pEdp;       
    }

    // ELSE  
    //    EXIT;
    pExitStmt                  = pExprMem->registerStmt();
    pCaseStmt->cases.pElse     = pExitStmt; 
    pExitStmt->styp            = CG_EXIT;
    pExitStmt->edp             = *pEdp;

    // i := i + 1;
    pAsnStmt            = pExprMem->registerStmt();
    pCaseStmt->pNext    = pAsnStmt;
    pAsnStmt->styp      = CG_ASSIGN;
    pAsnStmt->edp       = *pEdp;
    pAsnStmt->asns.pLhs = countVar;
    pAsnStmt->asns.pRhs = countPlusOne;

    return pFirstStmt;
}

const CG_STStmt* createDoActStmts(CG_Edipos* pEdp, CG_ExprMem* pExprMem, const CG_SFCStruct* pSFCS, BODY_CTX_INFO* pctx)
{
    const CG_STStmt*   pFirstStmt;
    CG_STStmt*         pWhileStmt;
    CG_STStmt*         pCaseStmt;
    CG_STStmt*         pActRetStmt;
    CG_STStmt*         pStmt;
    CG_ExprTree*       countVar;
    CG_ExprTree*       countPlusOne;
    CG_ExprTree*       pExpr;
    CG_STCaseList*     pCase;
    CG_STCaseList*     pNewCase;

    countVar               = pExprMem->registerCGExpr(CG_VARREF, pEdp, pExprMem->pIntType);
    countVar->var.accFlags = CG_ACCESS_R|CG_ACCESS_W;
    countVar->var.pEntry   = pExprMem->registerSymEntry(pEdp, _T("__count"), pExprMem->pIntType);
   
    countPlusOne           = pExprMem->registerCGExpr(CG_ADD, pEdp, pExprMem->pIntType);
    countPlusOne->ops.pLhs = countVar;
    countPlusOne->ops.pRhs = pExprMem->lit1;
    
    // doTransAct[i] 
    pExpr = pExprMem->registerCGExpr(CG_USEREXPR, pEdp, pExprMem->pIntType);
    pExpr->usr.ldata = UE_SFC_INSTANCE_ARRAY;
    pExpr->usr.psz1  = pExprMem->registerString (_T("__doTransAct0")); // base address of doTransAct array
    pExpr->usr.psz2  = countVar->var.pEntry->pszId;
    
    // i := 0;
    pStmt = pExprMem->registerStmt();
    pFirstStmt       = pStmt;
    pStmt->styp      = CG_ASSIGN;
    pStmt->edp       = *pEdp;
    pStmt->asns.pLhs = countVar;
    pStmt->asns.pRhs = pExprMem->lit0;
    
    // WHILE true DO
    pWhileStmt              = pExprMem->registerStmt();
    pStmt->pNext            = pWhileStmt;
    pWhileStmt->styp        = CG_WHILE;
    pWhileStmt->edp         = *pEdp;
    pWhileStmt->whls.pWhile = pExprMem->litTrue;

    // CASE doTransAct[i] OF
    pCaseStmt               = pExprMem->registerStmt();
    pWhileStmt->whls.pStmts = pCaseStmt;
    pCaseStmt->styp         = CG_CASE;
    pCaseStmt->edp          = *pEdp;
    pCaseStmt->cases.pExpr  = pExpr;

    //0:  actionbool := byte_to_bool(Action0__Q);
    //1:  var1       := byte_to_bool(Step1__X);
    //2:  var2       := Step1__T >= t#40ms;
    //3:  var2       := byte_to_bool(Action2__Q) AND byte_to_bool(Action3__A);
    //4:  var2       := false;
    for (int i = 0; i < pSFCS->nActions; i++)
    {
        pNewCase  = pExprMem->registerCaseList(i, pctx->pIBdy->getAction(i));
        if (i == 0)
            pCaseStmt->cases.pCases = pNewCase;
        else 
            pCase->pNextCase        = pNewCase;

        pCase         = pNewCase;
        pCase->edp    = *pEdp;       
    }

    // ELSE  
    //    EXIT;
    pStmt                  = pExprMem->registerStmt();
    pCaseStmt->cases.pElse = pStmt; 
    pStmt->styp            = CG_EXIT;
    pStmt->edp             = *pEdp;

    //##d-1454
    //@__action_return label, used for return stmt in action, 
    //such a return will be dumped as goto this label, don't exit the POU!
    pActRetStmt            = pExprMem->registerStmt();
    pCaseStmt->pNext       = pActRetStmt;
    pActRetStmt->styp      = CG_LABEL;
    pActRetStmt->edp       = *pEdp;
    pActRetStmt->lbls.bUsed= 1;
    pActRetStmt->lbls.nLblNum= -1;
    pActRetStmt->lbls.pszLbl= VM_LABEL_ACTION_RETURN;


    // i := i + 1;
    pStmt              = pExprMem->registerStmt();
    pActRetStmt->pNext = pStmt;
    pStmt->styp        = CG_ASSIGN;
    pStmt->edp         = *pEdp;
    pStmt->asns.pLhs   = countVar;
    pStmt->asns.pRhs   = countPlusOne;


    return pFirstStmt;
}

/* ---------------------------------------------------------------------------- */
