/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtools.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asmtools.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asmtools.h $
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

#ifndef _ASMTOOLS_H_
#define _ASMTOOLS_H_


#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <objbase.h>

#include "fc_tools\fc_defines.h"
#include "fc_todebug\fc_assert.h"
#include "fc_todebug\fc_mem.h"
#include "fc_tools\fc_string.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_fileIO.h"


#include "asmerr.h"



#define ASM_MAX_LREAL_DIGITS    15
#define ASM_MAX_REAL_DIGITS     7


//GArray: generic dynamic array, can be used for all 
//types that have no constructor/destructor, GArray
//uses malloc/realloc and is faaast.
void ST_GArray_Alloc(void** ppArray, int* pnCurrent, int nWanted, size_t nElemSizeBytes);
void ST_GArray_Resize(void** ppArray, int* pnCurrent, int nWanted, size_t nElemSizeBytes);
template <class T> class GArray
{

    //NOTE:  GArray shall not be used if T has class type
    //or any construction /copy construction code.
    //GArray uses maaloc/free/realloc directry.
    //To get a compile time error in case some one tries to 
    //use GArray with a class the unionT type wraps a T
    //the C++ compiler will complain if T is not an aggregate type.
    union unionT
    {
        T aT;
    };

    unionT* m_pArray;
    int     m_maxAlloc;
    int     m_maxUsed;

public:
    GArray()
    {
        assert(sizeof(unionT)==sizeof(T));//<-otherwise append won't work
        m_maxAlloc= 0;
        m_maxUsed = 0;
        m_pArray  = NULL;
    }
    ~GArray()
    {
        if(m_pArray)
            FC_FreeMemory(m_pArray);
    }
    void set(int i, const T& val)
    {
        if(i>=m_maxAlloc)
            ST_GArray_Resize((void**)&m_pArray, &m_maxAlloc, i+1, sizeof(unionT));
        if(i>=m_maxUsed)
            m_maxUsed = i+1;

        assert(i>=0);
        m_pArray[i].aT = val;
    }
    //allocSize cannot shrink the array, use this function to avoid realloc if
    //you know the max size in advance
    void allocSize(int i)
    {
        ST_GArray_Alloc((void**)&m_pArray, &m_maxAlloc, i, sizeof(unionT));
    }
    const T& get(int i)const
    {
        assert(i>=0 && i<m_maxUsed);
        return m_pArray[i].aT;
    }
    T& get(int i)
    {
        assert(i>=0 && i<m_maxUsed);
        return m_pArray[i].aT;
    }
    T* getAddrOf(int i)
    {
        assert(i>=0 && i<m_maxUsed);
        return &m_pArray[i].aT;
    }
    void add(const T& val)
    {
        set(m_maxUsed, val);
    }
    int getMaxUsed()const
    {
        return m_maxUsed;
    }
    void resetMaxUsed()
    {
        m_maxUsed = 0;
    }
    void setMaxUsed(int i)
    {
        assert(i>=0 && i<=m_maxAlloc);
        m_maxUsed = i;
    }
    T* getElements()const
    {
        return (T*)m_pArray;
    }
    void copyto(GArray<T>* pNew)const
    {
        assert(pNew != this);
        pNew->m_maxUsed = m_maxUsed;
        if(m_maxUsed>0)
        {
            if(pNew->m_maxAlloc < m_maxUsed)
                pNew->allocSize(m_maxUsed);

            memcpy(pNew->m_pArray, this->m_pArray, m_maxUsed* sizeof(T));
        }
    }
    void append(const GArray<T>* pOther)
    {
        assert(pOther != this);
        if(pOther->m_maxUsed>0)
        {
            int ioldsize = m_maxUsed;
            m_maxUsed += pOther->m_maxUsed;
            if(m_maxAlloc < m_maxUsed)
                allocSize(m_maxUsed);

            memcpy(m_pArray+ioldsize, pOther->m_pArray, pOther->m_maxUsed* sizeof(T));
        }
    }
    void appendElems(const T* pElems, int nElems)
    {
        if(nElems>0)
        {
            int ioldsize = m_maxUsed;
            m_maxUsed += nElems;
            if(m_maxAlloc < m_maxUsed)
                allocSize(m_maxUsed);

            memcpy(m_pArray+ioldsize, pElems, nElems* sizeof(T));
        }
    }
};







class Tostr_C
{
    TCHAR buffer[512];//long enough for any floating point ???? HACK
public:
    Tostr_C(){buffer[0]=0;}
    Tostr_C(int l){ltoa(l,buffer,10);}
    Tostr_C(long l){ltoa(l,buffer,10);}
    Tostr_C(__int64 l64)
    {
        //CAUTION: the runtime function _i64tot does not work with negative input
        //handle negative input by hand!
        if(l64<0)
        {
            buffer[0]=_T('-');
            _ui64tot((unsigned __int64)-l64,buffer+1,10);
        }
        else
        {
            _ui64tot((unsigned __int64)l64,buffer,10);
        }
    }
    Tostr_C(unsigned long ul){ultoa(ul,buffer,10);}
    Tostr_C(const TCHAR* pszFmt, double d){ _stprintf(buffer, pszFmt, d);}

    operator const TCHAR *() const { return buffer; }
    void loadUpper(const TCHAR* pstr);
    void loadLower(const TCHAR* pstr);

    //getting a resource string:
    const TCHAR* loadResource(unsigned int uiResNum);
    //getting a resource string prepended by pre and appended by post 
	//usefull for e.g. "<resource>":
    const TCHAR* loadResource2(const TCHAR* pre, unsigned int uiResNum, const TCHAR* post);
};


//minimal string class:
class ST_String_C 
{ // helper class
  private:
      ST_String_C &operator = (const ST_String_C& that);  // operator "=" not supported

      TCHAR *szString;

  public:
    ST_String_C();
    ST_String_C(const char *sz)
    {
      szString =0;
      load(sz);
    }
    ST_String_C(const ST_String_C &that);
    void load(const TCHAR* sz);
    void removeLastChar()
    {
        int iLen = _tcslen(szString);
        if(iLen>0)
            szString[iLen-1] = 0;
    }

#ifndef UNICODE
    ST_String_C(const wchar_t *sz)
    {
      szString =0;
      load(sz);
    }
    void load(const wchar_t *sz);
#endif

    void loadLower(const TCHAR* sz);
    void toLowerCase(){ _tcslwr(szString); }
    ST_String_C& operator<<(const TCHAR* psz);
    ST_String_C& operator<<(long l);
    ST_String_C& operator<<(unsigned long ul);
    ST_String_C& operator<<(__int64 i64);
    ST_String_C& operator<<(double dbl);
    ST_String_C& operator<<(float flt);
    ST_String_C& operator<<(int i){*this<<(long)i; return *this;}


    ~ST_String_C();
    operator const char *() const { return szString; }
};


//get a file into memory or an IStream:
interface IStream;
bool LoadFileToMem(const TCHAR*   pszFileName, 
                   TCHAR**        ppCharBuffer, 
                   IStream**      pIStream,
                   unsigned long* pulLength, 
                   __int64*       pftLastWrite);




//Hash table string to pointer use a C Interface
struct ST_HashTable;
ST_HashTable* ST_CreateHashTable(bool bIgnoreCase, bool bAllocateKey, long initialCapacity);
void          ST_DeleteHashTable(ST_HashTable* pHT);
void          ST_ClearHashTable(ST_HashTable* pHT);

void*         ST_GetHashValue(const ST_HashTable* pHT, const TCHAR* pszKey);
const TCHAR*  ST_PutHashValue(ST_HashTable* pHT, 
                              const TCHAR*  pszKey, 
                              void*         pValue, 
                              bool          bReplace, 
                              void**        ppPreviousValue);



//Wrap ST_Hashtable functions:
template <class ValueType, int initalSize = -1> class ST_HashPointer
{
private:
    ST_HashTable* m_pHash;
public:
    ST_HashPointer(bool bAllocKey, bool bIgnoreCase=true)
    {
        m_pHash = ST_CreateHashTable(bIgnoreCase, bAllocKey, initalSize);
    }
    ~ST_HashPointer()
    {
        ST_DeleteHashTable(m_pHash);
    }

    ValueType* get(const TCHAR* pszKey)const
    {
        return (ValueType*)ST_GetHashValue(m_pHash, pszKey);
    }
    ValueType* put(ValueType* pValue, const TCHAR* pszKey, bool bReplace)
    {
        const TCHAR*  pszNewKey;
        ValueType*    pPrevious;
        
        pszNewKey = ST_PutHashValue(m_pHash, pszKey, (void*)pValue, bReplace, (void**)&pPrevious);
        return pPrevious;
    }
};



//ST_IdentBag: use if you want to store strings only once
//if used with ignore case=true, note that insert returns
//the first version of the string if any, which may be 
//different with respect of case.
struct ST_IDENT_TYP
{
    const TCHAR* psz;
};
struct ASM_INST_PATH_TYP
{
    int          depth;  //number of ids
    ST_IDENT_TYP ids;    //unique pointers to ST_IDENT_TYP.psz
};
inline bool ASM_IDENT_EQ(ST_IDENT_TYP a, ST_IDENT_TYP b)
{
    return a.psz==b.psz;
}
inline bool ASM_IsInstPathEq(const ASM_INST_PATH_TYP& p1, const ASM_INST_PATH_TYP& p2)
{
    return ASM_IDENT_EQ(p1.ids, p2.ids);
}




class ST_IdentBag
{
private:
    ST_HashTable* m_pHash;
public:
    ST_IdentBag(bool bIgnoreCase=false);
    ~ST_IdentBag();
    ST_IDENT_TYP insert(const TCHAR* psz);
    const TCHAR* insertString(const TCHAR* psz);
};




///////////////////////////////////////////////////////////////////////////////
//
// .map file stuff
//

extern FILE* MapFile;
void TO_OpenMapFile(LPCTSTR pFileName);
void TO_CloseMapFile();

#ifdef UNICODE
#define TO_MapWrite TO_MapWriteW
#else
#define TO_MapWrite TO_MapWriteA
#endif

void TO_MapWriteA(const char*  lpszFormat, ...);
void TO_MapWriteW(const wchar_t* lpszFormat, ...);

#define TO_MAPWRITE0(pFormat)                      TO_MapWrite(pFormat)
#define TO_MAPWRITE1(pFormat, p1)                  TO_MapWrite(pFormat, p1)
#define TO_MAPWRITE2(pFormat, p1, p2)              TO_MapWrite(pFormat, p1, p2)
#define TO_MAPWRITE3(pFormat, p1, p2, p3)          TO_MapWrite(pFormat, p1, p2, p3)
#define TO_MAPWRITE4(pFormat, p1, p2, p3, p4)      TO_MapWrite(pFormat, p1, p2, p3, p4)
#define TO_MAPWRITE5(pFormat, p1, p2, p3, p4, p5)  TO_MapWrite(pFormat, p1, p2, p3, p4, p5)
#define TO_MAPWRITE6(pFormat, p1, p2, p3,p4,p5,p6) TO_MapWrite(pFormat, p1, p2, p3, p4, p5, p6)
#define TO_MAPWRITE7(pFormat, p1, p2, p3,p4,p5,p6,p7) TO_MapWrite(pFormat, p1, p2, p3, p4, p5, p6, p7)
#define TO_MAPWRITE8(pFormat, p1, p2, p3,p4,p5,p6,p7,p8) TO_MapWrite(pFormat, p1, p2, p3, p4, p5, p6, p7, p8)


void SwapDouble(double *val, double *result);
void SwapFloat(float *val, float *result);
__int64 SwapI64(__int64);
long SwapLong(long);
short SwapShort(short);

void TRANSLATE_DOUBLE(double* x, double* y);
void TRANSLATE_FLOAT(float* x, float* y);
__int64 TRANSLATE_I64(__int64 x);
long TRANSLATE_LONG(long x);
DWORD TRANSLATE_DWORD(DWORD x);
short TRANSLATE_SHORT(short x);
WORD TRANSLATE_WORD(WORD x);

#endif

/* ---------------------------------------------------------------------------- */
