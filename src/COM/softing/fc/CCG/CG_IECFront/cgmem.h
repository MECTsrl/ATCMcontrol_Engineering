#ifndef __CGMEM_H_
#define __CGMEM_H_

#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_hash.h"



class ST_CExprMem
{
    FC_CHashStrTable           m_strings;
    FC_CGArray<CG_ExprTree*>   m_exprs; //TODO make performance allocator !!!
    FC_CGArray<CG_ArgExpr*>    m_argexprlists;
    FC_CHashPtr<const TCHAR*,CG_Literal*> 
                               m_hashLiterals;
public:
    ST_CExprMem();
    ~ST_CExprMem();

    const TCHAR* registerString(
        const TCHAR* psz
    );
    const CG_Literal* registerLiteral(
        CG_DTYPES     type,
        const TCHAR*  psNormal,
        __int64       anyInt,
        double        anyReal
    );
    const CG_Literal* registerStringLiteral(
        CG_DTYPES     type,
        const TCHAR*  ps       // IEC format
    );
    CG_ExprTree* registerCGExpr(
        CG_EXPRTYP       extyp, 
        const CG_Edipos* pEdp, 
        const CG_DType*  pType
    );
    CG_ExprTree* registerCGExprArray(int nLen);
    const CG_ArgExpr* registerArgExprList(
        CG_ArgExpr* pArgList
    );

    
    //get info of var use in body:
    int countExpr()const{return m_exprs.getUsed();}

    void getVarExtWrtList(
     FC_CHashPtr
        <const CG_SymEntry*, 
        FC_CGArray<CG_Edipos>*>* pHash
    )const;
};







//------------------------------------------------------------------*
/**
 *  class ST_CBody: used to hold the memory of a body
 *
 *  @see    ....
*/
class ST_CBody 
{
    FC_CGArray<CG_STStmt*>     m_stmts;         //holds the memory 
    FC_CGArray<CG_STCaseList*> m_caseLists;     //holds the memory
    FC_CGArray<CG_EdpEntry*>   m_entryRefLists; //holds the memory

    const CG_STStmt*           m_pFirstStmt;    //final result for non SFC body
                                                //usually points to the first 
                                                //array elem of m_stmts
                                                //will be NULL for SFC body
    
    //only used if SFC body:
    //These arrays hold const pointers for the first stmt of a 
    //trans/action which are certain elements of m_stmts.
    //the second,third,... stmts of a trans/action are reached 
    //by the CG_STStmt::pNext (and possibly some simple if then endif)
    //pointer.
    bool                         m_bIsSFC;
    CG_SFCStruct*                m_pSFCS;      //sfc topology info
    FC_CGArray<const CG_STStmt*> m_actions;    //[0..NumActions-1]
    FC_CGArray<const CG_STStmt*> m_transitions;//[0..NumTrans-1]
    const CG_STStmt*             m_pExAction;  //statement list of the exception action


    
    int                          m_usedTriggers; //LD coil triggers, must be 
                                                 //counted up even accross 
                                                 //SFC TRans/Actions
public:
    //public members (for convenience)
    ST_CExprMem      m_exprMem; //hold memory of all exprs
    CG_Edipos        m_edpBeg;  //body begin edp (needed for special diags)
    CG_Edipos        m_edpEnd;  //body end edp   (needed for special diags)
    FC_CHashPtr<const CG_Symtab*, int> m_callFun;//remember all functions called

    ST_CBody();
    ~ST_CBody();

    //virtually only for assertions:
    bool isUnused()const{return m_stmts.getUsed()==0;}
    
    CG_STStmt*         newStmt(CG_STMTTYP st, const CG_Edipos* pEdp);
    CG_STCaseList*     registerCaseList(CG_STCaseList* pcl);
    const CG_EdpEntry* registerEntryRefList(const CG_EdpEntry* pList, int nLength);

    //non SFC:
    void setSTBody(const CG_STStmt* pStmt);
    
    //SFC:
    void setSfcBody();
    void setSFCStruct(CG_SFCStruct* pSFCS);
    void setTransiBody(const CG_STStmt* pStmt);
    void setActionBody(const CG_STStmt* pStmt);
    void setExceptionActionBody(const CG_STStmt* pStmt);



    bool isSFC()const{return m_bIsSFC;}
    //non SFC:
    const CG_STStmt* getStmts()const;
    //SFC:
    const CG_SFCStruct* getSFCStruct()const;
    const CG_STStmt* getTransi(long nTransi)const;
    const CG_STStmt* getAction(long nAction)const;
    const CG_STStmt* getExceptionAction()const;
    const CG_STStmt*const* getAllTransi(int* pnTransi)const;
    const CG_STStmt*const* getAllAction(int* pnActions)const;

    
    int  getUsedTriggers()const{return m_usedTriggers;}
    int incUsedTriggers(){return m_usedTriggers++;}
};



#endif // __CGMEM_H_


