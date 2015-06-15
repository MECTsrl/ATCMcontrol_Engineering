

//----  Local Defines:   -------------------------------------------*
#define FB_SR		_T("SR")
#define FB_RS		_T("RS")
#define FB_F_TRIG	_T("F_TRIG")
#define FB_R_TRIG	_T("R_TRIG")
#define FB_CTU		_T("CTU")
#define FB_CTD		_T("CTD")
#define FB_CTUD		_T("CTUD")
#define FB_TP		_T("TP")
#define FB_TON		_T("TON")
#define FB_TOF		_T("TOF")

//----  Includes:   -------------------------------------------*
#include "il_ParseContext.h"
#include "st_ParseContext.h"

//----  Static Initializations:   ----------------------------------*


IL_Instr::IL_Instr(const CG_Edipos* pEdp, IL_OP op, IL_INSTRTYP ityp, const TCHAR* name)
: m_edp(*pEdp), m_op(op), m_ityp(ityp), pNext(NULL), m_szName(name)
{ 
    switch(m_ityp)
    {
    case IL_SIMPLEOP:
        simpleop.expr  = NULL;
        break;
    case IL_EXPR:
        expr.subop     = NULL;
        expr.instrList = NULL;
        break;
    case IL_JMPTYP:
        jmp.label      = NULL;
        break;
    case IL_CALFB:
        fbc.FBIName   = NULL;
        fbc.paramlist = NULL;
        break;
    case IL_CALFUN:
        func.paramlist = NULL;
        break;
    case IL_RETTYP:
    case IL_LBLTYP:
        break;
    default:
        assert("invalid IL instuction type");
    }
}

IL_Instr::~IL_Instr()
{
    switch(m_ityp)
    {
    case IL_EXPR:
        if(expr.instrList) delete expr.instrList;
        break;
    case IL_SIMPLEOP:
    case IL_JMPTYP:
    case IL_CALFB:
    case IL_CALFUN:
    case IL_RETTYP:
    case IL_LBLTYP:
        break;
    default:
        assert("invalid IL instuction type");
    }
 
    if(pNext) //avoid stackoverflow do not recurise delete
        delete pNext;
}

IL_INSTRTYP il_ParseContext::getInstType(IL_OP op)
{
    if(op >= IL_LD  && op <= IL_PT)     return IL_SIMPLEOP;
    if(op >= IL_NOT && op <= IL_NE)     return IL_EXPR;      /* suppose expression; can be also simple operation */
    if(op == IL_FUNCALL)                return IL_CALFUN;
    if(op >= IL_CAL && op <= IL_CALCN)  return IL_CALFB;
    if(op >= IL_JMP && op <= IL_JMPCN)  return IL_JMPTYP;
    if(op >= IL_RET && op <= IL_RETCN)  return IL_RETTYP;
    return IL_INSTR_UNDEF;
}

struct STStmtList{
   CG_STStmt*  plistbegn;
   CG_STStmt*  plistlast;
};

void ST_StmtList_insert(STStmtList* pStmtList, CG_STStmt* pStmt)
{
    assert(pStmtList);
    if(pStmt)
    {
        assert(pStmt->pNext == NULL);
        if(pStmtList->plistbegn)
        {
            assert(pStmtList->plistlast);
            pStmtList->plistlast->pNext = pStmt;
        }
        else
        {
            pStmtList->plistbegn = pStmt;
        }
        pStmtList->plistlast = pStmt;
    }
}


CG_STStmt* il_ParseContext::transformToST()
{
    if(!m_pSTContext->m_pBody)
        return NULL;

    ST_Expression_C* acc         = NULL;
    ST_Expression_C* op          = NULL;
    STStmtList       StmtList    = { NULL, NULL };
    CG_STStmt*       pStmt       = NULL;
    const IL_Instr*  pInst       = m_Instrs;
	const IL_Instr*  pTempInst   = NULL;		
	int			     tmp_acc_num = 0;
    ST_CParamList*   pParList;
    ST_Expression_C* pCallExpr;
    
        
    while(pInst)
    {

        switch(pInst->m_ityp)
        {
        case IL_SIMPLEOP:
            op = pInst->simpleop.expr;
            switch(pInst->m_op)
            {
            case IL_STN:
            case IL_ST:
				if(!acc)
					m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if(pInst->pNext && !isBeginOfSTStmt(pInst->pNext))
					{
						ST_TmpVar_C* tmp_acc;
						CG_BITSET    tbs;

						tbs = op->GetType().GetTypeSet();

						if(tmp_acc_num != 0)
							m_pSTContext->freeTmpVar(tmp_acc_num);
						tmp_acc_num = m_pSTContext->getFreeTmp(tbs, false);

						tmp_acc = new ST_TmpVar_C(&pInst->m_edp, m_pSTContext, tmp_acc_num, tbs, 0, 0);

	                    pStmt = m_pSTContext->newAssignStmt(NULL, tmp_acc, acc);
						ST_StmtList_insert(&StmtList, pStmt);

						acc = new ST_TmpVar_C(&pInst->m_edp, m_pSTContext, tmp_acc_num, tbs, 0, 0);

						if(pInst->m_op == IL_STN)
							acc = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, acc, NULL);

	                    pStmt = m_pSTContext->newAssignStmt(&pInst->m_edp, op, acc);						
						ST_StmtList_insert(&StmtList, pStmt);
						
						acc = new ST_TmpVar_C(&pInst->m_edp, m_pSTContext, tmp_acc_num, tbs, 0, 0);
					}
					else
					{
						if(pInst->m_op == IL_STN)
							acc = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, acc, NULL);
	                    pStmt = m_pSTContext->newAssignStmt(&pInst->m_edp, op, acc);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
						if(tmp_acc_num != 0)
						{
							m_pSTContext->freeTmpVar(tmp_acc_num);
							tmp_acc_num = 0;
						}
					}
				}
				break;
            case IL_LDN:
                op = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, op, NULL);
            case IL_LD:
                if(acc)
                {
                    m_pSTContext->getDM()->msg0(CG_W_IL_ACC_NOT_SAVED, &pInst->m_edp);
                    delete acc;
                }
                acc = op;
                break;
			case IL_S1:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if(op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_SR))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_SR, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
			case IL_R:
				if(!acc)
					m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if (op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_SR)  ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTU) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTUD))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
					{
						ST_BINVAL_TYP* pBinVal = new ST_BINVAL_TYP;
						ST_InitBinValI64(pBinVal, 0);

						ST_Literal_C*  pFalseLit = new ST_Literal_C(&pInst->m_edp, m_pSTContext, pBinVal, CG_TBS_BOOL, true, NULL);

						pStmt = m_pSTContext->newAssignStmt(&pInst->m_edp, op, pFalseLit);
						pStmt = m_pSTContext->newIfElseStmt(&pInst->m_edp, acc, pStmt, NULL);				
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
				}
				break;
			case IL_S:
				if(!acc)
					m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if(op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_RS))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
					{
						ST_BINVAL_TYP* pBinVal = new ST_BINVAL_TYP;
						ST_InitBinValI64(pBinVal, 1);

						ST_Literal_C*  pTrueLit = new ST_Literal_C(&pInst->m_edp, m_pSTContext, pBinVal, CG_TBS_BOOL, true, NULL);

						pStmt = m_pSTContext->newAssignStmt(&pInst->m_edp, op, pTrueLit);
						pStmt = m_pSTContext->newIfElseStmt(&pInst->m_edp, acc, pStmt, NULL);				
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
				}
				break;
			case IL_R1:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if(op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_RS))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_RS, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
			case IL_CLK:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if( op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_F_TRIG) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_R_TRIG))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_F_TRIG _T("|") FB_R_TRIG, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
			case IL_CU:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if (op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTU) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTUD))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_CTU _T("|") FB_CTUD, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
			case IL_CD:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if (op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTD) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTUD))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_CTD _T("|") FB_CTUD, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
			case IL_PV:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if (op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTU) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTD) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_CTUD))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_CTU _T("|") FB_CTD _T("|") FB_CTUD, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
			case IL_IN:
			case IL_PT:
                if(!acc)
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else
				{
					if (op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_TP)  ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_TON) ||
						op->GetType() == ST_TypeSet_C(CG_TBS_INSTANCE, FB_TOF))
					{
                        pParList = new ST_CParamList(&pInst->m_edp, m_pSTContext, 1);
                        pParList->addNamedParam(&pInst->m_edp, pInst->m_szName, false, false, acc);
                        pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp,  op->getSymEntry()->pszId, CG_POU_FB, pParList); 
						pStmt = m_pSTContext->newCallStmt(pCallExpr);
						ST_StmtList_insert(&StmtList, pStmt);
						acc = NULL;
					}
					else
						m_pSTContext->getDM()->msg3(CG_E_INVALID_POU_TYPE, op->getEdp(), 
							FB_TP _T("|") FB_TON _T("|") FB_TOF, pInst->m_szName, op->GetType().GetTypeName());
                }
				break;
            default:
				if(!acc)
		            m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
				else 
				{
					if(IL_EXPR == getInstType(pInst->m_op))
						acc = makeSimpleExpr(pInst, acc, pInst->simpleop.expr);
					else
		                assert(!"internal error");
				}
		        break;
            }
            break;
        case IL_EXPR:
			if(!acc)
                m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
			else
			{
				if(pInst->expr.instrList) 
					op = makeExpression(pInst->expr.subop, pInst->expr.instrList, &pTempInst);
				else 
				{
					op = pInst->expr.subop;
					pTempInst = NULL;
				}

				acc = makeSimpleExpr(pInst, acc, op);

				if(pTempInst)
					m_pSTContext->getDM()->msg1(CG_E_IL_INVALID_OP_IN_EXPRESSION, &pTempInst->m_edp, pTempInst->m_szName);
			}
			break;
        case IL_JMPTYP:
            switch(pInst->m_op)
            {
            case IL_JMPCN:
                if(acc) acc = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, acc, NULL);
            case IL_JMPC:
                if(!acc)
                {
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
                }
                else
                {
                    pStmt = m_pSTContext->newGotoStmt(&pInst->m_edp, pInst->jmp.label->getName());
                    pStmt = m_pSTContext->newIfElseStmt(&pInst->m_edp, acc, pStmt, NULL);
                    ST_StmtList_insert(&StmtList, pStmt);
					acc = NULL;
                }
				delete(pInst->jmp.label);
                break;
            case IL_JMP:
                if(acc)
                {
                    m_pSTContext->getDM()->msg0(CG_W_IL_ACC_NOT_SAVED, &pInst->m_edp);
                    delete acc;
                }
                pStmt = m_pSTContext->newGotoStmt(&pInst->m_edp, pInst->jmp.label->getName());
                ST_StmtList_insert(&StmtList, pStmt);
				acc = NULL;
				delete(pInst->jmp.label);
                break;
            default:
                assert(!"internal error");
            }
            acc = NULL;
            break;
        case IL_RETTYP:
            switch(pInst->m_op)
            {
            case IL_RETCN:
                if(acc) acc = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, acc, NULL);
            case IL_RETC:
                if(!acc)
                {
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
                }
                else
                {
                    pStmt = m_pSTContext->newReturnStmt(&pInst->m_edp);
                    pStmt = m_pSTContext->newIfElseStmt(&pInst->m_edp, acc, pStmt, NULL);
                    ST_StmtList_insert(&StmtList, pStmt);
					acc = NULL;
                }
                break;
            case IL_RET:
                if(acc)
                {
                    m_pSTContext->getDM()->msg0(CG_W_IL_ACC_NOT_SAVED, &pInst->m_edp);
                    delete acc;
                }
                pStmt = m_pSTContext->newReturnStmt(&pInst->m_edp);
                ST_StmtList_insert(&StmtList, pStmt);
				acc = NULL;
                break;
            default:
                assert(!"internal error");
            }
            acc = NULL;
            break;
        case IL_CALFUN:
			acc = makeFunCall(pInst, acc);
			if(acc && acc->GetType().GetTypeSet() == CG_TBS_VOID)
			{
				pStmt = m_pSTContext->newCallStmt(acc);
				ST_StmtList_insert(&StmtList, pStmt);
				acc = NULL;
			}
            break;
        case IL_CALFB:
            switch(pInst->m_op)
            {
            case IL_CALCN:
                if(acc) acc = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, acc, NULL);
            case IL_CALC:
                if(!acc)
                {
                    m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
                }
                else
                {
                    pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp, pInst->fbc.FBIName->getName(), CG_POU_FB, pInst->fbc.paramlist);
                    delete(pInst->fbc.FBIName);
                    pStmt = m_pSTContext->newCallStmt(pCallExpr);
                    pStmt = m_pSTContext->newIfElseStmt(&pInst->m_edp, acc, pStmt, NULL);
                    ST_StmtList_insert(&StmtList, pStmt);
					acc = NULL;
                }
                break;
            case IL_CAL:
                if(acc)
                {
                    m_pSTContext->getDM()->msg0(CG_W_IL_ACC_NOT_SAVED, &pInst->m_edp);
                    delete acc;
                }

                pCallExpr = m_pSTContext->newCallExpr(&pInst->m_edp, pInst->fbc.FBIName->getName(), CG_POU_FB, pInst->fbc.paramlist);
                delete(pInst->fbc.FBIName);
                pStmt = m_pSTContext->newCallStmt(pCallExpr);
                ST_StmtList_insert(&StmtList, pStmt);
				acc = NULL;
                break;
            default:
                assert(!"internal error");
            }
            acc = NULL;
            break;
		case IL_LBLTYP:
			assert(pInst->m_op == IL_LABEL);
			if(pInst->pNext == NULL || isBeginOfSTStmt(pInst->pNext))
			{
                pStmt = m_pSTContext->newLabel(&pInst->m_edp, pInst->m_szName.psz(), NULL);
                ST_StmtList_insert(&StmtList, pStmt);
			}
			else 
			{
				m_pSTContext->getDM()->msg1(CG_E_IL_LABEL_IN_EXPRESSION, &pInst->m_edp, 
					pInst->m_szName.psz());
			}
			break;
        default:
            assert(!"internal error");
        }
        pInst = pInst->pNext;
    }

    return StmtList.plistbegn;
}


ST_Expression_C* il_ParseContext::makeExpression(ST_Expression_C* acc, const IL_Instr* pInstrs, const IL_Instr** pLastInstr)
{
    const IL_Instr*  pTempInst = NULL;

    for(const IL_Instr* pInst = *pLastInstr = pInstrs; pInst; *pLastInstr = pInst = pInst->pNext)
    {
        switch(pInst->m_ityp)
        {
        case IL_SIMPLEOP:
			switch(pInst->m_op)
			{
			case IL_LDN:
				if(acc)
				{
					m_pSTContext->getDM()->msg0(CG_W_IL_ACC_NOT_SAVED, &pInst->m_edp);
					delete acc;
				}
				acc = m_pSTContext->createOp(&pInst->m_edp, CG_NOT, pInst->simpleop.expr, NULL);
				break;
			case IL_LD:
				if(acc)
				{
					m_pSTContext->getDM()->msg0(CG_W_IL_ACC_NOT_SAVED, &pInst->m_edp);
					delete acc;
				}
				acc = pInst->simpleop.expr;
				break;
			default:
				if(!acc)
		            m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
		
				if(acc && IL_EXPR == getInstType(pInst->m_op))
	                acc = makeSimpleExpr(pInst, acc, pInst->simpleop.expr);
				else
					return acc;
			}
			break;
        case IL_EXPR:
			if(!acc)
                m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
			else
			{
				assert(pInst->expr.instrList || pInst->expr.subop || pInst->m_op == IL_NOT);
				acc = makeSimpleExpr(pInst, acc, makeExpression(pInst->expr.subop, pInst->expr.instrList, &pTempInst));
				if(pTempInst)
					m_pSTContext->getDM()->msg1(CG_E_IL_INVALID_OP_IN_EXPRESSION, &pTempInst->m_edp, pTempInst->m_szName);
			}
			break;
        case IL_CALFUN:
			acc = makeFunCall(pInst, acc);
			if(acc && acc->GetType().GetTypeSet() == CG_TBS_VOID)
			{
				m_pSTContext->getDM()->msg1(CG_E_IL_INVALID_OP_IN_EXPRESSION, acc->getEdp(), _T("void func call"));
				delete acc;
				acc = NULL;
			}
            break;
        case IL_LBLTYP:
			m_pSTContext->getDM()->msg1(CG_E_IL_LABEL_IN_EXPRESSION, &pInst->m_edp, pInst->m_szName.psz());
			break;
        default:
            // non simple operation; return accumulated result
            return acc;
        }
    }
    return acc;
}

ST_Expression_C* il_ParseContext::makeSimpleExpr(const IL_Instr* pInst, ST_Expression_C* acc, ST_Expression_C* op)
{
    assert(acc && pInst && getInstType(pInst->m_op) == IL_EXPR);
    switch(pInst->m_op)
    {
    case IL_NOT:
        assert(!op);
        return m_pSTContext->createOp(&pInst->m_edp, CG_NOT, acc, NULL);
    // expression operators
    case IL_AND:
        return m_pSTContext->createOp(&pInst->m_edp, CG_AND, acc, op);
    case IL_ANDN:
        return m_pSTContext->createOp(&pInst->m_edp, CG_AND, acc, m_pSTContext->createOp(&pInst->m_edp, CG_NOT, op, NULL));
    case IL_OR:
        return m_pSTContext->createOp(&pInst->m_edp, CG_OR, acc, op);
    case IL_ORN:
        return m_pSTContext->createOp(&pInst->m_edp, CG_OR, acc, m_pSTContext->createOp(&pInst->m_edp, CG_NOT, op, NULL));
    case IL_XOR:
        return m_pSTContext->createOp(&pInst->m_edp, CG_XOR, acc, op);
    case IL_XORN:
        return m_pSTContext->createOp(&pInst->m_edp, CG_XOR, acc, m_pSTContext->createOp(&pInst->m_edp, CG_NOT, op, NULL));
    case IL_ADD:
        return m_pSTContext->createOp(&pInst->m_edp, CG_ADD, acc, op);
    case IL_SUB:
        return m_pSTContext->createOp(&pInst->m_edp, CG_SUB, acc, op);
    case IL_MUL:
        return m_pSTContext->createOp(&pInst->m_edp, CG_MUL, acc, op);
    case IL_DIV:
        return m_pSTContext->createOp(&pInst->m_edp, CG_DIV, acc, op);
    case IL_MOD:
        return m_pSTContext->createOp(&pInst->m_edp, CG_MOD, acc, op);
    case IL_GT:
        return m_pSTContext->createOp(&pInst->m_edp, CG_GT, acc, op);
    case IL_GE:
        return m_pSTContext->createOp(&pInst->m_edp, CG_GE, acc, op);
    case IL_EQ:
        return m_pSTContext->createOp(&pInst->m_edp, CG_EQ, acc, op);
    case IL_LT:
        return m_pSTContext->createOp(&pInst->m_edp, CG_LT, acc, op);
    case IL_LE:
        return m_pSTContext->createOp(&pInst->m_edp, CG_LE, acc, op);
    case IL_NE:
        return m_pSTContext->createOp(&pInst->m_edp, CG_NE, acc, op);
    default:
        return NULL;
    }
}

ST_Expression_C* il_ParseContext::makeFunCall(const IL_Instr* pInst, ST_Expression_C* acc)
{
    assert(pInst && pInst->m_ityp == IL_CALFUN && pInst->m_op == IL_FUNCALL);

    // accumulator is first parameter of function call
    if(acc)
    {
		ST_CParamList* paramlist = pInst->func.paramlist; 
		if(!paramlist)
			paramlist = new ST_CParamList(acc->getEdp(), m_pSTContext, 16);
        paramlist->addFirstParam(acc);
        acc = m_pSTContext->newCallExpr(&pInst->m_edp,  pInst->m_szName, CG_POU_FUN, paramlist); 
    }
    else
    {
        if(pInst->func.paramlist)
        {
            m_pSTContext->getDM()->msg0(CG_E_IL_ACC_UNDEF, &pInst->m_edp);
            acc = NULL;
        }
        else  // function call with no parameters
        {
            acc = m_pSTContext->newCallExpr(&pInst->m_edp,  pInst->m_szName, CG_POU_FUN, NULL); 
        }
    }

    return acc;
}


bool il_ParseContext::isBeginOfSTStmt(const IL_Instr* pInst)
{
	switch(pInst->m_op)
	{
	case IL_LD:
	case IL_LDN:
	case IL_JMP:
	case IL_FUNCALL:
	case IL_CAL:
	case IL_RET:
	case IL_LABEL:
		return true;
	}
	return false;
}

