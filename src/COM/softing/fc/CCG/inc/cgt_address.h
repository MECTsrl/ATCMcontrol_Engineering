
#ifndef __CGT_ADDRESS_H_
#define __CGT_ADDRESS_H_


//----  Aggregate Includes:   --------------------------------------*
#include <tchar.h>

//----  Forward Class Definitions:   -------------------------------*

//----  Prototypes:    ---------------------------------------------*






//TODO comment !
struct CGT_CHECK_ADDR_TYP
{
    void*                   pContext;
    const TCHAR*            pszAddr;
    bool (*AddressScanHelp)(const CGT_CHECK_ADDR_TYP* pContext, const TCHAR* psz, int nChars, const TCHAR* pszExpect);
};


// Syntax:
// pszAddr := "%"[IQM][XBWDL]?{field}(\.{field})*
//
// or if incomplete address definition:
// pszAddr := "%"(\*([.+]))?{field}(\.{field})
//
// or if incomplete address referece:
// pszAddr := "%"[IQM][XBWDL]?\*[.+]{field}(\.{field})*
// field = ".."|integer|identifier
bool CGT_CheckDirectAddrSyntax(
    const TCHAR*        pszAddr, 
    const TCHAR**       ppszEnd, 
    CGT_CHECK_ADDR_TYP* pContext //NULL or contains call back for err msg
);




TCHAR CGT_GetAddrLocation(const TCHAR* pszAddr);
int   CGT_GetAddrChannelNumber(const TCHAR* pszAddr);

//return %[IQM][X] = 1, %[IQM]B = 8, %[IQM]W = 16, %[IQM]D = 32, %[IQM]L = 64
//       %[IQM]S = -1, all other syntax assert and return 0
int   CGT_GetAddrBitSize(const TCHAR* pszAddr);
bool  CGT_IsFullBasicAddress(const TCHAR* pszAddr);
bool  CGT_IsIncompleteAddressDef(const TCHAR* pszAddr);
bool  CGT_IsIncompleteAddressRef(const TCHAR* pszAddr);
bool  CGT_IsVarConfigAddressRef(const TCHAR* pszAddr);
bool  CGT_IsByteSwapAddress(const TCHAR* pszAddr);

#endif // __CGT_ADDRESS_H_

