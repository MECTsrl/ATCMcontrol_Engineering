/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/dump4asm.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: dump4asm.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/dump4asm.h $
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

#ifndef _DUMPJAVA_H_
#define _DUMPJAVA_H_

#include "cgt_CDiagMsg.h"
#include "cgt_tools.h"
#include "cgmem.h"
class CGlobCtx;

class CCodeInfo
{
public:
    CCodeInfo(const CG_Symtab* pSym) 
        : m_pSym(pSym)
    {
        m_bErrors = false;
        m_pIExtBdyInf = NULL;
        m_pDmp = NULL;
    }
    ~CCodeInfo()
    {
        if(m_pIExtBdyInf)
            m_pIExtBdyInf->Release();
        if (m_pDmp != NULL)
            delete m_pDmp;
    }

    bool                  m_bErrors;
    const CG_Symtab*const m_pSym;
    ICG_ExtBodyInfo*      m_pIExtBdyInf;
    
    FC_CStr               m_asmFile;  //relative to gen

    CGT_CDmp*             m_pDmp;//dumper of the current POU
};


struct BODY_CTX_INFO;
bool CG_VM_DumpOneVariable(
    CGT_CDmp&           dmp,
    CGlobCtx*           pgctx,
    CTarget*            pTrg, //can be NULL if build libs !!
    const CG_SymEntry*  pEntry,
    bool                bDumpExternals
);

//TODO use check decl for all not supported diags, currently only used for macro function's decls
//but be care full CG_VM_DumpOneVariable does the same diags but also increments some IO counters
//don't simply call VMCheckDecl inside CG_VM_DumpOneVariable. Solution: rework CG_VM_DumpOneVariable
//to "use VMCheckDecl".
bool VMCheckDecl(
    CGlobCtx*           pgctx,
    CTarget*            pTrg, //can be NULL if build libs !!
    const CG_SymEntry*  pEntry
);

bool VMCalcDirectAddress(
	CGlobCtx*			pgctx,
	CTarget*			ptrg,
	const CG_SymEntry*  pEntry,
	VM_Address*         pVMAddr
);

//get precompiled code from lib:
void GenCodeLibPOU(
    CGlobCtx*               pgctx,
    const CG_Symtab*        pSym,
    CCodeInfo*              pInfo,
    FC_CGArray<CGT_BP_TYP>* pbpList
);
extern "C" void GenCodeStruct(
    CGlobCtx*                 pgctx,
    CCodeInfo*                pInfo
);
extern "C" void GenCodeSTPrgFB(
    CGlobCtx*                 pgctx,
    const CG_Resource*        pRes, //may be NULL if building a library
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
);
extern "C" void GenCodeSFCPrgFB(
    CGlobCtx*                 pgctx,
    const CG_Resource*        pRes, //may be NULL if building a library
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
);
extern "C" void GenCodeFun(
    CGlobCtx*                 pgctx,
    const CG_Resource*        pRes, //may be NULL if building a library
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
);

// syntetize next ST statements witch
// execute all transitions specified by doTrans
//    ST:i := 0; (* loop tmp 'i' *)
//    ST:WHILE true DO:
//    ST:    CASE doTransAct[i] OF
//    ST:    0: doTransAct[i] := bool_to_int(t1);
//    ST:    1: doTransAct[i] := bool_to_int(t2);
//    ST:    2: doTransAct[i] := bool_to_int(t3);
//    ST:    3: doTransAct[i] := bool_to_int(t4);
//    ST:    4: doTransAct[i] := bool_to_int(t5);
//    ST:    5: doTransAct[i] := bool_to_int(t6);
//    ST:    6: doTransAct[i] := bool_to_int(true);
//    ST:....ELSE
//    ST:       EXIT; (* -1 terminates the doTransAct array *)
//    ST:    END_CASE;
//    ST:    i := i + 1;    
//    ST: END_WHILE:
const CG_STStmt* createDoTransStmts(CG_Edipos* pEdp, CG_ExprMem* pExprMem, const CG_SFCStruct* pSFCS, BODY_CTX_INFO* pctx);

// syntetize next ST statements witch
// execute all transitions specified by doAct
//    ST:i := 0; (* loop tmp 'i' *)
//    ST:WHILE true DO:
//    ST:    CASE doTransAct[i] OF
//    ST:    0:  actionbool := byte_to_bool(Action0__Q);
//    ST:    1:  var1       := byte_to_bool(Step1__X);
//    ST:    2:  IF bReturn THEN
//                   goto  __action_return; (* return from action*)
//               END_IF
//               var2       := Step1__T >= t#40ms;
//    ST:    3:  var2       := byte_to_bool(Action2__Q) AND byte_to_bool(Action3__A);
//    ST:    4:  var2       := false;
//    ST:....ELSE
//    ST:       EXIT; (* -1 terminates the doTransAct array *)
//    ST:    END_CASE;
//    ST:    __action_return:
//    ST:    i := i + 1;
//    ST: END_WHILE:
const CG_STStmt* createDoActStmts(CG_Edipos* pEdp, CG_ExprMem* pExprMem, const CG_SFCStruct* pSFCS, BODY_CTX_INFO* pctx);

#endif

/* ---------------------------------------------------------------------------- */
