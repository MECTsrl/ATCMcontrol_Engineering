/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_ParseContext.cpp 2     17.04.07 16:28 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asm_ParseContext.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_ParseContext.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 17.04.07 16:28 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_ParseContext.cpp 2     17.04.07 16:28 Ef $";

#include <limits.h>
#include <malloc.h>
#include <tchar.h>

#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_defines.h"

#include "asm_ParseContext.h"
#include "globalContext.h"
#include "baselib.h"



asm_ParseContext::asm_ParseContext(FileContext_C* pfc_in)
{
    assert(pfc_in);
    m_pfc             = pfc_in;
    m_pScanner        = NULL;
    m_pszFileContext  = NULL;
    m_ulFileLength    = 0;

    m_curredp.pfc = pfc_in;
    m_curredp.locid = EDP_ST_LINE;
    m_curredp.x = -1;
    m_curredp.y = -1;

    m_bEnableInstr = false;
    m_bIsRetain    = false;
    m_curProc = NULL;
}
asm_ParseContext::~asm_ParseContext()
{
    if(m_pScanner)
        ASM_DeleteScanner(m_pScanner);    
    
    delete(m_pszFileContext);
}


bool asm_ParseContext::asm_LoadFile()
{
    if(!LoadFileToMem(m_pfc->getFileName(),  
                     &m_pszFileContext, 
                     NULL,
                     &m_ulFileLength, 
                     NULL)) 
        return false;

    m_pScanner = ASM_CreateScanner();
    ASM_SetInputString(m_pScanner, m_pszFileContext, m_ulFileLength);
    return true;
}

void asm_ParseContext::tokenSyntax(const TCHAR* pszTok)
{
    TCHAR buff[256];
    unsigned int ui;

    if(!_istascii(*pszTok))
    {
        ui = *pszTok & 0xff; 
        wsprintf(buff, "'%c' = 0x%x", *pszTok, ui);
        
        ST_DiagMsg1(getEdiPos(), E_4CCG_ERR_SYNTAX_UNKNOWNCHAR,buff); 
    }
    else    
    {
        ST_DiagMsg0(getEdiPos(), E_4CCG_ERR_SYNTAX);
    }
}
void asm_ParseContext::makeBpPos
(
    ST_LineCol             pos,
    const ASM_LITERAL_TYP* BpNum,
    bool                   bInsertOPCode
)
{
    ASM_PRIMARY_TYP     BpArg;

    // TODO: check whether num already used with method like down below

    
    BpArg.argkind = ASM_PRM_LIT;
    BpArg.lit.any64 = BpNum->any64;
    BpArg.lit.type= BpNum->type;

    ASM_INSTR_TYP* pBpPos;


    ASM_PRIMARY_TYP* args[] = {&BpArg};
    pBpPos = new ASM_INSTR_TYP(getEdiPos(), bInsertOPCode ? ASM_BPOP : ASM_STMTBEG, m_curProc, NULL, 1, args);
    m_curProc->insertBpPos(pBpPos);
}

void asm_ParseContext::makeCalop(ST_LineCol   pos, 
                                 ST_IDENT_TYP opName)
{
    ASM_PRIMARY_TYP Arg;

    // is function
    Arg.argkind = ASM_PRM_LABEL;
    Arg.label = GetGC()->registerId(opName.psz);
    
    ASM_PRIMARY_TYP* args[] = {&Arg};
    new ASM_INSTR_TYP(getEdiPos(), ASM_CALOP, m_curProc, NULL, 1, args);
}

void asm_ParseContext::makeJmp(ST_LineCol             pos, 
                               ASM_INSTR_ENUM         instr, 
                               ST_IDENT_TYP           label)
{
    ASM_PRIMARY_TYP labelArg;
    labelArg.argkind = ASM_PRM_LABEL;
    labelArg.label = label;
    ASM_PRIMARY_TYP* args[] = {&labelArg};
    new ASM_INSTR_TYP(getEdiPos(), instr, m_curProc, NULL, 1, args);
}
void asm_ParseContext::makeLabel(ST_IDENT_TYP label)
{
    ASM_PRIMARY_TYP     labelArg;
    ASM_INSTR_TYP*      pLabel;

    labelArg.argkind = ASM_PRM_LABEL;
    labelArg.label = label;

    ASM_PRIMARY_TYP* args[] = {&labelArg};
    pLabel = new ASM_INSTR_TYP(getEdiPos(), ASM_LABDEF, m_curProc, NULL, 1, args);

    m_curProc->insertLabel(pLabel);
}




void asm_ParseContext::checkVarReference(ASM_INST_PATH_TYP path, ASM_PRIMARY_TYP* pNewArg)
{
    const ASM_GLOB_COMP* pGComp; 
    FC_CString           fieldBuff(MAX_PATH);
    const TCHAR*         pszField;
    const TCHAR*         pszNext;
    const ASMPROC_TYP*   pFBDef;
    const TCHAR*         psz;

    memset(pNewArg, 0 , sizeof(*pNewArg)); 

    pNewArg->argkind = ASM_PRM_NONE;

    //check for global buildin tmp var referece:
    //must have syntax: [(@|§)<bitsize>_<int>]|[@s<strlen>_<int>]
	//@= rel temp va and §= FB-member for trigger helper vars
    psz = path.ids.psz;
    if(*psz==_T('@') || *psz==_T('§'))
    {
        int           ic;
        unsigned      size=0;
        TCHAR         buffer[256];
        bool          bIsString = false;
        
        if (*(psz+1) == _T('s'))
        {
            // tmp var for STRING data type
            bIsString = true;
            psz++;
        }

        for(psz++, ic=0; ic<250 && _istdigit(*psz); )
            buffer[ic++] = *psz++;
        
        buffer[ic] = 0;
        size = _ttoi(buffer);

        //simply ignore the rest while(*psz != 0) psz++;

        pNewArg->argkind = ASM_PRM_LOCALCOMP;
        pNewArg->pLocal  = m_curProc->addTmpVar(getEdiPos(), path.ids, size, bIsString);

        return;
    }


    if(!m_curProc)
        return;

    if(path.depth==1)
    {
        pszField= path.ids.psz;
        pszNext = _T("");
    }
    else
    {
        pszNext = fieldBuff.appendUpTo(path.ids.psz, _T("."));
        pszField = fieldBuff;
    }

    
    pNewArg->pLocal = m_curProc->lookUpLocalVar(pszField);
    if(pNewArg->pLocal)
    {
        pNewArg->argkind = ASM_PRM_LOCALCOMP;
        pNewArg->path = path;
        pFBDef = pNewArg->pLocal->pType->pFBIDef;
    }
    else 
    {
        pGComp = GetGC()->lookUpGlobalVar(pszField);
        if(pGComp)
        {
            pNewArg->argkind = ASM_PRM_GLOBCOMP;
            pNewArg->pGComp  = pGComp;  
            pFBDef = pNewArg->pGComp->pType->pFBIDef;
            pNewArg->path = path;
        }
        else
        {
            ST_DiagMsg1(getEdiPos(), E_4CCG_ERR_UNDEF_VAR, pszField);
            return;
        }
    }

    while(*pszNext)
    {
        if(!pFBDef)
        {
            ST_DiagMsg0(getEdiPos(), E_4CCG_ERR_NON_STRUCT);
            pNewArg->argkind = ASM_PRM_NONE;
            return;
        }
        
        pszNext = fieldBuff.clear().appendUpTo(pszNext, _T("."));
        if(!pFBDef->lookUpLocalVar(fieldBuff))
        {
            ST_DiagMsg2(getEdiPos(), E_4CCG_ERR_UNDEF_MEMBER, fieldBuff.psz(), pFBDef->getName());
            pNewArg->argkind = ASM_PRM_NONE;
            return;
        }
    }
}

const ASM_DATA_TYP* asm_ParseContext::makeType(ST_IDENT_TYP typeName,
                                               bool bArray,
                                               const ASM_LITERAL_TYP* pArrayDim, 
                                               const ASM_LITERAL_TYP* pStrLen)
{
    ASM_TYPE_ENUM      basicType;
    const ASMPROC_TYP* pProcDef;
    const TCHAR*       pszName;

    pszName = typeName.psz;
    basicType = ASM_TYPE_UNDEF;
    
    if(!_tcsicmp(pszName, _T("bool")))
        basicType = ASM_BOOL;
    else if(!_tcsicmp(pszName, _T("byte")))
        basicType = ASM_BYTE;
    else if(!_tcsicmp(pszName, _T("word")))
        basicType = ASM_WORD;
    else if(!_tcsicmp(pszName, _T("dword")))
        basicType = ASM_DWORD;
    else if(!_tcsicmp(pszName, _T("int")))
        basicType = ASM_INT;
    else if(!_tcsicmp(pszName, _T("uint")))
        basicType = ASM_UINT;
    else if(!_tcsicmp(pszName, _T("dint")))
        basicType = ASM_DINT;
    else if(!_tcsicmp(pszName, _T("udint")))
        basicType = ASM_UDINT;
    else if(!_tcsicmp(pszName, _T("real")))
        basicType = ASM_REAL;
    else if(!_tcsicmp(pszName, _T("lreal")))
        basicType = ASM_LREAL;
    else if(!_tcsicmp(pszName, _T("time")))
        basicType = ASM_TIME;
    else if(!_tcsicmp(pszName, _T("string")))
        basicType = ASM_STRING;
    else if(!_tcsicmp(pszName, _T("void")))
        basicType = ASM_VOID;
    
    if(basicType == ASM_TYPE_UNDEF)
    {
        //must be a function block or the like:
        pProcDef = GetGC()->lookUpPrgFB(typeName);
        if(!pProcDef)
            pProcDef = GetGC()->lookUpFunction(typeName);

        if(!pProcDef)
        {
            ST_DiagMsg1(getEdiPos(), E_4CCG_ERR_NO_DEF_FOUND, pszName);
        }
        else
        {
            switch(pProcDef->getPouTyp())
            {
            case ASM_FB:     return GetGC()->registerType(ASM_FBI, typeName, 
                                        pProcDef, bArray, pArrayDim, pStrLen);
            case ASM_PRG:    return GetGC()->registerType(ASM_PRGI, typeName, 
                                        pProcDef, bArray, pArrayDim, pStrLen);
            case ASM_STRUCT: return GetGC()->registerType(ASM_STRI, typeName, 
                                        pProcDef, bArray, pArrayDim, pStrLen);
            case ASM_FUN:    
                if(bArray || pArrayDim || pStrLen)
                {
                    ST_DiagMsg1(getEdiPos(), E_4CCG_ERR_TYPE_NOT_IMPL,  _T("<function> with [] or ()"));
                    break;
                }
                else
                {
                    return GetGC()->registerType(ASM_FUNNAME, typeName, 
                                            pProcDef, false, NULL, 0);
                }
                
            default:
                ST_DiagMsg3(getEdiPos(), E_4CCG_ERR_INVALID_POU_TYPE, 
                    _T("INSTANCE|STRUCTURE"), pszName, pProcDef->getPouTypeString());
            }
        }
    }

    if ( ! bArray && pStrLen == NULL)
    {
        return GetGC()->registerBasicType(basicType);
    }
    
    return GetGC()->registerType(basicType, typeName, NULL, bArray, pArrayDim, pStrLen);
}


void asm_ParseContext::addProc(const ST_EdiPos*       pEdp, 
                               ST_IDENT_TYP           name, 
                               ASM_PROC_ENUM          kind, 
                               const ASM_LITERAL_TYP& lit)
{
   
    long opcode = -1;

    if(lit.type != ASM_TYPE_UNDEF && kind != ASM_STRUCT)
    {
        if(lit.type!=ASM_DINT || lit.any64<0 || lit.any64>32675)
            ST_DiagMsg3(pEdp, E_4CCG_ERR_BAD_OPCODE, Tostr_C(lit.any64), _T("0"), _T("32675"));
        else if(kind!=ASM_FUN && kind!=ASM_FB)
            ST_DiagMsg0(pEdp, E_4CCG_ERR_OPCODE_FOR_NON_FUNFB);
        else
            opcode = (long)lit.any64;
    }
    m_curProc = new ASMPROC_TYP(pEdp, kind, name, opcode);
    GetGC()->addProc(m_curProc);
}

void asm_ParseContext::assignClssid(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP& lit)
{
    assert( ! m_curProc->isBuildIn());
    assert(lit.type != ASM_TYPE_UNDEF);

    m_curProc->assignClassID((long) lit.any64);
    GetGC()->registerClassName(m_curProc->getTypeIdent(), (long) lit.any64);
}


void asm_ParseContext::addCopyItem(ASM_INST_PATH_TYP path, FC_CGArray<ST_IDENT_TYP>* pMembers)
{
    ASM_COPY_ITEM  cpi;
    ST_IDENT_TYP*  pMem;

    cpi.inst = path;
    cpi.nMem = pMembers->getUsed();
    cpi.pMem = NULL;
    if(cpi.nMem>0)
    {
        pMem = pMembers->getMemCopy();
        pMembers->resetUsed();
        //globally register the id list pointer:
        GetGC()->m_listOfIdLists.add(pMem);
        cpi.pMem = pMem;
    }
    m_tmpCopyItemList.add(cpi);
}
void asm_ParseContext::setActualTask(const ST_EdiPos* pEdp,
                                     ST_IDENT_TYP*    name)
{
    m_pAktTask = GetGC()->lookUpTaskUC(name->psz);
    if (m_pAktTask == NULL)
    {
        ST_DiagMsg1(pEdp, E_4CCG_ERR_TASK_UNDEFINED, name->psz);
    }
}

void asm_ParseContext::appendRegionList(const ST_EdiPos* pEdp,
                                        ST_IDENT_TYP     name,
                                        ASM_MEMRGN_TYP   typ)
{
    if (typ == ASM_WRITE_LIST)
    {
        if ( ! _tcscmp(name.psz, _T("input")))
            m_pAktTask->MemRegions.ISegMemRgnWR.append(&m_tmpRegionList);
        else if ( ! _tcscmp(name.psz, _T("output")))
            m_pAktTask->MemRegions.OSegMemRgnWR.append(&m_tmpRegionList);
        else if ( ! _tcscmp(name.psz, _T("global")))
            m_pAktTask->MemRegions.GSegMemRgnWR.append(&m_tmpRegionList);
        else if ( ! _tcscmp(name.psz, _T("retain")))
            m_pAktTask->MemRegions.GRSegMemRgnWR.append(&m_tmpRegionList);
        else
            ST_DiagMsg1(pEdp, E_4CCG_ERR_UNKNOWN_SEGM_NAME, name.psz);
    }
    else if (typ == ASM_READ_LIST)
    {
        if ( ! _tcscmp(name.psz, _T("input")))
            m_pAktTask->MemRegions.ISegMemRgnRD.append(&m_tmpRegionList);
        else if ( ! _tcscmp(name.psz, _T("output")))
            m_pAktTask->MemRegions.OSegMemRgnRD.append(&m_tmpRegionList);
        else if ( ! _tcscmp(name.psz, _T("global")))
            m_pAktTask->MemRegions.GSegMemRgnRD.append(&m_tmpRegionList);
        else if ( ! _tcscmp(name.psz, _T("retain")))
            m_pAktTask->MemRegions.GRSegMemRgnRD.append(&m_tmpRegionList);
        else
            ST_DiagMsg1(pEdp, E_4CCG_ERR_UNKNOWN_SEGM_NAME, name.psz);
    }
    else
    {
        assert( ! "should never be");
    }
}

ASM_LITERAL_TYP* asm_ParseContext::makeLiteralList(const ASM_LITERAL_TYP& lit)
{
    ASM_LITERAL_TYP* pLitList = new ASM_LITERAL_TYP;
    GetGC()->m_ListOfLitLists.add(pLitList);
    pLitList[0] = lit;
    
    return pLitList;
}

ASM_LITERAL_TYP* asm_ParseContext::getLiteralList()
{
    ASM_LITERAL_TYP* pLitList = m_LitList.getMemCopy();
    GetGC()->m_ListOfLitLists.add(pLitList);
    
    return pLitList;
}

FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>* asm_ParseContext::getInitList()
{
    assert(m_InitList.getUsed() != 0);

    FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>* pList = new FC_CGArray<ASM_INIT_REPEAT_VAL_TYP>;
    m_InitList.copyto(pList);

    GetGC()->m_ListOfInitLists.add(pList);

    return pList;
}




bool ASM_LitOffsetToLiteral(const TCHAR* pText, ASM_LITERAL_TYP* pLit)
{
    const TCHAR*    psz;
    TCHAR           sz[1024];
    int             i;

    for(psz=pText; *psz && *psz!='('; psz++)
        ;
    if(*psz!='(')
        return false;
    psz++;

    for(i=0 ;*psz &&  *psz!=')' && *psz!=',' && i<1023; psz++)
        sz[i++] = *psz;

    sz[i] = 0;
    FC_StringTrim(sz);

    if(*psz==',')
    {
        pLit->idStruct = GetGC()->registerId(sz);

        psz++;
        for(i=0 ;*psz &&  *psz!=')' && i<1023; psz++)
            sz[i++] = *psz;

        sz[i] = 0;
        FC_StringTrim(sz);
        pLit->idMember = GetGC()->registerId(sz);
    }
    else
    {
        pLit->idStruct.psz = NULL;
        pLit->idMember = GetGC()->registerId(sz);
    }

    pLit->type = ASM_OFFSET_LITERAL;

    return *psz==')';
}


bool ASM_LitObjidToLiteral(const TCHAR* pText, ASM_LITERAL_TYP* pLit)
{
    const TCHAR*    psz;
    TCHAR           sz[1024];
    int             i;

    for(psz=pText; *psz && *psz!='('; psz++)
        ;
    if(*psz!='(')
        return false;
    psz++;

    for(i=0 ;*psz &&  *psz!=')' && *psz!=',' && i<1023; psz++)
        sz[i++] = *psz;

    sz[i] = 0;
    FC_StringTrim(sz);
    pLit->idObj = GetGC()->registerId(sz);

    pLit->type = ASM_OBJID_LITERAL;
    return *psz==')';
}

bool ASM_LitTokenToLiteral(const ST_EdiPos* pEdp, const TCHAR* pText, ASM_LITERAL_TYP* pLit)
{
    TCHAR              typePrefix[256+1];
    int                i;
    const TCHAR*       psz = pText;
    BSTR               bstr;
    HRESULT            hr;
    VARIANT            va;
    BL_IEC_TYP         blt;
    ASM_TYPE_ENUM      asmtype;
    float              flt;
         

    pLit->type = ASM_TYPE_UNDEF;
    pLit->any64 = 0;

    i=0;
    for(i=0; *psz && i < FC_ARRAY_LEN(typePrefix)-2 && (_istalpha(*psz) || *psz==_T('_')); psz = _tcsinc(psz))
        typePrefix[i++] = *psz;

    typePrefix[i] = 0;
    
    if(!_tcscmp(typePrefix, _T("true")))
    {
        asmtype = ASM_BOOL;
        blt = BL_BOOL;
    }
    else if(!_tcscmp(typePrefix, _T("false")))
    {
        asmtype = ASM_BOOL;
        blt = BL_BOOL;
    }
    else if(!_tcscmp(typePrefix, _T("bool")))
    {
        asmtype = ASM_BOOL;
        blt = BL_BOOL;
    }
    else if(!_tcscmp(typePrefix, _T("byte")))
    {
        asmtype = ASM_BYTE;
        blt = BL_BYTE;
    }
    else if(!_tcscmp(typePrefix, _T("word")))
    {
        asmtype = ASM_WORD;
        blt = BL_WORD;
    }
    else if(!_tcscmp(typePrefix, _T("dword")))
    {
        asmtype = ASM_DWORD;
        blt = BL_DWORD;
    }
    else if(!_tcscmp(typePrefix, _T("int")))
    {
        asmtype = ASM_INT;
        blt = BL_INT;
    }
    else if(!_tcscmp(typePrefix, _T("uint")))
    {
        asmtype = ASM_UINT;
        blt = BL_UINT;
    }
    else if(!_tcscmp(typePrefix, _T("dint")))
    {
        asmtype = ASM_DINT;
        blt = BL_DINT;
    }
    else if(!_tcscmp(typePrefix, _T("udint")))
    {
        asmtype = ASM_UDINT;
        blt = BL_UDINT;
    }
    else if(!_tcscmp(typePrefix, _T("lint")))
    {
        asmtype = ASM_LINT;
        blt = BL_LINT;
    }
    else if(!_tcscmp(typePrefix, _T("real")))
    {
        asmtype = ASM_REAL;
        blt = BL_REAL;
    }
    else if(!_tcscmp(typePrefix, _T("lreal")))
    {
        asmtype = ASM_LREAL;
        blt = BL_LREAL;
    }
    else if(!_tcscmp(typePrefix, _T("t")) || !_tcscmp(typePrefix, _T("time")))
    {
        asmtype = ASM_TIME;
        blt = BL_TIME;
    }
    else if(_istdigit(*pText))
    {
        //assume DINT type:
        asmtype = ASM_DINT;
        blt = BL_DINT;
    }
    else
    {
        ST_DiagMsg1(pEdp, E_4CCG_ERR_NOT_SUPPORTED, pText);
        return false;
    }
    
    bstr = FC_StringToBSTR(pText);

    VariantInit(&va);
    hr = BL_StringToVariant(bstr,   // in parameter, zero-terminated
                            blt,    // may be BL_ANY if not known
                            NULL,  // may be NULL or "" if not known
                            BL_USE_IEC_FORMAT,     // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                            &va);     // out parameter

    SysFreeString(bstr);

    
    if(FAILED(hr))
    {
        int iErr;
        if(hr==E_BL_ERR_RANGE)
        {
            if(blt==BL_BOOL||blt==BL_BYTE||blt==BL_WORD||blt==BL_DWORD)
                iErr = E_4CCG_ERR_BIT_LITERAL;
            else if(blt==BL_TIME)
                iErr = E_4CCG_ERR_TIME_LITERAL;
            else
                iErr = E_4CCG_ERR_INT_LITERAL;
        }
        else
        {
            iErr = E_4CCG_ERR_SYNTAX;
        }
        ST_DiagMsg1(pEdp, iErr, pText);
        return false;
    }


    pLit->type = asmtype;

    switch(blt)
    {
    case BL_BOOL:
        assert(va.vt == VT_BOOL);
        pLit->any64 = va.boolVal==VARIANT_TRUE ? 1 : 0;
        break;
    case BL_BYTE:
    case BL_WORD:
    case BL_DWORD:
    case BL_INT:
    case BL_DINT:
    case BL_UINT:
    case BL_UDINT:
        assert(va.vt == VT_I4);
        pLit->any64 = va.lVal;
        break;
    case BL_TIME:
    case BL_LINT:
        assert(va.vt == VT_DECIMAL);
        if(va.decVal.sign == DECIMAL_NEG)
            pLit->any64 = - ((__int64)(va.decVal.Lo64));
        else
            pLit->any64 =  + va.decVal.Lo64;
        break;
    case BL_REAL:
        assert(va.vt == VT_R8);
        flt = (float) va.dblVal;
        memcpy(&pLit->any64, &flt, sizeof(float));
        break;
    case BL_LREAL:
        assert(va.vt == VT_R8);
        memcpy(&pLit->any64, &va.dblVal, sizeof(double));
        break;
    default:
        assert(!"bad enum");
    }
    VariantClear(&va);

    return true;
}    

/* ---------------------------------------------------------------------------- */
