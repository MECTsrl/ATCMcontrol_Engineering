#include "cgt_tools.h"
#include "cgt_misc.h"



extern "C" void CGT_EnumExpr(
    const CG_ExprTree* pExprRoot,
    void*              pCtx,
    CGT_OnNewExpr      callBack
)
{
    int i;

    switch(pExprRoot->extyp)
    {
    default: //any operator:
        assert(CG_EXPRTYP_UNDEF<pExprRoot->extyp && pExprRoot->extyp<=CG_MOVE);
        callBack(pCtx, pExprRoot);
        
        CGT_EnumExpr(pExprRoot->ops.pLhs, pCtx, callBack);
        if(pExprRoot->ops.pRhs) //maybe unary
            CGT_EnumExpr(pExprRoot->ops.pRhs, pCtx, callBack);
    break;

    case CG_LITERAL:   
        callBack(pCtx, pExprRoot);
    break;

    case CG_VARREF:    
        callBack(pCtx, pExprRoot);
    break;

    case CG_STRUCTREF: 
        callBack(pCtx, pExprRoot);
        CGT_EnumExpr(pExprRoot->str.pBase, pCtx, callBack);
    break;

    case CG_ARRAYREF:  
        callBack(pCtx, pExprRoot);
        
        CGT_EnumExpr(pExprRoot->arr.pBase, pCtx, callBack);
        
        for(i=0; i<pExprRoot->arr.nSubs; i++)
            CGT_EnumExpr(&pExprRoot->arr.pSubs[i], pCtx, callBack);
    break;

    case CG_FUNCALL:  
        callBack(pCtx, pExprRoot);
        
        for(i=0; i<pExprRoot->fun.nArgs; i++)
            CGT_EnumExpr(pExprRoot->fun.pArgs[i].pActual, pCtx, callBack);
    break;

    case CG_FUNMACRO:
        callBack(pCtx, pExprRoot);
    
        for(i=0; i<pExprRoot->mac.nArgs; i++)
            CGT_EnumExpr(pExprRoot->mac.pArgs[i].pActual, pCtx, callBack);
    break;

    case CG_BUILDIN:
        callBack(pCtx, pExprRoot);
    
        for(i=0; i<pExprRoot->bin.nArgs; i++)
            CGT_EnumExpr(&pExprRoot->bin.pArgs[i], pCtx, callBack);
    break;

    case CG_TMPVAR:
        callBack(pCtx, pExprRoot);
    break;

    case CG_TRIGVAR:
        callBack(pCtx, pExprRoot);
    break;
    
    case CG_STEPACTION:
        callBack(pCtx, pExprRoot);
    break;

    case CG_USEREXPR:  
        callBack(pCtx, pExprRoot);
    break;
    case CG_FBCALLEXPR:
        callBack(pCtx, pExprRoot);

        CGT_EnumExpr(pExprRoot->fbc.pInst, pCtx, callBack);
        for(i=0; i<pExprRoot->fbc.nArgs; i++)
            CGT_EnumExpr(pExprRoot->fbc.pArgs[i].pActual, pCtx, callBack);
    case CG_LABELREF:
        callBack(pCtx, pExprRoot);
    break;
    }
}


extern "C" void CGT_EnumExprStmt(
    const CG_STStmt* pStmtRoot,
    void*            pCtx,
    CGT_OnNewExpr    callBack
)
{
    int                  i;
    const CG_STCaseList* pCase;
    const CG_STStmt*     pStmt;

    pStmt = pStmtRoot;

Next:
    if(!pStmt)
        return;
    
    switch(pStmt->styp)
    {
    case CG_ASSIGN:
        if(pStmt->asns.pLhs) //NULL if "SFC transition assignment".
            CGT_EnumExpr(pStmt->asns.pLhs, pCtx, callBack);

        CGT_EnumExpr(pStmt->asns.pRhs, pCtx, callBack);
    break;
    case CG_CALLVOIDFUN:
        CGT_EnumExpr(pStmt->funs.pExpr, pCtx, callBack);
    break;
    case CG_CALLFB:
        CGT_EnumExpr(pStmt->fbcs.pInst, pCtx, callBack);
        for(i=0; i<pStmt->fbcs.nArgs; i++)
            CGT_EnumExpr(pStmt->fbcs.pArgs[i].pActual, pCtx, callBack);
    break;
    case CG_CASE:
        CGT_EnumExpr(pStmt->cases.pExpr, pCtx, callBack);
        for(pCase=pStmt->cases.pCases; pCase; pCase=pCase->pNextCase)
            CGT_EnumExprStmt(pCase->pStmts, pCtx, callBack);

        CGT_EnumExprStmt(pStmt->cases.pElse, pCtx, callBack);
    break;
    case CG_IF:
        CGT_EnumExpr(pStmt->ifs.pCond, pCtx, callBack);
        CGT_EnumExprStmt(pStmt->ifs.pThen, pCtx, callBack);
        if(pStmt->ifs.pElse)
            CGT_EnumExprStmt(pStmt->ifs.pElse,  pCtx, callBack);
    break;
    case CG_REPEAT:
        CGT_EnumExprStmt(pStmt->reps.pStmts, pCtx, callBack);
        CGT_EnumExpr(pStmt->reps.pUntil, pCtx, callBack);
    break;
    case CG_WHILE:
        CGT_EnumExpr(pStmt->whls.pWhile, pCtx, callBack);
        CGT_EnumExprStmt(pStmt->whls.pStmts, pCtx, callBack);
    break;
    case CG_FOR:
        CGT_EnumExpr(pStmt->fors.pAssign, pCtx, callBack);
        CGT_EnumExpr(pStmt->fors.pToExpr, pCtx, callBack);
        if(pStmt->fors.pByExpr)
            CGT_EnumExpr(pStmt->fors.pByExpr, pCtx, callBack);

        CGT_EnumExprStmt(pStmt->fors.pStmts, pCtx, callBack);
    break;
    case CG_GOTO:
    case CG_LABEL:
    case CG_EXIT:
    case CG_RETURN:
    case CG_EMPTY:
    case CG_NATIVE:
        //nothing to do
    break;
    case CG_SYNCHRONIZED:
        CGT_EnumExprStmt(pStmt->syns.pStmts, pCtx, callBack);
    break;
    case CG_LDFBDNETWORK:
        CGT_EnumExprStmt(pStmt->netw.pStmts, pCtx, callBack);
    break;

    default:
        assert(!"bad stmt type");
        return;    
    };//end union switch(stmt)

    pStmt = pStmt->pNext;
    goto Next;
}



extern "C" int CGT_GetExprEntryPath(
    const CG_ExprTree*  pExpr,
    CGT_PathElem*       pPathBuffer,
    int                 maxBuffer,
    CGT_PathElem**      ppRoot
)
{
    int                i;
    const CG_ExprTree* pE;

    pE = pExpr;
    for(i=maxBuffer-1;i>=0; )
    {
        switch(pE->extyp)
        {
        case CG_VARREF:    
            pPathBuffer[i].pEntry = pE->var.pEntry;
            *ppRoot = &pPathBuffer[i];
            assert(maxBuffer - i>0);
            return maxBuffer - i;

        case CG_STRUCTREF: 
            pPathBuffer[i--].pEntry = pE->str.pSelc;
            pE = pE->str.pBase;
        break;
        case CG_ARRAYREF:
            assert(pE->arr.nSubs==1);//TODO multi dim array
            if(pE->arr.pSubs[0].extyp==CG_LITERAL)
            {
                pPathBuffer[i].nIndex = 
                    (long)pE->arr.pSubs[0].lit.pLit->i64 -
                    pE->arr.pBase->pType->range[0].lRangeStart;
                assert(pPathBuffer[i].nIndex>=0);
            }
            else
            {
                pPathBuffer[i].nIndex = -1; //not lit index, flag -1 for 'any field'
            }
            i--;
            pE = pE->arr.pBase;
        break;
        case CG_NOOP:
             //can this ever happen???? could be (a).b.(c)
             //I think pure syntax does not allow (a).b, but be safe..
             pE = pE->ops.pLhs;
        break;
        default:
            //happens if we have fun().member or other strange things....
            *ppRoot = NULL;
            return 0;
        }
    }

    assert(!"CGT_GetExprEntryPath buffer to small");
    *ppRoot = NULL;
    return -1;
}



void CGT_DumpEntryPath(
    FILE*                pFile,
    const TCHAR*         pszPre,
    int                  nPath,
    const CGT_PathElem*  pPath,
    const TCHAR*         pszPost
)
{
    int    i;
    TCHAR  buffer[64];
    if(!pFile)
        pFile = stdout;

    if(pszPre)
        _fputts(pszPre, pFile);
   
    for(i=0; i<nPath; i++)
    {
        if(i>0)
            _fputtc(_T('.'), pFile);

        _fputts(pPath[i].pEntry->pszId, pFile);
        if(pPath[i].pEntry->pType->bIsArray)
        {
            i++;
            assert(i<nPath);
            _fputtc(_T('.'), pFile);
            
            if(pPath[i].nIndex<0)
                _fputtc(_T('*'), pFile);
            else
                _fputts(ltoa(pPath[i].nIndex, buffer, 10), pFile);
        }
    }

    if(pszPost)
        _fputts(pszPost, pFile);
}

const CG_DType* CGT_ParseAccessPath(
    CGT_CDiagMsg*        pdm,     //for err msg
    const CG_Edipos*     pEdp,
    ICG_IECFront*        pIF, 
    TCHAR*               pszPath, //on successfull exit it contains a normalised path (white spaces removed)
    const CG_Symtab*     pSym,    //local scope
    const CG_Resource*   pRes,    //global scope
    bool*                pbGlobal
)
{
    int                nPath;
    TCHAR*             pszFields[256];
    const CG_SymEntry* pEntry = NULL;
    CG_Edipos          edptmp = *pEdp;
    int                nPathStrLen;
    const CG_DType*    pType= NULL;
    

    *pbGlobal = false;

    nPathStrLen = _tcslen(pszPath);
    nPath = FC_StringSplitBuf(pszPath, '.', pszFields, 256, 1);
    if(nPath>=256)
    {
        pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: path too long (max is 255)"));
        return NULL;
    }
    if(nPath<1)
    {
        pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: missing path"));
        return NULL;
    }

    if(pSym)
    {
        pEntry = pIF->lookUpEntry(pSym, pszFields[0]);
        if(pEntry)
        {
            if(pEntry->declScope==CG_VAR_EXTERNAL)
            {
                pEntry = NULL;
            }
            else if(pEntry->declScope!=CG_VAR && pEntry->declScope!=CG_VAR_INPUT && pEntry->declScope!=CG_VAR_OUTPUT)
            {
                pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: '")<<pszFields[0]<<_T("' is not a VAR(_INPUT|OUTPUT)"));
                return NULL;
            }
        }
    }
    if(!pEntry && pRes)
    {
        pEntry = pIF->lookUpGlobVar(pRes, pszFields[0]);
        if(pEntry)
            *pbGlobal = true;
    }

    if(!pEntry)
    {
        pdm->msg1(CG_E_UNDEF_VAR, &edptmp, pszFields[0]);
        return NULL;
    }

    pType = pEntry->pType;


    for(int j=1; j<nPath; j++)
    {
        CG_IncColEdipos(&edptmp, pszFields[j] - pszFields[j-1]);

        TCHAR* pszEnd;

        if(pszFields[j][0]=='[')
        {
            pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: not supported: '<arrayvar>[expr]', use <arrayvar>.<index> syntax instead"));
            pEntry = NULL;
            break;
        }

        strtoul(pszFields[j], &pszEnd, 10);
        if(_istdigit(pszFields[j][0]) && *pszEnd==0)  //.<array idx> TODO check ranges, put this to the front end any way...
        {
            if(!pEntry->pType->bIsArray)
            {
                pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: '...")<<pszFields[j-1]<<'.'<<pszFields[j]<<_T("...': '")<<pszFields[j-1]<<_T("' is not an array"));
                pEntry = NULL;
                break;
            }
            pType = pIF->getArrayBaseType(pEntry->pType);
        }
        else if(!pEntry->pType->pSym)
        {
            pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: '...")<<pszFields[j-1]<<'.'<<pszFields[j]<<_T("...': '")<<pszFields[j-1]<<_T("' is not a struct/FB"));
            pEntry = NULL;
            break;
        }
        else
        {
            pEntry = pIF->lookUpEntry(pEntry->pType->pSym, pszFields[j]);
            if(pEntry)
            {
                if(pEntry->declScope!=CG_VAR && pEntry->declScope!=CG_VAR_INPUT && pEntry->declScope!=CG_VAR_OUTPUT)
                {
                    pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: '...")<<pszFields[j-1]<<'.'<<pszFields[j]<<_T("...': '")<<pszFields[j]<<_T("' is not a VAR(_INPUT|OUTPUT)"));
                    pEntry = NULL;
                    break;
                }
            }
            else
            {
                pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("variable path: '...")<<pszFields[j-1]<<'.'<<pszFields[j]<<_T("...': '")<<pszFields[j]<<_T("' member undefined"));
                pEntry = NULL;
                break;
            }
            pType = pEntry->pType;
        }
    }

    if(pEntry && pType)
    {
        //HACK: must do this in front end using the parser !!
        int iout = 0;
        for(j=0; j<nPathStrLen; j++)
        {
            if(pszPath[j]==0)
               pszPath[iout++] = '.';
            else if(!isspace(pszPath[j]))
               pszPath[iout++] = pszPath[j];
        }

        pszPath[iout] = 0;
        return pType;
    }
    else
    {
        return NULL;//any error above...
    }
}




//##AC beg
static int CGT_EnumVASTExpr(
    const CG_ExprTree*   pEx,
    const CG_STStmt*     pFromStmt,
    const CG_SymEntry*   pFormal,
    void*                pCtx,
    CGT_VASTExprCallBack callBack
);
static int VASTEnumFBCall(
    const CG_STStmt*    pFromStmt,
    const CG_SymEntry*  pFormalFBCEx,
    const CG_ExprTree*  pInst,//strict IEC: always a CG_VARREF, future  //##AC
                              //version may support array of FBs        //##AC
    long                nArgs,//>=0 number of in/out assignments        //##AC
    const CG_ArgExpr*   pArgs,//NULL or array of arg expr.              //##AC 
    void*               pCtx,
    CGT_VASTExprCallBack callBack
)
{
    int                 i;
    int                 iOk;
    int                 nPath;
    CGT_PathElem*       pPath;
    const CG_ArgExpr*   pArgEx;
    const CG_SymEntry*  pFormal;
    const CG_ExprTree*  pActual;
    CGT_PathElem        pathBuffer[CG_MAX_INST_PATH_LEN];

    nPath = CGT_GetExprEntryPath(
                pInst, 
                pathBuffer, 
                FC_ARRAY_LEN(pathBuffer)-1,//-1 essential! below FB-member is added to path
                &pPath
            ) +1; //+1 will add FB-member below

    iOk = CGT_EnumVASTExpr(pInst, pFromStmt, pFormalFBCEx, pCtx, callBack);

    for(i=0; iOk && i<nArgs; i++)
    {
        pArgEx  = &pArgs[i];
        pFormal = pArgEx->pFormal;
        pActual = pArgEx->pActual;
        pathBuffer[FC_ARRAY_LEN(pathBuffer)-1].pEntry = pFormal;

        switch(pFormal->declScope)
        {
        case CG_VAR_IN_OUT:
            if(pArgEx->argFlags & CG_EXPR_ARG_OUTASN)
                iOk = CGT_EnumVASTExpr(pActual, pFromStmt, NULL, pCtx, callBack);
            else
                iOk = CGT_EnumVASTExpr(pActual, pFromStmt, pFormal, pCtx, callBack);
        break;
        case CG_VAR_INPUT:
            iOk = CGT_EnumVASTExpr(pActual, pFromStmt, NULL, pCtx, callBack);
            iOk = iOk ? callBack(pCtx, &pActual->edp, CG_ACCESS_W, pFromStmt, NULL, nPath, pPath, NULL) : 0;
        break;
        case CG_VAR_OUTPUT:
            iOk = callBack(pCtx, &pActual->edp, CG_ACCESS_R, pFromStmt, NULL, nPath, pPath, NULL);
            iOk = iOk ? CGT_EnumVASTExpr(pActual, pFromStmt, NULL, pCtx, callBack) : 0;
        break;
        default: 
            assert("bad decl scope of FB param");
            iOk = 0;
        }
    }

    return iOk;
}
//##AC end




static int CGT_EnumVASTExpr(
    const CG_ExprTree*   pEx,
    const CG_STStmt*     pFromStmt,
    const CG_SymEntry*   pFormal,
    void*                pCtx,
    CGT_VASTExprCallBack callBack
)
{
    int                 i;
    const CG_ArgExpr*   pArgEx;
    int                 nPath;
    CGT_PathElem*       pPath;
    CGT_PathElem        pathBuffer[CG_MAX_INST_PATH_LEN];


    switch(pEx->extyp)
    {
    default: //any operator:
        assert(CG_EXPRTYP_UNDEF<pEx->extyp && pEx->extyp<=CG_MOVE);
        
        if(!CGT_EnumVASTExpr(pEx->ops.pLhs, pFromStmt, NULL, pCtx, callBack))
            return 0;
        if(pEx->ops.pRhs) //maybe unary
            if(!CGT_EnumVASTExpr(pEx->ops.pRhs, pFromStmt, NULL, pCtx, callBack))
                return 0;
    break;

    case CG_LITERAL:   
    break;

    case CG_VARREF:
        if((pEx->var.accFlags & CG_ACCESS_B)==0)
        {
            nPath = CGT_GetExprEntryPath(pEx, pathBuffer, FC_ARRAY_LEN(pathBuffer), &pPath);
            assert(nPath>0 && pPath);
            if(!callBack(pCtx, &pEx->edp, pEx->var.accFlags, pFromStmt, pEx, nPath, pPath, pFormal))
                return 0;
        }
    break;

    case CG_STRUCTREF:
        if((pEx->str.accFlags & CG_ACCESS_B)==0)
        {
            nPath = CGT_GetExprEntryPath(pEx, pathBuffer, FC_ARRAY_LEN(pathBuffer), &pPath);
            assert(nPath>0 && pPath);
            if(!callBack(pCtx, &pEx->edp, pEx->str.accFlags, pFromStmt, pEx, nPath, pPath, pFormal))
                return 0;
        }
        if(!CGT_EnumVASTExpr(pEx->str.pBase, pFromStmt, NULL, pCtx, callBack))
            return 0;
    break;

    case CG_ARRAYREF:   
        if((pEx->arr.accFlags & CG_ACCESS_B)==0)
        {
            nPath = CGT_GetExprEntryPath(pEx, pathBuffer, FC_ARRAY_LEN(pathBuffer), &pPath);
            assert(nPath>0 && pPath);
            if(!callBack(pCtx, &pEx->edp, pEx->arr.accFlags, pFromStmt, pEx, nPath, pPath, pFormal))
                return 0;
        }
        
        if(!CGT_EnumVASTExpr(pEx->arr.pBase, pFromStmt, NULL, pCtx, callBack))
            return 0;

        for(i=0; i<pEx->arr.nSubs; i++)
        {
            if(!CGT_EnumVASTExpr(&pEx->arr.pSubs[i], pFromStmt, NULL, pCtx, callBack))
                return 0;
        }
    break;

    case CG_FUNCALL:  
        for(i=0; i<pEx->fun.nArgs; i++)
        {
            pArgEx = &pEx->fun.pArgs[i];
            if(!CGT_EnumVASTExpr(pArgEx->pActual, pFromStmt, pArgEx->pFormal, pCtx, callBack))
                return 0;
        }
    break;

    case CG_FUNMACRO:
        for(i=0; i<pEx->mac.nArgs; i++)
        {
            pArgEx = &pEx->mac.pArgs[i];
            if(!CGT_EnumVASTExpr(pArgEx->pActual, pFromStmt, pArgEx->pFormal, pCtx, callBack))
                return 0;
        }
    break;

    case CG_BUILDIN:
        for(i=0; i<pEx->bin.nArgs; i++)
        {
            if(!CGT_EnumVASTExpr(&pEx->bin.pArgs[i], pFromStmt, NULL, pCtx, callBack))
                return 0;
        }
    break;

    case CG_TMPVAR:
        //assert tmp.member or tmp[7] should not happen, at the moment...
        assert((pEx->var.accFlags & CG_ACCESS_B)==0); 
        if(!callBack(pCtx, &pEx->edp, pEx->tmp.accFlags, pFromStmt, pEx, 0, NULL, pFormal))
            return 0;
    break;
    case CG_TRIGVAR:
    case CG_STEPACTION:
    break;
    case CG_USEREXPR:  
        assert(!"unexpected CG_USEREXPR");
    break;
    case CG_FBCALLEXPR:  //##AC
        if(!VASTEnumFBCall(pFromStmt, pFormal, pEx->fbc.pInst, pEx->fbc.nArgs, pEx->fbc.pArgs, pCtx, callBack))
            return 0;
    break;
    case CG_LABELREF:
    break;
    }

    return 1;
}



extern "C" int CGT_EnumVASTExprStmt(
    const CG_STStmt*     pStmtRoot,
    void*                pCtx,
    CGT_VASTExprCallBack callBack
)
{
    int                  i;
    int                  iOk;
    const CG_STCaseList* pCase;
    const CG_STStmt*     pStmt;

    pStmt = pStmtRoot;

Next:
    if(!pStmt)
        return 1;
    
    switch(pStmt->styp)
    {
    case CG_ASSIGN:
        if(pStmt->asns.pLhs) //NULL if "SFC transition assignment".
            iOk = CGT_EnumVASTExpr(pStmt->asns.pLhs, pStmt, NULL, pCtx, callBack);
        else
            iOk = 1;

        iOk = iOk ? CGT_EnumVASTExpr(pStmt->asns.pRhs, pStmt, NULL, pCtx, callBack) : 0;
    break;
    case CG_CALLVOIDFUN:
        iOk = CGT_EnumVASTExpr(pStmt->funs.pExpr, pStmt, NULL, pCtx, callBack);
    break;
    case CG_CALLFB:
        iOk = VASTEnumFBCall(pStmt, NULL, pStmt->fbcs.pInst, pStmt->fbcs.nArgs, pStmt->fbcs.pArgs, pCtx, callBack); //##AC
    break;
    case CG_CASE:
        iOk = CGT_EnumVASTExpr(pStmt->cases.pExpr, pStmt, NULL, pCtx, callBack);

        for(pCase=pStmt->cases.pCases; iOk && pCase; pCase=pCase->pNextCase)
            iOk = CGT_EnumVASTExprStmt(pCase->pStmts, pCtx, callBack);

        if(iOk && pStmt->cases.pElse)
            iOk = CGT_EnumVASTExprStmt(pStmt->cases.pElse, pCtx, callBack);
    break;
    case CG_IF:
        iOk = CGT_EnumVASTExpr(pStmt->ifs.pCond, pStmt, NULL, pCtx, callBack);
        iOk = iOk ? CGT_EnumVASTExprStmt(pStmt->ifs.pThen, pCtx, callBack) : 0;
        if(iOk && pStmt->ifs.pElse)
            iOk = CGT_EnumVASTExprStmt(pStmt->ifs.pElse,  pCtx, callBack);
    break;
    case CG_REPEAT:
        iOk = CGT_EnumVASTExpr(pStmt->reps.pUntil, pStmt, NULL, pCtx, callBack);
        iOk = iOk ? CGT_EnumVASTExprStmt(pStmt->reps.pStmts, pCtx, callBack) : 0;
    break;
    case CG_WHILE:
        iOk = CGT_EnumVASTExpr(pStmt->whls.pWhile, pStmt, NULL, pCtx, callBack);
        iOk = iOk ? CGT_EnumVASTExprStmt(pStmt->whls.pStmts, pCtx, callBack) : 0;
    break;
    case CG_FOR:
        iOk = CGT_EnumVASTExpr(pStmt->fors.pAssign, pStmt, NULL, pCtx, callBack);
        iOk = iOk ? CGT_EnumVASTExpr(pStmt->fors.pToExpr, pStmt, NULL, pCtx, callBack) : 0;
        if(iOk && pStmt->fors.pByExpr)
            iOk = CGT_EnumVASTExpr(pStmt->fors.pByExpr, pStmt, NULL, pCtx, callBack);

        iOk = iOk ? CGT_EnumVASTExprStmt(pStmt->fors.pStmts, pCtx, callBack) : 0;
    break;
    case CG_GOTO:
    case CG_LABEL:
    case CG_EXIT:
    case CG_RETURN:
    case CG_EMPTY:
        iOk = 1;
        //nothing to do
    break;
    case CG_NATIVE:
        for(i=0, iOk=1; iOk && i<pStmt->nats.nRefEntries; i++)
        {
            CGT_PathElem path;
            path.pEntry = pStmt->nats.pRefEntries[i].pEntry;
            iOk = callBack(pCtx, &pStmt->nats.pRefEntries[i].edp, CG_ACCESS_N, pStmt, NULL, 1, &path, NULL);
        }
    break;
    case CG_SYNCHRONIZED:
        iOk = CGT_EnumVASTExprStmt(pStmt->syns.pStmts, pCtx, callBack);
    break;
    case CG_LDFBDNETWORK:
        iOk = CGT_EnumVASTExprStmt(pStmt->netw.pStmts, pCtx, callBack);
    break;

    default:
        assert(!"bad stmt type");
        iOk = 0;    
    };//end union switch(stmt)


    if(!iOk)
        return 0;

    pStmt = pStmt->pNext;
    goto Next;
}

extern "C" WORD CGT_GetVASTAccFlags(
    const CG_ExprTree* pVASTExpr
)
{
    switch(pVASTExpr->extyp)
    {
    case CG_VARREF:      return pVASTExpr->var.accFlags;
    case CG_STRUCTREF:   return pVASTExpr->str.accFlags;
    case CG_ARRAYREF:    return pVASTExpr->arr.accFlags;
    case CG_TMPVAR:      return pVASTExpr->tmp.accFlags;
    default:
        assert(!"not a VAST expr. node");
        return 0;
    }
}


//##AC beg
static void DumpFBCall(
    
    const CG_ExprTree*  pInst,//strict IEC: always a CG_VARREF, future
                              //version may support array of FBs
    long                nArgs,//>=0 number of in/out assignments
    const CG_ArgExpr*   pArgs,//NULL or array of arg expr.
    CGT_CDmp&           dmp,
    const TCHAR*        pszTmpPrFix
)
{
    int i;

    CGT_DumpSTExpr(pInst, dmp, pszTmpPrFix);
    dmp<<'(';
    for(i=0; i<nArgs; i++)
    {
        const CG_ArgExpr* pAE = &pArgs[i];
        if(i>0)
            dmp<<_T(", ");
        if(pAE->argFlags & CG_EXPR_ARG_OUTASN)
        {
            if(pAE->argFlags & CG_EXPR_ARG_NEGOUT)
                dmp<<"not ";
            dmp<<pAE->pFormal->pszId<<_T(" => ");
        }
        else
        {
            assert((pAE->argFlags & CG_EXPR_ARG_NEGOUT)==0);
            dmp<<pAE->pFormal->pszId<<_T(" := ");
        }
        CGT_DumpSTExpr(pAE->pActual, dmp, pszTmpPrFix);
    }
    dmp<<')';
}
//##AC end


extern "C" void CGT_DumpSTExpr(
    const CG_ExprTree* pExpr,
    CGT_CDmp&          dmp,
    const TCHAR*       pszTmpPrFix
)
{
    int i;

    switch(pExpr->extyp)
    {
    //unary ops:
    case CG_NOT:     
        dmp<<_T("NOT ");
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
    break;
    case CG_UMINUS:  
        dmp<<'-';
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
    break;
    case CG_NOOP:    
        dmp<<'(';
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<')';
    break;

    //binary ops:
    case CG_OR: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" or ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_XOR: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" xor ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_AND:
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" & ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_EQ: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" = ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_NE: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" <> ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_LT: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" < ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_LE: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" <= ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_GT: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" > ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_GE:
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" >= ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_ADD: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" + ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_SUB: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" - ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_MUL: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" * ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_DIV: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" / ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_MOD: 
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" mod ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_POW:
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" ** ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;
    case CG_MOVE:
        CGT_DumpSTExpr(pExpr->ops.pLhs, dmp, pszTmpPrFix);
        dmp<<_T(" := ");
        CGT_DumpSTExpr(pExpr->ops.pRhs, dmp, pszTmpPrFix);
    break;



    case CG_LITERAL:
        if(pExpr->lit.pConst)
            dmp<<pExpr->lit.pConst->pszId;
        else
            dmp<<pExpr->lit.pLit->psz;
    break;

    case CG_VARREF:    
        dmp<<pExpr->var.pEntry->pszId;
    break;

    case CG_STRUCTREF:
        CGT_DumpSTExpr(pExpr->str.pBase, dmp, pszTmpPrFix);
        dmp<<_T('.')<<pExpr->str.pSelc->pszId;
    break;

    case CG_ARRAYREF:  
        CGT_DumpSTExpr(pExpr->arr.pBase, dmp, pszTmpPrFix);

        dmp<<'[';
        for(i=0; i<pExpr->arr.nSubs; i++)
        {
            if(i>0)
                dmp<<',';

            CGT_DumpSTExpr(&pExpr->arr.pSubs[i], dmp, pszTmpPrFix);
        }
        dmp<<']';
    break;

    case CG_FUNCALL:  
        dmp<<pExpr->fun.pSym->pszId;
        
        dmp<<'(';
        for(i=0; i<pExpr->fun.nArgs; i++)
        {
            if(i>0)
                dmp<<_T(", ");
            CGT_DumpSTExpr(pExpr->fun.pArgs[i].pActual, dmp, pszTmpPrFix);
        }
        dmp<<')';
    break;

    case CG_FUNMACRO:
        dmp<<pExpr->mac.pMac->pSym->pszId;
        
        dmp<<'(';
        for(i=0; i<pExpr->mac.nArgs; i++)
        {
            if(i>0)
                dmp<<_T(", ");
            CGT_DumpSTExpr(pExpr->mac.pArgs[i].pActual, dmp, pszTmpPrFix);
        }
        dmp<<')';
    break;

    case CG_BUILDIN:
        dmp<<pExpr->bin.pszId;
    
        dmp<<'(';
        for(i=0; i<pExpr->bin.nArgs; i++)
        {
            if(i>0)
                dmp<<_T(", ");
            CGT_DumpSTExpr(&pExpr->bin.pArgs[i], dmp, pszTmpPrFix);
        }
        dmp<<')';
    break;


    case CG_TMPVAR:
        dmp<<(pszTmpPrFix?pszTmpPrFix:_T("_tmp"))<<pExpr->tmp.lTmpNum;
    break;

    case CG_TRIGVAR:
        dmp<<(pszTmpPrFix?pszTmpPrFix:_T("_tmp"));
        if(pExpr->trg.trigTyp == CG_TMP_R_TRIG)
            dmp<<_T("_R");
        else
            dmp<<_T("_F");
        dmp<<pExpr->tmp.lTmpNum;
    break;
    
    case CG_STEPACTION:
        dmp<<pExpr->sfc.pszSA;
        dmp<<'.';
        switch(pExpr->sfc.typ)
        {
        case CG_STEP_X:    dmp<<_T(".X");
        case CG_STEP_T:    dmp<<_T(".T");
        case CG_ACTION_A:  dmp<<_T(".A");
        case CG_ACTION_Q:  dmp<<_T(".Q");
        default:
            assert(!"bad CG_STEPACTION enum");
        }
    break;

    case CG_USEREXPR:  
        dmp<<_T("<CG_USEREXPR>");//TODO
    break;

    case CG_FBCALLEXPR:
        DumpFBCall(pExpr->fbc.pInst, pExpr->fbc.nArgs, pExpr->fbc.pArgs, dmp, pszTmpPrFix);
    break;

    case CG_LABELREF:
        dmp<<pExpr->lbl.pszLbl<<':';
    break;

    default: 
        assert(!"bad expr typ enum");
    }
}



extern "C" void CGT_DumpSTStmt(
    const CG_STStmt*   pStmtRoot,
    CGT_CDmp&          dmp,
    const TCHAR*       pszTmpPrFix,
    bool               bRecurse   
)
{
    int                  i;
    const CG_STCaseList* pCase;
    const CG_STStmt*     pStmt;

    pStmt = pStmtRoot;

    
Next:
    if(!pStmt)
        return;
    
    switch(pStmt->styp)
    {
    case CG_ASSIGN:
    {
        bool bRefAsn = false;
        if(pStmt->asns.pLhs)
        {
            //NULL if "SFC transition assignment".
            CGT_DumpSTExpr(pStmt->asns.pLhs, dmp, pszTmpPrFix);
            bRefAsn = pStmt->asns.pLhs->extyp==CG_TMPVAR && pStmt->asns.pLhs->tmp.pRefExpr;
        }
        if(bRefAsn)
            dmp<<_T(" := &"); 
        else
            dmp<<_T(" := "); 

        CGT_DumpSTExpr(pStmt->asns.pRhs, dmp, pszTmpPrFix);
        dmp<<';'; 
    }
    break;
    case CG_CALLVOIDFUN:
         CGT_DumpSTExpr(pStmt->funs.pExpr, dmp, pszTmpPrFix);
        dmp<<';';
    break;
    case CG_CALLFB:
        DumpFBCall(pStmt->fbcs.pInst, pStmt->fbcs.nArgs, pStmt->fbcs.pArgs, dmp, pszTmpPrFix);//##AC
        dmp<<';';
    break;
    case CG_CASE:
        dmp<<_T("case "); 
        CGT_DumpSTExpr(pStmt->cases.pExpr, dmp, pszTmpPrFix);
        dmp<<_T(" of"); 
        if(bRecurse)
        {
            dmp<<'\n'; 
            for(pCase=pStmt->cases.pCases; pCase; pCase=pCase->pNextCase)
            {
                for(i=0; i<pCase->nRanges; i++)
                {
                    if(i>0)
                        dmp<<_T(", "); 
                    dmp<<pCase->ranges[i].lRangeStart;
                    if(pCase->ranges[i].ulRangeLength!=0)
                        dmp<<_T("..")<<(long)(pCase->ranges[i].lRangeStart+pCase->ranges[i].ulRangeLength);
                }
                dmp<<_T(":\n"); 
                dmp.incrIndent();
                CGT_DumpSTStmt(pCase->pStmts, dmp, pszTmpPrFix, bRecurse);
                dmp<<'\n'; 
                dmp.decrIndent();
            }

            if(pStmt->cases.pElse)
            {
                dmp<<_T("else\n");
                dmp.incrIndent();
                CGT_DumpSTStmt(pStmt->cases.pElse, dmp, pszTmpPrFix, bRecurse);
                dmp<<_T('\n');
                dmp.decrIndent();
            }
            dmp<<_T("end_case;");
        }
    break;
    case CG_IF:
        dmp<<_T("if ");
        CGT_DumpSTExpr(pStmt->ifs.pCond, dmp, pszTmpPrFix);
        dmp<<_T(" then");
        if(bRecurse)
        {
            dmp<<'\n'; 
            dmp.incrIndent();
            CGT_DumpSTStmt(pStmt->ifs.pThen, dmp, pszTmpPrFix, bRecurse);
            dmp<<_T('\n');
            dmp.decrIndent();
            if(pStmt->ifs.pElse)
            {
                dmp<<_T("else\n");
                dmp.incrIndent();
                CGT_DumpSTStmt(pStmt->ifs.pElse, dmp, pszTmpPrFix, bRecurse);
                dmp<<_T('\n');
                dmp.decrIndent();
            }
            dmp<<_T("end_if;");
        }
    break;
    case CG_REPEAT:
        dmp<<_T("repeat");
        if(bRecurse)
        {
            dmp<<'\n'; 
            dmp.incrIndent();
            CGT_DumpSTStmt(pStmt->reps.pStmts, dmp, pszTmpPrFix, bRecurse);
            dmp<<_T('\n');
            dmp.decrIndent();
            dmp<<_T("until ");
            CGT_DumpSTExpr(pStmt->reps.pUntil, dmp, pszTmpPrFix);
            dmp<<_T(" end_repeat;");
        }
    break;
    case CG_WHILE:
        dmp<<_T("while ");
        CGT_DumpSTExpr(pStmt->whls.pWhile, dmp, pszTmpPrFix);
        dmp<<_T("do");
        if(bRecurse)
        {
            dmp<<'\n'; 
            dmp.incrIndent();
            CGT_DumpSTStmt(pStmt->whls.pStmts, dmp, pszTmpPrFix, bRecurse);
            dmp<<_T('\n');
            dmp.decrIndent();
            dmp<<_T("end_while;");
        }
    break;
    case CG_FOR:
        dmp<<_T("for ");
        dmp<<_T(":=");
        CGT_DumpSTExpr(pStmt->fors.pAssign, dmp, pszTmpPrFix);
        dmp<<_T(" to ");
        CGT_DumpSTExpr(pStmt->fors.pToExpr, dmp, pszTmpPrFix);
        if(pStmt->fors.pByExpr)
        {
            dmp<<_T(" by ");
            CGT_DumpSTExpr(pStmt->fors.pByExpr, dmp, pszTmpPrFix);
        }
        dmp<<_T(" do");

        if(bRecurse)
        {
            dmp<<'\n'; 
            dmp.incrIndent();
            CGT_DumpSTStmt(pStmt->fors.pStmts, dmp, pszTmpPrFix, bRecurse);
            dmp<<_T('\n');
            dmp.decrIndent();
            dmp<<_T("end_for;");
        }
    break;
    case CG_GOTO:
        dmp<<_T("goto ")<<pStmt->gots.pLblStmt->lbls.pszLbl<<';';
    break;
    case CG_LABEL:
        dmp<<_T("goto ")<<pStmt->lbls.pszLbl<<';';
    break;
    case CG_EXIT:
        dmp<<_T("exit;");
    break;
    case CG_RETURN:
        dmp<<_T("return;");
    break;
    case CG_EMPTY:
        dmp<<_T(";");
    break;
    case CG_NATIVE:
        dmp<<_T("native");
        pStmt->nats.pszNative;
        dmp<<_T("end_native;");
        //nothing to do
    break;
    case CG_SYNCHRONIZED:
        dmp<<_T("synchronized");
        if(bRecurse)
        {
            dmp<<'\n'; 
            dmp.incrIndent();
            CGT_DumpSTStmt(pStmt->syns.pStmts, dmp, pszTmpPrFix, bRecurse);
            dmp<<_T('\n');
            dmp.decrIndent();
            dmp<<_T("end_synchronized;");
        }
    break;
    case CG_LDFBDNETWORK:
        if(bRecurse)
            CGT_DumpSTStmt(pStmt->netw.pStmts, dmp, pszTmpPrFix, bRecurse);
    break;

    default:
        assert(!"bad stmt type");
        return;    
    };//end union switch(stmt)

    if(bRecurse)
    {
        pStmt = pStmt->pNext;
        goto Next;
    }
}



