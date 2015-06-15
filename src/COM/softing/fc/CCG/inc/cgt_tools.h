#ifndef __CGT_TOOLS_H_
#define __CGT_TOOLS_H_



//----  Aggregate Includes:   --------------------------------------*
#include <stdio.h>
#include "CGT_CDiagMsg.h"
#include "fc_tools\fc_cgarray.h"


//----  Defines:    ------------------------------------------------*
//file extension of debug info files:
#define CGT_EXT_DBI _T("dbi")
#define CGT_SECURTY_FILE_NAME   _T("security.txt")


//----  Prototypes:    ---------------------------------------------*
class CGT_CDmp;


//------------------------------------------------------------------*
/**
 * CGT_CheckVarExternalsRec validate recursively a pSym's var external.
 * Loops all programs and all global FBs of the resource and recurses
 * into called/instantiated POUs and checks the var_externals and 
 * #import files against the resource gvl(s)
 * Note: This function calles ICG_IECFront::loadExtBodyInfo for all 
 * attracted POUs of the resource, read the performance note of
 * ICG_IECFront::loadExtBodyInfo
 *
 * TODO  LIBs ????? FUNs with var_external?
 * @param   pdm          for err msg
 * @param   pIF          front end IF
 * @param   pRes         calling resource
 * @param   ppzpAutoExt  if not NULL will be filled
 *                       with all VAR_EXTERNAL entries 
 *                       that have the (declMod & CG_DMOD_SYS_VAR)!=0
 *                       This is a 0-terminated array of pointers to that
 *                       sym entries.
 *
 *
 * @return          -1  if pRes->bNoError is false on entry
 *                  >=0 the number of link errors of the resource 
 * @exception       -
 * @see             ICG_IECFront
*/
extern "C" long CGT_CheckVarExternalsRec(
    CGT_CDiagMsg*        pdm,        //for err msg
    ICG_IECFront*        pIF,        //front end IF
    const CG_Resource*   pRes,       //calling resource
    const CG_SymEntry*** ppzpAutoExt //may be NULL if not needed
);




/**
 * CGT_EnumRefSymtabs  gets all symtabs (also functions) attracted 
 * by a resource.
 * If bodies are not loaded the function assert this and returns NULL.
 * Note: call this function also to detect recursive calls.
 *
 * @param   pdm      diag msg sink, mainly for recursive functions diag.
 * @param   pIF      front end interface
 * @param   pRes     the resource to query the referenced symtabs, 
 *                   should have no errors so far and bodies must
 *                   already loaded at least once.
 *
 * @return           NULL if any errors in header or bodies or if 
 *                   recursive function calls.
 *                   
 *                   terminated array of symtabs, caller must free 
 *                   the array with FC_LibFreeMemory.
 *
 * @see     CG_Symtab, CG_STAB_* defines in CG_IECFront.h
 */
extern "C" const CG_Symtab** CGT_EnumRefSymtabs(
    CGT_CDiagMsg*      pdm,
    ICG_IECFront*      pIF,
    const CG_Resource* pRes
);



/**
 * CGT_EnumRefSymtabsOfTask gets all symtabs (also functions) attracted 
 * by task.
 * If bodies are not loaded the function assert this and returns NULL.
 * Global Function blocks are only considered to be referenced by a 
 * task if they are used by any other prg,FB or fun via VAR_EXTERNAL.
 * Instantiated but not called FBs are treated as if called and hence
 * functions of non-called FBs are treated as referenced altough they
 * will not be really executed by the task
 * Recursive calls are detected by this function and an error is submitted.
 * 
 *
 * @param   pdm      diag msg sink, mainly for recursive functions diag.
 * @param   pIF      front end interface
 * @param   pTask    the task to query the referenced symtabs, 
 *                   should have no errors so far and bodies must
 *                   already loaded at least once.
 *
 * @return           NULL if any errors in header or bodies or if 
 *                   recursive function calls.
 *                   
 *                   terminated array of symtabs, caller must free 
 *                   the array with FC_LibFreeMemory.
 *
 * @see     CG_Symtab, CG_STAB_* defines in CG_IECFront.h
 */
extern "C" const CG_Symtab** CGT_EnumRefSymtabsOfTask(
    CGT_CDiagMsg*      pdm,
    ICG_IECFront*      pIF,
    const CG_Task*     pTask
);


//TODO comment
typedef struct _CGT_VarExtAccessInfo CGT_VarExtAccessInfo;

typedef struct _CGT_AccInfo
{
    const CG_SymEntry* pEntry;   //GVL entry of calling resource
                                 //or AutoDecl GVL entry (the first found with this name)
    //at least bRead or bWritten (or both) are set:
    int                bRead;    //pEntry  gets possibly read
    int                bWritten; //pEntry  gets possibly written
    
    //TODO:  if multi elem variable add somehow "member access"
} CGT_AccInfo;


CGT_VarExtAccessInfo* CGT_GetVarExtAccessInfo(
    CGT_CDiagMsg*      pdm,      //for err msg
    ICG_IECFront*      pIF,      //front end IF
    const CG_Resource* pRes,     //calling resource
    const CG_Task*     pTask     //calling task
);
void CGT_FreeVarExtAccessInfo(
    CGT_VarExtAccessInfo* pInfo
);

//Example how to get info:
//  CGT_AccInfo acc;
//  CGT_InitVarExtAccInfoList(pInfo);
//  while(CGT_GetNextVarExtAccInfo(pInfo, &acc))
//       do something with acc;
//  
//returns number of entries and initialises the iterator
long CGT_InitVarExtAccInfoList(
    CGT_VarExtAccessInfo* pInfo
);
//returns 1 if pAcc received new info
//        0 if end of list, then pAcc is unchanged
int CGT_GetNextVarExtAccInfo(
    CGT_VarExtAccessInfo* pInfo,
    CGT_AccInfo*          pAcc
);



void CGT_DumpVarExtAccessInfo(
    FILE*                 pFile, 
    CGT_VarExtAccessInfo* pInfo
);

const TCHAR* CGT_ExprAccFlagsToString(
    WORD accFlags, 
    TCHAR szBuffer[/*32*/]
);



#define CGT_DRL_SIMPL_ARRAY_EXPAND  0x0001
#define CGT_DRL_DATA_TYPE_FULL      0x0002
#define CGT_DRL_FULL_PATH           0x0004 


extern "C" bool CGT_DumpRetainList(
    CGT_CDiagMsg*    pdm,
    CGT_CDmp&        dmp,
    const CG_Symtab* pSym, 
    long             flags   //bitset of CGT_DRL_* 
);



//TODO comment
extern "C" void CGT_BuildInstanceTree(
    const CG_Symtab* pSym, 
    FC_CStrList*     pInstPathes
);
 
extern "C" bool CGT_DumpXREFFileList(
    CGT_CDiagMsg*          pdm,
    ICG_IECFront*          pIF,
    const CG_Resource*     pRes,
    const CG_Symtab*const* pRefSym
);

//------------------------------------------------------------------*
/**
 * CGT_ConsiderBP can be used to check a breakpoint makes sense at 
 * it's edipos, especially compared to the edipos of the last breakpoint
 * actually set.
 * 
 * Appended by back end, precompiled source and console stuff:
 * put;<full path>\ton.hdr
 * put;<full path>\classes\ton.class
 *
 * The header and body of the POU should already be loaded with
 * no errors. The function also places the header part of the POU 
 * into the gen\<target type>\.link directory.
 *
 * @param   pLast      NULL or edipos of last breakpoint point set in POU 
 * @param   pCurr      edipos of breakpoint to consider next
 *
 * @return          true if pCurr is a new and meaningfull breakpoint
 *                  false if e.g. pLast and pCurr point to the same ST-line.
 *                  If pLast is NULL the function always returns true, assuming
 *                  that pCurr is the first BP anyway.
 * @exception       -
*/
bool CGT_ConsiderBP(const CG_Edipos* pLast, const CG_Edipos* pCurr);





//------------------------------------------------------------------*
/**
 * CGT_DumpLinkFile create a new 
 *       gen\<target type>\.link\<pouname>.<ext>.link file.
 * Example: TODO example not up to date, re-document if libs are done...
 * Pure IEC interface lib info done by CGT_DumpLinkFile:
 * TODO
 * 
 * Appended by back end, precompiled source and console stuff:
 * put;<full path>\ton.hdr
 * put;<full path>\classes\ton.class
 *
 * The header and body of the POU should already be loaded with
 * no errors. The function also places the header part of the POU 
 * into the gen\<target type>\.link directory.
 *
 * @param   pdm         for err msg
 * @param   pIF         front end IF
 * @param   pSym        the POU's symtab, should be one of the 4cp [classes.*] 
 * @param   bSourceCode flag, true if the source code will be downloaded to library
 * @paramn  pszTrgType  the type of the target
 * @paramn  pszVersion  the binary code (or backend) version
 * @param   pszClass    a custom class or can be NULL use the IEC Pou type
 * @param   pszBackend  NULL or backend domain list to be checked into 
 *                      <POU name>\<target type> lib folder
 *                      can contain the command 'put' 
 *                      put;<full path>\<domain file>
 *                     
 *
 * @return          true if no error
 * @exception       -
*/
extern "C" bool CGT_DumpLinkFile(
    CGT_CDiagMsg*      pdm,   
    ICG_IECFront*      pIF,   
    const CG_Symtab*   pSym, 
    bool               bSourceCode,
    const TCHAR*       pszTrgType, 
    const TCHAR*       pszVersion, 
    const TCHAR*       pszClass,
    const TCHAR*       pszBackend
);



struct CGT_BP_TYP
{
    int       iBP;
    CG_Edipos edp;
};


//TODO comment
extern "C" bool CGT_DumpPOUDBIFile(
    CGT_CDiagMsg*        pdm,   
    ICG_IECFront*        pIF,   
    const CG_Symtab*     pSym,
    ICG_Body*            pIBdy,
    const CGT_BP_TYP*    pBPs,
    int                  nBPs,
	const TCHAR*         pszClsPath
);

//TODO comment
extern "C" bool CGT_DumpTRGDBIFile2(
    CGT_CDiagMsg*        pdm,   
    ICG_IECFront*        pIFront,   
    const CG_Resource*   pRes,
	const TCHAR*         pszTrgName,
	const TCHAR*         pszTrgTyp,
	const TCHAR*         pszTrgAddr,
	const TCHAR*         pszDbiPath,
    const bool           bDumpFunInst,
    const CG_SymEntry**  ppzpAutoExt, //can be NULL 
    const GUID*          pAutoExtObj,//can be NULL
    const TCHAR*         pszAutoExtIni//can be NULL
);


//------------------------------------------------------------------*
/**
 * CGT_InitI64LitExpr initialises a CG_ExprTree struct to be 
 * a literal for any __int64 value.
 * Can be used for e.g. CG_ExprTree structs created as temp. object
 * Note: the input pszI64 and pLit pointer life time.
 *
 * @param   pLitEx     tmp. expr. object to be initialised as literal.
 * @param   pLit       tmp. literal object to be initialised, pointer must
 *                     be valid as long the lit expr. will be used.
 * @param   pType      type of lit expression
 * @param   pEdp       edipos of the expression.
 * @param   pszI64     the lit text, pointer must be valid as long 
 *                     the lit expr. will be used.
 *
 * @return          the input pLitEx
 * @exception       -
*/
extern "C" __inline const CG_ExprTree* CGT_InitI64LitExpr(
    CG_ExprTree*     pLitEx, 
    CG_Literal*      pLit, 
    const CG_DType*  pType,
    const CG_Edipos* pEdp,
    const TCHAR*     pszI64
)
{
    pLit->type= pType->type;
    pLit->ut  = CG_LIT_I64;
    pLit->psz = pszI64;
    pLit->i64 = _atoi64(pszI64);

    pLitEx->extyp = CG_LITERAL;
    pLitEx->pType = pType;
    pLitEx->edp   = *pEdp;
    pLitEx->lit.pLit   = pLit;
    pLitEx->lit.pConst = NULL;

    return pLitEx;
}



//TODO comment
extern "C" typedef  bool (*CGT_OnNewExpr)(void* pCtx, const CG_ExprTree* pExpr);
extern "C" void CGT_EnumExprStmt(
    const CG_STStmt* pStmtRoot,
    void*            pCtx,
    CGT_OnNewExpr    callBack
);
extern "C" void CGT_EnumExpr(
    const CG_ExprTree* pExprRoot,
    void*              pCtx,
    CGT_OnNewExpr      callBack
);


union CGT_PathElem
{ 
    //if previous CGT_PathElem was of array type:
    long               nIndex;  //-1: if index is a non literal expr. [i], [i+2]
                                //>0: index is a literal expr. [9], [MY_CONST*2-1]

    //previous CGT_PathElem was of not any array type, first entry is never array:
    const CG_SymEntry* pEntry;  //The sym entry of the selector field.
};


/**
 * CGT_GetExprEntryPath can be used to split an expression of the 
 * form x.y[j*3].z[i].w[7] into field selector entries and array index values. 
 *
 * Usage Example:
 * nLen = CGT_GetExprEntryPath(pExpr, buff, 100, &pRoot)
 * for(i=0;i<nLen; i++)
 * {
 *    if(path[i]->pEntry->pType->bIsArray)
 *    {
 *          i++;  //next element in array of CGT_PathElem is the index.
 *          //use path[i].nIndex
 *    }
 * }
 *
 * @param pExpr       a CG_VARREF, CG_STRUCTREF or CG_ARRAYREF expression. TODO tmp var ?.
 * @param pPathBuffer buffer for selector expressions  
 * @param maxBuffer   size of buffer
 * @param pRoot       receives pointer to the position where the root field selector 
 *                    is placed in pPathBuffer (the buffer gets filled from the end).
 * 
 * return             >=0 the number selectors filled in pPathBuffer, 
 *                    <=0 any (fatal) error.
 *
 * @see  CGT_GetExprSymEntry
*/
extern "C" int CGT_GetExprEntryPath(
    const CG_ExprTree*  pExpr,
    CGT_PathElem*       pPathBuffer,
    int                 maxBuffer,
    CGT_PathElem**      ppRoot
);
void CGT_DumpEntryPath(
    FILE*                pFile,
    const TCHAR*         pszPre,
    int                  nPath,
    const CGT_PathElem*  pPath,
    const TCHAR*         pszPost
);


/**TODO: consider: better as a front end interface function ?
 * CGT_ParseAccessPath can be used to check a access string of the 
 * form x.y.0.a.
 *
 * @param pdm         error message sink
 * @param pEdp        edipos for diag messages
 * @param pIF         pointer to front end instance
 * @param pszPath     string buffer that contains the access path to check
 * @param pSym        local scope (may be NULL if only global is valid scope)
 * @param pRes        global scope (may be NULL if only local is v valid scope)
 * @param pbGlobal    OUT: TRUE if the first part of the access path is a global
 *                    variable. FALSE if it is a local variable
 * 
 * return             the datatype of the last field of the access path or NULL
 *                    if any error
*/
const CG_DType* CGT_ParseAccessPath(
    CGT_CDiagMsg*        pdm,     //for err msg
    const CG_Edipos*     pEdp,
    ICG_IECFront*        pIF, 
    TCHAR*               pszPath, //on successfull exit it contains a normalised path (white spaces removed)
    const CG_Symtab*     pSym,    //local scope
    const CG_Resource*   pRes,    //global scope
    bool*                pbGlobal
);







extern "C" typedef int (*CGT_VASTExprCallBack)(
    void*               pCtx,
    const CG_Edipos*    pEdp,
    WORD                accFlags,
    const CG_STStmt*    pFromStmt,
    const CG_ExprTree*  pFromExpr,
    int                 nPathLen,
    const CGT_PathElem* pPath,
    const CG_SymEntry*  pArgEntry
);
extern "C" int CGT_EnumVASTExprStmt(
    const CG_STStmt*     pStmtRoot,
    void*                pCtx,
    CGT_VASTExprCallBack callBack
);
extern "C" WORD CGT_GetVASTAccFlags(
    const CG_ExprTree* pVASTExpr
);





//------------------------------------------------------------------*
/**
 * CGT_DumpSTExpr can be used to dump a CG_ExprTree struct as ST code.
 * Tmp vars will be dumped in form of <pszTmpPrFix><num> where num 
 * is the tmp num.
 * The FC_CString dmp is not cleard on enter, all dump
 * will be appended to it.
 *
 * @param   pExpr       the lit to dump
 * @param   dmp         receives the dump result in append mode
 * @param   pszTmpPrFix used as prefix for tmp vars, if NULL use default _tmp
 *
*/
extern "C" void CGT_DumpSTExpr(
    const CG_ExprTree* pExpr,
    CGT_CDmp&          dmp,
    const TCHAR*       pszTmpPrFix
);

//------------------------------------------------------------------*
/**
 * CGT_DumpSTStmt can be used to dump a CG_STStmt struct as ST code.
 * Tmp vars will be dumped in form of <pszTmpPrFix><num> where num 
 * is the tmp num. The ; is appended but no terminating \n. 
 *
 * @param   pStmtRoot   the lit to dump
 * @param   dmp         receives the dump result in append mode
 * @param   pszTmpPrFix used as prefix for tmp vars, if NULL use default _tmp
 * @param   bRecurse    true: recurce into stmt tree, false only this one
 *                      if false and the stmt is e.g. CCG_CASE the output 
 *                      is like this "case aInt of"
 *
*/
extern "C" void CGT_DumpSTStmt(
    const CG_STStmt*   pStmtRoot,
    CGT_CDmp&          dmp,
    const TCHAR*       pszTmpPrFix,
    bool               bRecurse   
);



//------------------------------------------------------------------*
/**
 * CGT_GetExprSymEntry can be used to get the sym entry that defines
 * an expression of the form x.y.z[i].w. The sym entry of x will be 
 * returned. The returned sym entry is always a member of the POU's
 * symtab whee the pExpr belongs to.
 *
 * @param   pExpr       an expression, normally a CG_VARREF, CG_STRUCTREF
 *                      or CG_ARRAYREF
 * return               NULL or the defining sym entry
 *
*/
const CG_SymEntry* CGT_GetExprSymEntry(
    const CG_ExprTree* pExpr
);



//------------------------------------------------------------------*
/**
 * CGT_DumpSecurityFile create 
 *   gen\security.txt file containing security information.
 *
 * After function call add CGT_SECURTY_FILE_NAME to control's loader list
 *
 * @param   pdm        for err msg
 * @param   pIF        front end IF
 *
 * @return          true if no error
 * @exception       -
*/
extern "C" bool CGT_DumpSecurityFile(
    CGT_CDiagMsg*      pdm,   
    ICG_IECFront*      pIF   
);


#endif // __CGT_TOOLS_H_

