#include <stdio.h>
#include "helper.h"
#include "pumplib.h"
#include "fc_todebug\fc_mem.h"
#include "newdel.h"
#include "cgt_misc.h"
#include "cmdl.h"
#include "cgframe.h"
#include "prjdbi.h"
#include "defines.h"



//file and full path of exe:
static TCHAR g_szModuleName[MAX_PATH];
const TCHAR* GetModuleName()
{
    return g_szModuleName;
}

int CGmain(int argc, TCHAR** argv)
{
    HRESULT                hr;
    int                    nErrors;
    int                    nWanings;
    ICG_IECFront*          pIFront;
    CGT_CDiagMsg*          pdm;
    CMDL_OPTIONSTYP        opts;

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

    pdm = new  CGT_CDiagMsg(MODULE_NAME, NULL); 
    pdm->setFront(pIFront);

    //init front end: loadKAD->loadFeatures->load4CP->load4CL(NULL)->loadCST(NULL)
    //Load the default KAD:
   
    pIFront->loadKAD(NULL);

    //Load features:
    if(pdm->hasNoErr())
        pIFront->loadFeatures(NULL);

    //Parse command line:
    if(pdm->hasNoErr())
        ParseCommandLine(pdm, pIFront, &opts, argc, argv);

    if(pdm->hasNoErr() && opts.sz4CP.isEmpty())
        SetCurrentDirectory(opts.sz4CPFullPath);

    //get 4cp info, we need this before we can do a meaningfull 'what is going on message':
    //This has changed since now the lib target type can be also set as [compiler] lib.trg option.
    if(!opts.sz4CP.isEmpty())
    {
        if(pdm->hasNoErr())
            pIFront->load4CP(opts.sz4CP);

        if(pdm->hasNoErr())
            Check4CPNeedVersion(pdm, pIFront);

        if(pdm->hasNoErr())
            Check4CPCompilerSec(pdm, pIFront, &opts);

        if(pdm->hasNoErr())//auto build libs from source even before custom buils steps... 
            AutoBuildLibs(pdm, pIFront, &opts);
    }

 
    //msg to user what is going on, if recursive the caller has a better idea 
    //what is usefull to display...:
    if(pdm->hasNoErr() && (opts.nVerbose > (opts.bRecursive?1:0)))
    {
        FC_CString msgtxt(MAX_PATH);
        if(!opts.szCheckFile.isEmpty())
        {
			//message done in CheckFile 
        }
        else if(opts.libAct==LIB_MAKE)
        {
            msgtxt<<(_T("build library '"))<<opts.szMakeLib<<_T("' ");
            if(!opts.szLibTargetType.isEmpty())
                msgtxt<<'('<<opts.szLibTargetType<<')';
        }
        else if(opts.libAct==LIB_MAKE_STUB)
        {
            msgtxt<<(_T("build stub library '"))<<opts.szMakeLib<<_T("'");
        }
        else if(opts.libAct==LIB_ACT_UNDEF)
        {
            int nTrgs = opts.targets.getUsed();
            if(nTrgs>0)
            {
                msgtxt.load(nTrgs==1 ? _T("build target ") : _T("build targets "));
                for(int i=0;i<nTrgs;i++)
                {
                    if(i>0)msgtxt<<' ';
                    (msgtxt<<opts.targets.get(i)).changeFileExt(NULL);
                }
            }
            else
            {
                msgtxt<<(_T("build "))<<FC_StringGetFilePart(opts.sz4CP);
            }
        }

        if(!msgtxt.isEmpty())
        {
            msgtxt<<_T(":\n");
            pdm->userMsg(msgtxt);
        }
    }

    if(!opts.sz4CP.isEmpty())
    {
        //cleanup gen?
        if(opts.isRebuildProject())
            CleanUpGenDir(pdm, pIFront);

        if(pdm->hasNoErr() && opts.szCheckFile.isEmpty())
            ExecCustomBuildSteps(pdm, pIFront, opts.nVerbose, true);
    }


    //Initialisation complete, now check the main processing options:
    if(pdm->hasNoErr())
    {
        if(!opts.szCheckFile.isEmpty())
        {
            CheckFile(pdm, pIFront, &opts);
        }
        else if(opts.libAct==LIB_MAKE)
        {
            //get all targets of type  -ml:<trg type>
            //Note that buildiing a lib is also the same 
            //as building the project for this trg type if there is
            //a config and targets typically used as 'test code':
            FC_CGArray<TargetAndType> targets;
            GetAllTargets(pdm, pIFront, &targets, opts.szLibTargetType);
            MakeLibrary(pdm, pIFront, &targets, &opts);
            SortTargetsByName(&targets);            // 23.08.06 SIS: for watch selection
            DumpProjectDBI(pdm, pIFront, &targets);

            //post build commands, only if no error:
            if(pdm->hasNoErr())
                ExecCustomBuildSteps(pdm, pIFront, opts.nVerbose, false);
        }
        else if(opts.libAct==LIB_MAKE_STUB)
        {
            //only create stub lib:
            MakeLibraryStub(pdm, pIFront, &opts);
        }
        else if(opts.libAct==LIB_LIST ||opts.libAct==LIB_LISTLONG)
        {
            ListLibrary(pdm, pIFront, &opts);
        }
        else if(opts.libAct==LIB_EXTRACT)
        {
            ExtractLibrary(pdm, pIFront, &opts); //undocumented feature...
        }
        else if(opts.libAct==LIB_APPEND)
        {
            //merge two libs:
            AppendLibrary(pdm, pIFront, &opts);
        }
        else if(opts.libAct==LIB_REMOVE)
        {
            RemoveLibTarget(pdm, pIFront, &opts);
        }
        else
        {
            //build or rebuild the project's targets or if 
            //pOpts->targets.getUsed()>0 only a subset of them
            //In any case get first all targtes (also the nested)
            //and sort them according to the target type.
            FC_CGArray<TargetAndType> targets;
            GetAllTargets(pdm, pIFront, &targets, NULL);
            BuildTargets(pdm, pIFront, targets, &opts);
            SortTargetsByName(&targets);        // 23.08.06 SIS: for watch selection
            DumpProjectDBI(pdm, pIFront, &targets);
            //post build commands, only if no error:
            if(pdm->hasNoErr())
                ExecCustomBuildSteps(pdm, pIFront, opts.nVerbose, false);
        }
    }

    nErrors = pdm->getDisplayedErrors();
    nWanings= pdm->getDisplayedWarnings();
    if(opts.libAct!=LIB_LIST && opts.libAct!=LIB_LISTLONG && !opts.bRecursive)
    {
        pdm->userMsg(pdm->jot1().fmtMsg2(FCG_I_FINAL_ERR_SUM, NULL, 
            pdm->jot2()<<nErrors, pdm->jot3()<<nWanings));
    }
    delete(pdm);
    hr = pIFront->Release();
    assert(hr==0);

    return nErrors;
}




int __cdecl _tmain(int argc, TCHAR** argv)
{
    int nErrors;

    //check if only command line help wanted:
    if(DumpLogoAndHelp(argc, argv))
        return 0;

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

    //enter main routine:
    nErrors = CGmain(argc, argv);
    
    CoUninitialize();

Ende:
    ExitModuleCheckMemory(MODULE_NAME);
    return nErrors;
}



















