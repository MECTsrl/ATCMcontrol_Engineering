


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "LDFBDTools.h"
#include "cgt_tools.h"
#include "cgt_misc.h"

//----  Static Initializations:   ----------------------------------*
class CWarnOnMultiWrtCtx
{
public:
    CGT_CDiagMsg*                   m_pdm;
    CGT_CDmp                        m_dmp;
    FC_CHashStr<const CG_ExprTree*> m_wrt;

    CWarnOnMultiWrtCtx(CGT_CDiagMsg* pdm) 
        : m_pdm(pdm),
          m_wrt(false, true, 128)
    {
    }
};

static bool WarnOnMultiWrtCallBack(void* pCtx, const CG_ExprTree* pExpr);














bool SortPinArray(
    CGT_CDiagMsg*             pdm,
    const CG_Edipos*          pEdp,
    FC_CGArray<PinElement*>&  pins,
    int*                      pnInputs,
    int*                      pnOutputs,
    PinElement**              ppEN,
    PinElement**              ppENO
)
{
    int         i;
    int         j;
    PinElement* pPin;
    int         nPins = pins.getUsed();
    int         nI = 0;
    int         nO = 0;
    PinElement**pTmpIn;
    PinElement**pTmpOut;
    PinElement* pEN = NULL;
    PinElement* pENO= NULL;

    if(pnInputs)
        *pnInputs = 0;
    if(pnOutputs)
        *pnOutputs = 0;
    if(ppEN)
        *ppEN = NULL;
    if(ppENO)
        *ppENO = NULL;

    if(nPins==0)    
        return true;//mainly to avoid new PinElement*[0], causes assertion in tools.dll
    
    pTmpIn  = new PinElement*[nPins];
    pTmpOut = new PinElement*[nPins];

    //sort top to bottom and:
    //pin[0]    = first input  (may be EN if any)
    //pin[1]    = second input 
    //...
    //pin[nI]   = last input
    //pin[nI+1] = first output  (may be ENO if any or function result if any)
    //pin[nI+2] = second output (may be function result if any and ENO was the first out)
    //...................
    //pin[nI+nO]= last output
    //

    for(i=0;i<nPins;i++)
    {
        pPin = pins.get(i);

        if(pPin->lPinType==PIN_TYPE_EN)
        {
            if(pEN)
            {
                pdm->msg0(CG_E_ENENO_ILLEGAL_PIN_LAYOUT, pEdp);
                return false;
            }
            pEN = pPin;
        }
        if(pPin->lPinType==PIN_TYPE_ENO)
        {
            if(pENO)
            {
                pdm->msg0(CG_E_ENENO_ILLEGAL_PIN_LAYOUT, pEdp);
                return false;
            }
            pENO = pPin;
        }
        
        if(pPin->lPinType & ANY_IN_PIN)
        {
            for(int k=0;k<nI;k++)
            {
                if(pTmpIn[k]->pos.y > pPin->pos.y)
                {
                    for(int ki=nI;ki>k;ki--)
                        pTmpIn[ki] = pTmpIn[ki-1];

                    break;
                }
            }
            pTmpIn[k] = pPin;
            nI++;
        }
        else
        {
            assert(pPin->lPinType & ANY_OUT_PIN);

            for(int k=0;k<nO;k++)
            {
                if(pTmpOut[k]->pos.y > pPin->pos.y)
                {
                    for(int ki=nO;ki>k;ki--)
                        pTmpOut[ki] = pTmpOut[ki-1];

                    break;
                }
            }
            pTmpOut[k] = pPin;
            nO++;
        }
    }


    //check that if EN-ENO is always a pair and top most io/out pin:
    if(pEN || pENO)
    {
        if(nI>0 && nO>0 && pTmpIn[0]->lPinType==PIN_TYPE_EN && pTmpOut[0]->lPinType==PIN_TYPE_ENO)
        {
            if(ppEN)
                *ppEN = pEN;
            if(ppENO)
                *ppENO = pENO;

            //EN/ENO may have a negate flag, but no trig flag:
            assert((pEN->lPinFlags & (PIN_FLAG_R_EDGE|PIN_FLAG_F_EDGE))==0);
            assert((pENO->lPinFlags & (PIN_FLAG_R_EDGE|PIN_FLAG_F_EDGE))==0);
        }
        else
        {
            pdm->msg0(CG_E_ENENO_ILLEGAL_PIN_LAYOUT, pEdp);
            return false;
        }
    }

    j=0;
    for(i=0;i<nI;i++)
        pins.set(j++, pTmpIn[i]);

    for(i=0;i<nO;i++)
        pins.set(j++, pTmpOut[i]);
    
    if(pnInputs)
        *pnInputs = nI;
    if(pnOutputs)
        *pnOutputs = nO;

    delete(pTmpIn);
    delete(pTmpOut);

    return true;
}



inline bool IsLDFBDInDecl(const CG_DECLSCOPE ds)
{
    return ds==CG_VAR_INPUT||ds==CG_VAR_IN_OUT||ds==CG_VAR_IN_EXT;
}

inline bool IsLDFBDOutDecl(const CG_DECLSCOPE ds)
{
    return ds==CG_VAR_OUTPUT||ds==CG_VAR_IN_OUT;
}

const CG_SymEntry* GetFirstLDFBDInEntry(const CG_Symtab* pSym)
{
    for(const CG_SymEntry* pEntry=pSym->pFirst;pEntry;pEntry=pEntry->pNext)
        if(IsLDFBDInDecl(pEntry->declScope))
            break;
    return pEntry;
}
const CG_SymEntry* GetNextLDFBDInEntry(const CG_SymEntry* pLast)
{
    if(pLast->declScope==CG_VAR_IN_EXT)
    { // ++**++ JD extInParam -->
        assert(!pLast->pNext || pLast->pNext->declScope==CG_VAR_IN_EXT);
	    return pLast->pNext ? pLast->pNext : pLast->pParent->pVarInExt;
    } // ++**++ JD extInParam <--

    for(const CG_SymEntry* pEntry=pLast->pNext;pEntry;pEntry=pEntry->pNext)
        if(IsLDFBDInDecl(pEntry->declScope))
            break;
    return pEntry;
}
const CG_SymEntry* GetFirstLDFBDOutEntry(const CG_Symtab* pSym)
{
    for(const CG_SymEntry* pEntry=pSym->pFirst;pEntry;pEntry=pEntry->pNext)
        if(IsLDFBDOutDecl(pEntry->declScope) && pEntry->pType->type!=CG_VOID)
            break;
    return pEntry;
}
const CG_SymEntry* GetNextLDFBDOutEntry(const CG_SymEntry* pLast)
{
    for(const CG_SymEntry* pEntry=pLast->pNext;pEntry;pEntry=pEntry->pNext)
        if(IsLDFBDOutDecl(pEntry->declScope))
            break;
    return pEntry;
}











FC_CStr CreatePinErrorMsgName(
    HINSTANCE         hInst, 
    const BoxElement* pBox, 
    const PinElement* pPin
)
{
    FC_CString  errName;
    FC_CString  jot;

    assert(pBox && pPin);
    
    if(pBox->boxType==BOX_TYPE_VARWRITE)
    {
        errName<<jot.loadRes(CG_RES_STR_VARWRITE, hInst);
    }
    else if(pBox->boxType==BOX_TYPE_VARREAD)
    {
        errName<<jot.loadRes(CG_RES_STR_VARREAD, hInst);
    }
    else if(pBox->boxType==BOX_TYPE_VARFEEDBACK)
    {
        if(pPin->lPinType & ANY_OUT_PIN)
            errName<<jot.loadRes(CG_RES_STR_FEEDBACK_R, hInst);
        else
            errName<<jot.loadRes(CG_RES_STR_FEEDBACK_L, hInst);
    }

    if(errName[0]!=0)
        errName<<_T(" '")<<pBox->text<<_T("'");

    
    
    if(errName[0]==0)
    {
        //pin has no name, use 'number 7' format:
        int n;
        int nPins = pBox->pins.getUsed();
        int nIn   = 0;
        int nOut  = 0;
        int resid;

         
        for(n=0;n<nPins;n++)
        {
            const PinElement* pPinX = pBox->pins.get(n);

            if(pPinX->lPinType & ANY_IN_PIN)
                nIn++;
            else
                nOut++;

            if(pPin==pPinX)
                break;
        }
        //assert: pPin must be from pBox:
        assert(n<=nPins);
        
        
        resid = ST_RES_STR_INPUT;
        if(pPin->lPinType & ANY_OUT_PIN)
            resid = ST_RES_STR_OUTPUT;
        if(pPin->lPinType == PIN_TYPE_INOUT_IN || pPin->lPinType == PIN_TYPE_INOUT_OUT)
            resid = ST_RES_STR_INOUT;


        errName<<jot.loadRes(resid, hInst)<<_T(" '");

        if(pPin->szName[0])
        {
            errName<<pPin->szName;
        }
        else
        {
            errName<<jot.loadRes(ST_RES_STR_NUMBER, hInst);
            errName<<_T(" ")<<(pPin->lPinType & ANY_IN_PIN ? nIn : nOut);
        }
        
        errName<<_T("'");
    }


    return errName;
}




bool CheckPinUseAgainstHeader(
    CGT_CDiagMsg*        pdm, 
    const CG_Edipos*     pBoxEdp,
    const BoxElement*    pBox, 
    const PinElement*    pPin, 
    const ST_CSymtabPrv* pSymOfEntry,
    const CG_SymEntry*   pEntry
)
{
    bool                    bRet = true;
    CG_DTYPES               instEdge;
    bool                    bOk;
    CG_DECLSCOPE            cls;
    
    //check symbolic name:
    if(pEntry->pParent->pouTyp==CG_POU_FUN)
    {
        if(pEntry->pParent->pFirst==pEntry)
        {
            //editor should left it blank:
            if(!pPin->szName.isEmpty())
            {
                pdm->msg2(CG_E_4GR_HEADER_MISMATCH, pBoxEdp, pBox->text, 
                    pdm->res1(CG_RES_STR_NONAME));
                return false;//don't check the rest.
            }
        }
        else if(pEntry->declScope==CG_VAR_IN_EXT)
        {
            //TODO check in ext name style...
        }
        else if(_tcsicmp(pPin->szName, pEntry->pszId))
        {
            FC_CString         jot;
            const CG_SymEntry* pEntryHdr;
            pEntryHdr = pSymOfEntry->lookUpEntry(pPin->szName);
            
            if(pPin->lPinType & ANY_IN_PIN)
            {
                if(pEntryHdr && IsLDFBDInDecl(pEntryHdr->declScope))
                {
                    jot.loadRes(CG_RES_STR_IN_PIN, pdm->getResHMODULE());
                    jot<<_T(" '")<<pPin->szName<<_T("' ")<<pdm->res1(CG_RES_STR_WRONGPOS);
                }
                else 
                    jot<<_T("'")<<pPin->szName<<_T("' ")<<pdm->res1(CG_RES_STR_NOTIN);
            }
            else
            {
                if(pEntryHdr && IsLDFBDOutDecl(pEntryHdr->declScope))
                {
                    jot.loadRes(CG_RES_STR_OUT_PIN, pdm->getResHMODULE());
                    jot<<_T(" '")<<pPin->szName<<_T("' ")<<pdm->res1(CG_RES_STR_WRONGPOS);
                }
                else 
                    jot<<_T("'")<<pPin->szName<<_T("' ")<<pdm->res1(CG_RES_STR_NOTOUT);
            }
            
            pdm->msg2(CG_E_4GR_HEADER_MISMATCH, pBoxEdp, pBox->text, jot);
            return false;//don't check the rest.
        }
    }//TODO also for FB


    //check access type:
    bOk = false;
    cls = pEntry->declScope;
    switch(pPin->lPinType)
    {
    case  PIN_TYPE_IN:          bOk = cls==CG_VAR_INPUT;  break;
    case  PIN_TYPE_OUT:         bOk = cls==CG_VAR_OUTPUT; break;
    case  PIN_TYPE_INOUT_IN:
    case  PIN_TYPE_INOUT_OUT:   bOk = cls==CG_VAR_IN_OUT; break;
    case  PIN_TYPE_INEXT:       bOk = cls==CG_VAR_IN_EXT; break;  
    default:
        assert(!"invalid pin type in CheckPinUseAgainstHeader");
    }
    if(!bOk)
    {
        pdm->msg2(CG_E_PIN_WRONG_STORECLASS, pBoxEdp,
            CreatePinErrorMsgName(pdm->getResHMODULE(), pBox, pPin), 
            ST_StoreClassName(cls));
        return false;//don't check the rest...
    }

    //check r/f_edge:
    if(pPin->lPinFlags & PIN_FLAG_R_EDGE)
        instEdge = CG_BOOL_R_EDGE;
    else if(pPin->lPinFlags & PIN_FLAG_F_EDGE)
        instEdge = CG_BOOL_F_EDGE;
    else
        instEdge = CG_DTYPES_UNDEF;

    if(instEdge!=CG_DTYPES_UNDEF && pEntry->pType->type != instEdge)
    {
        pdm->msg1(CG_E_PIN_WRONG_EDGE_DECL, pBoxEdp,
            CreatePinErrorMsgName(pdm->getResHMODULE(), pBox, pPin));
        bRet = false;
    }

    //check negated in/outputs.
    if(pPin->lPinFlags & PIN_FLAG_NEGATE)
    {
        //data type must be bool and In/out put, not an inout:
        if((pEntry->pType->tbs & CG_TBS_BOOL)==0 || //<-use tbs: allow also Edge bool
           (cls!=CG_VAR_INPUT && cls!=CG_VAR_IN_EXT && cls!=CG_VAR_OUTPUT))
        {
            pdm->msg1(CG_E_ILLEGAL_NEGATE_PIN, pBoxEdp,
                  CreatePinErrorMsgName(pdm->getResHMODULE(), pBox, pPin));
            bRet = false;
        }
    }
    
    return bRet;
}




void BoxTypeToDiagString(BOX_TYPE boxType, FC_CString& text, HMODULE hmodRes)
{
    switch(boxType)
    {
    case BOX_TYPE_VARREAD:      text.loadRes(CG_RES_STR_VARREAD, hmodRes);
    break;
    case BOX_TYPE_VARWRITE:     text.loadRes(CG_RES_STR_VARWRITE, hmodRes);
    break;
    case BOX_TYPE_VARFEEDBACK:  text.loadRes(CG_RES_STR_FEEDBACK, hmodRes);
    break;
    case BOX_TYPE_CONNECTOR:    text.loadRes(CG_RES_STR_CONNECTOR, hmodRes);
    break;
    case BOX_TYPE_CONTINUATION: text.loadRes(CG_RES_STR_CONTINUATION, hmodRes);
    break;
    case BOX_TYPE_POWERRAIL:    text.loadRes(CG_RES_STR_POWERRAIL, hmodRes);
    break;
    case BOX_TYPE_COIL:         text.loadRes(CG_RES_STR_COIL, hmodRes);
    break;
    case BOX_TYPE_CONTACT:      text.loadRes(CG_RES_STR_CONTACT, hmodRes);
    break;
    case BOX_TYPE_WIREDOR:      text.loadRes(CG_RES_STR_WIREDOR, hmodRes);
    break;
    case BOX_TYPE_FB:           text.loadRes(CG_RES_STR_FB, hmodRes);
    break;
    case BOX_TYPE_FNC:          text.loadRes(CG_RES_STR_FCT, hmodRes);
    break;
    case BOX_TYPE_LABEL:        text.loadRes(CG_RES_STR_LABEL, hmodRes);
    break;
    case BOX_TYPE_JUMP:         text.loadRes(CG_RES_STR_JUMP, hmodRes);
    break;
    case BOX_TYPE_RETURN:       text.loadRes(CG_RES_STR_RETURN, hmodRes);
    break;
    case BOX_TYPE_SFC_RETURN:   text.loadRes(CG_RES_STR_TRANS_RET, hmodRes);
    break;
    case BOX_TYPE_UNDEF:
    default:
        assert(!"bad LD/FBD box type enum");
        text.load(_T("<undef box type>"));
    }
};



void WarnMultibleWriteInNW(
    CGT_CDiagMsg*    pdm,
    const CG_STStmt* pNW
)
{
    CWarnOnMultiWrtCtx ctx(pdm);
    CGT_EnumExprStmt(pNW, &ctx, WarnOnMultiWrtCallBack);
}    



//local helper:
static bool WarnOnMultiWrtCallBack(void* pCtx, const CG_ExprTree* pExpr)
{
    CWarnOnMultiWrtCtx* pContext;
    WORD               accFlags;


    pContext = (CWarnOnMultiWrtCtx*)pCtx;


    switch(pExpr->extyp)
    {
    case CG_VARREF:    accFlags = pExpr->var.accFlags; break;
    case CG_STRUCTREF: accFlags = pExpr->str.accFlags; break;
    case CG_ARRAYREF:  accFlags = pExpr->arr.accFlags; break;
    default:
        return true;
    }

    if(accFlags & CG_ACCESS_B)
        return true; //gets used as base of field selector, wait for the top
    if((accFlags & CG_ACCESS_ANY_W)==0)
        return true; //gets not written...
    if(accFlags & CG_ACCESS_X)
        return true; //TODO HACK:  problem inout--inout-----inout chain shall not warn duply wrt!

    //it's a var or the top level part of any a.b.c or a[<expr>]
    //or even a[<expr>].a.b.c  ...
    //and is not an expr that gets followed by .<member> or [<expr>]
    //it is the last part of any such a.b.c.z:


    //clear first:
    pContext->m_dmp.seek(0);

    CGT_DumpSTExpr(pExpr, pContext->m_dmp, NULL);
    
    //don't need tolower any more: the inds are always normalized because
    //always the corresponding decl is used as the pszId.
    if(!pContext->m_wrt.insert(pContext->m_dmp.getSzBuffer(), pExpr))
    {
        pContext->m_pdm->msg1(CG_W_MULTIWRITE_IN_NW, 
            &pExpr->edp, pContext->m_dmp.getSzBuffer()); 
    }//else was newly inserted.

    return true;
}









