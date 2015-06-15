//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include <stdio.h>
#include "helper.h"
#include "fc_todebug\fc_mem.h"
#include "globctx.h"
#include "4cpc.h"
#include "newdel.h"

#include "BuildNr.h"
#include "Product.h"
#include "CG_IECFront_i.c" //define the guids from front end


//file and full path of exe:
static TCHAR g_szModuleName[MAX_PATH];

const TCHAR* GetModuleName()
{
    return g_szModuleName;
}

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
        psz = _T(PROD_PRODUCTNAME) _T(" IEC 1131-3 Compiler PC backend ")
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
    const TCHAR*     pszMLAL;
    int              oldErr;
    

    oldErr = pdm->getTotalErrors();

    //init cmdl to show error location 'command line'
    CG_InitCMDLEdipos(&cmdl);


    for(i=0;i<argc;i++)
    {
        psz = argv[i]; 
        if(*psz==_T('/') || *psz==_T('-'))
        {
            psz++;
            if(!lstrcmpi(psz, _T("c")))
            {
                if(!pOpts->szCheckFile.isEmpty())
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                
                if(++i>=argc)
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, psz);
                else
                    pOpts->szCheckFile.load(argv[i]);
            }
            else if((is = FC_StringStartsWithI(psz, _T("c:"))) >0)
            {
                if(i+1>=argc)
                {
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, _T("c:"));
                }
                else if(!pOpts->szCheckFile.isEmpty())
                {
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), _T("c"));
                }
                else
                {
                    //check but ignore any target type, must be 4cpc:
                    if(psz[is] && stricmp(&psz[is], TARGET_EXT))
                        pdm->msg1(CG_E_CMDL_UNKNOWN_OPT, &cmdl, --psz);

                    i++;
                    pOpts->szCheckFile.load(argv[i]);
                }
            }
            else if( (is = FC_StringStartsWithI(psz, _T("ml"))) >0)
            {
                //-ml
                if(pOpts->bAppendLib)
                    pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-ml"), _T("-al")); 
                else if(!pOpts->szMakeLib.isEmpty())
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else if(psz[is]==_T(':'))
                {
                    jot.load(&psz[is+1]);
                    assert( ! _tcsicmp(jot, TARGET_EXT));
                }
                
                if(++i>=argc)
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, psz);
                else if(pOpts->szMakeLib.isEmpty())
                {
                    pOpts->szMakeLib.load(argv[i]);
                    if(lstrcmpi(FC_StringGetFileExt(argv[i]), CG_PREDEF_LIB_EXT))
                        pdm->msg3(CG_E_BAD_CMDL_FILETYPE, &cmdl, _T("ml"), argv[i], CG_PREDEF_LIB_EXT);
                }
            }
            else if( (is = FC_StringStartsWithI(psz, _T("al"))) >0)
            {
                //-al[:pc]
                if(pOpts->bAppendLib)
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else if(!pOpts->szMakeLib.isEmpty())
                    pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-ml"), _T("-al")); 
                else if(psz[is]==_T(':'))
                {
                    jot.load(&psz[is+1]);
                    assert( ! _tcsicmp(jot, TARGET_EXT));
                }
                
                if(++i>=argc)
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, psz);
                else if(pOpts->szMakeLib.isEmpty())
                {
                    pOpts->szMakeLib.load(argv[i]);
                    pOpts->bAppendLib = true;
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
            else if(pOpts->targets.findI(psz))
            {
                pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T(""), psz);
            }
            else
            {
                pOpts->targets.add(psz);
            }
        }
    }


    //check option combinations goto end after first error
    //in order to produce only one error (the user is frustrated anyway...):
    if(pdm->hasErr())
        goto Ende;

    if(pOpts->sz4CP.isEmpty())
    {
        pdm->msg0(CG_E_CMDL_NO4CP, &cmdl);
        goto Ende;
    }


    if(!pOpts->szCheckFile.isEmpty() && !pOpts->szMakeLib.isEmpty())
    {
        //-c check file cannot be together with -al or -ml:
        pszMLAL = pOpts->bAppendLib ? _T("-al") : _T("-ml");
        pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-c"), pszMLAL); 
        goto Ende;
    }

    if(!pOpts->szCheckFile.isEmpty() && pOpts->targets.getUsed()>0)
    {
        //-c check file cannot be together with build target by name
        pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-c"), pOpts->targets.get(0)); 
        goto Ende;
    }

    if(pdm->hasNoErr() && pOpts->targets.getUsed()>0)
    {
        //check target names in respect 
        //to target type extension:
        for(i=0;i<pOpts->targets.getUsed();i++)
        {
            psz = pOpts->targets.get(i);
            psz2 = FC_StringGetFileExt(psz);
            if(lstrcmpi(psz2, TARGET_EXT))
                pdm->msg2(CG_E_NO_TARGETTYPE_EXT, &cmdl, psz, TARGET_EXT);
        }
    }


Ende:
    //if any cmdl error dump hint how to get help (but not if back end..):
    //if(pdm->getErr())
    //{
    //    pdm->msg0(CG_E_BAD_CMDL_LINE, NULL);
    //    return false;
    //}
    return oldErr == pdm->getTotalErrors();
}









static int __cdecl CGPCmain(int argc, TCHAR** argv)
{
    int                   nErrors;
    ICG_IECFront*         pIFront;
    CGlobCtx*             pgc;
    HRESULT               hr;
    FC_CString            jot;
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

    //load the KAD for this module:
    jot.load(g_szModuleName).changeFileExt(_T("kad"));
    pgc->m_kad.load(&pgc->m_dm, jot);

    //check if only command line help wanted:
    if(DumpLogoAndHelp(&pgc->m_dm, argc, argv))
        goto Ende; //no response file

    //init front end: loadKAD->loadFeatures->load4CP->load4CL(NULL)->loadCST(NULL)
    //Load the default KAD:
    if(pgc->m_dm.hasNoErr())
        pIFront->loadKAD(NULL);

    //Load features:
    if(pgc->m_dm.hasNoErr())
        SetFeatures(pIFront);

    //Parse command line:
    if(pgc->m_dm.hasNoErr())
        ParseCommandLine(&pgc->m_dm, pgc->m_pIF, pgc->getWrtOpts(), argc, argv);

    //load the 4CP, if no error so far popts->sz4CP is set:
    if(pgc->m_dm.hasNoErr())
        pIFront->load4CP(pgc->m_popt->sz4CP);

    pszGen = pIFront->FCPgetGenPath();
    if(!pszGen || !pszGen[0])
        goto Ende;//no response file!! load4CP is the prerequisite to get 
                  //the gen-path, without gen path info we TODO cannot dump the 
                  //response file!

    if(pgc->m_dm.hasNoErr())
        SetCurrentDirectory(pIFront->FCPgetPrjPath());

    if(pgc->m_dm.hasNoErr())
        pgc->getOptionsFrom4CP();

    //load all libs referenced in 4CP:
    if(pgc->m_dm.hasNoErr())
        pIFront->loadLIBs();

    //init lic info (after loadLibs!):
    if(pgc->m_dm.hasNoErr())
        pgc->initLicenseInfo(pIFront->FCPgetAllLIBs());

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
                pgc->m_dm.userMsg(_T("   - generating debug code -\r\n"));

            pgc->rebuildAll();
        }
        else
        {
            //only check a single source file:
            pgc->checkFile();
        }
    }
 

    pzpProcessed = pgc->getProcessed4CPFiles();
    assert(pIFront->FCPgetGenPath() && pIFront->FCPgetGenPath()[0]);
    DumpResponseFile(&pgc->m_dm, TARGET_EXT, pIFront, pzpProcessed);
    delete(pzpProcessed);

Ende:
    nErrors = pgc->m_dm.getDisplayedErrors();
    delete(pgc);
    hr = pIFront->Release();
    assert(hr==0);
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
    InitModuleCheckMemory(TARGET_EXT);

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
    ExitModuleCheckMemory(TARGET_EXT);
    return nErrors?1:0;
}


