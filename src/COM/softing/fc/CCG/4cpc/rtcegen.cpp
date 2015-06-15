//----  Local Defines:   -------------------------------------------*
#define RTC_END_OF_POU_LABEL  _T("__end")

#define RTC_EXPR_TMPVAR_BASE  0
#define RTC_LDFBD_TMPVAR_BASE 1000
#define RTC_TRIG_TMPVAR_BASE  20000
//----  Includes:   -------------------------------------------*
#include "cgt_misc.h"
#include "cgt_cdiagmsg.h"
#include "cgt_address.h"
#include "cg_iecfront.h"
#include "rtcegen.h"
#include "helper.h"
#include "defines.h"
#include "fc_tools\fc_system.h"

//----  Static Initializations:   ----------------------------------*

struct RTC_CTX_INFO
{
    CGT_CDiagMsg*            pdm; 
    ICG_IECFront*            pIF; 
    const CTarget*           pTrg;
    const CG_Resource*       pRes;
};

class RTCBodyContext
{
    CGT_CDmp&     m_dmp;
    CGT_CDiagMsg& m_dm; 
    int           m_ExprTempVarCount;
    bool          m_bHasReturnStmt;
public: 
    RTCBodyContext(CGT_CDmp& dmp, CGT_CDiagMsg& dm): m_dmp(dmp), m_dm(dm) 
    { m_ExprTempVarCount = RTC_EXPR_TMPVAR_BASE; m_bHasReturnStmt = false;}

    void dumpRTCEdipos(const CG_Edipos* pEdp)
    { m_dmp<<_T('"')<<m_dm.edpToStr(pEdp, m_dm.jot1())<<_T("\"\n"); }

    void dumpSTStmts(const CG_STStmt* pStmts);
    void dumpExpr(const CG_ExprTree* pExpr); 
    void dumpEndOfPOU();

    // if pExpr is immediate then dump it to str 
    // else dump operations to m_dmp and put result (temp variable) in str
    FC_CString& dumpAnyExpr(const CG_ExprTree* pExpr, FC_CString& str);
    FC_CString& dumpImmediateExpr(const CG_ExprTree* pExpr, FC_CString& sDmp);
    FC_CString& dumpFBArgList(const CG_ArgExpr* pArg, long nArgs, FC_CString& str);
    FC_CString& dumpFunArgList(const CG_ArgExpr* pArg, long nArgs, FC_CString& str);
};

static bool dumpRTCFileForMaster(
    const PC_CIOComp* pfbm, 
    const CG_Task*    pTask, 
    FC_CString&       fileName, 
    RTC_CTX_INFO*     pCtxInfo
);
static bool dumpRCTGlobalSection(
    const CG_Task*    pTask, 
    const PC_CIOComp* pfbm, 
    CGT_CDmp&         dmp, 
    RTC_CTX_INFO*     pCtxInfo
);
static bool dumpRTCDeclSection(const CG_Symtab* pSym, const PC_CIOComp* pfbm, CGT_CDmp& dmp, 
                               RTC_CTX_INFO* pCtxInfo);
static bool createProgInstRTCcode(const PC_CIOComp* pfbm, const CG_SymEntry* pPrgEntry,
                                  CGT_CDmp& rtceFile, RTC_CTX_INFO* pCtxInfo);
static CGT_CDmp& dumpRTCType(CGT_CDmp& dmp, const CG_DType* pType);
static CGT_CDmp& dumpRTCBinVal(CGT_CDmp& dmp, const CG_Literal* const pBin, CG_BITSET lTypeSet);
static bool getRTCFormatOfDirectAddr(const CG_SymEntry* pEntry, const PC_CIOComp* pfbm,
                                     TCHAR result[ST_MAX_RTC_ADDR_LEN], RTC_CTX_INFO* pCtxInfo);
static bool RTC_PatchInitFiles(const FC_CStrList& rtcFileNames, const FC_CGArray<const PC_CIOComp*>& rtcMasters, 
                               FC_CGArray<const CG_SymEntry*>& prgList, FC_CStrList& duplyUsedGlobs, 
                               RTC_CTX_INFO* pCtxInfo);


static const TCHAR* RTC_StmtTypToString(CG_STMTTYP styp)
{
    switch(styp)
    {
    case CG_STMTTYP_UNDEF:
        assert(!"bad stmt typ enum");
        return _T("<internal error, no statement type");
        
    case CG_ASSIGN:       return _T("ASSIGN");
    case CG_CALLVOIDFUN:  return _T("CALLVOIDFUN");
    case CG_CALLFB:       return _T("CALLFB");
    case CG_CASE:         return _T("CASE");
    case CG_IF:           return _T("IF");
    case CG_REPEAT:       return _T("REPEAT");
    case CG_WHILE:        return _T("WHILE");
    case CG_FOR:          return _T("FOR");
    case CG_GOTO:         return _T("GOTO");
    case CG_LABEL:        return _T("LABEL");
    case CG_EXIT:         return _T("EXIT");
    case CG_RETURN:       return _T("RETURN");
    case CG_EMPTY:        return _T("EMPTY"); 
    case CG_NATIVE:       return _T("NATIVE"); 
    case CG_SYNCHRONIZED: return _T("SYNCHRONIZED"); 
    case CG_LDFBDNETWORK: return _T("LDFBDNETWORK");
    }         
    assert(!"bad stmt typ enum");
    return _T("<internal error, no statement type");
}

static const TCHAR* RTC_ExprTypToString(CG_EXPRTYP etyp)
{
    switch(etyp)
    {
    case CG_EXPRTYP_UNDEF: 
        assert(!"bad expr typ enum");
        return _T("<internal error, no expr. type");
    //unary ops:
    case CG_NOT:        return _T("not");
    case CG_UMINUS:     return _T("?uminus");   // need special treatment !
    case CG_NOOP:       return _T("?()");       // need special treatment !
    //binary ops:
    case CG_OR:         return _T("or"); 
    case CG_XOR:        return _T("xor"); 
    case CG_AND:        return _T("and"); 
    case CG_EQ:         return _T("eq"); 
    case CG_NE:         return _T("ne"); 
    case CG_LT:         return _T("lt");
    case CG_LE:         return _T("le");
    case CG_GT:         return _T("gt");
    case CG_GE:         return _T("ge");
    case CG_ADD:        return _T("add");
    case CG_SUB:        return _T("sub");
    case CG_MUL:        return _T("mul");
    case CG_DIV:        return _T("div");
    case CG_MOD:        return _T("mod");
    case CG_POW:        return _T("pow");
    case CG_MOVE:       return _T("?move");     // need special treatment !
    //Primary:
    case CG_LITERAL:    return _T("<literal>");  
    case CG_VARREF:     return _T("<variable>"); 
    case CG_STRUCTREF:  return _T("<struct ref>");
    case CG_ARRAYREF:   return _T("<array ref>");
     //function call:
    case CG_FUNCALL:    
    case CG_FUNMACRO:   return _T("<function>");
    case CG_BUILDIN:    return _T("<buildin>");
    //tmp vars:
    case CG_TMPVAR:     return _T("<tmp>");
    case CG_TRIGVAR:    return _T("<trig tmp>");
    //special:
    case CG_STEPACTION: return _T("<step/action ref>");
    case CG_USEREXPR:   return _T("<user expr>");
    default:
        assert(!"bad expr type enum");
        return _T("<internal error, no expr. type");
    }
}

//entry function for RTCE code gen:
bool CG_GenRTCECodeForTarget(CGT_CDiagMsg* pdm, ICG_IECFront* pIF, CTarget* pTrg)
{
    int                            jj;
    int                            iio;
    const CG_Task*                 pTask;
    const PC_CIOComp*              pfbm;
    bool                           bErrors = false;
    const CG_Task*                 pSyncTask;
    FC_CString                     fileName;
    FC_CStrList                    rtcFileNames;
    FC_CGArray<const PC_CIOComp*>  rtcMasters;
    const FC_CGArray<PC_CIOComp*>* pTrgMasters;
    const CG_TaskInit*             pTaskInit;       
    FC_CGArray<const CG_SymEntry*> prgList;

    RTC_CTX_INFO ctxInfo;
    ctxInfo.pdm  = pdm;
    ctxInfo.pIF  = pIF;
    ctxInfo.pTrg = pTrg;
    ctxInfo.pRes = pTrg->getCGRes();
    assert(ctxInfo.pRes);
    
  
    pTrgMasters = pTrg->getMasters();
    //loop all field bus masters of the assigned PC:
    for(iio=0; iio<pTrgMasters->getUsed(); iio++)
    {
        pfbm = pTrgMasters->get(iio);

        //loop all tasks and pick out those that are IOSYNC to pfbm:
        pSyncTask = NULL;
        for(jj=0; jj<ctxInfo.pRes->nTasks; jj++)
        {
            pTask = &ctxInfo.pRes->pTasks[jj];
            pTaskInit = pIF->lookUpTaskInit(pTask, CG_TASK_IOSYNC);
            
            if(!pTaskInit)
                continue;
            
            if(!ctxInfo.pTrg->isIOEnabled())
            {
                pTrg->warnDisableIO(pdm, &pTaskInit->edp);
                return false;
            }

            // must have been checked already
            assert(pTaskInit->val.extyp==CG_LITERAL && pTaskInit->val.lit.pLit->ut==CG_LIT_I64);
            
            if(pTaskInit && pTaskInit->val.lit.pLit->i64==pfbm->getChannel())
            {           
                if(pfbm->getDPMasterNum()<0)
                {
                    pdm->msg2(CG_E_FBM_NOT_CONFIGURED, &pfbm->m_edp,  
                        pfbm->m_masterName, pTrg->getControl());
                    bErrors = true;
                    continue;
                }
                //only one such task may exist:
                if(pSyncTask)
                {
                    pdm->msg0(CG_E_RTCE_SECOND_SYNC_TASK, &pTask->edp);
                    bErrors = true;
                }
                else
                {
                    pSyncTask = pTask;    
                    fileName.clear();
                    if(!dumpRTCFileForMaster(pfbm, pTask, fileName, &ctxInfo))
                    {
                        bErrors = true;
                    }
                    else
                    {
                        for(int kp=0; kp<pTask->nPrgs; kp++)
                            prgList.add(pTask->pPrgs[kp]);

                        rtcFileNames.add(fileName);
                        rtcMasters.add(pfbm);
                    }
                }
            }
        }
    }

    if(!bErrors && rtcFileNames.getUsed()>0)
    {
        FC_CStrList duplyUsedGlobs;

        if(!RTC_PatchInitFiles(rtcFileNames, rtcMasters, prgList, duplyUsedGlobs, &ctxInfo))
        {
            bErrors = true;
            for(jj=0; jj<duplyUsedGlobs.getUsed(); jj++)
            {
                const CG_SymEntry* pGlobEntry = pIF->lookUpGlobVar(ctxInfo.pRes, duplyUsedGlobs.get(jj));
                assert(pGlobEntry);
                pdm->msg1(CG_E_RTCE_GLOBVAR_REF_TWICE, &pGlobEntry->edp, pGlobEntry->pszId);
            }
        }
    }

    assert(!bErrors || pdm->hasErr());
    return !bErrors;
}


static bool dumpRTCFileForMaster(
    const PC_CIOComp* pfbm, 
    const CG_Task*    pTask, 
    FC_CString&       fileName, 
    RTC_CTX_INFO*     pCtxInfo
)
{
    int                      i;
    const TCHAR*             psz;
    CGT_CDmp                 rtcFile;
    bool                     bErrors = false;
    long                     lInBytes , lOutBytes, lStatusBytes;
    

#if 0
    FC_CString               strFeedback(MAX_PATH);
    strFeedback<<_T(" DEBUGINFO: RTCE code generation: '")<<pTask->pszId<<_T("' channel ")<<pfbm->getChannel();
    strFeedback<<_T(" '")<<pfbm->m_masterName<<_T("'\n");
    pCtxInfo->pdm->userMsg(strFeedback);
#endif
    

    //open code file:
    fileName.load(pCtxInfo->pIF->FCPgetGenPath());
    fileName.appendDir(pCtxInfo->pTrg->getTrgName());
    fileName.toLower();    
    fileName.appendDir(_T("dp@card"));
    fileName << pfbm->getChannel();
    fileName.changeFileExt(RTC_FILE_EXT);

    //copy right:
    rtcFile<<_T(';');
    for(psz = Get4CCopyRightSourceHeader(); *psz; psz = _tcsinc(psz))
    {
        rtcFile<<*psz;
        if(*psz==_T('\n'))
            rtcFile<<_T(';');
    }
    rtcFile<<_T("\n\n");

    //IO size:
    if(pfbm->getDPIOSize(&lInBytes, &lOutBytes, &lStatusBytes))
    {
        assert(lInBytes>=0 && lOutBytes>=0 && lStatusBytes>=0);
        rtcFile<<_T(".rtce\n");
        rtcFile<<_T(" io_in     ")<<lInBytes<<_T('\n');
        rtcFile<<_T(" io_out    ")<<lOutBytes<<_T('\n');
        rtcFile<<_T(" io_status ")<<lStatusBytes<<_T('\n');
        rtcFile<<_T(".end\n\n");
    }
    else
    {
        assert(pCtxInfo->pdm->getDisplayedErrors() != 0);
        return false;
    }

    //append global vars of all prg instances:
    if(!dumpRCTGlobalSection(pTask, pfbm, rtcFile, pCtxInfo))
    {
        assert(pCtxInfo->pdm->getDisplayedErrors() != 0);
        return false;
    }

    //append rtc code of all prg instances of the task:
    for(i=0; i<pTask->nPrgs; i++)
    {
        if(!createProgInstRTCcode(pfbm, pTask->pPrgs[i], rtcFile, pCtxInfo))
            bErrors = true;
        rtcFile<<_T("\n\n");
    }

    //.global section for prg instances (order important!):
    rtcFile<<_T(".global\n");
    for(i=0; i<pTask->nPrgs; i++)
        rtcFile.lo(pTask->pPrgs[i]->pType->pszName).sep(20).lo(pTask->pPrgs[i]->pszId)<<_T('\n');

    rtcFile<<_T(".end\n\n");

    if(!CGT_DmpToFile(pCtxInfo->pdm, NULL, fileName, rtcFile, false))
        return false;
    
    if(!bErrors)//if no errors call 4rtce assembler:
    {
        FC_CString   cmd(2*MAX_PATH);
        FC_CString   RtcAssembler(MAX_PATH);
 
        cmd.load(_T(" -nologo \""))<<fileName<<_T("\"");

        RtcAssembler<<_T(" 4rtc -nologo ")<<FC_StringGetFilePart(fileName)<<_T("\r\n");
        pCtxInfo->pdm->userMsg(RtcAssembler);

        RtcAssembler.load(GetModuleName());
        RtcAssembler.stripFilePart();
        RtcAssembler.appendDir(_T("4rtc"));

        if(!FC_System(RtcAssembler, cmd, NULL, NULL))
        {
            pCtxInfo->pdm->sysErr(GetLastError(), NULL, NULL, NULL);
            bErrors = true;
        }
    }

    assert(!bErrors || pCtxInfo->pdm->getDisplayedErrors()!=0);
    return !bErrors;
}

static bool dumpRCTGlobalSection(
    const CG_Task*    pTask, 
    const PC_CIOComp* pfbm, 
    CGT_CDmp&         dmp, 
    RTC_CTX_INFO*     pCtxInfo
)
{
    int                      jp;
    const CG_SymEntry*       pEntry;
    const CG_SymEntry*       pEntry2;
    TCHAR                    adrbuf[ST_MAX_RTC_ADDR_LEN];
    bool                     bErrorRecovered = false;
    const CG_Symtab*         pSym;
    const CG_SymEntry*       pPrgInst;
    FC_CGArray
        <const CG_SymEntry*> globals;

    dmp<<_T(".global\n");

    //0. find all globals of the resource t
    for(jp = 0; jp < pTask->nPrgs; jp++)
    {
        pPrgInst = pTask->pPrgs[jp];
        if(pPrgInst->declMod & CG_DMOD_RETAIN)
        {
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pPrgInst->edp, _T(""), _T("'RETAIN'"), _T(""));
            bErrorRecovered = true;
        }

        pSym = pPrgInst->pType->pSym;
        for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
        {
            if(pEntry->declScope != CG_VAR_EXTERNAL)
                continue;
        
            pEntry2 = pCtxInfo->pIF->lookUpGlobVar(pCtxInfo->pRes, pEntry->pszId);
            //assert: cannot happen we have made java code with 0 errors !
            assert(pEntry2);
            if(!pEntry2)
                return false;
        
            if(globals.findElem(pEntry2)>=0)//already dumped ? 
                continue;

            globals.add(pEntry2);

            //check data type:        
            if((pEntry2->pType->tbs & ST_RTC_SUPPORTED_TYPES_GVL)==0)
            {
                pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                    pCtxInfo->pdm->jot1()<<pEntry->pszId<<_T(": data type for global variables '"), 
                    pEntry2->pType->pszName, _T("'"));
                //continue after this error to find all other RTCE decl errors:
                bErrorRecovered = true;
            }
            if(pEntry->pType->bIsArray)
            {
                pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                    pEntry->pszId, _T(": data type 'ARRAY'"), _T(""));
                //continue after this error to find all other RTCE decl errors:
                bErrorRecovered = true;
            }
            if(pEntry2->pAttribs && pCtxInfo->pIF->lookUpAttrib(pEntry2->pAttribs, PC_ATTR_JAVACLASS))
            {
                pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry2->edp,
                    _T("object attribute '") PC_ATTR_JAVACLASS _T("' cannot be used in RTCE code"), _T(""), _T(""));
                //continue after this error to find all other RTCE decl errors:
                bErrorRecovered = true;
            }

            dumpRTCType(dmp, pEntry2->pType)<<_T(' ');
            dmp.lo(pEntry2->pszId);
            if(pEntry2->pszAddr)
            {
                if(getRTCFormatOfDirectAddr(pEntry2, pfbm, adrbuf, pCtxInfo))
                {
                    assert(adrbuf[0]==_T('%'));
                    dmp<<_T(" at ")<<adrbuf;
                }
                else
                {
                    pCtxInfo->pdm->msg2(CG_E_RTCE_BAD_ADDR, &pEntry->edp, 
                        pCtxInfo->pdm->jot1()<<pEntry2->pszId<<_T(" AT ")<<pEntry2->pszAddr, pfbm->m_masterName);
                    //continue after this error to find all other RTCE decl errors:
                    bErrorRecovered = true;
                }
            }
            if(pEntry2->pInit)
            {
                if(pEntry2->pszAddr)
                {
                    dmp<<_T("; 4ccg: IO-Initvalues are handled by __iomanager: "); 
                }
                dumpRTCBinVal(dmp<<_T(":="), pEntry2->pInit->sim.pLit, pEntry2->pType->tbs);
            }

            dmp<<_T('\n');
        }
    }

    dmp<<_T(".end\n\n");
    
    assert(!bErrorRecovered || pCtxInfo->pdm->getDisplayedErrors()>0);
    return !bErrorRecovered;
}

static CGT_CDmp& dumpRTCType(CGT_CDmp& dmp, const CG_DType* pType)
{ 
    if((pType->tbs & (CG_TBS_INSTANCE|CG_TBS_STRUCT|CG_TBS_PROGRAM)) && pType->pszName)
    {
        dmp.lo(pType->pszName);
    }
    else if(pType->bIsArray)
    {
        int         i;
        CG_Subrange range;
        int         nDims = pType->nDim;

        dmp.lo(pType->pszName);
        dmp<<_T('[');
        
        for(i=0;i<nDims;i++)
        {
            range = pType->range[i];
            if(i>0)
                dmp<<_T(",");

            dmp<<range.lRangeStart<<_T(".."); 
            //CAUTION: long + ulong == ulong, tthis is bad for negative array bounds
            dmp<<(long)(range.lRangeStart+range.ulRangeLength);
        }
        dmp<<_T(']');
    }
    else if((pType->tbs & CG_TBS_ANY_STRING) && pType->lStrLen>=0)
    {
        dmp.lo(pType->pszName);
        dmp<<_T('[')<<pType->lStrLen<<_T(']');
    }
    else
    {
        dmp.lo(pType->pszName);
    }
    
    return dmp;
}

static CGT_CDmp& dumpRTCBinVal(CGT_CDmp& dmp, const CG_Literal* const pBin, CG_BITSET lTypeSet)
{
    switch(lTypeSet)
    {
    case CG_TBS_BOOL:  dmp<<(pBin->i64 ? _T("true") : _T("false")); break;
    case CG_TBS_BYTE:  dmp<<_T("byte#")<<pBin->i64;  break;
    case CG_TBS_WORD:  dmp<<_T("word#")<<pBin->i64;  break;
    case CG_TBS_DWORD: dmp<<_T("dword#")<<pBin->i64; break; 
    case CG_TBS_DINT:  dmp<<_T("dint#")<<pBin->i64;  break;
    case CG_TBS_TIME:  dmp<<_T("t#")<<pBin->i64<<_T("ms"); break;
    default:
        assert(!"bad rtc type");
        dmp<<_T("<bad rtc type>");
    }
    return dmp;
}

static FC_CString& dumpRTCBinVal(FC_CString& dmp, const CG_Literal* const pBin, CG_BITSET lTypeSet)
{
    switch(lTypeSet)
    {
    case CG_TBS_BOOL:  dmp<<(pBin->i64 ? _T("true") : _T("false")); break;
    case CG_TBS_BYTE:  dmp<<_T("byte#")<<pBin->i64;  break;
    case CG_TBS_WORD:  dmp<<_T("word#")<<pBin->i64;  break;
    case CG_TBS_DWORD: dmp<<_T("dword#")<<pBin->i64; break; 
    case CG_TBS_DINT:  dmp<<_T("dint#")<<pBin->i64;  break;
    case CG_TBS_TIME:  dmp<<_T("t#")<<pBin->i64<<_T("ms"); break;
    default:
        assert(!"bad rtc type");
        dmp<<_T("<bad rtc type>");
    }
    return dmp;
}

static bool createProgInstRTCcode(const PC_CIOComp*  pfbm,
                                  const CG_SymEntry* pPrgEntry,
                                  CGT_CDmp&          rtceFile,
                                  RTC_CTX_INFO*      pCtxInfo)
{
    const CG_Symtab* const& pSymtab = pPrgEntry->pType->pSym;
    int                     iErrorsInFile = pCtxInfo->pdm->getTotalErrors();

    assert(pSymtab && pSymtab->pouTyp == CG_POU_PRG);

    if(!dumpRTCDeclSection(pSymtab, pfbm, rtceFile, pCtxInfo))
        return false;

    ICG_Body* pBody;
    const CG_Symtab* dg;
    dg = pCtxInfo->pIF->loadBody(pPrgEntry->pType->pSym->pszId, CG_LOADBDY_TMPVAR_OPT1, &pBody);

    assert(pBody || pCtxInfo->pdm->hasErr());
    if (pBody)
    {
        // this is cheched before
        assert(!pBody->isSFC());

        const CG_STStmt* pStmts; 
        RTCBodyContext bodyCtx(rtceFile, *pCtxInfo->pdm);
        pStmts = pBody->getSTStmts();

        bodyCtx.dumpSTStmts(pStmts);
        bodyCtx.dumpEndOfPOU();

        pBody->Release();
    }

    iErrorsInFile = pCtxInfo->pdm->getTotalErrors()-iErrorsInFile; 
    return iErrorsInFile==0;
}

static bool dumpRTCDeclSection(const CG_Symtab* pSym, const PC_CIOComp* pfbm, CGT_CDmp& dmp, 
                               RTC_CTX_INFO* pCtxInfo)
{
    const CG_SymEntry* pEntry;
    TCHAR              adrbuf[ST_MAX_RTC_ADDR_LEN];
    bool               bErrorRecovered = false;
    
    switch(pSym->pouTyp)
    {
    case CG_POU_PRG:   dmp<<_T(".prg "); break;
    case CG_POU_FUN:   dmp<<_T(".fun ");
    case CG_POU_FB:    dmp<<_T(".fb ");
        pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pSym->edp, 
            _T("POU type '"), CGT_PouTypeName(pSym->pouTyp, false), _T("'"));
        return false;
    }
    dmp.lo(pSym->pszId)<<_T('\n');

    // check for sfc (not allowed in rtce)
    if (pSym->srcTyp == CG_SRC_TYPE_4GR && (pSym->flags & CG_STAB_BDY_IS_SFC))
    {
        pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pSym->edp, _T(""), _T("SFC elements"), _T(""));
        return false;
    }

    //1. dump all local variables, no const no externals
    //   but do also type restiction of var externals (and the used var_glob from #import):
    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {      
        //check restrictions:
        if((pEntry->pType->tbs & ST_RTC_SUPPORTED_TYPES)==0)
        {
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp,
                _T("data type '"), pEntry->pType->pszName, _T("'"));
            //continue after this error to find all other RTCE decl errors:
            bErrorRecovered = true;
        }
        if(pEntry->pType->bIsArray)
        {
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                pEntry->pszId, _T(": data type 'ARRAY'"), _T(""));
            //continue after this error to find all other RTCE decl errors:
            bErrorRecovered = true;
        }

        if(pEntry->pAttribs && pCtxInfo->pIF->lookUpAttrib(pEntry->pAttribs, PC_ATTR_JAVACLASS))
        {
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp,
                _T("object attribute '") PC_ATTR_JAVACLASS _T("' cannot be used in RTCE code"), _T(""), _T(""));
            //continue after this error to find all other RTCE decl errors:
            bErrorRecovered = true;
        }
        
        if(pEntry->pType->pSym && pEntry->pType->pSym->pouTyp==CG_POU_FB)
        {
            if(pEntry->pType->pSym->pLibCtx)
            {
                //check only a few are supporeted:
                const TCHAR* fbTypeName = pEntry->pType->pSym->pszId;

                if (_tcsicmp(fbTypeName, _T("sr")) && //HACK??
                    _tcsicmp(fbTypeName, _T("rs")) && 
                    _tcsicmp(fbTypeName, _T("f_trig")) &&
                    _tcsicmp(fbTypeName, _T("r_trig")) && 
                    _tcsicmp(fbTypeName, _T("ctu_dint")) && 
                    _tcsicmp(fbTypeName, _T("ctd_dint")) && 
                    _tcsicmp(fbTypeName, _T("ctud_dint")) && 
                    _tcsicmp(fbTypeName, _T("tof")) && 
                    _tcsicmp(fbTypeName, _T("ton")) && 
                    _tcsicmp(fbTypeName, _T("tp")))
                {
                    //not supported function block
                    pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                        _T("'"), _T("only the following function blocks are available: "), 
                        _T("sr, rs, f_trig, r_trig, ctu_dint, ctd_dint, ctud_dint, tof, ton, tp'"));
                    bErrorRecovered = true;
                }
                if(pEntry->pInit)
                {
                    //not supported function block
                    pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                        _T(""), _T(" instance initialization "), _T(""));
                    bErrorRecovered = true;
                }
            }
            else
            {
                //check user defined FB:
                pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                    _T("'"), _T("call of user defined function block"), _T("'"));
                bErrorRecovered = true;
            }
        }

        //the RETAIN must be stripped when inserting a glob from #imprt gvl 
        //into this symtab, therefore the & CG_DMOD_RETAIN test is ok 
        //also for var external and import gvl:
        if(pEntry->declMod & CG_DMOD_RETAIN)
        {
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                _T(""), _T("'VAR RETAIN'"), _T(" (only global variables may be retain)"));
            bErrorRecovered = true;
        }
        if(pEntry->pszAddr)
        {
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                _T(""), _T("'AT %<addr> inside program'"), _T(""));
            bErrorRecovered = true;
        }
        if(bErrorRecovered)
            continue;

        switch(pEntry->declScope)
        {
        case CG_VAR:
            dmp<<_T("var    ");
            dumpRTCType(dmp, pEntry->pType)<<_T(' ');
            dmp.lo(pEntry->pszId);

            if(pEntry->pszAddr)
            {
                if(getRTCFormatOfDirectAddr(pEntry, pfbm, adrbuf, pCtxInfo))
                {
                    assert(adrbuf[0]==_T('%'));
                    dmp<<_T(" at ")<<adrbuf;
                }
                else
                {
                    pCtxInfo->pdm->msg2(CG_E_RTCE_BAD_ADDR, &pEntry->edp, 
                        pCtxInfo->pdm->jot1()<<pEntry->pszId<<_T(" AT ")<<pEntry->pszAddr, pfbm->m_masterName);
                    //continue after this error to find all other RTCE decl errors:
                    bErrorRecovered = true;
                }
            }

            if(pEntry->pInit)
                dumpRTCBinVal(dmp<<_T(":="), pEntry->pInit->sim.pLit, pEntry->pType->tbs);

            dmp<<_T('\n');
        break;
        case CG_VAR_EXTERNAL:
        case CG_CONST:
        continue;
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT:
        case CG_VAR_IN_OUT:
        case CG_VAR_TEMP:
        case CG_VAR_IN_EXT:
            pCtxInfo->pdm->msg3(CG_E_RTCE_RESTRICTION, &pEntry->edp, 
                _T("declaration scope '"), CGT_StoreClassName(pEntry->declScope), _T("'"));
        }
    }

    dmp<<_T('\n')<<_T('\n');

    assert(!bErrorRecovered || pCtxInfo->pdm->getDisplayedErrors()>0);
    return !bErrorRecovered;
}

static bool getRTCFormatOfDirectAddr(const CG_SymEntry* pEntry, 
                                     const PC_CIOComp* pfbm,
                                     TCHAR result[ST_MAX_RTC_ADDR_LEN], 
                                     RTC_CTX_INFO* pCtxInfo)
{
    long  lChNumOfAddr;
    long  lBitOffset;
    long  lByteOffset;
    IOCOMP_CHK_ADDR checkAddr; 

    checkAddr.pEdp       = &pEntry->edp;
    checkAddr.pszAddr    = pEntry->pszAddr;
    checkAddr.pszType    = pEntry->pType->pszName;
    checkAddr.pszInst    = pEntry->pszId;

    
    lChNumOfAddr = CGT_GetAddrChannelNumber(pEntry->pszAddr);
    if(lChNumOfAddr != pfbm->getChannel())
        return false;

    //check:
    if(pfbm->checkAddr(pCtxInfo->pdm, &checkAddr) && pfbm->getDPAddrByteOffset(pEntry->pszAddr, &lByteOffset, &lBitOffset))
    {
        assert(lByteOffset>=0 && lBitOffset>=0);

        result[0] = _T('%');
        result[1] = CGT_GetAddrLocation(pEntry->pszAddr);

        switch(CGT_GetAddrBitSize(pEntry->pszAddr))
        {
        case 1:  
            wsprintf(result+2, _T("X%d.%d"), lByteOffset, lBitOffset);
            return true;
        case 8:   result[2] = _T('B');break;
        case 16:  result[2] = _T('W');break;
        case 32:  result[2] = _T('D');break; 
        case 64:  result[2] = _T('L');break;
        default: assert(0);
        }
        
        assert(lBitOffset==0);
        ltoa(lByteOffset, result+3, 10);
        if(CGT_IsByteSwapAddress(pEntry->pszAddr))
            _tcscat(result, _T("!swap"));

        return true;
    }
    return false;
}

static inline bool isImmediateExpr(const CG_ExprTree* pExpr)
{
    assert(pExpr);
    switch(pExpr->extyp)
    {
    case CG_LITERAL:
    case CG_VARREF:
    case CG_STRUCTREF:
    case CG_TMPVAR:
    case CG_TRIGVAR:
        return true;
    }
    return false;
}

FC_CString& RTCBodyContext::dumpImmediateExpr(const CG_ExprTree* pExpr, FC_CString& sDmp)
{
    switch(pExpr->extyp)
    {
    case CG_LITERAL:
        dumpRTCBinVal(sDmp, pExpr->lit.pLit, pExpr->pType->tbs);
        break;
    case CG_VARREF:
        sDmp << pExpr->var.pEntry->pszId;
        break;
    case CG_STRUCTREF:
        assert(pExpr->str.pSelc->declScope != CG_VAR_IN_OUT);
        dumpImmediateExpr(pExpr->str.pBase, sDmp);
        sDmp<<_T(".")<<pExpr->str.pSelc->pszId;
        break;
    case CG_TMPVAR:
        sDmp<<_T("@")<<pExpr->pType->pszName<<pExpr->tmp.lTmpNum+RTC_LDFBD_TMPVAR_BASE;
        break;
    case CG_TRIGVAR:
        sDmp<<_T("@")<<pExpr->pType->pszName<<pExpr->trg.lTrigNum+RTC_TRIG_TMPVAR_BASE;
        break;
    default:
        assert(!"expression is not immediate: use function dumpExpr");
        break;
    }
    return sDmp;
}


void RTCBodyContext::dumpExpr(const CG_ExprTree* pExpr) 
{
    FC_CString        jot(1024);
    int               tempNr = m_ExprTempVarCount;
     
    switch(pExpr->extyp)
    {
    case CG_NOT:
        jot<<_T("calf  ")<<RTC_ExprTypToString(pExpr->extyp)<<_T(" ");        
        dumpAnyExpr(pExpr->ops.pLhs, jot)<<_T(",");
        dumpRTCEdipos(&pExpr->edp);
        m_dmp.lo(jot);
        break;
    case CG_UMINUS:
        jot<<_T("calf  ")<<RTC_ExprTypToString(CG_SUB)<<_T(" ");       
        {
        CG_Literal lit;
        lit.i64 = 0;
        dumpRTCBinVal(jot, &lit, pExpr->pType->tbs);
        jot<<_T(",");
        }
        dumpAnyExpr(pExpr->ops.pLhs, jot)<<_T(",");
        dumpRTCEdipos(&pExpr->edp);
        m_dmp.lo(jot);
        break;
    case CG_NOOP:
        if(isImmediateExpr(pExpr->ops.pLhs))
            dumpImmediateExpr(pExpr->ops.pLhs, jot)<<_T(",");
        else
            dumpExpr(pExpr->ops.pLhs);
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
        jot<<_T("calf  ")<<RTC_ExprTypToString(pExpr->extyp)<<_T(" ");
        dumpAnyExpr(pExpr->ops.pLhs, jot)<<_T(",");
        dumpAnyExpr(pExpr->ops.pRhs, jot)<<_T(",");
        dumpRTCEdipos(&pExpr->edp);
        m_dmp.lo(jot);
        break;
    case CG_FUNCALL:
        jot<<_T("calf  ")<<pExpr->fun.pSym->pszId <<_T(" "); 
        dumpFunArgList(pExpr->fun.pArgs, pExpr->fun.nArgs, jot);
        if (pExpr->pType->type == CG_VOID)
            jot.cutAtLast(_T(","));
        dumpRTCEdipos(&pExpr->edp);
        m_dmp.lo(jot);
        break;
    case CG_FUNMACRO:
        jot<<_T("calf  ")<<pExpr->mac.pMac->pSym->pszId <<_T(" "); 
        dumpFunArgList(pExpr->mac.pArgs, pExpr->mac.nArgs, jot);
        if (pExpr->pType->type == CG_VOID)
            jot.cutAtLast(_T(","));
        dumpRTCEdipos(&pExpr->edp);
        m_dmp.lo(jot);
        break;
    case CG_MOVE:
    case CG_ARRAYREF:
    case CG_BUILDIN:
    case CG_STEPACTION:
    case CG_USEREXPR:
        assert(!"bad expr type enum");
        m_dmp<<_T("<internal error: ")<<RTC_ExprTypToString(pExpr->extyp);
        break;
    case CG_LITERAL:
    case CG_VARREF:
    case CG_STRUCTREF:
    case CG_TMPVAR:
    case CG_TRIGVAR:
        assert(!"expression is immediate: use function dumpImmediateExpr");
        break;
    case CG_EXPRTYP_UNDEF:
    default:
        assert(!"bad expr type enum");
        m_dmp<<_T("<internal error: bad expr type>");
        break;
    }
    m_ExprTempVarCount = tempNr;
}

FC_CString& RTCBodyContext::dumpAnyExpr(const CG_ExprTree* pExpr, FC_CString& str)
{
    if(isImmediateExpr(pExpr))
        dumpImmediateExpr(pExpr, str);
    else
    {
        dumpExpr(pExpr);
        if (pExpr->pType->type != CG_VOID)
        {
            m_dmp<<_T("@");
            m_dmp.lo(pExpr->pType->pszName)<<++m_ExprTempVarCount<<_T('\n');
            str<<_T("@")<<pExpr->pType->pszName<<m_ExprTempVarCount;
        }
    }
    return str;
}

FC_CString& RTCBodyContext::dumpFunArgList(const CG_ArgExpr* pArgs, long nArgs, FC_CString& str)
{
    for(long l=0; l<nArgs; l++)
    {
        dumpAnyExpr(pArgs[l].pActual, str);
        str<<_T(",");
    }
    return str;
}

FC_CString& RTCBodyContext::dumpFBArgList(const CG_ArgExpr* pArgs, long nArgs, FC_CString& str)
{
    for(long l=0; l<nArgs; l++)
    {
        if(l) str<<_T(",");

        str<<pArgs[l].pFormal->pszId;

        if(pArgs[l].pFormal->declScope == CG_VAR_INPUT)
            str<<_T(":=");
        else if(pArgs[l].pFormal->declScope == CG_VAR_OUTPUT)
            str<<_T("=>");
        else 
            m_dm.msg3(CG_E_RTCE_RESTRICTION, &pArgs[l].pFormal->edp, 
                _T("'"), _T("VAR_IN_OUT parameter passing"), _T("'"));

        dumpAnyExpr(pArgs[l].pActual, str);
    }
    return str;
}


//dump actual body ....
void RTCBodyContext::dumpSTStmts(const CG_STStmt* pStmts)
{
    const CG_STStmt*  pS;
    FC_CString        jot(1024);
    int               tempNr = m_ExprTempVarCount;

    for(pS = pStmts; pS; pS = pS->pNext)
    {
        switch(pS->styp)
        {
        case CG_ASSIGN:
            assert(pS->asns.pLhs->extyp==CG_TMPVAR  ||
                   pS->asns.pLhs->extyp==CG_VARREF  ||
                   pS->asns.pLhs->extyp==CG_TRIGVAR ||
                   pS->asns.pLhs->extyp==CG_STRUCTREF);
            if(!isImmediateExpr(pS->asns.pRhs))
                dumpExpr(pS->asns.pRhs);
            else 
            {
                m_dmp<<_T("move  ");
                m_dmp.lo(dumpImmediateExpr(pS->asns.pRhs, jot))<<_T("=>");
                jot.clear();
            }
            m_dmp.lo(dumpImmediateExpr(pS->asns.pLhs, jot))<<_T('\n');
            break;
        case CG_CALLFB:
            dumpRTCEdipos(&pS->edp);
            jot<<_T("call  ")<<pS->fbcs.pInst->lit.pConst->pszId<<_T(" "); 
            dumpFBArgList(pS->fbcs.pArgs, pS->fbcs.nArgs, jot);
            m_dmp.lo(jot)<<_T('\n');
            break;
        case CG_CALLVOIDFUN:
            dumpAnyExpr(pS->funs.pExpr, jot);
            m_dmp.lo(jot)<<_T('\n');
            break;
        case CG_IF:
            jot<<_T("if ");
            dumpAnyExpr(pS->ifs.pCond, jot);
            m_dmp.lo(jot)<<_T('\n');
            m_dmp.incrIndent();
            dumpSTStmts(pS->ifs.pThen);
            if(pS->ifs.pElse)
            {
                m_dmp.decrIndent();
                m_dmp<<_T("else\n");
                m_dmp.incrIndent();
                dumpSTStmts(pS->ifs.pElse);
            }
            m_dmp.decrIndent();
            m_dmp<<_T("endif\n");
            break;
        case CG_LABEL:
            m_dmp.up(pS->lbls.pszLbl)<<_T(":\n");
            break;
        case CG_GOTO:
            if(pS->gots.bUpward)
                m_dm.msg3(CG_E_RTCE_RESTRICTION, &pS->edp, _T("'"), _T("GOTO: upward jump"), _T("'"));
            m_dmp<<_T("jmp ");
            m_dmp.up(pS->gots.pLblStmt->lbls.pszLbl)<<_T('\n');
            break;
        case CG_RETURN:
            m_bHasReturnStmt = true;
            m_dmp<<_T("jmp ") RTC_END_OF_POU_LABEL _T("\n");            
            break;
        case CG_EMPTY:
            break;  
        case CG_LDFBDNETWORK:
            m_dmp<<_T("\n;Network\n");
            dumpSTStmts(pS->netw.pStmts);
            break;
        case CG_CASE:
        case CG_REPEAT:
        case CG_WHILE:
        case CG_FOR:
        case CG_EXIT:
        case CG_NATIVE:
        case CG_SYNCHRONIZED:
            m_dm.msg3(CG_E_RTCE_RESTRICTION, &pS->edp, _T("'"), RTC_StmtTypToString(pS->styp), _T("'"));
            break;
        case CG_STMTTYP_UNDEF:
        default:
            assert(!"unknown stmt type");
            m_dmp<<_T("internal error: unknown stmt type");
            break;
        }
        jot.clear();
    }
    m_ExprTempVarCount = tempNr;
}

void RTCBodyContext::dumpEndOfPOU()
{
    if(m_bHasReturnStmt)
        m_dmp<<RTC_END_OF_POU_LABEL _T(":\n");
    
    m_dmp<<_T(".end\n");
}

static TCHAR* RTC_SkipMAPLine(TCHAR* psz, int* piLine)
{
    while(*psz && *psz!=_T('\n'))
        psz = _tcsinc(psz);

    //skip the \n (if any) and advance to first non white space:
    while(_istspace(*psz))
    {
        if(*psz==_T('\n'))
            (*piLine)++;
        psz = _tcsinc(psz);
    }

    return psz;
}

//HACK ??????? but works fine as long there are no user defined structs/fbs:
static bool RTC_PatchInitFiles(const FC_CStrList&                   rtcFileNames, 
                               const FC_CGArray<const PC_CIOComp*>& rtcMasters, 
                               FC_CGArray<const CG_SymEntry*>&      prgList,
                               FC_CStrList&                         duplyUsedGlobs, 
                               RTC_CTX_INFO*                        pCtxInfo)
{
    int          i;
    int          iFound;
    FC_CString   fileName;
    TCHAR*       psz;
    TCHAR*       pszFile;
    int          iLine;
    const TCHAR* pszField;
    CGT_CSVFile  csvRead;
    const TCHAR* pszMemAddr;
    int          channelNum;
    int          masterId;
    FC_CStrList  memoryAddr;
    FC_CStrList  memoryVars;

    FC_CGArray<const PC_CIOComp*> memoryMaster;
    const PC_CIOComp*             pfbm;

    assert(rtcFileNames.getUsed()>0);

    for(i=0; i<rtcFileNames.getUsed(); i++)
    {
        fileName.load(rtcFileNames.get(i));
        fileName.changeFileExt(_T("map"));

        if(!CGT_LoadTextFileToMem(pCtxInfo->pdm, NULL, NULL, fileName, &pszFile, NULL, NULL))
            return false;

        //find the global section(s):
        //.global
        //%MX288.0:  bool:  glob_bool
        //%MB289:    byte:  glob_byte
        //%MW290:    word:  glob_word := word#16#6

        psz = pszFile;
        iLine = 1;
        for( ; *psz; psz = RTC_SkipMAPLine(psz, &iLine))
        {
            if(_tcsncmp(psz, _T(".global"), 7))
                continue;

            psz = RTC_SkipMAPLine(psz, &iLine);
            for( ; *psz && *psz!=_T('.'); )
            {
                const TCHAR* pszAddr;
                const TCHAR* pszVar;

                pszAddr= psz;
                //skip addr:
                while(*psz && *psz!=_T(':') && *psz!=_T('\n'))
                    psz = _tcsinc(psz);
                assert(*psz==_T(':'));
                if(*psz)
                {
                    *psz = 0;
                    psz = _tcsinc(psz);
                }

                //skip type:
                while(*psz && *psz!=_T(':') && *psz!=_T('\n'))
                    psz = _tcsinc(psz);
                assert(*psz==_T(':'));
                if(*psz)
                {
                    *psz = 0;
                    psz = _tcsinc(psz);
                }
                //white space after type:
                while(_istspace(*psz) && *psz!=_T('\n'))
                    psz = _tcsinc(psz);
                
                pszVar= psz;
                //skip instpath:
                while(_istalnum(*psz) || *psz==_T('.') || *psz==_T('_'))
                    psz = _tcsinc(psz);

                bool bWasNewLine = *psz==_T('\n');
                if(*psz)
                {
                    *psz = 0;
                    psz = _tcsinc(psz);
                }
                //at least some test:
                if(pszAddr[0]!=_T('%') || pszAddr[1]!=_T('M') || 
                   (*pszVar!=_T('_') && !_istalpha(*pszVar)))
                {
                    pCtxInfo->pdm->msg2(CG_E_READING_MAP_FILE, NULL, 
                        fileName, pCtxInfo->pdm->jot1()<<iLine);
                    delete(pszFile);
                    return false;
                }

                if(memoryVars.find(pszVar))
                    duplyUsedGlobs.add(pszVar); //error: we cannot share such addresses:

                memoryAddr.add(pszAddr);
                memoryVars.add(pszVar);
                memoryMaster.add(rtcMasters.get(i));
                
                //skip rest of line: 
                if(bWasNewLine)
                    iLine++;
                else if(*psz)
                    psz = RTC_SkipMAPLine(psz, &iLine);
            }
        }

        delete(pszFile);
    }

    if(duplyUsedGlobs.getUsed()>0)
        return false;

    iFound=0;
    //get the __init file name:
    //use the fact that the map files are in directory of 
    //resource and the init file has the fixed name "__.init"
    fileName.load(rtcFileNames.get(0));
    fileName.stripFilePart();
    fileName.appendDir(_T("__.init"));

    if(!CGT_LoadCSVFile(pCtxInfo->pdm, &csvRead, NULL, fileName))
        return false;
    CGT_CDmp resInit;
    bool res = false;
    //skip the following:
    //version;1.20
    //resource_name;resource1
    res = csvRead.getNextField(&pszField);
    assert(res);
    resInit.dumpAsCSV(pszField,-1,true)<<_T(';');

    res = csvRead.getNextField(&pszField);
    assert(res && csvRead.isLastFieldEOL());
    resInit.dumpAsCSV(pszField,-1,true)<<_T('\n');

    res = csvRead.getNextField(&pszField);
    assert(res);
    resInit.dumpAsCSV(pszField,-1,true)<<_T(';');

    res = csvRead.getNextField(&pszField);
    assert(res && csvRead.isLastFieldEOL());
    resInit.dumpAsCSV(pszField,-1,true)<<_T('\n');

    while(!csvRead.isEOF())
    {
        const TCHAR* pszId;
        const TCHAR* pszClass;
        const TCHAR* pszDeclId;
        const TCHAR* pszInitString;
        const TCHAR* pszInitFile;
        res &= csvRead.getNextField(&pszId);
        res &= csvRead.getNextField(&pszClass);
        res &= csvRead.getNextField(&pszDeclId);
        res &= csvRead.getNextField(&pszInitString);
        res &= csvRead.getNextField(&pszInitFile);

        assert(res && csvRead.isLastFieldEOL());

        const CG_SymEntry* pPrg= pCtxInfo->pIF->lookUpGlobVar(pCtxInfo->pRes, pszId);

        if(prgList.findElem(pPrg)!=-1)
        {
            //remove ret prg inst completely
        }
        else if((i = memoryVars.getIndex(pszId)) >= 0)
        {
            iFound++;
            pszMemAddr = memoryAddr.get(i);
            pfbm = memoryMaster.get(i);
            channelNum = pfbm->getChannel();
            masterId   = pfbm->getDPMasterNum();

            if(!_tcscmp(pszClass, _T("system.BoolVar")))
                pszClass = _T("system.IOBoolVar");
            else if(!_tcscmp(pszClass, _T("system.IntVar")) || 
                    !_tcscmp(pszClass, _T("system.ByteVar")) || 
                    !_tcscmp(pszClass, _T("system.WordVar")))
                pszClass = _T("system.IOIntVar");
            else if(!_tcscmp(pszClass, _T("system.LongVar")))
                pszClass = _T("system.IOLongVar");
            else if(!_tcscmp(pszClass, _T("system.DoubleVar")))
                pszClass = _T("system.IODoubleVar");
            else
            {
                pCtxInfo->pdm->userMsg(_T("4CCG: failed to patch init file for rtce: reason: illegal java class\r\n"));
                return false;
            }

            resInit<<pszId<<_T(';');
            resInit<<pszClass<<_T(';');

            resInit<<_T("%M")<<pszMemAddr[2]<<channelNum<<_T('.')<<masterId<<_T('.')<<pszMemAddr+3<<_T(';');
            resInit<<_T("%M")<<pszMemAddr[2]<<channelNum<<_T('.')<<masterId<<_T('.')<<pszMemAddr+3;
            
            if(*pszInitString)
                   resInit<<_T('|')<<pszInitString;
            
            resInit<<_T(';');
            resInit<<_T('\n');
        }
        else if(!_tcscmp(pszId, _T("__iomanager")))
        {
            FC_CString   out(MAX_PATH);
            int          chNumFound;
            const TCHAR* pszIn;
            //append all rtce domain names to init string of the DP-Layer for this channel:
            //__iomanager;system.IOManager;;{0AD3F381-29DE-4440-90CA-A0FA1DBAB265},0,dp@card0.bin,dp@card0.rtce|...;

            resInit<<pszId<<_T(';');
            resInit<<pszClass<<_T(';');
            resInit.dumpAsCSV(pszDeclId,-1,true)<<_T(';');

            //scann through the init string and pick out layers and their channel number
            //then append all dp@Card<chnum>.rtc domains that have the same chnum as
            //new bin domains to that layer:
            for(pszIn= pszInitString; *pszIn; )
            {
                if(pszIn != pszInitString)
                    out.addc(_T('|')); //layers are seperated by |

                //the clsid:
                pszIn = out.appendUpTo(pszIn, _T(","));
                assert(out.len()>0 &&  out[out.len()-1]== /*{*/ _T('}'));
                out.addc(_T(','));

                //the channel number:
                chNumFound = atoi(pszIn);
                assert(_istdigit(*pszIn) && chNumFound>=0);
                
                //get existing domain list of layer, text up to | or end of pszIn if last layer:
                pszIn = out.appendUpTo(pszIn, _T("|"));

            
                //pick out the RTCE domain that has the same ch num:
                for(int kk=0;kk<rtcFileNames.getUsed();kk++)
                {
                    const TCHAR* pszRTCFile = FC_StringGetFilePart(rtcFileNames.get(kk));
                    int          iChNumOff  = FC_StringStartsWithI(pszRTCFile, _T("dp@card"));
                    //rtceDomain look like:   dp@card<chnum>.rtc
                
                
                    if(iChNumOff<=0 || !_istdigit(pszRTCFile[iChNumOff]))
                    {
                        assert(!"bad rtc file name");
                        return false;
                    }

                    if(chNumFound == atoi(pszRTCFile+iChNumOff))
                    {
                        if(out[out.len()-1] != _T(','))
                            out.addc(_T(','));

                        //change file ext from ret to rtce and lower case version:
                        out.appendLo(pszRTCFile).addc(_T('e'));
                    }
                }
            }


            resInit.dumpAsCSV(out,-1,true);
            resInit<<_T(';');
            resInit.dumpAsCSV(pszInitFile,-1,true)<<_T('\n');
        }
        else
        {
            resInit<<pszId<<_T(';');
            resInit<<pszClass<<_T(';');
            resInit.dumpAsCSV(pszDeclId,-1,true)<<_T(';');
            resInit.dumpAsCSV(pszInitString,-1,true)<<_T(';');
            resInit.dumpAsCSV(pszInitFile,-1,true)<<_T('\n');
        }
    }

    if(iFound != memoryVars.getUsed())
    {
        pCtxInfo->pdm->userMsg(_T("4CCG: failed to write rtce map file info to __.init\r\n"));
        return false;
    }

    if(!CGT_DmpToFile(pCtxInfo->pdm, NULL, fileName, resInit, false))
        return false;
    
    //append rtce domains to loader.list:
    fileName.load(rtcFileNames.get(0));
    fileName.stripFilePart();
    fileName.appendDir(_T("loader.list"));
    
    CGT_CDmp loaderList; // append to loader list
    for(i=0; i<rtcFileNames.getUsed(); i++)
    {
        const TCHAR* pszGenRelative;
        FC_CString jot;
        jot.load(rtcFileNames.get(i));
        jot.changeFileExt(_T("rtce"));
        jot.toLower();

        pszGenRelative = jot.psz();
        res = (FC_StringIncBeginI(&pszGenRelative, pCtxInfo->pIF->FCPgetGenPath()) == 1);
        assert(res);

        if(*pszGenRelative==_T('\\') || *pszGenRelative==_T('/'))
            pszGenRelative = _tcsinc(pszGenRelative);

        loaderList<<pszGenRelative<<_T(';')<<FC_StringGetFilePart(jot)<<_T('\n');
    }

    if(!CGT_DmpToFile(pCtxInfo->pdm, NULL, fileName, loaderList, true))
        return false;

    return true;
}


