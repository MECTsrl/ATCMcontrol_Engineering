

//----  Includes:   -------------------------------------------*
#include <stdlib.h>
#include <wchar.h>

#include "cgt_misc.h"
#include "globctx.h"
#include "dumpjava.h"
#include "exprcode.h"
#include "helper.h"
#include "defines.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_hash.h"


//----  Local Defines:   -------------------------------------------*
//limits:
#define PC_MAX_JAVA_STACK_VARS  65000




//local helper:
void PC_DumpLookUpCode(
    CGT_CDmp &     dmp, 
    BODY_CTX_INFO* pBdyctx, 
    FC_CStrList*   pGlobFBnames
);
void PC_AddStmtToBody(
    CGT_CDmp&     dmp, 
    bool          bHasGoto, 
    CGT_CDmp&     tmpDmp,
    FC_CStrList*  pGlobFBnames
);

static void PC_DumpSFCBody(
    CGT_CDmp&               dmp, 
    ICG_Body*               pIBdy,
    CGT_CDiagMsg*           pdm,
    const CG_Symtab*        pSym,
    ICG_IECFront*           pIF,
    BP_OPTION               BPoption,
    FC_CGArray<CGT_BP_TYP>* pbpList,
    bool                    bJitMeNot
);
static void PC_AppendCalledFunMethods(
    CGlobCtx*               pgctx,
    CGT_CDmp&               dmpFBPrg,
    const CCodeInfo*const*  pzCalledFuns
);
static void PC_DumpTmpVariables(
    CGT_CDmp&   dmp,
    CBodyInfo*  pInf
);
static void PC_DumpNameArray(
    CGT_CDmp&                   dmp, 
    const CG_Symtab*            pSym
);
static bool PC_DumpOneVariableLookUpCode(
    CGT_CDmp&           dmp, 
    CGT_CDiagMsg*     pdm,
    const CG_SymEntry*  pEntry, 
    FC_CStrList*        pGlobFBnames
);
static void PC_DumpEdgeHelperClassVariables(
    CGT_CDmp&        dmp, 
    const CG_Symtab* pSym
);
static bool DumpFunctionMethodDecl(
    CGT_CDiagMsg*      pdm,
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    BP_OPTION          bpOpt
);
static void PC_DumpFunctionLocalVarsV20(
    ICG_IECFront*      pIF,
    CGT_CDiagMsg*      pdm,
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    FC_CStrList*       pGlobFBnames
);                             
static void PC_DumpFunctionLocalVarsForBP(
    ICG_IECFront*      pIF,
    CGT_CDiagMsg*      pdm,
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    FC_CStrList*       pGlobFBnames
);                             
static void PC_DumpQVTInitValue(
    CGT_CDmp& dmp, 
    CG_DTYPES type
);
enum PC_NATIVE_SCOPE{//##AC
    PC_NDECL_FILE,
    PC_NDECL_CLASS,
    //TODO PC_NDECL_METHOD
};
static void DumpVarNative(//##AC
    CGT_CDiagMsg*       pdm, 
    CGT_CDmp&           dmp, 
    const CG_Symtab*    pCaller,
    PC_NATIVE_SCOPE     scope
);




//get precompiled code from lib fun and fb,prg pass here:
void GenCodeLibPOU(
    CGlobCtx*               pgctx,
    const CG_Symtab*        pSym,
    CCodeInfo*              pInfo,
    FC_CGArray<CGT_BP_TYP>* pbpList
)
{
    long            nFiles;
    FC_CString      codeDomain;

    assert(pSym->pLibCtx);
  
    if(pSym->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR))
        return; //nothing to do...
    
    codeDomain.resize(MAX_PATH);
    codeDomain.load(pSym->pszId);
    
    if(pSym->pouTyp==CG_POU_FUN)
    {
        codeDomain<<_T(".")PC_INIT_JIF;
        codeDomain.toLower();
    }
    else
    {
        codeDomain<<_T(".class");
        codeDomain.toLower();
    }
    //TODO pInfo->m_jitmenot ????

    nFiles =  CGT_LIBCheckOutFiles(
        &pgctx->m_dm,          //for err msg
        pgctx->m_pIF,          //used to get 4cl COM IF by context info
        pSym->pLibCtx,         //4CL context info
        pSym->pszId,           //must be given 
        TARGET_EXT,            //can be NULL or empty string
        codeDomain,            //must be given
        pgctx->getClsGenPath() //dest directory
    );

    if(nFiles!=1)
    {
        assert(pgctx->m_dm.hasErr());
        pInfo->m_bErrors = true;
    }

    //pInfo filenames must be relative to gen:
    codeDomain.prepend(PC_GEN_CLASSES_DIR _T("\\"));
    if(pSym->pouTyp==CG_POU_FUN)
    {
        pInfo->m_jifFile.load(codeDomain);
    }
    else
    {
        pInfo->m_classFile.load(codeDomain);
    }

}



//entry function for this file: 
extern "C" void GenCodeStruct(
    CGlobCtx*                 pgctx,
    CCodeInfo*                pInfo
)
{
    //nothing to do for java...
}






//entry function for this file: 
extern "C" void GenCodeSFCPrgFB(
    CGlobCtx*                 pgctx,
    ICG_Body*                 pIBdy,
    const CCodeInfo*const*    pzCalledFuns,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
)
{
    FC_CString           jot(MAX_PATH);
    const CG_Symtab*     pSym;
    CGT_CDmp             dmp;
    const CG_KeyValPair* pAttr;
    bool                 bJitMeNot;


    //init local variables:
    pSym = pIBdy->getHeader();
    assert(pSym->pouTyp != CG_POU_SDT);

    pAttr = pgctx->m_pIF->lookUpAttrib(pSym->pAttribs, _T("jitmenot"));
    bJitMeNot = pAttr && !_tcsicmp(pAttr->pszVal, _T("on"));
    pInfo->m_jitmenot = bJitMeNot;


    //copy right and packages:
    dmp<<Get4CCopyRightSourceHeader();
    dmp<<'\n';
    dmp<<_T("package ") PACKAGE_BASE_PATH _T(".user;\n");
    dmp<<_T("import  ") PACKAGE_BASE_PATH _T(".util.*;\n");
    dmp<<_T("import  ") PACKAGE_BASE_PATH _T(".builtin.*;\n");
    dmp<<_T("\n\n\n");

    //optional leading NATIVE declarations:
    DumpVarNative(&pgctx->m_dm, dmp, pSym, PC_NDECL_FILE);//##AC

    //class definition:
    dmp<<_T("public final class ");
    dmp.lo(pSym->pszId)<<_T(" extends SFCFunctionBlock\n"CRLO"\n");
    dmp.incrIndent();

    //optional class NATIVE declarations:
    DumpVarNative(&pgctx->m_dm, dmp, pSym, PC_NDECL_CLASS);//##AC

    
    //dump the 'object number to identifier' array:
    PC_DumpNameArray(dmp, pSym);
    PC_DumpEdgeHelperClassVariables(dmp, pSym);

    //dump LD-trigger variables (if any):
    int nTrig = pIBdy->getUsedTriggers();
    if(nTrig)
    {
        dmp<<_T("//LD trigger tmp variables\n");
        for(int i=0; i<nTrig; i++)
            dmp<<_T("boolean ")JAVA_TRIGTMP_PRFIX<<i<<_T(";\n");
        dmp<<_T("\n");
    }



    //dump the sfc class methods
    PC_DumpSFCBody(dmp, pIBdy, &pgctx->m_dm, pSym, pgctx->m_pIF, 
                   pgctx->getBPOption(), pbpList, bJitMeNot);



    //dump called functions as class methods:
    dmp.decrIndent();
    PC_AppendCalledFunMethods(pgctx, dmp, pzCalledFuns);
    dmp.incrIndent();


    //close class definition:
    dmp.decrIndent();
    dmp<<_T(CRLC"\n");


    //dump the java file to disk:
    FC_CString javafile(MAX_PATH);
    javafile<<pSym->pszId<<_T(".java");
    CGT_DmpToFile(&pgctx->m_dm, pgctx->getClsGenPath(), javafile, dmp, false);

    //create gen dir relative file name for loader.list:
    jot.load(PC_GEN_CLASSES_DIR);
    jot.appendDir(pSym->pszId)<<_T(".class");
    jot.toLower();
    pInfo->m_classFile.load(jot);
}


static void DumpVarNative(//##AC
    CGT_CDiagMsg*       pdm, 
    CGT_CDmp&           dmp, 
    const CG_Symtab*    pCaller,
    PC_NATIVE_SCOPE     scope
)
{
    int              i;
    int              j;
    const CG_Symtab* pSym;
    const TCHAR*     pszN;
    TCHAR            szOnceTag[MAX_PATH];
    FC_CStrList      onceList;
    PC_NATIVE_SCOPE  scopeFound;
    bool             bAdded;
    bool             bIsIndent = dmp.getIndent()>0;

    bAdded = false;
    for(j=0;j<pCaller->nVarNative;j++)
    {
        pszN = pCaller->pVarNative[j].psz;
        
        scopeFound = FC_StringIncBegin(&pszN, _T("#movetop")) ? PC_NDECL_FILE : PC_NDECL_CLASS;
        if(scopeFound != scope)
            continue;

        while(*pszN==' '||*pszN=='\t' || *pszN=='\r')
            pszN++;
        if(*pszN=='\n')
            pszN++;

        dmp<<_T("//VAR_NATIVE");
        if(scopeFound == PC_NDECL_FILE)
            dmp<<_T("#movetop");
        dmp<<" from this POU '"<<pCaller->pszId<<_T("':\n");

        if(bIsIndent)
            dmp.decrIndent();
        
        dmp<<pszN;
  
        if(bIsIndent)
            dmp.incrIndent();

        if(dmp.column()>1)
            dmp<<'\n';
        bAdded = true;
    }

    for(i=0; (pSym = pCaller->pzRefSyms[i])!=NULL; i++)
    {
        if((pSym->flags & CG_STAB_MACRO) && pSym->nVarNative>0)
        {
            for(j=0;j<pSym->nVarNative;j++)
            {
                pszN = pSym->pVarNative[j].psz;
                
                scopeFound = FC_StringIncBegin(&pszN, _T("#movetop")) ? PC_NDECL_FILE : PC_NDECL_CLASS;
                if(scopeFound != scope)
                    continue;

                szOnceTag[0] = 0;
                if(FC_StringIncBegin(&pszN, _T("#once("))>=0)
                {
                    if(FC_StrGetID(&pszN, true, szOnceTag, FC_ARRAY_LEN(szOnceTag))==0 && *pszN==')')
                    {
                        pszN++;

                        if(onceList.find(szOnceTag))
                            continue;
                        onceList.add(szOnceTag);
                    }
                    else
                    {
                        pdm->msg0(CG_E_MACRO_EXPANSION, &pSym->pVarNative[j].edp);
                    }
                }
 
                
                while(*pszN==' '||*pszN=='\t' || *pszN=='\r')
                    pszN++;
                if(*pszN=='\n')
                    pszN++;

                dmp<<_T("//VAR_NATIVE");
                if(scopeFound == PC_NDECL_FILE)
                    dmp<<_T("#movetop");

                if(szOnceTag[0])
                    dmp<<_T("#once(")<<szOnceTag<<')';

                dmp<<_T(" from macro inline function '")<<pSym->pszId<<_T("':\n");
                
                if(bIsIndent)
                    dmp.decrIndent();
                
                dmp<<pszN;
            
                if(bIsIndent)
                    dmp.incrIndent();
                
                if(dmp.column()>1)
                    dmp<<'\n';

                bAdded = true;
            }
        }
    }

    if(bAdded)
        dmp<<'\n';
}

static bool GetTMPVarUse(void* pCtx, const CG_ExprTree* pExpr)
{
    const BODY_CTX_INFO* pctx = (const BODY_CTX_INFO*)pCtx;

    if(pExpr->extyp==CG_TMPVAR)
    {
        long accFlag = pExpr->tmp.accFlags;
        bool bNeedFCObj = !pExpr->tmp.pRefExpr && (accFlag & (CG_ACCESS_O|CG_ACCESS_X))!=0;
        pctx->pInf->newLDFBDTmpVar(pExpr, bNeedFCObj);
    }
    return true;
}

//entry function for this file: 
extern "C" void GenCodeSTPrgFB(
    CGlobCtx*                 pgctx,
    ICG_Body*                 pIBdy,
    const CCodeInfo*const*    pzCalledFuns,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
)
{
    FC_CString           jot(MAX_PATH);
    const CG_Symtab*     pSym;
    CGT_CDmp             dmp;
    const CG_KeyValPair* pAttr;


    //init local variables:
    pSym = pIBdy->getHeader();
    assert(pSym->pouTyp != CG_POU_SDT);

    pAttr = pgctx->m_pIF->lookUpAttrib(pSym->pAttribs, _T("jitmenot"));
    pInfo->m_jitmenot = pAttr && !_tcsicmp(pAttr->pszVal, _T("on"));


    //copy right and packages:
    dmp<<Get4CCopyRightSourceHeader();
    dmp<<'\n';
    dmp<<_T("package ") PACKAGE_BASE_PATH _T(".user;\n");
    dmp<<_T("import  ") PACKAGE_BASE_PATH _T(".util.*;\n");
    dmp<<_T("import  ") PACKAGE_BASE_PATH _T(".builtin.*;\n");
    dmp<<_T("\n\n\n");


    //optional leading NATIVE declarations:
    DumpVarNative(&pgctx->m_dm, dmp, pSym, PC_NDECL_FILE);//##AC

    //class definition:
    dmp<<_T("public final class ");
    dmp.lo(pSym->pszId)<<_T(" extends FunctionBlock\n"CRLO"\n");
    dmp.incrIndent();


    //optional NATIVE declarations:
    DumpVarNative(&pgctx->m_dm, dmp, pSym, PC_NDECL_CLASS);//##AC

    //dump the 'object number to identifier' array:
    PC_DumpNameArray(dmp, pSym);
    PC_DumpEdgeHelperClassVariables(dmp, pSym);

    //dump LD-trigger variables (if any):
    int nTrig = pIBdy->getUsedTriggers();
    if(nTrig)
    {
        dmp<<_T("//LD trigger tmp variables\n");
        for(int i=0; i<nTrig; i++)
            dmp<<_T("boolean ")JAVA_TRIGTMP_PRFIX<<i<<_T(";\n");
        dmp<<_T("\n");

    }


    //exec function:
    {
    CBodyInfo     bdyinf(pSym, pbpList, false);
    BODY_CTX_INFO bdyctx;
    CGT_CDmp      tmpDmp;
    FC_CStrList   globFBnames;

    bdyctx.pSym    = pSym;
    bdyctx.pIF     = pgctx->m_pIF;
    bdyctx.pdm     = &pgctx->m_dm;
    bdyctx.pIBdy   = pIBdy;
    bdyctx.nTransi = -1;
    bdyctx.nAction = -1;
    bdyctx.BPoption= pgctx->getBPOption();
    bdyctx.pInf    = &bdyinf;

    //get all tmp vars IL POUs:
	if(pIBdy->getHeader()->srcTyp == CG_SRC_TYPE_IL)
		CGT_EnumExprStmt(pIBdy->getSTStmts(), &bdyctx, GetTMPVarUse);

    if(pInfo->m_jitmenot)
        dmp<<_T("__attribute JITMeNot()\n");
    dmp<<_T("public void exec(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc)\n"CRLO"\n");
   
    tmpDmp.setIndent(2);

    //"begin of POU" BP function unless not option bp0,
    //use special BP number 0:
    if(bdyctx.BPoption!=BP_OPT0)
    {
        DumpBPFun(tmpDmp, pIBdy->edpBeg(), pSym, pbpList, 0);
        bdyinf.m_pLastBPEdp = pIBdy->edpBeg();
    }

    //dump body to tmpDmp stream first, for the java declaration
    //we need information that is easiest got by dumping 
    //the body first. All this information is stored in 
    //the bdyctx.pInf member.
    PC_DumpSTStmts(tmpDmp, pIBdy->getSTStmts(), &bdyctx);

    //"end of POU" BP function unless not option bp0,
    //use special BP number 1:
    if(bdyctx.BPoption!=BP_OPT0)
    {
        DumpBPFun(tmpDmp, pIBdy->edpEnd(), pSym, pbpList, 1);
        bdyinf.m_pLastBPEdp = pIBdy->edpEnd();
    }

	
    //dump lookup code & tmp vars (uses information generated by PC_DumpSTStmts):
    dmp.incrIndent();
	// for IL POUs declare tmp vars
	if(pIBdy->getHeader()->srcTyp == CG_SRC_TYPE_IL)
	{
		int j = bdyinf.m_ldfbdtmp.getUsed();
		if(j)
			dmp<<_T("//////////////// temp variables ////////////////////\n");
		for(int i=0;i<j;i++)
		{
			const TMP_VAR_TYP* pTmp = bdyinf.m_ldfbdtmp.getAddrOf(i);
			int bcol = dmp.column()+25;
			(dmp<<pTmp->pszType).sep(bcol);
			PC_DumpLDFBDStackTmpVarName(dmp, i+1);
			if(pTmp->bFCTmpObj)
			{
				dmp<<_T(" = new ");
				dmp<<ST_GetFunTempVarName(pTmp->pType);
				dmp<<_T("()");
			}
			dmp<<_T(";\n");
		}
		bdyinf.m_ldfbdtmp.resetUsed();
	}
    PC_DumpLookUpCode(dmp,  &bdyctx, &globFBnames);
    PC_DumpTmpVariables(dmp, &bdyinf);
    dmp.decrIndent();
    //add the tmpDmp: handle "goto state machine":
    PC_AddStmtToBody(dmp, bdyinf.m_bHasGOTO, tmpDmp, &globFBnames);
    //close exec:
    dmp<<_T(CRLC"\n");

    //check java limit of local variables:
    if(bdyinf.m_StackVarCount > PC_MAX_JAVA_STACK_VARS)
    {
        pgctx->m_dm.msg2(CG_E_MAX_JAVA_STACK_VARS, 
            pIBdy->edpEnd(),
            pgctx->m_dm.jot1()<<bdyinf.m_StackVarCount, 
            pgctx->m_dm.jot2()<<PC_MAX_JAVA_STACK_VARS
        );
    }

    }//end of dump exec method



    //dump called functions as class methods:
    dmp.decrIndent();
    PC_AppendCalledFunMethods(pgctx, dmp, pzCalledFuns);
    dmp.incrIndent();


    //close class definition:
    dmp.decrIndent();
    dmp<<_T(CRLC"\n");


    //dump the java file to disk:
    FC_CString javafile(MAX_PATH);
    javafile<<pSym->pszId<<_T(".java");
    CGT_DmpToFile(&pgctx->m_dm, pgctx->getClsGenPath(), javafile, dmp, false);

    //create gen dir relative file name for loader.list:
    jot.load(PC_GEN_CLASSES_DIR);
    jot.appendDir(pSym->pszId)<<_T(".class");
    jot.toLower();
    pInfo->m_classFile.load(jot);
}


//entry function for this file: 
extern "C" void GenCodeFun(
    CGlobCtx*                 pgctx,
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
)
{
    FC_CString        jot;
    const CG_Symtab*  pSym;
    CGT_CDmp          dmp;
    FC_CStrList       globFBnames;
    CGT_CDmp          tmpDmp;
    BODY_CTX_INFO     bdyctx;
    CBodyInfo         bdyinf(pInfo->m_pSym, pbpList, false);

    pSym = pIBdy->getHeader();
    assert(pInfo->m_pSym==pSym);


    //dump body to temp stream first, for the java declaration
    //we need information that is easiest got by dumping 
    //the body first. All this information is stored in 
    //the bdyctx.pInf member.
    bdyctx.pSym    = pSym;
    bdyctx.pIF     = pgctx->m_pIF;
    bdyctx.pdm     = &pgctx->m_dm;
    bdyctx.pIBdy   = pIBdy;
    bdyctx.nTransi = -1;
    bdyctx.nAction = -1;
    if(pgctx->getControlVer()<22000 || pSym->flags & CG_STAB_BDY_NATIVE)
        bdyctx.BPoption = BP_OPT0; //old code or funs using native blocks don't have BPs
    else
        bdyctx.BPoption = pgctx->getBPOption();
    bdyctx.pInf    = &bdyinf;

    //get all tmp vars IL POUs:
	if(pIBdy->getHeader()->srcTyp == CG_SRC_TYPE_IL)
		CGT_EnumExprStmt(pIBdy->getSTStmts(), &bdyctx, GetTMPVarUse);
	
    tmpDmp.setIndent(2);

    //"begin of POU" BP function unless not option bp0,
    //use special BP number 0:
    if(bdyctx.BPoption!=BP_OPT0)
    {
        DumpBPFun(tmpDmp, pIBdy->edpBeg(), pSym, pbpList, 0);
        bdyinf.m_pLastBPEdp = pIBdy->edpBeg();
    }

    PC_DumpSTStmts(tmpDmp, pIBdy->getSTStmts(), &bdyctx);

    //"end of POU" BP function unless not option bp0,
    //use special BP number 1:
    if(bdyctx.BPoption!=BP_OPT0)
    {
        DumpBPFun(tmpDmp, pIBdy->edpEnd(), pSym, pbpList, 1);
        bdyinf.m_pLastBPEdp = pIBdy->edpEnd();
    }

    //get all decls:    
    //optional leading NATIVE declarations:
    if(pSym->nVarNative)
    {
        for(int i=0;i<pSym->nVarNative;i++)
        {
            const TCHAR* pszN = pSym->pVarNative[i].psz;
            int          is   = FC_StringStartsWithI(pszN, _T("#movetop"));
            if(is>=0)
            {
                //no leading native block allowed in functions
                pgctx->m_dm.errNS1(&pSym->pVarNative[i].edp, _T("leading native block in functions"));
            }
        }
        dmp<<_T('\n');
    }

    //NO trigger tmp vars in functions !
    assert(pIBdy->getUsedTriggers()==0);


    dmp.incrIndent();


    //dump name array if any breakpoints set in function:
    if(bdyctx.BPoption != BP_OPT0)
    {
        (dmp<<_T("private static String[] nameArray_")).lo(pSym->pszId)<<_T(" =\n");
        dmp.incrIndent();
            dmp<<CRLO _T("\n");
            dmp.incrIndent();
            for(const CG_SymEntry* pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
            {
                if(pEntry->declScope==CG_VAR_INPUT || pEntry->declScope==CG_VAR || pEntry==pSym->pFirst) 
                {
                    if(!CG_IsMultiElemType(pEntry->pType) && pEntry->pType->type!=CG_VOID)
                        (dmp<<'"').lo(pEntry->pszId)<<_T("\",\n");
                }
            }
            dmp.decrIndent();
            dmp<<CRLC _T(";\n");
       dmp.decrIndent();
    }

    //function method definition:
    if(!DumpFunctionMethodDecl(&pgctx->m_dm, dmp, pSym, bdyctx.BPoption))
    {
        assert(pgctx->m_dm.hasErr());//any not supported error
        return;
    }
    dmp<<_T(CRLO"\n");
    
    dmp.incrIndent();
	// for IL POUs declare tmp vars
	if(pIBdy->getHeader()->srcTyp == CG_SRC_TYPE_IL)
	{
		int j = bdyinf.m_ldfbdtmp.getUsed();
		if(j)
			dmp<<_T("//////////////// temp variables ////////////////////\n");
		for(int i=0;i<j;i++)
		{
			const TMP_VAR_TYP* pTmp = bdyinf.m_ldfbdtmp.getAddrOf(i);
			int bcol = dmp.column()+25;
			(dmp<<pTmp->pszType).sep(bcol);
			PC_DumpLDFBDStackTmpVarName(dmp, i+1);
			if(pTmp->bFCTmpObj)
			{
				dmp<<_T(" = new ");
				dmp<<ST_GetFunTempVarName(pTmp->pType);
				dmp<<_T("()");
			}
			dmp<<_T(";\n");
		}
		bdyinf.m_ldfbdtmp.resetUsed();
	}
    //function locals:
    if(bdyctx.BPoption==BP_OPT0)
        PC_DumpFunctionLocalVarsV20(pgctx->m_pIF, &pgctx->m_dm, dmp, pSym, &globFBnames);
    else
        PC_DumpFunctionLocalVarsForBP(pgctx->m_pIF, &pgctx->m_dm, dmp, pSym, &globFBnames);

    PC_DumpTmpVariables(dmp, bdyctx.pInf);
    dmp.decrIndent();
    //add the tmpDmp: handle "goto state machine":
    PC_AddStmtToBody(dmp, bdyinf.m_bHasGOTO, tmpDmp, &globFBnames);


    // dump return statement
    dmp.incrIndent();
    PC_DumpFunctionReturn(dmp, &bdyctx, false);
    dmp.decrIndent();


    //close function java method:
    dmp <<_T(CRLC"\n");

    //dump the jif file to disk:
    //set .jif file domain to code info, path must be
    //relative to gen dir, will be needed for lib create:
    jot.load(PC_GEN_CLASSES_DIR);
    jot.appendDir(pSym->pszId)<<_T(".jif");
    pInfo->m_jifFile.load(jot);

    CGT_DmpToFile(&pgctx->m_dm, pgctx->getGenPath(), jot, dmp, false);
}








static void PC_DumpSFCBody(
    CGT_CDmp&               dmp, 
    ICG_Body*               pIBdy,
    CGT_CDiagMsg*           pdm,
    const CG_Symtab*        pSym,
    ICG_IECFront*           pIF,
    BP_OPTION               BPoption,
    FC_CGArray<CGT_BP_TYP>* pbpList,
    bool                    bJitMeNot
)
{
    int                 i;
    const CG_SFCStruct* pSFCS;

    BODY_CTX_INFO  bdyctx;
    CGT_CDmp       tmpDmp;
    FC_CStrList    globFBnames;
    int            lastBPNum = 2; //BP num 0,1 are reserved for POU beg/end

//Dump the main exec() method:
//public void exec(ObjectDirectory globals, FcObject []inoutVars, TaskContext tc)
//{
//    tc.breakPoint(this, 46);
//    super.exec(globals, inoutVars, tc);
//    tc.breakPoint(this, 1);
//}
    dmp<<_T("public void exec(ObjectDirectory globals, FcObject []inoutVars, TaskContext tc)\n"CRLO"\n");
    dmp.incrIndent();

    //"begin of POU" BP function unless not option bp0, use special BP number 0:
    if(pSym->srcTyp == CG_SRC_TYPE_4GR && BPoption!=BP_OPT0)
        DumpBPFun(dmp, pIBdy->edpBeg(), pSym, pbpList, 0);

    // special sfc exception action
    const CG_STStmt* pExActionStmt = pIBdy->getExceptionAction();
    if (pExActionStmt)
    {
        CBodyInfo bdyinf(pSym, pbpList, true);

        bdyctx.pSym     = pSym;
        bdyctx.pIF      = pIF;
        bdyctx.pdm      = pdm;
        bdyctx.pIBdy    = pIBdy;
        bdyctx.nTransi  = -1;
        bdyctx.nAction  = -1;
        bdyctx.BPoption = BPoption;
        bdyinf.m_lastBPNum = lastBPNum;
        bdyctx.pInf     = &bdyinf;

        tmpDmp.setIndent(2);

        PC_DumpSTStmts(tmpDmp, pExActionStmt, &bdyctx);
        lastBPNum = bdyinf.m_lastBPNum;

        PC_DumpLookUpCode(dmp, &bdyctx, &globFBnames);
        PC_DumpTmpVariables(dmp, &bdyinf);
        //add the tmpDmp: handle "goto state machine" and sync stmts:
        PC_AddStmtToBody(dmp, bdyinf.m_bHasGOTO, tmpDmp, &globFBnames);
    }


    dmp<<_T("super.exec(globals, inoutVars, tc);\n");

    //"end of POU" BP function unless not option bp0, use special BP number 1:
    if(pSym->srcTyp == CG_SRC_TYPE_4GR && BPoption!=BP_OPT0)
        DumpBPFun(dmp, pIBdy->edpEnd(), pSym, pbpList, 1);

    dmp.decrIndent();
    dmp<<_T(CRLC"\n\n\n");






//Dump the SFC structure arrays:
//protected int  __FollowTrans []     = {0, 2, 3, 4, 6, 8, 9, 10, 10, };
//protected int  __AltTrans []        = {1, -1, -1, -1, 5, -1, 7, -1, -1, -1, -1, };
//protected int  __NextSteps []       = {-1, 1, -1, 2, -1, 3, 4, -1, 0, -1, 5, -1, 7, -1, 6, -1, 8, -1, 7, -1, 8, -1, 0, -1, };
//protected int  __NextStepsOffset [] = {1, 3, 5, 8, 10, 12, 14, 16, 18, 20, 22, };
//protected int  __PrevSteps []       = {-1, 0, -1, 0, -1, 1, -1, 2, -1, 3, -1, 3, -1, 4, -1, 4, -1, 5, -1, 6, -1, 7, 8, -1, };
//protected int  __PrevStepsOffset [] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, };
//protected int  __StepActionBlockOffset []        = {1, 3, 5, 0, 0, 7, 9, 11, 13, };
//protected int  __StepActionBlocks []             = {-1, 2, -1, 0, -1, 1, -1, 5, -1, 6, -1, 3, -1, 4, -1, };
//protected int  __StepActionBlockQualifier []     = {-1, 4, -1, 32, -1, 32, -1, 32, -1, 32, -1, 32, -1, 32, -1, };
//protected long __StepActionBlockTimeParameter [] = {-1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, };
    pSFCS = pIBdy->getSFCStruct();

    dmp<<_T("protected int  __FollowTrans []     = ")CRLO;
    for(i=0; i<pSFCS->nSteps; i++)
    {
        dmp<<pSFCS->pFollowTrans[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __AltTrans []        = ")CRLO;
    for (i=0; i<pSFCS->nTrans; i++)
    {
        dmp<<pSFCS->pAltTrans[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __NextSteps []       = ")CRLO;
    for (i=0; i<pSFCS->nNextSteps; i++)
    {
        dmp<<pSFCS->pNextSteps[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __NextStepsOffset [] = ")CRLO;
    for (i=0; i<pSFCS->nTrans; i++)
    {
        dmp<<pSFCS->pNextStepsOffset[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __PrevSteps []       = ")CRLO;
    for (i=0; i<pSFCS->nPrevSteps; i++)
    {
        dmp<<pSFCS->pPrevSteps[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __PrevStepsOffset [] = ")CRLO;
    for (i=0; i<pSFCS->nTrans; i++)
    {
        dmp<<pSFCS->pPrevStepsOffset[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");


    dmp<<_T("protected int  __StepActionBlocks []             = ")CRLO;
    for(i=0; i<pSFCS->nStepAction; i++)
    {
        dmp<<pSFCS->pStepActionBlocks[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __StepActionBlockOffset []        = ")CRLO;
    for(i=0; i<pSFCS->nSteps; i++)
    {
        dmp<<pSFCS->pStepActionBlockOffset[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected int  __StepActionBlockQualifier []     = ")CRLO;
    for(i=0; i<pSFCS->nStepAction; i++)
    {
        dmp<<pSFCS->pStepActionBlockQualifier[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");
    dmp<<_T("protected long __StepActionBlockTimeParameter [] = ")CRLO;
    for(i=0; i<pSFCS->nStepAction; i++)
    {
        dmp<<pSFCS->pStepActionBlockTimeParameter[i];
        dmp<<_T(", ");
    }
    dmp<<_T(CRLC";\n");

    dmp<<_T("\n\n\n");







//Dump class constructor
//public simulatesfc()
//{
//    anzSteps   = 9;
//    anzTrans   = 11;
//    anzActions = 7;
//    varOffset  = 5;
//    initStep   = 0;

//    followTrans                  = __FollowTrans;
//    altTrans                     = __AltTrans; 
//    nextSteps                    = __NextSteps;
//    nextStepsOffset              = __NextStepsOffset;
//    prevSteps                    = __PrevSteps;
//    prevStepsOffset              = __PrevStepsOffset;
//    stepActionBlockOffset        = __StepActionBlockOffset;
//    stepActionBlocks             = __StepActionBlocks;
//    stepActionBlockQualifier     = __StepActionBlockQualifier;
//    stepActionBlockTimeParameter = __StepActionBlockTimeParameter;
//
//    initVars();
//}
    (dmp<<_T("public ")).lo(pSym->pszId)<<_T("()\n"CRLO"\n");
    dmp.incrIndent();
    dmp<<_T("anzSteps   = ")<<pSFCS->nSteps<<_T(";\n");
    dmp<<_T("anzTrans   = ")<<pSFCS->nTrans<<_T(";\n");
    dmp<<_T("anzActions = ")<<pSFCS->nActions<<_T(";\n");
    assert(pSym->nTmp==0); //<- rework this if var_tmp comes !
    dmp<<_T("varOffset  = ")<<pSym->nInput + pSym->nOutput + pSym->nVar<<_T(";\n");
    dmp<<_T("initStep   = ")<<pSFCS->initStep<<_T(";\n");
    dmp<<_T("\n");
    dmp<<_T("followTrans                  = __FollowTrans;\n");
    dmp<<_T("altTrans                     = __AltTrans;\n");
    dmp<<_T("nextSteps                    = __NextSteps;\n");
    dmp<<_T("nextStepsOffset              = __NextStepsOffset;\n");
    dmp<<_T("prevSteps                    = __PrevSteps;\n");
    dmp<<_T("prevStepsOffset              = __PrevStepsOffset;\n");
    dmp<<_T("stepActionBlockOffset        = __StepActionBlockOffset;\n");
    dmp<<_T("stepActionBlocks             = __StepActionBlocks;\n");
    dmp<<_T("stepActionBlockQualifier     = __StepActionBlockQualifier;\n");
    dmp<<_T("stepActionBlockTimeParameter = __StepActionBlockTimeParameter;\n");
    dmp<<_T("\n");
    dmp<<_T("initVars();\n");
    dmp.decrIndent();
    dmp<<_T(CRLC"\n\n\n");






//Dump the  doActions method:
//protected void doAction(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc, int nr)
//{
//    switch(nr){
//    case 0:
//        __Action0(globals, inoutVars, tc);
//        break;
//    ......................................
//    case 6:
//        __Action6(globals, inoutVars, tc);
//        break;
//    default :
//        return;
//    }
//}
    dmp<<_T("protected void doAction(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc, int nr)\n");
    dmp<<_T(CRLO"\n");
    dmp.incrIndent();
   
    dmp<<_T("switch(nr)");
    dmp<<_T(CRLO"\n");
    
    for (i=0; i<pSFCS->nActions; i++)
    {
        dmp<<_T("case ");
        dmp<<i;
        dmp<<_T(":\n");
        dmp.incrIndent();
        dmp<<_T("__Action");
        dmp<<i;
        dmp<<_T("(globals, inoutVars, tc");
        dmp<<_T(");\n");
        dmp<<_T("break;\n");
        dmp.decrIndent();
    }
    dmp<<_T("default :\n");
    dmp.incrIndent();
    dmp<<_T("return;\n");
    dmp.decrIndent();
    dmp<<_T(CRLC"\n");
    dmp.decrIndent();
    dmp<<_T(CRLC"\n\n\n");








//Dump the doTransitions method:
//protected boolean doTransition(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc, int nr)
//{
//    switch(nr){
//    case 0:
//        return __Transition0(globals, inoutVars, tc);
//    ................................................
//    case 2:
//    default :
//        return false;
//    }
//}
    dmp<<_T("protected boolean doTransition(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc, int nr)\n");
    dmp<<_T(CRLO"\n");
    dmp.incrIndent(); 
    
    dmp<<_T("switch(nr)");
    dmp<<_T(CRLO"\n");
    
    for (i=0; i<pSFCS->nTrans; i++)
    {
        dmp<<_T("case ");
        dmp<<i;
        dmp<<_T(":\n");
        dmp.incrIndent();
        dmp<<_T("return __Transition");
        dmp<<i;
        dmp<<_T("(globals, inoutVars, tc");
        dmp<<_T(");\n");
        dmp.decrIndent();
    }
    dmp<<_T("default :\n");
    dmp.incrIndent();
    dmp<<_T("return false;\n");
    dmp.decrIndent();
    dmp<<_T(CRLC"\n");
    dmp.decrIndent();
    dmp<<_T(CRLC"\n\n\n");

     

//Dump individual Transition and Action methods
//Prepare a BODY_CTX_INFO struct:

    bdyctx.pSym     = pSym;
    bdyctx.pIF      = pIF;
    bdyctx.pdm      = pdm;
    bdyctx.pIBdy    = pIBdy;
    bdyctx.nTransi  = -1;
    bdyctx.nAction  = -1;
    bdyctx.BPoption = BPoption;
    bdyctx.pInf     = NULL;
    //indent is 2 for all tmp trans/action dumping:
    tmpDmp.setIndent(2);

    for (i=0; i<pSFCS->nTrans; i++)
    {
        if(bJitMeNot)
            dmp<<_T("__attribute JITMeNot()\n");
        dmp<<_T("protected boolean __Transition")<<i;
        dmp<<_T("(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc)\n");
        dmp<<_T(CRLO"\n");

        //update the body ctx struct:
        CBodyInfo bdyinf(pSym, pbpList, true);
        bdyinf.m_lastBPNum = lastBPNum;
        bdyctx.nTransi = i;
        bdyctx.nAction = -1;
        bdyctx.BPoption= BPoption;
        bdyctx.pInf    = &bdyinf;
    
        tmpDmp.seek(0);
        if(pSym->srcTyp==CG_SRC_TYPE_4GR && BPoption==BP_OPT2) //TODO transition BP only for named inside a box
        {
            DumpBPFun(tmpDmp, &pSFCS->pTransEdp[i], pSym, bdyinf.m_pBPlist, bdyinf.m_lastBPNum++);
            bdyinf.m_pLastBPEdp = &pSFCS->pTransEdp[i];
        }

        PC_DumpSTStmts(tmpDmp, pIBdy->getTransi(i), &bdyctx);
        //update local BP counter:
        lastBPNum = bdyinf.m_lastBPNum;
        
        //dump lookup code & tmp vars (uses information generated by PC_DumpSTStmts):
        dmp.incrIndent();
        PC_DumpLookUpCode(dmp, &bdyctx, &globFBnames);
        PC_DumpTmpVariables(dmp, &bdyinf);
        dmp.decrIndent();
        //add the tmpDmp: handle "goto state machine" and sync stmts:
        PC_AddStmtToBody(dmp, bdyinf.m_bHasGOTO, tmpDmp, &globFBnames);
        dmp<<_T(CRLC"\n\n\n");        
    }


    for (i=0; i<pSFCS->nActions; i++)
    {
        if(bJitMeNot)
            dmp<<_T("__attribute JITMeNot()\n");
        dmp<<_T("protected void __Action")<<i;
        dmp<<_T("(ObjectDirectory globals, FcObject[] inoutVars, TaskContext tc)\n");
        dmp<<_T(CRLO"\n");

        //update the body ctx struct:
        CBodyInfo bdyinf(pSym, pbpList, true);
        bdyinf.m_lastBPNum = lastBPNum;
        bdyctx.nTransi = -1;
        bdyctx.nAction = i;
        bdyctx.BPoption= BPoption;
        bdyctx.pInf    = &bdyinf;
    
        tmpDmp.seek(0);
        if(pSym->srcTyp==CG_SRC_TYPE_4GR && BPoption==BP_OPT2) //TODO transition BP only for named inside a box
        {
            DumpBPFun(tmpDmp, &pSFCS->pActionEdp[i], pSym, bdyinf.m_pBPlist, bdyinf.m_lastBPNum++);
            bdyinf.m_pLastBPEdp = &pSFCS->pActionEdp[i];
        }//TODO BPs in ST-Actions 

        PC_DumpSTStmts(tmpDmp, pIBdy->getAction(i), &bdyctx);
        //update local BP counter:
        lastBPNum = bdyinf.m_lastBPNum;

        //dump lookup code & tmp vars (uses information generated by PC_DumpSTStmts):
        dmp.incrIndent();
        PC_DumpLookUpCode(dmp, &bdyctx, &globFBnames);
        PC_DumpTmpVariables(dmp, &bdyinf);
        dmp.decrIndent();
        //add the tmpDmp: handle "goto state machine" and sync stmts:
        PC_AddStmtToBody(dmp, bdyinf.m_bHasGOTO, tmpDmp, &globFBnames);
        dmp<<_T(CRLC"\n\n\n");        
    }


//done.
}

void PC_DumpLookUpCode(
    CGT_CDmp &     dmp, 
    BODY_CTX_INFO* pBdyctx, 
    FC_CStrList*   pGlobFBnames
)
{
    const CG_SymEntry* pEntry;

    //reset the used glob fb array:
    pGlobFBnames->clear();

    dmp<<      _T("//////////////// get variables /////////////////////\n");
    for(pEntry=pBdyctx->pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(!pBdyctx->pInf->m_bHasNative)//TODO dump all if native?
        {
            if(pBdyctx->pInf->m_pUsedVars && !pBdyctx->pInf->m_pUsedVars[pEntry->lNum])
                continue;
        }

        if(PC_DumpOneVariableLookUpCode(dmp, pBdyctx->pdm, pEntry, pGlobFBnames))
            pBdyctx->pInf->m_StackVarCount++;
    }
}

void PC_AddStmtToBody(
    CGT_CDmp&     dmp, 
    bool          bHasGoto, 
    CGT_CDmp&     tmpDmp,
    FC_CStrList*  pGlobFBnames
)
{
    int iSyncStmts;
    
    
    dmp.incrIndent();
    dmp<<_T("\n\n\n//////////////////// POU body //////////////////////\n");
    dmp.decrIndent();

    //append sync if any:
    iSyncStmts = pGlobFBnames ? pGlobFBnames->getUsed() : 0;
    if(iSyncStmts>0)
    {
        //the synchronization must be in the same order in all Programs
        //therefore we sort the globFBnames list ignoreinf the case:
        pGlobFBnames->qsortI();
        for(int i=0;i<iSyncStmts;i++)
        {
            (dmp<<_T("synchronized("));
            JavaId(dmp, pGlobFBnames->get(i))<<_T(")"CRLO"\n");
        }
    }


    if(bHasGoto)
    {
        //dump the for & switch construct:
        //
        //int _Goto = 0;
        //_GotoFor: for(;;){switch(_Goto){            }}
        //case 0:
        dmp<<_T("int ")JAVA_GOTO_STATE_VAR _T(" = 0;\n")
             JAVA_GOTO_FOR _T(": for(;;)"CRLO"\nswitch(") JAVA_GOTO_STATE_VAR _T(")"CRLO"\n")
             _T("case 0:\n");
    }

    //append body:
    dmp.append(tmpDmp);

    if(bHasGoto)
    {
        dmp<<_T(CRLC"\n")   //<-close switch
                _T("break ") JAVA_GOTO_FOR _T(";\n")  //<-break endless loop
                _T(CRLC"\n"); //<-close endless loop
    }

    //close synchronized statements:
    while(--iSyncStmts>=0)
        dmp<<_T(CRLC "//close synchronized block\n");

}








static void PC_AppendCalledFunMethods(
    CGlobCtx*               pgctx,
    CGT_CDmp&               dmpFBPrg,
    const CCodeInfo*const*  pzCalledFuns
)
{
    int                 i;  
    const CCodeInfo*    pInf;
    TCHAR*              pszText;

    for(i=0; (pInf = pzCalledFuns[i])!=NULL; i++)
    {
        assert(pInf->m_pSym->pouTyp==CG_POU_FUN);
        assert((pInf->m_pSym->flags & CG_STAB_LIBIF_ONLY)==0);
        assert(!pInf->m_bErrors || pgctx->m_dm.hasErr());
        
        if(pInf->m_bErrors)
            continue;
        assert(!pInf->m_jifFile.isEmpty());
        
        //TODO lib 

        if(CGT_LoadTextFileToMem(&pgctx->m_dm, NULL, 
            pgctx->m_pIF->FCPgetGenPath(), pInf->m_jifFile, 
            &pszText, NULL, NULL)
          )
        {
            dmpFBPrg<<pszText;
            delete(pszText);
        }
    }
}


static void PC_DumpTmpVariables(
    CGT_CDmp&   dmp,
    CBodyInfo*  pInf
)
{
    int                bcol;
    int                i;
    int                j;
    const TMP_VAR_TYP* pTmp;
    bool               bSepDone = false;

    //dump tmp vars needed in body:
    bcol = dmp.column()+30;
    if(pInf->m_bHasCASE)
    {
        if(!bSepDone)
        {
            dmp<< _T("\n//////////////// tmp variables /////////////////////\n");
            bSepDone = true;
        }
        pInf->m_StackVarCount++;
        (dmp<<_T("int")).sep(bcol);
        dmp<<JAVA_CASE_TMP_VAR _T(";\n");
    }

    
    j = pInf->m_tmpvars.getUsed();
    if(j>0 && !bSepDone)
    {
        dmp<< _T("\n//////////////// tmp variables /////////////////////\n");
        bSepDone = true;
    }

    for(i=0;i<j;i++)
    {
        pTmp = pInf->m_tmpvars.getAddrOf(i);
        int bcol = dmp.column()+30;
        (dmp<<pTmp->pszType).sep(bcol);
        PC_DumpJavaStackTmpVarName(dmp, i+1);
        if(pTmp->bFCTmpObj)
        {
            dmp<<_T(" = new ")<<ST_GetFunTempVarName(pTmp->pType)<<_T("()");
        }
        dmp<<_T(";\n");
    }
    assert(pInf->m_ldfbdtmp.getUsed()==0);
    
    
    //the inout param array if needed:
    if(pInf->m_maxFBinoutArray>0)
    {
        if(!bSepDone)
        {
            dmp<< _T("\n//////////////// tmp variables /////////////////////\n");
            bSepDone = true;
        }
        dmp<<_T("FcObject[] ")JAVA_INOUT_ARRAY 
            _T(" = new FcObject[")<<pInf->m_maxFBinoutArray<<_T("];\n");
        pInf->m_StackVarCount++;
    }
}


static void PC_DumpNameArray(
    CGT_CDmp&                   dmp, 
    const CG_Symtab*            pSym
)
{
    const CG_SymEntry* pEntry;

    
    dmp<<"private static String[] nameArray =\n"CRLO"\n";
    dmp.incrIndent();

    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        switch(pEntry->declScope)
        {
        case CG_VAR:
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT:
            (dmp<<_T('"')).lo(pEntry->pszId)<<_T("\",\n");
        break;
        case CG_VAR_IN_OUT:
        case CG_VAR_GLOBAL:
        case CG_VAR_EXTERNAL:
        case CG_CONST:
        break;
            assert(!"bad enum");
        }
    }

 
    //SFC special variables:
    if(pSym->flags & CG_STAB_BDY_IS_SFC)
    {
        dmp<<_T("\"__sfcvis\",\n")
             _T("\"__retain\",\n")
             _T("\"__manualmode\",\n")
             _T("\"__donext\",\n")
             _T("\"__onceforcedtransitions\",\n")
             _T("\"__forcedtransitions\",\n")
             _T("\"__blockedtransitions\",\n")
             _T("\"__alltransitionson\",\n")
             _T("\"__onceforcedactions\",\n")
             _T("\"__forcedactions\",\n")
             _T("\"__blockedactions\",\n")
             _T("\"__allactionsoff\",\n")
             _T("\"__mancmd\",\n");
    }

    
    dmp.decrIndent();
    dmp<<CRLC";\n";
    
    dmp<<"protected String[] getNameList()"CRLO"return nameArray;"CRLC"\n\n";
}


static bool PC_DumpOneVariableLookUpCode(
    CGT_CDmp&           dmp, 
    CGT_CDiagMsg*       pdm,
    const CG_SymEntry*  pEntry, 
    FC_CStrList*        pGlobFBnames
)
{
    CG_DECLSCOPE    store;
    int             b;
    const CG_DType* pType;
    CG_DTYPES       typ;
    const TCHAR*    pszId;

    store   = pEntry->declScope;
    pType   = pEntry->pType;
    typ     = pType->type;
    pszId   = pEntry->pszId;


    //select action:
    switch(store)
    {
        case CG_VAR:
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT:
        case CG_VAR_IN_OUT:   break;
        case CG_VAR_GLOBAL:   assert(!"bad enum");
        case CG_VAR_EXTERNAL:   
        break;
        case CG_CONST:         return false;
        default:
            assert(!"bad enum");
    }
   

    assert(CG_IsMonomorph(pType->type) || store==CG_VAR_IN_OUT);

    b = dmp.column();
    dmp<<GetFcInterfaceNameFromType(pType);
    dmp.sep(b+25); JavaId(dmp, pszId)<<_T(" = ");
    dmp.sep(b+45);
    dmp<<'('<<GetFcInterfaceNameFromType(pType)<<')';

    //'get object code' depends on storage class:
    if(store==CG_VAR_EXTERNAL)
    {
        dmp<<_T("globals.getObject(\"");
        dmp.lo(pszId)<<_T("\")");

        if(typ==CG_INSTANCE)
            pGlobFBnames->add(pszId);
    }
    else if(store==CG_VAR_IN_OUT)
    {
        assert(pEntry->lInstNum>=0);
        dmp<<_T("inoutVars[")<<pEntry->lInstNum<<_T(']');
    }
    else
    {
        dmp<<_T("locVars[")<<PC_GetObjNumOfEntry(pEntry)<<_T(']');
    }
    dmp<<_T(";\n");

    // sfc does not support EDGE_Variables
    if((pEntry->pParent->flags & CG_STAB_BDY_IS_SFC) && 
       (typ==CG_BOOL_R_EDGE || typ==CG_BOOL_F_EDGE))
    {
        pdm->errNS1(&pEntry->edp, _T("edge variables in SFC"));
    }

    //inline edge detection code, if any:
    if(typ==CG_BOOL_R_EDGE)
    {
        //dump the following:
        //TRIG__in  = !_0ed && _in.getBool();
        //_0ed= _in.getBool();
        (dmp<<_T("boolean ") JAVA_TRIG_PREFIX).lo(pszId)<<_T(" = ");
        dmp<<_T("!")JAVA_EDGE_NAME_PREFIX<<PC_GetObjNumOfEntry(pEntry)<<_T(" && ");
        JavaId(dmp, pszId)<<_T('.')<<GetGetFunctionFromType(CG_BOOL)<<_T("();\n");
    
        dmp<<JAVA_EDGE_NAME_PREFIX<<PC_GetObjNumOfEntry(pEntry)<<_T("= ");
        JavaId(dmp, pszId)<<_T('.')<<GetGetFunctionFromType(CG_BOOL)<<_T("();\n");
    }
    else if(typ==CG_BOOL_F_EDGE)
    {
        //dump the following:
        //TRIG__in  = !_0ed && !_in.getBool();
        //_0ed= !_in.getBool();
        (dmp<<_T("boolean ") JAVA_TRIG_PREFIX).lo(pszId)<<_T(" = ");
        dmp<<_T("!")JAVA_EDGE_NAME_PREFIX<<PC_GetObjNumOfEntry(pEntry)<<_T(" && !");
        JavaId(dmp, pszId)<<_T('.')<<GetGetFunctionFromType(CG_BOOL)<<_T("();\n");
    
        dmp<<JAVA_EDGE_NAME_PREFIX<<PC_GetObjNumOfEntry(pEntry)<<_T("= !");
        JavaId(dmp, pszId)<<_T('.')<<GetGetFunctionFromType(CG_BOOL)<<_T("();\n");
    }

    return true;
}




static void PC_DumpEdgeHelperClassVariables(
    CGT_CDmp&        dmp, 
    const CG_Symtab* pSym
)
{
    bool               bDumpHelp;
    const CG_SymEntry* pEntry;
    CG_DTYPES          typ;
    bDumpHelp = true;

    for(pEntry=pSym->pFirst;pEntry;pEntry=pEntry->pNext)
    {
        typ = pEntry->pType->type;
        if(typ!=CG_BOOL_R_EDGE && typ!=CG_BOOL_F_EDGE)
            continue;

        if(bDumpHelp)
        {
            dmp<<_T("//helper variables for edge detecting inputs:\n");
            bDumpHelp = false;
        }

        dmp<<_T("boolean ")JAVA_EDGE_NAME_PREFIX<<PC_GetObjNumOfEntry(pEntry);
        
        if(typ==CG_BOOL_F_EDGE)
            dmp<<_T(" = true");
        
        dmp<<_T(";\n");
    }

    if(!bDumpHelp)
        dmp<<_T("\n\n");
}


static bool DumpFunctionMethodDecl(
    CGT_CDiagMsg*      pdm,
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    BP_OPTION          bpOpt
)
{
    bool               bErrorNS;
    CG_DTYPES          type;
    const CG_SymEntry* pEntry;
   
    bErrorNS = false;

    pEntry = pSym->pFirst;
    assert(pEntry);

    dmp<<_T("private ");
    
    // dump return type
    type = pEntry->pType->type;
    
    if(CG_IsElementary(type) || CG_IsAnyQVT(type))
    {
        dmp<<GetJavaBasicTypeName(pEntry->pType, false);
    }
    else if (type == CG_VOID)
    {
        dmp<<_T("void");
    }
    else // complex type->error
    {
        pdm->errNS1(&pEntry->edp, _T("functions return complex type (only supported if inline{<native>} body)"));
        bErrorNS = true;
    }


    //class method name:
    (dmp<<_T("  ")JAVA_FUNCTION_PRE).lo(pSym->pszId)<<_T("(");
    

    for (pEntry = pEntry->pNext; pEntry; pEntry = pEntry->pNext)
    { 
        switch(pEntry->declScope)
        {
        case CG_VAR_INPUT:
            
            dmp<<GetJavaBasicTypeName(pEntry->pType, false)<<_T(" ");

            if(bpOpt==BP_OPT0 || pEntry->pType->pSym || pEntry->pType->bIsArray)
                JavaId(dmp, pEntry->pszId);
            else
                (dmp<<_T("_I_")).lo(pEntry->pszId); //extra var for Breakpoint support

            dmp<<_T(", ");
        break;
        case CG_VAR_IN_OUT:
            dmp<<GetFcInterfaceNameFromType(pEntry->pType)<<_T(" ");
            JavaId(dmp, pEntry->pszId);
            dmp<<_T(", ");
        break;
        case CG_VAR_OUTPUT:
            if(CG_IsMultiElemType(pEntry->pType))
            {
                assert(!"front end error"); //such error shloud not pass here
                pdm->errNS1(&pEntry->edp, _T("complex datatypes as function output (only supported if inline{<native>} body)"));
                bErrorNS = true;
            }
            else
            {
                dmp<<GetFcInterfaceNameFromType(pEntry->pType)<<_T(" ");
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(", ");
            }
        break;
        case CG_VAR_IN_EXT:
            assert(!"front end error"); //such error should not pass here
            pdm->errNS1(&pEntry->edp, _T("extended input-variables in ST/LD/FBD functions"));
            bErrorNS = true;
        break;
        case CG_VAR:
            if(CG_IsMultiElemType(pEntry->pType))
            {
                pdm->errNS1(&pEntry->edp, _T("complex datatypes as function local variables"));
                bErrorNS = true;
            }
        break;
        default:
            ;//not relevant 
        }
    }

    //dump task context tc and globals always for user funs:
    dmp<<_T(" ObjectDirectory globals, TaskContext tc)\n");

    return !bErrorNS;
}


static void PC_DumpQVTInitValue(
    CGT_CDmp& dmp, 
    CG_DTYPES type
)
{
    assert(CG_IsAnyQVT(type));

    switch (type) {
    case CG_QVT_BOOL:
        dmp<<_T("new QvtBoolValue(0,false,0)");
    break;
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:
        dmp<<_T("new QvtIntValue(0,0,0)");
    break;
    case CG_QVT_LWORD:
    case CG_QVT_LINT:
        dmp<<_T("new QvtLongValue(0,0l,0)");
    break;
    case CG_QVT_LREAL:
        dmp<<_T("new QvtDoubleValue(0,0.0,0)");
    break;
    case CG_QVT_WSTRING:
        dmp<<_T("new QvtStringValue(0,\"\",0)");
    break;
    default:
        assert(!"type not implemenented");
    }
}



static void PC_DumpFunctionLocalVarsV20( //<-this is the excatly the old dump routine before
    ICG_IECFront*      pIF,              //introducing brekpoints, I leave it unchanged to make
    CGT_CDiagMsg*      pdm,              //sure old code is 100% the same
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    FC_CStrList*       pGlobFBnames
)                             
{
    const CG_SymEntry*   pEntry;
    CG_DTYPES            type;
    int                  b = dmp.column();

    dmp<<_T("//////////////// local variables ///////////////////\n");

    //optional NATIVE declarations:
    DumpVarNative(pdm, dmp, pSym, PC_NDECL_CLASS);//##AC
    
    for (pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    { 
        switch(pEntry->declScope)
        {
        FunctionReturn:
        case CG_VAR:
            type = pEntry->pType->type;
            if(type==CG_VOID)
                break; //only if was reached from goto FunctionReturn; 

            if(IsJavaBasicType(BP_OPT0, pEntry))
            {
                dmp<<GetJavaBasicTypeName(pEntry->pType, false)<<_T(" ");
                dmp.sep(b+25); JavaId(dmp, pEntry->pszId);

                if(CG_IsElementary(type))
                {
                    // dump init value
                    dmp<<_T(" = ");
                    dmp.sep(b+45);

                    if(pEntry->pInit)
                    {
                        assert(pEntry->pInit->ut == CG_INIT_SIMPLE);
                        PC_DumpJavaInitValue(dmp, pEntry->pInit->sim.pLit);
                    }
                    else
                    {
                        PC_DumpJavaInitValue(dmp, pIF->getDefaultInit(type));
                    }
                }
                else if(CG_IsAnyQVT(type))
                {
                    assert(!pEntry->pInit);
                    dmp<<_T(" = ");
                    dmp.sep(b+45);
                    PC_DumpQVTInitValue(dmp, type);
                }
                else
                {
                    CGT_IERR2(pdm, &pEntry->edp, _T("'IsJavaBasicType(pEntry)==true' but type is array/struct"));
                }
                dmp<<_T(";\n");
            }
            else if(CG_IsElementary(type) || CG_IsAnyQVT(type))
            {
                dmp<<GetFcInterfaceNameFromType(pEntry->pType)<<_T(" ");
                dmp.sep(b+25); 
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(" = new ")<<ST_GetFunTempVarName(pEntry->pType)<<_T("();\n");
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(".")<<GetPutFunctionFromType(type)<<_T("(");
                
                if(pEntry->pInit)
                {
                    assert(pEntry->pInit->ut == CG_INIT_SIMPLE && !CG_IsAnyQVT(type));
                    PC_DumpJavaInitValue(dmp, pEntry->pInit->sim.pLit);
                }
                else if(CG_IsElementary(type))
                {
                    PC_DumpJavaInitValue(dmp, pIF->getDefaultInit(type));
                }
                else
                {
                    PC_DumpQVTInitValue(dmp, type);
                }
                dmp<<_T(");\n");
            }
            else
            {
                //assert: should have been detected in DumpFunctionMethodDecl:
                assert(!"unexpected bad decl");
                dmp<<_T("internal error!");
            }
        break;
        case CG_VAR_OUTPUT:
        {
            if(pEntry == pSym->pFirst)
                goto FunctionReturn;

            type = pEntry->pType->type;
            assert(!IsJavaBasicType(BP_OPT0, pEntry));
            if(CG_IsElementary(type) || CG_IsAnyQVT(type))
            {
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(".")<<GetPutFunctionFromType(type)<<_T("(");
                
                if(pEntry->pInit)
                {
                    assert(pEntry->pInit->ut == CG_INIT_SIMPLE && !CG_IsAnyQVT(type));
                    PC_DumpJavaInitValue(dmp, pEntry->pInit->sim.pLit);
                }
                else if(CG_IsElementary(type))
                {
                    PC_DumpJavaInitValue(dmp, pIF->getDefaultInit(type));
                }
                else
                {
                    PC_DumpQVTInitValue(dmp, type);
                }
                dmp<<_T(");\n");
            }
            else
            {
                //assert: should have been detected in DumpFunctionMethodDecl:
                assert(!"unexpected bad decl");
                dmp<<_T("internal error!");
            }
        }
        break;
        case CG_VAR_EXTERNAL:
        {
            dmp<<GetFcInterfaceNameFromType(pEntry->pType);
            dmp.sep(b+25); JavaId(dmp, pEntry->pszId)<<_T(" = ");
            dmp.sep(b+45);
            dmp<<'('<<GetFcInterfaceNameFromType(pEntry->pType)<<')';
            dmp<<_T("globals.getObject(\"");
            dmp.lo(pEntry->pszId)<<_T("\")");
            dmp<<_T(";\n");

            if(pEntry->pType->type==CG_INSTANCE)
                pGlobFBnames->add(pEntry->pszId);
        }
        break;    
        default: 
            ;//not relevant
        }
    }
}


static void PC_DumpFunctionLocalVarsForBP(//<-new dump function code the breakpoint support
    ICG_IECFront*      pIF,
    CGT_CDiagMsg*      pdm,
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    FC_CStrList*       pGlobFBnames
)                             
{
    const CG_SymEntry*   pEntry;
    CG_DTYPES            type;
    FC_CString           tempList;
    int                  b = dmp.column();

    dmp<<_T("//////////////// local variables ///////////////////\n");

    //optional NATIVE declarations:
    DumpVarNative(pdm, dmp, pSym, PC_NDECL_CLASS);//##AC
    
    for (pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    { 
        switch(pEntry->declScope)
        {
        FunctionReturn:
        case CG_VAR:
            type = pEntry->pType->type;
            if(type==CG_VOID)
                break; //only if was reached from goto FunctionReturn; 

            if((CG_IsElementary(type) || CG_IsAnyQVT(type)) && !pEntry->pType->bIsArray)
            {
                (tempList<<'_').appendLo(pEntry->pszId)<<','<<' ';
                    
                dmp<<ST_GetFunTempVarName(pEntry->pType)<<_T(" ");
                dmp.sep(b+25); 
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(" = new ")<<ST_GetFunTempVarName(pEntry->pType)<<_T("();");

                if(pEntry->pInit)
                {
                    JavaId(dmp, pEntry->pszId);
                    dmp<<'.'<<GetPutFunctionFromType(type)<<'(';
                    assert(pEntry->pInit->ut == CG_INIT_SIMPLE && !CG_IsAnyQVT(type));
                    PC_DumpJavaInitValue(dmp, pEntry->pInit->sim.pLit);
                    dmp<<_T(");");
                }
                else if(CG_IsAnyQVT(type))
                {
                    JavaId(dmp, pEntry->pszId);
                    dmp<<'.'<<GetPutFunctionFromType(type)<<'(';
                    PC_DumpQVTInitValue(dmp, type);
                    dmp<<_T(");");
                }
                else if(type==CG_WSTRING)
                {
                    JavaId(dmp, pEntry->pszId);
                    dmp<<'.'<<GetPutFunctionFromType(type)<<'(';
                    PC_DumpJavaInitValue(dmp, pIF->getDefaultInit(type));
                    dmp<<_T(");");
                }
                dmp<<'\n';
            }
            else
            {
                //assert: unsupported complex type should have been detected in DumpFunctionMethodDecl:
                assert(!"unexpected bad decl");
                dmp<<_T("internal error!");
            }
        break;
        case CG_VAR_INPUT:
            type = pEntry->pType->type;
            if(!pEntry->pType->bIsArray && !pEntry->pType->pSym)
            {
                (tempList<<'_').appendLo(pEntry->pszId)<<','<<' ';

                dmp<<ST_GetFunTempVarName(pEntry->pType)<<_T(" ");
                dmp.sep(b+25); 
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(" = new ")<<ST_GetFunTempVarName(pEntry->pType)<<_T("();");
                JavaId(dmp, pEntry->pszId);
                (dmp<<'.'<<GetPutFunctionFromType(type)<<_T("(_I_")).lo(pEntry->pszId)<<_T(");\n");
            }
            //else: input of complex type cannot be monitored, no temp var...
        break;
        case CG_VAR_OUTPUT:
        {
            if(pEntry == pSym->pFirst)
                goto FunctionReturn;

            type = pEntry->pType->type;
            if(CG_IsElementary(type) || CG_IsAnyQVT(type))
            {
                JavaId(dmp, pEntry->pszId);
                dmp<<_T(".")<<GetPutFunctionFromType(type)<<_T("(");
                
                if(pEntry->pInit)
                {
                    assert(pEntry->pInit->ut == CG_INIT_SIMPLE && !CG_IsAnyQVT(type));
                    PC_DumpJavaInitValue(dmp, pEntry->pInit->sim.pLit);
                }
                else if(CG_IsElementary(type))
                {
                    PC_DumpJavaInitValue(dmp, pIF->getDefaultInit(type));
                }
                else
                {
                    PC_DumpQVTInitValue(dmp, type);
                }
                dmp<<_T(");\n");
            }
            else
            {
                //assert: should have been detected in DumpFunctionMethodDecl:
                assert(!"unexpected bad decl");
                dmp<<_T("internal error!");
            }
        }
        break;
        case CG_VAR_EXTERNAL:
        {
            dmp<<GetFcInterfaceNameFromType(pEntry->pType);
            dmp.sep(b+25); JavaId(dmp, pEntry->pszId)<<_T(" = ");
            dmp.sep(b+45);
            dmp<<'('<<GetFcInterfaceNameFromType(pEntry->pType)<<')';
            dmp<<_T("globals.getObject(\"");
            dmp.lo(pEntry->pszId)<<_T("\")");
            dmp<<_T(";\n");

            if(pEntry->pType->type==CG_INSTANCE)
                pGlobFBnames->add(pEntry->pszId);
        }
        break;    
        default: 
            ;//not relevant
        }
    }


    //finaly dump:
    //FcObject         locals[]   = {_function1, _in1, _var1,};
    //String           funName    = tc.pushFrame("function1", new TempObjectDirectory(nameArray_function1, locals)); 

    dmp<<_T("FcObject");
    dmp.sep(b+25);
    dmp<<_T("locals[] = ");
    dmp<<'{'<<tempList<<_T("};\n");

    dmp<<_T("String");
    dmp.sep(b+25);
    dmp<<_T("funName = ");
    dmp<<_T("tc.pushFrame(\"");
    dmp.lo(pSym->pszId)<<_T("\", new TempObjectDirectory(nameArray_");
    dmp.lo(pSym->pszId)<<_T(", locals));\n"); 




}
