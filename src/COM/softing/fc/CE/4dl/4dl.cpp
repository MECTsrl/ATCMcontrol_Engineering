
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include "helper.h"


int g_bForceShutdown = FALSE;
static BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    g_bForceShutdown = TRUE;
    _tprintf("\n4dl: error: received ctrl-c: cancel operation...\n");
    _tprintf("^C");
    return TRUE;
}







int main(int argc, char* argv[])
{
    int                        i;
    HRESULT                    hr;
    IFCSessions*               pISessions = NULL;
    bool                       bOk = false;
    FC_CString                 jot;
    FC_CStrList                deleteFileList;
    CMDL_INPUT                 cmdl;


    if(DumpLogoAndHelp(argc, argv))
        return 0;
    
    if(!ParseCmdLine(argc, argv, &cmdl))
        return 1;


    //need COINIT_MULTITHREADED, otherwise IFCSession object won't work:
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if(FAILED(hr))
    {
        _tprintf("4dl: fatal error: CoInitialize failed: hr=%x\n" , hr);
        goto error;
    }
    // This provides a NULL DACL which will allow access to everyone.
    hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    if(FAILED(hr))
    {
        _tprintf("4dl: fatal error: CoInitializeSecurity failed: hr=%x\n" , hr);
        goto error;
    }

    InitModule();

    //create the sessions interface
    hr = CoCreateInstance(CLSID_FCSessions, NULL, CLSCTX_ALL, IID_IFCSessions, (void**)&pISessions);
    if(FAILED(hr)) 
    {
        _tprintf("4dl: fatal error: CoCreateInstance for CLSID_FCSessions failed: hr=%x\n", hr);
	    goto error;
    }

    SetConsoleCtrlHandler(HandlerRoutine, TRUE);

    if(cmdl.bMergeOnly && cmdl.destDirMerged.isEmpty())
        cmdl.destDirMerged.load(_T(".")); //HACK otherwise .merged gets deleted..



    if(cmdl.safe4PL.getUsed()>0)
    {
        const TCHAR* pszTrgName = cmdl.trgNames.get(0);
        const TCHAR* pszTrgAddr = cmdl.connectStr.get(0);
        const TCHAR* psz4PL;
        
        CConnection  cnt(pISessions, pszTrgName, pszTrgAddr, cmdl.pszPassWord);

        if(cmdl.nVerbose>=1)
            _tprintf(_T("4dl: safe parameter: stop resource\n"));

        long         lKey;
        IControlMgt* pCtrlMgt = cnt.getCtrlMgt(&lKey);
        if(!pCtrlMgt)
            goto error;
        hr = pCtrlMgt->PauseResource(lKey);
        if(FAILED(hr))
        {
            _tprintf(_T("4dl: error: stop resource failed: hr=0x%x\n"), hr);
            goto error;
        }

        for(i=0; i<cmdl.safe4PL.getUsed();i++)
        {
            if(g_bForceShutdown)
                goto error;

            psz4PL = cmdl.safe4PL.get(i);
            if(cmdl.uploadDestDir.isEmpty())
                jot.load(psz4PL)<<_T(".tmp");
            else
                jot.load(cmdl.uploadDestDir)<<_T("\\")<<psz4PL;

            int iret = UploadParameter(&cnt, psz4PL, jot, cmdl.nVerbose, cmdl.nMaxGS);
            if(iret>0)
            {
                cmdl.download4PL.add(jot);
                if(cmdl.uploadDestDir.isEmpty())
                    deleteFileList.add(jot);
            }
            else if(iret==0)
            {
                 _tprintf(_T("4dl: warning safe '%s': no such parameters in running control\n"), psz4PL);
            }
            else
            {
                goto error;
            }
        }
    }


    if(!cmdl.upload4PL.isEmpty())
    {
        const TCHAR* pszTrgName = cmdl.trgNames.get(0);
        const TCHAR* pszTrgAddr = cmdl.connectStr.get(0);
        long         lKey = 0;
        IControlMgt* pCtrlMgt = NULL;
        
        CConnection  cnt(pISessions, pszTrgName, pszTrgAddr, cmdl.pszPassWord);
 
        if(cmdl.uploadDestDir.isEmpty())
            jot.load(cmdl.upload4PL);
        else
            jot.load(cmdl.uploadDestDir)<<_T("\\")<<cmdl.upload4PL;

        if(g_bForceShutdown)
            goto error;


        if(cmdl.bStopTasksForUpload)
        {
            if(cmdl.nVerbose>=1)
                _tprintf(_T("4dl: upload parameter: stop all tasks\n"));

            pCtrlMgt = cnt.getCtrlMgt(&lKey);
            if(!pCtrlMgt)
                goto error;
            hr = pCtrlMgt->StopAllTasksOnResource(lKey);
            if(FAILED(hr))
            {
                _tprintf(_T("4dl: error: stop all tasks failed: hr=0x%x\n"), hr);
                goto error;
            }
        }

        int iret = UploadParameter(&cnt, cmdl.upload4PL, jot, cmdl.nVerbose, cmdl.nMaxGS);

        
        if(cmdl.bStopTasksForUpload && pCtrlMgt)
        {
            if(cmdl.nVerbose>=1)
                _tprintf(_T("4dl: upload parameter: start all tasks\n"));

            hr = pCtrlMgt->StartAllTasksOnResource(lKey);
            if(FAILED(hr))
            {
                _tprintf(_T("4dl: error: start all tasks failed: hr=0x%x\n"), hr);
                goto error;
            }
        }
        
        
        if(iret<1)
        {
            if(iret==0)
            {
                _tprintf(_T("4dl: error: upload '%s': no such parameters in running control\n"), 
                    cmdl.upload4PL.psz());
            }
            goto error;
        }
    }

    if(cmdl.bDownload)
    {
        for(i=0;i<cmdl.trgNames.getUsed();i++)
        {
            FC_CStrList  paramFilesMerged;
            const TCHAR* pszTrgName = cmdl.trgNames.get(i);
        
            if(g_bForceShutdown)
                goto error;

            if(!MergeTargetParameterFiles(pszTrgName, cmdl.loaderLists.get(i), 
                 &cmdl.download4PL, &paramFilesMerged, 
                 cmdl.bMergeWarn, cmdl.nVerbose, cmdl.destDirMerged, &deleteFileList))
                goto error;

            if(cmdl.bMergeOnly)
                continue;

            CConnection  cnt(pISessions, pszTrgName, cmdl.connectStr.get(i), cmdl.pszPassWord);
            if(!cnt.openCommissioningSession())
                goto  error;
            if(!DownloadControl(&cnt, &cmdl, &paramFilesMerged, cmdl.nVerbose, cmdl.nMaxGS))
                goto  error;


            if(cmdl.safe4PL.getUsed()>0)
            {
                if(cmdl.nVerbose>=1)
                    _tprintf(_T("4dl: safe parameter: start resource\n"));

                long         lKey;
                IControlMgt* pCtrlMgt = cnt.getCtrlMgt(&lKey);
                if(!pCtrlMgt)
                    goto error;
                hr = pCtrlMgt->ContinueResource(lKey);
                if(FAILED(hr))
                {
                    _tprintf(_T("4dl: error: start resource failed: hr=0x%x\n"), hr);
                    goto error;
                }
            }
        }

        if(cmdl.bMergeOnly)//HACK
            _tprintf(_T("4dl: merge completed successfully\n"));
        else
            _tprintf(_T("4dl: download completed successfully\n"));
    }

    bOk = true;
error:
    if (pISessions)
    {
        pISessions->Shutdown_Gracefully();
        pISessions->Release();
    }
    CoUninitialize();
    
    for(i=0; i<deleteFileList.getUsed(); i++)
    {
        if(!FC_Exist(deleteFileList.get(i)))
            continue;
        if(cmdl.nVerbose>=1)
            _tprintf(_T("4dl: delete file '%s'\n"), deleteFileList.get(i));
        unlink(deleteFileList.get(i));
    }

    return bOk ? 0 : 1;
}

