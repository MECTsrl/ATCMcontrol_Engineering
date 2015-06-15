/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/cgmem.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: cgmem.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/cgmem.h $
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

#ifndef _CGMEM_H_
#define _CGMEM_H_

#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_cgarray.h"
#include "cg_iecfront.h"

typedef  bool (*ST_ExprEnumCallBack)(void* pCtx, const CG_ExprTree* pExpr);

class CG_ExprMem
{
	const CG_Symtab*           m_pSymCurrPOU;//used for pParent in registerSymEntry; 
    FC_CHashStrTable           m_strings;
    FC_CGArray<CG_ExprTree*>   m_exprs; //TODO make performance allocator !!!
    FC_CGArray<CG_SymEntry*>   m_symEntries;
    FC_CGArray<CG_ArgExpr*>    m_argExprs;
    FC_CHashStr<CG_DType*>     m_hashDType;
    FC_CHashPtr<const TCHAR*, CG_Literal*> 
                               m_hashLiterals;
    FC_CGArray<CG_STStmt*>     m_stmts;
    FC_CGArray<CG_STCaseList*> m_cases;

public:
    const CG_DType* pIntType;
    const CG_DType* pBoolType;
	/*const TODO*/
    /*const*/CG_ExprTree*    lit0;
    /*const*/CG_ExprTree*    lit1;
    /*const*/CG_ExprTree*    litTrue;
    /*const*/CG_ExprTree*    litFalse;

    CG_ExprMem(const CG_Edipos* pEdp, const CG_Symtab* pSymCurrPOU);
    ~CG_ExprMem();

    const TCHAR* registerString(
        const TCHAR* psz
    );
    const CG_Literal* registerLiteral(
        CG_DTYPES     type,
        const TCHAR*  psNormal,
        __int64       anyInt,
        double        anyReal
    );

    CG_ExprTree* registerCGExpr(
        CG_EXPRTYP       extyp, 
        const CG_Edipos* pEdp, 
        const CG_DType*  pType
    );

    const CG_DType* registerDType(
        CG_DTYPES dType, 
        long      lStrLen //-1 if no string or auto size string
                          // >=0 otherwise
    );

    const CG_SymEntry* registerSymEntry(
        const CG_Edipos* pEdp, 
        const TCHAR*     pszId, 
        const CG_DType*  pDType
    );

    const CG_ArgExpr* registerArgExpr(
        const CG_ExprTree* pExpr,
        const CG_SymEntry* pFormal
    );
    
    CG_STStmt* registerStmt();
    
    CG_STCaseList* registerCaseList(
        long lCase, 
        const CG_STStmt* pStmts
    );
   
    // return an optimized expression
    // it has a limited functionality needed for optimizing the for loop
    // it treats only GT, LT, GE, LE, AND, OR, UMINUS, SUB, ADD expressions
    const CG_ExprTree* optimizeExpr(
        const CG_ExprTree* pExpr
    );

    const  CG_ExprTree* newLiteral(
        const CG_Edipos* pEdp,
        const CG_DType* pType, 
        __int64 tmpVal
    );

    const CG_ExprTree* newVarRef(
        const CG_Edipos* pEdp, 
        const CG_DType* pType, 
        const TCHAR* pszVarName, 
        WORD accFlags
    );
};

#endif

/* ---------------------------------------------------------------------------- */
