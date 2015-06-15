/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/4cvm.cpp 2     22.05.07 16:33 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: 4cvm.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/4cvm.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 22.05.07 16:33 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include <stdio.h>
#include "helper.h"
#include "fc_todebug\fc_mem.h"
#include "fc_tools\fc_fileIO.h"
#include "globctx.h"
#include "4cvm.h"
#include "newdel.h"
#include "cgt_xmlparse.h"
#include "utilif.h"

#include "BuildNr.h"
#include "Product.h"
#include "CG_IECFront_i.c" //define the guids from front end
#include "resource.h"

static TCHAR g_szModuleName[MAX_PATH];

static bool DumpLogoAndHelp(
    CGT_CDiagMsg* pdm,
    int       argc, 
    TCHAR**   argv

)
{
    int              i;
    const TCHAR*     psz;
    int              iNologo;
    int              iHelp;
    FC_CString       jot;


    iNologo = 0;
    iHelp   = 0;
    for(i=0;i<argc;i++)
    {
        psz = argv[i]; 
        if((*psz==_T('/') || *psz==_T('-')))
        {
            if(!lstrcmpi(psz+1, _T("nologo")))
                iNologo++;
            else if(!lstrcmpi(psz+1, _T("help")) || !lstrcmpi(psz+1, _T("?")))
                iHelp++;
        }
    }

    if(iNologo==0)
    {
        psz = _T(PROD_PRODUCTNAME) _T(" IEC 1131-3 Compiler embedded-VM backend ")
              _T(PROD_FULL_VERSION_STR) _T("\n")
              _T(PROD_COPYRIGHT) _T("\n\n");
        //replace © with (C) for dos box...
        psz = jot.clear().appendUpTo(psz, _T("\251"));
        jot<<_T("(C)")<<psz;
        pdm->userMsg(jot);
       
    }

    if(iHelp>0)
    {
        pdm->userMsg(jot.clear().fmtMsg(FCG_I_USAGE, NULL));
        return true;
    }

    if(iNologo>1)
    {
        CG_Edipos        cmdl;
        CG_InitCMDLEdipos(&cmdl);
        pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), _T("nologo"));
    }

    return false;
}

static bool ParseCommandLine(
    CGT_CDiagMsg* pdm,
    ICG_IECFront* pIFront, 
    CMDL_OPT_TYP* pOpts,
    int           argc, 
    TCHAR**       argv
)
{
    int              i;
    int              is;
    const TCHAR*     psz;
    const TCHAR*     psz2;
    FC_CString       jot;
    CG_Edipos        cmdl;
    

    //init cmdl to show error location 'command line'
    CG_InitCMDLEdipos(&cmdl);


    for(i=0;i<argc;i++)
    {
        psz = argv[i]; 
        if(*psz==_T('/') || *psz==_T('-'))
        {
            psz++;
            if((is = FC_StringStartsWithI(psz, _T("c:"))) >0)
            {
                if(i+1>=argc || !psz[is])
                {
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, _T("c:"));
                }
                else if(!pOpts->szCheckFile.isEmpty())
                {
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), _T("c"));
                }
                else
                {
                    pOpts->szCurTrgTyp.load(&psz[is]);
                    i++;
                    pOpts->szCheckFile.load(argv[i]);
                }
            }
            else if( (is = FC_StringStartsWithI(psz, _T("ml"))) >0)
            {
                if(!pOpts->szMakeLib.isEmpty())
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else if(psz[is]==_T(':'))
                    pOpts->szLibTrgType.load(&psz[is+1]);    
                
                if(++i>=argc)
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, psz);
                else if(pOpts->szMakeLib.isEmpty())
                {
                    pOpts->szMakeLib.load(argv[i]);
                    if(lstrcmpi(FC_StringGetFileExt(argv[i]), CG_PREDEF_LIB_EXT))
                        pdm->msg3(CG_E_BAD_CMDL_FILETYPE, &cmdl, _T("ml"), argv[i], CG_PREDEF_LIB_EXT);
                }
            }
            else if(!lstrcmpi(psz, _T("i")))
            {
                if(pOpts->bIncrementelly)
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else
                    pOpts->bIncrementelly = true;
            }
            else if(!lstrcmpi(psz, _T("nologo")))
            {
                ;//nologo already handeled, -? must not pass here any more ...
            }
            else
            {
                pdm->msg1(CG_E_CMDL_UNKNOWN_OPT, &cmdl, --psz);
            }
        }   
        else
        { 
            //pure file argument, must be either 4CP or 
            //build target by name:
            psz2 = FC_StringGetFileExt(psz);
            if(!lstrcmpi(psz2, CG_4CP_EXTENSION))
            {
                if(pOpts->sz4CP[0])
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("."), psz2);
                else
                    pOpts->sz4CP.load(psz);
            }
            else if(pOpts->targets.getUsed()>0)
            {
                pdm->errNS1(&cmdl, pdm->res1(CG_RES_STR_CMD1));
            }
            else
            {
                pOpts->targets.add(psz);
                psz2 = FC_StringGetFileExt(psz);
                pOpts->szCurTrgTyp.load(psz2);
            }
        }
    }


    //check option combinations got end after first error
    //in order to produce only one error (the user is frustrated anyway...):
    if(pdm->hasErr())
        goto Ende;

    if(pOpts->sz4CP.isEmpty())
    {
        pdm->msg0(CG_E_CMDL_NO4CP, &cmdl);
        goto Ende;
    }


    if (!pOpts->szMakeLib.isEmpty())
    {
        if ( ! pOpts->szCurTrgTyp.isEmpty()
            && _tcsicmp(pOpts->szCurTrgTyp, pOpts->szLibTrgType))
        {
            //-ml cannot be together with <target file> of other target type:
            pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, 
                pdm->res1(CG_RES_STR_CMD2)<<_T("'")<<pOpts->szCurTrgTyp<<_T("'"),
                pdm->res2(CG_RES_STR_CMD3)<<_T("-ml:")<<pOpts->szLibTrgType
            );
            goto Ende;
        }
        else
        {
            pOpts->szCurTrgTyp.load(pOpts->szLibTrgType);
        }
    }

    if(!pOpts->szCheckFile.isEmpty() && !pOpts->szMakeLib.isEmpty())
    {
        //-c check file cannot be together with -ml:
        pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-c"), _T("-ml")); 
        goto Ende;
    }

    if(!pOpts->szCheckFile.isEmpty() && pOpts->targets.getUsed()>0)
    {
        //-c check file cannot be together with build target by name
        pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-c"), pOpts->targets.get(0)); 
        goto Ende;
    }

    return true;

Ende:
    //if any cmdl error dump hint how to get help (but not if back end..):
    //if(pdm->getErr())
    //{
    //    pdm->msg0(CG_E_BAD_CMDL_LINE, NULL);
    //    return false;
    //}
    
    return false;
}


//version Dependent KAD:
/**
 * GetTargetVersion. Reads a target file and get it's firmware version.
 * @param           pdm        IN diag message sink
 * @param           pszPrjPath IN the project path
 * @param           pszTrgFile IN that target file name
 * @return          unsigned long: the firmware version found, 0 on error
*/
static unsigned long GetTargetVersion(
    CGT_CDiagMsg*  pdm,
    const TCHAR*   pszPrjPath,
    const  TCHAR*  pszTrgFile
)
{
    IXMLDOMNode*        pNode;
    unsigned long       ulTrgVer;
    FC_CString          control(MAX_PATH);

    //open xml file:
    pNode = CGT_XMLLoadFile(pdm, pszPrjPath, pszTrgFile, NULL);
    if(!pNode)
        return 0;
    
    //next three attr are optional:
    CGT_XMLGetStrAttrTrim(pdm, pNode, L"CONTROL", &control, true, true);

    TCHAR szFirmwareVersion[UTIL_MAX_FIRMWARE_VERSION_LEN+1];
    szFirmwareVersion[0] = 0;
    
    if(UTIL_ParseControlString(
        control, 
        NULL, // pszOPCServer 
        NULL, // pszCustomConnectGuid 
        NULL, // pszOPCInstName
        NULL, // pszCommChannel 
        NULL, // pszConnectPar
        NULL, // pszProtocol
        NULL, // pszProtocolPar
        szFirmwareVersion) != S_OK)
    {
        //TODO diag ?
    }

    if(szFirmwareVersion[0])
    {
        TCHAR* pszEnd;
        ulTrgVer = _tcstoul(szFirmwareVersion, &pszEnd, 10);
        //TODO: diag ?
        assert(ulTrgVer>=20000 && *pszEnd==0);
    }
    else
    {
        //version "20000" has no VERSION(), set default:
        ulTrgVer = 20000;
    }

    FC_RELEASE_PTR(pNode);
    return ulTrgVer;
}
//version Dependent KAD:
/**
 * GetVersionDependentKADFileName. Gets the version dependent compiler KAD.
 * It uses the command line, if there is a target file it's firmware version 
 * is read and then using the <target type>_Addon.kad the final name of the kad is 
 * searched. If no target or no speciffic kad is specified in <target type>_Addon.kad
 * the default kad <<target type>_4cvm.kad is assumed.
 * @param           pdm         IN diag message sink
 * @param           pOpt        IN the command line arguments
 * @param           kadFile     OUT  receives the kad file name and full path
 * @param           ulTrgVer    OUT receive firmware version
 * @return          void
*/
static void GetVersionDependentKADFileName(
    CGT_CDiagMsg*       pdm,
    const CMDL_OPT_TYP* pOpt,
    FC_CString&         kadFile,
    unsigned long&      ulTrgVer
)
{
    FC_CString      addonKad(MAX_PATH);
    FC_CString      prjPath(MAX_PATH);
    FC_CString      binPath(MAX_PATH);
    IXMLDOMNode*    pNode;
    IXMLDOMNodeList*pNList;
    long            len;
    FC_CString      jot;
    long            i;

	ulTrgVer = 0;
    kadFile.clear();

    binPath.load(g_szModuleName).stripFilePart();
    prjPath.load(pOpt->sz4CP).stripFilePart();

    //if there is a target file or check file given in the command line get the target version
	if(!pOpt->szCheckFile.isEmpty())
	{
		//TODO: Workaround: passing an optional target version number via the command line
		//is not possible because we may have very old backends, we pass this info
		//inside a file with fixed name, old backend simple don't read it.
		//If we make a new major version all backends should add this to there cmd line
		//and then we don't need this tmp file any more...
		FC_CString sCheckInfo(MAX_PATH);
		sCheckInfo.load(prjPath)<<_T("\\gen\\checkinf.tmp");
		ulTrgVer = GetPrivateProfileInt(_T("target"), _T("version"), 0, sCheckInfo);
	}
	else if(pOpt->targets.getUsed()>0)
	{
        ulTrgVer = GetTargetVersion(pdm, prjPath, pOpt->targets.get(0));
        //Note: if more than one target is specified CGlobCtx::loadTargets() will 
        //later treat this as an "error not supported multiple targets". 
        //This solution here is only possible if at max one target at a time is built!
	}


    if(ulTrgVer>0)
    {
        //Addon KAD = "<installdir>\bin\<trgtyp>_Addon.kad"
        addonKad<<binPath<<'\\'<<pOpt->szCurTrgTyp<<_T("_Addon.kad");
        if(FC_Exist(addonKad))
        {
            //open xml file:
            pNode = CGT_XMLLoadFile(pdm, NULL, addonKad, NULL);
            if(pNode)
            {
                // read targets
                pNList = NULL; len = 0;
                pNode->selectNodes(L"FIRMWARE", &pNList);
                FC_RELEASE_PTR(pNode);
                pNList->get_length(&len);
                for(i=0;i<len;i++)
                {
                    pNList->get_item(i, &pNode);
                    if(CGT_XMLGetStrAttrTrim(pdm, pNode, L"VERSION", &jot, false, false))
                    {
                        if(strtoul(jot, NULL, 10)==ulTrgVer)
                        {
                            CGT_XMLGetStrAttrTrim(pdm, pNode, L"COMPILERKAD", &kadFile, false, true);
                            FC_RELEASE_PTR(pNode);
                            break;
                        }
                    }
                    FC_RELEASE_PTR(pNode);
                }
            }
        }//<targettyp>_addon.kad does not exist, this is for backward compatibillity (4cec!)
    }//else: maybe check file or build lib, we have no version info!! use default KAD


    if(kadFile.isEmpty())
    {//Not found in AddonKAD with version:
        //default compiler kad = "<installdir>\bin\<trgtyp>_4cvm.kad"
        kadFile<<binPath<<'\\'<<pOpt->szCurTrgTyp<<_T("_4cvm.kad");
    }
    else
    {//Found in AddonKAD with version:
        //make an absolute path:
        kadFile.prepend(binPath, "\\");
    }
}





static int __cdecl CGPCmain(int argc, TCHAR** argv)
{
    int            nErrors;
    ICG_IECFront*  pIFront;
    CGlobCtx*      pgc;
    HRESULT        hr;
    FC_CString     jot;
    const TCHAR*          pszGen;
    const CG_POU4CPInfo** pzpProcessed;

    //core init:
    //get a front end:
    hr = CoCreateInstance(CLSID_CG_IECFront,
                          NULL, 
                          CLSCTX_INPROC_SERVER,
                          IID_ICG_IECFront, 
                          (void**)&pIFront);
    if(FAILED(hr))
    {
        _tprintf(CG_FATAL_MSGFMT,__FILE__,__LINE__,_T("failed to create front end COM instance"));
        return 1;
    }

    //new global context:
    pgc = new CGlobCtx(pIFront);



    //check if only command line help wanted:
    if(DumpLogoAndHelp(&pgc->m_dm, argc, argv))
        goto Ende;

    //init front end: loadKAD->loadFeatures->load4CP->load4CL(NULL)->loadCST(NULL)
    //Load the default KAD:
    pIFront->loadKAD(NULL);


    //Parse command line:
    if(pgc->m_dm.hasNoErr())
    {
        ParseCommandLine(&pgc->m_dm, pgc->m_pIF, pgc->getWrtOpts(), argc, argv);
    }


    //load the KAD for this module:
    if(pgc->m_dm.hasNoErr())
    {
        unsigned long ulTrgVer;
        GetVersionDependentKADFileName(&pgc->m_dm, pgc->m_popt, jot, ulTrgVer);
        //ulTrgVer = 21040;
        pgc->m_kad.load(&pgc->m_dm, jot, pgc->m_popt->szCurTrgTyp, ulTrgVer);
    }


    //Load features:
    if(pgc->m_dm.hasNoErr())
    {
        //set the target type name for the not supported diags:
        assert(!pgc->m_popt->szCurTrgTyp.isEmpty());
        pgc->m_dm.setNotSupportedName(pgc->m_popt->szCurTrgTyp);

        //init feature set in front end:
        SetFeatures(pIFront, &pgc->m_kad, pgc->m_popt->szCurTrgTyp);
    }


    //load the 4CP, if no error so far popts->sz4CP is set:
    if(pgc->m_dm.hasNoErr())
        pIFront->load4CP(pgc->m_popt->sz4CP);


    pszGen = pIFront->FCPgetGenPath();
    if(!pszGen || !pszGen[0])
        goto Ende;//no response file!! load4CP is the prerequisite to get 
                  //the gen-path, without gen path info we cannot dump the 
                  //response file!


    //get 4CP options
    if(pgc->m_dm.hasNoErr())
        pgc->getOptionsFrom4CP();

    //load all libs referenced in 4CP:
    if(pgc->m_dm.hasNoErr())
        pIFront->loadLIBs();

    //init lic info (after loadLibs!):
    if(pgc->m_dm.hasNoErr())
        pgc->initLicenseInfo(pIFront->FCPgetAllLIBs(), pgc->m_popt->szCurTrgTyp);

    if(pgc->m_dm.hasNoErr())
        pgc->checkSourceProtection();

    //load all CONST of project and all libs:
    if(pgc->m_dm.hasNoErr())
        pIFront->loadCSTs();

    //Initialisation complete, now check the main processing options:
    if(pgc->m_dm.hasNoErr())
    {
        if(pgc->m_popt->szCheckFile.isEmpty())
        {
            //do a rebuild -i incrementell not supported yet:
            //call a sequence of methods of CGlobCtx: 
            if(pgc->getBPOption()==BP_OPT2)
                pgc->m_dm.userMsg(pgc->m_dm.res1(CG_RES_STR_CMD_MSG));

            pgc->rebuildAll();
        }
        else
        {
            //only check a single source file:
            pgc->checkFile();
        }
    }
 

    if(FC_Exist(pgc->m_popt->sz4CP))
    {
        pzpProcessed = pgc->getProcessed4CPFiles();
        assert(pIFront->FCPgetGenPath() && pIFront->FCPgetGenPath()[0]);
        DumpResponseFile(&pgc->m_dm, pgc->m_popt->szCurTrgTyp, pIFront, pzpProcessed);
        delete(pzpProcessed);
    }//else: do not if no 4cp file, this may happen if you call the backend by hand
     //with a typo in the 4cp file or path, typically this happens if you debug the
     //backend. If called from 4cg the 4cp will always exist
     //TODO:  make a cmdl option if or if not dump a response file, ans also the file name and
     //path should be given on the cmdl by 4cg, this will also give the info in we are called
     //embedded or not, if called embedded and there is a cmdl error we can dump the cmdl 
     //to stdout in order to give the user a hint, if called from dos box the cmdl is visible 
     //anyway...

Ende:
    nErrors = pgc->m_dm.getDisplayedErrors();
    delete(pgc);

    jot.load(pIFront->FCPgetPrjPath())<<"\\"<<pIFront->FCPgetPrjName()<<_T(".4cp");
    hr = pIFront->Release();
    assert(hr==0);
    
    LCT_DumpResult(jot);

    return nErrors;
}



int __cdecl _tmain(int argc, TCHAR** argv)
{
    int nErrors;

    //init the static g_szModuleName:
    if(GetModuleFileName(NULL, g_szModuleName, FC_ARRAY_LEN(g_szModuleName))==0)
    {
	   FC_ASSERT_LASTERR(_T("GetModuleFileName"));
       return 1;
    }

    //do some core init:
    InitModuleCheckMemory(MODULE_NAME);

    if(FAILED(CoInitialize(NULL)))
    {
        _tprintf(CG_FATAL_MSGFMT,__FILE__,__LINE__,_T("CoInitialize failed"));
        nErrors = 1;
        goto Ende;
    }


    //always remove the module name.
    argv++;
    argc--;

    //get internal debug switches:
    if(argc>0 && !lstrcmpi(_T("-!bug"), argv[0]))
    {
        argv++;argc--;
        if(argc>0 && !lstrcmpi(_T("heap"), argv[0]))
        {
            argv++;argc--;
            if(argc>0)
            {
                FC_SetAllocBreak(_tcstoul(argv[0], NULL, 10));
                argv++;argc--;
            }
        }
    }


    //TODO get from registry
    if(!SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT)))
    {
        FC_ASSERT_LASTERR("SetThreadLocale()");
        //don't care any more...
    }

    //enter main backend routine:
    nErrors = CGPCmain(argc, argv);
    
    CoUninitialize();

Ende:
    ExitModuleCheckMemory(MODULE_NAME);
    return nErrors?1:0;
}

/* ---------------------------------------------------------------------------- */
