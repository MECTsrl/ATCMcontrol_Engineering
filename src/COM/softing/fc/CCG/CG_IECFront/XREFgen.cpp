
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "XREFgen.h"
#include "cgt_misc.h"
#include "cgt_tools.h"
#include "parsers.h"

//----  Static Initializations:   ----------------------------------*


//VARIABLENAME;VARIABLE_SCOPE;VARIABLE_TYPE;EDITOR_POSITION
static void ST_DumpXRefDeclRecord(
    CGT_CDiagMsg*       pdm, 
    CGT_CDmp&           dmp, 
    const CG_SymEntry*  pEntry,
    const CG_Edipos*    pEdpUse
)
{
    TCHAR              szEdipos[MAX_PATH];

    //name:
    dmp<<pEntry->pszId<<_T(";");
    
    //scope:
    dmp<<ST_StoreClassName(pEntry->declScope);
    if(pEntry->declMod & CG_DMOD_CONSTANT)
        dmp<<_T("_CONSTANT");
    if(pEntry->declMod & CG_DMOD_RETAIN)
        dmp<<_T("_RETAIN");
    if(pEntry->declMod & CG_DMOD_NON_RETAIN)
        dmp<<_T("_NON_RETAIN");
    dmp<<_T(";");
    
    //type:
    //ignore arrayness and string len, also R/F_EDGE 
    if(pEntry->pType->pSym)
    {
        dmp<<pEntry->pType->pSym->pszId;
    }
    else
    {
        CG_DTYPES type = pEntry->pType->type;
        if(type==CG_BOOL_R_EDGE || type==CG_BOOL_F_EDGE)
            type = CG_BOOL;
        dmp<<ST_CG_DTYPES_ToString(type);
    }
    dmp<<_T(";");

    //edipos:
    pdm->edpToStrBuffer(pEdpUse ? pEdpUse : &pEntry->edp, szEdipos, FC_ARRAY_LEN(szEdipos), true);
    dmp<<szEdipos<<_T("\n");
}


//VAST CallBack struct & function:
struct ExprXREFCtx
{
    CGT_CDiagMsg*     pdm;
    FC_CString        record;
    FC_CHashStr<int>* pRefHash;
    CGT_CDmp*         pDmp;

    ExprXREFCtx() : record(MAX_PATH) { pDmp= NULL; pRefHash= NULL;}
};
extern "C" int XrefVASTExprCallBack(
    void*               pCtx,
    const CG_Edipos*    pEdp,
    WORD                accFlags,
    const CG_STStmt*    pFromStmt,
    const CG_ExprTree*  pFromExpr,
    int                 nPath,
    const CGT_PathElem* pPath,
    const CG_SymEntry*  pArgEntry
)
{
    int          i;
    bool         bOk;
    TCHAR        szEDPNoFile[2*MAX_PATH];
    ExprXREFCtx* pContext = (ExprXREFCtx*)pCtx;;
    FC_CString&  record   = pContext->record;


    //pPath is a.b.c or a[<expr>] or a[<expr>].a.b.c  ...
     //[REFERENCE]
    //VARIABLENAME;VAR_ACCESS,EDITOR_POSITION
    //Where VAR_ACCESS is READ|WRITE|READWRITE

    //ignore all tmp var stuff for xref:
    if(pFromExpr && pFromExpr->extyp==CG_TMPVAR)
        return 1;

    //only for tmp var access the nPath can be 0:
    assert(nPath>0);

    //TODO ??? no good: FB inst call is always rw acc:
    //and prod management wants the fb.in/out not as extra 
    //read/write
    //HACK: ignore all refs FB.in,FB.out that come from FB(in:=1, out:=var) 
    if(pFromStmt->styp==CG_CALLFB && !pFromExpr)
        return 1;



    record.clear();
    for(i=0; i<nPath; i++)
    {
        if(i>0)
            record.addc(_T('.'));
        record<<pPath[i].pEntry->pszId;

        //skip all arrayness info for xref purpose:
        if(pPath[i].pEntry->pType->bIsArray)
            i++;
    }
    record.addc(_T(';'));

    assert(accFlags & (CG_ACCESS_ANY_R|CG_ACCESS_ANY_W));
    if(accFlags & CG_ACCESS_ANY_R)
        record<<_T("READ");
    if(accFlags & CG_ACCESS_ANY_W)
        record<<_T("WRITE");
    
    bOk = pContext->pdm->edpToStrBuffer(pEdp, szEDPNoFile, FC_ARRAY_LEN(szEDPNoFile), true);
    assert(bOk);

    record.addc(_T(';'));
    record<<szEDPNoFile;
    record.addc(_T('\n'));

    //if hash use it else dmp now:
    if(pContext->pRefHash)
        pContext->pRefHash->insertEx(record, 0, false, NULL);
    else
       (*pContext->pDmp)<<record;

    //return true to carry on enum VAST expr:
    return 1;
}




static void DumpXREFReferenceSection(
    CGT_CDiagMsg*        pdm,
    CGT_CDmp&            dmp,
    ST_CSymtabPrv*       pSymPrv,
    const ST_CBody*      pBody
)
{
    ExprXREFCtx                     callBackCtx;
    TCHAR                           szEDPNoFile[MAX_PATH];
    const FC_CGArray<ST_CONST_REF>* pRefCst;



    callBackCtx.pdm      = pdm;
    callBackCtx.pDmp     = &dmp;
    callBackCtx.pRefHash = NULL;
    
    if(pBody && pSymPrv->getCGSym()->srcTyp==CG_SRC_TYPE_4GR)
    {
        //alloc hash, why?
        //reason: there are situation when the same expr appears
        //more than once with the same edipos in the body expr list
        //this happens only for 4gr input if 
        //a) the expr gets reparsed (see st_ParseContext::parse4GRBox) 
        //b) inside a VarRead/Write box there is something like
        //   fun(x,x,x)*x**x (we allow this in ATCMControl)
        //c) tmp var optimizations
        //
        //The full record is hashed with VARIABLENAME;VAR_ACCESS,EDITOR_POSITION
        //Note: not ignore case compare is ok since the VARIABLENAME is the 
        //      normalised variant of the CG_SymEntry::pszId which is unique. 
        //      Also the other parts of the record are case sensitive even the edipos
        //      since we ommit the file name (in fact the file name should be the same
        //      pointer for all expr in the body...)
        //Performance: alloc initial size of hash very big (half of all expr nodes).
        callBackCtx.pRefHash = new FC_CHashStr<int>(false, true, pBody->m_exprMem.countExpr()/2);
    }
    
    dmp<<_T("[REFERENCE]\n");
    //hdr/body ref CONSTs:
    pRefCst = pSymPrv->getCONSTRefList();
    if(pRefCst)
    {
        for(int i = 0; i<pRefCst->getUsed(); i++)
        {
            const ST_CONST_REF* pRef = pRefCst->getAddrOf(i);
            pdm->edpToStrBuffer(&pRef->edpOfRef, szEDPNoFile, FC_ARRAY_LEN(szEDPNoFile), true);
            callBackCtx.record.load(pRef->pConstEntry->pszId)<<_T(";READ;")<<szEDPNoFile<<_T("\n");
            
            //if hash use it else dmp now:
            if(callBackCtx.pRefHash)
                callBackCtx.pRefHash->insertEx(callBackCtx.record, 0, false, NULL);
            else
                dmp<<callBackCtx.record;
        }
    }
    //delete this info:
    pSymPrv->freeCONSTRefList();


    //body refs using enum expr callback:
    if(pBody)
    {
        int                    i;
        int                    iRet;
        int                    nStmts;
        const CG_STStmt*const* pTAStmts;

        if(pBody->isSFC())
        {
            pTAStmts = pBody->getAllTransi(&nStmts);
            for(i=0; i<nStmts; i++)
            {
                iRet = CGT_EnumVASTExprStmt(pTAStmts[i], &callBackCtx, XrefVASTExprCallBack);
                assert(iRet==1);
            }
            pTAStmts = pBody->getAllAction(&nStmts);
            for(i=0; i<nStmts; i++)
            {
                iRet = CGT_EnumVASTExprStmt(pTAStmts[i], &callBackCtx, XrefVASTExprCallBack);
                assert(iRet==1);
            }
        }
        else
        {
            iRet = CGT_EnumVASTExprStmt(pBody->getStmts(), &callBackCtx, XrefVASTExprCallBack);
            assert(iRet==1);
        }

    }

    //what ever was stored in hash:
    if(callBackCtx.pRefHash)
    {
        const FC_CHashStr<int>::Pair* pPair;
        for(pPair=callBackCtx.pRefHash->firstPair(); pPair; pPair=pPair->pNext)
        {
            dmp<<pPair->pszKey;
        }

        delete(callBackCtx.pRefHash);
    }

    dmp<<_T("\n");
}










//[INFO]
//VERSION=1	             // The version, if changes are made, increment the number
//SCOPE=POUName          // defines the scope, e.g. PC1.GVL or PC2.PRG1
//DATE= 965128227        // Date of creation in universal time format
//LANG=LADDER|FBD|ST|... // the language of the item 
//LOCATION=file of edipos//where to locat the source maybe also a lib ref

//[DEFINITION]
//VARIABLENAME;VARIABLE_SCOPE;VARIABLE_TYPE;EDITOR_POSITION
//...
//VARIABLE_SCOPE=VAR,VAR_INPUT,VAR_OUTPUT,VAR_CONSTANT,VAR_GLOBAL,VAR_RETAIN,...
//VARIABLE_TYPE=IEC_STANDARD_TYPE,USER TYPES

//[DECLARATION]
//VARIABLENAME;VARIABLE_SCOPE;VARIABLE_TYPE;EDITOR_POSITION
//VARIABLE_SCOPE=VAR_EXTERNAL,VAR_EXTERNAL_CONSTANT,...
static void DumpXREFDeclSections(
    CGT_CDiagMsg*    pdm, 
    CGT_CDmp&        dmp, 
    ST_CSymtabPrv*   pSymPrv
)
{
    const CG_Symtab*                pSym;
    const CG_SymEntry*              pEntry;
    int                             nCrefs;
    const FC_CGArray<ST_CONST_REF>* pCrefs;


    pSym = pSymPrv->getCGSym();
    
    dmp<<_T("[INFO]\n")
         _T("VERSION=1\n");

    //scope is [libname\][GVL|CONST|<IEC POUname>]
    dmp<<_T("SCOPE=");
    if(pSym->pouTyp == CG_POU_GVL)
        dmp<<_T("GVL\n");
    else if(pSym->pouTyp == CG_POU_CST)
        dmp<<_T("CONST\n");
    else
        dmp<<pSym->pszId<<_T("\n");
    

    dmp<<_T("DATE=")<<pSym->ftLastWrite<<_T("\n");

    if(pSym->srcTyp==CG_SRC_TYPE_ST)
        dmp<<_T("LANG=ST\n");
    else if(pSym->flags & CG_STAB_BDY_IS_SFC)
        dmp<<_T("LANG=SFC\n");
    else if(pSym->flags & CG_STAB_BDY_IS_LD)
        dmp<<_T("LANG=LD\n");
    else if(pSym->flags & CG_STAB_BDY_IS_FBD)
        dmp<<_T("LANG=FBD\n");
    else if(pSym->flags & CG_STAB_BDY_IS_IL)
        dmp<<_T("LANG=IL\n");
    //else no lang may be hdr file...

    dmp<<_T("LOCATION=")<<pSym->edp.pszFile<<_T("\n\n");
    

    //[DEFINITION]
    //VARIABLENAME;VARIABLE_SCOPE;VARIABLE_TYPE;EDITOR_POSITION
    dmp<<_T("[DEFINITION]\n");
    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        //no externals and no prog instances:
        if(pEntry->declScope!=CG_VAR_EXTERNAL && pEntry->pType->type!=CG_PROGRAM)
            ST_DumpXRefDeclRecord(pdm, dmp, pEntry, NULL);
    }
    dmp<<_T("\n");


    //[DECLARATION]
    //VARIABLENAME;VARIABLE_SCOPE;VARIABLE_TYPE;EDITOR_POSITION
    dmp<<_T("[DECLARATION]\n");
    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(pEntry->declScope==CG_VAR_EXTERNAL)
            ST_DumpXRefDeclRecord(pdm, dmp, pEntry, NULL);
    }


    //assert: must have been set (even if empty list) at this state:
    assert(pSym->pzRefGlbCst);


    pCrefs = pSymPrv->getCONSTRefList();
    nCrefs = pCrefs ? pCrefs->getUsed() : 0;
    if(nCrefs>0)
    {
        int                                  i;
        FC_CHashPtr<const CG_SymEntry*, int> cdecls(nCrefs);

        for(i=0; i<nCrefs; i++)
        {
            const ST_CONST_REF& cref = pCrefs->get(i);
            if(cdecls.insert(cref.pConstEntry, 0))
            {
                ST_DumpXRefDeclRecord(pdm, dmp, cref.pConstEntry, &cref.edpOfRef);
            }
        }
    }


    dmp<<_T("\n");
}





static bool XREFDump(
    CG_CIECFront*        pFront,
    const TCHAR*         pszXREFFile,
    ST_CSymtabPrv*       pSymPrv,
    const ST_CBody*      pBody
)
{
    //ok, this is the best time to do the xref:
    const CG_Symtab*  pSym;
    CGT_CDmp          dmpXREF;
    ST_CSymtabPrv*    pSymPrvTmp;


    pSym = pSymPrv->getCGSym();


    pSymPrvTmp = NULL;
    if(pSym->pLibCtx && pSym->pouTyp!=CG_POU_CST)
    {
        FC_CString        jot(MAX_PATH);
        //get target indepenent interface header:

        jot.load(pSym->pLibCtx->pszFile)<<_T("::")<<pSym->pszId<<_T("\\")<<pSym->pszId;
        if(pSym->pouTyp==CG_POU_SDT)
            jot<<_T(".sdt");
        else
            jot<<_T(".hdr");

        pSymPrvTmp = ST_ParseAnyHeader(&pSym->pLibCtx->edp, pFront, jot, pSym->pLibCtx);
        //why should this fail, maybe file IO error?
        if(!pSymPrvTmp)
        {
            assert(pFront->m_pdm->hasErr());
            return false;
        }

        pSymPrv = pSymPrvTmp;
        pSym = pSymPrv->getCGSym();
    }


    DumpXREFDeclSections(pFront->m_pdm, dmpXREF, pSymPrv);
    DumpXREFReferenceSection(pFront->m_pdm, dmpXREF, pSymPrv, pBody);
        

    
    if(CGT_DmpToFile(pFront->m_pdm, NULL, pszXREFFile, dmpXREF, false))
    {
        //patch file time:
        HANDLE hFile  = CreateFile(pszXREFFile,GENERIC_WRITE,FILE_SHARE_READ,NULL,
                                   OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

        assert(hFile != INVALID_HANDLE_VALUE);
        if(hFile != INVALID_HANDLE_VALUE)
        {
            LARGE_INTEGER  lint;
            FILETIME       ftime;

            lint.QuadPart = pSym->ftLastWrite;
            ftime.dwLowDateTime = lint.LowPart;
            ftime.dwHighDateTime = lint.HighPart;
        
            BOOL bOk = SetFileTime(hFile, NULL, NULL, &ftime);
            assert(bOk);
            CloseHandle(hFile);
        }
    }

    if(pSymPrvTmp)
        delete(pSymPrvTmp);

    return true;
}



//entry function to this file:
bool ST_ConsiderXREFDump(
    CG_CIECFront*   pFront,
    ST_CSymtabPrv*  pSymPrv,
    const ST_CBody* pBody
)
{
    const CG_Symtab*           pSym;
    long                       flags;

    pSym = pSymPrv->getCGSym();
    flags= pSym->flags;

    if(pSym->pszXREFFile)
    {
       //can happen if reload body, for some reason we already dumped the XREF.
       assert((flags & CG_STAB_HDR_NOERR) && (!(flags & CG_STAB_HAS_BDY) || (flags & CG_STAB_BDY_LOADED)));
       return false;
    }
    
    //do some flag logic to get the right time of XREF dump:
    if((flags & CG_STAB_HDR_NOERR)==0)
        return false; //hdr in error, forget leave possible old xref

    if(!pSym->pLibCtx)
    {
        if(flags & CG_STAB_HAS_BDY)
        {
            if((flags & CG_STAB_BDY_LOADED)==0)
                return false; //wait until bdy is loaded
            if((flags & CG_STAB_BDY_NOERR)==0)
                return false; //bdy in error, forget leave possible old xref
            assert(pBody);
            if(!pBody)
                return false;
        }
    }


    FC_CString jot(MAX_PATH);
    if(pSym->pLibCtx && pSym->pouTyp==CG_POU_CST)
    {
        jot<<pSym->pLibCtx->pszId;
        jot.changeFileExt(NULL);
        jot.addc(_T('@'))<<FC_StringGetFilePart(pSym->edp.pszFile);
        jot<<_T(".")CG_XREF_POU_EXT;
    }
    else
    {
        jot<<FC_StringGetFilePart(pSym->edp.pszFile)<<_T(".")CG_XREF_POU_EXT;
    }

    //set the xref file name attribute:
    assert(!pSym->pszXREFFile);
    pSymPrv->registerXREFFile(jot);

    //make full path:
    jot.prepend(pFront->m_4CP.getGenPath(), _T("\\")CG_XREF_GEN_DIR _T("\\"));
    
    //get last mod file time:
    HANDLE hFile  = CreateFile(jot, GENERIC_READ,FILE_SHARE_READ,NULL,
                               OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER  lint;
        FILETIME       ftime;

        if(GetFileTime(hFile, NULL, NULL, &ftime))
        {
            lint.LowPart  = ftime.dwLowDateTime;
            lint.HighPart = ftime.dwHighDateTime;
        
            if(lint.QuadPart == pSym->ftLastWrite)
            {
                CloseHandle(hFile);
                return false;
            }
        }
        CloseHandle(hFile);
    }

    return XREFDump(pFront, jot, pSymPrv, pBody);
}
 
















//define typesafe handle as class:
class ACCESS_TREE
{
public:
    FC_CHashPtr<const CG_SymEntry*, ACCESS_TREE*>* m_pTree;
    ACCESS_TREE()
    {
        m_pTree = new FC_CHashPtr<const CG_SymEntry*, ACCESS_TREE*>;
    }
    ~ACCESS_TREE()
    {
        m_pTree->deletePtrs();
        delete(m_pTree);
    }
};


extern "C" int SMAVASTExprCallBack(
    void*               pCtx,
    const CG_Edipos*    pEdp,
    WORD                accFlags,
    const CG_STStmt*    pFromStmt,
    const CG_ExprTree*  pFromExpr,
    int                 nPath,
    const CGT_PathElem* pPath,
    const CG_SymEntry*  pArgEntry
)
{
    int                i;
    ACCESS_TREE*       pTree;
    ACCESS_TREE*       pTree2;
    const CG_SymEntry* pEntry;
    ACCESS_TREE**      ppCtxTree  = ((ACCESS_TREE**)pCtx);

    //pPath is a.b.c or a[i+1] or a[i].a.b.c  ...

    //ignore all tmp var stuff for sma access:
    if(pFromExpr && pFromExpr->extyp==CG_TMPVAR)
        return 1;

    //only for tmp var access the nPath can be 0:
    assert(nPath>0);

#ifdef DEBUG
    bool bAccFOk;
    //check accFlags 
    if((pFromStmt->styp==CG_CALLFB && pFromStmt->fbcs.pInst==pFromExpr))
    {
        //if FBI call the only possible flags are (up to now):
        bAccFOk = (accFlags & ~(CG_ACCESS_r|CG_ACCESS_w|CG_ACCESS_C))==0;
    }
    else
    {
        //all other expr must not contain any partially bit and no B:
        bAccFOk = (accFlags & (CG_ACCESS_w|CG_ACCESS_o|CG_ACCESS_x|CG_ACCESS_a|CG_ACCESS_c|CG_ACCESS_B))==0;
    }
    if(!bAccFOk && !(pArgEntry && pArgEntry->pType->type==CG_ANY_WAITABLE))//##AC HACK consider to remove the DEBUG code now...
    {
        TCHAR szBuffer[32+2];
        szBuffer[0] = _T(':');
        szBuffer[1] = _T(' ');
        CGT_ExprAccFlagsToString(accFlags, szBuffer+2);
        CGT_DumpEntryPath(NULL, _T("SMAVASTExprCallBack: bad accFlags: "), nPath, pPath,szBuffer);
    }
#endif

    if(nPath<=0 || !pPath[0].pEntry->pType->pSym)
        return 1; //is no Struct/FB entry
    if((accFlags & (CG_ACCESS_W|CG_ACCESS_O|CG_ACCESS_X))==0)
        return 1; //this access is no full write, ignore...
    if(pPath[0].pEntry->accFlags & (CG_ACCESS_W|CG_ACCESS_X|CG_ACCESS_O))
        return 1; //entry fully written or passed as inout, fun output...

    //any possible new info:
    pTree = *ppCtxTree;
    assert(pTree);

    
    for(i=0;i<nPath; pTree = pTree2)
    {
        pEntry = pPath[i].pEntry;

        //skip all arrayness info for sma purpose:
        i++;
        if(pEntry->pType->bIsArray)
            i++;
        

        if(pTree->m_pTree->get(pEntry, &pTree2))
        {
            if(!pTree2)
            {
                return 1;//member found but no sub tree => full written or simple type:
            }
            //else assert: this must be a symtab type:
            assert(pEntry->pType->pSym);

            if(i>=nPath) 
            {  
                //i>=nPath: this is the last part of the access path:
                //existing x.y.z is cut at x.y because x.y was fully written
                pTree->m_pTree->insert(pEntry, NULL);
                delete(pTree2);
                return 1;
            }
        }
        else
        {
            //new information
            if(i<nPath)
            {
                //this is not the end of this(!) write path, assert that type is 
                //complex:
                assert(pEntry->pType->pSym);
                //insert new but empty member list:
                pTree2 = new ACCESS_TREE();
            }
            else
            {
                //end of this(!) write path, if type is simpl or complex (both possible)
                //always insert a NULL to indicate full write access
                pTree2 = NULL;
            }
            pTree->m_pTree->insert(pEntry, pTree2);
        }
    }

    return 1;
}




//entry function:
void FreeSMAHash(ACCESS_TREE* pSMA)
{
    delete(pSMA);
}
//entry function:
ACCESS_TREE* CreateSMAHash(
    const ST_CBody* pBody
)
{
    int                    i;
    int                    iRet;
    int                    nStmts;
    const CG_STStmt*const* pTAStmts;
    ACCESS_TREE*           pRefTree;


    pRefTree = new ACCESS_TREE;

    //get body refs using enum expr callback:
    if(pBody->isSFC())
    {
        pTAStmts = pBody->getAllTransi(&nStmts);
        for(i=0; i<nStmts; i++)
        {
            iRet = CGT_EnumVASTExprStmt(pTAStmts[i], &pRefTree, SMAVASTExprCallBack);
            assert(iRet==1);
        }
        pTAStmts = pBody->getAllAction(&nStmts);
        for(i=0; i<nStmts; i++)
        {
            iRet = CGT_EnumVASTExprStmt(pTAStmts[i], &pRefTree, SMAVASTExprCallBack);
            assert(iRet==1);
        }
    }
    else
    {
        iRet = CGT_EnumVASTExprStmt(pBody->getStmts(), &pRefTree, SMAVASTExprCallBack);
        assert(iRet==1);
    }
    
    return pRefTree;
}

bool GetsSMAWritten(
    ACCESS_TREE*             pSMA, 
    long                     nPath, 
    const CG_SymEntry*const* pPath/*[nPath]*/
)
{
    int                i;
    bool               bFullWrt;
    ACCESS_TREE*       pTree;
    const CG_SymEntry* pEntry;


    bFullWrt = (pPath[0]->accFlags & (CG_ACCESS_W|CG_ACCESS_X|CG_ACCESS_O))!=0;
    
    if(bFullWrt)
        return true; //if first entry was full written all other (if nPath>1) are too
    else if(nPath==1)
        return false;//if not and nPath==1 it is not written, don't querry pTree
                     //such simple cases are not stored there.
    
    pTree = pSMA;
    for(i=0; i<nPath; i++)
    {
        pEntry = pPath[i];
        assert(i==0 || pEntry->pParent == pPath[i-1]->pType->pSym);
        
        if(!pTree->m_pTree->get(pEntry, &pTree))
            return false;
        if(!pTree)
            return true;
    }
    return true;
}


//entry function:
void SMATreeDump(ACCESS_TREE* pTree, FILE* pFile)
{
    const FC_CHashPtr<const CG_SymEntry*, ACCESS_TREE*>::Pair* pPair;

    for(pPair = pTree->m_pTree->firstPair(); pPair; pPair = pPair->pNext)
    {
        _fputts(pPair->pKey->pszId, pFile);
        if(pPair->pVal)
        {
            _fputts(_T("."), pFile);
            SMATreeDump(pPair->pVal, pFile);
        }
        else
        {
            _fputts(_T("\n"), pFile);
        }
    }
}

