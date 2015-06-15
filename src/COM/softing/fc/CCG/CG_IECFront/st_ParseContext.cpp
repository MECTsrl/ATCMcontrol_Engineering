#include <stdio.h>
#include <limits.h>
#include <stddef.h>
#include "fc_tools\fc_fileIO.h"
#include "st_ParseContext.h"
#include "configuration.h"
#include "SFCChecker.h"
#include "IECKeywords.h"
#include "declcheck.h"
#include "CG_Messages.h"
#include "utilif.h"
#include "utilsts.h"
#include "cgt_address.h"
#include "CG_AutoDecl.h"






const CG_Edipos* st_ParseContext::setCurrEdp(int nLine, int nCol)
{
    assert(nLine>0 && nCol>0);
    if(m_currEdp.typ==CG_EDP_ST)//==0 is ok for first token in file
    {
        m_currEdp.lc.l = m_lLineOffset + nLine;//HACK rework the col,line offset method
        m_currEdp.lc.c = m_lColOffset  + nCol;
    }
    else if(m_currEdp.typ==CG_EDP_4GR_STBOX)
    {
        m_currEdp.gr.l = m_lLineOffset + nLine;
        m_currEdp.gr.c = m_lColOffset  + nCol;
    }
    return &m_currEdp;
}


const CG_Symtab* st_ParseContext::getSymtabForDType(
    const CG_Edipos* pEdp, 
    const TCHAR*     pszId,
    long             want
)
{
    ST_CSymtabPrv*    pSymPrv;
    const CG_Symtab*  pSym;
    long              lres;

    lres = m_pFront->loadHeaderEx(pEdp, pszId, want, &pSymPrv, NULL);

    //if trg typ not supported error, carry on. All other errors ignore the fun call:
    if(lres!=ST_LHDR_OK && lres!=ST_LDHD_TRG_NS)
    {
        if(lres & ST_LDHD_ERRIND) //indirect error update internal err counter
            indirectError();
        return NULL;
    }

    pSym = pSymPrv->getCGSym();
    assert(pSym->flags & CG_STAB_HDR_NOERR);
    return pSym;
}

long st_ParseContext::checkTypeStrLen(CG_DTYPES dType, ST_Expression_C* pExpr)
{
    long iStrLen = -1;

    switch(dType)
    {
    case CG_STRING:    
    case CG_WSTRING:    
    case CG_QVT_STRING:    
    case CG_QVT_WSTRING:    
    break;
    default:
        m_pdm->msg0(CG_E_BAD_STRLENGTH_TYPE, pExpr->getEdp());
        delete(pExpr);
        return -1;
    }

    if(pExpr->checkTypedExpr(CG_TBS_ANY_INT & ~CG_TBS_LINT, true))
    {
        pExpr->fixType();
        const ST_BINVAL_TYP* pVal = pExpr->getLitValue();
        if(pVal)
        {
            if(pVal->any64<=0 || pVal->any64>MAX_STRING_LENGTH)
            {
                m_pdm->msg2(CG_E_BAD_STRLENGTH, pExpr->getEdp(),
                    m_pdm->jot1()<<pVal->any64, m_pdm->jot2()<<MAX_STRING_LENGTH);
            }
            else
            {
                iStrLen = (long)pVal->any64;
            }
        }
        else    
        {
            m_pdm->msg0(CG_E_EXPR_NOT_CONST, pExpr->getEdp());
        }   
    }

    delete(pExpr);
    return iStrLen;
}
int st_ParseContext::checkArrayDims(
    ST_ExpressionList_C* pDims, 
    CG_Subrange ranges[CG_MAX_ARRAY_DIM]
)
{
    __int64              lo;
    __int64              up;
    ST_Expression_C*     pLo;
    ST_Expression_C*     pUp;
    const ST_BINVAL_TYP* pLoVal;
    const ST_BINVAL_TYP* pUpVal;
 

    pLo = pDims->m_list.get(0);//##AC
    pUp = pDims->m_list.get(1);//##AC

    //TODO only one dim supported here
    if(!pLo->checkTypedExpr(CG_TBS_ANY_INT & ~CG_TBS_LINT, true) ||
       !pUp->checkTypedExpr(CG_TBS_ANY_INT & ~CG_TBS_LINT, true))
    {
        delete(pDims);
        return 0;
    }

    pLo->fixType();
    pUp->fixType();
    
    pLoVal = pLo->getLitValue();
    pUpVal = pUp->getLitValue();
        
    if(!pLoVal)
        m_pdm->msg0(CG_E_EXPR_NOT_CONST, pLo->getEdp());
    if(!pUpVal)
        m_pdm->msg0(CG_E_EXPR_NOT_CONST, pUp->getEdp());

    if(pLoVal && pUpVal)
    {

        lo = pLoVal->any64;    
        up = pUpVal->any64;    

        if(lo>up)
        {
            m_pdm->msg1(CG_E_BAD_RANGE, pLo->getEdp(), m_pdm->jot1()<<lo<<_T("..")<<up);
            delete(pDims);
            return 0;
        }
    
        //assert: type cast below
        assert(LONG_MAX-1>MAX_ARRAY_ELEMENTS);
        ranges[0].lRangeStart = (long)lo;
        ranges[0].ulRangeLength=(unsigned long)(up-lo);

        //check limit:
        if(up-lo+1>MAX_ARRAY_ELEMENTS)
        {
            m_pdm->msg2(CG_E_TOO_MANY_ARRAY_ELEMS, pLo->getEdp(),
                m_pdm->jot1()<<up-lo+1, m_pdm->jot2()<<MAX_ARRAY_ELEMENTS);
        }
    }
    else
    {
        delete(pDims);
        return 0;
    }

    delete(pDims);

    return 1;
}


ST_Literal_C* st_ParseContext::getStringLiteral(
    const CG_Edipos* pEdp, 
    TCHAR            delim
)
{
    const TCHAR*   psz;
    ST_Literal_C*  pLit;

    if(UTIL_STS_SkipIECStringLiteral(m_pScanner, delim, true))
    {
        psz = UTIL_STS_GetSkippedTextBlock(m_pScanner); 
    }
    else
    {
       m_pdm->msg0(CG_E_UNTERM_STRING, pEdp); 
        //fake:
        psz = delim==_T('"') ? _T("\"\"") : _T("''");
    }
    pLit = ST_LitTokenToLiteral(
                m_pdm,
                pEdp, 
                this,
                &m_pFront->m_features,
                UTIL_STS_STRING_LITERAL_BEGIN, 
                psz
            );

    return pLit;
}
ST_Identifier_C* st_ParseContext::getAttribVal(
    const CG_Edipos* pEdp
)
{
    const TCHAR*     psz;

    if(UTIL_STS_SkipBlock(m_pScanner, true, _T("\""), false))
    {
        psz = UTIL_STS_GetSkippedTextBlock(m_pScanner); 
    }
    else
    {
       m_pdm->msg0(CG_E_UNTERM_STRING, pEdp); 
        //fake, the caller must handle this!:
       return NULL;
    }

    //new line in string?
    const TCHAR*  pszNL = _tcschr(psz, _T('\n'));
    if(pszNL)
    {
        CG_Edipos edptmp = *pEdp;
        CG_IncColEdipos(&edptmp, pszNL - psz);
        m_pdm->msg0(CG_E_NEWLINE_IN_STRING, &edptmp);
        //ignore and get next token
        return NULL;
    }


    return new ST_Identifier_C(pEdp, this, psz);
}


const TCHAR* st_ParseContext::getSkippedText(
    const CG_Edipos* pEdp, 
    const TCHAR*     pszEnd
)
{
    if(UTIL_STS_SkipBlock(m_pScanner, true, pszEnd, true))
    {
        return UTIL_STS_GetSkippedTextBlock(m_pScanner);
    }
    else
    {
        m_pdm->msg0(CG_E_EOF_IN_NATIVE_BLOCK, pEdp);
        return NULL;
    }
}

const TCHAR* st_ParseContext::getSkippedBraceText(
    const CG_Edipos* pEdp, 
    TCHAR            cOpenBr
)
{
    if(UTIL_STS_SkipMatchingBrace(m_pScanner, true, cOpenBr)==1)
    {
        return UTIL_STS_GetSkippedTextBlock(m_pScanner);
    }
    else
    {
        m_pdm->msg0(CG_E_EOF_IN_NATIVE_BLOCK, pEdp);
        return NULL;
    }
}

TCHAR* st_ParseContext::getImportFile(
    const CG_Edipos* pEdp
)
{
    int              tok;
    const TCHAR*     psz;
    int              nLine;
    int              nCol;
    const TCHAR*     pszNL;
    CG_Edipos        edptmp;

    pszNL = NULL;
    do
    {
        tok = UTIL_STS_NextToken(m_pScanner, &psz, &nLine, &nCol);
        setCurrEdp(nLine, nCol);

        if(tok=='\n' && !pszNL)
        {
            pszNL = psz;
            m_pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, 
               &m_currEdp, _T("\"<file>\""), m_pdm->res1(CG_RES_STR_NL));
            //is ok, we can live with it, this error msg is only 
            //to make sure that the syntax is not too friendly..
        }
    }while(tok=='\n');

    if(tok!=UTIL_STS_STRING_LITERAL_BEGIN || *psz!=_T('"'))
    { 
        m_pdm->msg2(CG_E_OLD_SYNTAX, pEdp, _T("#import file"), _T("#import \"file\""));
        //try to ignore the full line and get next token:
        UTIL_STS_SkipLine(m_pScanner);
        return NULL; 
    }


    if(UTIL_STS_SkipBlock(m_pScanner, true, _T("\""), false))
    {
        psz = UTIL_STS_GetSkippedTextBlock(m_pScanner); 
    }
    else
    {
       m_pdm->msg0(CG_E_UNTERM_STRING, &m_currEdp); 
       //get next token (the EOF)
       return NULL;
    }

    //new line in string?
    pszNL = _tcschr(psz, _T('\n'));
    if(pszNL)
    {
        edptmp = m_currEdp;
        CG_IncColEdipos(&edptmp, pszNL - psz);
        m_pdm->msg0(CG_E_NEWLINE_IN_STRING, &edptmp);
        //ignore and get next token
        return NULL;
    }

    //do not trim the string, same as C++
    return FC_StringDup(psz);
}



void st_ParseContext::newDeclScope(
    const CG_Edipos* pEdp, 
    CG_DECLSCOPE     s, 
    WORD             declBitSet
)
{
    m_currDeclScope = s;
    m_currDeclBitSet= declBitSet;
    assert(m_pSymtabPrv);

    if(s==CG_VAR_GLOBAL && m_srcType==CG_SRC_TYPE_RES)
    {
        m_pdm->msg1(CG_E_NOT_SUPPORTED, pEdp, 
           _T("VAR_GLOBAL in resource, use #import \"<file>.gvl\" instead")
          );
    }   

    ST_CheckNewDeclScope(m_pFront, pEdp, 
        pSymtab, &m_currDeclScope, &m_currDeclBitSet);
}


void st_ParseContext::appendDeclListToSymTab(
    ST_CDeclList*    pIdList, 
    ST_DirectVar_C*  pAddr,
	const TCHAR*     pszSysAddr, //forosai
    const CG_DType*  pDType, //NULL if type in error
    WORD             wStructConstantDecl, //IEC Extension CONSTANT Modifier in Struct decl
    ST_Expression_C* pInit
)
{
    int                    i;
    CG_Initialiser*        pIniStruct;
    int                    idlistLen;
    const TCHAR*           pszAddr;
    const CG_Edipos*       pEdpAdr;
    const ST_CAttrList*    pAttr;
    const ST_Identifier_C* pId;
    const CG_AttribList*   pAttrStruct;

//TODO check with semnatic structs have no decl list nor 'only %-addr'

    
    pszAddr = NULL;
    pEdpAdr = NULL;
    pIniStruct =NULL;
    idlistLen= 0;

    if(pIdList)
    {
        idlistLen = pIdList->getLen();
        assert(idlistLen>0);
    }

    //check syntax: if %-addr only one id, no id-list:
    if(idlistLen>1 && (pAddr || pSymtab->pouTyp==CG_POU_SDT))
    {
        const ST_Identifier_C* pIdAttr = pIdList->getIdAndAttr(1, NULL);
        m_pdm->msg1(CG_E_SYNTAX_UNEXPECTED, pIdAttr->getEdp(), pIdAttr->getName());
        //carry on
    }


    if(pAddr)
    {
        pszAddr = pAddr->getAddr();
        pEdpAdr = pAddr->getEdp();
    }
    
    if(idlistLen==0 && !pAddr)
    {
        m_pdm->msg1(CG_E_SYNTAX_MISSING, &m_currEdp,
            m_pdm->res1(CG_RES_STR_IDENTIFIER));
        assert(m_pdm->hasErr());
    }

    //error on data type, if so forget...
    //Note: do not check init if type name itself was in error...
    if(!pDType)
    {
        assert(m_pdm->hasErr());
        return;
    }

    if (m_pFront->m_features.m_pCFeat->nMaxStrLen && 
        m_pFront->m_features.m_pCFeat->nMaxStrLen < pDType->lStrLen)
    {
        assert(pDType->tbs & CG_TBS_ANY_STRING);
        m_pdm->errNS3(&m_currEdp, ST_BitsetToStr(m_pdm->jot1(), pDType->tbs),
            m_pdm->res2(CG_RES_STR_LONGERTHAN)<<m_pFront->m_features.m_pCFeat->nMaxStrLen,
            m_pdm->res3(CG_RES_STR_CHARS));
    }
    
    if(pInit)
    {
        FC_NEW_ZSTRUCT(pIniStruct, CG_Initialiser);
        if(!pInit->checkInit(pDType, pIniStruct))
        {
            delete(pIniStruct);
            pIniStruct = NULL;
        }
        else
        {
            m_pFront->registerInit(pIniStruct);
        }
    }
    
    //caution: struct don't have a var section, simulate:
    if(pSymtab->pouTyp==CG_POU_SDT)
    {
        m_currDeclScope = CG_VAR; 
        m_currDeclBitSet= wStructConstantDecl; 
    }
    else if(wStructConstantDecl!=0)
    {
        m_pdm->msg0(CG_E_SYNTAX, getCurrEdp());
    }

    //TODO if no ID only %-addr?
    for(i=0;i<idlistLen;i++)
    {
        pId = pIdList->getIdAndAttr(i, &pAttr);
        assert(pId);
        pAttrStruct = NULL;
        if(pAttr)
        {
          //TODO  
           pAttrStruct = m_pFront->registerAttrList(
                  pAttr->convertToStruct());
        }
        if(!ST_CheckAndAppendDecl(
                m_pFront, 
                m_pSymtabPrv,
                m_currDeclScope, 
                m_currDeclBitSet, 
                pId->getName(), 
                pId->getEdp(), 
                pDType, 
                pId->getEdp(), //TODO wrong edipos
                pszAddr,
				pszSysAddr, //forosai
                pEdpAdr, 
                pAttrStruct, 
                pIniStruct
            )
         )
            break;
    }
    
    delete(pIdList);
    delete(pInit);
    delete(pAddr);
}





CG_POUTYP st_ParseContext::checkFileNameToPOUTyp(
    const CG_Edipos*  pEdp,
    CG_POUTYP         pouTypFound
)
{
    bool bOk = false;

    switch(pouTypFound)
    {
    case CG_POU_SDT:
        bOk = m_srcType==CG_SRC_TYPE_SDT;
    break;
    case CG_POU_CST:
        bOk = m_srcType==CG_SRC_TYPE_CST;
    break;
    case CG_POU_GVL:
        bOk = m_srcType==CG_SRC_TYPE_GVL;
    break;
    case CG_POU_FB:
    case CG_POU_FUN:
    case CG_POU_PRG:
        bOk = m_srcType==CG_SRC_TYPE_ST  ||
              m_srcType==CG_SRC_TYPE_IL  ||
              m_srcType==CG_SRC_TYPE_4GR ||
              m_srcType==CG_SRC_TYPE_HDR;
    break;
    default:
        assert(0);
    }

    if(!bOk)
    {
        m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_FILE, pEdp);
        return CG_POUTYP_UNDEF;
    }
    return pouTypFound;
}

void st_ParseContext::newSymtab(
    const CG_Edipos*     pEdp,
    CG_POUTYP            pouTyp,
    const TCHAR*         pszPouName
)
{
    assert(!pSymtab && !m_pSymtabPrv);

    m_pSymtabPrv = new ST_CSymtabPrv(m_pFront, pEdp, m_pLibCtx,
        pszPouName, pouTyp, m_srcType, m_ftLastWrite);
    assert(m_ftLastWrite);
    pSymtab = m_pSymtabPrv->getCGSym();
}

bool st_ParseContext::newPOU(
    const ST_CAttrList*pAttr,
    CG_POUTYP          pouTyp, //PGR,FB,FUN,SDT
    ST_Identifier_C*   pId,
    const CG_DType*    pFunTyp //return type or SDT, void fun 
                               //yields pFunTyp==CG_VOID, 
                               //NULL if SDT
)
{
    const TCHAR*     pszPouName;
    const TCHAR*     pszPureFileName;
    FC_CString       pureFileNameNoExt;
    const CG_Edipos* pEdp;


    pszPureFileName = FC_StringGetFilePart(m_pszFile);
    pureFileNameNoExt.load(pszPureFileName).changeFileExt(NULL);

    pszPouName = pId->getName();
    pEdp       = pId->getEdp();


    if(pouTyp!=CG_POU_FUN)
    {
        //special problem:
        //check that pou name isn't MOD,NOT,AND,...
        //this is only allowed if pou is a fun and usually 
        //a standard lib defined fun, we don't care if a user 
        //declared his own version of AND, this will cause a warning
        //POU declared twice anyway if the standard lib is attached.
        //Don't forget we want to create our standard IEC lib
        //out of a normal project too!
        if(UTIL_IsIECKeyword(pszPouName)!=UTIL_KW_NO_KEYWORD)
        {
            m_pdm->msg1(CG_E_SYNTAX_KW_USE, pEdp, pszPouName);
            return false;
        }
    }



    if(pSymtab)//already a struct defined in this file ? 
    {
        m_pdm->msg0(CG_E_ONLY_ONE_DUT, pEdp);
        return false;
    }

    if(!ST_IS_NAME_EQUAL(pszPouName, pureFileNameNoExt))
    {
        m_pdm->msg2(CG_E_POUNAME_FILENAME, pEdp, pszPouName, pszPureFileName);
        //continue after this error is not safe
        return false;
    }

    newSymtab(pEdp, pouTyp, pszPouName);

    if(pFunTyp)
    {
        if(pouTyp==CG_POU_FUN)
        {
            m_pSymtabPrv->registerDecl(
                   pSymtab->pszId, 
                   &pSymtab->edp, 
                   CG_VAR_OUTPUT,
                   0,
                   NULL,
                   pFunTyp, 
                   NULL, 
     			   NULL, //forosai
                   NULL, 
                   NULL
             );
        }
        else if(pFunTyp->type!=CG_VOID)
        {
            m_pdm->msg2(CG_E_RETTYPE_NO_FUN, pEdp, pszPouName, pszPureFileName);
        }
    }
    else
    {
        assert(pouTyp==CG_POU_SDT);
    }

    if(pAttr)
    {
        //TODO check std attribs:
        m_pSymtabPrv->registerAttributes(pAttr->convertToStruct());
        delete(pAttr);
    }

    delete(pId);
    return true;
}

bool st_ParseContext::checkPOUNameToFileName(
    const CG_Edipos* pEdp,
    const TCHAR*     pszPouName
)
{
    const TCHAR*     pszPureFileName;
    FC_CString       pureFileNameNoExt;

    pszPureFileName = FC_StringGetFilePart(m_pszFile);
    pureFileNameNoExt.load(pszPureFileName).changeFileExt(NULL);

    if(!ST_IS_NAME_EQUAL(pszPouName, pureFileNameNoExt))
    {
        m_pdm->msg2(CG_E_POUNAME_FILENAME, pEdp, 
            pszPouName, pszPureFileName);
        //don't continue makes no sense to the user..
        return false;
    }
    return true;
}


bool st_ParseContext::newRES(
    ST_CAttrList*    pAttr,
    ST_Identifier_C* pId,
    ST_CAttrList*    pAttrId,
    ST_Identifier_C* pOptResType
)
{
    FC_CString jot(MAX_PATH);
    assert(!m_pResource);

    if(m_srcType!=CG_SRC_TYPE_RES)
    {
        m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_FILE, pId->getEdp());
        return false;
    }

    if(!checkPOUNameToFileName(pId->getEdp(), pId->getName()))
        return false;
        
    //unique name is gvl@<resource name>
    jot.load(_T("gvl@"))<<pId->getName();

    //create one symtab this one will receive all #import if any:
    newSymtab(pId->getEdp(), CG_POU_GVL, jot);

    if(pAttrId)
    {
        m_pdm->msg2(CG_W_OLD_SYNTAX, pAttrId->getEdp(), 
            m_pdm->res1(CG_RES_STR_RESATTR1),
            m_pdm->res1(CG_RES_STR_RESATTR2));

        if(pAttr)
        {
            pAttr->m_attrtag.append(&pAttrId->m_attrtag);
            pAttrId->m_attrtag.resetUsed();

            pAttr->m_attrval.append(&pAttrId->m_attrval);
            pAttrId->m_attrval.resetUsed();
            delete(pAttrId);
        }
        else
        {
            pAttr = pAttrId;
            pAttrId = NULL;
        }
    }

    FC_NEW_ZSTRUCT(m_pResource, CG_Resource);
    m_pResource->edp    = pSymtab->edp;
    m_pResource->pszId  = m_pFront->registerString(pId->getName());

    if(pOptResType)
    {
        if(m_pFront->m_kad.getTargetTypeByExt(pOptResType->getName(), NULL, NULL, NULL)<=0)
        {
            m_pdm->msg2(CG_W_NO_TARGETTYPE_EXT, pOptResType->getEdp(), 
               pOptResType->getName(), m_pFront->m_kad.getTargetTypeExts());            
        }
        else
        {
            m_pResource->pszTyp = m_pFront->registerString(pOptResType->getName());
        }
        delete(pOptResType);
    }
    if(pAttr)
    {
        //TODO check attribs:
        m_pResource->pAttribs = 
            m_pFront->registerAttrList(pAttr->convertToStruct());
        delete(pAttr);
    }
                          
    //set later m_pResource->nTasks;
    //set later m_pResource->pTasks;
    m_pResource->pGVL = m_pSymtabPrv->getCGSym();

    delete(pId);
    return true;
}


bool st_ParseContext::newCON(
    const CG_Edipos* pEdpName,
    const TCHAR*     pszConName,
    CG_AttribList*   pAttribs
)
{
                  
    if(m_srcType!=CG_SRC_TYPE_CON)
    {
        m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_FILE, pEdpName);
        return false;
    }

    if(!checkPOUNameToFileName(pEdpName, pszConName))
        return false;

    assert(!m_pConfig);//only one possible by syntax
    
    //if(pAttr) TODO check attribs:
    m_pConfig = new ST_CConfig(m_pFront, pEdpName, 
        pszConName, pAttribs, m_ftLastWrite);
    return true;
}



void st_ParseContext::attachPrgToTask(
    WORD             retBitSet,
    ST_Identifier_C* pPrgInstName, 
    ST_CAttrList*    pOptAttr,
    ST_Identifier_C* pOptTaskName,
    ST_Identifier_C* pPrgTypeName
)
{ 
    const CG_SymEntry*   pEntry;
    const TCHAR*         pszId;
    const CG_Edipos*     pEdpId;
    const TCHAR*         pszTyp;
    const CG_Edipos*     pEdpTyp;
    const CG_Symtab*     pSymPrg;

    pszId = pPrgInstName->getName();
    pEdpId = pPrgInstName->getEdp();
    pszTyp = pPrgTypeName->getName();
    pEdpTyp = pPrgTypeName->getEdp();

    
    //currently the task name cannot be defaulted, 
    //we don't have a default task in 4CC: //TODO feature set?
    if(!pOptTaskName)
    {
        m_pdm->msg1(CG_E_TASK_NAME_REQUIRED, pEdpId, pszId);
        goto Ende;
    }


    //check if prg inst name is not already used as glob var name:
    pEntry = m_pSymtabPrv->lookUpEntryDeep(pszId, NULL);
    if(pEntry)
    {
        m_pdm->msg2(CG_E_PRGINSTNAME_IS_GLOBVAR, pEdpId, 
                pszId, m_pdm->edpToStr(&pEntry->edp, m_pdm->jot1()));
        goto Ende;
    }

    //check if prg inst name is not already used as task name:
    if(m_pCurrTaskList && m_pCurrTaskList->findTaskByName(pszId)>=0)
    {
        m_pdm->msg1(CG_E_PRGINSTNAME_IS_TASKNAME, pEdpId, pszId);
        //carry on: goto Ende;
    }


    //try to load the prg definition:
    pSymPrg = getSymtabForDType(pEdpTyp, pszTyp, ST_WANT_PRG);
    if(!pSymPrg)
        goto Ende;

    assert(pSymPrg->pouTyp==CG_POU_PRG && !pSymPrg->pLibCtx);

    //append new prg inst like a global FBI:
    pEntry = m_pSymtabPrv->registerDecl(
        pszId, 
        pEdpId, 
        CG_VAR_GLOBAL,
        retBitSet,
        (pOptAttr ? m_pFront->registerAttrList(
                  pOptAttr->convertToStruct()) 
                 : NULL),
        m_pFront->registerDType(pSymPrg),
        NULL, 
        NULL, //forosai
        NULL, 
        NULL
    ); //alles klar ?
    
    assert(pEntry);
    if(pEntry)
    {
   
        if(!m_pCurrTaskList || 
           !m_pCurrTaskList->addPrg(pOptTaskName->getName(), pEntry)
        )
        {
            m_pdm->msg1(CG_E_TASK_UNDEFINED,  
                pOptTaskName->getEdp(), pOptTaskName->getName());
        }
    }

Ende:
    delete(pPrgInstName);
    delete(pOptAttr);
    delete(pOptTaskName);
    delete(pPrgTypeName);
}



void st_ParseContext::GVLImport(
    const CG_Edipos* pEdp, 
    const TCHAR*     pszFile
)
{
    const CG_Symtab* pGVL;  
    const TCHAR*     pszExt;

    assert(pSymtab);

    //check file extension:
    pszExt = FC_StringGetFileExt(pszFile);
    if(lstrcmpi(pszExt, CG_PREDEF_SOURCE_GVL))
    {
        m_pdm->msg3(CG_E_IMPORT_BAD_EXTENSION, 
            pEdp, pszFile, CG_PREDEF_SOURCE_GVL, pszExt);
        return;
    }

    if(pSymtab->pouTyp!=CG_POU_FB && pSymtab->pouTyp!=CG_POU_PRG && 
       pSymtab->pouTyp!=CG_POU_FUN &&
        !(pSymtab->pouTyp==CG_POU_GVL && m_srcType==CG_SRC_TYPE_RES))
    {
        m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_POU_TYP, pEdp);
        return; //TOD var_global in functions would be sexy
    }

    pGVL = m_pSymtabPrv->registerGVLImp(pEdp, pszFile);
    if(pGVL && (pGVL->flags & CG_STAB_HDR_NOERR)==0)
        indirectError();
}


void st_ParseContext::RESImport(
    const CG_Edipos* pEdp, 
    const TCHAR*     pszFile
)
{
    const TCHAR*     pszExt;
    TCHAR            canrel[2*MAX_PATH];

    assert(m_pConfig && m_srcType==CG_SRC_TYPE_CON);

    //check file extension:
    pszExt = FC_StringGetFileExt(pszFile);
    if(lstrcmpi(pszExt, CG_PREDEF_SOURCE_RES))
    {
        m_pdm->msg3(CG_E_IMPORT_BAD_EXTENSION, 
            pEdp, pszFile, CG_PREDEF_SOURCE_RES, pszExt);
        return;
    }

    FC_StringCanonicalPath(pszFile, canrel, FC_ARRAY_LEN(canrel),
        m_pFront->m_4CP.getPrjPath(), FC_PATHM_KEEPREL);


    if(!m_pConfig->addResImp(pEdp, canrel))
    {
        m_pdm->msg1(CG_W_IMPORT_FILE_TWICE, pEdp, pszFile);
    }
    else
    {
        //check existence now:
        if(!FC_FileExists(m_pFront->m_4CP.getPrjPath(), pszFile))
            m_pdm->msg1(CG_E_FILE_NOT_EXIST, pEdp, pszFile);
    }
}



void st_ParseContext::endDeclSection(unsigned long ulEndSectionFpos)
{
    const TCHAR*   pszNextTok;
    const TCHAR*   pszNextText;
    long           nCharNext;
    const TCHAR*   psz;
    int            n;
    TCHAR          buf[UTIL_STS_MAX_TOKEN_LENGTH];
    UTIL_KW_ENUM   kwEnum;
    CG_Edipos      edptmp;
    bool           bNextDecl;
    
    if(!m_pSymtabPrv)
    {
        //can happen if #import before POU decl
        assert(getErrorsInFile());
        return;
    }

    if(CG_HasPOUTypBody(pSymtab->pouTyp))
    {
        bool bSecIsIFPart = true;
        
        if(!m_bDeclSecHasAddr)
        { 
            switch(m_currDeclScope)
            {
            case CG_DECLSCOPE_UNDEF://happens if called 
                                    //from function decl. or leading #import
            case CG_VAR_INPUT:
            case CG_VAR_OUTPUT:
            case CG_VAR_IN_OUT:
            case CG_VAR_EXTERNAL:
            case CG_VAR_GLOBAL:
            case CG_VAR_IN_EXT:
            break;
            default:
                bSecIsIFPart = false;
            }
        }//else if decl sec. has %-addr it's always part of interface

        //increment end of header and possible interface byte offset:
        m_pSymtabPrv->setNewHdrIFEnd(ulEndSectionFpos, bSecIsIFPart);
    }//else: at end of cst or gvl file it's set to file size...


    //next code is only to get error msg:
    pszNextText = UTIL_STS_GetTextAfterLastToken(m_pScanner, &nCharNext);
    pszNextTok = pszNextText;
    if(!UTIL_STS_BufferSkipComment(&pszNextTok, &nCharNext))
        return; //EOF in comment, will be the next error any way...

    //test by syntax if a next decl section follows
    //decl_section
    // : decl_scope
    //   opt_decl_list
    //   END_VAR
    // | VAR_NATIVE
    // | IMPORT_DIRECTIVE {pContext->symtabGvlImport(&$1);}
    // | DECL_CONST opt_decl_list DECL_END_CONST
    // ;
    
    //Note we are working on a 0-term string, see ST_LoadFileToMem
    //a 0-char inside the file will causes a syntax error anyway.


    //all people make the same error using the ';'
    if(!pszNextTok)
        pszNextTok = _T("");//TODO UTIL_STS_BufferSkipComment should not return NULL if text is null term

    psz = pszNextTok;
    while(*psz==_T(';') || _istspace(*psz))
        psz++;

    bNextDecl = false;
    if(*psz==_T('#'))
    {
        bNextDecl = FC_StringStartsWithI(psz, _T("#import"))!=0;
    }
    else
    {
        //get the keyword:
        n = 0;
        while((_istalpha(*psz)||*psz==_T('_')))
        {
            if(n>=FC_ARRAY_LEN(buf)-1)
                return; //definitely not a keyword and somthing strange....
            buf[n++] = *psz++; //TODO ++ on char, egal---
        }

        buf[n++] = 0;
        kwEnum = UTIL_IsIECKeyword(buf);
        switch(kwEnum)
        {
        case UTIL_KW_CONST:
        case UTIL_KW_VAR_NATIVE:
        case UTIL_KW_VAR:
        case UTIL_KW_VAR_INPUT:
        case UTIL_KW_VAR_OUTPUT:
        case UTIL_KW_VAR_IN_OUT:
        case UTIL_KW_VAR_EXTERNAL:
        case UTIL_KW_VAR_GLOBAL:
        case UTIL_KW_VAR_TEMP:
        case UTIL_KW_VAR_IN_EXT:
        case UTIL_KW_OVERLOAD: //also like decl
        case UTIL_KW_INLINE:   //also like decl 
             bNextDecl = true;
        break;
        default:
            ;//hit is the end of the header !
        }
    }

    //now do the conclusion:
    if(*pszNextTok==_T(';') && 
        (bNextDecl || !CG_HasPOUTypBody(pSymtab->pouTyp)))
    {
        //get line col info of this fuck colon:
        //we hunt it realy down!
        edptmp = m_currEdp;    
        assert(edptmp.typ == CG_EDP_ST);//must be ST edp!

        edptmp.lc.c += UTIL_STS_GetLengthOfLastToken(m_pScanner);    

        for(psz=pszNextText; psz<pszNextTok; psz=_tcsinc(psz))
        {
            edptmp.lc.c++;
            if(*psz=='\n')
            {
                edptmp.lc.c = 1;
                edptmp.lc.l++;
            }
        }
        m_pdm->msg1(CG_E_SYNTAX_UNEXPECTED, &edptmp, _T(";"));
    }
}

bool st_ParseContext::endOfDecls(bool bBodyFollows)
{
    bool bOk;
 
    if(!m_pSymtabPrv)
    {
        //can happen if syntax error before newPOU/CON/RES was called
        //due to the 'error' production in decl_sections:
        assert(getErrorsInFile()>0);
        return false; //ignore the body if the header as errors anyway
    }

    if(!CG_HasPOUTypBody(pSymtab->pouTyp))//if not has body e.g gvl, res, cst
        m_pSymtabPrv->setNewHdrIFEnd(UTIL_STS_GetLengthOfText(m_pScanner), true);

    assert(pSymtab->ulHdrBytes<=UTIL_STS_GetLengthOfText(m_pScanner));

    bOk = m_pSymtabPrv->endOfHeader(m_srcType, getErrorsInFile(), &m_currEdp);
    assert(!(bOk && getErrorsInFile()!=0));

    //Check if is ST input with a body and only the header is wanted
    //return false returns false.
    if(bBodyFollows && !m_pBody && (m_srcType==CG_SRC_TYPE_ST || m_srcType==CG_SRC_TYPE_IL))
    {
        //special thing: clean up the yacc lookahead 
        //that has allocated memory (is alwasy a ST_Identifier_C)
        if(m_pFirstHeapElement && getErrorsInFile()==0)
        {
            //assert should be only one otherwise this hacky
            //assumption is wrong?? 
            assert(m_pFirstHeapElement==m_pLastHeapElement);
            delete(m_pFirstHeapElement);
        }
        bOk = false; //skip the body.
    }
    return bOk;
}


static void ST_BuildGVLRefSymList(
    const CG_Symtab*                     pSym, 
    FC_CHashPtr<const CG_Symtab*, long>* pList
)
{
    int                i;
    const CG_DType*    pTyp;
    const CG_SymEntry* pEntry;
    
    for(pEntry=pSym->pFirst;pEntry;pEntry=pEntry->pNext)
    {
        pTyp = pEntry->pType;
        if(pTyp->pSym)
        {
            assert(pTyp->pSym->flags & CG_STAB_HDR_NOERR);
            pList->insert(pTyp->pSym, 0);
        }
    }

    for(i=0;i<pSym->nGVLImp;i++)
        ST_BuildGVLRefSymList(pSym->pGVLImp[i].pSym, pList);
}

void st_ParseContext::endOfResource(ST_CTaskList* pTasks)
{
    FC_CHashPtr<const CG_Symtab*, long> list;
    
    if(getErrorsInFile()!=0)
        return;

    if(pTasks)
    {
        m_pResource->nTasks = pTasks->countTasks(); 
        m_pResource->pTasks = m_pFront->registerTaskLists(
                            pTasks->convertToStruct());
        delete(pTasks);
    }
    else
    {
        //TODO warn no tasks 
        assert(m_pResource->nTasks==0);
    }

    endOfDecls(false);

    //get the used global prg and FB list
    ST_BuildGVLRefSymList(m_pResource->pGVL, &list);

    assert(!m_pResource->pzGVLRefSyms);
    m_pResource->pzGVLRefSyms = m_pFront->registerSymtabList(list.allocKeyArray(NULL));
    assert(m_pResource->pzGVLRefSyms);
}

void st_ParseContext::handleEmptyFile()
{
    const TCHAR* pszExp;
    FC_CString   jot;
    
    if(getErrorsInFile()>0)
        return; //can be e.g. unterminated comment 

    jot.loadRes(CG_RES_STR_EOF, m_pdm->getResHMODULE());
    switch(m_srcType)
    {
    case CG_SRC_TYPE_GVL:
    case CG_SRC_TYPE_CST:
        //assert: already done in parseAnyHeader:
        assert(m_pSymtabPrv);
        return; //empty file is ok

    case CG_SRC_TYPE_SDT:
        pszExp = _T("TYPE STRUCT ...");
    break;
    
    case CG_SRC_TYPE_4GR:
        jot.loadRes(CG_RES_STR_EMPTYHDR, m_pdm->getResHMODULE()); 
        //fall th..
    case CG_SRC_TYPE_HDR:
    case CG_SRC_TYPE_ST:
    case CG_SRC_TYPE_IL:
        pszExp = _T("PROGRAM|FUNCTION_BLOCK|FUNCTION");
    break;

    case CG_SRC_TYPE_RES:
        pszExp = _T("RESOURCE");
    break;

    case CG_SRC_TYPE_CON:
        pszExp = _T("CONFIGURATION");
    break;

    default:
        assert(!"bad file type");
    }

    m_pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, &m_currEdp, 
      pszExp, jot); 

    //sys comment is hanging unchecked TODO
    if(m_pSysCommentTMP)
    {
        delete(m_pSysCommentTMP);
        m_pSysCommentTMP = NULL;
    }
}




static void ST_WarnUnusedVariables(
    CGT_CDiagMsg*    pdm,
    const CG_Symtab* pSym,
    const ST_CBody*  pBdy,
    const CG_Edipos* pEdpEndBdy
)
{
    int                                   nUnrefs;
    FC_CString                            unref;
    const CG_SymEntry*                    pEntry;  
    const CG_SymEntry*                    pEntryFirstUnRef;

    //m_edpEnd must be set!
    assert(pBdy->m_edpEnd.pszFile && pBdy->m_edpEnd.pszFile[0]);

    pEntry = pSym->pFirst;

    //the fun ret: may be void and will get a special warning 
    if(pSym->pouTyp==CG_POU_FUN)
    {
        if(pEntry->pType->type!=CG_VOID && (pEntry->accFlags & CG_ACCESS_ANY_W)==0)
            pdm->msg0(CG_W_FUN_NOT_ASSIGN_RET, pEdpEndBdy);
    
        pEntry = pEntry->pNext;
    }

    nUnrefs = 0;
    pEntryFirstUnRef = NULL;
    for(;pEntry; pEntry=pEntry->pNext)
    {
        CG_DECLSCOPE ds = pEntry->declScope;

        if(ds==CG_VAR || ds==CG_VAR_TEMP || ds==CG_VAR_EXTERNAL)
        {
            long accBits = pEntry->accFlags;

            if(accBits==0)
            {
                nUnrefs++;
                if(unref.len()==0)
                {
                    pEntryFirstUnRef = pEntry;
                    unref.resize(MAX_PATH);
                    unref<<pEntry->pszId;
                }
                else if(nUnrefs<=4)
                {
                    unref<<_T(",")<<pEntry->pszId;
                }
                else if(nUnrefs==5)
                {
                    unref<<_T(",...");
                }

            }
            else if(ds!=CG_VAR_EXTERNAL && pEntry->pType->pSym && 
                    pEntry->pType->pSym->pouTyp==CG_POU_FB)
            {
                 //|CG_ACCESS_c: array of FB, CG_ACCESS_X,x: passed as inout, assume ok.
                if((accBits & (CG_ACCESS_C|CG_ACCESS_c|CG_ACCESS_X|CG_ACCESS_x))==0)
                    pdm->msg1(CG_W_FBI_NOTCALLED, &pEntry->edp, pEntry->pszId);
            }
        }
    }

    if(unref.len()>0)
        pdm->msg1(CG_W_VARIABLE_UNUSED, &pEntryFirstUnRef->edp, unref);

}


void st_ParseContext::endOfPou(
    const CG_Edipos*  pEdp,
    CG_POUTYP         endMarkerFound
)
{
    //check the endmarker:
    if(m_srcType==CG_SRC_TYPE_ST || m_srcType==CG_SRC_TYPE_IL)
    {
        if(pSymtab->pouTyp != endMarkerFound)
        {
            const TCHAR* pszEndFound = _T("");

            if(endMarkerFound!=CG_POUTYP_UNDEF)
                pszEndFound = ST_PouTypeName(endMarkerFound, true);

             m_pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, pEdp,
                ST_PouTypeName(pSymtab->pouTyp, true),
                pszEndFound);
        }
    }
    else//hdr or 4gr file:
    {
        if(endMarkerFound!=CG_POUTYP_UNDEF)
            m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_FILE, pEdp);
    }



    if(!m_pBody)
    {
        return; //can happen if hdr file or parse only wanted 
                //header portion, in the later case m_pBody
                //was not allocated and set to zero on entry.
    }

    if((pSymtab->flags & CG_STAB_HAS_BDY)==0)
    {
        //this can happen only if inline since we don't know 
        //this prefore parsing the m_pBody was allocated
        //but must be unused, assert this:
        assert(pSymtab->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR));
        assert(m_pBody->isUnused());
        return; 
    }

    //reach here if there is a real body (graphics or ST):
    

//final actions for body:
  //body end edp:
    m_pBody->m_edpEnd = *pEdp;

  //body beg edp:
    memset(&m_pBody->m_edpBeg, 0, sizeof(m_pBody->m_edpBeg));
    if(m_srcType==CG_SRC_TYPE_4GR)
    {
        m_pBody->m_edpBeg.typ     = CG_EDP_4GR_BEG;
        m_pBody->m_edpBeg.pszFile = m_pszFile;
    }
    else if(!m_pSFC && m_pBody->getStmts())
    {
        m_pBody->m_edpBeg = m_pBody->getStmts()->edp;
    }
    else
    {
        assert(m_pSFC || m_srcType == CG_SRC_TYPE_IL || getErrorsInFile()>0);
        m_pBody->m_edpBeg = pSymtab->edpEndHdr;
        //TODO use edp of first STEP|TRANSITIO|... keyword if was SFC?
    }

   
  //finalise the SFC check and build the COM IF body data:
    if(m_pSFC)
    {
        assert(pSymtab);
        if(getErrorsInFile()==0)
            m_pSFC->endOfSFC(pEdp);
        delete(m_pSFC);
        m_pSFC = NULL;
    }
    //else
    //if not SFC: the COM IF structs are already done in m_pBody.
    //Simply the ST_CBody::m_pFirstStmt should be!=NULL if no error.



  //Now do some final actions that should not be redone on reload body
  //A reload body is detected by checking the symtab's CG_STAB_BDY_LOADED bit:
    if(pSymtab->flags & CG_STAB_BDY_LOADED)
        return; //can happen if body gets reloaded.

  //finally do warn unused vars:
    if(getErrorsInFile()==0) 
        ST_WarnUnusedVariables(m_pdm, pSymtab, m_pBody, &m_pBody->m_edpEnd);
    
  //get language info bits:
    long langBits;
    if(m_pBody->isSFC())
    {
        langBits = CG_STAB_BDY_IS_SFC;
    }
    else if(m_srcType==CG_SRC_TYPE_ST)
    {
        langBits = CG_STAB_BDY_IS_ST;
    }
    else if(m_srcType==CG_SRC_TYPE_IL)
    {
        langBits = CG_STAB_BDY_IS_IL;
    }
    else if(m_srcType==CG_SRC_TYPE_4GR)
    {
        const CG_STStmt* pFirst = m_pBody->getStmts();
        
        assert(!pFirst || pFirst->styp==CG_LDFBDNETWORK);
        if(pFirst && pFirst->styp==CG_LDFBDNETWORK)
        {
            langBits = pFirst->netw.lang;
        }
        else
        {
            //TODO if no network must gues the lang, better use the info stored by 
            //editor in thr 4gr XML ? 
            langBits = CG_STAB_BDY_IS_FBD;
        }
    }
    else
    {
        //Note: add on filters must return an st or 4gr and this must 
        //be the file name that passes here not the original one!
        assert(!"bad source type enum neither st nor 4gr");
    }
   
    //check supported language:
    if((langBits & CG_STAB_BDY_IS_SFC) && !m_pFront->m_features.m_pCFeat->bSFC)
    {
        m_pdm->errNS1(&m_pBody->m_edpBeg, m_pdm->res1(CG_RES_STR_SFC_LANG));
    }
    else if((langBits & CG_STAB_BDY_IS_FBD) && !m_pFront->m_features.m_pCFeat->bFBD)
    {
        m_pdm->errNS1(&m_pBody->m_edpBeg, m_pdm->res1(CG_RES_STR_FBD_LANG));
    }
    else if((langBits & CG_STAB_BDY_IS_LD) && !m_pFront->m_features.m_pCFeat->bLadder)
    {
        m_pdm->errNS1(&m_pBody->m_edpBeg, m_pdm->res1(CG_RES_STR_LADDER_LANG));
    }

    
    //even if error call endOfBody: 
    m_pSymtabPrv->endOfBody(getErrorsInFile()!=0, langBits);
}



extern "C" static bool ST_AddressScanHelp(
    const CGT_CHECK_ADDR_TYP* pContext, 
    const TCHAR* psz, 
    int nChars, 
    const TCHAR* pszExpect
)
{
    int               i;
    CG_Edipos         edp2;
    ST_Identifier_C*  pId;
    __int64           ival = 0;
    const TCHAR*      pszExpanded;
    TCHAR             buffer[3*UTIL_MAX_ID_LENGTH];
    ST_Expression_C*  pExpr;
    st_ParseContext*  pCtx = (st_ParseContext*)(pContext->pContext);

    assert(nChars>0);

    if(pszExpect)
    {
        edp2 = *pCtx->getCurrEdp();
        CG_IncColEdipos(&edp2, psz - pContext->pszAddr);
        pCtx->getDM()->msg2(CG_E_ADDR_SYNTAX, &edp2, pContext->pszAddr, pszExpect);
        return false;
    }

    if(nChars<=0)
        return false;


    if(_istalpha(*psz) || *psz==_T('_'))
    {
        for(i=0;i<nChars && i<FC_ARRAY_LEN(buffer)-2;i++)
            buffer[i] = psz[i];
        
        buffer[i] = 0;

        edp2 = *pCtx->getCurrEdp();
        CG_IncColEdipos(&edp2, psz - pContext->pszAddr);
        if(!ST_CheckIdentifier(pCtx->getFront()->m_pdm, &edp2, buffer))
            return false;

        pId = new ST_Identifier_C(&edp2, pCtx, buffer);
        
        //must be a const, get the const:
        pExpr = pCtx->createVar(pId);
        pExpr->fixType();

        bool bErr = false;
        if(pExpr->isErrorExpr())
        {
            bErr = true;
        }
        else if(!pExpr->getLitValue())
        {
            pCtx->getDM()->msg1(CG_E_NOT_A_CONST, pExpr->getEdp(), buffer);
            bErr = true;
        }
        else if(!pExpr->isAnyIntLiteral(&ival) || ival<0 || ival>LONG_MAX)
        {
            pCtx->getDM()->msg2(CG_E_BAD_CONST_IN_ADDR, 
                pExpr->getEdp(), buffer, pCtx->getDM()->jot1()<<ival);
            bErr = true;
        }

        TCHAR itoabuff[128];
        _itot((long)ival, itoabuff, 10);
        pszExpanded = itoabuff;
        for(;*pszExpanded;)
            pCtx->m_addrExpansionResult.add(*pszExpanded++);
                    
        delete(pExpr);
        return !bErr;
    }
    else if(*psz==_T('!'))
    {
        for(i=0;i<nChars && i<FC_ARRAY_LEN(buffer)-2;i++)
            buffer[i] = psz[i];
        
        buffer[i] = 0;
        _tcslwr(buffer);
        for(i=0;buffer[i];i++)
            pCtx->m_addrExpansionResult.add(buffer[i]);

        int errID = 0;
        if(!(CGT_IsFullBasicAddress(pContext->pszAddr) || 
             CGT_IsIncompleteAddressRef(pContext->pszAddr))
           )
        {
            errID = CG_E_ADDR_ILL_MODIFIER;
        }
        else if(!_tcsicmp(_T("!swap"), buffer) || !_tcsicmp(_T("!noswap"), buffer))
        {
            if(CGT_GetAddrBitSize(pContext->pszAddr)<=8)
                errID = CG_E_ADDR_ILL_MODIFIER_SWAP;
        }
        
        if(errID) 
        {
            edp2 = *pCtx->getCurrEdp();
            CG_IncColEdipos(&edp2, psz - pContext->pszAddr);
            pCtx->getFront()->m_pdm->msg2(errID, &edp2, pContext->pszAddr, buffer);
            return false;
        }
    }
    else
    {
        for(i=0;i<nChars;i++)
        {
            if(psz[i]!=_T('"'))//HACK ???????????????? OPC Addr
                pCtx->m_addrExpansionResult.add(psz[i]);
        }
    }

    return true;
}

ST_DirectVar_C* st_ParseContext::getDirectAddr(const TCHAR* pszDirectAddr)
{
    CGT_CHECK_ADDR_TYP context;
    ST_DirectVar_C*    pDirect;

    context.pContext = this;
    context.AddressScanHelp = ST_AddressScanHelp;

    m_addrExpansionResult.resetUsed();

    //check the bare syntax and expand all CONST:
    if(!CGT_CheckDirectAddrSyntax(pszDirectAddr, NULL, &context))
    {
        assert(getErrorsInFile()>0);
        return NULL;
    }

    m_addrExpansionResult.add(0);

    // fill in missing X into direct address
    assert(m_addrExpansionResult.getUsed()>=3);
    if (_istalpha(m_addrExpansionResult.get(1)) && !_istalpha(m_addrExpansionResult.get(2)))
    {
        //make space 
        m_addrExpansionResult.add('x');
        for (int mov=m_addrExpansionResult.getUsed()-1; mov>2; mov--)
        {
            m_addrExpansionResult.set(mov, m_addrExpansionResult.get(mov-1));
        }
        m_addrExpansionResult.set(2, _TCHAR('X'));
    }


    pDirect = new ST_DirectVar_C(
        getCurrEdp(), 
        this, 
        m_addrExpansionResult.getElements()
    );
    return pDirect;
}

//forosai >>
ST_DirectVar_C* st_ParseContext::directAddrFromSysVar(const CG_Edipos* pEdp, const TCHAR* pszSysvar)
{
	ST_DirectVar_C* pDirect = NULL;
	ICG_AutoDecl*   pAD = m_pFront->m_features.getAutoDeclIF(this->m_pdm);

	if(pAD)
	{
		BSTR sSysId = FC_StringToBSTR(pszSysvar);
        BSTR sIECAddr = NULL;
        BSTR sIECType = NULL;
		BOOL bReadOnly;
		HRESULT hr = pAD->getAddressInfo(sSysId, &sIECAddr, &sIECType, &bReadOnly);
		
		FC_FREE_BSTR(sIECType); //ignore the data type, simple assume match to X,B,W,D,L

		if(sIECAddr && hr==S_OK)
		{
			FC_CStr iecArddr(sIECAddr);
			pDirect = new ST_DirectVar_C(pEdp, this, iecArddr);
		}
		else
		{
			m_pdm->msg2(CG_E_INVALID_ADDR, pEdp, pszSysvar, _T("undefined system variable"));
		}
		FC_FREE_BSTR(sIECAddr);
	}
	else if(!m_pFront->m_bFileCheckMode)
	{
		m_pdm->errNS1(pEdp, m_pdm->res1(CG_RES_NS_AT_SYSVAR));
	}
	//else: may happen if only "check" a gvl, in this case 
	//      simply ignore the addr without diag
	
	return pDirect;
}
//forosai <<

ST_Expression_C* st_ParseContext::checkDirectBodyVar(
    ST_Expression_C* pDirect
)
{
    if(!pDirect)
    {
        assert(getErrorsInFile()>0);
        pDirect = new ST_ErrorExpr_C(getCurrEdp(), this, CG_TBS_ANY_BIT);
    }
    
    if(!pSymtab || pSymtab->pouTyp!=CG_POU_PRG)
        m_pdm->msg0(CG_E_DIRECTVAR, pDirect->getEdp());

    //currently we can't handle %add in bodies 
    m_pdm->msg1(CG_E_NOT_SUPPORTED, pDirect->getEdp(), " direct variable inside body ");
    return pDirect;
}



ST_Expression_C* st_ParseContext::createVar(
    ST_Identifier_C*  pId
)
{
    const TCHAR*        pszId;
    const CG_Edipos*    pEdp;
    const CG_SymEntry*  pEntry;
    ST_Expression_C*    pVar;
    CG_DECLSCOPE        store;

    
    pszId  = pId->getName();
    pEdp   = pId->getEdp();
    pVar   = NULL;
    pEntry = NULL;

    if(m_pSymtabPrv)
    {
        //CAUTION: if context is a resouce task inits := <expr>
        //may have var references two, but don't auto decl a VAR_EXTERNAL
        //from an #import , don't use lookUpDeepAndInsertGVImp.
        if(m_pResource)
        {
            assert(m_srcType==CG_SRC_TYPE_RES && pSymtab->pouTyp==CG_POU_GVL);
            pEntry = m_pSymtabPrv->lookUpEntryDeep(pszId, NULL);
        }
        else
        {
            pEntry = m_pSymtabPrv->lookUpDeepAndInsertGVImp(pszId);
        }
    }

    //lookup global CONST if not found in symtab of POU:
    if(!pEntry)
    {
        pEntry = m_pFront->lookUpCONST(pszId);
        if(pEntry && m_pSymtabPrv && pSymtab)
        {
            if((pSymtab->flags & CG_STAB_BDY_LOADED)==0)//HACK, used cst should be stored in body not symtab !!
                m_pSymtabPrv->registerUsedGlobCONST(pEntry);
            else
                assert(pSymtab->pzRefGlbCst);
        }
    }
        
    
    store = pEntry ? pEntry->declScope : CG_DECLSCOPE_UNDEF;

    if(store==CG_CONST)
    {
        if(m_pSymtabPrv && pSymtab)
        {
            if((pSymtab->flags & CG_STAB_BDY_LOADED)==0)//HACK, used cst should be stored in body not symtab !!
                m_pSymtabPrv->registerCONSTRef(pEntry, pEdp);
        }
        
        assert(pEntry->pInit);
        if(pEntry->pInit)
        {
            //check if type is supported, this must be done on use of const
            //not when the cst file was loaded, see CG_CIECFront::loadCST.
            if(!m_pFront->m_features.checkBasicType(pEntry->pType->tbs))
            {
                m_pdm->errNS3(pEdp, 
                    m_pdm->res1(CG_RES_STR_DATATYPE)<<pEntry->pType->pszName,
                    m_pdm->res2(CG_RES_STR_USEDCONST)<<_T("'")<<pEntry->pszId<<_T("'"),
                    m_pdm->res3(CG_RES_STR_FROM)<<m_pdm->edpToStr(&pEntry->edp, m_pdm->jot3())<<_T(")")
                  );
                //carry on...
            }


            //clone the lit:
            ST_BINVAL_TYP* pValCopy = new ST_BINVAL_TYP;
            assert(pEntry->pInit->ut == CG_INIT_SIMPLE);
            ST_InitBinValFrom(pValCopy, pEntry->pInit->sim.pLit);
            pVar = new ST_Literal_C(
                pEdp,
                this,
                pValCopy,  
                pEntry->pType->tbs, 
                false,
                pEntry
            );
        }
    }
    else if(pEntry)
    {
        //check: polymorph typed variables (e.g. function inputs, 
        //FB in_outs) may only be accessed inside NATIVE..END_NATIVE;
        //therefore no ST_Variable_C may be created
        if(!CG_IsMonomorph(pEntry->pType->type))
        {
            m_pdm->msg2(CG_E_POLYTYPE_VAR_USE, pEdp,
                pszId, pEntry->pType->pszName);
            //fake: 
            ST_TypeSet_C errTyp;
            errTyp.initFromCG_DType(pEntry->pType);
            pVar = new ST_ErrorExpr_C(pEdp, this, errTyp);
        }
        else
        {
            //normal variable:
            bool bLvalue = !(pEntry->declMod & CG_DMOD_CONSTANT) && 
                            store!=CG_VAR_INPUT &&
                            store!=CG_VAR_IN_EXT &&
                            (!pEntry->pszAddr || CGT_GetAddrLocation(pEntry->pszAddr)!=_T('I')) &&
                           !(m_pbt && m_pbt->isActiveForVar(pszId));

            pVar = new ST_Variable_C(pEdp, this, bLvalue, pEntry);
        }
    }
    else if(m_pSFC)
    {
        // no entry in symbol table and no global const - is pou sfc
        // is it a step or a action
        int sNr = m_pSFC->isStep(pszId);
        int aNr = m_pSFC->isAction(pszId);

        assert(!(sNr!=-1 && aNr!=-1));
        if (sNr!=-1)
        {
            pVar = new ST_StepAction_C(pEdp, this, sNr, pszId, false);
        }
        else if (aNr!=-1)
        {
            pVar = new ST_StepAction_C(pEdp, this, aNr, pszId, true);
        }

        if (sNr==-1 && aNr==-1)
        {
            m_pdm->msg1(CG_E_UNDEF_VAR, pEdp, pszId);
            //fake: 
            pVar = new ST_ErrorExpr_C(pEdp, this, CG_TBS_DINT);
        }
    }
    else
    {
        m_pdm->msg1(CG_E_UNDEF_VAR, pEdp, pszId);
        //fake: 
        pVar = new ST_ErrorExpr_C(pEdp, this, CG_TBS_DINT);
    }

    delete(pId);
    return pVar;
}


ST_Expression_C* st_ParseContext::createOp(
    const CG_Edipos* pEdp,
    CG_EXPRTYP       op,
    ST_Expression_C* pFirstArg,
    ST_Expression_C* pSecondArg
)
{
    ST_CParamList*             args;//##AC
    ST_Function_C*             pResult;
    ST_Expression_C*           pFake;
    ST_Literal_C*              pLitVal;
    const ST_FUNCTION_DEF_TYP* pOverloads;

    // HACK?
    if(op == CG_UMINUS && !pSecondArg && pFirstArg &&
       pFirstArg->toLiteralClass() && pFirstArg->toLiteralClass()->isPureLiteral() &&
       pFirstArg->GetType().GetTypeSet()==(CG_TBS_LWORD) &&
       pFirstArg->getLitValue()->any64==0x8000000000000000)
    {
        ST_BINVAL_TYP*  pValue = new ST_BINVAL_TYP;
        ST_InitBinValI64(pValue, 0x8000000000000000);
        pLitVal = new ST_Literal_C(pEdp, this, pValue, CG_TBS_LINT, 
          false, NULL);
        delete pFirstArg;
        return pLitVal;
    }

    args = new ST_CParamList(pEdp, this, 2);//##AC
    args->addPosParam(pFirstArg);//##AC
    if(pSecondArg)
        args->addPosParam(pSecondArg);//##AC


    pOverloads = ST_LookUpOperatorDefinition(op, m_srcType==CG_SRC_TYPE_IL && m_IlContext.m_bParseBody);
    if(!pOverloads)
        CGT_IERR(m_pdm, _T("no buildin operator definition found"));

    pResult = new ST_Function_C(pEdp, this, op, args, NULL, pOverloads);//##AC

    if(!pResult->selectOverload())
    {
        //fake on error:
        assert(getErrorsInFile()>0);
        
        //CAUTION: do not delete(result), the caller may hold 
        //pointers the the arglist.
        //The lost objects will be collected anyway!
        pFake = new ST_ErrorExpr_C(pEdp, this, pResult->GetType());
        return pFake;
    }

    
    //on the fly constant folding 
    bool  bDivByZ = false;
    const ST_Literal_C* pLit1 = pFirstArg->toLiteralClass();
    const ST_Literal_C* pLit2 = pSecondArg ? pSecondArg->toLiteralClass() : NULL;

    if(op==CG_DIV && pLit2)
    {
        if(pLit2->GetType().GetTypeSet() & CG_TBS_ANY_REAL)
        {
            if(_tcstod(pLit2->getLitValue()->pText, NULL) == 0.0)
                bDivByZ = true;
        }
        else if(pLit2->getLitValue()->any64 == 0)
        {
            bDivByZ = true; 
        }
        //fake: don't try to eval literals:
    }

    if(bDivByZ)
    {
        m_pdm->msg0(CG_E_DIVIDE_BY_ZERO, pEdp); 
    }
    else if(pLit1 && (!pSecondArg || pLit2))
    {
        ST_BINVAL_TYP*  pLitResult = NULL;
        CG_BITSET       lResultType = pResult->GetType().GetTypeSet();  

        pLitResult = ST_LitBinValEval(m_pdm, pEdp, op, pLit1, pLit2, &lResultType);
        if(pLitResult)
        {//TODO loose ref CONSTs this way from body and header debug info !!!
            pLitVal = new ST_Literal_C(pEdp, this, pLitResult, 
                lResultType, false, NULL);
            delete pResult;
            return pLitVal;
        }
    }
    
    return pResult;
}


//##AC beg
ST_Expression_C* st_ParseContext::expandInline(
    const CG_Edipos*     pEdpOP,
    CG_EXPRTYP           openum,
    int                  nArgs,
    ST_PARAMSPEC*        pArgs
)
{
    //function is in fact a buildin operator: HACK? same problems as V13
    int                  iOldErr = getErrorsInFile();
    ST_Expression_C*     pResult = NULL;
    ST_Expression_C*     pFirst  = NULL;
    ST_Expression_C*     pSec    = NULL;
    int                  iArg;

    if(nArgs>0)
    {
        pFirst = pArgs[0].pActual;
        iArg = 1;
    }
    if(nArgs>1)
    {
        pSec = pArgs[1].pActual;
        iArg = 2;
    }
    if(nArgs>2)
    {
        switch(openum)
        {
        case CG_EQ:
        case CG_NE:
        case CG_LT:
        case CG_LE:
        case CG_GT:
        case CG_GE:
            //TODO: GT,LE for >2 args
            m_pdm->msg1(CG_E_NOT_SUPPORTED, pArgs[2].pActual->getEdp(), 
                m_pdm->res1(CG_RES_STR_GT_VAR_EXT));
            delete(pFirst);
            delete(pSec);
            return NULL;
        break;
        default:
            ;
        }
    }


    if(pFirst)
    {
        //special case for ladder move: simply use the () noop to avoid any eval order conflicts:
        //the move itself is made when the fun return gets assigned.
        if(openum==CG_MOVE)
            openum = CG_NOOP;

        pResult = createOp(pFirst->getEdp(), openum, pFirst, pSec);
        for( ; iArg<nArgs; iArg++)
        {
            assert(!pArgs[iArg].bOutputArg);
            pFirst= pResult;
            pSec = pArgs[iArg].pActual;
            pResult = createOp(pFirst->getEdp(), openum, pFirst, pSec);
        }
    }
//##AC end
    if(!pResult || getErrorsInFile()!=iOldErr)
    {
        m_pdm->msg0(CG_E_MACRO_EXPANSION, pEdpOP);
        if(pResult)
        {
            delete(pResult);
            pResult = NULL;
        }
    }
    return pResult;
}

//##AC removed createFunCall


ST_Expression_C* st_ParseContext::checkUPlusMinus(
    const CG_Edipos* pEdp,
    ST_Expression_C* pExpr, 
    bool             bMinus
)
{
    if(bMinus)
    {
        return createOp(pEdp, CG_UMINUS, pExpr, NULL);
    }
    else if(pExpr->GetType().GetTypeSet() & CG_TBS_ANY_SIGNED)
    {
        return pExpr;
    }

    m_pdm->msg2(CG_E_UNARY_PLUSMINUS_ON_EXPR, 
             pEdp,
             bMinus?_T("-"):_T("+"), 
             ST_TypeSetToErrMsgString(pExpr->GetType()));

    return pExpr;
}

//##AC beg
ST_Expression_C* st_ParseContext::newCallExpr(
    const CG_Edipos* pEdp,
    const TCHAR*     pszCalee,
    CG_POUTYP        caleeTyp,
    ST_CParamList*   pParList 
)
{
    const CG_SymEntry*         pFBI;
    ST_CFBCallExpr*            pFBICall;
    ST_CSymtabPrv*             pSymFunPrv;
    const CG_Symtab*           pSymFun;
    long                       lres;
    const ST_FUNCTION_DEF_TYP* pBuildInFun;
    ST_Function_C*             pFunExpr;
    CG_EXPRTYP                 openum;
    ST_CFunction*              pFunExpr2;
    pFBI = NULL;
 
    
    if(caleeTyp==CG_POUTYP_UNDEF)
    {
        //Handle the ambigous case: fun or fb ? 
        //xyz(in1:=a,out1=>b) syntax assume FB call, may change here if we support fun called with named list.
        //problem is xyz() then pParList==NULL, assume FB call too:
        if(!pParList || pParList->m_bNamed)
            pFBI = m_pSymtabPrv->lookUpDeepAndInsertGVImp(pszCalee);

        if(pFBI)
        {
            if((pFBICall = new ST_CFBCallExpr(pEdp, this, pFBI, pParList))->checkFBCall())
                return pFBICall;
        
            delete(pFBICall);
            goto Error;
        }
        else if(pParList && pParList->m_bNamed)
        {
            //do not try a function if syntax is FB:
            m_pdm->msg1(CG_E_UNDEF_VAR, pEdp, pszCalee); //TODO better diag: undefined function block instance
            goto Error;
        }
    }
    else if(caleeTyp==CG_POU_FB)
    {
        pFBI = m_pSymtabPrv->lookUpDeepAndInsertGVImp(pszCalee);
        if(pFBI)
        {
            if((pFBICall = new ST_CFBCallExpr(pEdp, this, pFBI, pParList))->checkFBCall())
                return pFBICall;
        
            delete(pFBICall);
            goto Error;
        }
        else
        {
            m_pdm->msg1(CG_E_UNDEF_VAR, pEdp, pszCalee); //TODO better diag: undefined function block instance
            goto Error;
        }
    }
    //else: try function;




    //try function:
    lres = m_pFront->loadHeaderEx(pEdp, pszCalee, ST_WANT_FUN, &pSymFunPrv, &pBuildInFun);
    
    //if trg typ not supported error, carry on. All other errors ignore the fun call:
    if(lres!=ST_LHDR_OK && lres!=ST_LDHD_TRG_NS)
    {
        if(lres & ST_LDHD_ERRIND) //indirect error update internal err counter
            indirectError();
        delete(pParList);
        goto Error;
    }


    if(pParList && pParList->m_bNamed)
    {
        //named param list not supported (for funs):
        m_pdm->errNS1(pParList->getEdp(), m_pdm->res1(CG_RES_STR_FCT_WRONG_PARAM));
        delete(pParList);
        goto Error;
    }


    if(pBuildInFun)
    {
        assert(!pSymFunPrv);
        //is build in, TODO for buildins m_callFun.insert() ???
        pFunExpr = new ST_Function_C(pEdp, this, CG_BUILDIN, pParList, NULL, pBuildInFun);
        if(pFunExpr->selectOverload())
            return pFunExpr;

        delete(pFunExpr);
        goto Error;
    }


    //non build in function call, update the used list in m_pBody and m_pSymtabPrv:
    pSymFun = pSymFunPrv->getCGSym();
    assert(pSymFun->pouTyp==CG_POU_FUN);

    if(m_pBody)//there is no body if parse singe expr is used...
        m_pBody->m_callFun.insert(pSymFun, 0);
    
    if((pSymtab->flags & CG_STAB_BDY_LOADED)==0)
    {
        m_pSymtabPrv->registerUsedType(pSymFun);
    }
    else
    {
        //if reload body the info must not change:
        assert(pSymtab->pzRefSyms);
#ifdef DEBUG
        if(pSymtab->pzRefSyms)
        {
            for(const CG_Symtab*const* ppSym = pSymtab->pzRefSyms; *ppSym; ppSym++)
            {
                if(pSymFun == *ppSym)
                    break;
            }
            if(pSymFun != *ppSym)
                CGT_IERR2(m_pdm, pEdp, m_pdm->jot1()<<_T("reload body: new reference to function '")<<pSymFun->pszId<<_T("' (body changed?)"));
        }
#endif 
    }
    
    pFunExpr2 = new ST_CFunction(pEdp, this, pSymFunPrv, pParList);
    if(!pFunExpr2->checkCall())
    {
        delete(pFunExpr2);
        goto Error;
    }
    
    openum = pSymFunPrv->isOperator();
    if(openum == CG_EXPRTYP_UNDEF)
    {
        return pFunExpr2;
    }
    else
    {
        //is AND, OR operator, we used the pFunExpr only tmp. for type checking:
        FC_CGArray<ST_PARAMSPEC> tempList;
        pFunExpr2->getArgListAndDel(&tempList);
        ST_Expression_C* pInlExpr = expandInline(pSymFunPrv->getOperatorEdp(), openum, tempList.getUsed(), tempList.getElements());
        if(!pInlExpr)
            goto Error;
        return pInlExpr;
    }

Error:
    //TODO: warn ambigous call FB or fun problem on error here, may be the type errors are not the real problem..
    return new ST_ErrorExpr_C(pEdp, this, NULL);

}
//##AC end

ST_CSFCBody* st_ParseContext::addSFC()
{
    if(!m_pSFC)
    {
        m_pBody->setSfcBody();
        m_pSFC = new ST_CSFCBody(this, m_pSymtabPrv, m_pBody);
    }
    return m_pSFC;
}



CG_STStmt* st_ParseContext::newEmptyStmt(
    const CG_Edipos*  pEdp
)
{
   CG_STStmt* pStmt;
   pStmt = m_pBody->newStmt(CG_EMPTY, pEdp);
   return pStmt;
}



CG_STStmt* st_ParseContext::newAssignStmt(
    const CG_Edipos*  pEdp,  //id NULL it's called for an internal generated assign
    ST_Expression_C*  pLval, 
    ST_Expression_C*  pExpr
)
{
    CG_STStmt*         pStmt = NULL;
    const CG_Symtab*   pSym;
    ST_Expression_C*   pTmp;
    CG_ExprTree        tmp;

    if(!pEdp)
    {
        //use the lval's edipos (this will happen if called from 4gr):
        pEdp = pLval->getEdp();
    }
    
    //TODO checkUnreachableStmt(pEdp);

    pTmp = createOp(pEdp, CG_MOVE, pLval, pExpr);
    pTmp->makeCGExpr(&m_pBody->m_exprMem, &tmp);
    if(tmp.extyp==CG_MOVE)
    {
        pStmt = m_pBody->newStmt(CG_ASSIGN, pEdp);
        pStmt->asns.pLhs = tmp.ops.pLhs;
        pStmt->asns.pRhs = tmp.ops.pRhs;

        pSym = pStmt->asns.pLhs->pType->pSym;
        if(pSym && pSym->flags & CG_STAB_DIAG_ON_COPY)
        {
            //special case: tmp var of ref type is no deep copy!
            if(!(tmp.ops.pLhs->extyp==CG_TMPVAR && tmp.ops.pLhs->tmp.pRefExpr))
                ST_OnDeepCopy(m_pFront, m_pdm, pEdp, pSym);
        }
    }
    else
    {
        assert(getErrorsInFile());
    }
    delete(pTmp);
    return pStmt;
}





CG_STStmt* st_ParseContext::newLabel(
    const CG_Edipos* pEdp,
    const TCHAR*     pszLbl, 
    const CG_STStmt* pStmt
)
{
    CG_STStmt*       pStmtLbl;
    ST_CJumpLabel*   pJL;

    if(m_pbt->for_stmts||m_pbt->while_stmts||m_pbt->repeat_stmts||
       m_pbt->case_stmts||m_pbt->if_stmts||m_pbt->sync_stmts)
    {
        m_pdm->msg1(CG_E_LABEL_IN_CONTROL_STMT, pEdp, pszLbl); 
    }

    
    pStmtLbl = m_pBody->newStmt(CG_LABEL, pEdp);
    pStmtLbl->pNext = pStmt;
    //TODO: pJL = checkLabel(pLabelName, &labelNum);
    pStmtLbl->lbls.pszLbl = m_pBody->m_exprMem.registerString(pszLbl);
    pStmtLbl->lbls.nLblNum= m_pbt->nlabels++;
    pJL = m_pbt->labels.get(pszLbl);
    if(!pJL)
    {
        //label first seen
        pJL = new ST_CJumpLabel();
        pJL->m_pDef = pStmtLbl;
        m_pbt->labels.insert(pszLbl, pJL);
    }
    else if(pJL->m_pDef)
    {
        //label already defined:
        m_pdm->msg1(CG_E_LABEL_DUPLY, pEdp, pszLbl); 
    }
    else 
    {
        //label already used in goto:
        assert(pJL->m_gotos.getUsed()+pJL->m_lblrefex.getUsed()>0);//##AC
        pJL->m_pDef = pStmtLbl;
    }


    return pStmtLbl;
}


CG_STStmt* st_ParseContext::newIfElseStmt(
    const CG_Edipos* pEdp,
    ST_Expression_C* pIfExpr, 
    const CG_STStmt* pThen,
    const CG_STStmt* pElse
)
{
    CG_STStmt*       pStmt = NULL;

    pIfExpr->checkTypedExpr(CG_TBS_BOOL, true);

    if(getErrorsInFile()==0)
    {
        pStmt = m_pBody->newStmt(CG_IF, pEdp);
        pStmt->ifs.pCond = pIfExpr->makeCGExpr(&m_pBody->m_exprMem, NULL);
        CG_SetAccFlags(m_pdm, pStmt->ifs.pCond, CG_ACCESS_R);

        pStmt->ifs.pThen = pThen;
        pStmt->ifs.pElse = pElse;
    }
    delete(pIfExpr);
    return pStmt;
}
CG_STStmt* st_ParseContext::newCaseStmt(
    const CG_Edipos* pEdp,
    ST_Expression_C* pExpr, 
    const CG_STCaseList*   pCases, 
    const CG_STStmt*       pElse
)
{
    CG_STStmt*       pStmt = NULL;

    pExpr->checkTypedExpr(CG_TBS_ANY_INT & ~CG_TBS_LINT, true); //TODO IEC extension: || CG_TBS_ANY_BIT & ~CG_TBS_BOOL
    pExpr->fixType();

    if(getErrorsInFile()==0)
    {
        pStmt = m_pBody->newStmt(CG_CASE, pEdp);
        pStmt->cases.pExpr = pExpr->makeCGExpr(&m_pBody->m_exprMem, NULL);
        CG_SetAccFlags(m_pdm, pStmt->cases.pExpr, CG_ACCESS_R);

        pStmt->cases.pCases= pCases;
        pStmt->cases.pElse = pElse;


        //check for duplicate labels:
        FC_CHashPtr<long, const CG_STCaseList*> labelsList;
        for(const CG_STCaseList* pcl =pCases; pcl; pcl = pcl->pNextCase)
        {
            for(int i=0; i<pcl->nRanges; i++)
            {
                if(pcl->ranges[i].ulRangeLength == 0)
                {
                    if(!labelsList.insert(pcl->ranges[i].lRangeStart, pcl))
                        m_pdm->msg1(CG_W_CASE_LABEL_DUPLY, &pcl->edp, m_pdm->jot1()<<pcl->ranges[i].lRangeStart); 
                }
                //else: ignore real ranges, pic out only simple integer labes
            }
        }
            

    }

    delete(pExpr);
    return pStmt;
}
CG_STCaseList* st_ParseContext::newCase(
    ST_CCaseExpr*   pCaseList,
    CG_STStmt*      pFirstStmt
)
{
    CG_STCaseList* pcl;
    int            rLen;
    
    rLen = pCaseList->m_rlist.getUsed();
    
    pcl = (CG_STCaseList*)new BYTE[offsetof(CG_STCaseList, ranges) + rLen*sizeof(CG_Subrange)];
    
    pcl->pNextCase =  NULL;
    pcl->edp       =  *pCaseList->getEdp();
    pcl->pStmts    = pFirstStmt;
    pcl->nRanges = rLen;
    memcpy(pcl->ranges, pCaseList->m_rlist.getElements(), rLen*sizeof(CG_Subrange));

    delete(pCaseList);
    return m_pBody->registerCaseList(pcl);
}

//##AC beg
CG_STStmt* st_ParseContext::newCallStmt(
    ST_Expression_C* pCallExpr
)
{
    CG_STStmt*              pStmt = NULL;
    const CG_ExprTree*      pEx;

    if(getErrorsInFile()==0)
    {
        pEx = pCallExpr->makeCGExpr(&m_pBody->m_exprMem, NULL);
        if(pEx->extyp == CG_FBCALLEXPR)
        {
            pStmt = m_pBody->newStmt(CG_CALLFB, &pEx->edp); //TODO use  pEx directly and consider onyl a CG_CALLSTMT
                                                            //no CG_CALLVOID, CG_CALLMAC, all calls should be the same... 
                                                            //it turned out to be of no help to have this differences, it is even bad....
            pStmt->fbcs.pInst = pEx->fbc.pInst;
            pStmt->fbcs.nArgs = pEx->fbc.nArgs;
            pStmt->fbcs.pArgs = pEx->fbc.pArgs;

            ST_CSymtabPrv* pSymPrv = m_pFront->getSymPrv(pStmt->fbcs.pInst->pType->pSym);
            if(pSymPrv->isMacro())
                pStmt->fbcs.pMac = pSymPrv->getNonPolyMacroBody();
        }
        else
        {
            pStmt = m_pBody->newStmt(CG_CALLVOIDFUN, &pEx->edp);
            pStmt->funs.pExpr = pEx;
        }
    }
    delete(pCallExpr);
    return pStmt;
}
//##AC end






CG_STStmt* st_ParseContext::newExitStmt(
    const CG_Edipos* pEdp
)
{
    if(!m_pbt->for_stmts && !m_pbt->while_stmts && !m_pbt->repeat_stmts)
        m_pdm->msg0(CG_E_EXIT, pEdp);

    return m_pBody->newStmt(CG_EXIT, pEdp);
}
CG_STStmt* st_ParseContext::newReturnStmt(
    const CG_Edipos* pEdp
)
{
    
    //TODO:
    //if(pSymtab->pFirst->pType->type!=CG_VOID && (hashUsed.get(pEntry) & CG_EXPR_ACC_W)==0)
    //    pdm->msg0(CG_W_FUN_NOT_ASSIGN_RET, pEdpEndBdy);
    
    
    return m_pBody->newStmt(CG_RETURN, pEdp);
}
CG_STStmt* st_ParseContext::newTransAssignStmt(
    const CG_Edipos*  pEdp,
    ST_Expression_C*  pExpr
)
{
    CG_STStmt*         pStmt = NULL;

    pExpr->checkTypedExpr(CG_TBS_BOOL, true);
    if(getErrorsInFile()==0)
    {
        pStmt = m_pBody->newStmt(CG_ASSIGN, pEdp);
        pStmt->asns.pLhs = NULL;
        pStmt->asns.pRhs = pExpr->makeCGExpr(&m_pBody->m_exprMem, NULL);
        CG_SetAccFlags(m_pdm, pStmt->asns.pRhs, CG_ACCESS_R);
    }
    delete(pExpr);
    return pStmt;
}





CG_STStmt* st_ParseContext::newNativeStmt(
    const CG_Edipos* pEdp,
    const TCHAR*     pszNative
)
{
    int                            j;
    int                            nLine;
    int                            nCol;
    const TCHAR*                   psz;
    const CG_SymEntry*             pEntry;
    TCHAR                          szIdBuffer[MAX_PATH];
    CG_STStmt*                     pStmt;
    CG_EdpEntry                    refEntry;
    FC_CGArray<CG_EdpEntry>        refEntries(36);

    pStmt = m_pBody->newStmt(CG_NATIVE, pEdp);

    pStmt->nats.pszNative = 
        m_pBody->m_exprMem.registerString(pszNative);


    //parse the native text for potentially used header vars:
    j = 0;
    nLine = 0;
    nCol = _tcslen(_T("NATIVE"));
    if(pEdp->typ == CG_EDP_ST)
        nCol += pEdp->lc.c;
    else if(pEdp->typ == CG_EDP_4GR_STBOX)
        nCol += pEdp->gr.c;
    else
        assert(!"no colum info on native block?");
    

    for(psz = pszNative; *psz; psz++)
    {
        nCol++;
        if(*psz=='\n')
        {
            nLine++;
            nCol = 0;
        }

        if(__iscsym(*psz))
        {
            if(j<FC_ARRAY_LEN(szIdBuffer))
                szIdBuffer[j++] = *psz;
        }
        else if(j>0)
        {
            //try the variant of '_' prefix: TODO could be part of feature set?
            if(j<FC_ARRAY_LEN(szIdBuffer) && szIdBuffer[0]==_T('_'))
            {
                szIdBuffer[j] = 0;

                pEntry = m_pSymtabPrv->lookUpEntry(szIdBuffer+1);
                if(pEntry)
                {
                    //sorry for type cast: TODO make setAccBit Function in symtab...
                    ((CG_SymEntry*)pEntry)->accFlags |= CG_ACCESS_N;

                    //update edipos and insert to list:
                    refEntry.edp = *pEdp;
                    CG_IncLineColEdipos(&refEntry.edp, nLine, nCol-j);

                    refEntry.pEntry = pEntry;
                    refEntries.add(refEntry);
                }
            }
            j = 0;
        }
    }

    
    pStmt->nats.nRefEntries = refEntries.getUsed();
    if(pStmt->nats.nRefEntries>0)
    {
        pStmt->nats.pRefEntries = m_pBody->registerEntryRefList(refEntries.getCElements(), pStmt->nats.nRefEntries);
    }

    m_pbt->bHasNative = true;

    m_pSymtabPrv->setBodyHasNativeBit();

    return pStmt;
}
CG_STStmt* st_ParseContext::newSynchronizedStmt(
    const CG_Edipos* pEdp,
    CG_STStmt*       pStmts
)
{
    CG_STStmt*       pStmt;

    pStmt = m_pBody->newStmt(CG_SYNCHRONIZED, pEdp);
    pStmt->syns.pStmts = pStmts;

    return pStmt;
}


CG_STStmt* st_ParseContext::newGotoStmt(
    const CG_Edipos* pEdp,
    const TCHAR*     pszLbl
)
{
    CG_STStmt*     pStmt;
    ST_CJumpLabel* pJL;
     
    pStmt = m_pBody->newStmt(CG_GOTO, pEdp);
    //set later at end of body: pStmt->gotos.pStmts;
    
    pJL = m_pbt->labels.get(pszLbl);
    if(pJL)
    {
        if(pJL->m_pDef) //label already seen, is upward jump
            pStmt->gots.bUpward = 1; 
    }
    else
    {
        //label neither defined nor used in any prior goto, new:
        //label first seen
        pJL = new ST_CJumpLabel();
        m_pbt->labels.insert(pszLbl, pJL);
    }
    pJL->m_gotos.add(pStmt);

    return pStmt;
}
/////////////////////////////////////////////////////////////////////
/**
 * generates C-code for the start of a FOR statement
 * and writes the debug information. 
 * 
 * @param   pEdps      the edipos of FOR keyword 
 * @param   pId        the variable after the FOR keyword 
 * @param   pFrom      the initializer expr
 * @param   pTo        the expr after the TO keyword 
 * @param   pBy        the expr after the BY keyword or NIL if BY is ommitted
 * @param   pStmts     the loop body statements
 * @return  void
 */
CG_STStmt* st_ParseContext::newForStmt(
    const CG_Edipos* pEdps, 
    ST_Identifier_C* pId, 
    ST_Expression_C* pFrom, 
    ST_Expression_C* pTo, 
    ST_Expression_C* pBy
)
{
    ST_Expression_C* pVar;
    ST_Expression_C* pAssign;
    CG_BITSET        lForTyp;
    CG_STStmt*       pStmt = NULL;
    
    
    pVar = createVar(pId);
    lForTyp = pVar->GetType().GetTypeSet();

    if((lForTyp & (CG_TBS_ANY_INT & ~CG_TBS_LINT))==0)
    {
        m_pdm->msg0(CG_E_FOR_VAR, pVar->getEdp());
    }
    else    
    {
		pFrom->checkTypedExpr(lForTyp, true);
        pTo->checkTypedExpr(lForTyp, true);
        if(pBy)
            pBy->checkTypedExpr(lForTyp, true);
    }

    if(m_pbt->sync_stmts!=0)
        m_pdm->msg0(CG_E_BAD_STMT_IN_SYNCBLOCK, pEdps); 

    pAssign = NULL;    
    if(getErrorsInFile()==0)
        pAssign = createOp(pId->getEdp(), CG_MOVE, pVar, pFrom);

    if(getErrorsInFile()==0)
    {
        pStmt = m_pBody->newStmt(CG_FOR, pEdps);
        pStmt->fors.pAssign = pAssign->makeCGExpr(&m_pBody->m_exprMem, NULL);
        

        pStmt->fors.pToExpr = pTo->makeCGExpr(&m_pBody->m_exprMem, NULL);
        CG_SetAccFlags(m_pdm, pStmt->fors.pToExpr, CG_ACCESS_R);
        if(pBy)
        {
            pStmt->fors.pByExpr = pBy->makeCGExpr(&m_pBody->m_exprMem, NULL);
            CG_SetAccFlags(m_pdm, pStmt->fors.pByExpr, CG_ACCESS_R);
        }

        //pStmt->fors.pStmts set later in y-file
    }

    delete(pAssign);
    delete(pTo);
    delete(pBy);

    return pStmt;
}

CG_STStmt* st_ParseContext::newWhileStmt(
    const CG_Edipos* pEdps, 
    ST_Expression_C* pWhile, 
    CG_STStmt*       pStmts

)
{
    CG_STStmt*       pStmt = NULL;

    pWhile->checkTypedExpr(CG_TBS_BOOL, true);
    if(getErrorsInFile()==0)
    {
        pStmt = m_pBody->newStmt(CG_WHILE, pEdps);
        pStmt->whls.pWhile = pWhile->makeCGExpr(&m_pBody->m_exprMem, NULL);
        CG_SetAccFlags(m_pdm, pStmt->whls.pWhile, CG_ACCESS_R);

        pStmt->whls.pStmts = pStmts;
    }
    
    delete(pWhile);

    return pStmt;
}

CG_STStmt* st_ParseContext::newRepeatStmt(
    const CG_Edipos* pEdps, 
    CG_STStmt*       pStmts,
    ST_Expression_C* pUntil 
)
{
    CG_STStmt*       pStmt = NULL;

    pUntil->checkTypedExpr(CG_TBS_BOOL, true);

    if(m_pbt->sync_stmts!=0)
        m_pdm->msg0(CG_E_BAD_STMT_IN_SYNCBLOCK, pEdps); 

    if(getErrorsInFile()==0)
    {
        pStmt = m_pBody->newStmt(CG_REPEAT, pEdps);
        pStmt->reps.pUntil = pUntil->makeCGExpr(&m_pBody->m_exprMem, NULL);
        CG_SetAccFlags(m_pdm, pStmt->reps.pUntil, CG_ACCESS_R);

        pStmt->reps.pStmts = pStmts;
    }
    
    delete(pUntil);

    return pStmt;
}




void st_ParseContext::foundStmts( //TODO not nice code: remove function, handle problem somewhere else, e.g. at end of POU 
    bool       bFound
)
{
    if(bFound)
    {
        if(m_srcType!=CG_SRC_TYPE_ST && 
            m_currEdp.typ!=CG_EDP_4GR_STBOX//HACK ST-Action
          )
        {
            m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_FILE, getCurrEdp());
        }
        else if(m_pSymtabPrv && m_pSymtabPrv->isPolyMorph()) 
        {
            assert(pSymtab->pouTyp==CG_POU_FUN);
            m_pdm->msg0(CG_E_BODY_IN_POLY_FUN, getCurrEdp());
        }
    }
    else 
    {
        if(m_srcType==CG_SRC_TYPE_ST)
            m_pdm->msg1(CG_E_SYNTAX_MISSING, getCurrEdp(), _T(";"));

        return;
    }
}

void st_ParseContext::checkControlStmts(
    const CG_Edipos* pEdp
)
{
    int            i; 
    ST_CJumpLabel* pJL;
    int            nGotos;
    int            nLblRefExs; //##AC
    const TCHAR*  pszMissing = NULL;
    const FC_CHashStr<ST_CJumpLabel*>::Pair* pit;
    
    if(!pEdp)
        pEdp = &m_currEdp;
    
    //check for open control stmts to make better err msgs:
    if (m_pbt->for_stmts!=0)
        pszMissing= _T("END_FOR");
    else if (m_pbt->while_stmts!=0)
        pszMissing= _T("END_WHILE");
    else if (m_pbt->repeat_stmts!=0)
        pszMissing= _T("END_REPEAT");
    else if (m_pbt->case_stmts!=0)
        pszMissing= _T("END_CASE");
    else if (m_pbt->if_stmts!=0)
        pszMissing= _T("END_IF");
    else if (m_pbt->sync_stmts!=0)
        pszMissing= _T("END_SYNCHRONIZED");

    if(pszMissing)
    {
        assert(getErrorsInFile()>0); //syntax error must be happened before
        m_pdm->msg1(CG_E_SYNTAX_MISSING, pEdp, pszMissing);
        return; //don't check labels....
    }
    
    //goto resolution:
    for(pit = m_pbt->labels.firstPair(); pit; pit = pit->pNext)
    {
        pJL = pit->pVal;
        nGotos = pJL->m_gotos.getUsed();
        nLblRefExs = pJL->m_lblrefex.getUsed();//##AC

        if(!pJL->m_pDef)
        {
			if(nGotos>0)
			{
				m_pdm->msg1(CG_E_LABEL_UNDEF, &pJL->m_gotos.get(0)->edp, pit->pszKey);
			}
			else if(nLblRefExs>0)
			{
				m_pdm->msg1(CG_E_LABEL_UNDEF, &pJL->m_lblrefex.get(0)->edp, pit->pszKey);
			}
			else
			{
				assert(!"label ref but nGotos+nLblRefExs==0"); //must be at least one use!
				m_pdm->msg1(CG_E_LABEL_UNDEF, pEdp, pit->pszKey);
			}
            continue;
        }
        if(nGotos==0 && nLblRefExs==0)//##AC
        {
            m_pdm->msg1(CG_W_UNUSED_LABEL, &pJL->m_pDef->edp, pit->pszKey); 
        }
        else
        {
            pJL->m_pDef->lbls.bUsed = true;    
        }

        //set the reference to the label definition in the goto stmt:
        for(i=0; i<nGotos; i++)
        {
            assert(!pJL->m_gotos.get(i)->gots.pLblStmt);
            pJL->m_gotos.get(i)->gots.pLblStmt = pJL->m_pDef;
        }
        for(i=0; i<nLblRefExs; i++) //##AC
        {
            CG_ExprTree* pLR = pJL->m_lblrefex.get(i);
			assert(pLR->extyp==CG_LABELREF);
            assert(pLR->lbl.nLblNum==-1 && !_tcsicmp(pLR->lbl.pszLbl, pJL->m_pDef->lbls.pszLbl));
            pLR->lbl.nLblNum = pJL->m_pDef->lbls.nLblNum;
        }
    }
}

void st_ParseContext::endOfSTBody(const CG_STStmt* pFirstStmt)
{
    checkControlStmts(NULL);//even if errors
    if(pFirstStmt)
        m_pBody->setSTBody(pFirstStmt);
    //else can happen if empty LD/FBD.
}



























ST_CSFCBody::ST_CSFCBody(
    st_ParseContext* pfc, 
    ST_CSymtabPrv*   pSymPrv,
    ST_CBody*        pBody
)
{
    m_pBodyCon    = pfc;
    m_pdm         = pfc->getDM();
    m_pFront      = pfc->getFront();
    m_pSymPrv     = pSymPrv; //sym tab of the sfc pou.
    m_pBody       = pBody;   
    m_pSFCChecker = ST_SFC_CreateSFCChecker(m_pdm);

    //m_inTransition = false;
    m_inTransitionFrom = false;
    m_inTransitionTo = false;

    m_pTransFromList = NULL;
    m_pTransToList = NULL;

    m_stPrio = 0;
}


ST_CSFCBody::~ST_CSFCBody()
{
    ST_SFC_FreeSFCChecker(m_pSFCChecker);

    if (m_pTransFromList)
        delete(m_pTransFromList);
    if (m_pTransToList)
        delete(m_pTransToList);
}




void ST_CSFCBody::sfcAddInitialStep(const ST_Identifier_C* pName)
{
    ST_SFC_AddInitialStep(m_pSFCChecker, pName->getName(), pName->getEdp());
    
    const CG_SymEntry* symEn = m_pSymPrv->lookUpEntry(pName->getName());
    if (symEn)
    {
        m_pdm->msg1(CG_E_SFC_REDEF, pName->getEdp(), m_pdm->jot1()
            <<_T("'")<<pName->getName()<<_T("'")<<m_pdm->res1(CG_RES_STR_ALREADY_DEF)
            <<m_pdm->edpToStr(&symEn->edp, m_pdm->jot2())
        ); 
    }
    
    m_stepName = pName;
}


void ST_CSFCBody::sfcAddStep(const ST_Identifier_C* pName)
{
    ST_SFC_AddStep(m_pSFCChecker, pName->getName(), pName->getEdp());
    
    // stepName already defined in symbol tab
    const CG_SymEntry* symEn = m_pSymPrv->lookUpEntry(pName->getName());
    if (symEn)
    {
        m_pdm->msg1(CG_E_SFC_REDEF, pName->getEdp(), m_pdm->jot1()
              <<_T("'")<<pName->getName()<<_T("'")<<m_pdm->res1(CG_RES_STR_ALREADY_DEF)
              <<m_pdm->edpToStr(&symEn->edp, m_pdm->jot2()) 
        );
    }

    m_stepName = pName;
}


void ST_CSFCBody::sfcStartStep()
{
    m_acbTime = 0;
}


void ST_CSFCBody::sfcEndStep()
{
    delete(m_stepName);
}


void ST_CSFCBody::sfcAddActionAssociation( ST_Identifier_C* pName)
{
    m_acbActionName = pName;

    ST_SFC_AddActionBlock(m_pSFCChecker,
                          m_stepName->getName(), 
                          m_acbActionName->getName(), 
                          m_acbQualifierName->getName(),
                          m_acbTime,
                          m_acbActionName->getEdp());

    const CG_SymEntry* symEn = m_pSymPrv->lookUpEntry(pName->getName());
    if (symEn != NULL)
    {
        //boolsche action ?:
        if (symEn->pType->type != CG_BOOL) //not BOOL_R/F_RDGE
        {
            m_pdm->msg1(CG_E_SFC_REDEF, pName->getEdp(), m_pdm->jot1()
                <<_T("'")<<pName->getName()<<_T("'")<<m_pdm->res1(CG_RES_STR_ALREADY_DEF)
                <<m_pdm->edpToStr(&symEn->edp, m_pdm->jot2()) 
            );
        }
        else
        {
            //yes:
            if (!m_boolActions.findI(pName->getName()))
            {
                sfcStartAction();

                m_boolActions.add(pName->getName());

                ST_SFC_AddAction(m_pSFCChecker, pName->getName(), pName->getEdp());
            
                ST_Identifier_C* qName = new ST_Identifier_C(pName->getEdp(), m_pBodyCon, _T("Q"));
                ST_Identifier_C* pLeftName = new ST_Identifier_C(pName->getEdp(), m_pBodyCon, pName->getName());
                ST_Expression_C* pLeftVar = m_pBodyCon->createVar(pLeftName);
            
                int aNr = isAction(pName->getName());
                ST_StepAction_C* pRightVar = new ST_StepAction_C(
                    pName->getEdp(), m_pBodyCon, aNr, pName->getName(), true);

                pRightVar->setFlagTyp(qName);
                
                const CG_STStmt* pAsn = m_pBodyCon->newAssignStmt(pName->getEdp(), pLeftVar, pRightVar);

                sfcEndAction(pName->getEdp(), pAsn, pName->getName(), pName->getEdp());
            }
        }
    }    

    

    delete(m_acbActionName);
    delete(m_acbQualifierName);
    m_acbTime = 0;
}

void ST_CSFCBody::sfcAddActionAssociationQualifier(const ST_Identifier_C* pName)
{
    m_acbQualifierName = pName;
}

void ST_CSFCBody::sfcAddActionAssociationTime(const ST_Literal_C* pTime)
{
    // *********** to do *********** //
}


    
void ST_CSFCBody::sfcAddAction(const ST_Identifier_C* pName)
{
    // if action has no name, it is the sfc exception action
    // special handling for this action.
    if (!_tcsicmp(pName->getName(), CG_SFC_EXCEPTIONACTION))
    {
        return;
    }
    
    ST_SFC_AddAction(m_pSFCChecker, pName->getName(), pName->getEdp());

    const CG_SymEntry* symEn = m_pSymPrv->lookUpEntry(pName->getName());
    if (symEn != NULL)
    {
        m_pdm->msg1(CG_E_SFC_REDEF, pName->getEdp(), m_pdm->jot1()
            <<_T("'")<<pName->getName()<<_T("'")<<m_pdm->res1(CG_RES_STR_ALREADY_DEF)
            <<m_pdm->edpToStr(&symEn->edp, m_pdm->jot2())
        );
    }
}


void ST_CSFCBody::sfcStartAction()
{
    m_pBodyCon->resetSTBodyTMP();
}

    
void ST_CSFCBody::sfcEndAction(
    const CG_Edipos* pEdp,
    const CG_STStmt* pFirstActionStmt, //can be NULL if FBD action 
                                      //ST always has at least ;
    const TCHAR* pszName,
    const CG_Edipos* pEdpId
)
{
    //final check control stmts & goto resolution:
    m_pBodyCon->checkControlStmts(pEdp);

    if (!_tcsicmp(pszName, CG_SFC_EXCEPTIONACTION))
    {
        if (m_pBody->getExceptionAction())
        {
            // only one exception action is allowed
            m_pdm->msg0(CG_E_SFC_MANY_EXCEPTION_ACTION, pEdpId);
        }
        m_pBody->setExceptionActionBody(pFirstActionStmt);
    }
    else
    {
        m_pBody->setActionBody(pFirstActionStmt);
    }
}



void ST_CSFCBody::sfcStartTransition(const CG_Edipos* ppos)
{
    m_transEdiPos = *ppos;
    //m_inTransition = true;
    m_isNamedTrans = false;
    m_pBodyCon->resetSTBodyTMP();

}


void ST_CSFCBody::sfcStartTransitionNamed(const ST_Identifier_C* pName)
{
    m_transEdiPos = *(pName->getEdp());
    //m_inTransition = true;
    m_isNamedTrans = true;

    const CG_SymEntry* symEn = m_pSymPrv->lookUpEntry(pName->getName());
    if (symEn != NULL)
    {
        m_pdm->msg1(CG_E_SFC_REDEF, pName->getEdp(), m_pdm->jot1()
            <<_T("'")<<pName->getName()<<_T("'")<<m_pdm->res1(CG_RES_STR_ALREADY_DEF)
            <<m_pdm->edpToStr(&symEn->edp, m_pdm->jot2())
        );
    }

    //delete(pName);
    m_transName = pName;
    m_pBodyCon->resetSTBodyTMP();
}

void ST_CSFCBody::sfcEndTransition(
    const CG_Edipos* pEdp,
    const CG_STStmt* pFirstTransiStmt
)
{
    assert(pFirstTransiStmt || m_pBodyCon->getErrorsInFile());
    
    
    m_inTransitionTo = false;
    //m_inTransition = false;
    // in st yacc sets the trans prio to -1
    // the transitions come in here in the order written in st,
    // so rise priority for each transition
    // the first transition has the highest priority 0, 
    // later transitions have lower priority
    /*if (m_transPrio == -1)
    {
        m_transPrio = m_stPrio;
        m_stPrio++;
    }*/ //TODO test trans prios !!!
    if (m_isNamedTrans)
    {
        ST_SFC_AddTransitionNamed(m_pSFCChecker,
                              m_transName->getName(),
                              m_transPrio, 
                              m_pTransFromList, 
                              m_pTransToList,
                              !m_transSingleStepFrom, 
                              !m_transSingleStepTo,
                              &m_transEdiPos,
                              &m_TransFromPos,
                              &m_TransToPos);
        delete(m_transName);
    }
    else
    {
        ST_SFC_AddTransition (m_pSFCChecker,
                              m_transPrio, 
                              m_pTransFromList, 
                              m_pTransToList,
                              !m_transSingleStepFrom, 
                              !m_transSingleStepTo,
                              &m_transEdiPos,
                              &m_TransFromPos,
                              &m_TransToPos);
    }

    //final check control stmts & goto resolution:
    //not needed for trans, since this is only an expr.
    m_pBody->setTransiBody(pFirstTransiStmt);
}


void ST_CSFCBody::sfcStartTransitionFrom()
{
    m_inTransitionFrom = true;
    if (m_pTransFromList)
        delete(m_pTransFromList);
    m_pTransFromList = new FC_CStrList();
    m_TransFromPos.resetUsed();
}


void ST_CSFCBody::sfcStartTransitionTo()
{
    m_inTransitionFrom = false;
    m_inTransitionTo = true;
    if (m_pTransToList)
        delete(m_pTransToList);
    m_pTransToList = new FC_CStrList;
    m_TransToPos.resetUsed();
    m_transPrio = 0;
}


void ST_CSFCBody::sfcAddTransitionStep(const ST_Identifier_C* pName, bool singleStep)
{
    assert(m_inTransitionFrom || m_inTransitionTo);

    if (m_inTransitionFrom)
    {
        m_pTransFromList->add(pName->getName());
        m_TransFromPos.add(*pName->getEdp());        
        m_transSingleStepFrom = singleStep;

    }
    else if (m_inTransitionTo)
    {
        m_pTransToList->add(pName->getName());
        m_TransToPos.add(*pName->getEdp());
        m_transSingleStepTo = singleStep;
    }

    delete(pName);
}

void ST_CSFCBody::sfcAddTransitionPrio(int prio)
{
    m_transPrio = prio;
}

int  ST_CSFCBody::isStep(const TCHAR* pname)
{
    return ST_SFC_IsStep(m_pSFCChecker, pname);
}
int  ST_CSFCBody::isAction(const TCHAR* pname)
{
    return ST_SFC_IsAction(m_pSFCChecker, pname);
}

void ST_CSFCBody::endOfSFC(const CG_Edipos* pEndOfFile)
{
    int           i;
    int           bOk;
    CG_SFCStruct* pSFCS;

    // SFC_DEBUG
    // Debug or not debug output of sfc codegenerator
    //return ST_SFC_CheckDebug(m_pSFCChecker, pEndOfFile);
    if(!ST_SFC_Check(m_pSFCChecker, pEndOfFile))
    {
        assert(m_pBodyCon->getErrorsInFile()>0);
        return;
    }


    if(m_pBodyCon->getErrorsInFile())
        return;
    


    FC_NEW_ZSTRUCT(pSFCS, CG_SFCStruct);

    pSFCS->nSteps   =    ST_SFC_GetAnzSteps  (m_pSFCChecker);
    pSFCS->nTrans   =    ST_SFC_GetAnzTrans  (m_pSFCChecker);
    pSFCS->nActions =    ST_SFC_GetAnzActions(m_pSFCChecker);

    pSFCS->nNextSteps =  ST_SFC_GetAnzNextSteps (m_pSFCChecker);
    pSFCS->nPrevSteps =  ST_SFC_GetAnzPrevSteps (m_pSFCChecker);
    pSFCS->nStepAction=  ST_SFC_GetAnzStepAction(m_pSFCChecker);

    pSFCS->initStep   =  ST_SFC_GetInitStep(m_pSFCChecker);



    pSFCS->pStepNames = new const TCHAR*[pSFCS->nSteps];
    pSFCS->pStepEdp   = new CG_Edipos[pSFCS->nSteps];
    for(i = 0; i<pSFCS->nSteps; i++)
    {
        pSFCS->pStepNames[i] = m_pBody->m_exprMem.registerString(ST_SFC_GetStepName(m_pSFCChecker, i));
        pSFCS->pStepEdp[i] = *ST_SFC_GetStepEdiPos(m_pSFCChecker, i);
    }

    pSFCS->pActionNames = new const TCHAR*[pSFCS->nActions];
    pSFCS->pActionEdp   = new CG_Edipos[pSFCS->nActions];
    for(i = 0; i<pSFCS->nActions; i++)
    {
        pSFCS->pActionNames[i] = m_pBody->m_exprMem.registerString(ST_SFC_GetActionName(m_pSFCChecker, i));
        pSFCS->pActionEdp[i] = *ST_SFC_GetActionEdiPos(m_pSFCChecker, i);
    }

    pSFCS->pTransNames = new const TCHAR*[pSFCS->nTrans];
    pSFCS->pTransEdp   = new CG_Edipos[pSFCS->nTrans];
    for(i = 0; i<pSFCS->nTrans; i++)
    {
        pSFCS->pTransNames[i] = m_pBody->m_exprMem.registerString(ST_SFC_GetTransName(m_pSFCChecker, i));
        pSFCS->pTransEdp[i] = *ST_SFC_GetTransEdiPos(m_pSFCChecker, i);
    }


    pSFCS->pFollowTrans = new long[pSFCS->nSteps];
    bOk = ST_SFC_GetFollowTransArray(m_pSFCChecker, pSFCS->pFollowTrans, pSFCS->nSteps);
    assert(bOk);

    pSFCS->pAltTrans = new long[pSFCS->nTrans];
    ST_SFC_GetAltTransArray(m_pSFCChecker, pSFCS->pAltTrans, pSFCS->nTrans);
    assert(bOk);

    pSFCS->pNextSteps = new long[pSFCS->nNextSteps];
    bOk = ST_SFC_GetNextStepsArray(m_pSFCChecker, pSFCS->pNextSteps, pSFCS->nNextSteps);
    assert(bOk);

    pSFCS->pNextStepsOffset = new long[pSFCS->nTrans];
    ST_SFC_GetNextStepsOffsetArray(m_pSFCChecker, pSFCS->pNextStepsOffset, pSFCS->nTrans);
    assert(bOk);

    pSFCS->pPrevSteps = new long[pSFCS->nPrevSteps];
    bOk = ST_SFC_GetPrevStepsArray(m_pSFCChecker, pSFCS->pPrevSteps, pSFCS->nPrevSteps);
    assert(bOk);

    pSFCS->pPrevStepsOffset = new long[pSFCS->nTrans];
    bOk = ST_SFC_GetPrevStepsOffsetArray(m_pSFCChecker, pSFCS->pPrevStepsOffset, pSFCS->nTrans);
    assert(bOk);

    pSFCS->pStepActionBlocks = new long[pSFCS->nStepAction];
    bOk = ST_SFC_GetStepActionBlocksArray(m_pSFCChecker, pSFCS->pStepActionBlocks, pSFCS->nStepAction);
    assert(bOk);

    pSFCS->pStepActionBlockOffset = new long[pSFCS->nSteps];
    bOk = ST_SFC_GetStepActionBlockOffsetArray(m_pSFCChecker, pSFCS->pStepActionBlockOffset, pSFCS->nSteps);
    assert(bOk);

    pSFCS->pStepActionBlockQualifier = new long[pSFCS->nStepAction];
    bOk = ST_SFC_GetStepActionBlockQualifierArray(m_pSFCChecker, pSFCS->pStepActionBlockQualifier, pSFCS->nStepAction);
    assert(bOk);

    pSFCS->pStepActionBlockTimeParameter = new long[pSFCS->nStepAction];
    bOk = ST_SFC_GetStepActionBlockTimeParameterArray(m_pSFCChecker, pSFCS->pStepActionBlockTimeParameter, pSFCS->nStepAction);
    assert(bOk);

    m_pBody->setSFCStruct(pSFCS);
}

