#include "helper.h"
#include "fc_tools\fc_registry.h"
#include "fc_tools\fc_cgarray.h"
#include "cgt_CDiagMsg.h"
#include "cgt_misc.h"
#include "parse4CP.h"
#include "utilif.h" //UTIL_INSTALL_KEY,..

struct KeyValEDP 
{
    long    line;

    long    keycol;
    FC_CStr key;

    long    valcol;
    FC_CStr val;
};
static void Build4CPAttrHash(
    const TCHAR*                                pszEdpFile,
    const FC_CHashStr<FC_CGArray<KeyValEDP*>*>& text,
    FC_CHashStr<CG_AttribList*>&                attr,
    FC_CHashStrTable&                           strtbl
);
static void BuildLIBInfo(
    CGT_CDiagMsg*                pdm,
    const CG_AttribList*         pSec,
    FC_CHashStrTable&            strtbl,
    FC_CHashStr<CG_LIBContext*>* pLIBinfo
);
static void BuildTRGInfo(
    CGT_CDiagMsg*                pdm,
    const CKADInfo&              kad,
    const CG_AttribList*         pSec,
    FC_CHashStrTable&            strtbl,
    FC_CHashStr<CG_POU4CPInfo*>* pTRGinfo
);
static void BuildPOUInfo(
    CGT_CDiagMsg*                pdm,
    const CKADInfo&              kad,
    const TCHAR*                 pszSecName,
    const CG_AttribList*         pSec,
    FC_CHashStrTable&            strtbl,
    FC_CHashStr<CG_POU4CPInfo*>* pCSTinfo,
    FC_CHashStr<CG_POU4CPInfo*>* pCONinfo,
    FC_CHashStr<CG_POU4CPInfo*>* pGVLinfo,
    FC_CHashStr<CG_POU4CPInfo*>* pPOUinfo
);



static const TCHAR*   g_pzpszEmpty[]   = {NULL};
static CG_POU4CPInfo* g_pzpPouInfoEmpty[] = {NULL};
static CG_LIBContext* g_pzpLibInfoEmpty[] = {NULL};

ST_Parse4CP::ST_Parse4CP()  
 : 
  m_4cp(true),
  m_TARinfo(true, false),
  m_CONinfo(true, false),
  m_POUinfo(true, false),
  m_CSTinfo(true, false),
  m_GVLinfo(true, false),
  m_LIBinfo(true, false)
{
    CG_InitEdipos(&m_edp4CPBeg);
    m_pzpszSecNames = g_pzpszEmpty;
    m_pzpCSTInfo    = g_pzpPouInfoEmpty;
    m_pzpGVLInfo    = g_pzpPouInfoEmpty;
    m_pzpLIBInfo    = g_pzpLibInfoEmpty;
    m_pzpPOUInfo    = g_pzpPouInfoEmpty;

    m_bDollarInsHelp = true;
}
ST_Parse4CP::~ST_Parse4CP() 
{
    clear();
}

void ST_Parse4CP::clear()
{
    if(m_pzpszSecNames != g_pzpszEmpty)
    {
        delete(m_pzpszSecNames);
        m_pzpszSecNames = g_pzpszEmpty;
    }
    if(m_pzpCSTInfo != g_pzpPouInfoEmpty)
    {
        delete(m_pzpCSTInfo);
        m_pzpCSTInfo = g_pzpPouInfoEmpty;
    }
    if(m_pzpGVLInfo != g_pzpPouInfoEmpty)
    {
        delete(m_pzpGVLInfo);
        m_pzpGVLInfo = g_pzpPouInfoEmpty;
    }
    if(m_pzpLIBInfo != g_pzpLibInfoEmpty)
    {
        delete(m_pzpLIBInfo);
        m_pzpLIBInfo = g_pzpLibInfoEmpty;
    }
    if(m_pzpPOUInfo != g_pzpPouInfoEmpty)
    {
        delete(m_pzpPOUInfo);
        m_pzpPOUInfo = g_pzpPouInfoEmpty;
    }


    m_TARinfo.deletePtrs();
    m_CONinfo.deletePtrs();
    m_POUinfo.deletePtrs();
    m_CSTinfo.deletePtrs();
    m_GVLinfo.deletePtrs();
    m_LIBinfo.deletePtrs();
    m_4cp.deletePtrs();

    CG_InitEdipos(&m_edp4CPBeg);
    m_sz4CPFile.load(_T(""));
    m_sz4CPFileForDiag.load(_T(""));
    m_szFileFullPath.load(_T(""));
    m_szFileNameNoExt.load(_T(""));
    m_szPrjPath.load(_T(""));
    m_szGenPath.load(_T(""));
    m_szInstDir.load(_T(""));

    m_bDollarInsHelp = true;
}

bool ST_Parse4CP::is4CPLoaded(void)const
{
    return !m_sz4CPFile.isEmpty();
}
const TCHAR* ST_Parse4CP::getFileAndPath(void)const
{
    return m_szFileFullPath;
}
const TCHAR* ST_Parse4CP::getPrjName(void)const
{
    return m_szFileNameNoExt;
}
const TCHAR* ST_Parse4CP::getPrjPath(void)const
{
    return m_szPrjPath;
}
const TCHAR* ST_Parse4CP::getGenPath(void)const
{
   return m_szGenPath;
}



const TCHAR*const*  ST_Parse4CP::getSectionNames()
{
    if(m_pzpszSecNames==g_pzpszEmpty)
        m_pzpszSecNames = m_4cp.allocKeyArray(NULL);
    return m_pzpszSecNames;
}
const CG_AttribList* ST_Parse4CP::getSection(const TCHAR* pszSection)
{
    return m_4cp.get(pszSection);
}



const CG_POU4CPInfo* ST_Parse4CP::getTARinfo(const TCHAR* pszTAR)
{
    assert(!_tcspbrk(pszTAR, FC_PATH_SEP _T(".")));
    return m_TARinfo.get(pszTAR);
}
const CG_POU4CPInfo* ST_Parse4CP::getCONinfo(const TCHAR* pszCON)
{
    if(pszCON)
    {
        assert(!FC_StringCGetAfterLast(pszCON, FC_PATH_SEP _T(".")));
        return m_CONinfo.get(pszCON);
    }
    else
    {
        return m_CONinfo.firstPair() ? m_CONinfo.firstPair()->pVal : NULL;
    }
}
const CG_POU4CPInfo* ST_Parse4CP::getPOUinfo(const TCHAR* pszPOU)
{
    assert(!_tcspbrk(pszPOU, FC_PATH_SEP _T(".")));
    return m_POUinfo.get(pszPOU);
}
const CG_LIBContext* ST_Parse4CP::getLIBinfo(const TCHAR* pszLIB)
{
    assert(!_tcspbrk(pszLIB, FC_PATH_SEP));
    assert(_tcschr(pszLIB, _T('.')));
    return m_LIBinfo.get(pszLIB);
}


const CG_POU4CPInfo*const* ST_Parse4CP::getAllCSTs(void)
{
    if(m_pzpCSTInfo==g_pzpPouInfoEmpty)
        m_pzpCSTInfo = m_CSTinfo.allocValArray(NULL);
    return m_pzpCSTInfo;
}
const CG_POU4CPInfo*const* ST_Parse4CP::getAllGVLs(void)
{
    if(m_pzpGVLInfo==g_pzpPouInfoEmpty)
        m_pzpGVLInfo = m_GVLinfo.allocValArray(NULL);
    return m_pzpGVLInfo;
}
const CG_POU4CPInfo*const* ST_Parse4CP::getAllPOUs(void)
{
    if(m_pzpPOUInfo==g_pzpPouInfoEmpty)
        m_pzpPOUInfo = m_POUinfo.allocValArray(NULL);
    return m_pzpPOUInfo;
}
const CG_LIBContext*const* ST_Parse4CP::getAllLIBs(void)
{
    if(m_pzpLIBInfo==g_pzpLibInfoEmpty)
        m_pzpLIBInfo = m_LIBinfo.allocValArray(NULL);
    return m_pzpLIBInfo;
}






bool ST_Parse4CP::initFileNames(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszInstPath,
    const TCHAR*  psz4CPFile
)
{
    FC_CString jot(MAX_PATH);
    TCHAR      szFileAndFullPath[2*MAX_PATH];

    m_szInstDir.load(pszInstPath);

    //initialise the file name members:
    //if 4cp was not a full path assume relative to cwd:
    FC_StringCanonicalPath(
        psz4CPFile, 
        szFileAndFullPath,
        FC_ARRAY_LEN(szFileAndFullPath), 
        NULL,
        FC_PATHM_FULL
      );
   jot.load(szFileAndFullPath);

    m_sz4CPFile.load(psz4CPFile);

    //TODO should be file part only if called from engineering
    //if called from cmdl it should be exactly what was passed 
    //on the command line, the 4CE calles with full path always
    //which makes not nice diags.
    m_sz4CPFileForDiag.load(FC_StringGetFilePart(psz4CPFile));
    CG_InitSTEdipos(&m_edp4CPBeg, m_sz4CPFileForDiag, 1);
    
    m_szFileFullPath.load(jot);
    jot.changeFileExt(NULL);
    m_szFileNameNoExt.load(jot.stripFilePart());
    m_szPrjPath.load(jot);

    assert(!m_szFileNameNoExt.isEmpty());

    //TODO: get gen/lib path from user/registry ? 
    jot.load(m_szPrjPath).appendDir(_T("gen"));
    m_szGenPath.load(jot);
    return true;
}





bool ST_Parse4CP::load(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszInstPath,
    const TCHAR*  psz4CPFile
)
{
    const TCHAR*   pszInFile;
    const TCHAR*   pszLineBeg;
    FC_CString     key(MAX_PATH);
    FC_CString     val(MAX_PATH);
    CG_Edipos      edp;
    KeyValEDP*     pPair;
    int            oldErr;
    TCHAR*         pszFileContent;
    unsigned long  ulLength;
    FC_CGArray<KeyValEDP*>*               pCurrSec;
    FC_CHashStr<FC_CGArray<KeyValEDP*>*>  text(true);

    oldErr = pdm->getTotalErrors();

    //reset all members first:
    clear();
    //file name init:
    initFileNames(pdm, pszInstPath, psz4CPFile);

    
    //do the dirty work:
    if(!ST_LoadTextFileToMem(pdm, NULL, NULL, m_szFileFullPath, 
             &pszFileContent, &ulLength, NULL)
       )
       return false;


    CG_InitSTEdipos(&edp, m_sz4CPFile);
    pszInFile = pszFileContent;
    pCurrSec = NULL;

    edp.lc.l = 1;

NextLine:
    pszLineBeg = pszInFile;
    while(_istspace(*pszInFile) && *pszInFile!='\n')
        pszInFile++;
 
    if(*pszInFile == '\n')
    {
        pszInFile++;
        edp.lc.l++;
        goto NextLine;
    }
    else if(*pszInFile == ';')
    {
        goto SkipLine;
    }
    else if(!*pszInFile)
    {
        goto EndeOfFile;
    }
    else if(*pszInFile==_T('['))//section = "[classes.Programm]
    {
        pszInFile++; //eat the [
        key.clear().appendUpTo(pszInFile, _T(";]\n"), &pszInFile);
        key.trim();
        if(*pszInFile!=_T(']')) 
        {
            edp.lc.c = pszInFile - pszLineBeg + 1;
            pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, &edp, _T("]"), pdm->jot1().addc(*pszInFile));
        }
        else
        {
            pszInFile++;//eat the ]
            //check that after the ] there is either a ; or only white space
            while(*pszInFile && _istspace(*pszInFile) && *pszInFile!='\n')
                pszInFile++;

            if(*pszInFile && *pszInFile!=_T(';') && *pszInFile!='\n')
            {
                edp.lc.c = pszInFile - pszLineBeg + 1;
                pdm->msg1(CG_E_SYNTAX_UNEXPECTED, &edp, pdm->jot1().addc(*pszInFile));
            }
        }

        pCurrSec = text.get(key);
        if(!pCurrSec)
        {
            pCurrSec = new FC_CGArray<KeyValEDP*>;
            text.insert(key, pCurrSec);
        }
    }
    else if(!pCurrSec)
    {
        edp.lc.c = pszInFile - pszLineBeg + 1;
        pdm->msg0(CG_E_SYNTAX, &edp);
    }
    else
    {
        //must be a value:  file=myFB.st
        int nKeyCol = pszInFile - pszLineBeg + 1;
        if(nKeyCol==1)
            nKeyCol = 0;

        key.clear().appendUpTo(pszInFile, _T("=\n"), &pszInFile);
        key.trim();

        //was there no '=' before \n?
        if(*pszInFile!=_T('='))
        {
            edp.lc.c = pszInFile - pszLineBeg + 1;
            pdm->msg1(CG_E_SYNTAX_MISSING, &edp, _T("="));
        }
        else
        {
            pszInFile++;//eat the =
            while(*pszInFile && _istspace(*pszInFile) && *pszInFile!='\n')
                pszInFile++;
            int nValCol = pszInFile - pszLineBeg + 1;

            //get the value trimmed:
            val.clear().appendUpTo(pszInFile, _T("\n"), &pszInFile);
            val.trim();
            
            if(!_tcsicmp(key, CG_KEY_4CP_FILE))
            {
                edp.lc.c = nValCol;
                expandFileInserts(pdm, &edp, &val);
            }//be carefull: don't think it is a good idea to $-sub in all values!!!
             //Reason: a) for example the pre/post build steps commands use 
             //OTHER (more) inserts and b) it is not clear how to handle blanks in file
             //names if the expanded line gets parsed in a speical way.
             //Only for file= we know the semantics !

            pPair = new KeyValEDP;
            pPair->line = edp.lc.l;
            pPair->keycol = nKeyCol;
            pPair->key.load(key);
            pPair->valcol = nValCol;
            pPair->val.load(val);
            pCurrSec->add(pPair);
        }
    }

SkipLine:
    while(*pszInFile && *pszInFile!='\n')
        pszInFile++;
    goto NextLine;

EndeOfFile:
    delete(pszFileContent);
    


    if(oldErr==pdm->getTotalErrors())
    {
        //build m_4cp from tmporary 'text'::
        Build4CPAttrHash(m_sz4CPFileForDiag, text, m_4cp, m_strtbl);
    }

    //delete deep tmp object 'text'
    {
    const FC_CHashStr<FC_CGArray<KeyValEDP*>*>::Pair* pp;
    for(pp=text.firstPair(); pp; pp = pp->pNext)
        pp->pVal->deletePtrs();

    text.deletePtrs();
    }

    if(oldErr!=pdm->getTotalErrors())
    {
        //clear again this, may be BuildInfoHashes added things...
        clear();
        return false; 
    }
    return true; 
}


bool ST_Parse4CP::expandFileInserts(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp, 
    FC_CString*      pVal
)
{
    int     ret;
    long    flags;
    TCHAR   buffer[2*MAX_PATH];

    if(_tcschr(pVal->psz(), _T('$')))
    {
        FC_CString expanded(MAX_PATH);
        //expand opt. $(libdir) e.g. $(libdir)\4C_system.4cl:
        flags = CGT_INS_WARN_UNDEF;
        if(m_bDollarInsHelp)
            flags |= CGT_INS_PRINTHELP;
    
        ret = CGT_SubsStdDollarIns(pdm, NULL, pVal->psz(), pEdp, flags, &expanded);
        if(ret<0)
           m_bDollarInsHelp = false;
        pVal->load(expanded);
    }

    if(!FC_StringCanonicalPath(pVal->psz(), buffer, FC_ARRAY_LEN(buffer), m_szPrjPath, FC_PATHM_KEEPREL))
    {
        pdm->msg1(CG_E_NAME_TOO_LONG, pEdp, pVal->psz());
        return false;
    }
    pVal->load(buffer);
    
    return true;
}

bool ST_Parse4CP::buildPOUHashes(
    CGT_CDiagMsg*   pdm,
    const CKADInfo& kad
)
{
    int          oldErr;
    const FC_CHashStr<CG_AttribList*>::Pair* pp;
    
    
    oldErr = pdm->getTotalErrors();

    //build other hashes from m_4cp:
    for(pp=m_4cp.firstPair(); pp; pp = pp->pNext)
    {
        if(!_tcsicmp(pp->pszKey, CG_SEC_4CP_TARGETS))
        {
            BuildTRGInfo(pdm, kad, pp->pVal, m_strtbl, &m_TARinfo);
        }
        else if(!_tcsicmp(pp->pszKey, CG_SEC_4CP_LIBS))
        {
            BuildLIBInfo(pdm, pp->pVal, m_strtbl, &m_LIBinfo);
        }
        else if(FC_StringStartsWithI(pp->pszKey, CG_SEC_4CP_CLASSES_BEG)>=0)
        {
            BuildPOUInfo(pdm, kad, pp->pszKey, pp->pVal, m_strtbl, 
                    &m_CSTinfo, &m_CONinfo, &m_GVLinfo, &m_POUinfo);
        }
    }


    //check: only one config allowed (or no config):
    if(m_CONinfo.countPairs()>1)
    {
        pdm->msg0(CG_E_ONLY_ONE_CON_PER_PRJ, 
            &m_CONinfo.firstPair()->pNext->pVal->edp);

    }


    return oldErr==pdm->getTotalErrors();
}



static void Build4CPAttrHash(
    const TCHAR*                                pszEdpFile,
    const FC_CHashStr<FC_CGArray<KeyValEDP*>*>& text,
    FC_CHashStr<CG_AttribList*>&                attr,
    FC_CHashStrTable&                           strtbl
)
{
    int                      i;
    bool                     bOk;
    int                      nAttribs;
    CG_AttribList*           pSecAttr;
    const TCHAR*             psz4CPFileReg;
    FC_CGArray<KeyValEDP*>*  pSec;

    const FC_CHashStr<FC_CGArray<KeyValEDP*>*>::Pair* pp;

    psz4CPFileReg = strtbl.registerString(pszEdpFile);

    for(pp=text.firstPair(); pp; pp = pp->pNext)
    {
        pSec = pp->pVal;
        nAttribs = pSec->getUsed();    
       
        pSecAttr = (CG_AttribList*) new BYTE[sizeof(CG_KeyValPair)*nAttribs + sizeof(long)]; 
        pSecAttr->nAttribs = nAttribs;
        
        for(i=0;i<nAttribs;i++)
        {
            CG_InitSTEdipos(&pSecAttr->pairs[i].edpKey, psz4CPFileReg, 
                pSec->get(i)->line, pSec->get(i)->keycol);

            pSecAttr->pairs[i].pszKey = 
               strtbl.registerString(pSec->get(i)->key);
       
            CG_InitSTEdipos(&pSecAttr->pairs[i].edpVal, psz4CPFileReg, 
                pSec->get(i)->line, pSec->get(i)->valcol);

            pSecAttr->pairs[i].pszVal = 
               strtbl.registerString(pSec->get(i)->val);
        }

        bOk = attr.insert(pp->pszKey, pSecAttr);
        assert(bOk);
    }
}


static CG_POU4CPInfo* MakePOUInfo(
    CGT_CDiagMsg*                pdm,
    const CG_KeyValPair*         pPair,
    const TCHAR*                 pszSection,
    FC_CHashStrTable&            strtbl
)
{
    CG_POU4CPInfo* pInfo;
    const TCHAR*   pExt;

    pInfo           = new CG_POU4CPInfo;
    pInfo->edp      = pPair->edpVal;
    pInfo->pszClass = strtbl.registerString(pszSection);
    pInfo->pszFile  = strtbl.registerString(pPair->pszVal);

    //get the object name from file name, if cst,gvl the obj name is the file
    //name itself even with opt. path. If 4cl it's the file base name as for 
    //POUs   //TODO name clash if gvl ?
    pExt = FC_StringGetFileExt(pPair->pszVal);
    if(!_tcsicmp(pExt, CG_PREDEF_SOURCE_CST) || !_tcsicmp(pExt, CG_PREDEF_SOURCE_GVL))
    {
        pInfo->pszId = strtbl.registerString(pPair->pszVal);
    }
    else
    {
        TCHAR buffer[2*MAX_PATH];
        _tcscpy(buffer, FC_StringGetFilePart(pPair->pszVal));
        FC_StringCutAtLast(buffer, _T("."));

        pInfo->pszId = strtbl.registerString(buffer);
    }

    return pInfo;
}


static CG_LIBContext* MakeLIBInfo(
    CGT_CDiagMsg*                pdm,
    const CG_KeyValPair*         pPair,
    FC_CHashStrTable&            strtbl
)
{
    CG_LIBContext*     pInfo;

    pInfo           = new CG_LIBContext;
    pInfo->edp      = pPair->edpVal;
    pInfo->pszFile  = strtbl.registerString(pPair->pszVal);
    //get the object name from file name, for 4cl it's the file 
    //base name with extension:
    pInfo->pszId = strtbl.registerString(FC_StringGetFilePart(pPair->pszVal));
    return pInfo;
}


static void BuildLIBInfo(
    CGT_CDiagMsg*                pdm,
    const CG_AttribList*         pSec,
    FC_CHashStrTable&            strtbl,
    FC_CHashStr<CG_LIBContext*>* pLIBinfo
)
{
    int                      i;
    const TCHAR*             pszExt;
    CG_LIBContext*           pInfo;
    const CG_KeyValPair*     pPair;


    for(i=0;i<pSec->nAttribs;i++)
    {
        pPair = &pSec->pairs[i];

        if(_tcsicmp(pPair->pszKey, CG_KEY_4CP_FILE))
        {
            pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, 
                &pPair->edpKey, pPair->pszKey, CG_SEC_4CP_LIBS);
            continue;
        }
        pszExt = FC_StringGetFileExt(pPair->pszVal);
        if(_tcsicmp(pszExt, CG_PREDEF_LIB_EXT))
        {
            pdm->msg4(CG_W_FILEEXT_CLASSES, &pPair->edpVal, 
             pPair->pszVal, pszExt, CG_SEC_4CP_LIBS, CG_PREDEF_LIB_EXT);
            continue;
        }

        pInfo = MakeLIBInfo(pdm, pPair, strtbl);
        if(!pInfo)
            continue;

        if(!pLIBinfo->insertEx(pInfo->pszId, pInfo, false, NULL))
        {
            pdm->msg1(CG_W_FILE_ALREADY_ATTACHED, 
                &pInfo->edp, pInfo->pszFile);
            delete(pInfo);
        }
    }
}

static void BuildTRGInfo(
    CGT_CDiagMsg*                pdm,
    const CKADInfo&              kad,
    const CG_AttribList*         pSec,
    FC_CHashStrTable&            strtbl,
    FC_CHashStr<CG_POU4CPInfo*>* pTRGInfo
)
{
    int                      i;
    CG_POU4CPInfo*           pInfo;
    const CG_KeyValPair*     pPair;

    if(!pSec)
        return; //section empty... 


    for(i=0;i<pSec->nAttribs;i++)
    {
        pPair = &pSec->pairs[i];

        if(_tcsicmp(pPair->pszKey, CG_KEY_4CP_FILE))
        {
            pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, 
                &pPair->edpKey, pPair->pszKey, CG_SEC_4CP_TARGETS);
            continue;
        }

        if(!kad.getTargetTypeByExt(FC_StringGetFileExt(pPair->pszVal), NULL, NULL, NULL))
        {
            pdm->msg2(CG_W_NO_TARGETTYPE_EXT, &pPair->edpVal, 
               pPair->pszVal, kad.getTargetTypeExts());
            continue;
        }
        
        pInfo = MakePOUInfo(pdm, pPair, CG_SEC_4CP_TARGETS, strtbl);

        if(!pTRGInfo->insertEx(pInfo->pszId, pInfo, false, NULL))
        {
            pdm->msg1(CG_W_FILE_ALREADY_ATTACHED, 
                &pInfo->edp, pInfo->pszFile);
            delete(pInfo);
        }
    }
}


static void BuildPOUInfo(
    CGT_CDiagMsg*                pdm,
    const CKADInfo&              kad,
    const TCHAR*                 pszSecName,
    const CG_AttribList*         pSec,
    FC_CHashStrTable&            strtbl,
    FC_CHashStr<CG_POU4CPInfo*>* pCSTinfo,
    FC_CHashStr<CG_POU4CPInfo*>* pCONinfo,
    FC_CHashStr<CG_POU4CPInfo*>* pGVLinfo,
    FC_CHashStr<CG_POU4CPInfo*>* pPOUinfo
)
{
    int                      i;
    const TCHAR*             pszExt;
    CG_POU4CPInfo*           pInfo;
    const CG_KeyValPair*     pPair;


    if(!_tcsicmp(pszSecName, CG_SEC_4CP_STDCLS_GVL))
    {
        for(i=0;i<pSec->nAttribs;i++)
        {
            pPair = &pSec->pairs[i];
            if(_tcsicmp(pPair->pszKey, CG_KEY_4CP_FILE))
            {
                pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, 
                    &pPair->edpKey, pPair->pszKey, pszSecName);
                continue;
            }
            pszExt = FC_StringGetFileExt(pPair->pszVal);
            if(!_tcsicmp(pszExt, CG_PREDEF_SOURCE_GVL))
            {
                pInfo = MakePOUInfo(pdm, pPair, pszSecName, strtbl);
                if(!pGVLinfo->insertEx(pInfo->pszId, pInfo, false, NULL))
                {
                    pdm->msg1(CG_W_FILE_ALREADY_ATTACHED, &pInfo->edp, pInfo->pszFile);
                    delete(pInfo);
                }
            }
            else
            {
                // MessageId: CG_W_FILEEXT_CLASSES
                //  warning C4037: ignore input file '%1': invalid type '%2' for section '%3' (expected only '%4')
                pdm->msg4(CG_W_FILEEXT_CLASSES, &pPair->edpVal, 
                       pPair->pszVal, pszExt, pszSecName, CG_PREDEF_SOURCE_GVL);
            }
        }
    }
    else if(!_tcsicmp(pszSecName, CG_SEC_4CP_STDCLS_CST))
    {
        for(i=0;i<pSec->nAttribs;i++)
        {
            pPair = &pSec->pairs[i];
            if(_tcsicmp(pPair->pszKey, CG_KEY_4CP_FILE))
            {
                pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, 
                    &pPair->edpKey, pPair->pszKey, pszSecName);
                continue;
            }
            pszExt = FC_StringGetFileExt(pPair->pszVal);
            if(!_tcsicmp(pszExt, CG_PREDEF_SOURCE_CST))
            {
                pInfo = MakePOUInfo(pdm, pPair, pszSecName, strtbl);
                if(!pCSTinfo->insertEx(pInfo->pszId, pInfo, false, NULL))
                {
                    pdm->msg1(CG_W_FILE_ALREADY_ATTACHED, &pInfo->edp, pInfo->pszFile);
                    delete(pInfo);
                }
            }
            else
            {
                // MessageId: CG_W_FILEEXT_CLASSES
                //  warning C4037: ignore input file '%1': invalid type '%2' for section '%3' (expected only '%4')
                pdm->msg4(CG_W_FILEEXT_CLASSES, &pPair->edpVal, 
                       pPair->pszVal, pszExt, pszSecName, CG_PREDEF_SOURCE_CST);
            }
        }
    }
    else if(!_tcsicmp(pszSecName, CG_SEC_4CP_STDCLS_CON))
    {
        for(i=0;i<pSec->nAttribs;i++)
        {
            pPair = &pSec->pairs[i];
            if(_tcsicmp(pPair->pszKey, CG_KEY_4CP_FILE))
            {
                pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, 
                    &pPair->edpKey, pPair->pszKey, pszSecName);
                continue;
            }
            pszExt = FC_StringGetFileExt(pPair->pszVal);
            if(!_tcsicmp(pszExt, CG_PREDEF_SOURCE_CON))
            {
                pInfo = MakePOUInfo(pdm, pPair, pszSecName, strtbl);
                if(!pCONinfo->insertEx(pInfo->pszId, pInfo, false, NULL))
                {
                    pdm->msg1(CG_W_FILE_ALREADY_ATTACHED, &pInfo->edp, pInfo->pszFile);
                    delete(pInfo);
                }
            }
            else
            {
                // MessageId: CG_W_FILEEXT_CLASSES
                //  warning C4037: ignore input file '%1': invalid type '%2' for section '%3' (expected only '%4')
                pdm->msg4(CG_W_FILEEXT_CLASSES, &pPair->edpVal, 
                       pPair->pszVal, pszExt, pszSecName, CG_PREDEF_SOURCE_CON);
            }
        }
    }
    else
    {
        //all other [classes.*] are expected to be 'POUs' their extension must be 
        //a valid language extension and the file content (maybe after passed a filter)
        //either a STRUCT,FUNCTION,FB or PROGRAM

        for(i=0;i<pSec->nAttribs;i++)
        {
            pPair = &pSec->pairs[i];
            if(_tcsicmp(pPair->pszKey, CG_KEY_4CP_FILE))
            {
                pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, 
                    &pPair->edpKey, pPair->pszKey, pszSecName);
                continue;
            }
            pszExt = FC_StringGetFileExt(pPair->pszVal);
        
            switch(kad.getSourceTypeByExt(pszExt))
            {
            default:
                assert(!"bad enum");
            case CG_SRC_TYPE_UNDEF:
            case CG_SRC_TYPE_4CP:
            case CG_SRC_TYPE_HDR:
            case CG_SRC_TYPE_RES:
            case CG_SRC_TYPE_GVL:
            case CG_SRC_TYPE_CON:
            case CG_SRC_TYPE_CST:

                //should not be in any POU classes.* section
                //classes.constants and configurations, GVLs
                //are treated specially above.
                pdm->msg4(CG_W_FILEEXT_CLASSES, &pPair->edpVal, 
                       pPair->pszVal, pszExt, pszSecName, kad.getLangExts());
            break;

            case CG_SRC_TYPE_SDT:
            case CG_SRC_TYPE_ST:
            case CG_SRC_TYPE_IL:
            case CG_SRC_TYPE_4GR:
            case CG_SRC_TYPE_ADDON_FILTER:

                pInfo = MakePOUInfo(pdm, pPair, pszSecName, strtbl);
                if(!pPOUinfo->insertEx(pInfo->pszId, pInfo, false, NULL))
                {
                    pdm->msg1(CG_W_FILE_ALREADY_ATTACHED, 
                        &pInfo->edp, pInfo->pszFile);
                    delete(pInfo);
                }

            break;
            }
        }
    }
}




