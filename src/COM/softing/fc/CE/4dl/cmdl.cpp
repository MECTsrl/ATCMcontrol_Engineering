

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include "helper.h"
#include "BuildNr.h"
#include "Product.h"





static Usage()
{
    _tprintf(_T("usage for download:\n"));
    _tprintf(_T("4dl <project>.4cp [<trg>, [<trg>,...]] [opt, [opt,...]]\n"));
    _tprintf(_T("download options:\n"));
    _tprintf(_T("  <trg>          : Process only named targets. If no targets are specified\n"));
    _tprintf(_T("                   process all targets defined in <project>.4cp.\n"));
    _tprintf(_T("  -sp[<file>.4pl]: Safe parameters: perfoms the following steps:\n"));
    _tprintf(_T("                   stop resource=>upload 4pl=>download=>write 4pl=>start resource\n"));
    _tprintf(_T("                   If any errors during parameter write the resource remains paused.\n"));
    _tprintf(_T("  -dp[<file>.4pl]: Write named parameter file with values to control after download.\n"));
    _tprintf(_T("  -up[<file>.4pl]: Upload named parameter file and their values.\n"));
    _tprintf(_T("                   If <file>.4pl is ommitted 'retain.4pl' is assumed.\n"));
    _tprintf(_T("  -st            : Stop/start all tasks before upload parameters. Can be used\n"));
    _tprintf(_T("                   together with -up. Default is: do not stop/start.\n"));
    _tprintf(_T("  -merge         : Do only parameter file merging but no download.\n"));
    _tprintf(_T("                   The merged file is placed into current working directory.\n"));
    _tprintf(_T("                   with name <file>.merge.4pl. Can be used with -sp and -up.\n"));
    _tprintf(_T("  -wm            : Enable parameter merging warnings\n"));
    _tprintf(_T("  -changes       : Download only changes.\n"));
    _tprintf(_T("  -flash         : Write also to flash (if any).\n"));
    _tprintf(_T("  -v0            : Disable progress info (default).\n"));
    _tprintf(_T("  -v1            : Verbose mode 1:display progress on domain level.\n"));
    _tprintf(_T("  -v2            : Verbose mode 2: -v1 and progress on variable read/write.\n"));
    _tprintf(_T("  -v             : same as -v2\n"));
    _tprintf(_T("  -gs <max>      : Set maximal number of items in OPC group. Default: 50.\n"));
    _tprintf(_T("  -upd <dir>     : Directory where to store uploaded 4pl data.\n"));
    _tprintf(_T("                   If not absolute it is relative to current working directory.\n"));
    _tprintf(_T("                   If the path does not exist it is created.\n"));
    _tprintf(_T("  -md <dir>      : Directory where to store merged 4pl data.\n"));
    _tprintf(_T("                   If not absolute it is relative to current working directory.\n"));
    _tprintf(_T("                   If this option is not given the merged paramter data is tmp only.\n"));
    _tprintf(_T("  -pw <password> : Use <password> to connect.\n"));
    _tprintf(_T("  -nologo        : No copyright message.\n"));
    _tprintf(_T("  -?             : This help.\n"));
    _tprintf(_T("\n"));
    _tprintf(_T("usage for upload without project:\n"));
    _tprintf(_T("4dl {<address>} -up [<file>.4pl]\n"));
    _tprintf(_T(" {<address>}     : Full qualified connect string using {...}.\n"));
    _tprintf(_T("  -up[<file>.4pl]: Upload named parameter file and their values.\n"));
    _tprintf(_T("                   If <file>.4pl is ommitted 'retain.4pl' is assumed.\n"));
    _tprintf(_T("\n"));
    _tprintf(_T("Examples:\n"));
    _tprintf(_T("4dl {W-EF,126a81fd-4b8e-4a44-96cb-e78c0c741258,W-EF,TCP(W-EF),SIMPLE(1000),VERSION(20500)} -up\n"));
    _tprintf(_T("4dl myProject.4cp win1 -sp\n"));
}

bool DumpLogoAndHelp(
    int           argc, 
    TCHAR**       argv
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
        psz = _T(PROD_PRODUCTNAME) _T(" ATCMControl V2.0 command line downloader ")
              _T(PROD_FULL_VERSION_STR) _T("\n")
              _T(PROD_COPYRIGHT) _T("\n\n");
        //replace © with (C) for dos box...
        psz = jot.clear().appendUpTo(psz, _T("\251"));
        jot<<_T("(C)")<<psz;
        _tprintf(_T("%s"), jot.psz());
    }

    if(iHelp>0)
    {
        Usage();
        return true;
    }

    return false;
}






bool ParseCmdLine(
    int             argc, 
    TCHAR*          argv[], 
    CMDL_INPUT*     pCmdl
)
{
    int           i;
    FC_CString    jot;
    TCHAR         buffer[MAX_PATH*2];
    const TCHAR*  psz;
    TCHAR*        pszFile;
    FILE*         p4CP;


    if(argc<2)
    {
        _tprintf(_T("4dl: error: command line: too few arguments\n"));
        return false;
    }

    if(argv[1][0] == _T('{'))
    {
        jot.load(&argv[1][1]);
        if(jot.removeLastChar(_T("}")) != _T('}'))
        {
            _tprintf(_T("4dl: command line error:  missing closing '}' in target address\n"));
            return false;
        }
        pCmdl->connectStr.add(jot);
        pCmdl->trgNames.add(jot);
        i = 2;
    }
    else if(!_tcsicmp(FC_StringGetFileExt(argv[1]), _T("4cp")))
    {
        GetFullPathName(argv[1], FC_ARRAY_LEN(buffer), buffer, &pszFile);
        if(!FC_Exist(buffer))
        {
            _tprintf(_T("4dl: error '%s' does not exist\n"), buffer);
            return false;
        }
        pCmdl->prjFile.load(buffer);

        //get targets:
        for(i=2; i<argc; i++)
        {
            if(argv[i][0]==_T('-') || argv[i][0]==_T('/'))
                break;

            pCmdl->trgNames.add(argv[i]);
        }
    }
    else
    {
        _tprintf(_T("4dl: error: command line must start with <project>.4cp or \"{<address>}\""));
        return false;
    }


    //get options:
    for(; i<argc; i++)
    {
        if(!_tcsicmp(_T("-gs"), argv[i]) || !_tcsicmp(_T("/gs"), argv[i]))
        {
            i++;
            if(i>=argc)
            {
                _tprintf(_T("4dl: command line error:  '-gs' needs a value\n"));
                return false;
            }
            pCmdl->nMaxGS = atoi(argv[i]);
            if(pCmdl->nMaxGS<=0 || pCmdl->nMaxGS>MAX_GROUP_SIZE)
            {
                _tprintf(_T("4dl: command line error: -gs %s: out of range: possible: 1..%d\n"), 
                    argv[i],MAX_GROUP_SIZE);
                return false;
            }
        }
        else if(!_tcsicmp(_T("-dp"), argv[i]) || !_tcsicmp(_T("/dp"), argv[i]))
        {
            if(i+1<argc && argv[i+1][0]!=_T('-') && argv[i+1][0]!=_T('/'))
                pCmdl->download4PL.add(argv[++i]);
            else
                pCmdl->download4PL.add(_T("retain.4pl"));
        }
        else if(!_tcsicmp(_T("-sp"), argv[i]) || !_tcsicmp(_T("/sp"), argv[i]))
        {
            if(i+1<argc && argv[i+1][0]!=_T('-') && argv[i+1][0]!=_T('/'))
                pCmdl->safe4PL.add(argv[++i]);
            else
                pCmdl->safe4PL.add(_T("retain.4pl"));
        }
        else if(!_tcsicmp(_T("-up"), argv[i]) || !_tcsicmp(_T("/up"), argv[i]))
        {
            if(i+1<argc && argv[i+1][0]!=_T('-') && argv[i+1][0]!=_T('/'))
                pCmdl->upload4PL.load(argv[++i]);
            else
                pCmdl->upload4PL.load(_T("retain.4pl"));
        }
        else if(!_tcsicmp(_T("-upd"), argv[i]) || !_tcsicmp(_T("/upd"), argv[i]))
        {
            i++;
            if(i>=argc)
            {
                _tprintf(_T("4dl: command line error:  '-upd' needs a value\n"));
                return false;
            }
            pCmdl->uploadDestDir.load(argv[i]);
        }
        else if(!_tcsicmp(_T("-md"), argv[i]) || !_tcsicmp(_T("/md"), argv[i]))
        {
            i++;
            if(i>=argc)
            {
                _tprintf(_T("4dl: command line error:  '-md' needs a value\n"));
                return false;
            }
            pCmdl->destDirMerged.load(argv[i]);
        }
        else if(!_tcsicmp(_T("-st"), argv[i]) || !_tcsicmp(_T("/st"), argv[i]))
        {
            pCmdl->bStopTasksForUpload = true;
        }
        else if(!_tcsicmp(_T("-wm"), argv[i]) || !_tcsicmp(_T("/wm"), argv[i]))
        {
            pCmdl->bMergeWarn = true;
        }
        else if(!_tcsicmp(_T("-merge"), argv[i]) || !_tcsicmp(_T("/merge"), argv[i]))
        {
            pCmdl->bMergeOnly = true;
        }
        else if(!_tcsicmp(_T("-changes"), argv[i]) || !_tcsicmp(_T("/changes"), argv[i]))
        {
            pCmdl->bDLChanges = true;
        }
        else if(!_tcsicmp(_T("-flash"), argv[i]) || !_tcsicmp(_T("/flash"), argv[i]))
        {
            pCmdl->bDLDoFlash = true;
        }
        else if(!_tcsicmp(_T("-pw"), argv[i]) || !_tcsicmp(_T("/pw"), argv[i]))
        {
            i++;
            if(i<argc)
            {
                pCmdl->passWord.load(argv[i]),
                pCmdl->pszPassWord = pCmdl->passWord;
            }
            else
            {
                _tprintf(_T("4dl: command line error:  '-pw' needs a value\n"));
                return false;
            }

        }
        //-trg only for compatibility:
        else if(!_tcsicmp(_T("-trg"), argv[i]) || !_tcsicmp(_T("/trg"), argv[i]))
        {
            i++;
            if(i>=argc)
            {
                _tprintf(_T("4dl: command line error:  '-trg' needs a value\n"));
                return false;
            }
            pCmdl->trgNames.add(argv[i]);
        }
        else if(!_tcsicmp(_T("-v"), argv[i]) || !_tcsicmp(_T("/v"), argv[i]))
        {
            pCmdl->nVerbose = 1000;
        }
        else if(!_tcsicmp(_T("-v0"), argv[i]) || !_tcsicmp(_T("/v0"), argv[i]))
        {
            pCmdl->nVerbose = 0;
        }
        else if(!_tcsicmp(_T("-v1"), argv[i]) || !_tcsicmp(_T("/v1"), argv[i]))
        {
            pCmdl->nVerbose = 1;
        }
        else if(!_tcsicmp(_T("-v2"), argv[i]) || !_tcsicmp(_T("/v2"), argv[i]))
        {
            pCmdl->nVerbose = 2;
        }
        else if(!_tcsicmp(_T("-nologo"), argv[i]) || !_tcsicmp(_T("/nologo"), argv[i]))
        {
            //already done.
        }
        else
        {
            _tprintf(_T("4dl: error: unknown command line option '%s'\n"),argv[i]);
            return false;
        }
    }


    if(pCmdl->prjFile.isEmpty())
    {
        if(pCmdl->upload4PL.isEmpty())
        {
            _tprintf(_T("4dl: error: command line: no command option given\n"));
            return false;
        }
        return true;
    }
    
    
    if(pCmdl->upload4PL.isEmpty() || pCmdl->download4PL.getUsed()>0 || pCmdl->safe4PL.getUsed()>0)
    {
        pCmdl->bDownload = true;
    }




    jot.load(FC_StringGetFilePart(buffer)).changeFileExt(NULL);
    pCmdl->prjName.load(jot);

    pCmdl->prjPath.load(jot.load(buffer).stripFilePart());
    
    assert(!pCmdl->sGenPath);
    jot.load(buffer).stripFilePart();
    jot<<_T("\\gen");
    pCmdl->genPath.load(jot);
    pCmdl->sGenPath = FC_StringToBSTR(jot);

    

    if(pCmdl->trgNames.getUsed()==0)
    {
        //Parse 4cp file to get all targets: TODO use get profile string function here!!
        p4CP = _tfopen(pCmdl->prjFile, _T("r"));
        if(!p4CP)
        {
            _tprintf(_T("4dl: error: cannot open file %s\n"),pCmdl->prjFile.psz());
            return false;
        }

        bool bInSection = false;
        bool bInVersion = false;
        FC_CString format;
        while(!feof(p4CP))
        {
            buffer[0] = 0;
            fgets(buffer, FC_ARRAY_LEN(buffer), p4CP);
            FC_StringTrim(buffer);
            psz= buffer;
            if(*psz==_T(';'))
                continue;

            if(*psz==_T('['))
            {
                jot.clear();
                for(;*psz && *psz!=_T(']'); psz++)
                {
                    if(!_istspace(*psz))
                        jot.addc(*psz);
                }
                bInSection = (*psz==_T(']') && !lstrcmpi(_T("[Targets"), jot));
                bInVersion = (*psz==_T(']') && !lstrcmpi(_T("[Format"), jot));
            }
            else if(bInSection || bInVersion)
            {
                jot.clear();
                for(;*psz && *psz!=_T('='); psz++)
                {
                    if(!_istspace(*psz))
                        jot.addc(*psz);
                }
                if(bInSection && *psz==_T('=') && !lstrcmpi(_T("file"), jot))
                {
                    //target name is without path and ext:
                    jot.load(FC_StringGetFilePart(psz+1)).changeFileExt(NULL);
                    pCmdl->trgNames.add(jot);
                }
                else if(bInVersion && *psz==_T('=') && !lstrcmpi(_T("Version"), jot))
                {
                    format.load(psz+1);
                }
            }
        
        }
        fclose(p4CP);
        if(lstrcmpi(_T("V2.0"), format))
        {
            _tprintf(_T("4dl: fatal error: bad project version: expected 'V2.0' but found '%s'\n"),format.psz());
            return false;
        }
    }

    if(pCmdl->trgNames.getUsed()==0)
    {
        _tprintf(_T("4dl: error: no targets specified or project has no targets\n"));
        return false;
    }


    if(pCmdl->download4PL.getUsed()>0 && pCmdl->trgNames.getUsed()>1)
    {
        _tprintf(_T("4dl: error: '-dp' cannot be used for multiple download targets\n"));
        return false;
    }
    if(pCmdl->safe4PL.getUsed()>0 && pCmdl->trgNames.getUsed()>1)
    {
        _tprintf(_T("4dl: error: '-sp' cannot be used for multiple download targets\n"));
        return false;
    }


    for(i=0;i<pCmdl->trgNames.getUsed();i++)
    {
        psz = pCmdl->trgNames.get(i);
        jot.load(pCmdl->genPath)<<_T("\\")<<psz<<_T("\\")<<_T("loader.list");
        if(!FC_Exist(jot))
        {
            _tprintf(_T("4dl: error: cannot open file %s (target '%s' not compiled?)"),
                jot.psz(), psz);
            return false;
        }
        pCmdl->loaderLists.add(jot);
        
        if(!GetConnectStringFromLoaderList(pCmdl->loaderLists.get(i), psz, &jot))
            return false;

        pCmdl->connectStr.add(jot);
    }


    return true;
}


