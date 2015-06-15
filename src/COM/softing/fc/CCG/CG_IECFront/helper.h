
#ifndef __HELPER_H_
#define __HELPER_H_

#include <tchar.h>
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_cgarray.h"
#include "defines.h"
#include "resource.h"
#include "CG_IECFront.h"  //COM front end interface
#include "CG_CIECFront.h" //Implementation class of COM front end interface
#include "typeset.h"
#include "cgt_xmlparse.h"



const TCHAR* GetModulePath();//defined in COMoverhead.cpp
HINSTANCE    GetHInstance();//defined in COMoverhead.cpp




//generall helper functions:
bool ST_CheckIdentifier(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp, 
    const TCHAR*     pszIdToCheck
);
bool ST_LoadTextFileToMem(
    CGT_CDiagMsg*    pdm,          //diag msg sink
    const TCHAR*     pszPrjPath,   //used if file has rel path, can be NULL for cwd
    const CG_Edipos* pEdp,         //NULL or a edipos for error msg 'file not found...'
    const TCHAR*     pszFileName,  //project relative or abolte file name and path
    TCHAR**          ppCharBuffer, //newly allocated buffer for file content 
    unsigned long*   pulLength,    //NULL or receives the size in TCHARSs of
                                   //*ppCharBuffer without the always
                                   //added terminating 0-TCHAR.
    __int64*         pftLastWrite  //NULL or receives the last file write time
);




FC_CStr ST_TypeSetToErrMsgString(const ST_TypeSet_C& type);
const TCHAR* ST_CG_DTYPES_ToString(CG_DTYPES dt);
CG_DTYPES ST_TypeKeywordToCG_DTYPES(const TCHAR* pszTypeKW);







//ST_ExprTypToString used for error messages
const TCHAR* ST_ExprTypToString(CG_EXPRTYP etyp);
//IL_ExprTypToString used for error messages
const TCHAR* IL_ExprTypToString(CG_EXPRTYP etyp);

const TCHAR* ST_StoreClassName(CG_DECLSCOPE s);
const TCHAR* ST_PouTypeName(CG_POUTYP pt, bool bEndMaker=false);

//CG_WANT_* bit set to be used in ST_CheckPOUType 
#define ST_WANT_ANY  0xff    
#define ST_WANT_SDT  0x01 
#define ST_WANT_FUN  0x02
#define ST_WANT_FB   0x04
#define ST_WANT_PRG  0x08

bool ST_CheckPOUType(
    CGT_CDiagMsg*        pdm, 
    const CG_Edipos*     pEdp, 
    const TCHAR*         pszPouName,
    CG_POUTYP            pouTypFound,
    long                 want
);








bool ST_CreateIECMangeledName(
    const CG_Symtab* pSym, 
    FC_CString&      mgName
);


TCHAR* ST_GetIECHeaderText(
    CGT_CDiagMsg*    pdm, 
    CG_CIECFront*    pFront,
    const CG_Symtab* pSym
);

bool ST_Get4GRHeaderAndBody(
    CGT_CDiagMsg*    pdm, 
    const CG_Edipos* pEdp,
    CG_CIECFront*    pFront,
    const TCHAR*     pszLocator, 
    TCHAR**          ppszHeader,
    IXMLDOMNode**    ppXMLBody,
    unsigned long*   pulLength,
    __int64*         pftLastWrite
);



void ST_OnDeepCopy(
    CG_CIECFront*    pFront,
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const CG_Symtab* pSym
);












#endif // __HELPER_H_
