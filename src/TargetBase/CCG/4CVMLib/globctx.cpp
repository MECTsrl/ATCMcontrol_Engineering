/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/globctx.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: globctx.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/globctx.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include <stdio.h>
#include <time.h>

#include "fc_tools\fc_registry.h"
#include "fc_tools\fc_fileIO.h"

#include "BuildNr.h"
#include "Product.h"
#include "..\..\inc\vmSharedDef.h" //for reading the classes.bin

#include "instview.h"
#include "license.h"
#include "cgt_tools.h"
#include "cgt_misc.h"
#include "cgt_address.h"
#include "globctx.h"
#include "Parse4CH.h"
#include "helper.h"
#include "dump4asm.h"
#include "vmasm.h"
#include "resource.h"

#include "utilif.h"

#include "4cvmAdapt.h"



static long SwapLong(long val)
{
  long result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[3];
  pResult[1] = pVal[2];
  pResult[2] = pVal[1];
  pResult[3] = pVal[0];
  return result;
}

static short SwapShort(short val)
{
  short result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[1];
  pResult[1] = pVal[0];
  return result;
}




CGT_CDiagMsg*  CGlobCtx::m_gpdm;
CGlobCtx::CGlobCtx(
    ICG_IECFront* pFront
) 
  : m_pIF(pFront), 
    m_dm(MODULE_NAME, NULL), 
    m_popt(&m_pCmdlOpts),
    m_bNccCalled(false),
    m_resources(true, false),
    m_NextFreeClassId(0),
    m_ComplexGlobals(100),
    m_lResTasks(0),
    m_maxChanelArraySizeAllocated(0),
    m_pAddrMaps(NULL)
{
    memset(&m_licData, 0, sizeof(m_licData));
    m_gpdm = &m_dm;

    m_pCmdlOpts.bIncrementelly = false;
    m_pCmdlOpts.nGenAsmMapFile = 0;

    m_AsmFilesCreated = 0;
    m_pConfig = NULL;
    m_dm.setFront(pFront);
    m_pAutoExternals = NULL;
}

CGlobCtx::~CGlobCtx() 
{
    m_targets.deletePtrs();
    m_POUcode.deletePtrs();
    FC_RELEASE_PTR(m_pConfig);

    {
        const FC_CHashPtr<const CG_LIBContext*, LibFunIdFile*>::Pair* pPair;
        for(pPair = m_LibFunIdFiles.firstPair(); pPair; pPair = pPair->pNext)
            pPair->pVal->deletePtrs();

        m_LibFunIdFiles.deletePtrs();
        delete m_pAutoExternals;
    }

    m_DirectAddrEntries.deletePtrs();

    for (int iTask=0; iTask<m_lResTasks; iTask++)
    {
        for (int iChn=0; iChn<m_maxChanelArraySizeAllocated; iChn++)
        {
            if (m_pAddrMaps[iTask].pGIMemWR[iChn].pAdr)
            {
                delete m_pAddrMaps[iTask].pGIMemWR[iChn].pAdr;
                delete m_pAddrMaps[iTask].pGIMemRD[iChn].pAdr;
            }
            
            if (m_pAddrMaps[iTask].pGOMemWR[iChn].pAdr)
            {
                delete m_pAddrMaps[iTask].pGOMemWR[iChn].pAdr;
                delete m_pAddrMaps[iTask].pGOMemRD[iChn].pAdr;
            }
        }

        delete m_pAddrMaps[iTask].pGIMemWR;
        delete m_pAddrMaps[iTask].pGIMemRD;
        delete m_pAddrMaps[iTask].pGOMemWR;
        delete m_pAddrMaps[iTask].pGOMemRD;
    }

    if (m_pAddrMaps)
        delete m_pAddrMaps;

    const FC_CHashPtr<const CG_Task*, MEM_RGNS*>::Pair* pPair3;
    for (pPair3 = m_TaskAllMemRgns.firstPair(); pPair3; pPair3 = pPair3->pNext)
    {
        MEM_RGNS* pAllMemRgns = pPair3->pVal;

        pAllMemRgns->GIMemRgnsWR.deletePtrs();
        pAllMemRgns->GIMemRgnsRD.deletePtrs();
        pAllMemRgns->GOMemRgnsWR.deletePtrs();
        pAllMemRgns->GOMemRgnsRD.deletePtrs();
    }
    m_TaskAllMemRgns.deletePtrs();
}


void CGlobCtx::vminterr(const CG_Edipos* pEdp, const TCHAR* pszFile, long nLine, const TCHAR* pszReason)
{
    if(m_gpdm)
    {
        m_gpdm->intErr(pEdp, pszFile, nLine, pszReason);
    }
    else if(pszReason)
    {
        _tprintf("4cvm:  fatal internal error: at %s(%d): reason=%s\n",
            pszFile, nLine, pszReason);
    }
    else
    {
        _tprintf("4cvm:  fatal internal error: at %s(%d)\n",
          pszFile, nLine);
    }
    exit(1);
}



void CGlobCtx::initLicenseInfo(
    const CG_LIBContext*const* pzLIBs,
    const TCHAR*               pszTrgType
)
{
    const CG_LIBContext*const* ppLIB;
    LIC_LIBS                   lb;
    FC_CGArray<LIC_LIBS>       liclibs;
    I4CLAccess*                pILibAcc;
    SAFEARRAY*                 parrFiles;
    BSTR                       bstrLicInf;
    BSTR                       bstrLicPOU;
    BSTR                       bstrLicTRG;
    bool                       bConsiderLic = false;
    TCHAR                      szTrgType[MAX_PATH];



    bConsiderLic = Adapt4CVM_NeedLicensing2(szTrgType);

    if(szTrgType[0] && pszTrgType[0] && _tcsicmp(szTrgType, pszTrgType))
    {
        m_dm.msg2(CG_E_SETUP_BAD4CVMEXE, NULL, szTrgType, pszTrgType);
        return; 
    }

    if (!bConsiderLic)
    {
        assert(!m_licData.bConsiderLic);
        return; 
    }


    bstrLicPOU = FC_StringToBSTR(CG_LIB_POU_LICENSE);
    bstrLicTRG = FC_StringToBSTR(m_popt->szCurTrgTyp);

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
            m_dm.msg2(CG_E_READ_LIB_DOMAIN, &(*ppLIB)->edp, m_dm.res1(CG_RES_STR_LIC_DATA), (*ppLIB)->pszId);
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
            m_dm.msg2(CG_E_READ_LIB_DOMAIN, &(*ppLIB)->edp, m_dm.res1(CG_RES_STR_LIC_DATA), (*ppLIB)->pszId);
        }
    }

    FC_FREE_BSTR(bstrLicPOU);
    FC_FREE_BSTR(bstrLicTRG);

    if (bConsiderLic)
    {
        SL_Get4CLicData(&m_dm, &m_licData, &liclibs);
    }
    else
        assert(!"no lic prod");

    // for starter kit only 2 targets are allowed
    if(m_licData.bConsiderLic && m_licData.bIsStartKit)
    {
        const CG_AttribList* pAttrList = m_pIF->FCPgetSection(CG_SEC_4CP_TARGETS);
        if(pAttrList)
        {
            int              iTrgs = 0;
            const CG_Edipos* pEdp  = NULL;
            
            for (int i=0; i<pAttrList->nAttribs; i++)
            {
                const CG_KeyValPair* pVal = &pAttrList->pairs[i];
                if(!_tcsicmp(FC_StringGetFileExt(pVal->pszVal), m_popt->szCurTrgTyp))
                    iTrgs++;

                pEdp = &pVal->edpKey;
            }

            if (iTrgs > 2)
                m_dm.msg2(CG_E_LICENSE_TO_MANY_TRG, pEdp, _T("2"), m_dm.jot1()<<iTrgs);
        }
    }
}

HRESULT CreateGuid (FC_CString& sGuidString)
{
    GUID guid;
    HRESULT hrResult;
    LPOLESTR sGuid;

    hrResult = CoCreateGuid(&guid);
    if (hrResult == S_OK) 
    {
        hrResult = StringFromCLSID(guid,&sGuid);
        if (hrResult == S_OK) 
        {
            sGuidString.load(sGuid);
            CoTaskMemFree(sGuid);
        }
    }
    return hrResult;
}

void CGlobCtx::checkSourceProtection()
{
    HRESULT hr;

//    if (!_tcsicmp(m_popt->szCurTrgTyp, _T("4cvxw")) || !_tcsicmp(m_popt->szCurTrgTyp, _T("dcu01")))
    if (Adapt4CVM_NeedSourceProtectionCheck())
    {  
        FC_CString     sz4gh;
        FC_CString     szProjGuid;
        const CG_AttribList* pAttributes = NULL;

        sz4gh.load(m_pIF->FCPgetPrjPath());
        sz4gh.appendDir(_T("project.4gh"));

        pAttributes = m_pIF->FCPgetSection(CG_SEC_4CP_ATTRIBUTES);

        if (!pAttributes)
        {
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }

        BOOL bFound = FALSE;

        for (int i=0; i<pAttributes->nAttribs; i++)
        {
            if (!_tcsicmp(pAttributes->pairs[i].pszKey, _T("guid")))
            {
                szProjGuid.load(pAttributes->pairs[i].pszVal);
                szProjGuid.stripBegin(_T("\""));
                szProjGuid.removeLastChar(_T("\""));
                bFound = TRUE;
                break;
            }
        }

        if (!bFound)
        {
            // OSAI-Hack >>
            // ensure that project guid is present
            if(_tcsicmp(m_popt->szCurTrgTyp, _T("4cosai")) == 0)
            {
                FC_CString  szProjPath;
                FC_CString  szProjName;
                FC_CString  sz4CPFilePath;

                CreateGuid(szProjGuid);

                szProjPath.load(m_pIF->FCPgetPrjPath());
                szProjName.load(m_pIF->FCPgetPrjName());
                sz4CPFilePath.load(szProjPath);
                sz4CPFilePath.appendDir(szProjName);
                sz4CPFilePath << ".4cp";

                int bReadOnly = FC_FileIsReadOnly(sz4CPFilePath.psz());
                if(bReadOnly)
                {
                    FC_FileSetReadOnly(sz4CPFilePath.psz(), FALSE);
                }

                WritePrivateProfileString("Attributes", "GUID", szProjGuid.psz(), sz4CPFilePath.psz());

                if(bReadOnly)
                {
                    FC_FileSetReadOnly(sz4CPFilePath.psz(), TRUE);
                }
            }
            // OSAI-Hack <<
            else
            {
                m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
                return;
            }
        }

        // read 4gh file

        int iFileError = FC_FileExists(NULL, sz4gh);

        if (iFileError != 1)
        {
            // OSAI-Hack >>
            if(_tcsicmp(m_popt->szCurTrgTyp, _T("4cosai")) == 0)
            {
                // generate 4gh file
                gen4GHFile(sz4gh.psz(), szProjGuid.psz());
            }
            else
            {
                m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
                return;
            }
            // OSAI-Hack <<
        }
        
        TCHAR* pBuffer;
        unsigned long ulLength = 0;
        iFileError = FC_LoadDiskTextFileToMem(sz4gh, &pBuffer, &ulLength, NULL);

        if (iFileError!=0 || pBuffer == NULL)
        {
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }
       
        TCHAR* pDecryptBuffer = NULL;
        pDecryptBuffer = new TCHAR[ulLength];
        if (pDecryptBuffer==NULL)
        {
            m_dm.msg0(CG_E_OUTOFMEM, NULL);
            return;
        }
        hr = UTIL_DecryptString(pBuffer, pDecryptBuffer, NULL);

        delete pBuffer;

        if (hr!=S_OK)
        {
            delete pDecryptBuffer;
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }
               
        FC_CString szTok;
        const TCHAR* pNext = pDecryptBuffer;

        // first line password
        pNext = szTok.appendUpTo(pNext, _T("\n\r"), NULL);

        // second line project guid
        if (pNext==NULL)
        {
            delete pDecryptBuffer;
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }
        szTok.clear();
        pNext = szTok.appendUpTo(pNext, _T("\n\r"), NULL);

        if (_tcscmp(szTok, szProjGuid)!=0)
        {
            delete pDecryptBuffer;
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }

        // then all files: filename,protstring
        if (pNext==NULL)
        {
            delete pDecryptBuffer;
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }
        szTok.clear();
        const TCHAR* pLast = pNext;
        pNext = szTok.appendUpTo(pNext, _T("\n\r"), NULL);

        while(pNext!=NULL && *pNext!=_T('\0'))
        {
            // check single files
            FC_CString szFileName;
            FC_CString szFullFileName;
            FC_CString szFileCrypt;

            szFileCrypt.load(szFileName.appendUpTo(szTok, _T(",")));
            szFullFileName.load(m_pIF->FCPgetPrjPath());
            szFullFileName.appendDir(szFileName);

            TCHAR* pFileBuf;
            ulLength = 0;
            iFileError = FC_LoadDiskTextFileToMem(szFullFileName, &pFileBuf, &ulLength, NULL);    
            
            if (iFileError!=0 || pBuffer == NULL)
            {
                delete pDecryptBuffer;
                m_dm.msg1(CG_E_PPROT_FILE_CHANGED, NULL, szFullFileName);
                return;
            }

            TCHAR szFileFingerPrint[40];
            hr = UTIL_GetStringFingerprint(pFileBuf, szFileFingerPrint);

            delete pFileBuf;

            if (_tcscmp(szFileCrypt, szFileFingerPrint)!=0)
            {
                delete pDecryptBuffer;
                m_dm.msg1(CG_E_PPROT_FILE_CHANGED, NULL, szFullFileName);
                return;
            }

            szTok.clear();
            pLast = pNext;
            pNext = szTok.appendUpTo(pNext, _T("\n\r"), NULL);
        }

        // last line protstring of the file itself, except last line
        TCHAR* pRun = pDecryptBuffer;
        while (pRun!=pLast && *pRun!=_T('\0'))
        {
            pRun++;
        }
        *pRun = _T('\0');
        TCHAR szFingerPrint[40];
        hr = UTIL_GetStringFingerprint(pDecryptBuffer, szFingerPrint);
        
        if (hr!=S_OK)
        {
            delete pDecryptBuffer;
            return;
        }

        if (_tcscmp(szTok, szFingerPrint)!=0)
        {
            delete pDecryptBuffer;
            m_dm.msg0(CG_E_PPROT_CORRUPT, NULL);
            return;
        }

        delete pDecryptBuffer;

    }


}


void CGlobCtx::initPathInfo()
{
    FC_CString  jot(MAX_PATH);

    
    jot.load(m_pIF->FCPgetGenPath());
    //assert that the front has at least the 4cp loaded
    assert(!jot.isEmpty());
    jot<<_T("\\.") << m_popt->szCurTrgTyp;
    m_TrgTypeGenPath.load(jot);
}

void CGlobCtx::cleanGenDir()
{
    int          i;
    const TCHAR* pszGen = getGenPath();
    FC_CString   trgDir(MAX_PATH);
 
    //do not delete: "gen\<trg typ>\*.*", this is done in 4cg 
    //when rebuild all, otherwise we destroy dbi files of previously 
    //build dbi files! HACK!
    //currentty only dbi files are stored here


    //read in all target files(=directories bellow gen) from command line:
    for(i=0;i<m_pCmdlOpts.targets.getUsed();i++)
    {
        //delete: "gen\<target>\*.*"  
        //- recurse into sub dirs but leave those that match  "__*"
        //  directory "gen\<target>\__cedl" is reserved for the 
        //  downloader's last loaded data base.
        //- all files:
        trgDir.load(FC_StringGetFilePart(m_pCmdlOpts.targets.get(i)));
        trgDir.changeFileExt(NULL)<<_T("\\*");
        CGT_DeleteFiles(&m_dm, pszGen, trgDir, _T("__.list"), true, _T("__*"));
    }
}

bool CGlobCtx::getOptionsFrom4CP()
{
    int is = 0;

    bool    bGenAsmMapFileGiven    = false;
    bool    bCallAsmGiven          = false;
    bool    bCallPostCompilerGiven = false;
    bool    bBPOptionGiven         = false;
    bool    bVerboseGiven          = false;
    bool    bMemInfoGiven          = false;
    bool    bDisableProcessImageGiven= false;

    m_pCmdlOpts.nGenAsmMapFile    = 1;
    m_pCmdlOpts.bCallAsm          = true;
    m_pCmdlOpts.bCallPostCompiler = true;       // default is: call the post compiler
    m_bpopt                       = BP_OPT1;    // default : simple debugging enabled
    m_pCmdlOpts.nVerboseMode      = 1;          // default for output of calling other tools

    FC_CString str(255);
    str << CG_SEC_4CP_COMPILER_BEG << m_pCmdlOpts.szCurTrgTyp;
    const CG_AttribList* pAttrList = m_pIF->FCPgetSection(str);
    if ( ! pAttrList)
        return true; // section not present

    int iMax = pAttrList->nAttribs;
    for (int i=0; i<iMax; i++)
    {
        const CG_KeyValPair* pVal = &pAttrList->pairs[i];
        if (FC_StringStartsWithI(pVal->pszKey, _T("opt")))
        {
            if ( ! _tcsnicmp(pVal->pszVal, _T("map:"), _tcslen(_T("map:"))))
            {
                if (bGenAsmMapFileGiven)
                {
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("map:[0|1|2]"));
                    continue;
                }

                bGenAsmMapFileGiven = true;
                
                if( ! _tcsicmp(pVal->pszVal, _T("map:0")))
                    m_pCmdlOpts.nGenAsmMapFile = 0;
                else if( ! _tcsicmp(pVal->pszVal, _T("map:1")))
                    m_pCmdlOpts.nGenAsmMapFile = 1;
                else if( ! _tcsicmp(pVal->pszVal, _T("map:2")))
                    m_pCmdlOpts.nGenAsmMapFile = 2;
                else
                    m_dm.msg2(CG_W_SYNTAX_EXPECTED_FOUND, &pVal->edpVal, _T("map:[0|1|2]"), pVal->pszVal);
            }
            else if ( ! _tcsncmp(pVal->pszVal, _T("asm:"), _tcslen(_T("asm:"))))
            {
                if (bCallAsmGiven)
                {
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("asm:<0|1>"));
                    continue;
                }

                if( ! _tcsicmp(pVal->pszVal, _T("asm:0")))
                {
                    bCallAsmGiven = true;
                    m_pCmdlOpts.bCallAsm = false;
                }
                else if( ! _tcsicmp(pVal->pszVal, _T("asm:1")))
                {
                    bCallAsmGiven = true;
                }
                else
                {
                    m_dm.msg2(CG_W_SYNTAX_EXPECTED_FOUND, &pVal->edpVal, _T("asm:[0|1]"), pVal->pszVal);
                }
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("ncc:"))) > 0)
            {
                if (bCallPostCompilerGiven)
                {
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("ncc:<0|1>"));
                    continue;
                }

                if (pVal->pszVal[is] == _T('0'))
                {
                    // "ncc:0"
                    bCallPostCompilerGiven = true;
                    m_pCmdlOpts.bCallPostCompiler = false;
                }
                else if(pVal->pszVal[is] == _T('1'))
                {
                    // "ncc:1" 
                    bCallPostCompilerGiven = true;
                }
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("verbose:0"))) > 0)
            {
                if (bVerboseGiven)
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("verbose:<0|1|2>"));
                else
                    m_pCmdlOpts.nVerboseMode = 0;
                bVerboseGiven = true;
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("verbose:1"))) > 0)
            {
                if (bVerboseGiven)
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("verbose:<0|1|2>"));
                else
                    m_pCmdlOpts.nVerboseMode = 1;
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("verbose:2"))) > 0)
            {
                if (bVerboseGiven)
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("verbose:<0|1|2>"));
                else
                    m_pCmdlOpts.nVerboseMode = 2;
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("meminfo:0"))) > 0)
            {
                if (bMemInfoGiven)
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("meminfo:<0|1>"));
                else
                    m_kad.getCgOptionsWrt()->bMemInfo = false;
                bMemInfoGiven= true;
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("meminfo:1"))) > 0)
            {
                if (bMemInfoGiven)
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("meminfo:<0|1>"));
                else
                    m_kad.getCgOptionsWrt()->bMemInfo = true;
                bMemInfoGiven= true;
            }
            else if (!_tcsicmp(pVal->pszVal, _T("DisableProcessImage")))
            {
                if (bDisableProcessImageGiven)
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("DisableProcessImage"));
                else
                {
                    m_kad.disableProcessImage();
                    m_dm.msg1(CG_W_TEXT, &pVal->edpVal, _T("disabled process image: %I and %Q are not in task cache"));
                }
                bDisableProcessImageGiven = true;
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("bp"))) > 0)
            {
                if (bBPOptionGiven)
                {
                    m_dm.msg2(CG_W_CMDL_OPT_TWICE, &pVal->edpVal, _T(""), _T("bp:<num>"));
                    continue;
                }

                if (pVal->pszVal[is] == _T('0'))
                {
                    // "bp0"
                    bBPOptionGiven = true;
                    m_bpopt = BP_OPT0;
                }
                else if (pVal->pszVal[is] == _T('1'))
                {
                    // "bp1"
                    bBPOptionGiven = true;
                    m_bpopt = BP_OPT1;
                }
                else if (pVal->pszVal[is] == _T('2'))
                {
                    // "bp2"
                    bBPOptionGiven = true;
                    m_bpopt = BP_OPT2;
                }
                else
                {
                    m_dm.msg2(CG_W_SYNTAX_EXPECTED_FOUND, &pVal->edpVal, _T("bp[0|1|2]"), pVal->pszVal);
                }
            }
            else if ( (is = FC_StringStartsWithI(pVal->pszVal, _T("postopt:"))) > 0)
            {
                // "postopt:<num>"
                m_pCmdlOpts.sPostCompileOpt.load(&pVal->pszVal[is]);
            }
            else
            {
                m_dm.msg1(CG_E_CMDL_UNKNOWN_OPT, &pVal->edpVal, pVal->pszVal);
            }
        }
        else
        {
            m_dm.msg1(CG_E_CMDL_UNKNOWN_OPT, &pVal->edpVal, pVal->pszVal);
        }
    }

    return true;
}




class CULToKB //helper class to convert unsigned longs to kb format
{
private:
    unsigned long m_ul;
    TCHAR         m_text[256];

    void format(bool fmtKB)
    {
        _stprintf(m_text, _T("%d"), fmtKB ? (m_ul+1023)/1024 : m_ul);
    }
public:
    CULToKB(bool fmtKB, unsigned long ul)
    {
        m_ul = ul;
        format(fmtKB);
    }
    CULToKB(bool fmtKB, const TCHAR* psz)
    {
        m_ul = strtoul(psz, NULL, 0);
        format(fmtKB);
    }

    inline operator const TCHAR*()  const{return m_text;}
};

static void FormatMemInfo(
    const TCHAR*       pszInfFile,
    bool               fmtKB, 
    const VM_SETTINGS* pVMS, 
    unsigned long      ulCodeBytes, 
    FC_CString&        msgtext
)
{
    TCHAR buffer[256];

    msgtext.clear();

    GetPrivateProfileString(_T("handels"), _T("objects"), _T(""), buffer, FC_ARRAY_LEN(buffer), pszInfFile);
    if(buffer[0])
    {
        msgtext<<_T(".dataobj=")<<strtoul(buffer, NULL, 0)-MAX_SEGMENTS;
        if(pVMS->ulMaxObjects != 0xffffffff && pVMS->ulMaxObjects != 0)
            msgtext<<'('<<pVMS->ulMaxObjects-MAX_SEGMENTS<<')';
        msgtext<<' ';
    }//else: should not happen..

    GetPrivateProfileString(_T("handels"), _T("classes"), _T(""), buffer, FC_ARRAY_LEN(buffer), pszInfFile);
    if(buffer[0])
    {
        msgtext<<_T(".codeobj=")<<strtoul(buffer, NULL, 0);
        if(pVMS->ulMaxCodeObjects != 0xffffffff && pVMS->ulMaxCodeObjects != 0)
            msgtext<<'('<<pVMS->ulMaxCodeObjects<<')';
        msgtext<<' ';
    }//else: should not happen..

    msgtext<<_T(".code=")<<CULToKB(fmtKB, ulCodeBytes);
    if(!pVMS->szCodeSegSize.isEmpty())
        msgtext<<'('<<CULToKB(fmtKB, pVMS->szCodeSegSize)<<')';
    msgtext<<' ';

    GetPrivateProfileString(_T("segsize"), _T("local"), _T(""), buffer, FC_ARRAY_LEN(buffer), pszInfFile);
    if(buffer[0])
    {
        msgtext<<_T(".local=")<<CULToKB(fmtKB, buffer);
        if(!pVMS->szLocDataSegSize.isEmpty())
            msgtext<<'('<<CULToKB(fmtKB, pVMS->szLocDataSegSize)<<')';
        msgtext<<' ';
    }//else: should not happen..



    GetPrivateProfileString(_T("segsize"), _T("global"), _T(""), buffer, FC_ARRAY_LEN(buffer), pszInfFile);
    if(buffer[0])
    {
        msgtext<<_T(".global=")<<CULToKB(fmtKB, buffer);
        if(!pVMS->szGMRSegSize.isEmpty())
            msgtext<<'('<<CULToKB(fmtKB, pVMS->szGMiecSize)<<')';
        msgtext<<' ';
    }//else: should not happen..

    GetPrivateProfileString(_T("segsize"), _T("retain"), _T(""), buffer, FC_ARRAY_LEN(buffer), pszInfFile);
    if(buffer[0])
    {
        msgtext<<_T(".retain=")<<CULToKB(fmtKB, buffer);
        if(!pVMS->szGMRSegSize.isEmpty())
            msgtext<<'('<<CULToKB(fmtKB, pVMS->szGMRSegSize)<<')';
        msgtext<<' ';
    }//else: should not happen..
}


void CGlobCtx::checkMeminfo(const CTarget* pTrg)
{
    FC_CString          meminf(MAX_PATH);
    TCHAR               classid[MAX_PATH];
    TCHAR               buffer[MAX_PATH];
    unsigned long       ulTotalCodeIP;
    unsigned long       ulTotalCodeNCC;
    FC_CString          msgtext(MAX_PATH);
    const TCHAR*        psz;


    meminf.load(pTrg->getTrgGenPath())<<_T("\\meminfo.txt");
    
    GetPrivateProfileString(_T("segsize"), _T("code"), _T(""), buffer, FC_ARRAY_LEN(buffer), meminf);
    psz = strchr(buffer, ':');//buffer = "ip: 467"
    assert(psz);
    if(psz)
    {
        psz++;
        FC_StringSkipWhite(&psz);
        ulTotalCodeIP = strtoul(psz, NULL, 0);
        ulTotalCodeNCC= 0;
    }

    if(m_bNccCalled)
    {
        int                 nr;
        bool                bOk;
        XDownHeader         cbHeader;
        FILE*               pFile;
        FC_CString          clsbin(MAX_PATH);
        FC_CString          updated(MAX_PATH);

        clsbin.load(pTrg->getTrgGenPath())<<_T("\\__classes.bin");
        pFile = _tfopen(clsbin, _T("rb"));
        if(!pFile)
        {
            m_dm.msg2(CG_E_FILE_OPEN, NULL, clsbin, m_dm.jot1().fmtSysErr(GetLastError()));
            return;
        }

        bOk = false;
        while(1)
        {
            nr = fread(&cbHeader, 1, sizeof(XDownHeader), pFile);
            if(nr != sizeof(XDownHeader))
            {
                bOk = feof(pFile)!=0;
                break;
            }
			
			if(m_kad.getVmSettings()->bigEndian)
			{
				cbHeader.uIndex   = SwapShort(cbHeader.uIndex);		
				cbHeader.uSegment = SwapShort(cbHeader.uSegment);	
				cbHeader.ulSize   = SwapLong(cbHeader.ulSize);		
				cbHeader.ulOffset = SwapLong(cbHeader.ulOffset);	
			}

            ulTotalCodeNCC += cbHeader.ulSize;
            _stprintf(classid, _T("%04u"), cbHeader.uIndex);
            GetPrivateProfileString(_T("codesizes"), classid, _T(""), buffer, FC_ARRAY_LEN(buffer), meminf);
//            assert(buffer[0]);
            updated.clear().appendWsprintf(_T("nc:%6u %s"), cbHeader.ulSize, buffer);
            WritePrivateProfileString(_T("codesizes"), classid, updated, meminf);
			fseek(pFile, cbHeader.ulSize, SEEK_CUR);
        }

        fclose(pFile);
        if(!bOk)
        {
            m_dm.msg1(CG_E_TEXT, NULL, 
                m_dm.jot1()<<_T("format error in binary file '")<<clsbin<<'\'');
            return;
        }

        GetPrivateProfileString(_T("segsize"), _T("code"), _T(""), buffer, FC_ARRAY_LEN(buffer), meminf);
        assert(buffer[0]);
        updated.clear().appendWsprintf(_T("nc:%6u %s"), ulTotalCodeNCC, buffer);
        WritePrivateProfileString(_T("segsize"), _T("code"), updated, meminf);
    }


    FormatMemInfo(meminf, false, m_kad.getVmSettings(), m_bNccCalled ? ulTotalCodeNCC : ulTotalCodeIP, msgtext);
    WritePrivateProfileString(_T("memuse"), _T("summary"), msgtext, meminf);


    if(m_kad.getCgOptions()->bMemInfo)
    {
        FormatMemInfo(meminf, true, m_kad.getVmSettings(), m_bNccCalled ? ulTotalCodeNCC : ulTotalCodeIP, msgtext);
        m_dm.userMsg(_T("\n"));
        m_dm.userMsg(pTrg->getTrgName());
        m_dm.userMsg(_T(": "));
        m_dm.userMsg(msgtext);
        m_dm.userMsg(_T("\n"));
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

    if(m_dm.hasNoErr() && m_pConfig)//no config is ok, then ignor targets if any
    {                               //no config but nTargets>0 was a warning in 
                                    //loadResources
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
            CGT_CheckVarExternalsRec(&m_dm, m_pIF, pRes, &m_pAutoExternals);


			// calculate direct addresses for global variables
			// for variables with user addresses
            if(m_dm.hasNoErr())
                calcDirectAddresses(pTrg);

            if(nErrOld==m_dm.getTotalErrors())
				calcVarExtAccessInfo(pTrg);

            if(m_dm.hasNoErr())
                buildAsmFile(pTrg);

            if(m_dm.hasNoErr())
            {
                CGT_CDmp dmp4PL;
                const CG_KeyValPair* pAttrVers;
                //user defined version string, might not exist:
                pAttrVers = m_pIF->lookUpAttrib(
                    m_pIF->FCPgetSection(CG_SEC_4CP_ATTRIBUTES), CG_KEY_4CP_VERSION); 

                dmp4PL<<_T("$format;1\n");
                dmp4PL<<_T("$target;")<<pTrg->getTrgType()<<';'<<pTrg->getTrgVer()<<'\n';
                dmp4PL<<_T("$genver;")PROD_VERSION_STR"\n";

                dmp4PL<<_T("$project;");
                dmp4PL.dumpAsCSV(m_pIF->FCPgetPrjName(), -1, true)<<'\n';
                dmp4PL<<_T(_T("$projver;"));
                if(pAttrVers)
                    dmp4PL.dumpAsCSV(pAttrVers->pszVal, -1, true);
                dmp4PL<<'\n';

                dmp4PL<<_T("$resource;")<<pTrg->getResName()<<'\n';
                dmp4PL<<_T("$control;\n");
                dmp4PL<<'\n';
                dmp4PL<<'\n';
                dmp4PL<<_T("$list\n");
                CGT_DumpRetainList(&m_dm, dmp4PL, pRes->pGVL, CGT_DRL_DATA_TYPE_FULL);
                CGT_DmpToFile(&m_dm, pTrg->getTrgGenPath(), _T("retain.4pl"), dmp4PL, false); 
                pTrg->m_paramList.load(_T("retain.4pl"));
                CheckGlobalFBLocalRetain(&m_dm, pRes->pGVL);
            }

            if(m_dm.hasNoErr())
			{
				if(m_AsmFilesCreated > 0)
					callExternalCompiler(pTrg);
				else
					assert(m_dm.getDisplayedWarnings() > 0);
			}

            if(m_AsmFilesCreated>0 && nErrOld==m_dm.getTotalErrors())
                dumpTargetDBIFile(pTrg, pRes);

            //inst view even if no control name given:
            if(nErrOld==m_dm.getTotalErrors())
                 dumpInstView(pTrg, pRes);

            //Finally make loader.list if
            //a) there is a resource assigned
            //b) no front end and no back end error in this resource
            //   and attraced POUs so far.
            //make no loader.list file if no machine name given.
            if(m_AsmFilesCreated>0 && nErrOld==m_dm.getTotalErrors() && pTrg->hasControl())
                dumpTrgLoaderList(pTrg, pzpRefSym);

            if (m_AsmFilesCreated>0 && nErrOld==m_dm.getTotalErrors())
                callPostCompile(pTrg);

            if (m_AsmFilesCreated>0 && nErrOld==m_dm.getTotalErrors())
                checkMeminfo(pTrg);

            if (m_AsmFilesCreated>0 && nErrOld==m_dm.getTotalErrors())
                buildDLFile(pTrg);  // necessary for some targets

            delete(pzpRefSym);

            if(nErrOld==m_dm.getTotalErrors())
            {
                //write build is ok to the sem file:
                FC_CString semf(MAX_PATH);
                semf<<m_pIF->FCPgetGenPath()<<_T("\\project.sem");
                WritePrivateProfileString(_T("compiled"),pTrg->getTrgName(),_T("1"),semf);
            }
        }
    }

    //do at leat check the syntax of the FunFbId.txt:
    //still TODO: check also that each mlib function gets assigned an id inside FunFbId.txt...
    if(FC_FileExists(m_pIF->FCPgetPrjPath(), _T("FunFbId.txt")))
        loadIdLibFunFb(NULL, NULL, NULL);

    //if lib build need the link files for check in:
    if(m_dm.hasNoErr() && !m_popt->szMakeLib.isEmpty())
        dumpPOULinkFiles();
}







void CGlobCtx::checkFile()
{
    int        iRet;
    CG_Edipos  edp;

    assert(!m_popt->szCheckFile.isEmpty());

    //TODO: call asm too but in tmp dir
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
    bool                       bOk;
    FC_CString                 aspf(MAX_PATH);
    const CCodeInfo*           pCInfo;
    const CG_Symtab*           pSym;
    const CG_POU4CPInfo*const* ppInf;
    CGT_CDmp                   dmp;
    FC_CString                 lFile(MAX_PATH);
    bool                       bHasInlinePOU = false;


    //don't call if already errors:
    assert(m_dm.hasNoErr());

    for(ppInf = m_pIF->FCPgetAllPOUs(); *ppInf; ppInf++)
    {
        pSym = m_pIF->loadHeader((*ppInf)->pszId);
        pCInfo = m_POUcode.get(pSym);

        assert(pCInfo);
        assert(pCInfo->m_pSym==pSym && !lstrcmpi((*ppInf)->pszId, pCInfo->m_pSym->pszId));
        assert(!pSym->pLibCtx && !pCInfo->m_bErrors);

        if(pSym->nExternal>0)
        {
            m_dm.msg1(CG_W_CANNOT_PUT_LIB, &pSym->edp, pSym->pszId);
            continue;
        }

        if(pSym->pouTyp==CG_POU_PRG)
            continue; //no prgs in lib...

        if (pSym->flags & CG_STAB_MACRO)
            bHasInlinePOU = true;

        //build back end domain list to be checked into 
        //lib folder: <POU name>\<target type>
        bOk = CGT_DumpLinkFile(
                  &m_dm, 
                  m_pIF, 
                  pSym, 
                  true,
                  m_popt->szCurTrgTyp, 
                  BIN_CODE_VERSION, 
                  NULL,   
                  dmp.getSzBuffer()
              );


        assert(bOk || m_dm.hasErr());
        if(!bOk)
            break;
    }

    // file "FunFbId.txt"
    if(FC_FileExists(m_pIF->FCPgetPrjPath(), _T("FunFbId.txt")))
    {
        //if exist check in even if currently no inline POUs...
        dmp << CG_LINK_LIB_PUT _T(";") << m_pIF->FCPgetPrjPath() << _T("\\FunFbId.txt\n");
    }
    else if(bHasInlinePOU)
    {
        m_dm.msg1(CG_W_NO_IDFILE_IN_PRJDIR, NULL, _T("FunFbId.txt"));
    }

    //write link file to disk:
    lFile.load(_T(".")) << m_popt->szCurTrgTyp << _T("\\.link\\__control.link");
    //assert: must not exist, directory should be clean:
    assert( ! FC_FileExists(m_pIF->FCPgetGenPath(), lFile));

    bOk = CGT_DmpToFile(&m_dm, m_pIF->FCPgetGenPath(), lFile, dmp, false); 
    assert(bOk || m_dm.hasErr());
}


// return -1 if not succesful: fatal error
long CGlobCtx::getIdOfLibFunFb(
    const CG_LIBContext* libCtx,
    const CG_Edipos*     pEdpMac,
    const TCHAR*         pszNameOrLit,
    long*                pMinVersion, //output: min control version, 0 if all versions are ok                
    long*                pMaxVersion  //output: max control version, 99999 if all versions are ok
)
{
    TCHAR*              pszEnd;
    LibFunIdFile*       pLibFunFbIds = NULL;
    const LibFunInfo*   pInf;

    *pMinVersion = 0;
    *pMaxVersion = 99999;

    if(isdigit(*pszNameOrLit))
    {
        long literalid = strtol(pszNameOrLit, &pszEnd, 10);
        if(*pszEnd)
        {
            m_dm.msg0(CG_E_MACRO_EXPANSION, pEdpMac);
            literalid = -1;
        }
        return literalid;
    }


    if(!m_LibFunIdFiles.get(libCtx, &pLibFunFbIds))
    {
        pLibFunFbIds = new LibFunIdFile(true, true, 1231);
        if (!loadIdLibFunFb(libCtx, pEdpMac, pLibFunFbIds))
            return -1;
        m_LibFunIdFiles.insert(libCtx, pLibFunFbIds);        
    }

    pInf = pLibFunFbIds->get(pszNameOrLit);
    if(!pInf)
    {
        m_dm.msg2(CG_E_LIBFUNFB_ID_NOT_DEFINED, pEdpMac, pszNameOrLit, 
            libCtx ? libCtx->pszId : _T("[current project]"));
        return -1;
    }

    *pMinVersion = pInf->nMinVersion;
    *pMaxVersion = pInf->nMaxVersion;

    return pInf->nNumInControl;
}

bool CGlobCtx::loadIdLibFunFb(const CG_LIBContext* libCtx,
                              const CG_Edipos*     pEdp,
                              LibFunIdFile*        pLibFunFbIds)
{
    unsigned long   ulLength = 0;
    TCHAR*          pCharBuf = NULL;
    FC_CString      lctrFileName(255);
    bool            bRes;

    if (libCtx == NULL)
    {
        // located in the current project
        lctrFileName.load(m_pIF->FCPgetPrjPath());
        lctrFileName.appendDir(CG_VM_LIBFUNFB_IDFILE);
    }
    else
    {
        // located in a lib
        if (m_popt->szCurTrgTyp == NULL)
        {
            assert( ! "target type string is empty");
        }
        lctrFileName.load(libCtx->pszId);
        lctrFileName << _T("::__CONTROL\\") << m_popt->szCurTrgTyp 
                     << _T("\\") CG_VM_LIBFUNFB_IDFILE;
    }

    if ( ! CGT_LoadLocatorFileToMem(&m_dm, 
        pEdp, m_pIF, lctrFileName, &pCharBuf, &ulLength, NULL))
    {
        // fatal error
        m_dm.msg2(CG_E_NO_LIBFUNFBFILE_INLIB, 
            pEdp, 
            CG_VM_LIBFUNFB_IDFILE, 
            libCtx == NULL ? m_pIF->FCPgetPrjPath() : libCtx->pszFile);
        return false;
    }

    bRes = fillIdHash(lctrFileName, pLibFunFbIds, pCharBuf);
    if (pCharBuf)
        delete pCharBuf;

    return bRes;
}

bool CGlobCtx::fillIdHash(const TCHAR* pszFunFBidFile, LibFunIdFile* pLibFunFbIds, LPCTSTR pCharBuf)
{
    LPCTSTR     pszPos = pCharBuf;
    LPCTSTR     psz = pCharBuf;
    LPCTSTR     pszLine;
    LPCTSTR     tmp = NULL;
    FC_CString  ident(64);
    int         lineNo=1;
    FC_CString  line(256);
    FC_CString  jot(64);
    TCHAR*      pszEnd;
    long        num;
    long        minver;
    long        maxver;
    CG_Edipos   edp;

    do
    {
        line.clear();
        line.appendUpTo(pszPos, _T("\n"), &pszPos);
        line.trim();

        if (line.isEmpty())
        {
            if (*pszPos == 0)
                break;  // end of file reached

            pszPos++;
            lineNo++;
            continue;
        }

        // COMMENT ?
        if (line.psz()[0] == _T(';'))
        {
            // comment
            if (*pszPos != 0)
                pszPos++;
            lineNo++;
            continue;
        }

        // function name
        ident.clear().appendUpTo(line, _T(" \t"), &pszLine);
        FC_StringSkipWhite(&pszLine);
        if(ident.isEmpty())
            goto Error;

        // fun ID
        jot.clear().appendUpTo(pszLine, _T(" \t;"), &pszLine);
        FC_StringSkipWhite(&pszLine);

        num = strtol(jot, &pszEnd, 10);

        if(jot.isEmpty() || *pszEnd != 0)
            goto Error;


        //min version:
        jot.clear().appendUpTo(pszLine, _T(" \t;"), &pszLine);
        FC_StringSkipWhite(&pszLine);

        minver = strtol(jot, &pszEnd, 10);

        if(jot.isEmpty() || *pszEnd != 0)
            goto Error;

        //max version:
        jot.clear().appendUpTo(pszLine, _T(" \t;"), &pszLine);
        FC_StringSkipWhite(&pszLine);

        maxver = strtol(jot, &pszEnd, 10);

        if(jot.isEmpty() || *pszEnd != 0)
            goto Error;


        LibFunInfo *pInf = new LibFunInfo;
        pInf->nNumInControl = num;
        pInf->nMinVersion   = minver;
        pInf->nMaxVersion   = maxver;

        if(pLibFunFbIds)
            pLibFunFbIds->insert(ident, pInf);
        //else: caller wants only to check syntax...
       
        if (*pszPos != 0)
            pszPos++;

        lineNo++;
    }
    while(*pszPos != 0);

    return true;

Error:
    CG_InitSTEdipos(&edp, pszFunFBidFile, lineNo); 
    m_dm.msg1(CG_E_TEXT, &edp, _T("syntax error in funfbid.txt file, expected a line like this:"));
    m_dm.userMsg(_T("<functionname> <id in control>  <minversion> <maxversion>\n"));
    return false;
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

    //read in all target files from command line:
    for(i=0;i<m_pCmdlOpts.targets.getUsed();i++)
    {
        pTrg = new CTarget();
        
        if(!pTrg->loadTarget(this, m_pCmdlOpts.targets.get(i)))
        {
            delete(pTrg);
            return; //must be a XML parse error, don't continue here
        }

        if (i >= 1)
        {
            m_dm.errNS1(pTrg->getEdp(), m_dm.res1(CG_RES_STR_MULTI_TRG));
            delete(pTrg);
            return; // multiple targets not supported: do not continue
        }
        
        if(!pTrg->hasControl())
        {
            m_dm.msg1(CG_W_TARGET_NO_ADDR, pTrg->getEdp(), pTrg->getTrgName());
        }

        //TODO error if machine name duplicate
        m_targets.add(pTrg);
    }

    //call the initial compile routine of all field bus masters:
    for(i=0;i<m_targets.getUsed();i++)
        m_targets.get(i)->compile(&m_dm, this);
}


void CGlobCtx::loadResources()
{
    int                    i, j;
    const CG_Resource*     pRes;
    const CG_Symtab*const* ppSym;
    CTarget*               ptrg; 
    const CG_ResImpInfo*   pImp;
    int                    nCurrErrors;
    FC_CGArray<const CG_SymEntry*> gvl;


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

        pRes = m_pIF->loadResource(pImp->pszImpFile);
        if(!pRes)
        {
            m_dm.msg1(CG_E_UNDEF_RESOURCE, &pImp->edp, ptrg->getResName());
        }
        else if(pRes->bNoError)
        {
            // check task properties
            for(j = 0; j < pRes->nTasks; j++)
                checkTaskProps(pRes->pTasks + j);
        
            //insertEx: maybe a resource is assigned to more than one
            //target, don't overwrite old pres, overwrite would also work,
            //but this is clearer.
            m_resources.insertEx(pRes->pszId, pRes, false, NULL);

            //get all programs and all global FBs
            //and generate their body code
            nCurrErrors = m_dm.getTotalErrors();

            if(m_kad.getMaxTasks()>=0 && pRes->nTasks>m_kad.getMaxTasks())
            {
                m_dm.errNS3(&pRes->edp, 
                    m_dm.res1(CG_RES_STR_MORE_THAN),
                    m_dm.jot2()<<m_kad.getMaxTasks(), 
                    m_dm.res3(CG_RES_STR_TASKS));
            }

            for(ppSym = pRes->pzGVLRefSyms; *ppSym; ppSym++)
                compilePOU(pRes, *ppSym);

            //if no errors in resource pres->bNoError and also no 
            //error while producing code set the m_pres member of target:
            if(nCurrErrors==m_dm.getTotalErrors())
                ptrg->setCGRes(pRes);
        }
        else
        {
            assert(m_dm.hasErr());
        }
    }

}


void CGlobCtx::checkTaskProps(const CG_Task* pTask)
{
    // nTIConstraints - each bit or a group of bits represent a task properties combination constraint
    // bit 0 represent INTERVAL and EVENT incompatible task properties constranint
    // bit 1 - 63 unused
    unsigned long      nTIConstraints = 0; 
    int                i;
    const TCHAR*       p;
    __int64            jot;
    const CG_TaskInit* pInit;
    const CG_DType*    pType;
    const CG_Literal*  pLit;

    for(i = 0; i < pTask->nInits; i++)
    {
        pInit = pTask->pInits + i;
        pType = pInit->val.pType;
        assert(pInit->val.extyp == CG_LITERAL);
        pLit = pInit->val.lit.pLit;

        if(!_tcsicmp(pInit->pszAttr, "INTERVAL"))
        {
            if(nTIConstraints & 0x01)
                m_dm.msg1(CG_E_TASK_ATTR_COMBI, &pInit->edp, pTask->pszId);

            nTIConstraints |= 0x01;
        }
        else if(!_tcsicmp(pInit->pszAttr, "PRIORITY"))
        {
            assert(pInit->val.extyp == CG_LITERAL && (pType->tbs & CG_TBS_ANY_INT));
            if(pLit->i64 < 0 || pLit->i64 > 9)
                m_dm.msg3(CG_E_BAD_TASK_PRIO, &pInit->val.edp, m_dm.jot1()<<pLit->i64, _T("0"), _T("9"));
        }
        else if(!_tcsicmp(pInit->pszAttr, "AUTOSTART"))
        {
            ;
        }
        else if(!_tcsicmp(pInit->pszAttr, "LOADED"))
        {
            ;
        }
        else if(!_tcsicmp(pInit->pszAttr, "EVENT"))
        {
            if(nTIConstraints & 0x01)
                m_dm.msg1(CG_E_TASK_ATTR_COMBI, &pInit->edp, pTask->pszId);

            nTIConstraints |= 0x01;          

            if(pType->tbs & CG_TBS_ANY_INT)
            {
                if(pLit->i64 < 1 || pLit->i64 > 64)
                    m_dm.msg1(CG_E_BAD_RANGE, &pInit->val.edp, m_dm.jot1()<<pLit->i64);
            }
            else if (pType->tbs & CG_TBS_ANY_STRING)
            {
                for (p = pLit->psz; p; p = _tcschr(p, _T(',')))
                {
                    p   = _tcsinc(p);
                    CG_Edipos tmpedp = pInit->val.edp;
                    CG_IncColEdipos(&tmpedp, p - pLit->psz);

                    jot = _tcstol(p, const_cast<TCHAR**>(&p), 10);

                    if(jot < 1 || jot > 64)
                        m_dm.msg1(CG_E_BAD_RANGE, &tmpedp, m_dm.jot1()<<jot);
                }
            }
            else 
                assert(!"wrong EVENT property type");//must be already checked by front end 

            if(getTarget() && getTarget()->getTrgVer() < 21000)
            {
				/* Hack for Osai EVENTS in version 20900 */
				if( !(getTarget()->getTrgVer()>=20900 && !_tcsicmp(getTarget()->getTrgType(), _T("4cosai"))))
				{
					m_dm.errNS1(&pInit->edp, m_dm.res1(VM_RES_STR_NS_FEATURE_VER_MIN).
                       fmtStr3(NULL, _T("EVENT"), m_dm.jot1()<<getTarget()->getTrgVer(), "21000") );
				}
            }

        }
        else if(!_tcsicmp(pInit->pszAttr, "WATCHDOG"))
        {
            assert(pInit->val.extyp == CG_LITERAL);
            if(getTarget())
            {
                if(getTarget()->getTrgVer() < 21001)
                {
                    m_dm.errNS1(&pInit->edp, m_dm.res1(VM_RES_STR_NS_FEATURE_VER_MIN).
                           fmtStr3(NULL, _T("WATCHDOG"), m_dm.jot1()<<getTarget()->getTrgVer(), "21001") );
                }
                else if(getTarget()->getTrgVer() < 21050)
                {
                    if(pType->type != CG_TIME)
                    {
                        m_dm.msg3(CG_E_BAD_ATTR_DATATYPE, &pInit->edp, 
                                  _T("WATCHDOG"), pType->pszName, _T("TIME") );
                    }
                }
                else //version >= 21050 must be any int
                {
                    if((pType->tbs & (CG_TBS_ANY_INT))==0)
                    {
                        m_dm.msg3(CG_E_BAD_ATTR_DATATYPE, &pInit->edp, 
                                  _T("WATCHDOG"), pType->pszName, _T("ANY_INT") );
                    }
                }

                //check range: must be positive integer:                
                if((pType->tbs & (CG_TBS_TIME|CG_TBS_ANY_INT)) && (pLit->i64 < 0 || pLit->i64 > LONG_MAX))
                {
                    m_dm.msg1(CG_E_BAD_RANGE, &pInit->val.edp,pLit->psz);
                }

            }
        }
        else
            assert(!"invalid task property");//must be already checked by front end
    }
}

const TCHAR* CGlobCtx::getTrgDBIPath()const
{
    assert(m_targets.getUsed() <= 1);
    if (m_targets.getUsed() == 1)
    {
        return m_targets.get(0)->getTrgName();
    }
    
    return FC_StringGetFilePart(m_TrgTypeGenPath);
}

void CGlobCtx::dumpTargetDBIFile(
    CTarget*            pTrg,
    const CG_Resource*  pRes
)
{

    const CG_OPTIONS* p_opt = m_kad.getCgOptions();
 
    CGT_DumpTRGDBIFile2(
        &m_dm,
        m_pIF,
        pRes,
        pTrg->getTrgName(),
        m_popt->szCurTrgTyp,
        pTrg->getControl(),
        getTrgDBIPath(),
        false,
        m_pAutoExternals,
        p_opt->clsidAutoDecl.Data1 != 0 ? &p_opt->clsidAutoDecl: NULL,
        p_opt->szAutoDeclIniFile[0] ? p_opt->szAutoDeclIniFile : NULL
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
        m_pIF,     //front end IF
        FC_StringGetFileExt(pTrg->getTrgFile()),
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
    FC_CString         jot(MAX_PATH);

    for(j=0; (pSym = pzpRefSym[j])!=NULL; j++)
    {
        CCodeInfo* pInfo = m_POUcode.get(pSym);
        if(!pInfo)
            continue;

        assert(pInfo->m_pSym==pSym);
        assert(!pInfo->m_bErrors);
    }

    pTrg->dumpLoaderList(this);
}

void CGlobCtx::calcVarExtAccessInfo(CTarget* pTrg)
{
    unsigned long      ulSize;
    const CChannelBase*pChBase;
    const CG_Resource* pRes;
        
        
    pRes = pTrg->getCGRes();
    if (pRes->nTasks == 0 || getLicData()->bDemo)
        return; //if demo all %-addr are ignored, therefore also no region info!

    // init the memory maps
    m_pAddrMaps = new ADDR_MAPS[pRes->nTasks];
    m_lResTasks = pRes->nTasks;
    assert(m_maxChanelArraySizeAllocated==0);
    m_maxChanelArraySizeAllocated = pTrg->getChannelArraySize();

    for (int iTask=0; iTask<pRes->nTasks; iTask++)
    {
        if(m_maxChanelArraySizeAllocated>0)
        {
            m_pAddrMaps[iTask].pGIMemWR = new MEM_ARRAY[m_maxChanelArraySizeAllocated];
            m_pAddrMaps[iTask].pGIMemRD = new MEM_ARRAY[m_maxChanelArraySizeAllocated];
            m_pAddrMaps[iTask].pGOMemWR = new MEM_ARRAY[m_maxChanelArraySizeAllocated];
            m_pAddrMaps[iTask].pGOMemRD = new MEM_ARRAY[m_maxChanelArraySizeAllocated];
            
            memset(m_pAddrMaps[iTask].pGIMemWR, 0, sizeof(MEM_ARRAY) * m_maxChanelArraySizeAllocated);
            memset(m_pAddrMaps[iTask].pGIMemRD, 0, sizeof(MEM_ARRAY) * m_maxChanelArraySizeAllocated);
            memset(m_pAddrMaps[iTask].pGOMemWR, 0, sizeof(MEM_ARRAY) * m_maxChanelArraySizeAllocated);
            memset(m_pAddrMaps[iTask].pGOMemRD, 0, sizeof(MEM_ARRAY) * m_maxChanelArraySizeAllocated);
        }//else: no channels in KAD and or no <FBUS> instantiated...
        
        // store address maps against pTask
        m_TaskAddrMaps.insert(&pRes->pTasks[iTask], &m_pAddrMaps[iTask]);

        for (int iChn=0; iChn<m_maxChanelArraySizeAllocated; iChn++)
        {
            if ((pChBase = pTrg->getIOCompByChannel(iChn)) == NULL)
                continue;//this is a 'gap' in the used channel numbers (depends on project!)

            // INPUT SEGMENT
            ulSize = pChBase->m_ulIChnSize;//if ulSize==0 => the channel does not use %I
            if (ulSize!=0 && (m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_INPUT))
            {
                m_pAddrMaps[iTask].pGIMemWR[iChn].pAdr = new BYTE[ulSize];
                m_pAddrMaps[iTask].pGIMemRD[iChn].pAdr = new BYTE[ulSize];
                m_pAddrMaps[iTask].pGIMemWR[iChn].ulSize = ulSize;
                m_pAddrMaps[iTask].pGIMemRD[iChn].ulSize = ulSize;
                memset(m_pAddrMaps[iTask].pGIMemWR[iChn].pAdr, 0, ulSize);
                memset(m_pAddrMaps[iTask].pGIMemRD[iChn].pAdr, 0, ulSize);
            }


            // OUTPUT SEGMENT
            ulSize = pChBase->m_ulOChnSize;//if ulSize==0 => the channel does not use %Q
            if (ulSize!=0 && (m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_OUTPUT))
            {
                m_pAddrMaps[iTask].pGOMemWR[iChn].pAdr = new BYTE[ulSize];
                m_pAddrMaps[iTask].pGOMemRD[iChn].pAdr = new BYTE[ulSize];
                m_pAddrMaps[iTask].pGOMemWR[iChn].ulSize = ulSize;
                m_pAddrMaps[iTask].pGOMemRD[iChn].ulSize = ulSize;
                memset(m_pAddrMaps[iTask].pGOMemWR[iChn].pAdr, 0, ulSize);
                memset(m_pAddrMaps[iTask].pGOMemRD[iChn].pAdr, 0, ulSize);
            }
        }
    }


    for(int j=0; j<pRes->nTasks; j++)
    {
        CGT_VarExtAccessInfo* pInfo;
        const CG_Task* pTask =  &pRes->pTasks[j];
        ADDR_MAPS* pAdrMap = m_TaskAddrMaps.get(pTask);
        assert(pAdrMap);

        pInfo = CGT_GetVarExtAccessInfo(&m_dm, m_pIF, pRes, pTask);
        if (pInfo)
        {
            CGT_AccInfo acc;
            
            CGT_InitVarExtAccInfoList(pInfo);
            while(CGT_GetNextVarExtAccInfo(pInfo, &acc))
            {
                if(!acc.pEntry->pszAddr)
                    continue;
                if(acc.pEntry->pszAddr[1]!='I' && acc.pEntry->pszAddr[1]!='Q')
                    continue;

                InsertIntoUsedMap(pAdrMap, &acc);
                /*
                _tprintf(_T("%s = %s%s\n"), 
                acc.pEntry->pszId, 
                acc.bRead   ? "R" : "",
                acc.bWritten ? "W" : ""
                );
                */
            }
            
            CGT_FreeVarExtAccessInfo(pInfo);
        }
    }


    for(j=0; j<pRes->nTasks; j++)
    {
        const CG_Task* pTask =  &pRes->pTasks[j];
        ADDR_MAPS* pAdrMap   = m_TaskAddrMaps.get(pTask);

        MEM_RGNS* pMemRegions = new MEM_RGNS;
        m_TaskAllMemRgns.insert(pTask, pMemRegions);

        //TODO: HACK devide the m_kad.getMaxMemRgnsRD()[VM_SEGM_IN/OUTPUT] number
        //of max regions by the actual used channels. Problem is that the 
        //calcMemRegions algorithm is called for every used channel(!) and not 
        //for the complete segment. REWORK NEEDED!
        int nUsedChannels = 0;
        for (int i=0; i<m_maxChanelArraySizeAllocated; i++)
            if (pTrg->getIOCompByChannel(i))
                nUsedChannels++;

        //printf("########### %s ###########\n", pTask->pszId);
        for (i=0; i<m_maxChanelArraySizeAllocated; i++)
        {
            if ((pChBase = pTrg->getIOCompByChannel(i)) == NULL)
                continue;//this is a 'gap' in the used channel numbers (depends on project!)
            
            // INPUT
            // no WRITE operations for INPUT
            if (   m_kad.getUseTIforSegm() & VM_BS_SEGM_INPUT 
                && m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_INPUT)
            {
                //printf("INPUT READ: ");
                //HACK: at least an error msg if devision yields 0:
                int nMaxIRead = m_kad.getMaxMemRgnsRD()[VM_SEGM_INPUT] / nUsedChannels;
                if(nMaxIRead==0)
                    m_dm.errNS1(NULL, _T("too less copy regions for input reads"));

                calcMemRegions(pAdrMap->pGIMemRD[i].pAdr, 
                    pAdrMap->pGIMemRD[i].ulSize,
                    pChBase->m_ulISegOffs, // 0: if there is no channel info ??
                    &pMemRegions->GIMemRgnsRD, 
                    nMaxIRead);
            }

           // OUTPUT
            if (   m_kad.getUseTIforSegm()        & VM_BS_SEGM_OUTPUT
                && m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_OUTPUT)
            {
                //printf("OUTPUT WRITE: ");
                //HACK: at least an error msg if devision yields 0:
                int nMaxQWrite= m_kad.getMaxMemRgnsWR()[VM_SEGM_OUTPUT] / nUsedChannels;
                if(nMaxQWrite==0)
                    m_dm.errNS1(NULL, _T("too less copy regions for output writes"));

                calcMemRegions(pAdrMap->pGOMemWR[i].pAdr, 
                    pAdrMap->pGOMemWR[i].ulSize, 
                    pChBase->m_ulOSegOffs, // 0: if there is no channel info
                    &pMemRegions->GOMemRgnsWR, 
                    nMaxQWrite);
            }
            if (   m_kad.getUseTIforSegm()        & VM_BS_SEGM_OUTPUT
                && m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_OUTPUT)
            {
                //HACK: at least an error msg if devision yields 0:
                int nMaxQRead = m_kad.getMaxMemRgnsRD()[VM_SEGM_OUTPUT] / nUsedChannels;
                if(nMaxQRead==0)
                    m_dm.errNS1(NULL, _T("too less copy regions for output reads"));

                //printf("OUTPUT READ: ");
                calcMemRegions(pAdrMap->pGOMemRD[i].pAdr, 
                    pAdrMap->pGOMemRD[i].ulSize, 
                    pChBase->m_ulOSegOffs,
                    &pMemRegions->GOMemRgnsRD, 
                    nMaxQRead);
            }
        }
    }
}

void CGlobCtx::InsertIntoUsedMap(ADDR_MAPS*    pAdrMap, 
                                 CGT_AccInfo*  pAccInfo)
{
    BYTE*               pMemWR;
    BYTE*               pMemRD;
    VM_ADR_OFF          ulSegOff;
    VM_SEGM_TYP         segm;
    long                Chn;
    const VM_Address*   pVMAddr;

    pVMAddr = m_DirectAddrEntries.get(pAccInfo->pEntry);
    if (pVMAddr == NULL)
    {
        // HACK: todo: implement it right making a string hash having as key the name of autodecl variables 
        //       global for all tasks; now they are matched per task and this hack fix 
        if(pAccInfo->pEntry->declScope == CG_VAR_EXTERNAL && pAccInfo->pEntry->declMod & CG_DMOD_SYS_VAR)
        {
            for(const FC_CHashPtr<const CG_SymEntry*, VM_Address*>::Pair* p = m_DirectAddrEntries.firstPair(); p; p = p->pNext)
            {
                if(!lstrcmpi(p->pKey->pszId, pAccInfo->pEntry->pszId))
                    break;
            }
            if(p)
            {
                pVMAddr = p->pVal;
            }
            else
            {
                CGT_IERR(&m_dm, "autodeclared variable declaration error");
                return;
            }

        }
        else
        {
            CGT_IERR(&m_dm, "memory regions for global/retain not implemented");
            return;
        }
    }

    segm     = pVMAddr->vmSegEnum;
    Chn      = pVMAddr->lChannel;
    ulSegOff = pVMAddr->ulByteOffs;
    
    int iByteSize  = pVMAddr->bitSize == 1 ? pVMAddr->bitSize : pVMAddr->bitSize / 8;

    switch(segm)
    {
    case VM_SEGM_INPUT:
        if ((m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_INPUT) == 0)
            return;//no optimisation for INPUT, pAdrMap->pGIMem[WR|RD][Chn].pAdr was not allocated!

        assert(Chn>=0 && Chn<m_maxChanelArraySizeAllocated);
        pMemWR = &pAdrMap->pGIMemWR[Chn].pAdr[ulSegOff - pVMAddr->ulChOff];
        pMemRD = &pAdrMap->pGIMemRD[Chn].pAdr[ulSegOff - pVMAddr->ulChOff];
        
        assert(ulSegOff - pVMAddr->ulChOff + iByteSize >= 0);
        assert(ulSegOff - pVMAddr->ulChOff + iByteSize <= pAdrMap->pGIMemRD[Chn].ulSize);
    break;
    case VM_SEGM_OUTPUT:
        if ((m_kad.getOptimizeTIforSegmBS() & VM_BS_SEGM_OUTPUT) == 0)
            return;//no optimisation for OUTPUT, pAdrMap->pGOMem[WR|RD][Chn].pAdr was not allocated!
        
        assert(Chn>=0 && Chn<m_maxChanelArraySizeAllocated);
        pMemWR = &pAdrMap->pGOMemWR[Chn].pAdr[ulSegOff - pVMAddr->ulChOff];
        pMemRD = &pAdrMap->pGOMemRD[Chn].pAdr[ulSegOff - pVMAddr->ulChOff];
        
        assert(ulSegOff - pVMAddr->ulChOff + iByteSize >= 0);
        assert(ulSegOff - pVMAddr->ulChOff + iByteSize <= pAdrMap->pGOMemRD[Chn].ulSize);
    break;
    default:
        assert( ! "invalid segment");
    }

    if (pAccInfo->bWritten != 0)
        for (int i=0; i<iByteSize; i++)
        {
            pMemWR[i] = 1; //TODO: check array range here to be safe.
            CHECK_MEMORY;  //      since the KAD I/OSEG_SIZE are checked 
        }                  //      and the %addr is also checked there should no 
                           //      overflow happen, but you never know...
        
    if (pAccInfo->bRead != 0)
        for (int i=0; i<iByteSize; i++)
        {
            pMemRD[i] = 1;
            CHECK_MEMORY;
        }
}


// license 
const int VM_STARTING_KIT_MAX_IO  = 6;


static int __cdecl CompareVMAddr(const void *elem1, const void *elem2 )
{
    const VM_Address* pA1 = *(const VM_Address**)elem1;
    const VM_Address* pA2 = *(const VM_Address**)elem2;

    return pA1->ulByteOffs*8+pA1->ucBitPos  > pA2->ulByteOffs*8+pA2->ucBitPos;
}


#define CUTOFF 10000000  //HACK: CUTOFF was 8 in MS Sourcecode, but QSORT does not work!!!! , therefore always use shortsort!!!

static void __cdecl swap (
    char *a,
    char *b,
    unsigned width
    )
{
    char tmp;

    if ( a != b )
        /* Do the swap one character at a time to avoid potential alignment
           problems. */
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}
static void __cdecl shortsort (
    char *lo,
    char *hi,
    unsigned width,
    int (__cdecl *comp)(const void *, const void *)
    )
{
    char *p, *max;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */

    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            /* A[i] <= A[max] for lo <= i < p */
            if (comp(p, max) > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */

        swap(max, hi, width);

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        hi -= width;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
}
void __cdecl myqsort (
    void *base,
    unsigned num,
    unsigned width,
    int (__cdecl *comp)(const void *, const void *)
    )
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    unsigned size;              /* size of the sub-array */
    char *lostk[30], *histk[30];
    int stkptr;                 /* stack for saving sub-array to be processed */

    /* Note: the number of stack entries required is no more than
       1 + log2(size), so 30 is sufficient for any array */

    if (num < 2 || width == 0)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char*)base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       prserved, locals aren't, so we preserve stuff on the stack */
recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    /* below a certain size, it is faster to use a O(n^2) sorting method */
    if (size <= CUTOFF) {
         shortsort(lo, hi, width, comp);
    }
    else {
        /* First we pick a partititioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the
           median of the values, but also that we select one fast.  Using
           the first one produces bad performace if the array is already
           sorted, so we use the middle one, which would require a very
           wierdly arranged array for worst case performance.  Testing shows
           that a median-of-three algorithm does not, in general, increase
           performance. */

        mid = lo + (size / 2) * width;      /* find middle element */
        swap(mid, lo, width);               /* swap it to beginning of array */

        /* We now wish to partition the array into three pieces, one
           consisiting of elements <= partition element, one of elements
           equal to the parition element, and one of element >= to it.  This
           is done below; comments indicate conditions established at every
           step. */

        loguy = lo;
        higuy = hi + width;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi + 1,
               A[i] <= A[lo] for lo <= i <= loguy,
               A[i] >= A[lo] for higuy <= i <= hi */

            do  {
                loguy += width;
            } while (loguy <= hi && comp(loguy, lo) <= 0);

            /* lo < loguy <= hi+1, A[i] <= A[lo] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[lo] */

            do  {
                higuy -= width;
            } while (higuy > lo && comp(higuy, lo) >= 0);

            /* lo-1 <= higuy <= hi, A[i] >= A[lo] for higuy < i <= hi,
               either higuy <= lo or A[higuy] < A[lo] */

            if (higuy < loguy)
                break;

            /* if loguy > hi or higuy <= lo, then we would have exited, so
               A[loguy] > A[lo], A[higuy] < A[lo],
               loguy < hi, highy > lo */

            swap(loguy, higuy, width);

            /* A[loguy] < A[lo], A[higuy] > A[lo]; so condition at top
               of loop is re-established */
        }

        /*     A[i] >= A[lo] for higuy < i <= hi,
               A[i] <= A[lo] for lo <= i < loguy,
               higuy < loguy, lo <= higuy <= hi
           implying:
               A[i] >= A[lo] for loguy <= i <= hi,
               A[i] <= A[lo] for lo <= i <= higuy,
               A[i] = A[lo] for higuy < i < loguy */

        swap(lo, higuy, width);     /* put partition element in place */

        /* OK, now we have the following:
              A[i] >= A[higuy] for loguy <= i <= hi,
              A[i] <= A[higuy] for lo <= i < higuy
              A[i] = A[lo] for higuy <= i < loguy    */

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy-1] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of length 2 or more.*/

        if ( higuy - 1 - lo >= hi - loguy ) {
            if (lo + width < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy - width;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo + width < higuy) {
                hi = higuy - width;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}


static void TestOverlappingAddr(CGT_CDiagMsg* pdm, FC_CGArray<const VM_Address*>* pList)
{
    int                i;
    int                n     = pList->getUsed();
    const VM_Address** ppAdr = pList->getElements();

//    for(i=0; i<n; i++)
//        printf("vorher: %3d '%32s' %d.%d\n", i, ppAdr[i]->m_pEntry->pszAddr, ppAdr[i]->ulByteOffs, ppAdr[i]->ucBitPos);


    myqsort(ppAdr, n, sizeof(VM_Address*), CompareVMAddr);

//    for(i=0; i<n; i++)
//        printf("nacher: %3d '%32s' %d.%d\n", i, ppAdr[i]->m_pEntry->pszAddr, ppAdr[i]->ulByteOffs, ppAdr[i]->ucBitPos);

    for(i=0; i<n-1; i++)
    {
        if(ppAdr[i]->ulByteOffs*8 + ppAdr[i]->ucBitPos + ppAdr[i]->bitSize
             >
           ppAdr[i+1]->ulByteOffs*8 + ppAdr[i+1]->ucBitPos
          )
        {
            pdm->msg2(CG_W_INVALID_ADDR, 
                &ppAdr[i]->m_pEntry->edpAddr, 
                ppAdr[i]->m_pEntry->pszAddr, 
			    pdm->res1(VM_RES_STR_OVERLAPSWITH)<<"'"<<
                ppAdr[i+1]->m_pEntry->pszId<< " AT " <<
                ppAdr[i+1]->m_pEntry->pszAddr<<"'"
            );
        }
        
    }
}

void CGlobCtx::calcDirectAddresses(CTarget* ptrg)
{
    const CG_Resource*  pRes;
    const CG_Symtab*    pGVL;
    const CG_SymEntry*  pEntry;
    VM_Address*         pVMAddr;

    pRes = ptrg->getCGRes();
    pGVL = pRes->pGVL;

    // first extract vars with user address
    for (pEntry=pGVL->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if (pEntry->pszAddr != NULL)
        {
            pVMAddr = new VM_Address(pEntry);
            VMCalcDirectAddress(this, ptrg, pEntry, pVMAddr);
            m_DirectAddrEntries.insert(pEntry, pVMAddr);
        }
    }
    for (int i=0; i<pGVL->nGVLImp; i++)
    {
        const CG_Symtab* pGVLImp = pGVL->pGVLImp[i].pSym;
        for (pEntry=pGVLImp->pFirst; pEntry; pEntry=pEntry->pNext)
        {
            if (pEntry->pszAddr != NULL)
            {
                pVMAddr = new VM_Address(pEntry);
                VMCalcDirectAddress(this, ptrg, pEntry, pVMAddr);
                m_DirectAddrEntries.insert(pEntry, pVMAddr);
            }
        }
    }
    for (i=0, pEntry = m_pAutoExternals[0]; pEntry; i++, pEntry = m_pAutoExternals[i])
    {
        if (pEntry->pszAddr != NULL)
        {
            pVMAddr = new VM_Address(pEntry);
            VMCalcDirectAddress(this, ptrg, pEntry, pVMAddr);
            m_DirectAddrEntries.insert(pEntry, pVMAddr);
        }
    }

    if(m_kad.getCheckAddressSpace())
    {
        FC_CGArray<const VM_Address*> ins(1024);
        FC_CGArray<const VM_Address*> outs(1024);
        FC_CGArray<const VM_Address*> mems(1024);
        const FC_CHashPtr<const CG_SymEntry*, VM_Address*>::Pair* pPair;


        for(pPair = m_DirectAddrEntries.firstPair(); pPair; pPair = pPair->pNext)
        {
            VM_Address* pVMAddr = pPair->pVal;
            if(pVMAddr->vmSegEnum == VM_SEGM_INPUT)
                ins.add(pVMAddr);
            else if(pVMAddr->vmSegEnum == VM_SEGM_OUTPUT)
                outs.add(pVMAddr);
            else if(pVMAddr->vmSegEnum == VM_SEGM_GLOBAL)
                mems.add(pVMAddr);
        }

        TestOverlappingAddr(&m_dm, &ins);
        TestOverlappingAddr(&m_dm, &outs);
        TestOverlappingAddr(&m_dm, &mems);
    }
}


#define SMALL_SPACE 8
static bool CalcMemRegions2(const BYTE*                pMem,
                            int                        iSize,
                            unsigned long              ulOffset,
                            FC_CGArray<VM_MemRegion*>* pMemRgns,
                            int                        iMaxMemRgnsPerSegm)
{
    int                         i;
    VM_MemRegion*               pRgn;
    VM_MemRegion*               pRgn2;
    FC_CGArray<VM_MemRegion*>   maxRgn(1024);
    int                         nRgn;
    int                         nUsedRegions;
    unsigned long               ul;

    //first assume an infinite number of regions:
    for(i=0;i<iSize; i++)
    {
        if(pMem[i])
        {
            VM_MemRegion rgn = {i + ulOffset, 0};
            for(;pMem[i] && i<iSize; i++, rgn.ulSize++)
                ;
            *(pRgn = new VM_MemRegion) = rgn;
            maxRgn.add(pRgn);
        }
    }

    
    //now join all regions that are at max 8 bytes apart:
    nRgn = maxRgn.getUsed();
    nUsedRegions = nRgn;
    for(i=0;i<nRgn-1;i++)
    {
        pRgn  = maxRgn.get(i);
        pRgn2 = maxRgn.get(i+1);

        if(pRgn2->ulOffset - (pRgn->ulOffset + pRgn->ulSize) < SMALL_SPACE)
        {
            //join: means 'pRgn2 swallows pRgn' and pRgn gets removed:
            ul = pRgn2->ulOffset;
            pRgn2->ulOffset = pRgn->ulOffset;
            pRgn2->ulSize = (ul + pRgn2->ulSize - pRgn->ulOffset);
            delete(pRgn);
            maxRgn.set(i, NULL);
            nUsedRegions--; //update the acual used regions counter
        }
    }


    if(nUsedRegions > iMaxMemRgnsPerSegm)
    {
        maxRgn.deletePtrs(); //<-delete any "try simple" solution from above.
        return false; //TODO more sophisticated algorithm if now more than iMaxMemRgnsPerSegm:
                      //return false means ues the old method...
    }

    //put result into return value pMemRgns:
    for(i=0;i<nRgn;i++)
    {
        if((pRgn  = maxRgn.get(i))!=NULL)
            pMemRgns->add(pRgn);
    }
   
    return true;
}

/* ---------------------------------------------------------------------------- */
/**
 *	CalcMemRegions3
 */
static bool CalcMemRegions3(const BYTE*                pMem,
                            int                        iSize,
                            unsigned long              ulOffset,
                            FC_CGArray<VM_MemRegion*>* pMemRgns,
                            int                        iMaxMemRgnsPerTask)
{
    int                         i;
    VM_MemRegion*               pRgn;
    VM_MemRegion*               pRgn2;
    FC_CGArray<VM_MemRegion*>   maxRgn(1024);
    int                         nRgn;
    int                         nUsedRegions;
    unsigned long               ul;
    FC_CHashPtr<long, long>     gaps;
    long                        minGapSize = 0;
    long                        gapNo;

    //first assume an infinite number of regions:
    for (i = 0; i < iSize; i++)
    {
        if (pMem[i])
        {
            VM_MemRegion rgn = {i + ulOffset, 0};
            for ( ; pMem[i] && i < iSize; i++, rgn.ulSize++)
            {
                ;
            }
            *(pRgn = new VM_MemRegion) = rgn;
            maxRgn.add(pRgn);
        }
    }

    // now detect all gaps 
    nRgn = maxRgn.getUsed();
    for (i = 0; i< nRgn - 1; i++)
    {
        pRgn    = maxRgn.get(i);
        pRgn2   = maxRgn.get(i + 1);

        int gapSize = pRgn2->ulOffset - (pRgn->ulOffset + pRgn->ulSize);
        
        if ( gaps.get(gapSize, &gapNo) )
        {
            gapNo++;
        }
        else
        {
            gapNo = 1;
        }
        gaps.insert(gapSize, gapNo);
    }

    nUsedRegions = nRgn;
    while (nUsedRegions > iMaxMemRgnsPerTask)
    {
        if (gaps.get(minGapSize, &gapNo))
        {
            nUsedRegions -= gapNo;
        }
        minGapSize++;
    }

    if(minGapSize > 0 && gaps.get(minGapSize-1, &gapNo))
        gapNo -= iMaxMemRgnsPerTask - nUsedRegions;
    else
        gapNo = 0;

    //gaps.deletePtrs();
    minGapSize = max(minGapSize, SMALL_SPACE);
   
    //join all regions that are at max "minGapSize" bytes apart:
    nUsedRegions = nRgn;
    for (i = 0; i < nRgn - 1; i++)
    {
        pRgn  = maxRgn.get(i);
        pRgn2 = maxRgn.get(i + 1);

        int gapSize = pRgn2->ulOffset - (pRgn->ulOffset + pRgn->ulSize);

        if (gapSize < minGapSize)
        {
            // from the last set of gaps to be removed, 
            // remove just the enough to be less than iMaxMemRgnsPerTask
            if (gapSize == minGapSize - 1 && gapNo-- <= 0)
                continue;

            //join: means 'pRgn2 swallows pRgn' and pRgn gets removed:
            ul = pRgn2->ulOffset;
            pRgn2->ulOffset = pRgn->ulOffset;
            pRgn2->ulSize = (ul + pRgn2->ulSize - pRgn->ulOffset);
            delete(pRgn);
            maxRgn.set(i, NULL);
            nUsedRegions--; //update the acual used regions counter
        }
    }

    //put result into return value pMemRgns:
    for (i = 0; i < nRgn; i++)
    {
        if((pRgn  = maxRgn.get(i)) != NULL)
        {
            pMemRgns->add(pRgn);
        }
    }
   
    return true;
}


/* ---------------------------------------------------------------------------- */
/**
 *	calcMemRegions
 */
void CGlobCtx::calcMemRegions(const BYTE*                pMem,
                              int                        iSize,
                              unsigned long              ulOffset,
                              FC_CGArray<VM_MemRegion*>* pMemRgns,
                              int                        iMaxMemRgnsPerTask)
{
    int iStartPos = -1;
    int iCopyLen = 0;
    int iNullLen = 0;
    int iVars = 0;
    int iRgns = 0;
    int iMaxCpy = 0;
    int iMax1 = 0;
    int i;
    
    if (iSize == 0 || pMem == NULL || iMaxMemRgnsPerTask == 0)
    {
        /*
        printf ("\n");
        */
        return; // nothing to calculate
    }

    if(getTarget() && getTarget()->getTrgVer() >= 21040)
    {
        if (CalcMemRegions3(pMem, iSize, ulOffset, pMemRgns, iMaxMemRgnsPerTask))
            return;
        //else: old solution: 
    }
    else
    {
	    if (CalcMemRegions2(pMem, iSize, ulOffset, pMemRgns, iMaxMemRgnsPerTask))
            return;
        //else: old solution: 
    }
    
    //general algorithm:    
    int min0 = iSize / iMaxMemRgnsPerTask;

    for (i=0; i<iSize; i++)
    {
        if (pMem[i] != 0)
        {
            iVars++;
            iMax1++;
            iCopyLen++;
            iNullLen = 0;
            if (iStartPos == -1)
                iStartPos = i;
        }
        else
        {
            if (iCopyLen == 0)
                continue;
            
            iCopyLen++;
            iNullLen++;
        }

        if (iNullLen > min0 || i == iSize-1)
        {
            // region found
            iRgns++;
            assert(i-iStartPos-(iNullLen-1) == iCopyLen-iNullLen);

            VM_MemRegion* pMemRgn = new VM_MemRegion;
            pMemRgn->ulOffset = iStartPos + ulOffset;   // ulOffset is the channel offset in the segment
            pMemRgn->ulSize   = iCopyLen-iNullLen;
            pMemRgns->add(pMemRgn);

            /*
            _tprintf ("Start: %05ld  len: %03ld  Vars: %ld\n", iStartPos, iCopyLen-iNullLen, iVars);
            */
            iMaxCpy += (iCopyLen-iNullLen);
            iNullLen = 0;
            iCopyLen = 0;
            iStartPos = -1;
            iVars = 0;
        }
    }

    /*
    if (iRgns != 0)
    {
        _tprintf ("min_0: %-3ld  Regions: %-4ld  cpy/Rgn: %-5ld  0_Bytes: %-3ld%%  Mem/Vars: %-3ld%%\n", 
        min0, iRgns, iMaxCpy / iRgns, iMaxCpy  * 100 / iMax1 - 100, iSize * 100 / iMax1 - 100);
    }
    else
    {
        printf ("\n");
    }
    */
}



void CGlobCtx::buildAsmFile(CTarget* pTrg)
{
    int                i;
    CGT_CDmp           dmp;
    const CG_Symtab**  pzpRefSym;
    const CG_Symtab*   pSym; 
    const CG_Resource* pRes;
    FC_CString         fileName(MAX_PATH);

    pRes = pTrg->getCGRes();
    if(!pRes || !pTrg->hasControl())
    {
        assert(m_dm.getDisplayedWarnings()>0);
        return;
    }


    dmp << get4CCopyRightSourceHeader(m_popt->szCurTrgTyp);

    //dump the .target section
    dumpTargetInfo(dmp, pTrg);
    
    // dump TaskInfoStruct and system globals
    dmp << _T("; struct __taskinfo is used for reporting task states\n");
    dmp << _T(".struct __taskinfo\n");
    dmp.incrIndent();
        dmp << _T("var    dword __state\n");
        dmp << _T("var    dword __prio\n");
        dmp << _T("var    dword __cycle\n");
        dmp << _T("var    dword __etmax\n");
        dmp << _T("var    dword __errno\n");
    dmp.decrIndent();
    dmp<<_T(".end\n\n");
    
    dmp<<_T(".global\n");
    dmp.incrIndent();
        dmp<<_T("dword       __state\n");//resource state
        for(i=0; i<pRes->nTasks; i++)
            (dmp<<_T("__taskinfo    ")).lo(pRes->pTasks[i].pszId)<<'\n';

        //.globals section for system variables
        for(i=0; m_pAutoExternals[i]; i++)
        {
            if(i==0)
                dmp<<_T("\n;globals from autodecl:\n");
            CG_VM_DumpOneVariable(dmp, this, pTrg, m_pAutoExternals[i], true);
        }

    dmp.decrIndent();
    dmp<<_T(".end\n\n");

    fileName.load(pTrg->getTrgGenPath());
    fileName.appendDir(pTrg->getTrgName()) << CG_VM_ASM_TEXT_EXT;
    assert( ! FC_FileExists(NULL, fileName));
    CGT_DmpToFile(&m_dm, NULL, fileName, dmp, false);


    //append now other parts of the 4asm file:
    pzpRefSym = CGT_EnumRefSymtabs(&m_dm, m_pIF, pRes);
    // first dump all STRUCTs
    for(i=0; (pSym = pzpRefSym[i])!=NULL; i++)
    {
        CCodeInfo* pInfo = m_POUcode.get(pSym);
        if(!pInfo || pSym->pouTyp != CG_POU_SDT)
            continue;
        assert(pInfo->m_pSym==pSym && !pInfo->m_bErrors && pInfo->m_pDmp);

        CGT_DmpToFile(&m_dm, NULL, fileName, *pInfo->m_pDmp, true);

        // free dumper
        delete pInfo->m_pDmp;
        pInfo->m_pDmp = NULL;
    }

    // dump all global variables
    dumpGlobalSimpleVars(pRes->pGVL, pTrg, fileName);

    // dump all boddies
    for(i=0; (pSym = pzpRefSym[i])!=NULL; i++)
    {
        CCodeInfo* pInfo = m_POUcode.get(pSym);
        if( ! pInfo || pSym->pouTyp == CG_POU_SDT)
        {
            if (pSym->pouTyp == CG_POU_SDT)
            {
                // dump all global vars of this type
                dumpGlobalVarsOfType(pRes->pGVL, pSym, pTrg, fileName);
            }
 
            continue;
        }

        assert(pInfo->m_pSym==pSym && !pInfo->m_bErrors);

        if(pInfo->m_pDmp == NULL)
        {
            assert((pSym->flags & CG_STAB_HAS_BDY) == 0);
            continue;
        }
        
        CGT_DmpToFile(&m_dm, NULL, fileName, *pInfo->m_pDmp, true);

        // free dumper
        delete pInfo->m_pDmp;
        pInfo->m_pDmp = NULL;

        // dump all global vars of this type
        if(pSym->pouTyp != CG_POU_FUN)
            dumpGlobalVarsOfType(pRes->pGVL, pSym, pTrg, fileName);
    }
    delete(pzpRefSym);

    // dump resource section
    dumpResource(pRes, fileName);
    // dump all task proginst assignments
    dumpTasks(pRes, fileName);
    // dump instance inits and VAR_CONFIG stuff (future)
    dumpConfigSect(pRes, fileName);
    // dump channel info (since Version V20213)
    pTrg->dumpChannelInfo(&m_dm, fileName);



    m_AsmFilesCreated++;
}

const TCHAR* CGlobCtx::get4CCopyRightSourceHeader(LPCTSTR trg)
{
    m_4C_Version.clear();
    m_4C_Version << _T("; ") << _T(PROD_PRODUCTNAME) _T(" ") << trg << _T("-")
                 << _T(PROD_FULL_VERSION_STR) _T("\n; ") _T(PROD_COPYRIGHT) _T("\n\n\n");

    return m_4C_Version;
}

void CGlobCtx::dumpTargetInfo(CGT_CDmp& dmp, const CTarget* pTrg)
{
    unsigned long ulVal;
    TCHAR*        psz;

    // target section:
    dmp<<_T(".target\n");
    dmp.incrIndent();

    dmp<<CG_VM_ASM_TARGETTYPE _T("'")<< pTrg->getTrgType() << _T("'\n");
    dmp<<CG_VM_ASM_FIRMWAREVERS _T("dword#") <<pTrg->getTrgVer() << '\n';

    assert(m_pIF);
    FC_CString strPIName;
    strPIName.load(m_pIF->FCPgetPrjName());
    //strPIName.load(m_pCmdlOpts.sz4CP);

    dmp<<CG_VM_ASM_PI_VERSION _T("'")<<pTrg->getPIVersion() << _T("'\n");
    dmp<<CG_VM_ASM_PI_NAME _T("'")<<strPIName << _T("'\n");         
    dmp<<CG_VM_ASM_PI_MODIFIED _T("'")<<pTrg->getPIModified() << _T("'\n");      
    dmp<<CG_VM_ASM_PI_CREATED _T("'")<<pTrg->getPICreated() << _T("'\n");       
    dmp<<CG_VM_ASM_PI_OWNER _T("'")<<pTrg->getPIOwner() << _T("'\n");        
    dmp<<CG_VM_ASM_PI_COMMENT1 _T("'")<<pTrg->getPIComment1() << _T("'\n");     
    dmp<<CG_VM_ASM_PI_COMMENT2 _T("'")<<pTrg->getPIComment2() << _T("'\n");      
    dmp<<CG_VM_ASM_PI_COMMENT3 _T("'")<<pTrg->getPIComment3() << _T("'\n");

    dmp<<_T("instAlignBytes   ")<<m_kad.getVmSettings()->instAlignBytes<<'\n';
    dmp<<_T("stackAlignBytes  ")<<m_kad.getVmSettings()->stackAlignBytes<<'\n';
    dmp<<_T("bigEndian        ")<<m_kad.getVmSettings()->bigEndian<<'\n';
    dmp<<_T("minOperandSize   ")<<m_kad.getVmSettings()->opMinSize<<'\n';

    // debug settings
    dmp << CG_VM_ASM_DGBSETTING;
    switch(m_bpopt)
    {
    case BP_OPT0: dmp << _T("0"); break; //no BP at all
    case BP_OPT1: dmp << _T("1"); break; //begin/end POU only
    case BP_OPT2: dmp << _T("2"); break; //whereever possible
    default: assert( ! "wrong bp option");
    }
    dmp<<'\n';

    // start points of IEC globals 
    if ( ! m_kad.getVmSettings()->szGMiecOffs.isEmpty())     // IEC GLOBAL
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szGMiecOffs, &psz, 16);
        dmp << CG_VM_ASM_GMIECOFFS << _T("lint#") << ulVal << '\n';
    }
    else
    {
        dmp << CG_VM_ASM_GMIECOFFS << 0 << '\n';
    }
    if ( ! m_kad.getVmSettings()->szGMiecSize.isEmpty())     // IEC GLOBAL
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szGMiecSize, &psz, 16);
        dmp << CG_VM_ASM_GMIECSIZE << _T("lint#") << ulVal << '\n';
    }

    // MEMORY SEGMENT start points (are optional)
    if ( ! m_kad.getVmSettings()->szISegStart.isEmpty())     // INPUT
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szISegStart, &psz, 16);
        dmp << CG_VM_ASM_ISEGSTART << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szISegSize.isEmpty())     // INPUT
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szISegSize, &psz, 16);
        dmp << CG_VM_ASM_ISEGSIZE << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szOSegStart.isEmpty())     // OUTPUT
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szOSegStart, &psz, 16);
        dmp << CG_VM_ASM_OSEGSTART << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szOSegSize.isEmpty())     // OUTPUT
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szOSegSize, &psz, 16);
        dmp << CG_VM_ASM_OSEGSIZE << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szGMSegStart.isEmpty())    // GLOBAL
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szGMSegStart, &psz, 16);
        dmp << CG_VM_ASM_GMSEGSTART << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szGMSegSize.isEmpty())    // GLOBAL
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szGMSegSize, &psz, 16);
        dmp << CG_VM_ASM_GMSEGSIZE << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szGMRSegStart.isEmpty())     // RETAIN
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szGMRSegStart, &psz, 16);
        dmp << CG_VM_ASM_GMRSEGSTART << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szGMRSegSize.isEmpty())     // RETAIN
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szGMRSegSize, &psz, 16);
        dmp << CG_VM_ASM_GMRSEGSIZE << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szCodeSegStart.isEmpty())     // CODE
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szCodeSegStart, &psz, 16);
        dmp << CG_VM_ASM_CODESEGSTART << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szCodeSegSize.isEmpty())     // CODE
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szCodeSegSize, &psz, 16);
        dmp << CG_VM_ASM_CODESEGSIZE << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szLocDataSegStart.isEmpty())     // LOCAL DATA
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szLocDataSegStart, &psz, 16);
        dmp << CG_VM_ASM_LOCDATASEGSTART << _T("lint#") << ulVal << '\n';
    }
    if ( ! m_kad.getVmSettings()->szLocDataSegSize.isEmpty())     // LOCAL DATA
    {
        ulVal = _tcstoul(m_kad.getVmSettings()->szLocDataSegSize, &psz, 16);
        dmp << CG_VM_ASM_LOCDATASEGSIZE << _T("lint#") << ulVal << '\n';
    }

    dmp<< CG_VM_ASM_MAXOBJECTS << _T("dword#")<<m_kad.getVmSettings()->ulMaxObjects<<'\n';
    dmp<<_T("dumpinfo         ") << _T("false")<<'\n';

    //close target section:
    dmp.decrIndent();
    dmp<<_T(".end\n\n");
}



void CGlobCtx::dumpGlobalSimpleVars(const CG_Symtab* pGVL,
                                    CTarget*         pTrg,
                                    LPCTSTR          fileName)
{
    CGT_CDmp            dmp;
    CGT_CDmp            dmpRet;
    const CG_SymEntry*  pEntry;
    const CG_EdpSymtab* pGVLImp = NULL;

    // globals section simple and array of simple:
    dmpRet.incrIndent();
    
    dmp<<_T(".global\n");
    dmp.incrIndent();

    for (pEntry=pGVL->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(pEntry->pType->pSym)
            m_ComplexGlobals.add(pEntry);
        else if(pEntry->declMod & CG_DMOD_RETAIN)
            CG_VM_DumpOneVariable(dmpRet, this, pTrg, pEntry, false);
        else
            CG_VM_DumpOneVariable(dmp, this, pTrg, pEntry, false);
    }

    for (long i=0; i<pGVL->nGVLImp; i++)
    {
        for (pEntry=pGVL->pGVLImp[i].pSym->pFirst; pEntry; pEntry=pEntry->pNext)
        {
            if(pEntry->pType->pSym)
                m_ComplexGlobals.add(pEntry);
            else if(pEntry->declMod & CG_DMOD_RETAIN)
                CG_VM_DumpOneVariable(dmpRet, this, pTrg, pEntry, false);
            else
                CG_VM_DumpOneVariable(dmp, this, pTrg, pEntry, false);
        }
    }

    //close globals section:
    dmp.decrIndent();
    dmp<<_T(".end\n\n");

    // global RETAIN section for simple type and array of simple:
    if(dmpRet.ftell())
    {
        dmp<<_T(".global_retain\n");
        dmp.append(dmpRet);
        dmp<<_T(".end\n\n");
    }

    CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
}

void CGlobCtx::dumpGlobalVarsOfType(
    const CG_Symtab* pGVL,
    const CG_Symtab* pSym,
    CTarget*         pTrg,
    LPCTSTR          fileName
)
{
    int                i;
    int                iMax;
    CGT_CDmp           dmp;
    FC_CGArray<const CG_SymEntry*> glob(100);
    FC_CGArray<const CG_SymEntry*> globRet(100);
    const CG_SymEntry* pEntry;

    assert(pSym->pouTyp != CG_POU_FUN);

    iMax = m_ComplexGlobals.getUsed();
    for(i=0; i<iMax; i++)
    {
        pEntry = m_ComplexGlobals.get(i);
        assert(pEntry->pType->pSym);
        if(pEntry->pType->pSym != pSym)
            continue;

        if(pEntry->declMod & CG_DMOD_RETAIN)
            globRet.add(pEntry);
        else
            glob.add(pEntry);
    }

    if ((iMax = glob.getUsed())>0)
    {
        dmp<<_T(".global\n");
        dmp.incrIndent();
        for(i=0; i<iMax; i++)
            CG_VM_DumpOneVariable(dmp, this, pTrg, glob.get(i), false);
        dmp.decrIndent();
        dmp<<_T(".end\n\n");
    }

    if ((iMax = globRet.getUsed())>0)
    {
        dmp<<_T(".global_retain\n");
        dmp.incrIndent();
        for(i=0; i<iMax; i++)
            CG_VM_DumpOneVariable(dmp, this, pTrg, globRet.get(i), false);
        dmp.decrIndent();
        dmp<<_T(".end\n\n");
    }

    if(dmp.ftell())
        CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
}

void CGlobCtx::dumpProgInstances(const CG_Symtab* pGVL,
                                 CTarget*         pTrg,
                                 LPCTSTR          fileName)
{
    CGT_CDmp           dmp;
    const CG_SymEntry* pEntry;

    // globals section:
    dmp << _T(".global\n");
    dmp.incrIndent();
    for(pEntry=pGVL->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(pEntry->pType->type == CG_PROGRAM)
            CG_VM_DumpOneVariable(dmp, this, pTrg, pEntry, false);
    }
    dmp.decrIndent();
    dmp<<_T(".end\n\n");

    CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
}

void CGlobCtx::dumpResource(const CG_Resource* pRes, LPCTSTR fileName)
{
    CGT_CDmp           dmp;

    //.resource section for task properties
    dmp << _T(".resource\n");
    dmp.incrIndent();

    for(int jj=0; jj<pRes->nTasks; jj++)
    {
        const CG_Task* pTask = &pRes->pTasks[jj];
        assert(pTask);

        dmp.lo(pTask->pszId);
        dmp << _T("  ") << jj << _T(' ');   // jj is task ID
        __int64 Interval;
        __int64 Autostart;
        __int64 Prio;
        __int64 Loaded;
        __int64 WatchDog;
        const CG_Literal* pEventsLit; 

        // TODO : use lookUpTaskInit() in future
        bool bIntervalGiven  = GetTaskInitVal(pTask, TASK_ATTR_INTERVAL, &Interval);
        bool bAutostartGiven = GetTaskInitVal(pTask, TASK_ATTR_AUTOSTART, &Autostart);
        bool bPrioGiven      = GetTaskInitVal(pTask, TASK_ATTR_PRIO, &Prio);
        bool bLoadedGiven    = GetTaskInitVal(pTask, TASK_ATTR_LOADED, &Loaded);
        bool bEventsGiven    = GetTaskInitLit(pTask, TASK_ATTR_EVENTS, &pEventsLit);
        bool bWatchDogGiven  = GetTaskInitVal(pTask, TASK_ATTR_WATCHDOG, &WatchDog);

        assert((bIntervalGiven && bEventsGiven) == false);
            
        if (bAutostartGiven)
            dmp << Autostart << _T(' ');
        else
            dmp << 1 << _T(' ');        // default is autostart=true

        if (bIntervalGiven)
		{
			if (Interval == 0)
				dmp << 0x7ffffffeul << _T(' ');
			else
				dmp << Interval << _T(' ');
		}
        else
        {
            dmp << (bEventsGiven ? 0x7ffffffful : 100) << _T(' ');      // default is 100ms
        }

        if (bPrioGiven)
            dmp << Prio << _T(' ');
        else
            dmp << 9 << _T(' ');        // default is prio 9

        if (bLoadedGiven)
            dmp << Loaded << _T(' ');
        else
            dmp << 1 << _T(' ');        // default is loaded=true

        if (bEventsGiven)
        {
            __int64 Events = 0;

            if(CG_IsAnyInt(pEventsLit->type))
            {
                assert(pEventsLit->i64 >= 1 || pEventsLit->i64 <= 64);
                Events |= (__int64)1 << (pEventsLit->i64-1);
            }
            else if (pEventsLit->type == CG_STRING || pEventsLit->type == CG_WSTRING)
            {
                const TCHAR* p;
                long         jot;
                
                for (p = pEventsLit->psz; p; p = _tcschr(p, _T(',')))
                {
                    p   = _tcsinc(p);
                    jot = _tcstol(p, const_cast<TCHAR**>(&p), 10);

                    assert(jot >= 1 || jot <= 64);
                    Events |= (__int64)1 << (jot-1);
                }
            }           
            dmp << _T("lint#") << Events << _T(' ');   
        }
        else
            dmp << 0 << _T(' ');        // default is event=0


        if(bWatchDogGiven)
            dmp << WatchDog << _T(' ');
        else
        {
            CTarget* pTrg = getTarget();
            if(pTrg && pTrg->getTrgVer()<22000)
                dmp << 0 << _T(' ');        // default is watchdog=0 if version is <22000
            else
                dmp << _T("16#ffffffff") << _T(' ');       // default is watchdog=-1 if version is >=22000
        }

        dmp << '\n';
    }

    //close resource section:
    dmp.decrIndent();
    dmp<<_T(".end\n\n");

    CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
}







static void DumpRetainListRecStruct(
    const CG_Symtab*   pSym,         //symtab of the instance
    long*              pNRetainItems,
    CGT_CDmp&          dmp,
    CGT_CInstPath&     instpath
)
{
}

static void DumpRetainListRec(
    CGT_CDiagMsg*      pdm,
    const CG_Edipos*   pEdp,
    const CG_Symtab*   pSym,         //symtab of the instance
    bool               bOuterRetain, //true if the instance is declared retain
    bool               bOuterConst,  //if instance is declared constan, only for [array of] struct
    CGT_CDmp&          dmpRC, 
    CGT_CDmp&          dmpRW, 
    CGT_CInstPath&     instpath
)
{
    const CG_SymEntry*  pEntry;
    CGT_CDmp*           pdmp;
    FC_CString          simpleRC;
    FC_CString          simpleRW;


    if(pSym->pouTyp==CG_POU_SDT)
    {
        assert(bOuterRetain);

        //get nested instances:
        bool bHasSimple = false;
        for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
        {
            assert((pEntry->declScope == CG_VAR) && (pEntry->declMod & CG_DMOD_ANY_RETAIN)==0);
            if(pEntry->pType->pSym)
            {
                instpath.append(pEntry->pszId);
                if(pEntry->pType->bIsArray)
                {
                    for(long kk=0; kk<pEntry->pType->nElems; kk++)
                    {
                        instpath.appendInt(kk);
                        DumpRetainListRec(pdm, &pEntry->edp, pEntry->pType->pSym, true, 
                                          bOuterConst, dmpRC, dmpRW, instpath);
                        instpath.stripLast();
                    }
                }
                else
                {
                    DumpRetainListRec(pdm, &pEntry->edp, pEntry->pType->pSym, true, 
                                      bOuterConst, dmpRC, dmpRW, instpath);
                }
                instpath.stripLast();
            }
            else if(pEntry->pType->bIsArray)
            {
                //array of simple:
                pdmp = bOuterConst ? &dmpRC : &dmpRW;
                (*pdmp).lo(instpath)<<'.'<<pEntry->pszId;
                (*pdmp).sep(64)<<_T(" ;")<<pEntry->pType->pszName;
                (*pdmp)<<'\n';
            }
            else
            {
                bHasSimple = true;
            }
        }

        //all members, as long as there is no non_retain for a struct decl:
        if(bHasSimple)
        {
            pdmp = bOuterConst ? &dmpRC : &dmpRW;
            (*pdmp).lo(instpath);
            (*pdmp).sep(64)<<_T(" ;")<<pSym->pszId;
            (*pdmp)<<'\n';
        }

        return;
    }
    //else
    assert(!bOuterConst);
    assert(pSym->pouTyp==CG_POU_FB || pSym->pouTyp==CG_POU_PRG);


    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        if(pEntry->declScope != CG_VAR && pEntry->declScope != CG_VAR_INPUT
            && pEntry->declScope != CG_VAR_OUTPUT
          )
            continue;

        if(pEntry->pType->type==CG_INSTANCE)
        {
            assert(!pEntry->pType->bIsArray); 
            instpath.append(pEntry->pszId);
            DumpRetainListRec(pdm, &pEntry->edp, pEntry->pType->pSym, 
                              (pEntry->declMod & CG_DMOD_RETAIN), 
                              false, dmpRC, dmpRW, instpath);
            instpath.stripLast();
            continue;
        }

        if(!((pEntry->declMod & CG_DMOD_RETAIN) ||
           (bOuterRetain && (pEntry->declMod & CG_DMOD_NON_RETAIN)==0))
          )
            continue;
        
        bool bConstant = (pEntry->declMod & CG_DMOD_CONSTANT)!=0;

        if(pEntry->pType->type==CG_STRUCT)
        {
            instpath.append(pEntry->pszId);
            if(pEntry->pType->bIsArray)
            {
                for(long kk=0; kk<pEntry->pType->nElems; kk++)
                {
                    instpath.appendInt(kk);
                    DumpRetainListRec(pdm, &pEntry->edp, pEntry->pType->pSym, true, 
                                      bConstant, dmpRC, dmpRW, instpath);
                    instpath.stripLast();
                }
            }
            else
            {
                DumpRetainListRec(pdm, &pEntry->edp, pEntry->pType->pSym, 
                                  true, bConstant, dmpRC, dmpRW, instpath);
            }
            instpath.stripLast();
        }
        else if(pEntry->pType->bIsArray)
        {
            //array of simple:
            pdmp = bConstant ? &dmpRC : &dmpRW;
            ((*pdmp).lo(instpath)<<'.').lo(pEntry->pszId);
            (*pdmp).sep(64)<<_T(" ;")<<pEntry->pType->pszName;
            (*pdmp)<<'\n';
        }
        else
        {
            if(bConstant)
            {
                if(!simpleRC.isEmpty())
                    simpleRC<<_T(", ");
                simpleRC.appendLo(pEntry->pszId);
            }
            else
            {
                if(!simpleRW.isEmpty())
                    simpleRW<<_T(", ");
                simpleRW.appendLo(pEntry->pszId);
            }
        }
    }      
    
    if(!simpleRC.isEmpty())
        dmpRC.lo(instpath)<<'('<<simpleRC<<_T(")\n");

    if(!simpleRW.isEmpty())
        dmpRW.lo(instpath)<<'('<<simpleRW<<_T(")\n");
}


static void DumpLocalRetainCopyList(
    CGT_CDiagMsg*  pdm, 
    CGT_CDmp&      dmpRC, 
    CGT_CDmp&      dmpRW, 
    const CG_Task* pTask
)
{
    int                i;
    CGT_CInstPath      instpath;
    const CG_SymEntry* pEntry;

    for(i=0; i<pTask->nPrgs; i++)
    {
        pEntry = pTask->pPrgs[i];
        assert(pEntry->pType->pSym->pouTyp==CG_POU_PRG);
        
        instpath.append(pEntry->pszId);
         DumpRetainListRec(pdm, &pEntry->edp, pEntry->pType->pSym, 
                          (pEntry->declMod & CG_DMOD_RETAIN)!=0, 
                          false, dmpRC, dmpRW, instpath);
        instpath.stripLast();
        assert(instpath.getDepth()==0);
    }
}



void CGlobCtx::dumpTasks(const CG_Resource* pRes, LPCTSTR fileName)
{
    int                jp;
    CGT_CDmp           dmp;
    CGT_CDmp           dmpRC;
    CGT_CDmp           dmpRW;

    dmpRC.incrIndent();
    dmpRW.incrIndent();


    // task section prginst assignment
    for(int jj=0; jj<pRes->nTasks; jj++)
    {
        const CG_Task* pTask = &pRes->pTasks[jj];

        (dmp<<_T(".task ")).lo(pTask->pszId)<<'\n';
        dmp.incrIndent();

        // proginst assignment
        for(jp=0; jp<pTask->nPrgs; jp++)
            (dmp<<_T("call ")).lo(pTask->pPrgs[jp]->pszId)<<'\n';
        dmp<<'\n';

        // read/write memory regions
        const MEM_RGNS* pAllMemRgns = m_TaskAllMemRgns.get(pTask);
        if(pAllMemRgns)
        {
            dumpMemRegion(dmp, &pAllMemRgns->GIMemRgnsRD, VM_BS_SEGM_INPUT,
                m_kad.getVmSettings()->szISegSize, 
                _T("read"), _T("input"));

            dumpMemRegion(dmp, &pAllMemRgns->GOMemRgnsWR, VM_BS_SEGM_OUTPUT,
                m_kad.getVmSettings()->szOSegSize, 
                _T("write"), _T("output"));

            dumpMemRegion(dmp, &pAllMemRgns->GOMemRgnsRD, VM_BS_SEGM_OUTPUT,
                m_kad.getVmSettings()->szOSegSize, 
                _T("read"),  _T("output"));
        }//d-2006: may be no mem region if no license and hence 'demo'
         //all addresses are treated as memory. Also this if(pAllMemRgns)
         //is generic is the sense that the task simply 'has no mem regions'.

        //close task section and write to file:
        dmp.decrIndent();
        dmp<<_T(".end\n\n");

        //dump all local retain items for program instances of the task:
        DumpLocalRetainCopyList(&m_dm, dmpRC, dmpRW, pTask);
        if(dmpRW.ftell()>0)
        {
            (dmp<<_T(".copy RW ")).lo(pTask->pszId)<<'\n';
            dmp.append(dmpRW);
            dmp<<_T(".end\n\n");
        }
        
        CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
        //clear dumpers for next task:
        dmp.seek(0);
        dmpRW.seek(0);
    }


    if(dmpRC.ftell()>0)
    {
        dmp.seek(0);
        dmp<<_T(".copy RC\n");
        dmp.append(dmpRC);
        dmp<<_T(".end\n\n");
        CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
    }

}

void CGlobCtx::dumpMemRegion( 
                             CGT_CDmp&                  dmp,
                             const FC_CGArray<VM_MemRegion*>* pMemRgns,
                             VM_BITSET                  bsCurSegm,
                             LPCTSTR                    pszSegmSize,
                             LPCTSTR                    pszListType,
                             LPCTSTR                    pszSegm)
{
    int        iNum = 0;
    const int  iPairsPerLine = 8;
    TCHAR      buf1[64];
    TCHAR      buf2[64];


    if ((m_kad.getUseTIforSegm() & bsCurSegm) == 0)
    {
        return; // for this access type (READ/WRITE) we have no task image
    }

    if ((m_kad.getOptimizeTIforSegmBS() & bsCurSegm) == 0)
    {
        // dump a default region. size is the segment size
        dmp << pszListType << _T(" ") << pszSegm << _T(": ");
        dmp << _T("(0x0, ")
            << (*pszSegmSize ? pszSegmSize : _T("0x0"))
            << _T(")\n");
    }


    int iListSize = pMemRgns->getUsed();
    if (iListSize == 0)
        return; // nothing to do

    int iMaxLoops = iListSize / iPairsPerLine + 1;
    for (int iLoop=0; iLoop<iMaxLoops; iLoop++)
    {
        dmp << pszListType << _T(" ") << pszSegm << _T(": ");
        if (iNum == iListSize)
        {
            dmp<<'\n';
            return;
        }

        for (int i=0; i<iPairsPerLine; i++)
        {
            iNum++;
            if (iNum > iListSize)
            {
                dmp<<'\n';
                return;
            }

            const VM_MemRegion* pMemRgn = pMemRgns->get(iNum-1);

            if(pMemRgn->ulSize > 0xffff)//HACK: size is only a WORD inside controller, at least
            {                           //check it here, should be considered in memory region algorithm ? 
                m_dm.msg1(CG_E_TEXT, NULL, _T("copy regions: region size exceeds max size of 65535"));
            }

            dmp << _T("(0x") << _ultot(pMemRgn->ulOffset, buf1, 16) 
                << _T(",0x") << _ultot(pMemRgn->ulSize, buf2, 16) << _T(")");

            if(iNum < iListSize && i < iPairsPerLine-1)
                dmp<<',';
        }
        dmp<<'\n';
    }
}



static void VM_ExpandArrayInits(
    FC_CGArray<const CG_Initialiser*>* painits, 
    const CG_ArrayInit*                pArrInit
)
{
    for (int ii=0; ii<pArrInit->nInits; ii++)
    for (int ir=0; ir<pArrInit->pRepeats[ii]; ir++)
    {
        if(pArrInit->pInits[ii].ut == CG_INIT_ARRAY)
            VM_ExpandArrayInits(painits, &pArrInit->pInits[ii].arr);
        else
            painits->add(&pArrInit->pInits[ii]);
    }
}

static void VM_ExpandInitialiser(
    const CG_SymEntry*                  pEntry, //the entry that is to be initialised
    const CG_Initialiser*               pInit,  //the init values
    CGT_CInstPath&                      path,   //path to entry to be initialised
    FC_CHashStr<const CG_Initialiser*>* pList   //hash of init values
)
{
    switch(pInit->ut)
    {
    case CG_INIT_SIMPLE:
    {
        const CG_Initialiser* pPrev = NULL;
        bool bNew = pList->insertEx(path, pInit, true, &pPrev);
        assert(bNew || pPrev);
    }
    break;
    case CG_INIT_STRUCT:
    {
        for (int ie=0; ie<pInit->str.nInits; ie++)
        {
            path.appendLo(pInit->str.pEntries[ie]->pszId);
            VM_ExpandInitialiser(pInit->str.pEntries[ie], &pInit->str.pInits[ie], path, pList);
            path.stripLast();
        }
    }
    break;
    case CG_INIT_ARRAY:
    {
        if(pEntry->pType->pSym)
        {   //array of struct initialiser, expand:
            FC_CGArray<const CG_Initialiser*> ainits(pEntry->pType->nElems);
            VM_ExpandArrayInits(&ainits, &pInit->arr);

            long nElemsMax = min(pEntry->pType->nElems, ainits.getUsed());
            for (int i=0; i<nElemsMax; i++)
            {
                path.appendInt(i);
                VM_ExpandInitialiser(pEntry, ainits.get(i), path, pList);
                path.stripLast();
            }
        }
        else
        {   //simple array must be passed directly, do not expand:
            const CG_Initialiser* pPrev = NULL;
            bool bNew = pList->insertEx(path, pInit, true, &pPrev);
            assert(bNew || pPrev);
        }
    }
    break;
    case CG_INIT_NONE:   //used only for array init feature 4()
    break;
    default:
        assert(!"bad init typ");
    }
}
static void VM_GetInstanceInitList(
    const CG_Symtab*                    pSym, 
    CGT_CInstPath&                      path, 
    FC_CHashStr<const CG_Initialiser*>* pList
)
{
    for(const CG_SymEntry* pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        if(!pEntry->pType->pSym)
            continue;//we want only process struct initial values, simple or array of simple
                     //inits are handeled directly in the type definition passsed to asm.
        if(pEntry->declScope==CG_VAR_EXTERNAL || pEntry->declScope==CG_VAR_IN_OUT)
            continue;

        path.appendLo(pEntry->pszId);
        if(pEntry->pType->bIsArray)
        {
            for(long kk=0; kk<pEntry->pType->nElems; kk++)
            {
                path.appendInt(kk);
                VM_GetInstanceInitList(pEntry->pType->pSym, path, pList);
                path.stripLast();
            }
            if(pEntry->pInit)
            {
                assert(pEntry->pInit->ut == CG_INIT_ARRAY);
                VM_ExpandInitialiser(pEntry, pEntry->pInit, path, pList);
            }
        }
        else
        {
            VM_GetInstanceInitList(pEntry->pType->pSym, path, pList);
            if(pEntry->pInit)
            {
                assert(pEntry->pInit->ut == CG_INIT_STRUCT);
                VM_ExpandInitialiser(pEntry, pEntry->pInit, path, pList);
            }
        }
        path.stripLast();
    }
}


void CGlobCtx::dumpConfigSect(const CG_Resource* pRes, LPCTSTR fileName)
{
    int                                             i;
    CGT_CDmp                                        dmp;
    CGT_CInstPath                                   path;
    FC_CHashStr<const CG_Initialiser*>              list(false);
    const FC_CHashStr<const CG_Initialiser*>::Pair* pPair;

    //.config section for complex inits
    dmp << _T(".config\n");
    dmp.incrIndent();

    //next function is tricky: we want all inits that arise from 
    //struct init, not those of simple or array of simple (these are 
    //passed to asm via type definitions directly).
    VM_GetInstanceInitList(pRes->pGVL, path, &list);
    for (i=0; i<pRes->pGVL->nGVLImp; i++)
        VM_GetInstanceInitList(pRes->pGVL->pGVLImp[i].pSym, path, &list);

    for(pPair = list.firstPair(); pPair; pPair = pPair->pNext)
    {
        dmp<<pPair->pszKey<<_T(" := ");
        assert(pPair->pVal);
        if(pPair->pVal->ut == CG_INIT_SIMPLE)
            VMDumpLitValue(dmp, pPair->pVal->sim.pLit);
        else if(pPair->pVal->ut == CG_INIT_ARRAY)
            VMDumpArrayInit(dmp, &pPair->pVal->arr);
        else
            assert(!"bad init type");

        dmp<<'\n';
    }

    dmp.decrIndent();
    dmp<<_T(".end\n\n");

    CGT_DmpToFile(&m_dm, NULL, fileName, dmp, true);
}

void CGlobCtx::compilePOU(
    const CG_Resource*   pRes,
    const CG_Symtab*     pSym
)
{
    bool                   bOk;
    ICG_Body*              pIBdy;
    CCodeInfo*             pInfo;
    int                    nCurrErr;
    FC_CGArray<CGT_BP_TYP> bpList;


    //assert: don't have inline fb,prg:
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
    
    //compile all bodies of used FBs and Structs, if pSym->pzRefSyms 
    // is already defined which should be true if has no body:
    bool  bDid = false;
    if(pSym->pzRefSyms)
    {
        bDid = true;
        //pass here if inline, precompiled lib or STRUCT:
        assert( ! (pSym->flags & CG_STAB_HAS_BDY));

        for(const CG_Symtab*const* ppSym=pSym->pzRefSyms; *ppSym; ppSym++)
            compilePOU(pRes, *ppSym);
    }

    


    if(pSym->pouTyp==CG_POU_SDT)
    {
        assert(bDid);
        GenCodeStruct(this, pInfo);
    }
    else if(pSym->flags & CG_STAB_HAS_BDY)
    {
        // output the file name but not if part of any lib!
        if(!pSym->pLibCtx)
            m_dm.userMsg(m_dm.jot1()<<pSym->edp.pszFile<<_T("\n"));
   
        //this back end does not reload bodies...
        assert((pSym->flags & CG_STAB_BDY_LOADED)==0);

        const CG_Symtab* pSym2 = m_pIF->loadBody(pSym->pszId, CG_LOADBDY_TMPVAR_OPT1, &pIBdy);
        //cannot return NULL, we have already a pSym
        assert(pSym2==pSym && (pSym->flags & CG_STAB_BDY_LOADED));
        //body or error:
        assert(pIBdy || m_dm.hasErr());

        if(pIBdy)
        {
            FC_CHashPtr<const CG_Symtab*, const CCodeInfo*> called;

            assert(pSym->flags & CG_STAB_BDY_NOERR);

            //compile all bodies of all used FBs and Structs, even
            // if attracted bei "#IMPORT *.GVL" therefore it 
            // must be done after loadBody()
            for(const CG_Symtab*const* ppSym=pSym->pzRefSyms; *ppSym; ppSym++)
                compilePOU(pRes, *ppSym);
                        
            //keep a reference to the ext body info:
            assert(!pInfo->m_pIExtBdyInf);
            pInfo->m_pIExtBdyInf = m_pIF->loadExtBodyInfo(pSym);

            if (pSym->pouTyp == CG_POU_FUN)
                GenCodeFun(this, pRes, pIBdy, pInfo, &bpList);
            else if(pIBdy->isSFC())
                GenCodeSFCPrgFB(this, pRes, pIBdy, pInfo, &bpList);
            else
                GenCodeSTPrgFB(this, pRes, pIBdy, pInfo, &bpList);
        }
    }
    else if(pSym->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR))
    {
        assert(pSym->pouTyp==CG_POU_FB || pSym->pouTyp==CG_POU_FUN);
        // need only POUINFO and DBI dumping

        //get the not supported diags also for macro functions:
        for(const CG_SymEntry* pEH = pSym->pFirst; pEH; pEH = pEH->pNext)
            VMCheckDecl(this, this->getTarget(), pEH);
    }
    else if(pSym->pLibCtx)
    {
        //get precompiled code from lib:
        GenCodeLibPOU(this, pSym, pInfo, &bpList);
    }
    else
    {
        assert(!"neither body nor lib Prg,FB,SDT,FB-Macro?");
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
           getTrgDBIPath()
        );
                                // TODO: dump only DBI for FB not
                                //       nested lib FBs of this
    }
    if (pIBdy)
        pIBdy->Release();

    if(nCurrErr!=m_dm.getTotalErrors())
        pInfo->m_bErrors = true;
}

void CGlobCtx::compile4CPPOU(
    const CG_POU4CPInfo* pInf //a 4cp POU info
)
{
    const CG_Symtab*    pSym;

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

   
    if(pSym->pouTyp==CG_POU_PRG || pSym->pouTyp==CG_POU_FB || pSym->pouTyp==CG_POU_SDT || pSym->pouTyp==CG_POU_FUN)
    {
        compilePOU(NULL, pSym); 
    }
    return;
}




void CGlobCtx::callExternalCompiler(const CTarget* pTrg)
{
    FC_CString  fileName(MAX_PATH);
    long        result;
    FC_CString  exeP(1024);
    FC_CString  cmdL(1024);
    FC_CString  msgLine(104);

   
    // debug feature
    if ( ! m_popt->bCallAsm)
    {
        // do not call the external assembler
        return;
    }

    fileName.load(pTrg->getTrgGenPath())<<'\\';
    fileName.appendLo(pTrg->getTrgName()) << CG_VM_ASM_TEXT_EXT;
    assert(FC_FileExists(NULL, fileName));

    //build full path to asm: 
    exeP<<m_pIF->FCPgetInstallationPath()<<_T("\\")<<m_popt->szCurTrgTyp<<_T("\\4asm.exe");
    
    // build command line
    cmdL.clear();
    cmdL.appendWsprintf(_T(" -nologo %s\"%s\""),
        m_popt->nGenAsmMapFile!=0 ? _T("-map ") : _T(""),
        fileName.psz()
    );


    // output to window
    if(m_popt->nVerboseMode!=0)
    {
        if(m_popt->nVerboseMode>1)
        {
            //if debug we want to see all details:
            msgLine << _T(" ") << exeP << cmdL << _T("\n");
        }
        else
        {
            //only short hint:
            msgLine << _T(" ") << FC_StringGetFilePart(exeP);
            msgLine.changeFileExt(NULL)<<_T(" -nologo ") << fileName << _T("\n");
        }
        m_dm.userMsg(msgLine);
    }


    // EXECUTE
    clock_t start = clock();
    result = 1236984624;//<-only to be safe
    if( ! FC_System(exeP, cmdL, NULL, &result))
    {
        DWORD dwLast = GetLastError();
        m_dm.msg1(CG_E_BACKEND_ERROR, NULL, m_popt->szCurTrgTyp);
        m_dm.sysErr(dwLast, NULL, exeP, NULL);
    }
    clock_t finish = clock();   
    
    if(result!=0)
    {    //bad return from 4asm
        m_dm.msg1(CG_E_BACKEND_ERROR, NULL, m_popt->szCurTrgTyp);
    }
}

void CGlobCtx::callPostCompile(const CTarget* pTrg)
{
    long         result;
    const TCHAR* psz;
    TCHAR        szExe[2*MAX_PATH];
    FC_CString   cmdL(MAX_PATH);
    FC_CString   msgLine(MAX_PATH);


    // debug feature
    if ( ! m_popt->bCallPostCompiler)
    {
        // do not call the external post copmile step
        return;
    }

    psz = m_kad.getCgOptions()->szPostCompileExe;
    if (*psz==0)
        return;// this target has no KAD entries for a post compile step
    //make the exe to be a full path if not already:
    FC_FileFullPath(m_pIF->FCPgetInstallationPath(), psz, szExe, FC_ARRAY_LEN(szExe));
    


    //build command line using inserts:
    psz = m_kad.getCgOptions()->szPostCall;
    cmdL.clear();

    while(*psz)
    {
        if(*psz!=_T('%'))
        {
            cmdL.addc(*psz);
            psz++;
            continue;
        }

        
        if(FC_StringIncBeginI(&psz, _T("%targetgenpath%")))
        {
            cmdL << _T("\"") << pTrg->getTrgGenPath() << _T("\"");
        }
        else if(FC_StringIncBeginI(&psz, _T("%postopt%")))
        {
            cmdL << m_popt->sPostCompileOpt;
        }
        else if(FC_StringIncBeginI(&psz, _T("%firmwareversion%")))
        {
            cmdL << pTrg->getTrgVer();
        }
        else if(FC_StringIncBeginI(&psz, _T("%bpopt%")))
        {
            cmdL << m_bpopt;
        }
        else if(FC_StringIncBeginI(&psz, _T("%targettype%")))
        {
            cmdL << pTrg->getTrgType();
        }
        //TODO: compatibillity: old syntax, tobe removed....
        else if(FC_StringIncBeginI(&psz, _T("%targetgenpath")))
        {
            cmdL << _T("\"") << pTrg->getTrgGenPath() << _T("\" ");
        }
        else if(FC_StringIncBeginI(&psz, _T("%postopt")))
        {
            cmdL << m_popt->sPostCompileOpt << _T(" ");
        }
        else
        {
            CG_Edipos edp;
            CG_InitSTEdipos(&edp, m_kad.getFile());
            m_dm.msg1(CG_E_SYNTAX_UNEXPECTED, &edp, psz);
            return;
        }
    }


    // output to window
    if(m_popt->nVerboseMode!=0)
    {
        if(m_popt->nVerboseMode>1)
        {
            //if debug we want to see all details:
            msgLine << _T(" ")<< szExe << _T(" ") << cmdL << _T("\n");
        }
        else
        {
            //only short hint:
            msgLine << _T(" ") << FC_StringGetFilePart(szExe);
            msgLine.changeFileExt(NULL);
            msgLine << _T(" -nologo ") << pTrg->getTrgGenPath() << _T("\n");

        }
        m_dm.userMsg(msgLine);
    }


    // EXECUTE
    // call post compile step
    clock_t start = clock();
    result = 1236984624;//<-only to be safe
    m_bNccCalled = true;
    if( ! FC_System(szExe, cmdL, NULL, &result))
    {
        if ( ! FC_FileExists(NULL, szExe))
        {
            m_dm.msg1(CG_E_FILE_NOT_EXIST, NULL, szExe);
        }
        else
        {
            DWORD dwLast = GetLastError();
            m_dm.msg1(CG_E_BACKEND_ERROR, NULL, m_popt->szCurTrgTyp);
            m_dm.sysErr(dwLast, NULL, szExe, NULL);
        }
    }
    
    clock_t finish = clock();   
    //TODO incrExternalCompileTime(finish - start);
    
    if(result!=0)
    { 
        //bad return from 4asm
        m_dm.msg1(CG_E_BACKEND_ERROR, NULL, m_popt->szCurTrgTyp);
    }
}

void CGlobCtx::buildDLFile(const CTarget* pTrg)
{
    FILE* pFile;
    
    if (_tcsicmp(m_pCmdlOpts.szCurTrgTyp, _T("4cosai")))
        return;

    FC_CString FileName(80);
    FileName.load(pTrg->getTrgGenPath());
    FileName << _T("\\") FILE_NAME_OSAI_DL_FILE_FRGR_TASK;
    
    pFile = fopen(FileName, "wb");

    appendFile(pFile, pTrg->getTrgGenPath(), FILE_NAME_COMMON_HDR);
    appendFile(pFile, pTrg->getTrgGenPath(), FILE_NAME_ALL_CLASSES FILE_EXT_BINARY);
    appendFile(pFile, pTrg->getTrgGenPath(), FILE_NAME_ALL_OBJECTS FILE_EXT_INITDATA);
    appendFile(pFile, pTrg->getTrgGenPath(), FILE_NAME_CALL_STACK);
}

void CGlobCtx::appendFile(FILE*         pDestFile,
                          const TCHAR*  pPath,
                          const TCHAR*  pFileToAppend)
{
    FILE* pFile;
    
    assert(pPath != NULL);
    assert(pFileToAppend != NULL);

    FC_CString FileName(80);
    FileName.load(pPath);
    FileName << _T("\\") << pFileToAppend;
    
    pFile = fopen(FileName, "rb");
    assert(pFile);
    if (pFile == NULL)
    {
        CGT_IERR(&m_dm, 
            m_dm.jot1() << _T("fatal: cannot open file '") << FileName << _T("'\n"));
        return;
    }
    
    fseek(pFile, 0, SEEK_END);
    long fileLen = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    fwrite(&fileLen, 1, sizeof(long), pDestFile);
    while ( ! feof(pFile) )
    {
        int ch;
        if ((ch = fgetc(pFile)) != EOF)
            fputc(ch, pDestFile);
    }

    fclose(pFile);
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


// OSAI Hack >>
bool gen4GHFile(const TCHAR* pszFileName, const TCHAR* pszProjGuid)
{
    FC_CString sFileName;
    FC_CString sTmp;

    sFileName.load(pszFileName);

    // build up write string
    HRESULT             hr;

    sTmp.addpsz(_T("\r\n"));
    sTmp.addpsz(pszProjGuid);
    sTmp.addpsz(_T("\r\n"));

    TCHAR szFingerPrint[40];
    hr = UTIL_GetStringFingerprint(sTmp, szFingerPrint);

    if(hr != S_OK)
    {
        return false;
    }

    sTmp.addpsz(szFingerPrint);

    // encrytion
    long    lSize = sTmp.len()*2+17;
    TCHAR*  pszEncrypted = new TCHAR[lSize*2 + 17];

    hr = UTIL_EncryptString(sTmp.psz(), pszEncrypted, NULL);

    if(hr != S_OK)
    {
        delete [] pszEncrypted;
        return false;
    }

    HANDLE  hFile = ::CreateFile(sFileName.psz(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        delete [] pszEncrypted;
        return false;
    }

    DWORD dwLen = _tcslen(pszEncrypted);
    DWORD dwWritten;
    bool  bReturn = false;

    if(::WriteFile(hFile, pszEncrypted, dwLen, &dwWritten, NULL) && dwWritten == dwLen)
    {
        bReturn = true;
    }

    delete [] pszEncrypted;
    ::CloseHandle(hFile);

    return bReturn;
}
// OSAI Hack <<


/* ---------------------------------------------------------------------------- */
