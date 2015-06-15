/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/opexasm.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opexasm.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/opexasm.cpp $
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

#include "vmasm.h"
#include "resource.h"



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
*/
void VMOperator(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    VM_STACKTYP          pushTyp, 
    CG_DTYPES            lTypeSet,
    CG_EXPRTYP           opEnum,
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs
)
{
    CG_DTYPES    typ1;
    CG_DTYPES    typ2;
    const TCHAR* pszOp;

    if(!CG_IsMonomorph(lTypeSet))
    {
        VMDmpOPC(dmp, NULL);
        return;
    }
    else if(pushTyp!=VM_PUSH_VALUE && lTypeSet!=CG_STRING) 
    {
        //currently only string type result operators do push a reference
        //this may change if there are functions return array/struct e.g. mux
        //and be carefull then with '()' CG_NOOP !
        VMDmpOPC(dmp, NULL);
        return;
    }

    switch(opEnum)
    {
    case CG_NOT:
        assert(!pRhs && CG_IsAnyBit(lTypeSet));
        VMPushValue(dmp, pctx, pLhs);
        VMDmpTypedOpOPC(dmp, _T("not"), lTypeSet)<<'\n';
    break;
    case CG_UMINUS:
        assert(!pRhs);
        VMPushValue(dmp, pctx, pLhs);

        typ1 = lTypeSet;
        if(typ1 == CG_TIME)
            typ1 = CG_DINT;

        VMDmpTypedOpOPC(dmp, _T("neg"), typ1)<<'\n';
    break;
    case CG_NOOP:
        VMEvalExpr(dmp, pctx, pushTyp, pLhs);
    break;

    case CG_OR:
    case CG_XOR:
    case CG_AND:
        assert(CG_IsAnyBit(lTypeSet));
        pszOp = CGT_ExprTypToLString(opEnum);
        VMPushValue(dmp, pctx, pLhs);
        VMPushValue(dmp, pctx, pRhs);
        VMDmpTypedOpOPC(dmp, pszOp, lTypeSet)<<'\n';
    break;
    case CG_EQ:
    case CG_NE:
    case CG_LT:
    case CG_LE:
    case CG_GT:
    case CG_GE:
        pszOp = CGT_ExprTypToLString(opEnum);
        typ1 = pLhs->pType->type;
        if(typ1==CG_STRING)
        {
            VMPushReference(dmp, pctx, false, pLhs);
            VMPushReference(dmp, pctx, false, pRhs);
        }
        else
        {
            VMPushValue(dmp, pctx, pLhs);
            VMPushValue(dmp, pctx, pRhs);
        }
        if(typ1==CG_TIME)
            typ1 = CG_DINT;
        VMDmpTypedOpOPC(dmp, pszOp, typ1)<<'\n';
    break;
    case CG_ADD:

        typ1 = pLhs->pType->type;
        if(typ1==CG_TOD)
        {
            assert(pRhs->pType->type==CG_TIME);
            pctx->pdm->errNS1(&pLhs->edp, pctx->pdm->jot1()<<_T("'+'")<<
                pctx->pdm->res1(CG_RES_STR_OP_ON_DTYPE)<<_T("TIME OF DAY"));
        }
        else if(typ1==CG_TIME)
        {
            typ1 = CG_DINT;
        }
        VMPushValue(dmp, pctx, pLhs);
        VMPushValue(dmp, pctx, pRhs);
        VMDmpTypedOpOPC(dmp, _T("add"), typ1)<<'\n';
    break;
    case CG_SUB:

        typ1 = pLhs->pType->type;
        if(typ1==CG_DATE)
        {
            pctx->pdm->errNS1(&pLhs->edp, pctx->pdm->jot1()<<_T("'-'")<<
                pctx->pdm->res1(CG_RES_STR_OP_ON_DTYPE)<<_T("DATE"));
        }
        else if(typ1==CG_TOD)
        {
            pctx->pdm->errNS1(&pLhs->edp, pctx->pdm->jot1()<<_T("'-'")<<
                pctx->pdm->res1(CG_RES_STR_OP_ON_DTYPE)<<_T("TOD"));
        }
        else if(typ1==CG_TIME)
        {
            typ1 = CG_DINT;
        }
        VMPushValue(dmp, pctx, pLhs);
        VMPushValue(dmp, pctx, pRhs);
        VMDmpTypedOpOPC(dmp, _T("sub"), typ1)<<'\n';
    break;
    case CG_MUL:

        typ1 = pLhs->pType->type;
        if (typ1 == CG_TIME)
        {
            switch(pRhs->pType->type)
            {
            case CG_DINT:
            case CG_UDINT:
                VMPushValue(dmp, pctx, pLhs);
                VMPushValue(dmp, pctx, pRhs);
                VMDmpOPC(dmp, _T("mul_dint"))<<'\n';
            break;

            case CG_INT:
                VMPushValue(dmp, pctx, pLhs);
                if (pRhs->extyp == CG_LITERAL)
                {
                    VMPushIntLit(dmp, CG_DINT, (long)pRhs->lit.pLit->i64);
                }
                else
                {
                    VMPushValue(dmp, pctx, pRhs);
                    VMDmpOPC(dmp, _T("int_to_dint"))<<'\n';
                }
                VMDmpOPC(dmp, _T("mul_dint"))<<'\n';
            break;
            case CG_UINT:
                VMPushValue(dmp, pctx, pLhs);
                if (pRhs->extyp == CG_LITERAL)
                {
                    VMPushIntLit(dmp, CG_DINT, (long)pRhs->lit.pLit->i64);
                }
                else
                {
                    VMPushValue(dmp, pctx, pRhs);
                    VMDmpOPC(dmp, _T("uint_to_dint"))<<'\n';
                }
                VMDmpOPC(dmp, _T("mul_dint"))<<'\n';
            break;

            case CG_REAL:
                VMPushValue(dmp, pctx, pLhs);
                VMDmpOPC(dmp, _T("dint_to_real"))<<'\n';
                VMPushValue(dmp, pctx, pRhs);
                VMDmpOPC(dmp, _T("mul_real"))<<'\n';
                VMDmpOPC(dmp, _T("real_to_dint"))<<'\n';
            break;
            case CG_LREAL:
                VMPushValue(dmp, pctx, pLhs);
                VMDmpOPC(dmp, _T("dint_to_lreal"))<<'\n';
                VMPushValue(dmp, pctx, pRhs);
        
                VMDmpOPC(dmp, _T("mul_lreal"))<<'\n';
                VMDmpOPC(dmp, _T("lreal_to_dint"))<<'\n';
            break;
            default:
                VMASSERT("bad types '*'", &pRhs->edp);
            }
        }
        else
        {
            VMPushValue(dmp, pctx, pLhs);
            VMPushValue(dmp, pctx, pRhs);
            VMDmpTypedOpOPC(dmp, _T("mul"), typ1)<<'\n';
        }

    break;
    case CG_DIV:
        typ1 = pLhs->pType->type;
        if (typ1 == CG_TIME)
        {
            switch(pRhs->pType->type)
            {
            case CG_DINT:
            case CG_UDINT:
                VMPushValue(dmp, pctx, pLhs);
                VMPushValue(dmp, pctx, pRhs);
                VMDmpOPC(dmp, _T("div_dint"))<<'\n';
            break;

            case CG_INT:
                VMPushValue(dmp, pctx, pLhs);
                if (pRhs->extyp == CG_LITERAL)
                {
                    VMPushIntLit(dmp, CG_DINT, (long)pRhs->lit.pLit->i64);
                }
                else
                {
                    VMPushValue(dmp, pctx, pRhs);
                    VMDmpOPC(dmp,  _T("int_to_dint"))<<'\n';
                }
                VMDmpOPC(dmp, _T("div_dint"))<<'\n';
            break;

            case CG_UINT:
                VMPushValue(dmp, pctx, pLhs);
                if (pRhs->extyp == CG_LITERAL)
                {
                    VMPushIntLit(dmp, CG_DINT, (long)pRhs->lit.pLit->i64);
                }
                else
                {
                    VMPushValue(dmp, pctx, pRhs);
                    VMDmpOPC(dmp,  _T("uint_to_dint"))<<'\n';
                }
                VMDmpOPC(dmp, _T("div_dint"))<<'\n';
            break;

            case CG_REAL:
                VMPushValue(dmp, pctx, pLhs);
                VMDmpOPC(dmp, _T("dint_to_real"))<<'\n';
                VMPushValue(dmp, pctx, pRhs);
                VMDmpOPC(dmp, _T("div_real"))<<'\n';
                VMDmpOPC(dmp, _T("real_to_dint"))<<'\n';
            break;
            case CG_LREAL:
                VMPushValue(dmp, pctx, pLhs);
                VMDmpOPC(dmp, _T("dint_to_lreal"))<<'\n';
                VMPushValue(dmp, pctx, pRhs);
                VMDmpOPC(dmp, _T("div_lreal"))<<'\n';
                VMDmpOPC(dmp, _T("lreal_to_dint"))<<'\n';
            break;
            default:
                CGT_IERR2(pctx->pdm, &pRhs->edp, _T("invalid data type of right side for multiplacation with data type TIME"));
            }
        }
        else
        {
            VMPushValue(dmp, pctx, pLhs);
            VMPushValue(dmp, pctx, pRhs);
            VMDmpTypedOpOPC(dmp, _T("div"), typ1)<<'\n';
        }
    break;
    case CG_MOD:
        assert(CG_IsAnyInt(lTypeSet));
        VMPushValue(dmp, pctx, pLhs);
        VMPushValue(dmp, pctx, pRhs);
        VMDmpTypedOpOPC(dmp, _T("mod"), pLhs->pType->type)<<'\n';
    break;
    case CG_POW:
        typ1 = pLhs->pType->type;
        typ2 = pRhs->pType->type;
        if(typ1 == CG_REAL && typ2 == CG_LREAL)
        {
            VMPushValue(dmp, pctx, pLhs);
            VMDmpOPC(dmp, _T("real_to_lreal"))<<'\n';
            VMPushValue(dmp, pctx, pRhs);
            VMDmpOPC(dmp, _T("pow_lreal_lreal"))<<'\n';
            VMDmpOPC(dmp, _T("lreal_to_real"))<<'\n';
        }
        else if(typ1 == CG_LREAL && typ2 == CG_REAL)
        {
            VMPushValue(dmp, pctx, pLhs);
            VMPushValue(dmp, pctx, pRhs);
            VMDmpOPC(dmp, _T("real_to_lreal"))<<'\n';
            VMDmpOPC(dmp, _T("pow_lreal_lreal"))<<'\n';
        }
        else
        {
            TCHAR buffer[MAX_PATH];
            assert(pLhs->pType->type==CG_LREAL || pLhs->pType->type==CG_REAL);
           
            strcpy(buffer, _T("pow_"));
            strcat(buffer, pLhs->pType->pszName);
            strcat(buffer, _T("_"));
            if(pRhs->pType->type==CG_UINT)
                strcat(buffer, "int"); //HACK: use int, will be incorrect if int value is neg
            else if(pRhs->pType->type==CG_UDINT)
                strcat(buffer, "dint");//HACK: use dint, will be incorrect if dint value is neg
            else
                strcat(buffer, pRhs->pType->pszName);

            strlwr(buffer);

            VMPushValue(dmp, pctx, pLhs);
            VMPushValue(dmp, pctx, pRhs);
            VMDmpOPC(dmp, buffer)<<'\n';
        }

    break;
    case CG_MOVE:
        VMAssign(dmp, pctx, pLhs, pRhs);
    break;
    default:
        CGT_IERR2(pctx->pdm, &pLhs->edp, _T("bad expression type"));
    }
}





void VMDumpLitValue(
    CGT_CDmp&         dmp, 
    const CG_Literal* pLit
)
{
    long          l;
    TCHAR    buffer[MAX_PATH];

    const TCHAR* pszTyp;

    switch(pLit->type)
    {
    case CG_BOOL:
        assert(pLit->ut==CG_LIT_I64 && (pLit->i64==0 || pLit->i64==1));
        dmp<< (pLit->i64 == 0 ? _T("false") : _T("true"));
    break;

    case CG_STRING:
        assert(pLit->ut==CG_LIT_NONE && pLit->psz);
        dmp << pLit->psz;
    break;

    case CG_REAL:   pszTyp = _T("real#"); goto xxFLT;
    case CG_LREAL:  pszTyp = _T("lreal#"); goto xxFLT;
    xxFLT:
        //don't dump opt +:
        if(pLit->psz[0]==_T('+'))
            dmp<<pszTyp<<pLit->psz+1;
        else
            dmp<<pszTyp<< pLit->psz;
    break;


    case CG_LINT:  
        dmp<<_T("lint#")<<pLit->i64;
    break;

    case CG_LWORD: 
        dmp<<_T("lword#16#")<<_ui64tot((unsigned __int64)pLit->i64, buffer, 16);
    break;

    case CG_TIME:
        dmp<<pLit->psz;
    break;


    case CG_BYTE:
        dmp<<_T("byte#16#")<<_ultoa((unsigned long)pLit->i64, buffer, 16);
    break;

    case CG_WORD:
        dmp<<_T("word#16#")<<_ultoa((unsigned long)pLit->i64, buffer, 16);
    break;

    case CG_DWORD:
        dmp<<_T("dword#16#")<<_ultoa((unsigned long)pLit->i64, buffer, 16);
    break;

    case CG_DINT:
        if(pLit->i64>=LONG_MIN && pLit->i64<=LONG_MAX)
            dmp<<_T("dint#")<<(long)pLit->i64;
        else if(pLit->i64>=0 && pLit->i64 <= 4294967295)
            dmp<<_T("dint#16#")<<_ultoa((unsigned long)pLit->i64, buffer, 16);
        else
            assert(!"dint out of range");
    break;

    case CG_UDINT:
        dmp<<_T("udint#16#")<<_ultoa((unsigned long)pLit->i64, buffer, 16);
    break;

    case CG_INT:
        l = (long)pLit->i64;
        if(l>=SHRT_MIN && l<=SHRT_MAX)
            dmp<<_T("int#")<<l;
        else if(l>=0 && l<=0xffff)
            dmp<<_T("int#16#")<<_ultoa((unsigned long)l, buffer, 16);
        else
            assert(!"int out of range");
    break;

    case CG_UINT:
        dmp<<_T("uint#16#")<<_ultoa((unsigned long)pLit->i64, buffer, 16);
    break;

    default:
        assert(!"unsupported data type in lit expr.");       
        dmp<<_T("<VMDumpLitValue: type error>");
    }
}

/* ---------------------------------------------------------------------------- */
