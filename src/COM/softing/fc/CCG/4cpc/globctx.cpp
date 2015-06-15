

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include <time.h>

#include "fc_tools\fc_fileIO.h"
#include "instview.h"
#include "cgt_tools.h"
#include "cgt_misc.h"
#include "globctx.h"
#include "Parse4CH.h"
#include "helper.h"
#include "dumpjava.h"
#include "resinit.h"
#include "CG_Console.h"
#include "CG_Console_i.c"
#include "rtcegen.h"
#include "4cpc.h"



CGlobCtx::CGlobCtx(
    ICG_IECFront* pFront
) 
  : m_pIF(pFront), 
    m_dm(TARGET_EXT, NULL), 
    m_popt(&m_pCmdlOpts),
    m_resources(true, false)
{
    
    m_dm.setNotSupportedName(TARGET_EXT);
    memset(&m_licData, 0, sizeof(m_licData));

    m_pCmdlOpts.bIncrementelly = false;
    m_pCmdlOpts.bAppendLib = false;

    m_CodeFilesCreated = 0;
    m_jitmenot = false;
    m_pConfig = NULL;
    m_dm.setFront(pFront);
    m_bpopt = BP_OPT1;  //default is bp1, if build lib we change the default to bp0
    m_controlVer = 20000;//assume default old version
    m_optVerbose = 0; 
}

CGlobCtx::~CGlobCtx() 
{
    m_targets.deletePtrs();
    m_POUcode.deletePtrs();
    FC_RELEASE_PTR(m_pConfig);
}

void CGlobCtx::initLicenseInfo(const CG_LIBContext*const* pzLIBs)
{
    const CG_LIBContext*const* ppLIB;
    LIC_LIBS                   lb;
    FC_CGArray<LIC_LIBS>       liclibs;
    I4CLAccess*                pILibAcc;
    SAFEARRAY*                 parrFiles;
    BSTR                       bstrLicInf;
    BSTR                       bstrLicPOU;
    BSTR                       bstrLicTRG;

    bstrLicPOU = FC_StringToBSTR(CG_LIB_POU_LICENSE);
    bstrLicTRG = FC_StringToBSTR(TARGET_EXT);

    for(ppLIB = pzLIBs; *ppLIB; ppLIB++)
    {
        pILibAcc = m_pIF->get4CLLibIF(*ppLIB);
        assert(pILibAcc);//must work after loadLibs and 0 errors!

        parrFiles = NULL;
        pILibAcc->GetFileNames(bstrLicPOU, bstrLicTRG, &parrFiles);
        if(!parrFiles)
        {
            pILibAcc->Release();
            continue;
        }
        if(parrFiles->rgsabound[0].cElements!=1)
        {
            m_dm.msg2(CG_E_READ_LIB_DOMAIN, &(*ppLIB)->edp, _T("<license data>"), (*ppLIB)->pszId);
            pILibAcc->Release();
            SafeArrayDestroy(parrFiles);
            continue;
        }
        
        bstrLicInf = NULL;
        pILibAcc->GetFileAsBSTR(bstrLicPOU, bstrLicTRG, ((BSTR*)parrFiles->pvData)[0], &bstrLicInf);
        pILibAcc->Release();
        SafeArrayDestroy(parrFiles);

        lb.nLibProdNum = bstrLicInf ? _wtoi(bstrLicInf) : 0;
        FC_FREE_BSTR(bstrLicInf);
        
        if(lb.nLibProdNum>0)
        {
            lb.pLIBCxt = *ppLIB;
            liclibs.add(lb);
        }
        else
        {
            m_dm.msg2(CG_E_READ_LIB_DOMAIN, &(*ppLIB)->edp, _T("<license data>"), (*ppLIB)->pszId);
        }
    }

    FC_FREE_BSTR(bstrLicPOU);
    FC_FREE_BSTR(bstrLicTRG);

    SL_Get4CPCLicData(&m_dm, &m_licData, &liclibs);
}



void CGlobCtx::initPathInfo()
{
    FC_CString  jot(MAX_PATH);
    
    jot.load(m_pIF->FCPgetGenPath());
    //assert that the front has at least the 4cp loaded
    assert(!jot.isEmpty());

    jot<<_T("\\.")TARGET_EXT;
    m_TrgTypeGenPath.load(jot);

    jot<<_T("\\.classes");
    m_clsGenPath.load(jot);
    
    jot.changeFileExt(_T(".link"));
    m_lnkGenPath.load(jot);
}

void CGlobCtx::cleanGenDir()
{
    int          i;
    FC_CString   trgDir(MAX_PATH);
 
    //do not delete: "gen\.4cpc\*.*", this is done in 4cg 
    //when rebuild all, otherwise we destroy dbi files of previously 
    //build dbi files. Not deleteting here is a HACK much better is to store .dbi 
    //into gen\<target>!!
    //But we must delete all java and class files from
    //the gen\.4cpc\.classes folder, we call jvc *.java !!

    //As you can see if getXXXGenPath() is now an empty string (simply if you 
    //call this function before initPathInfo, which is a simple mistake..)
    //then you delete all files in the current working dir and below !!!
    //CGT_DeleteFiles has some heuristics to avoid such damage, so we 
    //call here call CGT_DeleteFiles and trust in the "buildin emergency break": 
    CGT_DeleteFiles(&m_dm, getClsGenPath(), _T("*"), NULL, true, NULL);
    
    //also delete and recursive in gen\.4cpc\.link
    CGT_DeleteFiles(&m_dm, getLnkGenPath(), _T("*"), NULL, true, NULL);


    //read in all target files(=directories below gen) from command line:
    for(i=0;i<m_pCmdlOpts.targets.getUsed();i++)
    {
        //delete: "gen\<target>\*.*"  
        //- recurse into sub dirs but leave those that match  "__*"
        //  directory "gen\<target>\__cedl" is reserved for the 
        //  downloader's last loaded data base.
        //- all files:
        trgDir.load(FC_StringGetFilePart(m_pCmdlOpts.targets.get(i)));
        trgDir.changeFileExt(NULL)<<_T("\\*");
        CGT_DeleteFiles(&m_dm, getGenPath(), trgDir, _T("__.list"), true, _T("__*"));

        //delete: "gen\<target>__vis\*.*"  
        //- recurse into sub dirs but leave those that match  "__*"
        //  directory "gen\<target>__vis\__cedl" is reserved for the 
        //  downloader's last loaded data base.
        //- all files:
        trgDir.load(FC_StringGetFilePart(m_pCmdlOpts.targets.get(i)));
        trgDir.changeFileExt(NULL)<<_T("__vis\\*");
        CGT_DeleteFiles(&m_dm, getGenPath(), trgDir, _T("__.list"), true, _T("__*"));
    }
}



void CGlobCtx::getOptionsFrom4CP()
{
    int                  i;
    int                  iret;
    const CG_AttribList* pAttr;
    const CG_KeyValPair* pPair;
    const CG_Edipos*     pEdpVal;
    long                 flags;
    FC_CString           valex(MAX_PATH);
    int                  ival;
    bool                 bKeyHelpDone = false;
    bool                 bOptHelpDone = false;
    bool                 bVerboseGiven = false;
    const CG_Edipos*     pEdpBPGiven = NULL;
    bool                 bJVCGenGiven = false;
    bool                 bJVCExtGiven = false;
    
    pAttr = m_pIF->FCPgetSection(CG_SEC_4CP_COMPILER_BEG TARGET_EXT);
    if(pAttr)
    {
        flags = CGT_INS_ESCAPE|CGT_INS_ENV|CGT_INS_PRINTHELP|CGT_INS_WARN_UNDEF;
        for(i=0; i<pAttr->nAttribs; i++)
        {
            pPair  = &pAttr->pairs[i];
            pEdpVal= &pPair->edpVal;

            iret = CGT_SubsStdDollarIns(&m_dm, m_pIF, pPair->pszVal, pEdpVal, flags, &valex);
            if(iret==0)
                return;
            if(iret<0)
                flags &= ~CGT_INS_PRINTHELP;

            if(!_tcsicmp(pPair->pszKey, _T("opt")))
            {
                if((iret = CGT_CheckNumOption(&m_dm, pEdpVal, valex, _T("bp"), 2, &ival))!=0)
                {
                    if(iret>0)
                    {
                        if(pEdpBPGiven)
                        {
                            m_dm.msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), _T("bp"));
                        }
                        else
                        {
                            if(ival==0)
                                m_bpopt = BP_OPT0;
                            else if(ival==1)
                                m_bpopt = BP_OPT1;
                            else if(ival==2)
                                m_bpopt = BP_OPT2;

                            pEdpBPGiven = pEdpVal;
                        }
                    }
                }
                else if((iret = CGT_CheckNumOption(&m_dm, pEdpVal, valex, _T("verbose:"), 2, &ival))!=0)
                {
                    if(iret>0)
                    {
                        if(bVerboseGiven)
                            m_dm.msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), _T("verbose:"));
                        else
                            m_optVerbose = ival;
                        bVerboseGiven = true;
                    }
                }
                else if(valex.stripBeginI(_T("jvc_gen:")))
                {
                    valex.trim();
                    if(bJVCGenGiven)
                        m_dm.msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), _T("jvc_gen:"));
                    else
                        m_optJVC_GEN << valex;
                    bJVCGenGiven = true;
                }
                else if(valex.stripBeginI(_T("jvc_ext:")))
                {
                    valex.trim();
                    if(bJVCExtGiven)
                        m_dm.msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), _T("jvc_ext:"));
                    else
                        m_optJVC_EXT << valex;
                    bJVCExtGiven = true;
                }
                else if(valex.stripBeginI(_T("version:")))
                {
                    m_dm.msg2(CG_E_OLD_SYNTAX, pEdpVal, 
                    m_dm.jot1()<<_T("opt=version:")<<valex,
                       _T("set version in target property dialog"));
                }
                else
                {
                    m_dm.msg1(CG_E_CMDL_UNKNOWN_OPT, pEdpVal, valex);
                    if(!bOptHelpDone)
                    {
                        bOptHelpDone = true;
                        m_dm.userMsg(_T("   known 'opt=' are: ")_T("bp(0|1|2), verbose:(0|1|2), jvc_(gen|ext):<options>, version:<num>") _T("\n")); 
                    }
                }
            }
            else if(!_tcsicmp(pPair->pszKey, _T("java")))
            {
                if(!valex.isEmpty())
                {
                    m_extJavaFilesEdp.add(pPair->edpVal);
                    m_extJavaFiles.add(valex);
                }
            }
            else if(!_tcsicmp(pPair->pszKey, _T("bin")) || !_tcsicmp(pPair->pszKey, _T("data")))
            {
                if(!valex.isEmpty())
                {
                    FC_CString   dstFileSpec;
                    const TCHAR* pszDest  = FC_StringCGetAfterLast(valex, _T(">"));
                    if(pszDest)
                    {
                        dstFileSpec.load(pszDest).trim();
                        valex.cutAtLast(_T(">"));
                        valex.trim();
                        
                        pszDest = FC_StringGetFilePart(dstFileSpec);
                        if(!_tcscmp(pszDest, _T("*")) || !_tcscmp(pszDest, FC_StringGetFilePart(valex)))
                        {
                            dstFileSpec.stripFilePart();
                        }
                        else
                        {
                            m_dm.errNS1(&pPair->edpVal, 
                                _T("download domain file renameing: 'source > dest': dest must end in '\\*' or '\\<source file name part>"));
                        }
                    }

                    valex.trim();
                    //force canonical form (may be used in hash tables !!)
                    valex.fileRelPath(m_pIF->FCPgetPrjPath());

                    if(FC_StringHasWildCards(valex))
                    {
                        m_dm.errNS3(&pPair->edpVal, _T("'source > dest' with wildcards, source='"), valex, _T("'"));
                    }

                    if(!dstFileSpec.isEmpty())
                    {
                        if(FC_StringIsAbsPath(dstFileSpec) || _tcsstr(dstFileSpec, _T(".."))!=NULL
                            || FC_StringHasWildCards(dstFileSpec))
                        {
                            m_dm.msg1(CG_E_TEXT, &pPair->edpVal, 
                                m_dm.jot1()<<_T("'source > dest': dest='")<<dstFileSpec<<_T("' must be a relative path without '..\\' and *?"));
                        }
                        //force canonical form (may be used in hash tables !!)
                        dstFileSpec.fileRelPath(NULL);
                    }


                    if(!_tcsicmp(pPair->pszKey, _T("bin")))
                    {
                        m_extBinDestDir.add(dstFileSpec);
                        m_extBinFiles.add(valex);
                        m_extBinFilesEdp.add(pPair->edpVal);
                    }
                    else
                    {
                        m_extDataDestDir.add(dstFileSpec);
                        m_extDataFiles.add(valex);
                        m_extDataFilesEdp.add(pPair->edpVal);
                    }
                }
            }
            else
            {
                m_dm.msg2(CG_E_PRJ_UNKNOWN_KEY, &pPair->edpKey, pPair->pszKey, CG_SEC_4CP_COMPILER_BEG TARGET_EXT);
                if(!bKeyHelpDone)
                {
                    bKeyHelpDone = true;
                    m_dm.userMsg(_T("   known keys are: ")_T("opt, java, bin, data")_T("\n")); 
                }
            }
        }
    }


    //check consistency & defaults:
    if(m_popt->szMakeLib.isEmpty())
    {
        if(!pEdpBPGiven)
            m_bpopt = BP_OPT1;
    }
    else
    {
#if 0
        if(pEdpBPGiven && m_bpopt!=BP_OPT0) //TODO: use front end .mc message res
            m_dm.msg1(CG_W_TEXT, pEdpBPGiven, m_dm.jot1()<<_T("build library: ignore option bp")<<m_bpopt<<_T(", use bp0"));
#endif 
        m_bpopt = BP_OPT0;
    }
}




void CGlobCtx::rebuildAll()
{
    int                 i;
    int                 nErrOld;
    CTarget*            pTrg;
    const CG_Resource*  pRes;
    const CG_Symtab**   pzpRefSym;

    assert(m_dm.hasNoErr());
    
    if(m_dm.hasNoErr())
        initPathInfo();

    if(m_dm.hasNoErr())
        cleanGenDir();

    if(m_dm.hasNoErr())
        getControlLibDomains();

    if(m_dm.hasNoErr())
        callJvcExt();

    if(m_dm.hasNoErr())
        loadConfig();

    if(m_dm.hasNoErr())
        loadTargets();

    if(m_dm.hasNoErr())
        loadResources();

            
    if(m_dm.hasNoErr() && !m_popt->szMakeLib.isEmpty())
    {
        //force all POUs of the project to be compiled
        //also thoses that are not referenced, if building
        //a library there are maybe no targtes and not even a 
        //configuration. Builing a lib from a project means
        //that all POUs are dedicated for this target:
        const CG_POU4CPInfo*const* ppInf = m_pIF->FCPgetAllPOUs();
        for( ; *ppInf; ppInf++)
            compile4CPPOU(*ppInf);
    }

    if(m_dm.hasNoErr())
        callJvcGen();
    
    if(m_dm.hasNoErr() && m_pConfig)//no config if ok, then ignor targets if any
    {                               //no config but nTargets>0 was a warning 
                                    //in loadResources
        for(i=0; i<m_targets.getUsed(); i++)
        {
            pTrg = m_targets.get(i);
            if(!pTrg->hasResName())
                continue; //no resource no code...

            pRes = pTrg->getCGRes();
            if(!pRes)
            {
                assert(m_dm.hasErr());//seems to be in error.
                continue;
            }
 
            //this target and it's resource have no error so far
            //generate all remaining files of the resource:
            nErrOld = m_dm.getTotalErrors();
            pzpRefSym = CGT_EnumRefSymtabs(&m_dm, m_pIF, pRes);

            CGT_DumpXREFFileList(&m_dm, m_pIF, pRes, pzpRefSym);

            //Call helper routine that recurses into all called POUs of 
            //the resource and check the var_externals and #import files 
            CGT_CheckVarExternalsRec(&m_dm, m_pIF, pRes, NULL);

            if(nErrOld==m_dm.getTotalErrors())
                dumpTrgInitFiles(pTrg);

            if(nErrOld==m_dm.getTotalErrors())
                dumpTargetDBIFile(pTrg, pRes);

            //inst view even if no control name given:
            if(nErrOld==m_dm.getTotalErrors())
                 dumpInstView(pTrg, pRes);

            //Finally make loader.list if
            //a) there is a resource assigned
            //b) no front end and no back end error in this resource
            //   and attraced POUs so far.
            //make no loader.list file if no machine name given.
            if(nErrOld==m_dm.getTotalErrors() && pTrg->hasControl())
                dumpTrgLoaderList(pTrg, pzpRefSym);

            delete(pzpRefSym);

            CG_GenRTCECodeForTarget(&m_dm, m_pIF, pTrg);

            if(nErrOld==m_dm.getTotalErrors() && pTrg->hasConsole())
            {
                int i, iret, ival;
                const CG_AttribList* pAttr;    
                const CG_KeyValPair* pPair;

                ival = 0;
                pAttr = m_pIF->FCPgetSection(CG_SEC_4CP_COMPILER);
                if(pAttr)
                {
                    for(i=0; i<pAttr->nAttribs; i++)
                    {
                        pPair  = &pAttr->pairs[i];

                        if(!_tcsicmp(pPair->pszKey, CG_BUILD_OPT))
                            if((iret = CGT_CheckNumOption(&m_dm, &pPair->edpVal, pPair->pszVal, CG_BUILD_OPT_CONSOLEINF, 1, &ival)) > 0)
                                if(ival == 1)
                                    break;
                    }
                }
                if(ival) 
				{
					switch(CG_GetOSVersion())
					{
					case E_WIN_98_95_ME:
					case E_WIN_NT35:
					case E_WIN_NT40:
					case E_WIN_2000:
					case E_WIN_XP:
						buildConsoleForTrg(pTrg);
					break;
					case E_WIN_2003_SERVER:
				        m_dm.msg1(CG_E_CONSOLE_BAD_OS, &pPair->edpVal, _T("Windows 2003 Server"));
					break;
					case E_WIN_VISTA:
				        m_dm.msg1(CG_E_CONSOLE_BAD_OS, &pPair->edpVal, _T("Windows Vista"));
					break;
					default:
						assert(!"bad return from CG_GetOSVersion");
						//fall th:
					case E_WIN_UNKNOWN:
				        m_dm.msg1(CG_E_CONSOLE_BAD_OS, &pPair->edpVal, _T("<Unknown Windows Version>"));
					}
				}
            }

            if(nErrOld==m_dm.getTotalErrors())
            {
                //write build is ok to the sem file:
                FC_CString semf(MAX_PATH);
                semf<<m_pIF->FCPgetGenPath()<<_T("\\project.sem");
                WritePrivateProfileString(_T("compiled"),pTrg->getTrgName(),_T("1"),semf);
            }

        }
    }

    //if lib build need the link files for check in:
    if(m_dm.hasNoErr() && !m_popt->szMakeLib.isEmpty())
    {
        dumpPOULinkFiles();
        buildConsoleForLib();
    }
}




void CGlobCtx::buildConsoleForTrg(const CTarget* pTrg)
{
    ICG_Console* pConGen;
    long         nErr;
    long         nWarn;
    BSTR         sTrgName;
    BSTR         sResName;
    BSTR         sConsoleAddr;
    BSTR         sControlAddr;
    HRESULT      hr;


    assert(!m_dm.getTotalErrors() && pTrg->hasConsole() && pTrg->getCGRes());


    hr = CoCreateInstance(CLSID_CG_Console,
                          NULL, 
                          CLSCTX_INPROC_SERVER,
                          IID_ICG_Console, 
                          (void**)&pConGen
                        );
    if(FAILED(hr))
    {
        m_dm.sysErr(hr, NULL, _T("failed to create Console code generator end COM instance"), NULL);
        return;
    }


    sTrgName     = FC_StringToBSTR(pTrg->getTrgName());
    sResName     = FC_StringToBSTR(pTrg->getResName());
    sConsoleAddr = FC_StringToBSTR(pTrg->getConsole());
    sControlAddr = FC_StringToBSTR(pTrg->getControl());

    hr = pConGen->genConsoleForTarget(
            m_pIF, 
            sTrgName, 
            sResName, 
            sConsoleAddr, 
            sControlAddr, 
            L"4cpc",
            &nErr,
            &nWarn
       );

    FC_FREE_BSTR(sTrgName);
    FC_FREE_BSTR(sResName);
    FC_FREE_BSTR(sConsoleAddr);
    FC_FREE_BSTR(sControlAddr);
    FC_RELEASE_PTR(pConGen);

    if(nErr)
    {
        assert(nErr>0);
        m_dm.incErrors(nErr);
    }
    if(nWarn)
    {
        assert(nWarn>0);
        m_dm.incWarnings(nWarn);
    }

    if(FAILED(hr) && nErr==0)
        m_dm.sysErr(hr, &pTrg->getCGRes()->edp, 
           _T("Console code generation failed unexpectedly target="),pTrg->getTrgName());
}


void CGlobCtx::buildConsoleForLib()
{
    ICG_Console* pConGen;
    long         nErr;
    long         nWarn;
    HRESULT      hr;


    assert(!m_dm.getTotalErrors());


    hr = CoCreateInstance(CLSID_CG_Console,
                          NULL, 
                          CLSCTX_INPROC_SERVER,
                          IID_ICG_Console, 
                          (void**)&pConGen
                        );
    if(FAILED(hr))
    {
        m_dm.sysErr(hr, NULL, _T("failed to create Console code generator end COM instance"), NULL);
        return;
    }

    hr = pConGen->genConsoleForLibrary(
            m_pIF, 
            L"4cpc",
            &nErr,
            &nWarn
       );

    FC_RELEASE_PTR(pConGen);

    if(nErr)
    {
        assert(nErr>0);
        m_dm.incErrors(nErr);
    }
    if(nWarn)
    {
        assert(nWarn>0);
        m_dm.incWarnings(nWarn);
    }

    if(FAILED(hr) && nErr==0)
        m_dm.sysErr(hr, NULL, _T("Console code generation failed unexpectedly when building a library"), NULL);
}


void CGlobCtx::checkFile()
{
    int        iRet;
    CG_Edipos  edp;

    assert(!m_popt->szCheckFile.isEmpty());

    //TODO: call jvc too but in tmp dir
    iRet =  m_pIF->checkSourceFile(m_popt->szCheckFile);
    if(iRet<0)
    {
        CG_InitCMDLEdipos(&edp);
        m_dm.msg3(CG_E_BAD_CMDL_FILETYPE, &edp, _T("c"), m_popt->szCheckFile, m_pIF->KADgetSourceExts());
    }
}


/* Pure IEC interface lib info done by CGT_DumpLinkFile:
 * lib.id;TON
 * lib.iecif;FB$TON(I$bool$in,O$bool$out)
 * lib.target;4CPC
 * lib.class;FUNCTION_BLOCK
 * lib.version;1.20
 * lib.header;ton.hdr
 * lib.link;FUN$getTimeSinceSytemBoot(O$time)
 * lib.link;CONST$FC_MAX_DINT$dint#21499542
 * lib.putif;.pc\.link\ton.hdr
 * lib.putif;..\icons\ioc_small.bmp
 * lib.putif;..\icons\ioc_large.bmp
 * 
 * Appended by back end, precompiled source and console stuff:
 * lib.put;.4cpc\ton.hdr
 * lib.put;.4cpc\classes\ton.class
 * lib.put;..\ton.asp
 * lib.putsource;
*/
void CGlobCtx::dumpPOULinkFiles()
{
    int                        i;
    bool                       bOk;
    const CCodeInfo*           pCInfo;
    const CG_Symtab*           pSym;
    const CG_POU4CPInfo*const* ppInf;
    const TCHAR*               pszFile;
    CGT_CDmp                   dmp;

    //don't call if already errors:
    assert(m_dm.hasNoErr());

    for(ppInf = m_pIF->FCPgetAllPOUs(); *ppInf; ppInf++)
    {
        pSym = m_pIF->loadHeader((*ppInf)->pszId);
        pCInfo = m_POUcode.get(pSym);

        assert(pCInfo->m_pSym==pSym && !lstrcmpi((*ppInf)->pszId, pCInfo->m_pSym->pszId));
        assert(!pSym->pLibCtx && !pCInfo->m_bErrors);

        if(pSym->pouTyp==CG_POU_PRG)
            continue; //no prgs in lib...

        if(pSym->nExternal>0)
        {
            m_dm.msg1(CG_W_CANNOT_PUT_LIB, &pSym->edp, pSym->pszId);
            continue;
        }

        //build back end domain list to be checked into 
        //lib folder: <POU name>\<target type>
        dmp.seek(0); //clear first, used in loop. 
        if(!pCInfo->m_classFile.isEmpty())
        {
            dmp<<CG_LINK_LIB_PUT _T(";")<<getGenPath()<<_T("\\")
               <<pCInfo->m_classFile<<_T('\n');
        }
 
        if(!pCInfo->m_jifFile.isEmpty())
        {
            dmp<<CG_LINK_LIB_PUT _T(";")<<getGenPath()<<_T("\\")
               <<pCInfo->m_jifFile<<_T('\n');
        }

        bOk = CGT_DumpLinkFile(
                  &m_dm, 
                  m_pIF, 
                  pSym, 
                  false,
                  TARGET_EXT, 
                  BIN_CODE_VERSION, 
                  NULL,   
                  dmp.getSzBuffer()
              );


        assert(bOk || m_dm.hasErr());
        if(!bOk)
            break;
    }


    if(m_usrClassFiles.getUsed()>0 || m_extBinFiles.getUsed()>0 || m_extDataFiles.getUsed()>0)
    {
        dmp.seek(0); //clear first, used in loop. 
        //file "__control.link" for extra stuff:

        for(i=0; i<m_usrClassFiles.getUsed(); i++)
            dmp<<CG_LINK_LIB_PUT _T(";")<<m_clsGenPath<<'\\'<<m_usrClassFiles.get(i)<<'\n';

        for(i=0; i<m_extBinFiles.getUsed(); i++)
        {
            pszFile = m_extBinFiles.get(i);
            if(!FC_Exist(pszFile))
            {   //only a warning, may be created in post build !
                m_dm.msg1(CG_W_TEXT, &m_extBinFilesEdp.get(i), 
                    m_dm.jot1()<<_T("file '")<<pszFile<<_T("' does not exist"));
            }

            //m_extBinFiles is full or prj relative:
            dmp<<CG_LINK_LIB_PUT _T(";");
            if(!FC_StringIsAbsPath(pszFile))
                dmp<<m_pIF->FCPgetPrjPath()<<'\\';
            dmp<<pszFile<<'\n';
        }

        for(i=0; i<m_extDataFiles.getUsed(); i++)
        {
            pszFile = m_extDataFiles.get(i);
            if(!FC_Exist(pszFile))
            {   //only a warning, may be created in post build !
                m_dm.msg1(CG_W_TEXT, &m_extDataFilesEdp.get(i), 
                    m_dm.jot1()<<_T("file '")<<pszFile<<_T("' does not exist"));
            }

            //m_extDataFiles is full or prj relative:
            dmp<<CG_LINK_LIB_PUT _T(";");
            if(!FC_StringIsAbsPath(pszFile))
                dmp<<m_pIF->FCPgetPrjPath()<<'\\';
            dmp<<pszFile<<'\n';
        }

 
        //install.inf file: HACK?
        dmp<<CG_LINK_LIB_PUT _T(";")<<m_lnkGenPath<<'\\'<<_T("install.inf")<<'\n';
        //write link file to disk:
        bOk = CGT_DmpToFile(&m_dm, m_lnkGenPath, _T("__control.link"), dmp, false); 
        assert(bOk || m_dm.hasErr());

        dmp.seek(0);
        for(i=0; i<m_usrClassFiles.getUsed(); i++)
            dmp<<_T("classes;")<<FC_StringGetFilePart(m_usrClassFiles.get(i))<<';'<<m_usrClassFiles.get(i)<<'\n';

        for(i=0; i<m_extBinFiles.getUsed(); i++)
        {
            pszFile = FC_StringGetFilePart(m_extBinFiles.get(i));
            //bin;mydll.dll;bin\myPath\mydll.dll
            dmp<<_T("bin;")<<pszFile<<_T(";bin\\");
            if(m_extBinDestDir.get(i)[0])
                dmp<<m_extBinDestDir.get(i)<<'\\';
            dmp<<pszFile<<'\n';
        }
        for(i=0; i<m_extDataFiles.getUsed(); i++)
        {
            pszFile = FC_StringGetFilePart(m_extDataFiles.get(i));
            //bin;myData.txt;data\relPath\mydll.dll
            dmp<<_T("data;")<<pszFile<<_T(";data\\");
            if(m_extDataDestDir.get(i)[0])
                dmp<<m_extDataDestDir.get(i)<<'\\';
            dmp<<pszFile<<'\n';
        }

        bOk = CGT_DmpToFile(&m_dm, m_lnkGenPath, _T("install.inf"), dmp, false); 
        assert(bOk || m_dm.hasErr());
    }
}



void CGlobCtx::loadConfig()
{
    const CG_POU4CPInfo* pInf;
    
    pInf = m_pIF->FCPgetCONinfo(NULL);
    if(!pInf)
        return;//no config, will be lib project...
    
    //load the one and only config:
    m_pConfig = m_pIF->loadConfig(pInf);
    assert(m_pConfig || m_dm.hasErr());
}

void CGlobCtx::loadTargets()
{
    int           i;
    CTarget*      pTrg;
    const TCHAR*  pszControlAddr;

    //read in all target files from command line:
    for(i=0;i<m_pCmdlOpts.targets.getUsed();i++)
    {
        pTrg = new CTarget();
        
        if(!pTrg->loadTarget(this, m_pCmdlOpts.targets.get(i)))
        {
            delete(pTrg);
            //must be a XML parse error, don't continue here
            continue;
        }

        pszControlAddr = pTrg->getControl();
        if(pTrg->hasControl())
        {
            pszControlAddr = pTrg->getControl();
            if(!m_licData.bDemo && m_licData.bIsBundle)
            {
                if(_tcsicmp(_T("LOCALHOST"), pszControlAddr))
                {
                    const TCHAR* pszMach = _tgetenv("COMPUTERNAME");
                    if(pszMach && _tcsicmp(pszMach, pszControlAddr))
                    {
                        m_dm.msg2(CG_E_LICENSE_BUNDLE, pTrg->getEdp(), 
                            pTrg->getTrgName(), pszControlAddr);
                    }
                }                
            }

            for(int j=0; j<m_targets.getUsed(); j++)
            {
                if(!_tcsicmp(pszControlAddr, m_targets.get(j)->getControl()))
                {
                    m_dm.msg3(CG_E_MULT_TARGET_ADDR, pTrg->getEdp(), 
                        pTrg->getTrgName(), pszControlAddr, m_targets.get(j)->getTrgName());
                }
            }
        }
        else
        {
            m_dm.msg1(CG_W_TARGET_NO_ADDR, pTrg->getEdp(), pTrg->getTrgName());
        }

        if(m_targets.getUsed()>0)
        {
            if(m_targets.get(0)->getVersion() != pTrg->getVersion())
               m_dm.msg0(CGB_E_MULT_TARGETVERSION_NOTSUP, pTrg->getEdp());
        }
        m_targets.add(pTrg);
    }

    //set the one and only global version if at least one target:
    if(m_targets.getUsed()>0)
        m_controlVer = m_targets.get(0)->getVersion();

    //call the initial compile routine of all field bus masters:
    for(i=0;i<m_targets.getUsed();i++)
        m_targets.get(i)->compile(&m_dm, &m_kad, m_pIF->FCPgetPrjPath());
}


void CGlobCtx::loadResources()
{
    int                    i;
    const CG_Resource*     pres;
    const CG_Symtab*const* ppSym;
    CTarget*               ptrg; 
    const CG_ResImpInfo*   pImp;
    int                    nCurrErrors;


    if(!m_pConfig)
    {
        if(m_targets.getUsed()>0) //warning unless no targets at all
            m_dm.msg0(CG_W_NO_CONFIG, NULL);   
        return;
    }

    for(i=0;i<m_targets.getUsed();i++)
    {
        ptrg = m_targets.get(i);
        if(!ptrg->hasResName())
        {
            m_dm.msg1(CG_W_NO_RESOURCE_FOR_TARGET, ptrg->getEdp(), ptrg->getTrgName());
            continue;
        }

        //find the import in the .con:
        pImp = m_pConfig->getResImpByName(ptrg->getResName());
        if(!pImp)
        {
            m_dm.msg1(CG_E_UNDEF_RESOURCE, ptrg->getEdp(), ptrg->getResName());   
            continue;
        }

        pres = m_pIF->loadResource(pImp->pszImpFile);
        if(!pres)
        {
            m_dm.msg1(CG_E_UNDEF_RESOURCE, &pImp->edp, ptrg->getResName());
        }
        else if(pres->bNoError)
        {
            //validate task inits:
			checkTaskSettings(&m_dm, m_pIF, pres, ptrg);
        
            //insertEx: maybe a resource is assigned to more than one
            //target, don't overwrite old pres, overwrite would also work,
            //but this is clearer.
            m_resources.insertEx(pres->pszId, pres, false, NULL);

            //get all programs and all global FBs
            //and generate their body code
            nCurrErrors = m_dm.getTotalErrors();

            for(ppSym = pres->pzGVLRefSyms; *ppSym; ppSym++)
                compilePrgFBSdt(*ppSym);

            //if no errors in resource pres->bNoError and also no 
            //error while producing code set the m_pres member of
            //the target:
            if(nCurrErrors==m_dm.getTotalErrors())
                ptrg->setCGRes(pres);
        }
        else
        {
            assert(m_dm.hasErr());
        }
    }

}







#if 0
//TODO    if(instpath.depth()>MAX_INSTPATH_NESTING-2)//-2: configuration.resource not part of instpath
    {
        ST_DiagMsg2(NULL, E_4CCG_ERR_MAX_FBNESTING,
            Tostr_C(MAX_INSTPATH_NESTING), instpath);
        
        return false;

    }

#endif 



void CGlobCtx::dumpTrgInitFiles(CTarget* pTrg)
{
    long nCurrErr = m_dm.getTotalErrors(); 
    //Make init file for the resource if no front end and no back 
    //end error in this resource and attraced POUs so far.
    //Make init files even if no machine name given, this way 
    //at least all diags of direct addresses will be produced.
    PC_DumpRetainFile(this, pTrg);
    PC_DumpTargetInitFile(this, pTrg);

    if(nCurrErr == m_dm.getTotalErrors())
    {
        CGT_CDmp dmp;
        dmp<<_T("Memory usage:\n");
        //TODO: total num of all vars !CG_W_TOO_MANY_VAR_OBJS
        dmp<<_T("Retain variables: ")<<pTrg->getUsedRetainItems()<<'\n';
        dmp<<_T("IO-variables:     ")<<pTrg->getUsedIOPoints()<<'\n';
        CGT_DmpToFile(&m_dm, pTrg->getTrgGenPath(), _T("meminfo.txt"), dmp, false);
    }
}



void CGlobCtx::dumpTargetDBIFile(
    CTarget*            pTrg,
    const CG_Resource*  pRes
)
{
    CGT_DumpTRGDBIFile2(
        &m_dm,
        m_pIF,
        pRes,
	    pTrg->getTrgName(),
	    TARGET_EXT,
	    pTrg->getControl(),
	    PC_GEN_DBI_DIR,
        getControlVer()>=22000 ? true : false,
        NULL,
        NULL,
        NULL
    );

}


void CGlobCtx::dumpInstView(
    CTarget*            pTrg,
    const CG_Resource*  pRes
)
{
    //dump the XML file for the debug view:    
    DumpInstView(
        &m_dm,     //for err msg
        m_controlVer,
        m_pIF,     //front end IF
        TARGET_EXT,
        pTrg->getTrgName(),
        pTrg->hasControl() ? pTrg->getControl() : pTrg->getTrgName(), //HACK if no control arrd given use PC1
        pRes     //called resource
    );
}




void CGlobCtx::dumpTrgLoaderList(
    CTarget*                pTrg,
    const CG_Symtab*const*  pzpRefSym
)
{
    int                j;
    const CG_Symtab*   pSym; 
    const TCHAR*       pszFile;
    CCodeInfo*         pInfo;
    FC_CString         anyOtherDomains(MAX_PATH);

    for(j=0; (pSym = pzpRefSym[j])!=NULL; j++)
    {
        pInfo = m_POUcode.get(pSym);
        if(!pInfo)
            continue;

        assert(pInfo->m_pSym==pSym);
        assert(!pInfo->m_bErrors);

        if(!pInfo->m_classFile.isEmpty())
            pTrg->addDomain(pInfo->m_classFile);
    }


    for(j=0; j<m_extBinFiles.getUsed(); j++)
    {
        pszFile = m_extBinFiles.get(j);
        if(!FC_Exist(pszFile))
        {   //only a warning, may be created in post build !
            m_dm.msg1(CG_W_TEXT, &m_extBinFilesEdp.get(j), 
                m_dm.jot1()<<_T("file '")<<pszFile<<_T("' does not exist"));
        }

        //m_extBinFiles is full or prj relative:
        if(FC_StringIsAbsPath(pszFile))
            anyOtherDomains<<pszFile<<';';
        else
            anyOtherDomains<<_T("..\\")<<pszFile<<';';

        anyOtherDomains<<_T("..\\bin\\");
        if(m_extBinDestDir.get(j)[0])
            anyOtherDomains<<m_extBinDestDir.get(j)<<'\\';
        
        anyOtherDomains<<FC_StringGetFilePart(pszFile)<<'\n';
    }

    for(j=0; j<m_extDataFiles.getUsed(); j++)
    {
        pszFile = m_extDataFiles.get(j);
        if(!FC_Exist(pszFile))
        {   //only a warning, may be created in post build !
            m_dm.msg1(CG_W_TEXT, &m_extDataFilesEdp.get(j), 
                m_dm.jot1()<<_T("file '")<<pszFile<<_T("' does not exist"));
        }

        //m_extDataFiles is full or prj relative:
        if(FC_StringIsAbsPath(pszFile))
            anyOtherDomains<<pszFile<<';';
        else
            anyOtherDomains<<_T("..\\")<<pszFile<<';';

        anyOtherDomains<<_T("..\\data\\");
        if(m_extDataDestDir.get(j)[0])
            anyOtherDomains<<m_extDataDestDir.get(j)<<'\\';
        
        anyOtherDomains<<FC_StringGetFilePart(pszFile)<<'\n';
    }

    pTrg->dumpLoaderList(this,  m_usrClassFiles, m_cmnClassFiles, m_cmnLoaderlist, anyOtherDomains);
}


void CGlobCtx::compilePrgFBSdt(
    const CG_Symtab*     pSym
)
{
    bool                   bOk;
    ICG_Body*              pIBdy;
    CCodeInfo*             pInfo;
    int                    nCurrErr;
    FC_CGArray<CGT_BP_TYP> bpList;


    //assert: don't have inline fb,prg:
    assert(pSym->flags & CG_STAB_HAS_BDY || pSym->pLibCtx || pSym->pouTyp==CG_POU_SDT);
    //assert: pSym with error can occure only if called loadHeader directly:
    assert(pSym->flags & CG_STAB_HDR_NOERR);

    pInfo = m_POUcode.get(pSym);
    if(pInfo)
        return;//already compiled.

    pInfo = new CCodeInfo(pSym);
    bOk = m_POUcode.insert(pSym, pInfo);
    assert(bOk);

    
    //recursive called function body errors count to the calling prg,FB
    //therefore get the curr err count before getRecurseCalledFuns:
    nCurrErr = m_dm.getTotalErrors();

    pIBdy = NULL;
    if(pSym->pouTyp==CG_POU_SDT)
    {
        GenCodeStruct(this, pInfo);
    }
    else if(pSym->flags & CG_STAB_HAS_BDY)
    {
        //this back end does not reload bodies...
        assert((pSym->flags & CG_STAB_BDY_LOADED)==0);

        long codeGenFlags = m_bpopt!=BP_OPT2 ? CG_LOADBDY_TMPVAR_OPT1 : 0;
        const CG_Symtab* pSym2 = m_pIF->loadBody(pSym->pszId, codeGenFlags, &pIBdy);
        //cannot return NULL, we have already a pSym
        assert(pSym2==pSym && (pSym->flags & CG_STAB_BDY_LOADED));
        //body or error:
        assert(pIBdy || m_dm.hasErr());

        if(pIBdy)
        {
            const CCodeInfo** pzCalledFuns;
            FC_CHashPtr<const CG_Symtab*, const CCodeInfo*> called;

            assert(pSym->flags & CG_STAB_BDY_NOERR);

            //keep a reference to the ext body info:
            assert(!pInfo->m_pIExtBdyInf);
            pInfo->m_pIExtBdyInf = m_pIF->loadExtBodyInfo(pSym);

            //get first all called functions of this prg/fb
            getRecurseCalledFuns(pSym, &called);
            pzCalledFuns = called.allocValArray(NULL);
            called.clear();

            if(pIBdy->isSFC())
                GenCodeSFCPrgFB(this, pIBdy, pzCalledFuns, pInfo, &bpList);
            else
                GenCodeSTPrgFB(this, pIBdy, pzCalledFuns, pInfo, &bpList);

            delete(pzCalledFuns);

            //update global code info members:
            m_CodeFilesCreated++;
            if(pInfo->m_jitmenot)
                m_jitmenot = true;
        }
    }
    else if(pSym->pLibCtx)
    {
        //get precompiled code from lib:
        GenCodeLibPOU(this, pSym, pInfo, &bpList);
    }
    else
    {
        assert(!"neither body nor lib Prg,FB,SDT?");
    }

    if(nCurrErr==m_dm.getTotalErrors())
    {
        CGT_DumpPOUDBIFile(
           &m_dm, 
           m_pIF, 
           pSym,
           pIBdy,
           bpList.getCElements(), 
           bpList.getUsed(), 
           PC_GEN_DBI_DIR
        );
    }

    if(pIBdy)
        pIBdy->Release();
    
    if(nCurrErr!=m_dm.getTotalErrors())
        pInfo->m_bErrors = true;


    //compile all bodies of used FBs and Structs:
    assert(pSym->pzRefSyms);
    if(pSym->pzRefSyms)
    {
        for(const CG_Symtab*const* ppSym=pSym->pzRefSyms; *ppSym; ppSym++)
        {
            if((*ppSym)->pouTyp != CG_POU_FUN)
                compilePrgFBSdt(*ppSym);
        }
    }

}


void CGlobCtx::getRecurseCalledFuns(
    const CG_Symtab*     pCaller, 
    FC_CHashPtr<
      const CG_Symtab*, 
      const CCodeInfo*>* pCalled
)
{
    int                    nCurrErr;
    const CG_Symtab*const* ppSym;
    const CG_Symtab*       pSym;
    const CG_Symtab*       pSym2;
    ICG_Body*              pIBdy;
    CCodeInfo*             pInfo;
    const CCodeInfo*       pCInfo;
    
    if(!pCaller && pCaller->pzRefSyms)
    {
        assert(!"bad symtab or body not loaded");
        return;
    }

    
    for(ppSym=pCaller->pzRefSyms;*ppSym; ppSym++)
    {
        pSym = *ppSym;
        if(pSym->pouTyp!=CG_POU_FUN)
            continue;

        if((pSym->flags & CG_STAB_HDR_NOERR)==0)
        {
            assert(!"unexpected error in hdr");//should not pass here
            continue;
        }

        if(pSym->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR))
            continue;//not relevant CAUTION: continue is only as long inline funs don't
                     //have local vars, if so we must build such local types if SDT or FB 
                     //If any fun in/output is SDT or FB that type will be processed by the caller
                     //as long as it's an error to have unconnected fun in/out puts.

        pCInfo = pCalled->get(pSym);
        if(pCInfo)
        {
            if(pCInfo->m_bErrors)
                m_dm.addIndirectErr();
            continue; //already seen
        }
        
        pCInfo = m_POUcode.get(pSym);
        if(pCInfo)
        {
            if(pCInfo->m_bErrors)
                m_dm.addIndirectErr();
            
            pCalled->insert(pSym, pCInfo);
            //recurse into not to build it here, just to get the called fun list done:
            this->getRecurseCalledFuns(pSym, pCalled);
            continue; //already compiled, may be with errors..
        }

        //must be compiled:
        pInfo = new CCodeInfo(pSym);
        m_POUcode.insert(pSym, pInfo);
        pCalled->insert(pSym, pInfo);
        nCurrErr = m_dm.getTotalErrors();

 
        pIBdy = NULL;
        FC_CGArray<CGT_BP_TYP> bpList;

        if((pSym->flags & CG_STAB_HAS_BDY)==0)
        {
            //must be a lib defined fun with precompiled code:
            assert(pSym->pLibCtx);

            //keep a reference to the ext body info:
            assert(!pInfo->m_pIExtBdyInf);
            pInfo->m_pIExtBdyInf = m_pIF->loadExtBodyInfo(pSym);

            GenCodeLibPOU(this, pSym, pInfo, &bpList);
            
            this->getRecurseCalledFuns(pSym, pCalled);
        }
        else
        {
            //this back end does not reload bodies...
            assert((pSym->flags & CG_STAB_BDY_LOADED)==0);
            assert(!pSym->pzRefSyms);
            
            long codeGenFlags = m_bpopt!=BP_OPT2 ? CG_LOADBDY_TMPVAR_OPT1 : 0;
            pSym2 = m_pIF->loadBody(pSym->pszId, codeGenFlags, &pIBdy);
            //cannot return NULL, we have already a pSym
            assert(pSym2==pSym && (pSym->flags & CG_STAB_BDY_LOADED));
            assert(pIBdy || m_dm.getDisplayedErrors()>nCurrErr);
            if(pIBdy)
            {
                assert(pSym->flags & CG_STAB_BDY_NOERR);

                //keep a reference to the ext body info:
                assert(!pInfo->m_pIExtBdyInf);
                pInfo->m_pIExtBdyInf = m_pIF->loadExtBodyInfo(pSym);
                
                GenCodeFun(this, pIBdy, pInfo, &bpList);

                //update global code info members:
                if(pInfo->m_jitmenot)
                   m_jitmenot = true;//TODO do funs have this attr?

                //recurse into:
                this->getRecurseCalledFuns(pSym, pCalled);
            }
        }

        if(nCurrErr==m_dm.getTotalErrors())
        {
            CGT_DumpPOUDBIFile(
               &m_dm, 
               m_pIF, 
               pSym,
               pIBdy,
               bpList.getCElements(), 
               bpList.getUsed(), 
               PC_GEN_DBI_DIR
            );
        }

        if(pIBdy)
            pIBdy->Release();

        //now build the FBs, SDTs used in the fun header
        assert(pSym->pzRefSyms);
        if(pSym->pzRefSyms)
        {
            for(const CG_Symtab*const* ppSym2=pSym->pzRefSyms; *ppSym2; ppSym2++)
            {
                if((*ppSym2)->pouTyp!=CG_POU_FUN)
                    compilePrgFBSdt(*ppSym2);
            }
        }

        if(nCurrErr!=m_dm.getTotalErrors())
            pInfo->m_bErrors = true;
    }
}



void CGlobCtx::compile4CPPOU(
    const CG_POU4CPInfo* pInf //a 4cp POU info
)
{
    int                    nCurrErr;
    const CG_Symtab*       pSym;
    const CG_Symtab*       pSym2;
    ICG_Body*              pIBdy;
    CCodeInfo*             pInfo;
    

    pSym = m_pIF->loadHeader(pInf->pszId);
    if(pSym)
    {
        if((pSym->flags & CG_STAB_HDR_NOERR)==0)
        {
            assert(m_dm.hasErr());
            return;
        }
    }
    else
    {
        //should not happen:
        assert(!"loadHeader failed but POU was in 4cp");
        m_dm.msg2(CG_E_UNDEF_POU_TYP, &pInf->edp, _T(""), pInf->pszId);   
        return;
    }

   
    if(pSym->pouTyp==CG_POU_PRG || pSym->pouTyp==CG_POU_FB)
    {
        compilePrgFBSdt(pSym);
        return;
    }


    if(m_POUcode.get(pSym))
        return; //already seen


    //must be compiled:
    pInfo = new CCodeInfo(pSym);
    m_POUcode.insert(pSym, pInfo);

    if(pSym->pouTyp==CG_POU_SDT)
        return; //nothing to do but want to have it in the m_POUcode 

    assert(pSym->pouTyp==CG_POU_FUN);//no gvl,cst symtab here!
    
    nCurrErr = m_dm.getTotalErrors();

    if(pSym->flags & CG_STAB_HAS_BDY)
    {
        //this back end does not reload bodies...
        assert((pSym->flags & CG_STAB_BDY_LOADED)==0);

        long codeGenFlags = m_bpopt!=BP_OPT2 ? CG_LOADBDY_TMPVAR_OPT1 : 0;
        pSym2 = m_pIF->loadBody(pSym->pszId, codeGenFlags, &pIBdy);

        //cannot return NULL, we have already a pSym
        assert(pSym2==pSym && (pSym->flags & CG_STAB_BDY_LOADED));
        assert(pIBdy || m_dm.getDisplayedErrors()>nCurrErr);
        if(pIBdy)
        {
            assert(pSym->flags & CG_STAB_BDY_NOERR);
            //keep a reference to the ext body info:
            assert(!pInfo->m_pIExtBdyInf);
            pInfo->m_pIExtBdyInf = pIBdy->getExtBodyInfo();
            FC_CGArray<CGT_BP_TYP> bpList;
            GenCodeFun(this, pIBdy, pInfo, &bpList);
            //TODO no DBI, ok for lib creation?
            pIBdy->Release();
        }
    }
    else
    {
        //TODO lib
    }

    if(nCurrErr!=m_dm.getTotalErrors())
        pInfo->m_bErrors = true;
}


void CGlobCtx::getControlLibDomains()
{
    const CG_LIBContext*const* ppLIB;
    const CG_LIBContext*       pLIB;
    FC_CString                 locator(MAX_PATH);
    CGT_CSVFile                csv;
    const TCHAR*               fields[3];
    CG_Edipos                  edp;
    int                        n;
    int                        nFields;
    FC_CString                 checkOutDir(MAX_PATH);
    FC_CString                 libCheckOutDir(MAX_PATH);


    for(ppLIB = m_pIF->FCPgetAllLIBs(); *ppLIB; ppLIB++)
    {  
        locator.load((*ppLIB)->pszFile);
        locator<<_T("::")CG_LIB_POU_CONTROL _T("\\")TARGET_EXT _T("\\install.inf");
        if(!CGT_ExistsLocatorFile(&m_dm, m_pIF, locator, &pLIB))
            continue;
        assert(*ppLIB == pLIB);

        libCheckOutDir.load(getTrgTypeGenPath())<<'\\'<<pLIB->pszId;
        libCheckOutDir.changeFileExt(NULL);
        
        n = CGT_LIBCheckOutFiles(&m_dm, m_pIF, pLIB, CG_LIB_POU_CONTROL, TARGET_EXT,
            _T("install.inf"), libCheckOutDir);
        assert(n==1);

        locator.load(libCheckOutDir)<<'\\'<<_T("install.inf");
        if(!CGT_LoadCSVFile(&m_dm, &csv, NULL, locator))
        {
            assert(m_dm.hasErr());
            return; 
        }

        while(csv.hasMoreFields())
        {
            CG_InitSTEdipos(&edp, locator, csv.getLineno());
            nFields = csv.getRecordFields(3, fields);
            if(nFields != 3 || !csv.isLastFieldEOL())
            {
                m_dm.msg1(CG_E_SYNTAX_EXPECTED, &edp,  
                    _T("<type>;<lib file>;<[<control relative path>\\]<dest file>"));
                continue;
            }

            if(_tcscmp(FC_StringGetFilePart(fields[2]), fields[1]))
            {
                m_dm.errNS1(&edp, _T("lib file different from file part of control destination"));
                continue;
            }

            if(!_tcsicmp(fields[0], _T("classes")))
            {
                if(!_tcscmp(FC_StringGetFileExt(fields[2]), _T("class")))
                {
                    m_cmnClassFiles.add(fields[2]);
                    checkOutDir.load(getClsGenPath())<<'\\'<<fields[2];
                    checkOutDir.stripFilePart();
                }
                else
                {
                    m_dm.msg2(CG_E_SYNTAX_EXPECTED_FOUND, &edp, 
                        _T("file extension .class"), FC_StringGetFileExt(fields[2]));
                    continue;
                }
            }
            else if(!_tcsicmp(fields[0], _T("bin")) || !_tcsicmp(fields[0], _T("data")))
            {
                checkOutDir.load(libCheckOutDir);
                m_cmnLoaderlist<<_T(".")TARGET_EXT _T("\\")<<FC_StringGetFilePart(checkOutDir)<<'\\'<<fields[1]<<';';
                m_cmnLoaderlist<<_T("..\\")<<fields[2]<<'\n';
            }
            else
            {
                m_dm.msg2(CG_E_SYNTAX_EXPECTED_FOUND, &edp, _T("class; bin; or data;"), fields[0]);
                continue;
            }

            if(_tcsstr(fields[2], _T("..")) || FC_StringIsAbsPath(fields[2]) ||
               FC_StringStartsWithI(fields[2], _T("config\\"))>0 || FC_StringStartsWithI(fields[2], _T("config/"))>0)
            {
                m_dm.msg1(CG_E_TEXT, &edp, m_dm.jot1()<<_T("invalid domain path '")<<fields[2]<<
                    _T("': path must be relative and must not contain '..' or start with 'config\\'"));
                continue;
            }

            n = CGT_LIBCheckOutFiles(&m_dm, m_pIF, pLIB, CG_LIB_POU_CONTROL, TARGET_EXT,
                fields[1], checkOutDir);
            assert(n==1);
        }
    }

}

void CGlobCtx::callJvcExt()
{
    int          i;
    FC_CString   opts(MAX_PATH);
    const TCHAR* pszEngBin;

    if(m_extJavaFiles.getUsed()==0)
        return; //user did not say .java=<files> in 4cp [compiler.4cpc] section
    
    //first user defined extra options from 
    //opt.jvc.ext= in 4cp [compiler.4cpc] section
    if(!m_optJVC_EXT.isEmpty())
    {
        opts<<m_optJVC_EXT;
        if(opts[opts.len()-1]!=' ')
            opts<<' ';
    }

    //nologo and class path: 
    //order important: first gen\.4cpc\.classes , then .zip then COM\softing:
    pszEngBin = m_pIF->FCPgetInstallationPath();
    opts<<_T("-nologo -cp:p ");
    CGT_AppendCMDLFileArg(opts, getClsGenPath())<<';';
    CGT_AppendCMDLFileArg(opts, pszEngBin)<<_T("\\4cpc\\classes")<<';';
    CGT_AppendCMDLFileArg(opts, pszEngBin)<<_T("\\4cpc\\classes\\classes.zip");

    //output path for class files -d c:\4c2\project1\gen\.4cpc\.classes
    opts<<_T(" -d ");
    CGT_AppendCMDLFileArg(opts, getClsGenPath());
    
    //jvc does not create the -d <dir>, only the package path related folders
    //If this is the first build after removed 'gen' there is a problem:
    if(!FC_Exist(getClsGenPath()))
        FC_CreateSubDir(getClsGenPath());

    //now loop all .java= from 4cp [compiler.4cpc] section in natural order
    //Note: a single '.java=' may contain more than one file or even *.java. 
    for(i=0; i<m_extJavaFiles.getUsed(); i++)
        ExecuteJvc(&m_dm, opts, m_extJavaFiles.get(i), m_optVerbose);


    if(m_dm.hasNoErr())
    {
        assert(m_usrClassFiles.getUsed()==0);

        TCHAR** pzpszClasses = FC_FileBuildFileList(getClsGenPath(), NULL, FC_FILE_ENUM_SUBDIRS);
        if(!pzpszClasses)
        {
            m_dm.msg2(CG_E_FILE_OPEN, NULL, getClsGenPath(), //TODO diag misleading...
                m_dm.jot1().fmtSysErr(GetLastError()));
        }

        //all class files that are now newly created are user class files
        //note that gen\.classes\... contains already class files of 4cl __control
        for(int i=0; pzpszClasses[i]; i++)
        {
            if(m_cmnClassFiles.find(pzpszClasses[i])) 
            {
                m_dm.msg1(CG_W_TEXT, m_extJavaFilesEdp.getAddrOf(0), m_dm.jot1()<<
                    _T("custom .java overrides precompiled '")<<pzpszClasses[i]<<
                    _T("' from one of the 4cl libraries"));
            }
            else
            {
                m_usrClassFiles.add(pzpszClasses[i]);
            }
        }

        FC_FileDeleteFileList(pzpszClasses);

    }
}

void CGlobCtx::callJvcGen()
{
    FC_CString   opts(MAX_PATH);
    FC_CString   java(MAX_PATH);
    const TCHAR* pszEngBin;

    if(m_CodeFilesCreated==0)
        return; //may happen no prg,fb !
   
    //first user defined extra options from 
    //opt.jvc.gen= in 4cp [compiler.4cpc] section
    if(!m_optJVC_GEN.isEmpty())
    {
        opts<<m_optJVC_GEN; 
        //force end in a blank:
        if(opts[opts.len()-1] != ' ')
            opts<<' ';
    }

    if(m_jitmenot)
        opts<<_T("-X:v ");

    //nologo and class path:
    pszEngBin = m_pIF->FCPgetInstallationPath();
    opts<<_T("-nologo -cp:p ");

    //if user java files created or from __common of any 4cl add also 
    //gen\.4cpc\.classes to -cp:
    //order important: first gen\.4cpc\.classes , then .zip then COM\softing:
    if(m_usrClassFiles.getUsed()>0 || m_cmnClassFiles.getUsed()>0)
        CGT_AppendCMDLFileArg(opts, getClsGenPath())<<';';
    CGT_AppendCMDLFileArg(opts, pszEngBin)<<_T("\\4cpc\\classes")<<';';
    CGT_AppendCMDLFileArg(opts, pszEngBin)<<_T("\\4cpc\\classes\\classes.zip");
    
    //input:
    CGT_AppendCMDLFileArg(java, getClsGenPath())<<_T("\\*.java");

    //class output path: use default:

    ExecuteJvc(&m_dm, opts, java, m_optVerbose);
}




const CG_POU4CPInfo** CGlobCtx::getProcessed4CPFiles()const
{
    FC_CGArray<const CG_POU4CPInfo*> seen(256);  
    const CG_POU4CPInfo*             pInf;
    const FC_CHashPtr<
        const CG_Symtab*, 
        CCodeInfo*>::Pair*           pPair;

    
    for(pPair = m_POUcode.firstPair(); pPair; pPair = pPair->pNext)
    {
        if(pPair->pKey->pLibCtx)
            continue;

        pInf = m_pIF->FCPgetPOUinfo(pPair->pKey->pszId);
        assert(pInf);
        if(pInf)
            seen.add(pInf);
    }
    
    seen.add(NULL);
    return seen.getMem();
}

