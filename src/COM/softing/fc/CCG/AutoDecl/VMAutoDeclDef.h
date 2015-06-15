
#ifndef __VMAUTODECLDEF_H_
#define __VMAUTODECLDEF_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//Enum, struct and table are used to lookup for the defined DATA types
typedef enum
{
    DATA_TYPE_BOOL,
    DATA_TYPE_WORD,
    DATA_TYPE_DWORD,
    DATA_TYPE_LWORD,
    DATA_TYPE_LREAL,
    DATA_TYPE_STRING,
    DATA_TYPE_WSTRING,
    DATA_TYPE_INT,
    DATA_TYPE_DINT,
    DATA_TYPE_LINT,
	DATA_TYPE_BYTE,
	DATA_TYPE_SINT,
	DATA_TYPE_REAL,		// d-227 01.06.05 SIS
} DATA_TYPE;

typedef struct 
{
    DATA_TYPE DataType;	
	TCHAR* pszDATAString;
    size_t iSize;
    char chPrefix;
} DATATYPE_DESC;

static DATATYPE_DESC DataTypeTbl[] = {
	{DATA_TYPE_BOOL,_T("BOOL"), 4, _T('X')},
    {DATA_TYPE_WORD,_T("WORD"), 4, _T('W')},
    {DATA_TYPE_DWORD,_T("DWORD"), 5, _T('D')},
    {DATA_TYPE_LWORD,_T("LWORD"), 5, _T('L')},
    {DATA_TYPE_LREAL,_T("LREAL"), 5, _T('L')},
    {DATA_TYPE_STRING,_T("STRING"), 6, _T('S')},
    {DATA_TYPE_WSTRING,_T("WSTRING"), 7, _T('S')},
    {DATA_TYPE_INT,_T("INT"), 3, _T('W')},
    {DATA_TYPE_DINT,_T("DINT"), 4, _T('D')},
    {DATA_TYPE_LINT,_T("LINT"), 4, _T('L')},
    {DATA_TYPE_BYTE,_T("BYTE"), 4, _T('B')},
    {DATA_TYPE_SINT,_T("SINT"), 4, _T('B')},
    {DATA_TYPE_REAL,_T("REAL"), 4, _T('D')},	// d-227 01.06.05 SIS
};


//Enum, struct and table are used to lookup for the defined types
typedef enum
{
    TYPE_GLOBAL,
    TYPE_INPUT,
    TYPE_OUTPUT
} TYPE;

typedef struct 
{
    TYPE Type;	
	char chCapitalize; 
    char chNotCapitalize;
} TYPE_DESC;

static TYPE_DESC TypeTbl[] = {
	{TYPE_GLOBAL,_T('M'), _T('m')},
    {TYPE_INPUT,_T('I'), _T('i')},
    {TYPE_OUTPUT,_T('Q'), _T('q')}
};

#define ADDRESS_FORMAT_STRING _T("%%%c%c%d.%d")
#define ADDRESS_FORMAT_STRING_BOOL _T("%%%c%c%d.%d.%d")

#define BYTES_PRO_BYTE 1
#define BYTES_PRO_WORD 2
#define BYTES_PRO_DOUBLE 4
#define BYTES_PRO_LONG 8
#define BIT_PRO_BYTE 8


#define COMMA _T(',')
#define NEWLINE _T('\n')
#define SEMICOLONE _T(';')
#define SPACE _T(' ')
#define UNDERSCORE _T('_')















#endif // __VMAUTODECLDEF_H_

