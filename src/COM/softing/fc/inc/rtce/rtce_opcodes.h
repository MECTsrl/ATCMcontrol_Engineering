/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/rtce/rtce_opcodes.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: rtce_opcodes.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/rtce/rtce_opcodes.h $
 *
 * =PROJECT             7VJ2061  ATCMControl ControlStack
 *
 * =SWKE                RTCE
 *
 * =COMPONENT           RTCE
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         NT Kernel:
 *                      MS Windows NT 4.0
 *                      NT DDK
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: rtce_opcodes.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/rtce
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/rtce
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 26.07.01   Time: 11:23
 * Created in $/4Control/COM/softing/fc/inc/rtce
 * 
 * *****************  Version 6  *****************
 * User: Ed           Date: 24.06.99   Time: 15:17
 * Updated in $/RTCE
 * 
 * *****************  Version 5  *****************
 * User: Ed           Date: 24.06.99   Time: 14:04
 * Updated in $/RTCE
 * add EQ, NE for BIT, UINT8, UINT16
 * 
 * *****************  Version 4  *****************
 * User: Ed           Date: 23.06.99   Time: 15:31
 * Updated in $/RTCE
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 18.06.99   Time: 16:13
 * Updated in $/RTCE
 * add globalSegment
 * 
 * *****************  Version 2  *****************
 * User: Ed           Date: 18.06.99   Time: 10:43
 * Updated in $/RTCE
 * moved some useful macros to rtce_opcodes.h
 * 
 * *****************  Version 1  *****************
 * User: Ed           Date: 11.06.99   Time: 9:29
 * Created in $/RTCE
 *==
 ******************************************************************************
H<<*/

//public header file for RTCE interpreter
#define RTCE_INTERPRETER_VERSION 19990601

//IEC Types and Names
//
typedef long            RTCE_BOOL32;
typedef unsigned char   RTCE_BYTE;
typedef unsigned short  RTCE_WORD;
typedef long            RTCE_DINT;
typedef unsigned long   RTCE_DWORD;
typedef __int64         RTCE_TIME;

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif


typedef long    RTCE_IPTR;        // relative instruction pointer
typedef long    RTCE_OFFSET;      // relative data pointer
typedef unsigned long RTCE_BITOFFSET;   // relative data pointer (in bits!)
typedef unsigned char *RTCE_DATA; // pointer to CODE and DATA.

typedef struct tagRTCE_SEGMENT_DEF{
    RTCE_OFFSET startOffset; //first byte
    RTCE_OFFSET endOffset;   //last byte+1
} RTCE_SEGMENT_DEF;   //length = endOffset-startOffset

typedef struct tagRTCE_CODE_HEADER{
    long versionID;
    long nBytesCodeAndData; //and header!
    RTCE_SEGMENT_DEF statusSegment; //readable
    RTCE_SEGMENT_DEF inputSegment;  //readable
    RTCE_SEGMENT_DEF outputSegment; //read&writable
    RTCE_SEGMENT_DEF globalSegment; //read&writable
    RTCE_SEGMENT_DEF dataSegment;   //read&writable
    RTCE_SEGMENT_DEF constSegment;  //readonly
    RTCE_SEGMENT_DEF codeSegment;   //codeonly
    char             reserved[16];
} RTCE_CODE_HEADER;

typedef struct tagRTCE_INSTRUCTION{
  unsigned short opcode;
  unsigned short params;
} RTCE_INSTRUCTION;

// utility functions
/*
 * RTCE_BOOL32 RTCE_GET_BIT(RTCE_DATA data, RTCE_BITOFFSET offs);
 */
#define RTCE_GET_BIT(data, offs)      (data[(offs)/8]>>((offs)&7) & 1)
/*
 * VOID RTCE_SET_BIT(RTCE_DATA data, RTCE_BITOFFSET offs, RTCE_BOOL32 value);
 */
#define RTCE_SET_BIT(data, offs, val) (data[(offs)/8] = (unsigned char)(data[(offs)/8] & ~(1<<((offs)&7)) | ((val)&1)<<((offs)&7)))

/*
 * RTCE_BYTE RTCE_GET_UINT8(RTCE_DATA data, RTCE_OFFSET offs);
 */
#define RTCE_GET_UINT8(data, offs)      (data[offs])
/*
 * VOID RTCE_SET_UINT8(RTCE_DATA data, RTCE_OFFSET offs, RTCE_BYTE value);
 */
#define RTCE_SET_UINT8(data, offs, val) (data[offs] = (val))

/*
 * RTCE_WORD RTCE_GET_UINT16(RTCE_DATA data, RTCE_OFFSET offs);
 */
#define RTCE_GET_UINT16(data, offs)      (*(RTCE_WORD*)&data[offs])
/*
 * VOID RTCE_SET_UINT16(RTCE_DATA data, RTCE_OFFSET offs, RTCE_WORD value);
 */
#define RTCE_SET_UINT16(data, offs, val) (*(RTCE_WORD*)&data[offs] = (val))

/*
 * RTCE_DWORD RTCE_GET_UINT32(RTCE_DATA data, RTCE_OFFSET offs);
 */
#define RTCE_GET_UINT32(data, offs)      (*(RTCE_DWORD*)&data[offs])
/*
 * VOID RTCE_SET_UINT32(RTCE_DATA data, RTCE_OFFSET offs, RTCE_DWORD value);
 */
#define RTCE_SET_UINT32(data, offs, val) (*(RTCE_DWORD*)&data[offs] = (val))

/*
 * RTCE_DINT RTCE_GET_INT32(RTCE_DATA data, RTCE_OFFSET offs);
 */
#define RTCE_GET_INT32(data, offs)      (*(RTCE_DINT*)&data[offs])
/*
 * VOID RTCE_SET_INT32(RTCE_DATA data, RTCE_OFFSET offs, RTCE_DINT value);
 */
#define RTCE_SET_INT32(data, offs, val) (*(RTCE_DINT*)&data[offs] = (val))

/*
 * RTCE_TIME RTCE_GET_INT64(RTCE_DATA data, RTCE_OFFSET offs);
 */
#define RTCE_GET_INT64(data, offs)      (*(RTCE_TIME*)&data[offs])
/*
 * VOID RTCE_SET_INT64(RTCE_DATA data, RTCE_OFFSET offs, RTCE_TIME value);
 */
#define RTCE_SET_INT64(data, offs, val) (*(RTCE_TIME*)&data[offs] = (val))

/*
 * RTCE_OFFSET RTCE_GET_SEGMENT_LEN(RTCE_SEGMENT_DEF s);
 */
#define RTCE_GET_SEGMENT_LEN(s) ((s).endOffset-(s).startOffset)


//basic opcodes
#define RTCE_OPCODE_RETURN     0
#define RTCE_OPCODE_RETURN_IF_TRUE  1
#define RTCE_OPCODE_RETURN_IF_FALSE 2

typedef struct tagRTCE_RETURN_IF_PARAM{
    RTCE_BITOFFSET condition;
} RTCE_RETURN_IF_PARAM;

#define RTCE_OPCODE_JUMP       3

typedef struct tagRTCE_JUMP_PARAM{
    RTCE_IPTR      branch;
} RTCE_JUMP_PARAM;

#define RTCE_OPCODE_JUMP_IF_TRUE    4
#define RTCE_OPCODE_JUMP_IF_FALSE   5

typedef struct tagRTCE_JUMP_IF_PARAM{
    RTCE_BITOFFSET condition;
    RTCE_IPTR      branch;
} RTCE_JUMP_IF_PARAM;

//move can handle multiple assignments!
#define RTCE_OPCODE_MOVE_BIT   6

typedef struct tagRTCE_MOVE_BIT_PARAM{
    RTCE_BITOFFSET source;
    RTCE_BITOFFSET target;
} RTCE_MOVE_BIT_PARAM;

//move can handle multiple assignments!
#define RTCE_OPCODE_MOVE_INT8  7
#define RTCE_OPCODE_MOVE_INT16 8
#define RTCE_OPCODE_MOVE_INT32 9
#define RTCE_OPCODE_MOVE_INT64 10

typedef struct tagRTCE_MOVE_INT_PARAM{
    RTCE_OFFSET source;
    RTCE_OFFSET target;
} RTCE_MOVE_INT_PARAM;

#define RTCE_OPCODE_MOVE_UINT8_TO_32  11
#define RTCE_OPCODE_MOVE_UINT16_TO_32 12
#define RTCE_OPCODE_MOVE_INT32_TO_8   RTCE_OPCODE_MOVE_INT8
#define RTCE_OPCODE_MOVE_INT32_TO_16  RTCE_OPCODE_MOVE_INT16

#define RTCE_OPCODE_SWAP_UINT16 13 // uses RTCE_MOVE_INT_PARAM
#define RTCE_OPCODE_SWAP_UINT32 14 // uses RTCE_MOVE_INT_PARAM


//arithmetic opcodes
#define RTCE_OPCODE_ADD_INT32   16
#define RTCE_OPCODE_SUB_INT32   17
#define RTCE_OPCODE_MUL_INT32   18
#define RTCE_OPCODE_DIV_INT32   19
#define RTCE_OPCODE_MOD_INT32   20
#define RTCE_OPCODE_ABS_INT32   21

typedef struct tagRTCE_BINARY_INT_PARAM { // SUB, DIV, MOD
  RTCE_OFFSET in1;
  RTCE_OFFSET in2;
  RTCE_OFFSET out;
} RTCE_BINARY_INT_PARAM;

typedef struct tagRTCE_UNARY_INT_PARAM { // ABS
  RTCE_OFFSET in;
  RTCE_OFFSET out;
} RTCE_UNARY_INT_PARAM;

#define RTCE_OPCODE_NE_INT32    22
#define RTCE_OPCODE_NE_BIT      74
#define RTCE_OPCODE_NE_UINT8    75
#define RTCE_OPCODE_NE_UINT16   76
#define RTCE_OPCODE_NE_UINT32   RTCE_OPCODE_NE_INT32
#define RTCE_OPCODE_EQ_INT32    23
#define RTCE_OPCODE_EQ_BIT      77
#define RTCE_OPCODE_EQ_UINT8    78
#define RTCE_OPCODE_EQ_UINT16   79
#define RTCE_OPCODE_EQ_UINT32   RTCE_OPCODE_EQ_INT32
#define RTCE_OPCODE_GT_INT32    24
#define RTCE_OPCODE_GE_INT32    25
#define RTCE_OPCODE_LT_INT32    26
#define RTCE_OPCODE_LE_INT32    27

// the codes EQ, GT, GE, LT, LE are extensible

typedef struct tagRTCE_COMPARE_INT_PARAM { // nonextensible like NE
  RTCE_OFFSET in1;
  RTCE_OFFSET in2;
  RTCE_BITOFFSET out;
} RTCE_COMPARE_INT_PARAM;

typedef struct tagRTCE_COMPARE_BIT_PARAM { // nonextensible like NE
  RTCE_BITOFFSET in1;
  RTCE_BITOFFSET in2;
  RTCE_BITOFFSET out;
} RTCE_COMPARE_BIT_PARAM;

/*
 * GT, GE .. fuer UINT8, 16, 32? TIME??
 */

//boolean opcodes
#define RTCE_OPCODE_AND_BIT     28
#define RTCE_OPCODE_AND_UINT8   29
#define RTCE_OPCODE_AND_UINT16  30
#define RTCE_OPCODE_AND_UINT32  31
#define RTCE_OPCODE_OR_BIT      32
#define RTCE_OPCODE_OR_UINT8    33
#define RTCE_OPCODE_OR_UINT16   34
#define RTCE_OPCODE_OR_UINT32   35
#define RTCE_OPCODE_XOR_BIT     36
#define RTCE_OPCODE_XOR_UINT8   37
#define RTCE_OPCODE_XOR_UINT16  38
#define RTCE_OPCODE_XOR_UINT32  39
#define RTCE_OPCODE_NOT_BIT     40
#define RTCE_OPCODE_NOT_UINT8   41
#define RTCE_OPCODE_NOT_UINT16  42
#define RTCE_OPCODE_NOT_UINT32  43
#define RTCE_OPCODE_ROL_UINT8   44
#define RTCE_OPCODE_ROL_UINT16  45
#define RTCE_OPCODE_ROL_UINT32  46
#define RTCE_OPCODE_ROR_UINT8   47
#define RTCE_OPCODE_ROR_UINT16  48
#define RTCE_OPCODE_ROR_UINT32  49
#define RTCE_OPCODE_SHL_UINT8   50
#define RTCE_OPCODE_SHL_UINT16  51
#define RTCE_OPCODE_SHL_UINT32  52
#define RTCE_OPCODE_SHR_UINT8   53
#define RTCE_OPCODE_SHR_UINT16  54
#define RTCE_OPCODE_SHR_UINT32  55
#define RTCE_OPCODE_MUX_BIT     56
#define RTCE_OPCODE_MUX_UINT8   57
#define RTCE_OPCODE_MUX_UINT16  58
#define RTCE_OPCODE_MUX_UINT32  59
#define RTCE_OPCODE_SEL_BIT     60
#define RTCE_OPCODE_SEL_UINT8   61
#define RTCE_OPCODE_SEL_UINT16  62
#define RTCE_OPCODE_SEL_UINT32  63

typedef struct tagRTCE_NOT_BIT_PARAM {
  RTCE_BITOFFSET in;
  RTCE_BITOFFSET out;
} RTCE_NOT_BIT_PARAM;

typedef struct tagRTCE_NOT_INT_PARAM {
  RTCE_OFFSET in;
  RTCE_OFFSET out;
} RTCE_NOT_INT_PARAM;

typedef struct tagRTCE_ROTATE_PARAM {
  RTCE_OFFSET in;
  RTCE_OFFSET n;
  RTCE_OFFSET out;
} RTCE_ROTATE_PARAM;

typedef struct tagRTCE_SEL_BIT_PARAM {
  RTCE_BITOFFSET g;
  RTCE_BITOFFSET in0;
  RTCE_BITOFFSET in1;
  RTCE_BITOFFSET out;
} RTCE_SEL_BIT_PARAM;

typedef struct tagRTCE_SEL_INT_PARAM {
  RTCE_BITOFFSET g;
  RTCE_OFFSET in0;
  RTCE_OFFSET in1;
  RTCE_OFFSET out;
} RTCE_SEL_INT_PARAM;

/*
 * LIMIT, MIN, MAX?
 */

//standard functions
#define RTCE_OPCODE_SR          64
#define RTCE_OPCODE_RS          65
#define RTCE_OPCODE_F_TRIG      66
#define RTCE_OPCODE_R_TRIG      67
#define RTCE_OPCODE_CTU         68
#define RTCE_OPCODE_CTD         69
#define RTCE_OPCODE_CTUD        70
#define RTCE_OPCODE_TOF         71
#define RTCE_OPCODE_TON         72
#define RTCE_OPCODE_TP          73


typedef struct tagRTCE_SET_RESET_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET s1_in;
  RTCE_BITOFFSET r1_in;
  RTCE_BITOFFSET q1_out;
} RTCE_SET_RESET_PARAM;

typedef struct tagRTCE_SET_RESET_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    s1;
  RTCE_BOOL32    r1;
  RTCE_BOOL32    q1;
} RTCE_SET_RESET_INST;

typedef struct tagRTCE_TRIG_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET clk_in;
  RTCE_BITOFFSET q_out;
} RTCE_TRIG_PARAM;

typedef struct tagRTCE_TRIG_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    clk;
  RTCE_BOOL32    q;
  RTCE_BOOL32    m;
} RTCE_TRIG_INST;

typedef struct tagRTCE_CTU_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET cu_in;
  RTCE_BITOFFSET r_in;
  RTCE_OFFSET    pv_in;
  RTCE_BITOFFSET q_out;
  RTCE_OFFSET    cv_out;
} RTCE_CTU_PARAM;

typedef struct tagRTCE_CTU_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    cu;
  RTCE_BOOL32    r;
  RTCE_DINT      pv;
  RTCE_BOOL32    q;
  RTCE_DINT      cv;
  RTCE_BOOL32    cu_old;
} RTCE_CTU_INST;

typedef struct tagRTCE_CTD_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET cd_in;
  RTCE_BITOFFSET ld_in;
  RTCE_OFFSET    pv_in;
  RTCE_BITOFFSET q_out;
  RTCE_OFFSET    cv_out;
} RTCE_CTD_PARAM;

typedef struct tagRTCE_CTD_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    cd;
  RTCE_BOOL32    ld;
  RTCE_DINT      pv;
  RTCE_BOOL32    q;
  RTCE_DINT      cv;
  RTCE_BOOL32    cd_old;
} RTCE_CTD_INST;

typedef struct tagRTCE_CTUD_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET cu_in;
  RTCE_BITOFFSET cd_in;
  RTCE_BITOFFSET r_in;
  RTCE_BITOFFSET ld_in;
  RTCE_OFFSET    pv_in;
  RTCE_BITOFFSET qu_out;
  RTCE_BITOFFSET qd_out;
  RTCE_OFFSET    cv_out;
} RTCE_CTUD_PARAM;

typedef struct tagRTCE_CTUD_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    cu;
  RTCE_BOOL32    cd;
  RTCE_BOOL32    r;
  RTCE_BOOL32    ld;
  RTCE_DINT      pv;
  RTCE_BOOL32    qu;
  RTCE_BOOL32    qd;
  RTCE_DINT      cv;
  RTCE_BOOL32    cu_old;
  RTCE_BOOL32    cd_old;
} RTCE_CTUD_INST;

typedef struct tagRTCE_TONF_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET in_in;
  RTCE_OFFSET    pt_in;
  RTCE_BITOFFSET q_out;
  RTCE_OFFSET    et_out;
} RTCE_TONF_PARAM;

typedef struct tagRTCE_TONF_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    in;
  RTCE_TIME      pt;
  RTCE_BOOL32    q;
  long           align0;
  RTCE_TIME      et;
  RTCE_TIME      starttime;
  RTCE_BOOL32    trig;
  long           align1;
} RTCE_TONF_INST;

typedef struct tagRTCE_TP_PARAM {
  RTCE_OFFSET    inst;
  RTCE_BITOFFSET in_in;
  RTCE_OFFSET    pt_in;
  RTCE_BITOFFSET q_out;
  RTCE_OFFSET    et_out;
} RTCE_TP_PARAM;

typedef struct tagRTCE_TP_INST {
  RTCE_OFFSET    size;
  RTCE_BOOL32    in;
  RTCE_TIME      pt;
  RTCE_BOOL32    q;
  long           align0;
  RTCE_TIME      et;
  RTCE_BOOL32    m;
  RTCE_BOOL32    trig;
  RTCE_TIME      starttime;
} RTCE_TP_INST;
