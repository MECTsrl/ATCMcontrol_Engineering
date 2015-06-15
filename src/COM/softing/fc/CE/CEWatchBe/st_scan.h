#ifndef __ST_SCAN_H_
#define __ST_SCAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#include <stdio.h>
#include <tchar.h>

class sts_scanner;

enum STS_ScannerMode
{
    STS_EXPRESSION_SYNTAX,
    STS_RANGES_SYNTAX
};

//C-interface to scanner.
extern "C"
{
const int STS_MAX_TOKEN_LENGTH=255;

sts_scanner *STS_CreateScanner ();
void        STS_DeleteScanner (sts_scanner *pScanner);

LONG STS_Column (const sts_scanner *pScanner);
LONG STS_GetOffset (const sts_scanner *pScanner);
LONG STS_GetStartOffset (const sts_scanner *pScanner);

void STS_SetInputString (sts_scanner *pScanner, const TCHAR* pszInput, unsigned long ulLen);
int  STS_NextToken (sts_scanner *pScanner, const TCHAR **pszToken);

int  STS_SetScannerMode(sts_scanner *pScanner, STS_ScannerMode nState);
}//extern "C"

//tokens:
enum STS_TokenEnum
{
    STS_EOF = 0,

    /* to be used in yacc tokens must have valued >256! */
    STS_IDENTIFIER = 257,
    STS_DIRECT_VARIABLE,
    
    STS_INT_LITERAL,
    STS_FLOAT_LITERAL,
    STS_DATE_LITERAL,
    STS_DT_LITERAL,
    STS_TOD_LITERAL,
    STS_TIME_LITERAL,
    STS_TRUE_LITERAL,
    STS_FALSE_LITERAL,
    STS_STRING_LITERAL,
    STS_WSTRING_LITERAL,

    STS_MOD,
    STS_POW,
    STS_AND,
    STS_OR,
    STS_XOR,
    STS_NOT,
    STS_EQ,
    STS_NE,
    STS_LT,
    STS_LE,
    STS_GT,
    STS_GE,
    STS_EQ_SHORT,
    STS_NE_SHORT,
    STS_LT_SHORT,
    STS_LE_SHORT,
    STS_GT_SHORT,
    STS_GE_SHORT,
    STS_ADD,
    STS_SUB,
    STS_MUL,
    STS_DIV,

    STS_MACHINE_ADDRESS,

    STS_FIRST_CONVERSION,
    STS_BYTE_TO_DINT=STS_FIRST_CONVERSION,
    STS_BYTE_TO_DWORD,
    STS_BYTE_TO_LREAL,
    STS_BYTE_TO_WORD,
    STS_DINT_TO_BYTE,
    STS_DINT_TO_DWORD,
    STS_DINT_TO_LREAL,
    STS_DINT_TO_TIME,
    STS_DINT_TO_WORD,
    STS_DWORD_TO_BYTE,
    STS_DWORD_TO_DINT,
    STS_DWORD_TO_LREAL,
    STS_DWORD_TO_WORD,
    STS_LREAL_TO_BYTE,
    STS_LREAL_TO_DINT,
    STS_LREAL_TO_DWORD,
    STS_LREAL_TO_WORD,
    STS_TIME_TO_DINT,
    STS_WORD_TO_BYTE,
    STS_WORD_TO_DINT,
    STS_WORD_TO_DWORD,
    STS_WORD_TO_LREAL,

    STS_RANGES_NUMBER,

    STS_DOTDOT,

    STS_SYNTAX_ERROR
};

#endif // __ST_SCAN_H_

