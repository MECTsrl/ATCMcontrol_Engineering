
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include "exprcode.h"
#include "cgt_CDiagMsg.h"
#include "helper.h"
#include "cgt_misc.h"
#include "cgt_tools.h"
#include "baselib.h"
#include <limits.h>





//declare local dirty helper functions:
static void PC_DumpCGFor(
    CGT_CDmp&            dmp, 
    const CG_Edipos*     pEdp,
    const CG_StmtFor*    pFor,
    const BODY_CTX_INFO* pctx
);
static const TCHAR* PC_NextMacroArg(
    const TCHAR* psz, 
    CGT_CDmp&    dmp, 
    int*         pNum
);
static bool PC_UsedTwice(
    int          argNum, 
    const TCHAR* pszRestOfInlineCode
);
static bool PC_NeedsTmpVar(const CG_ExprTree* pExpr);
static bool PC_NeedsParent(const CG_ExprTree* pExpr);
static PC_DumpInlineFunOpMacro(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const TCHAR*         pszOp,//some java text used like an operator
    int                  nArgs,
    const CG_ArgExpr     pAE[]   //the argument list, NULL if no args
);
static void PC_DumpInlineFunMacro(
    CGT_CDmp&           dmp, 
    const CG_Edipos*    pEdp,
    const BODY_CTX_INFO*pctx,
    int                 nActArg,
    const CG_ArgExpr    pAE[],
    const CG_Symtab*    pFun,
    const TCHAR*        pszMac,
    const CG_Edipos*    pEdpMac
);
static void DumpBuildInOps
(
    CGT_CDmp&           dmp, 
    const CG_Edipos*    pEdp,
    const BODY_CTX_INFO*pctx,
    const TCHAR*        pszId, 
    int                 nArgs,
    const CG_ExprTree*  pArgs
);
















CBodyInfo::CBodyInfo(
    const CG_Symtab*        pSym,
    FC_CGArray<CGT_BP_TYP>* pbpList, 
    int                     bSampleUsed
)
{
    //assert: bp list shoulde be initialliy empty:
    assert(pSym && pbpList /* && pbpList->getUsed()==0 can be >0 if S´FC !*/);

    m_maxFBinoutArray = 0;
    m_bHasNative      = false;
    m_bHasCASE        = false;
    m_bHasGOTO        = false; 
    m_StackVarCount   = 0;
    m_pUsedVars       = NULL;
    m_pSym            = pSym;
    if(bSampleUsed && pSym->nEntries>0)
    {
        m_pUsedVars = new bool[pSym->nEntries];
        memset(m_pUsedVars, 0, pSym->nEntries*sizeof(bool));
    }

    m_pLastBPEdp      = NULL;
    m_lastBPNum       = 2; //start with 2, 0/1 is reserved for POU beg/end
    m_pBPlist         = pbpList;
}
CBodyInfo::~CBodyInfo()
{
    delete(m_pUsedVars);
}


int CBodyInfo::getFreeTmpVar(const CG_DType* pType)
{
    int          i;
    const TCHAR* psz;
    TMP_VAR_TYP* pTmp;
    TMP_VAR_TYP  tmp;

    psz = GetJavaBasicTypeName(pType, false);
    
    for(i = m_tmpvars.getUsed()-1; i>=0; i--)
    {
        pTmp = m_tmpvars.getAddrOf(i);
        assert(!pTmp->bRef && !pTmp->bFCTmpObj);
        if(!pTmp->bUsed && pTmp->pszType==psz)
        {
            pTmp->bUsed = true;
            return i+1;
        }
    }

    i = m_tmpvars.getUsed();
    if(m_tmpvars.getMaxAlloc()<=i)
        m_tmpvars.allocSize(i+128);
    tmp.bUsed   = true;   
    tmp.pszType = psz;
    tmp.pType   = pType;
    tmp.bRef    = false;   
    tmp.bFCTmpObj = false;
    return m_tmpvars.add(tmp)+1;
}
void CBodyInfo::freeTmpVar(int i)
{
    m_tmpvars.getAddrOf(i-1)->bUsed = false;
}


const TMP_VAR_TYP* CBodyInfo::newLDFBDTmpVar(const CG_ExprTree* pTmp, bool bNeedFCObj)
{
    int          iNew;
    int          iLast;
    TMP_VAR_TYP  tmp;
    
    
    iNew = pTmp->tmp.lTmpNum-1;
    assert(iNew>=0);
    iLast = m_ldfbdtmp.getUsed()-1;

    //next if is only to do a minimal assertion that a 
    //tmp gets written before read.
    if((pTmp->tmp.accFlags & CG_ACCESS_ANY_W)==0)
    {
        assert(pTmp->tmp.accFlags & CG_ACCESS_ANY_R);
        assert(iLast>=iNew);
        return m_ldfbdtmp.getAddrOf(iNew);
    }

    if(iLast>=iNew)
    {
        assert(m_ldfbdtmp.getAddrOf(iLast)->bUsed);
        if(bNeedFCObj) //check that we now need an FCObject type (first use of tmp might not need !) 
        {
            TMP_VAR_TYP* pt = m_ldfbdtmp.getAddrOf(iNew);
            
            if(!pt->bFCTmpObj)
            {
                pt->pszType = GetJavaBasicTypeName(pTmp->pType, true);
                pt->bFCTmpObj = true;
            }
        }
        return m_ldfbdtmp.getAddrOf(iNew);
    }


    //gap will virtually never be >1.
    for(int gap = iNew - iLast; gap>0; gap--)
        m_ldfbdtmp.addZero();

    tmp.bUsed     = true;   
    tmp.pType     = pTmp->pType;
    tmp.bRef      = pTmp->tmp.pRefExpr!=0;
    tmp.bFCTmpObj = bNeedFCObj;
    tmp.pszType   = GetJavaBasicTypeName(tmp.pType, tmp.bRef || bNeedFCObj);
    m_ldfbdtmp.set(iNew, tmp);
    return m_ldfbdtmp.getAddrOf(iNew);
}

void CBodyInfo::addBdyUsed(const CG_SymEntry* pEntry)
{
    if(!m_pUsedVars) //not wanted...
        return;
    
    assert(pEntry->lNum>=0 && pEntry->lNum<m_pSym->nEntries);
    m_pUsedVars[pEntry->lNum] = true;
}


bool IsJavaBasicType(BP_OPTION bpOpt, const CG_SymEntry* pEntry)
{
    CG_DECLSCOPE ds;
    
    if(pEntry->pParent->pouTyp != CG_POU_FUN)
        return false; //all are obj if not Function scope

    if(pEntry->pType->pSym || pEntry->pType->bIsArray)
        return false; //complex types are always objects
    
    if(bpOpt!=BP_OPT0)
        return false; //if any Breakpoint set in a function we use tmp vars objects

    //pass here only for simple type:
    ds = pEntry->declScope;

    if(ds==CG_VAR_IN_OUT || ds==CG_VAR_EXTERNAL)
        return false; //must be object type

    if(ds==CG_VAR_INPUT || ds==CG_VAR_IN_EXT)
    {
        assert((pEntry->accFlags & (CG_ACCESS_ANY_W & ~CG_ACCESS_N))==0);
        return true;//simple inputs are always java basic
    }

    if(ds==CG_VAR_OUTPUT && pEntry->pParent->pFirst!=pEntry)
        return false; //all outputs except the function return are object type

    //is a simple type fun var local or 'function name output'
    //assert: a simple type (no array/struct) cannot have partially bits:
    assert((pEntry->accFlags & (CG_ACCESS_x|CG_ACCESS_o|CG_ACCESS_a))==0);
    
    //note: ignore the CG_ACCESS_N bit here, the native block code must 
    //be changed if the var gets used XOA, that is ok... .
    return !(pEntry->accFlags & (CG_ACCESS_X|CG_ACCESS_O|CG_ACCESS_A));
}



//TODO comment
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

static void DumpLDFBDNetwork(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmts,
    const BODY_CTX_INFO* pctx
)
{
    int j;
    int i;
    
    //get all tmp vars of the NW:
    CGT_EnumExprStmt(pStmts, (void*)pctx, GetTMPVarUse);

    dmp<<_T("\n"CRLO" //Network\n");
    
    j = pctx->pInf->m_ldfbdtmp.getUsed();
    for(i=0;i<j;i++)
    {
        const TMP_VAR_TYP* pTmp = pctx->pInf->m_ldfbdtmp.getAddrOf(i);
        int bcol = dmp.column()+30;
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

    PC_DumpSTStmts(dmp, pStmts, pctx);

    pctx->pInf->m_ldfbdtmp.resetUsed();

    dmp<<_T("\n"CRLC"\n");

}




void DumpBPFun(
    CGT_CDmp&               dmp, 
    const CG_Edipos*        pEdp,
    const CG_Symtab*        pSym,
    FC_CGArray<CGT_BP_TYP>* pbpList,
    int                     nBPNum
)
{
    CGT_BP_TYP bps;

    if(pSym->pouTyp == CG_POU_FUN)
        dmp<<_T("tc.funBreakPoint(funName, ");
    else
        dmp<<_T("tc.breakPoint(this, ");
    
    dmp<<nBPNum<<_T(");\n");

    bps.iBP = nBPNum;
    bps.edp = *pEdp;

    if(pbpList)
    {
        if(pbpList->getMaxAlloc()==0)
            pbpList->allocSize(128);

        pbpList->add(bps);
    }
}


static void ConsiderBP(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmt,
    const BODY_CTX_INFO* pctx
)
{
    if(pctx->BPoption!=BP_OPT2)
        return; //no BPs inside body wanted
   
    if(CGT_ConsiderBP(pctx->pInf->m_pLastBPEdp, &pStmt->edp))
    {
        DumpBPFun(dmp, &pStmt->edp, pctx->pSym, pctx->pInf->m_pBPlist, pctx->pInf->m_lastBPNum++);
        pctx->pInf->m_pLastBPEdp = &pStmt->edp;
    }
}

static const TCHAR* FC_StrFindId(const TCHAR* pszText, const TCHAR* pszID)//##AC
{
   const TCHAR* psz;
   int iLenId = _tcslen(pszID);

   while( (psz = _tcsstr(pszText, pszID))!=NULL)
   {
       if(psz==pszText || !__iscsym(psz[-1]))
       {
           int iLenRest = _tcslen(psz);

           if(iLenRest==iLenId || !__iscsym(psz[iLenId]))
               return psz;
       }
       pszText = psz+iLenId;
   }

   return NULL;
}


void PC_DumpFunctionReturn(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    bool                 bDumpIfTrue //used to avoid a statement not reached error from java compiler
)
{
    assert(pctx->pSym->pouTyp==CG_POU_FUN);
    const CG_SymEntry* pFunEntry = pctx->pSym->pFirst;

    if(pFunEntry->pType->type != CG_VOID)
    {
        if(pctx->BPoption==BP_OPT0)
        {
            //case 1: function returning a value and no BPs
            CG_ExprTree  varref;
            if(bDumpIfTrue)
                dmp<<_T("if(true)");
            dmp<<_T("return ");
            varref.extyp      = CG_VARREF;
            varref.edp        = *pctx->pIBdy->edpEnd();
            varref.pType      = pFunEntry->pType;
            varref.var.pEntry = pFunEntry;
            PC_DumpCGExprVal(dmp, &varref, pctx);
            dmp<<_T(";\n");
        }
        else
        {
            //case 2: function returning a value and has BPs
            dmp<<CRLO _T("\n");
            dmp.incrIndent();
                dmp<<GetJavaBasicTypeName(pFunEntry->pType, false)<<_T(" ret = ");
                JavaId(dmp, pFunEntry->pszId)<<'.'<<GetGetFunctionFromType(pFunEntry->pType->type)<<_T("();\n");
                dmp<<_T("tc.popFrame();\n");
                if(bDumpIfTrue)
                    dmp<<_T("if(true)");
                dmp<<_T("return ret;\n");
            dmp.decrIndent();
            dmp<<CRLC _T("\n");
        }
    }
    else
    {    
        if(pctx->BPoption==BP_OPT0)
        {
            //case 3: function returning void and has no BPs:
            if(bDumpIfTrue)
                dmp<<_T("if(true)");
            dmp<<_T("return;\n");
        }
        else
        {
            //case 4: function returning void and has BPs:
            dmp<<_T("tc.popFrame();\n");
            if(bDumpIfTrue)
                dmp<<_T("if(true)");
            dmp<<_T("return;\n");
        }
    }
}


//dump actual body ....
void PC_DumpSTStmts(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmts,
    const BODY_CTX_INFO* pctx
)
{
    const CG_STStmt*     pS;

    pS = pStmts;

Next:
    if(!pS)
        return;
    

    switch(pS->styp)
    {
    case CG_ASSIGN:
    {
        if(pS->asns.pLhs)
        {
            ConsiderBP(dmp, pS, pctx); 
            PC_DumpCGAssign(dmp, pS->asns.pLhs, pS->asns.pRhs, pctx);
            dmp<<_T(";\n");
        }
        else
        {
            assert(pctx->nTransi >= 0);
            dmp<<_T("return ");
            PC_DumpCGExprVal(dmp, pS->asns.pRhs, pctx);
            dmp<<_T(";\n");
        }
    }
    break;
    case CG_CALLVOIDFUN:
    {
        ConsiderBP(dmp, pS, pctx); 
        PC_DumpCGExprVal(dmp, pS->funs.pExpr, pctx);
        dmp<<_T(";\n");
    }
    break;
    case CG_CALLFB:
    {
        assert(!pS->fbcs.pMac);//<-feature not enabled
        ConsiderBP(dmp, pS, pctx); 
        PC_DumpFBCall(pctx, dmp, pS->fbcs.pInst, pS->fbcs.nArgs, pS->fbcs.pArgs, NULL, 0);
        dmp<<_T("\n");
    }
    break;
    case CG_CASE:
    {
        int                  i;
        const CG_STCaseList* pCase;

        ConsiderBP(dmp, pS, pctx); 
        pctx->pInf->m_bHasCASE = true;
        //TODO front end? check first if simple 'case', test if has no ranges:
        //for(pCase = pStmtRoot->cases.pCases; pCase; pCase=pCase->pNextCase)
        //{
        //    if(!(pCase->nRanges==1 && pCase->ranges[0].ulRangeLength==0))
        //        break;
        //}
        dmp<<_T("\n"JAVA_CASE_TMP_VAR" = ");
        PC_DumpCGExprVal(dmp, pS->cases.pExpr, pctx);
        dmp<<_T(";\n");
        
        for(pCase = pS->cases.pCases; pCase; pCase=pCase->pNextCase)
        {
            dmp<< (pCase==pS->cases.pCases ? _T("if(") : _T(CRLC"\nelse if("));
            
            assert(pCase->nRanges>=1);
            
            for(i=0; i<pCase->nRanges; i++)
            {
                if(i>0)
                    dmp<<_T(" || ");
           
                if(pCase->ranges[i].ulRangeLength==0)
                {
                    dmp<<pCase->ranges[i].lRangeStart<<_T("=="JAVA_CASE_TMP_VAR);
                }
                else
                {
                    dmp<<_T('(')<<pCase->ranges[i].lRangeStart;
                    dmp<<_T("<="JAVA_CASE_TMP_VAR" && "JAVA_CASE_TMP_VAR"<=");
                    //CAUTION: long + ulong == ulong, this is bad for negative range ends
                    dmp<<(long)(pCase->ranges[i].lRangeStart + pCase->ranges[i].ulRangeLength);
                    dmp<<_T(')');
                }
            }
            dmp<<_T(")\n"CRLO"\n");

    	    dmp.incrIndent();
            PC_DumpSTStmts(dmp, pCase->pStmts, pctx);
            dmp.decrIndent();
        }
        if(pS->cases.pElse)
        {
            dmp<< _T(CRLC"\nelse\n"CRLO"\n");
    	    dmp.incrIndent();
            PC_DumpSTStmts(dmp, pS->cases.pElse, pctx);
    	    dmp.decrIndent();
        }

        dmp<<_T(CRLC"\n\n");
    }
    break;
    case CG_IF:
    {
        ConsiderBP(dmp, pS, pctx); 

        dmp<<_T("if(");
        PC_DumpCGExprVal(dmp, pS->ifs.pCond, pctx);
        dmp<<_T(")\n"CRLO"\n");
        
        dmp.incrIndent();
        PC_DumpSTStmts(dmp, pS->ifs.pThen, pctx);
        dmp.decrIndent();
        
        if(pS->ifs.pElse)
        {
            dmp<< _T(CRLC"\nelse\n"CRLO"\n");
            dmp.incrIndent();
            PC_DumpSTStmts(dmp, pS->ifs.pElse, pctx);
            dmp.decrIndent();
        }
        dmp<<_T(CRLC"\n");
    }
    break;
    case CG_REPEAT:
    {
        ConsiderBP(dmp, pS, pctx);

        dmp<<_T("do\n"CRLO"\n");
        dmp.incrIndent();
        dmp<<_T("tc.checkLoop();\n");
        PC_DumpSTStmts(dmp, pS->reps.pStmts, pctx);
        dmp.decrIndent();
        dmp<<_T(CRLC"while(!");
        PC_DumpCGExprVal(dmp, pS->reps.pUntil, pctx);
        dmp<<_T(");\n\n"); 
    }
    break;
    case CG_WHILE:
    {
        ConsiderBP(dmp, pS, pctx); 
        
        dmp<<_T("\nwhile(");
        PC_DumpCGExprVal(dmp, pS->whls.pWhile, pctx);
        dmp<<_T(")\n"CRLO"\n");
        dmp.incrIndent();
        dmp<<_T("tc.checkLoop();\n");
        PC_DumpSTStmts(dmp, pS->whls.pStmts, pctx);
        dmp.decrIndent();
        dmp<<_T(CRLC"\n");
    }
    break;
    case CG_FOR:
    {
        ConsiderBP(dmp, pS, pctx); 
        PC_DumpCGFor(dmp, &pS->edp, &pS->fors, pctx);
    }
    break;
    case CG_GOTO:
    {
        ConsiderBP(dmp, pS, pctx); 

        pctx->pInf->m_bHasGOTO = true;
        if(pS->gots.bUpward)
        {
            //CAUTION: if upward jump we must call the software watchdog to
            //detect possible endless lops, same problem in all repeate statements...
		    dmp<<_T("tc.checkLoop();\n");
        }
    
        dmp<<JAVA_GOTO_STATE_VAR _T(" = ");
        dmp<<pS->gots.pLblStmt->lbls.nLblNum+1<<_T(";if(");
        dmp<<_T("true");
        dmp<<_T(") continue ")JAVA_GOTO_FOR _T(";//");
        dmp<<pS->gots.pLblStmt->lbls.pszLbl<<_T("\n");
    }
    break;
    case CG_LABEL:
    {
        if(pS->lbls.bUsed) //only if used, otherwise the m_bHasGOTO makes no sense
        {
            dmp.decrIndent();
            dmp<<_T("case ")<<pS->lbls.nLblNum+1<<_T(": //");
            dmp<<pS->lbls.pszLbl<<_T('\n');
            dmp.incrIndent();
        }
    }
    break;
    case CG_EXIT:
    {
        ConsiderBP(dmp, pS, pctx); 
        dmp<<_T("if(true)");
        dmp<<_T("break;\n");
    }
    break;
    case CG_RETURN:
    {
        ConsiderBP(dmp, pS, pctx); 
        //special case on return from POU (not Action!): insert another BP fun that 
        //belongs to the end of POU edp (parm #3 == NULL means: don't insert into bplist)
        if(pctx->BPoption != BP_OPT0 && pctx->nTransi<0 && pctx->nAction<0)
            DumpBPFun(dmp, pctx->pIBdy->edpEnd(), pctx->pSym, NULL, 1);

        if(pctx->pSym->pouTyp == CG_POU_FUN)
        {
            PC_DumpFunctionReturn(dmp, pctx, true);
        }
        else
        {
            dmp<<_T("if(true)return;\n");
        }
    }
    break;
    case CG_EMPTY:
    {
        ConsiderBP(dmp, pS, pctx); 
    }
    break;
    case CG_NATIVE:
    {
        ConsiderBP(dmp, pS, pctx); 

        pctx->pInf->m_bHasNative = true;
	    dmp<<_T("/*NATIVE*/");
	    dmp<<pS->nats.pszNative;
	    dmp<<_T("/*END_NATIVE*/\n");
    }
    break;
    case CG_SYNCHRONIZED:
    {
        ConsiderBP(dmp, pS, pctx); 

        dmp<<_T("try\n"CRLO"\n"); 
        dmp.incrIndent();
        dmp<<_T("DebugManager.beginSynchronized();\n");
        PC_DumpSTStmts(dmp, pS->syns.pStmts, pctx);

        dmp.decrIndent();
        dmp<<_T(CRLC"\nfinally\n"CRLO"\n");
        dmp.incrIndent();
        dmp<<_T("DebugManager.endSynchronized();\n");
        dmp.decrIndent();
        dmp<<_T(CRLC"\n");
    }
    break;
    case CG_LDFBDNETWORK:
        if(pS->netw.pStmts->styp == CG_LABEL)
        {
            //special case case labels should not passed to DumpLDFBDNetwork
            //because DumpLDFBDNetwork dumps { ... } around the java 'case:'
            //assert: lables are always networks by topology:
            assert(!pS->netw.pStmts->pNext);
            PC_DumpSTStmts(dmp, pS->netw.pStmts, pctx);
        }
        else
        {
            DumpLDFBDNetwork(dmp, pS->netw.pStmts, pctx);
        }
    break;
    default:
        assert(!"bad stmt type enum");
        return;
    };//end union switch(stmt)


    pS = pS->pNext;
    goto Next;
}



/*
for index := left to right by st do
 stmts;
end_for;

will be dumped as:

int st2 = st;
index = left;
if (st2 > 0 && right >= left)
   N = left - right + 32767;
else if (st2 < 0 && right <= left)
   N = right - left + 32767;
else
   st2 = N = 0;

while (st2!=0)
{
   stmts; 
   index = index + st2;
   if (32767 + st2>0?-st2:st2 < N)
      exit;
   N = N - st2>0?-st2:st2;
}


if limits and stepby are literals and overflow is imposible
then there is a SIMPLE SOLUTION and for loop will be

for (index = left; index <= right; index+= st)
{
    stmts;
}
*/  
static void PC_DumpCGFor(
    CGT_CDmp&            dmp, 
    const CG_Edipos*     pEdp,
    const CG_StmtFor*    pFor,
    const BODY_CTX_INFO* pctx
)
{
    const CG_DType*const& pType     = pFor->pAssign->ops.pLhs->pType;
    
    const CG_ExprTree*const& pIndex = pFor->pAssign->ops.pLhs;
    const CG_ExprTree*const& pLeft  = pFor->pAssign->ops.pRhs;
    const CG_ExprTree*const& pRight = pFor->pToExpr;
    const CG_ExprTree*const& pBy    = pFor->pByExpr;

    __int64 maxValue;
    assert(pType->tbs & (CG_TBS_INT | CG_TBS_DINT));
    assert(pIndex->extyp == CG_VARREF);
    if(pType->type == CG_INT)
        maxValue = SHRT_MAX;
    else 
        maxValue = INT_MAX;

    // the SIMPLE case
    if (pLeft->extyp == CG_LITERAL && pRight->extyp == CG_LITERAL && (!pBy || pBy->extyp == CG_LITERAL))
    {
        if ((!pBy || pBy->lit.pLit->i64 > 0) && pLeft->lit.pLit->i64 <= pRight->lit.pLit->i64)
        {
            if (maxValue - (pBy?pBy->lit.pLit->i64:1) >= pRight->lit.pLit->i64) // no overflow detection needed
            {
                // for (index = left; index <= right; index+= st)
                dmp << _T("for(");
                PC_DumpCGAssign(dmp, pIndex, pLeft, pctx);
                dmp << _T(";");
                PC_DumpCGExprVal(dmp, pIndex, pctx);
                dmp << _T("<=");
                PC_DumpCGExprVal(dmp, pRight, pctx);
                dmp << _T(";");
                if (IsJavaBasicType(pctx->BPoption, pIndex->var.pEntry))
                {
                    PC_DumpCGExprRef(dmp, pIndex, pctx);
                    dmp << _T("=(");
                    PC_DumpCGExprRef(dmp, pIndex, pctx);
                }
                else
                {
                    PC_DumpCGExprRef(dmp, pIndex, pctx);
                    dmp<<_T('.')<<GetPutFunctionFromType(pType->type)<<_T('(');
                    PC_DumpCGExprVal(dmp, pIndex, pctx);
                }
                dmp << _T("+");
                if (pBy)
                    if(pBy->extyp == CG_LITERAL || !IsJavaBasicType(pctx->BPoption, pBy->var.pEntry))
                        PC_DumpCGExprVal(dmp, pBy, pctx);                    
                    else
                        PC_DumpCGExprRef(dmp, pBy, pctx);                    
                else 
                    dmp << _T("1");
                dmp << _T("))\n"CRLO"\n");
                dmp.incrIndent();
                PC_DumpSTStmts(dmp, pFor->pStmts, pctx);
                dmp << _T("tc.checkLoop();\n");
                dmp.decrIndent();
                dmp<<_T(CRLC"\n\n"); 
                return;
            }
            // else go to overflow detection algorithm
        }
        else if (pBy && pBy->lit.pLit->i64 < 0 && pLeft->lit.pLit->i64 >= pRight->lit.pLit->i64)
        {
            if (-maxValue-1 - pBy->lit.pLit->i64 <= pRight->lit.pLit->i64) // no overflow detection needed
            {
                // for (index = left; index >= right; index+= st)
                dmp << _T("for(");
                PC_DumpCGAssign(dmp, pIndex, pLeft, pctx);
                dmp << _T(";");
                PC_DumpCGExprVal(dmp, pIndex, pctx);
                dmp << _T(">=");
                PC_DumpCGExprVal(dmp, pRight, pctx);
                dmp << _T(";");
                if (IsJavaBasicType(pctx->BPoption, pIndex->var.pEntry))
                {
                    PC_DumpCGExprRef(dmp, pIndex, pctx);
                    dmp << _T("=(");
                    PC_DumpCGExprRef(dmp, pIndex, pctx);
                }
                else
                {
                    PC_DumpCGExprRef(dmp, pIndex, pctx);
                    dmp<<_T('.')<<GetPutFunctionFromType(pType->type)<<_T('(');
                    PC_DumpCGExprVal(dmp, pIndex, pctx);
                }
                dmp << _T("+");
                if (pBy)
                    if(pBy->extyp == CG_LITERAL || !IsJavaBasicType(pctx->BPoption, pBy->var.pEntry))
                        PC_DumpCGExprVal(dmp, pBy, pctx);                    
                    else
                        PC_DumpCGExprRef(dmp, pBy, pctx);                    
                else 
                    dmp << _T("1");
                dmp << _T("))\n"CRLO"\n");
                dmp.incrIndent();
                PC_DumpSTStmts(dmp, pFor->pStmts, pctx);
                dmp << _T("tc.checkLoop();\n");
                dmp.decrIndent();
                dmp<<_T(CRLC"\n\n"); 
                return;
            }
            // else go to overflow detection algorithm
        }
        else
        {
            // for (index = left; index != left;)  // loop statements never reached
            dmp << _T("for(");   
            PC_DumpCGAssign(dmp, pIndex, pLeft, pctx);
            dmp << _T(";");
            if (IsJavaBasicType(pctx->BPoption, pIndex->var.pEntry))
                PC_DumpCGExprRef(dmp, pIndex, pctx);
            else
                PC_DumpCGExprVal(dmp, pIndex, pctx);
            dmp << _T("!=");
            if(pLeft->extyp == CG_LITERAL || !IsJavaBasicType(pctx->BPoption, pLeft->var.pEntry))
                PC_DumpCGExprVal(dmp, pLeft, pctx);
            else
                PC_DumpCGExprRef(dmp, pLeft, pctx);
            dmp << _T(";)\n"CRLO"\n");   
            dmp.incrIndent();
            PC_DumpSTStmts(dmp, pFor->pStmts, pctx);
            dmp << _T("tc.checkLoop();\n");
            dmp.decrIndent();
            dmp<<_T(CRLC"\n\n"); 

            pctx->pdm->msg1(CG_W_UNREACHABLE_STMT, pEdp, _T("FOR"));
            return;
        }
    }
    // for loop with overflow detection algorithm
    {
    int     nNVar   = pctx->pInf->getFreeTmpVar(pType);
    int     nst2Var;
    __int64 st2Val  = 0;
    bool    isByLit = false;
    bool    isIfDumped = false;
    
    if (!pBy || pBy->extyp == CG_LITERAL)
    {
        st2Val = pBy?pBy->lit.pLit->i64:1;
        isByLit = true;
    }
    nst2Var = pctx->pInf->getFreeTmpVar(pType);
    // int st2 = st;
    PC_DumpJavaStackTmpVarName(dmp, nst2Var);
    dmp << _T(" = ");
    if (pBy)
        PC_DumpCGExprVal(dmp, pBy, pctx);
    else
        dmp << _T("1");
    dmp << _T(";\n");

    // index = left;
    PC_DumpCGAssign(dmp, pIndex, pLeft, pctx);
    dmp << _T(";\n\n");

    // if (st2 > 0 AND right >= left)
    if ((isByLit && st2Val <=0) || 
        (pRight->extyp==CG_LITERAL && pLeft->extyp==CG_LITERAL && pRight->lit.pLit->i64<pLeft->lit.pLit->i64))
        goto skip_if1;

    isIfDumped = false;
    if (!isByLit)
    {
        dmp << _T("if(");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T(">0");
        isIfDumped = true;
    }
    if (pRight->extyp!=CG_LITERAL || pLeft->extyp!=CG_LITERAL)
    {
        if (isIfDumped)
            dmp << _T(" && ");
        else 
            dmp << _T("if(");
        PC_DumpCGExprVal(dmp, pRight, pctx);
        dmp << _T(">=");
        PC_DumpCGExprVal(dmp, pLeft, pctx);
        isIfDumped = true;
    }
    if (isIfDumped)
    {
        dmp << _T(")\n");
        dmp.incrIndent();
    }
    // N = left - right + 32767;
    PC_DumpJavaStackTmpVarName(dmp, nNVar);
    dmp << _T("=");
    PC_DumpCGExprVal(dmp, pLeft, pctx);
    dmp << _T(" - ");
    PC_DumpCGExprVal(dmp, pRight, pctx);
    dmp << _T(" + ") << maxValue << _T(";\n");
    if (isIfDumped)
    {
        dmp.decrIndent();
        dmp << _T("else ");
    }
    else
        goto begin_loop;

skip_if1:
    // if (st2 < 0 and right <= left)
    if ((isByLit && st2Val >=0) || 
        (pRight->extyp==CG_LITERAL && pLeft->extyp==CG_LITERAL && pRight->lit.pLit->i64>pLeft->lit.pLit->i64))
        goto skip_if2;

    isIfDumped = false;
    if (!isByLit)
    {
        dmp << _T("if(");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T("<0");
        isIfDumped = true;
    }
    if (pRight->extyp!=CG_LITERAL || pLeft->extyp!=CG_LITERAL)
    {
        if (isIfDumped)
            dmp << _T(" && ");
        else 
            dmp << _T("if(");
        PC_DumpCGExprVal(dmp, pRight, pctx);
        dmp << _T("<=");
        PC_DumpCGExprVal(dmp, pLeft, pctx);
        isIfDumped = true;
    }
    if (isIfDumped)
    {
        dmp << _T(")\n");
        dmp.incrIndent();
    }
    // N = right - left + 32767;
    PC_DumpJavaStackTmpVarName(dmp, nNVar);
    dmp << _T("=");
    PC_DumpCGExprVal(dmp, pRight, pctx);
    dmp << _T(" - ");
    PC_DumpCGExprVal(dmp, pLeft, pctx);
    dmp << _T(" + ") << maxValue << _T(";\n");
    if (isIfDumped)
    {
        dmp.decrIndent();
        dmp << _T("else ");
    }
    else
        goto begin_loop;

skip_if2:
    // st2 = N = 0;
    dmp.incrIndent();
    PC_DumpJavaStackTmpVarName(dmp, nst2Var);
    dmp << _T("=");
    PC_DumpJavaStackTmpVarName(dmp, nNVar);
    dmp << _T("=0;\n\n");
    dmp.decrIndent();  

begin_loop:
    // while (st2!=0)
    dmp << _T("while(");
    PC_DumpJavaStackTmpVarName(dmp, nst2Var);
    dmp << _T("!=0");
    dmp << _T(")\n"CRLO"\n");
    dmp.incrIndent();
    // stmts; 
    PC_DumpSTStmts(dmp, pFor->pStmts, pctx);
    // index = index + st2;
    if (IsJavaBasicType(pctx->BPoption, pIndex->var.pEntry))
    {
        PC_DumpCGExprRef(dmp, pIndex, pctx);
        dmp << _T("=(");
        PC_DumpCGExprRef(dmp, pIndex, pctx);
    }
    else
    {
        PC_DumpCGExprRef(dmp, pIndex, pctx);
        dmp<<_T('.')<<GetPutFunctionFromType(pType->type)<<_T('(');
        PC_DumpCGExprVal(dmp, pIndex, pctx);
    }
    dmp << _T("+");
    if (!isByLit)
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
    else
        dmp << st2Val;
    dmp << _T(");\n");
    // if (32767 + st2>0?-st2:st2 < N)
    dmp << _T("if(") << maxValue << _T("+");
    if (!isByLit)
    {
        dmp << _T("(");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T(">0?-");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T(":");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T(")");
    }    
    else 
        dmp << (st2Val>0?-st2Val:st2Val);
    dmp << _T("<");
    PC_DumpJavaStackTmpVarName(dmp, nNVar);
    dmp << _T(")\n");
    // exit;
    dmp.incrIndent();
    dmp << _T("break;\n");
    dmp.decrIndent();
    // N -= st2>0?-st2:st2;
    PC_DumpJavaStackTmpVarName(dmp, nNVar);
    dmp << _T("-=");
    if (!isByLit)
    {
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T(">0?-");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
        dmp << _T(":");
        PC_DumpJavaStackTmpVarName(dmp, nst2Var);
    }    
    else 
        dmp << (st2Val>0?-st2Val:st2Val);
    dmp << _T(";\n");
    dmp << _T("tc.checkLoop();\n");
    dmp.decrIndent();
    dmp<<_T(CRLC"\n\n");     

    if (!isByLit)
        pctx->pInf->freeTmpVar(nst2Var);
    pctx->pInf->freeTmpVar(nNVar);
    }
}



void PC_DumpCGAssign(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs,
    const BODY_CTX_INFO* pctx
)
{
    CG_DTYPES  type = pLhs->pType->type;
    
    assert(type == pRhs->pType->type);

    if(pLhs->extyp == CG_TMPVAR)
    {
        const TMP_VAR_TYP* pTmpDecl =  pctx->pInf->m_ldfbdtmp.getAddrOf(pLhs->tmp.lTmpNum-1);
            
        if(pLhs->tmp.pRefExpr)
        {
            PC_DumpLDFBDStackTmpVarName(dmp, pLhs->tmp.lTmpNum);
            dmp<<_T(" = ");
            PC_DumpCGExprRef(dmp, pRhs, pctx);
        }
        else if(CG_IsMultiElemType(pLhs->pType))
        {
            assert(pRhs->extyp != CG_TMPVAR);//<-not supported array,fb,struct must not pass as tmp!

            //temp_var = user_var.newInitializedClone()
            PC_DumpLDFBDStackTmpVarName(dmp, pLhs->tmp.lTmpNum);
            dmp<<_T(" = ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(".newInitializedClone()");
        }
        else if(pTmpDecl->bFCTmpObj)
        {
            PC_DumpLDFBDStackTmpVarName(dmp, pLhs->tmp.lTmpNum);
            dmp<<_T('.')<<GetPutFunctionFromType(pLhs->pType->type)<<_T('(');
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
        else //java stack type.
        {
            PC_DumpLDFBDStackTmpVarName(dmp, pLhs->tmp.lTmpNum);
            dmp<<_T(" = ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
        }
    }
    else if(pLhs->extyp == CG_TRIGVAR)
    {
        //simple boolean member tmp var:
        dmp<<JAVA_TRIGTMP_PRFIX << pLhs->trg.lTrigNum;
        dmp<<_T(" = ");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
    }
    //##AC beg
    else if(pLhs->extyp == CG_USEREXPR)
    {
        if(pLhs->usr.ldata==PC_USEREXPR_RAWJAVA)
        {
            dmp<<pLhs->usr.psz1;
            if(CG_IsMultiElemType(pLhs->pType)) 
            {
                dmp<<_T(".assignFrom(");
                PC_DumpCGExprVal(dmp, pRhs, pctx);
                dmp<<')';
            }
            else
            {
                dmp << _T(" = ");
                PC_DumpCGExprVal(dmp, pRhs, pctx);
            }
        }
        else
        {
            assert(!"bad value in pExpr->usr.ldata");
        }
    }
    //##AC end
    else if(pctx->pSym->pouTyp==CG_POU_FUN)
    {   
        if(CG_IsMultiElemType(pLhs->pType)) 
        {
            //always ok (may change in future if local struct/array supp.
            PC_DumpCGExprRef(dmp, pLhs, pctx);
            dmp<<_T(".assignFrom(");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
        else if(pLhs->extyp==CG_VARREF)
        {
            if(IsJavaBasicType(pctx->BPoption, pLhs->var.pEntry))
            {
                JavaId(dmp, pLhs->var.pEntry->pszId);
                dmp << _T(" = ");
                PC_DumpCGExprVal(dmp, pRhs, pctx);
            }
            else
            {
                PC_DumpCGExprRef(dmp, pLhs, pctx);
                dmp<<_T('.')<<GetPutFunctionFromType(type)<<_T('(');
                PC_DumpCGExprVal(dmp, pRhs, pctx);
                dmp<<_T(')');
            }
        }
        else
        {
            //reach here for x[i] := <expr> or x.z := <expr>
            //x is currently passed as input/inout, local complex types
            //are not supported, but this would be ok here anyway..
            PC_DumpCGExprRef(dmp, pLhs, pctx);
            dmp<<_T('.')<<GetPutFunctionFromType(type)<<_T('(');
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
    }
    else if(CG_IsMultiElemType(pLhs->pType))
    {
        PC_DumpCGExprRef(dmp, pLhs, pctx);
        dmp<<_T(".assignFrom(");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    }
    else
    {
        PC_DumpCGExprRef(dmp, pLhs, pctx);
        dmp<<_T('.')<<GetPutFunctionFromType(type)<<_T('(');
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    }
}

//##AC beg
void PC_DumpFBCall(
    const BODY_CTX_INFO*        pctx,
    CGT_CDmp&                   dmp, 
    const CG_ExprTree*          pInst,     //strict IEC: always a CG_VARREF, future 
                                           //version may support array of FBs
    long                        nArgs,     //>=0 number of in/out assignments
    const CG_ArgExpr*           pArgs,     //NULL or array of arg expr.
    const PC_InlineArg*const    pInlArgs[],//optinal extra in/out arguments form marco call
    int                         nInlArgs   //sizeof pInlArgs
)
{
    int                 i;
    const CG_ArgExpr*   pAE;
    const PC_InlineArg* pIA;
    CG_ExprTree         stracc;
    
    //process input assignments before the call:
    for(i = 0; i<nArgs; i++)
    {
        pAE = &pArgs[i];
        if(pAE->argFlags & CG_EXPR_ARG_OUTASN)
            continue;//is an output assignment.

        if(pAE->pFormal->declScope==CG_VAR_INPUT)
        { 
            //create normal assignment
            stracc.extyp      = CG_STRUCTREF;
            stracc.edp        = pAE->pActual->edp;
            stracc.pType      = pAE->pActual->pType;//HACK ?? use acutal expr's type to avoid R|F_EDGE
                                                    //to appear in stracc
            stracc.str.pBase  = pInst;
            stracc.str.pSelc  = pAE->pFormal;
            PC_DumpCGAssign(dmp, &stracc, pAE->pActual, pctx);
            dmp<<_T(";\n");
        }
        else if(pAE->pFormal->declScope==CG_VAR_IN_OUT)
        { 
            //inout special handling with pAE->accFlags read comment 
            //at definition of CG_ArgExpr in CG_IECFront.h : TODO extra test
            long lInoutDecl = pAE->pFormal->lInstNum;
            assert(pInst->pType->pSym->nInOut>0 && lInoutDecl>=0 && lInoutDecl<pInst->pType->pSym->nInOut);

            if(lInoutDecl >= pctx->pInf->m_maxFBinoutArray)
                pctx->pInf->m_maxFBinoutArray = lInoutDecl + 1;

            dmp<<JAVA_INOUT_ARRAY _T("[")<<lInoutDecl<<_T("] = (FcObject)");
            PC_DumpCGExprRef(dmp, pAE->pActual, pctx);    
            dmp<<_T(";\n");
        }
        else
        {
            assert("bad decl scope for FB input");
        }
    }

    for(i = 0; i<nInlArgs; i++)
    {
        pIA = pInlArgs[i];
        if(!pIA->m_bInput)
            continue;

        if(pIA->m_pFormal->declScope!=CG_VAR_IN_OUT)
        { 
            //create normal assignment
            stracc.extyp      = CG_STRUCTREF;
            stracc.edp        = pIA->m_actual.edp;
            stracc.pType      = pIA->m_actual.pType;//HACK ?? use acutal expr's type to avoid R|F_EDGE
                                                    //to appear in stracc
            stracc.str.pBase  = pInst;
            stracc.str.pSelc  = pIA->m_pFormal;
            PC_DumpCGAssign(dmp, &stracc, &pIA->m_actual, pctx);
            dmp<<_T(";\n");
        }
        else
        { 
            //inout special handling with pAE->accFlags read comment 
            //at definition of CG_ArgExpr in CG_IECFront.h : TODO extra test
            long lInoutDecl = pIA->m_pFormal->lInstNum;
            assert(pInst->pType->pSym->nInOut>0 && lInoutDecl>=0 && lInoutDecl<pInst->pType->pSym->nInOut);

            if(lInoutDecl >= pctx->pInf->m_maxFBinoutArray)
                pctx->pInf->m_maxFBinoutArray = lInoutDecl + 1;

            dmp<<JAVA_INOUT_ARRAY _T("[")<<lInoutDecl<<_T("] = (FcObject)");
            PC_DumpCGExprRef(dmp, &pIA->m_actual, pctx);    
            dmp<<_T(";\n");
        }
    }


    //dump _fbinst.exec(..)
    PC_DumpCGExprVal(dmp, pInst, pctx);
    if(pInst->pType->pSym->nInOut > 0)
        dmp<<_T(".exec(globals, ") JAVA_INOUT_ARRAY _T(", tc);\n");
    else
        dmp<<_T(".exec(globals, null, tc);\n");


    //process output assignments after the call:
    for(i=0; i<nArgs; i++)
    {
        pAE = &pArgs[i];
        if((pAE->argFlags & CG_EXPR_ARG_OUTASN)==0)
            continue;//is an input assignment.
            
        if(pAE->pFormal->declScope==CG_VAR_IN_OUT)
        {
            long lInoutDecl = pAE->pFormal->lInstNum;
            assert(lInoutDecl>=0 && lInoutDecl<pAE->pFormal->pParent->nInOut);
            assert(lInoutDecl < pctx->pInf->m_maxFBinoutArray);
            assert((pAE->argFlags & CG_EXPR_ARG_NEGOUT)==0);
            //inout-output assignment:   
            //a) inoutmember => referencetmpvar   (copy only the obj reference)
            //b) inoutmember => l-val.            (deep copy)

            if(pAE->pActual->extyp==CG_TMPVAR && pAE->pActual->tmp.pRefExpr)
            {//a)
                //dump e.g.:   _T1 = ((FcBool)_Inouts[0]);

                PC_DumpCGExprRef(dmp, pAE->pActual, pctx);
                dmp<< _T(" = ");
                dmp<<_T("((")<<GetFcInterfaceNameFromType(pAE->pFormal->pType)<<_T(')');
                dmp<<JAVA_INOUT_ARRAY _T("[")<<lInoutDecl<<(_T("])"));
                dmp<<_T(";\n");
            }
            else
            {//b)
                assert(pAE->pActual->extyp!=CG_TMPVAR);//<-not supported signal split at inout out
                                                       //the next code would be not correct if such a tmp 
                                                       //is a java stack typ.
                //dump e.g.:    _bvar.putBool(((FcBool)_Inouts[0]).getBool());
                PC_DumpCGExprRef(dmp, pAE->pActual, pctx);
                if(CG_IsMultiElemType(pAE->pActual->pType))
                    dmp<<_T(".assignFrom(");
                else 
                    dmp<<_T('.')<<GetPutFunctionFromType(pAE->pActual->pType->type)<<_T('(');

                dmp<<_T("((")<<GetFcInterfaceNameFromType(pAE->pFormal->pType)<<_T(')');
                dmp<<JAVA_INOUT_ARRAY _T("[")<<lInoutDecl<<(_T("])"));
            
                //if is simple need the value call get-function:
                if(!CG_IsMultiElemType(pAE->pActual->pType))
                    dmp<<_T('.')<<GetGetFunctionFromType(pAE->pActual->pType->type)<<_T("()");

                dmp<<_T(");\n");
            }
        }
        else 
        {
            //normal output => l-val assign:
            assert(pAE->pFormal->declScope==CG_VAR_OUTPUT);
            stracc.extyp      = CG_STRUCTREF;
            stracc.edp        = pAE->pActual->edp;
            stracc.pType      = pAE->pFormal->pType;
            stracc.str.pBase  = pInst;
            stracc.str.pSelc  = pAE->pFormal;
            
            if(pAE->argFlags & CG_EXPR_ARG_NEGOUT)
            {
                CG_ExprTree  notop;
                assert(pAE->pFormal->pType->type == CG_BOOL);
                notop.extyp = CG_NOT;
                notop.edp   = pAE->pActual->edp;
                notop.pType = pAE->pFormal->pType;
                notop.ops.pLhs = &stracc;
                notop.ops.pRhs = NULL; 
                
                PC_DumpCGAssign(dmp, pAE->pActual, &notop, pctx);
            }
            else
            {
                PC_DumpCGAssign(dmp, pAE->pActual, &stracc, pctx);
            }
            
            dmp<<_T(";\n");
        }
    }







    for(i = 0; i<nInlArgs; i++)
    {
        pIA = pInlArgs[i];
        if(pIA->m_bInput)
            continue;

        if(pIA->m_pFormal->declScope!=CG_VAR_IN_OUT)
        { 
            //normal output => l-val assign:
            stracc.extyp      = CG_STRUCTREF;
            stracc.edp        = pIA->m_actual.edp;
            stracc.pType      = pIA->m_pFormal->pType;
            stracc.str.pBase  = pInst;
            stracc.str.pSelc  = pIA->m_pFormal;
            
            PC_DumpCGAssign(dmp, &pIA->m_actual, &stracc, pctx);
            dmp<<_T(";\n");
        }
        else
        {
            assert(!"not supported: inline FB call with 'inout => '");
        }
    }
}
//##AC end




void PC_DumpCGExprVal(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pExpr,
    const BODY_CTX_INFO* pctx
)
{
    
    switch(pExpr->extyp)
    { 
    case CG_LITERAL:
        //assert for lit it must be the same type in expr and lit struct:
        assert(pExpr->pType->type==pExpr->lit.pLit->type);
        PC_DumpJavaInitValue(dmp, pExpr->lit.pLit);
    break;
    case CG_VARREF:
    {
        CG_DTYPES entryType = pExpr->var.pEntry->pType->type;
        if(entryType==CG_BOOL_R_EDGE || entryType==CG_BOOL_F_EDGE)
        {
            assert(pExpr->pType->type == CG_BOOL);
            (dmp<<JAVA_TRIG_PREFIX).lo(pExpr->var.pEntry->pszId);
        }
        else if(CG_IsMultiElemType(pExpr->var.pEntry->pType))
        {
            assert(pExpr->pType->type == entryType);
            JavaId(dmp, pExpr->var.pEntry->pszId);
        }
        else
        {
            assert(pExpr->pType->type == entryType);
            JavaId(dmp, pExpr->var.pEntry->pszId);
            
            if(!IsJavaBasicType(pctx->BPoption, pExpr->var.pEntry))
            {
                dmp<<_T('.')<<GetGetFunctionFromType(pExpr->pType->type)<<_T("()");
            }
        }
        pctx->pInf->addBdyUsed(pExpr->var.pEntry);
    }
    break;
    case CG_STRUCTREF:
    {
        PC_DumpCGExprRef(dmp, pExpr, pctx);
        if(!CG_IsMultiElemType(pExpr->pType))
            dmp<<_T('.')<<GetGetFunctionFromType(pExpr->pType->type)<<_T("()");
    }
    break;
    case CG_ARRAYREF:
    {
        PC_DumpCGExprRef(dmp, pExpr, pctx);
        if(!CG_IsMultiElemType(pExpr->pType))
           dmp<<_T('.')<<GetGetFunctionFromType(pExpr->pType->type)<<_T("()");
    }
    break;
    default: 
    {
       PC_DumpOperator(dmp, pctx, pExpr->pType->type, pExpr->extyp, 
           pExpr->ops.pLhs, pExpr->ops.pRhs);
    }
    break;
    case CG_FUNCALL:
    {
        int               i;
        const CG_ArgExpr* pAE;

        // ST/FBD/LD function
        assert(pExpr->fun.pSym->flags & CG_STAB_HDR_NOERR);       
        
        // dump Function name with prefix:
        (dmp<<JAVA_FUNCTION_PRE).lo(pExpr->fun.pSym->pszId)<<_T("(");
        //TODO test this, should be done by front:checking of WSTRING[len] len
        for(i=0; i<pExpr->fun.nArgs; i++)
        {
            pAE = &pExpr->fun.pArgs[i];
            if(pAE->pFormal->declScope == CG_VAR_OUTPUT ||
               pAE->pFormal->declScope == CG_VAR_IN_OUT
             )
            {
                PC_DumpCGExprRef(dmp, pAE->pActual, pctx);
            }
            else
            {
                PC_DumpCGExprVal(dmp, pAE->pActual, pctx);
            }
            dmp<<_T(", ");
        }

        // dump TaskContext parameter and globals:
        dmp<< _T("globals, tc)");
    }
    break;
    case CG_FUNMACRO:
    {
        if(pExpr->mac.pMac->pszMac[0]==_T('@'))
        {
            PC_DumpInlineFunOpMacro(
                dmp, 
                pctx,
                &pExpr->mac.pMac->pszMac[1],
                pExpr->mac.nArgs,
                pExpr->mac.pArgs
            );
        }
        else
        {
            PC_DumpInlineFunMacro(
                dmp, 
                &pExpr->edp, 
                pctx,
                pExpr->mac.nArgs,
                pExpr->mac.pArgs,
                pExpr->mac.pMac->pSym,
                pExpr->mac.pMac->pszMac,
                &pExpr->mac.pMac->edp
            );

        }
    }
    break;
    case CG_BUILDIN:
        DumpBuildInOps(dmp, &pExpr->edp, pctx, pExpr->bin.pszId, pExpr->bin.nArgs, pExpr->bin.pArgs);
    break;
    case CG_TMPVAR:
    {
        const TMP_VAR_TYP* pTmpDecl =  pctx->pInf->m_ldfbdtmp.getAddrOf(pExpr->tmp.lTmpNum-1);
        PC_DumpLDFBDStackTmpVarName(dmp, pExpr->tmp.lTmpNum);
        if((pTmpDecl->bFCTmpObj || pTmpDecl->bRef) && !CG_IsMultiElemType(pExpr->pType))
            dmp<<_T('.')<<GetGetFunctionFromType(pExpr->pType->type)<<_T("()");
    }
    break;
    case CG_TRIGVAR:
    {
        dmp<<JAVA_TRIGTMP_PRFIX << pExpr->trg.lTrigNum;
    }
    break;
    case CG_STEPACTION:
    {
        assert(pExpr->sfc.iSANum>=0);
        switch (pExpr->sfc.typ)
        {
        case CG_STEP_X:
            dmp<<_T("getStepX(")<<pExpr->sfc.iSANum<<_T(")");
        break;
        case CG_STEP_T:
            dmp<<_T("getStepT(")<<pExpr->sfc.iSANum<<_T(")");
        break;
        case CG_ACTION_A:
            dmp<<_T("getActionA(")<<pExpr->sfc.iSANum<<_T(")");
        break;
        case CG_ACTION_Q:
            dmp<<_T("getActionQ(")<<pExpr->sfc.iSANum<<_T(")");
        break;
        default:
            assert(!"bad enum val in CG_SFCEXpr::typ");
        }
    }
    //##AC beg
    break;
    case CG_USEREXPR:
    {
        if(pExpr->usr.ldata==PC_USEREXPR_RAWJAVA)
        {
            dmp<<pExpr->usr.psz1;
        }
        else
        {
            assert(!"bad value in pExpr->usr.ldata");
        }
    }
    break;
    case CG_FBCALLEXPR:
    {
        assert(!"CG_FBCALLEXPR unexpected");
    }
    break;
    case CG_LABELREF:
    {
        assert(pExpr->lbl.nLblNum>=0);
        pctx->pInf->m_bHasGOTO = true;
        dmp<<pExpr->lbl.nLblNum+1;
    }
    //##AC end
    break;
    }; //end switch;
}









void PC_DumpCGExprRef(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pExpr,
    const BODY_CTX_INFO* pctx
)
{
    switch(pExpr->extyp)
    { 
    case CG_VARREF:
    {
        CG_DTYPES entryType = pExpr->var.pEntry->pType->type;

        assert(pExpr->var.pEntry->pParent==pctx->pSym);
        assert(pExpr->pType->type == entryType);//<-will fail also if entry is edge

        JavaId(dmp, pExpr->var.pEntry->pszId);
        pctx->pInf->addBdyUsed(pExpr->var.pEntry);
    }
    break;
    case CG_STRUCTREF:
    {
        dmp<<_T("((")<<GetFcInterfaceNameFromType(pExpr->pType)<<')';
        PC_DumpCGExprVal(dmp, pExpr->str.pBase, pctx);
        dmp<<_T(".getObjectByID(")<<PC_GetObjNumOfEntry(pExpr->str.pSelc)<<_T("))");
    }
    break;
    case CG_ARRAYREF:
    {
        assert(pExpr->arr.pBase->pType->bIsArray && 
               pExpr->arr.pBase->pType->nDim<=1 && pExpr->arr.nSubs==1);
    
        dmp<<_T("((")<<GetFcInterfaceNameFromType(pExpr->pType)<<_T(')');
        
        PC_DumpCGExprVal(dmp, pExpr->arr.pBase, pctx);

        dmp<<_T(".getObjectByID(");
        
        if(pExpr->arr.pSubs[0].pType->type==CG_LINT)
        {
            //TODO can this ever happen??
            dmp<< _T("(int)(");
            PC_DumpCGExprVal(dmp, &pExpr->arr.pSubs[0], pctx);
            dmp<< ')';
        }
        else
        {
            PC_DumpCGExprVal(dmp, &pExpr->arr.pSubs[0], pctx);
        }

        if(pExpr->arr.pBase->pType->nDim==1)
        {
            long lrs = pExpr->arr.pBase->pType->range[0].lRangeStart;
            if(lrs > 0)
            {
                dmp<< '-' << lrs;
            }
            else if(lrs < 0)
            {
                dmp<< '+' << -(lrs);
            }
        }//else assumed size arrays start at 0:

        dmp<<_T("))");
    }
    break;
    case CG_TMPVAR:
    {
        PC_DumpLDFBDStackTmpVarName(dmp, pExpr->tmp.lTmpNum);
    }
    break;
    default: 
        assert(!"PC_DumpCGExprRef illegal call");

    }; //end switch;
}

























/*
Precedence of ST ops:
0. [] ()
1. NOT
2. **
3. '*' '/' MOD 
4. '+' '-'
5.  < <= > >=
6. = <>
7. & AND
8. XOR
9. OR    

Table of Java precedence taken from 'Learn Java Now'
Java precedence 	java operator                ST Precedence/Op
1. 		            . [ ] ( )      			    0. [] ()
2. 		            ++ -- ! ~ instanceof		1. NOT	
3. 		            * / %					    3. '*' '/' MOD
4. 		            + -					        4. '+' '-' 
5. 		            << >> >>>				
6. 		            < > <= >=				    5. < <= > >=
7. 		            == !=					    6. = <>
8. 		            &
9. 		            ^                           8. XOR 
10. 		        |
11. 		        &&                          7. AND
12. 		        ||                          9. OR
13. 		        ?:
14. 		        = op=
15. 		        ,

The only difference in Precedence is the XOR / ^ Operator.
Conclusion: Although precedence is quite the same in 
Java and ST, we do the following to be 100% save:
 
ST_Operator_C::dumpValue dumps always
  ( op expr)
  or 
  (expr op expr)

  he result of ST_Operator_C::Dump is an java expression 
  inside "()"
*/
void PC_DumpOperator(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    CG_DTYPES            lTypeSet,
    CG_EXPRTYP           opEnum,
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs
)
{
    CG_DTYPES typ1;
    CG_DTYPES typ2;

    switch(opEnum)
    {
    case CG_NOT:
        assert(!pRhs);
        switch(lTypeSet)
        {
        case CG_BOOL:
            dmp<<_T("(!");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(')');
        break;
        case CG_BYTE:
            dmp<<_T("(~");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" & 0xff)");
        break;
        case CG_WORD:
            dmp<<_T("(~");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" & 0xffff)");
        break;
        case CG_DWORD:
        case CG_LWORD:
            dmp<<_T("(~");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(")");
        break;
        default:
             assert(!"illegal type");
        }
    break;
    case CG_UMINUS:
        assert(!pRhs);

        //assert(/*TODO: ANY SIGNED (lTypeSet & CG_ANY_SIGNED)==lTypeSet || lTypeSet==CG_TIME*/);

        dmp<<_T("(- ");//<-CAUTION: dump (- expr) otherwise eg - -3, + -5 may be a problem!
        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(')');
    break;
    case CG_NOOP:
        dmp<<_T('(');
        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(')');
    break;
    case CG_OR:
        assert(CG_IsAnyBit(lTypeSet));

        dmp<<_T('(');
        PC_DumpCGExprVal(dmp, pLhs, pctx);

        if(lTypeSet==CG_BOOL)
            dmp<<_T(" || ");
        else
            dmp<<_T(" | ");
        
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    break;
    case CG_XOR:
        assert(CG_IsAnyBit(lTypeSet));

        dmp<<_T('(');
        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(" ^ ");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    break;
    case CG_AND:
        assert(CG_IsAnyBit(lTypeSet));

        dmp<<_T('(');
        PC_DumpCGExprVal(dmp, pLhs, pctx);

        if(lTypeSet==CG_BOOL)
            dmp<<_T(" && ");
        else
            dmp<<_T(" & ");
        
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    break;
    case CG_EQ:

        typ1 = pLhs->pType->type;
        if(typ1==CG_WSTRING)
        {
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(".compareTo( ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(")==0)");
        }
        else
        {
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" == ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
    break;
    case CG_NE:

        typ1 = pLhs->pType->type;
        if(typ1==CG_WSTRING)
        {
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(".compareTo( ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(")!=0)");
        }
        else
        {
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" != ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
    break;

    case CG_LT:

        typ1 = pLhs->pType->type;
        assert(typ1==pRhs->pType->type); 
        
        switch(typ1)
        {
        case CG_BOOL:
            // B1 < B2 ==> (!B1 && B2)
            dmp<<_T("(!");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" && ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        case CG_WSTRING:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(".compareTo(");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(")<0)");
        break;
        case CG_DWORD:
            //trick:  expr1 ^ 0x80000000 : conversion like unsigned int
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x80000000) < (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x80000000))");
        break;
        case CG_LWORD:
            //trick:  expr1 ^ 0x80000000 00000000 : conversion like unsigned long
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l) < (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l))");
        break;
        case CG_BYTE:
        case CG_WORD:
        case CG_DINT:
        case CG_LINT:
        case CG_LREAL:
        case CG_TIME:
        case CG_DATE:
        case CG_TOD:
        case CG_DT:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" < ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        default:
            assert(!"bad type");
        }
    break;
    case CG_LE:

        typ1 = pLhs->pType->type;
        assert(typ1==pRhs->pType->type); 
        
        switch(typ1)
        {
        case CG_BOOL:
            // B1 <= B2 ==> (!B1 || B2)
            dmp<<_T("(!");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" || ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        case CG_WSTRING:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(".compareTo(");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(")<=0)");
        break;
        case CG_DWORD:
            //trick:  expr1 ^ 0x80000000 : conversion like unsigned int
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x80000000) <= (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x80000000))");
        break;
        case CG_LWORD:
            //trick:  expr1 ^ 0x80000000 00000000 : conversion like unsigned long
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l) <= (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l))");
        break;
        case CG_BYTE:
        case CG_WORD:
        case CG_DINT:
        case CG_LINT:
        case CG_LREAL:
        case CG_TIME:
        case CG_DATE:
        case CG_TOD:
        case CG_DT:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" <= ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        default:
            assert(!"bad type");
        }
    break;
    case CG_GT:

        typ1 = pLhs->pType->type;
        assert(typ1==pRhs->pType->type); 
        switch(typ1)
        {
        case CG_BOOL:
            // B1 > B2 ==> (B1 && !B2)
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" && !");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        case CG_WSTRING:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(".compareTo(");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(")>0)");
        break;
        case CG_DWORD:
            //trick:  expr1 ^ 0x80000000 : conversion like unsigned int
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x80000000) > (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x80000000))");
        break;
        case CG_LWORD:
            //trick:  expr1 ^ 0x80000000 00000000 : conversion like unsigned long
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l) > (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l))");
        break;
        case CG_BYTE:
        case CG_WORD:
        case CG_DINT:
        case CG_LINT:
        case CG_LREAL:
        case CG_TIME:
        case CG_DATE:
        case CG_TOD:
        case CG_DT:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" > ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        default:
            assert(!"bad type");
        }
    break;
    case CG_GE:

        typ1 = pLhs->pType->type;
        assert(typ1==pRhs->pType->type); 
        
        switch(typ1)
        {
        case CG_BOOL:
            // B1 >= B2 ==> (B1 || !B2)
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" || !");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        case CG_WSTRING:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(".compareTo(");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(")>=0)");
        break;
        case CG_DWORD:
            //trick:  expr1 ^ 0x80000000 : conversion like unsigned int
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x80000000) >= (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x80000000))");
        break;
        case CG_LWORD:
            //trick:  expr1 ^ 0x80000000 00000000 : conversion like unsigned long
            dmp<<_T("((");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l) >= (");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(" ^ 0x8000000000000000l))");
        break;
        case CG_BYTE:
        case CG_WORD:
        case CG_DINT:
        case CG_LINT:
        case CG_LREAL:
        case CG_TIME:
        case CG_DATE:
        case CG_TOD:
        case CG_DT:
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" >= ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        break;
        default:
            assert(!"bad type");
        }
    break;
    case CG_ADD:

        typ1 = pLhs->pType->type;
        if(typ1==CG_TOD)
        {
            assert(pRhs->pType->type==CG_TIME);
            dmp<<_T("Utils.add_tod_time(");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(", ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
        else
        {
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" + ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
    break;
    case CG_SUB:

        typ1 = pLhs->pType->type;
        if(typ1==CG_DATE)
        {
            dmp<<_T("(86400000l*(");
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" - ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T("))");
        }
        else if(typ1==CG_TOD)
        {
            if(pRhs->pType->type==CG_TOD)
            {
                dmp<<_T("(long)(");
                PC_DumpCGExprVal(dmp, pLhs, pctx);
                dmp<<_T(" - ");
                PC_DumpCGExprVal(dmp, pRhs, pctx);
                dmp<<_T(')');
            }
            else
            {
                dmp<<_T("Utils.add_tod_time(");
                PC_DumpCGExprVal(dmp, pLhs, pctx);
                dmp<<_T(", - ");
                PC_DumpCGExprVal(dmp, pRhs, pctx);
                dmp<<_T(')');
            }
        }
        else
        {
            dmp<<_T('(');
            PC_DumpCGExprVal(dmp, pLhs, pctx);
            dmp<<_T(" - ");
            PC_DumpCGExprVal(dmp, pRhs, pctx);
            dmp<<_T(')');
        }
    break;
    case CG_MUL:
        if(pLhs->pType->type==CG_TIME)
        {
            if(CG_IsAnyReal(pRhs->pType->type))
                dmp<<_T("(long)");
        }
        dmp<<_T('(');
        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(" * ");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    break;
    case CG_DIV:
        if(pLhs->pType->type==CG_TIME)
        {
            if(CG_IsAnyReal(pRhs->pType->type))
                dmp<<_T("(long)");
        }
        dmp<<_T('(');
        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(" / ");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    break;
    case CG_MOD:

        assert(CG_IsAnyInt(lTypeSet));
        if(lTypeSet==CG_LINT)
        {
            dmp<<_T("Utils.mod_l(");
        }
        else
        {
            dmp<<_T("Utils.mod_i(");
        }
        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(", ");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');
    break;
    case CG_POW:

        typ2 = pRhs->pType->type;
        assert(pLhs->pType->type==CG_LREAL);
        
        if(typ2==CG_LREAL)
        {
            dmp<<_T("Utils.expt_ddd(");
        }
        else if(typ2==CG_DINT)
        {
            dmp<<_T("Utils.expt_ddi(");
        }
        else if(typ2==CG_LINT)
        {
            dmp<<_T("Utils.expt_ddl(");
        }
        else
        {
            assert(!"illegal type");
        }

        PC_DumpCGExprVal(dmp, pLhs, pctx);
        dmp<<_T(", ");
        PC_DumpCGExprVal(dmp, pRhs, pctx);
        dmp<<_T(')');

    break;
    case CG_MOVE:
        PC_DumpCGAssign(dmp, pLhs, pRhs, pctx);
    break;
    default:
        assert(!"illegal enum");
    }
}






void PC_DumpJavaStackTmpVarName(CGT_CDmp&dmp, int iNum)
{
    assert(iNum>0);
    dmp<<JAVA_TMPVAR_PREFIX<<iNum;
}
void PC_DumpLDFBDStackTmpVarName(CGT_CDmp&dmp, int iNum)
{
    assert(iNum>0);
    dmp<<JAVA_LDFBD_TMPVAR_PREFIX<<iNum;
}



static void PC_DumpULONGAsHex(
    CGT_CDmp&     dmp, 
    unsigned long ul, 
    int           iMinText)
{
    TCHAR itoa_puff[64];

    _ultot(ul,itoa_puff,16);
    for(int i=iMinText - _tcslen(itoa_puff); i>0; i--)
        dmp<<'0';

    dmp<<itoa_puff;
}
static void PC_DumpAsJavaString(
    CGT_CDmp&    dmp, 
    const TCHAR* pszIECStr 
)
{
    long         len     = _tcslen(pszIECStr);
    TCHAR*       pBinStr = new TCHAR[len];
    const TCHAR* psz;

    HRESULT hr = BL_IECWStringToBinary(pszIECStr, pBinStr, &len, NULL, NULL);
    assert(hr == S_OK);

    psz = pBinStr;

    dmp<<_T('"');
    
    for(int i = 0; i < len; i++, psz++)
    {
#ifdef UNICODE
        if(*psz >= 32)
#else
        if((unsigned char)*psz >= 32)
#endif
        {
            switch(*psz)
            {
            case _T('\\'): dmp<<_T("\\\\"); break;
            case _T('"'):  dmp<<_T("\\\""); break;
            case _T('\0'): dmp<<_T("\\0");  break;
            default:       dmp<<*psz;
            }
        }
        else
        {           
            dmp<<_T('\\');
            switch(*psz)
            {
                case _T('\n'): dmp<<_T('n');break; 
                case _T('\r'): dmp<<_T('r');break;
                case _T('\f'): dmp<<_T('f');break;
                case _T('\t'): dmp<<_T('t');break;
                default:
                    //use \uhhhh notation:
                   dmp<<_T('u');
                   #ifdef UNICODE
                    PC_DumpULONGAsHex(dmp, *psz, 4);
                   #else
                    PC_DumpULONGAsHex(dmp, (unsigned char)*psz, 4);
                   #endif
            }
        }   
    }

    dmp<<_T('"');

    delete [] pBinStr;
}



void PC_DumpJavaInitValue(
    CGT_CDmp&         dmp, 
    const CG_Literal* pLit
)
{
    switch(pLit->type)
    {
    case CG_BOOL:
    {
        assert(pLit->ut==CG_LIT_I64 && (pLit->i64==0 || pLit->i64==1));
        dmp<< (pLit->i64 ? _T("true") : _T("false"));
    }
    break;
    case CG_STRING:
    case CG_WSTRING:
    {
        assert(pLit->ut==CG_LIT_NONE && pLit->psz);
        PC_DumpAsJavaString(dmp, pLit->psz);
    }
    break;
    case CG_REAL: 
    case CG_LREAL:
    {
        assert(pLit->ut==CG_LIT_FLT);
        //don't dump opt +:
        if(pLit->psz[0]==_T('+'))
            dmp<< pLit->psz+1;
        else
            dmp<< pLit->psz;
    }
    break;
    case CG_DT:
    case CG_TIME:
    case CG_LINT:
    case CG_LWORD:
    {
        assert(pLit->ut==CG_LIT_I64);
        dmp<<pLit->i64<<_T("l");
    }
    break;
    case CG_TOD:
    case CG_DATE:
    case CG_BYTE:
    case CG_WORD:
    case CG_DWORD:
    case CG_DINT:
    {
        assert(pLit->ut==CG_LIT_I64);
        assert(pLit->i64 >= (-2147483647-1) && pLit->i64 <= 4294967295);
        dmp<<(long)pLit->i64;
    }
    break;
    default:
        assert(!"unsupported data type in lit expr.");       
        dmp<<_T("<ST_DumpJavaInitValue: type error>");
    }
}





const TCHAR* GetFcInterfaceNameFromType(const CG_DType* pType)
{
   
    if(pType->bIsArray)
        return _T("ObjectDirectoryEx");

    switch(pType->type)
    {
    case CG_BOOL_R_EDGE:
    case CG_BOOL_F_EDGE://fall through TODO test this!
    case CG_BOOL:       return _T("FcBool");
    case CG_BYTE:
    case CG_WORD:
    case CG_DWORD:
    case CG_DINT:       return _T("FcInt");
    case CG_LWORD:
    case CG_LINT:       return _T("FcLong");
    case CG_REAL:
    case CG_LREAL:      return _T("FcDouble");
    case CG_TIME:       return _T("FcLong");
    case CG_DATE:       return _T("FcInt");
    case CG_TOD:        return _T("FcInt");
    case CG_DT:         return _T("FcLong");
    case CG_STRING:     
    case CG_WSTRING:    return _T("FcString");
    case CG_STRUCT:     return _T("ObjectDirectoryEx");
    case CG_INSTANCE:   return _T("FunctionBlock");

    case CG_QVT_BOOL:   return _T("FcQvtBool");
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:   return _T("FcQvtInt");
    case CG_QVT_LREAL:  return _T("FcQvtDouble");
    case CG_QVT_LINT:
    case CG_QVT_LWORD:  return _T("FcQvtLong");
    case CG_QVT_WSTRING:return _T("FcQvtString");
    default:            
        //assert: may happen if e.g. Functionblock var_in_out of poly type
        //but pass here only for real poly type!
        assert(!CG_IsMonomorph(pType->type));

        return _T("FcObject");//poly type are 'FcObject' inside native code still usefull
    }
    return _T("<compiler error>");
}



const TCHAR* GetFcTypeNameFromType(const CG_DType* pType)
{
    
    if(pType->bIsArray)
        return _T("system.ArrayVar");
    
    switch(pType->type)
    {
    case CG_BOOL_R_EDGE:
    case CG_BOOL_F_EDGE://fall th.
    case CG_BOOL:       return _T("system.BoolVar");
    case CG_BYTE:       return _T("system.ByteVar");
    case CG_WORD:       return _T("system.WordVar");
    case CG_DWORD:
    case CG_DINT:       return _T("system.IntVar");
    case CG_LWORD:
    case CG_LINT:       return _T("system.LongVar");
    case CG_REAL:
    case CG_LREAL:      return _T("system.DoubleVar");
    case CG_TIME:       return _T("system.LongVar");
    case CG_DATE:       
    case CG_TOD:        return _T("system.IntVar");
    case CG_DT:         return _T("system.LongVar");
    case CG_STRING:     
    case CG_WSTRING:    return pType->lStrLen>=0 ? _T("system.StringVarLimit") : _T("system.StringVar");
    case CG_STRUCT:     return _T("system.StructVar");
    case CG_QVT_BOOL:   return _T("system.QvtBoolVar");
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:   return _T("system.QvtIntVar");
    case CG_QVT_LREAL:  return _T("system.QvtDoubleVar");
    case CG_QVT_LINT:
    case CG_QVT_LWORD:  return _T("system.QvtLongVar");
    case CG_QVT_WSTRING:return _T("system.QvtStringVar");
    default:            assert(!"bad enum");
    }
    return _T("<compiler error>");
}




const TCHAR* GetPutFunctionFromType(CG_DTYPES typ)
{
    switch(typ)
    {
    case CG_BOOL:       return _T("putBool");
    case CG_DINT:       return _T("putInt");
    case CG_LINT:       return _T("putLong");
    case CG_REAL:       
    case CG_LREAL:      return _T("putDouble");
    case CG_TIME:       return _T("putLong");
    case CG_DATE:       return _T("putInt");
    case CG_TOD:        return _T("putInt");
    case CG_DT:         return _T("putLong");
    case CG_STRING:     
    case CG_WSTRING:    return _T("putString");
    case CG_BYTE:       
    case CG_WORD:       
    case CG_DWORD:      return _T("putInt");
    case CG_LWORD:      return _T("putLong");
    case CG_QVT_BOOL:   return _T("putQvtBool");
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:   return _T("putQvtInt");
    case CG_QVT_LREAL:  return _T("putQvtDouble");
    case CG_QVT_LINT:
    case CG_QVT_LWORD:  return _T("putQvtLong");
    case CG_QVT_WSTRING:return _T("putQvtString");
    default:            assert(!"bad enum");
    }
    return _T("<compiler error>");
}

const TCHAR* GetGetFunctionFromType(CG_DTYPES typ)
{
    switch(typ)
    {
    case CG_BOOL:       return _T("getBool");
    case CG_DINT:       return _T("getInt");
    case CG_LINT:       return _T("getLong");
    case CG_REAL:       
    case CG_LREAL:      return _T("getDouble");
    case CG_TIME:       return _T("getLong");
    case CG_DATE:       return _T("getInt");
    case CG_TOD:        return _T("getInt");
    case CG_DT:         return _T("getLong");
    case CG_STRING:     
    case CG_WSTRING:    return _T("getString");
    case CG_BYTE:       
    case CG_WORD:       
    case CG_DWORD:      return _T("getInt");
    case CG_LWORD:      return _T("getLong");
    case CG_QVT_BOOL:   return _T("getQvtBool");
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:   return _T("getQvtInt");
    case CG_QVT_LREAL:  return _T("getQvtDouble");
    case CG_QVT_LINT:
    case CG_QVT_LWORD:  return _T("getQvtLong");
    case CG_QVT_WSTRING:return _T("getQvtString");
    default:            assert(!"bad enum");
    }
    return _T("<compiler error>");
}


const TCHAR* GetJavaBasicTypeName(
    const CG_DType* pType, 
    bool            bReference
)
{
    if(bReference)
        return GetFcInterfaceNameFromType(pType);

    if(pType->bIsArray)
        return _T("ObjectDirectoryEx");

    switch(pType->type)
    {
    case CG_BOOL:       return _T("boolean");
    case CG_DINT:       return _T("int");
    case CG_LINT:       return _T("long");
    case CG_REAL:       
    case CG_LREAL:      return _T("double");
    case CG_TIME:       return _T("long");
    case CG_DATE:       return _T("int");
    case CG_TOD:        return _T("int");
    case CG_DT:         return _T("long");
    case CG_STRING:     
    case CG_WSTRING:    return _T("String");
    case CG_BYTE:       
    case CG_WORD:       
    case CG_DWORD:      return _T("int");
    case CG_LWORD:      return _T("long");

    case CG_INSTANCE:
    case CG_STRUCT:
    case CG_PROGRAM:    return _T("ObjectDirectoryEx");

    case CG_QVT_BOOL:   return _T("QvtBoolValue");
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:   return _T("QvtIntValue");
    case CG_QVT_LREAL:  return _T("QvtDoubleValue");
    case CG_QVT_LINT:
    case CG_QVT_LWORD:  return _T("QvtLongValue");
    case CG_QVT_WSTRING:return _T("QvtStringValue");
    }
    assert(!"invalid type");
    return _T("<compiler error>");
}


const TCHAR* ST_GetFunTempVarName(const CG_DType* pType)
{
    const TCHAR* pszFunTempVarName = _T("internal type error!");
        
    switch(pType->type)
    {
    case CG_BOOL:       return _T("TempVarBool");
    case CG_DINT:       return _T("TempVarInt");
    case CG_LINT:       return _T("TempVarLong");
    //case CG_REAL:       
    case CG_LREAL:      return _T("TempVarDouble");
    case CG_TIME:       return _T("TempVarLong");
    case CG_DATE:       return _T("TempVarInt");
    case CG_TOD:        return _T("TempVarInt");
    case CG_DT:         return _T("TempVarLong");
    //case CG_STRING:     
    case CG_WSTRING:    return _T("TempVarString");
    case CG_BYTE:       
    case CG_WORD:       
    case CG_DWORD:      return _T("TempVarInt");
    case CG_LWORD:      return _T("TempVarLong");

    case CG_INSTANCE:
    case CG_STRUCT:
    case CG_PROGRAM:    assert(!"tmp var of complex type");
                        return _T("internal type error!");

    case CG_QVT_BOOL:   return _T("TempVarQvtBool");
    case CG_QVT_BYTE:
    case CG_QVT_WORD:
    case CG_QVT_DWORD:
    case CG_QVT_DINT:   return _T("TempVarQvtInt");
    case CG_QVT_LREAL:  return _T("TempVarQvtDouble");
    case CG_QVT_LINT: 
    case CG_QVT_LWORD:  return _T("TempVarQvtLongValue");
    default:
        assert(!"tmp var of unsupported type");
        return _T("internal type error!");
    }
}




















































//local dirty helper functions:
static const TCHAR* PC_NextMacroArg(
    const TCHAR* psz, 
    CGT_CDmp&    dmp, 
    int*         pNum
)
{
    for( ;*psz; psz++)
    {
        if(*psz==_T('%'))
        {        
            if(isdigit(psz[1]))
            {
                psz++;
                TCHAR* pszEnd;
                *pNum = (int)_tcstol(psz, &pszEnd, 10);
                return pszEnd;
            }//%nargs
            else if(psz[1]==_T('n') && psz[2]==_T('a') && 
                    psz[3]==_T('r') && psz[4]==_T('g') && psz[5]==_T('s'))
            {
                //%va:  var arg array 
                psz +=6;
                *pNum = -1;
                return psz;
            }//%__edipos
            else if(psz[1]==_T('_') && psz[2]==_T('_') && 
                    psz[3]==_T('e') && psz[4]==_T('d') && psz[5]==_T('i') &&
                    psz[6]==_T('p') && psz[7]==_T('o') && psz[8]==_T('s'))
            {
                //%__edipos:
                psz +=9;
                *pNum = -2;
                return psz;
            }//%const:
            else if(psz[1]=='c' && psz[2]=='o' && psz[3]=='n' && psz[4]=='s' && psz[5]=='t' && psz[6]==':')
            {
                //%const:
                psz +=7;
                *pNum = -3;
                return psz;
            }
            else if(psz[1]==_T('%'))
            {
                //%% escape:
                psz +=1;
            }
            else
            {
                *pNum = -10;
                return psz;//error
            }
        }
        dmp<<*psz;
    }
    return NULL;
}

static bool PC_UsedTwice(
    int          argNum, 
    const TCHAR* pszRestOfInlineCode
)
{
    TCHAR buffer[50];

    buffer[0] = _T('%');
    _itot(argNum, buffer+1, 10);
    
    //CAUTION %1 matches %11
    while((pszRestOfInlineCode = _tcsstr(pszRestOfInlineCode, buffer)) != NULL)
    {
        pszRestOfInlineCode += _tcslen(buffer);
        if(!isdigit(*pszRestOfInlineCode))
            return true;
    }
    
    return false;
}


static bool PC_NeedsTmpVar(const CG_ExprTree* pExpr)
{
    switch(pExpr->extyp)
    {
    case CG_TMPVAR:
    case CG_TRIGVAR: //<-will this ever happen?
    case CG_LITERAL:
    case CG_STEPACTION:
        return false;
    default:
        return true;
    }
}

static bool PC_NeedsParent(const CG_ExprTree* pExpr)
{
    switch(pExpr->extyp)
    {
    case CG_TMPVAR:
    case CG_TRIGVAR: //<-will this ever happen?
    case CG_LITERAL:
    case CG_STEPACTION:
    case CG_VARREF:
        return false;
    default:
        return true;
    }
}


static PC_DumpInlineFunOpMacro(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const TCHAR*         pszOp,//some java text used like an operator
    int                  nArgs,
    const CG_ArgExpr     pAE[]   //the argument list, NULL if no args
)
{
    int  i;
    int  nBr;
    bool bParent;
 
    if(__iscsym(pszOp[0]))
    {
        ///max(n1,max(n2,n3))
        //max(n1,max(n2,max(n3,n4)))
        for(i = 0, nBr=0; i<nArgs; i++)
        {
            if(i+1<nArgs)
            {
                dmp<<pszOp<<_T('(');
                nBr++;
            }

            if(pAE[i].pFormal->declScope==CG_VAR_OUTPUT ||
               pAE[i].pFormal->declScope==CG_VAR_IN_OUT
            )
            {
                PC_DumpCGExprRef(dmp, pAE[i].pActual, pctx);
            }
            else
            {
                PC_DumpCGExprVal(dmp, pAE[i].pActual, pctx);
            }
            if(i+1<nArgs)
                dmp<<_T(',');
        }
        
        while(--nBr>=0)
            dmp<<_T(')');
    }
    else
    {
        dmp<<_T('(');
        for(i = 0; i<nArgs; i++)
        {
            if(i>0)
                dmp<<pszOp;
 
            bParent = PC_NeedsParent(pAE[i].pActual);
            if(bParent)
                dmp<<_T('(');
            
            if(pAE[i].pFormal->declScope==CG_VAR_OUTPUT ||
               pAE[i].pFormal->declScope==CG_VAR_IN_OUT
            )
            {
                //will virtually never happen ....
                PC_DumpCGExprRef(dmp, pAE[i].pActual, pctx);
            }
            else
            {
                PC_DumpCGExprVal(dmp, pAE[i].pActual, pctx);
            }
            if(bParent)
                dmp<<_T(')');
        }
        dmp<<_T(')');
    }
}



static const TCHAR* PC_ParseInlineArgs(
    const BODY_CTX_INFO*        pctx, 
    const CG_Edipos*            pEdp,
    const TCHAR**               ppsz, 
    const CG_Symtab*            pSymFB,
    FC_CGArray<PC_InlineArg*>*  pArgLst           
)
{
    const TCHAR*        psz;
    const TCHAR*        pszEnd;
    CG_DECLSCOPE        ds;
    TCHAR               szBuff[MAX_PATH];
    bool                bInput;
    const CG_SymEntry*  pEntry;

    pArgLst->setUsed(0);

    psz = *ppsz;

    if(!FC_StringIncBegin(&psz, _T("#callfb(")) || !( pszEnd = _tcsstr(psz, _T(");")) ))
        return _T("FB call expression must be followed by '#callfb([<in/out list>]);'");
    
    
    //id[:=|=>](<java expr>) 
    while(psz < pszEnd)
    {
        if(FC_StrGetID(&psz, 1, szBuff, FC_ARRAY_LEN(szBuff))!=0)
            return _T("syntax: expected identifer");

        pEntry = pctx->pIF->lookUpEntry(pSymFB, szBuff);
        if(!pEntry)
            return _T("not a member of the function block");

        if(psz[0]==':' && psz[1]=='=')
        {
            ds = pEntry->declScope;
            if(ds!=CG_VAR_INPUT && ds!=CG_VAR_IN_OUT && ds!=CG_VAR)
                return _T("FB-call: ':=': member not accessible");

            bInput = true;
            psz += 2;
        }
        else if(psz[0]=='=' && psz[1]=='>')
        {
            ds = pEntry->declScope;
            if(ds!=CG_VAR_OUTPUT && ds!=CG_VAR)
                return _T("FB-call: '=>': member neither VAR_OUTPUT nor VAR (inout-out assign are not supported here)");

            bInput = false;
            psz += 2;
        }
        else
        {
            return _T("syntax: expected ':=' or '=>'");
        }
        
        FC_StringSkipWhite(&psz);
        for(int i=0; psz < pszEnd && *psz!=',' && i<MAX_PATH; )
            szBuff[i++] = *psz++;

        if(*psz==',')
            psz++;

        if(i<MAX_PATH)
            szBuff[i] = 0;
        else
            return _T("syntax: token too long");

        FC_StringTrimRight(szBuff);
        if(pArgLst->getMaxAlloc()==0)
            pArgLst->allocSize(32);
        pArgLst->add(new PC_InlineArg(pEdp, bInput, pEntry, szBuff));
    }

    *ppsz = psz+2; //+2 for ");"
    return NULL;
}

static void PC_DumpInlineFunMacro(
    CGT_CDmp&           dmp, 
    const CG_Edipos*    pEdp,
    const BODY_CTX_INFO*pctx,
    int                 nActArg,
    const CG_ArgExpr    pAE[],
    const CG_Symtab*    pFun,
    const TCHAR*        pszMac,
    const CG_Edipos*    pEdpMac
)
{
    int           argNum;
    const TCHAR*  psz;
    int           maxArgNum = 0;
    int*          ipActArgTmpVarNum = NULL;
    bool          bFoundActArg;
    const TCHAR*  pszHint = NULL; 


    if(!pctx->pInf->m_bHasGOTO)
        pctx->pInf->m_bHasGOTO = FC_StrFindId(pszMac, _T("_GotoFor")) != NULL;//##AC

    if(nActArg>0)
    {
        ipActArgTmpVarNum = new int[nActArg];
        memset(ipActArgTmpVarNum, 0, (nActArg)*sizeof(int));
    }

    psz = pszMac;


    //do not () if void for e.g  #PRAGMA inline{if(%1)tc.userBreak(this)}
    if(pFun->pFirst->pType->type != CG_VOID)
        dmp<<_T('(');

    while((psz = PC_NextMacroArg(psz, dmp, &argNum))!=NULL)
    {
        if(argNum<=0)
        {
            //special casees:
            if(argNum==-1)
            {
                //dump number of actual arguments:
                dmp<<nActArg;
            }
            else if(argNum==-2)
            {
                //edipos of inline fun call as string:
                TCHAR    szEdipos[2*MAX_PATH];
                int      len;
                pctx->pIF->edpToStr(pEdp, szEdipos+1, FC_ARRAY_LEN(szEdipos)-2);
                szEdipos[0] = _T('"');
                len = _tcslen(szEdipos);
                szEdipos[len++] = _T('"');
                szEdipos[len]   = _T('\0');
                assert(_tcschr(szEdipos, '$')==NULL); //TODO: '$' in edipos is not supported in current version
                PC_DumpAsJavaString(dmp, szEdipos);
            }
            else if(argNum==-3)
            {
                //%const:
                TCHAR szId[MAX_PATH];
                if(FC_StrGetID(&psz, 0, szId, FC_ARRAY_LEN(szId))!=0)
                {
                    pszHint = _T("syntax: expected identifier after %const:");
                    goto Error;
                }
                const CG_SymEntry* pCst= pctx->pIF->lookUpCONST(szId);//TOOD: global search works only as 
                //long as all const are project global also those from attached all libs, this is itself a todo..!!
                if(!pCst)
                {
                    pszHint = _T("%const:<id> is undefined in the project");
                    goto Error;
                }
                assert(pCst->pInit->ut==CG_INIT_SIMPLE);
                PC_DumpJavaInitValue(dmp, pCst->pInit->sim.pLit);
            }
            else
            {
                goto Error;
            }
            continue;
        }

        if(maxArgNum<argNum)
            maxArgNum = argNum;

        bFoundActArg = false;
        if(argNum<=nActArg)//<-optimization if search is useless anyway
        {
            bFoundActArg = true;
            argNum -= 1;

            if(ipActArgTmpVarNum[argNum]!=0)
            {
                PC_DumpJavaStackTmpVarName(dmp, ipActArgTmpVarNum[argNum]);
            }
            else if(pAE[argNum].pActual->extyp==CG_FBCALLEXPR)
            {
                FC_CGArray<PC_InlineArg*> aList;
                const CG_FBCallExpr*      pfbc = &pAE[argNum].pActual->fbc;
                pszHint = PC_ParseInlineArgs(pctx, pEdp, &psz, pfbc->pInst->pType->pSym, &aList);
                if(pszHint)
                    goto Error;

                int idenOld = dmp.getIndent();
                dmp.setIndent(dmp.column()/4);
                    dmp<<_T("//inline FB call expansion:\n");
                    PC_DumpFBCall(pctx, dmp, pfbc->pInst, pfbc->nArgs, pfbc->pArgs, aList.getCElements(), aList.getUsed());
                dmp.setIndent(idenOld);
                
               aList.deletePtrs();
            }
            else if(pAE[argNum].pFormal->declScope==CG_VAR_OUTPUT ||
                    pAE[argNum].pFormal->declScope==CG_VAR_IN_OUT
            )
            {
                //if output arg dump VarBase no tmp var even if used twice!
                PC_DumpCGExprRef(dmp, pAE[argNum].pActual, pctx);
            }
            else if(PC_NeedsTmpVar(pAE[argNum].pActual) && 
                    PC_UsedTwice(argNum+1, psz)
            )
            {
                ipActArgTmpVarNum[argNum] = 
                   pctx->pInf->getFreeTmpVar(pAE[argNum].pActual->pType);
         
                dmp<<_T('(');
                PC_DumpJavaStackTmpVarName(dmp, ipActArgTmpVarNum[argNum]);
                dmp<< _T('=');
                PC_DumpCGExprVal(dmp, pAE[argNum].pActual, pctx);
                dmp<< _T(')');
            }
            else
            {
                PC_DumpCGExprVal(dmp, pAE[argNum].pActual, pctx);
            }
        }
        
        //default value may follow %<num> inside "()"
        if(*psz==_T('('))
        {
            psz++;

            //skip/dump default value:
            while(*psz && *psz!=_T(')'))
            {
                if(bFoundActArg)
                    psz++;
                else
                    dmp<<*psz++;
            }
            if(*psz!=_T(')'))
                goto Error;

            psz++;
        }
        else 
        {
            //default value needed if there is no actArg with number %<num>:
            if(!bFoundActArg)
                goto Error;
        }
    }

    if(pFun->pFirst->pType->type != CG_VOID)
        dmp<<_T(')');
    

    //check that the implementation has enough %arNums for 
    //the actual call
    if(pFun->pVarInExt && maxArgNum<nActArg)
    {
        //Warning: do not treat this as inline code error, it simply a not supported error (old solution of V1.3 still TODO)
        pctx->pdm->msg2(CG_E_FUN_OVERLOAD_NO_BODY_ARGC, pEdp,
               pFun->pszId, pctx->pdm->jot1()<<nActArg);
    }

    delete(ipActArgTmpVarNum);
    return;

Error:
    if(pszHint)
        pctx->pdm->msg1(CG_E_TEXT, pEdpMac, pctx->pdm->jot1()<<_T("inline code expansion: ")<<pszHint);
    else
        pctx->pdm->msg0(CG_E_MACRO_EXPANSION, pEdpMac);
    delete(ipActArgTmpVarNum);
}
//##AC end


static void DumpBuildInOps
(
    CGT_CDmp&           dmp, 
    const CG_Edipos*    pEdp,
    const BODY_CTX_INFO*pctx,
    const TCHAR*        pszId, 
    int                 nArgs,
    const CG_ExprTree*  pArgs
)
{
    CG_DTYPES  tkw;
    bool       bParent;

    //always dump parent!!
    dmp<<_T('(');

    if(nArgs==1 && !_tcsicmp(pszId, _T("QualityOf")))
    {
        bParent = PC_NeedsParent(&pArgs[0]);
        if(bParent)dmp<<_T('(');
        PC_DumpCGExprVal(dmp, &pArgs[0], pctx);
        if(bParent)dmp<<_T(')');         
        dmp<<_T(".q");
    }
    else if(nArgs==1 && !_tcsicmp(pszId, _T("ValueOf")))
    {
        bParent = PC_NeedsParent(&pArgs[0]);
        if(bParent)dmp<<_T('(');
        PC_DumpCGExprVal(dmp, &pArgs[0], pctx);
        if(bParent)dmp<<_T(')');         
        dmp<<_T(".v");
    }
    else if(nArgs==1 && !_tcsicmp(pszId, _T("TimestampOf")))
    {
        bParent = PC_NeedsParent(&pArgs[0]);
        if(bParent)dmp<<_T('(');
        PC_DumpCGExprVal(dmp, &pArgs[0], pctx);
        if(bParent)dmp<<_T(')');         
        dmp<<_T(".t");
    }
    else if(nArgs==3)
    {
        tkw = pctx->pIF->getEnumFromTypeKW(pszId);
        switch(tkw)
        {
        case CG_QVT_BOOL:   dmp<<_T("new QvtBoolValue");   break;
        case CG_QVT_BYTE:   
        case CG_QVT_WORD:
        case CG_QVT_DWORD:  dmp<<_T("new QvtIntValue");    break;
        case CG_QVT_LWORD:  dmp<<_T("new QvtLongValue");   break;
        case CG_QVT_REAL:   goto error;
        case CG_QVT_LREAL:  dmp<<_T("new QvtDoubleValue"); break;
        case CG_QVT_SINT:   
        case CG_QVT_INT:    goto error;
        case CG_QVT_DINT:   dmp<<_T("new QvtIntValue");    break;
        case CG_QVT_LINT:   dmp<<_T("new QvtLongValue");   break;
        case CG_QVT_USINT:
        case CG_QVT_UINT:
        case CG_QVT_UDINT:
        case CG_QVT_ULINT: 
        case CG_QVT_DT:
        case CG_QVT_TOD:
        case CG_QVT_DATE:
        case CG_QVT_TIME:
        case CG_QVT_STRING: goto error;  
        case CG_QVT_WSTRING:dmp<<_T("new QvtStringValue"); break;
        default:
            goto error;
        }
 
        dmp<<_T("(");
        PC_DumpCGExprVal(dmp, &pArgs[0], pctx);
        dmp<<_T(",");
        PC_DumpCGExprVal(dmp, &pArgs[1], pctx);
        dmp<<_T(",");
        PC_DumpCGExprVal(dmp, &pArgs[2], pctx);
        dmp<<_T(")");
    }
    else
    {
        goto error;
    }

    dmp<<_T(')');

    return;

error:
    CGT_IERR2(pctx->pdm, pEdp, _T("bad buildin function call"));
}








