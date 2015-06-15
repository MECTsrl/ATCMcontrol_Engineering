/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtype.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmtype.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtype.h $
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

#ifndef _ASMTYPE_H_
#define _ASMTYPE_H_

//----  Aggregate Includes:   --------------------------------------*
#include "limits.h"
#include "asmtools.h"
//----  Forward Class Definitions:   -------------------------------*
class ASMPROC_TYP;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
struct ASM_DATA_TYP;
typedef long          ASM_BYTEOFF;     //relative data pointer in bytes
typedef long          ASM_BYTESIZE;    //size of any object in bytes
typedef __int64       ASM_BITOFF;      //relative data pointer in bits
typedef long          ASM_BITOFF32;    //relative data pointer in bits
typedef long          ASM_BITSIZE;     //size of any object in bits
#define ASM_INVALID_BITOFF 0xffffffffffffffff

//safe type conversion, should submitt an error message if out of range (TODO)
inline unsigned long ASM_BITOFF_TO_UL(ASM_BITOFF i64)
{
    assert(i64>=0 && i64<=ULONG_MAX);
    return (unsigned long)i64;
}
inline unsigned long ASM_BITOFF_TO_L(ASM_BITOFF i64)
{
    assert(i64>=0 && i64<=LONG_MAX);
    return (long)i64;
}
inline ASM_BYTESIZE ASM_BITOFF_TO_BYTESIZE(ASM_BITOFF i64)
{
    assert(i64>=0);
    i64 = ((i64+7)/8);
    assert(i64<=LONG_MAX);
    return (long)i64;
}

//ASM_AlignBitOffset: helper function for alignment calulation:
//bitOff gets incremented to the next possible aligned position with 
//an alignment of ula bits:
inline ASM_BITOFF ASM_AlignBitOffset(ASM_BITOFF bitOff, unsigned long ula)
{
    return ((bitOff+ula-1)/ula)*ula;
}
inline ASM_BITOFF32 ASM_AlignBitOff32(ASM_BITOFF32 bitOff, unsigned long ula)
{
    return ((bitOff+ula-1)/ula)*ula;
}
//returns the bit offset of the first array element, the array layout is: 
//VM_ARRAY_LIMIT_SIZE|<alignment gap>|<first element>
//--------returnd bit offset----------^
//main purpose of this function is to calculate the <alignment gap>
//Out param pElemBitSize:
// gets filled with the array elem bit size, which is 
// for strings 2*8+len and if array of struct VM_OBJ_INDEX_SIZE, 
// as computed by ASM_BitSizeOfArrayElem.
ASM_BITOFF32 ASM_GetFirstArrayElemBitOff(const ASM_DATA_TYP* pArrayType, ASM_BITSIZE* pElemBitSize);


struct ASM_BITREGION
{
    ASM_BITOFF    bitOff;
    ASM_BITSIZE   bitSize;
};
struct ASM_BYTEREGION
{
    ASM_BYTEOFF  byteOff;
    ASM_BYTESIZE byteSize;
};



enum ASM_PROC_ENUM
{
    ASM_PROC_TYP_UNDEF,
    ASM_FUN,
    ASM_FB,
    ASM_PRG,
    ASM_STRUCT
};
enum ASM_DECL_SCOPE
{
    ASM_DECL_VAR,
    ASM_DECL_IN,
    ASM_DECL_OUT,
    ASM_DECL_INOUT,
    ASM_DECL_INEXT
};
const TCHAR* ASM_DeclScopeToString(ASM_DECL_SCOPE sc);


enum ASM_TYPE_ENUM 
{
    ASM_TYPE_UNDEF=0,
    ASM_VOID,
    ASM_BOOL,
    ASM_BYTE,
    ASM_WORD,
    ASM_DWORD,
    ASM_INT,
    ASM_UINT,
    ASM_DINT,
    ASM_UDINT,
    ASM_LINT,
    ASM_REAL,
    ASM_LREAL,
    ASM_TIME,
    ASM_STRING,
    ASM_LAST_BASIC_TYPE = ASM_STRING,
    ASM_FBI,   //fb instance
    ASM_STRI,  //structure instance
    ASM_PRGI,  //program instance
    ASM_LAST_TYPE_ENUM = ASM_PRGI,
    ASM_FUNNAME,  //program instance
    ASM_OFFSET_LITERAL,
    ASM_OBJID_LITERAL,
};
const TCHAR* ASM_TypeEnumToString(ASM_TYPE_ENUM type);






//ASM_GetBasicTypeBitSize:
//returns 1,8,16,32, or 64. asserts if type is string or other complex.
unsigned long ASM_BasicTypeBitSize(ASM_TYPE_ENUM type);

//returns 1,8,16,32, or 64 or (strlen+2)*8, asserts if type is other complex.
unsigned long ASM_SimpleTypeBitSize(const ASM_DATA_TYP* pType);


//returns 1,8,16,32, or 64 or (strlen+2)*8, asserts if type is not an array.
//array of structs: size of such an array is VM_INDEX_SIZE
unsigned long ASM_BitSizeOfArrayElem(const ASM_DATA_TYP* pType);

//ASM_GetVMByteSize: the number of bytes
//occupied in the VM. This is not alwys the same as the 
//'IEC-usable' space, for example arrays have the arrays 
//size as prefix in the VM memory.
//This function has no idea of alignment, alignment bytes 
//are not counted as storage bytes.
//This function calles ASM_INTERNAL_ERROR_STOP if any
//fb type is not evaluated yet or if called for a simple BOOL
//a BOOL does not have a byte size !
ASM_BYTESIZE ASM_GetVMStoreByteSize(const ASM_DATA_TYP* pType);



struct ASM_DATA_TYP
{
    ASM_TYPE_ENUM      type;
    ST_IDENT_TYP       name;   //dint, myfb, dint[], dint[12], 
                               //string(3), string, string[3](4), string[3]
    const ASMPROC_TYP* pFBIDef;//struct,prg or fb or NULL
    long               lStrlen;//if (w)string, if -1 auto/default string: LONG_MIN if no string
    unsigned           nElems; //0 if assumed size, 1 if simple type ?? ARRAY[0..0]
    bool               bArray; // 'true' if ARRAY, else 'false'
};
inline bool ASM_AreTypesAssignable(const ASM_DATA_TYP* pt1, const ASM_DATA_TYP* pt2)
{
    return ASM_IDENT_EQ(pt1->name, pt2->name);
}
inline bool ASM_AreTypesAssignable(ASM_TYPE_ENUM t1, ASM_TYPE_ENUM t2)
{
    if (t1 == ASM_OFFSET_LITERAL || t1 == ASM_OBJID_LITERAL)
        return t2 == ASM_WORD || t2 == ASM_DWORD || t2 == ASM_INT || t2 == ASM_DINT|| t2 == ASM_UINT || t2 == ASM_UDINT;

    return t1==t2;
}




enum ASM_SEGM_TYP
{
    ASM_SEGM_UNDEF   =-1,
    ASM_SEGM_INPUT   = 0, //in VM  SEG_INPUT
    ASM_SEGM_OUTPUT  = 1, //in VM  SEG_OUTPUT
    ASM_SEGM_GLOBAL  = 2, //in VM  SEG_GLOBAL
    ASM_SEGM_RETAIN  = 3, //in VM  SEG_RETAIN
    ASM_SEGM_INST    = 4, //in VM  SEG_LOCAL
    ASM_SEGM_CODE    = 5, //in VM  SEG_CODE
    ASM_SEGM_CONST,       //must not be passed to VM, only internal
};

#endif

/* ---------------------------------------------------------------------------- */
