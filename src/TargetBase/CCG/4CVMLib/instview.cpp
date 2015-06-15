/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/instview.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: instview.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/instview.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "cgt_tools.h"
#include "cgt_misc.h"


//local helper:
static void InstViewEntryRec(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //used to ?
    CGT_CInstPath      ipath, 
    CGT_CDmp&          dmp,
    const CG_SymEntry* pSym,     //Prg,FB 
    const CG_Resource* pRes     //called resource
);

static void InstViewSymtabRec(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //used to ?, currently not used..
    CGT_CInstPath      ipath, 
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    const CG_Resource* pRes     //calling res.
);
static void ProcessUserCmd(
    CGT_CDiagMsg*        pdm,     //for err msg
    ICG_IECFront*        pIF,     
    const CG_AttribList* pAttr,
    const CGT_CInstPath& ipath, 
    CGT_CDmp&            dmp,
    const CG_Symtab*     pSym,   //Prg,FB 
    const CG_Resource*   pRes   //calling res.
);                           







extern "C" long DumpInstView(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //front end IF
    const TCHAR*       pszTrgTyp, //target type
    const TCHAR*       pszTrgId, //PC1
    const TCHAR*       pszTrgAddr, //w-ef
    const CG_Resource* pRes     //called resource
)
{
    int           i;
    int           j;
    long          nErrBeg;
    CGT_CInstPath ipath;
    CGT_CInstPath ipathState; //HACK must use {w-ef} for STATE_VAR but PC1 for INSTANCE_PATH
    CGT_CDmp      dmp;
    FC_CString    jot(MAX_PATH);
    
        
    if(!pRes->bNoError)
        return -1;

    nErrBeg = pdm->getTotalErrors();

    dmp<<CGT_XML_STANDARD_HDR _T("\n\n");

    //want to dump:
    //<NODE TEXT             = "Res1"
    //      ID               = "Res1"
    //      TYPE             = "RESOURCE_INST"
    //      ASSIGNMENT       = "PC1"
    //      INSTANCE_PATH    = "PC1.Res1"
    //      SOURCE           = "Res1.res"
    //      LOCATION         = "(2,10)"
    //      STATE_VAR        = "{W-EF}.Res1.Task1.__state"
    //>
    ipath.append(pszTrgId);
    ipath.append(pRes->pszId);
    ipathState.appendTargetAddr(pszTrgAddr);
    ipathState.append(pRes->pszId);
    dmp<<  _T("<NODE TEXT             = "); dmp.xmlAttrVal(pRes->pszId);
    dmp<<_T("\n      ID               = "); dmp.xmlAttrVal(pRes->pszId);
    dmp<<_T("\n      TYPE             = \"RESOURCE_INST.");
    dmp.up(pszTrgTyp)<<_T("\"");
    dmp<<_T("\n      ASSIGNMENT       = \"")<<pszTrgId<<_T('"');
    dmp<<_T("\n      INSTANCE_PATH    = "); dmp.xmlAttrVal(ipath);
    dmp<<_T("\n      SOURCE           = "); dmp.xmlAttrVal(pRes->edp.pszFile);
    dmp<<_T("\n      LOCATION         = "); dmp.xmlAttrVal(pdm->edpToStrNoFile(&pRes->edp, jot));
    dmp<<_T("\n      STATE_VAR        = \""); dmp.dumpAsXMLUTF8(ipathState)<<_T(".__state\"");

    if(pRes->pAttribs)
        ProcessUserCmd(pdm, pIF, pRes->pAttribs, ipath, dmp, NULL, pRes);

    dmp<<_T("\n>\n\n");

    dmp.incrIndent();        


    //want to dump:
    //<NODE TEXT  ="Resource1.gvl"
    //      ID    ="Resource1.gvl"
    //      TYPE  ="GLOBAL_VAR_FILE_INST"
    //      SOURCE="Resource1.gvl"
    //>
    for(j=0; j<pRes->pGVL->nGVLImp; j++)
    { 
        const CG_Symtab* pGVLImp = pRes->pGVL->pGVLImp[j].pSym;

        dmp<<  _T("<NODE TEXT  ="); dmp.xmlAttrVal(pGVLImp->edp.pszFile);
        dmp<<_T("\n      ID    ="); dmp.xmlAttrVal(pGVLImp->edp.pszFile);
        dmp<<_T("\n      TYPE  =\"GLOBAL_VAR_FILE_INST\"");
        dmp<<_T("\n      SOURCE="); dmp.xmlAttrVal(pGVLImp->edp.pszFile);
        dmp<<_T("\n      INSTANCE_PATH= "); dmp.xmlAttrVal(ipath);

        //dump all global FBs that are from this GVL if any:
        if(pGVLImp->flags & CG_STAB_HAS_INST)
        {
            dmp<<_T("\n>\n");
            InstViewSymtabRec(pdm, pIF, ipath, dmp, pGVLImp, pRes);
            dmp<<_T("</NODE>\n\n");
        }
        else
        {
            dmp<<_T("/>\n\n");
        }
    }
    dmp<<_T("\n");

 
    //want to dump:
    //<NODE TEXT     = "Task2"
    //      ID       = "Task2"
    //      TYPE     = "TASK_INST"
    //      SOURCE   = "Res1.res"
    //      LOCATION = "(13,10)"
    //      STATE_VAR= "{W-EF}.Res1.Task2.__state"
    //>
    for(j=0; j<pRes->nTasks; j++)
    {
        const CG_Task* pTsk = &pRes->pTasks[j];

        dmp<<  _T("<NODE TEXT     = \"")<<pTsk->pszId<<_T('"');
        dmp<<_T("\n      ID       = \"")<<pTsk->pszId<<_T('"');
        dmp<<_T("\n      TYPE     = \"TASK_INST\"");
        dmp<<_T("\n      SOURCE   = "); dmp.xmlAttrVal(pTsk->edp.pszFile);
        dmp<<_T("\n      LOCATION = "); dmp.xmlAttrVal(pdm->edpToStrNoFile(&pTsk->edp, jot));
        dmp<<_T("\n      STATE_VAR= \"");
        dmp.dumpAsXMLUTF8(ipathState, false)<<_T(".")<<pTsk->pszId<<_T(".__state\"");
        dmp<<_T("\n>\n");

        //loop all programs that are assigned to the task:
        for(i=0; i<pTsk->nPrgs; i++)
            InstViewEntryRec(pdm, pIF, ipath, dmp, pTsk->pPrgs[i], pRes);

        dmp<<_T("</NODE>\n\n");
    }

    dmp.decrIndent();        
    dmp<<_T("</NODE>\n");

    jot.load(pszTrgId)<<_T("\\")<<pszTrgId<<_T(".inst");
    CGT_DmpToFile(pdm, pIF->FCPgetGenPath(), jot, dmp, false);

    return pdm->getTotalErrors() - nErrBeg;
}

static void ProcessUserCmd(
    CGT_CDiagMsg*        pdm,     //for err msg
    ICG_IECFront*        pIF,     
    const CG_AttribList* pAttr,    
    const CGT_CInstPath& ipath, 
    CGT_CDmp&            dmp,
    const CG_Symtab*     pSym,   //Prg,FB 
    const CG_Resource*   pRes   //calling res.
)                           
{
    TCHAR               buffer[2*MAX_PATH];
    TCHAR*              pszField[4];
    int                 nUserCmd = 0;
    const CG_DType*     pType;
    int                 n;
    CG_Edipos           edptmp;
    bool                bGlobal;

    if(!pAttr)
        return;

    //usercmd= "Reset TMPC 14|iFileNumber|13"
    for(n = 0; n<pAttr->nAttribs; n++) //TODO diag msg, chsck var path and value ....
    {
        if(_tcsicmp(pAttr->pairs[n].pszKey, _T("usercmd")))
            continue;

        edptmp = pAttr->pairs[n].edpVal;
        if(!FC_StringCopyToBuffer(buffer, FC_ARRAY_LEN(buffer), pAttr->pairs[n].pszVal))
        {
            pdm->msg1(CG_E_ATTR_VALUE_TO_LONG, &edptmp, "256");
            continue;
        }
        if(FC_StringSplitBuf(buffer,'|', pszField, 4, 1) != 3)
        {
            pdm->msg1(CG_E_TEXT, &edptmp, _T("usercmd: invalid menue description format"));
            pdm->userMsg(_T("    expeted: usercmd=\"<menue text>|<var path>|<literal value>\"\n"));
            continue;
        }

        CG_IncColEdipos(&edptmp, pszField[1] - buffer + 1); //+1 for attrip string start delim
        pType = CGT_ParseAccessPath(pdm, &edptmp, pIF, pszField[1], pSym, pRes, &bGlobal);

        if(!pType)
            continue;
        if(pType->pSym || pType->bIsArray)
        {
            pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<
                _T("usercmd: '"<<pszField[1]<<"' points to non simple data type '")<<pType->pszName<<'\'');
            continue;
        }

         //if string type use pszField[2] as is, if non string parse the value 
        //using the front end: 
        FC_CString szValue(256);
        if(pType->type==CG_WSTRING || pType->type==CG_STRING)
        {
            szValue.load(pszField[2]);
        }
        else
        {
            CG_IncColEdipos(&edptmp, pszField[2] - pszField[1]);
            ICG_Expr* pValExpr = pIF->parseExprFromString(&edptmp, true, pszField[2], pSym?pSym->pszId:NULL, false);
            if(!pValExpr)
                continue;
            const CG_ExprTree* pLit = pValExpr->getExpr(pType->type);
            if(!pLit)
            {
                pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("usercmd: '"<<pszField[2]<<"' must be of datatype "<<pType->pszName));
                pValExpr->Release();
                continue;
            }
            else if(pLit->extyp != CG_LITERAL)
            {
                pdm->msg1(CG_E_TEXT, &edptmp, pdm->jot1()<<_T("usercmd: '"<<pszField[2]<<"' does not evaluate to a simple type literal"));
                pValExpr->Release();
                continue;
            }
            szValue.load(pLit->lit.pLit->psz);
            pValExpr->Release();
        }
       
        dmp<<_T("\n      USER_ONLCMD")<<nUserCmd<<_T("=\"");
        dmp.dumpAsXMLUTF8(pszField[0], false)<<'|';
        
        if(bGlobal)
        {
            //add pc1.Resource1
            for(const TCHAR* psz=ipath; *psz && *psz!='.'; psz++)
                dmp<<*psz;
            dmp<<'.';
            for(psz++; *psz && *psz!='.'; psz++)
                dmp<<*psz;

            dmp<<'.'<<pszField[1];
        }
        else
        { 
            //start with FB instance path:
            dmp<<ipath<<'.'<<pszField[1];
        }

        dmp<<'|'<<pType->pszName<<'|';
        dmp.dumpAsXMLUTF8(szValue, false)<<'"';    

        nUserCmd++;
    }
}





static void InstViewEntryRec(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //used to ?, currently not used..
    CGT_CInstPath      ipath, 
    CGT_CDmp&          dmp,
    const CG_SymEntry* pEntry,  //Prg,FB 
    const CG_Resource* pRes     //called resource
)
{
    const CG_Symtab*  pSym;

        
    pSym = pEntry->pType->pSym;
    assert(pSym);
    if(!pSym)
        return;
    
    ipath.append(pEntry->pszId);
    dmp.incrIndent();



    dmp<<  _T("<NODE TEXT         = \"")<<pEntry->pszId<<_T(" (")<<pSym->pszId<<_T(")\"");
    dmp<<_T("\n      ID           = \"")<<pEntry->pszId<<_T('"');
    dmp<<_T("\n      INSTANCE_PATH= "); dmp.xmlAttrVal(ipath);
    dmp<<_T("\n      TYPE         = \"")<<CGT_PouTypeName(pSym->pouTyp, false)<<_T("_INST\"");
    dmp<<_T("\n      DATA_TYPE    = \"")<<pSym->pszId<<_T("\"");
    dmp<<_T("\n      SOURCE       = "); dmp.xmlAttrVal(pSym->edp.pszFile);
    dmp<<_T("\n      LOCATION     = "); dmp.xmlAttrVal(pdm->edpToStrNoFile(&pSym->edp, pdm->jot1()));


    if(pSym->pAttribs)
        ProcessUserCmd(pdm, pIF, pSym->pAttribs, ipath, dmp, pSym, pRes);

    //dump all nested FBs/structs that are underneath this entry if any:
    if(pSym->flags & CG_STAB_HAS_INST)
    {
        dmp<<_T("\n>\n");
        InstViewSymtabRec(pdm, pIF, ipath, dmp, pSym, pRes);
        dmp<<_T("</NODE>\n");
    }
    else
    {
        dmp<<_T("/>\n");
    }

    dmp.decrIndent();        

    ipath.stripLast();
}


static void InstViewSymtabRec(
    CGT_CDiagMsg*      pdm,     //for err msg
    ICG_IECFront*      pIF,     //used to ?, currently not used..
    CGT_CInstPath      ipath, 
    CGT_CDmp&          dmp,
    const CG_Symtab*   pSym,
    const CG_Resource* pRes     //calling res.
)
{
    int                i;
    const CG_SymEntry* pEntry;

    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(!pEntry->pType->pSym || pEntry->declScope==CG_VAR_EXTERNAL || pEntry->declScope==CG_VAR_IN_OUT)
            continue;

        if(pSym->pLibCtx && pEntry->declScope==CG_VAR)
            continue;

        InstViewEntryRec(pdm, pIF, ipath, dmp, pEntry, pRes);
    }
    
    if(pSym->pouTyp == CG_POU_GVL)
    {
        for(i=0; i<pSym->nGVLImp; i++)
        {
            if(pSym->pGVLImp[i].pSym->flags & CG_STAB_HAS_INST)
                InstViewSymtabRec(pdm, pIF, ipath, dmp, pSym->pGVLImp[i].pSym, pRes);
        }
    }
}

/* ---------------------------------------------------------------------------- */
