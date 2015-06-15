

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "CG_Console.h"  //COM interface base
#include "CG_Console_i.c"  //COM interface GUID defines
#include "stdio.h"
#include "fc_tools\fc_registry.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_system.h"
#include "cgt_misc.h"
#include "CG_IECFront.h"
#include "CG_IECFront_i.c"
#include "defines.h"
#include "cgt_xmlparse.h"
#include <process.h>

//----  Static Initializations:   ----------------------------------*
static HINSTANCE g_hinstDLL = NULL;
static int       g_dwRefCount = 0;
static TCHAR     g_szModulePath[MAX_PATH];    //full module path without file name

#include "CG_CConsole.h"  

CG_CConsole::CG_CConsole(): 
    m_dwRefCount(0),
    m_dm(_T("console gen"), g_hinstDLL),
    m_pIFront(NULL),
    m_lConfiFilesCount(2),
    m_aAspFiles(1024),
    m_hSymAsp(128),
    m_sVisPath(MAX_PATH),
    m_sTargetInstName(NULL),
    m_sResourceName(NULL),
    m_sWebServerAddr(NULL),
    m_sControlAddr(NULL),
    m_sTargetTypeName(NULL)
{
    //init the standard asps:       
    m_aAspFiles.add(_T("CP_TuningDisplay.asp"));   // index CP_TuningDisplay.asp and CP_SFC.asp
    m_aAspFiles.add(_T("CP_SFC.asp"));             // should be on position 0 and 1
}

CG_CConsole::~CG_CConsole()
{

}

HRESULT STDMETHODCALLTYPE CG_CConsole::genConsoleForTarget(
    IUnknown* pIUnkFront, 
    BSTR      sTargetInstName, 
    BSTR      sResourceName, 
    BSTR      sWebServerAddr,
    BSTR      sControlAddr,
    BSTR      sTargetTypeName,
    long*     plErrors,
    long*     plWarnings
)
{
    HRESULT hr;

    m_sTargetInstName = sTargetInstName;
    m_sResourceName   = sResourceName;
    m_sWebServerAddr  = sWebServerAddr;
    m_sControlAddr    = sControlAddr;
    m_sTargetTypeName = sTargetTypeName;

    //init outputs:
    *plErrors   = 0;
    *plWarnings = 0;

    hr = pIUnkFront->QueryInterface(IID_ICG_IECFront, (void**)&m_pIFront);
    if(FAILED(hr))
    {
        assert(!"QueryInterface(IID_ICG_IECFront");
        return E_FAIL;
    }

    initErrMsgSink();
    
    m_sVisPath << m_sTargetInstName << VIS_DIR_EXT _T("\\");

    m_LoaderList.init(m_sVisPath.psz());

    m_sVisPath.prepend(m_pIFront->FCPgetGenPath(), _T("\\"));

    genVisFiles();

    //set front to NULL, this way we get the sum of newly created diags, not also 
    //the previously submitted error counters:
    m_dm.setFront(NULL);

    *plErrors   = m_dm.getDisplayedErrors();
    *plWarnings = m_dm.getDisplayedWarnings();
    
    m_pIFront->Release();

    return *plErrors? E_FAIL: S_OK;
}

HRESULT STDMETHODCALLTYPE CG_CConsole::genConsoleForLibrary(
    IUnknown*  pIUnkFront,           
    BSTR       sTargetTypeName,
    long*      plErrors,
    long*      plWarnings
)
{
    HRESULT hr;
    
    m_sTargetInstName = NULL;
    m_sResourceName   = NULL;
    m_sWebServerAddr  = NULL;
    m_sControlAddr    = NULL;
    m_sTargetTypeName = sTargetTypeName;

    //init outputs:
    *plErrors   = 0;
    *plWarnings = 0;

    hr = pIUnkFront->QueryInterface(IID_ICG_IECFront, (void**)&m_pIFront);
    if(FAILED(hr))
    {
        assert(!"QueryInterface(IID_ICG_IECFront");
        return E_FAIL;
    }

    initErrMsgSink();

    const CG_Symtab*           pSymTab;
    const CG_POU4CPInfo*const* ppInf;
    bool                       bEntryHasCustomAsp;
    bool                       bIsEntrySFC;
    const TCHAR*               pszAspFileName;
    FC_CString                 sOutFile(MAX_PATH);
    FC_CString                 sOutPath(MAX_PATH);
    FC_CString                 sImportedFile(MAX_PATH);
    const TCHAR*const*         pzpszSecs;


    sOutPath.load(m_pIFront->FCPgetPrjPath()) << _T("\\gen\\.") << m_sTargetTypeName << _T("\\.link\\");

    for (ppInf = m_pIFront->FCPgetAllPOUs(); *ppInf; ppInf++)
    {
        pSymTab = m_pIFront->loadHeader((*ppInf)->pszId);

        if (pSymTab->pLibCtx || pSymTab->pouTyp == CG_POU_PRG)  // ignore library POUs and prgs
            continue; 
        
        sOutFile.load(pSymTab->edp.pszFile) << _T(".link");

        if (FC_FileExists(sOutPath, sOutFile) != 1)             // do not append something if the file does not exist
            continue;
        
        bIsEntrySFC = pSymTab->srcTyp == CG_SRC_TYPE_4GR && 
                      (pSymTab->flags & CG_STAB_BDY_IS_SFC) && 
                      !pSymTab->pLibCtx;

        bEntryHasCustomAsp = (checkOutAspFiles(pSymTab, &pszAspFileName) == S_OK);
        if (bEntryHasCustomAsp)
        {
            sImportedFile.load(m_pIFront->FCPgetPrjPath()) << _T("\\") 
                << FC_StringGetFilePart(pszAspFileName);
        }
        else if (bIsEntrySFC)
        {
            if (FAILED(gen4grvFile(pSymTab->edp.pszFile)))
                continue;
            sImportedFile.load(m_pIFront->FCPgetPrjPath()) << _T("\\gen\\.") << m_sTargetTypeName 
                << _T("\\.classes\\") << pSymTab->edp.pszFile;
            sImportedFile.changeFileExt("4grv");
        }

        if (bEntryHasCustomAsp || bIsEntrySFC)
        {
            CGT_CDmp  dmp;

            dmp << CG_LINK_LIB_PUT _T(";") << sImportedFile << _T("\n");

            if (!CGT_DmpToFile(&m_dm, sOutPath, sOutFile, dmp, true))
            {
                return E_FAIL;
            }
        }
    }

    addCustomFilesToConsoleLinkFile(m_pIFront->FCPgetSection(CG_SEC_4CP_CONSOLE));

    for(pzpszSecs = m_pIFront->FCPgetSectionNames(); *pzpszSecs; pzpszSecs++)
    {
        if (FC_StringStartsWithI(*pzpszSecs, CG_SEC_4CP_CONSOLE_BEG) >= 0)
        {
            const CG_AttribList* al = m_pIFront->FCPgetSection(*pzpszSecs);
            if (al && al->nAttribs)
                m_dm.msg1(CG_W_TEXT, &(al->pairs[0].edpKey), 
                    m_dm.jot1() << _T("ignoring section '") << *pzpszSecs 
                    << _T("'; no resource associated while creating library") );
        }
    }
        
    //set front to NULL, this way we get the sum of newly created diags, not also 
    //the previously submitted error counters:
    m_dm.setFront(NULL);

    *plErrors   = m_dm.getDisplayedErrors();
    *plWarnings = m_dm.getDisplayedWarnings();
    
    m_pIFront->Release();

    return *plErrors? E_FAIL: S_OK;
}


HRESULT CG_CConsole::initErrMsgSink()
{
    FC_CString jot(MAX_PATH);

    //init err msg sink:
    m_dm.setFront(m_pIFront);
    if(m_sTargetTypeName)
        jot.load(m_sTargetTypeName);
    m_dm.setNotSupportedName(jot);

    return S_OK;
}

HRESULT CG_CConsole::genVisFiles()
{
    //example  dump for the children.txt file:
    //#<Level>;<Type>;<Properties>;<Name>[;[<ClassName>][;[Server][;[<ASP File>][;[<Config File>][;[<Child Objects File>]]]]]]
    //2;8;0;Station_M1;RESOURCE;;CP_TaskList.asp;config/tasks.txt
    //3;16;0;T_500ms;TASK;;CP_Task.asp;config/tasks.txt
    //4;32;0;P_500ms_1
    //5;64;0;mean;MEAN;;config/lib/FC_Control.4cl/mean/mean.asp;config/cp_conf4.txt
    
    FC_CString childrenFileName, 
               taskListFileName;

    FC_CString jot(MAX_PATH);
    FC_CString consoleName;

    childrenFileName << m_sVisPath << VIS_CHILD_OBJ_FILE;
    taskListFileName << m_sVisPath << VIS_TASK_FILE;
    
    const CG_POU4CPInfo* pCON = m_pIFront->FCPgetCONinfo(NULL);
    assert(pCON);
    if(!pCON)
        return E_FAIL;

    ICG_Config* pICon = m_pIFront->loadConfig(pCON);
    assert(pICon);
    if(!pICon)
        return E_FAIL;

    m_LoaderList.addFile(VIS_CHILD_OBJ_FILE);
    m_LoaderList.addFile(VIS_TASK_FILE);
    consoleName.load(pICon->getName());

    genTargets();

    const CG_ResImpInfo* pResImp = pICon->getResImpByName(jot.load(m_sResourceName));

    const CG_Resource* pRes = m_pIFront->loadResource(pResImp->pszImpFile);
    FC_RELEASE_PTR(pICon);
    assert(pRes);
    if(!pRes)
        return E_FAIL;

    //dump the header
    m_dmpChildren << _T("#<Level>;<Type>;<Properties>;<Name>[;[<ClassName>][;[Server]")
                     _T("[;[<ASP File>][;[<Config File>][;[<Child Objects File>]]]]]]\n");

    m_dmpChildren << _T("2;8;0;") << jot <<
                     _T(";RESOURCE;;CP_Resource.asp;") CONSOLE_CONFIG_DIR _T("\\") VIS_RESOURCE_FILE _T("\n")
                     _T("3;128;0;Task List;;;CP_TaskList.asp;") CONSOLE_CONFIG_DIR _T("\\") VIS_TASK_FILE _T("\n")
                     _T("3;128;0;Message History;;;CP_MsgHistory.asp\n");
 
    for (int jj = 0; jj < pRes->nTasks; jj++)
             genTaskFiles(&pRes->pTasks[jj]);

    FC_CGArray<const CG_SymEntry*> instancePath(20);

    for(jj=0; jj<pRes->pGVL->nGVLImp; jj++)
    {
        genConfiFiles(2, pRes->pGVL->pGVLImp[jj].pSym, &instancePath);
    }

    parseConsoleSection();
    parseConsoleSection(m_sResourceName);

    if(!CGT_DmpToFile(&m_dm, NULL, childrenFileName, m_dmpChildren, false))
        return E_FAIL;

    if(!CGT_DmpToFile(&m_dm, NULL, taskListFileName, m_dmpTaskList, false))
        return E_FAIL;

    checkOutLibCommonFiles();

    FC_CString sLoaderListHeader;

    sLoaderListHeader << _T("console;") << jot.load(m_sWebServerAddr) << _T("\n")
                         _T("project;") << m_pIFront->FCPgetPrjName() << _T("\n")
                         _T("version;2.0\n")
                         _T("config;") << consoleName << _T("\n")
                         _T("resource;") << m_sResourceName << _T("\n")
                         _T("targettype;4cconsole\n");

    m_LoaderList.dump(&m_dm, m_sVisPath, sLoaderListHeader);

    return S_OK;
}

HRESULT CG_CConsole::genTaskFiles(const CG_Task* pTask)
{
    FC_CString                     jot(MAX_PATH);
    FC_CString                     jot2(MAX_PATH);  
    
    FC_CGArray<const CG_SymEntry*> instancePath(50);

    bool isIoSync = (m_pIFront->lookUpTaskInit(pTask, CG_TASK_IOSYNC) != NULL);

    m_dmpTaskList << pTask->pszId << _T("\n")
                     _T(" CLASS;") << (isIoSync?_T("RTCE TASK\n"):_T("TASK\n")) <<
                     _T(" PATH;[") << jot.load(m_sControlAddr) << _T("].") << jot2.load(m_sResourceName) << _T("\n")
                     _T(" OUT;__state;DINT;r\n")
                     _T(" OUT;__prio;DINT;r\n")
                     _T(" OUT;__cycle;DINT;r\n")
                     _T(" OUT;__etmax;DINT;r\n")
                     _T(" OUT;__errno;DINT;r\n");

    m_dmpChildren << _T("3;16;0;") << pTask->pszId << _T(';') <<
                     (isIoSync?_T("RTCE TASK\n"):
                               _T("TASK;;CP_Task.asp;") CONSOLE_CONFIG_DIR _T("\\") VIS_TASK_FILE _T("\n"));
    
    for (int jj = 0; jj < pTask->nPrgs; jj++)
    {
        const CG_SymEntry* pPrg = pTask->pPrgs[jj];

        genConfiFiles(4, pPrg, &instancePath, isIoSync);
    }

    return S_OK;
}

HRESULT CG_CConsole::genConfiFiles(int level, const CG_SymEntry* pSymEntry, 
                                   FC_CGArray<const CG_SymEntry*>* pInstancePath, bool bNoAsp)
{
    CGT_CDmp*  pDmpConfiFile = NULL;
    FC_CString sConfiFileName;

    HRESULT hr = genSymEntry(level, pSymEntry, pDmpConfiFile, sConfiFileName, pInstancePath, bNoAsp);

    if (pDmpConfiFile)
    {
        if (!CGT_DmpToFile(&m_dm, m_sVisPath, sConfiFileName, *pDmpConfiFile, false))
        {
            delete pDmpConfiFile;
            return E_FAIL;
        }
        delete pDmpConfiFile;
    }

    return hr;
}


HRESULT CG_CConsole::genConfiFiles(int level, const CG_Symtab* pSymTab, 
                                   FC_CGArray<const CG_SymEntry*>* pInstancePath, bool bNoAsp)
{
    CGT_CDmp*  pDmpConfiFile = NULL;
    FC_CString sConfiFileName;

    for (const CG_SymEntry* pEntry = pSymTab->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        switch (pEntry->declScope)
        {
        case CG_VAR_EXTERNAL:
        case CG_VAR_IN_OUT:
        case CG_CONST:
            continue;
        }

        if (pEntry->pType->pSym) // ignore simple types
        {
            assert(pEntry->pType->type == CG_INSTANCE || pEntry->pType->type == CG_STRUCT); 
            genSymEntry(level+1, pEntry, pDmpConfiFile, sConfiFileName, pInstancePath, bNoAsp); 
        }
    }

    if (pDmpConfiFile)
    {
        if(!CGT_DmpToFile(&m_dm, m_sVisPath, sConfiFileName, *pDmpConfiFile, false))
        {
            delete pDmpConfiFile;
            return E_FAIL;
        }
        delete pDmpConfiFile;
    }

    return S_OK;
}


HRESULT CG_CConsole::genSymEntry(int level, const CG_SymEntry* pSymEntry,  
                                 CGT_CDmp*& pDmpConfiFile, FC_CString& sConfiFileName,
                                 FC_CGArray<const CG_SymEntry*>* pInstancePath, bool bNoAsp)
{
    if (!pSymEntry || pSymEntry->pType->bIsArray) // ignore arrays
        return S_OK;
    
    const CG_Symtab* const& pSymTab       = pSymEntry->pType->pSym;
    const CG_SymEntry*      pSymEntryTemp = NULL;

    switch (pSymEntry->declScope) 
    {
    case CG_VAR_EXTERNAL:
    case CG_VAR_IN_OUT:
    case CG_CONST:
        return S_OK;
    case CG_VAR:    
    case CG_VAR_GLOBAL:
    case CG_VAR_INPUT:
    case CG_VAR_OUTPUT:
        if (!pSymTab)      // pSymEntry is a simple type
        {
            assert(!"Call this function only for complex types");
            return S_OK;
        }

        pInstancePath->push(pSymEntry);

        const TCHAR*       pszAspFileName        = NULL;
        FC_CString         jot(MAX_PATH);
        bool               bEntryHasCustomAsp    = false;
        bool               bIsEntrySFC           = pSymTab->srcTyp == CG_SRC_TYPE_4GR && 
                                                   (pSymTab->flags & CG_STAB_BDY_IS_SFC) && 
                                                   !pSymTab->pLibCtx;
        const CG_SymEntry* pEntry;
        CGT_CDmp*          pDmpChildrenConfiFile = NULL; 
        FC_CString         sChildrenConfiFileName;

        if (!pDmpConfiFile && (pSymTab->pouTyp != CG_POU_PRG || bIsEntrySFC))
        {
            pDmpConfiFile  = new CGT_CDmp;
            sConfiFileName << TASK_CONFIG_FILE << m_lConfiFilesCount++
                           << TASK_CONFIG_EXT;  // only the name not absolute path

            m_LoaderList.addFile(sConfiFileName);
        }

        m_dmpChildren << level;

        if (pSymTab->pouTyp == CG_POU_PRG)
            m_dmpChildren << _T(";32;0;");
        else
            m_dmpChildren << _T(";64;0;");

        m_dmpChildren << pSymEntry->pszId << _T(';') << pSymEntry->pType->pszName;      

        if (pSymTab->pouTyp != CG_POU_PRG || bIsEntrySFC)
        {
            *pDmpConfiFile << pSymEntry->pszId << _T("\n")
                              _T(" CLASS;") << pSymEntry->pType->pszName << _T("\n")
                              _T(" PATH;[") << m_sControlAddr  << _T("].") << m_sResourceName;

            for (int i = 0; i < pInstancePath->getUsed()-1; i++)
                *pDmpConfiFile << _T(".") << pInstancePath->get(i)->pszId;

            *pDmpConfiFile << _T('\n');
        }

        if (!bNoAsp)
        {
            bEntryHasCustomAsp = (checkOutAspFiles(pSymTab, &pszAspFileName) == S_OK);
            if (pszAspFileName)
            {
                assert(pszAspFileName);
                m_dmpChildren << _T(";;") << pszAspFileName << _T(";") CONSOLE_CONFIG_DIR _T("\\") 
                              << sConfiFileName;
            }
        }
        m_dmpChildren << _T('\n');

        switch(pSymTab->pouTyp)
        {
        case CG_POU_FB:
        case CG_POU_FUN:
        case CG_POU_PRG:
            if (!bEntryHasCustomAsp && bIsEntrySFC)
            {
                if (SUCCEEDED(gen4grvFile(pSymTab->edp.pszFile)))
                {
                    jot.load(FC_StringGetFilePart(pSymTab->edp.pszFile));
                    jot.changeFileExt("4grv");
                    m_LoaderList.addFile(jot);
                    *pDmpConfiFile << _T(" 4GRV;") CONSOLE_CONFIG_DIR _T("\\") << jot << _T("\n");
                }
                else 
                    return E_FAIL;
            }
            break;
        }

        for(pEntry=pSymTab->pFirst; pEntry; pEntry=pEntry->pNext) 
        {
            switch (pEntry->declScope)
            {
            case CG_VAR_EXTERNAL:
            case CG_VAR_IN_OUT:
            case CG_CONST:
                continue;
            }

            if (pEntry->pType->pSym)
            {
                assert(pEntry->pType->type == CG_INSTANCE || pEntry->pType->type == CG_STRUCT); 
                if (FAILED(genSymEntry(level+1, pEntry, pDmpChildrenConfiFile, sChildrenConfiFileName, 
                                       pInstancePath, bNoAsp)))
                {
                    if (pDmpChildrenConfiFile) 
                        delete pDmpChildrenConfiFile;
                    return E_FAIL;
                }
            }
            else  // simple types generate here don't make a new recursion
            {
                if (pEntry->pParent->pouTyp != CG_POU_PRG &&    // ingnore local program simple variables
                    !pEntry->pType->bIsArray)                   // and arrays
                {                                                                   
                    if (pEntry->declScope == CG_VAR_GLOBAL)     // ingnore simple type globals
                        continue;

                    pInstancePath->push(pEntry);

                    if (pEntry->declScope == CG_VAR_OUTPUT)
                    {
                        *pDmpConfiFile << _T(" OUT;") << pEntry->pszId << _T(';') 
                                       << pEntry->pType->pszName << _T(";r\n");
                    }
                    else 
                    {
                        ICG_ExtBodyInfo* pIExtBodyInfo;
                        boolean          bIsMemberReadOnly = false;

                        for (int i=pInstancePath->getUsed()-2/*exclude simple type*/; i >= 0; i--)
                        {
                            pSymEntryTemp = pInstancePath->get(i);
                            assert(pSymEntryTemp->pType->pSym); // should be nonsimple 

                            if (pSymEntryTemp->pType->pSym->pLibCtx) // don't search for references in libaries
                                continue;
                            if (!CG_HasPOUTypBody(pSymEntryTemp->pType->pSym->pouTyp))
                                continue;

                            pIExtBodyInfo = m_pIFront->loadExtBodyInfo(pSymEntryTemp->pType->pSym);
                            assert(pIExtBodyInfo);

                            bIsMemberReadOnly = pIExtBodyInfo->getSimpleMemberWriteInfo(pInstancePath->getUsed()-i-1, 
                                                    pInstancePath->getCElements()+i+1);
                            pIExtBodyInfo->Release();
                            if (bIsMemberReadOnly)
                                break;
                        }

                        *pDmpConfiFile << _T(" IN;") << pEntry->pszId << _T(';') 
                                       << pEntry->pType->pszName;                       
                        if (bIsMemberReadOnly)
                            *pDmpConfiFile << _T(";r\n");
                        else 
                            *pDmpConfiFile << _T(";rw\n");
                    }

                    pInstancePath->pop(pSymEntryTemp);
                }
            }
        }

        if (bIsEntrySFC)
        {
            *pDmpConfiFile << _T(" OUT;__sfcvis;WSTRING;rh\n")
                              _T(" IN;__manualmode;BOOL;rh\n")
                              _T(" IN;__donext;BOOL;rh\n")
                              _T(" OUT;__onceforcedtransitions;WSTRING;rh\n")
                              _T(" IN;__forcedtransitions;WSTRING;rh\n")
                              _T(" IN;__blockedtransitions;WSTRING;rh\n")
                              _T(" IN;__alltransitionson;BOOL;rh\n")
                              _T(" IN;__onceforcedactions;WSTRING;rh\n")
                              _T(" IN;__forcedactions;WSTRING;rh\n")
                              _T(" IN;__blockedactions;WSTRING;rh\n")
                              _T(" IN;__allactionsoff;BOOL;rh\n")
                              _T(" IN;__mancmd;WSTRING;rh\n");
        }

        if (pDmpChildrenConfiFile)
        {
            if(!CGT_DmpToFile(&m_dm, m_sVisPath, sChildrenConfiFileName, *pDmpChildrenConfiFile, false))
            {
                delete pDmpChildrenConfiFile;
                return E_FAIL;
            }
            delete pDmpChildrenConfiFile;
        }
    }

    pInstancePath->pop(pSymEntryTemp);   
    return S_OK;
}

HRESULT CG_CConsole::checkOutAspFiles(const CG_Symtab* pSymTab, const TCHAR** pMainAsp)
{
    FC_CString jot(MAX_PATH);
    long       idx;

    if(m_hSymAsp.get(pSymTab, &idx))  // file already checked out, return asp file name
    {
        if (idx < 0)
        {
            *pMainAsp = NULL;
            return S_FALSE;
        }
        else if (idx <= 1)
        {
            *pMainAsp = m_aAspFiles.get(idx);
            return S_FALSE;
        }
        else 
        {
            *pMainAsp = m_aAspFiles.get(idx);
            return S_OK;
        }
    }

    jot.load(pSymTab->edp.pszFile);
    jot.changeFileExt(_T("asp"));

    const CG_LIBContext* pLibCtx = NULL;

    if(CGT_ExistsLocatorFile(&m_dm, m_pIFront, jot, &pLibCtx))
    {
        if (pLibCtx) // file is in library
        {
            CGT_FileLocator splitted;

            int iRet= CGT_SplitFileLocator(jot, &splitted);
            assert(iRet > 0);

            checkOutLibFiles(pLibCtx, splitted.pszPOUName, splitted.pszLibFile);
            jot.load(pLibCtx->pszId) << _T("\\") << splitted.pszLibFile;
        }
        else  // file is a custom asp file in project directory
        {
            m_LoaderList.addFile(jot, FILE_PRJ_DIR_RELATIVE);
        }
        jot.prepend(_T("config\\"));
        idx = m_aAspFiles.add(jot);
    }
    else if (pSymTab->srcTyp == CG_SRC_TYPE_4GR && (pSymTab->flags & CG_STAB_BDY_IS_SFC) && !pSymTab->pLibCtx)
    {
        *pMainAsp = m_aAspFiles.get(DEFAULT_ASP_SFC_POS);  // return default value "CP_SFC.asp"(1);
        m_hSymAsp.insert(pSymTab, DEFAULT_ASP_SFC_POS);
        return S_FALSE;     
    }
    else if (pSymTab->pouTyp != CG_POU_PRG)
    {
        *pMainAsp = m_aAspFiles.get(DEFAULT_ASP_TUNINGDISPLAY_POS);  // return default value "CP_TuningDisplay.asp"(0);
        m_hSymAsp.insert(pSymTab, DEFAULT_ASP_TUNINGDISPLAY_POS);
        return S_FALSE;
    }
    else         // no asp, even default
    {
        *pMainAsp = NULL;
        m_hSymAsp.insert(pSymTab, -1);  // return null
        return S_FALSE;
    }

    *pMainAsp = m_aAspFiles.get(idx);
    m_hSymAsp.insert(pSymTab, idx);

    return S_OK;
}

HRESULT CG_CConsole::checkOutLibCommonFiles()
{
    const FC_CHashPtr<const CG_Symtab*, long>::Pair* p;
    FC_CGArray<const CG_LIBContext*>                 aLibCtx(128);

    for (p = m_hSymAsp.firstPair(); p; p = p->pNext)
    {
        if (p->pKey->pLibCtx && aLibCtx.findElem(p->pKey->pLibCtx) == -1)
            aLibCtx.add(p->pKey->pLibCtx);
    }

    for (int i = 0; i < aLibCtx.getUsed(); i++)
    {
        if (FAILED(checkOutLibFiles(aLibCtx.get(i), CG_LIB_POU_CONSOLE, _T("*"))))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CG_CConsole::checkOutLibFiles(const CG_LIBContext* pLibCtx, const TCHAR* sPOUName, const TCHAR* sFileSpec)
{
    FC_CString  sCheckOutDir;
    FC_CString  sLibFldr;
    FC_CString  jot;
    
    I4CLAccess* pILib = m_pIFront->get4CLLibIF(pLibCtx);
    if(!pILib)
    {
        CGT_IERR2(&m_dm, &pLibCtx->edp, m_dm.jot1()
            << _T("CGT_LIBCheckOutFiles: failed to get library interface by context info '")
            << pLibCtx->pszId << _T("'"));
        return E_FAIL;
    }

    sLibFldr.load(pLibCtx->pszId);

    sCheckOutDir.load(m_sVisPath) << sLibFldr; //check out directory

    SAFEARRAY* parrFiles;
    BSTR       sERR  = NULL;
    BSTR       sPOU  = FC_StringToBSTR(sPOUName); 
    BSTR       sFILE = FC_StringToBSTR(sFileSpec);
    BSTR       sDIR  = FC_StringToBSTR(sCheckOutDir);   

    HRESULT hr = pILib->CheckOut(sPOU, m_sTargetTypeName, sFILE, sDIR, &parrFiles, &sERR);

    if(SUCCEEDED(hr))
    {
        if(hr==S_OK)
        {
            sCheckOutDir.load(m_sTargetInstName) << VIS_DIR_EXT _T("\\") << sLibFldr; 

            BSTR jot2;
            int nFiles = parrFiles->rgsabound[0].cElements;

            for(long i = 0; i < nFiles; i++)
            {
                SafeArrayGetElement(parrFiles, &i, &jot2);
                if (_tcsicmp(jot.load(jot2), _T(".link")) != 0)   // ignore '.link' file; it has no meaning for console
                    m_LoaderList.addFile(jot.prepend(sLibFldr, _T("\\")));
            }
            SafeArrayDestroy(parrFiles);
        }
        else
        {
            assert(!parrFiles);
        }
    }
    else        
    {
        BSTR       sLIB;
        FC_CString tmp(MAX_PATH);

        tmp << sPOUName << _T("\\")
            << m_sTargetTypeName << _T("\\")
            << sFileSpec;

        pILib->GetLibFileName(&sLIB);

        m_dm.msg2(CG_E_READ_LIB_DOMAIN, &pLibCtx->edp, tmp, m_dm.jot1() << sLIB);
        m_dm.userMsg(m_dm.jot1() << _T("    ") << sERR << _T("\r\n"));
        FC_FREE_BSTR(sLIB);        
    }

    FC_FREE_BSTR(sPOU);        
    FC_FREE_BSTR(sFILE);        
    FC_FREE_BSTR(sDIR);
    FC_FREE_BSTR(sERR);
    pILib->Release();
    
    return S_OK;
}

/*
<?xml version="1.0" encoding="UTF-8"?>
<PC  CONTROL="W-EF"  CONSOLE="W-ED" RESOURCE="Resource1" >
        <FBUS LINK="Profiboard0.iodp"/>
        <FBUS LINK="myPAMS\NestedPAM.pam"/>
</PC>
*/
HRESULT CG_CConsole::genTargets()
{
    const CG_AttribList* al = m_pIFront->FCPgetSection(CG_SEC_4CP_TARGETS);
    IXMLDOMNode*         pNode;
    FC_CString           control;
    FC_CString           console;
    FC_CString           resName;
    FC_CString           jot;
    const TCHAR*         psz4CPCFile;
    CGT_CDmp             dmpObjects;
    CGT_CDmp             dmpResources;

    dmpObjects << _T("#<Level>;<Type>;<Properties>;<Name>[;[<ClassName>][;[Server][;[<ASP File>][;[<Config File>][;[<Child Objects File>]]]]]]\n")
                  _T("0;1;0;") << m_pIFront->FCPgetPrjName() << _T("\n")
                  _T("1;128;0;Target List;;;CP_ConfigurationList.asp;") CONSOLE_CONFIG_DIR _T("\\") VIS_RESOURCE_FILE _T("\n");

    for(int i = 0; i < al->nAttribs; i++)
    {
        //pick out only 4cpc targtes:
        psz4CPCFile = FC_StringGetFilePart(al->pairs[i].pszVal);
        if(_tcsicmp("4cpc", FC_StringGetFileExt(psz4CPCFile)))
            continue;

        pNode = CGT_XMLLoadFile(&m_dm, m_pIFront->FCPgetPrjPath(), al->pairs[i].pszVal, NULL);
        if(!pNode)
            return E_FAIL;

        CGT_XMLGetStrAttrTrim(&m_dm, pNode, L"CONTROL", &control.clear(), true, true); 
        CGT_XMLGetStrAttrTrim(&m_dm, pNode, L"CONSOLE", &console.clear(), true, true);
        CGT_XMLGetStrAttrTrim(&m_dm, pNode, L"RESOURCE", &resName.clear(), true, true);

        //if CONSOLE attrib. is missing or empty console is disabled by user:
        //if resource or control is empty also continue
        if(console.isEmpty() || resName.isEmpty() || control.isEmpty())
            continue; 

        //get target instance name (simply the file name):
        jot.load(psz4CPCFile).changeFileExt(NULL);
        //for better feedback add the control address:
        jot<<_T(" {")<<control<<_T("}");
        dmpObjects   << _T("1;2;0;") << jot << _T(";;") << console << _T(";;;") CONSOLE_CONFIG_DIR _T("\\") VIS_CHILD_OBJ_FILE _T("\n");

        dmpResources << resName << _T("\n")
                        _T(" CLASS;RESOURCE\n")
                        _T(" PATH;[") << control << _T("]\n")
                        _T("  OUT;__state;DINT;r\n")
                        _T("  OUT;__memused;DINT;r\n")
                        _T("  OUT;__projectname;WSTRING;r\n");
    }

    if(!CGT_DmpToFile(&m_dm, NULL, jot.clear() << m_sVisPath << VIS_OBJECTS_FILE, dmpObjects, false))
        return E_FAIL;
    m_LoaderList.addFile(VIS_OBJECTS_FILE);

    if(!CGT_DmpToFile(&m_dm, NULL, jot.clear() << m_sVisPath << VIS_RESOURCE_FILE, dmpResources, false))
        return E_FAIL;
    m_LoaderList.addFile(VIS_RESOURCE_FILE);

    return S_OK;
}

HRESULT CG_CConsole::parseConsoleSection(BSTR sResourceName)
{   
    const CG_AttribList* al;
    FC_CString           jot10;

    if (!sResourceName)
        jot10.load(CG_SEC_4CP_CONSOLE);
    else
        jot10.load(CG_SEC_4CP_CONSOLE_BEG) << m_sResourceName;

    jot10.toLower();
    al = m_pIFront->FCPgetSection(jot10);

    if (!al)
        return S_OK;

    const CG_KeyValPair* pair;
    TCHAR**              files;
    const TCHAR*         prjPath    = m_pIFront->FCPgetPrjPath();
    
    for (int i = 0; i < al->nAttribs; i++)
    {
        pair = &al->pairs[i];
        if (!_tcsicmp(pair->pszKey, _T("file")))
        { 
            if(FC_StringIsAbsPath(pair->pszVal))
            {
                m_dm.msg1(CG_E_TEXT, &pair->edpVal, 
                    m_dm.jot1()<<_T("section [console]: file='")
                      <<pair->pszVal<<_T("' file must be project relative"));

            }
            if (_tcspbrk(pair->pszVal, _T("*?")) == NULL) // no wildchar
            {
                if (FC_FileExists(prjPath, pair->pszVal) != 1)
                    m_dm.msg1(CG_E_FILE_NOT_EXIST, &pair->edpVal, pair->pszVal);
                else 
                    if (!m_LoaderList.addFile(pair->pszVal, FILE_PRJ_DIR_RELATIVE))
                        m_dm.msg1(CG_W_TEXT, &pair->edpVal, 
                        m_dm.jot1()<<_T("file '")<<pair->pszVal<<_T("' already specified ignoring this one"));
            }
            else 
            {
                files = FC_FileBuildFileList(prjPath, pair->pszVal, 0);
                if (files)
                {
                    for (int j = 0; files[j] != NULL; j++)
                    {
                        const TCHAR* pszFileMatch = files[j];
                        if (!m_LoaderList.addFile(pszFileMatch, FILE_PRJ_DIR_RELATIVE))
                            m_dm.msg1(CG_W_TEXT, &pair->edpVal, 
                            m_dm.jot1()<<_T("file '")<<pszFileMatch<<_T("' already specified ignoring this one"));
                    }

                    if (files[0] == NULL)
                        m_dm.msg1(CG_W_TEXT, &pair->edpVal, 
                        m_dm.jot1()<<_T("no such file or folder name '")<<pair->pszVal<<_T("'"));

                    FC_FileDeleteFileList(files);
                }
                else 
                {
                    m_dm.msg2(CG_E_FILE_OPEN, &pair->edpVal, pair->pszVal, m_dm.jot1().fmtSysErr(GetLastError()));
                }
            }
        }
        else
            m_dm.msg2(CG_E_PRJ_UNKNOWN_KEY, &pair->edpKey, pair->pszKey, jot10);
    }
    return S_OK;
}

HRESULT CG_CConsole::gen4grvFile(const TCHAR* pszSourceFile)
{
    FC_CString   sfcPatch(MAX_PATH);
    FC_CString   cmd(1024);
    const TCHAR* pszPrjPath = m_pIFront->FCPgetPrjPath();
    TCHAR        fullPath[MAX_PATH];

    cmd << _T("\"") << pszPrjPath << _T("\" ");

    //full path to 4gr source:
    FC_FileFullPath(pszPrjPath, pszSourceFile, fullPath, FC_ARRAY_LEN(fullPath));
    cmd << _T("\"") << fullPath << _T("\" ");

    //full path dbi file:
    cmd << _T("\"") << pszPrjPath << _T("\\gen\\.") << m_sTargetTypeName << _T("\\.dbi\\") 
        << FC_StringGetFilePart(pszSourceFile);
    cmd.changeFileExt(_T("dbi")) << _T("\" ");
    
    //full path to output 4grv file:
    cmd <<_T("\"") << pszPrjPath << _T("\\gen\\"); 
    if (m_sTargetInstName)
        cmd << m_sTargetInstName << VIS_DIR_EXT _T("\\");
    else 
        cmd << _T(".") << m_sTargetTypeName << _T("\\.classes\\");
    cmd << FC_StringGetFilePart(pszSourceFile);
    cmd.changeFileExt(_T("4grv")) << _T("\" ");
    
    cmd << _T("-f");

    sfcPatch.load(g_szModulePath) << _T("\\SFCPatch");

    if (!FC_System(sfcPatch, cmd, NULL, NULL))
    {
        m_dm.sysErr(GetLastError(), NULL, NULL, NULL);
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CG_CConsole::addCustomFilesToConsoleLinkFile(const CG_AttribList* al)
{   
    if (!al)
        return S_OK;

    const CG_KeyValPair* pair;
    TCHAR**              files;
    const TCHAR*         prjPath = m_pIFront->FCPgetPrjPath();
    CGT_CDmp             dmp;
    FC_CString           sOutPath;

    sOutPath.load(m_pIFront->FCPgetPrjPath()) << _T("\\gen\\.") << m_sTargetTypeName << _T("\\.link\\");
    

    long lNrChars = dmp.getNChars();

    for (int i = 0; i < al->nAttribs; i++)
    {
        pair = &al->pairs[i];
        if (!_tcsicmp(pair->pszKey, _T("file")))
        { 
            if(FC_StringIsAbsPath(pair->pszVal))
            {
                m_dm.msg1(CG_E_TEXT, &pair->edpVal, 
                    m_dm.jot1()<<_T("section [console]: file='")
                      <<pair->pszVal<<_T("' file must be project relative"));

            }

            if (_tcspbrk(pair->pszVal, _T("\\")) != NULL) // file not in project directory
            {
                m_dm.msg1(CG_W_TEXT, &pair->edpVal, 
                m_dm.jot1()<<_T("ignoring file(s) '")<<pair->pszVal<<_T("' because subdirectories are not supported when creating library"));       
                continue;
            }

            if (_tcspbrk(pair->pszVal, _T("*?")) == NULL) // no wildchar
            {
                if (FC_FileExists(prjPath, pair->pszVal) != 1)
                    m_dm.msg1(CG_E_FILE_NOT_EXIST, &pair->edpVal, pair->pszVal);
                else 
                    dmp << CG_LINK_LIB_PUT _T(";") << prjPath << _T("\\") << pair->pszVal << _T("\n");
            }
            else 
            {
                files = FC_FileBuildFileList(prjPath, pair->pszVal, 0);
                if (files)
                {
                    for (int j = 0; files[j] != NULL; j++)
                    {
                        const TCHAR* pszFileMatch = files[j];
                        dmp << CG_LINK_LIB_PUT _T(";") << prjPath << _T("\\") << pszFileMatch << _T("\n");
                    }

                    if (files[0] == NULL)
                        m_dm.msg1(CG_W_TEXT, &pair->edpVal, 
                        m_dm.jot1()<<_T("no such file or folder name '")<<pair->pszVal<<_T("'"));

                    FC_FileDeleteFileList(files);
                }
                else 
                {
                    m_dm.msg2(CG_E_FILE_OPEN, &pair->edpVal, pair->pszVal, m_dm.jot1().fmtSysErr(GetLastError()));
                }
            }
        }
        else 
            m_dm.msg2(CG_E_PRJ_UNKNOWN_KEY, &pair->edpKey, pair->pszKey, CG_SEC_4CP_CONSOLE);
    }

    if (lNrChars != dmp.getNChars())
        if (!CGT_DmpToFile(&m_dm, sOutPath, _T("__console.link"), dmp, false))
            return E_FAIL;

    return S_OK;
}

//===========================================================================================

//DLL Main is needed to get the hinstDLL:
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,   // handle to the DLL module
    DWORD     fdwReason,  // reason for calling function
    LPVOID    lpvReserved // reserved
)
{
    if(fdwReason==DLL_PROCESS_ATTACH)
    {
        g_hinstDLL = hinstDLL;
    }
    return TRUE;
}

//Standard COM DLL Exports:
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) 
{
    *ppObject = NULL;

    if(rclsid==CLSID_CG_Console) 
    {
        COM_CConsoleFactory* pFactory = new COM_CConsoleFactory;
        if(FAILED(pFactory->QueryInterface(riid, ppObject))) 
        {
            delete pFactory;
            *ppObject = NULL;
            return E_INVALIDARG;
        }
    }
    else 
    {   // here you could check for additional CLSID's you DLL may provide
        return CLASS_E_CLASSNOTAVAILABLE;
    }
    return NO_ERROR;
}

STDAPI DllCanUnloadNow() 
{
   return g_dwRefCount!=0 ? S_FALSE : S_OK ;
}

STDAPI DllRegisterServer(void) 
{
    LONG         lret =ERROR_SUCCESS;
    const TCHAR* pszClass = _T("4CONTROL.CG_Consolet10");
    TCHAR        szModule[MAX_PATH+1];
    FC_CString   jot;

    if(GetModuleFileName(g_hinstDLL, szModule, FC_ARRAY_LEN(szModule))==0)
    {
        FC_ASSERT_LASTERR(_T("GetModuleFileName"));
        lret = E_UNEXPECTED;
    }
    if(lret==ERROR_SUCCESS)
    {
        jot<<_T("CLSID\\")<<CLSID_CG_Console;
        lret=FC_RegSetStringVal(HKEY_CLASSES_ROOT, jot, NULL, pszClass);
    }
    if(lret==ERROR_SUCCESS)
    {
        jot<<_T("\\InprocServer32");
        lret=FC_RegSetStringVal(HKEY_CLASSES_ROOT, jot, NULL, szModule);
    }
    if(lret==ERROR_SUCCESS)
    {
        lret=FC_RegSetStringVal(HKEY_CLASSES_ROOT, jot, 
            _T("ThreadingModel"), _T("Both"));
    }

    FC_ASSERT_SYSERR(lret, _T("DllRegisterServer"));
    
    return lret==ERROR_SUCCESS ? NOERROR : E_UNEXPECTED;
}

STDAPI DllUnregisterServer(void) 
{
    LONG        lret;
    FC_CString  jot;

    jot << _T("CLSID\\") << CLSID_CG_Console;
    lret = FC_RegRecursiveDelete(HKEY_CLASSES_ROOT, jot);
    FC_ASSERT_SYSERR(lret, jot);
    return lret==ERROR_SUCCESS ? NOERROR : E_UNEXPECTED;
}

