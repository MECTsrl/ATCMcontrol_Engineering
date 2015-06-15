


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



#include "cgt_misc.h"
#include "utilif.h"



extern "C" const TCHAR* CGT_PouTypeName(CG_POUTYP pt, bool bEndMaker)
{
    switch(pt)
    {
    case CG_POUTYP_UNDEF:       return _T("POU"); //generic ?
    case CG_POU_PRG:            return bEndMaker ? _T("END_PROGRAM") :_T("PROGRAM");
    case CG_POU_FUN:            return bEndMaker ? _T("END_FUNCTION") :_T("FUNCTION"); 
    case CG_POU_FB:             return bEndMaker ? _T("END_FUNCTION_BLOCK") :_T("FUNCTION_BLOCK");
    case CG_POU_SDT:            return bEndMaker ? _T("END_STRUCT") : _T("STRUCT");
    case CG_POU_GVL:            return _T("GVL");
    case CG_POU_CST:            return bEndMaker ? _T("END_CONST") : _T("CONST");
    default:
        assert(!"bad pou typ class");
        return _T("<undefined pou typ>");
    }
}


extern "C" const TCHAR* CGT_StoreClassName(CG_DECLSCOPE s)
{
    switch(s)
    {
    case CG_VAR:            return _T("VAR");
    case CG_VAR_INPUT:      return _T("VAR_INPUT"); 
    case CG_VAR_OUTPUT:     return _T("VAR_OUTPUT");
    case CG_VAR_IN_OUT:     return _T("VAR_IN_OUT");
    case CG_VAR_GLOBAL:     return _T("VAR_GLOBAL");
    case CG_VAR_EXTERNAL:   return _T("VAR_EXTERNAL");
    case CG_CONST:          return _T("CONST");
    case CG_VAR_TEMP:       return _T("VAR_TEMP");
    case CG_VAR_IN_EXT:     return _T("VAR_IN_EXT");
    case CG_VAR_NATIVE:     return _T("VAR_NATIVE");
    case CG_DECLSCOPE_UNDEF:return _T("");
    default:
        assert(!"bad store class");
        return _T("<undefined storage class>");
    }
}

//##AC beg new enums
extern "C" CG_BITSET CGT_DTypesToTBS(CG_DTYPES dt)
{
    switch(dt)
    {
    case CG_DTYPES_UNDEF:   return CG_TBS_EMPTY;
    case CG_VOID:           return CG_TBS_VOID;
    case CG_BOOL_R_EDGE:    
    case CG_BOOL_F_EDGE:    
    case CG_BOOL:           return CG_TBS_BOOL;
    case CG_BYTE:           return CG_TBS_BYTE;
    case CG_WORD:           return CG_TBS_WORD;
    case CG_DWORD:          return CG_TBS_DWORD;
    case CG_LWORD:          return CG_TBS_LWORD;
    case CG_REAL:           return CG_TBS_REAL;
    case CG_LREAL:          return CG_TBS_LREAL;
    case CG_SINT:           return CG_TBS_SINT;
    case CG_INT:            return CG_TBS_INT;
    case CG_DINT:           return CG_TBS_DINT;
    case CG_LINT:           return CG_TBS_LINT;
    case CG_USINT:          return CG_TBS_USINT;
    case CG_UINT:           return CG_TBS_UINT;
    case CG_UDINT:          return CG_TBS_UDINT;
    case CG_ULINT:          return CG_TBS_ULINT;
    case CG_DT:             return CG_TBS_DT;
    case CG_TOD:            return CG_TBS_TOD;
    case CG_DATE:           return CG_TBS_DATE;
    case CG_TIME:           return CG_TBS_TIME;
    case CG_STRING:         return CG_TBS_STRING;
    case CG_WSTRING:        return CG_TBS_WSTRING;
        //user defined:
    case CG_INSTANCE:       return CG_TBS_INSTANCE;
    case CG_STRUCT:         return CG_TBS_STRUCT;
    case CG_PROGRAM:        return CG_TBS_PROGRAM;
        //QVT for all simple
    case CG_QVT_BOOL:       return CG_TBS_QVT_BOOL;
    case CG_QVT_BYTE:       return CG_TBS_QVT_BYTE;
    case CG_QVT_WORD:       return CG_TBS_QVT_WORD;
    case CG_QVT_DWORD:      return CG_TBS_QVT_DWORD;
    case CG_QVT_LWORD:      return CG_TBS_QVT_LWORD;
    case CG_QVT_REAL:       return CG_TBS_QVT_REAL;
    case CG_QVT_LREAL:      return CG_TBS_QVT_LREAL;
    case CG_QVT_SINT:       return CG_TBS_QVT_SINT;
    case CG_QVT_INT:        return CG_TBS_QVT_INT;
    case CG_QVT_DINT:       return CG_TBS_QVT_DINT;
    case CG_QVT_LINT:       return CG_TBS_QVT_LINT;
    case CG_QVT_USINT:      return CG_TBS_QVT_USINT;
    case CG_QVT_UINT:       return CG_TBS_QVT_UINT;
    case CG_QVT_UDINT:      return CG_TBS_QVT_UDINT;
    case CG_QVT_ULINT:      return CG_TBS_QVT_ULINT; 
    case CG_QVT_DT:         return CG_TBS_QVT_DT;
    case CG_QVT_TOD:        return CG_TBS_QVT_TOD;
    case CG_QVT_DATE:       return CG_TBS_QVT_DATE;
    case CG_QVT_TIME:       return CG_TBS_QVT_TIME;
    case CG_QVT_STRING:     return CG_TBS_QVT_STRING;
    case CG_QVT_WSTRING:    return CG_TBS_QVT_WSTRING;
        //polymorphic types:
    case CG_ANY:            return CG_TBS_ANY;
    case CG_ANY_NUM:        return CG_TBS_ANY_NUM;
    case CG_ANY_REAL:       return CG_TBS_ANY_REAL;
    case CG_ANY_INT:        return CG_TBS_ANY_INT;
    case CG_ANY_BIT:        return CG_TBS_ANY_BIT;
    case CG_ANY_STRING:     return CG_TBS_ANY_STRING;
    case CG_ANY_DATE:       return CG_TBS_ANY_DATE;
    case CG_ANY_ELEMENTARY: return CG_TBS_ANY_ELEMENTARY;
    case CG_ANY_MAGNITUDE:  return CG_TBS_ANY_MAGNITUDE;
    case CG_ANY_DERIVED:    return CG_TBS_ANY_DERIVED;
    case CG_ANY_WAITABLE:   return CG_TBS_ALLBITS;
    case CG_GOTOLABEL:      return CG_TBS_GOTOLABEL;
    //case CG_POLYMORPHMIX:
    default:        
        assert(!"bad enum"); 
    }
    
    return CG_TBS_EMPTY;
}


extern "C" CG_DTYPES CGT_TBSToDTypes(CG_BITSET type)
{
    switch(type)
    {
    case CG_TBS_EMPTY:          return CG_DTYPES_UNDEF;
    case CG_TBS_VOID:           return CG_VOID;
    case CG_TBS_BOOL:           return CG_BOOL;
    case CG_TBS_BYTE:           return CG_BYTE;
    case CG_TBS_WORD:           return CG_WORD;
    case CG_TBS_DWORD:          return CG_DWORD;
    case CG_TBS_LWORD:          return CG_LWORD;
    case CG_TBS_REAL:           return CG_REAL;
    case CG_TBS_LREAL:          return CG_LREAL;
    case CG_TBS_SINT:           return CG_SINT;
    case CG_TBS_INT:            return CG_INT;
    case CG_TBS_DINT:           return CG_DINT;
    case CG_TBS_LINT:           return CG_LINT;
    case CG_TBS_USINT:          return CG_USINT;
    case CG_TBS_UINT:           return CG_UINT;
    case CG_TBS_UDINT:          return CG_UDINT;
    case CG_TBS_ULINT:          return CG_ULINT;
        
    case CG_TBS_DT:             return CG_DT;
    case CG_TBS_TOD:            return CG_TOD;
    case CG_TBS_DATE:           return CG_DATE;
    case CG_TBS_TIME:           return CG_TIME;

    case CG_TBS_STRING:         return CG_STRING;
    case CG_TBS_WSTRING:        return CG_WSTRING;
        
    case CG_TBS_QVT_BOOL:       return CG_QVT_BOOL; 
    case CG_TBS_QVT_BYTE:       return CG_QVT_BYTE; 
    case CG_TBS_QVT_WORD:       return CG_QVT_WORD; 
    case CG_TBS_QVT_DWORD:      return CG_QVT_DWORD; 
    case CG_TBS_QVT_LWORD:      return CG_QVT_LWORD;
    case CG_TBS_QVT_REAL:       return CG_QVT_REAL;
    case CG_TBS_QVT_LREAL:      return CG_QVT_LREAL; 
    case CG_TBS_QVT_SINT:       return CG_QVT_SINT;
    case CG_TBS_QVT_INT:        return CG_QVT_INT;
    case CG_TBS_QVT_DINT:       return CG_QVT_DINT;
    case CG_TBS_QVT_LINT:       return CG_QVT_LINT;
    case CG_TBS_QVT_USINT:      return CG_QVT_USINT;
    case CG_TBS_QVT_UINT:       return CG_QVT_UINT;
    case CG_TBS_QVT_UDINT:      return CG_QVT_UDINT;
    case CG_TBS_QVT_ULINT:      return CG_QVT_ULINT;

    case CG_TBS_QVT_DT:         return CG_QVT_DT;
    case CG_TBS_QVT_TOD:        return CG_QVT_TOD;
    case CG_TBS_QVT_DATE:       return CG_QVT_DATE;
    case CG_TBS_QVT_TIME:       return CG_QVT_TIME;
    
    case CG_TBS_QVT_STRING:     return CG_QVT_STRING;
    case CG_TBS_QVT_WSTRING:    return CG_QVT_WSTRING;

    case CG_TBS_INSTANCE:       return CG_INSTANCE;
    case CG_TBS_STRUCT:         return CG_STRUCT;
    case CG_TBS_PROGRAM:        return CG_PROGRAM;
    case CG_TBS_ARRAY:          assert(!"CG_TBS_ARRAY"); return CG_DTYPES_UNDEF;
    //case CG_TBS_ENUM:           return CG_ENUM;
    case CG_TBS_GOTOLABEL:      return CG_GOTOLABEL;
    
    //well known poly types:
    case CG_TBS_ANY:            return CG_ANY;
    case CG_TBS_ANY_NUM:        return CG_ANY_NUM;
    case CG_TBS_ANY_REAL:       return CG_ANY_REAL;
    case CG_TBS_ANY_INT:        return CG_ANY_INT;
    case CG_TBS_ANY_BIT:        return CG_ANY_BIT;
    case CG_TBS_ANY_STRING:     return CG_ANY_STRING;
    case CG_TBS_ANY_DATE:       return CG_ANY_DATE;
    case CG_TBS_ANY_ELEMENTARY: return CG_ANY_ELEMENTARY;
    case CG_TBS_ANY_MAGNITUDE:  return CG_ANY_MAGNITUDE;
    case CG_TBS_ANY_DERIVED:    return CG_ANY_DERIVED;
    //poly mix:
    default:                    return CG_POLYMORPHMIX;
    }
}

extern "C" CG_DTYPES CGT_TypeKeywordToCG_DTYPES(const TCHAR* pszTypeKW)
{
    UTIL_KW_ENUM kw = UTIL_IsIECKeyword(pszTypeKW);

    switch(kw)
    {
    case UTIL_KW_SINT:              return CG_SINT;
    case UTIL_KW_INT:               return CG_INT;
    case UTIL_KW_DINT:              return CG_DINT;
    case UTIL_KW_LINT:              return CG_LINT;
    case UTIL_KW_USINT:             return CG_USINT;
    case UTIL_KW_UINT:              return CG_UINT;
    case UTIL_KW_UDINT:             return CG_UDINT;
    case UTIL_KW_ULINT:             return CG_ULINT;
    case UTIL_KW_REAL:              return CG_REAL;
    case UTIL_KW_LREAL:             return CG_LREAL;
    case UTIL_KW_DATE:              return CG_DATE;
    case UTIL_KW_TIME_OF_DAY:       return CG_TOD;
    case UTIL_KW_TOD:               return CG_TOD;
    case UTIL_KW_DATE_AND_TIME:     return CG_DT;
    case UTIL_KW_DT:                return CG_DT;
    case UTIL_KW_STRING:            return CG_STRING;
    case UTIL_KW_WSTRING:           return CG_WSTRING;
    case UTIL_KW_TIME:              return CG_TIME;
    case UTIL_KW_BOOL:              return CG_BOOL;
    case UTIL_KW_BYTE:              return CG_BYTE;
    case UTIL_KW_WORD:              return CG_WORD;
    case UTIL_KW_DWORD:             return CG_DWORD;
    case UTIL_KW_LWORD:             return CG_LWORD;
    case UTIL_KW_ANY:               return CG_ANY;
    case UTIL_KW_ANY_NUM:           return CG_ANY_NUM;
    case UTIL_KW_ANY_REAL:          return CG_ANY_REAL;
    case UTIL_KW_ANY_INT:           return CG_ANY_INT;
    case UTIL_KW_ANY_BIT:           return CG_ANY_BIT;
    case UTIL_KW_ANY_STRING:        return CG_ANY_STRING;
    case UTIL_KW_ANY_DATE:          return CG_ANY_DATE;
    case UTIL_KW_ANY_ELEMENTARY:    return CG_ANY_ELEMENTARY;
    case UTIL_KW_ANY_MAGNITUDE:     return CG_ANY_MAGNITUDE;
    case UTIL_KW_ANY_DERIVED:       return CG_ANY_DERIVED;


    case UTIL_KW_QVT_BOOL:          return CG_QVT_BOOL;
    case UTIL_KW_QVT_BYTE:          return CG_QVT_BYTE;
    case UTIL_KW_QVT_WORD:          return CG_QVT_WORD;
    case UTIL_KW_QVT_DWORD:         return CG_QVT_DWORD;
    case UTIL_KW_QVT_DINT:          return CG_QVT_DINT;
    case UTIL_KW_QVT_LREAL:         return CG_QVT_LREAL;
    case UTIL_KW_QVT_LINT:          return CG_QVT_LINT;
    case UTIL_KW_QVT_LWORD:         return CG_QVT_LWORD;
    case UTIL_KW_QVT_REAL:          return CG_QVT_REAL;
    case UTIL_KW_QVT_STRING:        return CG_QVT_STRING;
    case UTIL_KW_QVT_WSTRING:       return CG_QVT_WSTRING;
    //TODO make kewords complte even if not supported.

    default:                        return CG_DTYPES_UNDEF;
    }
}

extern "C" const TCHAR* CGT_DTYPES_ToString(CG_DTYPES dt)
{
    switch(dt)
    {
    case CG_DTYPES_UNDEF:return _T("<undefined>");
     //simple:
    case CG_VOID:        return _T("VOID");
    case CG_BOOL_R_EDGE: return _T("BOOL R_EDGE");
    case CG_BOOL_F_EDGE: return _T("BOOL F_EDGE");
    case CG_BOOL:        return _T("BOOL");
    case CG_BYTE:        return _T("BYTE");
    case CG_WORD:        return _T("WORD");
    case CG_DWORD:       return _T("DWORD");
    case CG_LWORD:       return _T("LWORD");
    case CG_REAL:        return _T("REAL");
    case CG_LREAL:       return _T("LREAL");
    case CG_SINT:        return _T("SINT");
    case CG_INT:         return _T("INT");
    case CG_DINT:        return _T("DINT");
    case CG_LINT:        return _T("LINT");
    case CG_USINT:       return _T("USINT");
    case CG_UINT:        return _T("UINT");  
    case CG_UDINT:       return _T("UDINT");
    case CG_ULINT:       return _T("ULINT");
    case CG_DT:          return _T("DT");
    case CG_TOD:         return _T("TOD");
    case CG_DATE:        return _T("DATE");
    case CG_TIME:        return _T("TIME");
    case CG_STRING:      return _T("STRING");
    case CG_WSTRING:     return _T("WSTRING");
     //user defined:
    case CG_INSTANCE:    return _T("INSTANCE");
    case CG_STRUCT:      return _T("STRUCTURE");
    case CG_PROGRAM:     return _T("PROGRAM");
    //QVT for all simple (ATCM extension):
    case CG_QVT_BOOL:    return _T("QVT_BOOL");
    case CG_QVT_BYTE:    return _T("QVT_BYTE");
    case CG_QVT_WORD:    return _T("QVT_WORD");
    case CG_QVT_DWORD:   return _T("QVT_DWORD");
    case CG_QVT_LWORD:   return _T("QVT_LWORD");
    case CG_QVT_REAL:    return _T("QVT_REAL");
    case CG_QVT_LREAL:   return _T("QVT_LREAL");
    case CG_QVT_SINT:    return _T("QVT_SINT");
    case CG_QVT_INT:     return _T("QVT_INT");
    case CG_QVT_DINT:    return _T("QVT_DINT");
    case CG_QVT_LINT:    return _T("QVT_LINT");
    case CG_QVT_USINT:   return _T("QVT_USINT");
    case CG_QVT_UINT:    return _T("QVT_UINT");
    case CG_QVT_UDINT:   return _T("QVT_UDINT");
    case CG_QVT_ULINT:   return _T("QVT_ULINT");
    case CG_QVT_DT:      return _T("QVT_DT");
    case CG_QVT_TOD:     return _T("QVT_TOD");
    case CG_QVT_DATE:    return _T("QVT_DATE"); 
    case CG_QVT_TIME:    return _T("QVT_TIME");
    case CG_QVT_STRING:  return _T("QVT_STRING");
    case CG_QVT_WSTRING: return _T("QVT_WSTRING");
    //polymorphic types:
    case CG_ANY:            return _T("ANY");
    case CG_ANY_NUM:        return _T("ANY_NUM");
    case CG_ANY_REAL:       return _T("ANY_REAL");
    case CG_ANY_INT:        return _T("ANY_INT");
    case CG_ANY_BIT:        return _T("ANY_BIT");
    case CG_ANY_STRING:     return _T("ANY_STRING");
    case CG_ANY_DATE:       return _T("ANY_DATE");
    case CG_ANY_ELEMENTARY: return _T("ANY_ELEMENTARY");
    case CG_ANY_MAGNITUDE:  return _T("ANY_MAGNITUDE");
    case CG_ANY_DERIVED:    return _T("ANY_DERIVED");
    case CG_ANY_WAITABLE:   return _T("ANY_WAITABLE");
    case CG_GOTOLABEL:      return _T("GOTOLABEL");
    case CG_POLYMORPHMIX:   return _T("<POLYMORPHMIX>");
    default:        
        assert(!"ST_CG_DTYPES_ToString(): invalid basic type enum"); 
        return _T("??");
    }
}

extern "C" const TCHAR* CGT_DTYPES_ToLString2(CG_DTYPES dt)
{
    if(dt==CG_BOOL || dt==CG_BOOL_R_EDGE || dt==CG_BOOL_F_EDGE)
        return _T("bool");
    else
        return CGT_DTYPES_ToLString(dt);
}

extern "C" const TCHAR* CGT_DTYPES_ToLString(CG_DTYPES dt)
{
    switch(dt)
    {
    case CG_DTYPES_UNDEF:return _T("<undefined>");
     //simple:
    case CG_VOID:        return _T("void");
    case CG_BOOL_R_EDGE: return _T("bool r_edge");
    case CG_BOOL_F_EDGE: return _T("bool f_edge");
    case CG_BOOL:        return _T("bool");
    case CG_BYTE:        return _T("byte");
    case CG_WORD:        return _T("word");
    case CG_DWORD:       return _T("dword");
    case CG_LWORD:       return _T("lword");
    case CG_REAL:        return _T("real");
    case CG_LREAL:       return _T("lreal");
    case CG_SINT:        return _T("sint");
    case CG_INT:         return _T("int");
    case CG_DINT:        return _T("dint");
    case CG_LINT:        return _T("lint");
    case CG_USINT:       return _T("usint");
    case CG_UINT:        return _T("uint");  
    case CG_UDINT:       return _T("udint");
    case CG_ULINT:       return _T("ulint");
    case CG_DT:          return _T("dt");
    case CG_TOD:         return _T("tod");
    case CG_DATE:        return _T("date");
    case CG_TIME:        return _T("time");
    case CG_STRING:      return _T("string");
    case CG_WSTRING:     return _T("wstring");
     //user defined:
    case CG_INSTANCE:    return _T("instance");
    case CG_STRUCT:      return _T("structure");
    case CG_PROGRAM:     return _T("program");
    //QVT for all simple (ATCM extension):
    case CG_QVT_BOOL:    return _T("qvt_bool");
    case CG_QVT_BYTE:    return _T("qvt_byte");
    case CG_QVT_WORD:    return _T("qvt_word");
    case CG_QVT_DWORD:   return _T("qvt_dword");
    case CG_QVT_LWORD:   return _T("qvt_lword");
    case CG_QVT_REAL:    return _T("qvt_real");
    case CG_QVT_LREAL:   return _T("qvt_lreal");
    case CG_QVT_SINT:    return _T("qvt_sint");
    case CG_QVT_INT:     return _T("qvt_int");
    case CG_QVT_DINT:    return _T("qvt_dint");
    case CG_QVT_LINT:    return _T("qvt_lint");
    case CG_QVT_USINT:   return _T("qvt_usint");
    case CG_QVT_UINT:    return _T("qvt_uint");
    case CG_QVT_UDINT:   return _T("qvt_udint");
    case CG_QVT_ULINT:   return _T("qvt_ulint");
    case CG_QVT_DT:      return _T("qvt_dt");
    case CG_QVT_TOD:     return _T("qvt_tod");
    case CG_QVT_DATE:    return _T("qvt_date"); 
    case CG_QVT_TIME:    return _T("qvt_time");
    case CG_QVT_STRING:  return _T("qvt_string");
    case CG_QVT_WSTRING: return _T("qvt_wstring");
    //polymorphic types:
    case CG_ANY:            return _T("any");
    case CG_ANY_NUM:        return _T("any_num");
    case CG_ANY_REAL:       return _T("any_real");
    case CG_ANY_INT:        return _T("any_int");
    case CG_ANY_BIT:        return _T("any_bit");
    case CG_ANY_STRING:     return _T("any_string");
    case CG_ANY_DATE:       return _T("any_date");
    case CG_ANY_ELEMENTARY: return _T("any_elementary");
    case CG_ANY_MAGNITUDE:  return _T("any_magnitude");
    case CG_ANY_DERIVED:    return _T("any_derived");
    case CG_ANY_WAITABLE:   return _T("any_waitable");
    case CG_GOTOLABEL:      return _T("gotolabel");
    case CG_POLYMORPHMIX:   return _T("<polymorphmix>");
    default:        
        assert(!"ST_CG_DTYPES_ToString(): invalid basic type enum"); 
        return _T("??");
    }
}


extern "C" char CGT_DTYPES_ToMangeledChar(CG_DTYPES dt)
{
    switch(dt)
    {
    case CG_VOID:                 return '0';

    case CG_BOOL_R_EDGE:          return 'a';
    case CG_BOOL_F_EDGE:          return 'a';
    case CG_BOOL:                 return 'a';

    case CG_BYTE:                 return 'b';
    case CG_WORD:                 return 'c';
    case CG_DWORD:                return 'd';
    case CG_LWORD:                return 'e';

    case CG_REAL:                 return 'f';
    case CG_LREAL:                return 'g';

    case CG_SINT:                 return 'h';
    case CG_INT:                  return 'i';
    case CG_DINT:                 return 'j';
    case CG_LINT:                 return 'k';
    case CG_USINT:                return 'l';
    case CG_UINT:                 return 'm';
    case CG_UDINT:                return 'n';
    case CG_ULINT:                return 'o';

    case CG_DT:                   return 'p';
//  case CG_DT32:                 return 'q';
    case CG_TOD:                  return 'r';
    case CG_DATE:                 return 's';
    case CG_TIME:                 return 't';
//  case CG_TIME32:               return 'u';

    case CG_STRING:               return 'v';
    case CG_WSTRING:              return 'w';


    case CG_INSTANCE:             return '1';
    case CG_STRUCT:               return '2';
    case CG_PROGRAM:              return '3';
//  case CG_ENUM:                 return '4';

    case CG_QVT_BOOL:             return 'A';
    case CG_QVT_BYTE:             return 'B';
    case CG_QVT_WORD:             return 'C';
    case CG_QVT_DWORD:            return 'D';
    case CG_QVT_LWORD:            return 'E';
    case CG_QVT_REAL:             return 'F';
    case CG_QVT_LREAL:            return 'G';
    case CG_QVT_SINT:             return 'H';
    case CG_QVT_INT:              return 'I';
    case CG_QVT_DINT:             return 'J';
    case CG_QVT_LINT:             return 'K';
    case CG_QVT_USINT:            return 'L';
    case CG_QVT_UINT:             return 'M';
    case CG_QVT_UDINT:            return 'N';
    case CG_QVT_ULINT:            return 'O';
    case CG_QVT_DT:               return 'P';
//  case CG_QVT_DT32:             return 'Q';
    case CG_QVT_TOD:              return 'R';
    case CG_QVT_DATE:             return 'S';
    case CG_QVT_TIME:             return 'T';
//  case CG_QVT_TIME32:           return 'U';
    case CG_QVT_STRING:           return 'V';
    case CG_QVT_WSTRING:          return 'W';
    default:
        assert(!"bad type enum");
        return 0;
    }
}


extern "C" CG_DTYPES CGT_MangeledChar_ToDTYPES(char c)
{
    switch(c)
    {
    case '0':    return CG_VOID;

    case 'a':    return CG_BOOL;

    case 'b':    return CG_BYTE;
    case 'c':    return CG_WORD;
    case 'd':    return CG_DWORD;
    case 'e':    return CG_LWORD;

    case 'f':    return CG_REAL;
    case 'g':    return CG_LREAL;

    case 'h':    return CG_SINT;
    case 'i':    return CG_INT;
    case 'j':    return CG_DINT;
    case 'k':    return CG_LINT;
    case 'l':    return CG_USINT;
    case 'm':    return CG_UINT;
    case 'n':    return CG_UDINT;
    case 'o':    return CG_ULINT;

    case 'p':    return CG_DT;
//  case 'q':    return CG_DT32;
    case 'r':    return CG_TOD;
    case 's':    return CG_DATE;
    case 't':    return CG_TIME;
//  case 'u':    return CG_TIME32;

    case 'v':    return CG_STRING;
    case 'w':    return CG_WSTRING;

    case '1':    return CG_INSTANCE;
    case '2':    return CG_STRUCT;
    case '3':    return CG_PROGRAM;
//  case '4':    return CG_ENUM;

    case 'A':    return CG_QVT_BOOL;
    case 'B':    return CG_QVT_BYTE;
    case 'C':    return CG_QVT_WORD;
    case 'D':    return CG_QVT_DWORD;
    case 'E':    return CG_QVT_LWORD;
    case 'F':    return CG_QVT_REAL;
    case 'G':    return CG_QVT_LREAL;
    case 'H':    return CG_QVT_SINT;
    case 'I':    return CG_QVT_INT;
    case 'J':    return CG_QVT_DINT;
    case 'K':    return CG_QVT_LINT;
    case 'L':    return CG_QVT_USINT;
    case 'M':    return CG_QVT_UINT;
    case 'N':    return CG_QVT_UDINT;
    case 'O':    return CG_QVT_ULINT;
    case 'P':    return CG_QVT_DT;
//  case 'Q':    return CG_QVT_DT32;
    case 'R':    return CG_QVT_TOD;
    case 'S':    return CG_QVT_DATE;
    case 'T':    return CG_QVT_TIME;
//  case 'U':    return CG_QVT_TIME32;
    case 'V':    return CG_QVT_STRING;
    case 'W':    return CG_QVT_WSTRING;
    default:
        assert(!"bad type char");
        return CG_DTYPES_UNDEF;
    }
}

extern "C" const TCHAR* CGT_DTypeToMangeld(const CG_DType* pType, TCHAR buffer[CGT_MAXMANGELED_TYPE])
{
    buffer[0] = CGT_DTYPES_ToMangeledChar(pType->type);
    
    if(pType->pSym)
    {
        _tcscpy(buffer+1, pType->pSym->pszId);
        _tcslwr(buffer+1);
    }
    else if(pType->lStrLen >= 0)
    {
        _ltot(pType->lStrLen, buffer+1, 10);
    }
    else if(pType->type == CG_BOOL_R_EDGE)
    {
        buffer[1] = 'r';
        buffer[2] = 0;
    }
    else if(pType->type == CG_BOOL_F_EDGE)
    {
        buffer[1] = 'f';
        buffer[2] = 0;
    }
    else
    {
        buffer[1] = 0;
    }


    if(pType->bIsArray)
    {
        int i;
        int iLen;

        for(i=0, iLen=_tcslen(buffer); i<pType->nDim; i++)
        {
            buffer[iLen++] = '[';
            if(pType->range[i].lRangeStart != 0)
                iLen += _tcslen(_ltot(pType->range[i].lRangeStart, buffer+iLen, 10));

            buffer[iLen++] = ',';
            iLen += _tcslen(_ultot(pType->range[i].ulRangeLength+1, buffer+iLen, 10));
        }    
    }

    return buffer;
}


extern "C" const TCHAR* CGT_ExprTypToLString(CG_EXPRTYP etyp)
{
    switch(etyp)
    {
    case CG_EXPRTYP_UNDEF:  return _T("<undefined>");
    //unary ops:
    case CG_NOT:        return _T("not");
    case CG_UMINUS:     return _T("-");
    case CG_NOOP:       return _T("");
    //binary ops:
    case CG_OR:         return _T("or"); 
    case CG_XOR:        return _T("xor"); 
    case CG_AND:        return _T("and"); 
    case CG_EQ:         return _T("eq"); 
    case CG_NE:         return _T("ne"); 
    case CG_LT:         return _T("lt");
    case CG_LE:         return _T("le");
    case CG_GT:         return _T("gt");
    case CG_GE:         return _T("ge");
    case CG_ADD:        return _T("add");
    case CG_SUB:        return _T("sub");
    case CG_MUL:        return _T("mul");
    case CG_DIV:        return _T("div");
    case CG_MOD:        return _T("mod");
    case CG_POW:        return _T("pow");
    case CG_MOVE:       return _T(":=");
    //Primary:
    case CG_LITERAL:    return _T("<literal>");  
    case CG_VARREF:     return _T("<variable>"); 
    case CG_STRUCTREF:  return _T("<struct ref>");
    case CG_ARRAYREF:   return _T("<array ref>");
     //function call:
    case CG_FUNCALL:    return _T("<function>");
    case CG_FUNMACRO:   return _T("<function>");
    case CG_BUILDIN:    return _T("<buildin>");
    //tmp vars:
    case CG_TMPVAR:     return _T("<tmp>");
    case CG_TRIGVAR:    return _T("<trig tmp>");
    //special:
    case CG_STEPACTION: return _T("<step/action ref>");
    case CG_USEREXPR:   return _T("<user expr>");
    case CG_FBCALLEXPR: return _T("<FB call expr>");
    case CG_LABELREF:   return _T("<label ref>");
    default:
        assert(!"bad expr type enum");
        return _T("bad expr type enum");
    }
}

//##AC end new enums



