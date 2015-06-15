/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtools.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmtools.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtools.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

static char szversioninfo[] = "$Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtools.cpp 1     28.02.07 19:04 Ln $";

#include <tchar.h>
#include <direct.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>


#include "fc_todebug\fc_assert.h"
#include "fc_todebug\fc_mem.h"
#include "fc_tools\fc_string.h"
#include "fc_tools\fc_fileIO.h"

#include "globalContext.h"
#include "asmtools.h"
#include "asmerr.h"



void ST_GArray_Alloc(void** ppArray, int* pnCurrent, int nWanted, size_t nElemSizeBytes) 
{
    int    iOk;
    size_t nBytes;

    assert(nWanted>0);
    if(nWanted>*pnCurrent)
    {
        *pnCurrent = nWanted;
        nBytes = *pnCurrent * nElemSizeBytes;
    
        if(*ppArray)
            iOk = FC_ResizeMemory(ppArray, nBytes);
        else
            iOk = FC_NewMemory(ppArray, nBytes);
    }
}


void ST_GArray_Resize(void** ppArray, int* pnCurrent, int nWanted, size_t nElemSizeBytes) 
{
    int    iOk;
    size_t nBytes;
    int    inc;

    assert(nWanted>*pnCurrent);

    if(*pnCurrent<=4)
    {
        inc = 0;
    }
    else if(*pnCurrent<=8)
    {
        inc = 2;
    }
    else if(*pnCurrent<=16)
    {
        inc = 4;
    }
    else if(*pnCurrent<=32)
    {
        inc = 8;
    }
    else if(*pnCurrent<=64)
    {
        inc = 16;
    }
    else if(*pnCurrent<=128)
    {
        inc = 32;
    }
    else
    {
        inc = 64;
    }

    *pnCurrent = nWanted + inc;
    
    assert(*pnCurrent>0);
    nBytes = *pnCurrent * nElemSizeBytes;
    
    if(*ppArray)
        iOk = FC_ResizeMemory(ppArray, nBytes);
    else
        iOk = FC_NewMemory(ppArray, nBytes);
}





static TCHAR szEmptyString[1] = _T("");
ST_String_C::ST_String_C()
{
    szString = szEmptyString;
}
void ST_String_C::load(const TCHAR *sz)
{
    if(szEmptyString != szString)
        delete(szString);

    if(sz) 
    {
        szString = new TCHAR[_tcslen(sz)+1];
        _tcscpy(szString, sz);
    }
    else
    {
        szString = szEmptyString;
    }
}
#ifndef UNICODE
void ST_String_C::load(const wchar_t *sz)
{
    TCHAR* pszBuffer;
    int    iLen = wcslen(sz);

    if(iLen==0)
    {
        pszBuffer = _T("");
    }
    else
    {
        iLen = iLen*3+1;
        pszBuffer = (TCHAR*)_alloca(iLen);
        FC_StringToMultiByte(sz, pszBuffer, iLen);
    }
    load(pszBuffer);
}
#endif

void ST_String_C::loadLower(const TCHAR* sz)
{
    load(sz);
    _tcslwr(szString);
}
ST_String_C& ST_String_C::operator<<(const TCHAR* psz)
{
    int    newLen = _tcslen(szString) + _tcslen(psz) + 1;
    TCHAR* pszNew = new TCHAR[newLen];

    _tcscpy(pszNew, szString);
    _tcscat(pszNew, psz);

    if(szEmptyString != szString)
        delete(szString);

    szString = pszNew;
    return *this;
}
ST_String_C& ST_String_C::operator<<(long l)
{
    TCHAR buffer[256];
    return *this<<ltoa(l,buffer,10);
}

ST_String_C& ST_String_C::operator<<(unsigned long ul)
{
    TCHAR buffer[256];
    return *this<<_ultot(ul,buffer,10);
}
ST_String_C& ST_String_C::operator<<(__int64 i64)
{
    TCHAR buffer[64];
    return *this<<_i64tot(i64,buffer,10);
}
ST_String_C& ST_String_C::operator<<(double dbl)
{
    TCHAR buffer[256];
    return *this<<_gcvt(dbl, ASM_MAX_LREAL_DIGITS, buffer);
}
ST_String_C& ST_String_C::operator<<(float flt)
{
    TCHAR buffer[256];
    return *this<<_gcvt((double)flt, ASM_MAX_REAL_DIGITS, buffer);
}

ST_String_C::ST_String_C(const ST_String_C &that)
{
    szString = new TCHAR[_tcslen(that.szString)+1];
    _tcscpy(szString, that.szString);
}
ST_String_C::~ST_String_C()
{
    if(szString != szEmptyString)
      delete []szString;
}


const TCHAR* Tostr_C::loadResource(unsigned int uiResNum)
{
    int iRet;
    
    iRet = LoadString(NULL, uiResNum, buffer, FC_ARRAY_LEN(buffer));
    assert(iRet < FC_ARRAY_LEN(buffer));
    if(iRet<=0)
        return _T("<4CCG: string resource not found>");

    return buffer;
}
const TCHAR* Tostr_C::loadResource2(const TCHAR* pre, unsigned int uiResNum, const TCHAR* post)
{
    int iRet;
    int lenPre = _tcslen(pre);
    int lenPost = _tcslen(post);

	if(lenPre+lenPost < FC_ARRAY_LEN(buffer))
	{
		_tcscpy(buffer, pre);
    
		iRet = LoadString(NULL, uiResNum, buffer+lenPre, FC_ARRAY_LEN(buffer)-lenPre);
		assert(iRet+lenPre < FC_ARRAY_LEN(buffer));
		if(iRet<=0)
			return _T("<4CCG: string resource not found>");

		if(_tcslen(buffer)+lenPost < FC_ARRAY_LEN(buffer))
		{
			_tcscat(buffer, post);
		}
		else
		{
			assert(!"internal buffer overflow");
		}
	}
	else
	{
		assert(!"internal buffer overflow");
	}

    return buffer;
}

void Tostr_C::loadUpper(const TCHAR* pstr)
{
    _tcscpy(buffer, pstr);
    _tcsupr(buffer);
}

void Tostr_C::loadLower(const TCHAR* pstr)
{
    _tcscpy(buffer, pstr);
    _tcslwr(buffer);
}


#define ST_MAX_FILE_SIZE  0x10000000

bool LoadFileToMem(const TCHAR*   pszFileName, 
                   TCHAR**        ppCharBuffer, 
                   IStream**      pIStream,
                   unsigned long* pulLength, 
                   __int64*       pftLastWrite)
{
    DWORD                       nBytes;
    HANDLE                      hFile;
    DWORD                       dwSize;
    BYTE*                       pFileContent;
    BY_HANDLE_FILE_INFORMATION  fileInfo;
    HGLOBAL                     memHandle;

    //printf("load->%s\r\n",  (const char*)pszFileName);


    if(pIStream)
        *pIStream = NULL;
    if(ppCharBuffer)
        *ppCharBuffer = NULL;
    if(pulLength)
        *pulLength = 0;


    hFile  = CreateFile(pszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
                        OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    
    if(hFile==INVALID_HANDLE_VALUE || !GetFileInformationByHandle(hFile, &fileInfo))
    {
        ST_DiagMsg1(NULL, E_4CCG_ERR_FILE_OPEN, pszFileName);
        
        if(hFile!=INVALID_HANDLE_VALUE)
            CloseHandle(hFile);

        return false;
    }
    
    dwSize = fileInfo.nFileSizeLow;

    if(pulLength)
        *pulLength = dwSize;
    
    if(pftLastWrite)
    {
        LARGE_INTEGER  lint;
        lint.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
        lint.HighPart= fileInfo.ftLastWriteTime.dwHighDateTime;
        *pftLastWrite = lint.QuadPart;
    }

    if(!pIStream && !ppCharBuffer)
    {
        CloseHandle(hFile);
        return true;
    }

    
    if (fileInfo.nFileSizeHigh!=0 || dwSize>=ST_MAX_FILE_SIZE)
    {
        CloseHandle(hFile);
        ST_DiagMsg1(NULL, E_4CCG_ERR_FILE_TO_LARGE, pszFileName);
        return false;
    }



    pFileContent = NULL;
    if(ppCharBuffer)
    {
        pFileContent = new BYTE[dwSize+1];//+1 we will 0 terminate anyway
    }
    else if(pIStream)
    {
        memHandle = GlobalAlloc(GMEM_MOVEABLE, dwSize);
        pFileContent = (BYTE*)GlobalLock(memHandle);
    }


    if(pFileContent)
    {
        //UNICODE Files ????????????????????
        if(!ReadFile(hFile,pFileContent,dwSize,&nBytes,NULL) || nBytes!=dwSize)
        {
            CloseHandle(hFile);

            ST_DiagMsg1(NULL, E_4CCG_ERR_FILE_READ_ERROR, pszFileName);
            if(ppCharBuffer)
            {
                delete(pFileContent);
            }
            else if(pIStream)
            {
                GlobalUnlock(memHandle);
                memHandle = GlobalFree(memHandle);
                assert(memHandle==NULL);
            }
            return false;
        }


        if(ppCharBuffer)
        {
            pFileContent[dwSize] = 0;
            *ppCharBuffer = (TCHAR*)pFileContent;//UNICODE ????????????????????????ß
        }
        else if(pIStream)
        {
            GlobalUnlock(memHandle);

            HRESULT hr = CreateStreamOnHGlobal(
                        memHandle,   //Memory handle for the stream object
                        true,        //Whether to free memory when the object is released
                        pIStream);   //Address of output variable that 
                                     // receives the IStream interface pointer
            if(SUCCEEDED(hr) && *pIStream)
            {
                ULARGE_INTEGER uli;
                LARGE_INTEGER  u0={0,0};

                uli.LowPart = fileInfo.nFileSizeLow;
                uli.HighPart = fileInfo.nFileSizeHigh;

                if(SUCCEEDED(hr = (*pIStream)->SetSize(uli)))
                    hr = (*pIStream)->Seek(u0, STREAM_SEEK_SET, NULL);

                if(FAILED(hr))
                {
                    (*pIStream)->Release();
                    *pIStream = NULL;
                }
            }
            assert(SUCCEEDED(hr));
            if(FAILED(hr))
            {
                ST_DiagMsg1(NULL, E_4CCG_ERR_FILE_READ_ERROR, pszFileName);
                *pIStream = NULL;
                CloseHandle(hFile);
                return false;
            }
        }
    }

    CloseHandle(hFile);

    return true;
}






static unsigned long ST_GetHash(const TCHAR* pszStr)
{
    register unsigned long nHash = 0;
	
    for(;*pszStr;pszStr++)
		nHash = (nHash<<5) + nHash + *pszStr;

	return nHash;
}
static unsigned long ST_GetIHash(const TCHAR* pszStr)
{
    register unsigned long nHash = 0;
	int                    iLen = _tcslen(pszStr)+1;
    TCHAR                  buffer[256];
    TCHAR*                 pszLower = buffer; 
    TCHAR*                 psz; 
    

    if(iLen>=FC_ARRAY_LEN(buffer) && !FC_NewMemory((void**)&pszLower, iLen))
    {
        assert(!"out of memory");
        return 0;
    }
        
    _tcscpy(pszLower, pszStr); 
    _tcslwr(pszLower);

    for(psz=pszLower;*psz;psz++)
		nHash = (nHash<<5) + nHash + *psz;

	if(pszLower != buffer)
        FC_FreeMemory(pszLower);
    
    return nHash;
}

struct TableEntry 
{
	unsigned long m_hash;
	void*         m_pValue;
	TCHAR*        m_pszKey;
	TableEntry*   m_pNext;
};
struct ST_HashTable
{
	TableEntry** m_pTable;
    bool         m_bIgnoreCase;
    bool         m_bAllocateKey;
    long         m_length;
	long         m_count;
	long         m_threshold;
};

ST_HashTable* ST_CreateHashTable(bool bIgnoreCase, bool bAllocateKey, long initialCapacity)
{
    ST_HashTable*   pHT;

    if(initialCapacity<=0)
		initialCapacity =  10;
	
    if(FC_NewMemory((void**)&pHT, sizeof(ST_HashTable)) && 
       FC_NewMemory((void**)&pHT->m_pTable, sizeof(TableEntry*)*initialCapacity))
    {
	    pHT->m_bIgnoreCase  = bIgnoreCase;
        pHT->m_bAllocateKey = bAllocateKey;
        pHT->m_count        = 0;
	    pHT->m_length       = initialCapacity;
	    pHT->m_threshold    = (long)(initialCapacity * 0.75f);
	    
        memset(pHT->m_pTable, 0, sizeof(TableEntry*)*initialCapacity);
    }
    else
    {
        assert(!"out of memory");
    }
    return pHT;
}
void ST_ClearHashTable(ST_HashTable* pHT)
{
	long        index;
    TableEntry* pe;
    TableEntry* pe2;

    if(pHT==NULL)
    {
        assert(!"ST_ClearHashTable: invalid arguments");
        return;
    }

    for(index = pHT->m_length; --index >= 0; )
	{
		for(pe = pHT->m_pTable[index] ; pe ; )
		{
			pe2= pe;
			pe = pe->m_pNext;
            
            if(pHT->m_bAllocateKey)
                FC_FreeMemory(pe2->m_pszKey);

			FC_FreeMemory(pe2);
		}
		pHT->m_pTable[index] = NULL;
	}
	
    pHT->m_count = 0;
}
void ST_DeleteHashTable(ST_HashTable* pHT)
{
    if(pHT==NULL)
    {
        assert(!"ST_DeleteHashTable: invalid arguments");
        return;
    }
    
    ST_ClearHashTable(pHT);
    FC_FreeMemory(pHT->m_pTable);
    FC_FreeMemory(pHT);
}
void* ST_GetHashValue(const ST_HashTable* pHT, const TCHAR* pszKey)
{
	TableEntry*   pe;
    unsigned long hash;
	unsigned long index;
	

    if(pHT==NULL || pszKey==NULL)
    {
        assert(!"ST_GetHashValue: invalid arguments");
        return NULL;
    }
    
    hash = pHT->m_bIgnoreCase ? ST_GetIHash(pszKey) : ST_GetHash(pszKey);
    index = hash % pHT->m_length;

    for (pe = pHT->m_pTable[index] ; pe; pe = pe->m_pNext) 
	{
		if(pe->m_hash==hash)
        {
            if(pHT->m_bIgnoreCase)
            {
                if(!_tcsicmp(pe->m_pszKey, pszKey))
        			return pe->m_pValue;
            }
            else
            {
                if(!_tcscmp(pe->m_pszKey, pszKey))
        			return pe->m_pValue;
            }
        } 
	}
	
    return NULL;
}
static bool ST_RehashTable(ST_HashTable* pHT) 
{
	long         i;
    long         index;
    TableEntry*  pe;
    long	     oldCapacity = pHT->m_length;
	TableEntry** pOldTable   = pHT->m_pTable;
	long	     newCapacity = oldCapacity * 2 + 1;
    TableEntry** pNewTable;
    TableEntry*  pOld;	


    if(!FC_NewMemory((void**)&pNewTable, sizeof(TableEntry*)*newCapacity))
    {
        assert(!"ST_RehashTable: out of memory");
        return false;
    }

	memset(pNewTable, 0, sizeof(TableEntry*)*newCapacity);


	pHT->m_threshold = (long)(newCapacity * 0.75f);
	pHT->m_pTable	 = pNewTable;
	pHT->m_length    = newCapacity;

	for(i = oldCapacity ; i-- > 0 ;) 
	{
		for(pOld = pOldTable[i] ; pOld ; ) 
		{
			pe = pOld;
			pOld = pOld->m_pNext;

			index = pe->m_hash % newCapacity;
			pe->m_pNext = pNewTable[index];
			pNewTable[index] = pe;
		}
	}

    FC_FreeMemory(pOldTable);

    return true;
}
const TCHAR* ST_PutHashValue(ST_HashTable* pHT, 
                             const TCHAR*  pszKey, 
                             void*         pValue, 
                             bool          bReplace, 
                             void**        ppPreviousValue)
{
	unsigned long hash;
    unsigned long index;
    TableEntry*   pe;


    if(ppPreviousValue)
        *ppPreviousValue = NULL;

    if(pHT==NULL || pszKey==NULL)
    {
        assert(!"ST_PutHashValue: invalid arguments");
        return false;
    }


	// Makes sure the key is not already in the hashtable.
    hash = pHT->m_bIgnoreCase ? ST_GetIHash(pszKey) : ST_GetHash(pszKey);
    index = hash % pHT->m_length;
	
    for(pe = pHT->m_pTable[index]; pe ; pe = pe->m_pNext) 
	{
		if(pe->m_hash==hash)
        {
            if(pHT->m_bIgnoreCase)
            {
                if(!_tcsicmp(pe->m_pszKey, pszKey))
                    break;
            }
            else
            {
                if(!_tcscmp(pe->m_pszKey, pszKey))
                    break;
            }
        } 
	}

    if(pe)
    {
        if(ppPreviousValue)
            *ppPreviousValue = pe->m_pValue;

        if(bReplace)
            pe->m_pValue = pValue;

        return pe->m_pszKey;
    }


	if(pHT->m_count >= pHT->m_threshold) 
	{
		// Rehash the table if the threshold is exceeded
        if(!ST_RehashTable(pHT))
            return NULL;
		return ST_PutHashValue(pHT, pszKey, pValue, bReplace, ppPreviousValue);
	} 

	//Creates the new entry:
    if(FC_NewMemory((void**)&pe, sizeof(TableEntry)))
    {
	    pe->m_hash   = hash;
	    pe->m_pValue = pValue;
        
        if(!pHT->m_bAllocateKey)
        {
            pe->m_pszKey = (TCHAR*)pszKey;
        }
        else if(FC_NewMemory((void**)&pe->m_pszKey, sizeof(TCHAR)*(_tcslen(pszKey)+1)))
        {
            _tcscpy(pe->m_pszKey, pszKey);
        }
        else
        {
            assert(!"out of memory");
            return false;
        }
        
	    pe->m_pNext  = pHT->m_pTable[index];
	    pHT->m_pTable[index] = pe;
	    pHT->m_count++;
    }
    else
    {
        assert(!"out of memory");
        return NULL;
    }

    return pe->m_pszKey;
}





ST_IdentBag::ST_IdentBag(bool bIgnoreCase)
{
    m_pHash = ST_CreateHashTable(bIgnoreCase, true, 1000);
}
ST_IdentBag::~ST_IdentBag()
{
    ST_DeleteHashTable(m_pHash);
}
ST_IDENT_TYP ST_IdentBag::insert(const TCHAR* psz)
{
    ST_IDENT_TYP ret; 
    
    ret.psz = ST_PutHashValue(m_pHash, psz, (void*)-1, false, NULL);
    return ret;
}

const TCHAR* ST_IdentBag::insertString(const TCHAR* psz)
{
    const TCHAR* csz = ST_PutHashValue(m_pHash, psz, (void*)-1, false, NULL);
    return csz;
}






///////////////////////////////////////////////////////////////////////////////
//
// .map file stuff
//

static FILE* MapFile = NULL;

void TO_OpenMapFile(LPCTSTR pFileName)
{
    MapFile = fopen(pFileName, "w");
    TO_MAPWRITE0(_T(".begin map file\n"));
}

void TO_CloseMapFile()
{
    if (MapFile)
    {
        TO_MAPWRITE0(_T("\n.end map file\n"));
        fclose(MapFile);
    }
}

void TO_MapWriteA(const char*  lpszFormat, ...)
{
    va_list ap;

    if (MapFile == NULL)
        return;
    
    va_start(ap, lpszFormat);
    vfprintf(MapFile, lpszFormat, ap);
    va_end(ap);
}

void TO_MapWriteW(const wchar_t* lpszFormat, ...)
{
    va_list ap;
    
    if (MapFile == NULL)
        return;

    va_start(ap, lpszFormat);
    vfwprintf(MapFile, lpszFormat, ap);
    va_end(ap);
}

///////////////////////////////////////////////////
// LITTLE - / BIG - ENDIAN

void TRANSLATE_DOUBLE(double* x, double* y)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        SwapDouble(x,y);
    else
        *y = *x;
}

void TRANSLATE_FLOAT(float* x, float* y)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        SwapFloat(x,y);
    else
        *y = *x;
}

__int64 TRANSLATE_I64(__int64 x)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        return SwapI64(x);
    else
        return x;
}

long TRANSLATE_LONG(long x)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        return SwapLong(x);
    else
        return x;
}

DWORD TRANSLATE_DWORD(DWORD x)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        return (DWORD) SwapLong((DWORD) x);
    else
        return x;
}

short TRANSLATE_SHORT(short x)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        return SwapShort(x);
    else
        return x;
}

WORD TRANSLATE_WORD(WORD x)
{
    if (GetGC()->getTrgAttr()->trg_bigEndian)
        return (WORD) SwapShort((WORD) x);
    else
        return x;
}


void SwapDouble(double *val, double *result)
{
  BYTE *pResult = (BYTE*)result;
  BYTE *pVal = (BYTE*)val;
  pResult[0] = pVal[7];
  pResult[1] = pVal[6];
  pResult[2] = pVal[5];
  pResult[3] = pVal[4];
  pResult[4] = pVal[3];
  pResult[5] = pVal[2];
  pResult[6] = pVal[1];
  pResult[7] = pVal[0];
  return;
}

void SwapFloat(float *val, float *result)
{
  BYTE *pResult = (BYTE*)result;
  BYTE *pVal = (BYTE*)val;
  pResult[0] = pVal[3];
  pResult[1] = pVal[2];
  pResult[2] = pVal[1];
  pResult[3] = pVal[0];
  return;
}

__int64 SwapI64(__int64 val)
{
  __int64 result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[7];
  pResult[1] = pVal[6];
  pResult[2] = pVal[5];
  pResult[3] = pVal[4];
  pResult[4] = pVal[3];
  pResult[5] = pVal[2];
  pResult[6] = pVal[1];
  pResult[7] = pVal[0];
  return result;
}

long SwapLong(long val)
{
  long result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[3];
  pResult[1] = pVal[2];
  pResult[2] = pVal[1];
  pResult[3] = pVal[0];
  return result;
}

short SwapShort(short val)
{
  short result;
  BYTE *pResult = (BYTE*)&result;
  BYTE *pVal = (BYTE*)&val;
  pResult[0] = pVal[1];
  pResult[1] = pVal[0];
  return result;
}

/* ---------------------------------------------------------------------------- */
