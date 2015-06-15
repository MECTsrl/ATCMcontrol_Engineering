#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "baselib.h"
#include "fc_tools\fc_string.h"

static int BL_IsDigit(TCHAR c, int base)
{
    int i;

    switch(c)
    {
    case _T('0'): i=0; break;
    case _T('1'): i=1; break;
    case _T('2'): i=2; break;
    case _T('3'): i=3; break;
    case _T('4'): i=4; break;
    case _T('5'): i=5; break;
    case _T('6'): i=6; break;
    case _T('7'): i=7; break;
    case _T('8'): i=8; break;
    case _T('9'): i=9; break;
    case _T('a'): case _T('A'): i=10; break;
    case _T('b'): case _T('B'): i=11; break;
    case _T('c'): case _T('C'): i=12; break;
    case _T('d'): case _T('D'): i=13; break;
    case _T('e'): case _T('E'): i=14; break;
    case _T('f'): case _T('F'): i=15; break;
    default:
        return -1;
    }

    return i>=base ? -1 : i;
}

HRESULT BL_IECWStringToBinary(
	const TCHAR*    pszIECStr, 
	TCHAR*          pBinBuffer,
	long*           pBufferLen,
    BL_IECStringCB  pCB,
	void*           pCtx
)
{
    const TCHAR wstrDelim = _T('"');
	const TCHAR* pszStr   = pszIECStr;
    long  nOutChars       = 0;

    //strip leading white space:
    while(_istascii(*pszStr) && _istspace(*pszStr))
        pszStr++;

	if(*pszStr != wstrDelim)
	{
		assert(!_T("invalid delimiter"));
		return E_BL_ERR_INVALID_IEC_STR;
	}
	//skip '"' delimiter
    pszStr++;

    for( ;*pszStr; )
    {
        if(*pszStr == wstrDelim)
            break;//normal exit

        if(*pszStr==_T('\n') && pCB)
			pCB(pCtx, pszStr-pszIECStr, 0, 0, 1);
        
        if(*pszStr!=_T('$'))
        {
            #ifndef UNICODE
            if (IsDBCSLeadByte(*pszStr) && pszStr[1])
			{
                if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=*pszStr;
				nOutChars++;
				pszStr++;
			}
            #endif
            if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=*pszStr;
			nOutChars++;
			pszStr++;
            continue;
        }

        //skip $ escape
        pszStr++;

        switch(*pszStr)
        {
        case _T('\''): 
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('\''); 
			nOutChars++; pszStr++; break;
        case _T('"') : 
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('"');  
			nOutChars++; pszStr++; break;
        case _T('$'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('$');  
			nOutChars++; pszStr++; break;
        case _T('L'):   
        case _T('l'):
        case _T('N'):
        case _T('n'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('\n'); 
			nOutChars++; pszStr++; break;
        case _T('P'):
        case _T('p'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('\f'); 
			nOutChars++; pszStr++; break;
        case _T('R'):
        case _T('r'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('\r'); 
			nOutChars++; pszStr++; break;
        case _T('T'):
        case _T('t'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('\t'); 
			nOutChars++; pszStr++; break;
        default:
            if(_istascii(pszStr[0]) && _istascii(pszStr[1]) && _istascii(pszStr[2]) && _istascii(pszStr[3]) &&
               _istxdigit(pszStr[0]) && _istxdigit(pszStr[1]) && _istxdigit(pszStr[2]) && _istxdigit(pszStr[3]))
            {
                int id;
                id = BL_IsDigit(pszStr[0], 16)*16*16*16 
                   + BL_IsDigit(pszStr[1], 16)*16*16
                   + BL_IsDigit(pszStr[2], 16)*16 
                   + BL_IsDigit(pszStr[3], 16);

                #ifdef UNICODE
				if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=(wchar_t)id; 
				nOutChars++;
                #else
                wchar_t tmp1[2];
                char tmp2[3];
                tmp1[0] = (wchar_t)id;
                tmp1[1] = 0;
                tmp2[0] = tmp2[1] = tmp2[2] = 0;
                FC_StringToMultiByte(tmp1, tmp2, 3);
               
                FC_StringToWideChar(tmp2,tmp1,2);
                if(tmp1[0] != (wchar_t)id && pCB)
					pCB(pCtx, pszStr-pszIECStr, 0, id, 0);
                
                if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=tmp2[0];
				nOutChars++;
                if (tmp2[0] && tmp2[1])
				{
                  if(nOutChars<*pBufferLen) pBinBuffer[nOutChars] = tmp2[1];
				  nOutChars++;
				}
                #endif
                pszStr+=4;
                break;
            }
            else if(pCB)
				pCB(pCtx, pszStr-pszIECStr, 1, 0, 0);
        }
    }
	if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=_T('\0');  // put \0 only enough space don't add it to nOutChars

	if(*pszStr != wstrDelim)
	{
		assert(!_T("invalid delimiter"));
		return E_BL_ERR_INVALID_IEC_STR;
	}
    pszStr++;
   
    //strip trailing white space:
    while(_istascii(*pszStr) && _istspace(*pszStr))
        pszStr++;
	if(*pszStr != _T('\0'))
	{
		assert(!_T("invalid string terminator"));
		return E_BL_ERR_INVALID_IEC_STR;
	}

	if(nOutChars >= *pBufferLen)
	{
		*pBufferLen = nOutChars;
		return E_BL_ERR_BUFFER_TO_SMALL;
	}

	*pBufferLen = nOutChars;
	return S_OK;
}


HRESULT BL_IECStringToBinary(
	const TCHAR*    pszIECStr, 
	char*           pBinBuffer,
	long*           pBufferLen,
    BL_IECStringCB  pCB,
	void*           pCtx
)
{
    const TCHAR wstrDelim = _T('\'');
	const TCHAR* pszStr   = pszIECStr;
    long  nOutChars       = 0;

    //strip leading white space:
    while(_istascii(*pszStr) && _istspace(*pszStr))
        pszStr++;

	if(*pszStr != wstrDelim)
	{
		assert(!_T("invalid delimiter"));
		return E_BL_ERR_INVALID_IEC_STR;
	}
	//skip '"' delimiter
    pszStr++;

    for( ;*pszStr; )
    {	
        if(*pszStr == wstrDelim)
            break;//normal exit

        if(*pszStr==_T('\n') && pCB)
			pCB(pCtx, pszStr-pszIECStr, 0, 0, 1);
        
        if(*pszStr!=_T('$'))
        {
			#ifdef UNICODE           
			wchar_t tmp1[2];
            char tmp2[3];
            tmp1[0] = *pszStr;
            tmp1[1] = 0;
            tmp2[0] = tmp2[1] = tmp2[2] = 0;
            FC_StringToMultiByte(tmp1, tmp2, 3);
           
            FC_StringToWideChar(tmp2, tmp1, 2);
            if(tmp1[0] != *pszStr && pCB)
				pCB(pCtx, pszStr-pszIECStr, 0, *pszStr, 0);
            
            if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=tmp2[0];
			nOutChars++;
            if (tmp2[0] && tmp2[1])
			{
				assert(!_T("invalid non ASCII character"));
				return E_BL_ERR_INVALID_IEC_STR;
			}
			#else
            if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=(char)*pszStr;
			nOutChars++;
			pszStr++;
			#endif
            continue;
        }

        //skip $ escape
        pszStr++;

        switch(*pszStr)
        {
        case _T('\''): 
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='\''; 
			nOutChars++; pszStr++; break;
        case _T('"') : 
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='"';  
			nOutChars++; pszStr++; break;
        case _T('$'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='$';  
			nOutChars++; pszStr++; break;
        case _T('L'):   
        case _T('l'):
        case _T('N'):
        case _T('n'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='\n'; 
			nOutChars++; pszStr++; break;
        case _T('P'):
        case _T('p'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='\f'; 
			nOutChars++; pszStr++; break;
        case _T('R'):
        case _T('r'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='\r'; 
			nOutChars++; pszStr++; break;
        case _T('T'):
        case _T('t'):  
			if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='\t'; 
			nOutChars++; pszStr++; break;
        default:
            if(_istascii(pszStr[0]) && _istascii(pszStr[1]) && _istxdigit(pszStr[0]) && _istxdigit(pszStr[1]))
            {
                int id;
                id = BL_IsDigit(pszStr[0], 16)*16 
                   + BL_IsDigit(pszStr[1], 16);
				if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]=(char)id; 
				nOutChars++;
                pszStr+=2;
                break;                    
            }
            else if(pCB)
				pCB(pCtx, pszStr-pszIECStr, 1, 0, 0);
        }
    }
	if(nOutChars<*pBufferLen) pBinBuffer[nOutChars]='\0';  // put \0 only enough space don't add it to nOutChars

	if(*pszStr != wstrDelim)
	{
		assert(!_T("invalid delimiter"));
		return E_BL_ERR_INVALID_IEC_STR;
	}
    pszStr++;
   
    //strip trailing white space:
    while(_istascii(*pszStr) && _istspace(*pszStr))
        pszStr++;
	if(*pszStr != _T('\0'))
	{
		assert(!_T("invalid string terminator"));
		return E_BL_ERR_INVALID_IEC_STR;
	}

	if(nOutChars >= *pBufferLen)
	{
		*pBufferLen = nOutChars;
		return E_BL_ERR_BUFFER_TO_SMALL;
	}

	*pBufferLen = nOutChars;
	return S_OK;
}

