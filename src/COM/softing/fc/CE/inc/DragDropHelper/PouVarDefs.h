#ifndef __POUVARDEFS_H_
#define __POUVARDEFS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000




// constants for mangled name parsing
#define PVS_MN_PART                _T('#')

#define PVS_MN_IEC_TYPE_FB         _T("FB")
#define PVS_MN_IEC_TYPE_FUN        _T("FUN")
#define PVS_MN_IEC_TYPE_STRUCT     _T("SDT")
#define PVS_MN_IEC_TYPE_PRG        _T("PRG")

#define PVS_MN_OUT_PARAM           _T('Q')
#define PVS_MN_IN_PARAM            _T('I')
#define PVS_MN_INOUT_PARAM         _T('X')
#define PVS_MN_EXT_PARAM           _T('E')
#define PVS_MN_RETAIN_PARAM        _T('R')
#define PVS_MN_NONRETAIN_PARAM     _T('N')
#define PVS_MN_CONSTANT_PARAM      _T('C')
#define PVS_MN_VAR_PARAM           _T('V')

#define PVS_MN_DT_BOOL_REDGE       _T("BOOL R_EDGE")
#define PVS_MN_DT_BOOL_FEDGE       _T("BOOL F_EDGE")


// constants to partition a xml node
#define PVS_XML_FB              _T("FUNCTION_BLOCK")
#define PVS_XML_FUN             _T("FUNCTION")
#define PVS_XML_SDT             _T("STRUCT")
#define PVS_XML_PRG             _T("PROGRAM")

#define PVS_XML_VAR             _T("VAR")
#define PVS_XML_STRUCT_VAR      _T("STRUCT_VAR")
#define PVS_XML_VAR_INPUT       _T("VAR_INPUT")
#define PVS_XML_VAR_OUTPUT      _T("VAR_OUTPUT")
#define PVS_XML_VAR_IN_OUT      _T("VAR_IN_OUT")
#define PVS_XML_VAR_IN_EXT      _T("VAR_IN_EXT")
#define PVS_XML_VAR_OUT_EXT     _T("VAR_OUT_EXT")
#define PVS_XML_VAR_GLOBAL      _T("VAR_GLOBAL")
#define PVS_XML_VAR_EXTERNAL    _T("VAR_EXTERNAL")
#define PVS_XML_VAR_TEMP        _T("VAR_TEMP")
#define PVS_XML_CONST           _T("CONST")

#define PVS_XML_RETAIN          _T("RETAIN")
#define PVS_XML_NON_RETAIN      _T("NON_RETAIN")
#define PVS_XML_CONSTANT        _T("CONSTANT")
// the next for the dialogs only
#define PVS_XML_NONRETAIN       _T("NONRETAIN")

#define PVS_XML_R_EDGE          _T("R_EDGE")
#define PVS_XML_F_EDGE          _T("F_EDGE")


// constants for drag and drop format
#define PVS_DAD_FB              _T("FB")
#define PVS_DAD_FUN             _T("FNC")
#define PVS_DAD_SDT             _T("SDT")

#define PVS_DAD_INPUT           _T("input")
#define PVS_DAD_OUTPUT          _T("output")
#define PVS_DAD_INOUT           _T("inoutput")
#define PVS_DAD_INEXT           _T("inext")
#define PVS_DAD_OUTEXT          _T("outext")


// constants to access the link file of a pou in a library
#define PVS_LINK_FILE       _T(".link")      // the link file name
#define PVS_LINK_IECIF      _T("iecif")     // the mangled name
#define PVS_LINK_POUCLASS   _T("pouclass")  // the native type of the pou
#define PVS_LINK_POUATTR    _T("pouattr")   // pou attribute
#define PVS_LINK_USERHELP   _T("UserHelpUrl")   // attribute: user help file
#define PVS_LINK_URL        _T("UserUrl")   // attribute: user url
#define PVS_LINK_HEADER     _T("header")    // header information

// other constants for library access
#define PVS_LIB_CONST_POU   _T("CONST")     // pou that contains constants
#define PVS_LIB_CHECKOUDIR  _T("\\gen\\temp\\")
#define PVS_LIB_CONSTEXT    _T("cst")

#define PVS_LIB_HELP        _T("__HELP")








// Simple Data Types definitions
// alternative basic type representation using
// a Type Bit Set GC_TBS_*  defines:
#define PVS_TBS_UNDEF            0x0000000000000000i64
#define PVS_TBS_VOID             0x0000000000000001i64
#define PVS_TBS_BOOL_R_EDGE      0x0000000000000002i64
#define PVS_TBS_BOOL_F_EDGE      0x0000000000000004i64
#define PVS_TBS_BOOL             0x0000000000000008i64
#define PVS_TBS_BYTE             0x0000000000000010i64
#define PVS_TBS_WORD             0x0000000000000020i64
#define PVS_TBS_DWORD            0x0000000000000040i64
#define PVS_TBS_LWORD            0x0000000000000080i64 
#define PVS_TBS_REAL             0x0000000000000100i64 
#define PVS_TBS_LREAL            0x0000000000000200i64 
#define PVS_TBS_SINT             0x0000000000000400i64 
#define PVS_TBS_INT              0x0000000000000800i64 
#define PVS_TBS_DINT             0x0000000000001000i64 
#define PVS_TBS_LINT             0x0000000000002000i64 
#define PVS_TBS_USINT            0x0000000000004000i64 
#define PVS_TBS_UINT             0x0000000000008000i64 
#define PVS_TBS_UDINT            0x0000000000010000i64 
#define PVS_TBS_ULINT            0x0000000000020000i64 
#define PVS_TBS_DT               0x0000000000040000i64 
#define PVS_TBS_TOD              0x0000000000080000i64 
#define PVS_TBS_DATE             0x0000000000100000i64 
#define PVS_TBS_TIME             0x0000000000200000i64 
#define PVS_TBS_STRING           0x0000000000400000i64 
#define PVS_TBS_WSTRING          0x0000000000800000i64 
#define PVS_TBS_INSTANCE         0x0000000001000000i64 
#define PVS_TBS_STRUCT           0x0000000002000000i64 
#define PVS_TBS_PROGRAM          0x0000000004000000i64 
#define PVS_TBS_ARRAY            0x0000000008000000i64
#define PVS_TBS_QVT_BOOL         0x0000000010000000i64 
#define PVS_TBS_QVT_BYTE         0x0000000020000000i64 
#define PVS_TBS_QVT_WORD         0x0000000040000000i64 
#define PVS_TBS_QVT_DWORD        0x0000000080000000i64 
#define PVS_TBS_QVT_LWORD        0x0000000100000000i64 
#define PVS_TBS_QVT_REAL         0x0000000200000000i64 
#define PVS_TBS_QVT_LREAL        0x0000000400000000i64 
#define PVS_TBS_QVT_SINT         0x0000000800000000i64 
#define PVS_TBS_QVT_INT          0x0000001000000000i64 
#define PVS_TBS_QVT_DINT         0x0000002000000000i64 
#define PVS_TBS_QVT_LINT         0x0000004000000000i64 
#define PVS_TBS_QVT_USINT        0x0000008000000000i64 
#define PVS_TBS_QVT_UINT         0x0000010000000000i64 
#define PVS_TBS_QVT_UDINT        0x0000020000000000i64 
#define PVS_TBS_QVT_ULINT        0x0000040000000000i64 
#define PVS_TBS_QVT_DT           0x0000080000000000i64 
#define PVS_TBS_QVT_TOD          0x0000100000000000i64 
#define PVS_TBS_QVT_DATE         0x0000200000000000i64 
#define PVS_TBS_QVT_TIME         0x0000400000000000i64 
#define PVS_TBS_QVT_STRING       0x0000800000000000i64 
#define PVS_TBS_QVT_WSTRING      0x0001000000000000i64 



//predefined sets, some correspond also to IEC keywords.
typedef __int64 PVS_BITSET;
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY         =0xffffffffffffffffi64;  


//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_REAL   =(PVS_TBS_REAL
                                  | PVS_TBS_LREAL);

const PVS_BITSET PVS_TBS_ANY_SIGN_INT=(PVS_TBS_SINT
                                   |  PVS_TBS_INT
                                   |  PVS_TBS_DINT
                                   |  PVS_TBS_LINT);

const PVS_BITSET PVS_TBS_ANY_USIGN_INT=(PVS_TBS_USINT
                                  | PVS_TBS_UINT
                                  | PVS_TBS_UDINT
                                  | PVS_TBS_ULINT);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_INT      =(PVS_TBS_ANY_SIGN_INT
                                   |PVS_TBS_ANY_USIGN_INT);

//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_NUM    =(PVS_TBS_ANY_REAL
                                  | PVS_TBS_ANY_INT);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_BIT    =(PVS_TBS_LWORD
                                  | PVS_TBS_DWORD
                                  | PVS_TBS_WORD
                                  | PVS_TBS_BYTE
                                  | PVS_TBS_BOOL);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_DATE   =(PVS_TBS_DT
                                  | PVS_TBS_DATE
                                  | PVS_TBS_TOD);

const PVS_BITSET PVS_TBS_ANY_SIGNED =(PVS_TBS_ANY_REAL
                                  | PVS_TBS_ANY_SIGN_INT
                                  | PVS_TBS_TIME);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_STRING =(PVS_TBS_STRING
                                  | PVS_TBS_WSTRING);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_ELEMENTARY=(PVS_TBS_ANY_NUM
                                  | PVS_TBS_ANY_BIT
                                  | PVS_TBS_ANY_STRING
                                  | PVS_TBS_ANY_DATE
                                  | PVS_TBS_TIME);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_DERIVED=(PVS_TBS_ARRAY
                                  | PVS_TBS_INSTANCE
                                  | PVS_TBS_STRUCT
                                  | PVS_TBS_PROGRAM);
//IEC Keword: 
const PVS_BITSET PVS_TBS_ANY_MAGNITUDE=(PVS_TBS_ANY_NUM
                                  | PVS_TBS_TIME);

const PVS_BITSET PVS_TBS_ANY_QVT    =(PVS_TBS_QVT_BOOL
                                  | PVS_TBS_QVT_BOOL
                                  | PVS_TBS_QVT_BYTE
                                  | PVS_TBS_QVT_WORD
                                  | PVS_TBS_QVT_DWORD
                                  | PVS_TBS_QVT_LWORD
                                  | PVS_TBS_QVT_REAL
                                  | PVS_TBS_QVT_LREAL
                                  | PVS_TBS_QVT_SINT
                                  | PVS_TBS_QVT_INT
                                  | PVS_TBS_QVT_DINT
                                  | PVS_TBS_QVT_LINT
                                  | PVS_TBS_QVT_USINT
                                  | PVS_TBS_QVT_UINT
                                  | PVS_TBS_QVT_UDINT
                                  | PVS_TBS_QVT_ULINT
                                  | PVS_TBS_QVT_DT
                                  | PVS_TBS_QVT_TOD
                                  | PVS_TBS_QVT_DATE
                                  | PVS_TBS_QVT_TIME
                                  | PVS_TBS_QVT_STRING
                                  | PVS_TBS_QVT_WSTRING);

const PVS_BITSET PVS_TBS_ANY_SIMPLE =(PVS_TBS_ANY_ELEMENTARY
                                  | PVS_TBS_ANY_QVT); 




#define PVS_TEXT_UNDEF            _T("---")
#define PVS_TEXT_VOID             _T("VOID")
#define PVS_TEXT_BOOL_R_EDGE      _T("BOOL R_EDGE")
#define PVS_TEXT_BOOL_F_EDGE      _T("BOOL F_EDGE")
#define PVS_TEXT_BOOL             _T("BOOL")
#define PVS_TEXT_BYTE             _T("BYTE")
#define PVS_TEXT_WORD             _T("WORD")
#define PVS_TEXT_DWORD            _T("DWORD")
#define PVS_TEXT_LWORD            _T("LWORD")
#define PVS_TEXT_REAL             _T("REAL")
#define PVS_TEXT_LREAL            _T("LREAL")
#define PVS_TEXT_SINT             _T("SINT")
#define PVS_TEXT_INT              _T("INT")
#define PVS_TEXT_DINT             _T("DINT")
#define PVS_TEXT_LINT             _T("LINT")
#define PVS_TEXT_USINT            _T("USINT")
#define PVS_TEXT_UINT             _T("UINT")
#define PVS_TEXT_UDINT            _T("UDINT")
#define PVS_TEXT_ULINT            _T("ULINT")
#define PVS_TEXT_DT               _T("DT")
#define PVS_TEXT_TOD              _T("TOD")
#define PVS_TEXT_DATE             _T("DATE")
#define PVS_TEXT_TIME             _T("TIME")
#define PVS_TEXT_STRING           _T("STRING")
#define PVS_TEXT_WSTRING          _T("WSTRING")
#define PVS_TEXT_INSTANCE         _T("---")
#define PVS_TEXT_STRUCT           _T("---")
#define PVS_TEXT_PROGRAM          _T("---")
#define PVS_TEXT_ARRAY            _T("ARRAY")
#define PVS_TEXT_QVT_BOOL         _T("QVT_BOOL")
#define PVS_TEXT_QVT_BYTE         _T("QVT_BYTE")
#define PVS_TEXT_QVT_WORD         _T("QVT_WORD")
#define PVS_TEXT_QVT_DWORD        _T("QVT_DWORD")
#define PVS_TEXT_QVT_LWORD        _T("QVT_LWORD")
#define PVS_TEXT_QVT_REAL         _T("QVT_REAL")
#define PVS_TEXT_QVT_LREAL        _T("QVT_LREAL")
#define PVS_TEXT_QVT_SINT         _T("QVT_SINT")
#define PVS_TEXT_QVT_INT          _T("QVT_INT")
#define PVS_TEXT_QVT_DINT         _T("QVT_DINT")
#define PVS_TEXT_QVT_LINT         _T("QVT_LINT")
#define PVS_TEXT_QVT_USINT        _T("QVT_USINT")
#define PVS_TEXT_QVT_UINT         _T("QVT_UINT")
#define PVS_TEXT_QVT_UDINT        _T("QVT_UDINT")
#define PVS_TEXT_QVT_ULINT        _T("QVT_ULINT")
#define PVS_TEXT_QVT_DT           _T("QVT_DT")
#define PVS_TEXT_QVT_TOD          _T("QVT_TOD")
#define PVS_TEXT_QVT_DATE         _T("QVT_DATE")
#define PVS_TEXT_QVT_TIME         _T("QVT_TIME")
#define PVS_TEXT_QVT_STRING       _T("QVT_STRING")
#define PVS_TEXT_QVT_WSTRING      _T("QVT_WSTRING")


#define PVS_TEXT_ANY              _T("QVT_ANY")
#define PVS_TEXT_ANY_REAL         _T("ANY_REAL")
#define PVS_TEXT_ANY_SIGN_INT     _T("ANY_SIGN_INT")
#define PVS_TEXT_ANY_USIGN_INT    _T("ANY_USIGN_INT")
#define PVS_TEXT_ANY_INT          _T("ANY_INT")
#define PVS_TEXT_ANY_NUM          _T("ANY_NUM")
#define PVS_TEXT_ANY_BIT          _T("ANY_BIT")
#define PVS_TEXT_ANY_DATE         _T("ANY_DATE")
#define PVS_TEXT_ANY_SIGNED       _T("ANY_SIGNED")
#define PVS_TEXT_ANY_STRING       _T("ANY_STRING")
#define PVS_TEXT_ANY_ELEMENTARY   _T("ANY_ELEMENTARY")
#define PVS_TEXT_ANY_DERIVED      _T("ANY_DERIVED")
#define PVS_TEXT_ANY_MAGNITUDE    _T("ANY_MAGNITUDE")
#define PVS_TEXT_ANY_QVT          _T("ANY_QVT")
#define PVS_TEXT_ANY_SIMPLE       _T("ANY_SIMPLE")




#endif // __POUVARDEFS_H_
