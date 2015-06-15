#include <ctype.h>
#include <stdlib.h>
#include "fc_todebug\fc_assert.h"
#include "cgt_address.h"


static bool ScanAddressHierarchyFields(
    const TCHAR**       ppsz, 
    CGT_CHECK_ADDR_TYP* pContext, 
    const TCHAR**       ppszExpected
);



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
    CGT_CHECK_ADDR_TYP* pContext
)
{
    const TCHAR* psz = pszAddr;
    const TCHAR* pszExpected = NULL;

    if(pContext)
    {
        assert(pContext->pContext && pContext->AddressScanHelp!=NULL);
        pContext->pszAddr = pszAddr;
    }

    if(*psz != _T('%'))
    {
        pszExpected= _T("%");
        goto end;
    }
    psz = _tcsinc(psz);


    if(_istalpha(*psz))
    {
        switch(*psz)
        {
        case _T('I'): //case _T('i'):
        case _T('Q'): //case _T('q'):
        case _T('M'): //case _T('m'):
            psz = _tcsinc(psz);
        break;
        default:
            pszExpected= _T("I|Q|M|*|<digit>");
            goto end;
        }
    }
    else
    {    
        if(*psz==_T('*'))
        {
            // "%*.2.6"
            psz = _tcsinc(psz);
            
            if(*psz==0)
            {
                if(pContext)
                    pContext->AddressScanHelp(pContext, pszAddr, (psz-pszAddr)/sizeof(TCHAR), NULL);
                goto end;
            }

            if(*psz==_T('+') || *psz==_T('.'))
            {
                psz = _tcsinc(psz);
            }
            else if(*psz==_T('!'))
            {
                //psz = _tcsinc(psz);
            }
            else
            {
                pszExpected= _T("+|.|!|<end>");
                goto end;
            }
        }
 
        if(pContext)
            pContext->AddressScanHelp(pContext, pszAddr, (psz-pszAddr)/sizeof(TCHAR), NULL);
        
        ScanAddressHierarchyFields(&psz, pContext, &pszExpected);
        goto end;
    }

    //"%"[IQM][XBWDLS]?{field}(\.{field})* I|Q|M|*|<hierarchy field>
    // or
    //"%"[IQM][XBWDLS]?\*[.+]{field}(\.{field})*
    //Note: if %IXconst.const is used, then the X cannot be defaulted: 
    if(_istalpha(*psz) || *psz==_T('_'))
    {
        switch(*psz)
        {
        case _T('X'): //case _T('x'):
        case _T('B'): //case _T('b'):
        case _T('W'): //case _T('w'):
        case _T('D'): //case _T('d'):
        case _T('L'): //case _T('l'):
        case _T('S'): //case _T('s'):
            psz = _tcsinc(psz);
        break;
        default:
            pszExpected= _T("X|B|W|D|L|S");
            goto end;
        }
    }
    
    if(*psz==_T('*'))
    {
        psz = _tcsinc(psz);

        if(*psz==0)
        {
            if(pContext)
                pContext->AddressScanHelp(pContext, pszAddr, (psz-pszAddr)/sizeof(TCHAR), NULL);
            goto end;
        }
        
        if(*psz==_T('+') || *psz==_T('.'))
        {
            psz = _tcsinc(psz);
        }
        else if(*psz==_T('!'))
        {
            //psz = _tcsinc(psz);
        }
        else
        {
            pszExpected= _T("+|.|!|<end>");
            goto end;
        }
    }

    if(pContext)
        pContext->AddressScanHelp(pContext, pszAddr, (psz-pszAddr)/sizeof(TCHAR), NULL);

    ScanAddressHierarchyFields(&psz, pContext, &pszExpected);

end:
    if(ppszEnd)
        *ppszEnd= psz;

    //if pContext and there is an error use pContext->AddressScanHelp
    //to report the error, but not if CONST, this kind of error
    //was produced by AddressScanHelp anyway:
    if(pContext && pszExpected && _tcscmp(pszExpected, _T("CONST")))
        pContext->AddressScanHelp(pContext, psz, (psz-pszAddr)/sizeof(TCHAR), pszExpected);

    return pszExpected==NULL;
}

static bool ScanAddressHierarchyFields(
    const TCHAR**       ppsz, 
    CGT_CHECK_ADDR_TYP* pContext, 
    const TCHAR**       ppszExpected
)
{
    const TCHAR* pszStart = *ppsz;
    const TCHAR* psz = *ppsz;
    const TCHAR* pszExpected = NULL;
    
    if(!*psz)
    {
        pszExpected =  _T("0-9|!<modifier>|CONST");
        goto end;
    }


    for(;;)
    {
        bool bNewField = false;
        if(_istdigit(*psz))
        {
            psz = _tcsinc(psz);
            while(_istdigit(*psz))
                psz = _tcsinc(psz);
            
            bNewField = true;
        }
        else if(*psz==_T('"'))
        {
            psz = _tcsinc(psz);
            while(*psz)
            {
                if(*psz==_T('"'))
                    break;
                psz = _tcsinc(psz);
            }
            if(*psz!=_T('"'))
            {
                pszExpected = _T("WSTRING");
                goto end;
            }
            psz = _tcsinc(psz);
            bNewField = true;
        }
        else if(_istalpha(*psz) || *psz==_T('_') || *psz==_T('$'))
        {
            if(pContext && psz!=pszStart)
            {
                pContext->AddressScanHelp(pContext, pszStart, (psz-pszStart)/sizeof(TCHAR), NULL);
                pszStart = psz;
            }
            
            if(*psz==_T('$'))
            {
                psz = _tcsinc(psz);
                pszStart = psz;
            }

            while(_istalnum(*psz) || *psz==_T('_'))
                psz = _tcsinc(psz);

            if(pContext)
            {
                if(!pContext->AddressScanHelp(pContext, pszStart, (psz-pszStart)/sizeof(TCHAR), NULL))
                {
                    pszExpected = _T("CONST");
                    goto end;
                }
                pszStart = psz;
            }
            bNewField = true;
        }
        
        if(*psz==0)
        {
           goto end;
        }
        else if(*psz==_T('.') && bNewField)
        {
            psz = _tcsinc(psz);
        }
        else if(*psz==_T('!'))
        {
            goto modifier;
        }
        else
        {
            pszExpected = _T("0-9|!<modifier>|CONST");
            goto end;
        }
    }

modifier:
    assert(!*psz || *psz==_T('!'));
    if(pContext && pszStart!=psz)
        pContext->AddressScanHelp(pContext, pszStart, (psz-pszStart)/sizeof(TCHAR), NULL);

    pszStart = psz;
    if(*psz)
    {
        while(*psz)
        {
            psz = _tcsinc(psz);
            if(*psz==_T('!'))
                goto modifier;
        }
        goto modifier;
    }

end: 
     *ppsz = psz;
     if(ppszExpected)
         *ppszExpected = pszExpected;
     
     if(pContext && pszStart!=psz && !pszExpected)
        pContext->AddressScanHelp(pContext, pszStart, (psz-pszStart)/sizeof(TCHAR), NULL);
     
     return pszExpected==NULL;
}







bool CGT_IsFullBasicAddress(const TCHAR* pszAddr)
{
    //no '*' and [IQM]
    if(pszAddr && pszAddr[0]==_T('%'))
    {
        pszAddr = _tcsinc(pszAddr);

        switch(*pszAddr)
        {
        case _T('I'):
        case _T('Q'):
        case _T('M'):
        break;
        default:
            return false;
        }

        while(_istalpha(*pszAddr))
            pszAddr = _tcsinc(pszAddr);
            
        return *pszAddr != _T('*');
    }
    else
    {
        assert(!"ST_IsFullBasicAddress: bad addr syntax");
        return false;
    }
}

int CGT_GetAddrChannelNumber(const TCHAR* pszAddr)
{
    int          masterNum;
    const TCHAR* psz = pszAddr;
    

    if(*psz!=_T('%'))
        return -1;

    psz = _tcsinc(psz);

    if(*psz!=_T('I') && *psz!=_T('Q') && *psz!=_T('M'))
        return -1;

    psz = _tcsinc(psz);

    if(*psz==_T('X') || *psz==_T('B') || *psz==_T('W') || 
       *psz==_T('D') || *psz==_T('L') || *psz==_T('S'))
        psz = _tcsinc(psz);

    if(!_istdigit(*psz))
        return -1;

    masterNum = _ttoi(psz);
    return masterNum;
}








bool CGT_IsIncompleteAddressDef(const TCHAR* pszAddr)
{
    //e.g. %17.1 or %2 
    //or a var config reference %IB* or %*, are also 
    //possible incomplete definitions.
    //or special thing %*.12.4, this is a definition that uses 
    //another definition.
    //if the second char is not a I,Q or M (or just not a letter)
    //it must be an incomplete address definition
    if(pszAddr && pszAddr[0]==_T('%') && pszAddr[1])
    {
        return !_istalpha(pszAddr[1]);
    }
    else
    {
        assert(!"CGT_IsIncompleteAddressDef: bad addr syntax");
        return false;
    }
}
bool CGT_IsIncompleteAddressRef(const TCHAR* pszAddr)
{
    //e.g. %IX*+17.1 or %IX*.17.1 or %*10.3
    //if pszAddr contains a '*' not followed ba  '.' or '+' 
    //it must be an incomplete address reference
    //(If the '*' is end of address, it's a var config ref, see next function) 
    if(pszAddr && pszAddr[0]==_T('%') && pszAddr[1])
    {
        pszAddr = _tcsinc(pszAddr);

        while(_istalpha(*pszAddr))
            pszAddr = _tcsinc(pszAddr);
        
        if(*pszAddr==_T('*'))
        {
            pszAddr = _tcsinc(pszAddr);

            if(*pszAddr==_T('.') || *pszAddr==_T('+') || *pszAddr==_T('!'))
                return true;
            else if(*pszAddr==0)
                return false;
        }
        else
        {
            return false;
        }

    }

    assert(!"ST_IsIncompleteAddressRef: bad addr syntax");
    return false;
}
bool CGT_IsVarConfigAddressRef(const TCHAR* pszAddr)
{
    //If the '*' is end of address, it's a var config ref 
    if(pszAddr && pszAddr[0]==_T('%') && pszAddr[1])
    {
        pszAddr = _tcsinc(pszAddr);

        while(_istalpha(*pszAddr))
            pszAddr = _tcsinc(pszAddr);
        
        if(*pszAddr==_T('*'))
        {
            pszAddr = _tcsinc(pszAddr);

            if(*pszAddr==_T('.') || *pszAddr==_T('+') || *pszAddr==_T('!'))
                return false;
            else if(*pszAddr==0)
                return true;
        }
        else
        {
            return false;
        }

    }

    assert(!"ST_IsVarConfigAddressRef: bad addr syntax");
    return false;
}


TCHAR CGT_GetAddrLocation(const TCHAR* pszAddr)
{
    if(pszAddr && pszAddr[0]==_T('%'))
    {
        switch(pszAddr[1])
        {
        case _T('I'): case _T('i'): return _T('I');
        case _T('Q'): case _T('q'): return _T('Q');
        case _T('M'): case _T('m'): return _T('M');
        }
    }
    return 0;
}

bool CGT_IsByteSwapAddress(const TCHAR* pszAddr)
{
    const TCHAR* pszAttr;

    if(pszAddr && pszAddr[0]==_T('%') && pszAddr[1])
    {
        pszAttr = pszAddr;
        while((pszAttr = _tcschr(pszAttr, _T('!')))!= NULL)
        {
            if(_tcschr(pszAddr, _T('"')))
                return false;//HACK ??????????????? OPC address????

            //skip !:
            pszAttr = _tcsinc(pszAttr);

            if(!_tcsnicmp(pszAttr, _T("swap"), _tcslen("swap")))
                return true;
        }
    }
    else
    {
        assert(!"ST_IsByteSwapAddress: bad addr syntax");
    }
    
    return false;
}

int CGT_GetAddrBitSize(const TCHAR* pszAddr)
{
    if(pszAddr && pszAddr[0]==_T('%') && _istalpha(pszAddr[1]))
    {
        switch(pszAddr[2]) 
        {
        default:
        case _T('X'): return 1;
        case _T('B'): return 8;
        case _T('W'): return 16;
        case _T('D'): return 32;
        case _T('L'): return 64;
        case _T('S'): return -1; // don't ask bitsize for string vars
        }
    }

    assert(!"ST_GetDirectAddrBitSize: bad addr syntax");
    return 0;
}

