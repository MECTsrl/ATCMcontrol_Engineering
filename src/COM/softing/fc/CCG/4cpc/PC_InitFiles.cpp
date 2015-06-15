

//----  Includes:   -------------------------------------------*
#include <ctype.h>
#include <stdlib.h>

#include "defines.h"
#include "fc_tools\fc_fileIO.h"
#include "PC_InitFiles.h"
#include "globctx.h"
#include "parse4CH.h"
#include "cgt_address.h"
#include "cgt_misc.h"
#include "exprcode.h"
#include "helper.h"
#include "baselib.h"


//local types:
struct ST_INIT_LINE_TYP
{
    const TCHAR* pszId;
    const TCHAR* pszClass;
    const TCHAR* pszDeclId;
    const TCHAR* pszInitString;
    const TCHAR* pszInitFile;
};

//local functions:
static bool ST_ScanInitLine(
    CGT_CSVFile&      csvRead, 
    ST_INIT_LINE_TYP* pInitLine
);
static void ST_DumpDeclId(
    CGlobCtx*             pgctx,
    const CG_SymEntry*    pEntry,  
    CGT_CDmp&              dmp, 
    const CG_Initialiser* pInstanceInit,
    const TCHAR*          pszAddr
);
void ST_DumpSimpleInitValue(
    CGT_CDiagMsg*         pdm,
    const CG_Edipos*     pEdp,
    long                 lStrLen,
    const CG_Literal*    pLit, 
    CGT_CDmp&             dmp, 
    const TCHAR*         pszAddr
);
static bool  ST_GenerateInstanceInitFiles
(
    CGlobCtx*              pgctx,
    CTarget*               pTrg,
    FC_CString&            domainFile,
    const CG_Edipos*       pEdp,
    const TCHAR*           pszVariable,
    const TCHAR*           pszOuterInitFile, 
    const CG_StructInit*   pInit, 
    const TCHAR*           pszAddr,
    const TCHAR*           pszPreviousIFileName,
    const CG_Symtab*       pTypeDescr
);
static bool ST_GenerateArrayInitFiles
(
    CGlobCtx*             pgctx,
    CTarget*              pTrg,
    FC_CString&           domainFile,
    const CG_Edipos*      pEdp,
    const TCHAR*          pszVariable,
    const TCHAR*          pszOuterInitFile, 
    const CG_ArrayInit*   pInit, 
    const TCHAR*          pszAddr,
    const TCHAR*          pszPreviousIFileName,
    const CG_DType*       pType
);
static bool ST_GenerateArrayInitFiles2
(
    CGlobCtx*               pgctx,
    CTarget*                pTrg,
    const CG_Edipos*        pEdp,
    const CG_ArrayInit*     pInit, 
    CGT_CDmp&                dmp,
    const TCHAR*            pszInitFileName,
    long*                   pnInitDone,
    ST_INIT_LINE_TYP*       pInitLine,
    CGT_CSVFile*            pPreviousIFile,
    const TCHAR*            pszAddr,
    const CG_DType*         pType
);








static void ST_DumpDeclId(
    CGlobCtx*             pgctx,
    const CG_SymEntry*    pEntry,  
    CGT_CDmp&              dmp, 
    const CG_Initialiser* pInstanceInit,
    const TCHAR*          pszAddr
)
{
    const CG_KeyValPair*   pAttr;
    const CG_DType*        pType     = pEntry->pType;
    CG_DTYPES              typ       = pType->type;


    //decl id if user defined declid or struct or direct var:
    pAttr = pgctx->m_pIF->lookUpAttrib(pEntry->pAttribs, PC_ATTR_DECLID);

    if(pAttr)
    {
        if(!_tcsicmp(pAttr->pszVal, _T("<initvalue>")))
        {
            //use init values (only for simple type) if any:
            if(pInstanceInit && pInstanceInit->ut==CG_INIT_SIMPLE)
            {
                ST_DumpSimpleInitValue(
                    &pgctx->m_dm, 
                    &pEntry->edp,
                    pEntry->pType->lStrLen,
                    pInstanceInit->sim.pLit, 
                    dmp, 
                    pszAddr
                );
                return;
            }
        }
        else
        {
            //for power users pszDeclId as it is:
            //but note: if the declid field starts with a % 
            //the %indirect address pass will try to replace it, therfore
            //insert a blank before pszDeclId.
            if(pAttr->pszVal[0]==_T('%'))
            {
                bool bEscape= CGT_CDmpCSVNeedEscape(pAttr->pszVal, -1);
                if(bEscape)
                    dmp<<_T('"');
                
                dmp<<_T(' ');
                dmp.dumpAsCSV(pAttr->pszVal, -1, false);
                if(bEscape)
                    dmp<<_T('"');
            }
            else
            {
                dmp.dumpAsCSV(pAttr->pszVal, -1, true);
            }
            return;
        }

    }
    
    //array dimensions:
    if(pType->bIsArray)
    {
        for(int i=0;i<pType->nDim;i++)
        {
            if(i>0)
                dmp<<_T(",");
            dmp<<pType->range[i].lRangeStart<<_T("..");
            dmp<<(long)(pType->range[i].lRangeStart + pType->range[i].ulRangeLength);
        }
    }
    else if(pType->pSym)
    {
        dmp.lo(pType->pSym->pszId);

        //if version attr use it also as decl id (replaces former GUID in java class name):
        pAttr = pgctx->m_pIF->lookUpAttrib(pEntry->pAttribs, CG_ATTR_VERSION);
        if(pAttr)
            dmp<<_T(" ")<<pAttr->pszVal;
    }
    else if((typ==CG_WSTRING||typ==CG_STRING) && pType->lStrLen>=0)
    {
        assert(!pszAddr);
        dmp<<pType->lStrLen;
    }
    else if(pszAddr)
    {
        dmp.dumpAsCSV(pszAddr, -1, true);
    }
}




static void ST_DumpSimpleInitValue(
    CGT_CDiagMsg*         pdm,
    const CG_Edipos*     pEdp,
    long                 lStrLen,
    const CG_Literal*    pLit, 
    CGT_CDmp&             dmp, 
    const TCHAR*         pszAddr
)
{
    bool bMustEscapeString = false;
    bool bMustEscapeAddr   = false;
    

    if(pszAddr)
    {
        bMustEscapeAddr = CGT_CDmpCSVNeedEscape(pszAddr, -1);
        if(bMustEscapeAddr)
            dmp<<_T('"');

        dmp.dumpAsCSV(pszAddr,-1,false);
        dmp<<_T('|');
    }

    switch(pLit->type)
    {
    case CG_BOOL:
        assert(pLit->ut == CG_LIT_I64);
        if(pLit->i64==0)
        {
            dmp<< _T("false");
        }
        else
        {
             assert(pLit->i64==1);
             dmp<< _T("true");
        }
    break;
    case CG_WSTRING:
	{
        assert(pLit->ut==CG_LIT_NONE);
        
		long   lBufferLen = _tcslen(pLit->psz);
		TCHAR* pBinBuffer = new TCHAR[lBufferLen];
		
		HRESULT hr = BL_IECWStringToBinary(pLit->psz, pBinBuffer, &lBufferLen, NULL, NULL);
		assert(hr == S_OK);

        if(!bMustEscapeAddr)
        {
            bMustEscapeString = CGT_CDmpCSVNeedEscape(pBinBuffer, lBufferLen);
            if(bMustEscapeString)
               dmp<<_T('"');
        }

        if(lStrLen>=0)
            dmp<<lStrLen<<_T('|');

        dmp.dumpAsCSV(pBinBuffer, lBufferLen, false);
		delete [] pBinBuffer;
    }    
    break;
    case CG_LREAL:
        assert(pLit->ut == CG_LIT_FLT);
        dmp<<pLit->psz;
    break;
    case CG_DT:
    case CG_TIME:
    case CG_LINT:
    case CG_LWORD:
        assert(pLit->ut == CG_LIT_I64);
        dmp<<pLit->i64;
    break;
    case CG_BYTE:
    case CG_WORD:
    case CG_DWORD:
    case CG_DINT:
    case CG_TOD:
    case CG_DATE:
        assert(pLit->ut == CG_LIT_I64);
        assert(pLit->i64 >= (-2147483647-1) && pLit->i64 <= 4294967295);
        dmp<<(long)pLit->i64;
    break;
    default:
        CGT_IERR2(pdm, pEdp, _T("unsupported data type after passed front"));
    }

    if(bMustEscapeAddr || bMustEscapeString)
        dmp<<_T('"');
}




static bool ST_ScanInitLine(
    CGT_CSVFile&      csvRead, 
    ST_INIT_LINE_TYP* pInitLine
)
{
    
    if(!csvRead.getNextField(&pInitLine->pszId))
        return false;
    if(!csvRead.getNextField(&pInitLine->pszClass))
        return false;
    if(!csvRead.getNextField(&pInitLine->pszDeclId))
        return false;
    if(!csvRead.getNextField(&pInitLine->pszInitString))
        return false;
    if(!csvRead.getNextField(&pInitLine->pszInitFile))
        return false;
    if(!csvRead.isLastFieldEOL())
        return false;
    
    return true;
}

static bool ST_MergeIncompleteAddress(const TCHAR* pszDef, const TCHAR* pszRef, FC_CGArray<TCHAR>* pSum)
{
    const TCHAR* p  = pszRef;
    const TCHAR* p2 = pszDef;
    const TCHAR* pLastDigit;
    int          indexOfLastField;
    long         lastField;
    long         firstField;
    TCHAR        buffer[64];
    int          i;

    assert(*p==_T('%'));
    while(*p && *p!=_T('*')){pSum->add(*p); p = _tcsinc(p);}
    assert(*p==_T('*'));
    p = _tcsinc(p);
   
    //add p2=%10.3 at position of *:
    assert(*p2==_T('%'));
    p2 = _tcsinc(p2);
    
    //assert pszDef may be %10,  %10.3 or %*.6 or %*.4 but not %* and also no !modifier:
    //NOTE: may change if VAR_CONFIG will be implemented
    assert(_istdigit(*p2) || (*p2==_T('*') && (p2[1]==_T('.') || p2[1]==_T('+'))));
    
    pLastDigit = NULL;
    indexOfLastField = -1;
    while(*p2)
    {
        if(_istdigit(*p2) && !pLastDigit)
        {
            pLastDigit = p2;
            indexOfLastField = pSum->getUsed();
        }
        else if(*p2==_T('.'))
        {
            pLastDigit = NULL;
            indexOfLastField = -1;
        }
        pSum->add(*p2); 
        p2 = _tcsinc(p2);
    }

    //add rest of p=(.|+)10.3  or trailing !modifier:
    if(*p==_T('.') || *p==_T('!'))
    {
        while(*p)
        {
            pSum->add(*p); 
            p = _tcsinc(p); 
        }
        //null term:
        pSum->add(0);
        return true;
    }
    
    
    if(*p==_T('+') && pLastDigit && indexOfLastField>=0)
    {
        lastField = -1;
        lastField = atol(pLastDigit);
        assert(lastField>=0);
        if(lastField<0)
            return false;

        //skip the '+'
        p = _tcsinc(p);

        //read text(=>must be a number!) up to '.' 
        //or '!' or end of pszRef and store it in buffer: 
        buffer[0] =0;
        assert(*p);
        if(!*p)
            return false;

        for(i=0; *p; )
        {
            if(*p==_T('.') || *p==_T('!'))
                break;

            if(i<FC_ARRAY_LEN(buffer)-1)
            {
                buffer[i++] = *p; 
                buffer[i] = 0;
            }
            else
            {
                //may habben if %*+1000000000000000000000
                //this is currently not checked but 999999999 will
                //fail in address check anyway:
                return false;
            }

            p = _tcsinc(p); 
        }

        firstField = atol(buffer);
        assert(firstField>=0);
        
        firstField += lastField;
        _ltot(firstField, buffer, 10);
        
        assert(_istdigit(pSum->get(indexOfLastField)));
        pSum->setUsed(indexOfLastField);
        
        for(i=0; buffer[i] && i<FC_ARRAY_LEN(buffer); i++)
            pSum->add(buffer[i]);

        //append rest if any, rest must start with '.' or '!'
        assert(!*p || *p==_T('.') || *p==_T('!'));
        while(*p)
        {
            pSum->add(*p); 
            p = _tcsinc(p); 
        }
    
        //null term:
        pSum->add(0);
        return true;
    }

    assert(!"bad incomplete syntax: * not followed by +|.");
    return false;

}

static bool ST_IncArrayElementDirectAddr(
    TCHAR        buffer[], 
    int          bufLen, 
    const TCHAR* pszAddr, 
    long         inc
)
{    
    //increment pszAddr by elem num:
    for(int iLen=_tcslen(pszAddr)-1;iLen>0 && _istdigit(pszAddr[iLen]); iLen--)
        ;
    
    if(iLen<=0 || iLen>bufLen-50)
        false;

    for(int jj=0;jj<=iLen; jj++)
        buffer[jj] = pszAddr[jj];

    long lNewOffs = _ttol(pszAddr+iLen+1) + inc;
    _ltot(lNewOffs, buffer+iLen+1, 10);

    return true;
}


static void ST_DumpInitFile(
    CGlobCtx*        pgctx,
    CTarget*         pTrg,
    const TCHAR*     pszAddr,
    const CG_Symtab* pSym, 
    const TCHAR*     pszGenRelInitFile
)
{
    const CG_SymEntry*  pEntry;
    CGT_CDmp             dmp;
    
    for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
    {
        switch(pEntry->declScope)
        {
            case CG_VAR:
            case CG_VAR_INPUT:
            case CG_VAR_OUTPUT:
            case CG_VAR_GLOBAL:
                ST_DumpInitFileEntry(pgctx, pTrg, pszGenRelInitFile, 
                    pszAddr, pEntry, dmp);
        }
    }


    //SFC special variables:
    if(pSym->flags & CG_STAB_BDY_IS_SFC)
    {
        dmp<<_T("__sfcvis;system.StringVar;;;\n")
             _T("__retain;system.StringVar;;;\n")
             _T("__manualmode;system.BoolVar;;;\n")
             _T("__donext;system.BoolVar;;;\n")
             _T("__onceforcedtransitions;system.StringVar;;;\n")
             _T("__forcedtransitions;system.StringVar;;;\n")
             _T("__blockedtransitions;system.StringVar;;;\n")
             _T("__alltransitionson;system.BoolVar;;;\n")
             _T("__onceforcedactions;system.StringVar;;;\n")
             _T("__forcedactions;system.StringVar;;;\n")
             _T("__blockedactions;system.StringVar;;;\n")
             _T("__allactionsoff;system.BoolVar;;;\n")
             _T("__mancmd;system.StringVar;;;\n");
    }

    //Dump to disk:
    CGT_DmpToFile(&pgctx->m_dm, pgctx->getGenPath(), pszGenRelInitFile, dmp, false);

}



static void ST_InsertInstanceName(
    FC_CString&   outerInit,
    const TCHAR*  pszId
)
{
    outerInit.changeFileExt(NULL).addc('@').appendLo(pszId)<<_T(".")PC_INIT_EXT;
}




//wegschmeisen !
static bool  ST_GenerateInstanceInitFiles
(
    CGlobCtx*              pgctx,
    CTarget*               pTrg,
    FC_CString&            domainFile, //output the gen rel file
    const CG_Edipos*       pEdp,
    const TCHAR*           pszVariable,
    const TCHAR*           pszOuterInitFile, 
    const CG_StructInit*   pInit, 
    const TCHAR*           pszAddr,
    const TCHAR*           pszPreviousIFileName,
    const CG_Symtab*       pTypeDescr
)
{
    bool                  bRet;
    const CG_SymEntry*    pEntry;
    const CG_Initialiser* pMemberInit;
    long                  lInit;
    CGT_CDiagMsg*         pdm;
    CGT_CSVFile           previousIFile;
    long                  lInitDone=0;
    ST_INIT_LINE_TYP      initLine;
    FC_CString            previousIFileName;
    FC_CString            helpFname;
    const TCHAR*          pszGenPath;

    pszGenPath = pgctx->getGenPath();
    pdm        = &pgctx->m_dm;

    //open previous init file with CSV reader:
    //create file name for previous init file, used as template:

    if(pszAddr && (pTypeDescr->flags & CG_STAB_HAS_INCADDR)==0)
    {
        //struct at %, but no incomplete struct:
        pdm->errNS1(pEdp, _T("struct/FB AT direct address (only incomplete address syntax allowed)"));
    }

    if(pszAddr || (!pTrg->isIOEnabled() && (pTypeDescr->flags & CG_STAB_HAS_ADDR)))
    {
        //create file name for new init file to dump
        previousIFileName.load(pszOuterInitFile);
        ST_InsertInstanceName(previousIFileName, pszVariable);
    }
    else
    {
        //assert: if pszAddr==NULL call this function only to create
        //an POU type init file in gen\.4cpc\classes
        assert((pszPreviousIFileName && pInit) || 
               !(pTypeDescr->flags & CG_STAB_HAS_INCADDR));
        
        previousIFileName.load(pTrg->getTrgName());
        //previousIFileName.load(PC_GEN_CLASSES_DIR);
    
        if(!pszPreviousIFileName)
            pszPreviousIFileName = pTypeDescr->pszId;

        previousIFileName.appendDir(pszPreviousIFileName);
        previousIFileName.changeFileExt(PC_INIT_EXT);
        previousIFileName.toLower();
    }
    
    if(!FC_FileExists(pszGenPath, previousIFileName))
        ST_DumpInitFile(pgctx, pTrg, pszAddr, pTypeDescr, previousIFileName);

    
    if(!pInit)
    {
        //nothing todo...
        domainFile.load(previousIFileName);
        return true;
    }


    if(!CGT_LoadCSVFile(pdm, &previousIFile, 
         pgctx->getGenPath(), previousIFileName)
       )
        return false;
    
    
    //create file name for new init file to dump
    domainFile.load(pszOuterInitFile);
    ST_InsertInstanceName(domainFile, pszVariable);
    
    //open output stream this new init file
    CGT_CDmp  dmp; 
    //printf("dump : %s\n",(const TCHAR*)domainFile);

    
    
    while(!previousIFile.isEOF())
    {
        if(!ST_ScanInitLine(previousIFile, &initLine))
        {
            pdm->intErr(previousIFileName, previousIFile.getLineno(),
                _T("syntax error in init file"));
            return false;
        }

        //get the symentry of the member that must be initialized
        //either there are initial values for that member or 
        //the member has an incomplete direct address (or both):
        pEntry = NULL;
        pMemberInit = NULL;
        for(lInit=0; lInit<pInit->nInits; lInit++)
        {
            pEntry = pInit->pEntries[lInit];
            if(!_tcsicmp(initLine.pszId, pEntry->pszId))
            {
                lInitDone++;//<-only for assertion at end of this function
                //instance init:
                pMemberInit = &pInit->pInits[lInit];

                assert(pEntry==pgctx->m_pIF->lookUpEntry(pTypeDescr, initLine.pszId));
                break;
            }
            pEntry = NULL;
        }

        //dump <id>;<class>;<declid>; 
        //<init string>;<init file> will be added in switch:
        dmp<<initLine.pszId<<_T(';');
        dmp<<initLine.pszClass<<_T(';');
        dmp.dumpAsCSV(initLine.pszDeclId,-1,true)<<_T(';');
        
        if(!pMemberInit)
        {
            dmp.dumpAsCSV(initLine.pszInitString,-1,true)<<_T(';');
            dmp<<initLine.pszInitFile<<_T('\n');
            continue;
        }

        if(!pEntry)
            CGT_IERR2(pdm, pEdp, _T("invalid struct init"));

        if(pEntry->pType->bIsArray)
        {
            assert(pMemberInit->ut==CG_INIT_ARRAY);
            bRet = ST_GenerateArrayInitFiles(
                        pgctx,
                        pTrg,
                        helpFname,
                        pEdp,
                        initLine.pszId,
                        domainFile, 
                        &pMemberInit->arr, 
                        NULL,
                        initLine.pszInitFile,
                        pEntry->pType
                   );
            if(!bRet)
                return false;
            
            //<init string>;<init file>
            dmp<<_T(';');
            dmp<<FC_StringGetFilePart(helpFname)<<_T('\n');
            
            //append new domain name to dom list:
            pTrg->addDomain(helpFname);
        }
        else if(pEntry->pType->pSym)
        {
            //ok, do the instance init
            assert(pMemberInit->ut==CG_INIT_STRUCT);
            bRet = ST_GenerateInstanceInitFiles(
                       pgctx,
                       pTrg,
                       helpFname,
                       pEdp,
                       initLine.pszId,
                       domainFile, 
                       &pMemberInit->str,
                       pszAddr,//d-2170, this is not a safe solution, this module must be redone! 
                       initLine.pszInitFile,
                       pEntry->pType->pSym
                   );
            if(!bRet)
                return false;
            
            //<init string>;<init file>
            dmp<<_T(';');
            dmp<<FC_StringGetFilePart(helpFname)<<_T('\n');
            
            //append new domain name to dom list:
            pTrg->addDomain(helpFname);
        }
        else
        {
            //any simple
            //HACK ??????????????????????????????????????????????
            const TCHAR* pszAddrOld = NULL;
            TCHAR buffer[512];

            if(pEntry->pszAddr && pTrg->isIOEnabled())
            {
                //assert: init line must not be an incomplete address any more
                if(CGT_IsIncompleteAddressRef(initLine.pszInitString))
                    CGT_IERR2(pdm, pEdp, _T("invalid %-address"));

                //HACK ??????????????????????????????????????????????
                pszAddrOld = initLine.pszInitString;
                const TCHAR* pszEnd;
                CGT_CheckDirectAddrSyntax(pszAddrOld, &pszEnd, NULL);
                if(*pszEnd == _T('|'))
                {
                    for(int i=0; i<500 && pszAddrOld+i != pszEnd;)
                        buffer[i++] = pszAddrOld[i];
                    buffer[i]=0;

                    pszAddrOld = buffer;
                }
            }
            assert(pMemberInit->ut==CG_INIT_SIMPLE);
            ST_DumpSimpleInitValue(
                pdm, 
                &pEntry->edp, 
                pEntry->pType->lStrLen, 
                pMemberInit->sim.pLit, 
                dmp,
                pszAddrOld
               );
            dmp<<_T(';');
            dmp<<_T('\n');
        }

    }//while(!previousIFile.isEOF())

    if(pInit && lInitDone!=pInit->nInits)
        CGT_IERR2(pdm, pEdp, _T("invalid init"));
    
    //printf("dump : %s\n",(const TCHAR*)domainFile);
    return CGT_DmpToFile(pdm, pszGenPath, domainFile, dmp, false); 
}

static bool ST_GenerateArrayInitFiles
(
    CGlobCtx*             pgctx,
    CTarget*              pTrg,
    FC_CString&           domainFile,
    const CG_Edipos*      pEdp,
    const TCHAR*          pszVariable,
    const TCHAR*          pszOuterInitFile, 
    const CG_ArrayInit*   pInit, 
    const TCHAR*          pszAddr,
    const TCHAR*          pszPreviousIFileName,
    const CG_DType*       pType
)
{
    bool                 bRet;
    CGT_CSVFile          IFileRead;
    ST_INIT_LINE_TYP     initLine;
    long                 nInitDone;
    FC_CString           structType;
    FC_CString           structInit;
    FC_CString           strLenDeclId;
    CGT_CDiagMsg*        pdm;
    const TCHAR*         pszGenPath;
    const CG_DType*      pBaseType;
    

    assert(pType->bIsArray);

    pszGenPath = pgctx->getGenPath();
    pdm        = &pgctx->m_dm;
    pBaseType  = pgctx->m_pIF->getArrayBaseType(pType);

    //create file name for new init file to dump
    domainFile.load(pszOuterInitFile);
    ST_InsertInstanceName(domainFile, pszVariable);
    
    
    if(pszPreviousIFileName)
    {
        //open previous init file with CSV reader:
        FC_CString prevIFile;

        //create file name for previous init file, used as template
        //for outer initvalues, theywill be over written by instance inits:
        
        //d-2170: if beginns with "__@" it's not in gen\.c4pc\__classes, it's in gen\<target>
        if(pszPreviousIFileName[0]==_T('_') && pszPreviousIFileName[1]==_T('_') &&
           pszPreviousIFileName[2]==_T('@')
          )
        {
            prevIFile.load(pTrg->getTrgName());
        }
        else
        {
            prevIFile.load(pTrg->getTrgName());
            //prevIFile.load(PC_GEN_CLASSES_DIR);
        }

        prevIFile.appendDir(pszPreviousIFileName);
        prevIFile.changeFileExt(PC_INIT_EXT);

        if(!CGT_LoadCSVFile(pdm, &IFileRead, pszGenPath, prevIFile))
           return false;
    }
    else
    {
        //fill initLine with default values for base type:
        //only the initLine.pszId member (and for %add also the pszClass...) 
        //will be changed to #elem num:
        const TCHAR* pszJavaClass = GetFcTypeNameFromType(pBaseType);

        if(pType->pSym) 
        {
            //Array of struct:
            const CG_KeyValPair* pAttr;
            pAttr = pgctx->m_pIF->lookUpAttrib(pType->pSym->pAttribs, PC_ATTR_JAVACLASS);
            if(pAttr)
                pszJavaClass = pAttr->pszVal;

            structType.appendLo(pType->pSym->pszId);
            structInit.appendLo(pType->pSym->pszId).changeFileExt(PC_INIT_EXT);

            FC_CString genRel(MAX_PATH);
            genRel.load(pTrg->getTrgName()).appendDir(structInit);
            //genRel.load(PC_GEN_CLASSES_DIR).appendDir(structInit);
            
            if(!FC_FileExists(pgctx->getClsGenPath(), structInit))
                ST_DumpInitFile(pgctx, pTrg, pszAddr, pType->pSym, genRel);
            
            pTrg->addDomain(genRel); //##d-2375
        }

        initLine.pszId = NULL; //<-must be set for every element 
        
        initLine.pszClass = pszJavaClass;
        if(pType->lStrLen>=0)
        {
            //ARRAY OF WSTRING[len]:  len is declid and init string:
            strLenDeclId<<pType->lStrLen;
            initLine.pszDeclId     = strLenDeclId;
            initLine.pszInitString = strLenDeclId;
            initLine.pszInitFile   = _T("");
        }
        else if(!structType.isEmpty())
        {
            //ARRAY OF structtyp: structtyp is declid and
            //structtyp.init is the init file:
            initLine.pszDeclId     = structType;
            initLine.pszInitString = _T("");
            initLine.pszInitFile   = structInit;
        } 
        else
        {
            //simple type: fields are empty for default:
            initLine.pszDeclId     = _T("");
            initLine.pszInitString = _T("");
            initLine.pszInitFile   = _T("");
        }
    }

    //open output stream for this new init file
    //d-2170, moved past previousIFile.open, 
    //domainFile may be same then previousIFile
    CGT_CDmp  dmp; 

    nInitDone = 0;
    if(pInit)
    {
        bRet = ST_GenerateArrayInitFiles2(
                   pgctx,
                   pTrg,
                   pEdp,
                   pInit, 
                   dmp,
                   domainFile,
                   &nInitDone,
                   &initLine,
                   pszPreviousIFileName ? &IFileRead : NULL,
                   pszAddr,
                   pType
               );
        if(!bRet)
            return false;
    }

    
    //too less inits ?
    while(nInitDone<pType->nElems)
    {
        if(pszPreviousIFileName)
        {
            if(!ST_ScanInitLine(IFileRead, &initLine))
            {
                CGT_IERR2(pdm,pEdp,_T("bad init string"));
                return false;
            }

            assert(_tcstol(initLine.pszId, NULL, 10)==nInitDone);
            dmp<<initLine.pszId<<_T(';');
            dmp<<initLine.pszClass<<_T(';');
            dmp.dumpAsCSV(initLine.pszDeclId,-1,true)<<_T(';');
            dmp.dumpAsCSV(initLine.pszInitString,-1,true)<<_T(';');
            dmp<<initLine.pszInitFile<<_T('\n');
            nInitDone++;
        }    
        else if(pszAddr)
        {
            TCHAR            buffer[256];
            IOCOMP_CHK_ADDR  chkdata;

            dmp<<nInitDone<<_T(';');

            if(!ST_IncArrayElementDirectAddr(buffer, FC_ARRAY_LEN(buffer), pszAddr, nInitDone))
                CGT_IERR2(pdm,pEdp,_T("bad %-address"));
                 
            //assert: not supported %addr at array of struct !
            assert(!pType->pSym);

            chkdata.pEdp    = pEdp;
            chkdata.pszAddr = buffer;
            chkdata.pszType = pBaseType->pszName;
            chkdata.pszInst = _T(""); //TODO build inst name;

            if(!pTrg->checkAddr(pdm, &chkdata))
                pszAddr = NULL;

            dmp<<chkdata.javaClass<<_T(';');
            dmp<<buffer<<_T(';');   //%addr as decl
            dmp<<chkdata.initString<<_T(';'); 
            dmp<<_T('\n');
            nInitDone++;
        }
        else
        {
            dmp<<nInitDone<<_T(';');
            dmp<<initLine.pszClass<<_T(';');
            dmp.dumpAsCSV(initLine.pszDeclId,-1,true)<<_T(';');
            dmp.dumpAsCSV(initLine.pszInitString,-1,true)<<_T(';');
            dmp<<initLine.pszInitFile<<_T('\n');
            nInitDone++;
        }
    }

    //printf("dump : %s\n",(const TCHAR*)domainFile);
    return CGT_DmpToFile(pdm, pszGenPath, domainFile, dmp, false); 
}

static bool ST_GenerateArrayInitFiles2
(
    CGlobCtx*               pgctx,
    CTarget*                pTrg,
    const CG_Edipos*        pEdp,
    const CG_ArrayInit*     pInit, 
    CGT_CDmp&               dmp,
    const TCHAR*            pszInitFileName,
    long*                   pnInitDone,
    ST_INIT_LINE_TYP*       pInitLine,
    CGT_CSVFile*            pPreviousIFile,
    const TCHAR*            pszAddr,
    const CG_DType*         pType
)
{
    bool                  bRet;
    long                  nInit;
    long                  nRep;
    const CG_Initialiser* pMemberInit;
    CGT_CDiagMsg*         pdm;
    FC_CString            helpId;
    FC_CString            helpFname;
    FC_CString            helpFname2;
    IOCOMP_CHK_ADDR       chkdata;

    pdm  = &pgctx->m_dm;


    for(nInit=0; nInit<pInit->nInits; nInit++)
    {
        nRep = pInit->pRepeats[nInit];
        pMemberInit = &pInit->pInits[nInit];

        helpFname2.load(_T(""));
        while(nRep-- >0)
        {
            //more inits than array elements is ok:
            if(*pnInitDone>=pType->nElems)
                return true;

            if(pMemberInit->ut != CG_INIT_ARRAY)
            {
                if(pPreviousIFile)
                {
                    if(!ST_ScanInitLine(*pPreviousIFile, pInitLine))
                        return false;
                }
                else
                {
                    //id is the array element number:
                    helpId.clear()<<*pnInitDone;
                    pInitLine->pszId = helpId;
                }
            }

            TCHAR buffer[256];
            const TCHAR* pszAddrInc= NULL;
            if(pszAddr)
            {
                if(!ST_IncArrayElementDirectAddr(buffer, FC_ARRAY_LEN(buffer), pszAddr, *pnInitDone))
                    CGT_IERR2(pdm, pEdp, _T("bad %-address"));

                chkdata.pEdp    = pEdp;
                chkdata.pszAddr = buffer;
                chkdata.pszType = pgctx->m_pIF->getArrayBaseType(pType)->pszName;
                chkdata.pszInst = _T(""); //TODO build inst name;

                if(pTrg->checkAddr(pdm, &chkdata))
                    pszAddrInc = buffer;
                else
                    pszAddr = NULL; //don't continue for this array after error
            }

            switch(pMemberInit->ut)
            {
            case CG_INIT_NONE:
                (*pnInitDone)++;
                dmp<<pInitLine->pszId<<_T(';');
                if(pszAddrInc)
                {
                    dmp<<chkdata.javaClass<<_T(';');
                    //decl id is addr:
                    dmp.dumpAsCSV(pszAddrInc,-1,true)<<_T(';');

                    dmp.dumpAsCSV(chkdata.initString,-1,true);
                    if(pInitLine->pszInitString[0])
                    {
                        //cannot handle " inside init string, but there are no string AT anyway...
                        if(_tcsstr(pInitLine->pszInitString, _T("\"")))
                            CGT_IERR2(pdm,pEdp,_T("bad init string"));

                        dmp<<_T('|');
                        dmp.dumpAsCSV(pInitLine->pszInitString,-1,true);
                    }
                    dmp<<_T(';');
                }
                else
                {
                    dmp<<pInitLine->pszClass<<_T(';');
                    dmp.dumpAsCSV(pInitLine->pszDeclId,-1,true)<<_T(';');
                    dmp.dumpAsCSV(pInitLine->pszInitString,-1,true)<<_T(';');
                }
                dmp<<pInitLine->pszInitFile<<_T('\n');
            break;
            case  CG_INIT_STRUCT:
                (*pnInitDone)++;
                
                //simple optimize: 4((struct init)) 
                //reuse helpFname from first run
                if(_tcscmp(helpFname2, pInitLine->pszInitFile) !=0)
                {
                    helpFname2.load(pInitLine->pszInitFile);
                    
                    bRet = ST_GenerateInstanceInitFiles(
                               pgctx,
                               pTrg,
                               helpFname,
                               pEdp,
                               pInitLine->pszId,
                               pszInitFileName, 
                               &pMemberInit->str, 
                               pszAddrInc,
                               pInitLine->pszInitFile,
                               pType->pSym
                           );

                    if(!bRet)
                        return false;
                }
                dmp<<pInitLine->pszId<<_T(';');
                dmp<<pInitLine->pszClass<<_T(';');
                dmp.dumpAsCSV(pInitLine->pszDeclId,-1,true)<<_T(';');
                dmp<< /* empty initString */ _T(';');
                dmp<<FC_StringGetFilePart(helpFname)<<_T('\n');
            
                //append new domain name to dom list:
                pTrg->addDomain(helpFname);

            break;
            case CG_INIT_ARRAY:
                bRet = ST_GenerateArrayInitFiles2(
                           pgctx,
                           pTrg,
                           pEdp,
                           &pMemberInit->arr, 
                           dmp,
                           pszInitFileName,
                           pnInitDone,
                           pInitLine,
                           pPreviousIFile,
                           pszAddr,//<-CAUTION pass pszAddr not pszAddrInc
                           pType
                        );
                if(!bRet)
                    return false;
            break;
            case CG_INIT_SIMPLE:
                (*pnInitDone)++;
                dmp<<pInitLine->pszId<<_T(';');
                dmp<<pInitLine->pszClass<<_T(';');

                TCHAR buffer2[512];
                const TCHAR* pszEnd;
                if(!pszAddrInc)
                {
                //HACK ?????????????????????????????????????????????? d-2170 (delete this source code !!)
                    if(pInitLine->pszInitString && pInitLine->pszInitString[0]==_T('%'))
                    {
                        CGT_CheckDirectAddrSyntax(pInitLine->pszInitString, &pszEnd, NULL);
                        if(*pszEnd==_T('|') || *pszEnd==0)
                        {
                            for(int i=0; i<500 && pInitLine->pszInitString+i != pszEnd;)
                                buffer2[i++] = pInitLine->pszInitString[i];
                            buffer2[i]=0;
                            pszAddrInc = buffer2; 
                        }
                    }
                }
                //assert: addr must not be an incomplete address any more
                if(pszAddrInc)
                {
                    if(!CGT_IsFullBasicAddress(pszAddrInc))
                        CGT_IERR2(pdm,pEdp,_T("bad %-address"));
                    //addr is decl id:
                    dmp<<pszAddrInc<<_T(';');
                }
                else
                {
                    //no decl id:
                    dmp<<_T(';'); 
                }

                assert(pMemberInit->ut==CG_INIT_SIMPLE && !pType->pSym);
                ST_DumpSimpleInitValue(
                    pdm, 
                    pEdp, 
                    pType->lStrLen, 
                    pMemberInit->sim.pLit, 
                    dmp,
                    pszAddrInc
                );

                dmp<<_T(';');
                dmp<</* empty initFile */ _T('\n');
            break;
            default:
                CGT_IERR2(pdm,pEdp,_T("bad enum"));
                return false;
            }
        }
        
    }

    return true;
}









//a init file record is:
//<id>;<class>;<declid>;<init string>;<init file>
void ST_DumpInitFileEntry(
    CGlobCtx*            pgctx,
    CTarget*             pTrg,
    const TCHAR*         pszGenRelInitFile,
    const TCHAR*         pszInstanceAddr,
    const CG_SymEntry*   pEntry, 
    CGT_CDmp&            dmp
)
{
    bool                     bRet;
    FC_CString               helpFname;
    FC_CGArray<TCHAR>        addrSum;
    const CG_DType*          pType;
    CG_DTYPES                type;
    const TCHAR*             pszAddr;
    const CG_KeyValPair*     pAttr;

    pszAddr = pEntry->pszAddr;
    pType   = pEntry->pType;
    type    = pType->type;

    assert(CG_IsMonomorph(pType->type));


    //write object name to init file:
    dmp.lo(pEntry->pszId)<<_T(';');

    //##d-1393 beg
    if(pszAddr && pType->bIsArray && type!=CG_BOOL && type!=CG_BYTE)
    {
        pgctx->m_dm.errNS1(&pEntry->edpAddr, 
           _T("'ARRAY AT %-address': only supported if base type is BOOL or BYTE"));
        
        pszAddr = NULL;
    }
    //##d-1393 end

    if(pszAddr)
    {
        if(pTrg->isIOEnabled())
        {
            if(pszInstanceAddr)
            {
                assert(CGT_IsIncompleteAddressDef(pszInstanceAddr) && 
                       CGT_IsIncompleteAddressRef(pszAddr));
                //substitute incomplete address:
                if(!ST_MergeIncompleteAddress(pszInstanceAddr, pszAddr, &addrSum))
                    CGT_IERR2(&pgctx->m_dm, &pEntry->edp, _T("bad %-address"));

                pszAddr = addrSum.getElements(); 
            }
        }
        else
        {
            pszAddr = NULL;
            pTrg->warnDisableIO(&pgctx->m_dm, &pEntry->edpAddr);
        }
    }



    //init string or init file:    
    if(pType->bIsArray)
    {
        //the java class:
        dmp<<_T("system.ArrayVar;");
        //the decl id:
        ST_DumpDeclId(pgctx, pEntry, dmp, pEntry->pInit, pszAddr);
        dmp<<_T(';');
        //no string init for Array Instance:
        dmp<<_T(';');

        //handle array initial values:
        assert(!pEntry->pInit || pEntry->pInit->ut==CG_INIT_ARRAY);
        bRet = ST_GenerateArrayInitFiles(
                    pgctx,
                    pTrg,
                    helpFname,
                    &pEntry->edp,
                    pEntry->pszId,
                    pszGenRelInitFile,
                    pEntry->pInit ? &pEntry->pInit->arr : NULL, 
                    pszAddr,
                    NULL,
                    pType
               );

        //be very carefull with errors here:
        if(!bRet && pgctx->m_dm.hasNoErr())
            CGT_IERR2(&pgctx->m_dm,&pEntry->edp,_T("error count out of sync"));

        dmp<<FC_StringGetFilePart(helpFname)<<_T('\n');

        //append new domain name to dom list:
        pTrg->addDomain(helpFname);
    }
    else if(pType->pSym)
    {
        //the java class:
        if(pType->pSym->pouTyp==CG_POU_SDT)
        {
            //the java class, possible for structs type as a whole:
            pAttr = pgctx->m_pIF->lookUpAttrib(pType->pSym->pAttribs, PC_ATTR_JAVACLASS);
            if(pAttr)
                dmp<<pAttr->pszVal<<_T(';');
            else
                dmp<<_T("system.StructVar;");
        }
        else
        {
            (dmp<<_T("user.")).lo(pType->pSym->pszId)<<_T(';');
        }
        
        //the decl id:
        ST_DumpDeclId(pgctx, pEntry, dmp, pEntry->pInit, pszAddr);
        dmp<<_T(';');

        //no string init for FB/DUT/PRG Instance
        dmp<<_T(';');
        
        assert(type==CG_STRUCT||type==CG_INSTANCE||type==CG_PROGRAM);
        assert(!pEntry->pInit || pEntry->pInit->ut==CG_INIT_STRUCT);
        
        bRet = ST_GenerateInstanceInitFiles(
                        pgctx,
                        pTrg,
                        helpFname,
                        &pEntry->edp,
                        pEntry->pszId,
                        pszGenRelInitFile,
                        pEntry->pInit ? &pEntry->pInit->str : NULL, 
                        pszAddr,
                        NULL,
                        pType->pSym);
           
        //be very carefull with errors here:
        if(!bRet && pgctx->m_dm.hasNoErr())
            CGT_IERR2(&pgctx->m_dm,&pEntry->edp,_T("error count out of sync"));

        dmp<<FC_StringGetFilePart(helpFname)<<_T('\n');

        //append new domain name to dom list:
        pTrg->addDomain(helpFname);
    }
    else //any simple
    {
        IOCOMP_CHK_ADDR  chkdata;
        //check direct address:
        if(pszAddr)
        {
            chkdata.pEdp    = &pEntry->edpAddr;
            chkdata.pszAddr = pszAddr;
            chkdata.pszType = pType->pszName;
            chkdata.pszInst = _T(""); //TODO build inst name;
            pTrg->checkAddr(&pgctx->m_dm, &chkdata);
        }

        //the java class:
        pAttr = pgctx->m_pIF->lookUpAttrib(pEntry->pAttribs, PC_ATTR_JAVACLASS);
        if(pAttr)
            dmp<<pAttr->pszVal<<_T(';');
        else if(pszAddr)
            dmp<<chkdata.javaClass<<_T(';');
        else
            dmp<<GetFcTypeNameFromType(pType)<<_T(';');
        
        //the decl id:
        ST_DumpDeclId(pgctx, pEntry, dmp, pEntry->pInit, pszAddr);
        dmp<<_T(';');

        if(pEntry->pInit)
        {
            assert(pEntry->pInit->ut==CG_INIT_SIMPLE);
            ST_DumpSimpleInitValue(
                &pgctx->m_dm,
                &pEntry->edp,
                pType->lStrLen,
                pEntry->pInit->sim.pLit, 
                dmp, 
                pszAddr ? chkdata.initString.psz() : NULL
            );
        }
        else if(pszAddr) 
        {
            assert(pType->lStrLen==-1);
            dmp.dumpAsCSV(chkdata.initString,-1,true);
        }
        else if(pType->lStrLen>=0)
        {
            assert(type==CG_WSTRING||type==CG_STRING);
            dmp<<pType->lStrLen;
        }
    
        //no init file
        dmp<<_T(";\n");
    }

}




