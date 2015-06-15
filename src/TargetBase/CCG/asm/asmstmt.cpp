/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmstmt.cpp 3     20.04.07 11:16 Ef $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmstmt.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmstmt.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 20.04.07 11:16 $
 *			 $Revision: 3 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmstmt.cpp 3     20.04.07 11:16 Ef $";

#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cstring.h"
#include "vmSharedDef.h"


#include "globalContext.h"
#include "asmstmt.h"
#include "limits.h"
#define OPC_ASM 1
#include "intOpcds.h"
#undef OPC_ASM
#include "VmConstants.h"
#include "baselib.h"


#define SETBIT(p, Offs, bit)    p = p & (BYTE) ~(1<<(Offs)) | (((BYTE)bit&1)<<(Offs))

void ASM_GetUserOpCodeRange(long* pFirst, long* pLast)
{
    *pFirst = 0;
    *pLast = 32675;
}

/*>>>>------------------------------------------------------------------
    Size: is string length for STRING
          is bit size for other IEC types
<<<<*/
const ASM_DATA_TYP* ASM_GetTmpVarTypeBySize(const ASMPROC_TYP* pProc,
                                            int                Size,
                                            bool               bIsString)
{
    if (bIsString)
    {
        // STRING data type
        ASM_LITERAL_TYP LenLit;

        LenLit.type = ASM_DINT;
        LenLit.any64 = Size;

        ST_IDENT_TYP typId = GetGC()->registerId(_T("string"));
        return GetGC()->registerType(ASM_STRING, typId, pProc, false, NULL, &LenLit);
    }
    else
    {
        switch(Size)
        {
        case 1 : return GetGC()->registerBasicType(ASM_BOOL);
        case 8 : return GetGC()->registerBasicType(ASM_BYTE);
        case 16: return GetGC()->registerBasicType(ASM_INT);
        case 32: return GetGC()->registerBasicType(ASM_DINT);
        case 64: return GetGC()->registerBasicType(ASM_LREAL);
        default: assert( ! "wrong data type for temp var"); return NULL;
        }
    }
}

FC_CString& ASM_LitToString(FC_CString& text, const ASM_LITERAL_TYP* pLit)
{

    switch(pLit->type)
    {
    case ASM_TYPE_UNDEF:
        text.load(_T("<no literal>"));
    break;
    case ASM_BOOL:
        if(pLit->any64!=0)
            text.load(_T("true"));
        else
            text.load(_T("false"));
    break;
    case ASM_BYTE:
        text.load(_T("byte#"));
        text<<(unsigned long)pLit->any64;
    break;
    case ASM_WORD:
        text.load(_T("word#"));
        text<<(unsigned long)pLit->any64;
    break;
    case ASM_DWORD:
        text.load(_T("dword#"));
        text<<(unsigned long)pLit->any64;
    break;
    case ASM_INT:
        text.load(_T("int#"));
        text<<(long)pLit->any64;
    break;
    case ASM_UINT:
        text.load(_T("uint#"));
        text<<(unsigned long)pLit->any64;
    break;
    case ASM_DINT:
        text.load(_T("dint#"));
        text<<(long)pLit->any64;
    break;
    case ASM_UDINT:
        text.load(_T("udint#"));
        text<<(unsigned long)pLit->any64;
    break;
    case ASM_LINT:
        text.load(_T("lint#"));
        text<<pLit->any64;
    break;
    case ASM_REAL:
        text.load(_T("real#"));
        text.appendWsprintf(_T("%e"), *(double*)(&pLit->any64));
    break;
    case ASM_LREAL:
        text.load(_T("lreal#"));
        text.appendWsprintf(_T("%e"), *(double*)(&pLit->any64));
    break;
    case ASM_TIME:
        text.load(_T("time#"));
        text<<Tostr_C(pLit->any64);
    break;
    case ASM_STRING:
        text.load(pLit->pszIECStr);
    break;
    case ASM_OFFSET_LITERAL:
        text.load(_T("offsetof("));
        if(pLit->idStruct.psz)
            text<<pLit->idStruct.psz<<',';
        text<<pLit->idMember.psz<<')';
    break;
    case ASM_OBJID_LITERAL:
        text.load(_T("objid("))<<pLit->idObj.psz<<')';
    break;
    default:
        assert(!"??<compiler error>");    
        text.load(_T("??<compiler error>"));
    }

    return text;
}

unsigned long ASM_ForceDWORD(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP* pLit)
{
    if(pLit->type==ASM_DWORD)
    {
        return (unsigned long)pLit->any64;
    }
    else
    {
        FC_CString text;
        ASM_LitToString(text, pLit);
        ST_DiagMsg2(pEdp, E_4CCG_ERR_EXPECTED_LIT_OF_TYPE, ASM_TypeEnumToString(ASM_DWORD), text);
        return 0;
    }
}

long ASM_ForceDINT(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP* pLit)
{
    if(pLit->type==ASM_DINT)
    {
        return (long)pLit->any64;
    }
    else
    {
        FC_CString text;
        ASM_LitToString(text, pLit);
        ST_DiagMsg2(pEdp, E_4CCG_ERR_EXPECTED_LIT_OF_TYPE, ASM_TypeEnumToString(ASM_DINT), text);
        return 0;
    }
}

void ASM_ForceCString(const ST_EdiPos* pEdp, const ASM_LITERAL_TYP* pLit, FC_CString* pCstr)
{
    pCstr->clear();

    if(pLit->type != ASM_STRING)
    {
        FC_CString text;
        ASM_LitToString(text, pLit);
        ST_DiagMsg2(pEdp, E_4CCG_ERR_EXPECTED_LIT_OF_TYPE, ASM_TypeEnumToString(ASM_DWORD), text);
    }
    else
    {
	    long  nStrLen = (strlen(pLit->pszIECStr)+1)*2;
	    char* pBinStr = (char*)alloca(nStrLen*sizeof(char));

	    HRESULT hr = BL_IECStringToBinary(pLit->pszIECStr, pBinStr, &nStrLen, NULL, NULL);
	    assert(SUCCEEDED(hr));
        pCstr->load(pBinStr);
    }
}


//make this type name: ST: Array[1..5] of string[45] ==> DEF2: string45[5]
//make this type name: ST: string[45] ==> DEF2: string
//all other type we use pType->name.psz as it is
FC_CString& ASM_AppendDefFileTypeName(FC_CString& dmp, const ASM_DATA_TYP* pType)
{
    if(pType->type == ASM_STRING)
	{
		dmp<<_T("string");
		if(pType->bArray)
			dmp<<'('<<pType->lStrlen<<')'<<'['<<pType->nElems<<']';
	}
	else
	{
		dmp<<pType->name.psz;
	}
	return dmp;
}


void ASM_InitValToMemBlock(BYTE*                      pMem,
                           const ASM_INITIALISER_TYP* pInit,
                           ASM_BITOFF                 Offs,
                           size_t                     size,
                           __int64                    limit)
{
    assert(pInit->initType == ASM_INIT_LIT);

    switch(pInit->initType)
    {
    case ASM_INIT_LIT:
        {
            ASM_ValToMemBlock(pMem, pInit->lit.any64, Offs, size, limit);
            break;
        }
    default:
        assert( ! "wrong initialiser type");
    }
}

void ASM_ValToMemBlock(BYTE* pMem,
                       __int64 val,
                       ASM_BITOFF Offs,
                       size_t size,
                       __int64 limit)
{
    assert((ASM_BITOFF) limit >= Offs/8 + size/8);
    assert(Offs >= 0);

    switch(size)
    {
    case 1:
        {
            BYTE* p = pMem + Offs / 8;
            SETBIT(*p, (long)(Offs & 0x07), val);
            break;
        }
    case 8:
        {
            BYTE ch = (BYTE) val;
            memcpy(pMem+Offs/8, &ch, 1);
            break;
        }
    case 16:
        {
            WORD w = TRANSLATE_WORD((WORD) val);
            memcpy(pMem+Offs/8, &w, 2);
            break;
        }
    case 32:
        {
            DWORD dw = TRANSLATE_DWORD((DWORD) val);
            memcpy(pMem+Offs/8, &dw, 4);
            break;
        }
    case 64:
        {
            __int64 i64 = TRANSLATE_I64(val);
            memcpy(pMem+Offs/8, &i64, 8);
            break;
        }
    default:
        {
            assert( ! _T("invalid size !") );
            break;
        }
    }
}

void ASM_StrToFile(FILE* pFile, const ASM_LITERAL_TYP* lit)
{
	long  nStrLen = strlen(lit->pszIECStr);
	char* pBinStr = new char[nStrLen];

	HRESULT hr = BL_IECStringToBinary(lit->pszIECStr, pBinStr, &nStrLen, NULL, NULL);
	assert(SUCCEEDED(hr));

    fwrite(&nStrLen, sizeof(char), sizeof(char), pFile); // max len
    fwrite(&nStrLen, sizeof(char), sizeof(char), pFile); // cur len
    fwrite(pBinStr, sizeof(char), nStrLen, pFile); // string

	delete[] pBinStr;
}

void ASM_ValToFile(FILE* pFile, __int64 val, size_t size)
{
    switch(size)
    {
    case 1:
        {
            BYTE ch = (BYTE) val;
            fwrite(&ch, sizeof(char), 1, pFile);
            break;
        }
    case 2:
        {
            WORD w = TRANSLATE_WORD((WORD) val);
            fwrite(&w, sizeof(WORD), 1, pFile);
            break;
        }
    case 4:
        {
            DWORD dw = TRANSLATE_DWORD((DWORD) val);
            fwrite(&dw, sizeof(DWORD), 1, pFile);
            break;
        }
    case 8:
        {
            __int64 i64 = TRANSLATE_I64(val);
            fwrite(&i64, sizeof(__int64), 1, pFile);
            break;
        }
    default:
        {
            assert( ! _T("invalid size !") );
            break;
        }
    }
}

void ASM_InitBinStringToMemBlock(BYTE*                      pMem,
                                 const ASM_DATA_TYP*        pType,
                                 ASM_BITOFF                 Offs,
                                 const ASM_INITIALISER_TYP* pInit,
                                 __int64                    limit)
{
    assert(pInit->initType == ASM_INIT_LIT);

    switch(pInit->initType)
    {
    case ASM_INIT_LIT:
        {
            ASM_BinStringToMemBlock(pMem, pType, Offs, &pInit->lit, limit);
            break;
        }
    default:
        assert( ! "wrong initialiser type");
    }
}

void ASM_BinStringToMemBlock(BYTE*                  pMem,
                             const ASM_DATA_TYP*    pType,
                             ASM_BITOFF             Offs,
                             const ASM_LITERAL_TYP* lit,
                             __int64                limit)
{
    VM_STRING string;

    if(pType->lStrlen > 255)
        ASM_INTERNAL_ERROR_STOP(NULL);

	string.MaxLen = (BYTE) pType->lStrlen;

    assert((ASM_BITOFF) limit >= Offs/8 + pType->lStrlen + 2 * sizeof(BYTE));


    if (lit->pszIECStr && lit->type != ASM_TYPE_UNDEF)
    {
	    long  nStrLen = _tcslen(lit->pszIECStr);
		char* pBinStr = new char[nStrLen];

		HRESULT hr = BL_IECStringToBinary(lit->pszIECStr, pBinStr, &nStrLen, NULL, NULL);
		assert(SUCCEEDED(hr));
   
    	string.CurLen = (BYTE) nStrLen;

        for (int o=0; o<nStrLen && o<pType->lStrlen; o++)
            string.Contents[o] = pBinStr[o];
        for (o=nStrLen; o<pType->lStrlen; o++)
            string.Contents[o] = 'X';
	
		delete [] pBinStr;
    }
    else
    {
    	string.CurLen = 0;
        // dummy init **HACK**
        for (int o=0; o<pType->lStrlen; o++)
            string.Contents[o] = 'X';
    }
	
    long Off = 0/*i * (pType->lStrlen + 2 * sizeof(BYTE))*/;
    memcpy(pMem+Offs/8+Off, &string, pType->lStrlen + 2 * sizeof(BYTE));
}


bool ASM_ParseDirectAddr(const TCHAR* pszAddr, ASM_DIRECT_TYP* pResult)
{
    ASM_SEGM_TYP    location; 
    char            bitsize;  //1,8,16,32,64
    __int64         ulBytes;
    __int64         ulBits;
    const TCHAR*    psz;
    bool            bSwap = false;
    TCHAR*          psz_unconst;
    
    if(pResult)
    {
        pResult->loc = ASM_SEGM_UNDEF;
        pResult->bitsize = 0;
        pResult->bitoff = 0;
    }

    psz = pszAddr;
    if(*psz!=_T('%'))
        return false;

    psz = _tcsinc(psz);


    switch(*psz)
    {
    case 'X':  bitsize = 1; psz = _tcsinc(psz); break;
    case 'B':  bitsize = 8; psz = _tcsinc(psz); break;
    case 'W':  bitsize =16; psz = _tcsinc(psz); break;
    case 'D':  bitsize =32; psz = _tcsinc(psz); break;
    case 'L':  bitsize =64; psz = _tcsinc(psz); break;
    case 'S':  bitsize =8;  psz = _tcsinc(psz); break;
    default:
        bitsize = 1;
    }

    if(!_istdigit(*psz))
        return false;

    // select segment
    switch(*psz)
    {
    case '0':  location = ASM_SEGM_INPUT;  break;
    case '1':  location = ASM_SEGM_OUTPUT; break;
    case '2':  location = ASM_SEGM_GLOBAL; break;
    case '3':  location = ASM_SEGM_RETAIN; break;
    default:
        return false;
    }
    psz = _tcsinc(psz);

    if(*psz!=_T('.'))
        return false;

    psz = _tcsinc(psz);

    ulBytes = _tcstoul(psz, &psz_unconst, 10);
    if(ULONG_MAX==ulBytes)
        return false;

    psz = psz_unconst;
    
    if(bitsize==1)
    {
        if(*psz!=_T('.'))
            return false;
        psz = _tcsinc(psz);
        if(!_istdigit(*psz))
            return false;

        ulBits = _tcstoul(psz, &psz_unconst, 10);
        if(ULONG_MAX==ulBits)
            return false;

        ulBits = 8*ulBytes + ulBits;
        psz = psz_unconst;
    }
    else
    {
        ulBits = ulBytes*8;
    }

    //!swap !noswap (must be end of string)
    if(*psz==_T('!'))
    {
        if(!_tcsicmp(psz, _T("!swap")))
        {
            if(bitsize==16 || bitsize==32)
                bSwap = true;
            else
                return false;
        }
        else if(!_tcsicmp(psz, _T("!noswap")))
        {
            if(bitsize==16 || bitsize==32)
                bSwap = false;
            else
                return false;
        }
        else
        {
            return false;
        }
    }
    else if(*psz)
    {
        return false;
    }


    if(pResult)
    {
        pResult->loc = location;
        pResult->bitsize = bitsize;
        pResult->bitoff = ulBits;
        pResult->bSwap = bSwap;
    }
    
    return true;
}
const TCHAR* ASM_AddrToString(TCHAR buffer[ASM_MAX_DIRADDR_STRLEN], const ASM_DIRECT_TYP& addr)
{
    buffer[0] = _T('%');

    
    switch(addr.loc)
    {
    case ASM_SEGM_INPUT:  buffer[1] = _T('I'); break;
    case ASM_SEGM_OUTPUT: buffer[1] = _T('Q'); break;
    case ASM_SEGM_GLOBAL: buffer[1] = _T('S'); break;
    default:              buffer[1] = _T('M'); 
    }

    
    switch(addr.bitsize)
    {
    case 1 : buffer[2] = _T('X'); break;
    case 8 : buffer[2] = _T('B'); break;
    case 16: buffer[2] = _T('W'); break;
    case 32: buffer[2] = _T('D'); break;
    case 64: buffer[2] = _T('L'); break;
    default:
        assert(!"invalid size");
        buffer[2] = _T('?');
    }

    if(addr.bitsize==1)
    {
        wsprintf(buffer+3, _T("%d.%d"), addr.bitoff/8, addr.bitoff % 8);
    }
    else
    {
        ultoa((long)(addr.bitoff/8), buffer+3, 10);
    }
    
    if(addr.bSwap)
        _tcscat(buffer, _T("!swap"));

    return buffer;
}


bool ASM_CheckDirectAddUse(const ST_EdiPos*     pEdp, 
                           const ASM_DIRECT_TYP addr,
                           const ASM_DATA_TYP*  pType,
                           bool                 bHasInitVal)
{
    TCHAR  buffer[ASM_MAX_DIRADDR_STRLEN];

    if(addr.loc==ASM_SEGM_UNDEF)
        return false;

    bool bBadAddrForType = false;
    switch(addr.bitsize)
    {
    case 1:
        if(pType->type!=ASM_BOOL) 
            bBadAddrForType = true;
    break;
    case 8:
        if(pType->type!=ASM_BYTE && pType->type!=ASM_STRING && pType->type!=ASM_STRI && !pType->bArray)
            bBadAddrForType = true;
    break;
    case 16:
        if(pType->type!=ASM_WORD && pType->type!=ASM_INT && pType->type!=ASM_UINT) 
            bBadAddrForType = true;
    break;
    case 32:
        if(pType->type!=ASM_DINT && pType->type!=ASM_DWORD && pType->type!=ASM_UDINT && pType->type!=ASM_REAL) 
            bBadAddrForType = true;
    break;
    case 64:
        if(pType->type!=ASM_LREAL) 
            bBadAddrForType = true;
    break;
    default:
        bBadAddrForType = true;
    }
    if(bBadAddrForType)
    {
        ST_DiagMsg2(pEdp, E_4CCG_ERR_DIRADDR_TYPE, ASM_AddrToString(buffer, addr), pType->name.psz);
        return false;
    }

    if(bHasInitVal && addr.loc==ASM_SEGM_INPUT)
    {
        ST_DiagMsg1(pEdp, E_4CCG_ERR_INIT_AT_NON_LVALUE_ADR, ASM_AddrToString(buffer, addr));
        return false; 
    }
    return true;
}




unsigned short ASM_GetSizeOfOpcode(unsigned short Opcode)
{
    if (Opcode < 256)
        return 1;

    return 2;
}

long ASM_GetObjectID(ST_IDENT_TYP Ident, 
                     const ASM_INST_PATH_TYP* Path, 
                     const ASM_OBJECT_TYP** pSubObj,
                     int ArrIdx /* = -1 */)
{
    ASM_INST_PATH_TYP Id;

    if (ArrIdx == -1)
        Id = GetGC()->registerInstPathPlusMember(Path, Ident);
    else
        Id = GetGC()->registerInstPathPlusArrayMember(Path, Ident, ArrIdx);
    
    const ASM_OBJECT_TYP* pObj = GetGC()->lookUpObject(Id.ids);
    if (pObj == NULL)
    {
        ASM_INTERNAL_ERROR_STOP(NULL);
        assert(pObj != NULL);
        return -1;
    }

    if (pSubObj)
        *pSubObj = pObj;

    return pObj->Index;
}

static void ASM_WriteArg(TCHAR* buf, unsigned ArgSize, __int64 Arg)
{
    switch(ArgSize)
    {
    case 1:
    case 8:
        sprintf(buf, _T("%02x "), (unsigned char) Arg);
        break;
    case 16:
        sprintf(buf, _T("%04x "), (unsigned short) Arg);
        break;
    case 32:
        sprintf(buf, _T("%08lx "), (unsigned long) Arg);
        break;
    case 64:
        sprintf(buf, _T("%016I64x "), Arg);
        break;
    default:
        assert( ! _T("invalid Argsize !") );
        break;
    }
}

static void ASM_WriteStrArg(TCHAR* buf,
                            unsigned ArgSize, 
                            const ASM_LITERAL_TYP* lit,
                            int iBufSize)
{
	long  nStrLen = strlen(lit->pszIECStr);
	char* pBinStr = new char[nStrLen+1];

	HRESULT hr = BL_IECStringToBinary(lit->pszIECStr, pBinStr, &nStrLen, NULL, NULL);
	assert(SUCCEEDED(hr));

    sprintf(buf,   _T("%02x"), (unsigned short) nStrLen);   // max len
    sprintf(buf+2, _T("%02x"), (unsigned short) nStrLen);   // cur len

    assert(iBufSize > 4+nStrLen*2);
    if (iBufSize < 4+nStrLen*2)
    {
        ASM_INTERNAL_ERROR_STOP(NULL);
    }

    for (int i=0; i<nStrLen; i++)
    {
        sprintf(buf+4+i*2, _T("%02x"), (unsigned short) *(pBinStr+i));
    }
	delete[] pBinStr;
}

ASM_INSTR_TYP::ASM_INSTR_TYP(
    const ST_EdiPos*       pEdp, 
    ASM_INSTR_ENUM         instr, 
    ASMPROC_TYP*           pProcDef,
    const ASM_OPCDEF*      popc,
    int                    nArgs,
    ASM_PRIMARY_TYP**      pArgs
)
{
    m_edp   = *pEdp;

    m_instr = instr;
    m_popc  = popc;
    m_pProcDef = pProcDef;
    m_finalByteOff = -1;
    m_pNext = NULL;
    m_Opcode = -1;
    if(nArgs>0)
    {
        m_args.allocSize(nArgs);
        for(int i=0; i<nArgs; i++)
            m_args.add(*pArgs[i]);    
    }
    pProcDef->addInst(this);
}
ASM_INSTR_TYP::~ASM_INSTR_TYP()
{
}



inline bool IsAnyIntLit(const ASM_PRIMARY_TYP* pArg)
{
	if(pArg->argkind == ASM_PRM_LIT)
	{
		switch(pArg->lit.type)
		{
		case ASM_BYTE: case ASM_WORD: case ASM_DWORD:  
		case ASM_INT:  case ASM_DINT: case ASM_LINT:
		case ASM_UINT: case ASM_UDINT:
			return true;
		}
	}
	return false;
}

void ASM_INSTR_TYP::processVMOPC()
{
    int                 i;
    ASM_PRIMARY_TYP*    pArg;
    __int64             i64Arg;
    ASM_PAR_TYP         parTyp;
    long                lWideForce = 0;
	unsigned long       ulMaxLitVal;

    //check number of parameters:
    if(m_popc->nPar != m_args.getMaxUsed())
    {
        if(m_args.getMaxUsed() > m_popc->nPar)
            ST_DiagMsg1(&m_edp, E_ASM_TOO_MANY_PARAM, m_popc->pszId);
        else
            ST_DiagMsg1(&m_edp, E_ASM_TOO_LESS_PARAM, m_popc->pszId);

        return;
    }


    //build binary arguments according to par type:
    for(i=0; i<m_popc->nPar; i++)
    {
        pArg  = m_args.getAddrOf(i);
        parTyp= m_popc->parTyp[i];
        pArg->argSize = 0;
            
        switch(parTyp)
        {
        case ASM_PAR_L_OFF1:
        case ASM_PAR_L_OFF8:
            if(pArg->argkind == ASM_PRM_LOCALCOMP)
            {
                i64Arg = pArg->pLocal->declBitOff;
                if(parTyp==ASM_PAR_L_OFF8)
                {
                    if(i64Arg & 0x7)
                    {
                        ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_BYTEOFF, 
                            Tostr_C(i+1), m_popc->pszId, Tostr_C(i64Arg)
                        );
                    }
                    i64Arg /= 8;
                }
            }
            else if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else
            {
                ST_DiagMsg2(&m_edp, E_ASM_BADPAR_NEED_LOCAL, Tostr_C(i+1), m_popc->pszId);
                return;
            }

            m_binArgs.add(i64Arg);
        break;

        case ASM_PAR_G_OFF1:
        case ASM_PAR_G_OFF8:
            //must be a global object with fixed offset, that is a struct at %-Address:
            if(pArg->argkind == ASM_PRM_GLOBCOMP && pArg->pGComp->bitOff != ASM_INVALID_BITOFF && pArg->path.depth<=2)
            {
                i64Arg = pArg->pGComp->bitOff;
                if(pArg->path.depth>1) //HACK: for the case struct.member, add the member offset.
                {
                    i64Arg += pArg->pGComp->pType->pFBIDef->
                              lookUpLocalVar(FC_StringGetFileExt(pArg->path.ids.psz))
                              ->declBitOff;
                }
                if(parTyp==ASM_PAR_G_OFF8)
                {
                    if(i64Arg & 0x7)
                    {
                        ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_BYTEOFF, 
                            Tostr_C(i+1), m_popc->pszId, Tostr_C(i64Arg)
                        );
                        return;
                    }
                    i64Arg /= 8;
                }
            }
            else if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else
            {
                ST_DiagMsg2(&m_edp, E_ASM_BADPAR_NEED_GLOBAL, Tostr_C(i+1), m_popc->pszId);
                return;
            }

            m_binArgs.add(i64Arg);
        break;

        case ASM_PAR_OFFSETOF1:
        case ASM_PAR_OFFSETOF8:
            if(pArg->argkind == ASM_PRM_LIT && pArg->lit.type==ASM_OFFSET_LITERAL)
            {
				if(GetGC()->getTrgAttr()->trg_version >= 21000 && pArg->lit.idStruct.psz &&
					(_tcscmp(pArg->lit.idStruct.psz, _T("__global")) == 0 ||
					_tcscmp(pArg->lit.idStruct.psz, _T("__global_retain")) == 0))
				{
					const ASM_GLOB_COMP* pGlob;
					pGlob = GetGC()->lookUpGlobalVar(pArg->lit.idMember);
					i64Arg = pGlob->bitOff;
				}
				else
				{
					const ASMPROC_TYP* pType;
					if(pArg->lit.idStruct.psz)
					{
						//must be a function block or the like:
						pType = GetGC()->lookUpPrgFB(pArg->lit.idStruct);
						if(!pType)
							pType = GetGC()->lookUpFunction(pArg->lit.idStruct);

						if(!pType)
						{
							FC_CString jot;
							ST_DiagMsg1(&m_edp, E_4CCG_ERR_NO_DEF_FOUND, ASM_LitToString(jot, &pArg->lit));
							return;
						}
					}
					else
					{
						pType = m_pProcDef;
					}
					const ASM_DECL_TYP* pDecl = pType->lookUpLocalVar(pArg->lit.idMember);
					if(!pDecl)
					{
						ST_DiagMsg2(&m_edp, E_4CCG_ERR_UNDEF_MEMBER, pArg->lit.idMember.psz, pType->getName());
						return;
					}
               
					i64Arg = pDecl->declBitOff;
				}

                if(i64Arg==ASM_INVALID_BITOFF)
                    ASM_INTERNAL_ERROR_STOP(&m_edp);

                if(parTyp == ASM_PAR_OFFSETOF8)
                {
                    if(i64Arg & 0x7)
                    {
                        ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_BYTEOFF, 
                            Tostr_C(i+1), m_popc->pszId, Tostr_C(i64Arg)
                        );
                    }
                    i64Arg /= 8;
                }
            }
            else if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else
            {
                ST_DiagMsg2(&m_edp, E_ASM_BADPAR_OFFSETOF, Tostr_C(i+1), m_popc->pszId);
                return;
            }
            m_binArgs.add(i64Arg);

        break;
        case ASM_PAR_SIZE1:
            if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else if(pArg->argkind == ASM_PRM_SIZEOF1)
            {
                if(pArg->pType->type!=ASM_STRING && !pArg->pType->bArray && !pArg->pType->pFBIDef)//TODO
                {
                    i64Arg = ASM_SimpleTypeBitSize(pArg->pType);
                }
				else
				{
	               ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, _T("1"), _T("sizeof1"), _T("simple type*1"));
				   return;
				}
            }
            else
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, _T("bit size"));
                return;
            }
            m_binArgs.add(i64Arg);
        break;
        case ASM_PAR_SIZE8:
            if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else if(pArg->argkind == ASM_PRM_SIZEOF8)
            {
                i64Arg = ASM_GetVMStoreByteSize(pArg->pType);
            }
            else if(pArg->argkind == ASM_PRM_FUNLOCBYTES)
            {
                i64Arg = pArg->pType->pFBIDef->getFunLocalBytes();
            }
            else if(pArg->argkind == ASM_PRM_LEAVEBYTES)
            {
                i64Arg = pArg->pType->pFBIDef->getFunLeaveBytes();
            }
            else
            {
                ST_DiagMsg2(&m_edp, E_ASM_BADPAR_NEED_BYTESIZE, Tostr_C(i+1), m_popc->pszId);
                return;
            }
            m_binArgs.add(i64Arg);
        break;

        case ASM_PAR_OBJID:
            if(pArg->argkind==ASM_PRM_LIT && pArg->lit.type==ASM_OBJID_LITERAL)
            {
				if(GetGC()->getTrgAttr()->trg_version >= 21000 &&
					_tcscmp(pArg->lit.idObj.psz, _T("__global_inst")) == 0)
				{
						i64Arg = MAX_SEGMENTS;
				}
				else if(GetGC()->getTrgAttr()->trg_version >= 21000 &&
					_tcscmp(pArg->lit.idObj.psz, _T("__global_retain_inst")) == 0)
				{
						i64Arg = MAX_SEGMENTS+1;
				}
				else
				{
					const ASM_OBJECT_TYP* pObj = GetGC()->lookUpObject(pArg->lit.idObj);
					if(!pObj || pObj->Index<0)
					{
						assert(!pObj);
						ST_DiagMsg1(&m_edp, E_4CCG_ERR_UNDEF_VAR, pArg->lit.idObj.psz);
						return;
					}
					i64Arg = pObj->Index;
				}
            }
            else if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, _T("<objid>)"));
                return;
            }
            m_binArgs.add(i64Arg);
        break;

        case ASM_PAR_LIT8:   ulMaxLitVal = 0xff;       pArg->argSize = 1; goto anyLit;
        case ASM_PAR_LIT16:  ulMaxLitVal = 0xffff;     pArg->argSize = 2; goto anyLit;
        case ASM_PAR_LIT32:  ulMaxLitVal = 0xffffffff; pArg->argSize = 4; goto anyLit;
        case ASM_PAR_LIT64:  ulMaxLitVal = 0;          pArg->argSize = 8; goto anyLit;
		anyLit:
            if(pArg->argkind==ASM_PRM_LIT && pArg->argkind!=ASM_STRING)
            {
	            i64Arg = pArg->lit.any64;
				ulMaxLitVal = 0; //TODO ?? ignore value range
            }
            else if(pArg->argkind == ASM_PRM_SIZEOF1)
            {
                if(!pArg->pType->bArray && pArg->pType->type!=ASM_BOOL)//TODO array includes size WORD?
                    i64Arg = ASM_GetVMStoreByteSize(pArg->pType)*8;
				else if(pArg->pType->type==ASM_BOOL)
                    i64Arg = 1;
				else
				{
	               ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, _T("1"), _T("sizeof1"), _T("simple type*1"));
				   return;
				}
            }
            else if(pArg->argkind == ASM_PRM_SIZEOF8)
            {
                if(!pArg->pType->bArray && pArg->pType->type!=ASM_BOOL)//TODO array includes size WORD?
                    i64Arg = ASM_GetVMStoreByteSize(pArg->pType);
				else
				{
	               ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, _T("1"), _T("sizeof8"), _T("simple type*8"));
				   return;
				}
            }
            else if(pArg->argkind == ASM_PRM_FUNLOCBYTES)
            {
                i64Arg = pArg->pType->pFBIDef->getFunLocalBytes();
            }
            else if(pArg->argkind == ASM_PRM_LEAVEBYTES)
            {
                i64Arg = pArg->pType->pFBIDef->getFunLeaveBytes();
            }
            else
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, "LIT<X>");
                return;
            }

            if(ulMaxLitVal && (i64Arg<0 || (unsigned __int64)i64Arg>ulMaxLitVal))
            {
				TCHAR szBuffer[MAX_PATH];
				wsprintf(szBuffer, "[0..%u]", ulMaxLitVal);
                ST_DiagMsg4(&m_edp, E_ASM_BADPAR_RANGE, Tostr_C(i+1), m_popc->pszId, Tostr_C(i64Arg), szBuffer);
                return;
            }
            m_binArgs.add(i64Arg);
        break;
        case ASM_PAR_LITS8:
            if(pArg->argkind==ASM_PRM_LIT && pArg->lit.type==ASM_STRING)
            {
                long nStrLen = -1;
			    HRESULT hr = BL_IECStringToBinary(pArg->lit.pszIECStr, NULL, &nStrLen, NULL, NULL);
      			assert((hr == E_BL_ERR_BUFFER_TO_SMALL || SUCCEEDED(hr)) && nStrLen>=0);
                pArg->argSize = 2 + nStrLen;
            }
            else
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, "string literal");
                return;
            }
            //special case: add bin arg value 0 only to make the for loops m_binArgs work.
            m_binArgs.add(0);
        break;
        case ASM_PAR_CLSID:
            if(pArg->argkind != ASM_PRM_CLSID || (i64Arg = pArg->pType->pFBIDef->getClassId())<0)
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, _T("clsid(<pouname>)"));
                return;
            }
            m_pProcDef->insertCalledClass(pArg->pType->pFBIDef);
            m_binArgs.add(i64Arg);
        break;
        case ASM_PAR_FUNID:
            if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, _T("<funid>)"));
                return;
            }
            m_binArgs.add(i64Arg);
        break;

        case ASM_PAR_FBID:
            if(IsAnyIntLit(pArg) && pArg->lit.any64>=0)
            {
                i64Arg = pArg->lit.any64;
            }
            else if(pArg->argkind == ASM_PRM_FBID)
            {
                i64Arg = pArg->lId;
            }
            else
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, _T("<fbid>)"));
                return;
            }
            m_binArgs.add(i64Arg);
        break;

        case ASM_PAR_AIDX:
            lWideForce = -1;
            if(pArg->argkind==ASM_PRM_AIDX)
            {
                if(pArg->pType->type==ASM_INT)
                    lWideForce = 2;
                else if(pArg->pType->type==ASM_DINT)
                    lWideForce = 4;
                else
                    ASM_INTERNAL_ERROR_STOP(&m_edp);
            }

            if(lWideForce!=2 && lWideForce!=4)
            {
                ST_DiagMsg3(&m_edp, E_ASM_BADPAR_NEED_X, Tostr_C(i+1), m_popc->pszId, _T("<array index size>)"));
                return;
            }
        break;
        default:
            ASM_INTERNAL_ERROR_STOP(&m_edp);
        }
    }


    if(m_popc->nPar>0 && (m_popc->usFlags & ASM_MAX_PRFIX)!=0)
    {
        long lMaxArgByteSize = GetGC()->getTrgAttr()->trg_minOperandSize;
        for(i=0; i<m_binArgs.getMaxUsed(); i++)
        {
            i64Arg = m_binArgs.get(i);

            if(i64Arg <= _UI8_MAX)
            {
                if(lMaxArgByteSize < 1)
                    lMaxArgByteSize = 1;
            }
            else if(i64Arg <= _UI16_MAX)
            {
                if(lMaxArgByteSize < 2)
                    lMaxArgByteSize = 2;
            }
            else if(i64Arg <= _UI32_MAX)
            {
                if(lMaxArgByteSize < 4)
                    lMaxArgByteSize = 4;
            }
            else if(i64Arg <= _UI64_MAX)
            {
                if(lMaxArgByteSize < 8)
                    lMaxArgByteSize = 8;
            }
            else
            {
                ASM_INTERNAL_ERROR_STOP(&m_edp);
            }
        }

        if(lWideForce>0)
        {
            if(lWideForce < lMaxArgByteSize)
            {
                printf("%d %d\n", lWideForce, lMaxArgByteSize);
                ST_DiagMsg0(&m_edp, E_ASM_AIDX_PARM_INCOMPAT);
                ASM_INTERNAL_ERROR_STOP(&m_edp);
                return;
            }
            lMaxArgByteSize = lWideForce;
        }

        for(i=0; i<m_popc->nPar; i++)
        {
            if(m_popc->parTyp[i] == ASM_PAR_AIDX)
                continue;
            //set arg size if not already done for some special arguments:
            pArg = m_args.getAddrOf(i);
            if(pArg->argSize == 0) 
                pArg->argSize = lMaxArgByteSize;
        }

        if((m_popc->uscod & 0xff00) != 0x0000)
            ASM_INTERNAL_ERROR_STOP(&m_edp); //currently only opcode <256 have prfix defining params.

        // WIDE8 prefix is default:
        // check now which prefix will be correct
        switch(lMaxArgByteSize)
        {
        case 1: m_Opcode = m_popc->uscod; break;
        case 2: m_Opcode = (PREF_WIDE16<<8) | (m_popc->uscod & 0x00ff); break;
        case 4: m_Opcode = (PREF_WIDE32<<8) | (m_popc->uscod & 0x00ff); break;
        case 8: m_Opcode = (PREF_WIDE64<<8) | (m_popc->uscod & 0x00ff); break;
        default:
            ASM_INTERNAL_ERROR_STOP(&m_edp);
        }
    }
    else
    {
        //opcode gets no prefix or has is set already in intOpcds.h (as for
		//example all PUSHIX[_(0|1)]
        m_Opcode = m_popc->uscod;
    }

    if(m_Opcode == 0)
        ASM_INTERNAL_ERROR_STOP(&m_edp);

    //set total size of opcode including parameters:
    m_Size = ((unsigned short)m_Opcode)<256 ? 1 : 2;
    for(i=0; i<m_binArgs.getMaxUsed(); i++)
    {
        pArg = m_args.getAddrOf(i);
        if(pArg->argSize <= 0)
            ASM_INTERNAL_ERROR_STOP(&m_edp);
        m_Size += pArg->argSize;
    }
}

void ASM_INSTR_TYP::calcOpCodeSize1()
{
    switch(m_instr)
    {
    case ASM_VMOPC: 
        processVMOPC();
    break;
    case ASM_BPOP:
        ProcessBreakpointOPCode();
    break;
    case ASM_STMTBEG:
    case ASM_ST_INFO:
    case ASM_LABDEF:
        m_Size =0;
    break;
    case ASM_CALOP:
        ProcessCalop();
    break;
    case ASM_JMP:
    case ASM_JMPC:
    case ASM_JMPCN:
        m_Size = 2 + 4; //in pass one we assume worst case size 2 + 4
    break;
    default:
       ASM_INTERNAL_ERROR_STOP(&m_edp);
    }
}

void ASM_INSTR_TYP::calcOpCodeSize2()
{
    switch(m_instr)
    {
    case ASM_JMP:
    case ASM_JMPC:
    case ASM_JMPCN:
        break;
    default:
        return;
    }

    //resloveLabel set the m_binArgs(0), now reset the argSize:
    __int64 i64Arg = m_binArgs.get(0);
    ASM_PRIMARY_TYP* pArg = m_args.getAddrOf(0);

    if(pArg->argkind != ASM_PRM_LABEL)
        ASM_INTERNAL_ERROR_STOP(&m_edp);

    if(GetGC()->getTrgAttr()->trg_version >= 21050)
    {
        // optimize jumps just to 2 bytes and keep some free space for ncc code expantion
        if(i64Arg >= _I16_MIN/8 && i64Arg <= _I16_MAX/8)
        {
            pArg->argSize = 2;
        }
        else if(i64Arg >= _I32_MIN && i64Arg <= _I32_MAX)
        {
            pArg->argSize = 4;
        }
        else
        {
            ASM_INTERNAL_ERROR_STOP(&m_edp);
        }
    }
    else
    {
        pArg->argSize = 4;//do not optimze for old controllers
    }

    // get opcode
    m_Opcode = GetGC()->getOpcode(getOpCodeName());
    if (m_Opcode == 0)
        ASM_INTERNAL_ERROR_STOP(&m_edp);

    // check now which prefix will be correct
    switch(pArg->argSize)
    {
    case 1: break; // no prefix
    case 2: m_Opcode = (PREF_WIDE16<<8) | (m_Opcode & 0x00ff);break;
    case 4: m_Opcode = (PREF_WIDE32<<8) | (m_Opcode & 0x00ff);break;
    case 8: //wide 8 should not happen here...
    default:ASM_INTERNAL_ERROR_STOP(&m_edp);
    }

    m_Size = ASM_GetSizeOfOpcode( (unsigned short) m_Opcode) + pArg->argSize;
}

void ASM_INSTR_TYP::resolveLabel(const ASMPROC_TYP* pMyProc)
{
    switch(m_instr)
    {
    case ASM_JMP:
    case ASM_JMPC:
    case ASM_JMPCN:
        break;
    default:
        return;
    }

    ST_IDENT_TYP         label = m_args.get(0).label;
    const ASM_INSTR_TYP* pLblInst =  pMyProc->getLabelInstr(label);
    if(!pLblInst)
    {
        ST_DiagMsg1(&m_edp, E_4CCG_ERR_LABEL_UNDEF, label.psz);
    }
    else
    {
        // set jump offset
        long Offset = pLblInst->getFinalByteOffset();
        assert(m_finalByteOff>=0);
        __int64 iArg = (__int64) Offset - (__int64) m_finalByteOff;
        
        if(m_binArgs.getMaxUsed()==0)
            m_binArgs.add(iArg); //resolveLabel pass1
        else
            m_binArgs.set(0, iArg);//resolveLabel pass2 only adapt
    }
}




void ASM_INSTR_TYP::ProcessCalop()
{
    ASM_PRIMARY_TYP* pArg  = NULL;

    if(m_args.getMaxUsed() != 1)
        ASM_INTERNAL_ERROR_STOP(&m_edp);
    pArg = m_args.getAddrOf(0);
    
    assert(pArg->argkind == ASM_PRM_LABEL);
    // get the opcode
    m_Opcode = GetGC()->getOpcode(pArg->label.psz);
    if (m_Opcode == 0)
        ASM_INTERNAL_ERROR_STOP(&m_edp);

    m_Size = ASM_GetSizeOfOpcode( (unsigned short) m_Opcode);
}

void ASM_INSTR_TYP::ProcessBreakpointOPCode()
{
    //backward compatibillity for wago
    ASMGlobalContext_C* pGC      = GetGC();
    bool                b2ByteBP = pGC->getTrgAttr()->trg_version <= 20100;
    ASM_PRIMARY_TYP*    pArg     = m_args.getAddrOf(0);
    
    if(b2ByteBP)
    {
        if(pArg->argkind != ASM_PRM_LIT)
            ASM_INTERNAL_ERROR_STOP(&m_edp);
        
        //binary arg of bp opcode is "1 byte space" initialized to 0xff
        //this byte will be used as variable for the VM:
        pArg->argSize = 1;
        m_binArgs.set(0, 0xffffffffffffffff);
    }
    //else: there is no argument any more, and hence the 
    //complete BP opcode is only 1 byte long

    // get opcode
    m_Opcode = pGC->getOpcode(_T("BREAK"));
    if (m_Opcode == 0)
        ASM_INTERNAL_ERROR_STOP(&m_edp);

    unsigned short usSize = ASM_GetSizeOfOpcode( (unsigned short) m_Opcode);
    if(b2ByteBP)
        usSize += pArg->argSize;

    m_Size = usSize;
}




void ASM_INSTR_TYP::DumpBinary(FILE* pFile)
{
    if (m_Size == 0)
        return; // nothing to do (LABEL)

    // opcode
    if (ASM_GetSizeOfOpcode(m_Opcode) == 1)
    {
        // no prefix
        fwrite(&m_Opcode, sizeof(char), sizeof(char), pFile);
    }
    else
    {
        // first write prefix: code is interpreted byte wise !
        BYTE bPref  = (m_Opcode & 0xff00) >> 8;
        BYTE bInstr = (m_Opcode & 0x00ff);
        fwrite(&bPref,  sizeof(char), sizeof(char), pFile);
        fwrite(&bInstr, sizeof(char), sizeof(char), pFile);
    }

    // arguments
    int iMax = m_binArgs.getMaxUsed();
    for (int i=0; i<iMax; i++)
    {
        const ASM_PRIMARY_TYP* pArg = m_args.getAddrOf(i);
        __int64 Arg = m_binArgs.get(i);

        if (pArg->argkind == ASM_PRM_LIT && pArg->lit.type == ASM_STRING)
            ASM_StrToFile(pFile, &pArg->lit);
        else
            ASM_ValToFile(pFile, Arg, pArg->argSize);
    }
}

const TCHAR* ASM_INSTR_TYP::getOpCodeName()
{
    switch(m_instr)
    {
    case ASM_LABDEF:   return _T("<LABEL>:"); 
    case ASM_VMOPC:    return m_popc->pszId;
    case ASM_BPOP:     return _T("BRKP");
    case ASM_STMTBEG:  return _T("STMT");
    case ASM_ST_INFO:  return _T("<ST>");
    case ASM_CALOP:    return m_args.getAddrOf(0)->label.psz;
    case ASM_JMP:      return _T("JMP");
    case ASM_JMPC:     return _T("JMPC");
    case ASM_JMPCN:    return _T("JMPCN");
    default:
        ASM_INTERNAL_ERROR_STOP(&m_edp);    
        return NULL;
    }
}

void ASM_INSTR_TYP::MarkUsedOpcode(const char** pArray)
{
    switch(m_instr)
    {
    //case ASM_BPOP:
    case ASM_STMTBEG:
    case ASM_ST_INFO:
    case ASM_LABDEF:
    break;
    default:
        assert(m_Opcode < 1024*64);
        pArray[m_Opcode] = getOpCodeName();
    }
}

void ASM_INSTR_TYP::dumpIntoMapFile(ASM_BYTEOFF offset)
{
    int                         i;
    TCHAR                       buf[1024];
    const ASM_PRIMARY_TYP*      pArg;
    FC_CString                  jot(MAX_PATH);

    switch(m_instr)
    {
    case ASM_LABDEF:
        TO_MAPWRITE1(_T(" %04lu  "), offset);
        TO_MAPWRITE1(_T("  %s:\n"), m_args.get(0).label.psz);
    break;
    case ASM_VMOPC:

        TO_MAPWRITE1(_T(" %04lu  "), offset);
        //Binary:
        if(ASM_GetSizeOfOpcode((unsigned short) m_Opcode) == 1)
            sprintf(buf, _T("  %02x "), (unsigned short) m_Opcode);
        else
            sprintf(buf, _T("  %04x "), (unsigned short) m_Opcode);

        for(i=0; i<m_binArgs.getMaxUsed(); i++)
        {
            pArg = m_args.getAddrOf(i);
            int iLen = _tcslen(buf);
            if(pArg->argkind==ASM_PRM_LIT && pArg->lit.type==ASM_STRING)
                ASM_WriteStrArg(buf+iLen, pArg->argSize*8, &pArg->lit, FC_ARRAY_LEN(buf)-iLen);
            else
                ASM_WriteArg(buf+iLen, pArg->argSize*8, m_binArgs.get(i));
        }
        TO_MAPWRITE1(_T("%-30s"), buf);
        
        //Symbolic:
        TO_MAPWRITE1(_T("%-16s"), m_popc->pszId);
        for(i=0; i<m_popc->nPar; i++)
        {
            pArg = m_args.getAddrOf(i);
            switch(pArg->argkind)
            {
            case ASM_PRM_LIT:
                ASM_LitToString(jot, &pArg->lit);
                TO_MAPWRITE1(_T("%s "), jot.psz());
            break;
            case ASM_PRM_SIZEOF1:
                TO_MAPWRITE1(_T("sizeof1(%s) "), pArg->pType->name.psz);
            break;
            case ASM_PRM_SIZEOF8:
                TO_MAPWRITE1(_T("sizeof8(%s) "), pArg->pType->name.psz);
            break;
            case ASM_PRM_AIDX:
                TO_MAPWRITE1(_T("aidx(%s) "), pArg->pType->name.psz);
            break;
            case ASM_PRM_CLSID:
                TO_MAPWRITE1(_T("clsid(%s) "), pArg->pType->name.psz);
            break;
            case ASM_PRM_FBID:
                TO_MAPWRITE1(_T("fbid(%d) "), pArg->lId);
            break;
            case ASM_PRM_FUNLOCBYTES:
                TO_MAPWRITE1(_T("funlocbytes(%s) "), pArg->pType->name.psz);
            break;
            case ASM_PRM_LEAVEBYTES:
                TO_MAPWRITE1(_T("leavebytes(%s) "), pArg->pType->name.psz);
            break;
            case ASM_PRM_LOCALCOMP:
                TO_MAPWRITE1(_T("%s "), pArg->path.ids.psz?pArg->path.ids.psz:pArg->pLocal->id.psz);//HACK path not set for tmp vars !
            break;
            case ASM_PRM_GLOBCOMP:
                TO_MAPWRITE1(_T("%s "), pArg->path.ids.psz);
            break;
            case ASM_PRM_LABEL:
                TO_MAPWRITE1(_T("%s "), pArg->label.psz);
            break;
            default:
                ASM_INTERNAL_ERROR_STOP(&m_edp);
            }
        }
        TO_MAPWRITE0(_T("\n"));
    break;
    case ASM_BPOP:
        TO_MAPWRITE1(_T(" %04lu  "), offset);
        if (m_Opcode != -1)
        {
            if(GetGC()->getTrgAttr()->trg_version <= 20100)
                TO_MAPWRITE1(_T("  01 ff                       BpNum=          (%I64d)\n"), m_args.get(0).lit.any64);
            else
                TO_MAPWRITE1(_T("  01                          BRKP            (%I64d)\n"), m_args.get(0).lit.any64);
        }
    break;
    case ASM_STMTBEG:
        TO_MAPWRITE0(_T(" STMT\n"));
    break;
    case ASM_ST_INFO:
        TO_MAPWRITE1(_T("\nST>%s\n"), m_args.get(0).label.psz);
    break;
    case ASM_CALOP:
        TO_MAPWRITE1(_T(" %04lu  "), offset);

        if (ASM_GetSizeOfOpcode(m_Opcode) == 1)
            sprintf(buf, _T("  %02x "), m_Opcode);
        else
            sprintf(buf, _T("  %04x "), m_Opcode);

        TO_MAPWRITE2(_T("%-30s%-16s\n"), buf, getOpCodeName());
    break;
    case ASM_JMP:
    case ASM_JMPC:
    case ASM_JMPCN:
        TO_MAPWRITE1(_T(" %04lu  "), offset);

        if(m_args.getMaxUsed()!=1)
            ASM_INTERNAL_ERROR_STOP(&m_edp);

        if (ASM_GetSizeOfOpcode(m_Opcode) == 1)
            sprintf(buf, _T("  %02x "), m_Opcode);
        else
            sprintf(buf, _T("  %04x "), m_Opcode);

        ASM_WriteArg(buf+_tcslen(buf), m_args.getAddrOf(0)->argSize * 8, m_binArgs.get(0));

        TO_MAPWRITE3(_T("%-30s%-16sJmpOff(%I64d)\n"), buf, getOpCodeName(), m_binArgs.get(0));
    break;
    default:
        ASM_INTERNAL_ERROR_STOP(&m_edp);
    }
}

void ASM_INSTR_TYP::DumpDebugInfo(FILE* pFile)
{
    if(m_instr == ASM_BPOP)
    {
        fprintf (pFile, _T("%I64d;%lu;I\n"), m_args.getAddrOf(0)->lit.any64, m_finalByteOff);
    }
    else if(m_instr == ASM_STMTBEG)
    {
        // no BP can be set, this is only to inform NCC where a stmt begins:
        fprintf (pFile, _T("%I64d;%lu;X\n"), m_args.getAddrOf(0)->lit.any64, m_finalByteOff);
    }//else: nothing to do
}




ASMPROC_TYP::ASMPROC_TYP(const ST_EdiPos* pEdp, ASM_PROC_ENUM kind, ST_IDENT_TYP name, long opcode)
 :  m_hashLabelInstr(false, false),
    m_hashDecls(false, false)

{
    m_edp = *pEdp;
    m_kind = kind;
    m_name = name;
    m_xopcode = opcode;
    m_firstInstr = NULL;
    m_lastInstr = NULL;

    m_nSizeBytes = -1;
    m_nFunRetValBytes = -1;
    m_nFunParamBytes = -1;
    m_nFunLocalBytes = -1;

    m_alignNextDecl = -1; //HACK only for fb and prg to seperate bool in retain cpy sections
};
ASMPROC_TYP::~ASMPROC_TYP()
{
    int i;


    for(i=0;i<m_decls.getMaxUsed();i++)
        delete(m_decls.get(i));

    ASM_INSTR_TYP* pInst = NULL;
    ASM_INSTR_TYP* pTmpInst = NULL;
    for (pInst = m_firstInstr; pInst; )
    {
        pTmpInst = pInst;
        pInst = pInst->m_pNext;
        delete pTmpInst;
    }
}
const ASM_DECL_TYP* ASMPROC_TYP::addTmpVar(const ST_EdiPos*  pEdp, 
                                           ST_IDENT_TYP      id,
                                           int               bitSize,
                                           bool              bIsString)
{
    ASM_DECL_TYP*       pDecl;
    const ASM_DECL_TYP* pDeclConst;
    const ASM_DATA_TYP* pType;

    pDeclConst = lookUpLocalVar(id);
    if (pDeclConst)
        return pDeclConst; // already declared

    // create new decl
    pDecl = new ASM_DECL_TYP;
    ASM_InitDeclTyp(pDecl);

    pType = ASM_GetTmpVarTypeBySize(this, bitSize, bIsString);

    if (pType == NULL)
    {
        // maybe wrong bit size
        ASM_INTERNAL_ERROR_STOP(&m_edp);
        return pDecl;
    }

    pDecl->nCount = m_decls.getMaxUsed();
    m_decls.add(pDecl);
    const ASM_DECL_TYP* pPrev = m_hashDecls.put(pDecl, id.psz, false);
    assert(pPrev == NULL);
    
    pDecl->edp    = *pEdp;
    pDecl->id     = id;
    pDecl->scope  = ASM_DECL_VAR;
    pDecl->pType  = pType;
    pDecl->align  = m_alignNextDecl;
    pDecl->declBitOff = ASM_INVALID_BITOFF;
    m_alignNextDecl = -1;
    return pDecl;
}
void ASMPROC_TYP::addDecl(const ST_EdiPos*           pEdp, 
                          ST_IDENT_TYP               id, 
                          ASM_DECL_SCOPE             scope, 
                          const ASM_DATA_TYP*        pType,
                          const ASM_DIRECT_TYP&      opt_addr,
                          const ASM_INITIALISER_TYP& opt_init)
{
    ASM_DECL_TYP*       pDecl;
    const ASM_DECL_TYP* pDeclPrev;

    pDeclPrev = lookUpLocalVar(id);
    if(pDeclPrev)
    {
        TCHAR buffer[EDP_MAX_EDP_STR_LEN];
        ST_EdiPosToString(&pDeclPrev->edp, buffer);
        ST_DiagMsg2(pEdp, E_4CCG_ERR_REDEF_VAR, id.psz, buffer);
        return;
    }

    pDecl = new ASM_DECL_TYP;
    ASM_InitDeclTyp(pDecl);
    pDecl->nCount = m_decls.getMaxUsed();
    m_decls.add(pDecl);
    const ASM_DECL_TYP* pPrev = m_hashDecls.put(pDecl, id.psz, false);
    assert(pPrev == NULL);
    
    pDecl->edp      = *pEdp;
    pDecl->id       = id;
    pDecl->scope    = scope;
    pDecl->pType    = pType;
    pDecl->align    = m_alignNextDecl;
    pDecl->declBitOff   = ASM_INVALID_BITOFF;
    m_alignNextDecl = -1;
    
    if(opt_init.initType == ASM_INIT_LIT)
    {
        if (ASM_AreTypesAssignable(opt_init.lit.type, pType->type))
            pDecl->initval = opt_init;
        else
            ST_DiagMsg2(pEdp, E_4CCG_ERR_INIT_TYPE, ASM_TypeEnumToString(opt_init.lit.type), pType->name.psz);
    }
    else if(opt_init.initType == ASM_INIT_LIST)
    {
        int iMax = opt_init.pInitList->getUsed();
        for (int i=0; i<iMax; i++)
        {
            const ASM_INIT_REPEAT_VAL_TYP* pRptVal = opt_init.pInitList->getAddrOf(i);
            for (int o=0; o<pRptVal->nListSize; o++)
            {
                if ( ! ASM_AreTypesAssignable(pRptVal->pLitList[o].type, pType->type))
                    ST_DiagMsg2(pEdp, E_4CCG_ERR_INIT_TYPE, ASM_TypeEnumToString(pRptVal->pLitList[o].type), pType->name.psz);
            }
        }
        pDecl->initval = opt_init;
    }
    if(opt_addr.loc != ASM_SEGM_UNDEF)
    {
        if (ASM_CheckDirectAddUse(pEdp, opt_addr, pType, opt_init.lit.type != ASM_TYPE_UNDEF))
            pDecl->direct = opt_addr;
    }

    if(  (m_kind==ASM_FUN 
            && (    scope!=ASM_DECL_IN
                &&  scope!=ASM_DECL_OUT
                &&  scope!=ASM_DECL_VAR
                &&  scope!=ASM_DECL_INOUT
                &&  scope!=ASM_DECL_INEXT))
       ||(m_kind==ASM_FB 
            && (    scope!=ASM_DECL_IN
                &&  scope!=ASM_DECL_OUT
                &&  scope!=ASM_DECL_VAR
                &&  scope!=ASM_DECL_INOUT
                &&  scope!=ASM_DECL_VAR))
       ||(m_kind==ASM_PRG
            && (scope!=ASM_DECL_VAR
                && scope!=ASM_DECL_OUT))
        )
    {
        ST_DiagMsg2(pEdp, E_4CCG_ERR_INVALID_DECL_SCOPE, ASM_DeclScopeToString(scope), getPouTypeString());
        return;
    }
}

void ASMPROC_TYP::addInst(ASM_INSTR_TYP* pInst)
{
    if(isBuildIn())
        ST_DiagMsg0(pInst->getEdp(), E_4CCG_ERR_OPCODE_AND_CODE);

    if(!m_firstInstr)
    {
        m_firstInstr = m_lastInstr = pInst;
    }
    else
    {
        assert(!m_lastInstr->m_pNext && !pInst->m_pNext);
        m_lastInstr->m_pNext = pInst;
        m_lastInstr = pInst;
    }
}

void ASMPROC_TYP::insertInst(ASM_INSTR_TYP* pPred, ASM_INSTR_TYP* pInst)
{
    if (pPred == NULL)
    {
        ASM_INTERNAL_ERROR_STOP(&m_edp);    // should never be
        return;
    }

    pInst->m_pNext = pPred->m_pNext;
    pPred->m_pNext = pInst;
}

const TCHAR* ASMPROC_TYP::getPouTypeString()const
{
    switch(m_kind)
    {   
    case ASM_FUN: return _T(".fun");
    case ASM_FB: return _T(".fb");
    case ASM_PRG: return _T(".prg");
    case ASM_STRUCT: return _T(".struct");
    default:
        assert(!"bad enum");
        return _T("<internal error>");
    }
}


bool ASMPROC_TYP::insertLabel(const ASM_INSTR_TYP* pLabelInst)
{
    bool         bOk;
    const TCHAR* pszLbl;

    pszLbl = pLabelInst->getLabel()->psz;
    assert(pszLbl);
    bOk = m_hashLabelInstr.insert(pszLbl, pLabelInst);
    if(!bOk)
        ST_DiagMsg1(pLabelInst->getEdp(), E_4CCG_ERR_LABEL_DUPLY, pszLbl);

    return bOk;
}


void ASMPROC_TYP::buildCallStack(FC_CHashPtr<long, long>* listOfCalled)const
{
    const FC_CHashPtr<const ASMPROC_TYP*, long>::Pair* pPair;

    // insert itself
    listOfCalled->insert(m_ClassID, 0);

    // insert called
    for(pPair = m_Called.firstPair(); pPair; pPair = pPair->pNext)
    {
        const ASMPROC_TYP* pCalled = pPair->pKey;
        pCalled->buildCallStack(listOfCalled);
    }

}


//ASM_INVALID_BITOFF
void ASMPROC_TYP::calcObjectNumOfNested(
    const ASM_INST_PATH_TYP&   InstPath,
    const ASM_DATA_TYP*        pType,
    ASM_SEGM_TYP               segm
)const
{
    int                 i;
    ASM_INST_PATH_TYP   path;
    const ASM_DECL_TYP* pDecl;
    ASM_TYPE_ENUM       dataType;

    assert(segm!=ASM_SEGM_CODE && segm!=ASM_SEGM_CONST);


    // recurse into instance
    for(i=0;i<m_decls.getMaxUsed();i++)
    {
        pDecl = m_decls.get(i);
        dataType = pDecl->pType->type;
        assert(dataType != ASM_PRGI);

        if(pDecl->scope == ASM_DECL_INOUT)
            continue;//VAR_INOUT objects are no objects but only references

        if (pDecl->pType->bArray)
        {
            if (dataType == ASM_FBI || dataType == ASM_STRI)
            {
                // register the instance array itself
                path = GetGC()->registerInstPathPlusMember(&InstPath, pDecl->id);
                GetGC()->registerObject(path, pDecl->pType, NULL, segm, ASM_INVALID_BITOFF);
                //pDecl->pType->pFBIDef->calcObjectNumOfNested(path, pDecl->pType, segm);

                // register data type
                const ASM_DATA_TYP* pInstType = GetGC()->registerType(
                                pDecl->pType->type, 
                                pDecl->pType->pFBIDef->getTypeIdent(), 
                                pDecl->pType->pFBIDef);

                // register every array element tself
                for(unsigned iElem=0; iElem < pDecl->pType->nElems; iElem++)
                {
                    path = GetGC()->registerInstPathPlusArrayMember(&InstPath, pDecl->id, iElem);
                    GetGC()->registerObject(path, pInstType, NULL, segm, ASM_INVALID_BITOFF);
                    pDecl->pType->pFBIDef->calcObjectNumOfNested(path, pInstType, segm);
                }
            }
            else
            {
                path = GetGC()->registerInstPathPlusMember(&InstPath, pDecl->id);
                GetGC()->registerObject(path, pDecl->pType, &pDecl->initval, segm, ASM_INVALID_BITOFF);
            }
        }
        else if (dataType == ASM_FBI || dataType == ASM_STRI)
        {
            path = GetGC()->registerInstPathPlusMember(&InstPath, pDecl->id);
            GetGC()->registerObject(path, pDecl->pType, NULL, segm, ASM_INVALID_BITOFF);
            pDecl->pType->pFBIDef->calcObjectNumOfNested(path, pDecl->pType, segm);
        }
    }
}


void ASMPROC_TYP::calcInstructions()
{
    ASM_BYTEOFF    Offset;
    ASM_INSTR_TYP* pInstr;

    //pass 1: calculate size of all opcodes and assume jumps are worstcase 32 bit wide: 
    Offset = 0;
    for(pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->calcOpCodeSize1();
        pInstr->setFinalByteOffset(Offset);
        Offset += pInstr->getSize();
    }

    //pass 2: now resolve all jumps in this "wortcase" szenario:
    for(pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->resolveLabel(this);
    }

    //pass 3: most jumps will now be small 1 or 2 byte wide, recalculate 
    //the opcode size of all jumps:
    Offset = 0;
    for(pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->calcOpCodeSize2();
        pInstr->setFinalByteOffset(Offset);
        Offset += pInstr->getSize();
    }

    //pass 4: now resolve all jumps again:
    for(pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->resolveLabel(this);
    }

    m_CodeSize = Offset;
}


void ASMPROC_TYP::DumpCodeblockHeader(FILE* pFile)const
{
    XDownHeader cbHeader;
    TCHAR       buffer1[MAX_PATH];
    TCHAR       buffer2[MAX_PATH];

    DWORD sizePou = (unsigned long)ASM_AlignBitOffset(m_CodeSize, 2);//TOOD align ok?
    if(sizePou > 65365)
    {
        sprintf(buffer1,_T("code.%s"), getName()); 
        ST_DiagMsg3(&m_edp, E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE,
            buffer1, _ltot(sizePou, buffer2, 10), _T("65365"));
    }
    
    sprintf(buffer1, _T("%04u"), m_ClassID); 
    sprintf(buffer2, _T("ip:%6u ;%s"), m_CodeSize, getName()); 

    GetGC()->writeMemInfoStr(_T("codesizes"), buffer1, buffer2);

    cbHeader.uIndex   = TRANSLATE_WORD((IEC_UINT) m_ClassID);
    cbHeader.uSegment = TRANSLATE_WORD((IEC_UINT) SEG_CODE);
    cbHeader.ulSize   = TRANSLATE_DWORD(m_CodeSize);
    cbHeader.ulOffset = 0;
    cbHeader.byFixed  = 0;
	switch(m_kind)
	{
	case ASM_FUN: 
		cbHeader.byType = VMM_OT_FUN; break;
	case ASM_FB:
		cbHeader.byType = VMM_OT_FB; break;
    case ASM_PRG:
		cbHeader.byType = VMM_OT_PROG; break;
    case ASM_STRUCT:
		cbHeader.byType = VMM_OT_STRUCT; break;
	default:
		cbHeader.byType = VMM_OT_UNKNOWN; break;
	}

    fwrite(&cbHeader, sizeof(char), sizeof(XDownHeader), pFile);
}

void ASMPROC_TYP::dumpCode(FILE* pFile)const
{
    ASM_INSTR_TYP* pInstr;

    DumpCodeblockHeader(pFile);

    for (pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->DumpBinary(pFile);
    }
}

void ASMPROC_TYP::dumpUsedOpcodes(const char** pArray)const
{
    ASM_INSTR_TYP* pInstr;

    for (pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->MarkUsedOpcode(pArray);
    }
}

void ASMPROC_TYP::dumpCodeIntoMapFile()const
{
    ASM_INSTR_TYP* pInstr;
    ASM_BYTEOFF    offset = 0;

    for (pInstr = m_firstInstr; pInstr; pInstr = pInstr->m_pNext)
    {
        pInstr->dumpIntoMapFile(offset);
        offset += pInstr->getInstrByteSize();
    }

    TO_MAPWRITE1(_T("CodeSize=%lu\n"), offset);
}


void ASMPROC_TYP::dumpDebugInfo(FILE* pFile)const
{
    int    iMax = m_BpPosList.getMaxUsed();

    for (int i=0; i<iMax; i++)
    {
        ASM_INSTR_TYP* pInstr = m_BpPosList.get(i);
        
        if(pInstr->getKind()==ASM_STMTBEG && i+1<iMax) //workaround for ##d-930 don't dump 
        {                                              //same ofset twice, last will not be update by NCC
                                                       //but don't miss out real BPs !
            if(m_BpPosList.get(i+1)->getFinalByteOffset() == pInstr->getFinalByteOffset())
                continue;
        }

        pInstr->DumpDebugInfo(pFile);
    }
}

//meaning of the 8 fields of a decl in a def file:
//name;typechar;segm;typeName;byteoff;bitoff;bitsize;strlen
// 1      2       3     4        5       6      7      8
//#define CSV_IDX_NAME         1
//#define CSV_IDX_TYPECHAR     2           /* S,N,G,S*,N*  (* == pointer)                                         */
//#define CSV_IDX_SEGMENT      3           /* Segment if appropriate, else nix                              */
//#define CSV_IDX_DATA_TYPE    4           /* BOOL, LREAL, MyType, ...                                            */
//#define CSV_IDX_BYTE_OFFSET  5           /* Offset in byte of member in parent structure/segment */
//#define CSV_IDX_BIT_OFFSET   6           /* Offset in bit (0-7) for BOOL variables, else nix            */    
//#define CSV_IDX_BIT_SIZE     7           /* Size of variable, index or pointer in bit               */
//#define CSV_IDX_STRLEN       8           /* Maximum number of characters if STRING[], else nix   */
void ASMPROC_TYP::dumpPrgFBClassDefinition(FILE* pFile)const
{
	int					i;
	int					nMax;
	const ASM_DECL_TYP*	pDecl;
	const ASM_DATA_TYP* pType;
	ASM_TYPE_ENUM		type;
	BOOL				bIsObj;
	BOOL				bIsPtr;
    FC_CString			defDecl(256);
	
	assert(m_kind!=ASM_FUN);

	nMax = m_decls.getMaxUsed();
    for(i=0; i<nMax; i++)
    {
        pDecl  = m_decls.get(i);
		pType  = pDecl->pType;
		type   = pType->type;
		bIsObj = ASM_IsObject(pType);
		bIsPtr = pDecl->scope == ASM_DECL_INOUT;
		
		//name
		defDecl.load(pDecl->id.psz)<<';';

		//typechar
		defDecl<<(bIsObj ? 'S' : 'N');
		if(bIsPtr)
			defDecl<<'*';
		defDecl<<';';

		//segm
		defDecl<</*nix*/';';
		
		//typeName
        ASM_AppendDefFileTypeName(defDecl, pType)<<';';
		
		//byteoff
		defDecl<<(long)(pDecl->declBitOff / 8)<<';';
		
		//bitoff
		if(type==ASM_BOOL && !bIsPtr && !bIsObj)
			defDecl<<(long)(pDecl->declBitOff % 8);
		//else: nix
		defDecl<<';';
		
		//bitsize
		defDecl<<pDecl->declBitSize<<';';

		//strlen
		if(type==ASM_STRING && !bIsPtr && !bIsObj)
			defDecl<<pType->lStrlen;
		//else: nix
		defDecl<<'\n';
		
		fwrite(defDecl.psz(), defDecl.len(), 1, pFile);
    }
}

unsigned long ASMPROC_TYP::dumpInitialValues(
    FILE*                 pFile,
    const ASM_OBJECT_TYP* pObj
)const
{
    // DOWNLOAD HEADER
    GetGC()->DumpObjblockHeader(pFile, pObj);

    unsigned long ulSize = pObj->byteSize;
    
    BYTE* pInit = NULL;
    if(ulSize != 0)
    {
        pInit = new BYTE[(long)ulSize];
        memset(pInit, 0, (long)ulSize);
    }

    // first take the class init values
    for (int i=0; i<m_decls.getMaxUsed(); i++)
    {
        ASM_DECL_TYP* pDecl = m_decls.get(i);
        ProcessDeclInitValue(pDecl, &pDecl->initval, &pObj->path, pInit, ulSize);
    }

    // now take the instance specific init values
    int iMax = pObj->listOfMemberInitVal.getUsed();
    for (i=0; i<iMax; i++)
    {
        const ASM_MEMBER_INIT* pMI = pObj->listOfMemberInitVal.getAddrOf(i);

        if (pMI->pDecl->pType->bArray)
        {
            assert(   pMI->pDecl->pType->type != ASM_FBI
                   && pMI->pDecl->pType->type != ASM_STRI
                   && pMI->pDecl->pType->type != ASM_PRGI);
        }
        else
        {
            ProcessDeclInitValue(pMI->pDecl, &pMI->initVal, &pObj->path, pInit, ulSize);
        }
    }

    fwrite(pInit, sizeof(char), ulSize, pFile);

    ((ASM_OBJECT_TYP*) pObj)->bInit = true;
    delete[] pInit;

    return ulSize;
}

void ASMPROC_TYP::ProcessDeclInitValue(const ASM_DECL_TYP*        pDecl,
                                       const ASM_INITIALISER_TYP* pInitval,
                                       const ASM_INST_PATH_TYP*   pObjPath,
                                       BYTE*                      pInit,
                                       unsigned long              ulSize)const
{
    if (pDecl->scope == ASM_DECL_INOUT)
    {
        // dummy: 0xcfcfcfcf
        ASM_INITIALISER_TYP dummy_init;
        dummy_init.initType = ASM_INIT_LIT;
        dummy_init.lit.type = ASM_DWORD;
        dummy_init.lit.any64 = 0xcfcfcfcf;
        ASM_InitValToMemBlock(
            pInit,
            &dummy_init,
            pDecl->declBitOff,
            VM_ADDRESS_SIZE,
            ulSize);
    }
    else if (pDecl->pType->type == ASM_STRING && ! pDecl->pType->bArray)
    {
        // STRING
        ASM_InitBinStringToMemBlock(
            pInit,
            pDecl->pType,
            pDecl->declBitOff,
            pInitval,
            ulSize);
    }
    else if (ASM_IsSimpleType(pDecl->pType) && pDecl->pType->type != ASM_STRING)
    {
        // SIMPLE
        ASM_InitValToMemBlock(
            pInit,
            pInitval,
            pDecl->declBitOff,
            ASM_SimpleTypeBitSize(pDecl->pType),
            ulSize);
    }
    else
    {
        // OBJECTs
        long ObjIdx = ASM_GetObjectID(pDecl->id, pObjPath, NULL);
        if (ObjIdx != -1)
        {
            ASM_ValToMemBlock(
                pInit,
                ObjIdx,
                pDecl->declBitOff,
                VM_OBJ_INDEX_SIZE,
                ulSize);
        }
    }
}

void ASMPROC_TYP::calcOffsetLiterals()const
{
    ASM_DECL_TYP*        pDecl;
    const ASM_DECL_TYP*  pDeclOffs;
    bool                 bHasLitOff = false;

    for (int i=0; i<m_decls.getMaxUsed(); i++)
    {
        pDecl = m_decls.get(i);

        if (pDecl->initval.initType == ASM_INIT_LIST)
            continue;

        if (pDecl->initval.lit.type == ASM_OFFSET_LITERAL)
        {
            // offsetof(<ident>) syntax
            assert(pDecl->declBitOff != ASM_INVALID_BITOFF);
            assert(!pDecl->initval.lit.idStruct.psz);
            pDeclOffs = lookUpLocalVar(pDecl->initval.lit.idMember);
            if(pDeclOffs == NULL)
            {
                ST_DiagMsg1(&pDecl->edp, E_4CCG_ERR_UNDEF_VAR, pDecl->initval.lit.idMember.psz);
            }
            else
            {
                LPCTSTR psz = pDecl->initval.lit.idMember.psz;

                if(pDeclOffs->pType->type == ASM_BOOL)
                    pDecl->initval.lit.any64 = pDeclOffs->declBitOff;
                else
                    pDecl->initval.lit.any64 = pDeclOffs->declBitOff / 8;

                pDecl->initval.lit.type = pDecl->pType->type;
                
                if(!bHasLitOff)
                {
                    TO_MAPWRITE1(_T(".begin %s:\n"), m_name.psz);
                    bHasLitOff = true;
                }

                TO_MAPWRITE3(_T("%s := %ld  [offsetof(%s)]\n"), 
                    pDecl->id.psz,
                    (long) pDecl->initval.lit.any64,
                    psz);
            }
        }
    }

    if(bHasLitOff)
        TO_MAPWRITE0(_T(".end\n"));
}


void ASMPROC_TYP::calcMemberOffsets()
{
    int                 i;
    ASM_DECL_TYP*       pDecl;
    const ASM_DATA_TYP* pType;
    ASM_BITOFF32        lastBitOff;
    ASM_BITSIZE         declBitSize;
    ASM_BITSIZE         instAlign = GetGC()->getTrgAttr()->trg_instAlignBytes*8;
    int                 nDecls = m_decls.getMaxUsed();
    ASM_BITSIZE         declAlign;
    ASM_BITSIZE         lMaxDeclAlignBits = 8;

    assert(m_nSizeBytes==-1);

    lastBitOff = 0; // start address for non boolean members
    for(i=0; i<nDecls; i++)
    {
        pDecl = m_decls.get(i);
        pType = pDecl->pType;
        assert(pDecl->declBitOff==ASM_INVALID_BITOFF && pDecl->declBitSize==0);

        if(pDecl->scope==ASM_DECL_INOUT)
        {
            if(pType->type == ASM_BOOL && !pType->bArray)
                declBitSize = VM_ADDRESS_SIZE*2;
            else
                declBitSize = VM_ADDRESS_SIZE;
            declAlign   = min(declBitSize, instAlign);
        }
        else if(pType->bArray || pType->pFBIDef)
        {
            declBitSize = VM_OBJ_INDEX_SIZE;
            declAlign   = min(declBitSize, instAlign);
        }
        else if(pType->type==ASM_STRING)
        {
            declBitSize = ASM_SimpleTypeBitSize(pType);
            declAlign   = 8;
        }
        else if(pType->type==ASM_BOOL)
        {
            declBitSize = 1;
            if(pDecl->align==8)
                declAlign = pDecl->align;
            else
                declAlign = 1;
        }
        else
        {
            declBitSize = ASM_BasicTypeBitSize(pType->type);
            declAlign   = min(declBitSize, instAlign);
            //TODO align directive must only be applied 
            //if 8,BOOL and no INOUT. We use it only to prevent BOOL at bit positions
            //and the 4cvm backend dumps an align8 simply at the end of the header
            //for a possible temp var decl that gets added later implicitely!
            if(pDecl->align==8)
                declAlign = pDecl->align;
        }
        lMaxDeclAlignBits = max(declAlign, lMaxDeclAlignBits); 
        pDecl->declBitSize = declBitSize;
        pDecl->declBitOff = lastBitOff = ASM_AlignBitOff32(lastBitOff, declAlign);
        lastBitOff += declBitSize;
    }
    
    // align to max decl alignemt:
    lastBitOff = ASM_AlignBitOff32(lastBitOff, lMaxDeclAlignBits);
    
    //set the object byte size:
    assert(lastBitOff % 8 == 0);
    m_nSizeBytes = lastBitOff / 8;    

    if(GetGC()->isMapFileCreated())
    {
        int iMaxTypeLen=0;
        int iMaxIdLen=0;

        TO_MAPWRITE3(_T("\n%s %s: bytesize=%d\n"), getPouTypeString(), m_name.psz, m_nSizeBytes);
        for(i=0; i<nDecls; i++)
        {
            pDecl = m_decls.get(i);
            int iLen = _tcslen(pDecl->pType->name.psz);
            if(iLen>iMaxTypeLen)
                iMaxTypeLen = iLen;
            iLen = _tcslen(pDecl->id.psz);
            if(iLen>iMaxIdLen)
                iMaxIdLen = iLen;
        }

        for(i=0; i<nDecls; i++)
        {
            const TCHAR* pszStore = _T("");

            pDecl = m_decls.get(i);
            pType = pDecl->pType;
            if(pType->bArray || pType->pFBIDef)
                pszStore = _T("#");
            else if(pDecl->scope==ASM_DECL_INOUT)
                pszStore = _T("*");

            TCHAR szOffStr[64];
            if(pType->type==ASM_BOOL && !pType->bArray && pDecl->scope!=ASM_DECL_INOUT)
                sprintf(szOffStr, _T("%I64d.%d"),pDecl->declBitOff/8, (int)(pDecl->declBitOff & 0x7));
            else
                sprintf(szOffStr, _T("%I64d"), pDecl->declBitOff/8);
    
            TO_MAPWRITE8(_T("  %-5s %s%-*s %-*s AT %-6s [%-2d bit]\n"), ASM_DeclScopeToString(pDecl->scope),
                pType->name.psz, iMaxTypeLen+1-_tcslen(pType->name.psz), pszStore, 
                iMaxIdLen+1, pDecl->id.psz, szOffStr, pDecl->declBitSize);
        }
        TO_MAPWRITE0(_T("\n\n\n"));
    }


    ASM_BYTESIZE ulMaxObjectByteSize = ((1 << 16) - 1); //TODO???
    if(m_nSizeBytes > ulMaxObjectByteSize)
    {
        FC_CString segmName(30);
        segmName<<getName()<<_T(".data");
        ST_DiagMsg3(getEdp(), E_4CCG_ERR_SEGM_EXCEEDS_MAX_SIZE, 
            (LPCTSTR) segmName, Tostr_C(m_nSizeBytes), Tostr_C(ulMaxObjectByteSize));
        return;
    }
}
void ASMPROC_TYP::calcFunMemberOffsets()
{
    int                 i;
    ASM_DECL_TYP*       pDecl;
    const ASM_DATA_TYP* pType;
    ASM_DECL_TYP*       pFunRetDecl;
    ASM_BITOFF32        lastBitOff;
    ASM_BITSIZE         declBitSize;
    ASM_BITSIZE         stackAlign = GetGC()->getTrgAttr()->trg_stackAlignBytes*8;
    ASM_BITSIZE         instAlign = GetGC()->getTrgAttr()->trg_instAlignBytes*8;
    ASM_BITSIZE         declAlign;
    int                 nDecls = m_decls.getMaxUsed();

    assert(m_nSizeBytes==-1);

    lastBitOff = 0;
    pFunRetDecl= NULL;

    //first pass only VAR
    for(i=0; i<nDecls; i++)
    {
        pDecl = m_decls.get(i);
        if(pDecl->scope != ASM_DECL_VAR)
        {
            if(pDecl->scope==ASM_DECL_OUT && !_tcscmp(getName(), pDecl->id.psz))
                pFunRetDecl = pDecl;

            continue; 
        }
        assert(pDecl->declBitOff==ASM_INVALID_BITOFF && pDecl->declBitSize==0);
        pType = pDecl->pType;

        if(pType->bArray || pType->pFBIDef)
        {
            ASM_INTERNAL_ERROR_STOP(&pDecl->edp);//not supported yet.
        }
        else if(pType->type==ASM_BOOL)
        {
            declBitSize = 1;
            declAlign   = 8;//for performance no bit merging in functions
        }
        else if(pType->type==ASM_STRING)
        {
            declBitSize = ASM_SimpleTypeBitSize(pType);
            declAlign   = 8;
        }
        else
        {
            declBitSize = ASM_BasicTypeBitSize(pType->type);
            declAlign   = min(declBitSize, instAlign);
        }
        pDecl->declBitSize = declBitSize;
        pDecl->declBitOff = lastBitOff = ASM_AlignBitOff32(lastBitOff, declAlign);
        lastBitOff += declBitSize;
    }

    //since all fun memory local+param is on stack we must align the local size 
    //to stackAlign:
    lastBitOff = ASM_AlignBitOff32(lastBitOff, stackAlign);

    //remember the size in byte of local vars:
    m_nFunLocalBytes = lastBitOff/8;


    //now params: use lastBitOff like 'SP', grow to negative values.
    //function return if any:
    lastBitOff = 0;
    if(pFunRetDecl)
    {
        pType = pFunRetDecl->pType;
        if(pType->bArray || pType->pFBIDef || pType->type==ASM_STRING)
            pFunRetDecl->declBitSize = VM_ADDRESS_SIZE;
        else 
            pFunRetDecl->declBitSize = ASM_BasicTypeBitSize(pType->type);

        m_nFunRetValBytes = max(pFunRetDecl->declBitSize, stackAlign)/8;
        lastBitOff -= m_nFunRetValBytes*8;
        pFunRetDecl->declBitOff = lastBitOff;
    }
    else
    {
        m_nFunRetValBytes = 0; 
    }


    //input relevant decls: in reverse order as they are also pushed
    for(i=nDecls-1; i>=0; i--)
    {
        pDecl = m_decls.get(i);
        if(pDecl->scope == ASM_DECL_VAR || pDecl==pFunRetDecl)
            continue; 
        assert(pDecl->declBitOff==ASM_INVALID_BITOFF && pDecl->declBitSize==0);
        pType = pDecl->pType;

        if(pDecl->scope==ASM_DECL_OUT || pDecl->scope==ASM_DECL_INOUT)
        {
            if(pType->type == ASM_BOOL && !pType->bArray)
                pDecl->declBitSize = VM_ADDRESS_SIZE*2;
            else
                pDecl->declBitSize = VM_ADDRESS_SIZE;
        }
        else if(pType->bArray || pType->pFBIDef || pType->type==ASM_STRING)
        {
            assert(pDecl->scope==ASM_DECL_IN || pDecl->scope==ASM_DECL_INEXT);
            pDecl->declBitSize = VM_ADDRESS_SIZE;
        }
        else
        {
            assert(pDecl->scope==ASM_DECL_IN || pDecl->scope==ASM_DECL_INEXT);
            pDecl->declBitSize = ASM_BasicTypeBitSize(pType->type);
        }
        lastBitOff -= max(pDecl->declBitSize, stackAlign);
        pDecl->declBitOff = lastBitOff;
    }

    //since all fun memory local + params is on stack we must align the total size 
    //to stackAlign:
    m_nFunParamBytes = -(lastBitOff/8);
    m_nSizeBytes     = m_nFunLocalBytes + m_nFunParamBytes;

    //all decls that have neg. offsets are params (also the ret val):
    //add fun local+param total size for final offset inside function:
    for(i=nDecls-1; i>=0; i--)
    {
        pDecl = m_decls.get(i);
        if(pDecl->declBitOff<0)
        {
            pDecl->declBitOff += m_nSizeBytes*8; 
            if((pDecl->declBitOff % stackAlign)!=0 || pDecl->declBitOff<0)
                ASM_INTERNAL_ERROR_STOP(&pDecl->edp);
        }
    }

    if(GetGC()->isMapFileCreated())
    {
        int iMaxTypeLen=0;
        int iMaxIdLen=0;

        TO_MAPWRITE1(_T("\n.fun %s"), m_name.psz);
        if (m_xopcode >= 0)
            TO_MAPWRITE1(_T(" %d"), m_xopcode);
        TO_MAPWRITE4(_T(": bytesize=%d (par=%d loc=%d ret=%d)\n"), 
            m_nSizeBytes, m_nFunParamBytes, m_nFunLocalBytes, m_nFunRetValBytes);

        for(i=0; i<nDecls; i++)
        {
            pDecl = m_decls.get(i);
            int iLen = _tcslen(pDecl->pType->name.psz);
            if(iLen>iMaxTypeLen)
                iMaxTypeLen = iLen;
            iLen = _tcslen(pDecl->id.psz);
            if(iLen>iMaxIdLen)
                iMaxIdLen = iLen;
        }
        for(i=0; i<nDecls; i++)
        {
            const TCHAR* pszStore = _T("");

            pDecl = m_decls.get(i);
            pType = pDecl->pType;

            if(pDecl->scope!=ASM_DECL_VAR)
            {
                if(pDecl == pFunRetDecl)
                {
                    if(pType->bArray || pType->pFBIDef || pType->type==ASM_STRING)
                        pszStore = _T("*");
                }
                else if(pDecl->scope==ASM_DECL_OUT || pDecl->scope==ASM_DECL_INOUT)
                    pszStore = _T("*");
                else if(pType->bArray || pType->pFBIDef || pType->type==ASM_STRING)
                    pszStore = _T("*");
            }

            TO_MAPWRITE8(_T("  %-5s %s%-*s %-*s AT %-6I64d [%-2d bit]\n"), ASM_DeclScopeToString(pDecl->scope),
                pType->name.psz, iMaxTypeLen+1-_tcslen(pType->name.psz), pszStore, iMaxIdLen+1, pDecl->id.psz, 
                pDecl->declBitOff/8, pDecl->declBitSize);
        } 
        TO_MAPWRITE0(_T("\n\n\n"));
    }
}

/* ---------------------------------------------------------------------------- */
