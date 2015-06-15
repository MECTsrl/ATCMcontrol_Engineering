/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/exprcode.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: exprcode.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/exprcode.h $
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

#ifndef _EXPRCODE_H_
#define _EXPRCODE_H_

#include "defines.h"
#include "globctx.h"
#include "cgt_misc.h"
#include "cgt_CDiagMsg.h"
#include "cgt_tools.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_system.h"




//body dump helper structs and classes:
struct TMP_VAR_TYP
{
    bool            bUsed;
    int             iBitSize;
};

typedef FC_CGArray<long> LStack;

class CBodyInfo
{
public:
    CBodyInfo(FC_CGArray<CGT_BP_TYP>* pbpList, CGlobCtx* pgctx);
    ~CBodyInfo();
    int getFreeTmpVar(const CG_DType* pType, bool bRef);
    const CG_ExprTree* getTmpVarExpr(
        const CG_DType* pType, 
        const CG_Edipos* pEdp 
    );
    void freeTmpVar(int i);
    void freeTmpVarExpr(const CG_ExprTree* pExpr);
    void freeTmpVars();
    void addTmpVarExpr(const CG_ExprTree* pExpr)
    {
        m_listTmpExpr.add(pExpr);
        m_hashTmpExpr.insert(pExpr, NULL);
    }

    long GetNextLabelId() { return m_LabelId++; }

    FC_CGArray<TMP_VAR_TYP>                  m_tmpvars;
    FC_CGArray<CG_ExprTree*>                 m_tmpVarExpr;
    FC_CGArray<const CG_ExprTree*>           m_listTmpExpr;
    FC_CHashPtr<const CG_ExprTree*, char*>   m_hashTmpExpr;

    long                             m_LabelId;
    LStack                           m_LblStack;

    const CG_Edipos*                 m_pLastBPEdp; //last BP edipos
    long                             m_lastBPNum;  //current BP num (ready to use initialized with 2)
    FC_CGArray<CGT_BP_TYP>*          m_pBPlist;    //list of BPs, used to form the DBI files BP info

    long                             m_UniqueInlineNum;
};



struct BODY_CTX_INFO
{
    const CG_Resource*              pRes; //may be NULL if building a library
    CTarget*                        pTrg; //may be NULL if building a library
    unsigned long                   ulTrgVer; //may be NULL if building a library
    const CG_Symtab*                pSym;
    ICG_IECFront*                   pIF;
    CGT_CDiagMsg*                   pdm;
    ICG_Body*                       pIBdy;
    bool                            bTransi;
    bool                            bAction;
    bool                            bExceptionAction;
    CBodyInfo*                      pInf;
    CGlobCtx*                       pgctx;
    BP_OPTION                       BPoption;
};



void VMDmpStmt(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmts,
    const BODY_CTX_INFO* pctx
);


//CG_USEREXPR ExprTree node types:
#define UE_SFC_INSTANCE_ARRAY   1l  //use from union psz1,psz2
#define UE_TYPE_CAST            2l  //use from union psz1,psz2



void ConsiderBP(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmt,
    const BODY_CTX_INFO* pctx
);
//body dump functions, they get the context via 'pctx'
void DumpBPFun(
    CGT_CDmp&               dmp, 
    const BODY_CTX_INFO*    pctx,
    const CG_Edipos*        pEdp,
    int                     nBPNum,
    bool                    bNCCOnly
);

#endif

/* ---------------------------------------------------------------------------- */
