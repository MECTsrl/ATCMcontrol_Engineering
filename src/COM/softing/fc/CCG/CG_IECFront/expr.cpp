 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "helper.h"
#include "cgt_tools.h"
#include "cgt_address.h"
#include "expr.h"
#include "st_parseContext.h"
#include "CG_Messages.h"

//HACK:
static const CG_DType* ToCGType(
    const ST_TypeSet_C& sttype,
    CG_CIECFront*       pIFront
)
{
    const CG_DType* pt;


    if(sttype.GetTypeSet()==CG_TBS_ARRAY)
    {
        pt = ToCGType(sttype.GetArrayBase(), pIFront);
        
        if(sttype.GetArrayDim()==0)
        {
            pt = pIFront->registerDType(
                    0,
                    NULL,
                    pt->type,
                    pt->lStrLen,
                    pt->pSym
                );
        }
        else if(sttype.GetArrayDim()==1)
        {
            ST_SUBRANGE_TYP rg;
            sttype.GetArrayRange(0, &rg);
            pt = pIFront->registerDType(
                    1,
                    (CG_Subrange*)&rg,
                    pt->type,
                    pt->lStrLen,
                    pt->pSym
            );
        }
        else
        {
            assert(!"array dim >1 not supported");
        }
        return pt;
    }


    return pIFront->registerDType(sttype.GetTypeSet(), sttype.GetTypeName(), sttype.GetStringLength());
#if 0 //##AC
    CG_BITSET
    t = ST_TBSToDTypes(sttype.GetTypeSet());
    switch(t)
    {
    case CG_STRING:
    case CG_WSTRING:
    case CG_QVT_STRING:
    case CG_QVT_WSTRING:
        pt = pIFront->registerDType(t, sttype.GetStringLength());
    break;

    case CG_INSTANCE:
    case CG_STRUCT:
    case CG_PROGRAM:
    {
        bool bRecursive;
        pt = pIFront->registerDType( //TODO HACK
              pIFront->loadHeader(sttype.GetTypeName(), &bRecursive)->getCGSym());
    }
    break;
    default:
        pt = pIFront->registerDType(t, -1);
    }
    
    return pt;
#endif
}


void CG_SetAccFlags(
    CGT_CDiagMsg*      pdm,
    const CG_ExprTree* pExpr, 
    WORD               accFlags
)
{
    const CG_ExprTree*  pE = pExpr;
    bool                bAlreadyPartially = false;
    WORD                wFlags = accFlags;
    WORD                accBase = 0;
    const CG_SymEntry*  pVarEntry;


    if(pE) //can be NULL but only after error TODO assert for error>0
    {    
        for(;;)
        { 
            switch(pE->extyp)
            {
            case CG_VARREF:
                //sorry for type cast
                ((CG_ExprTree*)pE)->var.accFlags |= (wFlags|accBase);
                pVarEntry = pE->var.pEntry;

                if(pVarEntry->declScope==CG_VAR_OUTPUT && 
                   pVarEntry->pParent->pouTyp==CG_POU_FUN &&
                   !pVarEntry->pInit
                  )
                {
                    if((wFlags & CG_ACCESS_ANY_R)!=0 && (pVarEntry->accFlags & CG_ACCESS_ANY_W)==0)
                    {
                        pdm->msg1(CG_W_FUNOUT_USED_BEFORE_INIT, &pE->edp, pVarEntry->pszId);
                    }
                }

                //more sorry for type cast
                ((CG_SymEntry*)pVarEntry)->accFlags |= (wFlags|accBase);
                return; 

            case CG_TMPVAR:
                //sorry for type cast
                ((CG_ExprTree*)pE)->tmp.accFlags |= (wFlags|accBase);
                //if it is a ref tmp update also the sym entry that gets ref
                //but not for the W-access when the tmp itself gets initialised.
                //It is assumed here that a tmp var is only written once, all
                //other subsequent accesses must be read type access:
                if(pE->tmp.pRefExpr && (wFlags & CG_ACCESS_W)==0)
                    ((CG_SymEntry*)CGT_GetExprSymEntry(pE->tmp.pRefExpr))->accFlags |= (wFlags|accBase);

                return; 
            case CG_STRUCTREF: 
                //sorry for type cast
                ((CG_ExprTree*)pE)->str.accFlags |= (wFlags|accBase);

                if(!bAlreadyPartially)//how about structs with only one member ??
                {
                    //assert: accFlags<<1 works only as lng no partial bit on entry (max change if array of FB/FB in struct) 
                    assert((accFlags & (CG_ACCESS_r|CG_ACCESS_w|CG_ACCESS_o|
                                        CG_ACCESS_i|CG_ACCESS_x|CG_ACCESS_c|
                                        CG_ACCESS_a|CG_ACCESS_N|CG_ACCESS_B))==0);
                    wFlags = accFlags<<1;
                    bAlreadyPartially = true;
                }
                accBase = CG_ACCESS_B;
                pE = pE->str.pBase;
            break;
            case CG_ARRAYREF:  

                //sorry for type cast
                ((CG_ExprTree*)pE)->arr.accFlags |= (wFlags|accBase);

                assert(pE->arr.nSubs==1);
                if(!bAlreadyPartially && pE->arr.pSubs[0].extyp==CG_LITERAL)
                {
                    //assert: accFlags<<1 works only as lng no partial bit on entry (max change if array of FB/FB in struct) 
                    assert((accFlags & (CG_ACCESS_r|CG_ACCESS_w|CG_ACCESS_o|
                                        CG_ACCESS_i|CG_ACCESS_x|CG_ACCESS_c|
                                        CG_ACCESS_a|CG_ACCESS_N|CG_ACCESS_B))==0);
                    wFlags = accFlags<<1;
                    bAlreadyPartially = true;
                }
                accBase = CG_ACCESS_B;
                pE = pE->arr.pBase;
            break;
            case CG_NOOP:
                pE = pE->ops.pLhs;
            break;
            default:
                return;
            }
        }
    }
}




enum ST_MEMORY_CLASS //TODO remove 
{
    ST_VAR,
    ST_VAR_IN = ST_VAR,
    ST_VAR_OUT,
    ST_VAR_IN_OUT,
    ST_VAR_PRIVATE
};
/*
 * ST_StoreClassToFBMemberAccess should be used to convert ST_STORAGECLASS
 * to ST_MEMORY_CLASS, which will be used to check the access rights of FB members.
 * DO NOT call this function for DUT member access, DUT member access is ST_VAR
 * anyway.
 * meaning of ST_MEMORY_CLASS should be as follows:
 * ST_VAR may be read or written anywhere (STRUCTs have only VARs)
 * ST_VAR_IN may be written only inside call statement
 * ST_VAR_IN_OUT must be written inside call statement
 * ST_VAR_OUT may only be read outside the call statement
 * ST_VAR_PRIVATE cannot be read or written
 *
 */
ST_MEMORY_CLASS ST_StoreClassToFBMemberAccess(CG_DECLSCOPE store) //TODO remove
{
    ST_MEMORY_CLASS mem;
    
    switch(store)
    {
    case CG_CONST:
    case CG_VAR:      mem = ST_VAR_PRIVATE; break;
    case CG_VAR_INPUT:    mem = ST_VAR_IN;      break;
    case CG_VAR_OUTPUT:   mem = ST_VAR_OUT;     break;
    case CG_VAR_IN_OUT:   mem = ST_VAR_IN_OUT;  break;
    case CG_VAR_GLOBAL:   mem = ST_VAR_PRIVATE; break;
    case CG_VAR_EXTERNAL: mem = ST_VAR_PRIVATE; break;  
    case CG_DECLSCOPE_UNDEF:
    default:
        assert(!_T("illegal enum"));
        mem = ST_VAR_PRIVATE;
    }

    return mem;
}























ST_CParseTree::ST_CParseTree(
    const CG_Edipos* pEdp, 
    st_ParseContext* pfc
)
{
    m_pfc     = pfc;
    m_pfc_link= pfc;
    m_edp     = *pEdp;
    m_pFront  = pfc->getFront();
    m_pdm     = m_pFront->m_pdm;
    assert(m_pFront);
    m_pNext   = NULL;
    m_pPrev   = pfc->m_pLastHeapElement;

    if(m_pfc_link->m_pLastHeapElement) 
    {
        assert(m_pfc_link->m_pLastHeapElement->m_pNext == NULL);
        m_pfc_link->m_pLastHeapElement->m_pNext = this;
    }
    else
    {
        assert(m_pfc_link->m_pFirstHeapElement == NULL);
        m_pfc_link->m_pFirstHeapElement = this;
    }
    m_pfc_link->m_pLastHeapElement = this;
}

ST_CParseTree::~ST_CParseTree()
{
    Detach();
}
void ST_CParseTree::Detach()
{
    if(m_pfc_link)
    {
        if(m_pNext) 
        {
            assert(m_pNext->m_pPrev == this);
            m_pNext->m_pPrev = m_pPrev;
        }
        else if(m_pfc_link->m_pLastHeapElement == this) 
        {
            m_pfc_link->m_pLastHeapElement = m_pPrev;
        }

        if(m_pPrev) 
        {
            assert(m_pPrev->m_pNext == this);
            m_pPrev->m_pNext = m_pNext;
        }
        else if(m_pfc_link->m_pFirstHeapElement == this) 
        {
            m_pfc_link->m_pFirstHeapElement = m_pNext;
        }
        m_pNext = NULL;
        m_pPrev = NULL;
        m_pfc_link = NULL;
    }
}


void ST_Expression_C::fixType()
{
    this->Propagate(ST_SelectBestType(m_pdm, &m_pFront->m_features, typeset, &m_edp));
}
bool ST_Expression_C::checkTypedExpr(
    CG_BITSET        lTypeSet, 
    bool             bErrMsg
)
{
    ST_TypeSet_C type = typeset;

    type &= lTypeSet;
    
    if(type.IsEmptyType()) 
    {
        if(bErrMsg)
            m_pdm->msg1(CG_E_WRONG_TYPE, &m_edp, ST_TypeSetToErrMsgString(lTypeSet));
        return false;
    }
    else
    {
        Propagate(type);
    }
    return true;
}    

const CG_ExprTree* ST_Expression_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    assert(!"ST_Expression_C::makeCGExpr called");
    return NULL;
}


CG_ExprTree* ST_Expression_C::initCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady,
    CG_EXPRTYP   extyp
)const
{
    if(pReady)
    {
        memset(pReady, 0, sizeof(*pReady));
        pReady->edp   = m_edp;
        pReady->extyp = extyp;
        pReady->pType = ToCGType(typeset, m_pFront);
        return pReady;
    }
    return pAlloc->registerCGExpr(
        extyp, &m_edp, ToCGType(typeset, m_pFront));
}
int ST_Expression_C::is_Lvalue() const
{
    return 0;
}
bool ST_Expression_C::isAny64Literal(__int64* pValue)const
{
    const ST_BINVAL_TYP* pVal = getLitValue();
    if(pVal && typeset.GetTypeSet() & (CG_TBS_ANY_INT|CG_TBS_DATE|CG_TBS_TIME))
    {
        if(pValue)
            *pValue = pVal->any64;
        return true;
    }
    return false;
}
bool ST_Expression_C::isBoolVal(bool* pValue)const
{
    const ST_BINVAL_TYP* pVal = getLitValue();
    if(pVal && typeset.GetTypeSet() & CG_TBS_BOOL)
    {
        if(pValue)
            *pValue = pVal->any64 ? 1 : 0;
        return true;
    }
    return false;
}

const ST_BINVAL_TYP* ST_Expression_C::getLitValue(const CG_SymEntry** ppConst)const
{
    if(ppConst)
        *ppConst = NULL;
    return NULL;
    
}
bool ST_Expression_C::isErrorExpr()
{
    return false;
}
const CG_DType* ST_Expression_C::getCGType()const
{
    return ToCGType(typeset, m_pFront);
}

void ST_Expression_C::Propagate(const ST_TypeSet_C &what)
{
    typeset = what;
}
ST_Variable_C* ST_Expression_C::toVariableClass()
{
    return NULL;
}
ST_StructAccess_C* ST_Expression_C::toStructAccess()
{
    return NULL;
}
ST_Literal_C* ST_Expression_C::toLiteralClass()
{
    return NULL;
}
ST_StepAction_C* ST_Expression_C::toStepActionClass()
{
    return NULL;
}
bool ST_Expression_C::isTmpVar()const
{
    return false;
}
bool ST_Expression_C::isReferenceTmpVar()const
{
    return false;
}
const CG_SymEntry* ST_Expression_C::getSymEntry()const
{
    return NULL;
}
const TCHAR* ST_Expression_C::getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const //##AC
{
    if(ppSym)
        *ppSym = NULL;
    if(ppArgList)
        *ppArgList = NULL;
    return NULL;
}
bool ST_Expression_C::needsTmpVar()const
{
    return true;
}
bool ST_Expression_C::needsParenthesis()const
{
    return true;
}
bool ST_Expression_C::isArrayInit()
{
    return false;
}
bool ST_Expression_C::checkInit(
    const CG_DType* pType, 
    CG_Initialiser* pRet
)
{
    bool    bRet;
    long    arrayLenTot;
    long    count;

    arrayLenTot = 1;
    if(pType->bIsArray)
        arrayLenTot = pType->nElems;
    
    count = 0;
    bRet = this->checkInit2(pType, pRet, &count);

    //warn too many/less inits for array ? 
    if(bRet && count!=arrayLenTot)
    {
        assert(pType->bIsArray);
        m_pdm->msg2(CG_W_INIT_ARRAY_TOO_LESSMUCH, getEdp(),
            m_pdm->jot1()<<count, 
            m_pdm->jot2()<<arrayLenTot);
    }

    return bRet;
}

//create CG_Initialiser for all simple types:
bool ST_Expression_C::checkInit2(
    const CG_DType* pType, 
    CG_Initialiser* pRet, 
    long*           pCount
)
{
    ST_TypeSet_C          litType = typeset;
    ST_TypeSet_C          typeIn;
    CG_BITSET             litTypeSet = litType.GetTypeSet();
    const ST_BINVAL_TYP*  pLitVal;
    const CG_SymEntry*    pConst;
    

    assert(CG_IsMonomorph(pType->type));
    
    (*pCount)++;

    if(this->isErrorExpr())
    {
        assert(m_pdm->hasErr());
        return false;
    }


    typeIn.initFromCG_DType(pType);

    //check data type:
    litType &= typeIn;
    if(litType.IsEmptyType()) 
    {
        if(CG_IsAnyQVT(pType->type))
        {
            //special diag in this case (otherwise some simple users try hours to 
            //find the corrent init value syntax for QVTs ...):
            m_pdm->msg0(CG_E_QVT_INIT_NOT_ALLOWED, &m_edp);
        }
        else
        {
            m_pdm->msg2(CG_E_INIT_TYPE, &m_edp, ST_TypeSetToErrMsgString(typeset), pType->pszName);
        }
        return false;
    }
    else
    {
		//warn if init string is longer than specified string len, if any:
		if((litTypeSet & CG_TBS_ANY_STRING) && pType->lStrLen>=0)
        {
            //assert: lit string must have a length
            assert(typeset.GetStringLength()>=0);

            if(pType->lStrLen < typeset.GetStringLength())
				m_pdm->msg2(CG_W_STR_TRUNC, &m_edp, ST_TypeSetToErrMsgString(typeset), pType->pszName);
		}
        Propagate(litType);
    }

    //check that expr can be evaluated:        
    pLitVal = getLitValue(&pConst);
    if(!pLitVal)
    {
        m_pdm->msg0(CG_E_EXPR_NOT_CONST, &m_edp);
        return false;
    }   

    if(pRet)
    {
        if(pConst)
        {
            assert(pConst->pType->type==pType->type);
            assert(pConst->pInit->ut==CG_INIT_SIMPLE);

            pRet->ut         = CG_INIT_SIMPLE;
            pRet->sim.pConst = pConst;
            pRet->sim.pLit   = pConst->pInit->sim.pLit;
        }
        else
        {
            pRet->sim.pConst = NULL;
            pRet->ut = CG_INIT_SIMPLE;
            switch(pType->type)
            {
            case CG_STRING:
            case CG_WSTRING:
                assert(pLitVal->iLen>=0);
                pRet->sim.pLit = m_pFront->m_exprMem.registerStringLiteral(pType->type, 
                    pLitVal->pText);
            break;
            case CG_REAL:
            case CG_LREAL:
                assert(pLitVal->iLen>=0);
                pRet->sim.pLit = m_pFront->m_exprMem.registerLiteral(pType->type, 
                    pLitVal->pText, 0, _tcstod(pLitVal->pText, NULL));
            break;
            default:
                assert(pLitVal->iLen==-1);
                pRet->sim.pLit = m_pFront->m_exprMem.registerLiteral(pType->type, 
                    ST_BinValToIECString(pLitVal, litType.GetTypeSet()),
                    pLitVal->any64, 0.0);
            }
        }
    }

    return true;
}




ST_ErrorExpr_C::ST_ErrorExpr_C(
    const CG_Edipos*    pEdp, 
    st_ParseContext*    pfc,
    const ST_TypeSet_C& type
)
  : ST_Expression_C(pEdp, pfc)
{
    assert(pfc->getErrorsInFile()>0);

    if(type != CG_TBS_EMPTY)
        typeset = type;
    else
        typeset = CG_TBS_ANY_INT;

    //must fix type to avoid internal error if still poly type:
    //e.g. createOp, selectOverload will not always call fixtype on error...
    fixType();
}
int  ST_ErrorExpr_C::is_Lvalue()const{return true;}
bool ST_ErrorExpr_C::isErrorExpr()
{
    return true;
}
const CG_ExprTree* ST_ErrorExpr_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_ExprTree* pRet;

    assert(m_pdm->hasErr());

    pRet = initCGExpr(pAlloc, pReady, CG_BUILDIN);
    pRet->bin.pszId = _T("<!!expression error node!!>");
    return pRet;
}




ST_TmpVar_C::ST_TmpVar_C(
    const CG_Edipos*    pEdp, 
    st_ParseContext*    pfc,
    int                 iTmpVarNum, 
    const ST_TypeSet_C& dtype, 
    const CG_ExprTree*  pReferencedExpr,
    WORD                tmpTyp
)
  : ST_Expression_C(pEdp, pfc)
{
    assert(iTmpVarNum>0 || tmpTyp && iTmpVarNum>=0);
    typeset = dtype; 
    m_iTmpVarNum = iTmpVarNum;
    m_pReferencedExpr = pReferencedExpr;
    m_tmpTyp = tmpTyp;
}
int ST_TmpVar_C::is_Lvalue() const
{
    return 1;
}
bool ST_TmpVar_C::isTmpVar()const
{
    return true;
}
bool ST_TmpVar_C::isReferenceTmpVar()const
{
    return m_pReferencedExpr!=NULL;
}
const CG_ExprTree* ST_TmpVar_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_ExprTree* pRet;

    if(m_tmpTyp==0)
    {
        assert(m_iTmpVarNum>0);
        pRet = initCGExpr(pAlloc, pReady, CG_TMPVAR);
        pRet->tmp.lTmpNum = m_iTmpVarNum;
        //pRet->tmp.bIsRef  = m_pReferencedExpr;
        pRet->tmp.pRefExpr  = m_pReferencedExpr;
    }
    else
    {
        pRet = initCGExpr(pAlloc, pReady, CG_TRIGVAR);
        pRet->trg.lTrigNum = m_iTmpVarNum;
        pRet->trg.trigTyp  = m_tmpTyp;
    }
    return pRet;
}
bool ST_TmpVar_C::needsTmpVar()const
{
    return false;
}
bool ST_TmpVar_C::needsParenthesis()const
{
    return false;
}
int ST_TmpVar_C::getTmpVarNum() const
{
    return m_iTmpVarNum;
}




ST_Literal_C::ST_Literal_C(
    const CG_Edipos*    pEdp, 
    st_ParseContext*    pfc,
    ST_BINVAL_TYP*      pBinVal,
    const ST_TypeSet_C& type,
    bool                bIsPureLit,
    const CG_SymEntry*  pConstEntry
)
: ST_Expression_C(pEdp, pfc)
{
    if(!pBinVal)
    {   //caller wants the IEC default:
        pBinVal = new ST_BINVAL_TYP;
        if(type.GetTypeSet() & CG_TBS_ANY_STRING)
            ST_InitBinValString(pBinVal, NULL);
        else if(type.GetTypeSet() & CG_TBS_ANY_REAL)
            ST_InitBinValString(pBinVal, _T("+0.0"));
        else
            ST_InitBinValI64(pBinVal, 0);
    }

    assert((type.GetTypeSet() & CG_TBS_ANY_REAL)==0 || 
              pBinVal->pText[0]==_T('+') || 
              pBinVal->pText[0]==_T('-'));
    m_pConstEntry= pConstEntry;
    m_pBinVal    = pBinVal;
    m_bIsPureLit = bIsPureLit;
    typeset      = type;
}
ST_Literal_C::~ST_Literal_C()
{
    ST_FreeInitBinVal(m_pBinVal);
    delete(m_pBinVal);
}
const ST_BINVAL_TYP* ST_Literal_C::getLitValue(const CG_SymEntry** ppConst)const
{
    if(ppConst)
        *ppConst = m_pConstEntry;
    return m_pBinVal;
}
ST_Literal_C* ST_Literal_C::toLiteralClass()
{
    return this;
}

const CG_ExprTree* ST_Literal_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_DTYPES t;
    CG_ExprTree* pRet;


    pRet = initCGExpr(pAlloc, pReady, CG_LITERAL);
    t = pRet->pType->type;

    pRet->lit.pConst= m_pConstEntry;

    switch(t)
    {
    case CG_STRING:
    case CG_WSTRING:
        assert(m_pBinVal->iLen>=0);
        pRet->lit.pLit = pAlloc->registerStringLiteral( 
            t, m_pBinVal->pText);
    break;
    case CG_REAL:
    case CG_LREAL:
        assert(m_pBinVal->iLen>=0);
        pRet->lit.pLit = pAlloc->registerLiteral( 
            t, m_pBinVal->pText, 0, _tcstod(m_pBinVal->pText, NULL));
    break;
    default:
        assert(m_pBinVal->iLen==-1);
        pRet->lit.pLit = pAlloc->registerLiteral(
            t, ST_BinValToIECString(m_pBinVal, typeset.GetTypeSet()),
            m_pBinVal->any64, 0.0);
    }

    return pRet;
}
bool ST_Literal_C::needsTmpVar()const
{
    return false;
}
bool ST_Literal_C::needsParenthesis()const
{
    return false;
}
                








static CG_BITSET ST_GetDirectAddressTypeSet(const TCHAR* pszAddr)
{
    if(pszAddr && pszAddr[0]==_T('%'))
    {
        if(_istalpha(pszAddr[1]))
        {
            switch(pszAddr[2]) 
            {
            default:
            case _T('X'): return CG_TBS_BOOL|CG_TBS_QVT_BOOL;
            case _T('B'): return CG_TBS_BYTE|CG_TBS_QVT_BYTE;
            case _T('W'): return CG_TBS_WORD|CG_TBS_QVT_WORD|CG_TBS_INT|CG_TBS_QVT_INT;
            case _T('D'): return CG_TBS_DWORD    |CG_TBS_DINT    |CG_TBS_REAL    |CG_TBS_LREAL|
                                 CG_TBS_QVT_DWORD|CG_TBS_QVT_DINT|CG_TBS_QVT_REAL|CG_TBS_QVT_LREAL;//allow LREAL for D size !
            case _T('L'): return CG_TBS_QVT_LWORD|CG_TBS_LWORD|CG_TBS_QVT_LINT|CG_TBS_LINT|CG_TBS_QVT_LREAL|CG_TBS_LREAL;
            case _T('S'): return CG_TBS_QVT_STRING|CG_TBS_STRING|CG_TBS_QVT_WSTRING|CG_TBS_WSTRING;
            }
        }
        else
        {
            //seems to be a %14.5:
            return CG_TBS_STRUCT|CG_TBS_INSTANCE;
        }
    }
    else
    {
        assert(!"ST_GetDirectAddressTypeSet: bad addr syntax");
    }
    return CG_TBS_EMPTY;
}



ST_DirectVar_C::ST_DirectVar_C(
    const CG_Edipos* pEdp, 
    st_ParseContext* pfc,
    const TCHAR*     pszDirectAddr
)
:  ST_Expression_C(pEdp, pfc),  szAddr(pszDirectAddr)
{
    bSyntaxOK = true;
	Lvalue = CGT_GetAddrLocation(pszDirectAddr)!=_T('I');
	typeset = ST_GetDirectAddressTypeSet(pszDirectAddr) & (~CG_TBS_ANY_QVT);
}

int ST_DirectVar_C::is_Lvalue() const
{
  return Lvalue;
}





ST_Variable_C::ST_Variable_C(
    const CG_Edipos*   pEdp, 
    st_ParseContext*   pfc,
    bool               bLvalue, 
    const CG_SymEntry* pEntry
)
 : ST_Expression_C(pEdp, pfc)
{
    assert(pEntry && pEntry->declScope!=CG_CONST);

    typeset.initFromCG_DType(pEntry->pType);
    m_bLvalue = bLvalue;
    m_pEntry  = pEntry;
}
int ST_Variable_C::is_Lvalue() const
{
    return m_bLvalue ? 1 : 0;
}
bool ST_Variable_C::needsParenthesis()const
{
    return false;
}
ST_Variable_C* ST_Variable_C::toVariableClass()
{
    return this;
}
const CG_SymEntry* ST_Variable_C::getSymEntry()const
{
    return m_pEntry;
}
const CG_ExprTree* ST_Variable_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_ExprTree* pRet;

    assert(m_pEntry && m_pEntry->declScope!=CG_CONST);

    pRet = initCGExpr(pAlloc, pReady, CG_VARREF);
    pRet->var.pEntry = m_pEntry;
    pRet->var.accFlags = 0;
    return pRet;
}







ST_StepAction_C::ST_StepAction_C(
    const CG_Edipos* pEdp, 
    st_ParseContext* pfc,
    int              stepActionNr, 
    const TCHAR*     pszId,
    bool             isAction
)
: ST_Expression_C(pEdp, pfc)
{
    m_iNr = stepActionNr;
    m_iFlagTyp = CG_SFCEXPR_TYP_UNDEF;
    m_id.load(pszId);
    m_bIsAction = isAction;

    //set the typeset, only needed for better err msgs:
    typeset = ST_TypeSet_C(CG_TBS_STRUCT, m_bIsAction ? _T("__ACTION") : _T("__STEP"));
}
ST_StepAction_C* ST_StepAction_C::toStepActionClass()
{
    return this;
}
void ST_StepAction_C::setFlagTyp(ST_Identifier_C* pId)
{
    if (m_iFlagTyp == CG_SFCEXPR_TYP_UNDEF)
    {
        if ( !m_bIsAction
           && !_tcsicmp(pId->getName(), _T("X")) )
        {
            m_iFlagTyp = CG_STEP_X;
            typeset = ST_TypeSet_C(CG_TBS_BOOL);
        }
        else if ( !m_bIsAction
                && !_tcsicmp(pId->getName(), _T("T")) )
        {
            m_iFlagTyp = CG_STEP_T;
            typeset = ST_TypeSet_C(CG_TBS_TIME);
        }
        else if ( m_bIsAction
                && !_tcsicmp(pId->getName(), _T("A")) )
        {
            m_iFlagTyp = CG_ACTION_A;
            typeset = ST_TypeSet_C(CG_TBS_BOOL);
        }
        else if ( m_bIsAction
                && !_tcsicmp(pId->getName(), _T("Q")) )
        {
            m_iFlagTyp = CG_ACTION_Q;
            typeset = ST_TypeSet_C(CG_TBS_BOOL);
        }
        else
        {
            // error
            if (m_bIsAction)
                m_pdm->msg2(CG_E_UNDEF_MEMBER, &m_edp, pId->getName(), _T("__ACTION")); 
            else
                m_pdm->msg2(CG_E_UNDEF_MEMBER, &m_edp, pId->getName(), _T("__STEP")); 
        }
    }
    else
    {
         // error
        if (m_bIsAction)
            m_pdm->msg2(CG_E_UNDEF_MEMBER, &m_edp, pId->getName(), _T("__ACTION")); 
        else
            m_pdm->msg2(CG_E_UNDEF_MEMBER, &m_edp, pId->getName(), _T("__STEP"));  

    }

    delete(pId);

}

const CG_ExprTree* ST_StepAction_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_ExprTree* pRet;

    pRet = initCGExpr(pAlloc, pReady, CG_STEPACTION);
    pRet->sfc.typ    = m_iFlagTyp;
    pRet->sfc.iSANum = m_iNr;
    pRet->sfc.pszSA  = pAlloc->registerString(m_id);

    return pRet;
}





//##AC beg
ST_ExpressionList_C::ST_ExpressionList_C(
    st_ParseContext*     pfc,
    ST_Expression_C*     pFirst
)
 : ST_CParseTree(pFirst->getEdp(), pfc), m_list(3)
{
    pFirst->Detach();
    m_list.add(pFirst);
}
ST_ExpressionList_C::~ST_ExpressionList_C()
{
    m_list.deletePtrs();
}
//##AC end

ST_CCaseExpr::ST_CCaseExpr(//TODO IEC extension: || ST_ANY_BIT & ~ST_BOOL
    st_ParseContext* pfc,
    ST_Expression_C* pLo,
    ST_Expression_C* pUp //if NULL not a range just a simple:
) : ST_CParseTree(pLo->getEdp(), pfc)
{
    addRange(pfc, pLo, pUp);
}


void ST_CCaseExpr::addRange(
    st_ParseContext* pfc,
    ST_Expression_C* pLo,
    ST_Expression_C* pUp //if NULL not a range just a simple case
)
{
    __int64     valLo = 0;
    __int64     valUp = 0;
    CG_Subrange ra;
    bool        bOk;

    //the simple case, no range and no list:
    bOk = pLo->checkTypedExpr(CG_TBS_ANY_INT & ~CG_TBS_LINT, true); 
    pLo->fixType();
    
    if(!pLo->toLiteralClass())
    {
        bOk = false;
        m_pdm->msg0(CG_E_EXPR_NOT_CONST, pLo->getEdp()); 
    }

    if(pUp)
    {
        bOk = bOk && pUp->checkTypedExpr(CG_TBS_ANY_INT & ~CG_TBS_LINT, true); 
        pUp->fixType();

        if(!pUp->toLiteralClass())
        {
            bOk = false;
            m_pdm->msg0(CG_E_EXPR_NOT_CONST, pUp->getEdp()); 
        }
    }

    if(bOk)
    {
        bool bOk2 = pLo->isAnyIntLiteral(&valLo);
        assert(bOk2);
        if(pUp)
        {
            bOk2 = pUp->isAnyIntLiteral(&valUp);
            assert(bOk2);

            if(valLo>valUp)
            {
                bOk = false;
                m_pdm->msg1(CG_E_BAD_RANGE, pLo->getEdp(), m_pdm->jot1()<<valLo<<_T("..")<<valUp);
            }
        }
        else
        {
            valUp = valLo;
        }
    
    }

    //carry on after error:
    ra.lRangeStart   = (long)valLo;
    ra.ulRangeLength = (unsigned long)(valUp-valLo);
    m_rlist.add(ra);

    delete(pLo);
    delete(pUp);
}

//##AC beg
const CG_ExprTree* ST_CLabelRef::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    ST_CJumpLabel* pJL; 
    CG_ExprTree*   pRet;
    ST_CBodyTMP*   pBdyTmp;


    pRet = initCGExpr(pAlloc, pReady, CG_LABELREF);
    pRet->lbl.nLblNum = -1;  //set later at end of body 
    pRet->lbl.pszLbl  = pAlloc->registerString(m_label);

    pBdyTmp = m_pfc->getBodyTMP();
    if(pBdyTmp)
    {
        pJL = pBdyTmp->labels.get(m_label);
        if(!pJL)
        {
            pJL = new ST_CJumpLabel();
            pBdyTmp->labels.insert(m_label, pJL);
        }
        pJL->m_lblrefex.add(pRet);
    }
    else
    {
        //may happen if parse single expr TODO use given context in this case ?
        m_pdm->msg1(CG_E_LABEL_UNDEF, getEdp(), m_label);
    }
    return pRet;
}



static FC_CString& ArgDiagID(FC_CString& jot, const ST_PARAMSPEC* pAS, int zIdx)
{
    jot.clear();
    if(pAS->pszFormal)
        jot<<'\''<<pAS->pszFormal<<'\'';
    else
        jot<<zIdx+1;
    return jot;
}



ST_CFunction::ST_CFunction(
    const CG_Edipos*           pEdp,
    st_ParseContext*           pfc,
    const ST_CSymtabPrv*       pSymPrv,
    ST_CParamList*             pActuals
) : ST_Expression_C(pEdp, pfc)
{
    m_pSymPrv = pSymPrv;

    if(pActuals)
    {
        pActuals->m_list.ownmemto(&m_actuals);
        delete(pActuals);
    }
    m_pPropIdx = NULL;
    m_bCheckOK = false;
}

ST_CFunction::~ST_CFunction()
{
    delete(m_pPropIdx);
    for(int i=m_actuals.getUsed()-1; i>=0; i--)
        delete(m_actuals.get(i).pActual);
}



static bool ST_SetParamSymEntries(
    CGT_CDiagMsg* pdm, const CG_Edipos* pEdp, 
    const CG_Symtab* pSym, int nArgs, ST_PARAMSPEC* pArgs
)
{
    int                 i;
    CG_DECLSCOPE        ds;
    const CG_SymEntry*  pEntry;
    const CG_SymEntry*  pEntry2;
    int                 nExtMulti;
    int                 nIOEntries;

    pEntry      = pSym->pFirst;
    nIOEntries  = pSym->nParam - 1;

    for(i=0; i<nArgs; i++)
    {
        do{
            pEntry = pEntry->pNext;
            if(!pEntry)
                goto CheckCount;
            ds=pEntry->declScope;
        }while(ds!=CG_VAR_INPUT && ds!=CG_VAR_OUTPUT && ds!=CG_VAR_IN_OUT && ds!=CG_VAR_IN_EXT);
        
        assert(!pArgs[i].pFormal && !pArgs[i].bOutputArg);

        if(ds==CG_VAR_IN_EXT)
        {
            for(nExtMulti=0, pEntry2 = pEntry; i<nArgs && pEntry2; nExtMulti++, pEntry2 = pEntry2->pNext)
            {
                assert(pEntry2->declScope==CG_VAR_IN_EXT);
                nIOEntries--;
                pArgs[i++].pFormal = pEntry2;
            }

            while(i<nArgs)
            {
                for(pEntry2 = pEntry; i<nArgs && pEntry2; pEntry2 = pEntry2->pNext)
                    pArgs[i++].pFormal = pEntry2;

                if(pEntry2)//special diag: not a multiple of TODO the message text is not so smart, better use nExtMulti inside the text !
                    pdm->msg0(CG_E_MULTIPLE_VAR_IN_EXT, pArgs[nArgs-1].pActual->getEdp());
            }

            //done:
            goto CheckCount;
        }
        else
        {
            nIOEntries--;
            pArgs[i].pFormal    = pEntry;
            pArgs[i].bOutputArg = ds==CG_VAR_OUTPUT || ds==CG_VAR_IN_OUT;
        }
    }


CheckCount:
    if(i<nArgs)
    {
        //too many actual params:
        pdm->msg2(CG_E_ILL_PARAMS, pArgs[i].pActual->getEdp(), pSym->pszId, pdm->jot1()<<nArgs);
        return false;
    }
    else if(nIOEntries!=0)
    {
        //too less actual params:
        //pEntry must be !=NULL, otherwise we have a severe problem (pSym->nParam was wrong), better die...
        if(!(pEntry->declMod & CG_DMOD_OPTIONAL))
        {
            pdm->msg2(CG_E_ILL_PARAMS, pEdp, pSym->pszId, pdm->jot1()<<nArgs);
            return false;
        }
    }

    return true;
}




static bool ST_CheckFunParamTypes(
    CGT_CDiagMsg*               pdm,
    CG_CIECFront*               pFront,
    const CG_Edipos*            pEdp, 
    const CG_Symtab*            pSym, 
    int                         nArgs, 
    const ST_PARAMSPEC*         pArgs,
    const ST_FUNCTION_DEF_TYP*  pOverloads,
    ST_TypeSet_C&               funResult,
    int**                       ppArgTypes
)
{
    register int         i;
    int*                 pArgTypes;
    ST_TypeSet_C*        lpTypes;


    assert(pOverloads->nOverloads==1);
    pArgTypes = new int[nArgs+1];
    lpTypes   = new ST_TypeSet_C[pOverloads->nTypes];


    assert(nArgs==pOverloads->lpArgs[0] || (nArgs>pOverloads->lpArgs[0] && pOverloads->lpVarArg[0]!=0));

    for(i=0; i<=pOverloads->lpArgs[0]; i++)
        pArgTypes[i] = pOverloads->lpArgTypes[pOverloads->lpOffset[0]+i];

    if(pOverloads->lpArgs[0]<nArgs) // generate variable arguments as needed
    {
        int nVarArg = pOverloads->lpVarArg[0];
        int add = nArgs - pOverloads->lpArgs[0];
        int nVarArgCount = nVarArg;
        assert(nVarArg>0); //the number of args has already been checked in ST_SetParamSymEntries
                           //don't call this function after arg count errors.

        for(i=1; i<=add; i++)
        {
            pArgTypes[pOverloads->lpArgs[0]+i] = pArgTypes[nVarArgCount];
            nVarArgCount++;
            if(nVarArgCount > pOverloads->lpArgs[0])
                nVarArgCount = nVarArg;
        }
        assert(nVarArgCount==nVarArg);
    }



    for(i=0; i<pOverloads->nTypes; i++)
        lpTypes[i] = pOverloads->lpTypes[i];

    for(i=0; i<nArgs; i++)
        lpTypes[pArgTypes[i+1]] &= pArgs[i].pActual->GetType();


    bool bAnyTypErr = false;
    bool bStringErr = false;
    for(i=0; i<nArgs; i++)
    {
        if(lpTypes[pArgTypes[i+1]].IsEmptyType())
        {
            bAnyTypErr = true; 
        }
        else
        {
            long frmSLen = pArgs[i].pFormal->pType->lStrLen;
            if(frmSLen>=0)
            {
                //test string length for function param passing:
                //if form & actual have a defined length we force them to be the same, 
                //this idea is that if you specify a string length for any fun in-out-inout
                //there is good reason for it, also we had problems in the VM code gen with 
                //arrays of string if the str len is not the same. Still TODO: consider that
                //for outputs frmSlen<=actSLen woud be ok too but don't forget the VM code gen!
            
                const ST_TypeSet_C& actType = pArgs[i].pActual->GetType();
                long actSLen = pArgs[i].pActual->GetType().GetStringLength();

                if(actSLen>=0 && frmSLen!=actSLen)
                {
    	            pdm->msg4(CG_E_TYPE_PARAM, pArgs[i].pActual->getEdp(), 
                        ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId,
                        pArgs[i].pFormal->pType->pszName,
                        ST_TypeSetToErrMsgString(actType)
                    );
                    bStringErr = true; 
                }
            }
        }
    }
    
    if(bAnyTypErr)
    {
        int simple = 0; //number of simple errors
        for(i=0; i<nArgs; i++)
        {
            const ST_TypeSet_C& frmTyp = pOverloads->lpTypes[pArgTypes[i+1]];
            const ST_TypeSet_C& actTyp = pArgs[i].pActual->GetType();
            ST_TypeSet_C        tmpTyp = frmTyp;

            tmpTyp &= actTyp;
            if(tmpTyp.IsEmptyType())
            {
                pdm->msg4(CG_E_TYPE_PARAM, pArgs[i].pActual->getEdp(), 
                            ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId, 
                            ST_TypeSetToErrMsgString(frmTyp),
                            ST_TypeSetToErrMsgString(actTyp)
                           );
                simple++;
            }
        }
        if(simple == 0)
        {
            assert(pSym->flags & CG_STAB_POLYMORH);
            //TODO better diag like: not all actuals of ANY_INT have the same type (see LD/FBD)
            pdm->msg1(CG_E_TYPE_FUNCT, pEdp, pSym->pszId);
        }
    }
    else if(bStringErr)
    {
        //diag done above...
    }
    else
    {
        for(i=0; i<nArgs; i++)
        {
            if(pArgTypes[i+1] != pArgTypes[0])
            {
                 // reduce number of warnings:
                 // 1. ignore var-outs
                 // 2. ignore int-literals
                 // 3. ignore ":="
                 //
                const ST_TypeSet_C& frmTyp = lpTypes[pArgTypes[i+1]];
                ST_Expression_C*    pActEx = pArgs[i].pActual;
                
                if(!frmTyp.IsMonomorphType() && !pArgs[i].bOutputArg && !pActEx->isAny64Literal())
                {
                    pdm->msg3(CG_W_TYPE_PARAM, pActEx->getEdp(), 
                        ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId,
                        ST_TypeSetToErrMsgString(frmTyp));
                }

                pActEx->Propagate(ST_SelectBestType(pdm, &pFront->m_features, frmTyp, pActEx->getEdp()));
            }
        }
    }


    //set even if error: 
    funResult = lpTypes[pArgTypes[0]];
    delete []lpTypes;

    if(bStringErr || bAnyTypErr)
    {
        delete []pArgTypes;
        *ppArgTypes = NULL;
        return false;
    }
    else
    {
        *ppArgTypes = pArgTypes;
        return true;
    }
}

static bool ST_CheckLvalues(CGT_CDiagMsg* pdm, const CG_Symtab* pSym, int nArgs, const ST_PARAMSPEC* pArgs)
{
    int             i;
    CG_DECLSCOPE    ds;
    bool            bOk;
    
    bOk = true;
    for(i=0; i<nArgs; i++)
    {
        ds = pArgs[i].pFormal->declScope;
        if((ds==CG_VAR_OUTPUT || ds==CG_VAR_IN_OUT) && !pArgs[i].pActual->is_Lvalue())
        {
            //TODO special diag if not writable because is a for-loop var...
            pdm->msg2(CG_E_LVAL_PARAM, pArgs[i].pActual->getEdp(), ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId);
            bOk= false;
        }
    }    
    return bOk;
}


static bool ST_CheckExecDoneIF(CG_CIECFront* pFront, const CG_Symtab* pSym)
{
    const CG_SymEntry* pExec = pFront->lookUpEntry(pSym, _T("exec"));
    const CG_SymEntry* pDone = pFront->lookUpEntry(pSym, _T("done"));

    if(!pExec || !pDone)
        return false;

    if(pExec->declScope!=CG_VAR_INPUT || pExec->pType->type!=CG_BOOL || pExec->pType->bIsArray) 
        return false;
        
    if(pDone->declScope!=CG_VAR_OUTPUT || pDone->pType->type!=CG_BOOL || pDone->pType->bIsArray) 
        return false;
        
    return true;
}

static bool ST_ProcessExBuildInFun(
    CGT_CDiagMsg*       pdm, 
    CG_CIECFront*       pFront, 
    const CG_Symtab*    pSym, 
    int                 nArgs, 
    const ST_PARAMSPEC* pArgs
)
{
    int                 i;
    bool                bOk;
    const CG_DType*     pFrmTyp;
    
   
    bOk = true;
    for(i=0; i<nArgs; i++)
    {
        pFrmTyp = pArgs[i].pFormal->pType;

        if(pFrmTyp->type == CG_ANY_WAITABLE)
        {
            const ST_Expression_C*  pActEx  = pArgs[i].pActual;
            const ST_TypeSet_C&     actType = pActEx->GetType();
            CG_BITSET               lTypeSet= actType.GetTypeSet();

            if(lTypeSet==CG_TBS_BOOL)
            {
                //ok, either simple var or expression that is bool.
            }
            else if(lTypeSet==CG_TBS_VOID)
            {
                const CG_Symtab*                pSymFB;
                const FC_CGArray<ST_PARAMSPEC>* pArgLst;
                pActEx->getCalledPOU(&pSymFB, &pArgLst);

                if(!pSymFB || pSymFB->pouTyp!=CG_POU_FB)
                {
                    bOk = false;
                    pdm->msg4(CG_E_TYPE_PARAM, pActEx->getEdp(), ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId, 
                                               pFrmTyp->pszName, ST_TypeSetToErrMsgString(actType)
                             );
                }
                else if(!ST_CheckExecDoneIF(pFront, pSymFB))
                {
                    bOk = false;
                    pdm->msg4(CG_E_TYPE_PARAM, pActEx->getEdp(), ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId, 
                                               pFrmTyp->pszName, _T("FB that does not have EXEC-DONE Interface")
                             );
                }
                else
                {
                    int                 j;
                    int                 nArgsFBC = pArgLst->getUsed();
                    const ST_PARAMSPEC* pArgsFBC = pArgLst->getCElements();

                    for(j=0; j<nArgsFBC; j++)
                    {
                        //test exec :=,  (done => var  may be useful !)
                        assert(pArgsFBC[j].pszFormal);
                        if(pArgsFBC[j].pszFormal && !_tcsicmp(pArgsFBC[j].pszFormal, _T("exec")))
                            pdm->msg2(CG_W_EXPR_NO_EFFECT, &pArgsFBC[j].edpFormal, _T("exec:="), _T("in ANY_WAITABLE FB call"));
                    }
                }
            }
            else
            {
                bOk = false;
                pdm->msg4(CG_E_TYPE_PARAM, pActEx->getEdp(), ArgDiagID(pdm->jot1(), &pArgs[i], i), pSym->pszId, 
                                           pFrmTyp->pszName, ST_TypeSetToErrMsgString(actType)
                         );
            }
        }
        //else: more ideas ?
    }
    
    return bOk;
}

bool ST_CFunction::checkCall()
{
    const CG_Symtab*            pSym;
    const ST_FUNCTION_DEF_TYP*  pFDT;
    int                         nArgs;
    ST_PARAMSPEC*               pArgs;
    
    pSym  = m_pSymPrv->getCGSym();
    nArgs = m_actuals.getUsed();
    pArgs = m_actuals.getElements();
    pFDT  = m_pSymPrv->getFunIF();    

    if(!ST_SetParamSymEntries(m_pdm, getEdp(), pSym, nArgs, pArgs))
        return false;

    if(!ST_CheckFunParamTypes(m_pdm, m_pFront, getEdp(), pSym, nArgs, pArgs, pFDT, typeset, &m_pPropIdx))
        return false;

    if(!ST_CheckLvalues(m_pdm, pSym, nArgs, pArgs))
        return false;

    if(!ST_ProcessExBuildInFun(m_pdm, m_pFront, pSym, nArgs, pArgs))
        return false;

    m_bCheckOK = true;
    return true;
}

void ST_CFunction::Propagate(const ST_TypeSet_C &what)
{
    int                   i;
    int                   nArgs;
    const ST_PARAMSPEC*   pAS;

    assert( !(typeset &= what).IsEmptyType() && what.IsMonomorphType());
    
    typeset = what;

    if(m_pPropIdx)
    {
        nArgs = m_actuals.getUsed();
        pAS   = m_actuals.getCElements();
        for(i=0; i<nArgs; i++)
        {
            if(m_pPropIdx[i+1] == m_pPropIdx[0])
                pAS[i].pActual->Propagate(typeset);
        }
    }
}

void ST_CFunction::noImplemErrMsg()const
{
    int         i;
    FC_CString  jot(MAX_PATH);

    jot<<ST_TypeSetToErrMsgString(GetType())<<' '<<m_pSymPrv->getCGSym()->pszId<<'(';

    for(i=0; i<m_actuals.getUsed(); i++)
    {
        if(i>0)
            jot<<',';
        jot<<ST_TypeSetToErrMsgString(m_actuals.get(i).pActual->GetType());
    }
    jot<<')';
    m_pdm->msg1(CG_E_FUN_OVERLOAD_NO_BODY, &m_edp, jot);
}

const TCHAR* ST_CFunction::getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const
{
    if(ppSym)
        *ppSym = m_pSymPrv->getCGSym();
    if(ppArgList)
        *ppArgList = &m_actuals;
    return m_pSymPrv->getCGSym()->pszId;
}



static CG_ExprTree* ST_InitCGExpr(
    ST_CExprMem*     pAlloc,
    CG_ExprTree*     pReady,
    CG_EXPRTYP       extyp,
    const CG_Edipos* pEdp,
    const CG_DType*  pType
)
{
    if(pReady)
    {
        memset(pReady, 0, sizeof(*pReady));
        pReady->edp   = *pEdp;
        pReady->extyp = extyp;
        pReady->pType = pType;
        return pReady;
    }
    return pAlloc->registerCGExpr(extyp, pEdp, pType);
}

static const CG_ExprTree* ST_CreateTypeCast(//TODO much better to make type casts as build in operators
    st_ParseContext*   pfc,                 //that would also allow some optimizations if dint_to_int(98)
    ST_CExprMem*       pAlloc,
    CG_ExprTree*       pReady,
    const CG_ExprTree* pExpr, 
    const CG_DType*    pToType
)
{
    FC_CString                 fName(MAX_PATH);
    const ST_FUNCTION_DEF_TYP* pOverloads;
    ST_CSymtabPrv*             pSymFunPrv;
    const CG_Symtab*           pSymFun;
    long                       lres;
    CGT_CDiagMsg*              pdm        = pfc->getDM();
    ST_CSymtabPrv*             pSymtabPrv = pfc->getSym();
    const CG_Symtab*           pSymtab    = pfc->getCGSym();
    ST_CBody*                  pBody      = pfc->getCBody();

    fName<<CGT_DTYPES_ToString(pExpr->pType->type); //avoid string[45] in fun name
    fName<<_T("_TO_")<<CGT_DTYPES_ToString(pToType->type);

    lres = pfc->getFront()->loadHeaderEx(&pExpr->edp, fName, ST_WANT_FUN, &pSymFunPrv, &pOverloads);
    //if trg typ not supported error, carry on. All other errors ignore the fun call:
    if(lres!=ST_LHDR_OK && lres!=ST_LDHD_TRG_NS)
    {
        if(lres & ST_LDHD_ERRIND) //indirect error update internal err counter
            pfc->indirectError();
        return NULL;
    }
    if(!pSymFunPrv)
        CGT_IERR2(pdm, &pExpr->edp, _T("not supported <builtin function used as implicit type cast"));
    assert(!pOverloads);

    pSymFun = pSymFunPrv->getCGSym();
    assert(pSymFun->pouTyp==CG_POU_FUN);

    //update counters of context if any:
    if(pBody)//there is no body if parse singe expr is used...
        pBody->m_callFun.insert(pSymFun, 0);

    assert(pSymtabPrv && pSymtab);//<-TODO fun call without pSymtabPrv, is this possible?
    if(pSymtabPrv && pSymtab)
    {
        if((pSymtab->flags & CG_STAB_BDY_LOADED)==0)
            pSymtabPrv->registerUsedType(pSymFun);
        else
        {
            assert(pSymtab->pzRefSyms);
#ifdef DEBUG
            if(pSymtab->pzRefSyms)
            {
                for(const CG_Symtab*const* ppSym = pSymtab->pzRefSyms; *ppSym; ppSym++)
                {
                    if(pSymFun == *ppSym)
                        break;
                }
                if(pSymFun != *ppSym)
                    CGT_IERR2(pdm, &pExpr->edp, pdm->jot1()<<_T("reload body: new reference to function '")<<pSymFun->pszId<<_T("' (body changed?)"));
            }
#endif 
        }
    }

    if(!pSymFun->pFirst->pNext || pSymFun->pFirst->pNext->pNext || pSymFun->pFirst->pNext->declScope!=CG_VAR_INPUT)
        CGT_IERR2(pdm, &pExpr->edp, _T("invalid implicit type cast"));

    CG_ArgExpr* pNewArgList;
    FC_NEW_ZASTRUCT(pNewArgList, CG_ArgExpr, 1);
    pNewArgList[0].pFormal = pSymFun->pFirst->pNext;
    pNewArgList[0].pActual = pExpr;
    CG_SetAccFlags(pdm, pExpr, CG_ACCESS_I);//must be VARIN !

    CG_ExprTree* pRet;
    pRet = ST_InitCGExpr(pAlloc,pReady, CG_FUNMACRO, &pExpr->edp, pToType);
    pRet->mac.pMac  = pSymFunPrv->getNonPolyMacroBody();
    pRet->mac.nArgs = 1;
    pRet->mac.pArgs = pAlloc->registerArgExprList(pNewArgList);

    if(!pRet->mac.pMac)
        CGT_IERR2(pdm, &pExpr->edp, _T("invalid implicit type cast"));

    return pRet;
}


const CG_ExprTree* ST_CFunction::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    int                   i;
    int                   nArgs;
    const ST_PARAMSPEC*   pArgs;
    const CG_MacroBody*   pMacDescr;
    const CG_DType*       pRCast;
    int                   nACasts;
    const CG_DType*const* pACasts;
    const CG_ArgExpr*     pArgListReg;
    CG_ExprTree*          pRet;



    assert(m_bCheckOK);

    nArgs = m_actuals.getUsed();
    pArgs = m_actuals.getCElements();
    
    
    pMacDescr = NULL;
    pRCast    = NULL;
    nACasts   = 0;
    pACasts   = NULL;
    if(m_pSymPrv->isMacro())
    {
        if(m_pSymPrv->isPolyMorph())
            pMacDescr = m_pSymPrv->lookUpFunMacroBody(GetType(), nArgs, pArgs, &pRCast, &nACasts, &pACasts);
        else
            pMacDescr = m_pSymPrv->getNonPolyMacroBody();//consider: pRCast, nACast, pACast also here ?

        if(!pMacDescr)
            noImplemErrMsg(); //carry on...
    }


    if(nArgs>0)
    {
        WORD                  accFlags;    
        const CG_ExprTree*    pEx;
        CG_ArgExpr*     pNewArgList;

        //alloc get argExpr<->Entry array:
        FC_NEW_ZASTRUCT(pNewArgList, CG_ArgExpr, nArgs);

        pArgs   = m_actuals.getCElements();
        
        for(i=0; i<nArgs; i++)
        {
            pEx = pArgs[i].pActual->makeCGExpr(pAlloc, NULL);

            if(i<nACasts && pACasts[i] && pACasts[i]->type != pEx->pType->type)
                pEx = ST_CreateTypeCast(getPfc(), pAlloc, NULL, pEx, pACasts[i]);

            switch(pArgs[i].pFormal->declScope)
            {
            case CG_VAR_IN_OUT:  accFlags = CG_ACCESS_X; break;
            case CG_VAR_IN_EXT: 
            case CG_VAR_INPUT:   accFlags = CG_ACCESS_I; break;
            case CG_VAR_OUTPUT:  accFlags = CG_ACCESS_O; break;
            default: 
                assert(!"bad decl scope for fun param");
            }
            CG_SetAccFlags(m_pdm, pEx, accFlags);

            assert(pArgs[i].pFormal);
            pNewArgList[i].pFormal = pArgs[i].pFormal;
            pNewArgList[i].pActual = pEx;
        }
        //register as body global mem:
        pArgListReg = pAlloc->registerArgExprList(pNewArgList);
    }
    else
    {
        pArgListReg = NULL;
    }

    
    if(pMacDescr)
    {
        pRet = initCGExpr(pAlloc, pReady, CG_FUNMACRO);
        pRet->mac.pMac  = pMacDescr;
        pRet->mac.nArgs = nArgs;
        pRet->mac.pArgs = pArgListReg;
    }
    else
    {
        pRet = initCGExpr(pAlloc, pReady, CG_FUNCALL);
        pRet->fun.pSym  = m_pSymPrv->getCGSym();
        pRet->fun.nArgs = nArgs;
        pRet->fun.pArgs = pArgListReg;
    }

    if(pRCast && pRCast->type != pRet->pType->type)
    {
        const CG_DType* pWantType = pRet->pType;
        pRet->pType = pRCast;
        return ST_CreateTypeCast(getPfc(), pAlloc, pReady, pRet, pWantType);
    }
    return pRet;
}


ST_CFBCallExpr::ST_CFBCallExpr(
    const CG_Edipos*    pEdp,  
    st_ParseContext*    pfc,
    const CG_SymEntry*  pFBIEntry,
    ST_CParamList*      pParList
) : ST_Expression_C(pEdp, pfc)
{
    if(pParList)
        pParList->Detach();
    m_pFBIEntry  = pFBIEntry;
    m_instAccFlags = 0;

    if(pParList)
    {
        pParList->m_list.ownmemto(&m_parlist);
        delete(pParList);
    }
}

ST_CFBCallExpr::~ST_CFBCallExpr()
{
    for(int i=m_parlist.getUsed()-1; i>=0; i--)
        delete(m_parlist.get(i).pActual);
}

bool ST_CFBCallExpr::checkFBCall()
{
    int                                 i;
    bool                                bOk  = true;
    int                                 nPar = m_parlist.getUsed();
    const ST_PARAMSPEC*                 pPar = m_parlist.getElements();
    FC_CGArray<const CG_SymEntry*>      inoutsAssigned(nPar);
    const CG_Symtab*                    pSymFB;

    typeset = CG_TBS_VOID;
    //check the entry properties:
    //FB call:
    if(m_pFBIEntry->pType->type != CG_INSTANCE || m_pFBIEntry->pType->bIsArray)
    {
        m_pdm->msg0(CG_E_ILLEGAL_CALL, getEdp());
        bOk = false; 
    }
    else
    {
        pSymFB = m_pFBIEntry->pType->pSym;

        if(m_pFBIEntry->declScope == CG_VAR_INPUT)
        {
            if(m_pfc->getCGSym()->pouTyp==CG_POU_FUN)
            {
                // error: fb-call in function
                m_pdm->msg0(CG_E_INPUT_FB_CALL_IN_FUN, getEdp());
                bOk = false; 
            }
            else
            {
                // warning
                m_pdm->msg0(CG_W_INPUT_FB_CALL, getEdp());
            }
        }
    }

    if(bOk && nPar)
    {
        for(i=0; i<nPar; i++)
        {
            if(!addParam(pSymFB, &pPar[i], inoutsAssigned))
                bOk = false;
        }
    }


    if(bOk && pSymFB->nInOut != inoutsAssigned.getUsed())
    {
        const CG_SymEntry* pEntry   = pSymFB->pFirst;
        int                nInError = 0;
        int                nChecked = 0;

        //for a better err msg find the inout(s) that are not assigned:
        for( ;pEntry && nChecked!=pSymFB->nInOut; pEntry=pEntry->pNext)
        {
            if(pEntry->declScope != CG_VAR_IN_OUT)
                continue;
    
            nChecked++;
            if(inoutsAssigned.findElem(pEntry)<0)
            {
                m_pdm->msg1(CG_E_INIT_MISSING, getEdp(), pEntry->pszId);
                nInError++;
            }
        }
        assert(pSymFB->nInOut == (inoutsAssigned.getUsed() + nInError));
        bOk = false;
    }

    return bOk;
}


bool ST_CFBCallExpr::addParam(const CG_Symtab* pSymFB, const ST_PARAMSPEC* pPS, FC_CGArray<const CG_SymEntry*>& inoutsAssigned)
{
    CG_ArgExpr*         pArg;
    ST_TypeSet_C        type;
    const CG_SymEntry*  pEntry;
    const CG_Symtab*    pSym;
    const ST_TypeSet_C* pActType;
    
    
    if(!pPS->pszFormal)
    {
        m_pdm->msg1(CG_E_SYNTAX_EXPECTED, pPS->pActual->getEdp(), _T("<identifier> :="));
        return false;
    }

    //is pId an accessible member of the FB type ? 
    pEntry = m_pFront->lookUpEntry(pSymFB, pPS->pszFormal);
    if(!pEntry)
    {
        m_pdm->msg1(CG_E_UNKNOWN_MEMBER, &pPS->edpFormal, pPS->pszFormal);
        return false;
    }


    //check storage class:
    if(pPS->bOut)
    {
        if(pEntry->declScope != CG_VAR_OUTPUT && pEntry->declScope != CG_VAR_IN_OUT)
        {
            m_pdm->msg1(CG_E_MEMBER_NOT_OUT, &pPS->edpFormal, pPS->pszFormal);
            return false;
        }
    }
    else
    {
        if(pEntry->declScope != CG_VAR_IN_OUT && pEntry->declScope != CG_VAR_INPUT)
        {
            m_pdm->msg1(CG_E_ILL_MEMBER, &pPS->edpFormal, pPS->pszFormal);
            return false;
        }
    }

    if(pEntry->declScope == CG_VAR_IN_OUT || pEntry->declScope == CG_VAR_OUTPUT)
    {
        //expr must be lval:
        if(!pPS->pActual->is_Lvalue())
        {
            m_pdm->msg1(CG_E_LVAL_MEMBER, pPS->pActual->getEdp(), pPS->pszFormal);
            return false;
        }
    }

    pActType = pPS->pActual->getTypePtr();
    //check data type match:
    type.initFromCG_DType(pEntry->pType);
    type &= *pActType;

    if(type.IsEmptyType())
    {
        m_pdm->msg3(CG_E_TYPE_MEMBER, pPS->pActual->getEdp(), 
            pEntry->pType->pszName, 
            pPS->bOut ? _T("=>") : _T(":="),
            ST_TypeSetToErrMsgString(*pActType)
            );
        return false;
    }
    
	//warn if argument is a literal string longer than specified formal 
    //parameter string len, if any:
	if(pActType->GetStringLength()>=0 && pEntry->pType->lStrLen>=0)
    {
        if (pEntry->pType->lStrLen != pActType->GetStringLength())
        {
            if (pEntry->declScope==CG_VAR_IN_OUT)
            {
                m_pdm->msg3(CG_E_TYPE_MEMBER, pPS->pActual->getEdp(), 
                    pEntry->pType->pszName,
                    pPS->bOut ? _T("=>") : _T(":="),
                    ST_TypeSetToErrMsgString(*pActType)
                   );
                return false;
            }
            else if (pPS->pActual->getLitValue() && pEntry->pType->lStrLen < pActType->GetStringLength())
            {
    		    m_pdm->msg2(CG_W_STR_TRUNC, pPS->pActual->getEdp(), 
                    ST_TypeSetToErrMsgString(*pActType), pEntry->pType->pszName);            
            }
        }
	}   

    pPS->pActual->Propagate(type);
    pPS->pActual->fixType();


    //check the "deep copy assignmet disable" feature:
    pSym = pEntry->pType->pSym;
    if(pSym && pEntry->declScope!=CG_VAR_IN_OUT && (pSym->flags & CG_STAB_DIAG_ON_COPY))
        ST_OnDeepCopy(m_pFront, m_pdm, pPS->pActual->getEdp(), pSym);


    //check that input member only used once in current call:
    if(!pPS->bOut)
    {
        for(int i=0; i<m_CGArglist.getUsed(); i++)
        {
            if(m_CGArglist.get(i).pFormal==pEntry)
            {
                m_pdm->msg1(CG_E_MULT_MEMBER, &pPS->edpFormal, pPS->pszFormal);
                return false;
            }
        }

        //remember count of inout assignments, but not if '=>': 
        if(pEntry->declScope == CG_VAR_IN_OUT)
            inoutsAssigned.add(pEntry);
    }



    if(m_pfc->getErrorsInFile()==0)
    {
        //code gen if no error:
        if(m_CGArglist.getUsed()==0)
            m_CGArglist.allocSize(64);

        pArg = m_CGArglist.addZero();
        pArg->pFormal = pEntry;
        pArg->pActual = pPS->pActual->makeCGExpr(&m_pfc->getCBody()->m_exprMem, NULL);

        if(pEntry->declScope == CG_VAR_IN_OUT)
        {
            assert(!pPS->bNeg);//LDFBD must report this error
            //special case '=>' read comment at CG_ArgExpr def in CG_IECFront.h:
            CG_SetAccFlags(m_pdm, pArg->pActual, pPS->bOut ? CG_ACCESS_W : CG_ACCESS_X);
            //set consequently no bit in m_instAccFlags the inout is a pointer!
        }
        else if(pEntry->declScope == CG_VAR_INPUT)
        {
            assert(!pPS->bNeg);//LDFBD must insert a NOT before the actual expr.
            CG_SetAccFlags(m_pdm, pArg->pActual, CG_ACCESS_R);
            m_instAccFlags |= CG_ACCESS_w;
        }
        else if(pEntry->declScope == CG_VAR_OUTPUT)
        {
            CG_SetAccFlags(m_pdm, pArg->pActual, CG_ACCESS_W);
            m_instAccFlags |= CG_ACCESS_r;
            
            if(pPS->bNeg && pPS->bOut) //LDFBD must currently insert a tmp var!
                pArg->argFlags = CG_EXPR_ARG_NEGOUT;
        }
        else
        {
            assert(!"bad declscope for param");
        }

        //set the output assign bit (but not for the act arg, only the arg expr!)
        if(pPS->bOut)
            pArg->argFlags |= CG_EXPR_ARG_OUTASN;
    }

    return true;
}

int ST_CFBCallExpr::is_Lvalue()const
{
    return true;
}

const TCHAR* ST_CFBCallExpr::getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const//##AC
{
    if(ppSym)
        *ppSym = m_pFBIEntry->pType->pSym;
    if(ppArgList)
        *ppArgList = &m_parlist;
    return m_pFBIEntry->pType->pSym->pszId;
}

const CG_ExprTree* ST_CFBCallExpr::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_ExprTree*     pRet;
    CG_ExprTree*     pVar;

    pRet = initCGExpr(pAlloc, pReady, CG_FBCALLEXPR);

    pVar = pAlloc->registerCGExpr(CG_VARREF, &m_edp, m_pFBIEntry->pType);
    pVar->var.pEntry = m_pFBIEntry;
    
    //the FBI expr is accessed r if only output, w if inputs
    //but aleways C just by the call itself. (Inout don't count 
    //as r or w):
    CG_SetAccFlags(m_pdm, pVar, m_instAccFlags | CG_ACCESS_C);

    pRet->fbc.pInst = pVar;
    pRet->fbc.nArgs = m_CGArglist.getUsed();
    pRet->fbc.pArgs = pAlloc->registerArgExprList(m_CGArglist.getMemCopy());
    return pRet;
}



ST_ArrayAccess_C::ST_ArrayAccess_C(
    st_ParseContext*     pfc,
    ST_Expression_C*     pVar, 
    ST_ExpressionList_C* pSublist
)
: ST_Expression_C(pVar->getEdp(), pfc)
{
    pVar->Detach();
    pSublist->Detach();
    
    basevariable = pVar;
    subscripts = pSublist;

    int nActDims = pSublist->m_list.getUsed(); //##AC
    int nDims    = basevariable->GetType().GetArrayDim();
    int bAssumedSize = (nDims==0);

    if(basevariable->GetType().IsMonomorphType() &&
        basevariable->GetType().GetTypeSet() & CG_TBS_ARRAY)
    {
        typeset = basevariable->GetType().GetArrayBase();

        if(nActDims==nDims || (bAssumedSize && nActDims==1))
        {
            int pos = 0;
            for( ; pos<nActDims; )//##AC
            {
                __int64				intval;
                ST_SUBRANGE_TYP	    range;
                ST_Expression_C*	pSubs = pSublist->m_list.get(pos);//##AC

                ST_TypeSet_C type = pSubs->GetType();
                type &= CG_TBS_ANY_INT;

                if (type.IsEmptyType())
                {
                    m_pdm->msg1(CG_E_TYPE_SUBSCR, &m_edp, m_pdm->jot1()<<pos+1);
                }
                else 
                {
                    bool bIsLit = pSubs->isAny64Literal(&intval); 

                    if(!type.IsMonomorphType() && !bIsLit)
                    {
                        m_pdm->msg2(CG_W_TYPE_SUBSCR, &m_edp, m_pdm->jot1()<<pos+1, ST_TypeSetToErrMsgString(type));
                    }
                    
                    pSubs->fixType();

                    if(!bAssumedSize && bIsLit && basevariable->GetType().GetArrayRange(pos, &range))
                    {
                        if((unsigned long)(intval - range.lRangeStart)>range.ulRangeLength)
	                        m_pdm->msg1(CG_E_RANGE_SUBSCR, &m_edp, m_pdm->jot1()<<pos+1);
                    }
                }
                pos++;
            }
        }
        else 
        {
            m_pdm->msg1(CG_E_ILL_DIMENSION, &m_edp, m_pdm->jot1()<<(bAssumedSize ? 1 : nDims));
        }

    }
    else
    {
        m_pdm->msg0(CG_E_NON_ARRAY, &m_edp);
        typeset = basevariable->GetType();
    }

}
const CG_SymEntry* ST_ArrayAccess_C::getSymEntry()const
{
    assert(basevariable);
    return basevariable?basevariable->getSymEntry(): NULL;
}

ST_ArrayAccess_C::~ST_ArrayAccess_C()
{
    delete basevariable;
    delete subscripts;
}
ST_ArrayAccess_C::is_Lvalue() const
{
    return basevariable->is_Lvalue();
}
const CG_ExprTree* ST_ArrayAccess_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
//##AC beg
    CG_ExprTree*     pRet;

    assert(basevariable);

    pRet = initCGExpr(pAlloc, pReady, CG_ARRAYREF);

    pRet->arr.pBase = basevariable->makeCGExpr(pAlloc, NULL);
    assert(subscripts->m_list.getUsed()==1);

    pRet->arr.nSubs = 1;
    pRet->arr.pSubs = subscripts->m_list.get(0)->makeCGExpr(pAlloc, NULL);
    CG_SetAccFlags(m_pdm, pRet->arr.pSubs, CG_ACCESS_R);
//##AC end

    return pRet;
}







ST_StructAccess_C::ST_StructAccess_C(
    st_ParseContext* pfc,
    ST_Expression_C* pStructVar, 
    ST_Identifier_C* pSelector
)
 : ST_Expression_C(pStructVar->getEdp(), pfc)
{
    pStructVar->Detach();

    m_basevariable = pStructVar;
    m_pEntry = NULL;


    const ST_TypeSet_C& baseTypeSet = m_basevariable->GetType();
    CG_BITSET lBaseTypeSet = baseTypeSet.GetTypeSet();
    

    if (baseTypeSet.IsMonomorphType() && (lBaseTypeSet & (CG_TBS_STRUCT|CG_TBS_INSTANCE)))
    {
        ST_MEMORY_CLASS      cls;
        const ST_CSymtabPrv* pSymPrv;
        bool                 bRecursive;

        pSymPrv = m_pFront->loadHeader(baseTypeSet.GetTypeName(), &bRecursive);
        assert(pSymPrv); //can this ever happen:
                         //data types are only created if symtab error free!!
        if(!pSymPrv)
        {
            m_pdm->msg1(CG_E_NO_DEF_FOUND, &m_edp, 
                baseTypeSet.GetTypeName());
            typeset = CG_TBS_ANY;
            m_bLvalue = true;
            delete pSelector;
            return;
        }

        m_pEntry = pSymPrv->lookUpEntry(pSelector->getName());
        
        if(m_pEntry)
        {
            typeset.initFromCG_DType(m_pEntry->pType);
            if(lBaseTypeSet & CG_TBS_STRUCT)
                //Structures do only have ST_VAR
                cls = ST_VAR;
            else
                //get FB member access right from storage class
                cls = ST_StoreClassToFBMemberAccess(m_pEntry->declScope);
           
        }
        else
        {
            m_pdm->msg2(CG_E_UNDEF_MEMBER, pSelector->getEdp(), pSelector->getName(),
                         baseTypeSet.GetTypeName());

            typeset = CG_TBS_ANY;
            cls = ST_VAR;
        }

        m_bLvalue = cls != ST_VAR_OUT && m_basevariable->is_Lvalue();

        
        if(m_bLvalue && m_pEntry)
        {
            const TCHAR* pszAddr = m_pEntry->pszAddr;
            if(pszAddr && CGT_GetAddrLocation(pszAddr)==_T('I'))
                m_bLvalue = false; //no lval if struct member AT %Input:
            
            if(m_pEntry->declMod & CG_DMOD_CONSTANT)
                m_bLvalue = false; //IEC Extension constant decl on struct member
        }


        if (cls != ST_VAR && cls != ST_VAR_OUT)
        {
            HRESULT errid = cls==ST_VAR_PRIVATE ? CG_E_PRIV_MEMBER : CG_E_INPUT_MEMBER;
            m_pdm->msg2(errid, pSelector->getEdp(), pSelector->getName(), baseTypeSet.GetTypeName());
        }
    }
    else
    {
        //assert: should not be created if base is error expr:
        assert(!m_basevariable->isErrorExpr());
        m_pdm->msg0(CG_E_NON_STRUCT, m_basevariable->getEdp());

        typeset = CG_TBS_DINT;
        m_bLvalue = true;
    }

    delete pSelector;
}

ST_StructAccess_C::~ST_StructAccess_C()
{
  delete m_basevariable;
}
const CG_SymEntry* ST_StructAccess_C::getSymEntry()const
{
    return m_pEntry;
}
int ST_StructAccess_C::is_Lvalue() const
{
    return m_bLvalue==1 ? true : false;
}

ST_StructAccess_C* ST_StructAccess_C::toStructAccess()
{
    return this;
}
const CG_ExprTree* ST_StructAccess_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    CG_ExprTree*     pRet;

    pRet = initCGExpr(pAlloc, pReady, CG_STRUCTREF);
    pRet->str.pBase = m_basevariable->makeCGExpr(pAlloc, NULL);
    pRet->str.pSelc = m_pEntry;
    return pRet;
}






ST_Function_C::ST_Function_C(
    const CG_Edipos*           pEdp,
    st_ParseContext*           pfc,
    CG_EXPRTYP                 opEnum, 
    ST_CParamList*             pArglist, //##AC
    const ST_CSymtabPrv*       pSymOfFun,
    const ST_FUNCTION_DEF_TYP* pOverloads
)
: ST_Expression_C(pEdp, pfc) 
{
    assert(pOverloads && !(pSymOfFun && opEnum!=CG_EXPRTYP_UNDEF)); 
    
    if(pArglist) //##AC
    {
        pArglist->m_list.ownmemto(&m_actuals);
        delete(pArglist);
    }
    lpArgTypes = NULL;

    //##AC
    m_opEnum = opEnum;
    m_pSymOfFun  = pSymOfFun;
    m_pOverloads = pOverloads;

    assert(!pSymOfFun || !_tcsicmp(m_pOverloads->pszId, pSymOfFun->getCGSym()->pszId));
}
ST_Function_C::~ST_Function_C()
{
    for(int i=m_actuals.getUsed()-1; i>=0; i--)//##AC
        delete(m_actuals.get(i).pActual);
    if (lpArgTypes)
        delete []lpArgTypes;
}

//##AC beg
bool ST_Function_C::selectOverload()
{
    register int         i;
    int                  nArgs;
    ST_PARAMSPEC*        pArgs;
    int                  nVarArg;
    int                  nTypeSet;
    bool                 bError = false;
    ST_TypeSet_C*        lpTypes;
    int                  any_overloads= 0;
    const CG_SymEntry*   pEntry;


    nArgs = m_actuals.getUsed();
    pArgs = m_actuals.getElements();
    
    assert(!lpArgTypes);
    lpArgTypes = new int[nArgs+1];
    lpTypes = new ST_TypeSet_C[m_pOverloads->nTypes];


    for (nTypeSet = 0; nTypeSet<m_pOverloads->nOverloads; nTypeSet++)
    {
        //check if the number of actual args is not less than 
        //the number of formal parameters, if so no match possible
        if (nArgs<m_pOverloads->lpArgs[nTypeSet])
            continue;

        //more actual params than formal params, there must be 'varargs'
        if (m_pOverloads->lpVarArg[nTypeSet] == 0 &&
            nArgs>m_pOverloads->lpArgs[nTypeSet])
            continue;

        any_overloads++;

        nVarArg = m_pOverloads->lpVarArg[nTypeSet];

        for (i=0; i<=m_pOverloads->lpArgs[nTypeSet]; i++)
            lpArgTypes[i] = m_pOverloads->lpArgTypes[m_pOverloads->lpOffset[nTypeSet]+i];

        if (m_pOverloads->lpArgs[nTypeSet]<nArgs) // generate variable arguments as needed
        {
            assert(nVarArg>0);
            int add = nArgs - m_pOverloads->lpArgs[nTypeSet];
            // ++**++ JD extInParam -->
            // The following loop is never used,
            // it is implemented for an unused feature: to have a fixed number of vars 
            // after all var input extended
            // This will no longer work like this with multiple var extended inputs
            //for (i=m_pOverloads->lpArgs[nTypeSet]; i>nVarArg; i--)
            //    lpArgTypes[i+add] = lpArgTypes[i];
            int nVarArgCount = nVarArg;
            for (i=1; i<=add; i++)
            {
                lpArgTypes[m_pOverloads->lpArgs[nTypeSet]+i] = lpArgTypes[nVarArgCount];
                nVarArgCount++;
                if (nVarArgCount > m_pOverloads->lpArgs[nTypeSet])
                {
                    nVarArgCount = nVarArg;
                }
            }
            if (nVarArgCount!=nVarArg)
            {   //diag at this point reqires only 1 overload, but
                //that is always the case, only some ops have more
                m_pdm->msg0(CG_E_MULTIPLE_VAR_IN_EXT, getEdp());
                assert(m_pOverloads->nOverloads==1);
                break;
            }
            // ++**++ JD extInParam <--
        }

        for (i=0; i<m_pOverloads->nTypes; i++)
            lpTypes[i] = m_pOverloads->lpTypes[i];

        for (i=0; i<nArgs; i++)
            lpTypes[lpArgTypes[i+1]] &= pArgs[i].pActual->GetType();

        for (i=0; i<nArgs; i++)
        {
            if (lpTypes[lpArgTypes[i+1]].IsEmptyType())
                break;
        }
        
        if (i<nArgs)
            continue;   // no acceptable overload, at least one type is VOID

        if (m_opEnum==CG_MOVE)
        {
            //warn if right value is a literal string longer than left value string len, if any:
            const ST_Expression_C* pLeft  = pArgs[0].pActual;
            const ST_Expression_C* pRight = pArgs[1].pActual;

	        if(pRight->getLitValue() && pRight->GetType().GetStringLength()>=0 && pLeft->GetType().GetStringLength()>=0)
            {
                if (pLeft->GetType().GetStringLength() < pRight->GetType().GetStringLength())
    		        m_pdm->msg2(CG_W_STR_TRUNC, pRight->getEdp(), 
                        ST_TypeSetToErrMsgString(pRight->GetType()), 
                        ST_TypeSetToErrMsgString(pLeft->GetType()));            
	        }               
        }


        if(m_pSymOfFun)
        {
            pEntry = m_pSymOfFun->getCGSym()->pFirst;
            pEntry = pEntry->pNext;// skip return value
        }
        else
        {
            pEntry = NULL;
        }

        for (i=0; i<nArgs; i++)
        {
            ST_PARAMSPEC* pAS = &pArgs[i]; //##AC

            //set formal sym entry if user function:
            if(m_pSymOfFun)
            {
                //skip all non in/out entries
                assert(pEntry);
                while(pEntry->declScope!=CG_VAR_INPUT && 
                      pEntry->declScope!=CG_VAR_OUTPUT &&
                      pEntry->declScope!=CG_VAR_IN_OUT &&
                      pEntry->declScope!=CG_VAR_IN_EXT)
                {
                    pEntry = pEntry->pNext;
                }
                
                assert(!pAS->pFormal && !pAS->bOutputArg && pEntry); //##AC
                pAS->pFormal = pEntry;                               //##AC
                pAS->bOutputArg = pEntry->declScope==CG_VAR_OUTPUT || pEntry->declScope==CG_VAR_IN_OUT;//##AC
                
                if(pEntry->declScope!=CG_VAR_IN_EXT)
                    pEntry = pEntry->pNext;
            }
            else if(i==0 && m_opEnum==CG_MOVE) //##AC
            {   assert(!pAS->bOutputArg);      //##AC
                pAS->bOutputArg = true;        //##AC
            }                                  //##AC

            ST_Expression_C*    pAEx    = pAS->pActual; //##AC
            const CG_Edipos*    pAEdp   = pAEx->getEdp();

            //test string length for function param passing:
            if(pAS->pFormal)//##AC
            {
                long lStrlenFormal = pAS->pFormal->pType->lStrLen;//##AC
                if(lStrlenFormal>=0)
                {
                    long lStrlenActual = pAEx->GetType().GetStringLength();
                    if(lStrlenActual>=0 && lStrlenFormal!=lStrlenActual)
                    {
    	                m_pdm->msg4(CG_E_TYPE_PARAM, pAEdp, 
                            m_pdm->jot1()<<_T("'")<<pAS->pFormal->pszId<<_T("'"), getFunName(),//##AC
                            pAS->pFormal->pType->pszName, //##AC
                            ST_TypeSetToErrMsgString(pAEx->GetType())
                        );
                    }
                }
            }

            if (lpArgTypes[i+1] != lpArgTypes[0])
            {
                /*
                 * reduce number of warnings:
                 * 1. ignore var-outs
                 * 2. ignore int-literals
                 * 3. ignore ":="
                 */
                const ST_TypeSet_C& formType= lpTypes[lpArgTypes[i+1]];
                pAEx->Propagate(ST_SelectBestType(m_pdm, &m_pFront->m_features, formType, pAEdp));

                if(m_opEnum!=CG_MOVE && !formType.IsMonomorphType() && !pAS->bOutputArg && !pAEx->isAny64Literal())
                {
                    m_pdm->msg3(CG_W_TYPE_PARAM, pAEdp, ArgDiagID(m_pdm->jot1(), pAS, i), getFunName(),
                        ST_TypeSetToErrMsgString(formType));
                }
            }
        
            if (pAS->bOutputArg && !pAEx->is_Lvalue())
            {
                //TODO special diag if not writable because is a for-loop var...
                m_pdm->msg2(CG_E_LVAL_PARAM, pAEdp, ArgDiagID(m_pdm->jot1(), pAS, i), getFunName());
                bError = true;
            }
        }
    
        typeset = lpTypes[lpArgTypes[0]];
        break;
    }

    if(nTypeSet >= m_pOverloads->nOverloads)
    {
        int simple = 0; // number of simple errors
        bool bHasErrorExpr = false;//don't display error if one expr is error expr

        for(i=0; i<nArgs; i++)
        {
            if(pArgs[i].pActual->isErrorExpr())
            {
                bHasErrorExpr = true;
                break;
            }
        }

        if(any_overloads == 1 && !bHasErrorExpr)
        {
            for(i=0; i<nArgs; i++)
            {
                const ST_TypeSet_C& formType = m_pOverloads->lpTypes[lpArgTypes[i+1]];
                const ST_TypeSet_C& actType  = pArgs[i].pActual->GetType();
                ST_TypeSet_C        type     = formType;

                type &= actType;

                if(type.IsEmptyType())
                {
                    m_pdm->msg4(CG_E_TYPE_PARAM, pArgs[i].pActual->getEdp(), 
                                ArgDiagID(m_pdm->jot1(), &pArgs[i], i), getFunName(), 
                                ST_TypeSetToErrMsgString(formType),
                                ST_TypeSetToErrMsgString(actType)
                               );
                    simple++;
                }
            }
        }

        if(simple == 0 && !bHasErrorExpr)
        {
            if(any_overloads)
            {
                if(m_opEnum==CG_MOVE)
                {
                    //special msg if assignment:
                    //error location is ":=" itself:
                    m_pdm->msg2(CG_E_TYPE_ASSIGN, &m_edp,
                        ST_TypeSetToErrMsgString(pArgs[0].pActual->GetType()),
                        ST_TypeSetToErrMsgString(pArgs[1].pActual->GetType()));
                }
                else
                {
                    m_pdm->msg1(CG_E_TYPE_FUNCT, &m_edp, getFunName());
                }
            }
            else
            {
                m_pdm->msg2(CG_E_ILL_PARAMS, &m_edp, getFunName(), m_pdm->jot1()<<nArgs);
            }
        }
    
        bError = true;
        delete []lpArgTypes;
        lpArgTypes = NULL;
    }

    delete []lpTypes;
    
    return !bError;
}
//##AC end




void ST_Function_C::Propagate(const ST_TypeSet_C &what)
{
//##AC beg
    int             i;
    int             nArgs;
    ST_PARAMSPEC*   pArgs;
    typeset = what;

    if(lpArgTypes)
    {
        nArgs = m_actuals.getUsed(); 
        pArgs = m_actuals.getElements(); 
        for(i=0; i<nArgs; i++)
        {
            if(lpArgTypes[i+1] == lpArgTypes[0])
                pArgs[i].pActual->Propagate(typeset);
        }
    }
  /*
   * if there is more than one result type
   * give a warning.
   * but ignore parentheses "()"
   * and if this is a unary "-" on a IntLiteral
   */
   if(m_opEnum!=CG_NOOP && !typeset.IsEmptyType() && !typeset.IsMonomorphType())
   {
        m_pdm->msg2(CG_W_TYPE_RESULT, &m_edp, getFunName(), ST_TypeSetToErrMsgString(typeset));
   }
//##AC end
}



void ST_Function_C::dumpNoImplementationFoundErrMsg()const
{
    //##AC beg
    FC_CString                 jot(MAX_PATH);

    jot.load(ST_TypeSetToErrMsgString(GetType()));
    jot<<(_T(" "));
    jot<<getFunName();
    jot<<_T("(");

    int                 nArgs = m_actuals.getUsed(); 
    const ST_PARAMSPEC* pArgs = m_actuals.getCElements(); 
    for(int i=0; i<nArgs; i++)
    {
        if(i>0)
            jot<<_T(",");

        jot<<ST_TypeSetToErrMsgString(pArgs[i].pActual->GetType());
    }
    //##AC end
    jot<<_T(")");

    m_pdm->msg1(CG_E_FUN_OVERLOAD_NO_BODY, &m_edp, jot);
}



const TCHAR* ST_Function_C::getCalledPOU(const CG_Symtab** ppSym, const FC_CGArray<ST_PARAMSPEC>** ppArgList)const//##AC
{
    if(ppSym)
        *ppSym = m_pSymOfFun ? m_pSymOfFun->getCGSym() : NULL;
    if(ppArgList)
        *ppArgList = &m_actuals;
    return m_pSymOfFun ? m_pSymOfFun->getCGSym()->pszId : m_opEnum==CG_BUILDIN ? getFunName() : CGT_ExprTypToLString(m_opEnum);
}

const CG_ExprTree* ST_Function_C::makeCGExpr(
    ST_CExprMem* pAlloc,
    CG_ExprTree* pReady
)const
{
    int                        iArg;
    CG_ExprTree*               pRet;
    int                        nArgs = m_actuals.getUsed(); //##AC
    const ST_PARAMSPEC*        pArgs = m_actuals.getCElements(); //##AC

    if(!m_pSymOfFun)
    {
        pRet = initCGExpr(pAlloc, pReady, m_opEnum);

        //operator:
        switch(m_opEnum)
        {
        //unary ops:
        case CG_NOT:
        case CG_UMINUS:
        case CG_NOOP:
            pRet->ops.pLhs = pArgs[0].pActual->makeCGExpr(pAlloc, NULL);//##AC
            pRet->ops.pRhs = NULL;

            //Note: NOOP the "()" is special: we dont know here if it gets 
            //read or written this is treated inside CG_SetAccFlags.
            if(m_opEnum != CG_NOOP)
                CG_SetAccFlags(m_pdm, pRet->ops.pLhs, CG_ACCESS_R);
        break;        
        //binary ops:
        case CG_MOVE:
        {
            assert(pRet->pType->type == CG_VOID);

            pRet->ops.pLhs = pArgs[0].pActual->makeCGExpr(pAlloc, NULL);//##AC
            pRet->ops.pRhs = pArgs[1].pActual->makeCGExpr(pAlloc, NULL);//##AC

            if(pArgs[0].pActual->isReferenceTmpVar())//if lhs is ref temp set P|R bits //##AC
                CG_SetAccFlags(m_pdm, pRet->ops.pRhs, CG_ACCESS_A|CG_ACCESS_R);
            else //only R:
                CG_SetAccFlags(m_pdm, pRet->ops.pRhs, CG_ACCESS_R);
  
            //always set the W after the R flags (to get read before write warning)
            CG_SetAccFlags(m_pdm, pRet->ops.pLhs, CG_ACCESS_W);
          }
        break;
        case CG_OR: 
        case CG_XOR:
        case CG_AND:
        case CG_EQ: 
        case CG_NE: 
        case CG_LT: 
        case CG_LE: 
        case CG_GT: 
        case CG_GE: 
        case CG_ADD:
        case CG_SUB:
        case CG_MUL:
        case CG_DIV:
        case CG_MOD:
        case CG_POW:
            pRet->ops.pLhs = pArgs[0].pActual->makeCGExpr(pAlloc, NULL);//##AC
            CG_SetAccFlags(m_pdm, pRet->ops.pLhs, CG_ACCESS_R);

            pRet->ops.pRhs = pArgs[1].pActual->makeCGExpr(pAlloc, NULL);//##AC
            CG_SetAccFlags(m_pdm, pRet->ops.pRhs, CG_ACCESS_R);
        break;        
        case CG_BUILDIN:
        {
            //##AC beg
            CG_ExprTree* pExprList = NULL;
            if(nArgs>0)
            {
                int       iArg;
                pExprList = pAlloc->registerCGExprArray(nArgs);
                
                for(iArg=0; iArg<nArgs; iArg++)
                {
                    pArgs[iArg].pActual->makeCGExpr(pAlloc, &pExprList[iArg]);
                    CG_SetAccFlags(m_pdm, &pExprList[iArg], CG_ACCESS_R);//TODO have no idea if written??
                }
            }
            //##AC end
            pRet = initCGExpr(pAlloc, pReady, CG_BUILDIN);
            pRet->bin.pszId = m_pOverloads->pszId;//is globally registered string or static
            pRet->bin.nArgs = nArgs;
            pRet->bin.pArgs = pExprList;
        }
        break;        
        default:
            CGT_IERR2(m_pdm, &m_edp, _T("bad expr. type enum"));
        }

        return pRet;
    }


    //function with ST body or macro
    // don't add function name if it had errors, to prevent from reading the java file
    // that does not exist
    if((m_pSymOfFun->getCGSym()->flags & CG_STAB_HDR_NOERR)==0)
    {
        assert(!"error count ?!");//should not pass here? 
        return NULL;
    }

    const CG_MacroBody*    pMacDescr = NULL;
    const CG_DType*        pRCast    = NULL;
    int                    nACasts   = 0;
    const CG_DType*const*  pACasts   = NULL;

    if(m_pSymOfFun->isMacro())
    {
        if(m_pSymOfFun->isPolyMorph())
            pMacDescr = m_pSymOfFun->lookUpFunMacroBody(GetType(), nArgs, pArgs, &pRCast, &nACasts, &pACasts);//##AC
        else
            pMacDescr = m_pSymOfFun->getNonPolyMacroBody();//consider: pRCast, nACast, pACast also here ?

        if(!pMacDescr)
            dumpNoImplementationFoundErrMsg(); //carry on...
    }


    const CG_ArgExpr* pArgListReg = NULL;
    if(nArgs>0)
    {
        CG_ArgExpr* pNewArgList;
        
        //alloc get argExpr<->Entry array:
        FC_NEW_ZASTRUCT(pNewArgList, CG_ArgExpr, nArgs);

        for(iArg=0; iArg<nArgs; iArg++) //##AC
        {
            const CG_ExprTree* pEx = pArgs[iArg].pActual->makeCGExpr(pAlloc, NULL);//##AC

            if(iArg<nACasts && pACasts[iArg] && pACasts[iArg]->type != pEx->pType->type)
                pEx = ST_CreateTypeCast(getPfc(), pAlloc, NULL, pEx, pACasts[iArg]);

            assert(pArgs[iArg].pFormal);
            pNewArgList[iArg].pFormal = pArgs[iArg].pFormal;
            pNewArgList[iArg].pActual = pEx;
        }


        for(iArg=0; iArg<nArgs; iArg++)
        {
            WORD  accFlags;    
            switch(pNewArgList[iArg].pFormal->declScope)
            {
            case CG_VAR_IN_OUT:  accFlags = CG_ACCESS_X; break;
            case CG_VAR_IN_EXT: 
            case CG_VAR_INPUT:   accFlags = CG_ACCESS_I; break;
            case CG_VAR_OUTPUT:  accFlags = CG_ACCESS_O; break;
            default: 
                assert(!"bad decl scope for fun param");
            }
            CG_SetAccFlags(m_pdm, pNewArgList[iArg].pActual, accFlags);
        }

        //register as body global mem:
        pArgListReg = pAlloc->registerArgExprList(pNewArgList);
    }
    assert(nArgs>0 || !pArgListReg);

    
    assert(m_opEnum==CG_EXPRTYP_UNDEF);
    if(pMacDescr)
    {
        pRet = initCGExpr(pAlloc, pReady, CG_FUNMACRO);
        pRet->mac.pMac  = pMacDescr;
        pRet->mac.nArgs = nArgs;
        pRet->mac.pArgs = pArgListReg;
    }
    else
    {
        pRet = initCGExpr(pAlloc, pReady, CG_FUNCALL);
        pRet->fun.pSym  = m_pSymOfFun->getCGSym();
        pRet->fun.nArgs = nArgs;
        pRet->fun.pArgs = pArgListReg;
    }

    if(pRCast && pRCast->type != pRet->pType->type)
    {
        const CG_DType* pWantType = pRet->pType;
        pRet->pType = pRCast;
        return ST_CreateTypeCast(getPfc(), pAlloc, pReady, pRet, pWantType);
    }
    return pRet;
}





ST_StructInit_C::ST_StructInit_C(
    const CG_Edipos* pEdp, 
    st_ParseContext* pfc,
    ST_Identifier_C* pMember, 
    ST_Expression_C* pInitExpr
) 
 : ST_Expression_C(pEdp, pfc)
{
    typeset = CG_TBS_STRUCT;
    append(pMember, pInitExpr);
}

ST_StructInit_C::~ST_StructInit_C()
{
    int i;

    for(i=m_Inits.getUsed()-1;i>=0;i--)
        delete(m_Inits.get(i));
    
    for(i=m_Members.getUsed()-1;i>=0;i--)
        delete(m_Members.get(i));
}
void ST_StructInit_C::append(
    ST_Identifier_C* pMember, 
    ST_Expression_C* pInitExpr
)
{
    pMember->Detach();
    pInitExpr->Detach();

    m_Inits.add(pInitExpr);
    m_Members.add(pMember);
}


bool ST_StructInit_C::checkInit2(
    const CG_DType* pType, 
    CG_Initialiser* pRet, 
    long*           pCount
)
{
    int                    i;
    unsigned long          j;
    unsigned long          nInitDone;
    bool                   bError;
    ST_Expression_C*       pInitExpr;
    const ST_Identifier_C* pMember;
    const TCHAR*           pszMember;
    const CG_SymEntry*     pEntry; 
    const CG_SymEntry**    pEntries; 
    CG_Initialiser*        pInits;
    int                    nInits;


    bError = false;
    nInits = m_Members.getUsed();
 

    //assert: at least one member:=value pair by syntax:
    assert(nInits>0);
    (*pCount)++;

    //1. type must be struct or fb but no array:
    if(!(pType->type==CG_STRUCT || pType->type==CG_INSTANCE) || pType->bIsArray) 
    {
        m_pdm->msg2(CG_E_INIT_TYPE, &m_edp, ST_TypeSetToErrMsgString(typeset), pType->pszName);
        return false;
    }


    //2. convert all member names to Symtab entries of pTypeDescr:
    // allocate memory:
    FC_NEW_ZASTRUCT(pInits, CG_Initialiser, nInits);
    FC_NEW_ZASTRUCT(pEntries, const CG_SymEntry*, nInits);
    

    nInitDone = 0;
    for(i=0; i<nInits; i++)
    {
        pInitExpr = m_Inits.get(i);
        pMember   = m_Members.get(i);
        pszMember = pMember->getName();
        pEntry    = m_pFront->lookUpEntry(pType->pSym, pszMember);

        if(pEntry)
        {
            for(j=0; j<nInitDone; j++)
            {
                if(pEntries[j] == pEntry)
                    break;
            }
            if(j<nInitDone)
            {
                m_pdm->msg1(CG_W_INIT_MEMBER_TWICE, pMember->getEdp(), pszMember);
                //do not append second init but check it:
                if(!pInitExpr->checkInit(pEntry->pType, NULL))
                    bError = true;
            }
            else
            {
                //check if FB member is accessible and writable:
                if(pType->type==CG_INSTANCE)
                {
                    if(pEntry->declScope!=CG_VAR_INPUT)
                    {
                        m_pdm->msg2(CG_E_INIT_AT_NON_INVAR,  pMember->getEdp(),
                             pszMember, ST_StoreClassName(pEntry->declScope));
                        bError = true;
                    }
                }
                //warn if init CONSTANT decl 
                if(pEntry->declMod & CG_DMOD_CONSTANT)
                    m_pdm->msg1(CG_W_INST_INIT_CONSTANT,  pMember->getEdp(), pszMember);

                if(!bError)
                {
                    //check that not AT %Input, this may also 
                    //happen for structures: 
                    if(pEntry->pszAddr && CGT_GetAddrLocation(pEntry->pszAddr)==_T('I'))
                    {
                        m_pdm->msg2(CG_E_INIT_ADDR_INPUT, pMember->getEdp(), pszMember, pEntry->pszAddr);
                        bError = true;
                    }
                }

                pEntries[nInitDone] = pEntry;
                if(!pInitExpr->checkInit(pEntry->pType, pRet ? &pInits[nInitDone] : NULL))
                    bError = true;
                nInitDone++;
            }
        }
        else
        {
            m_pdm->msg2(CG_E_INIT_MEMBER_UNDEF, pMember->getEdp(), pszMember, pType->pszName);
            bError = true;
        }
    }

    if(bError || !pRet)
    {
        delete(pInits);
        delete(pEntries);
        return !bError;
    }
    
    pRet->ut = CG_INIT_STRUCT;
    pRet->str.nInits   = nInitDone;
    pRet->str.pInits   = m_pFront->registerInitArray(nInitDone, pInits);
    pRet->str.pEntries = m_pFront->registerEntryArray(nInitDone, pEntries);
    
    assert(!bError && nInitDone>0);
    return true;
}


//Note: either pRepeat or pInitExpr is NULL not both
//pInitExpr is NULL if "4()" is used
ST_ArrayInit_C::ST_ArrayInit_C(
    const CG_Edipos* pEdp, 
    st_ParseContext* pfc,
    ST_Expression_C* pRepeat, 
    ST_Expression_C* pInitExpr
) : ST_Expression_C(pEdp, pfc)
{
    typeset = CG_TBS_ARRAY;
    append(pRepeat, pInitExpr);
}

ST_ArrayInit_C::~ST_ArrayInit_C()
{
    int i;

    for(i=m_Repeat.getUsed()-1;i>=0;i--)
        delete(m_Repeat.get(i));
    
    for(i=m_Inits.getUsed()-1;i>=0;i--)
        delete(m_Inits.get(i));
}
void ST_ArrayInit_C::append(
    ST_Expression_C* pRepeat, 
    ST_Expression_C* pInitExpr
)
{
    if(pRepeat)
        pRepeat->Detach();
    if(pInitExpr)
        pInitExpr->Detach();

    m_Inits.add(pInitExpr);
    m_Repeat.add(pRepeat);
}
bool ST_ArrayInit_C::isArrayInit()
{
    return true;
}
bool ST_ArrayInit_C::checkInit2(
    const CG_DType* pType, 
    CG_Initialiser* pRet, 
    long*           pCount
)
{
    int               i;
    __int64           lrep;
    bool              bError;
    CG_BITSET         lTypeSet;
    ST_Expression_C*  pRep;
    ST_Expression_C*  pInitExpr;
    CG_Initialiser*   pInits;
    long              count;
    long*             pRepeats;
    long              nInitDone;
    int               nInits;


    nInits = m_Inits.getUsed();
    //assert: at least one repeat(value) pair:
    assert(nInits>0);

    //1. type must be array:
    if(!pType->bIsArray) 
    {
        m_pdm->msg2(CG_E_INIT_TYPE, &m_edp, ST_TypeSetToErrMsgString(typeset), pType->pszName);
        return false;
    }


    //2. convert all repeate expr to int and store info to m_rv_list:
    bError = false;
    // allocate memory:
    FC_NEW_ZASTRUCT(pInits, CG_Initialiser, nInits);
    FC_NEW_ZASTRUCT(pRepeats, long, nInits);
    
    
    nInitDone = 0;
    for(i=0; i<nInits; i++)
    {
        pRep = m_Repeat.get(i);
        if(pRep)
        {
            lTypeSet = pRep->GetType().GetTypeSet() & (CG_TBS_ANY_INT & ~CG_TBS_LINT);
            if(lTypeSet==0)
            {
                m_pdm->msg1(CG_E_INIT_REPEAT_NOT_INT, pRep->getEdp(),
                    ST_TypeSetToErrMsgString(pRep->GetType()));
                bError = true;
            }
            else
            {
                pRep->Propagate(ST_SelectBestType(
                    m_pdm, &m_pFront->m_features, lTypeSet, pRep->getEdp())
                  );
            
                if(!pRep->isAny64Literal(&lrep))
                {
                    m_pdm->msg0(CG_E_EXPR_NOT_CONST, pRep->getEdp());
                    bError = true;
                }
                else if(lrep<=0 || lrep>MAX_ARRAY_ELEMENTS)
                {
                    m_pdm->msg2(CG_E_INIT_REPEAT_BAD_VALUE, pRep->getEdp(),
                        m_pdm->jot1()<<lrep, m_pdm->jot2()<<MAX_ARRAY_ELEMENTS);
                    bError = true;
                }
            }

            if(bError)
                lrep = 1;
        }
        else
        {
            //no repeate -> default is 1:
            lrep = 1;
        }    

        pInitExpr = m_Inits.get(i);

        //recurse into init value:
        if(pInitExpr)
        {
            count = 0;
            assert(pInitExpr->isArrayInit() || pType->bIsArray);
            if(pInitExpr->isArrayInit())
            {
                if(!pInitExpr->checkInit2(pType, 
                    pRet ? &pInits[nInitDone] : NULL, 
                    &count)
                  )
                   bError = true;
            }
            else
            {
                if(!pInitExpr->checkInit2(
                    m_pFront->getArrayBaseDType(pType), 
                    pRet ? &pInits[nInitDone] : NULL, 
                    &count)
                  )
                   bError = true;
                    
            }
        }
        else
        {
            //pInitExpr is NULL if "4()" syntax is used:
            count = 1;
            pInits[nInitDone].ut = CG_INIT_NONE; 
        }

        *pCount += (long)(lrep*count);

        //optimize: 23(34(78)))
        if(!bError && pInits[nInitDone].ut==CG_INIT_ARRAY)
        {
            if(pInits[nInitDone].arr.nInits==1)
            {
                lrep *= pInits[nInitDone].arr.pRepeats[0];
                pInits[nInitDone] = pInits[nInitDone].arr.pInits[0];
            }
        }

        //TODO check overflow ????
        pRepeats[nInitDone++] = (long)lrep;
    }


    
    if(bError || !pRet)
    {
        delete(pInits);
        delete(pRepeats);
        return !bError;
    }
    
    pRet->ut = CG_INIT_ARRAY;
    pRet->arr.nInits   = nInitDone;
    pRet->arr.pInits   = m_pFront->registerInitArray(nInitDone, pInits);
    pRet->arr.pRepeats = m_pFront->registerLongArray(nInitDone, pRepeats);
    
    assert(!bError && nInitDone>0);
    return true;
}































ST_CIdentList::~ST_CIdentList()
{
    m_ids.deletePtrs();
}
const ST_Identifier_C* ST_CIdentList::addIdOnce(ST_Identifier_C* pId, HRESULT ew)
{
    for(int i=0;i<m_ids.getUsed();i++)
    {
        if(!lstrcmpi(m_ids.get(i)->getName(), pId->getName()))
        {
            m_pdm->msg1(ew, pId->getEdp(), pId->getName());
            delete(pId);
            return NULL;
        }
    }
    pId->Detach();
    m_ids.add(pId);
    return pId;
}



ST_CAttrList::~ST_CAttrList()
{
    m_attrtag.deletePtrs();
    m_attrval.deletePtrs();
}
void ST_CAttrList::addAttr(ST_Identifier_C* pTag, ST_Identifier_C* pVal)
{
    pTag->Detach();
    pVal->Detach();
    m_attrtag.add(pTag);
    m_attrval.add(pVal);
}
 
CG_AttribList* ST_CAttrList::convertToStruct()const
{
    int             i;
    int             nAttribs;
    CG_AttribList*  pRet;

    nAttribs = m_attrtag.getUsed();    
    
    pRet = (CG_AttribList*) new BYTE[sizeof(CG_KeyValPair)*nAttribs + sizeof(long)]; 

    pRet->nAttribs = nAttribs;

    for(i=0;i<nAttribs;i++)
    {
       pRet->pairs[i].edpKey = *m_attrtag.get(i)->getEdp();
       pRet->pairs[i].pszKey = m_pFront->registerString(m_attrtag.get(i)->getName());
       
       pRet->pairs[i].edpVal = *m_attrval.get(i)->getEdp();
       pRet->pairs[i].pszVal = m_pFront->registerString(m_attrval.get(i)->getName());
    }

    return pRet;
}


void ST_CTypeOrList::addType(const CG_Edipos* pEdp, const ST_TypeSet_C& type, const CG_DType* pACast)
{
    if(m_iArgCount<ST_MAX_FUNCTION_PARAMS)
    {
        m_pArgTypeSets[m_iArgCount]  = type;
        m_optCastToType[m_iArgCount] = pACast;
        m_iArgCount++;
    }
    else
    {
        m_pdm->msg0(CG_E_TOO_MANY_FUN_PARAMS, pEdp);
        //reduce number of error messages...
        m_iArgCount = 0;
    }
}




ST_CTaskInits::ST_CTaskInits(
    st_ParseContext* pfc,
    ST_CExprMem*     pAlloc,
    ST_Identifier_C* pAttr, 
    ST_Expression_C* pVal
) : ST_CParseTree(pAttr->getEdp(), pfc)
{
    addPair(pfc, pAlloc, pAttr, pVal);
}

void ST_CTaskInits::addPair(
    st_ParseContext* pfc,
    ST_CExprMem*     pAlloc,
    ST_Identifier_C* pAttr, 
    ST_Expression_C* pVal
)
{
    int          idx;
    CG_BITSET    lWantedType;
    const TCHAR* pszAttr;
    CG_TaskInit  tmp;

    pszAttr = pAttr->getName();

    for(idx=0;idx<m_inits.getUsed(); idx++)
    {
        if(!lstrcmpi(pszAttr, m_inits.getAddrOf(idx)->pszAttr))
        {
            m_pdm->msg1(CG_E_DOUBLE_ATTRIB, pAttr->getEdp(), pszAttr);
            goto Ende;
        }
    }
	
    for(idx=0;idx<m_pFront->m_features.m_pCFeat->nTIPairs; idx++)
    {
		if (FC_StringMatchFilePattern(pszAttr, m_pFront->m_features.m_pCFeat->psTI[idx]) == 1)
			break;										// consider first matching
	}

	if (idx == m_pFront->m_features.m_pCFeat->nTIPairs)  // attr not found
    {
		FC_CString validTasks;
		for(idx = 0; idx<m_pFront->m_features.m_pCFeat->nTIPairs; idx++)
		{
			if(idx != 0) validTasks << _T(",");
			validTasks << m_pFront->m_features.m_pCFeat->psTI[idx];
		}
        m_pdm->msg2(CG_E_ILLEGAL_TASK_INIT, pAttr->getEdp(), pszAttr, validTasks);
        goto Ende;
    }

    lWantedType = m_pFront->m_features.m_pCFeat->pTITypes[idx];

    if(!pVal->checkTypedExpr(lWantedType, false))
    {
        m_pdm->msg3(CG_E_BAD_ATTR_DATATYPE, pVal->getEdp(), 
                    pszAttr, 
                    ST_TypeSetToErrMsgString(pVal->GetType()),
                    ST_TypeSetToErrMsgString(lWantedType)
                );
        goto Ende;
    }

	if(!pVal->getLitValue())
	{
        m_pdm->msg0(CG_E_EXPR_NOT_CONST, pVal->getEdp());
		goto Ende;
	}	

    pVal->fixType();
    
    if(pfc->getErrorsInFile()==0)//don't eval if errors
    {
        tmp.edp = *pAttr->getEdp();
        tmp.pszAttr = m_pFront->registerString(pszAttr);
        pVal->makeCGExpr(pAlloc, &tmp.val);
        //TODO if not literal? error
        m_inits.add(tmp);
    }

Ende:
    delete(pAttr);
    delete(pVal);
}

ST_CTaskList::ST_CTaskList(
    st_ParseContext* pfc,
    ST_Identifier_C* pId,
    ST_CAttrList*   pAttr,
    ST_CTaskInits*  pInits
) : ST_CParseTree(pId->getEdp(), pfc)
{
    addTask(pfc, pId, pAttr, pInits);
}

void ST_CTaskList::addTask(
    st_ParseContext* pfc,
    ST_Identifier_C* pId,
    ST_CAttrList*    pAttr,
    ST_CTaskInits*   pInits
)
{
    int                 i;
    CG_Task*            pNew;
    const TCHAR*        pszId;
    const CG_SymEntry*  pEntry;
    const CG_Edipos*    pEdpId;

    pszId = pId->getName();
    pEdpId= pId->getEdp();

    //check if task name is unique:
    for(i=0;i<m_tasks.getUsed();i++)
    {
        if(!lstrcmpi(m_tasks.getAddrOf(i)->pszId, pszId))
        {
            m_pdm->msg1(CG_E_TASK_ALREADY_DEFINED, pEdpId, pszId);
            goto Ende;
        }
    }

    //check if task name is not already used as 
    //glob var name (we want to have unique names here):
    pEntry = pfc->getSym()->lookUpEntryDeep(pszId, NULL);
    if(pEntry)
    {
        m_pdm->msg2(CG_E_TASKNAME_IS_GLOBVAR, 
            pEdpId, pszId, m_pdm->edpToStr(&pEntry->edp, m_pdm->jot1()));
        //fake: carry on ....
    }

    pNew = m_tasks.addZero();

    pNew->edp   = *pEdpId;
    pNew->pszId = m_pFront->registerString(pszId);
    if(pAttr)
    {
        pNew->pAttribs = m_pFront->registerAttrList(
                    pAttr->convertToStruct());
    }
    //set later: pNew->nPrgs;  in this->convertToStruct
    //set later: pNew->pPrgs;
    if(pInits)
    {
        pNew->nInits = pInits->m_inits.getUsed();
        pNew->pInits = m_pFront->registerTaskInits(
                 pInits->m_inits.getMem());
    }
 
    //MUST now add at least an empty prg inst list:
    //st_ParseContext::attachPrgToTask
    m_prgs.add(new FC_CGArray<const CG_SymEntry*>);

Ende:
    delete(pId);
    delete(pAttr);
    delete(pInits);
}
int ST_CTaskList::findTaskByName(
    const TCHAR* pszTaskName
)const
{
    int i;
    for(i=0; i<m_tasks.getUsed(); i++)
    {
        if(!lstrcmpi(pszTaskName, m_tasks.getAddrOf(i)->pszId))
            return i;
    }
    return -1;
}
bool ST_CTaskList::addPrg(
    const TCHAR*       pszTaskName, 
    const CG_SymEntry* pEntry
)
{
    //try to find the task where to attach to:
    int i = findTaskByName(pszTaskName);
    if(i>=0)
    {
        //attach prg inst name to task:
        m_prgs.get(i)->add(pEntry);
    }
    return i>=0;
}
CG_Task* ST_CTaskList::convertToStruct()
{
    int      i;
    CG_Task* pT;

    for(i=0; i<m_tasks.getUsed(); i++)
    {
        pT = m_tasks.getAddrOf(i);
        pT->nPrgs = m_prgs.get(i)->getUsed();
        if(pT->nPrgs==0)
        {
            m_pdm->msg1(CG_W_NO_PROGRAM_AT_TASK, 
                &pT->edp, pT->pszId);
        }
        else
        {
            pT->pPrgs = m_pFront->registerEntryArray(pT->nPrgs,
                            m_prgs.get(i)->getMem());
        }
    }
    pT = m_tasks.getMem();
    return pT;
}
















