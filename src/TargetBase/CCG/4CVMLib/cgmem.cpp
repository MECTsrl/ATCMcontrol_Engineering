/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/cgmem.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: cgmem.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/cgmem.cpp $
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

#include <stddef.h>
#include "cgmem.h"
#include "cgt_misc.h"

CG_ExprMem::CG_ExprMem(
	const CG_Edipos* pEdp,
	const CG_Symtab* pSymCurrPOU
) :
	m_hashDType  (/*bIgnoreCase=*/false,/*bAllocKey=*/false, /*initalSize=*/0)
{
	m_pSymCurrPOU = pSymCurrPOU;
    pIntType  = registerDType(CG_INT, -1);
    pBoolType = registerDType(CG_BOOL, -1);

    lit0           = registerCGExpr(CG_LITERAL, pEdp, pIntType);
    lit0->lit.pLit = registerLiteral(CG_INT, _T("0"), 0, 0);

    lit1           = registerCGExpr(CG_LITERAL, pEdp, pIntType);
    lit1->lit.pLit = registerLiteral(CG_INT, _T("1"), 1, 0);

    litTrue           = registerCGExpr(CG_LITERAL, pEdp, pBoolType);
    litTrue->lit.pLit = registerLiteral(CG_BOOL, _T("true"), 1, 0);

    litFalse           = registerCGExpr(CG_LITERAL, pEdp, pBoolType);
    litFalse->lit.pLit = registerLiteral(CG_BOOL, _T("false"), 0, 0);
}

CG_ExprMem::~CG_ExprMem()
{
    m_exprs.deletePtrs();
    m_symEntries.deletePtrs();
    m_argExprs.deletePtrs();
    m_hashDType.deletePtrs();
    m_hashLiterals.deletePtrs();
    m_stmts.deletePtrs();
    m_cases.deletePtrs();
}

const TCHAR* CG_ExprMem::registerString(const TCHAR* psz)
{
    return m_strings.registerString(psz);
}

const CG_Literal* CG_ExprMem::registerLiteral(
    CG_DTYPES     type,
    const TCHAR*  pszNormal,
    __int64       anyInt,
    double        anyReal
)
{
    bool          bOk;
    CG_Literal*   pLit;
    const TCHAR*  pszLitReg;  //registerd string pointer
    const TCHAR*  pszLitIFReg;//text of lit as used in CG_Literal:psz
                              //no <type># if ANY_STRING,ANY_FLOAT
                              //but t# , dt#, tod# dt# for ANY_DATE 
                              //also a registered string pointer
    TCHAR         buffer[256];

    if(type==CG_BOOL_R_EDGE || type==CG_BOOL_F_EDGE)
        type = CG_BOOL;

    assert(CG_IsElementary(type));
    assert(type!=CG_STRING && type!=CG_WSTRING);

    if(!pszNormal) //can happen if caller want default initial value
    {
        anyInt = 0;
        anyReal= 0.0;
        switch(type)
        {
        case CG_BOOL:     pszNormal = _T("false");  break;
        case CG_REAL:
        case CG_LREAL:    pszNormal = _T("0.0");    break;
        case CG_BYTE:     
        case CG_WORD:     
        case CG_DWORD:    
        case CG_LWORD:    pszNormal = _T("16#0");   break;
        case CG_SINT:
        case CG_INT:
        case CG_DINT:
        case CG_LINT:
        case CG_USINT: 
        case CG_UINT:
        case CG_UDINT:
        case CG_ULINT:    pszNormal = _T("0");      break;
        case CG_DT:       pszNormal = _T("dt#0001-01-01-00:00:00.000"); break;
        case CG_TOD:      pszNormal = _T("tod#00:00:00.000");   break;
        case CG_DATE:     pszNormal = _T("d#0001-01-01");       break;
        case CG_TIME:     pszNormal = _T("t#0ms");  break;
        default:
            assert(!_T("bad literal data type"));
        }
    }

    //build an unique string to hash the lit must include
    //the data type if not already (e.g if t#)
    if(_istalpha(pszNormal[0]) && _tcschr(pszNormal, _T('#')))
    {
        //assert: only these types should have a type# in it's normalised
        //representation:
        assert(type==CG_TIME||type==CG_TOD||type==CG_DT||type==CG_DATE);
        pszLitReg   = registerString(pszNormal);
        pszLitIFReg = pszLitReg;
    }
    else
    {
        const TCHAR* pszType = CGT_DTYPES_ToString(type);
        int          iLen    = lstrlen(pszType);
        memcpy(buffer, pszType, sizeof(TCHAR)*iLen);
        buffer[iLen] = _T('#');
        lstrcpy(buffer+iLen+1, pszNormal);

        pszLitReg   = registerString(buffer);
        pszLitIFReg = pszLitReg + iLen+1; 
    }
   
    pLit = m_hashLiterals.get(pszLitReg);
    if(!pLit)
    {
        FC_NEW_ZSTRUCT(pLit, CG_Literal);

        //fill the data:
        pLit->type = type;
        pLit->psz  = pszLitIFReg;
        if(type==CG_REAL || type==CG_LREAL)
        {
            pLit->ut = CG_LIT_FLT;
            pLit->dbl= anyReal;
        }
        else
        {
            pLit->ut  = CG_LIT_I64;
            pLit->i64 = anyInt;
        }
    
        bOk = m_hashLiterals.insert(pszLitReg, pLit);
        assert(bOk);
    }
    return pLit;
}

CG_ExprTree* CG_ExprMem::registerCGExpr(
    CG_EXPRTYP       extyp, 
    const CG_Edipos* pEdp, 
    const CG_DType*  pType
)
{
    CG_ExprTree* p;

    FC_NEW_ZSTRUCT(p, CG_ExprTree);
    m_exprs.add(p);
    p->extyp = extyp;
    p->edp   = *pEdp;
    p->pType = pType;
    return p;
}

const CG_ArgExpr* CG_ExprMem::registerArgExpr(
    const CG_ExprTree* pExpr,
    const CG_SymEntry* pFormal
)
{
    CG_ArgExpr* pArgList;
    FC_NEW_ZSTRUCT(pArgList, CG_ArgExpr);
    m_argExprs.add(pArgList);
    pArgList->pActual = pExpr;
    pArgList->pFormal = pFormal;
    return pArgList;
}

const CG_DType* CG_ExprMem::registerDType(
    CG_DTYPES dType, 
    long      lStrLen //-1 if no string or auto size string
                      // >=0 otherwise
)
{
    bool               bOk;
    const CG_DType*    pDType;
    CG_DType*          pDTypeNew;
    const TCHAR*       pszStr;
    TCHAR              buffer[256];

    assert(dType!=CG_DTYPES_UNDEF);
    pszStr = CGT_DTYPES_ToString(dType);

    if(lStrLen>=0)
    {
        assert(dType==CG_STRING || dType==CG_WSTRING || 
               dType==CG_QVT_STRING || dType==CG_QVT_WSTRING);
        wsprintf(buffer, _T("%s[%d]"), pszStr, lStrLen);
        pszStr = buffer;
    }
    else
    {
        //strlen should be always -1 if not specified:
        assert(lStrLen==-1);
        lStrLen = -1;
    }

    pDType = m_hashDType.get(pszStr);

    if(!pDType)
    {    
        FC_NEW_ZSTRUCT(pDTypeNew, CG_DType);
        pDTypeNew->type    = dType;
        pDTypeNew->tbs     = CGT_DTypesToTBS(dType);
        pDTypeNew->lStrLen = lStrLen;
        pDTypeNew->pszName = registerString(pszStr);

        bOk = m_hashDType.insert(pDTypeNew->pszName, pDTypeNew);
        assert(bOk);
        pDType = pDTypeNew;
    }
    return pDType;
}

const CG_SymEntry* CG_ExprMem::registerSymEntry(
    const CG_Edipos* pEdp, 
    const TCHAR*     pszId, 
    const CG_DType*  pDType
)
{
    CG_SymEntry* pEntry;
    FC_NEW_ZSTRUCT(pEntry, CG_SymEntry);

    pEntry->pszId     = registerString(pszId);
    pEntry->edp       = *pEdp;
    pEntry->pType     = pDType;
    pEntry->declScope = CG_VAR;
    pEntry->declMod   = CG_DMOD_NON_RETAIN;
    pEntry->lInstNum  = -1;
    pEntry->lParamNum = -1;
	pEntry->pParent   = m_pSymCurrPOU;
    m_symEntries.add(pEntry);
    return pEntry;
}

CG_STStmt* CG_ExprMem::registerStmt()
{
    CG_STStmt* pStmt;   
    FC_NEW_ZSTRUCT(pStmt, CG_STStmt);
    m_stmts.add(pStmt);
    return pStmt;
}

CG_STCaseList* CG_ExprMem::registerCaseList(
    long lCase, 
    const CG_STStmt* pStmt
)
{
    CG_STCaseList* pCase = (CG_STCaseList*)new BYTE[offsetof(CG_STCaseList, ranges) +sizeof(CG_Subrange)];
    memset(pCase, 0, offsetof(CG_STCaseList, ranges) +sizeof(CG_Subrange)); 
    m_cases.add(pCase);
    pCase->nRanges = 1;
    pCase->pStmts = pStmt;
    pCase->ranges[0].lRangeStart   = lCase;
    pCase->ranges[0].ulRangeLength = 0;
    return pCase;
}

const CG_ExprTree* CG_ExprMem::newLiteral(const CG_Edipos* pEdp, const CG_DType* pType, __int64 tmpVal)
{
    FC_CString   jot;
    CG_ExprTree* pLit;

    pLit = registerCGExpr(CG_LITERAL, pEdp , pType);    
    jot << tmpVal;
    pLit->lit.pLit = registerLiteral(pType->type, jot, tmpVal, 0);
    return pLit;
}

const CG_ExprTree* CG_ExprMem::newVarRef(const CG_Edipos* pEdp, const CG_DType* pType, const TCHAR* pszVarName, WORD accFlags)
{
    CG_ExprTree* pVarRef  = registerCGExpr(CG_VARREF, pEdp, pType);
    pVarRef->var.accFlags = accFlags;
    pVarRef->var.pEntry   = registerSymEntry(pEdp, registerString(pszVarName), pType);
    return pVarRef;
}

// return an optimized expression
// it has a limited functionality needed for optimizing the for loop
// it treats only GT, LT, GE, LE, AND, OR, UMINUS, SUB, ADD expressions
const CG_ExprTree* CG_ExprMem::optimizeExpr(const CG_ExprTree* pExpr)
{
    const CG_ExprTree* pOp1 = NULL;
    const CG_ExprTree* pOp2 = NULL;

    if(!(pExpr->pType->tbs & (CG_TBS_INT | CG_TBS_DINT | CG_TBS_BOOL)))
        return pExpr;

    switch(pExpr->extyp)
    {
    case CG_GT:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL && pOp2->extyp == CG_LITERAL)
            return (pOp1->lit.pLit->i64 > pOp2->lit.pLit->i64) ? litTrue : litFalse;
        break;
    case CG_LT:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL && pOp2->extyp == CG_LITERAL)
            return (pOp1->lit.pLit->i64 < pOp2->lit.pLit->i64) ? litTrue : litFalse;
        break;
    case CG_GE:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL && pOp2->extyp == CG_LITERAL)
            return (pOp1->lit.pLit->i64 >= pOp2->lit.pLit->i64) ? litTrue : litFalse;
        break;
    case CG_LE:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL && pOp2->extyp == CG_LITERAL)
            return (pOp1->lit.pLit->i64 <= pOp2->lit.pLit->i64) ? litTrue : litFalse;
        break;
    case CG_AND:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL)
            return pOp1->lit.pLit->i64? pOp2: litFalse;
        if (pOp2->extyp == CG_LITERAL)
            return pOp2->lit.pLit->i64? pOp1: litFalse;
        break;
    case CG_OR:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL)
            return pOp1->lit.pLit->i64? litTrue: pOp2;
        if (pOp2->extyp == CG_LITERAL)
            return pOp2->lit.pLit->i64? litTrue: pOp1;
        break;
    case CG_ADD:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL && pOp2->extyp == CG_LITERAL)
            return newLiteral(&pExpr->edp, pExpr->pType, pOp1->lit.pLit->i64 + pOp2->lit.pLit->i64);
        break;
    case CG_SUB:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        pOp2 = optimizeExpr(pExpr->ops.pRhs);
        if (pOp1->extyp == CG_LITERAL && pOp2->extyp == CG_LITERAL)
            return newLiteral(&pExpr->edp, pExpr->pType, pOp1->lit.pLit->i64 - pOp2->lit.pLit->i64);
        break;
    case CG_UMINUS:
        pOp1 = optimizeExpr(pExpr->ops.pLhs);
        if (pOp1->extyp == CG_LITERAL)
            return newLiteral(&pExpr->edp, pExpr->pType, -pOp1->lit.pLit->i64);
        break;
    }
    return pExpr;
}

/* ---------------------------------------------------------------------------- */
