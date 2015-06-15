/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/exprcode.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: exprcode.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/exprcode.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include <limits.h>

#include "vmasm.h"
#include "cgt_CDiagMsg.h"
#include "helper.h"
#include "cgt_misc.h"
#include "Parse4CH.h"
#include "dump4asm.h"
#include "VmConstants.h"
#include "resource.h"

int VMSplitMacroText(
	const TCHAR* pszMac, 
	FC_CStrList* pCode, 
	FC_CStrList* pCmnt, 
	bool         bTrim
);

static void VMDumpForLoop(
    CGT_CDmp&            dmp, 
    const CG_Edipos*     pEdp,
    const CG_StmtFor*    pFor,
    const BODY_CTX_INFO* pctx
);
static void VMDmpFBCall(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pInst,//strict IEC: always a CG_VARREF, future 
                              //version may support array of FBs
    long                 nArgs,//>=0 number of in/out assignments
    const CG_ArgExpr*    pArgs,//NULL or array of arg expr.
    const BODY_CTX_INFO* pctx,
    const CG_MacroBody*  pMac  //!=NULL if macro FB
);
static VM_STACKTYP getInlineCodeCmd(const TCHAR* psz, int* pBitSize)
{
    *pBitSize = 0;

    if(!_tcscmp(psz, _T("push1") ))
    {
        *pBitSize = 1;
        return VM_PUSH_VALUE;
    }
    if(!_tcscmp(psz, _T("push8")))
    {
        *pBitSize = 8;
        return VM_PUSH_VALUE;
    }
    if(!_tcscmp(psz, _T("push16")))
    {
        *pBitSize = 16;
        return VM_PUSH_VALUE;
    }
    if(!_tcscmp(psz, _T("push32")))
    {
        *pBitSize = 32;
        return VM_PUSH_VALUE;
    }
    if(!_tcscmp(psz, _T("push64")))
    {
        *pBitSize = 64;
        return VM_PUSH_VALUE;
    }



    if(!_tcscmp(psz, _T("pop1")))
    {
       *pBitSize = 1;
       return VM_POP_VALUE;
    }
    if(!_tcscmp(psz, _T("pop8")))
    {
       *pBitSize = 8;
       return VM_POP_VALUE;
    }
    if(!_tcscmp(psz, _T("pop16")))
    {
       *pBitSize = 16;
       return VM_POP_VALUE;
    }
    if(!_tcscmp(psz, _T("pop32")))
    {
       *pBitSize = 32;
       return VM_POP_VALUE;
    }
    if(!_tcscmp(psz, _T("pop64")))
    {
       *pBitSize = 64;
       return VM_POP_VALUE;
    }
        

    if(!_tcscmp(psz, _T("lea")))
        return VM_PUSH_WRT_REF;

    if(!_tcscmp(psz, _T("leac")))
        return  VM_PUSH_CST_REF;

    return VM_STK_UNDEF;
}
static void CheckExprBitSize(CGT_CDiagMsg* pdm, const CG_ExprTree* pEx, int bitSize)
{
    bool            bOk;
    const TCHAR*    pszErrHint;
    CG_DTYPES       dt = pEx->pType->type;

    switch(bitSize)
    {
    case 1:
        bOk = dt==CG_BOOL;
        pszErrHint = "BOOL";
    break;
    case 8:
        bOk = dt==CG_BYTE || dt==CG_SINT || dt==CG_USINT;
        pszErrHint = "ANY8";
    break;
    case 16:
        bOk = dt==CG_WORD || dt==CG_INT || dt==CG_UINT;
        pszErrHint = "ANY16";
    break;
    case 32:
        bOk = dt==CG_DWORD || dt==CG_DINT || dt==CG_UDINT || dt==CG_REAL
              || dt==CG_TIME; //TODO add other date&time types here if these types gets implemented 
        pszErrHint = "ANY32";
    break;
    case 64:
        bOk = dt==CG_LWORD || dt==CG_LINT || dt==CG_ULINT || dt==CG_LREAL;
        pszErrHint = "ANY64";
    break;
    default:
        pszErrHint = "??";
        assert(!"bad bitsize");
    }
    
    
    if(!bOk)
    {
        pdm->msg1(CG_E_TEXT, &pEx->edp, 
            pdm->jot1()<<_T("expected an expression of type ")<<pszErrHint
            <<_T(" but found ")<<pEx->pType->pszName);
    }
}


static void DumpArrayInitsFlatRec(CGT_CDmp& dmp, const CG_ArrayInit* pArrInit)
{
    for (int i=0; i<pArrInit->nInits; i++)
    {
        if (i > 0)
            dmp << _T(",");
        
        for (int o=0; o<pArrInit->pRepeats[i]; o++)
        {
            if (o > 0)
                dmp << _T(",");

            if (pArrInit->pInits[i].ut == CG_INIT_ARRAY)
            {
                DumpArrayInitsFlatRec(dmp, &pArrInit->pInits[i].arr);
            }
            else if (pArrInit->pInits[i].ut == CG_INIT_SIMPLE)
            {
                VMDumpLitValue(dmp, pArrInit->pInits[i].sim.pLit);
            }
            else if (pArrInit->pInits[i].ut == CG_INIT_NONE)
            {
                dmp << _T("<>");
            }
            else
            {
                dmp << _T("!!internal compiler error!!");
                assert( ! "invalid array init type");
            }
        }
    }
}

CGT_CDmp& VMDumpArrayInit(CGT_CDmp& dmp, const CG_ArrayInit* pArrInit)
{
    dmp << _T("[");
    for(int i=0; i<pArrInit->nInits; i++)
    {
        if (i > 0)
            dmp << _T(",");

        if (   pArrInit->pRepeats[i] > 1
            || (   pArrInit->pInits[i].ut == CG_INIT_ARRAY
                && pArrInit->pInits[i].arr.nInits > 1
               )
            || pArrInit->pInits[i].ut == CG_INIT_NONE
           )
        {
            dmp << pArrInit->pRepeats[i];
            dmp << _T("(");

            if (pArrInit->pInits[i].ut == CG_INIT_ARRAY)
            {
                DumpArrayInitsFlatRec(dmp, &pArrInit->pInits[i].arr);
            }
            else if (pArrInit->pInits[i].ut == CG_INIT_SIMPLE)
            {
                VMDumpLitValue(dmp, pArrInit->pInits[i].sim.pLit);
            }
            else if (pArrInit->pInits[i].ut == CG_INIT_NONE)
            {
                // we take the init value of the 
                // type declaration (if there is any)
                // ==> we dump nothing
            }
            else
            {
                dmp << _T("!!internal compiler error!!");
                assert( ! "invalid array init type");
            }

            dmp << _T(")");
        }
        else if(pArrInit->pInits[i].ut == CG_INIT_SIMPLE)
        {
            VMDumpLitValue(dmp, pArrInit->pInits[i].sim.pLit);
        }
        else
        {
            dmp << _T("!!internal compiler error!!");
            assert( ! "invalid array init type");
        }
    }
    dmp << _T("]");

    return dmp;
}

CGT_CDmp& CG_VM_DumpBinVal(CGT_CDmp& dmp, const CG_SymEntry* pEntry)
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


CBodyInfo::CBodyInfo(FC_CGArray<CGT_BP_TYP>* pbpList, CGlobCtx* pgctx) :
    m_LabelId         (0),
    m_pLastBPEdp      (NULL),
    m_lastBPNum       (2), //start with 2, 0/1 is reserved for POU beg/end
    m_pBPlist         (pbpList),
    m_UniqueInlineNum (0)
{
    //assert: bp list shoulde be initialliy empty:
    assert(pbpList && pbpList->getUsed()==0);

    m_listTmpExpr.allocSize(25);
}

CBodyInfo::~CBodyInfo()
{
    m_tmpVarExpr.deletePtrs();
}


// Tmp Vars
// attention: we assume that we have no more than 500 LD/FBD Tmp Vars
const int VM_TMPVAR_OFFSET      = 500;

int CBodyInfo::getFreeTmpVar(const CG_DType* pType, bool bRef)
{
    int          i;
    int          iBitSize;
    TMP_VAR_TYP* pTmp;
    TMP_VAR_TYP  tmp;

    // TODO: assert(bRef || pType == simple type)

    if (bRef)
        iBitSize = VM_ADDRESS_SIZE;
    else
        iBitSize = VMSimpleDTypeBitSize(pType);
    
    for(i = m_tmpvars.getUsed()-1; i>=0; i--)
    {
        pTmp = m_tmpvars.getAddrOf(i);
        if( ! pTmp->bUsed && pTmp->iBitSize == iBitSize)
        {
            pTmp->bUsed = true;
            return i + VM_TMPVAR_OFFSET;
        }
    }

    i = m_tmpvars.getUsed();
    if(m_tmpvars.getMaxAlloc()<=i)
        m_tmpvars.allocSize(i+128);
    tmp.bUsed    = true;   
    tmp.iBitSize = iBitSize;

    return m_tmpvars.add(tmp) + VM_TMPVAR_OFFSET;
}

const CG_ExprTree* CBodyInfo::getTmpVarExpr(const CG_DType*  pType,
                                            const CG_Edipos* pEdp)
{
    CG_ExprTree* pExpr = NULL;

    int iTmpNum = getFreeTmpVar(pType, false);

    for (int i=m_tmpVarExpr.getUsed()-1; i>=0; i--)
    {
        pExpr = m_tmpVarExpr.get(i);
        if (pExpr->tmp.lTmpNum == iTmpNum)
            return pExpr;
    }

    FC_NEW_ZSTRUCT(pExpr, CG_ExprTree);
    pExpr->extyp = CG_TMPVAR;
    pExpr->tmp.pRefExpr = NULL;
    pExpr->tmp.lTmpNum = iTmpNum;
    pExpr->pType = pType;
    pExpr->edp = *pEdp;

    m_tmpVarExpr.add(pExpr);

    return pExpr;
}

void CBodyInfo::freeTmpVar(int i)
{
    m_tmpvars.getAddrOf(i - VM_TMPVAR_OFFSET)->bUsed = false;
}

void CBodyInfo::freeTmpVarExpr(const CG_ExprTree* pExpr)
{
    freeTmpVar(pExpr->tmp.lTmpNum);
}

void CBodyInfo::freeTmpVars()
{
    int iTmpUsed = m_listTmpExpr.getUsed();

    for (int i=0; i<iTmpUsed; i++)
    {
        const CG_ExprTree* pExpr = m_listTmpExpr.get(i);
        freeTmpVarExpr(pExpr);
    }

    m_listTmpExpr.resetUsed();
}






void DumpBPFun(
    CGT_CDmp&               dmp, 
    const BODY_CTX_INFO*    pctx,
    const CG_Edipos*        pEdp,
    int                     nBPNum,
    bool                    bNCCOnly
)
{
    CGT_BP_TYP               bps;
    FC_CGArray<CGT_BP_TYP>*  pBPlist;

    pctx->pInf->m_pLastBPEdp = pEdp;

    //##d-1751 HACK: no BP if POU is from any lib ! 
    if(!bNCCOnly && pctx->pSym && pctx->pSym->pLibCtx)
        bNCCOnly = true;


    if(bNCCOnly)
        VMDmpOPC(dmp, _T("stmtbeg"));
    else
        VMDmpOPC(dmp, _T("bp"));

    dmp<< nBPNum << _T("   ;\"");
    dmp<< pctx->pdm->edpToStrNoFile(pEdp, pctx->pdm->jot1()) << _T("\"\n");

    if(bNCCOnly)
        return;

    bps.iBP = nBPNum;
    bps.edp = *pEdp;

    pBPlist = pctx->pInf->m_pBPlist;
    if(pBPlist->getMaxAlloc()==0)
        pBPlist->allocSize(128);

    pBPlist->add(bps);
}


void ConsiderBP(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmt,
    const BODY_CTX_INFO* pctx
)
{
    bool bNCCOnly; 
    assert(pctx->pInf->m_lastBPNum >1);//0 and 1 are reseverd for pOU beg/end

    if(pStmt->styp!=CG_EMPTY && pctx->pgctx->getOpts()->nGenAsmMapFile==2)
    {
        dmp<<_T("ST>");
        if(pStmt->styp==CG_NATIVE)
        {
            dmp<<_T("native");
        }
        else
        {
            CGT_DumpSTStmt(pStmt, dmp, NULL, false);
        }
        dmp<<'\n';
    }

    if(CGT_ConsiderBP(pctx->pInf->m_pLastBPEdp, &pStmt->edp))
    {
        if(pctx->BPoption != BP_OPT2 || pctx->pSym->pouTyp==CG_POU_FUN)
            bNCCOnly = true;
        else if(pStmt->edp.pszFile[0]==_T(':'))
            bNCCOnly = true; //TODO HACK no BP at internal generated stmts grep for _T(":!gen")
        else 
            bNCCOnly = false;
    
        DumpBPFun(dmp, pctx, &pStmt->edp, pctx->pInf->m_lastBPNum++, bNCCOnly);
    }
}



static int GetColOfEdp(const CG_Edipos* pEdp)
{
    if(pEdp->typ==CG_EDP_ST && pEdp->lc.l>0)
        return pEdp->lc.c;
    if(pEdp->typ==CG_EDP_4GR_STBOX && pEdp->gr.l>0)
        return pEdp->gr.c;
    return 0;
}

int StrIncToken(const TCHAR** ppszLine, const TCHAR* pszTok)
{
    int   iInc;
    const TCHAR* psz = *ppszLine;
    
    FC_StringSkipWhite(&psz);
    
    if(!FC_StringIncBegin(&psz, pszTok))
        return 0;
    if(__iscsym(*psz))
        return 0;

    FC_StringSkipWhite(&psz);
    iInc = psz - *ppszLine;
    *ppszLine = psz;

    return iInc;
}

int StrIncId(const TCHAR** ppszLine, TCHAR id[MAX_PATH])
{
    int   i = 0;
    const TCHAR* psz = *ppszLine;

    id[0] = 0;
    FC_StringSkipWhite(&psz);
    
    while(__iscsym(*psz) && i<MAX_PATH-1) 
        id[i++] = *psz++;

    if(i > 0)
    {
        id[i] = 0;
        FC_StringSkipWhite(&psz);
        i = psz - *ppszLine;
        *ppszLine = psz;
    }
    return i;
}

inline long PopItem(LStack* pLStack)
{
    assert(pLStack);
    
    int N = pLStack->getUsed();
    assert(N>0);
    
    long result = pLStack->get(N-1);
    pLStack->setUsed(N-1);
    
    return result;
}

//dump actual body ....
void VMDmpStmt(
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
        ConsiderBP(dmp, pS, pctx); 
        
        if(!pS->asns.pLhs)
        {
            assert(pctx->pIBdy->isSFC());
            VMPushValue(dmp, pctx, pS->asns.pRhs);
            //push right side: bool_to_int(tn)
            VMDmpOPC(dmp, _T("bool_to_int"))<<'\n';
            //pop left side: __doTransAct[i]
            VMDmpOPC(dmp, _T("PSHD_IN_16"))<<_T("__count\n");       //push index (which is an INT)
            VMDmpOPC(dmp, _T("POPX____16"))<<_T("__dotransact0 ");//array access as INT
			dmp<<_T("aidx(int)\n");
        }
        else
        {
            VMAssign(dmp, pctx, pS->asns.pLhs, pS->asns.pRhs);
            dmp<<'\n';
        }

        // free all used temp var expresions
        pctx->pInf->freeTmpVars();
    }
    break;
    case CG_CALLVOIDFUN:
    {
        ConsiderBP(dmp, pS, pctx); 
        VMPushValue(dmp, pctx, pS->funs.pExpr);
        dmp<<'\n';
    }
    break;
    case CG_CALLFB:
    {
        ConsiderBP(dmp, pS, pctx); 
        VMDmpFBCall(dmp, pS->fbcs.pInst, pS->fbcs.nArgs, pS->fbcs.pArgs, pctx, pS->fbcs.pMac);
        dmp<<'\n';
    }
    break;
    case CG_CASE:
    {
        const CG_STCaseList*  pCase;
        CG_Literal            lit;
         
        ConsiderBP(dmp, pS, pctx); 
        //TODO front end? check first if simple 'case', test if has no ranges:
       
        // temp var assign
        long TmpVarNum = pctx->pInf->getFreeTmpVar(pS->cases.pExpr->pType, false);
        VMPushValue(dmp, pctx, pS->cases.pExpr);
        VMDmpSizedOPC(dmp, _T("POPD_IN_"), pS->cases.pExpr->pType->type);
        VMDmpTmpVarName(dmp, pctx->pgctx, pS->cases.pExpr->pType, TmpVarNum, false)<<'\n';
        
        long lblReady   = pctx->pInf->GetNextLabelId();
        long lblStart   = pctx->pInf->GetNextLabelId();
        long lblNext    = lblStart;
        long lblOldNext = lblStart;
        for(pCase = pS->cases.pCases; pCase; pCase=pCase->pNextCase)
        {
            assert(pCase->nRanges > 0);

            if(pctx->pgctx->getOpts()->nGenAsmMapFile==2)
            {
                dmp<<_T("ST>case ");  
                for(int i=0; i<pCase->nRanges; i++)
                {
                    if(i>0)
                        dmp<<_T(", "); 
                    dmp<<pCase->ranges[i].lRangeStart;
                    if(pCase->ranges[i].ulRangeLength!=0)
                        dmp<<_T("..")<<(long)(pCase->ranges[i].lRangeStart+pCase->ranges[i].ulRangeLength);
                }
                dmp<<_T(":\n"); 
            }


            //TODO optmization? check whether the statement list is empty?
            lit.ut = CG_LIT_I64;
            lit.type = pS->cases.pExpr->pType->type;

            for (int r=0; r<pCase->nRanges; r++)
            {
                if(r == 0)
                    if(lblStart != lblNext)
                       dmp<<_T(":")CG_VM_LABEL_L<<lblOldNext<<'\n';
                
                if(pCase->ranges[r].ulRangeLength == 0)
                {
                    lit.i64 = pCase->ranges[r].lRangeStart;
                    VMPushLitValue(dmp, pctx, &lit);
                    VMDmpSizedOPC(dmp, _T("PSHD_IN_"), lit.type);
                    VMDmpTmpVarName(dmp, pctx->pgctx, pS->cases.pExpr->pType, TmpVarNum, false)<<'\n';
                    VMDmpTypedOpOPC(dmp, _T("eq"), lit.type)<<'\n';
                }
                else
                {
                    // lower bound
                    lit.i64 = pCase->ranges[r].lRangeStart;
                    VMDmpSizedOPC(dmp, _T("PSHD_IN_"), lit.type);
                    VMDmpTmpVarName(dmp, pctx->pgctx, pS->cases.pExpr->pType, TmpVarNum, false)<<'\n';
                    VMPushLitValue(dmp, pctx, &lit);dmp<<_T('\n'); 
                    VMDmpTypedOpOPC(dmp, _T("ge"), lit.type)<<'\n';

                    // upper bound
                    lit.i64 = (long) (pCase->ranges[r].lRangeStart + pCase->ranges[r].ulRangeLength);
                    VMDmpSizedOPC(dmp, _T("PSHD_IN_"), lit.type);
                    VMDmpTmpVarName(dmp, pctx->pgctx, pS->cases.pExpr->pType, TmpVarNum, false)<<'\n';
                    VMPushLitValue(dmp, pctx, &lit);dmp<<_T('\n'); 
                    VMDmpTypedOpOPC(dmp, _T("le"), lit.type)<<'\n';
                    VMDmpOPC(dmp, _T("and_bool"))<<'\n';
                }

                if(r > 0)
                    VMDmpOPC(dmp, _T("or_bool"))<<'\n';

                if(r == pCase->nRanges - 1)
                {
                    if (pCase->pNextCase == NULL && pS->cases.pElse == NULL)
                        VMDmpOPC(dmp, _T("jmpcn"))<<CG_VM_LABEL_L<<lblReady<<_T("  ;to end_case\n");
                    else
                        VMDmpOPC(dmp, _T("jmpcn"))<<CG_VM_LABEL_L<<lblNext<<_T("  ;to next case\n");
                }
            }
            
            dmp.incrIndent();
            {
                VMDmpStmt(dmp, pCase->pStmts, pctx);
                
                if(pCase->pNextCase || pS->cases.pElse)
                    VMDmpOPC(dmp, _T("jmp"))<<CG_VM_LABEL_L<<lblReady<<_T("  ;to end_case\n");
                //else: jump to last/next case is not necessary
            }
            dmp.decrIndent();

            lblOldNext = lblNext;
            lblNext = pctx->pInf->GetNextLabelId();
        }

        if(pS->cases.pElse)
        {
            if(lblStart != lblNext)
                dmp<<_T(":")CG_VM_LABEL_L<<lblOldNext<<'\n';

            dmp<<_T("; case else:\n");  
            dmp.incrIndent();
                VMDmpStmt(dmp, pS->cases.pElse, pctx);
            dmp.decrIndent();
        }
        
        dmp<<_T(":")CG_VM_LABEL_L<<lblReady<<'\n';
        pctx->pInf->freeTmpVar(TmpVarNum);
        dmp<<'\n';
    }
    break;
    case CG_IF:
    {
        ConsiderBP(dmp, pS, pctx); 
        VMPushValue(dmp, pctx, pS->ifs.pCond);

        long lblNext = pctx->pInf->GetNextLabelId();
        long lblEnd = pctx->pInf->GetNextLabelId();

        if(pS->ifs.pElse)
        {
            VMDmpOPC(dmp, _T("jmpcn"))<<CG_VM_LABEL_L<<lblNext<<_T("  ;to else\n");
        }
        else
        {
            VMDmpOPC(dmp, _T("jmpcn"))<<CG_VM_LABEL_L<<lblEnd<<_T("  ;to end_if\n");
        }
        
        dmp.incrIndent();
        VMDmpStmt(dmp, pS->ifs.pThen, pctx);
        dmp.decrIndent();
        
        if(pS->ifs.pElse)
        {
            VMDmpOPC(dmp, _T("jmp"))<<CG_VM_LABEL_L<<lblEnd<<_T("  ;to end_if\n");
            dmp<<_T(":")CG_VM_LABEL_L<<lblNext<<_T("  ;else\n"); 
            
            dmp.incrIndent();
                VMDmpStmt(dmp, pS->ifs.pElse, pctx);
            dmp.decrIndent();
        }
        dmp<<_T(":")CG_VM_LABEL_L<<lblEnd<<_T("  ;end_if\n");
        dmp<<'\n';
    }
    break;
    case CG_REPEAT:
    {
        ConsiderBP(dmp, pS, pctx);
        long LblLoop = pctx->pInf->GetNextLabelId();  // loop label
        long lblReady = pctx->pInf->GetNextLabelId(); // ready label for EXIT
        pctx->pInf->m_LblStack.add(lblReady);

        dmp<<_T(":")CG_VM_LABEL_LOOP<<LblLoop<<'\n';  // "loop label" for UNTIL
        dmp.incrIndent();
            VMDmpStmt(dmp, pS->reps.pStmts, pctx);
        dmp.decrIndent();

        VMPushValue(dmp, pctx, pS->reps.pUntil);
        VMDmpOPC(dmp, _T("jmpcn"))<<CG_VM_LABEL_LOOP<<LblLoop<<'\n';
        
        dmp<<_T(":")CG_VM_LABEL_READY<<lblReady<<'\n';

        PopItem(&pctx->pInf->m_LblStack);     // clear last stack item
        dmp<<'\n';
    }
    break;
    case CG_WHILE:
    {
        ConsiderBP(dmp, pS, pctx); 
        long LblLoop = pctx->pInf->GetNextLabelId();    // loop label
        long lblReady = pctx->pInf->GetNextLabelId();   // ready label for EXIT
        pctx->pInf->m_LblStack.add(lblReady);

        dmp<<_T(":")CG_VM_LABEL_LOOP<<LblLoop<<'\n';    // "loop label" for UNTIL
        VMPushValue(dmp, pctx, pS->whls.pWhile);
        VMDmpOPC(dmp, _T("jmpcn"))<<CG_VM_LABEL_READY<<lblReady<<'\n';

        dmp.incrIndent();
            VMDmpStmt(dmp, pS->whls.pStmts, pctx);
        dmp.decrIndent();

        VMDmpOPC(dmp, _T("jmp"))<<CG_VM_LABEL_LOOP<<LblLoop<<'\n';
        dmp<<_T(":")<<CG_VM_LABEL_READY<<lblReady<<'\n';

        PopItem(&pctx->pInf->m_LblStack);
        dmp<<'\n';
    }
    break;
    case CG_FOR:
    {
        ConsiderBP(dmp, pS, pctx); 
        VMDumpForLoop(dmp, &pS->edp, &pS->fors, pctx);
        dmp<<'\n';
    }
    break;
    case CG_GOTO:
    {
        ConsiderBP(dmp, pS, pctx); 
        assert(!pctx->bTransi);

        VMDmpOPC(dmp, _T("jmp"));
        if((pctx->bAction || pctx->bExceptionAction) && pS->gots.pLblStmt->lbls.pszLbl[0]!=_T('@')) //##d-1454, '@' ignore internal gen labels!
        {
            //the HACK is that we are not able to cretae the proper stmts for the 
            //action switch, we cannot change the label names !
            //need a better way to create stmt/expr on the fly.

            //Problem: label that have the same name in different actions:
            //HACK is simply to dump out the stmt pointer value as part of the label name:
            LCT("stmt goto in sfc action");
            dmp<<pS->gots.pLblStmt->lbls.pszLbl<<_T("__")<<(unsigned long)pS->gots.pLblStmt<<'\n';
        }
        else
        {
            LCT("stmt goto");
            dmp<<pS->gots.pLblStmt->lbls.pszLbl<<'\n';
        }
        dmp<<'\n';
    }
    break;
    case CG_LABEL:
    {
        dmp.decrIndent();

        if((pctx->bAction || pctx->bExceptionAction) && pS->lbls.pszLbl[0]!=_T('@')) //##d-1454, '@' ignore internal gen labels!
        {
            dmp<<':'<<pS->lbls.pszLbl<<_T("__")<<(unsigned long)pS<<'\n';//HACK see above !
        }
        else
        {
            dmp<<':'<<pS->lbls.pszLbl<<'\n';
        }
        dmp.incrIndent();
    }
    break;
    case CG_EXIT:
    {
        ConsiderBP(dmp, pS, pctx); 
        long lTop = pctx->pInf->m_LblStack.get(pctx->pInf->m_LblStack.getUsed()-1);
        VMDmpOPC(dmp, _T("jmp"))<<CG_VM_LABEL_READY<<lTop<<'\n';
        dmp<<'\n';
    }
    break;
    case CG_RETURN:
    {
        ConsiderBP(dmp, pS, pctx);
        //jump to end or end of action switch        
        if(pctx->bAction)//##d-1454
            VMDmpOPC(dmp, _T("jmp"))<<VM_LABEL_ACTION_RETURN<<'\n';
        else
            VMDmpOPC(dmp, _T("jmp"))<<CG_VM_LABEL_POUEND<<'\n';

        dmp<<'\n';
    }
    break;
    case CG_EMPTY:
    {
        // do nothing
        ConsiderBP(dmp, pS, pctx); 
    }
    break;
    case CG_NATIVE:
    {
        ConsiderBP(dmp, pS, pctx); 

        int                 iCol;
        int                 iLine;
        const TCHAR*        pszCode;
        const TCHAR*        pszCmnt;
        TCHAR               szOPC[MAX_PATH];
        FC_CStrList         code(128);
        FC_CStrList         cmnt(128);
        CG_Edipos           edp;
        ICG_Expr*           pExIF;
        VM_STACKTYP         stackTyp;
        const CG_ExprTree*  pEx;
		int                 nTotLines;

        dmp<<_T(";native\n");    

        nTotLines = VMSplitMacroText(pS->nats.pszNative, &code, &cmnt, false);
        for(iLine=0; iLine<nTotLines; iLine++)
        {
            pszCode = code.get(iLine);
            pszCmnt = cmnt.get(iLine);

            if(!*pszCode && !*pszCmnt && (iLine==0 || iLine==nTotLines-1))
                continue;
            if(!*pszCode)
            {
                if(*pszCmnt)
                    dmp<<pszCmnt<<'\n';
                continue;
            }

            iCol = StrIncId(&pszCode, szOPC);
            int bitSize;
            stackTyp = getInlineCodeCmd(szOPC, &bitSize);
            if(stackTyp == VM_STK_UNDEF)
            {
                VMDmpOPC(dmp, szOPC)<<pszCode;
                if(*pszCmnt)
                    dmp<<pszCmnt;
                dmp<<'\n';
                continue;
            }

            //dump comment of line before it is expanded:
			if(*pszCmnt)
				dmp<<pszCmnt<<'\n';
            edp = pS->edp;
            if(iLine==0)
                iCol += GetColOfEdp(&edp) + _tcslen(_T("native"))-1;
            CG_IncLineColEdipos(&edp, iLine-1, iCol);
            
            pExIF = pctx->pIF->parseExprFromString(
                &edp, TRUE, pszCode, pctx->pSym->pszId, FALSE);
            if(!pExIF)
            {
                assert(pctx->pdm->hasErr());
                continue;
            }
            pEx = pExIF->getExpr(CG_ANY);
            if(!pEx)
            {
                assert(pctx->pdm->hasErr());
                FC_RELEASE_PTR(pExIF);            
                continue;
            }
            
            if(stackTyp==VM_PUSH_VALUE || stackTyp==VM_POP_VALUE)
            {
                assert(bitSize>0);
                CheckExprBitSize(pctx->pdm, pEx, bitSize); //HACK for d-211 (need a better interface to the frontend to parse an expression)
            }


            VMEvalExpr(dmp, pctx, stackTyp, pEx);
            FC_RELEASE_PTR(pExIF);            
        }

        dmp<<_T(";end_native\n");
    }
    break;
    case CG_SYNCHRONIZED:
    {
        ConsiderBP(dmp, pS, pctx); 
        pctx->pdm->errNS1(&pS->edp, _T("SYNCHRONIZED block"));
        dmp<<'\n';
    }
    break;
    case CG_LDFBDNETWORK:
    {
        // dump statements
        dmp<<_T(";network\n");
        VMDmpStmt(dmp, pS->netw.pStmts, pctx);
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

index := left;

if st > 0 AND right >= left then
   N := left - right + 32767;
   st2 := -st;
else 
   if st < 0 and right <= left then
      N := right - left + 32767;
      st2 := st;
   else
      goto @ready;
   end_if;
end_if;

tmp    := 32767 + st2;
st_tmp := st; 

@loop:
 stmts; 
   index := index + st_tmp;
   if tmp < N then
      goto @ready;
   end_if;
   N := N - st2;
   goto @loop;

@ready:


if limits and stepby are literals and overflow is imposible
then there is a SIMPLE SOLUTION and for loop will be

@loop:
stmts;
index = index + st;
if index <= right
   goto @loop;
@ready:
*/  
static void VMDumpForLoop(
    CGT_CDmp&            dmp, 
    const CG_Edipos*     pEdp,
    const CG_StmtFor*    pFor,
    const BODY_CTX_INFO* pctx
)
{
    CG_ExprMem      mem(pEdp, pctx->pSym);
    const CG_DType* pForVarType = pFor->pAssign->ops.pLhs->pType;
    int             indexSize   = VMSimpleTypeBitSize(pForVarType->type);
    
    CG_ExprTree* lit1;
    lit1           = mem.registerCGExpr(CG_LITERAL, pEdp, pForVarType);
    lit1->lit.pLit = mem.registerLiteral(pForVarType->type, _T("1"), 1, 0);

    const CG_ExprTree*const& pIndex = pFor->pAssign->ops.pLhs;
    const CG_ExprTree*const& pLeft  = pFor->pAssign->ops.pRhs;
    const CG_ExprTree*const& pRight = pFor->pToExpr;
    const CG_ExprTree*const& pBy    = pFor->pByExpr?pFor->pByExpr:lit1;

    __int64 maxValue;
    if(pForVarType->tbs & (CG_TBS_INT | CG_TBS_DINT))
    {
        if(pForVarType->type == CG_INT)
            maxValue = SHRT_MAX;
        else
            maxValue = INT_MAX;
    }
    else if(pForVarType->tbs & (CG_TBS_UINT | CG_TBS_UDINT))
    {
        if(pForVarType->type == CG_TBS_UINT)
            maxValue = USHRT_MAX;
        else
            maxValue = UINT_MAX;
        bool bOk = false;
        if (pLeft->extyp == CG_LITERAL && pRight->extyp == CG_LITERAL && pBy->extyp == CG_LITERAL)
            if (pBy->lit.pLit->i64 > 0 && pLeft->lit.pLit->i64 <= pRight->lit.pLit->i64)
                if (maxValue - pBy->lit.pLit->i64 >= pRight->lit.pLit->i64) // no overflow detection needed
                    bOk = true;        

        if(!bOk)
            pctx->pdm->errNS1(pEdp, _T("for loop with unsigned types and non constant limits"));
    }
    else
    {
        pctx->pdm->errNS2(pEdp, _T("for loop with data type "), pForVarType->pszName);
    }

    BODY_CTX_INFO ctxNoBP = *pctx;
    ctxNoBP.BPoption      = BP_OPT_INTERNAL;

    CG_ExprTree*     pExpr1;
    CG_ExprTree*     pExpr2;
    FC_CString       jot;
    const CG_STStmt* pForStmts = NULL;
    CG_STStmt*       pStmt;
    CG_STStmt*       pPrevStmt;

    CG_STStmt*       pLoopLabel;
    pLoopLabel       = mem.registerStmt();
    pLoopLabel->styp = CG_LABEL;
    pLoopLabel->edp  = *pEdp;
    pLoopLabel->lbls.bUsed   = true;
    pLoopLabel->lbls.nLblNum = pctx->pInf->GetNextLabelId();
    jot.load(CG_VM_LABEL_LOOP) << pLoopLabel->lbls.nLblNum;
    pLoopLabel->lbls.pszLbl = mem.registerString(jot);

    CG_STStmt* pReadyLabel;
    pReadyLabel       = mem.registerStmt();
    pReadyLabel->styp = CG_LABEL;
    pReadyLabel->edp  = *pEdp;
    pReadyLabel->lbls.bUsed = true;
    pReadyLabel->lbls.nLblNum = pctx->pInf->GetNextLabelId();
    jot.load(CG_VM_LABEL_READY) << pReadyLabel->lbls.nLblNum;
    pReadyLabel->lbls.pszLbl = mem.registerString(jot);

    pctx->pInf->m_LblStack.add(pReadyLabel->lbls.nLblNum);

    // index :=  left;
    pStmt            = mem.registerStmt();
    pStmt->styp      = CG_ASSIGN;
    pStmt->edp       = *pEdp;
    pStmt->asns.pLhs = pIndex;
    pStmt->asns.pRhs = pLeft;

    VMDmpStmt(dmp, pStmt, &ctxNoBP);   

    // the SIMPLE case
    if (pLeft->extyp == CG_LITERAL && pRight->extyp == CG_LITERAL && pBy->extyp == CG_LITERAL)
    {
        if (pBy->lit.pLit->i64 > 0 && pLeft->lit.pLit->i64 <= pRight->lit.pLit->i64)
        {
            if (maxValue - pBy->lit.pLit->i64 >= pRight->lit.pLit->i64) // no overflow detection needed
            {
                // stmts:
                VMDmpStmt(dmp, pLoopLabel, &ctxNoBP);   

                // stmts;
                VMDmpStmt(dmp, pFor->pStmts, pctx);   
                
                 // index := index + st;
                pStmt            = mem.registerStmt();
                pForStmts        = pStmt;
                pStmt->styp      = CG_ASSIGN;
                pStmt->edp       = *pEdp;
                pStmt->asns.pLhs = pIndex;
                pExpr1           = mem.registerCGExpr(CG_ADD, pEdp, pForVarType);
                pStmt->asns.pRhs = pExpr1;
                pExpr1->ops.pLhs = pIndex;
                pExpr1->ops.pRhs = pBy;

                // if index <= right
                pPrevStmt          = pStmt;
                pStmt              = mem.registerStmt();
                pPrevStmt->pNext   = pStmt;
                pStmt->styp        = CG_IF;
                pStmt->edp         = *pEdp;
                pExpr1             = mem.registerCGExpr(CG_LE, pEdp, mem.pBoolType);
                pStmt->ifs.pCond   = pExpr1;
                pExpr1->ops.pLhs   = pIndex;
                pExpr1->ops.pRhs   = pRight;

                // goto @loop;
                pPrevStmt            = pStmt;
                pStmt                = mem.registerStmt();
                pPrevStmt->ifs.pThen = pStmt;
                pStmt->styp          = CG_GOTO;
                pStmt->edp           = *pEdp;
                pStmt->gots.bUpward  = true;
                pStmt->gots.pLblStmt = pLoopLabel;
                
                VMDmpStmt(dmp, pForStmts, &ctxNoBP);   

                goto clean_up;
            }
            // else go to overflow detection algorithm
        }
        else if (pBy->lit.pLit->i64 < 0 && pLeft->lit.pLit->i64 >= pRight->lit.pLit->i64)
        {
            if (-maxValue-1 - pBy->lit.pLit->i64 <= pRight->lit.pLit->i64) // no overflow detection needed
            {
                // stmts:
                VMDmpStmt(dmp, pLoopLabel, &ctxNoBP);   

                // stmts;
                VMDmpStmt(dmp, pFor->pStmts, pctx);   
                
                 // index := index + st;
                pStmt            = mem.registerStmt();
                pForStmts        = pStmt;
                pStmt->styp      = CG_ASSIGN;
                pStmt->edp       = *pEdp;
                pStmt->asns.pLhs = pIndex;
                pExpr1           = mem.registerCGExpr(CG_ADD, pEdp, pForVarType);
                pStmt->asns.pRhs = pExpr1;
                pExpr1->ops.pLhs = pIndex;
                pExpr1->ops.pRhs = pBy;

                // if index >= right
                pPrevStmt          = pStmt;
                pStmt              = mem.registerStmt();
                pPrevStmt->pNext   = pStmt;
                pStmt->styp        = CG_IF;
                pStmt->edp         = *pEdp;
                pExpr1             = mem.registerCGExpr(CG_GE, pEdp, mem.pBoolType);
                pStmt->ifs.pCond   = pExpr1;
                pExpr1->ops.pLhs   = pIndex;
                pExpr1->ops.pRhs   = pRight;

                // goto @loop;
                pPrevStmt            = pStmt;
                pStmt                = mem.registerStmt();
                pPrevStmt->ifs.pThen = pStmt;
                pStmt->styp          = CG_GOTO;
                pStmt->edp           = *pEdp;
                pStmt->gots.bUpward  = true;
                pStmt->gots.pLblStmt = pLoopLabel;
                
                VMDmpStmt(dmp, pForStmts, &ctxNoBP);   

                goto clean_up;
            }
            // else go to overflow detection algorithm
        }
        else
        {
            // goto @ready;
            pStmt                = mem.registerStmt();
            pStmt->styp          = CG_GOTO;
            pStmt->edp           = *pEdp;
            pStmt->gots.bUpward  = true;
            pStmt->gots.pLblStmt = pReadyLabel;

            VMDmpStmt(dmp, pStmt, &ctxNoBP);   

            // stmts;
            VMDmpStmt(dmp, pFor->pStmts, pctx);   

            pctx->pdm->msg1(CG_W_UNREACHABLE_STMT, &pFor->pStmts->edp, _T("FOR"));
            goto clean_up;
        }
    }
    // for loop with overflow detection algorithm
    {
    CG_STStmt*         pTmpStmt;
    CG_STStmt*         pIf1      = NULL;
    CG_STStmt*         pIf2      = NULL;
    CG_ExprTree*       pCond;
    const CG_ExprTree* pCond1;
    const CG_ExprTree* pCond2;
    const CG_ExprTree* pNVar     = NULL;
    const CG_ExprTree* pst2Var   = NULL;
    const CG_ExprTree* pTmpVar   = NULL;
    const CG_ExprTree* pstTmpVar = NULL;
    int nNVar, nst2Var, nTmpVar, nstTmpVar; 

    const CG_ExprTree* lit0   = mem.newLiteral(pEdp, pForVarType, 0);
    const CG_ExprTree* litMax = mem.newLiteral(pEdp, pForVarType, maxValue);

    nNVar = pctx->pInf->getFreeTmpVar(pForVarType, false);                 
    jot.load(_T("@")) << indexSize << _T("_") << nNVar;
    pNVar = mem.newVarRef(pEdp, pForVarType, jot, CG_ACCESS_R|CG_ACCESS_W);

    if (pBy->extyp != CG_LITERAL)
    {
        nst2Var = pctx->pInf->getFreeTmpVar(pForVarType, false); 
        jot.load(_T("@")) << indexSize << _T("_") << nst2Var;
        pst2Var = mem.newVarRef(pEdp, pForVarType, jot, CG_ACCESS_R|CG_ACCESS_W);

        nTmpVar = pctx->pInf->getFreeTmpVar(pForVarType, false);                 
        jot.load(_T("@")) << indexSize << _T("_") << nTmpVar;
        pTmpVar = mem.newVarRef(pEdp, pForVarType, jot, CG_ACCESS_R|CG_ACCESS_W);

        nstTmpVar = pctx->pInf->getFreeTmpVar(pForVarType, false);                 
        jot.load(_T("@")) << indexSize << _T("_") << nstTmpVar;
        pstTmpVar = mem.newVarRef(pEdp, pForVarType, jot, CG_ACCESS_R|CG_ACCESS_W);
    }
    else  // don't need nTmpVar, nstTmpVar if BY is literal and nst2Var is literal also then
    {
        pst2Var = mem.newLiteral(pEdp, pForVarType, pBy->lit.pLit->i64>0? -pBy->lit.pLit->i64: pBy->lit.pLit->i64);
        if (pBy->lit.pLit->i64 == 0)
            pctx->pdm->msg1(CG_W_UNREACHABLE_STMT, &pFor->pStmts->edp, _T("FOR"));
    }
   
    // st > 0 AND right >= left 
    pExpr1           = mem.registerCGExpr(CG_GT, pEdp, mem.pBoolType);
    pExpr1->ops.pLhs = pBy;
    pExpr1->ops.pRhs = lit0;
    pExpr2           = mem.registerCGExpr(CG_GE, pEdp, mem.pBoolType);
    pExpr2->ops.pLhs = pFor->pToExpr;
    pExpr2->ops.pRhs = pFor->pAssign->ops.pRhs;
    pCond            = mem.registerCGExpr(CG_AND, pEdp, mem.pBoolType);
    pCond->ops.pLhs  = pExpr1;
    pCond->ops.pRhs  = pExpr2;
    pCond1           = mem.optimizeExpr(pCond);

    if (pCond1->extyp != CG_LITERAL)
    {
        // if st > 0 AND right >= left (cond1) then    
        pIf1            = mem.registerStmt();
        pIf1->styp      = CG_IF;
        pIf1->edp       = *pEdp;
        pIf1->ifs.pCond = pCond1;
        pPrevStmt       = pIf1;
        pForStmts       = pPrevStmt;
    }
    else if (!pCond1->lit.pLit->i64)
        goto skip_if1;

    // N := left - right + 32767;   
    pStmt = mem.registerStmt();
    if (pIf1) 
        pIf1->ifs.pThen = pStmt;
    else 
    {
        pPrevStmt = pStmt;
        pForStmts = pPrevStmt;
    }

    pStmt->styp      = CG_ASSIGN;
    pStmt->edp       = *pEdp;
    pStmt->asns.pLhs = pNVar;
    pExpr1           = mem.registerCGExpr(CG_SUB, pEdp, pForVarType);
    pExpr1->ops.pLhs = pFor->pAssign->ops.pRhs;
    pExpr1->ops.pRhs = pFor->pToExpr;
    pExpr2           = mem.registerCGExpr(CG_ADD, pEdp, pForVarType);
    pExpr2->ops.pLhs = pExpr1;
    pExpr2->ops.pRhs = litMax;
    pStmt->asns.pRhs = mem.optimizeExpr(pExpr2);

    // st2 := -st;
    if (pBy->extyp != CG_LITERAL)
    {
        pTmpStmt         = pStmt;
        pStmt            = mem.registerStmt();
        pTmpStmt->pNext  = pStmt;
        pStmt->styp      = CG_ASSIGN;
        pStmt->edp       = *pEdp;
        pStmt->asns.pLhs = pst2Var;
        pExpr1           = mem.registerCGExpr(CG_UMINUS, pEdp, pForVarType);
        pExpr1->ops.pLhs = pBy;
        pExpr1->ops.pRhs = NULL;
        pStmt->asns.pRhs = pExpr1;
        if (!pIf1)
            pPrevStmt = pStmt;
    }
    if (pCond1->extyp == CG_LITERAL && pCond1->lit.pLit->i64)
        goto begin_loop;
    
skip_if1:
    // st < 0 AND right <= left
    pExpr1           = mem.registerCGExpr(CG_LT, pEdp, mem.pBoolType);
    pExpr1->ops.pLhs = pBy;
    pExpr1->ops.pRhs = lit0;
    pExpr2           = mem.registerCGExpr(CG_LE, pEdp, mem.pBoolType);
    pExpr2->ops.pLhs = pFor->pToExpr;
    pExpr2->ops.pRhs = pFor->pAssign->ops.pRhs;
    pCond            = mem.registerCGExpr(CG_AND, pEdp, mem.pBoolType);
    pCond->ops.pLhs  = pExpr1;
    pCond->ops.pRhs  = pExpr2;
    pCond2           = mem.optimizeExpr(pCond);
    
    if (pCond2->extyp != CG_LITERAL)
    {
        // if st < 0 and right <= left then
        pIf2            = mem.registerStmt();
        pIf2->styp      = CG_IF;
        pIf2->edp       = *pEdp;
        pIf2->ifs.pCond = pCond2;
        if (pIf1)
            pIf1->ifs.pElse = pIf2;
        else
        {
            pPrevStmt = pIf2;
            pForStmts = pPrevStmt;
        }
    }
    else if (!pCond2->lit.pLit->i64)
        goto skip_if2;

    // N := right - left + 32767;
    pStmt            = mem.registerStmt();
    if (pIf2)
        pIf2->ifs.pThen = pStmt;
    else if (pIf1)
        pIf1->ifs.pElse = pStmt;
    else
    {
        pPrevStmt = pStmt;   
        pForStmts = pPrevStmt;
    }

    pStmt->styp      = CG_ASSIGN;
    pStmt->edp       = *pEdp;
    pStmt->asns.pLhs = pNVar;
    pExpr1           = mem.registerCGExpr(CG_SUB, pEdp, pForVarType);
    pExpr1->ops.pLhs = pFor->pToExpr;
    pExpr1->ops.pRhs = pFor->pAssign->ops.pRhs;
    pExpr2           = mem.registerCGExpr(CG_ADD, pEdp, pForVarType);
    pExpr2->ops.pLhs = pExpr1;
    pExpr2->ops.pRhs = litMax;
    pStmt->asns.pRhs = mem.optimizeExpr(pExpr2);

    // st2 := st;
    if (pBy->extyp != CG_LITERAL)
    {
        pTmpStmt = pStmt;
        pStmt            = mem.registerStmt();
        pTmpStmt->pNext = pStmt;
        pStmt->styp      = CG_ASSIGN;
        pStmt->edp       = *pEdp;
        pStmt->asns.pLhs = pst2Var;
        pStmt->asns.pRhs = pBy;
        if (!pIf2 && !pIf1)
            pPrevStmt = pStmt;
    }
    if (pCond2->extyp == CG_LITERAL && pCond2->lit.pLit->i64)
        goto begin_loop;

skip_if2:
    // goto @ready;
    pStmt                = mem.registerStmt();
    if (pIf2)
        pIf2->ifs.pElse = pStmt;
    else if (pIf1)
        pIf1->ifs.pElse = pStmt;
    else
    {
        pPrevStmt = pStmt;   
        pForStmts = pPrevStmt;
    }
    pStmt->styp          = CG_GOTO;
    pStmt->edp           = *pEdp;
    pStmt->gots.bUpward  = false;
    pStmt->gots.pLblStmt = pReadyLabel;

begin_loop:
    // tmp := 32767 + st2;
    if (pBy->extyp != CG_LITERAL)
    {
        pStmt            = mem.registerStmt();
        pPrevStmt->pNext = pStmt;
        pStmt->styp      = CG_ASSIGN;
        pStmt->edp       = *pEdp;
        pStmt->asns.pLhs = pTmpVar;
        pExpr1           = mem.registerCGExpr(CG_ADD, pEdp, pForVarType);
        pStmt->asns.pRhs = pExpr1;
        pExpr1->ops.pLhs = litMax;
        pExpr1->ops.pRhs = pst2Var;

        // st_tmp := st;
        pPrevStmt        = pStmt;
        pStmt            = mem.registerStmt();
        pPrevStmt->pNext = pStmt;
        pStmt->styp      = CG_ASSIGN;
        pStmt->edp       = *pEdp;
        pStmt->asns.pLhs = pstTmpVar;
        pStmt->asns.pRhs = pBy;

        pPrevStmt = pStmt;
    }

    // @loop:
    pPrevStmt->pNext = pLoopLabel;
    VMDmpStmt(dmp, pForStmts, &ctxNoBP);   

    // stmts; 
    VMDmpStmt(dmp, pFor->pStmts, pctx);   

    // index := index + st_tmp;
    pStmt            = mem.registerStmt();
    pForStmts        = pStmt;
    pStmt->styp      = CG_ASSIGN;
    pStmt->edp       = *pEdp;
    pStmt->asns.pLhs = pFor->pAssign->ops.pLhs;
    pExpr1           = mem.registerCGExpr(CG_ADD, pEdp, pForVarType);
    pStmt->asns.pRhs = pExpr1;
    pExpr1->ops.pLhs = pFor->pAssign->ops.pLhs;    
    if (pBy->extyp != CG_LITERAL)
        pExpr1->ops.pRhs = pstTmpVar;
    else 
        pExpr1->ops.pRhs = pBy;

    // if tmp < N then
    pPrevStmt          = pStmt;
    pStmt              = mem.registerStmt();
    pPrevStmt->pNext   = pStmt;
    pStmt->styp        = CG_IF;
    pStmt->edp         = *pEdp;
    pExpr1             = mem.registerCGExpr(CG_LT, pEdp, mem.pBoolType);
    pStmt->ifs.pCond   = pExpr1;  
    if (pBy->extyp != CG_LITERAL)
        pExpr1->ops.pLhs = pTmpVar;
    else 
        pExpr1->ops.pLhs = mem.newLiteral(pEdp, pForVarType, maxValue+pst2Var->lit.pLit->i64);
    pExpr1->ops.pRhs   = pNVar;

    // goto @ready;
    pTmpStmt                = mem.registerStmt();
    pStmt->ifs.pThen        = pTmpStmt;
    pTmpStmt->styp          = CG_GOTO;
    pTmpStmt->edp           = *pEdp;
    pTmpStmt->gots.bUpward  = false;
    pTmpStmt->gots.pLblStmt = pReadyLabel;

    // N := N - st2;
    pPrevStmt        = pStmt;
    pStmt            = mem.registerStmt();
    pPrevStmt->pNext = pStmt;
    pStmt->styp      = CG_ASSIGN;
    pStmt->edp       = *pEdp;
    pStmt->asns.pLhs = pNVar;
    pExpr1           = mem.registerCGExpr(CG_SUB, pEdp, pForVarType);
    pStmt->asns.pRhs = pExpr1;
    pExpr1->ops.pLhs = pNVar;
    pExpr1->ops.pRhs = pst2Var;

    // goto @loop;
    pPrevStmt            = pStmt;
    pStmt                = mem.registerStmt();
    pPrevStmt->pNext     = pStmt;
    pStmt->styp          = CG_GOTO;
    pStmt->edp           = *pEdp;
    pStmt->gots.bUpward  = true;
    pStmt->gots.pLblStmt = pLoopLabel;

    // pForStmts = mem.optimizeStmts(pForStmts);
    // here should be nothing to optimize
    VMDmpStmt(dmp, pForStmts,  &ctxNoBP);   
   
    pctx->pInf->freeTmpVar(nNVar);
    if (pBy->extyp != CG_LITERAL)
    {
        pctx->pInf->freeTmpVar(nst2Var);
        pctx->pInf->freeTmpVar(nTmpVar);
        pctx->pInf->freeTmpVar(nstTmpVar);
    }
    }

clean_up:
    // @ready:
    VMDmpStmt(dmp, pReadyLabel,  &ctxNoBP);   

    PopItem(&pctx->pInf->m_LblStack);     // clear last stack item
}






static void VMDmpFBCall(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pInst,//strict IEC: always a CG_VARREF, future 
                              //version may support array of FBs
    long                 nArgs,//>=0 number of in/out assignments
    const CG_ArgExpr*    pArgs,//NULL or array of arg expr.
    const BODY_CTX_INFO* pctx,
    const CG_MacroBody*  pMac  //!=NULL if macro FB
)
{
    int               i;
    const CG_ArgExpr* pAE;
    
    
    // TODO: remove this restriction
    if(pInst->extyp!=CG_VARREF || VMIsRefFunFBParam(&pInst->var))
    {
        pctx->pdm->errNS1(&pInst->var.pEntry->edp, pctx->pdm->res1(CG_RES_STR_CALL_INOUT_FB));
        return;
    }


    for(i = 0; i<nArgs; i++)
    {
        pAE = &pArgs[i];

        if (pAE->argFlags & CG_EXPR_ARG_OUTASN)
            continue;

        if(pAE->pFormal->declScope==CG_VAR_INPUT)
        {
            // create normal assignment:  fbinst.input := actual expr
            CG_ExprTree       stracc;
            stracc.extyp      = CG_STRUCTREF;
            stracc.edp        = pAE->pActual->edp;
            stracc.pType      = pAE->pActual->pType;//use actual type avoids r_edge !
            stracc.str.pBase  = pInst;
            stracc.str.pSelc  = pAE->pFormal;
            VMAssign(dmp, pctx, &stracc, pAE->pActual);
        }
        else if(pAE->pFormal->declScope == CG_VAR_IN_OUT)
        { 
            assert(pInst->pType->pSym->nInOut > 0);
            assert(pInst->extyp==CG_VARREF); 

            //TODO :this works only if !VMIsRefFunFBParam(&pInst->var), 
            //as checked above:

            VMPushReference(dmp, pctx, true, pAE->pActual);
            
            //POP the real pointer to the inout member, which is a pointer too:
            //HACK: TODO make real ptr data types, we cannot make a tmp struct ref 
            //and use the normal dump routines, simply because there is no idea about 
            //a 'pointer type':
            if(pInst->var.pEntry->declScope==CG_VAR_EXTERNAL)
            {
                VM_SEG_TYP seg = VMGetGlobVarSegment(pctx, pInst->var.pEntry);
                if(seg==VM_SEG_GA || seg==VM_SEG_GR)
                {
                    if (pAE->pFormal->pType->type==CG_BOOL && !pAE->pFormal->pType->bIsArray)
                    {
                        //auto global or retain param: index and selector offset:
                        VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("POPO_ID_64") : _T("POPO_ID_32"));
                    }
                    else
                    {
                        //auto global or retain param: index and selector offset:
                        VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("POPO_ID_32") : _T("POPO_ID_16"));
                    }

                    (dmp<<_T("objid(")).lo(pInst->var.pEntry->pszId)<<_T(") ");
                    VMDmpOffsetOf(dmp, pAE->pFormal)<<'\n';
                }
                else
                {
                    //no FBI at I,Q,M glob seg!
                    assert(!"bad glob segm for FBI");
                    dmp<<"internal error";
                }
            }
            else
            {
                if (pAE->pFormal->pType->type==CG_BOOL && !pAE->pFormal->pType->bIsArray)
                {
                    //POPO_IN_    inst_struct offsetof(type,member)
                    VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("POPO_IN_64") : _T("POPO_IN_32"));
                }
                else
                {
                    //POPO_IN_    inst_struct offsetof(type,member)
                    VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("POPO_IN_32") : _T("POPO_IN_16"));
                }
                dmp.lo(pInst->var.pEntry->pszId)<<' ';
                VMDmpOffsetOf(dmp, pAE->pFormal)<<'\n';
            }
        }
        else
        {
            assert(!"bad decl scope for FB input assign");
        }
    }



    //call opcode:
    if(pMac)
    {
        TCHAR           funopid[MAX_PATH];

        switch(VMClassifyMacroText(pctx->pdm, &pMac->edp, pMac->pszMac, funopid))
        {
        case VM_MAC_EXTLIB:
            LCT("FB call (mlib)");
            if(pInst->var.pEntry->declScope==CG_VAR_EXTERNAL)
                VMDmpOPC(dmp, _T("CALB_CI"));
            else
                VMDmpOPC(dmp, _T("CALB_CN"));
        break;
        case VM_MAC_STDLIB:
            if(pInst->var.pEntry->declScope==CG_VAR_EXTERNAL)
                VMDmpOPC(dmp, _T("CALB_SI"));
            else
                VMDmpOPC(dmp, _T("CALB_SN"));
        break;
        default:
            pctx->pdm->errNS1(&pMac->edp, pctx->pdm->res1(CG_RES_STR_WRONG_INLINE_FB));
        }

        (dmp<<_T("fbid(")).lo(pInst->var.pEntry->pType->pSym->pszId)<<_T(") ");
        if(pInst->var.pEntry->declScope==CG_VAR_EXTERNAL)
            (dmp<<_T("objid(")).lo(pInst->var.pEntry->pszId)<<_T(")\n");
        else
            dmp.lo(pInst->var.pEntry->pszId)<<'\n';
    }
    else
    {
        if(pInst->var.pEntry->declScope==CG_VAR_EXTERNAL)
        {
            VMDmpOPC(dmp, _T("CALB_PI"));
            (dmp<<_T("clsid(")).lo(pInst->var.pEntry->pType->pSym->pszId)<<_T(") ");
            (dmp<<_T("objid(")).lo(pInst->var.pEntry->pszId)<<_T(")\n");
        }
        else
        {
            VMDmpOPC(dmp, _T("CALB_PN"));
            (dmp<<_T("clsid(")).lo(pInst->var.pEntry->pType->pSym->pszId)<<_T(") ");
            dmp.lo(pInst->var.pEntry->pszId)<<'\n';
        }
    }



    // OUTPUTs
    for(i=0; i<nArgs; i++)
    {
        pAE = &pArgs[i];

        if((pAE->argFlags & CG_EXPR_ARG_OUTASN) == 0)
            continue;

        // create struct node
        if(pAE->pFormal->declScope == CG_VAR_OUTPUT)
        {
            // create normal assignment:   actualexpr := [not] fbinst.output
            CG_ExprTree       stracc;
            stracc.extyp      = CG_STRUCTREF;
            stracc.edp        = pAE->pActual->edp;
            stracc.pType      = pAE->pActual->pType;//use actual type avoids r_edge !
            stracc.str.pBase  = pInst;
            stracc.str.pSelc  = pAE->pFormal;

            if(pAE->argFlags & CG_EXPR_ARG_NEGOUT)
            {
                LCT("FB neg out");
                CG_ExprTree   notop;
                notop.extyp   = CG_NOT;
                notop.edp     = stracc.edp;
                notop.pType   = stracc.pType;
                notop.ops.pLhs= &stracc;
                notop.ops.pRhs= NULL;
                VMAssign(dmp, pctx, pAE->pActual, &notop);
            }
            else
            {
                VMAssign(dmp, pctx, pAE->pActual, &stracc);
            }
        }
        else if(pAE->pFormal->declScope == CG_VAR_IN_OUT)
        {
            LCT("FB(inout=>l-value");
            assert((pAE->argFlags & CG_EXPR_ARG_NEGOUT)==0);
            // create inout out assignment:   actualOutexpr := actualIn
            //sreach the expr. that was assigned to the inout
            for(int i2=0; i2<nArgs; i2++)
            {
                if(pArgs[i2].pFormal == pAE->pFormal && (pArgs[i2].argFlags & CG_EXPR_ARG_OUTASN)==0)
                {
                    //TODO: optimze if pArgs[i2].pActual was complex with array idx [expr]???
                    VMAssign(dmp, pctx, pAE->pActual, pArgs[i2].pActual);
                    break;
                }
            }
            assert(i2<nArgs);    
        }
        else
        {
            assert(!"bad decl scope in FB out asn");
        }
    }
}


int FC_StringOnlyWhite(const TCHAR* psz)
{
    while(_istspace(*psz))
        psz++;
    return *psz==0;
}

int VMSplitMacroText(
	const TCHAR* pszMac, 
	FC_CStrList* pCode, 
	FC_CStrList* pCmnt, 
	bool         bTrim
)
{
    const TCHAR* psz;
    FC_CString   line(MAX_PATH);

    pCode->clear();
	if(pCmnt)
		pCmnt->clear();
    psz = pszMac;

    while(*psz)
    {
        line.clear().appendUpTo(psz, _T(";\n"), &psz);

		if(FC_StringOnlyWhite(line))
		{
			pCode->add("");//blank line: return "" even if !bTrim
		}
        else if(bTrim)
        {
            line.trim();
            pCode->add(line);
        }
        else
        {
			pCode->add(line);
        }

        if(*psz==';')
        {
            line.clear().appendUpTo(psz, _T("\n"), &psz);
            if(pCmnt)
			{
				line.trimRight();
                pCmnt->add(line);
			}
        }
        else
        {
            if(pCmnt) //sync the comment line array with "" 
                pCmnt->add("");
        }

		if(*psz=='\n')
			psz++;
    }

	return pCode->getUsed();
}    




static bool VMConsiderTmpVarOnResue(const CG_ExprTree* pExpr)
{
    switch(pExpr->extyp)
    {
    case CG_LITERAL:   return false;
    case CG_VARREF:    return false;
    case CG_STRUCTREF:
        switch(pExpr->str.pBase->extyp)
        {
        case CG_VARREF: 
        case CG_TMPVAR: 
            return false;
        default:
            return true;
        }
    break;

    case CG_TMPVAR:     return false;
    case CG_TRIGVAR:    return false;
    case CG_STEPACTION: return false;
    case CG_USEREXPR:   
        if(pExpr->usr.ldata == UE_SFC_INSTANCE_ARRAY)
        {
            return false;
        }
		else
        {
            assert(!"bad user expr");
            return true;
        }

    default:
        return true; //all other a tmp var is better..
    }
}

#define VM_MAX_FUN_PARAMS 256 //TODO should be in CG_IECFront.h ??
class VMInlineArgCache
{
    const CG_ExprTree*  m_pTmp[VM_MAX_FUN_PARAMS+1];
    VM_STACKTYP         m_puTyp[VM_MAX_FUN_PARAMS+1];
    int                 m_nActArg;
    const CG_ArgExpr*   m_pAE;
    const BODY_CTX_INFO*m_pctx;
public:
    VMInlineArgCache(const BODY_CTX_INFO* pctx, int nActArg, const CG_ArgExpr* pAE)
        : m_pctx(pctx),
          m_nActArg(nActArg), 
          m_pAE(m_pAE)
    {
        if(nActArg>=FC_ARRAY_LEN(m_pTmp))
            CGT_IERR2(pctx->pdm, &pAE[0].pActual->edp, _T("too many function arguments"));

        memset(m_pTmp, NULL, sizeof(const CG_ExprTree*)*nActArg);
        memset(m_puTyp, NULL, sizeof(VM_STACKTYP)*nActArg);
    }
    
    void vmDumpArgNo(CGT_CDmp& dmp, int nArg)
    {
        const CG_ExprTree* pAct;
        VM_STACKTYP        puTyp;

        nArg--;
        assert(nArg>=0 && nArg<m_nActArg);
        
        pAct = m_pAE[nArg].pActual;
        puTyp= VMGetFunArgPushType(&m_pAE[nArg]);

        if(m_pTmp[nArg])
        {
            VMEvalExpr(dmp, m_pctx, puTyp, pAct);
            return;
        }
        else if(VMConsiderTmpVarOnResue(pAct))
        {
            m_pTmp[nArg] = m_pctx->pInf->getTmpVarExpr(pAct->pType, &pAct->edp);
            m_pctx->pInf->addTmpVarExpr(m_pTmp[nArg]);
            VMEvalExpr(dmp, m_pctx, puTyp, m_pTmp[nArg]);
        }
        else
        {
            VMEvalExpr(dmp, m_pctx, puTyp, pAct);
        }
    }
};



//    overload STRING(STRING,STRING) inline{
//        %1
//        %2
//        gt_string
//        jmpcn L1%u
//        %1
//        jmp Lend%u
//    :L1%u
//        %2
//    :Lend%u
//    }
static void CG_VM_DumpInlineFun(//ganze function ein HACK
    CGT_CDmp&           dmp, 
    const CG_Edipos*    pEdp,
    const BODY_CTX_INFO*pctx,
    int                 nActArg,
    const CG_ArgExpr    pAE[],
    const CG_Symtab*    pFun,
    const CG_DType*     pRetType,
    const TCHAR*        pszMac,
    const CG_Edipos*    pEdpMac,
    const CG_ExprTree*  pCmplRes
)
{
    int                 nLine;
    FC_CStrList         lines(64);
    //left TODO VMInlineArgCache    tmpCache(pctx, nActArg, pAE);
    bool                bUsedCmplxR = false;
    TCHAR               szErrHint[MAX_PATH];
    long                nUniqueNum = -1;

	szErrHint[0] = 0;

    if(pCmplRes)
    {
        LCT("funmac inl cmpl ret immediate");
        assert(pRetType->type==CG_STRING && !pRetType->bIsArray);
    }


    VMSplitMacroText(pszMac, &lines, NULL, true);

    for(nLine=0; nLine<lines.getUsed(); nLine++)
    {
        const TCHAR* psz          = lines.get(nLine);
        VM_STACKTYP  stackTyp     = VM_STK_UNDEF;
        bool         bArgInsert   = false;
		bool   		 bCheckStkAct = true;

        if(!*psz)
            continue; //ignore empty lines:

        if(*psz=='%') //old syntax....
        {
            bArgInsert = true; 
        }
        else if(*psz!=':')
        {
            //get first ident in line, shoule be a opcode:
            TCHAR szOPC[MAX_PATH];
            StrIncId(&psz, szOPC);
             
            int bitSize;
            stackTyp = getInlineCodeCmd(szOPC, &bitSize);//TODO what about checking bitSize here?
            if(stackTyp == VM_STK_UNDEF)
            {
                //extra undocumented:
                if(!_tcscmp(szOPC, _T("ptr32")))
                {
                    stackTyp = VM_PUSH_WRT_REF;
				    bCheckStkAct = false;
                }
                else if(!_tcscmp(szOPC, _T("ptr32c")))
                {
                    stackTyp = VM_PUSH_CST_REF;
				    bCheckStkAct = false;
                }
            }

            if(stackTyp==VM_STK_UNDEF) 
                VMDmpOPC(dmp, szOPC);//any other dump as is 
            else  
                bArgInsert = true;//argument %<1> must follow:
        }
        //else should be ':label'
        

        if(bArgInsert)
        {
            unsigned long  ulArgNo;

            //%<argnum> must follow:
            FC_StringSkipWhite(&psz);
            if(*psz=='%')
                ulArgNo = _tcstoul(psz+1, (TCHAR**)&psz, 10);

            if(*psz)//test EOL
            {
                _tcscpy(szErrHint, _T("expected %<num> for formal argument insertion"));
                goto Error;
            }
            if(ulArgNo>(unsigned long)nActArg)
            {
                wsprintf(szErrHint, _T("'%%%u': argument number too big"), ulArgNo);
                goto Error;
            }

            if(ulArgNo==0)
            {
                LCT("funmac %0");
                if(pRetType->type!=CG_STRING || pRetType->bIsArray)
                {
                    _tcscpy(szErrHint, _T("%0 only allowed for functions returning string"));
                    goto Error;
                }
                
                if(!pCmplRes)
                {
                    LCT("funmac %0: make tmp");
                    pCmplRes = pctx->pInf->getTmpVarExpr(pRetType, pEdp);
                    pctx->pInf->addTmpVarExpr(pCmplRes);
                }

                if(stackTyp == VM_STK_UNDEF)
                {
                    stackTyp = VM_PUSH_WRT_REF;
                }
                else if(stackTyp!=VM_PUSH_WRT_REF && stackTyp!=VM_PUSH_CST_REF)
                {
                    _tcscpy(szErrHint, _T("invalid stack action: complex type needs 'lea' or leac"));
                    goto Error;
                }
                
                bUsedCmplxR = true;
                VMEvalExpr(dmp, pctx, stackTyp, pCmplRes);
            }
            else
            {
                if(stackTyp == VM_STK_UNDEF)
                {
                    stackTyp = VMGetFunArgPushType(&pAE[ulArgNo-1]);
                }
                else if(bCheckStkAct && stackTyp != VMGetFunArgPushType(&pAE[ulArgNo-1]))
                {
                    _tcscpy(szErrHint, _T("invalid stack action for this formal argument"));
                    goto Error;
                }
                VMEvalExpr(dmp, pctx, stackTyp, pAE[ulArgNo-1].pActual);//<-newline gets appened... 
            }
        }
        else
        {
            while(*psz)
            {
                if(psz[0]=='%' && psz[1]=='u')
                {
                    //build a unique number for inline replacement, once for 
                    //every call to this function:
                    if(nUniqueNum<0)
                        nUniqueNum = ++pctx->pInf->m_UniqueInlineNum;

                    dmp<<_T("_inline_")<<nUniqueNum;
                    psz += 2;    
                    continue;
                }
                dmp<<*psz++;
            }
            dmp<<'\n';
        }
    }


    if(pCmplRes && !bUsedCmplxR)
    {
        //example s := max('A', 'B');
        //%0 was not used inside the inline and that means a 'const string*'
        //is now on stack as 'the normal' stack result for a function returning string.
        //pCmplRes is not NULL to indicate that the result 'const string*' is not 
        //needed, but the deep copy:
        VMPushReference(dmp, pctx, true,  pCmplRes);
        VMDmpOPC(dmp, _T("MOVE_STRING"))<<'\n';
        //now stack is 0.
    }
    return;


Error:
    if(szErrHint[0])
    {
		CG_Edipos edp = *pEdpMac;
		CG_IncLineColEdipos(&edp, nLine, 0);
        pctx->pdm->msg1(CG_E_TEXT, &edp, szErrHint);
    }
    pctx->pdm->msg0(CG_E_MACRO_EXPANSION, pEdp);
}



static long VMPushFunArgList(
    const BODY_CTX_INFO* pctx,
    CGT_CDmp&            dmp, 
    int                  nArgs,
    const CG_ArgExpr*    pArgs,
    long                 nStkAlign
)
{
    int                 i;
    long                lStack = 0; //retrun value is the number bytes pushed.
    VM_STACKTYP         puTyp;
    const CG_ExprTree*  pAct;
    int                 nExtCnt = 0;

    //CG_VAR_INPUT:
    for(i=nArgs-1; i>=0; i--)
    {
        pAct = pArgs[i].pActual;
        puTyp = VMGetFunArgPushType(&pArgs[i]);

        //update lStack also for extended params (but dont dump extended now!)
        lStack += VMTypeStackBytes(pAct->pType->type, pAct->pType->bIsArray, puTyp, nStkAlign);

        if(pArgs[i].pFormal->declScope == CG_VAR_IN_EXT)
            nExtCnt++;//push later, first all non extended.
        else
            VMEvalExpr(dmp, pctx, puTyp, pAct);
    }

    //CG_VAR_IN_EXT if any:
    if(nExtCnt>0)
    {
        TCHAR      szInt[64];
        CG_Literal lit; 

        for(i=nArgs-1; i>=0; i--)
        {
            if(pArgs[i].pFormal->declScope != CG_VAR_IN_EXT)
                continue;
            VMEvalExpr(dmp, pctx, VMGetFunArgPushType(&pArgs[i]), pArgs[i].pActual);
        }

        // ++**++ JD extInParam -->
        // count formal in ext parameter
        assert(nArgs>0);
        int nFormInExt = 0;
        const CG_Symtab* pSymTab = pArgs[0].pFormal->pParent;
        assert(pSymTab);
        const CG_SymEntry* pEntry = pSymTab->pVarInExt;
        assert(pEntry);
        while(pEntry)
        {
            if (pEntry->declScope == CG_VAR_IN_EXT)
                nFormInExt++;
            pEntry = pEntry->pNext;
        }
        assert(nFormInExt>0);
        nExtCnt /= nFormInExt;  // new in ext counter: number of in ext groups
        // ++**++ JD extInParam <--

        //last is the number of ext, always an INT:
        lit.type   = CG_INT; 
        lit.ut     = CG_LIT_I64;//not nice: TODO make more util helpers in front end IF...
        lit.psz    = itoa(nExtCnt, szInt, 10);
        lit.i64    = nExtCnt;
        VMPushLitValue(dmp, pctx, &lit);
        lStack += VMTypeStackBytes(lit.type, false, VM_PUSH_VALUE, nStkAlign); 
    }

    return lStack;
}






static void VMDumpFunMacro(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_Edipos*     pEdp,
    const CG_DType*      pRetType,
    const CG_MacroExpr*  pFunMac,
    const CG_ExprTree*   pCmplRes
)
{
    int                 i;
    VM_MACRO_TYP        mtyp;
    TCHAR               funopid[MAX_PATH];
    const CG_Symtab*    pSym;
    int                 nArgs;
    const CG_ArgExpr*   pArgs;
    const CG_MacroBody* pMac;

    pMac = pFunMac->pMac;
    pSym = pMac->pSym;
    nArgs= pFunMac->nArgs;
    pArgs= pFunMac->pArgs;
    
    mtyp = VMClassifyMacroText(pctx->pdm, &pMac->edp, pMac->pszMac, funopid);

    switch(mtyp)
    {
    case VM_MAC_ERROR:  
        assert(pctx->pdm->hasErr()); 
    break;

    case VM_MAC_NOOP:
        VMASSERT(!pCmplRes, pEdp);
        //TODO: NOOP make only sense if the first argument has the same type 
        //than the function return, if void do nothing !
        if(nArgs != 1)
            pctx->pdm->msg1(CG_E_INL_NOOP_TO_MANY_ARG, &pMac->edp, CG_VM_NOOP);
        else
            VMEvalExpr(dmp, pctx, VMGetFunArgPushType(&pArgs[0]), pArgs[0].pActual);
    break;
    
    case VM_MAC_VMOP:
        VMASSERT(!pCmplRes, pEdp);
        for(i=0; i<nArgs; i++)
            VMEvalExpr(dmp, pctx, VMGetFunArgPushType(&pArgs[i]), pArgs[i].pActual);
        
        if(!funopid[0])
            _tcslwr(_tcscpy(funopid, pFunMac->pMac->pSym->pszId));
        
        VMDmpOPC(dmp, funopid)<<'\n';
    break;
    
    case VM_MAC_STDLIB:
    case VM_MAC_EXTLIB:
    {    
        long  lParamStkBytes = 0;
        long  nStkAlign = pctx->pgctx->m_kad.getVmSettings()->stackAlignBytes;

        //first always push the return value dummy or pointer to complex result:
        if(pCmplRes)
        {
            LCT("funmac ret string immediate");
            assert(pRetType->type==CG_STRING);
            VMPushReference(dmp, pctx, true, pCmplRes);

            //function return shall clear this too, it is not needed any more:
            lParamStkBytes += max(nStkAlign, VM_ADDRESS_SIZE/8);
        }
        else if(pRetType->type==CG_STRING || pRetType->pSym || pRetType->bIsArray)
        {
            LCT("funmac ret string need tmp");
            //function return complex and no pExprL: need a real tmp var:
            assert(!pRetType->pSym && !pRetType->bIsArray);
            const CG_ExprTree* pTmp = pctx->pInf->getTmpVarExpr(pRetType, pEdp);
            VMPushReference(dmp, pctx, true, pTmp);
            pctx->pInf->addTmpVarExpr(pTmp);
            //do not lStack +=: this address of tmp shall be left on stack.
        }
        else if(pRetType->type != CG_VOID)
        {
            LCT("funmac ret simple");
            //function return simple stack type: push a lit of size:
            dmp<<_T(";space for ret val\n");
            VMPushLitValue(dmp, pctx, pctx->pIF->getDefaultInit(pRetType->type));
            //do not lStack +=: this value shall be left on stack.
        }
        else
        {
            LCT("funmac ret void");
        }

        if(nArgs>0)
            lParamStkBytes += VMPushFunArgList(pctx, dmp, nArgs, pArgs, nStkAlign);
       
        assert((lParamStkBytes % nStkAlign)==0);
        long lFunId;
        long minVer;
        long maxVer;
        lFunId = pctx->pgctx->getIdOfLibFunFb(pSym->pLibCtx, &pMac->edp, funopid, &minVer,&maxVer);
        assert((lFunId>=0 || pctx->pdm->hasErr()));

        if(minVer!=0 && pctx->ulTrgVer!=0 && pctx->ulTrgVer<(unsigned long)minVer)
        {
            pctx->pdm->errNS1(
                pEdp, pctx->pdm->res1(VM_RES_STR_NS_FEATURE_VER_MIN).fmtStr3(
                        NULL, 
                        pctx->pdm->jot1()<<"'"<<funopid<<"'", 
                        pctx->pdm->jot2()<<pctx->ulTrgVer, 
                        pctx->pdm->jot3()<<minVer
                      ) 
            );
        }
        
        if(pctx->ulTrgVer!=0 && pctx->ulTrgVer>(unsigned long)maxVer)
        {
            pctx->pdm->errNS1(
                pEdp, pctx->pdm->res1(VM_RES_STR_NS_FEATURE_VER_MAX).fmtStr3(
                        NULL, 
                        pctx->pdm->jot1()<<"'"<<funopid<<"'", 
                        pctx->pdm->jot2()<<pctx->ulTrgVer, 
                        pctx->pdm->jot3()<<maxVer
                      ) 
            );
        }


        VMDmpOPC(dmp, mtyp==VM_MAC_STDLIB? _T("CALF_SL") : _T("CALF_CL"));
        dmp<<lFunId<<' '<<lParamStkBytes<<'\n';
    }
    break;
    
    case VM_MAC_ASM:
        CG_VM_DumpInlineFun(
            dmp, 
            pEdp, 
            pctx,
            nArgs,
            pArgs,
            pSym,
            pRetType,
            pMac->pszMac,
            &pMac->edp,
            pCmplRes
        );
    break;

    default:
        assert(!"bad enum");
        dmp<<"bad enum";
    }
}

void VMDumpFunCall(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pExprR,
    const CG_ExprTree*   pCmplxRes
)
{
    const CG_FunExpr*  pFun;
    const CG_Symtab*   pSym;
    const CG_DType*    pRetType;
    long               lParamStkBytes;
    long               nStkAlign;

    pRetType = pExprR->pType; 

    if(pExprR->extyp==CG_FUNMACRO)
    {
        assert(pExprR->mac.pMac->pSym->flags & CG_STAB_HDR_NOERR);       
        VMDumpFunMacro(dmp, pctx, &pExprR->edp, pRetType, &pExprR->mac, pCmplxRes);
        return;
    }
    //else user fun:
    assert(pExprR->extyp==CG_FUNCALL);
    pFun      = &pExprR->fun;
    pSym      = pFun->pSym;
    nStkAlign = pctx->pgctx->m_kad.getVmSettings()->stackAlignBytes;

    if(!(pSym->flags & CG_STAB_HAS_BDY) || !(pSym->flags & CG_STAB_HDR_NOERR))
        CGT_IERR2(pctx->pdm, &pExprR->edp, _T("bad symtab flags in user fun call"));

    //immediate from lhs := fun() or stack dummy for Function return value
    if(pRetType->type==CG_STRING || pRetType->bIsArray || pRetType->pSym)
    {
        //assert: currently only string as cmply ret:
        assert(pRetType->type==CG_STRING);
        //complex type provide no dummy but the real return parameter
        if(pCmplxRes)
        {
            LCT("fun ret string immediate");
            VMPushReference(dmp, pctx, true, pCmplxRes);
        }
        else
        {
            LCT("fun ret string tmp");
            const CG_ExprTree* pTmp = pctx->pInf->getTmpVarExpr(pRetType, &pExprR->edp);
            pctx->pInf->addTmpVarExpr(pTmp);
            VMPushReference(dmp, pctx, true, pTmp);
        }
    }
    else if(pRetType->type != CG_VOID)
    {
        LCT("fun ret simple");
        dmp<<_T(";space for ret val\n");
        VMPushLitValue(dmp, pctx, pctx->pIF->getDefaultInit(pRetType->type));
    }
    else
    {
        LCT("fun ret void");
    }

    //lParamStkBytes are the number of bytes that are removed from 
    //stack by the function call, that is all parametres but not the fun ret, 
    //even if it was a "lval_string := fun(...)".
    //In versions <2.06 the asm assumed always that the &string ret should be removed
    //by the fun return LEAVE, this is now changed for clearity:
    //The commitment is: a user function always returns on stack a return value.
    //The situation "lval_string := fun(...)" is handeled by the caler with an 
    //extra LEAVE (see below). Note that for mlib calls we can tell the CALF_CL opcode
    //how many byte to leave, that is why no extra LEAVE is needed for 
    //"lval_string := concat(...)"
    if(pFun->nArgs>0)
        lParamStkBytes = VMPushFunArgList(pctx, dmp, pFun->nArgs, pFun->pArgs, nStkAlign);
    else
        lParamStkBytes = 0;

    assert((lParamStkBytes % nStkAlign)==0);

    // calf statement
    if(pctx->ulTrgVer>=20600)
    {
        VMDmpOPC(dmp, _T("CALF_PR"));
        (dmp<<_T("clsid(")).lo(pSym->pszId)<<_T(") ");
        (dmp<<_T("funlocbytes(")).lo(pSym->pszId)<<_T(") ")<<lParamStkBytes<<'\n';
    }
    else
    {
        VMDmpOPC(dmp, _T("CALF"));
        (dmp<<_T("clsid(")).lo(pSym->pszId)<<_T(")");
        (dmp<<_T("funlocbytes(")).lo(pSym->pszId)<<_T(")\n");
    }

    //Leave extar if "lval_string := fun(...)" was optimized:
    if(pCmplxRes)
        VMDmpOPC(dmp, _T("CLST"))<<max(nStkAlign, VM_ADDRESS_SIZE/8)<<'\n';
}

/* ---------------------------------------------------------------------------- */
