


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_carchive.h"
#include "helper.h"
#include "cmdl.h"
#include "pumplib.h"
#include "cgt_misc.h"
#include "cgt_link.h"
#include "cgt_tools.h"
#include "BuildNr.h" //for $(version)
#include "Product.h" //for $(version)

//----  Static Initializations:   ----------------------------------*
static bool MakeLibFullPath(
    CGT_CDiagMsg*  pdm,
    ICG_IECFront*  pIFront,
    const TCHAR*   pszLib,      //may contain $(libdir)
    FC_CString*    plibFullPath //receives the full path
);
static I4CLAccess* OpenLib(
    CGT_CDiagMsg*  pdm,
    ICG_IECFront*  pIFront,
    bool           bWrite,
    const TCHAR*   pszLib,      //may contain $(libdir)
    I4CLOpen**     ppILibOpen,  //out param
    FC_CString*    plibFullPath //if not NULL receives the full path
);
static void PumpUpLibrary(
    CGT_CDiagMsg*          pdm,
    const CMDL_OPTIONSTYP* pOpt,
    ICG_IECFront*          pIFront,
    const TCHAR*           pszLibFile,
    const TCHAR*           pszTrgTyp,
    const TCHAR*           pszCompilerVersion
);
static bool Process4CPLibPuts(
    CGT_CDiagMsg*           pdm,
    const CMDL_OPTIONSTYP*  pOpt,
    ICG_IECFront*           pIFront,
    I4CLAccess*             pILibAcc,
    const TCHAR*            pszTrgTyp
);




static const TCHAR* GetCompilerVersion()
{
    const TCHAR* psz = PROD_FULL_VERSION_STR;
    FC_StringIncBeginI(&psz, _T("Version"));
    FC_StringSkipWhite(&psz);
    return psz;
}


bool FC_GetFileVersion(
    const TCHAR* pszBackEnd,
    TCHAR        szFileVer[/*dwBytes*/],
    unsigned int dwBytes
)
{
    BOOL  bOk;
    DWORD dwLen;
    DWORD dw;
    char* pData;
    struct LANGANDCODEPAGE {
        WORD wLanguage;
        WORD wCodePage;
    } *lpTranslate;
    UINT   cbTranslate;
    TCHAR  szBuffer[2*MAX_PATH];

    assert(dwBytes>100);
    szFileVer[0] = 0;

    dw = 0;
    
    assert(_tcslen(pszBackEnd) < 2*MAX_PATH -1);
    strcpy(szBuffer, pszBackEnd);
    dwLen =  GetFileVersionInfoSize(szBuffer,  &dw);
    if(dwLen==0)
        return false;
    
    pData = new char[dwLen];

    bOk = GetFileVersionInfo(
              szBuffer,    // file name
              0,             // ignored
              dwLen,         // size of buffer
              pData          // version information buffer
            );

    if(bOk)
    {

        // Read the list of languages and code pages.
        bOk= VerQueryValue(
            pData,                              // buffer for version resource
            _T("\\VarFileInfo\\Translation"),   // value to retrieve
            (LPVOID*)&lpTranslate,              // buffer for version value pointer
            &cbTranslate                              // version length
        );

        if(bOk)
        {
            // Read the file version for first language and code page.
            szFileVer[0] = 0;
            for(unsigned int i=0; i<(cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
            {
                wsprintf(szBuffer, 
                         _T("\\StringFileInfo\\%04x%04x\\FileVersion"),
                         lpTranslate[i].wLanguage,
                         lpTranslate[i].wCodePage);

                // Retrieve file description for language and code page "i". 
                char* pVersion;
                UINT  uinchar;
                if(VerQueryValue(pData, szBuffer, (void**)&pVersion, &uinchar))
                {
                    assert(strlen(pVersion)==uinchar-1);
                    if(uinchar<=dwBytes)
                        memcpy(szFileVer, pVersion, uinchar);
                    else
                        szFileVer[0] = 0;
                    break;
                }
            }

            if(szFileVer[0]==0)
                bOk = false;
        }
    }

    delete(pData);
    return bOk!=0;
}







extern "C" void MakeLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const FC_CGArray<TargetAndType>* pTargets,
    const CMDL_OPTIONSTYP*           pOpt
)
{
    int                       i;
    int                       iTrg;
    FC_CString                jotTrg(MAX_PATH); 
    FC_CString                jotCmd(MAX_PATH);
    FC_CString                libExpanded(MAX_PATH);
    const TCHAR*              pszBackend;
    CG_Edipos                 edp;

    assert(!pOpt->szMakeLib.isEmpty() && !pOpt->szLibTargetType.isEmpty());
    assert(pOpt->libAct==LIB_MAKE);

    CG_InitCMDLEdipos(&edp);
    long flags = CGT_INS_PRINTHELP|CGT_INS_WARN_UNDEF;
    int ret = CGT_SubsStdDollarIns(pdm, pIFront, pOpt->szMakeLib, &edp, flags, &libExpanded);
    if(ret==0)
        return;
    
    //command line starts with 4cp file (path is same as 4cg was called)
    pszBackend = NULL;
    iTrg = pIFront->KADgetTargetTypeByExt(pOpt->szLibTargetType, &pszBackend, NULL, NULL);
    assert(iTrg>0 && pszBackend);

    //build command line:
    jotCmd.load(_T("-nologo "));
    CGT_AppendCMDLFileArg(jotCmd, pOpt->sz4CP)<<' ';

    //possible targets:
    if(pTargets)
    {
        for(i=0; i<pTargets->getUsed(); i++)
        {
            assert(pTargets->getAddrOf(i)->iType==iTrg);
            CGT_AppendCMDLFileArg(jotCmd, pTargets->getAddrOf(i)->pszTrg)<<' ';
        }
    }

    //add the -ml:<trgtyp> <lib name> argument: 
    jotCmd<<_T("-ml:")<<pOpt->szLibTargetType<<_T(" ");
    CGT_AppendCMDLFileArg(jotCmd, libExpanded);

    
    //create process:
    if(CallBackend(pdm, pOpt->nVerbose, pszBackend, jotCmd, pIFront->FCPgetGenPath(), pOpt->szLibTargetType))
    {
        //check in the produced link files:
        TCHAR szBEFileVer[MAX_PATH];
        FC_GetFileVersion(pszBackend, szBEFileVer, sizeof(szBEFileVer));
        PumpUpLibrary(pdm, pOpt, pIFront, libExpanded, pOpt->szLibTargetType, szBEFileVer);
    }
}


extern "C" void MakeLibraryStub(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpt
)
{
    const CG_Symtab*          pSym;
    CG_Edipos                 edp;
    const CG_POU4CPInfo*const*ppInf;
    FC_CString                libExpanded(MAX_PATH);

    assert(!pOpt->szMakeLib.isEmpty() && pOpt->libAct==LIB_MAKE_STUB);

    CG_InitCMDLEdipos(&edp);
    long flags = CGT_INS_PRINTHELP|CGT_INS_WARN_UNDEF;
    if(CGT_SubsStdDollarIns(pdm, pIFront, pOpt->szMakeLib, &edp, flags, &libExpanded)==0)
        return;

    //load all libs referenced in 4CP:
    if(pdm->hasNoErr())
        pIFront->loadLIBs();

    //load all CONST of project and all libs:
    if(pdm->hasNoErr())
        pIFront->loadCSTs();
    
    if(pdm->hasErr())
        return;


    //no error so far, loop all POU of 4cp and build link files:
    for(ppInf = pIFront->FCPgetAllPOUs(); *ppInf; ppInf++)
    {
        pSym = pIFront->loadHeader((*ppInf)->pszId);
        if(!pSym)
        {
            //should not happen:
            assert(!"loadHeader failed but POU was in 4cp");
            pdm->msg2(CG_E_UNDEF_POU_TYP, &(*ppInf)->edp, _T(""), (*ppInf)->pszId);   
            return;
        }

        if(!(pSym->flags & CG_STAB_HDR_NOERR))
        {
            assert(pdm->hasErr());
            continue;//carry on...
        }

        //if body load it just to get any errors:
        if((pSym->flags & CG_STAB_HAS_BDY) && !(pSym->flags & CG_STAB_BDY_LOADED))
        {
            ICG_Body* pIBody;
            pIFront->loadBody((*ppInf)->pszId, CG_LOADBDY_TMPVAR_OPT1, &pIBody);
            FC_RELEASE_PTR(pIBody);

            if(!(pSym->flags & CG_STAB_BDY_NOERR))
                continue;//carry on...
        }


        //no error in header/body, dump link file:
        if(pSym->pouTyp==CG_POU_PRG)
            continue; //no prgs in lib...


        if(!CGT_DumpLinkFile(pdm, pIFront, pSym, false, NULL, NULL, NULL, NULL))
        {
            assert(pdm->hasErr());
            return;//fatal, could not create link
        }
    }


    if(pdm->hasNoErr())
        PumpUpLibrary(pdm, pOpt, pIFront, libExpanded, NULL, PROD_VERSION_STR);
}



static bool ListPOU(
    CGT_CDiagMsg*   pdm,
    bool            bLongList,
    const TCHAR*    pszPOU,
    const TCHAR*    pszPOUDisplay,
    I4CLAccess*     pILibAcc
)
{
    HRESULT       hr;
    FC_CString    jot;
    unsigned long ul2;
    unsigned long ul3;
    SAFEARRAY*    sTrgTypes;
    BSTR          sPOU;

    sPOU = FC_StringToBSTR(pszPOU);
    hr = pILibAcc->GetTargetTypesOfPOU(sPOU,  &sTrgTypes);
    if(FAILED(hr) || !sTrgTypes)
    {
        FC_FREE_BSTR(sPOU);
        return false;
    }


    if(bLongList)
    {
        jot.load(pszPOUDisplay)<<_T(":\n");
        pdm->userMsg(jot);

        //long list including files:
        SAFEARRAY* psDomains;
        hr = pILibAcc->GetFileNames(sPOU, NULL, &psDomains);
        for(ul3=0; ul3<psDomains->rgsabound[0].cElements; ul3++)
        {
            BSTR sDom = ((BSTR*)(psDomains->pvData))[ul3];
            jot.load(_T("    "))<<sDom<<_T("\n");
            pdm->userMsg(jot);
        }
        SafeArrayDestroy(psDomains);
    }
    else
    {
        jot.load(pszPOUDisplay).tab(32);
    }



    for(ul2=0; ul2<sTrgTypes->rgsabound[0].cElements; ul2++)
    {
        BSTR sTrg;
        sTrg = ((BSTR*)(sTrgTypes->pvData))[ul2];

        if(!bLongList)
        {
            if(ul2>0)
                jot<<_T(",");
            jot << sTrg;
        }
        else
        {
            //long list including files:
            jot.load(pszPOUDisplay)<<_T("\\")<<sTrg<<_T(":\n");
            pdm->userMsg(jot);

            SAFEARRAY* psDomains;
            hr = pILibAcc->GetFileNames(sPOU, sTrg, &psDomains);
            for(ul3=0; ul3<psDomains->rgsabound[0].cElements; ul3++)
            {
                BSTR sDom = ((BSTR*)(psDomains->pvData))[ul3];
                jot.load(_T("    "))<<sDom<<_T("\n");
                pdm->userMsg(jot);
            }
            SafeArrayDestroy(psDomains);
        }
    }
    SafeArrayDestroy(sTrgTypes);
    
    if(!bLongList)
    {
        jot<<_T("\n");
        pdm->userMsg(jot);
    }

    FC_FREE_BSTR(sPOU);

    return true;
}





static void GetLibVersionInfo(
    CGT_CDiagMsg*   pdm,
    I4CLAccess*     pILibAcc
)
{
    BSTR          sPOU;
    SAFEARRAY*    sTrgTypes;
    BSTR          sVerInfo;
    unsigned long ul;

    sPOU = FC_StringToBSTR(CG_LIB_POU_VERSION);


    pdm->userMsg(_T("Version:\n"));
    if(pILibAcc->GetFileAsBSTR(sPOU, NULL, L"v", &sVerInfo)==S_OK)
    {
        pdm->userMsg((pdm->jot1()<<_T("IEC-Interface")).tab(22)<<_T(": ")<<sVerInfo<<_T("\n"));
        SysFreeString(sVerInfo);
    }
    else
    {
        pdm->userMsg((pdm->jot1()<<_T("IEC-Interface")).tab(22)<<_T(": error! no version info\n"));
    }


    sTrgTypes = NULL;
    pILibAcc->GetTargetTypesOfPOU(sPOU,  &sTrgTypes);
    if(sTrgTypes && sTrgTypes->rgsabound[0].cElements)
    {
        for(ul=0; ul<sTrgTypes->rgsabound[0].cElements; ul++)
        {
            BSTR sTrg;
            sTrg = ((BSTR*)(sTrgTypes->pvData))[ul];
            pdm->userMsg((pdm->jot1()<<_T("Implementation ")<<sTrg).tab(22)<<_T(": "));

            if(pILibAcc->GetFileAsBSTR(sPOU, sTrg, L"v", &sVerInfo)==S_OK)
            {
                pdm->userMsg(pdm->jot1()<<sVerInfo<<_T("\n"));
                SysFreeString(sVerInfo);
            }
            else
            {
                pdm->userMsg(_T("error! no version info\n"));
            }
        }
    }
    else
    {
        pdm->userMsg(_T("Implementation Version: -none-\n"));
    }

    if(sTrgTypes)
        SafeArrayDestroy(sTrgTypes);

    FC_FREE_BSTR(sPOU);
}



static void GetLibLicenseInfo(
    CGT_CDiagMsg*   pdm,
    I4CLAccess*     pILibAcc
)
{
    BSTR          sPOU;
    SAFEARRAY*    sTrgTypes;
    unsigned long ul;


    sPOU = FC_StringToBSTR(CG_LIB_POU_LICENSE);
    if(pILibAcc->ExistsPOU(sPOU, NULL)==S_OK)
    {
        pdm->userMsg("License protection for: ");

        sTrgTypes = NULL;
        pILibAcc->GetTargetTypesOfPOU(sPOU,  &sTrgTypes);
        if(sTrgTypes)
        {
            for(ul=0; ul<sTrgTypes->rgsabound[0].cElements; ul++)
            {
                if(ul>0)
                    pdm->userMsg(_T(", "));
                pdm->userMsg(pdm->jot1()<< ((BSTR*)(sTrgTypes->pvData))[ul]);
            }

            SafeArrayDestroy(sTrgTypes);
        }
        pdm->userMsg(_T("\n"));
    }
    else
    {
        pdm->userMsg("License protection:  NO\n");
    }
    FC_FREE_BSTR(sPOU);
}



extern "C" void ListLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpt
)
{
    unsigned long     ul;
    HRESULT           hr;
    I4CLOpen*         pILibOpen = NULL;
    I4CLAccess*       pILibAcc  = NULL;
    FC_CString        jot(MAX_PATH);
    SAFEARRAY*        parrPOUs;
    FC_CString        libFullPath;



    assert(!pOpt->szMakeLib.isEmpty());
    assert(pOpt->libAct==LIB_LISTLONG || pOpt->libAct==LIB_LIST);
    assert(pdm->hasNoErr());


    pILibAcc = OpenLib(pdm, pIFront, false, pOpt->szMakeLib, &pILibOpen, &libFullPath);
    if(!pILibAcc)
    {
        assert(pdm->hasErr());
        return;
    }

    //version info
    GetLibVersionInfo(pdm, pILibAcc);
    pdm->userMsg(_T("\n"));
    
    //license info
    GetLibLicenseInfo(pdm, pILibAcc);
    pdm->userMsg(_T("\n"));


    //Pou info:
    hr = pILibAcc->GetAllPOUs(&parrPOUs);
    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("GetAllPOUs() failed for library"), libFullPath);
        FC_RELEASE_PTR(pILibOpen);
        FC_RELEASE_PTR(pILibAcc);
        return;
    }

    for(ul=0; ul<parrPOUs->rgsabound[0].cElements; ul++)
    {
        BSTR sPOU = ((BSTR*)(parrPOUs->pvData))[ul];
        jot.load(sPOU);

        if( !_tcsicmp(jot, CG_LIB_POU_VERSION) || !_tcsicmp(jot, CG_LIB_POU_LICENSE))
            continue;

        if(!ListPOU(pdm, pOpt->libAct==LIB_LISTLONG, jot, jot, pILibAcc))
            pdm->msg1(CG_E_TEXT, NULL, pdm->jot1()<<_T("could read library info of '")<<sPOU<<_T("'"));
        
        if(pOpt->libAct==LIB_LISTLONG)
            pdm->userMsg(_T("\n"));
    }

    SafeArrayDestroy(parrPOUs);


    hr = pILibOpen->Close(pILibAcc);
    assert(!FAILED(hr));
    FC_RELEASE_PTR(pILibOpen);
    FC_RELEASE_PTR(pILibAcc);
}



extern "C" void ExtractLibrary( //undocumented feature ...
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpt
)
{
    FC_CArchive ar;
    TCHAR       szVerFound[FC_AR_MAX_VER];
    FC_CArDir*  pRoot;
    FC_ARSTAT   stat;
    FC_CString  jot;

    
    stat = ar.open(pOpt->szMakeLib, false, szVerFound);

    pRoot = ar.getRoot()  ;
    if(!pRoot)
        return; //TODO error msg!
    
    jot.load(pOpt->szMakeLib).changeFileExt(NULL)<<_T("_content");
    stat = pRoot->getFilesToDisk(jot, NULL, true, NULL);//TODO error msg!


}



extern "C" void RemoveLibTarget(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpt
)
{
    unsigned long     ul;
    HRESULT           hr;
    I4CLOpen*         pILibOpen = NULL;
    I4CLAccess*       pILibAcc  = NULL;
    FC_CString        jot;
    SAFEARRAY*        parrPOUs;
    FC_CString        libFullPath;
    BSTR              sTrg;


    assert(!pOpt->szMakeLib.isEmpty() && 
           !pOpt->szLibTargetType.isEmpty() && pOpt->libAct==LIB_REMOVE);
    assert(pdm->hasNoErr());

    pILibAcc = OpenLib(pdm, pIFront, true, pOpt->szMakeLib, &pILibOpen, &libFullPath);
    if(!pILibAcc)
    {
        assert(pdm->hasErr());
        return;
    }

    hr = pILibAcc->GetAllPOUs(&parrPOUs);
    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("GetAllPOUs() failed for library"), libFullPath);
        goto Error; 
    }

    sTrg = FC_StringToBSTR(pOpt->szLibTargetType);
    for(ul=0; ul<parrPOUs->rgsabound[0].cElements; ul++)
    {
        BSTR          sPOU;
       
        sPOU = ((BSTR*)(parrPOUs->pvData))[ul];

        if(pILibAcc->ExistsPOU(sPOU, sTrg) == S_OK)
        {
            hr = pILibAcc->DeleteFiles(sPOU, sTrg, NULL);
            assert(hr==S_OK);
            pdm->userMsg((pdm->jot1()<<sPOU).tab(25)<<_T(": remove target ")<<sTrg<<_T("\n"));
        }
    }

    SafeArrayDestroy(parrPOUs);
    FC_FREE_BSTR(sTrg);

    hr = pILibOpen->Close(pILibAcc);
    assert(!FAILED(hr));
Error:
    FC_RELEASE_PTR(pILibOpen);
    FC_RELEASE_PTR(pILibAcc);
}



static bool LibCopyFiles(
    CGT_CDiagMsg*  pdm,
    I4CLAccess*    pILibAccSrc, 
    I4CLAccess*    pILibAccDst, 
    BSTR           sPOU, 
    BSTR           sTrg
)
{
    HRESULT        hr;
    SAFEARRAY*     psaFiles;
    unsigned long  ul;

    

    hr = pILibAccSrc->GetFileNames(sPOU, sTrg, &psaFiles);
    assert(hr==S_OK);

    for(ul=0; ul<psaFiles->rgsabound[0].cElements; ul++)
    {
        DWORD lFileByteSize;
        BSTR  sFile = ((BSTR*)(psaFiles->pvData))[ul];

        //get the file size:
        lFileByteSize = 0;
        hr = pILibAccSrc->ExistsFile(sPOU, sTrg,  sFile, &lFileByteSize);
        assert(hr==S_OK);

        BYTE* pBytes = new BYTE[lFileByteSize+1];//+1 can be 0
        hr = pILibAccSrc->GetFileRaw(sPOU, sTrg, sFile, pBytes, lFileByteSize);

        if(hr==S_OK)
        {
            hr = pILibAccDst->PutFileRaw(sPOU, sTrg, sFile, pBytes, lFileByteSize);
            if(hr!=S_OK)
            {
                BSTR bstr;
                pILibAccDst->GetLibFileName(&bstr);
                pdm->msg2(CG_E_LIB_FILE_WRT_FAILED, NULL, sFile, bstr);
            }
        }
        else
        {
            BSTR bstr;
            pILibAccSrc->GetLibFileName(&bstr);
            pdm->msg2(CG_E_READ_LIB_DOMAIN, NULL, sFile, bstr);
        }

        delete(pBytes);

        if(hr != S_OK)
            break;
    }
    
    SafeArrayDestroy(psaFiles);
    return true;
}
                         


static bool AppendLibPOU(
    CGT_CDiagMsg*  pdm,
    I4CLAccess*    pILibAccSrc, 
    I4CLAccess*    pILibAccDst, 
    BSTR           sPOU, 
    BSTR           sTrg
)
{
    HRESULT hr;
    
    assert(sTrg);

    //check if POU exists at dest:
    hr = pILibAccDst->ExistsPOU(sPOU, NULL);
    assert(hr==S_OK || hr==S_FALSE);

    if(hr==S_FALSE)
    {
        pdm->userMsg((pdm->jot1()<<sPOU).tab(25)<<_T(": newly inserted\n"));
        
        //no, then simply add all files of the new POU:
        //TODO: should it be better not allowed 
        //to add a POU to an existing lib? 
        if(!LibCopyFiles(pdm, pILibAccSrc, pILibAccDst, sPOU, NULL))
            return false;
        if(!LibCopyFiles(pdm, pILibAccSrc, pILibAccDst, sPOU, sTrg))
            return false;
    }
    else if(sPOU[0]==L'_' && sPOU[1]==L'_')
    {
        hr = pILibAccDst->ExistsPOU(sPOU, sTrg);
        if(hr == S_OK)
        {
            pdm->userMsg((pdm->jot1()<<sPOU).tab(25)<<_T(": update target '")<<sTrg<<_T("'\n"));
            hr = pILibAccDst->DeleteFiles(sPOU, sTrg, NULL);
        }
        else
        {
            assert(hr ==S_FALSE);
            pdm->userMsg((pdm->jot1()<<sPOU).tab(25)<<_T(": add target '")<<sTrg<<_T("'\n"));
        }
        //special POUs (e.g. __CONTROL, __CONSOLE)
        //just copy, don't get a .link file:
        if(!LibCopyFiles(pdm, pILibAccSrc, pILibAccDst, sPOU, sTrg))
            return false;
    }
    else  
    {
        //not one of the special POUs, there must be a .link:
        //check that the pure IEC interface is the same
        //get the dest/src-lib's sPOU\.link file:
        BSTR     sLink;
        wchar_t* pszMangSrc = NULL;
        wchar_t* pszMangDst = NULL;

        sLink = NULL;
        pILibAccSrc->GetFileAsBSTR(sPOU, NULL, L".link", &sLink);
        if(sLink)
            pszMangSrc = CGT_IECMangeledNameFromLinkFile(sLink);
        FC_FREE_BSTR(sLink);

        sLink = NULL;
        pILibAccDst->GetFileAsBSTR(sPOU, NULL, L".link", &sLink);
        if(sLink)
            pszMangDst = CGT_IECMangeledNameFromLinkFile(sLink);
        FC_FREE_BSTR(sLink);

        if(!pszMangSrc || !pszMangDst)
        {
            CGT_IERR(pdm, pdm->jot1()<<_T("format error in link file: ")<<sPOU<<_T("\\.link"));
            delete(pszMangSrc);
            delete(pszMangDst);
            return false;
        }

        if(!wcsicmp(pszMangSrc, pszMangDst))
        {
            hr = pILibAccDst->ExistsPOU(sPOU, sTrg);
            if(hr == S_OK)
            {
                pdm->userMsg((pdm->jot1()<<sPOU).tab(25)<<_T(": update target '")<<sTrg<<_T("'\n"));
                hr = pILibAccDst->DeleteFiles(sPOU, sTrg, NULL);
            }
            else
            {
                assert(hr ==S_FALSE);
                pdm->userMsg((pdm->jot1()<<sPOU).tab(25)<<_T(": add target '")<<sTrg<<_T("'\n"));
            }
            
            if(!LibCopyFiles(pdm, pILibAccSrc, pILibAccDst, sPOU, sTrg))
            {
                delete(pszMangSrc);
                delete(pszMangDst);
                return false;
            }
        }
        else
        {
            BSTR bstr;
            pdm->msg1(CG_E_LIB_MERGE_IECIF_DIFFER, NULL, sPOU);

            pILibAccSrc->GetLibFileName(&bstr);
            pdm->userMsg(pdm->jot1()<<bstr<<_T(" mangeled name: ")<<pszMangSrc<<_T("\n"));
            FC_FREE_BSTR(bstr);

            pILibAccDst->GetLibFileName(&bstr);
            pdm->userMsg(pdm->jot1()<<bstr<<_T(" mangeled name: ")<<pszMangDst<<_T("\n"));
            FC_FREE_BSTR(bstr);
        }
        delete(pszMangSrc);
        delete(pszMangDst);
    }


    return true;
}


extern "C" void AppendLibrary(
    CGT_CDiagMsg*                    pdm,
    ICG_IECFront*                    pIFront,
    const CMDL_OPTIONSTYP*           pOpt
)
{
    FC_CString    srcFullPath;
    FC_CString    dstFullPath;
    HRESULT       hr;
    BSTR          bstr        = NULL;
    BSTR          bstrErr     = NULL;
    I4CLOpen*     pILibOpen   = NULL;
    I4CLAccess*   pILibAccSrc = NULL;
    I4CLAccess*   pILibAccDst = NULL;
    SAFEARRAY*    parrPOUs;
    unsigned long ul;
    

    //make full path for dest and source libs:
    if(!MakeLibFullPath(pdm, pIFront, pOpt->szSourceLib, &srcFullPath))
        return;
    if(!MakeLibFullPath(pdm, pIFront, pOpt->szMakeLib, &dstFullPath))
        return;

    pdm->userMsg(_T("4cg: merge libraries\n"));
    pdm->userMsg(_T("source     : "));
    pdm->userMsg(srcFullPath);
    pdm->userMsg(_T("\n"));
    pdm->userMsg(_T("destination: "));
    pdm->userMsg(dstFullPath);
    pdm->userMsg(_T("\n\n"));



    //src must exists:
    if(!FC_Exist(srcFullPath))
    {
        pdm->msg1(CG_E_FILE_NOT_EXIST, NULL, srcFullPath);
        return;
    }
    

    //if dest lib does not exists, simply copy src -> dest:
    if(!FC_Exist(dstFullPath))
    {
        pdm->msg1(CG_I_MAKELIB_NEW, NULL, dstFullPath);
        
        if(!CopyFile(srcFullPath, dstFullPath, true))
            pdm->msg2(CG_E_FILE_COPY, NULL, srcFullPath, dstFullPath);
        return;
    }




    //create lib access instance:
    hr = CoCreateInstance(
              CLSID_FC4CLAccess,
              NULL, 
              CLSCTX_INPROC_SERVER,
              IID_I4CLOpen, 
              (void**)&pILibOpen
         );    

    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("CoCreateInstance for CLSID_FC4CLAccess"), NULL);
        return;
    }

    
    //open src for read access:
    bstr  = FC_StringToBSTR(srcFullPath);
    hr    = pILibOpen->Open(false, bstr, &bstrErr, &pILibAccSrc);
    if(FAILED(hr))
    {
        assert(!pILibAccSrc && bstrErr);
        pdm->msg1(CG_E_LIBRARIAN_ERROR, NULL, srcFullPath);
        pdm->userMsg(pdm->jot1()<<_T("    ")<<bstrErr<<_T("\r\n"));
        FC_FREE_BSTR(bstrErr);
        goto Error;
    }
    FC_FREE_BSTR(bstr);



    //open dst for write access:
    bstr  = FC_StringToBSTR(dstFullPath);
    hr    = pILibOpen->Open(true, bstr, &bstrErr, &pILibAccDst);
    if(FAILED(hr))
    {
        assert(!pILibAccDst && bstrErr);
        pdm->msg1(CG_E_LIBRARIAN_ERROR, NULL, dstFullPath);
        pdm->userMsg(pdm->jot1()<<_T("    ")<<bstrErr<<_T("\r\n"));
        FC_FREE_BSTR(bstrErr);
        goto Error;
    }
    FC_FREE_BSTR(bstr);




    //loop over all POUs of the src lib:
    hr = pILibAccSrc->GetAllPOUs(&parrPOUs);
    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("GetAllPOUs() failed for library"), srcFullPath);
        goto Error; 
    }
    for(ul=0; ul<parrPOUs->rgsabound[0].cElements; ul++)
    {
        BSTR          sPOU;
        unsigned long ul2;
        SAFEARRAY*    sTrgTypes;
        bool          bError;
        
        sPOU = ((BSTR*)(parrPOUs->pvData))[ul];

        hr = pILibAccSrc->GetTargetTypesOfPOU(sPOU,  &sTrgTypes);
        if(FAILED(hr))
        {
            pdm->sysErr(hr, NULL, _T("GetTargetTypesOfPOU() failed for library"), srcFullPath);
            goto Error; 
        }

        //loop all targets of the src POUs, only if there are any targets 
        //something gets appended (this way e.g. CONST remains unchanged)
        bError = false;
        for(ul2=0; ul2<sTrgTypes->rgsabound[0].cElements; ul2++)
        {
            if(!AppendLibPOU(pdm, pILibAccSrc, pILibAccDst, sPOU, ((BSTR*)(sTrgTypes->pvData))[ul2]))
            {
                bError = true;
                break;
            }
        }
        SafeArrayDestroy(sTrgTypes);

        if(bError)
            break;
    }
    SafeArrayDestroy(parrPOUs);



    hr = pILibOpen->Close(pILibAccSrc);
    assert(hr==S_OK);
    hr = pILibOpen->Close(pILibAccDst);
    assert(hr==S_OK);


Error:
    FC_FREE_BSTR(bstr);
    FC_FREE_BSTR(bstrErr);
    FC_RELEASE_PTR(pILibOpen);
    FC_RELEASE_PTR(pILibAccSrc);
    FC_RELEASE_PTR(pILibAccDst);
}



static bool CheckInFile(
    CGT_CDiagMsg*   pdm,
    const CG_Edipos*pEdp,
    I4CLAccess*     pILibAcc,
    const TCHAR*    pszPouName,
    const TCHAR*    pszTrgType,
    const TCHAR*    pszFileAndPath
)
{

    assert(FC_StringIsAbsPath(pszFileAndPath));

    BSTR bstrErr     = NULL;
    BSTR sPOUName    = FC_StringToBSTR(pszPouName);
    BSTR sTargetType = pszTrgType ? FC_StringToBSTR(pszTrgType) : NULL;
    BSTR sFileSpec   = FC_StringToBSTR(pszFileAndPath);

    HRESULT hr = pILibAcc->CheckIn(
                    sPOUName, 
                    sTargetType,
                    sFileSpec,
                    &bstrErr
                 );

    FC_FREE_BSTR(sPOUName);
    FC_FREE_BSTR(sTargetType);
    FC_FREE_BSTR(sFileSpec);
    if(FAILED(hr))
    {
        BSTR bstrLibFileName = NULL;
        pILibAcc->GetLibFileName(&bstrLibFileName);
        pdm->msg1(CG_E_LIBRARIAN_ERROR, pEdp, bstrLibFileName);
        pdm->userMsg(pdm->jot1()<<_T("    ")<<bstrErr<<_T("\r\n"));
        FC_FREE_BSTR(bstrLibFileName);
    }
    FC_FREE_BSTR(bstrErr);

    return SUCCEEDED(hr);
}



static bool CheckInInterface(
    CGT_CDiagMsg*   pdm,
    I4CLAccess*     pILibAcc,
    const TCHAR*    pszPouName,
    const TCHAR*    fullPathIntHdr,
    long            interfaceBytes
)
{
    bool          bOk;
    TCHAR*        pszContent; 
    unsigned long ulLength;


    bOk = CGT_LoadTextFileToMem(
        pdm,
        NULL,
        NULL,
        fullPathIntHdr,
        &pszContent, 
        &ulLength, 
        NULL
      );
    if(!bOk)
        return false;

    if(interfaceBytes==0)
        interfaceBytes = ulLength;

    assert((long)ulLength>=interfaceBytes);
    interfaceBytes = min((long)ulLength, interfaceBytes);

    BSTR sPOUName    = FC_StringToBSTR(pszPouName);
    BSTR sFileSpec   = FC_StringToBSTR(FC_StringGetFilePart(fullPathIntHdr));
#ifdef UNICODE 
#error "rework this code"
#endif
    HRESULT hr = pILibAcc->PutFileRaw( 
                    sPOUName,
                    NULL,
                    sFileSpec,
                    (unsigned char*)pszContent,
                    interfaceBytes
                 );
    FC_FREE_BSTR(sPOUName);
    FC_FREE_BSTR(sFileSpec);

    delete(pszContent);

    if(FAILED(hr))   
    {
        BSTR sLibFileName = NULL;
        pILibAcc->GetLibFileName(&sLibFileName);
        pdm->sysErr(hr, NULL, 
            pdm->jot2()<<sLibFileName<<_T(" I4CLAccess::CheckIn failed"),
            pdm->jot1().fmtSysErr(hr));
        FC_FREE_BSTR(sLibFileName);
    }
    return SUCCEEDED(hr);
}



static bool CheckInText(
    CGT_CDiagMsg*   pdm,
    I4CLAccess*     pILibAcc,
    const TCHAR*    pszPouName,
    const TCHAR*    pszTrgType,
    const TCHAR*    pszFileName,
    const TCHAR*    pszText,
    bool            bAppend
)
{
    HRESULT  hr;
    BSTR     sPOU      = FC_StringToBSTR(pszPouName);
    BSTR     sTRG      = (pszTrgType && pszTrgType[0]) ? FC_StringToBSTR(pszTrgType) : NULL;
    BSTR     sFILE     = FC_StringToBSTR(pszFileName);
    DWORD    nBytesNew = pszText ? _tcslen(pszText) : 0;
    DWORD    nBytesOld;

#ifdef UNICODE 
#error "rework this code"
#endif
    if(bAppend && S_OK==pILibAcc->ExistsFile(sPOU, sTRG, sFILE, &nBytesOld))
    {
        byte* pNew = new byte[nBytesOld + nBytesNew + 2];
        hr = pILibAcc->GetFileRaw(sPOU, sTRG, sFILE, pNew, nBytesOld);
        if(hr==S_OK)
        {
            memcpy(pNew+nBytesOld, pszText, nBytesNew);
            pNew[nBytesOld+nBytesNew+0] = '\r'; //HACK assume here that pszText is trimmed single line
            pNew[nBytesOld+nBytesNew+1] = '\n';

            hr = pILibAcc->PutFileRaw(sPOU, sTRG, sFILE, pNew, nBytesOld + nBytesNew +2);
        }
        delete(pNew);
    }
    else
    {
        hr = pILibAcc->PutFileRaw(sPOU, sTRG, sFILE, (unsigned char*)pszText, nBytesNew);
    }

    FC_FREE_BSTR(sPOU);
    FC_FREE_BSTR(sTRG);
    FC_FREE_BSTR(sFILE);

    if(FAILED(hr))   
    {
        BSTR sLibFileName = NULL;
        pILibAcc->GetLibFileName(&sLibFileName);
        pdm->sysErr(hr, NULL, 
            pdm->jot2()<<sLibFileName<<_T(" I4CLAccess::CheckIn failed"),
            pdm->jot1().fmtSysErr(hr));
        FC_FREE_BSTR(sLibFileName);
    }
    return SUCCEEDED(hr);
}


static bool CheckInLinkFile(
    CGT_CDiagMsg*     pdm,
    I4CLAccess*       pILibAcc,
    CGT_STDLINK_INFO* pLnkInf
)
{
    int        i;
    CGT_CDmp   dmp;
    FC_CString jot(MAX_PATH);

    //dump .link in mem stream:
    dmp<<CG_LINK_LIB_VERSION  _T(";");
    dmp.dumpAsCSV(pLnkInf->version,-1,true) <<_T('\n');
    
    dmp<<CG_LINK_LIB_IECIF    _T(";");
    dmp.dumpAsCSV(pLnkInf->iecif,-1,true)   <<_T('\n');
    
    dmp<<CG_LINK_LIB_POUCLASS _T(";");
    dmp.dumpAsCSV(pLnkInf->pouclass,-1,true)<<_T('\n');
    
    dmp<<CG_LINK_LIB_TRGTYP   _T(";");
    dmp.dumpAsCSV(pLnkInf->trgtyp,-1,true)  <<_T('\n');
    
    dmp<<CG_LINK_LIB_HEADER   _T(";");
    dmp.dumpAsCSV(pLnkInf->header,-1,true)<<_T(';')<<pLnkInf->hbytes<<_T('\n');

    dmp<<CG_LINK_LIB_SOURCE   _T(";");
    dmp.dumpAsCSV(pLnkInf->source,-1,true)<<_T('\n');
                                     
    for(i=0; i<pLnkInf->attr.getUsed(); i++)
    {
        const TCHAR* pszVal = jot.clear().appendUpTo(pLnkInf->attr.get(i), _T("="));
        dmp<<CG_LINK_LIB_POUATTR _T(";")<<jot<<_T(";");
        dmp.dumpAsCSV(pszVal, -1, true)<<_T('\n');
    }
    for(i=0; i<pLnkInf->links.getUsed(); i++)
    {
        dmp<<CG_LINK_LIB_LINK _T(";");
        dmp.dumpAsCSV(pLnkInf->links.get(i), -1, true)<<_T('\n');
    }


    BSTR sPOUName    = FC_StringToBSTR(pLnkInf->pouName);
    BSTR sTargetType = FC_StringToBSTR(pLnkInf->trgtyp);
    BSTR sFileSpec   = FC_StringToBSTR(".link");
#ifdef UNICODE 
#error "rework this code"
#endif
    HRESULT hr = pILibAcc->PutFileRaw( 
                    sPOUName,
                    sTargetType,
                    sFileSpec,
                    (unsigned char*)dmp.getPChars(),
                    dmp.getNChars()
                 );
    FC_FREE_BSTR(sPOUName);
    FC_FREE_BSTR(sTargetType);
    FC_FREE_BSTR(sFileSpec);

    if(FAILED(hr))            
    {
        BSTR sLibFileName = NULL;
        pILibAcc->GetLibFileName(&sLibFileName);
        pdm->sysErr(hr, NULL, 
            pdm->jot2()<<sLibFileName<<_T(" I4CLAccess::CheckIn failed"),
            pdm->jot1().fmtSysErr(hr));
        FC_FREE_BSTR(sLibFileName);
    }
    return SUCCEEDED(hr);
}



static bool CheckInCSTs(
    CGT_CDiagMsg*               pdm,
    const CMDL_OPTIONSTYP*      pOpt,
    ICG_IECFront*               pIFront,
    I4CLAccess*                 pILibAcc
)
{
    CGT_CDmp                    dmp;
    const TCHAR*                pszCSTfull;
    FC_CString                  jot(MAX_PATH);
    const CG_POU4CPInfo*const*  ppInf;
    const TCHAR*                pszPrjPath;

    pszPrjPath = pIFront->FCPgetPrjPath();

    for(ppInf = pIFront->FCPgetAllCSTs(); *ppInf; ppInf++)
    {
        //build up a file named "cst" that holds the natural order of cst files:
        dmp<<FC_StringGetFilePart((*ppInf)->pszFile)<<_T("\n");

        if(FC_StringIsAbsPath((*ppInf)->pszFile))
            pszCSTfull = (*ppInf)->pszFile;
        else
            pszCSTfull = jot.load(pszPrjPath).addc(_T('\\'))<<(*ppInf)->pszFile;

        if(pOpt->nVerbose > (pOpt->bRecursive?1:0)) 
            pdm->msg1(CG_I_PUT_LIB, NULL, (*ppInf)->pszFile);

        if(!CheckInFile(pdm, &(*ppInf)->edp, pILibAcc, CG_LIB_POU_CONST, NULL, pszCSTfull))
            return false;
    }

    if(dmp.getNChars())
    {
        if(!CheckInText(pdm, pILibAcc, CG_LIB_POU_CONST, NULL, CG_PREDEF_SOURCE_CST, dmp.getSzBuffer(), false))
            return false;
    }
    //else: no cst at all

    return true;
}




bool CheckInVersionInfo(
    CGT_CDiagMsg*   pdm,
    ICG_IECFront*   pIFront,
    I4CLAccess*     pILibAcc,
    const TCHAR*    pszTrgTyp,
    const TCHAR*    pszCompilerVersion
)
{
    const CG_KeyValPair* pUserVer;
    FC_CString           verinfo;
    

    pUserVer = pIFront->lookUpAttrib(pIFront->FCPgetSection(CG_SEC_4CP_ATTRIBUTES), CG_KEY_4CP_VERSION);
    
    if(pUserVer)
        verinfo<<pUserVer->pszVal;
    verinfo<<_T("/")<<pszCompilerVersion;
    
    if(!CheckInText(pdm, pILibAcc, CG_LIB_POU_VERSION, NULL, _T("v"), verinfo.psz(), false))
        return false;

    if(pszTrgTyp && pszTrgTyp[0])
    {
        if(!CheckInText(pdm, pILibAcc, CG_LIB_POU_VERSION, pszTrgTyp, _T("v"), verinfo.psz(), false))
            return false;
    }
    return true;
}




static void PumpUpLibrary(
    CGT_CDiagMsg*           pdm,
    const CMDL_OPTIONSTYP*  pOpt,
    ICG_IECFront*           pIFront,
    const TCHAR*            pszLibFile,
    const TCHAR*            pszTrgTyp,
    const TCHAR*            pszCompilerVersion
)
{
    HRESULT                     hr;
    TCHAR                       libFullPath[2*MAX_PATH];
    I4CLOpen*                   pILibOpen = NULL;
    I4CLAccess*                 pILibAcc  = NULL;
    BSTR                        bstr      = NULL;
    BSTR                        bstrErr   = NULL;
    const TCHAR*                pszGenPath;
    const TCHAR*                pszPrjPath;
    FC_CString                  linkFile(MAX_PATH);
    CGT_CSVFile                 linkReader;
    const CG_POU4CPInfo*const*  ppInf;
    CG_Edipos                   edptmp;
    FC_CString                  jot(MAX_PATH);
    CGT_STDLINK_INFO            lnkInf;
    int                         i;
    const TCHAR*                pzpszSpecialPOUs[] = {_T("__console"), 
                                                      _T("__control"),    
                                                      NULL}; 


    //used often:
    pszGenPath = pIFront->FCPgetGenPath();
    pszPrjPath = pIFront->FCPgetPrjPath();


    assert(pdm->hasNoErr());

    //create lib access instance:
    hr = CoCreateInstance(
              CLSID_FC4CLAccess,
              NULL, 
              CLSCTX_INPROC_SERVER,
              IID_I4CLOpen, 
              (void**)&pILibOpen
         );    

    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("CoCreateInstance for CLSID_FC4CLAccess"), NULL);
        goto Error;
    }



    //make full path relative prj dir, the  opt. $(libdir)
    //was expanded in calling function already:
    FC_FileFullPath(pszPrjPath, pszLibFile, libFullPath, FC_ARRAY_LEN(libFullPath));


    //remove any existing unless append mode:
    if(!CGT_DeleteFile(pdm,NULL, libFullPath))
        goto Error;

    if(pOpt->nVerbose > (pOpt->bRecursive?1:0))
        pdm->msg1(CG_I_MAKELIB_NEW, NULL, libFullPath);


    //open for write access:
    bstr  = FC_StringToBSTR(libFullPath);
    hr    = pILibOpen->Open(true, bstr, &bstrErr, &pILibAcc);
    if(FAILED(hr))
    {
        assert(!pILibAcc && bstrErr);
        pdm->msg1(CG_E_LIBRARIAN_ERROR, NULL, libFullPath);
        pdm->userMsg(pdm->jot1()<<_T("    ")<<bstrErr<<_T("\r\n"));
        goto Error; 
    }
    assert(pILibAcc && !bstrErr);
    FC_FREE_BSTR(bstr);
    FC_FREE_BSTR(bstrErr);

    
    if(!CheckInVersionInfo(pdm, pIFront, pILibAcc, pszTrgTyp, pszCompilerVersion))
        return;

    if(!CheckInCSTs(pdm, pOpt, pIFront, pILibAcc))
        goto Error;


    for(ppInf = pIFront->FCPgetAllPOUs(); *ppInf; ppInf++)
    {
        if(pszTrgTyp && pszTrgTyp[0])
            linkFile.load(_T("."))<<pszTrgTyp<<_T("\\")CG_LINK_FILE _T("\\");
        else
            linkFile.load(CG_LINK_FILE _T("\\"));

        linkFile<<FC_StringGetFilePart((*ppInf)->pszFile)<<CG_LINK_FILE;

        if(!FC_FileExists(pszGenPath, linkFile))
            continue; //no warning, back end must warn if don't want to 
                      //check in e.g. program or has var_external
        

        
        //open link file and initialise csv reader:
        if(!CGT_LoadCSVFile(pdm, &linkReader, pszGenPath, linkFile))
            goto Error;
     
        if(!CGT_ReadLinkFile(&linkReader, &lnkInf, &jot))
        {
            //TODO
            linkFile.load("error in file: ")<<linkFile<<": "<<jot;
            CGT_IERR(pdm, linkFile);
            goto Error;
        }
 
        if(lnkInf.attr.findI(_T("lib=no")))
            continue;

        if(pOpt->nVerbose > (pOpt->bRecursive?1:0)) 
            pdm->msg1(CG_I_PUT_LIB, NULL, (*ppInf)->pszFile);

        
        if(pszTrgTyp && pszTrgTyp[0])
        {
            if(!CheckInLinkFile(pdm, pILibAcc, &lnkInf))
                goto Error;

            while(FC_StringStartsWith(linkReader.getCurrChars(), CG_LINK_LIB_PUT)>=0)
            {
                long         nCurrLine = linkReader.getLineno();
                const TCHAR* fields[3]; 
                if(linkReader.getRecordFields(FC_ARRAY_LEN(fields), fields)==2)
                {
                    if(!CheckInFile(pdm, NULL, pILibAcc, lnkInf.pouName, pszTrgTyp, fields[1]))
                       goto Error; 
                }
                else
                {
                    CG_InitSTEdipos(&edptmp, linkFile, nCurrLine);
                    pdm->msg1(CG_E_SYNTAX_EXPECTED, &edptmp, _T("put;<path>\\<file>"));
                    goto Error;
                }
            }
        }//called from MakeLibraryStub


        //build <pouname\.link from the target type specific .link
        lnkInf.version.load(CG_FRONT_END_VERSION);
        lnkInf.trgtyp.load("");
        lnkInf.source.load("");
        lnkInf.links.clear();

        if(!CheckInLinkFile(pdm, pILibAcc, &lnkInf))
            goto Error;

        //check in also the inferface portion of header:    
        jot.load(pszGenPath);
        if(pszTrgTyp && pszTrgTyp[0])
            jot<<_T("\\.")<<pszTrgTyp;

        jot<<_T("\\")CG_LINK_FILE _T("\\")<<lnkInf.header;
        if(!CheckInInterface(pdm, pILibAcc, lnkInf.pouName, jot, lnkInf.hbytes))
           goto Error; 
    }


    
    if(pszTrgTyp && pszTrgTyp[0])
    {
        for(i=0; pzpszSpecialPOUs[i]; i++)
        {
            linkFile.load(_T("."))<<pszTrgTyp<<_T("\\")CG_LINK_FILE _T("\\");
            linkFile<<pzpszSpecialPOUs[i]<<CG_LINK_FILE;

            if(!FC_FileExists(pszGenPath, linkFile))
                continue; //no warning, back end must warn
        
            if(pOpt->nVerbose > (pOpt->bRecursive?1:0)) 
                pdm->msg1(CG_I_PUT_LIB, NULL, pzpszSpecialPOUs[i]);

            //open link file and initialise csv reader:
            if(!CGT_LoadCSVFile(pdm, &linkReader, pszGenPath, linkFile))
                goto Error;
     
            while(!linkReader.isEOF())
            {
                const TCHAR* pszField;
                if(!linkReader.getNextField(&pszField))
                {
                    CG_InitSTEdipos(&edptmp, linkFile, linkReader.getLineno());
                    pdm->msg1(CG_E_SYNTAX_EXPECTED, &edptmp, _T("put;<path>\\<file>"));
                    goto Error;

                }

                if(!_tcscmp(pszField, CG_LINK_LIB_PUT))
                {
                    if(linkReader.isLastFieldEOL() || !linkReader.getNextField(&pszField) || !linkReader.isLastFieldEOL())
                    {
                        CG_InitSTEdipos(&edptmp, linkFile, linkReader.getLineno());
                        pdm->msg1(CG_E_SYNTAX_EXPECTED, &edptmp, _T("put;<file>"));
                        goto Error;
                    }

                    if(!CheckInFile(pdm, NULL, pILibAcc, pzpszSpecialPOUs[i], pszTrgTyp, pszField))
                       goto Error; 
                }
                else
                {
                    CG_InitSTEdipos(&edptmp, linkFile, linkReader.getLineno());
                    pdm->msg2(CG_W_SYNTAX_EXPECTED_FOUND, &edptmp, CG_LINK_LIB_PUT, pszField);
                
                    while(!linkReader.isLastFieldEOL())
                        linkReader.getNextField(&pszField);
                }
            }
        }
    }//else called from MakeLibraryStub only


    if(!Process4CPLibPuts(pdm, pOpt, pIFront, pILibAcc, pszTrgTyp))
        goto Error;


    hr = pILibOpen->Close(pILibAcc);
    assert(!FAILED(hr));
Error:
    FC_FREE_BSTR(bstr);
    FC_FREE_BSTR(bstrErr);
    FC_RELEASE_PTR(pILibOpen);
    FC_RELEASE_PTR(pILibAcc);
}








static bool MakeLibFullPath(
    CGT_CDiagMsg*  pdm,
    ICG_IECFront*  pIFront,
    const TCHAR*   pszLib,      //may contain $(libdir)
    FC_CString*    plibFullPath //receives the full path
)
{
    TCHAR                     libFullPath[2*MAX_PATH];
    FC_CString                libExpanded(MAX_PATH);
    const TCHAR*              pszPrjPath;
    CG_Edipos                 edp;
    
    plibFullPath->clear();

    //expand opt. $(libdir) e.g. $(libdir)\4C_system.4cl:
    CG_InitCMDLEdipos(&edp);
    long flags = CGT_INS_PRINTHELP|CGT_INS_WARN_UNDEF;
    int ret = CGT_SubsStdDollarIns(pdm, pIFront, pszLib, &edp, flags, &libExpanded);
    if(ret==0)
        return false;

    pszPrjPath = pIFront->FCPgetPrjPath();
    if(pszPrjPath[0]==0)
        pszPrjPath = NULL; //no project loaded, then use cwd (pass NULL to arg#1 of FC_FileFullPath)

    FC_FileFullPath(pszPrjPath, libExpanded, libFullPath, FC_ARRAY_LEN(libFullPath));
    plibFullPath->load(libFullPath);
    return true;
}


static I4CLAccess* OpenLib(
    CGT_CDiagMsg*  pdm,
    ICG_IECFront*  pIFront,
    bool           bWrite,
    const TCHAR*   pszLib,      //may contain $(libdir)
    I4CLOpen**     ppILibOpen,  //out param
    FC_CString*    plibFullPath //if not NULL receives the full path
)
{
    HRESULT                   hr;
    I4CLAccess*               pILibAcc  = NULL;
    BSTR                      bstr      = NULL;
    BSTR                      bstrErr   = NULL;
    FC_CString                libFullPath;

    *ppILibOpen = NULL;
    if(plibFullPath)
        plibFullPath->clear();


    if(!MakeLibFullPath(pdm, pIFront, pszLib, &libFullPath))
        return NULL;

    if(plibFullPath)
        plibFullPath->load(libFullPath);

    if(!FC_Exist(libFullPath))
    {
        pdm->msg1(CG_E_FILE_NOT_EXIST, NULL, libFullPath);
        return NULL;
    }


    //create lib access instance:
    hr = CoCreateInstance(
              CLSID_FC4CLAccess,
              NULL, 
              CLSCTX_INPROC_SERVER,
              IID_I4CLOpen, 
              (void**)ppILibOpen
         );    

    if(FAILED(hr))
    {
        pdm->sysErr(hr, NULL, _T("CoCreateInstance for CLSID_FC4CLAccess"), NULL);
        return NULL;
    }

    //open for read access:
    bstr  = FC_StringToBSTR(libFullPath);
    hr    = (*ppILibOpen)->Open(bWrite, bstr, &bstrErr, &pILibAcc);

    if(FAILED(hr))
    {
        assert(!pILibAcc && bstrErr);
        pdm->msg1(CG_E_LIBRARIAN_ERROR, NULL, libFullPath);
        pdm->userMsg(pdm->jot1()<<_T("    ")<<bstrErr<<_T("\r\n"));
        FC_RELEASE_PTR(*ppILibOpen);
        return NULL;
    }

    assert(pILibAcc && !bstrErr);
    FC_FREE_BSTR(bstr);

    return pILibAcc;
}



//handle all 
//[compiler]
//lib.put=source > <dest> 
//commands, example:
//
//[compiler]
//;put all gif files of the project folder into the lib that 
//;is currently created, subfolder is __CONSOLE\<current target type>:
//lib.put=pic\*.*  >  __CONSOLE\4cpc
//lib.license=5
//
static bool Process4CPLibPuts(
    CGT_CDiagMsg*           pdm,
    const CMDL_OPTIONSTYP*  pOpt,
    ICG_IECFront*           pIFront,
    I4CLAccess*             pILibAcc,
    const TCHAR*            pszTrgTyp
)
{
    int                  i;
    const CG_AttribList* pAttr;
    const CG_KeyValPair* pPair;
    FC_CString           src(MAX_PATH);
    FC_CString           dest(MAX_PATH);
    const TCHAR*         endPtr;
    CG_Edipos            edp;
    FC_CString           pouName;
    FC_CString           fileName;
   
    
    assert(pdm->hasNoErr());
 
    //the 'LIB_HELP_FILES_SEC' is a HACK for V1.3 compatibillity:
    pAttr = pIFront->FCPgetSection(LIB_HELP_FILES_SEC);
    if(pAttr &&  (pOpt->nVerbose > (pOpt->bRecursive?1:0)) ) 
        pdm->msg1(CG_I_PUT_LIB, NULL, _T("[")LIB_HELP_FILES_SEC _T("] > __HELP"));

    for(i=0; pAttr && i<pAttr->nAttribs; i++)
    {
        pPair = &pAttr->pairs[i];
        
        src.load(pPair->pszVal);
        if(!FC_StringIsAbsPath(src))
            src.prepend(pIFront->FCPgetPrjPath(), _T("\\"));

        if(!CheckInFile(pdm, &pPair->edpVal, pILibAcc, _T("__HELP"), NULL, src))
            return false;
    }
    

    if(!pszTrgTyp || !pszTrgTyp[0])
        return pdm->hasNoErr();//all other lib.put require a target typ TODO put help also needs target type?



    pAttr = pIFront->FCPgetSection(CG_SEC_4CP_COMPILER);
    for(i=0; pAttr && i<pAttr->nAttribs; i++)
    {
        pPair = &pAttr->pairs[i];
        
        if(!_tcsicmp(pPair->pszKey, CG_BUILDCMD_LIBPUT))
        {
            src.load(pPair->pszVal).cutAtLast(_T(">"));
            src.trim();
            endPtr = FC_StringCGetAfterLast(pPair->pszVal, _T(">"));
            if(!endPtr)
            {
                pdm->msg1(CG_E_SYNTAX_MISSING, &pPair->edpVal, _T(">"));
                pdm->userMsg(_T("    Example: lib.put= *.gif  >  __CONSOLE\n"));
                continue;
            }
            if(src.isEmpty())
            {
                edp = pPair->edpVal;
                CG_IncColEdipos(&edp, endPtr - pPair->pszVal);
                pdm->msg2(CG_E_MISSING_X_BEFORE_Y, &edp, _T("<file name to put>"), _T(">"));
                pdm->userMsg(_T("    Example: lib.put= *.gif  >  __CONSOLE\n"));
                continue;
            }

            pouName.clear().appendUpTo(endPtr, FC_PATH_SEP, &endPtr);
            pouName.trim();
            if(pouName.isEmpty())
            {
                edp = pPair->edpVal;
                CG_IncColEdipos(&edp, endPtr - pPair->pszVal);
                pdm->msg2(CG_E_MISSING_X_AFTER_Y, &edp, _T("<lib POU name>"), _T(">"));
                pdm->userMsg(_T("    Example: lib.put= *.gif  >  __CONSOLE\n"));
                continue;
            }
            if(*endPtr)
            {
                edp = pPair->edpVal;
                CG_IncColEdipos(&edp, endPtr - pPair->pszVal);
                pdm->msg2(CG_W_OLD_SYNTAX, &edp, _T("'\\<target type>'"), 
                    pdm->jot1()<<'\''<<pouName<<_T('\''));
                if(pdm->isDisableWarnings()==0)
                    pdm->userMsg(_T("    Example: lib.put= *.gif  >  __CONSOLE\n"));
            }

            if(!FC_StringIsAbsPath(src))
                src.prepend(pIFront->FCPgetPrjPath(), _T("\\"));

            if(pOpt->nVerbose > (pOpt->bRecursive?1:0)) 
                pdm->msg1(CG_I_PUT_LIB, NULL, pPair->pszVal);

            if(!CheckInFile(pdm, &pPair->edpKey, pILibAcc, pouName, pszTrgTyp, src))
                return false;
        }
        else if(!_tcsicmp(pPair->pszKey, CG_BUILDCMD_LIBTXT))
        {
            src.load(pPair->pszVal).cutAtLast(_T(">"));
            src.trim();
            endPtr = FC_StringCGetAfterLast(pPair->pszVal, _T(">"));
            if(!endPtr)
            {
                pdm->msg1(CG_E_SYNTAX_MISSING, &pPair->edpVal, _T(">"));
                pdm->userMsg(_T("    Example: lib.txt= regsvr32 /s mydll.dll > __CONTROL\\installMyDll.bat\n"));
                continue;
            }
            dest.load(endPtr).trim();
           
            fileName.load(FC_StringGetFilePart(dest));
            dest.stripFilePart();
            pouName.load(dest);

            if(FC_StringHasPath(dest) || fileName.isEmpty() || pouName.isEmpty())
            {
                pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, &pPair->edpVal, _T("<POU name>\\<file name>"), endPtr);
                pdm->userMsg(_T("    Example: lib.txt= regsvr32 /s mydll.dll > __CONTROL\\installMyDll.bat\n"));
                continue;
            }

            if(pOpt->nVerbose > (pOpt->bRecursive?1:0)) 
                pdm->msg1(CG_I_PUT_LIB, NULL, pdm->jot1()<<pPair->pszKey<<'='<<pPair->pszVal);

            if(src.stripBegin(_T("\"")))
                src.removeLastChar(_T("\""));

            if(!CheckInText(pdm, pILibAcc, pouName, pszTrgTyp, fileName, src, true))
                return false;
        }
        else if(!_tcsicmp(pPair->pszKey, CG_BUILDCMD_LIBLIC))
        {
            if(!CheckInText(pdm, pILibAcc, CG_LIB_POU_LICENSE, pszTrgTyp, pPair->pszVal, pPair->pszVal, false))
                return false;
        }
    }

    return pdm->hasNoErr();
}





