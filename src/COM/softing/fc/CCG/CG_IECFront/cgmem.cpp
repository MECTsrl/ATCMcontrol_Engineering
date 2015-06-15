

#include "helper.h"
#include "cgmem.h"






ST_CExprMem::ST_CExprMem()
{
}
ST_CExprMem::~ST_CExprMem()
{
    m_exprs.deletePtrs();
    m_argexprlists.deletePtrs();
    m_hashLiterals.deletePtrs();
}


const TCHAR* ST_CExprMem::registerString(const TCHAR* psz)
{
    return m_strings.registerString(psz);
}


const CG_Literal* ST_CExprMem::registerLiteral(
    CG_DTYPES     type,
    const TCHAR*  pszNormal,
    __int64       anyInt,
    double        anyReal
)
{
    bool          bOk;
    CG_Literal*   pLit;
    const TCHAR*  pszLitReg;  //registerd string pointer
    const TCHAR*  pszLitIFReg;//text of lit as used in CG_Literal:psz
                              //no <type># if ANY_STRING,ANY_FLOAT
                              //but t# , dt#, tod# dt# for ANY_DATE 
                              //also a registered string pointer
    TCHAR         buffer[256];

    if(type==CG_BOOL_R_EDGE || type==CG_BOOL_F_EDGE)
        type = CG_BOOL;

    assert(CG_IsElementary(type));
    assert(type!=CG_STRING && type!=CG_WSTRING);

    if(!pszNormal) //can happen if caller want default initial value
    {
        anyInt = 0;
        anyReal= 0.0;
        switch(type)
        {
        case CG_BOOL:     pszNormal = _T("false");  break;
        case CG_REAL:
        case CG_LREAL:    pszNormal = _T("0.0");    break;
        case CG_BYTE:     
        case CG_WORD:     
        case CG_DWORD:    
        case CG_LWORD:    pszNormal = _T("16#0");   break;
        case CG_SINT:
        case CG_INT:
        case CG_DINT:
        case CG_LINT:
        case CG_USINT: 
        case CG_UINT:
        case CG_UDINT:
        case CG_ULINT:    pszNormal = _T("0");      break;
        case CG_DT:       pszNormal = _T("dt#0001-01-01-00:00:00.000"); break;
        case CG_TOD:      pszNormal = _T("tod#00:00:00.000");   break;
        case CG_DATE:     pszNormal = _T("d#0001-01-01");       break;
        case CG_TIME:     pszNormal = _T("t#0ms");  break;
        default:
            assert(!_T("bad literal data type"));
        }
    }


    //build an unique string to hash the lit must include
    //the data type if not already (e.g if t#)
    if(_istalpha(pszNormal[0]) && _tcschr(pszNormal, _T('#')))
    {
        //assert: only these types should have a type# in it's normalised
        //representation:
        assert(type==CG_TIME||type==CG_TOD||type==CG_DT||type==CG_DATE);
        pszLitReg   = registerString(pszNormal);
        pszLitIFReg = pszLitReg;
    }
    else
    {
        const TCHAR* pszType = ST_CG_DTYPES_ToString(type);
        int          iLen    = lstrlen(pszType);
        memcpy(buffer, pszType, sizeof(TCHAR)*iLen);
        buffer[iLen] = _T('#');
        lstrcpy(buffer+iLen+1, pszNormal);

        pszLitReg   = registerString(buffer);
        pszLitIFReg = pszLitReg + iLen+1; 
    }
   
    pLit = m_hashLiterals.get(pszLitReg);
    if(!pLit)
    {
        FC_NEW_ZSTRUCT(pLit, CG_Literal);

        //fill the data:
        pLit->type = type;
        pLit->psz  = pszLitIFReg;
        if(type==CG_REAL || type==CG_LREAL)
        {
            pLit->ut = CG_LIT_FLT;
            pLit->dbl= anyReal;
        }
        else
        {
            pLit->ut  = CG_LIT_I64;
            pLit->i64 = anyInt;
        }
    
        bOk = m_hashLiterals.insert(pszLitReg, pLit);
        assert(bOk);
    }
    return pLit;
}



const CG_Literal* ST_CExprMem::registerStringLiteral(
    CG_DTYPES     type,
    const TCHAR*  ps
)
{
    bool          bOk;
    CG_Literal*   pLit;
    const TCHAR*  pszLitReg;  //registerd string pointer contains " or ' delimiters

    assert(type==CG_STRING || type==CG_WSTRING);

    if(!ps) //can happen if caller want default initial value
    {   
        ps = type==CG_STRING ? _T("''") :  _T("\"\"");
    }

	assert(ps[0]==_T('\'') || ps[0]==_T('"'));


    pszLitReg = m_strings.registerString(ps);
    pLit = m_hashLiterals.get(pszLitReg);
    if(pLit)
        return pLit;
    
    FC_NEW_ZSTRUCT(pLit, CG_Literal);
    //fill the data:
    pLit->type   = type;
    pLit->psz    = pszLitReg;

    bOk = m_hashLiterals.insert(pszLitReg, pLit);
    assert(bOk);

    return pLit;
}




CG_ExprTree* ST_CExprMem::registerCGExpr(
    CG_EXPRTYP       extyp, 
    const CG_Edipos* pEdp, 
    const CG_DType*  pType
)
{
    CG_ExprTree* p;

    FC_NEW_ZSTRUCT(p, CG_ExprTree);
    m_exprs.add(p);
    p->extyp = extyp;
    p->edp   = *pEdp;
    p->pType = pType;
    return p;
}
CG_ExprTree* ST_CExprMem::registerCGExprArray(int nLen)
{
    CG_ExprTree* p;
    FC_NEW_ZASTRUCT(p, CG_ExprTree, nLen);
    m_exprs.add(p);
    return p;
}

const CG_ArgExpr* ST_CExprMem::registerArgExprList(
    CG_ArgExpr* pArgList
)
{
    m_argexprlists.add(pArgList);
    return pArgList;
}



void ST_CExprMem::getVarExtWrtList(
 FC_CHashPtr
    <const CG_SymEntry*, 
    FC_CGArray<CG_Edipos>*>* pHash
)const
{
    int                             i;
    const CG_ExprTree*              pE;
    const CG_ExprTree*const*        ppEx;
    FC_CGArray<CG_Edipos>*          pEdpList;
    int                             nExprs;
    const FC_CHashPtr
       <const CG_SymEntry*, 
       FC_CGArray<CG_Edipos>*>::Pair* pPair;


    //must be empty on entry
    assert(pHash->countPairs()==0);

    ppEx  = m_exprs.getCElements();
    nExprs= m_exprs.getUsed();

    for(i=0; i<nExprs; i++)
    {
        pE = ppEx[i];
        if(pE->extyp!=CG_VARREF)
            continue;

        if(pE->var.pEntry->declScope != CG_VAR_EXTERNAL)
            continue;

        if((pE->var.accFlags & CG_ACCESS_ANY_W)==0)
            continue;

        //hit:
        pEdpList = pHash->get(pE->var.pEntry);
        if(!pEdpList)
        {
            pEdpList = new FC_CGArray<CG_Edipos>;
            pEdpList->allocSize(64);
            pHash->insert(pE->var.pEntry, pEdpList);
        }
 
        //assert: make sure that edp is safe if we memcpy it, be strikt here
        //only st, box and sfc st action (=stbox) may pass here:
        assert(pE->edp.typ == CG_EDP_ST || pE->edp.typ == CG_EDP_4GR_BOX || pE->edp.typ == CG_EDP_4GR_STBOX);

        pEdpList->add(pE->edp);
    }


    for(pPair = pHash->firstPair(); pPair; pPair = pPair->pNext)
        pPair->pVal->sizeToFit();

}




ST_CBody::ST_CBody()
{
    m_bIsSFC     = false;
    m_pFirstStmt = NULL;
    m_pSFCS      = NULL;
    m_pExAction  = NULL;
    m_usedTriggers = 0;
    CG_InitCMDLEdipos(&m_edpBeg);
    CG_InitCMDLEdipos(&m_edpEnd);
}
ST_CBody::~ST_CBody()
{
    m_stmts.deletePtrs();
    m_caseLists.deletePtrs();
    m_entryRefLists.deletePtrs();
    if(m_pSFCS)
    {
        delete(m_pSFCS->pStepNames);
        delete(m_pSFCS->pStepEdp);
        delete(m_pSFCS->pActionNames);
        delete(m_pSFCS->pActionEdp);
        delete(m_pSFCS->pTransNames);
        delete(m_pSFCS->pTransEdp);
        delete(m_pSFCS->pFollowTrans);
        delete(m_pSFCS->pAltTrans);
        delete(m_pSFCS->pNextSteps);
        delete(m_pSFCS->pNextStepsOffset);
        delete(m_pSFCS->pPrevSteps);
        delete(m_pSFCS->pPrevStepsOffset);

        delete(m_pSFCS->pStepActionBlocks);
        delete(m_pSFCS->pStepActionBlockOffset);
        delete(m_pSFCS->pStepActionBlockQualifier);
        delete(m_pSFCS->pStepActionBlockTimeParameter);
        
        delete(m_pSFCS);
    }
}




CG_STStmt* ST_CBody::newStmt(CG_STMTTYP st, const CG_Edipos* pEdp)
{
    CG_STStmt* ps;

    FC_NEW_ZSTRUCT(ps, CG_STStmt);
    ps->styp = st;
    ps->edp  = *pEdp;
    m_stmts.add(ps);
    return ps;
}
CG_STCaseList* ST_CBody::registerCaseList(CG_STCaseList* pcl)
{
    m_caseLists.add(pcl);  
    return pcl;
}
const CG_EdpEntry* ST_CBody::registerEntryRefList(const CG_EdpEntry* pList, int nLength)
{
    CG_EdpEntry* p;
    if(nLength<=0)
        return NULL;
    p = new CG_EdpEntry[nLength];
    memcpy(p, pList, sizeof(CG_EdpEntry)*nLength);
    m_entryRefLists.add(p);
    return p;
}
void ST_CBody::setSTBody(const CG_STStmt* pStmt)
{
    assert(!m_pFirstStmt && !m_bIsSFC);
    m_pFirstStmt = pStmt;
}
void ST_CBody::setSfcBody()
{
    assert(!m_bIsSFC);
    m_bIsSFC = true;
}
void ST_CBody::setSFCStruct(CG_SFCStruct* pSFCS)
{
    assert(!m_pSFCS && m_bIsSFC);
    m_pSFCS = pSFCS;
}

void ST_CBody::setTransiBody(const CG_STStmt* pStmt)
{
    assert(!m_pFirstStmt);
    m_transitions.add(pStmt);
}
void ST_CBody::setActionBody(const CG_STStmt* pStmt)
{
    assert(!m_pFirstStmt);
    m_actions.add(pStmt);
}
void ST_CBody::setExceptionActionBody(const CG_STStmt* pStmt)
{
    assert(!m_pFirstStmt);
    m_pExAction = pStmt;
}






//non SFC:
const CG_STStmt* ST_CBody::getStmts()const
{
    assert(!m_bIsSFC && m_actions.getUsed()==0 && m_transitions.getUsed()==0);
    return m_pFirstStmt;
}


//SFC:
const CG_SFCStruct* ST_CBody::getSFCStruct()const
{
    assert(m_pSFCS && m_bIsSFC);
    return m_pSFCS;
}
const CG_STStmt* ST_CBody::getTransi(long nTransi)const
{
    assert(!m_pFirstStmt && m_bIsSFC);
    if(nTransi>=0 && nTransi<m_transitions.getUsed())
    {
        return m_transitions.get(nTransi);
    }
    else
    {
        assert("ST_CBody::getTransi: transition number out of bounds");
        return NULL;
    }
}
const CG_STStmt* ST_CBody::getAction(long nAction)const
{
    assert(!m_pFirstStmt && m_bIsSFC);
    if(nAction>=0 && nAction<m_actions.getUsed())
    {
        return m_actions.get(nAction);
    }
    else
    {
        assert("ST_CBody::getAction: action number out of bounds");
        return NULL;
    }
}
const CG_STStmt* ST_CBody::getExceptionAction()const
{
    assert(m_bIsSFC);
    return m_pExAction;
}
const CG_STStmt*const* ST_CBody::getAllTransi(int* pnTransi)const
{
    assert(!m_pFirstStmt && m_bIsSFC);
    *pnTransi = m_transitions.getUsed();
    return m_transitions.getCElements();
}
const CG_STStmt*const* ST_CBody::getAllAction(int* pnActions)const
{
    assert(!m_pFirstStmt && m_bIsSFC);
    *pnActions = m_actions.getUsed();
    return m_actions.getCElements();
}



