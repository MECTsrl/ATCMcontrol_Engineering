/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/lic/fc_slbase.h 4     25.02.08 12:54 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_slbase.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/lic/fc_slbase.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                LIC
 *
 * =COMPONENT           fc_slbase
 *
 * =CURRENT      $Date: 25.02.08 12:54 $
 *               $Revision: 4 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *   Wrapper for all functions (expect the "license creating" functions)
 *   form static library liccore.lib
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  10.10.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FC_SLBASE_H_
#define __FC_SLBASE_H_

#include "licerror.h"

//----  Defines:    ------------------------------------------------*
#ifndef FC_LICAPI
#define FC_LICAPI __declspec(dllimport)
#endif
#define FC_LICDECL _stdcall

//----  Prototypes:    ---------------------------------------------*

#ifdef __cplusplus
extern "C" {
#endif

//----  Defines:    ------------------------------------------------*
#define SLIC_RKEY_BYTES   10
#define SLIC_RKEY_CHARS   (5*4+4)

#define SLIC_MAX_COMP_CHARS 24
#define SLIC_MAX_IKEY_CHARS  (SLIC_MAX_COMP_CHARS+1+2*4+1)

#define SLIC_MIN_PRODNUM 0
#define SLIC_MAX_PRODNUM 4095

#define SLIC_MEDIA_ANY       0
#define SLIC_MEDIA_SOFT      1
#define SLIC_MEDIA_DONGLE    2
#define SLIC_MEDIA_INTERFACE 3


FC_LICAPI long FC_LICDECL SLIC_GetInstallationKey(
    char  pszInstKey[/*nBuffer*/],
    long  nBuffer
);


FC_LICAPI long FC_LICDECL SLIC_GetRegistrationKey(
    long  prodNum,
    char  pszRegKey[/*nBuffer*/],
    long  nBuffer
);


FC_LICAPI long FC_LICDECL SLIC_RegisterCompanyName(
    const char*  pszCompany 
);



FC_LICAPI long FC_LICDECL SLIC_GetCompanyFromReg(
    char  pszCompany[/*nBuffer*/],
    long  nBuffer //need SLIC_MAX_COMP_CHARS+1  
);





FC_LICAPI long FC_LICDECL SLIC_GetCompanyFromInstKey(
    const char*  pszInstKey,
    char         pszCompany[/*nBuffer*/],
    long         nBuffer   //SLIC_MAX_COMP_CHARS+1   
);



FC_LICAPI long FC_LICDECL  SLIC_CreateRegKey(
    long  prodNum,
    long  dwFeatureBits,
    long  nTmpDays,
    char  szNewRegKey[/*nBuffer*/],
    long  nBuffer //need SLIC_RKEY_CHARS+1   
);



FC_LICAPI long FC_LICDECL SLIC_CheckSoftLicKey(
    const char* pszKey,
    const char* pszInstKey,
    long*       pprodNum,
    long*       pdwFeatureBits,
    long*       pnTmpDays
);




FC_LICAPI long FC_LICDECL SLIC_RegisterSoftLicKey(
    const char* pszRegKeys
);



long FC_LICDECL RegisterSoftLicKeyThreadProc(void *pParam);


long FC_LICDECL RegisterSingleSoftLicKey(const char* pszKey);


FC_LICAPI long FC_LICDECL SLIC_RegisterSoftLicKeyCompleted(
    long* plRetCode
);



FC_LICAPI long FC_LICDECL SLIC_RefreshHardLock();


FC_LICAPI long FC_LICDECL SLIC_GetHardLockLastErrno(
    long* plHLRet
);

FC_LICAPI long FC_LICDECL SLIC_GetHardLockErrorText(
    long    lHLRet,
    char    pszBuffer[/*nBuffer*/],
    long    nBuffer
);


FC_LICAPI long FC_LICDECL  SLIC_GetLicenseForProduct(
    long  prodNum,
    long* pdwFeatureBits,
    long* pnHoursLeft,
    long* pLicMedia
);


typedef  long  (FC_LICDECL*  SLIC_GetLicenseForProductProc)
(
    long  prodNum,
    long* pdwFeatureBits,
    long* pnHoursLeft,
    long* pLicMedia
);

FC_LICAPI long FC_LICDECL SLIC_GetVersionInfo
(
    long* plMajor, 
    long* plMinor, 
    long* plBuildNr,
    char* pszFullVersStr,
    long  nBufferVer,
    char* pszCopyRight,
    long  nBufferCpr
);


typedef  long/*==HRESULT*/ (FC_LICDECL* SLIC_GetVersionFromDataFileTyp)
(
    const char* pszFile,
    char        szVersion[/*nBuffer*/],
    long        nBuffer
);
#define SLIC_GET_FILEVER_INFO_PROC_HDR STDAPI SLIC_GetVersionFromDataFile(const char* pszFile, char szVersion[/*nBuffer*/], long nBuffer)
#define SLIC_GET_FILEVER_INFO_PROC "SLIC_GetVersionFromDataFile"

FC_LICAPI long FC_LICDECL SLIC_GetVersionFromProductFile
( 
    const char* pszProductFile,
    const char* pszDLL,
    char*       pszVersion,
    long        nBufferVer
);



FC_LICAPI long FC_LICDECL SLIC_BitAnd(
    long  dw1,
    long  dw2
);
FC_LICAPI long FC_LICDECL SLIC_BitOr(
    long  dw1,
    long  dw2
);
FC_LICAPI long FC_LICDECL SLIC_BitNot(
    long  dw1
);

FC_LICAPI long FC_LICDECL SLIC_ShiftLeft(
    long  dw,
    long  nBits //>=0
);
//>> not sign extended (as unsigned)
FC_LICAPI long FC_LICDECL SLIC_ShiftRight(
    long  dw,
    long  nBits //>=0
);
FC_LICAPI long FC_LICDECL SLIC_SetBit(
    long  dw,
    long  nBitNum 
);
FC_LICAPI long FC_LICDECL SLIC_TestBit(
    long  dw,
    long  nBitNum //0..31
);

FC_LICAPI long FC_LICDECL SLIC_strtoul(
    const char* pszUl, 
    long        radix, 
    long*       pulResult
);

FC_LICAPI long FC_LICDECL SLIC_ultoa(
    long        aUl,
    long        radix,
    char*       pszBase2long,
    long        nBuffer //>=33
);


FC_LICAPI long FC_LICDECL SLIC_DecodeFile(const char *lpszFileIn, const char *lpszFileOut, char *lpszErrTxt, int iBufLen);



#ifdef __cplusplus
}
#endif


#endif // __FC_SLBASE_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_slbase.h $
 * 
 * *****************  Version 4  *****************
 * User: Ln           Date: 25.02.08   Time: 12:54
 * Updated in $/4CReleased/V2.20.00/COM/softing/fc/inc/lic
 * 
 * *****************  Version 5  *****************
 * User: Ln           Date: 25.02.08   Time: 12:54
 * Updated in $/4Control/COM/softing/fc/inc/lic
 * 
 * *****************  Version 4  *****************
 * User: Ln           Date: 12.02.08   Time: 19:39
 * Updated in $/4Control/COM/softing/fc/inc/lic
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 5.12.01    Time: 18:09
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * updated comments
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 15.11.01   Time: 22:25
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * more dongle
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 14.11.01   Time: 14:36
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * more dongle
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 13.11.01   Time: 12:09
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 7.11.01    Time: 21:03
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * SoftingLM: Version info for 4cl files
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 30.10.01   Time: 10:54
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * added typedef for DLL Proc  
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 24.10.01   Time: 23:38
 * Updated in $/4Control/COM/softing/fc/inc/slic
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 18.10.01   Time: 20:46
 * Created in $/4Control/COM/softing/fc/inc/slic
 *==
 *----------------------------------------------------------------------------*
*/
