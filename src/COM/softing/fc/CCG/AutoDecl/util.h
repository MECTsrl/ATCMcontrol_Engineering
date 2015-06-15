
#ifndef __UTIL_H_
#define __UTIL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <limits.h>
#include "baselib.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



class  LU_CDiag
{
public:
    TCHAR        szDetails[MAX_PATH];
    
    LU_CDiag(){szDetails[0] = 0;}
    //init(unsigned uiFmt, ...); TOOD use resource
    set(const TCHAR* pszFmt, ...);
};






#define LU_MAX_ADDR_FIELDS  32
#define LU_MAX_ADDR_TCHARS  256
//pure syntax split by LU_ParseIECDirectAddress
typedef struct _LU_IECADDRESS
{
    TCHAR           szSplit[LU_MAX_ADDR_TCHARS];//a splitted copy of the address
                                                //all '.' and '!' seperators
                                                //are replaced by a 0-char
    TCHAR           cLoc;     //I,Q,M
    TCHAR           cSize;    //X, B, W, D, L, S
    unsigned long   nBits;    //1, 8,16,32,64, 0
    long            nFields;  //number of numeric fields
    unsigned long   ulFields [LU_MAX_ADDR_FIELDS];//ul-value of fields
    const TCHAR*    pszFields[LU_MAX_ADDR_FIELDS];//points into szSplit
    long            nMods;    //number of !<modifiers>
    const TCHAR*    pszMods[LU_MAX_ADDR_FIELDS]; //points into szSplit
} LU_IECADDRESS;
//read from pszAddr as long as it looks like an IEC %-address
//modifier list end on the first char that is not iscsym:
BOOL LU_ParseIECDirectAddress( //return FALSE if any syntax or limit error
    LU_CDiag*        pDiag,    //detail diag
    const TCHAR*     pszAddr,  //where to read from
    LU_IECADDRESS*   pAdrInfo, //NULL or parse result 
    const TCHAR**    ppszEnd   //NULL or receives the char that stopped the scan
);
//Example:  
//LU_ParseIECDirectAddress(&diag,"%IW0.2.4!Swap!BCD,text",&adr,&pszEnd);
//szAddr = "%IW0.2.4!Swap!BCD"
//cLoc  = 'I'   
//cSize = 'W'
//nFields = 3
//ulFields[0]=0    pszFields[0] = "0"
//ulFields[1]=2    pszFields[1] = "2"
//ulFields[2]=4    pszFields[2] = "4"
//nMods = 2
//pszMod[0]="Swap"
//pszMod[1]="BCD"
//
//pszEnd = ",text"




class IO_COMP_TYP
{
public:
    IO_COMP_TYP*  pNext;               // used to hold a short list of channel definitions
    long          lChannelNum;         // channel num ofd this IO component
    unsigned long ulISegOffs;          // offset for INPUT variables
    unsigned long ulOSegOffs;          // offset for OUTPUT variables
    unsigned long ulGMSegOffs;         // offset for global variables
    unsigned long ulGMRSegOffs;        // offset for global retain variables
    unsigned long ulIChnSize;          // mem size for INPUT variables
    unsigned long ulOChnSize;          // mem size for OUTPUT variables
    unsigned long ulGMChnSize;         // mem size for global variables //REMOVE
    unsigned long ulGMRChnSize;        // mem size for global retain variables //REMOVE

    IO_COMP_TYP()
    {
        pNext = NULL;
        lChannelNum = -1;
        ulISegOffs = ulOSegOffs = ulGMSegOffs = ulGMRSegOffs = ULONG_MAX;
        ulIChnSize = ulOChnSize = ulGMChnSize = ulGMRChnSize = ULONG_MAX;
    }
    ~IO_COMP_TYP()
    {
        delete(pNext);
    }
};

BOOL Parse4CVMKADForChannels(       //return false if file could not be parsed
    const TCHAR*    pszTrgTypeName, //4cwin, 4cosai, ...
    const TCHAR*    pszAutoDeclIni, //full path and file name of AutoDecl.ini
    IO_COMP_TYP**   ppIOCmps        //return, maqy be NULL if no channel def found
);

BOOL LookUpChannelDef( //return true if a channel was found, false if failed
    const IO_COMP_TYP*  pChannelDefs, //list of known channels
    TCHAR               cLoc,         //I,Q or M
    long                nChannel,     //number of channel (first field of %-addr)
    unsigned long*      pulOff,       //output, set to ULONG_MAX if function failed
    unsigned long*      pulSize       //output, set to ULONG_MAX if function failed
);

BOOL ParseDataType( //return true if type could be parsed, false if not
    const wchar_t* pszIECType,  //simple type or at most complex (W)STRING[<len>] (no arrays!)
    BL_IEC_TYP*    piecTyp,     //output: baselib enum of basic type.
    long*          pstrLength   //output: string length if any sort of string, 0 otherwise 
);

//build a string that can be interpreted by the OPC Server (see also OPC Server's 
//implementaion -> function 'IsDirectVarAddress')
BOOL BuildOnlString(
    TCHAR                   szOnlString[MAX_PATH], //output: example: L"__system.direct.W.2.2"
    const LU_IECADDRESS*    pAddr,                 //parsed IEC address
    unsigned long           ulChannelOffset,       //channel offset
    BL_IEC_TYP              iecTyp,                //iec type to be used
    long                    strLength              //str len is type is STRING
);

#endif // __UTIL_H_

