

#include "cgt_tools.h"
#include "cgt_misc.h"
#include "XML_dbi_names.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_libmem.h"



static void DumpEntryDBI(
    const CG_SymEntry*    pEntry,
	CGT_CDmp&             dmp,
    const TCHAR*          pszDbiPath,
    bool                  bMakeGlobal,
    bool                  bIFOnly,
    int                   nGVL      //if >=0  dump GVL="<nGVL>"
)
{
    CG_DECLSCOPE ds = pEntry->declScope;

    if(pEntry->pType->type==CG_VOID)
        return;  //nothing todo if type is void (function name of void function)

    if (bMakeGlobal && ds == CG_VAR_EXTERNAL)
        ds = CG_VAR_GLOBAL;
    
    if(bIFOnly)
    {
        if(pEntry->pParent->pouTyp==CG_POU_FB)
            if(ds!=CG_VAR_INPUT && ds!=CG_VAR_OUTPUT && ds!=CG_VAR_IN_OUT)
                return;
    }
    
    dmp<<_T('<')<<CGT_StoreClassName(ds)<<_T(" ")XML_DBI_ATTR_ID;
    dmp<<_T("=\"")<<pEntry->pszId<<_T("\" ")XML_DBI_ATTR_TYPE;
    dmp<<_T("=\"")<<pEntry->pType->pszName<<_T('"');
    if(nGVL>=0)
        dmp<<_T("  GVL=\"")<<nGVL<<_T('"');

    if(pEntry->pType->pSym)
    {
        dmp<<_T(" ")XML_DBI_ATTR_DBI _T("=\"");
        dmp.dumpAsXMLUTF8(pszDbiPath)<<_T('\\')<<pEntry->pType->pSym->pszId<<_T(".")CGT_EXT_DBI _T("\"");
    }
    if(ds==CG_CONST)
    {
        assert(pEntry->pInit);
        if(pEntry->pInit && pEntry->pInit->ut==CG_INIT_SIMPLE)
        {
            dmp<<_T(" ")XML_DBI_ATTR_VALUE _T("=");
            dmp.xmlAttrVal(pEntry->pInit->sim.pLit->psz);
        }
    }

    dmp<<_T("/>\n");
}


static void DumpVariablesDBI(
    const CG_Symtab*         pSym,
	CGT_CDmp&                dmp,
    const TCHAR*             pszDbiPath,
    const CG_SymEntry**      ppzpAutoExt, //can be NULL 
    int                      nGVL         //if >=0  dump GVL="<nGVL>"
)
{
    int                      i;
    const CG_SymEntry*       pEntry;
    const CG_SymEntry*const* pzRefGlbCst;

    for(pEntry=pSym->pFirst; pEntry; pEntry = pEntry->pNext)
        DumpEntryDBI(pEntry, dmp, pszDbiPath, false, pSym->pLibCtx!=NULL, nGVL);

    //assert: must be set, it's not set only if body not loaded yet or
    //was in error but then don't call dbi dump!
    pzRefGlbCst = pSym->pzRefGlbCst;
    assert(pzRefGlbCst);
    if(pzRefGlbCst)
    {   
        for(i= 0; pzRefGlbCst[i]; i++)
            DumpEntryDBI(pzRefGlbCst[i], dmp, pszDbiPath, false, false, -1);
    }

    if(pSym->pouTyp==CG_POU_GVL)
    {
        for(i=0; i<pSym->nGVLImp; i++)
            DumpVariablesDBI(pSym->pGVLImp[i].pSym, dmp, pszDbiPath, NULL, i);

        if (ppzpAutoExt)
            for(i=0; ppzpAutoExt[i] != NULL; i++)
                DumpEntryDBI(ppzpAutoExt[i], dmp, pszDbiPath, true, false, -1);

    }
}



static void DumpSFCElemEdp(
    CGT_CDmp&        dmp,
    const CG_Edipos* pEdp
)
{
    if(pEdp->typ == CG_EDP_4GR_BOX)
    {
        dmp<<_T(" ")<< XML_DBI_ATTR_X << _T("=\"") << pEdp->gr.x << _T("\"");
        dmp<<_T(" ")<< XML_DBI_ATTR_Y << _T("=\"") << pEdp->gr.y << _T("\"");
    }
    else if(pEdp->typ == CG_EDP_ST)
    {
        dmp<<_T(" ")<< XML_DBI_ATTR_X << _T("=\"") << pEdp->lc.c << _T("\"");
        dmp<<_T(" ")<< XML_DBI_ATTR_Y << _T("=\"") << pEdp->lc.l << _T("\"");
    }
    else
    {
        assert(!"bad edipos for SFC");
    }
}
 
extern "C" bool CGT_DumpPOUDBIFile(
    CGT_CDiagMsg*        pdm,   
    ICG_IECFront*        pIF,   
    const CG_Symtab*     pSym,
    ICG_Body*            pIBdy,
    const CGT_BP_TYP*    pBPs,
    int                  nBPs,
	const TCHAR*         pszDbiPath
)
{
	int                      i;
    CGT_CDmp                 dmp;
    FC_CString               jot(MAX_PATH);
    const TCHAR*             pszPouTyp;

    dmp<<CGT_XML_STANDARD_HDR _T("\n\n");

    dmp<<_T("<")XML_DBI_ELEM_POUS _T(">\n");

    if(pSym->pouTyp==CG_POU_FUN)
        pszPouTyp = _T("FUNCTION_BLOCK");//TODO: fake a fb, dbi client in 4ce does not understand functions...
    else
        pszPouTyp = CGT_PouTypeName(pSym->pouTyp, false);
        
    dmp<<_T("<")<<pszPouTyp<<_T(" ")XML_DBI_ATTR_ID _T("=");
    dmp.xmlAttrVal(pSym->pszId);
    
    dmp<<_T(" ")XML_DBI_ATTR_SOURCE _T("=");
    
    dmp.xmlAttrVal(pSym->edp.pszFile);
    
    if(pSym->srcTyp==CG_SRC_TYPE_ST)
    {
        dmp<<_T(" ")XML_DBI_ATTR_LANG _T("=\"ST\"");
    }
    else if (pSym->srcTyp==CG_SRC_TYPE_4GR)
    {
        dmp<<_T(" ")XML_DBI_ATTR_LANG _T("=\"4GR\"");
    }
    
    if(pSym->flags & CG_STAB_HAS_BDY)
    {
        if(pSym->flags & CG_STAB_BDY_IS_SFC)
            dmp<<_T(" ")XML_DBI_ATTR_ISSFC _T("=\"1\"");
        else
            dmp<<_T(" ")XML_DBI_ATTR_ISSFC _T("=\"0\"");
    }
    dmp<<_T(">\n");

    
    dmp.incrIndent();
    dmp<<_T("<")XML_DBI_ELEM_VARIABLES _T(">\n");
    dmp.incrIndent();


    DumpVariablesDBI(pSym, dmp, pszDbiPath, NULL, -1);


    dmp.decrIndent();
    dmp<<_T("</") XML_DBI_ELEM_VARIABLES _T(">\n");

    if(nBPs>0)
    {
        dmp<<_T("<")XML_DBI_ELEM_BREAKPOINTS _T(">\n");
        dmp.incrIndent();
        for(i=0; i<nBPs; i++)
        {
            const CGT_BP_TYP& bps = pBPs[i];
        
            dmp<<_T("<")XML_DBI_ELEM_BP _T(" ") XML_DBI_ATTR_NUM _T("=\"")<<bps.iBP;
            dmp<<_T("\" ")XML_DBI_ATTR_KIND _T("=\"");

            switch(bps.edp.typ)
            {
            case CG_EDP_ST:
                dmp<<XML_DBI_ATTR_VAL_STLINE _T("\" ")XML_DBI_ATTR_Y _T("=\"")<<bps.edp.lc.l<<_T("\"/>\n");;
            break;
            case CG_EDP_4GR_BOX:
                dmp<<XML_DBI_ATTR_VAL_GRBOX _T("\" ")XML_DBI_ATTR_X  _T("=\"")<<bps.edp.gr.x
                                          <<_T("\" ")XML_DBI_ATTR_Y  _T("=\"")<<bps.edp.gr.y
                                          <<_T("\"/>\n");
            break;
            case CG_EDP_4GR_STBOX:
                dmp<<XML_DBI_ATTR_VAL_GRSTBOX _T("\" ")XML_DBI_ATTR_X  _T("=\"")<<bps.edp.gr.x
                                            <<_T("\" ")XML_DBI_ATTR_Y  _T("=\"")<<bps.edp.gr.y
                                            <<_T("\" ")XML_DBI_ATTR_CY _T("=\"")<<bps.edp.gr.l
                                            <<_T("\"/>\n");
            break;
            case CG_EDP_4GR_REGION:
                dmp<<XML_DBI_ATTR_VAL_GRREGION _T("\" ")XML_DBI_ATTR_X  _T("=\"")<<bps.edp.gr.x
                                             <<_T("\" ")XML_DBI_ATTR_Y  _T("=\"")<<bps.edp.gr.y
                                             <<_T("\" ")XML_DBI_ATTR_CX _T("=\"")<<bps.edp.gr.l
                                             <<_T("\" ")XML_DBI_ATTR_CY _T("=\"")<<bps.edp.gr.c
                                             <<_T("\"/>\n");
            break;
            case CG_EDP_4GR_END:
                dmp<<XML_DBI_ATTR_VAL_GREND _T("\"/>\n");
            break;
            case CG_EDP_4GR_BEG:
                dmp<<XML_DBI_ATTR_VAL_GRBEGIN _T("\"/>\n");
            break;
            default:
                assert(!"bad BP edo locid");
            }
        }
        dmp.decrIndent();
        dmp<<_T("</") XML_DBI_ELEM_BREAKPOINTS _T(">\n");
    }


    if(pIBdy && pSym->flags & CG_STAB_BDY_IS_SFC)
    {
        assert(pIBdy->isSFC());
        const CG_SFCStruct* pSFCS = pIBdy->getSFCStruct();
    
        dmp<<_T("<") XML_DBI_ELEM_SFC _T(">\n");
        dmp.incrIndent();
        for (i=0; i<pSFCS->nSteps; i++)
        {
            dmp<<_T("<")<< XML_DBI_ELEM_STEP;
            dmp<<_T(" ")<< XML_DBI_ATTR_ID << _T("=\"")<<pSFCS->pStepNames[i]<< _T("\"");
            dmp<<_T(" ")<< XML_DBI_ATTR_NUM << _T("=\"") << i << _T("\"");
            DumpSFCElemEdp(dmp, &pSFCS->pStepEdp[i]);
            dmp<<_T("/>\n");
        }
        for (i=0; i<pSFCS->nActions; i++)
        {
            dmp<<_T("<")<< XML_DBI_ELEM_ACTION;
            dmp<<_T(" ")<< XML_DBI_ATTR_ID << _T("=\"") << pSFCS->pActionNames[i] << _T("\"");
            dmp<<_T(" ")<< XML_DBI_ATTR_NUM << _T("=\"") << i << _T("\"");
            DumpSFCElemEdp(dmp, &pSFCS->pActionEdp[i]);
            dmp<<_T("/>\n");
        }
        for (i=0; i<pSFCS->nTrans; i++)
        {
            dmp<<_T("<")<< XML_DBI_ELEM_TRANSITION;
            dmp<<_T(" ")<< XML_DBI_ATTR_ID << _T("=\"") << pSFCS->pTransNames[i] << _T("\"");
            dmp<<_T(" ")<< XML_DBI_ATTR_NUM << _T("=\"") << i << _T("\"");
            DumpSFCElemEdp(dmp, &pSFCS->pTransEdp[i]);
            dmp<<_T("/>\n");
        }
        dmp.decrIndent();
        dmp<<_T("</") XML_DBI_ELEM_SFC _T(">\n");
    }


    dmp.decrIndent();
    dmp<<_T("</")<<pszPouTyp<<_T(">\n");
    dmp<<_T("</")XML_DBI_ELEM_POUS _T(">\n");



    jot.load(pszDbiPath)<<_T("\\")<<pSym->pszId<<_T(".")CGT_EXT_DBI;

    return CGT_DmpToFile(pdm, pIF->FCPgetGenPath(), jot, dmp, false);
}




//Dump the following file task1.task.dbi:
//<?xml version="1.0" encoding="UTF-8"?>
//
//<POUS>
//<PROGRAM ID="Task1" SOURCE="Resource1.res" LANG="ST" ISSFC="0">
//    <VARIABLES>
//        <VAR ID="Function1" TYPE="Function1" DBI=".4cpc\.dbi\Function1.dbi"/>
//        <VAR ID="Function2" TYPE="Function2" DBI=".4cpc\.dbi\Function2.dbi"/>
//    </VARIABLES>
//</PROGRAM>
//</POUS>

static bool DumpTaskDBIFile(
    CGT_CDiagMsg*        pdm,   
    ICG_IECFront*        pIFront,   
    const TCHAR*         pszDbiPath,
	const TCHAR*         pszTrgName,
    const CG_Task*       pTsk, 
    const CG_Symtab**    pzpSym
)
{
    int         j;
    CGT_CDmp    dmp;
    FC_CString  fileName;
    FC_CStrList funNames;

    dmp<<CGT_XML_STANDARD_HDR _T("\n\n");

    dmp<<_T("<")XML_DBI_ELEM_POUS _T(">\n");
    dmp<<_T("<PROGRAM ")XML_DBI_ATTR_ID _T("="); //TODO: fake a program, dbi client does not yet understand task instances...
    dmp.xmlAttrVal(pTsk->pszId);
    dmp<<_T(" ")XML_DBI_ATTR_SOURCE _T("=");
    dmp.xmlAttrVal(pTsk->edp.pszFile);
    dmp<<_T(" ")XML_DBI_ATTR_LANG _T("=\"ST\"");
    dmp<<_T(" ")XML_DBI_ATTR_ISSFC _T("=\"0\">\n");

    dmp.incrIndent();
    dmp<<_T("<")XML_DBI_ELEM_VARIABLES _T(">\n");
    dmp.incrIndent();

    for(j=0; pzpSym[j]; j++)
    {
        if(pzpSym[j]->pouTyp==CG_POU_FUN && 
           (pzpSym[j]->flags & CG_STAB_HAS_BDY)!=0 &&
           !pzpSym[j]->pLibCtx
          )
            funNames.add(pzpSym[j]->pszId);
    }

    funNames.qsortI(); //sort alphabetical order before dumping...

    for(j=0; j<funNames.getUsed(); j++)
    {
        const TCHAR* pszId = funNames.get(j);
        dmp<<_T("<")XML_DBI_ELEM_VAR _T(" ")XML_DBI_ATTR_ID _T("=");
        dmp.xmlAttrVal(pszId);
        dmp<<_T(" ")XML_DBI_ATTR_TYPE _T("=");
        dmp.xmlAttrVal(pszId);
        dmp<<_T(" ")XML_DBI_ATTR_DBI _T("=\"");
        dmp.dumpAsXMLUTF8(pszDbiPath)<<_T('\\')<<pszId<<_T(".")CGT_EXT_DBI _T("\"");
        dmp<<_T("/>\n");
    }



    dmp.decrIndent();
    dmp<<_T("</") XML_DBI_ELEM_VARIABLES _T(">\n");

    dmp.decrIndent();
    dmp<<_T("</PROGRAM>\n");
    dmp<<_T("</")XML_DBI_ELEM_POUS _T(">\n");

    (fileName.load(pszTrgName)<<'\\').appendLo(pTsk->pszId)<<_T(".task.")CGT_EXT_DBI;
    return CGT_DmpToFile(pdm, pIFront->FCPgetGenPath(), fileName, dmp, false);
}




extern "C" bool CGT_DumpTRGDBIFile2(
    CGT_CDiagMsg*        pdm,   
    ICG_IECFront*        pIFront,   
    const CG_Resource*   pRes,
	const TCHAR*         pszTrgName,
	const TCHAR*         pszTrgTyp,
	const TCHAR*         pszTrgAddr,
	const TCHAR*         pszDbiPath,
    const bool           bDumpFunInst,
    const CG_SymEntry**  ppzpAutoExt, //can be NULL 
    const GUID*          pAutoExtObj,//can be NULL
    const TCHAR*         pszAutoExtIni//can be NULL
)
{
    int                 i;
    bool                bOk;
    FC_CStrList         instPathes(5000);
    const TCHAR*        pszPath;
    FC_CString          typeName(MAX_PATH);
    FC_CString          typeName2(MAX_PATH);
    const CG_Task*      pTsk;
   
    CGT_CDmp dmp;

    dmp<<CGT_XML_STANDARD_HDR _T("\n\n");

    dmp<<_T("<")XML_DBI_ELEM_POUS _T(">\n");
    dmp<<_T("<")XML_DBI_ELEM_RESOURCE _T(" ")XML_DBI_ATTR_ID _T("=");
    dmp.xmlAttrVal(pRes->pszId)<<_T(" ")XML_DBI_ATTR_TYPE _T("=");
    dmp.xmlAttrVal(pRes->pszTyp ? pRes->pszTyp : _T(""));
    dmp<<_T(">\n");
    dmp.incrIndent();

    //the import GVLs:
    dmp<<_T("<GVLS>\n");
    dmp.incrIndent();
    for(i=0; i<pRes->pGVL->nGVLImp; i++)
    {
        dmp<<_T("<GVL FILE=");
        dmp.xmlAttrVal(pRes->pGVL->pGVLImp[i].pSym->edp.pszFile);
        dmp<<_T(" ID=\"")<<i<<_T("\"/>\n");
    }
    dmp.decrIndent();
    dmp<<_T("</GVLS>\n");
    
    if(pAutoExtObj)
    {
        wchar_t wbuf[64];
        dmp<<_T("<CUSTOMGLOBAL\n");

        dmp.incrIndent();

        dmp<<_T("TARGETTYPE=\"")<<pszTrgTyp<<_T("\"\n");

        dmp<<_T("CUSTOM_DECLARATION=\""); 
        StringFromGUID2(*pAutoExtObj, wbuf, FC_ARRAY_LEN(wbuf));
        dmp<<wbuf<<_T("\"\n");
        
        dmp<<_T("CUSTOM_DECL_CONFIG=\"");
        if(pszAutoExtIni)
            dmp<<pszAutoExtIni;
        dmp<<_T("\"\n");
        
        dmp.decrIndent();
        
        dmp<<_T("/>\n");
    }

    //get all Program and FB Instances of this resource:
    CGT_BuildInstanceTree(pRes->pGVL, &instPathes);


    //the glob vars:
    dmp<<_T("<") XML_DBI_ELEM_VARIABLES _T(">\n");
    dmp.incrIndent();
    DumpVariablesDBI(pRes->pGVL, dmp, pszDbiPath, ppzpAutoExt, -1);
    if(bDumpFunInst)
    {
        for(i=0; i<pRes->nTasks; i++)
        {
            //<VAR_GLOBAL ID="Task1" TYPE="Task1" DBI=".4cpc\.dbi\Task1.dbi"/>
            pTsk = &pRes->pTasks[i];

            dmp<<_T("<VAR_GLOBAL ")XML_DBI_ATTR_ID _T("=\"")<<pTsk->pszId<<_T("\" ");
            dmp<<XML_DBI_ATTR_TYPE _T("=\"")<<pTsk->pszId<<_T("\" ");
            dmp<<XML_DBI_ATTR_DBI _T("=\"")<<pszTrgName<<'\\'<<pTsk->pszId<<_T(".task.")CGT_EXT_DBI _T("\"/>\n");


            const CG_Symtab** pzpSym = CGT_EnumRefSymtabsOfTask(pdm, pIFront, pTsk);
            if(pzpSym)
            {
                FC_CString typeAndPath(256);
                //loop all referenced FUNs of this tasks:
                for(int j=0; pzpSym[j]; j++)
                {
                    if(pzpSym[j]->pouTyp==CG_POU_FUN)
                    {
                        typeAndPath.load(pzpSym[j]->pszId)<<' '<<pTsk->pszId<<'.'<<pzpSym[j]->pszId;
                        instPathes.add(typeAndPath);
                    }
                }

                bOk = DumpTaskDBIFile(pdm, pIFront, pszDbiPath, pszTrgName, pTsk, pzpSym);
                FC_LibFreeMemory(pzpSym);
                if(!bOk)
                    return false;
            }
            else
            {
                assert(pdm->hasErr());
            }

        }
    }

    dmp.decrIndent();
    dmp<<_T("</")XML_DBI_ELEM_VARIABLES _T(">\n");


    //sort them by case ignore class name Name:
    instPathes.qsortI();

    dmp<<_T("<") XML_DBI_ELEM_INSTANCES _T(">\n");
    dmp.incrIndent();

    for(i=0; i<instPathes.getUsed(); i++)
    {
        pszPath = typeName.clear().appendUpTo(instPathes.get(i), _T(" "));
        assert(*pszPath);
        
        if(lstrcmpi(typeName2, typeName))
        {
            typeName2.load(typeName);

            assert(pIFront->loadHeader(typeName2));
            if(i>0)
                dmp<<_T("</")XML_DBI_ELEM_INSTLIST _T(">\n");

            dmp<<_T("<")XML_DBI_ELEM_INSTLIST _T(" ") XML_DBI_ATTR_TYPE _T("=\"");
            dmp<<typeName2<<_T('"');

            dmp<<_T(" ")XML_DBI_ATTR_DBI _T("=\"");
            dmp.dumpAsXMLUTF8(pszDbiPath)<<_T('\\')<<typeName2<<_T(".")CGT_EXT_DBI _T("\"");
            dmp<<_T(">\n");
        }

        dmp.incrIndent();
        dmp<<_T("<")XML_DBI_ELEM_INST _T(" ") XML_DBI_ATTR_PATH _T("=\"");
        dmp<<pszPath<<_T("\"/>\n");
        dmp.decrIndent();
    }
    if(i>0)
        dmp<<_T("</")XML_DBI_ELEM_INSTLIST _T(">\n");

    dmp.decrIndent();
    dmp<<_T("</")XML_DBI_ELEM_INSTANCES _T(">\n");

    dmp.decrIndent();
    dmp<<_T("</")XML_DBI_ELEM_RESOURCE _T(">\n");
    dmp<<_T("</")XML_DBI_ELEM_POUS _T(">\n");

    //dump to disk: file name PC1\PC1.res.dbi 
    //d-1508: use .res.dbi in order to remove name clash with POU dbi!)
    typeName.load(pszTrgName)<<_T("\\")<<pszTrgName<<_T(".")CG_PREDEF_SOURCE_RES _T(".")CGT_EXT_DBI;
    bOk = CGT_DmpToFile(pdm, pIFront->FCPgetGenPath(), typeName, dmp, false);




    //HACK: as long as DBI is not fully multi target type aware....
    //<TARGET ID="WhiskeyPlant" PCNAME="LocalPC" PCADDR="W-lue1">
    //    <RESOURCEREF ID="Resource1" DBI="pc1\pc1.dbi"/>
    //</TARGET>
    dmp.seek(0);
    dmp.incrIndent();
    dmp<<_T("<TARGET ID=\"")<<pszTrgName<<_T("\" PCNAME=\"")<<pszTrgName<<_T("\" PCADDR=");
    dmp.xmlAttrVal(pszTrgAddr)<<_T(">\n");

    dmp.incrIndent();
    dmp<<_T("<RESOURCEREF ID=\"")<<pRes->pszId<<_T("\" DBI=\"")<<typeName<<_T("\"/>\n");
    dmp.decrIndent();
    dmp<<_T("</TARGET>");
    dmp.decrIndent();

    typeName.load(pszTrgName)<<_T("\\")<<pszTrgName<<_T(".trg");
    bOk = bOk && CGT_DmpToFile(pdm, pIFront->FCPgetGenPath(), typeName, dmp, false);
    return bOk;
}
