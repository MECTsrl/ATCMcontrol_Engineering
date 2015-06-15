/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmerr.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmerr.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmerr.h $
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

#ifndef _ASMERR_H_
#define _ASMERR_H_

#include <windows.h>
#include "ASMmessages.h"


//functions for HRESULTS
//errorCode is coded as defined by Message Compiler
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//   Sev:  00=Success, 01=Informational, 10=Warning, 11=Error
//   C=Customer code flag,   R=reserved bit
inline long GetFacilityCode(HRESULT l)         {return 0x0fff & (l>>16);}
inline long GetSeverity(HRESULT l)             {return 0x3 & (l>>30);}
inline long GetErrorCode(HRESULT l)            {return 0xffff & l;}
inline bool GetCustomerBit(HRESULT l)          {return (0x20000000 & l) != 0;}
inline long SetCustomerBit(HRESULT l, bool bit){return bit?l|0x20000000:l&~0x20000000;}
inline bool IsErrorSeverity(HRESULT l)         {return (0x3 & (l>>30)) == 3;}

#define ST_MAX_NUMBER_ERRORS 50
#define MODULE_USER_NAME  _T("4asm")




int ASM_GetTotalErrors();
int ASM_GetTotalWarnings();
void ASM_InitErrors();

struct ST_LineCol
{
    int nLine;
    int nColumn;
};


//define a location in a file with struct ST_EdiPos:
enum EDP_LOC_ID
{
    FDP_SUPRESS_DIAGNOSTICS,
    FDP_MODULE,
    FDP_CMD_LINE,
    EDP_FILE_ONLY,
    EDP_ST_LINE,
    EDP_4GR_BOX,  
    EDP_4GR_SIGNAL,
    EDP_4GR_END_POU,
    EDP_4GR_BEG_POU
};

class FileContext_C;

struct ST_EdiPos
{
    FileContext_C* pfc;
    EDP_LOC_ID     locid;
    int            x,y;
};

//return an edipos used for errors at command line level
const ST_EdiPos* ASM_CMDLEdiPos();
const ST_EdiPos* ASM_ModuleEdiPos();

inline void ST_InitEDP(ST_EdiPos* pEdp)
{   
    pEdp->pfc = NULL;
    pEdp->locid = EDP_ST_LINE;
    pEdp->x = pEdp->y = -1;
}
inline void ST_SetEDP(ST_EdiPos* pEdp, FileContext_C* pfc, EDP_LOC_ID locid, int x, int y)
{   
    pEdp->pfc = pfc;
    pEdp->locid = locid;
    pEdp->x = x;
    pEdp->y = y;
}
inline void ST_InitLineEDP(ST_EdiPos* pEdp, FileContext_C* pfc, ST_LineCol pos)
{
    pEdp->pfc = pfc;
    pEdp->locid=EDP_ST_LINE;
    pEdp->x = pos.nColumn;
    pEdp->y = pos.nLine;
}
inline void ST_InitLineEDP(ST_EdiPos* pEdp, FileContext_C* pfc, int line)
{
    pEdp->pfc = pfc;
    pEdp->locid=EDP_ST_LINE;
    pEdp->x = -1;
    pEdp->y = line;
}
inline void ST_InitFileEDP(ST_EdiPos* pEdp, FileContext_C* pfc)
{
    pEdp->pfc = pfc;
    pEdp->locid=EDP_FILE_ONLY;
    pEdp->x = pEdp->y = -1;
}

const int EDP_MAX_EDP_STR_LEN= 2*256+128; //maximum number of characters needed by
                                          //the string representaion of a ST_EdiPos struct.
const TCHAR* ST_EdiPosToString(const ST_EdiPos* pEdp, TCHAR buffer[EDP_MAX_EDP_STR_LEN]);




//Diagnostic dumping routines:
void DumpUserText(const TCHAR* pszText);


void ST_DiagnosticMsg(FileContext_C* pfc,
                      const TCHAR*   pszEdipos, 
                      HRESULT        err_id, 
                      const TCHAR**  pszArgArray);

void ST_DiagMsg(const ST_EdiPos* pEdp, 
                HRESULT          err_id, 
                const TCHAR**    pszArgArray);

void ST_DiagMsg0(const ST_EdiPos* pEdp, 
                 HRESULT          err_id);

void ST_DiagMsg1(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1);

void ST_DiagMsg2(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1,
                 const TCHAR*     psz2);

void ST_DiagMsg3(const ST_EdiPos* pEdp, 
                 HRESULT          err_id, 
                 const TCHAR*     psz1,
                 const TCHAR*     psz2,
                 const TCHAR*     psz3);

void ST_DiagMsg4(const ST_EdiPos* pEdp,
                 HRESULT          err_id, 
                 const TCHAR*     psz1,
                 const TCHAR*     psz2,
                 const TCHAR*     psz3,
                 const TCHAR*     psz4);

//ST_InfoMessage<N> are used to display a message like "compile files..."
//no edit position info is dumped:
void ST_InfoMessage0(HRESULT info_id);
void ST_InfoMessage1(HRESULT info_id, 
                     const TCHAR* psz1);
void ST_InfoMessage2(HRESULT info_id, 
                     const TCHAR* psz1, 
                     const TCHAR* psz2);
void ST_InfoMessage3(HRESULT info_id, 
                     const TCHAR* psz1, 
                     const TCHAR* psz2, 
                     const TCHAR* psz3);
void ST_InfoMessage4(HRESULT info_id, 
                     const TCHAR* psz1, 
                     const TCHAR* psz2, 
                     const TCHAR* psz3, 
                     const TCHAR* psz4);

#define ASM_INTERNAL_ERROR_STOP(pEdp) ASMInternalError(pEdp,  szversioninfo, __LINE__) 
void ASMInternalError(const ST_EdiPos* pEdp,  const TCHAR* pszSrcFile, int line);

#endif

/* ---------------------------------------------------------------------------- */
