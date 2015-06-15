/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtype.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmtype.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtype.cpp $
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

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtype.cpp 1     28.02.07 19:04 Ln $";
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "VmConstants.h"
#include "asmtype.h"
#include "asmstmt.h"
#include "globalContext.h"


ASM_BYTESIZE ASM_GetVMStoreByteSize(const ASM_DATA_TYP* pType)
{
    if(pType->bArray)
    {
        if(pType->nElems==0)
            ASM_INTERNAL_ERROR_STOP(NULL);

        ASM_BITSIZE  elemBitSize;
        ASM_BITOFF32 firstElemBitOff = ASM_GetFirstArrayElemBitOff(pType, &elemBitSize);
       
        return (firstElemBitOff + elemBitSize*pType->nElems + 7)/8;
    }
    else if(pType->pFBIDef)
    {
        if(pType->pFBIDef->getByteSize()<0)
            ASM_INTERNAL_ERROR_STOP(NULL);

        return pType->pFBIDef->getByteSize();
    }
    else if(pType->type == ASM_BOOL)
    {
        ASM_INTERNAL_ERROR_STOP(NULL);
    }
    else
    {
		// non-strings have pType->lStrlen == LONG_MIN
        if(pType->lStrlen < 0 && pType->lStrlen != LONG_MIN)
            ASM_INTERNAL_ERROR_STOP(NULL);

        return (ASM_SimpleTypeBitSize(pType)+7)/8;
    }
    return 888855555;
}

const TCHAR* ASM_DeclScopeToString(ASM_DECL_SCOPE sc)
{
    switch(sc)
    {
    case ASM_DECL_VAR:    return _T("var");
    case ASM_DECL_IN:     return _T("in");
    case ASM_DECL_OUT:    return _T("out");
    case ASM_DECL_INOUT:  return _T("inout");
    case ASM_DECL_INEXT:  return _T("inext");
    default:
        assert(!"bad enum");
        return _T("<internal error");
    }
}


ASM_BITOFF32 ASM_GetFirstArrayElemBitOff(const ASM_DATA_TYP* pArrayType, ASM_BITSIZE* pElemBitSize)//##efal
{
    ASM_BITSIZE alignRelevantElemBitSize;
    ASM_BITSIZE declAlign;

    *pElemBitSize = ASM_BitSizeOfArrayElem(pArrayType);

    //ASM_BitSizeOfArrayElem returns for string the full string size= 2+len
    //here we need the alignment relevant element size:
    alignRelevantElemBitSize = pArrayType->type==ASM_STRING ? 8 : *pElemBitSize;

    declAlign        = min(alignRelevantElemBitSize, GetGC()->getTrgAttr()->trg_instAlignBytes*8);
    
    return ASM_AlignBitOff32(VM_ARRAY_LIMIT_SIZE, declAlign);
}


const TCHAR* ASM_TypeEnumToString(ASM_TYPE_ENUM type)
{
    switch(type)
    {
    case ASM_TYPE_UNDEF:   return _T("<undefined type");
    case ASM_BOOL:         return _T("bool"); 
    case ASM_BYTE:         return _T("byte");
    case ASM_WORD:         return _T("word");
    case ASM_DWORD:        return _T("dword");
    case ASM_INT:          return _T("int");
    case ASM_UINT:         return _T("uint");
    case ASM_DINT:         return _T("dint");
    case ASM_UDINT:        return _T("udint");
    case ASM_LINT:         return _T("lint");
    case ASM_REAL:         return _T("real");
    case ASM_LREAL:        return _T("lreal");
    case ASM_TIME:         return _T("time");
    case ASM_STRING:       return _T("string");
    case ASM_VOID:         return _T("void");
    case ASM_FBI:          return _T(".fb");
    case ASM_STRI:         return _T(".struct");
    case ASM_PRGI:         return _T(".prg");
    case ASM_OFFSET_LITERAL:return _T("offsetof()");
    case ASM_OBJID_LITERAL:return _T("objid()");
    default:
        assert(!"bad enum");
        return _T("<internal error>");
    }
};




unsigned long ASM_BasicTypeBitSize(ASM_TYPE_ENUM type)
{
    switch(type)
    {
    case ASM_BOOL:    return 1;   break;
    case ASM_BYTE:    return 8;   break;
    case ASM_INT:     return 16;  break;
    case ASM_UINT:    return 16;  break;
    case ASM_WORD:    return 16;  break;
    case ASM_DWORD:   return 32;  break;
    case ASM_DINT:    return 32;  break;
    case ASM_UDINT:   return 32;  break;
    case ASM_LINT:    return 64;  break;
    case ASM_REAL:    return 32;  break;
    case ASM_LREAL:   return 64;  break;
    case ASM_TIME:    return 32;  break;
    default:
        ASM_INTERNAL_ERROR_STOP(NULL);
    }
    return 100010001;
}
unsigned long ASM_SimpleTypeBitSize(const ASM_DATA_TYP* pType)
{
    if(pType->bArray || pType->pFBIDef)
        ASM_INTERNAL_ERROR_STOP(NULL);

    switch(pType->type)
    {
    case ASM_BOOL:    return 1;
    case ASM_BYTE:    return 8;
    case ASM_INT:     return 16;
    case ASM_UINT:    return 16;
    case ASM_WORD:    return 16;
    case ASM_DWORD:   return 32;
    case ASM_DINT:    return 32;
    case ASM_UDINT:   return 32;
    case ASM_LINT:    return 64;
    case ASM_REAL:    return 32;
    case ASM_LREAL:   return 64;
    case ASM_TIME:    return 32;
    case ASM_VOID:    return 0;
    case ASM_STRING:
        if(pType->lStrlen<0)
            ASM_INTERNAL_ERROR_STOP(NULL);
        // +2: 1 byte for max and 1 byte for cur len. *8 : 8 bit/byte
        return (pType->lStrlen + 2) * 8;
    default:
        ASM_INTERNAL_ERROR_STOP(NULL);
        return 88887777;
    }
}
unsigned long ASM_BitSizeOfArrayElem(const ASM_DATA_TYP* pType)
{
    if(!pType->bArray)
        ASM_INTERNAL_ERROR_STOP(NULL);

    switch(pType->type)
    {
    case ASM_BOOL:    return 1;
    case ASM_BYTE:    return 8;
    case ASM_INT:     return 16;
    case ASM_UINT:    return 16;
    case ASM_WORD:    return 16;
    case ASM_DWORD:   return 32;
    case ASM_DINT:    return 32;
    case ASM_UDINT:   return 32;
    case ASM_LINT:    return 64;
    case ASM_REAL:    return 32;
    case ASM_LREAL:   return 64;
    case ASM_TIME:    return 32;
    case ASM_STRING:
        if(pType->lStrlen<0)
            ASM_INTERNAL_ERROR_STOP(NULL);
        // +2: 1 byte for max and 1 byte for cur len. *8 : 8 bit/byte
        return (pType->lStrlen + 2) * 8;
    case ASM_FBI:     return VM_OBJ_INDEX_SIZE;
    case ASM_STRI:    return VM_OBJ_INDEX_SIZE;
    case ASM_PRGI:    //error 1 FALL th.
    case ASM_VOID:    //error 1 FALL th. 
    default:
        ASM_INTERNAL_ERROR_STOP(NULL);
        return 88887777;
    }
}

bool ASM_IsObject(const ASM_DATA_TYP* pType)
{
    ASM_TYPE_ENUM type = pType->type;
    return pType->bArray || type==ASM_STRI || type==ASM_PRGI || type==ASM_FBI;
}


bool ASM_IsSimpleType(const ASM_DATA_TYP* pType)
{
    if (pType->bArray)
        return false;

    switch(pType->type)
    {
    case ASM_BOOL:
    case ASM_BYTE:
    case ASM_WORD:
    case ASM_DWORD:
    case ASM_INT:
    case ASM_UINT:
    case ASM_DINT:
    case ASM_UDINT:
    case ASM_LINT:
    case ASM_REAL:
    case ASM_LREAL:
    case ASM_TIME:
    case ASM_STRING:
        return true;
    default:
        return false;
    }

    return false;
}

bool ASM_IsSimpleBasicType(ASM_TYPE_ENUM type)
{
    switch(type)
    {
    case ASM_BOOL:
    case ASM_BYTE:
    case ASM_WORD:
    case ASM_DWORD:
    case ASM_INT:
    case ASM_UINT:
    case ASM_DINT:
    case ASM_UDINT:
    case ASM_LINT:
    case ASM_REAL:
    case ASM_LREAL:
    case ASM_TIME:
    case ASM_STRING:
        return true;
    default:
        return false;
    }

    return false;
}

/* ---------------------------------------------------------------------------- */
