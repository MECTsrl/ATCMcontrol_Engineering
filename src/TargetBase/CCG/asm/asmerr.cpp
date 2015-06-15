/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmerr.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmerr.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmerr.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
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

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmerr.cpp 1     28.02.07 19:04 Ln $";

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>

#include "fc_todebug\fc_assert.h"

#include "asmerr.h"
#include "globalContext.h"


static int nTotalErrors =0;
static int nTotalWarnings =0;
static ST_EdiPos ModuleErrLocation = {NULL, FDP_MODULE, -1, -1};
static ST_EdiPos CMDLErrLocation =   {NULL, FDP_CMD_LINE, -1, -1};


int ASM_GetTotalErrors(){return nTotalErrors;}
int ASM_GetTotalWarnings(){return nTotalWarnings;}
const ST_EdiPos* ASM_CMDLEdiPos(){return &CMDLErrLocation;}
const ST_EdiPos* ASM_ModuleEdiPos(){return &ModuleErrLocation;}

void ASM_InitErrors()
{
    //change mode of stdout to binary !
    int iRet = _setmode( _fileno( stdout ), _O_BINARY );   
    assert(iRet != -1 );

    nTotalErrors = nTotalWarnings =0;
}



inline void DumpUserText(const TCHAR* pszText)
{
    _fputts(pszText, stdout);
    fflush(stdout);
}


void ST_DiagnosticMsg(FileContext_C* pfc,
                      const TCHAR*  pszEdipos, 
                      HRESULT       err_id, 
                      const TCHAR** pszArgArray)
{
    TCHAR*  pBuf;    
    DWORD   dwRet;
    int     iSev;

    
    iSev = GetSeverity(err_id);
    //iSev is:
    //   00 - Success
    //   01 - Informational
    //   10 - Warning
    //   11 - Error
    if(pfc && iSev==3)//CAUTION: increment errors 
                      //in file even if ST_MAX_NUMBER_ERRORS
    {
        pfc->incrementErrorsInFile();
    }


    //no error/war msgs if too many errors but info msgs 
    //must still be displayed:
    if(nTotalErrors>=ST_MAX_NUMBER_ERRORS && iSev>=2)
        return;

    if(pszEdipos)
    {
        DumpUserText(pszEdipos);
        DumpUserText(_T(" : "));
    }

    dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
                          FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM|
                          FORMAT_MESSAGE_ARGUMENT_ARRAY, 
                          NULL, 
                          err_id, 
                          MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), //0, force english msgs... 
                          (LPTSTR)&pBuf, 
                          0, 
                          (TCHAR**)pszArgArray);


    if(dwRet==0)
    {
        assert(dwRet>0 && pBuf);
        if(pBuf==0 || dwRet==0)
        {
            pBuf = (TCHAR*)LocalLock(LocalAlloc(NONZEROLPTR, MAX_PATH));
            wsprintf(pBuf, MODULE_USER_NAME _T(" : internal error: no resource for error id 0x%x\n"),err_id );

        }

    }
    
    DumpUserText(pBuf);
    LocalFree(pBuf);

    if(iSev>=3)
    {
        nTotalErrors++;

        if(nTotalErrors==ST_MAX_NUMBER_ERRORS)
            ST_InfoMessage0(E_4CCG_ERR_TO_MANY_ERRORS);
    }
    else if(iSev==2)
    {
        nTotalWarnings++;
    }
}


const TCHAR* ST_EdiPosToString(const ST_EdiPos* pEdp, TCHAR buffer[EDP_MAX_EDP_STR_LEN])
{
    const TCHAR* pszFile;
    
    assert(pEdp);
    if(!pEdp)
        pEdp = &ModuleErrLocation;

    if(pEdp->pfc)
    {
        pszFile = pEdp->pfc->getErrMsgFileName();
    }
    else
    {
        assert(pEdp->locid==FDP_MODULE || pEdp->locid==FDP_CMD_LINE);
        pszFile = _T("<no error file?>");
    }

    switch(pEdp->locid)
    {
    case FDP_MODULE:   
        wsprintf(buffer, MODULE_USER_NAME);
    break;
    case FDP_CMD_LINE:   
        wsprintf(buffer, _T("command line"));
    break;
    case EDP_ST_LINE:   
        if(0 && pEdp->x>0)
            wsprintf(buffer, _T("%s(%d,%d)"), pszFile, pEdp->y, pEdp->x);
        else if(pEdp->y>0)
            wsprintf(buffer, _T("%s(%d)"), pszFile, pEdp->y);
        else 
            wsprintf(buffer, _T("%s"), pszFile, pEdp->y);
    break;
    case EDP_4GR_BOX:
        wsprintf(buffer, _T("%s(b,%d,%d)"), pszFile, pEdp->x, pEdp->y);
    break;
    case EDP_4GR_SIGNAL:
        wsprintf(buffer, _T("%s(s,%d,%d)"), pszFile, pEdp->x, pEdp->y);
    break;
    case EDP_4GR_END_POU:
        wsprintf(buffer, _T("%s(end)"), pszFile);
    break;
    case EDP_4GR_BEG_POU:
        wsprintf(buffer, _T("%s(begin)"), pszFile);
    break;
    case EDP_FILE_ONLY:
        wsprintf(buffer, _T("%s"), pszFile);
    break;
    default:
        assert(!"bad enum");
        wsprintf(buffer, _T("%s"), pszFile);
    }

    return buffer;
}




void ST_DiagMsg(const ST_EdiPos* pEdp, 
                HRESULT          err_id, 
                const TCHAR**    pszArgArray)
{
    TCHAR           buffer[EDP_MAX_EDP_STR_LEN];
    const TCHAR*    pszErrLoc = NULL;
    
    if(!pEdp)
        pEdp =&ModuleErrLocation;

    //special case: no diags, used to parse expression N-times
    if(pEdp->locid==FDP_SUPRESS_DIAGNOSTICS)
    {
        //assert: if there are errors on reparse, then the first parse must 
        //have displayd these errors:
        if(IsErrorSeverity(err_id) && pEdp->pfc && pEdp->pfc->getErrorsInFile()==0)
            ASM_INTERNAL_ERROR_STOP(NULL);
        else
            return;
    }
    
    pszErrLoc = ST_EdiPosToString(pEdp, buffer);

    ST_DiagnosticMsg(pEdp->pfc, pszErrLoc, err_id, pszArgArray);
}
void ST_DiagMsg0(const ST_EdiPos* pEdp, 
                 HRESULT          err_id)
{
    ST_DiagMsg(pEdp, err_id, NULL);
}
void ST_DiagMsg1(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1)
{
    const TCHAR* pszArgs[1] = {psz1};
    ST_DiagMsg(pEdp, err_id, pszArgs);
}
void ST_DiagMsg2(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1,
                 const TCHAR*     psz2)
{
    const TCHAR* pszArgs[2] = {psz1,psz2};
    ST_DiagMsg(pEdp, err_id, pszArgs);
}
void ST_DiagMsg3(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1,
                 const TCHAR*     psz2,
                 const TCHAR*     psz3)
{
    const TCHAR* pszArgs[3] = {psz1,psz2,psz3};
    ST_DiagMsg(pEdp, err_id, pszArgs);
}
void ST_DiagMsg4(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1,
                 const TCHAR*     psz2,
                 const TCHAR*     psz3,
                 const TCHAR*     psz4)
{
    const TCHAR* pszArgs[4] = {psz1,psz2,psz3,psz4};
    ST_DiagMsg(pEdp, err_id, pszArgs);
}

void ST_InfoMessage0(HRESULT info_id)
{
    ST_DiagnosticMsg(NULL, NULL, info_id, NULL);
}
void ST_InfoMessage1(HRESULT info_id, 
                     const TCHAR* psz1)
{
    const TCHAR* pszArgs[1] = {psz1};
    ST_DiagnosticMsg(NULL, NULL, info_id, pszArgs);
}
void ST_InfoMessage2(HRESULT info_id, 
                     const TCHAR* psz1, 
                     const TCHAR* psz2)
{
    const TCHAR* pszArgs[2] = {psz1,psz2};
    ST_DiagnosticMsg(NULL, NULL, info_id, pszArgs);
}
void ST_InfoMessage3(HRESULT info_id, 
                     const TCHAR* psz1, 
                     const TCHAR* psz2, 
                     const TCHAR* psz3)
{
    const TCHAR* pszArgs[3] = {psz1,psz2,psz3};
    ST_DiagnosticMsg(NULL, NULL, info_id, pszArgs);
}
void ST_InfoMessage4(HRESULT info_id, 
                     const TCHAR* psz1, 
                     const TCHAR* psz2, 
                     const TCHAR* psz3, 
                     const TCHAR* psz4)
{
    const TCHAR* pszArgs[4] = {psz1,psz2,psz3,psz4};
    ST_DiagnosticMsg(NULL, NULL, info_id, pszArgs);
}
void ASMInternalError(const ST_EdiPos* pEdp,  const TCHAR* pszSrcFile, int line)
{
    ST_DiagMsg2(pEdp, E_4CCG_ERR_INTERNAL, pszSrcFile, Tostr_C(line));
    assert(!"ASMInternalError");
    exit(1);
}

/* ---------------------------------------------------------------------------- */
