#ifndef EXPR_H_
#define EXPR_H_

#include "helper.h"
#include "typeset.h"
#include "literals.h"

class ST_CExprMem;
class ST_Literal_C;
class ST_Variable_C;
class ST_Expression_C;
class ST_StructAccess_C;
class ST_TmpVar_C;
class ST_StepAction_C;
class ST_CSymtabPrv;
class st_ParseContext;

struct ST_BINVAL_TYP;
struct ST_FUNCTION_DEF_TYP;



void CG_SetAccFlags(
    CGT_CDiagMsg*       pdm,
    const CG_ExprTree*  pExpr, 
    WORD                accFlags //CG_EXPR_ACC_R|W|.. bits
);







class ST_CParseTree 
{
private:
   ST_CParseTree*   m_pNext;//TODO beter use central free no ptr list..
   ST_CParseTree*   m_pPrev;
   st_ParseContext* m_pfc_link;
protected:
    st_ParseContext* m_pfc;
    CG_Edipos        m_edp;  
    CG_CIECFront*    m_pFront;
    CGT_CDiagMsg*    m_pdm;
public:

    ST_CParseTree(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc
    );
    virtual ~ST_CParseTree();

    const CG_Edipos* getEdp()const{return &m_edp;}
    st_ParseContext* getPfc()const{return m_pfc;}
    void Detach();
};


class ST_Expression_C: public ST_CParseTree 
{
protected:
    ST_TypeSet_C typeset;
public:
    ST_Expression_C(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc
    ) : ST_CParseTree(pEdp, pfc)
    {}

    //general methods:
    const CG_DType* getCGType()const;
    virtual void Propagate(const ST_TypeSet_C &what);
    void fixType();
    bool checkTypedExpr(
        CG_BITSET  lTypeSet, 
        bool       bErrMsg
    );

    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
    CG_ExprTree* initCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady, 
        CG_EXPRTYP extyp
    )const;

    virtual int  is_Lvalue() const;
    
    bool isAny64Literal(__int64* pValue = 0) const;
    bool isAnyIntLiteral(__int64* pValue = 0) const
    {
        return (typeset.GetTypeSet() & CG_TBS_ANY_INT) && isAny64Literal(pValue);
    }
    bool isBoolVal(bool* pValue = NULL)const;

    virtual const ST_BINVAL_TYP* getLitValue(
        const CG_SymEntry** ppConst=NULL
    )const;
    const ST_TypeSet_C& GetType()const{ return typeset; }
    const ST_TypeSet_C* getTypePtr()const{ return &typeset; }
    virtual ST_Variable_C* toVariableClass();
    virtual ST_StructAccess_C* toStructAccess();
    virtual ST_Literal_C* toLiteralClass();
    virtual ST_StepAction_C* toStepActionClass();
    virtual bool isErrorExpr();
    virtual bool isTmpVar()const;
    virtual bool isReferenceTmpVar()const;
    virtual const CG_SymEntry* getSymEntry()const;
    virtual const TCHAR* getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const;//##AC
    virtual bool needsTmpVar()const;
    virtual bool needsParenthesis()const;
    virtual bool isArrayInit();      

    bool checkInit(
        const CG_DType* pType, 
        CG_Initialiser* pRet
    );
    virtual bool checkInit2(
        const CG_DType* pType, 
        CG_Initialiser* pRet, 
        long* pCount
    );
};


class ST_ErrorExpr_C: public ST_Expression_C 
{
public:
    ST_ErrorExpr_C(
        const CG_Edipos*    pEdp, 
        st_ParseContext*    pfc, 
        const ST_TypeSet_C& type
    );

    virtual int  is_Lvalue()const;
    virtual bool isErrorExpr();
    const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};

class ST_TmpVar_C: public ST_Expression_C 
{
    int                 m_iTmpVarNum;
    const CG_ExprTree*  m_pReferencedExpr;
    WORD                m_tmpTyp; //additional flags see CG_TMP_R_TRIG,..
public:
    ST_TmpVar_C(
        const CG_Edipos*    pEdp, 
        st_ParseContext*    pfc, 
        int                 iTmpVarNum, 
        const ST_TypeSet_C& dtype, 
        const CG_ExprTree*  pReferencedExpr,
        WORD                tmpTyp
    );

    virtual int  is_Lvalue()const;
    virtual bool isTmpVar()const;
    virtual bool isReferenceTmpVar()const;

    const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
    virtual bool needsTmpVar()const;
    virtual bool needsParenthesis()const;

    virtual int  getTmpVarNum()const;
};



//TODO const entry and lit treatement !
class ST_Literal_C: public ST_Expression_C 
{
    bool               m_bIsPureLit;
    ST_BINVAL_TYP*     m_pBinVal;
    const CG_SymEntry* m_pConstEntry;

public:
    ST_Literal_C(
        const CG_Edipos*    pEdp, 
        st_ParseContext*    pfc,
        ST_BINVAL_TYP*      pBinVal,
        const ST_TypeSet_C& type,
        bool                bIsPureLit,
        const CG_SymEntry*  pConstEntry
    );
    ~ST_Literal_C();

    virtual const ST_BINVAL_TYP* getLitValue(
        const CG_SymEntry** ppConst=NULL
    )const;
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem*     pAlloc,
        CG_ExprTree*     pReady
    )const;
    virtual bool needsTmpVar()const;
    virtual bool needsParenthesis()const;
    virtual ST_Literal_C* toLiteralClass();
    bool isPureLiteral()const{return m_bIsPureLit;}
};


class ST_Identifier_C: public ST_CParseTree 
{
    FC_CStr szName;
public:
    unsigned long m_FilePos;
    ST_Identifier_C(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc, 
        const TCHAR*     szIdentifier)
        : ST_CParseTree(pEdp, pfc), 
          szName(szIdentifier),
          m_FilePos(0)
    {}
    const TCHAR* getName()const{return szName;}
};

class ST_DirectVar_C: public ST_Expression_C 
{
private:
    int         Lvalue;
    FC_CStr     szAddr;
    bool        bSyntaxOK;
public:
    ST_DirectVar_C(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc, 
        const TCHAR*     pszDirectAddr
    );
    int is_Lvalue() const;
    const TCHAR* getAddr()const{return szAddr;}
    bool isSyntaxOK()const{return bSyntaxOK;}
};


class ST_Variable_C: public ST_Expression_C 
{
private:
    const CG_SymEntry* m_pEntry;
    bool               m_bLvalue;
public:
    ST_Variable_C(
        const CG_Edipos*    pEdp, 
        st_ParseContext*    pfc,
        bool                bLvalue, 
        const CG_SymEntry*  pEntry
    );

    int is_Lvalue() const;
    virtual ST_Variable_C* toVariableClass();
    virtual const CG_SymEntry* getSymEntry()const;
    virtual bool needsParenthesis()const;
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};

class ST_StepAction_C: public ST_Expression_C 
{
private:
    bool            m_bLvalue;
    bool            m_bIsAction;
    CG_SFCEXPR_TYP  m_iFlagTyp;
    int             m_iNr;
    FC_CString      m_id;
    
public:
    ST_StepAction_C(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc, 
        int              stepActionNr,
        const TCHAR*     pszId,
        bool             isAction
    );

    virtual ST_StepAction_C* toStepActionClass();
    virtual bool needsParenthesis()const{return false;}
    void setFlagTyp(ST_Identifier_C* pId);

    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};

//##AC end
class ST_ExpressionList_C: public ST_CParseTree 
{
public:
    FC_CGArray<ST_Expression_C*> m_list;

    ST_ExpressionList_C(
        st_ParseContext*     pfc,
        ST_Expression_C*     pFirst
    );
    ~ST_ExpressionList_C();
};
//##ACC beg
class ST_CCaseExpr : public ST_CParseTree
{
public:
    FC_CGArray<CG_Subrange> m_rlist;

    ST_CCaseExpr(
         st_ParseContext*pfc,
        ST_Expression_C* pLo,
        ST_Expression_C* pUp //if NULL not a range just a simple case
    );

    void addRange(
         st_ParseContext*pfc,
        ST_Expression_C* pLo,
        ST_Expression_C* pUp //if NULL not a range just a simple case
    );

};

//##AC beg
class ST_CLabelRef : public ST_Expression_C
{
    FC_CStr m_label;
public:
    ST_CLabelRef(const CG_Edipos* pEdp, st_ParseContext* pfc, const TCHAR* pszLabel)
        : ST_Expression_C(pEdp, pfc), m_label(pszLabel) 
    {
        typeset = CG_TBS_GOTOLABEL;
    }
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};


struct ST_PARAMSPEC
{
    ST_Expression_C*  pActual;  //never NULL
    const TCHAR*      pszFormal;//NULL if position arglist
    CG_Edipos         edpFormal;//edp of formal if any, else memset to 0
    bool              bOut;     //true if =>
    bool              bNeg;     //true if negated out/in (only LD/FBD FB call)

    //set later if user fun/FB:
    bool               bOutputArg;
    const CG_SymEntry* pFormal;
};
class ST_CParamList: public ST_CParseTree 
{
public:
    FC_CGArray<ST_PARAMSPEC> m_list;
    bool                     m_bNamed;

    ST_CParamList(const CG_Edipos* pEdp, st_ParseContext* pfc, int nInitialLen) 
        : ST_CParseTree(pEdp, pfc), m_list(nInitialLen), m_bNamed(false)
    { 
    }
    ~ST_CParamList()
    {
        for(int i=m_list.getUsed()-1; i>=0; i--)
            delete(m_list.getAddrOf(i)->pActual);
    }
    void addNamedParam(const CG_Edipos* pEdpFormal, const TCHAR* pszFormal, bool bOut, bool bNeg, ST_Expression_C* pActual)
    {
        ST_PARAMSPEC* ps = m_list.addZero();
        pActual->Detach();

        ps->pActual   = pActual;
        ps->pszFormal = m_pFront->registerString(pszFormal);
        ps->edpFormal = *pEdpFormal;
        ps->bOut      = bOut;
        ps->bNeg      = bNeg;
        m_bNamed = true;
    }
    void addPosParam(ST_Expression_C* pActual)
    {
        pActual->Detach();
        m_list.addZero()->pActual = pActual;
    }
    void addFirstParam(ST_Expression_C* pFirst)
    {
        pFirst->Detach();
        m_list.addZero();
        for(int i=m_list.getUsed()-1; i>0; i--)
            m_list.set(i, m_list.get(i-1));
        
        m_list.getAddrOf(0)->pActual = pFirst;
        m_list.getAddrOf(0)->pszFormal = NULL;
        m_list.getAddrOf(0)->bOut = false;
        m_list.getAddrOf(0)->bNeg = false;
    }

    bool hasErrorExpr()
    {
        for(int i=m_list.getUsed()-1; i>=0; i--)
        {
            if(m_list.getAddrOf(i)->pActual->isErrorExpr())
                return true;
        }
        return false;
    }
};




class ST_CFunction: public ST_Expression_C 
{
private:
    const ST_CSymtabPrv*       m_pSymPrv;  //definition of function
    FC_CGArray<ST_PARAMSPEC>   m_actuals;
    int*                       m_pPropIdx; //!=NULL if there are arguments with same ANY_* as result in Propagate
    bool                       m_bCheckOK; //set true if no errors


    void noImplemErrMsg()const;

public:
    ST_CFunction(
        const CG_Edipos*           pEdp,
        st_ParseContext*           pfc,
        const ST_CSymtabPrv*       pSymPrv,
        ST_CParamList*             pActuals
    );
    ~ST_CFunction();

    bool checkCall();
    virtual void Propagate(const ST_TypeSet_C &what);

    virtual const TCHAR* getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const;

    void getArgListAndDel(FC_CGArray<ST_PARAMSPEC>* pDest)
    {
        m_actuals.ownmemto(pDest);
        delete(this);
    }
    
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};

class ST_CFBCallExpr : public ST_Expression_C
{
    const CG_SymEntry*      m_pFBIEntry;   //only set if FB call
    FC_CGArray<ST_PARAMSPEC>m_parlist;     //param list or NULL if no params
    long                    m_instAccFlags;//final access flags for inst var updated in addParam
    FC_CGArray<CG_ArgExpr>  m_CGArglist;   //build in checkFBCall:

    bool addParam(const CG_Symtab* pSymFB, const ST_PARAMSPEC* pPS, FC_CGArray<const CG_SymEntry*>& inoutsAssigned);

public:

    ST_CFBCallExpr(
        const CG_Edipos*    pEdp,  
        st_ParseContext*    pfc,
        const CG_SymEntry*  pFBIEntry,
        ST_CParamList*      pParList
    );
    ~ST_CFBCallExpr();

    bool checkFBCall();

    virtual int is_Lvalue()const;
    virtual const TCHAR* getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const;
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};
//##AC end


class ST_ArrayAccess_C : public ST_Expression_C 
{
private:
    ST_Expression_C*     basevariable;  // an expression of array type
    ST_ExpressionList_C* subscripts;
public:
    ST_ArrayAccess_C(
        st_ParseContext*     pfc,
        ST_Expression_C*     pVar, 
        ST_ExpressionList_C* pSublist
    );
    ~ST_ArrayAccess_C();

    int is_Lvalue() const;
    virtual const CG_SymEntry* getSymEntry()const;
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};

class ST_StructAccess_C: public ST_Expression_C 
{
private:
    ST_Expression_C*   m_basevariable;
    bool               m_bLvalue;   // = basevariable->is_Lvalue() && !CONST(field_selector)
    const CG_SymEntry* m_pEntry;    //<-sym tab entry of the field selector in the 
                                      //symtab of the basevariable's type (FB or Struct!)
    
public:
    ST_StructAccess_C(
        st_ParseContext* pfc,
        ST_Expression_C* pStructVar, 
        ST_Identifier_C* pSelector
    );
    ~ST_StructAccess_C();

    virtual const CG_SymEntry* getSymEntry()const;
    int is_Lvalue() const;
    virtual ST_StructAccess_C* toStructAccess();
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};

class ST_Function_C: public ST_Expression_C 
{
private:
    int*                       lpArgTypes; // useful for Propagate()
    const ST_CSymtabPrv*       m_pSymOfFun;//definition of function, 
                                           //NULL if Operator or buildin
    CG_EXPRTYP                 m_opEnum;   //set if op or is CG_BUILDIN if buildin
    
    FC_CGArray<ST_PARAMSPEC>   m_actuals; //##AC
    
    const ST_FUNCTION_DEF_TYP* m_pOverloads;
    void dumpNoImplementationFoundErrMsg()const;
public:
    ST_Function_C(
        const CG_Edipos*           pEdp,
        st_ParseContext*           pfc,
        CG_EXPRTYP                 opEnum, 
        ST_CParamList*             pArglist,
        const ST_CSymtabPrv*       pSymOfFun,
        const ST_FUNCTION_DEF_TYP* pOverloads
    );
    ~ST_Function_C();
    void getArgListAndDel(FC_CGArray<ST_PARAMSPEC>* pDest)
    {
        m_actuals.ownmemto(pDest);
        delete(this);
    }

    bool selectOverload();
    const TCHAR* getFunName()const{return m_pOverloads->pszId;}
    void Propagate(const ST_TypeSet_C &what);

    virtual const TCHAR* getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const;//##AC
    virtual const CG_ExprTree* makeCGExpr(
        ST_CExprMem* pAlloc,
        CG_ExprTree* pReady
    )const;
};


class ST_StructInit_C : public ST_Expression_C
{
    FC_CGArray<ST_Identifier_C*> m_Members;
    FC_CGArray<ST_Expression_C*> m_Inits;
public:   
    ST_StructInit_C(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc,
        ST_Identifier_C* pMember, 
        ST_Expression_C* pInitExpr
    );
    ~ST_StructInit_C();
    
    void append(
        ST_Identifier_C* pMember, 
        ST_Expression_C* pInitExpr
    );
    virtual bool checkInit2(
        const CG_DType* pType, 
        CG_Initialiser* pRet, 
        long*           pCount
    );
};

class ST_ArrayInit_C : public ST_Expression_C
{
    FC_CGArray<ST_Expression_C*>  m_Repeat;
    FC_CGArray<ST_Expression_C*>  m_Inits;
public:   
    ST_ArrayInit_C(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc,
        ST_Expression_C* pRepeat, 
        ST_Expression_C* pInitExpr
    );
    ~ST_ArrayInit_C();
    
    void append(
        ST_Expression_C* pRepeat, 
        ST_Expression_C* pInitExpr
    );
    virtual bool isArrayInit();      
    virtual bool checkInit2(
        const CG_DType* pType, 
        CG_Initialiser* pRet, 
        long*           pCount);
};




//other non expression related temp parse tree objects:
class ST_CIdentList : public ST_CParseTree
{
public:
    FC_CGArray<ST_Identifier_C*> m_ids;

    
    ST_CIdentList(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc
    ) : ST_CParseTree(pEdp, pfc)
    {}
    ~ST_CIdentList();
    const ST_Identifier_C* addIdOnce(ST_Identifier_C* pId, HRESULT ew);
};

class ST_CAttrList : public ST_CParseTree
{
public:
    FC_CGArray<ST_Identifier_C*>   m_attrtag;
    FC_CGArray<ST_Identifier_C*>   m_attrval;

    ST_CAttrList(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc
    ) : ST_CParseTree(pEdp, pfc)
    {}
    ~ST_CAttrList();
    
    void addAttr(ST_Identifier_C* pTag, ST_Identifier_C* pVal);
    CG_AttribList* convertToStruct()const;
};

class ST_CTypeOrList : public ST_CParseTree
{
public:
    ST_TypeSet_C    m_pArgTypeSets[ST_MAX_FUNCTION_PARAMS];
    const CG_DType* m_optCastToType[ST_MAX_FUNCTION_PARAMS];
    int             m_iArgCount;
    bool            m_bExtended;

    ST_CTypeOrList(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc
    ) : ST_CParseTree(pEdp, pfc)
    {
        m_iArgCount = 0;
        m_bExtended = false;
    }
    void addType(const CG_Edipos* pEdp, const ST_TypeSet_C& type, const CG_DType* toCast);
};


class ST_CDeclList : public ST_CParseTree
{
    FC_CGArray<ST_Identifier_C*> m_ids;
    FC_CGArray<ST_CAttrList*>    m_attr;
public:
    ST_CDeclList(
        const CG_Edipos* pEdp, 
        st_ParseContext* pfc
    ) : ST_CParseTree(pEdp, pfc)
    {}
    ~ST_CDeclList()
    {
        m_ids.deletePtrs();
        m_attr.deletePtrs();
    }
    void addIdAttr(ST_Identifier_C* pId, ST_CAttrList* pAttr)
    {
        pId->Detach();
        if(pAttr)
            pAttr->Detach();
        m_ids.add(pId);
        m_attr.add(pAttr);
    }
    int getLen()const
    {
        return m_ids.getUsed();
    }
    const ST_Identifier_C* getIdAndAttr(
        int i, 
        const ST_CAttrList** ppAttrList
    )const
    {
        if(ppAttrList)
            *ppAttrList = m_attr.get(i);
        return m_ids.get(i);
    }
};

class ST_CTaskInits : public ST_CParseTree
{
public:
    FC_CGArray<CG_TaskInit> m_inits;

    ST_CTaskInits(
        st_ParseContext* pfc,
        ST_CExprMem*     pAlloc,
        ST_Identifier_C* pAttr, 
        ST_Expression_C* pVal
    );
    void addPair(
        st_ParseContext* pfc,
        ST_CExprMem*     pAlloc,
        ST_Identifier_C* pAttr, 
        ST_Expression_C* pVal
    );
    
};

class ST_CTaskList : public ST_CParseTree
{
    FC_CGArray<CG_Task> m_tasks;
    FC_CGArray<FC_CGArray<const CG_SymEntry*>*> m_prgs;
public:

    ST_CTaskList(
        st_ParseContext* pfc,
        ST_Identifier_C* pId,
        ST_CAttrList*   pAttr,
        ST_CTaskInits*  pInits
    );     
    ~ST_CTaskList()
    {
        m_prgs.deletePtrs();
    }

    void addTask(
        st_ParseContext* pfc,
        ST_Identifier_C* pId,
        ST_CAttrList*   pAttr,
        ST_CTaskInits*  pInits
    );     
    int findTaskByName(
        const TCHAR* pszTaskName
    )const;
    bool addPrg(
        const TCHAR*       pszTaskName, 
        const CG_SymEntry* pEntry
    );
    int countTasks()const{return m_tasks.getUsed();}
    CG_Task* convertToStruct();
};





#endif // EXPR_H_
