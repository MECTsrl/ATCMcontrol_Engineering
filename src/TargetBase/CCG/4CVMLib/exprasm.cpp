/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/exprasm.cpp 2     17.04.07 16:21 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: exprasm.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/exprasm.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 17.04.07 16:21 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "vmasm.h"
#include "VmConstants.h"
#include "resource.h"
#include "cgt_address.h"


static void VMVarExprPushPopValue(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
	bool                 bPush,
    const CG_DType*      pType,
    const CG_VarExpr*    pVar
);
static void VMVarExprPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable, 
    const CG_VarExpr*    pVar
);

static bool VMTryOptStructExprPushPopValue(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
	bool                 bPush,
    const CG_DType*      pType,
    const CG_StructExpr* pStr
);
static bool VMTryOptStructRefPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable,
    const CG_DType*      pType,
    const CG_StructExpr* pStr
);

static bool VMTryOptArrayExprPushPopValue(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
	bool                 bPush,
    const CG_DType*      pType,
    const CG_ArrayExpr*  pStr
);
static bool VMTryOptArrayRefPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable,
    const CG_DType*      pType,
    const CG_ArrayExpr*  pArr
);

static CG_DTYPES VMArrIndex(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    const CG_ArrayExpr*  pArr
);







void VMEvalExpr(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    VM_STACKTYP          pushTyp, 
    const CG_ExprTree*   pExpr
)
{
    switch(pushTyp)
    {
    case VM_PUSH_VALUE:     
        VMPushValue(dmp, pctx, pExpr);
    break;
	case VM_POP_VALUE:      
        VMPopValue(dmp, pctx, pExpr);
    break; 

    case VM_PUSH_CST_REF:   
        VMPushReference(dmp, pctx, false, pExpr); 
    break;
    case VM_PUSH_WRT_REF:   
        VMPushReference(dmp, pctx, true,  pExpr); 
    break;
    default:
        CGT_IERR2(pctx->pdm, &pExpr->edp, _T("invalid stack type"));
    }
}




VM_STACKTYP VMGetFunArgPushType(
    const CG_ArgExpr* pArgExpr
)
{
    bool            bRef;
    const CG_DType* pType = pArgExpr->pActual->pType;
    CG_DECLSCOPE    ds    = pArgExpr->pFormal->declScope;
    
    assert(CG_IsMonomorph(pType->type));
        
    bRef = ds==CG_VAR_IN_OUT || ds==CG_VAR_OUTPUT || 
		   pType->pSym || pType->bIsArray || pType->type==CG_STRING;

    if(bRef)
        return (ds==CG_VAR_INPUT||ds==CG_VAR_IN_EXT) ? VM_PUSH_CST_REF : VM_PUSH_WRT_REF;

    return VM_PUSH_VALUE;
}
bool VMIsRefFunFBParam(const CG_VarExpr* pVar)
{
    const CG_SymEntry* pEntry = pVar->pEntry;
    CG_DECLSCOPE       ds     = pEntry->declScope;

    assert(CG_IsMonomorph(pEntry->pType->type) && ds!=CG_VAR_IN_EXT);

    if(ds == CG_VAR_IN_OUT)
        return true;

    if(pEntry->pParent->pouTyp != CG_POU_FUN)
        return false;
    
    //if function:
    if(pEntry == pEntry->pParent->pFirst)
    {
        return VMIsPtrType(pEntry->pType);
    }
    else
    {
        if(ds == CG_VAR_OUTPUT)
            return true;

        if(ds==CG_VAR_INPUT && VMIsPtrType(pEntry->pType))
            return true;
    }
    return false;
}










void VMPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable, 
    const CG_ExprTree*   pExpr
)
{
    const CG_DType* pType;

    pType = pExpr->pType;

    if(!CG_IsMonomorph(pType->type))
    {
        VMDmpOPC(dmp, NULL);
        return;
    }   

    switch(pExpr->extyp)
    {
    case CG_VARREF:
        VMVarExprPushReference(dmp, pctx, bWritable, &pExpr->var);
    break;
    case CG_STRUCTREF:
        if(!VMTryOptStructRefPushReference(dmp, pctx, bWritable, pType, &pExpr->str))
        {
            VMPushReference(dmp, pctx, false, pExpr->str.pBase);

            if(pType->pSym || pType->bIsArray)
                VMDmpOPC(dmp, _T("LEAI_ID"));
            else if(pType->type == CG_STRING)
                VMDmpOPC(dmp, _T("LEAO_ST"));
            else
                if(pType->type==CG_BOOL)
                    VMDmpOPC(dmp, _T("LEAO_ST_1"));
                else
                    VMDmpOPC(dmp, _T("LEAO_ST"));

            //selector offset is the first and only paramter of all these opcodes:
            VMDmpOffsetOf(dmp, pExpr->str.pSelc)<<'\n';
        }
    break;        
    case CG_ARRAYREF:
        if(!VMTryOptArrayRefPushReference(dmp, pctx, bWritable, pType, &pExpr->arr))
        {
            VMPushReference(dmp, pctx, false, pExpr->arr.pBase);

            CG_DTYPES aidxtyp = VMArrIndex(dmp, pctx, &pExpr->arr);
            if(pType->pSym)
            {
                LCT("cmplx array ref struct");
                VMDmpOPC(dmp, _T("LEAV_SS"));
            }
            else if(pType->type == CG_STRING)
            {
                LCT("cmplx array ref string");
                VMDmpOPC(dmp, _T("LEAV_TX"));
            }
            else if(pType->bIsArray)
            {
                assert(!"array of array");
            }
            else
            {
                LCT("cmplx array ref simple");
                //param 1 is the elem type bit size:
                VMDmpOPC(dmp, _T("LEAV_ST"));
				(dmp<<_T("sizeof1(")).lo(pType->pszName)<<_T(") ");
            }
		    dmp<<_T("aidx(")<<CGT_DTYPES_ToLString(aidxtyp)<<_T(")\n");
        }
    break;
    case CG_LITERAL:
        if(pType->type==CG_STRING && pType->type==pExpr->lit.pLit->type && !bWritable)
            VMDmpOPC(dmp, _T("PSHC_TX"));
        else
	        goto Error;
        VMDumpLitValue(dmp, pExpr->lit.pLit);
        dmp<<'\n';
    break;        
    case CG_TMPVAR:
        // TODO: remove this restriction
        if(!pExpr->tmp.pRefExpr &&
            (pExpr->pType->type==CG_STRUCT || pExpr->pType->bIsArray)
          )
        {
            pctx->pdm->errNS1(&pExpr->edp, pctx->pdm->res1(CG_RES_STR_CPLX_TMP_LD_FBD));
            return;
        }
        
        if(pExpr->tmp.pRefExpr)
		{
			if(pExpr->pType->type==CG_BOOL && !pExpr->pType->bIsArray)
				VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_64") : _T("PSHD_IN_32"));
			else
				VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16"));
		}	
        else if(pExpr->pType->type == CG_STRING)
            VMDmpOPC(dmp, _T("LEAD_IN"));
        else
	        goto Error;

        VMDmpTmpVarName(
            dmp, 
            pctx->pgctx, 
            pExpr->pType, 
            pExpr->tmp.lTmpNum, 
            pExpr->tmp.pRefExpr != NULL
        );
        dmp<<'\n';
    break;
    case CG_FUNCALL:
    case CG_FUNMACRO:
        if(!bWritable && (pType->type == CG_STRING || pType->pSym || pType->bIsArray))
            VMDumpFunCall(dmp, pctx, pExpr, NULL);
        else
	        goto Error;
    break;
    case CG_NOOP:
        VMPushReference(dmp, pctx, bWritable, pExpr->ops.pLhs);
    break;
    default:
        goto Error;
    }

	return;

Error:
	pctx->pdm->msg1(CG_E_TEXT, &pExpr->edp, 
		pctx->pdm->jot1()<<
		_T("cannot take _addressof('")<<CGT_ExprTypToLString(pExpr->extyp)<<
		"') which is of type '"<< pExpr->pType->pszName<<'\'');  
}


static void VMVarExprPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable, 
    const CG_VarExpr*    pVar
)
{
    bool                 bObjType;
    CG_DECLSCOPE         dsr;
    const CG_DType*      pType;
    const CG_SymEntry*   pEntry;


    pEntry   = pVar->pEntry;
    dsr      = pEntry->declScope;
    pType    = pEntry->pType;
    bObjType = pType->bIsArray || pType->pSym;

    assert(pctx->pSym == pEntry->pParent);
    assert(CG_IsMonomorph(pType->type));


    if(dsr == CG_VAR_EXTERNAL)
    {
        //all var external are tricky but (luck!) the same for fun, prg and FB:
        if(bObjType)
        {
            //if IQM (this is struct AT %-address feature), simply use the LEADG[IOM] 
            //with argument <globvar>.<member> do not use LEAO_XD or other!! we must 
            //make sure that, if there is a local task image for these segments, the 
            //address of the cached version is pushed, not the global copy!. 
            switch(VMGetGlobVarSegment(pctx, pEntry))
            {
            case VM_SEG_I: 
                VMDmpOPC(dmp, _T("LEAD_GI")).lo(pEntry->pszId)<<'\n';
            break;
            case VM_SEG_Q: 
                if(pctx->ulTrgVer>=20600)
                {
                    VMDmpOPC(dmp, _T("LEAD_GX")).lo(pEntry->pszId);
                    if(bWritable)
                    {
                        dmp<<_T(" sizeof8(");
                        VMDmpType(dmp, pType)<<_T(")\n");
                    }
                    else
                    {
                        dmp<<_T(" 0\n");
                    }
                }
                else
                {
                    VMDmpOPC(dmp, _T("LEAD_GO")).lo(pEntry->pszId)<<'\n';
                }
            break;
            case VM_SEG_M: 
                VMDmpOPC(dmp, _T("LEAD_GM")).lo(pEntry->pszId)<<'\n';
            break;
            case VM_SEG_GA: 
            case VM_SEG_GR: 
                //auto global or retain struct/array/FB:
                //Auto global and global retain never have a task cache, and also 
                //we don't know the final global offset at compile time (this may change in future!)
                VMDmpOPC(dmp, _T("LEAO_XD"));
                (dmp<<_T("objid(")).lo(pEntry->pszId)<<_T(")\n");
            break;
            default:
                assert(!"bad glob segm");
            }
        }
        else
        {
            //if IQM or auto glob(retain) simply use the LEADG[IOMR] 
            //with argument offset of globvar. If there is a task cache 
            //the task local copy is used. In contrast too global struct,array,FB
            //we also know the final offset of simple type (also string) for automatic 
            //globals too.
            switch(VMGetGlobVarSegment(pctx, pEntry))
            {
            case VM_SEG_I: 
                if(pType->type==CG_BOOL)
                    VMDmpOPC(dmp, _T("LEAD_GI_1")).lo(pEntry->pszId)<<'\n';
                else
                    VMDmpOPC(dmp, _T("LEAD_GI")).lo(pEntry->pszId)<<'\n';
            break;
            case VM_SEG_Q: 
                if(pType->type==CG_BOOL)
                    VMDmpOPC(dmp, _T("LEAD_GX_1")).lo(pEntry->pszId)<<'\n';
                else
                    if(pctx->ulTrgVer>=20600)
                    {
                        VMDmpOPC(dmp, _T("LEAD_GX")).lo(pEntry->pszId);
                        if(bWritable)
                        {
                            dmp<<_T(" sizeof8(");
                            VMDmpType(dmp, pType)<<_T(")\n");
                        }
                        else
                        {
                            dmp<<_T(" 0\n");
                        }
                    }
                    else
                    {
                        VMDmpOPC(dmp, _T("LEAD_GO")).lo(pEntry->pszId)<<'\n';
                    }
            break;
            case VM_SEG_GA:
                if(pctx->ulTrgVer>=21000)
                {
                    VMDmpOPC(dmp, pType->type==CG_BOOL ? _T("LEAO_ID_1") : _T("LEAO_ID"));
                    (dmp<<_T("objid(__global_inst) offsetof(__global,")).lo(pEntry->pszId)<<_T(")\n");
                    break;
                }//else: fall th.: older version use LEAD_GM(1) for simple globals:
            case VM_SEG_M: 
                if(pType->type==CG_BOOL)
                    VMDmpOPC(dmp, _T("LEAD_GM_1")).lo(pEntry->pszId)<<'\n';
                else
                    VMDmpOPC(dmp, _T("LEAD_GM")).lo(pEntry->pszId)<<'\n';
            break;
            case VM_SEG_GR: 
                if(pctx->ulTrgVer>=21000)
                {
                    VMDmpOPC(dmp, pType->type==CG_BOOL ? _T("LEAO_ID_1") : _T("LEAO_ID"));
                    (dmp<<_T("objid(__global_retain_inst) offsetof(__global_retain,")).lo(pEntry->pszId)<<_T(")\n");
                }
                else
                {
                    if(pType->type==CG_BOOL)
                        VMDmpOPC(dmp, _T("LEAD_GR_1")).lo(pEntry->pszId)<<'\n';
                    else
                        VMDmpOPC(dmp, _T("LEAD_GR")).lo(pEntry->pszId)<<'\n';
                }
            break;
            default:
                assert(!"bad glob segm");
            }
        }
    }
    else if(dsr == CG_VAR_IN_OUT)
    {
        //any form of inout regardless if fun,prg or FB are pointers (simple BOOL is 64bit ptr!):
        if(pType->type==CG_BOOL && !pType->bIsArray) //d-335
            VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_64") : _T("PSHD_IN_32")).lo(pEntry->pszId)<<'\n';
        else
            VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16")).lo(pEntry->pszId)<<'\n';
    }
    else if(pctx->pSym->pouTyp == CG_POU_FUN)
    {
        //function is tricky:
        assert(dsr != CG_VAR_IN_EXT);
        if(dsr == CG_VAR_INPUT)
        {
			if(bObjType || pType->type==CG_STRING)
            {
                VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16")).lo(pEntry->pszId)<<'\n';
            }
			else
            {
                VMDmpOPC(dmp, _T("LEAD_IN")).lo(pEntry->pszId)<<'\n';//this line will never be reached...
            }
        }
        else if(dsr == CG_VAR_OUTPUT)
        {
			if(pEntry == pEntry->pParent->pFirst)
            {
				if(bObjType || pType->type==CG_STRING)
                {
                    //the fun name itself is only a pointer if struct or array or string:
                    VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16")).lo(pEntry->pszId)<<'\n';
                }
				else
                {
                    //the fun name itself is a value if simple type:
                    VMDmpOPC(dmp, _T("LEAD_IN")).lo(pEntry->pszId)<<'\n';//this line will never be reached...
                }
            }
            else
            {
                //all function output that are not the fun name itself are pointers (simple BOOL are 64bit ptr!):
                if(pType->type==CG_BOOL && !pType->bIsArray) //d-335
                    VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_64") : _T("PSHD_IN_32")).lo(pEntry->pszId)<<'\n';
                else
                    VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("PSHD_IN_32") : _T("PSHD_IN_16")).lo(pEntry->pszId)<<'\n';
            }
        }
        else if(bObjType)
        {
            //Prg,FB local struct,array:
            VMDmpOPC(dmp, _T("LEAO_XN")).lo(pEntry->pszId)<<'\n';
        }
        else
        {
            //fun local simple (also string):
            if(pType->type==CG_BOOL)
                VMDmpOPC(dmp, _T("LEAD_IN_1")).lo(pEntry->pszId)<<'\n';
            else
                VMDmpOPC(dmp, _T("LEAD_IN")).lo(pEntry->pszId)<<'\n';
        }
    }
    else
    {
        if(bObjType)
        {
            //Prg,FB local struct,array:
            VMDmpOPC(dmp, _T("LEAO_XN")).lo(pEntry->pszId)<<'\n';
        }
        else
        {
            //Prg,FB local simple (also string):
            if(pType->type==CG_BOOL || CG_IsAnyEdge(pType->type))   //<-may be r/f_edge bool 
                VMDmpOPC(dmp, _T("LEAD_IN_1")).lo(pEntry->pszId)<<'\n';//too since pType = pEntry->pType
            else
                VMDmpOPC(dmp, _T("LEAD_IN")).lo(pEntry->pszId)<<'\n';
        }
    }
}




void VMPushValue(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pExpr
)
{
    const CG_DType*      pType;
    CG_DTYPES            type;
    
    pType = pExpr->pType;
    type  = pType->type;
    
    assert(!pType->bIsArray && !pType->pSym && type!=CG_STRING && CG_IsMonomorph(type));

    switch(pExpr->extyp)
    { 
    case CG_LITERAL:
        assert(type == pExpr->lit.pLit->type);
        VMPushLitValue(dmp, pctx, pExpr->lit.pLit);
    break;
    case CG_VARREF:
        VMVarExprPushPopValue(dmp, pctx, true, pType, &pExpr->var);
    break;
    case CG_STRUCTREF:
        if(!VMTryOptStructExprPushPopValue(dmp, pctx, true, pType, &pExpr->str))
        {
            VMPushReference(dmp, pctx, false, pExpr->str.pBase);
            VMDmpSizedOPC(dmp, _T("PSHO_ST_"), type);
            VMDmpOffsetOf(dmp, pExpr->str.pSelc)<<'\n';
        }
    break;
    case CG_ARRAYREF:
        if(!VMTryOptArrayExprPushPopValue(dmp, pctx, true, pType, &pExpr->arr))
        {
            VMPushReference(dmp, pctx, false, pExpr->arr.pBase);
            CG_DTYPES aidxtyp = VMArrIndex(dmp, pctx, &pExpr->arr);
            VMDmpSizedOPC(dmp, _T("PSHV_ST_"), type);
		    dmp<<_T("aidx(")<<CGT_DTYPES_ToLString(aidxtyp)<<_T(")\n");
        }
    break;
    case CG_USEREXPR:
        if(pExpr->usr.ldata == UE_SFC_INSTANCE_ARRAY)
		{
            VMDmpOPC(dmp, _T("PSHD_IN_16")).lo(pExpr->usr.psz2)<<'\n';  //push index (which is an INT)
            VMDmpOPC(dmp, _T("PSHX____16")).lo(pExpr->usr.psz1)<<' ';//array access as INT
			dmp<<_T("aidx(int)\n");
		}
		else
		{
            assert(!"bad user expr");
        }
    break;
    case CG_STEPACTION:
    {
        //TODO: the x,a and q are stored as bytes, must convert, better use packed bits...
        //var   time32       step0__t                           AT 2446   [32 bit]
        //var   time32       step1__t                           AT 2450   [32 bit]
        //var   byte         step0__x                           AT 2454   [8  bit]
        //var   byte         step1__x                           AT 2455   [8  bit]
        //var   byte         action0__a                         AT 2456   [8  bit]
        //var   byte         action1__a                         AT 2457   [8  bit]
        //var   byte         action0__q                         AT 2458   [8  bit]
        //var   byte         action1__q                         AT 2459   [8  bit]
        switch(pExpr->sfc.typ)
        {
        case CG_ACTION_Q: 
            VMDmpOPC(dmp, _T("PSHD_IN_8")).lo(pExpr->sfc.pszSA);
            dmp<<_T("__q\n"); VMDmpOPC(dmp, _T("byte_to_bool"))<<'\n';
        break;
        case CG_ACTION_A: 
            VMDmpOPC(dmp, _T("PSHD_IN_8")).lo(pExpr->sfc.pszSA);
            dmp<<_T("__a\n"); VMDmpOPC(dmp, _T("byte_to_bool"))<<'\n';
        break;
        case CG_STEP_X:   
            VMDmpOPC(dmp, _T("PSHD_IN_8")).lo(pExpr->sfc.pszSA);
            dmp<<_T("__x\n"); VMDmpOPC(dmp, _T("byte_to_bool"))<<'\n';
        break;
        case CG_STEP_T:   
            assert(pExpr->pType->type==CG_TIME);
            VMDmpSizedOPC(dmp, _T("PSHD_IN_"), CG_TIME).lo(pExpr->sfc.pszSA);
            dmp<<_T("__t\n");
        break;
        default:
            assert(!"bad step action expr");
        }
    }
    break;
    default: 
       VMOperator(dmp, pctx, VM_PUSH_VALUE, pExpr->pType->type, pExpr->extyp, 
           pExpr->ops.pLhs, pExpr->ops.pRhs);
    break;
    case CG_FUNCALL:
    case CG_FUNMACRO:
        VMDumpFunCall(dmp, pctx, pExpr, NULL);
    break;
    case CG_TMPVAR:
        if(pExpr->tmp.pRefExpr)
        {
			if(pType->type == CG_BOOL)
			{
				//TODO: bit pointer: case 1 : VMDmpOPC(dmp, _T("PSHI_IN_1"));
				//Workaround: push original expr:
				VMPushValue(dmp, pctx, pExpr->tmp.pRefExpr);
				break;
			}
			else
			{
	            VMDmpSizedOPC(dmp, _T("PSHI_IN_"), pType->type);
			}
        }
        else
        {
            VMDmpSizedOPC(dmp, _T("PSHD_IN_"), pType->type);
        }

        VMDmpTmpVarName(
            dmp, 
            pctx->pgctx, 
            pExpr->pType, 
            pExpr->tmp.lTmpNum, 
            pExpr->tmp.pRefExpr != NULL
         );
        dmp<<'\n';
    break;
    case CG_TRIGVAR:
    {
        assert(pExpr->pType->type == CG_BOOL);
        LCT("push trig var");

        // TODO: optimize: tmpvar with register size
        VMDmpOPC(dmp, _T("PSHD_IN_1"))<<_T("§1_T_")<<pExpr->trg.lTrigNum<<'\n';
    }
    break;
    }; //end switch;
}


void VMPopValue(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pExpr
)
{
    const CG_DType*      pType;
    CG_DTYPES            type;
    
    pType = pExpr->pType;
    type  = pType->type;
    
    assert(CG_IsMonomorph(type));
    assert(!pType->bIsArray && !pType->pSym && type!=CG_STRING);

    switch(pExpr->extyp)
    { 
    case CG_VARREF:
        VMVarExprPushPopValue(dmp, pctx, false, pType, &pExpr->var);
    break;
    case CG_STRUCTREF:
        if(!VMTryOptStructExprPushPopValue(dmp, pctx, false, pType, &pExpr->str))
        {
            VMPushReference(dmp, pctx, false, pExpr->str.pBase);
            VMDmpSizedOPC(dmp, _T("POPO_ST_"), type);
            VMDmpOffsetOf(dmp, pExpr->str.pSelc)<<'\n';
        }
    break;
    case CG_ARRAYREF:
        if(!VMTryOptArrayExprPushPopValue(dmp, pctx, false, pType, &pExpr->arr))
        {
            VMPushReference(dmp, pctx, false, pExpr->arr.pBase);
            CG_DTYPES aidxtyp = VMArrIndex(dmp, pctx, &pExpr->arr);
            VMDmpSizedOPC(dmp, _T("POPV_ST_"), type);
		    dmp<<_T("aidx(")<<CGT_DTYPES_ToLString(aidxtyp)<<_T(")\n");
        }
    break;
    //case CG_USEREXPR: currently no pop on these...
    case CG_TMPVAR:
        assert(!pExpr->tmp.pRefExpr);
        VMDmpSizedOPC(dmp, _T("POPD_IN_"), type);
        VMDmpTmpVarName(
            dmp, 
            pctx->pgctx, 
            pType, 
            pExpr->tmp.lTmpNum, 
            pExpr->tmp.pRefExpr != NULL
        );
        dmp<<'\n';
    break;
    case CG_TRIGVAR:
    {
        assert(pType->type == CG_BOOL);
        LCT("pop trig var");
        VMDmpOPC(dmp, _T("POPD_IN_1"))<<_T("§1_T_")<<pExpr->trg.lTrigNum<<'\n';
    }
    break;
    default: 
        assert(!"invalid left expression type");
    };
}


static bool CG_IsUserStrFunRetOptSafe(const CG_ExprTree* pLhs, const CG_FunExpr* pFun)
{
	//in order to omit a temp var for a function returning a string
	//we must make sure that the left hand side of the assignment is never
	//a part of the argument list or can be reached from inside the function.
	//Note that this opt is never possible in C because of pointers that you never
	//know of, but in IEC there are no pointers, at least not yet...

	//rule 0: the max string length of the rhs must match the fun return string size!
//TODO if you fix d-356 then also check strlen:
//	if(pLhs->pType->lStrLen != pFun->pSym->pFirst->pType->lStrLen)
//		return false;

	//rule 1: if the lhs is not a local var it could be referenced inside the 
	//        called fun by using VAR_EXTERNAL. 
	const CG_SymEntry* pLhsEntry = CGT_GetExprSymEntry(pLhs);
	if(!pLhsEntry)
		return false; //should not happen in IEC language...

	if(pLhsEntry->declScope!=CG_VAR && pLhsEntry->declScope!=CG_VAR_TEMP && 
	  !(pLhsEntry->declScope==CG_VAR_OUTPUT && pLhsEntry->pParent->pouTyp==CG_POU_FB)
	  )
	  return false; //not a var/vartemp and also not a FB output, not local => do not opt
		

	//rule 3: now check that this entry is not referenced in the arglist:
	for(int i=0; i<pFun->nArgs; i++)
	{
		if(CGT_GetExprSymEntry(pFun->pArgs[i].pActual) == pLhsEntry)
			return false; //possible alias! => do not opt
	}

	return true; //ok it is safe...
}

void VMAssign(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs
)
{
    const CG_DType* pLType = pLhs->pType;
    const CG_DType* pRType = pLhs->pType;

    assert(CG_IsMonomorph(pLType->type));
    if(pLhs->extyp==CG_TMPVAR && pLhs->tmp.pRefExpr)
    {
        //HACK: ref tmps are a bad solution, we must first implement 
        //a pointer/and or reference data type description!
        //Now the idea is if a ref tmp is on the left side of an assignment,
        //then use take the ref of the right side:

        //to understand the assert see documentaion in _CG_TmpVarExpr struct decl:
        assert(pRhs->extyp!=CG_TMPVAR);
        //pLhs->tmp.pRefExpr must be really the "same expr" then pRhs, but not necessarily 
        //the same pointer, TODO: implement a CGT_IsSameExpr function, here we test simply
        //the entry of any x.y.[i].z expression:
        assert(CGT_GetExprSymEntry(pLhs->tmp.pRefExpr)==CGT_GetExprSymEntry(pRhs));
        
        LCT("reftmp := lval-expr"); //<-this will happen only in LD/FBD
        VMPushReference(dmp, pctx, true, pRhs); //TODO, is this always a writable ref ??
                                                //currently ref tmp only used for 
                                                //inout--inout LD/FBD stuff, therefore 
                                                //true is ok.
        //TODO: as long as there is no concept of ref or pointzer data types it is better
        //to make here a special case then to call the "VMPopValue", minimum would 
        //be to have a VMPopReference, but the real solution is to have a Value datatype '&','const&' or '*':
        //It is assumed that only the CG_ASSIGN is used to initialise a ref tmp (as domumented in CG_TmpVar
        //struct decl!) and all later uses are either 
        // *reftmp   if simple type
        // or 
        // push reftmp for any function input complex or function output or any inout
		if(pLType->type==CG_BOOL && !pLType->bIsArray)
			VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("POPD_IN_64") : _T("POPD_IN_32"));
		else
			VMDmpOPC(dmp, VM_ADDRESS_SIZE==32 ? _T("POPD_IN_32") : _T("POPD_IN_16"));
        VMDmpTmpVarName(dmp, pctx->pgctx, pLType, pLhs->tmp.lTmpNum, true);
        dmp<<'\n';
    }
    else if(pLType->bIsArray)
    {
        // cannot copy assumed size arrays
        if(CG_IsAssumedSizeArray(pLType) || CG_IsAssumedSizeArray(pRType))
            pctx->pgctx->m_dm.errNS1(&pLhs->edp, pctx->pgctx->m_dm.res1(VM_RES_STR_COPY_ASSUMED_SIZE));
        // cannot copy array of complex data type
        if(pLType->pSym)
            pctx->pgctx->m_dm.errNS1(&pLhs->edp,  pctx->pgctx->m_dm.res1(VM_RES_STR_CPLX_ARRAY_DEEPCOPY));

        VMPushReference(dmp, pctx, false, pRhs);
        VMPushReference(dmp, pctx, true,  pLhs);
        VMDmpOPC(dmp, _T("MCPY"));
        dmp<<_T("sizeof8(");
        VMDmpType(dmp, pLhs->pType)<<_T(")\n");
    }
    else if(pLType->pSym)
    {
        //cannot copy struct with complex members:
        for(const CG_SymEntry* pEntry = pLType->pSym->pFirst; pEntry; pEntry = pEntry->pNext)
        {
            if(pEntry->pType->pSym || pEntry->pType->bIsArray)
            {
                pctx->pgctx->m_dm.errNS1(&pLhs->edp,  pctx->pgctx->m_dm.res1(CG_RES_STR_CPLX_DEEPCOPY));
                break;
            }
        }
        VMPushReference(dmp, pctx, false, pRhs);
        VMPushReference(dmp, pctx, true,  pLhs);
        VMDmpOPC(dmp, _T("MCPY"));
        dmp<<_T("sizeof8(");
        VMDmpType(dmp, pLhs->pType)<<_T(")\n");
    }
    else if(pLType->type==CG_STRING)
    {
        if(pRhs->extyp==CG_FUNMACRO || //by definition of VM c-code we can opt. macro calls.
			                           //the c-code implementation must use a temp var internally if needed! 
			(pRhs->extyp==CG_FUNCALL && CG_IsUserStrFunRetOptSafe(pLhs, &pRhs->fun)) 
	      )
        {
            //optimize the case string := fun(....)
            //s1 := concat(s2, s3, 'x');
            //lea           s1
            //lea           s2
            //push          'x'
            //lea           s3
            //push          int#2
            //CALF_SL       3 18 ;ConcatString
            VMDumpFunCall(dmp, pctx, pRhs, pLhs);
        }
        else
        {
            VMPushReference(dmp, pctx, false, pRhs);
            VMPushReference(dmp, pctx, true,  pLhs);
            VMDmpOPC(dmp, _T("MOVE_STRING"))<<'\n';
        }
    }
    else
    {
        VMPushValue(dmp, pctx, pRhs);
        VMPopValue(dmp, pctx,  pLhs);
    }
}


void VMDumpInternalSyslibCall1(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const TCHAR*         pszSyslibfun,
    long                 lSyslibFunID,
    bool                 bClearRetVal,
    CG_DTYPES            retType,
    CG_DTYPES            arg1Type //TODO need!!! a CG_POINTER 
)
{
    long        lParamStkBytes = 0;
    VM_STACKTYP stkType;
    long        nStkAlign = pctx->pgctx->m_kad.getVmSettings()->stackAlignBytes;

    assert(lSyslibFunID>=0);

    if(bClearRetVal && retType!=CG_VOID)
    {
        if(retType==CG_STRING || retType==CG_STRUCT || retType==CG_INSTANCE)
            stkType = VM_PUSH_WRT_REF;
        else
            stkType = VM_PUSH_VALUE;

        lParamStkBytes += VMTypeStackBytes(retType, false, stkType, nStkAlign);
    }

    if(arg1Type != CG_VOID)
    {
        if(arg1Type==CG_STRING || arg1Type==CG_STRUCT || arg1Type==CG_INSTANCE)
            stkType = VM_PUSH_WRT_REF;
        else
            stkType = VM_PUSH_VALUE;

        lParamStkBytes += VMTypeStackBytes(arg1Type, false, stkType, nStkAlign);
    }

    VMDmpOPC(dmp, _T("CALF_SL"))<<lSyslibFunID<<' '<<lParamStkBytes;
    dmp<<_T("; ")<<pszSyslibfun<<'\n';
}






VM_SEG_TYP VMGetGlobVarSegment(
    const BODY_CTX_INFO* pctx,
    const CG_SymEntry*   pEntry
)
{
    VM_SEG_TYP         seg;
    const CG_SymEntry* pGlob;
    
    assert(pEntry->declScope==CG_VAR_EXTERNAL);

    if(!pctx->pRes)
    {
        LCT("build lib pou with var ext");
        return VM_SEG_GA;
    }

    // if var external is autodecl then there is no GVL file
    if(pEntry->declMod & CG_DMOD_SYS_VAR)
    {
        pGlob = pEntry;
    }
    else
    {
        pGlob = pctx->pIF->lookUpGlobVar(pctx->pRes, pEntry->pszId);
        assert(pGlob); 
        if(!pGlob)
            return VM_SEG_GA;
    }

    if(pGlob->pszAddr)
    {
        switch(CGT_GetAddrLocation(pGlob->pszAddr))
        {
        case 'I': seg = VM_SEG_I; break;
        case 'Q': seg = VM_SEG_Q; break;
        case 'M': seg = VM_SEG_M; break;
        default:
            assert(!"bad %-address");
            seg = VM_SEG_UNDEF;
        }
    }
    else if(pGlob->declMod & CG_DMOD_RETAIN)
    {
        seg = VM_SEG_GR;
    }
    else
    {
        seg = VM_SEG_GA;
    }
    
    return seg;
}




static void VMVarExprPushPopValue(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
	bool                 bPush,
    const CG_DType*      pType,
    const CG_VarExpr*    pVar
)
{
    const CG_SymEntry*   pEntry;
	CG_DTYPES            type;
    
    pEntry = pVar->pEntry;
    type   = pType->type;
    
    assert(CG_IsMonomorph(type) && type==pEntry->pType->type);
    assert(!pType->bIsArray && !pType->pSym && type!=CG_STRING);

    if(pEntry->declScope==CG_VAR_EXTERNAL)
    {
        //if IQM or auto glob(retain) simply use the POPGD[IOMR] 
        //with argument offset of globvar. If there is a task cache 
        //the task local copy is used. In contrast too global struct,array,FB
        //we also know the final offset of simple type (also string) for automatic 
        //globals too.
        switch(VMGetGlobVarSegment(pctx, pEntry))
        {
        case VM_SEG_I: 
            VMDmpSizedOPC(dmp, bPush ? _T("PSHD_GI_") : _T("POPD_GI_"), type);
        break;
        case VM_SEG_Q: 
			if(bPush)
			{
				VMDmpSizedOPC(dmp, _T("PSHD_GO_"), type);
			}
			else
			{
				if(pctx->ulTrgVer>=20600)
				{
					LCT(POPGDOX_SIMPLE);
					VMDmpSizedOPC(dmp, _T("POPD_GX_"), type);
				}
				else
				{
					VMDmpSizedOPC(dmp, _T("POPD_GO_"), type);
				}
			}
        break;
        case VM_SEG_M:
            VMDmpSizedOPC(dmp, bPush ? _T("PSHD_GM_") : _T("POPD_GM_"), type);
        break;
        case VM_SEG_GA:
			if(pctx->ulTrgVer >= 21000)
			{
	            VMDmpSizedOPC(dmp, bPush ? _T("PSHO_ID_") : _T("POPO_ID_"), type);
				dmp<<_T("objid(__global_inst) ");
				dmp<<_T("offsetof(__global,");
				dmp.lo(pEntry->pszId)<<_T(')')<<'\n';
				return;
			}	
			else
				VMDmpSizedOPC(dmp, bPush ? _T("PSHD_GM_") : _T("POPD_GM_"), type);
        break;
        case VM_SEG_GR: 
			if(pctx->ulTrgVer >= 21000)
			{
	            VMDmpSizedOPC(dmp, bPush ? _T("PSHO_ID_") : _T("POPO_ID_"), type);
				dmp<<_T("objid(__global_retain_inst) ");
				dmp<<_T("offsetof(__global_retain,");
				dmp.lo(pEntry->pszId)<<_T(')')<<'\n';
				return;
			}	
			else
	            VMDmpSizedOPC(dmp, bPush ? _T("PSHD_GR_") : _T("POPD_GR_"), type);
        break;
        default:
            assert(!"bad glob segm");
        }
    }
	else if(VMIsRefFunFBParam(pVar))
    {
        VMDmpSizedOPC(dmp, bPush ? _T("PSHI_IN_") : _T("POPI_IN_"), type);
    }
    else
    {
        VMDmpSizedOPC(dmp, bPush ? _T("PSHD_IN_") : _T("POPD_IN_"), type);
    }

	//all opcodes have a simple glo or local offset argument:
	dmp.lo(pEntry->pszId)<<'\n';
}










static bool VMTryOptStructExprPushPopValue(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
	bool                 bPush,
    const CG_DType*      pType,
    const CG_StructExpr* pStr
)
{
    const CG_SymEntry* pVarBase;
    CG_DTYPES          type;

	if(pStr->pBase->extyp != CG_VARREF)
		return false;

    type = pType->type;
    pVarBase = pStr->pBase->var.pEntry;

    if(pType->pSym || pType->bIsArray || type==CG_STRING || !CG_IsMonomorph(type))
    {
        assert(!"value no simple");
        return false;
    }

    //1. if the base of the struct is a global variable:
    if(pVarBase->declScope==CG_VAR_EXTERNAL)
    {
        const TCHAR* pszB = pVarBase->pszId;
        const TCHAR* pszS = pStr->pSelc->pszId;

        switch(VMGetGlobVarSegment(pctx, pVarBase))
        {
        case VM_SEG_I: 
            VMDmpSizedOPC(dmp, bPush ? _T("PSHD_GI_") : _T("POPD_GI_"), type);
            (dmp.lo(pszB)<<'.').lo(pszS)<<'\n';
        break;
        case VM_SEG_Q: 
			if(bPush)
			{
				VMDmpSizedOPC(dmp, _T("PSHD_GO_"), type);
			}
			else
			{
				if(pctx->ulTrgVer>=20600)
				{
					LCT(POPGDOX_STR_MEMBER);
					VMDmpSizedOPC(dmp, _T("POPD_GX_"), type);
				}
				else
				{
					LCT(POPGDO_STR_MEMBER);
					VMDmpSizedOPC(dmp, _T("POPD_GO_"), type);
				}
			}
            (dmp.lo(pszB)<<'.').lo(pszS)<<'\n';
        break;
        case VM_SEG_M: 
            VMDmpSizedOPC(dmp, bPush ? _T("PSHD_GM_"): _T("POPD_GM_"), type);
            (dmp.lo(pszB)<<'.').lo(pszS)<<'\n';
        break;
        case VM_SEG_GA: 
        case VM_SEG_GR: 
            //auto global or retain param: index and selector offset:
            VMDmpSizedOPC(dmp, bPush ? _T("PSHO_ID_") : _T("POPO_ID_"), type);
            (dmp<<_T("objid(")).lo(pszB)<<_T(") ");
            VMDmpOffsetOf(dmp, pStr->pSelc)<<'\n';
        break;
        default:
            assert(!"bad glob segm");
            return false;
        }

        return true;
    }


    //2. special op code for <local struct>.<simple member>
    if(VMIsRefFunFBParam(&pStr->pBase->var))
        return false;

    //optimize <instloc_struct>.<simple member>:
    //with special opcode:
    VMDmpSizedOPC(dmp, bPush ? _T("PSHO_IN_") : _T("POPO_IN_"), type);
	dmp.lo(pVarBase->pszId)<<' ';
    VMDmpOffsetOf(dmp, pStr->pSelc)<<'\n';
    
    return true;
}

static bool VMTryOptStructRefPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable,
    const CG_DType*      pType,
    const CG_StructExpr* pStr
)
{
    const CG_SymEntry* pVarBase;
    CG_DTYPES          type;

    type = pType->type;
    pVarBase = pStr->pBase->extyp==CG_VARREF ? pStr->pBase->var.pEntry : NULL;

    if(!pVarBase)
        return false;

    if(pType->pSym || pType->bIsArray)
        return false;

    if(VMIsRefFunFBParam(&pStr->pBase->var))
        return false;

    //1. if the base of the struct is a global variable:
    if(pVarBase->declScope==CG_VAR_EXTERNAL)
    {
        const TCHAR* pszB = pVarBase->pszId;
        const TCHAR* pszS = pStr->pSelc->pszId;

        switch(VMGetGlobVarSegment(pctx, pVarBase))
        {
        //if IQM (this is struct AT %-address feature, simply use the LEADG[IOM] 
        //with argument <globvar>.<member> do not use LEAO_IN or other!! we must 
        //make sure that, if there is a local task image for these segments, the 
        //address of the cached version is pushed, not the global copy!. 
        case VM_SEG_I: 
            if(pType->type==CG_BOOL)//array of bool does not pass here, so don't need to the !bIsArray 
                (VMDmpOPC(dmp, _T("LEAD_GI_1")).lo(pszB)<<'.').lo(pszS)<<'\n';
            else
                (VMDmpOPC(dmp, _T("LEAD_GI")).lo(pszB)<<'.').lo(pszS)<<'\n';
        break;
        case VM_SEG_Q: 
            if(pType->type==CG_BOOL)//array of bool does not pass here, so don't need to the !bIsArray 
                (VMDmpOPC(dmp, _T("LEAD_GX_1")).lo(pszB)<<'.').lo(pszS)<<'\n';
            else
                if(pctx->ulTrgVer>=20600)
                {
                    (VMDmpOPC(dmp, _T("LEAD_GX")).lo(pszB)<<'.').lo(pszS);
                    if(bWritable)
                    {
                        dmp<<_T(" sizeof8(");
                        VMDmpType(dmp, pType)<<_T(")\n");
                    }
                    else
                    {
                        dmp<<_T(" 0\n");
                    }
                }
                else
                {
                    (VMDmpOPC(dmp, _T("LEAD_GO")).lo(pszB)<<'.').lo(pszS)<<'\n';
                }
        break;
        case VM_SEG_M: 
            if(pType->type==CG_BOOL)//array of bool does not pass here, so don't need to the !bIsArray 
                (VMDmpOPC(dmp, _T("LEAD_GM_1")).lo(pszB)<<'.').lo(pszS)<<'\n';
            else
                (VMDmpOPC(dmp, _T("LEAD_GM")).lo(pszB)<<'.').lo(pszS)<<'\n';
        break;
        case VM_SEG_GA: 
        case VM_SEG_GR: 
            //auto global or retain param: index and selector offset:
            //Auto global and global retain never have a task cache, and also 
            //we don't know the final global offset at compile time (this may change in future!)
            if(pType->type==CG_BOOL)//array of bool does not pass here, so don't need to the !bIsArray 
                VMDmpOPC(dmp, _T("LEAO_ID_1"));
            else
                VMDmpOPC(dmp, _T("LEAO_ID"));
            (dmp<<_T("objid(")).lo(pszB)<<_T(") ");
            VMDmpOffsetOf(dmp, pStr->pSelc)<<'\n';
        break;
        default:
            assert(!"bad glob segm");
            return false;
        }
    }
    else
    {
        if(pType->type==CG_BOOL)//array of bool does not pass here, so don't need to the !bIsArray 
            VMDmpOPC(dmp, _T("LEAO_IN_1"));
        else
            VMDmpOPC(dmp, _T("LEAO_IN"));
        dmp.lo(pVarBase->pszId)<<' ';
        VMDmpOffsetOf(dmp, pStr->pSelc)<<'\n';
    }
   
    return true;
}





static bool VMTryOptArrayExprPushPopValue(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
	bool                 bPush,
    const CG_DType*      pType,
    const CG_ArrayExpr*  pArr
)
{
    const CG_SymEntry* pEntry;
    CG_DTYPES          elemType;
	CG_DTYPES          aidxtyp;

	elemType = pType->type;

    assert(!pType->bIsArray && CG_IsMonomorph(elemType));
	
    if(pArr->pBase->extyp != CG_VARREF)
        return false;

    if(pType->pSym || elemType == CG_STRING)
        return false;
    
	if(VMIsRefFunFBParam(&pArr->pBase->var))
        return false;

    pEntry = pArr->pBase->var.pEntry;

    //optimize inst.array[i] or globalarray[i] with special opcode:
    aidxtyp = VMArrIndex(dmp, pctx, pArr);

	if(pEntry->declScope == CG_VAR_EXTERNAL)
	{
		VMDmpSizedOPC(dmp, bPush ? _T("PSHV_ID_") : _T("POPV_ID_"), elemType);
		(dmp<<_T("objid(")).lo(pEntry->pszId)<<_T(") ");
	}
	else
	{
		VMDmpSizedOPC(dmp, bPush ? _T("PSHV_IN_") : _T("POPV_IN_"), elemType);
		dmp.lo(pEntry->pszId)<<' ';
	}

    dmp<<_T("aidx(")<<CGT_DTYPES_ToLString(aidxtyp)<<_T(")\n");
    
    return true;
}

static bool VMTryOptArrayRefPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable,
    const CG_DType*      pType,
    const CG_ArrayExpr*  pArr
)
{
    long               lElemBits;
    const CG_SymEntry* pEntry;
    CG_DTYPES          aidxtyp;

    assert(!pType->bIsArray && CG_IsMonomorph(pType->type));//array of array ?


    if(pArr->pBase->extyp != CG_VARREF)
        return false;
        
    if(pType->pSym || pType->bIsArray || pType->type == CG_STRING)
        return false;

    if(VMIsRefFunFBParam(&pArr->pBase->var))
        return false;

    pEntry = pArr->pBase->var.pEntry;
    //optimize inst.array[i] or globalarray[i] with special opcode:
    //push          int#1
    //leavid        loc_simple_array
    //or
    //leav          glob_simple_array

    //push index:
    aidxtyp = VMArrIndex(dmp, pctx, pArr);
 
    //push bit size of element type as an int of aidxtyp size !!:
    lElemBits = VMSimpleTypeBitSize(pType->type);
    VMPushIntLit(dmp, aidxtyp, lElemBits);

    if(pEntry->declScope == CG_VAR_EXTERNAL)
    {
        VMDmpOPC(dmp, _T("LEAV_ID"));
        (dmp<<_T("objid(")).lo(pEntry->pszId)<<_T(") ");
    }
    else
    {
        VMDmpOPC(dmp, _T("LEAV_IN"));
        dmp.lo(pEntry->pszId)<<' ';
    }

    dmp<<_T("aidx(")<<CGT_DTYPES_ToLString(aidxtyp)<<_T(")\n");
    
    return true;
}


static CG_DTYPES VMArrIndex(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    const CG_ArrayExpr*  pArr
)
{
    const CG_DType* pIdxTyp;

    // check the index data type
    pIdxTyp = pArr->pSubs[0].pType;
    if((pIdxTyp->tbs & pctx->pgctx->m_kad.getArrIdxIECTypes())==0 || pArr->nSubs!=1)
    {
        assert(pArr->nSubs==1);
        pctx->pdm->errNS1(
            &pArr->pSubs->edp, 
            pctx->pdm->res1(VM_RES_STR_NS_ARR_IDX).fmtStr1(NULL, pIdxTyp->pszName));
        return CG_INT;
    }

    VMPushValue(dmp, pctx, &pArr->pSubs[0]);
    //correct the range of the array

    CG_DTYPES at = pIdxTyp->type;
    if(pArr->pBase->pType->range[0].lRangeStart != 0)
    {
        if(at==CG_UINT)
            at = CG_INT;
        else if(at==CG_UDINT)
            at = CG_DINT;

        VMPushIntLit(dmp, at, pArr->pBase->pType->range[0].lRangeStart);
        VMDmpTypedOpOPC(dmp, _T("sub"), at)<<'\n';
    }

    return at;
}           

CGT_CDmp& VMDmpTmpVarName(
    CGT_CDmp&         dmp,
    CGlobCtx*         pgctx,
    const CG_DType*   pType,
    long              TmpVarId,
    bool              bReference
)
{
    assert(CG_IsMonomorph(pType->type));
    // TODO: optimize size of STRING temp var
    if(bReference)
    {
		if(pType->type==CG_BOOL && !pType->bIsArray)
			dmp<<'@'<<VM_ADDRESS_SIZE*2<<'_'<<TmpVarId;
		else
			dmp<<'@'<<VM_ADDRESS_SIZE<<'_'<<TmpVarId;
    }
    else if(pType->type==CG_STRING)
    {
        assert(!pType->bIsArray);
        dmp<<_T("@s")<<VM_MAX_STRLEN<<'_'<<TmpVarId;
    }
    else
    {
        assert(!pType->pSym && !pType->bIsArray);
        dmp<<'@'<<VMSimpleTypeBitSize(pType->type)<<'_'<<TmpVarId;
    }
    return dmp;
}













VM_MACRO_TYP VMClassifyMacroText(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszMac, 
    TCHAR            funopid[MAX_PATH]
)
{
    int          i;
    const TCHAR* psz;
    VM_MACRO_TYP mtyp;

    funopid[0] = 0;

    psz = pszMac;
    FC_StringSkipWhite(&psz);

    if(FC_StringIncBeginI(&psz, CG_VM_NOOP))
        mtyp = VM_MAC_NOOP;
    else if(FC_StringIncBeginI(&psz, CG_VM_VMOP))
        mtyp = VM_MAC_VMOP;
    else if(FC_StringIncBeginI(&psz, CG_VM_EXTLIB))
        mtyp = VM_MAC_EXTLIB;
    else if(FC_StringIncBeginI(&psz, CG_VM_STDLIB))
        mtyp = VM_MAC_STDLIB;
    else
        return VM_MAC_ASM;

    FC_StringSkipWhite(&psz);
    if(*psz==_T('('))
    {
        *psz++;
        i = 0;
        while(*psz && *psz!=_T(')') && i<MAX_PATH-1)
            funopid[i++] = *psz++;

        funopid[i] = 0;
        FC_StringTrim(funopid);
        
        if(*psz==_T(')'))
            psz++;
    }
    
    FC_StringSkipWhite(&psz);
    if(*psz)
    {
        pdm->msg0(CG_E_MACRO_EXPANSION, pEdp);
        mtyp = VM_MAC_ERROR;
    }
    return mtyp;
}






//type size functions:
long VMSimpleTypeBitSize(CG_DTYPES type)
{
    switch(type)
    {
    case CG_VOID:   return 0;
    case CG_BOOL_R_EDGE: //should not pass here ?
    case CG_BOOL_F_EDGE:
    case CG_BOOL:   return 1;
    case CG_BYTE:   return 8;
    case CG_WORD:   return 16;
    case CG_DWORD:  return 32;
    case CG_INT:    return 16;
    case CG_UINT:   return 16;
    case CG_DINT:   return 32;
    case CG_UDINT:  return 32;
    case CG_TIME:   return 32; //TODO CG_TIME[32|64]
    case CG_REAL:   return 32;
    case CG_LREAL:  return 64;
    default:
        assert( ! "invalid IEC simple data type");
        return 1000000004;
    }
}

long VMSimpleDTypeBitSize(const CG_DType* pType)
{
	 if(pType->pSym || pType->bIsArray)
	 {
		 assert(!"not simple type");
         return 1000000003;
	 }

     if(pType->type==CG_STRING)
	 {
        if(pType->lStrLen == -1)
            return 2 * 8 + VM_MAX_STRLEN * 8;
        else
            return 2 * 8 + pType->lStrLen * 8; //Content
	 }
	 else
	 {
		 return VMSimpleTypeBitSize(pType->type);
	 }
}

static long VMStructAtByteSize(
	const CG_Edipos* pEdp, 
	const CG_Symtab* pSym,
	CGlobCtx*        pgctx
)
{
    const CG_SymEntry*  pEntry;
    long				lBitSize = 0;
    long				lDeclBits;
    long				lDeclAlign;
	long                lTrgAlignBits = pgctx->m_kad.getVmSettings()->instAlignBytes * 8;
    long                lMaxDeclAlignBits = 8;

    for(pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        if(pEntry->pType->pSym || pEntry->pType->bIsArray)
        {
			pgctx->m_dm.errNS1(pEdp, pgctx->m_dm.jot1().loadRes(VM_RES_STR_NS_STRUCTAT_NOSIMPLE));
            return -1;
        }

        //align bits at bits and strings at byte, allll other (simple!) types
        //to min(lDeclBits, lTrgAlignBits):
        lDeclBits = VMSimpleDTypeBitSize(pEntry->pType);
        switch(pEntry->pType->type)
        {
        case CG_BOOL:    lDeclAlign = 1; break;
        case CG_STRING:  lDeclAlign = 8; break;
        default:         lDeclAlign = min(lDeclBits, lTrgAlignBits); break; 
        }
        lMaxDeclAlignBits = max(lDeclAlign, lMaxDeclAlignBits);
        lBitSize = VM_AlignBitOffset(lBitSize, lDeclAlign);
        lBitSize += lDeclBits;
    }

    // align the size to max decl align:
    lBitSize = VM_AlignBitOffset(lBitSize, lMaxDeclAlignBits);
    assert(lBitSize % 8 == 0);
    
    return lBitSize/8;
}



long VMDTypeFullByteSize(
    CGlobCtx*        pgctx, 
    const CG_Edipos* pEdp, 
    const CG_DType*  pType
)
{
     long elemBits;
 
     if(pType->bIsArray && CG_IsAssumedSizeArray(pType))
        pgctx->m_dm.errNS1(pEdp, pgctx->m_dm.res1(VM_RES_STR_COPY_ASSUMED_SIZE));
	 
     if(pType->pSym)
	 {
        if(pType->bIsArray) //array of struct is very special, is an array of obj indices!?
        {
            assert(VM_ARRAY_LIMIT_SIZE==VM_OBJ_INDEX_SIZE);//assert: otherwise need alignment treatement...
            return (VM_ARRAY_LIMIT_SIZE + pType->nElems*VM_OBJ_INDEX_SIZE) / 8;
        }

        return VMStructAtByteSize(pEdp, pType->pSym, pgctx);
     }
     
     
     
     if(pType->type==CG_STRING)
	 {
        if(pType->lStrLen == -1)
            elemBits = 2*8 + VM_MAX_STRLEN*8;
        else
            elemBits = 2*8 + pType->lStrLen*8;
	 }
	 else
	 {
		 elemBits = VMSimpleTypeBitSize(pType->type);
	 }

     if(pType->bIsArray)
     {
        //this alignment code is copied from asm's function 'ASM_GetFirstArrayElemBitOff'
        long alignRelevantElemBitSize = pType->type==CG_STRING ? 8 : elemBits;
        long declAlign = min(alignRelevantElemBitSize, pgctx->m_kad.getVmSettings()->instAlignBytes*8);
        long arrayLimitBits = ((VM_ARRAY_LIMIT_SIZE+declAlign-1)/declAlign)*declAlign;
         
        elemBits = arrayLimitBits + elemBits*pType->nElems;
     }

     return (elemBits+7)/8;
}







long VMTypeStackBytes(
    CG_DTYPES    exprType, 
    boolean      bIsArray,
    VM_STACKTYP  stackTyp, 
    long         stackAlignBytes
)
{
    assert(stackAlignBytes>=1 && stackAlignBytes<=8);

    switch(stackTyp)
    {
    case VM_PUSH_WRT_REF:
    case VM_PUSH_CST_REF:
        if(!bIsArray && (exprType==CG_BOOL || CG_IsAnyEdge(exprType)))
            // double address size to have place for bit offset also
            return max(VM_ADDRESS_SIZE*2/8, stackAlignBytes);
        else
            return max(VM_ADDRESS_SIZE/8, stackAlignBytes); 
    case VM_PUSH_VALUE:   
    case VM_POP_VALUE:
        assert(!bIsArray);
        switch(exprType)
        {
        case CG_BOOL:   return stackAlignBytes;//don't need to test for max...
        case CG_BYTE:   return stackAlignBytes;//don't need to test for max...
        case CG_WORD:   return max(2, stackAlignBytes);
        case CG_DWORD:  return max(4, stackAlignBytes);
        case CG_INT:    return max(2, stackAlignBytes);
        case CG_UINT:   return max(2, stackAlignBytes);
        case CG_DINT:   return max(4, stackAlignBytes);
        case CG_UDINT:  return max(4, stackAlignBytes);
        case CG_TIME:   return max(4, stackAlignBytes);//TODO CG_TIME[32|64]
        case CG_REAL:   return max(4, stackAlignBytes);
        case CG_LREAL:  return max(8, stackAlignBytes);
        default:
            ;
        }
    default:
        ; 
    }
    VMFATAL(_T("invalid stack type"), NULL);
    return 1000000004;
}

CGT_CDmp& VMDmpOPC(CGT_CDmp& dmp, const TCHAR* pszOP)
{
    int icol;

    if(!pszOP)
    {
        assert(!"internal error");
        pszOP = "internal error";
    }
    if(!pszOP[0])
        pszOP = " ";//HACK empty opc shall indent...
    dmp<<pszOP[0]; //HACK: problem with column() and indent level...
    icol = dmp.column();
    return (dmp<<pszOP+1).sep(icol+15);
}

CGT_CDmp& VMDmpTypedOpOPC(
    CGT_CDmp&       dmp, 
    const TCHAR*    pszOP, 
    CG_DTYPES       type
)
{
    int icol;

    if(!pszOP || type==CG_TIME || !CG_IsMonomorph(type))
    {
        assert(!"internal error");
        pszOP = "internal error";
    }
    if(!pszOP[0])
        pszOP = " ";//HACK
    dmp<<pszOP[0]; //HACK: problem with column() and indent level...
    icol = dmp.column();
    return (dmp<<pszOP+1<<'_'<<CGT_DTYPES_ToLString2(type)).sep(icol+15);
}


CGT_CDmp& VMDmpSizedOPC(
    CGT_CDmp&       dmp, 
    const TCHAR*    pszOP, 
    CG_DTYPES       type
)
{
    int icol;
    dmp<<pszOP[0]; //HACK: problem with column() and indent level...
    icol = dmp.column();
    return (dmp<<pszOP+1<<VMSimpleTypeBitSize(type)).sep(icol+15);
}

CGT_CDmp& VMDmpOffsetOf(CGT_CDmp& dmp, const CG_SymEntry* pSelector)
{

    dmp<<_T("offsetof(");
    dmp.lo(pSelector->pParent->pszId);
    dmp<<',';
    dmp.lo(pSelector->pszId);
    dmp<<')';
    return dmp;
}

CGT_CDmp& VMDmpType(CGT_CDmp& dmp, const CG_DType* pType)
{
    assert(CG_IsMonomorph(pType->type));

    if(pType->pSym)
    {
        dmp.lo(pType->pSym->pszId);
    }
    else if(pType->type == CG_STRING)
    {
        dmp<<_T("string(");
        
        if(pType->lStrLen != -1)
            dmp<<pType->lStrLen;
        else
            dmp<<VM_MAX_STRLEN;

        dmp<<')';
    }
    else 
    {
        dmp<<CGT_DTYPES_ToLString2(pType->type);
    }
    
    if(pType->bIsArray)
    {
        if(CG_IsAssumedSizeArray(pType))
        {
            dmp<<_T("[]");
        }
        else
        {
            int   i;
            int   nDims = pType->nDim;
            assert(nDims == 1); //nDims==0 
            
            for(i=0; i < nDims;i++)
                dmp << _T("[") << (long)(pType->range[i].ulRangeLength + 1) << _T("]");
        }
    }
    return dmp;
}


void VMPushLitValue(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_Literal*    pLit
)
{
    const TCHAR* pszOP = _T("PSHC_");
    int          icol;

    if(pLit->type==CG_STRING)
    {
        assert(!"string as value");
        dmp<<"internal error";
        return;
    }

    dmp<<pszOP[0]; //HACK: problem with column() and indent level...
    icol = dmp.column();
    dmp<<pszOP+1/*<<VMSimpleTypeBitSize(pLit->type)*/;

    //optimize push 'binary 0 or 1':
    if(pLit->i64==0) //this is also ok for real 0, and double 1
	{
        dmp<<_T("00_");
	    dmp<<VMSimpleTypeBitSize(pLit->type);
	}
    else if(pLit->i64==1) //this does not work for real, double 1.0 ??
	{
        dmp<<_T("01_");
	    dmp<<VMSimpleTypeBitSize(pLit->type);
	}
    else
	{
		dmp<<_T("VV_");
		dmp<<VMSimpleTypeBitSize(pLit->type);
        VMDumpLitValue(dmp.sep(icol+15), pLit);
	}

    dmp<<'\n';
}


void VMPushIntLit(
    CGT_CDmp& dmp, 
    CG_DTYPES typ, //only CG_INT, CG_DINT
    long      l
)
{
    if(l==0)
    {
        switch(typ)
        {
        case CG_INT: case CG_UINT: VMDmpOPC(dmp, _T("PSHC_00_16")); break;
        case CG_DINT:case CG_UDINT:VMDmpOPC(dmp, _T("PSHC_00_32")); break;
        default:
            assert(!"not impl.");
            dmp<<"internal error";
        }
    }
    else if(l==1)
    {
        switch(typ)
        {
        case CG_INT: case CG_UINT: VMDmpOPC(dmp, _T("PSHC_01_16")); break;
        case CG_DINT:case CG_UDINT:VMDmpOPC(dmp, _T("PSHC_01_32")); break;
        default:
            assert(!"not impl.");
            dmp<<"internal error";
        }
    }
    else
    {
        TCHAR buffer[256];
        switch(typ)
        {
        case CG_INT:   VMDmpOPC(dmp, _T("PSHC_VV_16"))<<_T("int#")<<l;  break;
        case CG_UINT:  VMDmpOPC(dmp, _T("PSHC_VV_16"))<<_T("uint#")<<_ultot(l, buffer, 10); break;
        case CG_DINT:  VMDmpOPC(dmp, _T("PSHC_VV_32"))<<_T("dint#")<<l; break;
        case CG_UDINT: VMDmpOPC(dmp, _T("PSHC_VV_32"))<<_T("udint#")<<_ultot(l, buffer, 10); break;
        default:
            assert(!"not impl.");
            dmp<<"internal error";
        }
    }
    dmp<<'\n';
}

/* ---------------------------------------------------------------------------- */
