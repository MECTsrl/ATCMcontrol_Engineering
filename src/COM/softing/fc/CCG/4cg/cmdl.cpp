#include <stdio.h>

#include "cmdl.h"
#include "cgt_misc.h"

#include "BuildNr.h" //for logo
#include "Product.h" //for logo
#include "cgframe.h" //from mc file
#include "fc_tools\fc_fileIO.h" //from mc file



const TCHAR* LIBOptName(LIB_ACTION o, bool bMinus)
{
    switch(o)
    {
    case LIB_MAKE:      return bMinus ? _T("-ml")  : _T("ml");
    case LIB_MAKE_STUB: return bMinus ? _T("-ml")  : _T("ml");
    case LIB_APPEND:    return bMinus ? _T("-al")  : _T("al");
    case LIB_REMOVE:    return bMinus ? _T("-rl")  : _T("rl");
    case LIB_LIST:      return bMinus ? _T("-ll")  : _T("ll");
    case LIB_LISTLONG:  return bMinus ? _T("-lll") : _T("lll");
    case LIB_EXTRACT:   return bMinus ? _T("-el")  : _T("el");
    }
    return _T("");
}


extern "C" bool DumpLogoAndHelp(
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
            if(!_tcsicmp(psz+1, _T("nologo")))
                iNologo++;
            else if(!_tcsicmp(psz+1, _T("help")) || !_tcsicmp(psz+1, _T("?")))
                iHelp++;
        }
    }

    if(iNologo==0)
    {
        psz = _T(PROD_PRODUCTNAME) _T(" IEC 1131-3 Multi Target Compiler ")
              _T(PROD_FULL_VERSION_STR) _T("\n")
              _T(PROD_COPYRIGHT) _T("\n\n");
        //replace © with (C) for dos box...
        psz = jot.clear().appendUpTo(psz, _T("\251"));
        jot<<_T("(C)")<<psz;
        _tprintf(jot);
    }

    if(iHelp>0)
    {
        _tprintf(jot.clear().fmtMsg(FCG_I_USAGE, NULL));
        return true;
    }
    return false;
}




extern "C" bool ParseCommandLine(
    CGT_CDiagMsg*    pdm,
    ICG_IECFront*    pIFront, 
    CMDL_OPTIONSTYP* pOpts,
    int              argc, 
    TCHAR**          argv
)
{
    int              i;
    int              is;
    const TCHAR*     psz;
    const TCHAR*     psz2;
    CG_Edipos        cmdl;
    const TCHAR*     pszTrgExts;
    LIB_ACTION       libAct;
    

    //init cmdl to show error location 'command line'
    CG_InitCMDLEdipos(&cmdl);


    //get the list of target exts:
    pszTrgExts = pIFront->KADgetTargetTypeExts();
    assert(pszTrgExts);

    for(i=0;i<argc;i++)
    {
        psz = argv[i]; 
        if(*psz==_T('/') || *psz==_T('-'))
        {
            psz++;
            libAct = LIB_ACT_UNDEF;

            if(!_tcsicmp(psz, _T("c")))
            {
                if(++i>=argc)
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, psz);
                else if(!pOpts->szCheckFile.isEmpty())
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else
                    pOpts->szCheckFile.load(argv[i]);
            }
            else if((is = FC_StringStartsWithI(psz, _T("c:"))) >0)
            {
                if(i+1>=argc)
                {
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, _T("c"));
                }
                else if(!pOpts->szCheckFile.isEmpty())
                {
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), _T("c"));
                }
                else
                {
                    if(psz[is])
                    {
						//check syntax of command line:  old: -c:4cwin  or  new: -c:win1:21050
						const TCHAR* pszTrgType = &psz[is];
						const TCHAR* pszTrgVers = _tcsrchr(pszTrgType, ':');
						FC_CStr      szTrgType;
						if(pszTrgVers)
						{
							szTrgType.loadLen(pszTrgType, pszTrgVers - pszTrgType);
							pszTrgVers++; //skip the ':'
						}
						else
						{
							szTrgType.load(pszTrgType);
						}

                        if(pIFront->KADgetTargetTypeByExt(szTrgType, NULL, NULL, NULL))
						{
                            pOpts->szCheckTrgType.load(szTrgType);
							if(pszTrgVers)
	                            pOpts->szCheckTrgVers.load(pszTrgVers);
						}
                        else
						{
                            pdm->msg2(CG_E_NO_TARGETTYPE_EXT, &cmdl, psz, pszTrgExts);
						}
                    }

                    i++;
                    pOpts->szCheckFile.load(argv[i]);
                }
            }
            else if( (is = FC_StringStartsWithI(psz, _T("ml"))) >0)
            {
                libAct = LIB_MAKE;

                if(psz[is]==_T(':'))
                    pOpts->szLibTargetType.load(&psz[is+1]);
                else if(psz[is] != 0)
                    pdm->msg1(CG_E_CMDL_OPT_NEEDS_TARGET, &cmdl, psz);

                if(pOpts->szLibTargetType.isEmpty())
                {
                    ;//checked later: either only one target type inKAD or libtyp=<trg typ> in [compiler] section
                }
                else if(!_tcsicmp(pOpts->szLibTargetType, _T("0")))
                {
                    //special case: only stub
                    libAct = LIB_MAKE_STUB;
                }
                else if(!pIFront->KADgetTargetTypeByExt(pOpts->szLibTargetType, NULL, NULL, NULL))
                {
                    pdm->msg2(CG_E_NO_TARGETTYPE_EXT, &cmdl, psz, pszTrgExts);
                }
                //get the lib name below...
            }
            else if(!_tcsicmp(psz, _T("al")))
            {
                libAct = LIB_APPEND;
            }
            else if(!_tcsicmp(psz, _T("ll")))
            {
                libAct = LIB_LIST;
            }
            else if(!_tcsicmp(psz, _T("lll")))
            {
                libAct = LIB_LISTLONG;
            }
            else if(!_tcsicmp(psz, _T("el")))
            {
                libAct = LIB_EXTRACT;
            }
            else if( (is = FC_StringStartsWithI(psz, _T("rl"))) >0)
            {
                libAct = LIB_REMOVE;
                if(psz[is]==_T(':'))
                {
                    pOpts->szLibTargetType.load(&psz[is+1]);

                    if(!pIFront->KADgetTargetTypeByExt(pOpts->szLibTargetType, NULL, NULL, NULL))
                        pdm->msg2(CG_E_NO_TARGETTYPE_EXT, &cmdl, psz, pszTrgExts);
                }
                else
                {
                    pdm->msg1(CG_E_CMDL_OPT_NEEDS_TARGET, &cmdl, psz);
                }
            }
            else if(!_tcsicmp(psz, _T("i")))
            {
                if(pOpts->bIncrementelly)
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else
                    pOpts->bIncrementelly = true;
            }
            else if(!_tcsicmp(psz, _T("r")))
            {
                if(pOpts->bRecursive)
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), psz);
                else
                    pOpts->bRecursive = true;
            }
            else if(!_tcsicmp(psz, _T("nologo")))
            {
                ;//nologo already handeled, -? must not pass here any more ...
            }
            else
            {
                pdm->msg1(CG_E_CMDL_UNKNOWN_OPT, &cmdl, --psz);
            }


            if(libAct != LIB_ACT_UNDEF)
            {
                //check opt twice or conflict with other lib action:
                if(pOpts->libAct == libAct)
                {
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("-"), LIBOptName(libAct));
                }
                else if(pOpts->libAct != LIB_ACT_UNDEF)
                {
                    pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, psz, LIBOptName(pOpts->libAct)); 
                }
                pOpts->libAct = libAct;

                if(++i>=argc)
                {
                    pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, LIBOptName(libAct));
                }
                else 
                {
                    pOpts->szMakeLib.load(argv[i]);

                    if(_tcsicmp(FC_StringGetFileExt(argv[i]), CG_PREDEF_LIB_EXT))
                        pdm->msg3(CG_E_BAD_CMDL_FILETYPE, &cmdl, psz, argv[i], CG_PREDEF_LIB_EXT);
                }

                //-al <source>.4cl <dest>.4cl
                if(libAct == LIB_APPEND)
                {
                    if(++i>=argc)
                    {
                        pdm->msg1(CG_E_CMDL_NEEDPARAM, &cmdl, _T("al"));
                    }
                    else 
                    {
                        pOpts->szSourceLib.load(pOpts->szMakeLib);
                        pOpts->szMakeLib.load(argv[i]);

                        if(_tcsicmp(FC_StringGetFileExt(argv[i]), CG_PREDEF_LIB_EXT))
                            pdm->msg3(CG_E_BAD_CMDL_FILETYPE, &cmdl, psz, argv[i], CG_PREDEF_LIB_EXT);
                    }
                }
            }

        }   
        else
        { 
            //pure file argument, must be either 4CP or 
            //build target by name:
            psz2 = FC_StringGetFileExt(psz);
            if(!_tcsicmp(psz2, CG_4CP_EXTENSION))
            {
                if(pOpts->sz4CP[0])
                {
                    pdm->msg2(CG_W_CMDL_OPT_TWICE, &cmdl, _T("."), psz2);
                }
                else
                {
                    pOpts->sz4CP.load(psz);

                    TCHAR buffer[2*MAX_PATH];
                    FC_FileFullPath(NULL, psz, buffer, FC_ARRAY_LEN(buffer));
                    FC_StringCutFilePart(buffer);
                    pOpts->sz4CPFullPath.load(buffer);
                }
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


    //check option combinations got end after first error
    //in order to produce only one error (the user is frustrated anyway...):
    if(pdm->hasErr())
        goto Ende;

    if(pOpts->sz4CP.isEmpty())
    {
        if(pOpts->libAct==LIB_ACT_UNDEF || pOpts->libAct==LIB_MAKE || pOpts->libAct==LIB_MAKE_STUB)
        {
            pdm->msg0(CG_E_CMDL_NO4CP, &cmdl);
            goto Ende;
        }
    }
    else if(pOpts->libAct!=LIB_ACT_UNDEF && pOpts->libAct!=LIB_MAKE && pOpts->libAct!=LIB_MAKE_STUB)
    {
        pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, pOpts->sz4CP, 
            pdm->jot1()<<LIBOptName(pOpts->libAct, true)<<_T(" (library management)"));
        goto Ende;
    }

    if(!pOpts->szCheckFile.isEmpty() && pOpts->libAct != LIB_ACT_UNDEF)
    {
        //-c check file cannot be together with -al,-ll-rl or -ml:
        pdm->msg2(CG_E_CMDL_OPT_CONFLICT, &cmdl, _T("-c"), LIBOptName(pOpts->libAct, true)); 
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
        //-c check build target by name in respect 
        //to target type extension:
        for(i=0;i<pOpts->targets.getUsed();i++)
        {
            psz = pOpts->targets.get(i);
            psz2 = FC_StringGetFileExt(psz);
            if(!pIFront->KADgetTargetTypeByExt(psz2, NULL, NULL, NULL))
            {
                pdm->msg2(CG_E_NO_TARGETTYPE_EXT, &cmdl, psz, pszTrgExts);
            }
        }
    }


Ende:
    //if any cmdl error dump hint how to get help:
    if(pdm->hasErr())
    {
        pdm->msg0(CG_E_BAD_CMDL_LINE, NULL);
        return false;
    }
    return true;
}
