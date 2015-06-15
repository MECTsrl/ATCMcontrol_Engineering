#ifndef EXPRCODE_H_
#define EXPRCODE_H_

#include "defines.h"
#include "cgt_misc.h"
#include "cgt_CDiagMsg.h"
#include "cgt_tools.h"
#include "globctx.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_system.h"




//body dump helper structs and classes:
struct TMP_VAR_TYP
{
    bool            bUsed;
    const TCHAR*    pszType;
    const CG_DType* pType;
    bool            bRef;
    bool            bFCTmpObj;
};


class CBodyInfo
{
public:
    CBodyInfo(
        const CG_Symtab*        pSym,
        FC_CGArray<CGT_BP_TYP>* pbpList, 
        int                     bSampleUsed
    );
    ~CBodyInfo();

    //for tmp vars from the back end:
    int getFreeTmpVar(const CG_DType* pType);
    void freeTmpVar(int i);

    //for tmp vars from the front end:
    const TMP_VAR_TYP* newLDFBDTmpVar(const CG_ExprTree* pTmp, bool bNeedFCObj);

    void addBdyUsed(const CG_SymEntry* pEntry);

    FC_CGArray<TMP_VAR_TYP>  m_tmpvars;
    FC_CGArray<TMP_VAR_TYP>  m_ldfbdtmp;
    int                      m_maxFBinoutArray;
    bool                     m_bHasNative;
    int                      m_StackVarCount;
    bool                     m_bHasCASE;
    bool                     m_bHasGOTO;
    bool*                    m_pUsedVars; //!=NULL if used vars should be recorded
                                          //array has size pSym->nEntries.
    const CG_Symtab*         m_pSym;

    const CG_Edipos*         m_pLastBPEdp; //last BP edipos
    long                     m_lastBPNum;  //current BP num (ready to use initialized with 2)
    FC_CGArray<CGT_BP_TYP>*  m_pBPlist;    //list of BPs, used to form the DBI files BP info
};




struct BODY_CTX_INFO
{
    const CG_Symtab* pSym;
    ICG_IECFront*    pIF;
    CGT_CDiagMsg*    pdm;
    ICG_Body*        pIBdy;
    int              nTransi;  //>= 0: transition number (body of trans)
    int              nAction;  //>= 0: action number (body of action)
    BP_OPTION        BPoption; 
    CBodyInfo*       pInf;     
};

bool IsJavaBasicType(BP_OPTION bpOpt, const CG_SymEntry* pEntry);




//body dump functions, they get the context via 'pctx'
void DumpBPFun(
    CGT_CDmp&               dmp, 
    const CG_Edipos*        pEdp,
    const CG_Symtab*        pSym,
    FC_CGArray<CGT_BP_TYP>* pbpList,
    int                     nBPNum
);

void PC_DumpFunctionReturn(
    CGT_CDmp&            dmp, 
    const BODY_CTX_INFO* pctx,
    bool                 bDumpIfTrue //used to avoid a statement not reached error from java compiler
);

void PC_DumpSTStmts(
    CGT_CDmp&            dmp, 
    const CG_STStmt*     pStmts,
    const BODY_CTX_INFO* pctx
);

void PC_DumpCGAssign(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs,
    const BODY_CTX_INFO* pctx
);

//##AC beg
#define PC_USEREXPR_RAWJAVA  10
class PC_InlineArg
{
public:
    bool                m_bInput;
    const CG_SymEntry*  m_pFormal;
    CG_ExprTree         m_actual;
    FC_CStr             m_javaExpr;

    PC_InlineArg(const CG_Edipos* pEdp, bool bInput, const CG_SymEntry* pFormal, const TCHAR* pszJavaExpr) :
        m_bInput(bInput), m_pFormal(pFormal), m_javaExpr(pszJavaExpr)
    {
        m_actual.extyp      = CG_USEREXPR;
        m_actual.pType      = pFormal->pType;
        m_actual.edp        = *pEdp;
        m_actual.usr.ldata  = PC_USEREXPR_RAWJAVA;
        m_actual.usr.psz1   = m_javaExpr;
    }
};

void PC_DumpFBCall(
    const BODY_CTX_INFO*        pctx,
    CGT_CDmp&                   dmp, 
    const CG_ExprTree*          pInst,     //strict IEC: always a CG_VARREF, future 
                                           //version may support array of FBs
    long                        nArgs,     //>=0 number of in/out assignments
    const CG_ArgExpr*           pArgs,     //NULL or array of arg expr.
    const PC_InlineArg*const    pInlArgs[],//optinal extra in/out arguments form marco call
    int                         nInlArgs   //sizeof pInlArgs
);
//##AC end


void PC_DumpCGExprRef(
    CGT_CDmp&             dmp, 
    const CG_ExprTree*   pExpr,
    const BODY_CTX_INFO* pctx
);


void PC_DumpCGExprVal(
    CGT_CDmp&            dmp, 
    const CG_ExprTree*   pExpr,
    const BODY_CTX_INFO* pctx
);


void PC_DumpOperator(
    CGT_CDmp&            dmp,
    const BODY_CTX_INFO* pctx,
    CG_DTYPES            lTypeSet,
    CG_EXPRTYP           opEnum,
    const CG_ExprTree*   pLhs,
    const CG_ExprTree*   pRhs
);




//very special java helpers:
//prefixes for generated variables:
#define JAVA_CASE_TMP_VAR        _T("_CaseTmp")
#define JAVA_TMPVAR_PREFIX       _T("_JT")
#define JAVA_LDFBD_TMPVAR_PREFIX _T("_T")
#define JAVA_EDGE_NAME_PREFIX    _T("_ED")
#define JAVA_TRIG_PREFIX         _T("_TRIG_")
#define JAVA_GOTO_STATE_VAR      _T("_Goto")
#define JAVA_GOTO_FOR            _T("_GotoFor")
#define JAVA_INOUT_ARRAY         _T("_Inouts")
#define JAVA_FUNCTION_PRE        _T("_F_")
#define JAVA_TRIGTMP_PRFIX       _T("_TT")

//helpfull defines for dumping java:
//if you are not using CRLO,CRLC you can't 
//use the edit function 'find matchig brace' either.
#define CRLO "{"
#define CRLC "}"

inline CGT_CDmp& JavaId(CGT_CDmp& dmp, const TCHAR* pszId)
{
   return (dmp<<_T('_')).lo(pszId);
}


void PC_DumpJavaInitValue(
    CGT_CDmp&         dmp, 
    const CG_Literal* pLit
);

void PC_DumpJavaStackTmpVarName(CGT_CDmp& dmp, int iNum);
void PC_DumpLDFBDStackTmpVarName(CGT_CDmp&dmp, int iNum);
inline long PC_GetObjNumOfEntry(const CG_SymEntry* pEntry)
{
    //definition of CG_Symtab::lInstNum this is exactly what we need
    assert(pEntry->lInstNum>=0);
    return pEntry->lInstNum;
}


const TCHAR* GetFcInterfaceNameFromType(const CG_DType* pType);
const TCHAR* GetFcTypeNameFromType(const CG_DType* pType);
const TCHAR* GetPutFunctionFromType(CG_DTYPES typ);
const TCHAR* GetGetFunctionFromType(CG_DTYPES typ);
const TCHAR* GetJavaBasicTypeName(
    const CG_DType* pType, 
    bool            bReference
);
const TCHAR* ST_GetFunTempVarName(const CG_DType* pType);



#endif