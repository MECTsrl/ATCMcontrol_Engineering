/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/helper.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: helper.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/helper.cpp $
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

#include "helper.h"
#include "BuildNr.h"
#include "Product.h"
#include "defines.h"
#include "globctx.h"
#include "resource.h"
#include "fc_tools\fc_fileIO.h"
#include "VmConstants.h"

#include "CG_IECFront.h"

#include "4cvmAdapt.h"



void SetFeatures(ICG_IECFront* pIFront, const CKADInfo* pKAD, LPCTSTR trg)
{
    CG_Features  feat;
    
    memset(&feat, 0, sizeof(CG_Features));

    //TODO make _CG_Features complete !!!! 
    feat.pszTargetTyp = trg;

    CG_BITSET SupportedSimpleType = pKAD->getIECTypes();

    //languages (ST is mandatory):
    feat.bLadder = _tcsstr(pKAD->getIECLanguages(), _T("LD")) != NULL;
    feat.bFBD    = _tcsstr(pKAD->getIECLanguages(), _T("FBD")) != NULL;
    feat.bSFC    = _tcsstr(pKAD->getIECLanguages(), _T("SFC")) != NULL;
  
    //declaration (header) features:
    feat.bIncompleteAddr    = 0;      //allow %* and %+ syntax
    feat.bAssumedSizeArrays = 1;      //allow ARRAY OF <type> syntax
    feat.declModifiers      = CG_DMOD_ANY_RETAIN|CG_DMOD_CONSTANT;
    feat.bMemoryDirectAddr  = 1;
    feat.allTypes           = SupportedSimpleType;
    feat.arrayTypes         = (SupportedSimpleType|CG_TBS_STRUCT);
    feat.structMemberTypes  = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.funRetTypes        = SupportedSimpleType; //TODO: |CG_TBS_ARRAY|CG_TBS_STRCUT;
    feat.funInputTypes      = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT; 
    feat.funOutputTypes     = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.funInOutTypes      = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.funLocalTypes      = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.FBInOutTypes       = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT;
    feat.gvlTypes           = SupportedSimpleType|CG_TBS_ARRAY|CG_TBS_STRUCT;

    feat.preferedInt        = 16;
    feat.preferedFloat      = 64;

    //allowed decl scopes:
    feat.bVarNativeFun   = 1;
    feat.bVarNativeFBPrg = 1;
    feat.bVarInOutFun    = 1; 
    feat.bVarInOutFB     = 1; 
    feat.bVarTmpFun      = 0; 
    feat.bVarTmpFBPrg    = 0; 
    feat.bVarGlobPrg     = 0; 
    feat.bVarExtFun      = 0; 

    //task features:
    TCHAR*          ppszTI[]  = {_T("INTERVAL"), _T("PRIORITY"), _T("AUTOSTART"), _T("LOADED"), _T("EVENT"), _T("WATCHDOG")}; 
    const CG_BITSET pTITypes[]= {CG_TBS_TIME, CG_TBS_ANY_INT, CG_TBS_BOOL, CG_TBS_BOOL, CG_TBS_ANY_INT|CG_TBS_ANY_STRING, CG_TBS_TIME|CG_TBS_ANY_INT}; 

    feat.nTIPairs = 6; 
    feat.ppszTI   = ppszTI;	
    feat.pTITypes = pTITypes; 


    // allow "inline{xyz}" in FBs
    feat.bMacroFB = true;

    //SFC features:
    //TODO: action qualifiers


    //general limits nMax_* 
    //TODO e.g. max vars per POU, max struct nesting level, ....
    feat.nMaxStrLen = 255;

    if (pKAD->getCgOptions()->clsidAutoDecl.Data1 != 0)
    {
        feat.pAutodecl = &pKAD->getCgOptions()->clsidAutoDecl;
        feat.pszAutoDeclConfig = pKAD->getCgOptions()->szAutoDeclIniFile;
                    // "szAutoDeclIniFile" should be interpreted as a
                    // relative path to $(INSTPATH)\Engineerung\bin + FileName
                    // eg. "4cOsai\\AutoDecl.ini"
    }
    else
    {
        feat.pAutodecl = NULL;
        feat.pszAutoDeclConfig = NULL;
    }


    Adapt4CVM_SetFeatures(&feat);

    pIFront->loadFeatures(&feat);

}

void DumpResponseFile(
    CGT_CDiagMsg*              pdm, 
    const TCHAR*               pszTrgType,
    ICG_IECFront*              pIFront,
    const CG_POU4CPInfo*const* pzpProcessed
)
{
    CGT_CDmp                   dmp;
    const CG_POU4CPInfo*const* ppInf;
  

    dmp<<pdm->getDisplayedErrors()<<_T("\n");
    dmp<<pdm->getDisplayedWarnings()<<_T("\n");

    for(ppInf = pzpProcessed; *ppInf; ppInf++)
        dmp<<(*ppInf)->pszFile<<_T("\n");
    

    FC_CString resp(MAX_PATH);
    resp.load(pIFront->FCPgetGenPath())<<_T("\\.")<<pszTrgType<<_T("\\result.txt");
    CGT_DmpToFile(pdm, NULL, resp, dmp, false);
}


bool GetTaskInitVal(const CG_Task* pTask, LPCTSTR attr, __int64* pVal)
{
    for (int ii=0; ii<pTask->nInits; ii++)
    {
        CG_TaskInit tskInit = pTask->pInits[ii];
        if ( ! stricmp(tskInit.pszAttr, attr))
        {
            assert(tskInit.val.extyp == CG_LITERAL);
            *pVal = tskInit.val.lit.pLit->i64;
            return true;
        }
    }

    return false;   // attribute not set
}

bool GetTaskInitLit(const CG_Task* pTask, LPCTSTR attr, const CG_Literal** pLit)
{
    for (int ii=0; ii<pTask->nInits; ii++)
    {
        CG_TaskInit tskInit = pTask->pInits[ii];
        if ( ! stricmp(tskInit.pszAttr, attr))
        {
            assert(tskInit.val.extyp == CG_LITERAL);
            *pLit = tskInit.val.lit.pLit;
            return true;
        }
    }

    return false;   // attribute not set
}


//TODO make a core feature of sysgen:
typedef FC_CHashPtr<long,bool>      CGT_LCTHASHL;
typedef FC_CHashStr<CGT_LCTHASHL* > CGT_LCTHASHF;

static CGT_LCTHASHF* g_lctHash = NULL;

struct LCT_LINE
{
    long        lineNo;
    const char* pszInf;
};
struct LCT_FILE
{
    const char*     pszSrc;
    const LCT_LINE* pLines;
};
//#include "__PerlGenLCT.inc"
LCT_FILE g_lctfileList[]={0,NULL};




void LCT_RecoredHit(const char* pszFile, long lineNo)
{
    CGT_LCTHASHL* pLines;
    if(!g_lctHash)
        g_lctHash = new CGT_LCTHASHF(false, false, 512);

    pszFile = FC_StringGetFilePart(pszFile);

    pLines = g_lctHash->get(pszFile);
    if(!pLines)
    {
        pLines = new CGT_LCTHASHL(1024);
        g_lctHash->insert(pszFile, pLines);
    }

    pLines->insert(lineNo, true);
}
static bool LCT_IsHit(const TCHAR* pszFile, long lineNo)
{
    CGT_LCTHASHL* pLines;
    
    if(!g_lctHash)
        return false;
    
    pLines = g_lctHash->get(FC_StringGetFilePart(pszFile));
    if(pLines)
        return pLines->getPair(lineNo) ? true : false;

    return false;
}

void LCT_DumpResult(const TCHAR* pszHint)
{
    const LCT_LINE* pl;
    const LCT_FILE* pf;
    TCHAR szFile[2*MAX_PATH];
    long nNewHits;
    long nOldHits;
    long nAllTest;
    FILE* pFile;
return;
    szFile[0] = 0;
    GetModuleFileName(NULL, szFile, FC_ARRAY_LEN(szFile));
    _tcscat(szFile, _T(".clt"));


    if(!FC_Exist(szFile))
    {
        _tprintf(_T("TEST: line coverage: create '%s'\n"), szFile);
          pFile = _tfopen(szFile, _T("at"));
        if(!pFile)
        {
            _tprintf(_T("error: line coverage test: cannot write file '%s'\n"),szFile);
            return;
        }

        for(pf= &g_lctfileList[0]; pf->pszSrc; pf++)
        {
            fprintf(pFile, "\n[%s]\n", pf->pszSrc);
            for(pl= &pf->pLines[0]; pl->lineNo>0; pl++)
                fprintf(pFile, "%d=TODO: %s\n", pl->lineNo, pl->pszInf);
        }
        fclose(pFile);
    }

// _tprintf(_T("TEST: line coverage: update '%s'\n"), szFile);

    if(pszHint)
        WritePrivateProfileString(_T("<History>"), "12.56.2002", pszHint, szFile);

    nNewHits = nOldHits = nAllTest = 0;
    for(pf= &g_lctfileList[0]; pf->pszSrc; pf++)
    {
        TCHAR szLineNo[64];
        TCHAR szBuffer[MAX_PATH];

        for(pl= &pf->pLines[0]; pl->lineNo>0; pl++)
        {
            nAllTest++;
            ltoa(pl->lineNo, szLineNo, 10);
            if(LCT_IsHit(pf->pszSrc, pl->lineNo))
            {
                _tcscpy(szBuffer, _T("OK: "));
                _tcscat(szBuffer, pl->pszInf);
                WritePrivateProfileString(pf->pszSrc, szLineNo, szBuffer, szFile);
                    nNewHits++;
            }
            else
            {
                GetPrivateProfileString(pf->pszSrc, szLineNo, _T(""), 
                szBuffer, FC_ARRAY_LEN(szBuffer), szFile);

                if(FC_StringBeginI(szBuffer, _T("OK:")))
                    nOldHits++;
            }
        }
    }

     _tprintf(_T("TEST: line coverage: new hits=%d, old hits=%d  (left to do=%d of %d)\n"),
         nNewHits, nOldHits, nAllTest-nNewHits-nOldHits, nAllTest);  

     if(g_lctHash)
     {
        g_lctHash->deletePtrs();
        delete(g_lctHash);
        g_lctHash = NULL;
     }

}




bool CheckGlobalFBLocalRetainRec(
    CGT_CDiagMsg*    pdm,
    const CG_Symtab* pSym, 
    bool             bInFB
)
{
    bool bOK = TRUE;
    const CG_SymEntry*  pEntry;

    if (pSym->pouTyp == CG_POU_FB)
    {
        bInFB = TRUE;
    }

    for(pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        if(pEntry->pType->pSym)
        {
            bOK &= CheckGlobalFBLocalRetainRec(pdm, pEntry->pType->pSym, bInFB);
            if (!bOK)
            {
                return bOK;
            }
        }
        
        if (bInFB && (pEntry->declMod & CG_DMOD_RETAIN)!=0)
        {
            bOK = false;
            pdm->msg1(CG_E_NOT_SUPPORTED, &(pEntry->edp), _T("RETAIN variables in global FunctionBlocks"));
            return bOK;
        }
    }

    return bOK;
}



bool CheckGlobalFBLocalRetain(    
    CGT_CDiagMsg*    pdm,
    const CG_Symtab* pSym 
)
{
    bool bOk = true;
    if(pSym->pouTyp==CG_POU_GVL)
    {
        for(int i=0; bOk && i<pSym->nGVLImp; i++)
            bOk = CheckGlobalFBLocalRetainRec(pdm, pSym->pGVLImp[i].pSym, false);
    }
    return bOk;
}

/* ---------------------------------------------------------------------------- */
