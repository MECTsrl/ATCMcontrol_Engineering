
#ifndef __IL_PARSECONTEXT_H_
#define __IL_PARSECONTEXT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "expr.h"
#include "CG_IECFront.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

enum IL_OP
{
    IL_OP_UNDEF, 
    // simple operators
    IL_LD,  IL_LDN,
    IL_ST,  IL_STN,
    IL_S,   IL_R,    IL_S1,   IL_R1,
    IL_CLK, 
    IL_CU,  IL_CD,   IL_PV,
    IL_IN,  IL_PT,
    // expression operators
    IL_NOT, 
    IL_AND, IL_ANDN,
    IL_OR,  IL_ORN,
    IL_XOR, IL_XORN,
    IL_ADD, IL_SUB,
    IL_MUL, IL_DIV,
    IL_MOD, 
    IL_GT,  IL_GE,   IL_EQ,    
    IL_LT,  IL_LE,   IL_NE,
    // dummy function call operator
    IL_FUNCALL,
    // FB call operators
    IL_CAL, IL_CALC, IL_CALCN,
    // jump operators
    IL_JMP, IL_JMPC, IL_JMPCN,
    // return operators
    IL_RET, IL_RETC, IL_RETCN,
    // ')' operator
    IL_RPAR,
    // create a special dummy instruction
    IL_LABEL
};

enum IL_INSTRTYP
{
    IL_INSTR_UNDEF,
    IL_SIMPLEOP,
    IL_EXPR,
    IL_JMPTYP,
    IL_CALFB,
    IL_CALFUN,
    IL_RETTYP,
	IL_LBLTYP
};


class IL_Instr;

struct IL_SimpleOperation
{
    ST_Expression_C* expr;
};

struct IL_Expression
{
    ST_Expression_C* subop;		// if instruction looks like 
								// inst (op1
								// then subexpr is op1 otherwise NULL
    IL_Instr*        instrList;
};

struct IL_Jump
{
    ST_Identifier_C* label;
};

struct IL_CallFB
{
    ST_Identifier_C* FBIName;
    ST_CParamList*   paramlist;
};

struct IL_CallFun
{
    ST_CParamList*   paramlist;
};

class IL_Instr
{
    friend class st_parse;
    friend class il_ParseContext;

    CG_Edipos        m_edp;
    IL_OP            m_op;
    FC_CStr          m_szName;
    
    IL_INSTRTYP      m_ityp;
    const IL_Instr*  pNext;
    
    union /*switch(m_ityp) */ 
    {
    /*case IL_SIMPLEOP:  */  IL_SimpleOperation simpleop;
    /*case IL_EXPR:      */  IL_Expression      expr;
    /*case IL_JMPTYP:    */  IL_Jump            jmp;
    /*case IL_CALFB:     */  IL_CallFB          fbc;
    /*case IL_CALFUN:    */  IL_CallFun         func;
    /*case IL_RETTYP:    */  //no special   data;
    /*case IL_LBLTYP:    */  //no special   data;
    };

    IL_Instr(const CG_Edipos* pEdp, IL_OP op, IL_INSTRTYP ityp, const TCHAR* name);
    ~IL_Instr();
};



/*
struct IL_Token
{
    int        tok;
    FC_CString text;
    CG_STEDP   edp;
};
*/
class il_ParseContext
{
    friend class st_ParseContext;
    friend class st_parse;
    st_ParseContext* m_pSTContext;
    IL_Instr*        m_Instrs;
    int              m_tokensafterCR;
    boolean          m_bParseBody;
    
    il_ParseContext(): m_Instrs(NULL), 
        m_tokensafterCR(0), m_bParseBody(true), m_pSTContext(NULL)
    {
    }
    ~il_ParseContext()
    {      
        
        delete(m_Instrs);
#if 0   //avoid stackoverflow, do not recursive delete p->pNext 
        //does this really work ? 
        for(const IL_Instr* p = m_Instrs; p; )
        {
            const IL_Instr* pNext = p->pNext;
            delete(p);
            p = pNext;
        }
#endif
    }
    int                getOperator(const TCHAR* pszToken);
    static IL_OP       getILOp(int op);
    static IL_INSTRTYP getInstType(IL_OP op);

    CG_STStmt*       transformToST();
    ST_Expression_C* makeExpression(ST_Expression_C* acc, const IL_Instr* pInstrs, 
                                   const IL_Instr** pLastInstr);
    ST_Expression_C* makeSimpleExpr(const IL_Instr* pInst, ST_Expression_C* acc,
                                   ST_Expression_C* op);
	ST_Expression_C* makeFunCall(const IL_Instr* pInst, ST_Expression_C* acc);
	bool isBeginOfSTStmt(const IL_Instr* pInst);
};

#endif // __IL_PARSECONTEXT_H_


