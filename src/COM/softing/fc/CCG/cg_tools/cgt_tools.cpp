#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_hash.h"
#include "cgt_tools.h"
#include "cgt_misc.h"







static bool RecurseSym(
    CGT_CDiagMsg*                       pdm,
    ICG_IECFront*                       pIF,
    const CG_Symtab*                    pSymCall, 
    FC_CHashPtr<const CG_Symtab*, int>* pSymHash,
    FC_CGArray<const CG_Symtab*>*       pCheckRec,
    bool                                bImports
)
{
    int                    i;
    const CG_Symtab*const* ppSym;
    

    //CAUTION: avoid endless recursion:
    i = pCheckRec->findElem(pSymCall);
    if(i>=0)
    {
        pdm->msg1(CG_E_RECURSIV, NULL, pSymCall->pszId);
        pdm->userMsg(_T("    calling sequence: "));

        for( ; i<pCheckRec->getUsed(); i++)
        {
            pdm->userMsg(pCheckRec->get(i)->pszId);
            pdm->userMsg(_T("->"));
        }
        pdm->userMsg(pSymCall->pszId);
        pdm->userMsg(_T("\n"));
        
        return false;
    }

    if((pSymCall->flags & CG_STAB_HDR_NOERR)==0)
        return false;

    if(!pSymCall->pzRefSyms)
    {
        assert(!"body not loaded?");
        return false;
    }

    pCheckRec->push(pSymCall);
    
    for(ppSym = pSymCall->pzRefSyms; *ppSym; ppSym++)
    {
        if(!pSymHash->getPair(*ppSym))
        {
            if(!RecurseSym(pdm, pIF, *ppSym, pSymHash, pCheckRec, false))
            {
                pCheckRec->pop(pSymCall);
                return false;
            
            }
            pSymHash->insert(*ppSym, 0);
        }
    }

    if(bImports)
    {
        for(i = 0; i<pSymCall->nGVLImp; i++)
        {
            if(!RecurseSym(pdm, pIF, pSymCall->pGVLImp[i].pSym, pSymHash, pCheckRec, true))
            {
                pCheckRec->pop(pSymCall);
                return false;
            }
        }
    }

    pCheckRec->pop(pSymCall);
    return true;
}

extern "C" const CG_Symtab** CGT_EnumRefSymtabs(
    CGT_CDiagMsg*      pdm,
    ICG_IECFront*      pIF,
    const CG_Resource* pRes
)
{
    FC_CHashPtr<const CG_Symtab*, int> symHash(1024);
    FC_CGArray<const CG_Symtab*>       checkRec(128); //used to find recusive fun calls

    
    if(!pRes->bNoError)
        return NULL;

    if(RecurseSym(pdm, pIF, pRes->pGVL, &symHash, &checkRec, true))
        return symHash.allocKeyArray(NULL);
    else    
        return NULL;
   
}


extern "C" const CG_Symtab** CGT_EnumRefSymtabsOfTask(
    CGT_CDiagMsg*      pdm,
    ICG_IECFront*      pIF,
    const CG_Task*     pTask
)
{
    int                                i;
    const CG_Symtab*                   pSym;
    FC_CHashPtr<const CG_Symtab*, int> symHash(1024);
    FC_CGArray<const CG_Symtab*>       checkRec(128); //used to find recusive fun calls

    for(i=0; i<pTask->nPrgs; i++)
    {
        pSym = pTask->pPrgs[i]->pType->pSym; //TODO the bImports last param is a bad idea
                                             //much better is to handle the #import gvls like any other POU 
                                             //especially in the recursion detection and the only
                                             //difference is that a gvl symtab in not put into the result symHash
        if(symHash.getPair(pSym))            //TODO there should only be one symHash.insert inside RecurseSym
            continue;

        if(!RecurseSym(pdm, pIF, pSym, &symHash, &checkRec, false))
            return NULL;

        symHash.insert(pSym, 0);
    }
   
    return symHash.allocKeyArray(NULL);
}





static void BuildInstanceTreeRec(
    const CG_Symtab* pSym, 
    CGT_CInstPath&   ipath,
    FC_CStrList*     pInstPathes, 
    FC_CString&      helpStr
)
{
    int                 i;
    const CG_SymEntry*  pEntry;
    const CG_Symtab*    pSymInst; 

    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        pSymInst = pEntry->pType->pSym;
        if(!pSymInst)
            continue;
        if(pEntry->pType->bIsArray || pSymInst->pouTyp==CG_POU_SDT)
            continue; //TODO array of FB and Struct ???

        switch(pEntry->declScope)
        {
        case CG_VAR:
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT:
        case CG_VAR_GLOBAL:   
        break;
        default:
            continue;
        }
            
        
        ipath.append(pEntry->pszId);
        helpStr.load(pSymInst->pszId).addc(_T(' '))<<ipath;
        pInstPathes->add(helpStr);

        BuildInstanceTreeRec(pSymInst, ipath, pInstPathes, helpStr);
        ipath.stripLast();
    }

    if(pSym->pouTyp==CG_POU_GVL)
    {
        for(i=0; i<pSym->nGVLImp; i++)
            BuildInstanceTreeRec(pSym->pGVLImp[i].pSym, ipath, pInstPathes, helpStr);
    }

}





static bool DumpRetainListRec(
    CGT_CDmp&        dmp,
    long             flags,
    const CG_Symtab* pSym, 
    bool             bOuterRetain,
    CGT_CInstPath&   ipath 
)
{
    const TCHAR*       pszType;
    const TCHAR*       pszPath;
    const CG_SymEntry* pEntry;
    bool               bRetainDecl;
    bool               bInstPathDone;



    bInstPathDone = false;
    for(pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        if(pEntry->pType->pSym)
            continue;

        bRetainDecl = 
            (pEntry->declMod & CG_DMOD_RETAIN)!=0 || 
            (bOuterRetain && (pEntry->declMod & CG_DMOD_NON_RETAIN)==0);

        if(!bRetainDecl)
            continue;

        if(!bInstPathDone)
        {
            bInstPathDone = true;

            if(ipath.getDepth()>0)
                pszPath = ipath;
            else
                pszPath = NULL;

            if((flags & CGT_DRL_FULL_PATH)==0 && pszPath)
            {
                dmp<<pszPath<<'\n';
                pszPath = _T("");
            }
        }


        pszType = NULL;
        if(flags & CGT_DRL_DATA_TYPE_FULL)
        {
            CG_DTYPES type = pEntry->pType->type;
            if(type==CG_BOOL_R_EDGE || type==CG_BOOL_F_EDGE)
                type = CG_BOOL;
            pszType = CGT_DTYPES_ToLString(type);
        }

        if(pEntry->pType->bIsArray)
        {
            if(flags & CGT_DRL_SIMPL_ARRAY_EXPAND)
            {
                for(long kk=0; kk<pEntry->pType->nElems; kk++)
                {
                    if(pszPath)
                        dmp<<pszPath<<'.';
                    
                    dmp<<pEntry->pszId<<'.'<<kk;
                    
                    if(pszType)
                        dmp<<';'<<pszType;
                    dmp<<'\n';
                }
            }
            else
            {
                if(pszPath)
                    dmp<<pszPath<<'.';

                dmp<<pEntry->pszId;

                if(pszType)
                    dmp<<';'<<pszType<<'['<<pEntry->pType->nElems<<']';
                dmp<<'\n';
            }
        }
        else
        {
            if(pszPath)
                dmp<<pszPath<<'.';

            dmp<<pEntry->pszId;
            
            if(pszType)
                dmp<<';'<<pszType;
            dmp<<'\n';
        }
    }


    for(pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        if(!pEntry->pType->pSym)
            continue;
        if(pEntry->pType->pSym->pouTyp == CG_POU_SDT)
        {
            bRetainDecl = 
                (pEntry->declMod & CG_DMOD_RETAIN)!=0 || 
                (bOuterRetain && (pEntry->declMod & CG_DMOD_NON_RETAIN)==0);
        }
        else
        {
            bRetainDecl = (pEntry->declMod & CG_DMOD_RETAIN)!=0;
        }


        ipath.append(pEntry->pszId);
        if(pEntry->pType->bIsArray)
        {
            for(long kk=0; kk<pEntry->pType->nElems; kk++)
            {
                ipath.appendInt(kk);
                if(!DumpRetainListRec(dmp, flags, pEntry->pType->pSym, bRetainDecl, ipath))
                    return false;
                ipath.stripLast();
            }
        }
        else
        {
            if(!DumpRetainListRec(dmp, flags, pEntry->pType->pSym, bRetainDecl, ipath))
                return false;
        }
        ipath.stripLast();
    }

    return true;
}





extern "C" bool CGT_DumpRetainList(
    CGT_CDiagMsg*    pdm,
    CGT_CDmp&        dmp,
    const CG_Symtab* pSym, 
    long             flags 
)
{
    int             i;
    bool            bOk;
    CGT_CInstPath   ipath;
   
    bOk = true;
    if(pSym->pouTyp==CG_POU_GVL)
    {
        for(i=0; bOk && i<pSym->nGVLImp; i++)
            bOk = DumpRetainListRec(dmp, flags, pSym->pGVLImp[i].pSym, false, ipath);
    }

    bOk = bOk && DumpRetainListRec(dmp, flags, pSym, false, ipath);
    assert(ipath.getDepth()==0 || !bOk);
    
    return bOk;
}





extern "C" void CGT_BuildInstanceTree(
    const CG_Symtab* pSym, 
    FC_CStrList*     pInstPathes
)
{
    CGT_CInstPath   ipath;
    FC_CString      helpStr(5*MAX_PATH);

    BuildInstanceTreeRec(pSym, ipath, pInstPathes, helpStr);
}


extern "C" bool CGT_DumpXREFFileList(
    CGT_CDiagMsg*          pdm,
    ICG_IECFront*          pIF,
    const CG_Resource*     pRes,
    const CG_Symtab*const* pRefSym
)
{
    int                        i;
    int                        nCsts;
    FC_CString                 xlistFile(MAX_PATH);
    const CG_Symtab*           pSym;
    const CG_Symtab*const*     ppSym;
    CGT_CDmp                   dmp;

   
    if(!pRes || !pRes->bNoError || !pRefSym)
    {
        assert(pdm->getDisplayedErrors()>0);
        return false;
    }


    //all cst files of the project and all libs:
    nCsts = pIF->getNumPrjCST();
    for(i=0; i<nCsts; i++)
    {
        pSym = pIF->getPrjCSTbyNum(i);
        assert(pSym && pSym->pszXREFFile);
        if(pSym && pSym->pszXREFFile)
            dmp<<pSym->pszXREFFile<<_T("\n");
    }

    //all gvls of resource:
    pSym = pRes->pGVL;
    assert(pSym->pszXREFFile);
    if(pSym->pszXREFFile)
        dmp<<pSym->pszXREFFile<<_T("\n");

    for(i=0; i<pSym->nGVLImp; i++)
    {
        assert(pSym->pGVLImp[i].pSym->pszXREFFile);
        if(pSym->pGVLImp[i].pSym->pszXREFFile)
            dmp<<pSym->pGVLImp[i].pSym->pszXREFFile<<_T("\n");
    }

    //all called POUs:
    for(ppSym=pRefSym; *ppSym; ppSym++)
    {
        //assert:  ref syms with body: loadBody must have been called to set the pszXREFFile??
        assert((*ppSym)->pszXREFFile); 
        if((*ppSym)->pszXREFFile)
            dmp<<(*ppSym)->pszXREFFile<<_T("\n");
    }


    xlistFile<<pIF->FCPgetGenPath()<<_T("\\")CG_XREF_GEN_DIR _T("\\");
    xlistFile<<FC_StringGetFilePart(pRes->edp.pszFile)<<_T(".")CG_XREF_LST_EXT;

    return CGT_DmpToFile(pdm, NULL, xlistFile, dmp, false);
}


bool CGT_ConsiderBP(const CG_Edipos* pLast, const CG_Edipos* pCurr)
{
    //select source location where BP wanted at all, no lines,signals:
    switch(pCurr->typ)
    {
    case CG_EDP_ST:          
    case CG_EDP_4GR_BOX:
    case CG_EDP_4GR_STBOX:   break;
    case CG_EDP_4GR_SIGNAL:  return false;
    case CG_EDP_4GR_REGION:  break;
    case CG_EDP_4GR_LINE:    return false; 
    case CG_EDP_4GR_END:     
    case CG_EDP_4GR_BEG:     break;
    default:
        assert(!"bad edipos typ");
    }

   
    if(!pLast || pLast->typ != pCurr->typ)
        return true;

    if(pLast->typ==CG_EDP_ST)
    {
        if(pLast->lc.l == pCurr->lc.l)
            return false; //don't set a BP on same line number
    }
    else if(pLast->typ==CG_EDP_4GR_STBOX)
    {
        if(pLast->gr.x==pCurr->gr.x && pLast->gr.y==pCurr->gr.y && pLast->gr.l==pCurr->gr.l)
            return false; //don't set a BP on same line number in same ST-Action box
    }
    else if(pLast->typ==CG_EDP_4GR_BOX)
    {
        if(pLast->gr.x==pCurr->gr.x && pLast->gr.y==pCurr->gr.y)
            return false; //don't set a BP on same 4GB box element
    }
    
    return true;
}


const CG_SymEntry* CGT_GetExprSymEntry(const CG_ExprTree* pExpr)
{
    const CG_ExprTree* pE = pExpr;

    for(;;)
    {
        switch(pE->extyp)
        {
        case CG_VARREF:    
            return pE->var.pEntry;
        case CG_STRUCTREF: 
            pE = pE->str.pBase;
        break;
        case CG_ARRAYREF:  
            pE = pE->arr.pBase;
        break;
        case CG_NOOP:  
            pE = pE->ops.pLhs;
        break;
        default:
            return NULL;
        }
    }
}


extern "C" bool CGT_DumpSecurityFile(
    CGT_CDiagMsg*      pdm,   
    ICG_IECFront*      pIF   
)
{
    int                  i;
    CGT_CDmp dmp;
    const CG_AttribList* al       = pIF->FCPgetSection(CG_SEC_4CP_ATTRIBUTES);
    const TCHAR*         pszRead  = NULL;
    const TCHAR*         pszWrite = NULL;
    const CG_KeyValPair* pair;

    if(al)
    {
        for (i = 0; i < al->nAttribs; i++)
        {
            pair = &al->pairs[i];
            if (!_tcsicmp(pair->pszKey, _T("password.read")))
                pszRead = pair->pszVal;
            if (!_tcsicmp(pair->pszKey, _T("password.write")))
                pszWrite = pair->pszVal;
        }
    }//else: attr section is opt., if not present use defaults ""

    dmp << CGT_XML_STANDARD_HDR _T("\n");
    dmp << _T("    <USERLIST>\n");

    dmp<<_T("        <USER PASSWORD=");
    dmp.xmlAttrVal(pszWrite?pszWrite : _T(""));
    dmp<<_T(" RIGHTS=\"rw\" />\n");

    dmp << _T("        <USER PASSWORD=");
    dmp.xmlAttrVal(pszRead?pszRead : _T(""));
    dmp<< _T(" RIGHTS=\"r\" />\n");

    dmp<<_T("    </USERLIST>\n");
  

    if (!CGT_DmpToFile(pdm, pIF->FCPgetGenPath(), CGT_SECURTY_FILE_NAME, dmp, false))
        return false;

    return true;
}




