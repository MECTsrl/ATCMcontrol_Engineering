

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include "helper.h"
#include "cmdl.h"
#include "fc_tools\fc_fileIO.h"
#include "lic\fc_slbase.h"
#include "cgt_misc.h"
#include "cgt_link.h"
#include "cgt_tools.h"
#include "cgt_xmlparse.h"
#include "CG_4CLAccess.h"
#include "CG_IECFront_i.c" //define the guids from front end
#include "CG_4CLAccess_i.c" //define the guids from libacess



static long g_dollarflags = CGT_INS_ESCAPE|CGT_INS_ENV|CGT_INS_PRINTHELP|CGT_INS_WARN_UNDEF;



extern "C" void ExecCustomBuildSteps(
    CGT_CDiagMsg*        pdm,
    ICG_IECFront*        pIFront,
    int                  nVerbose,
    bool                 bPre //else post
)
{
    int                  i;
    long                 lExitCode;
    const TCHAR*         pszVal;
    const TCHAR*         pszKey;
    const CG_AttribList* pSec;
    FC_CString           preCmd(MAX_PATH);
    FC_CString           szWorkDir(MAX_PATH);
    const TCHAR*         pszPrjPath;
    DWORD                dwLastErr;
    FC_CString           strCDMsgViewSync;
    int                  ret;    

    pSec = pIFront->FCPgetSection(CG_SEC_4CP_COMPILER);
    if(!pSec)
        return;
    
    pszPrjPath = pIFront->FCPgetPrjPath();
    szWorkDir.load(pszPrjPath);

    for(i=0; i<pSec->nAttribs; i++)
    {
        pszKey = pSec->pairs[i].pszKey;
        
        if(bPre && (!_tcsicmp(pszKey, CG_BUILDCMD_PRE) ||
                    !_tcsicmp(pszKey, CG_BUILDCMD_PRECD) ||
                    !_tcsicmp(pszKey, CG_BUILDCMD_PREI) ||
                    !_tcsicmp(pszKey, CG_BUILDCMD_PREP))
        )
        {
            // do pre
        }
        else if(!bPre && (!_tcsicmp(pszKey, CG_BUILDCMD_POST) ||
                          !_tcsicmp(pszKey, CG_BUILDCMD_POSTCD) ||
                          !_tcsicmp(pszKey, CG_BUILDCMD_POSTI) ||
                          !_tcsicmp(pszKey, CG_BUILDCMD_POSTP))
        )
        {
            // do post
        }
        else
        {
            //do nothing
            continue;
        }

        pszVal = pSec->pairs[i].pszVal;
        ret = CGT_SubsStdDollarIns(pdm, pIFront, pszVal, &pSec->pairs[i].edpVal, g_dollarflags, &preCmd);
        if(ret==0)
            break;
        if(ret<0)
            g_dollarflags &= ~CGT_INS_PRINTHELP;
        
        if(!_tcsicmp(pszKey, CG_BUILDCMD_PREP) || !_tcsicmp(pszKey, CG_BUILDCMD_POSTP))
        {
            pdm->userMsg(preCmd);
            pdm->userMsg(_T("\n"));
            continue;
        }



        if(!_tcsicmp(pszKey, CG_BUILDCMD_PRECD) || !_tcsicmp(pszKey, CG_BUILDCMD_POSTCD))
        {
            if(FC_StringIsAbsPath(preCmd))
                szWorkDir.load(preCmd);
            else
                szWorkDir.load(pszPrjPath)<<'\\'<<preCmd;

            if(!FC_Exist(szWorkDir))
            {
                pdm->msg1(CG_E_CUST_BUILD_ERROR, &pSec->pairs[i].edpVal, 
                    pdm->jot1()<<_T("cd ")<<szWorkDir);
                break;
            }

            strCDMsgViewSync.load(_T("cd "))<<preCmd<<'\n';
            pdm->userMsg(strCDMsgViewSync);
            continue;
        }

        if(nVerbose>1)
        {
            pdm->userMsg(preCmd);
            pdm->userMsg(_T("\n"));
        }


        lExitCode = -99;
        dwLastErr = ERROR_SUCCESS;
        if(!FC_System(NULL, preCmd, szWorkDir, &lExitCode))
        {
            dwLastErr = GetLastError();
            if(dwLastErr == ERROR_SUCCESS)//just in case the GetLastError() does not work fine...
                dwLastErr = ERROR_FILE_NOT_FOUND;
        }

        if((dwLastErr!=ERROR_SUCCESS || lExitCode!=0))
        {
            if(_tcsicmp(pszKey, CG_BUILDCMD_PREI) && _tcsicmp(pszKey, CG_BUILDCMD_POSTI))
            {
                pdm->msg1(CG_E_CUST_BUILD_ERROR, &pSec->pairs[i].edpVal, preCmd);
                if(dwLastErr!=ERROR_SUCCESS)
                {
                    pdm->userMsg(pdm->jot1()<<_T("   create process failed: hr=")
                          <<dwLastErr<<_T(": ")<<pdm->jot2().fmtSysErr(dwLastErr)<<'\n');
                }
                else
                {
                    pdm->userMsg(pdm->jot1()<<_T("   tool returned exit code ")<<lExitCode<<'\n');
                }
                break;
            }
        }
    }

    if(!strCDMsgViewSync.isEmpty())
        pdm->userMsg(_T("cd $(prjdir)\n"));
}




extern "C" void Check4CPNeedVersion(
    CGT_CDiagMsg*  pdm,
    ICG_IECFront*  pIFront
)
{
    int                        i;
    const CG_AttribList*       pSec;
    const TCHAR*               pszKey;
    const TCHAR*               pszVal;
    FC_CString                 expanded;
    FC_CString                 verFile;
    FC_CString                 jot;
    FC_CString                 verString;
    const TCHAR*               psz;
    CG_EXPRTYP                 cmpOP; 
    bool                       bIs4CL;
    bool                       bIsKAD;
    FC_CString                 libDLL;
    HMODULE                    hLibAccDll = NULL;
    int                        nNeedBuildNr;
    int                        nFoundBuildNr;
    SLIC_GetVersionFromDataFileTyp pGetVerOfProd;
    const CG_Edipos*           pEdp;
    int                        ret;

   
    pSec = pIFront->FCPgetSection(CG_SEC_4CP_FORMAT);
    if(!pSec)
        return;
 
    for(i=0; i<pSec->nAttribs; i++)
    {
        pszKey = pSec->pairs[i].pszKey;
        pszVal = pSec->pairs[i].pszVal;
        pEdp   =&pSec->pairs[i].edpVal;

        if(_tcsicmp(pszKey, CG_KEY_4CP_NEEDVER))
            continue;
        
        ret = CGT_SubsStdDollarIns(pdm, pIFront, pszVal, pEdp, g_dollarflags, &expanded);
        if(ret==0)
            return;
        if(ret<0)
            g_dollarflags &= ~CGT_INS_PRINTHELP;


        jot.load(verFile.clear().appendUpTo(expanded, _T("><="), &psz));
        verFile.trim();
        jot.trim();
        //jot contains text after _T("><="):
        nNeedBuildNr = 0;
        if(jot.stripBeginI(_T("build")))
            nNeedBuildNr = atoi(jot);

        
        if(FC_StringIncBegin(&psz, _T("<=")))
            cmpOP = CG_LE;
        else if(FC_StringIncBegin(&psz, _T(">=")))
            cmpOP = CG_GE;
        else if(FC_StringIncBegin(&psz, _T(">")))
            cmpOP = CG_GT;
        else if(FC_StringIncBegin(&psz, _T("<")))
            cmpOP = CG_LT;
        else if(FC_StringIncBegin(&psz, _T("=")))
            cmpOP = CG_EQ;
        else
            psz = NULL;


        if(!psz || *psz==_T('<') || *psz==_T('=') || *psz==_T('>') ||
            verFile.isEmpty() || nNeedBuildNr<3355
           )
        {
            pdm->msg1(CG_E_SYNTAX_EXPECTED, pEdp, _T("<file> [<>=] Build<No>  (No must be >=3355)"));
            pdm->userMsg(_T("   Example: needver= 4C_IEC1131_3.4cl >= Build3355\n"));
            continue;
        }

        psz = FC_StringGetFileExt(verFile);
        bIs4CL = !_tcsicmp(psz, CG_PREDEF_LIB_EXT);
        bIsKAD = !_tcsicmp(psz, _T("kad"));
        if(!bIs4CL && !bIsKAD && _tcsicmp(psz, _T("dll")) && _tcsicmp(psz, _T("exe")))
        {
            pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, pEdp, _T("version file type: exe, dll, 4cl or kad"), psz);
            continue;
        }

        if(!FC_StringIsAbsPath(verFile))
        {
            psz = bIs4CL ? pIFront->FCPgetStandardLibPath() : pIFront->FCPgetInstallationPath();
            verFile.prepend(psz, _T("\\")); 
        }

        if(!FC_Exist(verFile))
        {
            pdm->msg1(CG_E_FILE_NOT_EXIST, pEdp, verFile);
            continue;
        }
        
 
        if(bIs4CL)
        {
            if(libDLL.isEmpty())
            {
                //HACK !
                libDLL.load(pIFront->FCPgetInstallationPath())<<_T("\\cg_4claccess.dll");
                hLibAccDll = LoadLibrary(libDLL);
                if(!hLibAccDll)
                {
                    pdm->msg1(CG_E_FILE_NOT_EXIST, pEdp, libDLL);
                    break;
                }

                //get the function pointer
                pGetVerOfProd = (SLIC_GetVersionFromDataFileTyp) 
                      GetProcAddress(hLibAccDll, SLIC_GET_FILEVER_INFO_PROC);
                if(!pGetVerOfProd)
                {
                    pdm->msg1(CG_E_TEXT, pEdp, 
                        libDLL.prepend("'")<<_T("' missing DLL-export for 4CL version info"));
                    break;
                }
            }

            TCHAR szFileVer[MAX_PATH];
            pGetVerOfProd(verFile, szFileVer, FC_ARRAY_LEN(szFileVer));
            verString.load(szFileVer);
        }
        else if(bIsKAD)
        {
            IXMLDOMNode* pXMLRoot = CGT_XMLLoadFile(pdm, NULL, verFile, NULL); 
            if(!pXMLRoot)
                continue;
            CGT_XMLGetStrAttrTrim(pdm, pXMLRoot, L"PRODVERSION", &verString, false, false);
            pXMLRoot->Release();
        }
        else
        {
            TCHAR szFileVer[MAX_PATH];
            FC_GetFileVersion(verFile, szFileVer, FC_ARRAY_LEN(szFileVer));
            verString.load(szFileVer);
        }

        if(verString.isEmpty())
        {
            pdm->msg1(CG_E_TEXT, pEdp, verFile.prepend("'")<<_T("' does not contain a version info"));
            continue;
        }

        //before V2.07: 4cl: V2.0/2.05.2608.0     exe/dll: 2.05.2608.0:  (<-we don't support this any longer)
        //since V2.07 : 4cl: V2.10/2.10.02.3355   exe/dll: 2.10.02.3355
        //remove .0: service pack ??
        jot.load(FC_StringGetFileExt(verString));
        nFoundBuildNr = atoi(jot);

        if(nFoundBuildNr<3355)
        {
            pdm->msg1(CG_E_TEXT, pEdp, verFile.prepend("'")<<_T("' bad version string format '")
                 <<verString<<_T("' expected format like: 2.10.02.<BuildNo>   (BuildNo must be >=3355"));
            continue;
        }


        bool bOk;
        switch(cmpOP)
        {
        case CG_GE: bOk = nFoundBuildNr >= nNeedBuildNr; psz=_T(">="); break;
        case CG_LE: bOk = nFoundBuildNr <= nNeedBuildNr; psz=_T("<="); break;
        case CG_GT: bOk = nFoundBuildNr >  nNeedBuildNr; psz=_T(">");  break;
        case CG_LT: bOk = nFoundBuildNr <  nNeedBuildNr; psz=_T("<");  break;
        case CG_EQ: bOk = nFoundBuildNr == nNeedBuildNr; psz=_T("=");  break;
        default:
            assert(!"bad op");
            bOk = false;
        }
        if(!bOk)
        {
            pdm->msg1(CG_E_TEXT, pEdp, 
                pdm->jot1()<<_T("'")<<verFile<<_T("': custom version check failed: need BuildNr ")
                <<psz<<nNeedBuildNr<<_T(" but found ")<<nFoundBuildNr);
        }
    }

    if(hLibAccDll)
        FreeLibrary(hLibAccDll);
}




//key names are check on load4CP now check files in 
//respect to the KAD info.
//The libs section is also completely checked by the front.
extern "C" void Check4CPCompilerSec(
    CGT_CDiagMsg*    pdm,
    ICG_IECFront*    pIFront,
    CMDL_OPTIONSTYP* pOpt
)
{
    int                        i;
    int                        is;
    int                        ival;
    int                        iret;
    const CG_AttribList*       pSec;
    const TCHAR*               pszKey;
    const TCHAR*               pszVal;
    const CG_Edipos*           pEdpVal;
    const TCHAR*const*         pzpszSecs;
    bool                       bKeyHelpDone = false;
    bool                       bOptHelpDone = false;
    bool                       bXRefGiven = false;
    bool                       bConsoleInfoGiven = false;
    bool                       bVerboseGiven = false;

//check [compiler]:
    pSec = pIFront->FCPgetSection(CG_SEC_4CP_COMPILER);
    if(pSec)
    {
        for(i=0; i<pSec->nAttribs; i++)
        {
            pszKey = pSec->pairs[i].pszKey;
            pszVal = pSec->pairs[i].pszVal;
            pEdpVal   =&pSec->pairs[i].edpVal;

            if(!_tcsicmp(pszKey, CG_BUILD_OPT))
            {
                if((iret = CGT_CheckNumOption(pdm, pEdpVal, pszVal, CG_BUILD_OPT_XREF, 1, &ival))!=0)
                {
                    if(iret>0)
                    {
                        if(bXRefGiven)
                            pdm->msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), CG_BUILD_OPT_XREF);
                        else
                            pOpt->bXREF = ival ? true : false;
                    }
                }
                else if((iret = CGT_CheckNumOption(pdm, pEdpVal, pszVal, CG_BUILD_OPT_VERBOSE, 2, &ival))!=0)
                {
                    if(iret>0)
                    {
                        if(bVerboseGiven)
                            pdm->msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), CG_BUILD_OPT_VERBOSE);
                        else
                            pOpt->nVerbose = ival;
                    }
                }
                else if((iret = CGT_CheckNumOption(pdm, pEdpVal, pszVal, CG_BUILD_OPT_CONSOLEINF, 1, &ival))!=0)
                {
                    if(iret>0)
                    {
                        if(bConsoleInfoGiven)
                            pdm->msg2(CG_W_CMDL_OPT_TWICE, pEdpVal, _T(""), CG_BUILD_OPT_XREF);
                        else
                            pOpt->bConsoleInfo = ival ? true : false;
                    }
                }
                else
                {
                    pdm->msg1(CG_E_CMDL_UNKNOWN_OPT, pEdpVal,  pszVal);
                    if(!bOptHelpDone)
                    {
                        bOptHelpDone = true;
                        pdm->userMsg(_T("   known 'opt=' are: ")CG_BUILD_OPT_HELP _T("\n")); 
                    }
                }
            }
            else if(!_tcsicmp(pszKey, _T("srcdir")))
            {
                FC_CString str(MAX_PATH);
                iret = CGT_SubsStdDollarIns(pdm, pIFront, pszVal, pEdpVal, g_dollarflags, &str);
                if(iret!=0)
                {
                    if(iret<0)
                        g_dollarflags &= ~CGT_INS_PRINTHELP;

                    pOpt->libSrcDirs.add(str);
                    pOpt->libSrcDirsEdp.add(pEdpVal);
                }
            }
            else if(!_tcsicmp(pszKey, _T("lib.trg")))
            {
                if(pIFront->KADgetTargetTypeByExt(pszVal, NULL, NULL, NULL) || !_tcscmp(pszVal, _T("0")))
                {
                    if(pOpt->szLibTargetType.isEmpty())
                        pOpt->szLibTargetType.load(pszVal);
                    //else: cmdl override
                }
                else
                {
                    pdm->msg2(CG_W_NO_TARGETTYPE_EXT, pEdpVal, pszVal, pIFront->KADgetTargetTypeExts());
                }
            }
            else if(_tcsicmp(pszKey, CG_BUILDCMD_LIBLIC)&&
                    _tcsicmp(pszKey, CG_BUILDCMD_LIBPUT)&&
                    _tcsicmp(pszKey, CG_BUILDCMD_LIBTXT)&&
                    _tcsicmp(pszKey, CG_BUILDCMD_PRE)   &&
                    _tcsicmp(pszKey, CG_BUILDCMD_PRECD) &&
                    _tcsicmp(pszKey, CG_BUILDCMD_PREI)  &&
                    _tcsicmp(pszKey, CG_BUILDCMD_PREP)  &&
                    _tcsicmp(pszKey, CG_BUILDCMD_POST)  &&
                    _tcsicmp(pszKey, CG_BUILDCMD_POSTCD)&&
                    _tcsicmp(pszKey, CG_BUILDCMD_POSTI) &&
                    _tcsicmp(pszKey, CG_BUILDCMD_POSTP)
                   )
            {
                //TODO: better a warning only?
                pdm->msg2(CG_E_PRJ_UNKNOWN_KEY, &pSec->pairs[i].edpKey, 
                   pszKey, CG_SEC_4CP_COMPILER);
                if(!bKeyHelpDone)
                {
                    bKeyHelpDone = true;
                    pdm->userMsg(_T("   known keys are: ")CG_COMPSEC_HELP _T(", lib.trg, srcdir\n")); 
                }
            }
        }
    }


    if(pOpt->libAct==LIB_MAKE && pOpt->szLibTargetType.isEmpty())
    {
        const TCHAR* pszTrgExts = pIFront->KADgetTargetTypeExts();
        const TCHAR* pszComma  = _tcschr(pszTrgExts, _T(','));
        if(pszComma)
        {
            //more target types in KAD use first as default do warn:
            pOpt->szLibTargetType.loadLen(pszTrgExts, pszComma - pszTrgExts);

            CG_Edipos cmdlEdp;
            CG_InitCMDLEdipos(&cmdlEdp);
            pdm->msg2(CG_W_CMDL_MLAL_DEFAULT_TARGET, &cmdlEdp, _T("-ml"), 
                pdm->jot1()<<_T("-ml:")<<pOpt->szLibTargetType);
        }
        else
        {
            //only one target type in KAD use as default don't warn:
            pOpt->szLibTargetType.load(pszTrgExts);
        }
    }

   
//check [compiler.*]: 
    for(pzpszSecs = pIFront->FCPgetSectionNames(); *pzpszSecs; pzpszSecs++)
    {
        is = FC_StringStartsWithI(*pzpszSecs, CG_SEC_4CP_COMPILER_BEG);
        if(is<0)
            continue;//not a compiler.*

        //check that [compiler.<target type>] is a registered target type:
        if(!pIFront->KADgetTargetTypeByExt(*pzpszSecs+is, NULL, NULL, NULL))
        {
            pSec = pIFront->FCPgetSection(*pzpszSecs);
            //if section is empty no edp available! solution: just ignor if empty...
            if(pSec && pSec->nAttribs>0)
            {
                pdm->msg2(CG_W_NO_TARGETTYPE_EXT, &pSec->pairs[0].edpKey, 
                *pzpszSecs, pIFront->KADgetTargetTypeExts());
            }
        }
        //don't validate [compiler.<target type>] content, this
        //must be done by back ends.
    }
  
}



#define BUILD_NO_RELEVANT_FILE_PAT _T("fileview.xml|CEPrjSettings.4cs") //HACK why is this not ONE file ???
static bool Check4CLUpToDate(
    CGT_CDiagMsg*    pdm, 
    const CG_Edipos* pEdpSrcdirSpec,
    const TCHAR*     psz4CLFullPath, 
    const TCHAR*     psz4CPFullPath, 
    FC_CString&      reason
)
{
    __int64    ftLastWrt4CP;
    TCHAR      szFile[2*MAX_PATH];
    FC_CString str4CPPath(MAX_PATH);

    reason.clear();
    if(!FC_Exist(psz4CLFullPath))
    {
        reason<<_T("that does not exist");
        return false;
    }

    //check if also upto date;
    str4CPPath.load(psz4CPFullPath).stripFilePart();

    if(FC_FileLastWrt(str4CPPath, NULL, NULL, _T("gen"), BUILD_NO_RELEVANT_FILE_PAT, 
        szFile, FC_ARRAY_LEN(szFile), &ftLastWrt4CP)
      )
    {
        if(FC_FileGetLastWriteTime(psz4CLFullPath) > ftLastWrt4CP)
            return true; //ok, is up to date
    }
    else
    {
        pdm->msg2(CG_E_FILE_OPEN, pEdpSrcdirSpec, str4CPPath, 
            pdm->jot1().fmtSysErr(GetLastError()));
        return false;
    }

    //for better diag use full path to file, the user 
    //can then see if is the correct project and what he wants to do:
    reason<<(_T("that is out of date (last change in '"))<<szFile<<')';
    return false;
}


static const CG_Edipos* Find4CPFor4CL(
    CGT_CDiagMsg*    pdm,
    CMDL_OPTIONSTYP* pOpt,
    bool*            pbWarnUndefSrcDir, 
    const TCHAR*     psz4CLFull, 
    FC_CString&      str4CPFull
)
{
    int             i;
    FC_CString      str4CLNoEx;
    const TCHAR*    pszDir;
    bool            bWarned = false;

    str4CPFull.clear();

    str4CLNoEx.load(FC_StringGetFilePart(psz4CLFull)).changeFileExt(NULL);

    for(i=0; i<pOpt->libSrcDirs.getUsed(); i++)
    { 
        //first try the longer macht:
        pszDir = pOpt->libSrcDirs.get(i);

        if(!FC_FileExists(pOpt->sz4CPFullPath, pszDir))
        {
            if(*pbWarnUndefSrcDir)
            {
                bWarned = true;
                pdm->msg1(CG_W_TEXT,pOpt->libSrcDirsEdp.get(i) , pdm->jot1()<<_T("file '")<<pszDir<<_T("' does not exist")); 
            }
            continue;
        }
        str4CPFull.load(pOpt->libSrcDirs.get(i))<<'\\'<<str4CLNoEx<<'\\'<<str4CLNoEx<<_T(".4cp");
        str4CPFull.fileFullPath(pOpt->sz4CPFullPath);

        if(!FC_Exist(str4CPFull))
        {
            //try variant 2:
            str4CPFull.stripFilePart();
            str4CPFull<<_T(".4cp");
            if(!FC_Exist(str4CPFull))
                continue;
        }
        return pOpt->libSrcDirsEdp.get(i);
    }

    *pbWarnUndefSrcDir = !bWarned;
    return NULL;
}

extern "C" void AutoBuildLibs(
    CGT_CDiagMsg*    pdm,
    ICG_IECFront*    pIFront,
    CMDL_OPTIONSTYP* pOpt
)
{
    int                          i;
    int                          j;
    const CG_AttribList*         pSec;
    const TCHAR*                 psz4CLFull;
    const CG_Edipos*             pEdpVal;  
    FC_CString                   str;
    FC_CString                   str4CPFull;
    FC_CString                   str4CPPath;
    FC_CString                   str4CLNoEx;
    FC_CString                   str4CLShrt;
    FC_CString                   strUsrHint;
    FC_CString                   cmdlOpts;
    FC_CGArray<const CG_Edipos*> cmdLEdps; 
    FC_CStrList                  prjFiles;
    FC_CStrList                  prjPaths;
    FC_CStrList                  libFiles;
    FC_CStrList                  userMsgs;
    FC_CString                   strCmdl;
    const CG_Edipos*             pEdpSrcdirSpec;
    bool                         bWarnUndefSrcDir = true;
    bool                         bSrcDirsPrinted = false;


    pSec = pIFront->FCPgetSection(CG_SEC_4CP_LIBS);
    //no [lib] section ?
    if(!pSec)
        return;

    //if no 'libsrc=<path>' is given we canot build libs:
    if(pOpt->libSrcDirs.getUsed()==0)
        return;//check existence will be done by front end any way...


    for(i=0; i<pSec->nAttribs; i++)
    {
        if(_tcsicmp(pSec->pairs[i].pszKey, CG_KEY_4CP_FILE))
            return; //front end will do err msg, TODO better diag here ?

        psz4CLFull = pSec->pairs[i].pszVal;
        pEdpVal    =&pSec->pairs[i].edpVal;


        str4CLNoEx.load(FC_StringGetFilePart(psz4CLFull)).changeFileExt(NULL);
        //for better display, back substitue $-insert for default lib dir, this works here as 
        //a string operation because all involved pathes have canonical form at this point:
        str4CLShrt.load(psz4CLFull);
        if(str4CLShrt.stripBeginI(pIFront->FCPgetStandardLibPath()))
            str4CLShrt.prepend(_T("$(")CG_INSERT_LIBDIR _T(")"));


        //try to find a corresponding 4cp:
        pEdpSrcdirSpec = Find4CPFor4CL(pdm, pOpt, &bWarnUndefSrcDir, psz4CLFull, str4CPFull);
        if(pEdpSrcdirSpec)
        {
            //update processing data arrays for build step:
            str4CPPath.load(str4CPFull).stripFilePart();

            if(!Check4CLUpToDate(pdm, pEdpSrcdirSpec, psz4CLFull, str4CPFull, strUsrHint))
            {
                cmdLEdps.add(pEdpVal);
                prjFiles.add(str4CPFull);
                prjPaths.add(str4CPPath);
                libFiles.add(str4CLShrt);
                userMsgs.add(str.load(_T("4cg: auto build '"))<<str4CLShrt<<_T("' ")<<strUsrHint<<'\n');
            }
        }
        else if(!FC_Exist(psz4CLFull))
        {
            pdm->msg1(CG_E_FILE_NOT_EXIST, pEdpVal, psz4CLFull);

            if(!bSrcDirsPrinted && bWarnUndefSrcDir)
            {
                bSrcDirsPrinted = true;
                pdm->msg1(CG_W_TEXT, pOpt->libSrcDirsEdp.get(0), 
                    pdm->jot1()<<_T("4cg: auto build '")<<str4CLShrt<<_T("': no project named '")
                    <<str4CLNoEx<<_T(".4cp' found in: "));

                pdm->userMsg(pdm->jot1()<<_T("    '")<<pOpt->libSrcDirs.get(0)<<"'\n");
                for(j=1; j<pOpt->libSrcDirs.getUsed(); j++)
                   pdm->userMsg(pdm->jot1()<<_T("or  '")<<pOpt->libSrcDirs.get(j)<<"'\n");

                pdm->userMsg(_T("\n"));
            }
        }
        //else: TODO 4CL exists but no 4CP, this means system libraries, we have no 
        //special semantics for system libs, that seems to be proplem here. The lib may be 
        //a user lib that should be rebuild but the 'srcdir' does not point to the 4CP.
        //We see a does not exist error message later in thisa case anyway...
    }

    if(pdm->hasErr())
        return;
    if(prjFiles.getUsed()==0)
    {
        if(pOpt->nVerbose > (pOpt->bRecursive?1:0)) 
            pdm->userMsg(_T("4cg: auto build: libraries up-to-date...\n"));
        return;
    }

    //get build options, same for all:
    cmdlOpts.load(_T(" -nologo -r -ml"));
    if(!pOpt->szLibTargetType.isEmpty())//inherit lib trg type if any available...
    {
        cmdlOpts<<':'<<pOpt->szLibTargetType;
    }
    else if(pOpt->targets.getUsed()==1)
    {
        cmdlOpts<<':'<<FC_StringGetFileExt(pOpt->targets.get(0));
    }
    else
    {
        FC_CGArray<TargetAndType> targets;
        GetAllTargets(pdm, pIFront, &targets, NULL);
        if(targets.getUsed()==1)
            cmdlOpts<<':'<<FC_StringGetFileExt(targets.get(0).pszTrg);
    }
    cmdlOpts<<' ';


    if(pOpt->nVerbose>0)
    {
        for(i=0; i<prjFiles.getUsed(); i++)
            pdm->userMsg(userMsgs.get(i));
    }

    for(i=0; i<prjFiles.getUsed(); i++)
    {
        //build command line use 4CP relative path we start the 4cg in the 4CP-Path anyway):
        strCmdl.clear();
        CGT_AppendCMDLFileArg(strCmdl, FC_StringGetFilePart(prjFiles.get(i)))<<cmdlOpts;
        CGT_AppendCMDLFileArg(strCmdl, libFiles.get(i));        

        //sync directory info with 4CE's message view:
        assert(FC_StringIsAbsPath(prjPaths.get(i)));
        pdm->userMsg(pdm->jot1()<<_T("cd ")<<prjPaths.get(i)<<'\n');

        if(pOpt->nVerbose>1)
            pdm->userMsg(pdm->jot1()<<GetModuleName()<<' '<< strCmdl<<'\n');

        //execute 4cg, force exe start dir the 4cp's directory (this will avoid any 
        //problems, the auto build lib should behave as if called from engineering):
        long exitCode;
        if(!FC_System(GetModuleName(), strCmdl, prjPaths.get(i), &exitCode))
        {
            pdm->sysErr(GetLastError(), cmdLEdps.get(i), _T("CreateProcess: "), GetModuleName());
            break;
        }
        if(exitCode!=0)
        {
            assert(exitCode>0);
            pdm->incErrors(abs(exitCode));
            break;
        }
    }

    //sync directory info with 4CE's message view:
    pdm->userMsg(_T("cd $(prjdir)\n"));
    //done.
}

extern "C" void CheckFile(
    CGT_CDiagMsg*          pdm,
    ICG_IECFront*          pIFront,
    const CMDL_OPTIONSTYP* pOpts
)
{
    CG_Edipos     edp;
    const TCHAR*  pszbackEnd;
    const TCHAR*  pszTrgType;
    const TCHAR*  pszTrgVers;
    FC_CString    jotCmd;
	FC_CString    msgtxt;

	pszTrgType = pOpts->szCheckTrgType;
	pszTrgVers = pOpts->szCheckTrgVers;

	//special case: no target type given on command line 
	//but only one target type installed => use this one:
	if(pszTrgType[0]==0 && pIFront->KADCountTargetTypes()==1)
		pszTrgType = pIFront->KADgetTargetTypeExts();


    msgtxt<<_T("check '")<<FC_StringGetFilePart(pOpts->szCheckFile)<<'\'';
	if(pszTrgType[0])
	{
		msgtxt<<_T(" for target type ")<<pszTrgType;
		if(pszTrgVers[0])
			msgtxt<<_T(" V")<<pszTrgVers;
		
		msgtxt<<_T(":\n");
	}
	else
	{
		msgtxt<<_T(" for IEC syntax:\n");
	}
	pdm->userMsg(msgtxt);


    if(pszTrgType[0])
    {
        pIFront->KADgetTargetTypeByExt(pszTrgType, &pszbackEnd, NULL, NULL);
        //assert: trg typ must exist in any case, this was checked already..
        assert(pszbackEnd);

        //command line for check file, starts with 4cp file (path is same as 
        //4cg was called). Quote with "<file>" may contain blanks...
        jotCmd.load(_T("-nologo "));
        CGT_AppendCMDLFileArg(jotCmd, pOpts->sz4CP)<<_T(" -c:");
        CGT_AppendCMDLFileArg(jotCmd, pszTrgType)<<' ';
        CGT_AppendCMDLFileArg(jotCmd, pOpts->szCheckFile);

		//TODO: Workaround: passing an optional target version number via the command line
		//is not possible because we may have very old backends, we pass this info
		//inside a file with fixed name, old backend simple don't read it.
		//If we make a new major version all backends should add this to there cmd line
		//and then we don't need this tmp file any more...
		if(pszTrgVers[0])
		{
			FC_CString fname(MAX_PATH);
			fname<<pIFront->FCPgetGenPath()<<_T("\\checkinf.tmp");
			WritePrivateProfileString(_T("target"), _T("version"), pszTrgVers, fname);
		}

        CallBackend(pdm, pOpts->nVerbose, pszbackEnd, jotCmd, pIFront->FCPgetGenPath(), pszTrgType);
		
		//delete file if done!
		CGT_DeleteFile(pdm, pIFront->FCPgetGenPath(), _T("checkinf.tmp"));
    }
	else
	{
		//no target info, pure IEC check using the front end:

		//load all libs referenced in 4CP:
		if(pdm->hasNoErr())
			pIFront->loadLIBs();

		//load all CONST of project and all libs:
		if(pdm->hasNoErr())
			pIFront->loadCSTs();
    
		//TODO every where : disable warnings on reparse.
		//only check a single source file:
		if(pdm->hasNoErr())
		{
			if(pIFront->checkSourceFile(pOpts->szCheckFile)<0)
			{
				CG_InitCMDLEdipos(&edp);
				pdm->msg3(CG_E_BAD_CMDL_FILETYPE, &edp,
					_T("c"), pOpts->szCheckFile, pIFront->KADgetSourceExts());
			}
		}
	}
}




extern "C" void BuildTargets(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const FC_CGArray<TargetAndType>& targets,
    const CMDL_OPTIONSTYP*           pOpts
)
{
    int                       i;
    int                       it;
    int                       itt;
    bool                      bFound;
    const TCHAR*              pszbackEnd;
    const TCHAR*              pszTrgTypeExt;
    CG_Edipos                 edp;
    FC_CString                jotTrg(MAX_PATH); 
    FC_CString                jotTrgDisplay(MAX_PATH);	// d-175 01.02.05 SIS
    FC_CString                jotCmd(MAX_PATH);
    FC_CString                jotCmdBeg(MAX_PATH);
    TargetAndType             trgType;             
    const TCHAR*              pszGenPath;
    boolean                   bMultipleTrgs;
    FC_CString                trgNoExt(MAX_PATH);
    FC_CString                semf(MAX_PATH);

    
    pszGenPath = pIFront->FCPgetGenPath();
    
    semf<<pszGenPath<<_T("\\project.sem");

    //warn if no targtes:
    if(targets.getUsed()==0)
    {
        CG_InitSTEdipos(&edp, pIFront->FCPgetDiagFileName());
        pdm->msg0(CG_W_NO_TARGETS_IN_4CP, &edp);
    }

    

    //if a named target list was given at the command line check 
    //that there is such a target, may be a nested one (that is why
    //we cannot check this earlier):
    for(i=0;i<pOpts->targets.getUsed();i++)
    {
        bFound = false;
        for(it=0;it<targets.getUsed();it++)
        {
            if(!_tcsicmp(targets.get(it).pszTrg, pOpts->targets.get(i)))
            {
                bFound = true;
                break;
            }
        }
        if(!bFound)
        {
            CG_InitCMDLEdipos(&edp);
            pdm->msg1(CG_E_CMDL_NO_SUCH_TARGET, &edp, pOpts->targets.get(i));
        }
    }

    //stop on error:
    if(pdm->hasErr())
        return;

    //in the KAD's order loop over all target types and 
    //call the back end once for all it's targtes:
    for(itt=1;itt<=pIFront->KADCountTargetTypes();itt++)
    {
        jotTrg.clear();
		jotTrgDisplay.clear();		// d-175 01.02.05 SIS
        jotCmd.clear();
        jotCmdBeg.load(_T("-nologo "));
        CGT_AppendCMDLFileArg(jotCmdBeg, pOpts->sz4CP);
        if(pOpts->bIncrementelly)
            jotCmdBeg<<_T(" -i");

        pszTrgTypeExt = pIFront->KADgetTargetTypeByNum(itt, &pszbackEnd, NULL, &bMultipleTrgs);
        assert(pszTrgTypeExt);

        for(it=0;it<targets.getUsed();it++)
        {
            trgType = targets.get(it);
            if(trgType.iType==itt)
            {
                //write build is not ok to the sem file:
                trgNoExt.load(trgType.pszTrg).changeFileExt(NULL);
                if(pOpts->targets.getUsed()>0)
                {
                    //build list of named targtes, build only those:
                    if(pOpts->targets.findI(trgType.pszTrg))
                    {
		                jotTrgDisplay<<trgNoExt<<' ';		// d-175 01.02.05 SIS
                        CGT_AppendCMDLFileArg(jotTrg<<' ', trgType.pszTrg);
                        WritePrivateProfileString(_T("compiled"),trgNoExt,_T("0"),semf);
                    }
                }
                else
                {
	                jotTrgDisplay<<trgNoExt<<' ';		// d-175 01.02.05 SIS
                    jotTrg<<_T(" \"")<<trgType.pszTrg<<_T("\"");
                    WritePrivateProfileString(_T("compiled"),trgNoExt,_T("0"),semf);
                }

            }

            //if the backend does not support multiple trgs on the command line
            //call individually:
            if(!bMultipleTrgs && !jotTrg.isEmpty())
            {
				// d-175 01.02.05 SIS >>
				jotTrgDisplay.trim();
				jotTrgDisplay << ":\n";
	            pdm->userMsg(jotTrgDisplay);
				jotTrgDisplay.clear();
				// d-175 01.02.05 SIS <<
                jotCmd.load(jotCmdBeg)<<jotTrg;
                jotTrg.clear();//must clear now!
                CallBackend(pdm, pOpts->nVerbose, pszbackEnd, jotCmd, pIFront->FCPgetGenPath(), pszTrgTypeExt);
            }
        }

        if(bMultipleTrgs && !jotTrg.isEmpty())
        {
			// d-175 01.02.05 SIS >>
			jotTrgDisplay.trim();
			jotTrgDisplay << ":\n";
	        pdm->userMsg(jotTrgDisplay);
			// d-175 01.02.05 SIS <<
            jotCmd.load(jotCmdBeg)<<jotTrg;
            CallBackend(pdm, pOpts->nVerbose, pszbackEnd, jotCmd, pszGenPath, pszTrgTypeExt);
        }
    }

}




bool CallBackend(
    CGT_CDiagMsg* pdm,
    int           nVerbose,
    const TCHAR*  pszExePath,
    const TCHAR*  pszCmdArgs,
    const TCHAR*  pszGenPath,
    const TCHAR*  pszTrgType
)
{
    long          nBackendErr;
    long          nBackendWarn;
    FC_CString    respfile(MAX_PATH);
    FC_CString    jot(MAX_PATH);
    TCHAR*        pText;
    const TCHAR*  pCText;
    long          exitCode;

    respfile.load(pszGenPath)<<_T("\\.")<<pszTrgType<<_T("\\result.txt");

    if(!CGT_DeleteFile(pdm, NULL, respfile))
        return false;

    if(nVerbose>1)
    {
        jot.load(pszExePath)<<' '<<pszCmdArgs<<'\n';
        pdm->userMsg(jot);
    }


    if(!FC_System(pszExePath, pszCmdArgs, NULL, &exitCode))
    {
        pdm->sysErr(GetLastError(), NULL, _T("CreateProcess: "), pszExePath);
        return false;
    }

    //exitCode 0 normal termination  0 errors
    //exitCode 1 normal termination >0 errors
    //exitCode 2 normal termination after fatal error
    //exitCode 3 normal termination after too many errors
    //all other exist codes mean that something crashed...
    if(exitCode<0 || exitCode>3)
    {
        pdm->msg1(CG_E_BACKEND_ERROR, NULL, pszTrgType);
        return false;
    }

    if(exitCode==0 || exitCode==1)
    {
        if(!FC_Exist(respfile))
        { 
            pdm->msg1(CG_E_FILE_NOT_EXIST, NULL,  respfile);
            pdm->userMsg("    Note: response file of backend '");
            pdm->userMsg(pszExePath);
            pdm->userMsg("' not found.\r\n");
            return false;
        }

        if(!CGT_LoadTextFileToMem(pdm, NULL, pszGenPath, respfile,  &pText, NULL, NULL))
            return false;

        pCText = pText;
    
        pCText = jot.clear().appendUpTo(pCText, _T("\r\n"));
        nBackendErr = atoi(jot); 
        assert(*pText && nBackendErr>=0);
    
        if(nBackendErr>0)
            pdm->incErrors(nBackendErr);

        pCText= jot.clear().appendUpTo(pCText, _T("\r\n"));
        nBackendWarn= atoi(jot); 
        assert(nBackendWarn>=0);
    
        if(nBackendWarn>0)
            pdm->incWarnings(nBackendWarn);

        delete(pText);
        return nBackendErr==0;
    }
    else
    {
        pdm->msg1(CG_E_BACKEND_ERROR, NULL, pszTrgType);
        return false;
    }
    return true;
}













extern "C" void GetAllTargets(
    CGT_CDiagMsg*               pdm,
    ICG_IECFront*               pIFront,
    FC_CGArray<TargetAndType>*  pTargets,
    const TCHAR*                pszTrgTypeOnly
)
{
    int                   i;
    const CG_AttribList*  pSecData;
    const TCHAR*          pszTrgType;
    TargetAndType         trgTyp;

    pTargets->resetUsed();
    pSecData = pIFront->FCPgetSection(CG_SEC_4CP_TARGETS);
    if(!pSecData || pSecData->nAttribs==0)
        return;//no top level tagtes...

    //sort all targets according to their type:
    //TODO getNestedTargets
    pTargets->allocSize(128);//that should be enough...
    for(i=0;i<pSecData->nAttribs;i++)
    {
        pszTrgType = FC_StringGetFileExt(pSecData->pairs[i].pszVal);
        
        //check for trg type filter:
        if(pszTrgTypeOnly && _tcsicmp(pszTrgTypeOnly, pszTrgType))
            continue;

        trgTyp.pszTrg= pSecData->pairs[i].pszVal;
        trgTyp.iType = pIFront->KADgetTargetTypeByExt(pszTrgType, NULL, NULL, NULL);
        if(trgTyp.iType>0)
        {
            pTargets->add(trgTyp);
        }
        else
        {
            //should cause a warning in Check4CP:
            assert(pdm->getDisplayedWarnings()>0);
        }
    }
}




static int CleanUpCB(void* pCtx, void* pCtx2, int bIsDir, int bOnError, const TCHAR* pszFile)
{
    CGT_CDiagMsg*    pdm    = (CGT_CDiagMsg*)pCtx;
    const TCHAR*     psz;

    if(bOnError)
    {
        pdm->msg1(CG_E_CLEANUP_FILES, NULL, pszFile);
        return 0;
    }

    if(bIsDir)
    {
        psz = FC_StringGetFilePart(pszFile);
        if(psz[0]==_T('_') && psz[1]==_T('_'))
            return 0;//skip dirs that do not begin with __
        else
            return 1;
    }
    else if(!_tcsicmp(FC_StringGetFileExt(pszFile), _T("inst")))//HACK: dont' delete PC1.inst, 
               //better solution is put the .inst to gen\__views and overwrite only if changed
    {
        return 1;
    }
    else
    {
        return CGT_DeleteFile(pdm, NULL, pszFile) ? 1 : -1;
    }

}

extern "C" void CleanUpGenDir(CGT_CDiagMsg* pdm, ICG_IECFront* pIFront)
{
    const TCHAR* pszGen;
    
    pszGen = pIFront->FCPgetGenPath();

    if(!FC_Exist(pszGen))
        return;

    if(!FC_FileEnumFiles(pszGen, FC_FILE_ENUM_SUBDIRS|FC_FILE_ENUM_FULLPATH, CleanUpCB, pdm, NULL))
    {
        assert(pdm->hasErr());
        if(!pdm->hasErr())
            pdm->msg1(CG_E_CLEANUP_FILES, NULL, pszGen);
    }
}



