#ifndef PARSERS_H_
#define PARSERS_H_

class ST_CBody;
class ST_CSymtabPrv;
class ST_CConfig;


ST_CSymtabPrv* ST_ParseAnyHeader(
    const CG_Edipos*     pEdp,
    CG_CIECFront*        pFront,
    const TCHAR*         pszFileLocator,
    const CG_LIBContext* pLibCtx
);


//------------------------------------------------------------------*
/**
 * ST_ParseSTIL4GR loads a complete ST, IL or 4gr file. If the pSymOld
 * is not NULL on entry the header section is skipped and 
 * only the body is loaded. In this case the file last modified
 * time must be exactly as stored in the old pSymOld (which must 
 * also have no errors).
 *
 * @param     pEdp           where to display file I/O errors 
 * @param     pFront         front end to use 
 * @param     pszFileLocator unique file locator 
 * @param     pLibCtx        if from a lib the lib contect
 * @param     ppBody         set to the body if any and no errors
 * @param     pSymOld        may be NULL if no old symtab.
 * @param     codeGenOpt     options bit set of CG_LOADBDY_* see ICG_IECFront::loadBody
 *
 * @return                   either new symtab if no hard errors like PPROGRAM
 *                           or if pSymOld!=NULL returns the value of pSymOld
 * @exception       -
 * @see             
*/
ST_CSymtabPrv* ST_ParseSTIL4GR(
    const CG_Edipos*     pEdp,
    CG_CIECFront*        pFront,
    const TCHAR*         pszFileLocator,
    const CG_LIBContext* pLibCtx,
    ST_CBody**           ppBody,
    ST_CSymtabPrv*       pSymOld,   
    long                 codeGenOpt
);

ST_CConfig* ST_ParseCON(
    const CG_Edipos* pEdp,
    CG_CIECFront*    pFront,
    const TCHAR*     pszFileAndRelPath
);

CG_Resource* ST_ParseRES(
    const CG_Edipos* pEdp,
    CG_CIECFront*    pFront,
    const TCHAR*     pszFileAndRelPath,
    ST_CSymtabPrv**  ppSymPrvNew
);

#endif


