/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/vmasm.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: vmasm.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/vmasm.h $
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

#ifndef _VMASM_H_
#define _VMASM_H_

#include "defines.h"
#include "globctx.h"
#include "exprcode.h"
#include "cgt_misc.h"

typedef enum VM_STACKTYP
{
    VM_STK_UNDEF,
    VM_PUSH_VALUE,    //push any from of simple type 1,8,16,32,64 bits, no strings
    VM_POP_VALUE,     //pop simple value (or pointer to something)
    VM_PUSH_WRT_REF,  //the reference will may be modified (passed as var_in_out, fun output) 
    VM_PUSH_CST_REF   //push a reference to any data type, simple, array,m struct, string
                      //the reference will not be modified.
};

//------------------------------------------------------------------*
/**
 * VMIsPtrType: see inline code.
 */
inline bool VMIsPtrType(const CG_DType* pType)
{
    assert(CG_IsMonomorph(pType->type));//don't try to use if poly type !
    return pType->pSym || pType->bIsArray || (pType->tbs & CG_TBS_ANY_STRING)!=0;
}

//------------------------------------------------------------------*
/**
 * VMGetFunArgPushType determines the push type (ref, const ref or value)
 * of a function actual argument. Both the decls scope of the formal 
 * and the data type of the actual expression is used to define the push type
 *
 * @param   pVar    var ref.
 * @return          ony of VM_STACKTYP enum values.
 *                    -VAR_IN_OUT :                         push ref 
 *                    -VAR_OUTPUT in FUN:                   push ref
 *                    -the fun return variable and type is 
 *                     any array, struct or string:         push ref
 *                      
 *                    -VAR_INPUT of fun and type is 
 *                     any array, struct or string          push cont ref
 *                    -all other: simple stack value:       push value 
 * @see VMIsPtrType, VMIsRefFunFBParam
*/
VM_STACKTYP VMGetFunArgPushType(
    const CG_ArgExpr* pArgExpr
);


//------------------------------------------------------------------*
/**
 * VMIsRefFunFBParam tests if a variable (inside the POU body!)
 * is in any form passed as a reference to the FB, function or program. 
 * Note: a previous form of this function used a CG_SymEntry not a CG_VarExpr, 
 * but this lead to wrong use of this method: use it only the classify 
 * how to access POU variables inside the body of the same POU.
 * The problem is that the type info of polymorphic function's sym entries
 * for exampel "ANY" is not sufficient, but a varref must be non polymorpic
 * and currently there is no body for poly funs anway...
 *
 * @param   pVar    var ref.
 * @return          true if 
 *                    -VAR_IN_OUT in FB or FUN
 *                    -VAR_OUTPUT in FUN, but not the fun return
 *                    -the fun return variable and type is any array, struct or string, 
 *                     in this case tha caller passes a refernce to the result.
 *                    -VAR_INPUT of fun and type is any array, struct or string
 * @see VMIsPtrType, VMGetFunArgPushType                     
*/
bool VMIsRefFunFBParam(
    const CG_VarExpr*    pEntry
);



void VMEvalExpr(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    VM_STACKTYP          pushTyp, 
    const CG_ExprTree*   pExpr
);

void VMPushValue(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pExpr
);


void VMPushReference(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    bool                 bWritable, 
    const CG_ExprTree*   pExpr
);
void VMPopValue(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pExpr
);


void VMAssign(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs
);

void VMOperator(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    VM_STACKTYP          pushTyp, 
    CG_DTYPES            lTypeSet,
    CG_EXPRTYP           opEnum,
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs
);
void VMDumpLitValue(
    CGT_CDmp&         dmp, 
    const CG_Literal* pLit
);
void VMDumpFunCall(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_ExprTree*   pExprR,
    const CG_ExprTree*   pExprL
);
void VMDumpInternalSyslibCall1(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const TCHAR* pszSyslibfun,
    long         lSyslibFunID,
    bool         bClearRetVal, //true if retval (if any) shall be cleared from stack
    CG_DTYPES    retType,
    CG_DTYPES    arg1Type
);
CGT_CDmp& VMDmpTmpVarName(
    CGT_CDmp&         dmp,
    CGlobCtx*         pgctx,
    const CG_DType*   pType,
    long              TmpVarId,
    bool              bReference
);



//inline{ VMOP(dword_to_bool) }
//inline{ VMOP }
//inline{ NOOP }
//inline{ STDLIB(dint_to_string) }
//inline{ EXTLIB(dint_to_string) }
//inline{ <any asm text with %-inserts> }
enum VM_MACRO_TYP
{
    VM_MAC_UNDEF,
    VM_MAC_ERROR,
    VM_MAC_NOOP,
    VM_MAC_VMOP,
    VM_MAC_STDLIB,
    VM_MAC_EXTLIB,
    VM_MAC_ASM,
};
VM_MACRO_TYP VMClassifyMacroText(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszMac, 
    TCHAR            funopid[MAX_PATH]
);

typedef enum VM_SEG_TYP
{
    VM_SEG_UNDEF,
    VM_SEG_I,
    VM_SEG_Q,
    VM_SEG_M,
    VM_SEG_GA,
    VM_SEG_GR,
    VM_SEG_L,
    VM_SEG_LR
};

VM_SEG_TYP VMGetGlobVarSegment(
    const BODY_CTX_INFO* pctx,
    const CG_SymEntry*   pEntry
);


//type size functions:
long VMSimpleTypeBitSize(CG_DTYPES type);
long VMSimpleDTypeBitSize(const CG_DType* pType);

//return byte size of type including any array limit fields
//Note: A simple BOOL has size 1 and an array of BOOL has 
//size  (VM_ARRAY_LIMIT_SIZE + nElems*1 +7)/8
//For structs the "struct At %<Addr> size algorithm" VMStructAtByteSize is used!!
long VMDTypeFullByteSize(
    CGlobCtx*        pgctx, 
    const CG_Edipos* pEdp, 
    const CG_DType*  pType
);

//return bit size of type including any array limit fields.
//Note: An array of bool has always bit size that is a multible of 8.
inline long VMDTypeFullBitSize(
    CGlobCtx*        pgctx, 
    const CG_Edipos* pEdp, 
    const CG_DType*  pType
)
{
    if(pType->type==CG_BOOL && !pType->bIsArray)
		return 1;
	else
		return VMDTypeFullByteSize(pgctx, pEdp, pType) * 8;
}

long VMTypeStackBytes(
    CG_DTYPES    exprType, 
    boolean      bIsArray,
    VM_STACKTYP  stackTyp, 
    long         stackAlignBytes
);



CGT_CDmp& VMDmpOPC(
    CGT_CDmp&            dmp, 
    const TCHAR*         pszOP
);
CGT_CDmp& VMDmpTypedOpOPC(
    CGT_CDmp&       dmp, 
    const TCHAR*    pszOP, 
    CG_DTYPES       type
);
CGT_CDmp& VMDmpSizedOPC(
    CGT_CDmp&       dmp, 
    const TCHAR*    pszOP, 
    CG_DTYPES       type
);
CGT_CDmp& VMDmpOffsetOf(
    CGT_CDmp&          dmp, 
    const CG_SymEntry* pSelector
);
CGT_CDmp& VMDmpType(
    CGT_CDmp& dmp, 
    const CG_DType* pType
);

void VMPushLitValue(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    const CG_Literal*    pLit
);
void VMPushIntLit(
    CGT_CDmp& dmp, 
    CG_DTYPES typ, //only CG_INT, CG_DINT
    long      l
);


//inital values:
CGT_CDmp& VMDumpArrayInit(CGT_CDmp& dmp, const CG_ArrayInit* pArrInit);

#endif

/* ---------------------------------------------------------------------------- */
