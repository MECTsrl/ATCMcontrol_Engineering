/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_cstring.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_cstring.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_cstring.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==String buffer class
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  09.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __FC_CSTRING_H_
#define __FC_CSTRING_H_

#include <tchar.h>  //define TCHAR 
#include <wtypes.h> //define BSTR and GUID

#include "fc_defines.h" 
#include "fc_string.h" 

#ifdef __cplusplus
extern "C"{
#endif
//Note: FC_StringDup[W] are not in fc_string.h to avoid mem allocation 
//and #include "fc_libmem.h" in fc_string.h, th fc_cstring.h needs that
//anyway.
//------------------------------------------------------------------*
/**
 * FC_StringDup[W] duplicate a TCHAR* using FC_LibNewMemory allocation.
 * Note:  using delete or free to free memory requires same 
 *        allocator as used for fc_libmem.h
 *                
 * @param           psz[w]  : string to duplicate
 * @return          pointer to duplicated string allocated
 *                  NULL if out of memory
 */
FC_TOAPI TCHAR* FC_TODECL  FC_StringDup(const TCHAR* psz);
#ifndef UNICODE
FC_TOAPI TCHAR* FC_TODECL  FC_StringDupW(const wchar_t* pszw);
#endif
/**
 * FC_StringDupLen[W] duplicate first nChars of a TCHAR* 
 * using FC_LibNewMemory allocation.
 * Note:  using delete or free to free memory requires same 
 *        allocator as used for fc_libmem.h
 *                
 * @param           psz[w] string to duplicate
 * @param           nChars: must be >=0, duplicated at max nChars
 *                  not conting the terminating 0-char.
 * @return          pointer to duplicated string allocated
 *                  NULL if out of memory
 */
FC_TOAPI TCHAR* FC_TODECL  FC_StringDupLen(const TCHAR* psz, int nChars);
#ifndef UNICODE
FC_TOAPI TCHAR* FC_TODECL  FC_StringDupLenW(const wchar_t* pszw, int nChars);
#endif

//------------------------------------------------------------------*
/**
 * FC_StringFree frees a previously FC_StringDup[W] allocated TCHAR* 
 *               using fc_libmem.h.
 * @param           psz[w]  : string to free, may be NULL
 */
FC_TOAPI void FC_TODECL  FC_StringFree(TCHAR* psz);

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------*
/**
 *  class FC_CString:
 *  Works like a growing buffer of chars. Internally realloc is used to 
 *  grow, never shrinks or size to fit. To work efficient the 
 *  initial size must be reasonable or resize with allocSize.
 *  FC_CString does not try to substitute the <string.h> standard functions.
 *  All standard string operations that do not change the content are not 
 *  repeated as member functions e.g. there is no FC_CString::stricmp.
 *
 *  Warning 1: whenever a TCHAR* is returned from any method this is a 
 *             pointer into the internal buffer that gets invalid after 
 *             any call to a non-const method.
 *  Warning2: don't do things like this:  str<<str
 *  Warning3: don't do printf("%s", str), you must call: printf("%s", str.psz())
 *  
*/
#ifdef __cplusplus
class FC_CString
{
private:
    //don't like: 
    FC_CString(const FC_CString&);
    FC_CString operator = (const FC_CString&);
private:
    int    m_iMax;// max. chars (bytes) that can be stored in m_psz
                  // without resize including the 0 
    int    m_iLen;// lstrlen(sz)
    TCHAR* m_psz; // data
public:
    inline FC_CString(int iLenPreAlloc=0);
    inline ~FC_CString();
    
    //resize never shrinks the allocation size of the buffer, 
    //if iNewLen<m_iLen then psz[iNewLen]=0
    //and m_iLen=iNewLen. Returns false if out of memory.
    inline bool resize(int iNewLen);

    //basic data:
    inline const TCHAR*   psz()     const{return m_psz;}
    inline operator const TCHAR*()  const{return m_psz;}
    inline int            len()     const{return m_iLen;}
    inline bool           isEmpty() const{return m_iLen==0;}
    inline BSTR           toBSTR()const;


    //initialise with text:
    inline FC_CString& load(const TCHAR* psz);
#ifndef UNICODE //you often need to load LPOLECHAR*: 
    inline FC_CString& load(const wchar_t* pszw);
#endif

    //appending text:
    inline FC_CString& addc(TCHAR c);//can not be an operator<< type conflict UNICODE
    inline FC_CString& addnc(const TCHAR ps[], int nChars);
    inline FC_CString& addpsz(const TCHAR* psz);
    inline FC_CString& tab(int pos); //insert at least one space and then more spaces until len()>=pos

    //Appending text from pszFrom but stop on char set:
    //The returned TCHAR* points into pszFrom at the position where a next appendUpTo
    //can be made to split a text or the 0-char of pszFrom.
    //If ppEndPtr is not NULL it receives a pointer to the char that stopped the append, 
    //this is either one of pszStopCharSet or 0.
    //Example:
    //  psz = "a-b..:c.-.";
    //  while(*psz)
    //  {
    //      psz = str.clear.appendUpTo(psz, "-.:", &pszStop);
    //      printf("str='%s' psz='%s' pszStop='%c'\n",psz, str.psz(), pszStop);
    //  }
    //Output will be:
    //  str='a' psz='b..:c.-.' pszStop='-b..:c'
    //  str='b' psz='c.-.' pszStop='-b..:c'
    //  str='c' psz='' pszStop='.-.'
    //  
    inline const TCHAR* appendUpTo(const TCHAR* pszFrom, const TCHAR* pszStopCharSet, const TCHAR** ppEndPtr=NULL);

    //appendWsprintf: return is false if more than 1024 chars including 0
    inline bool appendWsprintf(const TCHAR* pszFmt, ...);

    //prepend: prepends text before the currently loaded text:
    //result is this = psz1 + psz2(opt) + this
    inline FC_CString& prepend(const TCHAR* psz1, const TCHAR* psz2=NULL);

    //also some << operators:
    inline FC_CString& operator<<(const TCHAR* psz);
#ifndef UNICODE //you often need to load LPOLECHAR*: 
    inline FC_CString& operator<<(const wchar_t* pszw);
#endif
    inline FC_CString& operator<<(char c);
    inline FC_CString& operator<<(int i);
    inline FC_CString& operator<<(long l);
    inline FC_CString& operator<<(__int64 l64);
    inline FC_CString& operator<<(unsigned int i);
    inline FC_CString& operator<<(unsigned long ul);
    inline FC_CString& operator<<(unsigned __int64 ul64);
    inline FC_CString& operator<<(const GUID& uuid); //<-TODO: remove this method !
    inline FC_CString& appendLo(const TCHAR* psz);//append lower case version of psz:
    inline FC_CString& appendUp(const TCHAR* psz);//append upper case version of psz:



    //Modify in place, always same len() or shrik, allocated size does not change:
    inline FC_CString& clear();//set to empty string
    //trim[Left|Rigth] returns the number of white space TCHARs removed
    inline int trim();
    inline int trimLeft();
    inline int trimRight();

    inline void toLower(){_tcslwr(m_psz);}
    inline void toUpper(){_tcsupr(m_psz);}
    
    //truncate at last occurence of any char in charset pszSep.
    //returns pointer to char after last occurence of pszSep or ""
    //The returned pointer gets invalid by any append!
    inline const TCHAR* cutAtLast(const TCHAR* pszSep); //TODO: rework, better no TCHAR* return!

    //removeLastChar(): if pszIfOneOf is NULL the last ch 
    //is always removed. return: char removed or 0 if nothing removed.
    inline TCHAR removeLastChar(const TCHAR* pszIfOneOf);

    //beginsWith[I] test if starts with pszWith [ignore case]
    //and if stip pszWith.
    //return true if begins false otherwise.
    inline bool stripBegin (const TCHAR* pszWith);
    inline bool stripBeginI(const TCHAR* pszWith);





    //file and directory name handling:
    //appendDir(): inserts a \ and asserts if pszDir starts with \/
    inline FC_CString& appendDir(const TCHAR* pszDir);
    
    //Truncate at last \/ and returns text after last FC_PATH_SEP 
    //or "" if no file part.
    //The returned pointer gets invalid by any append!
    inline const TCHAR* stripFilePart();//TODO: rework: better no TCHAR* return.
    inline FC_CString& fileFullPath(const TCHAR* pszRoot);
    inline FC_CString& fileRelPath (const TCHAR* pszRoot /*TODO ,bool bTryRel */);
    //if pszNew is NULL a possible existing extension is removed.
    //pszNew may or may not begin with a '.'
    //the current text may or may not have an extension.
    inline FC_CString& changeFileExt(const TCHAR* pszNew);

    
    //format message:
    inline const TCHAR* fmtSysErr(DWORD dwSystemErr);
    inline const TCHAR* fmtMsgAr(HRESULT hr, HMODULE hModule, const TCHAR* ppsz[]);
    inline const TCHAR* fmtMsg(HRESULT hr, HMODULE hModule, ...);
    inline const TCHAR* fmtMsg1(HRESULT hr, HMODULE hModule, const TCHAR* psz1);
    inline const TCHAR* fmtMsg2(HRESULT hr, HMODULE hModule, const TCHAR* psz1, const TCHAR* psz2);
    inline const TCHAR* fmtMsg3(HRESULT hr, HMODULE hModule, const TCHAR* psz1, const TCHAR* psz2,
                                            const TCHAR* psz3);
    inline const TCHAR* fmtMsg4(HRESULT hr, HMODULE hModule, const TCHAR* psz1, const TCHAR* psz2, 
                                            const TCHAR* psz3, const TCHAR* psz4);

    //format message from string pszFmt, if pszFmt==NULL the current content is used
    //as message format. return the new formated content.
    inline FC_CString& fmtStr1(const TCHAR* pszFmt, const TCHAR* psz1);
    inline FC_CString& fmtStr2(const TCHAR* pszFmt, const TCHAR* psz1, const TCHAR* psz2);
    inline FC_CString& fmtStr3(const TCHAR* pszFmt, const TCHAR* psz1, const TCHAR* psz2, 
                               const TCHAR* psz3);
    inline FC_CString& fmtStr4(const TCHAR* pszFmt, const TCHAR* psz1, const TCHAR* psz2, 
                               const TCHAR* psz3, const TCHAR* psz4);


    //loadRes: clear current content and load with resource string.
    //if resource not found content remains empty. max. string len is 1023 TCHARs:
    inline FC_CString& loadRes(unsigned int uiResNum, HINSTANCE hInst=NULL);
};



//------------------------------------------------------------------*
/**
 *  class FC_CStr:
 *  Works like const TCHAR* with automatic destruction. This class can be 
 *  used to store string data in other classes, it's not intended
 *  that you change the TCHAR* psz. The psz is not allocated if it was "".
*/
class FC_CStr 
{
    TCHAR* m_psz;
public:

    inline FC_CStr()
    {
        m_psz = _T("");
    }
    inline FC_CStr(const TCHAR* psz)
    {
        m_psz = psz[0]?FC_StringDup(psz):_T("");
    }
    inline void load(const TCHAR* psz)
    {
        if(m_psz[0])
            FC_StringFree(m_psz);
        m_psz = psz[0]?FC_StringDup(psz):_T("");
    }
    inline void loadLen(const TCHAR* psz, int nChars)
    {
        if(m_psz[0])
            FC_StringFree(m_psz);
        m_psz = psz[0] && nChars>0?FC_StringDupLen(psz, nChars):_T("");
    }

#ifndef UNICODE //you often need to load LPOLECHAR*: 
    inline FC_CStr(const wchar_t* pszw)
    {
        m_psz = pszw[0]?FC_StringDupW(pszw):_T("");
    }
    inline void load(const wchar_t* pszw)
    {
        if(m_psz[0])
            FC_StringFree(m_psz);
        m_psz = pszw[0]?FC_StringDupW(pszw):_T("");
    }
    inline void loadLen(const wchar_t* pszw, int nChars)
    {
        if(m_psz[0])
            FC_StringFree(m_psz);
        m_psz = pszw[0] && nChars>0?FC_StringDupLenW(pszw, nChars):_T("");
    }
#endif
    inline ~FC_CStr()
    {
        if(m_psz[0])
            FC_StringFree(m_psz);
    }

    //get data:
    inline const TCHAR*   psz()     const{return m_psz;}
    inline operator const TCHAR*()  const{return m_psz;}
    inline bool  isEmpty()          const{return m_psz[0]==0;}
};

#endif //__cplusplus











//----------------------------------- inlined members ----------------------------
//next functions should only be used by class FC_CString:
#if defined(__cplusplus) || defined(FC_STRING_IMPL_C_)
#ifdef __cplusplus
extern "C"{
#endif
typedef struct _FC_StringTYP
{
    int    iMax;// max. chars (bytes) that can be stored in psz
                // without resize including the 0 
    int    iLen;// lstrlen(sz)
    TCHAR* psz;
} FC_StringTYP;

FC_TOAPI int FC_TODECL FC_StringNewString(
    FC_StringTYP* pData,
    int           iAllocChars
);
FC_TOAPI void FC_TODECL FC_StringFreeString(
    FC_StringTYP* pData
);
FC_TOAPI int FC_TODECL FC_StringResize(
    FC_StringTYP* pData, 
    int           iNeedChars
);
FC_TOAPI int  FC_TODECL FC_StringAppendString(
    FC_StringTYP* pData, 
    const TCHAR* pszAdd
);
FC_TOAPI int  FC_TODECL FC_StringAppendChar(
    FC_StringTYP* pData, 
    TCHAR         cadd
);
FC_TOAPI int  FC_TODECL FC_StringAppendChars(
    FC_StringTYP* pData, 
    const TCHAR*  pAdd,
    int           iChars
);
FC_TOAPI void FC_TODECL FC_StringChangeFileExt(
    FC_StringTYP*  pData, 
    const TCHAR*   pszNew
); 
FC_TOAPI int FC_TODECL FC_StringAppendWsprintf(
    FC_StringTYP*  pData, 
    const TCHAR*   pszFmt, 
    va_list        arglist
);
FC_TOAPI int FC_TODECL FC_StringPrepend(
    FC_StringTYP*  pData, 
    const TCHAR*   psz1, 
    const TCHAR*   psz2
);
FC_TOAPI const TCHAR* FC_TODECL FC_StringAppendUpTo(
    FC_StringTYP*  pData, 
    const TCHAR*   pszFrom, 
    const TCHAR*   pszStopCharSet,
    const TCHAR**  ppEndPtr
);
FC_TOAPI const TCHAR* FC_TODECL FC_StringLoadResource(
    FC_StringTYP*  pData, 
    unsigned int   uiResNum,
    HINSTANCE      hInst
);
FC_TOAPI const TCHAR* FC_TODECL FC_StringFmtMsg(
    FC_StringTYP*  pData, 
    HMODULE        hModule, // module handle, may be NULL
    HRESULT        err_id, 
    int            bArgArray,
    va_list*       arglist
);
FC_TOAPI const TCHAR* FC_TODECL FC_StringFmtMsgStr(
    FC_StringTYP*  pData, 
    const TCHAR*   pszFmt,
    int            bArgArray,
    va_list*       arglist
);

#ifdef __cplusplus
}//extern "C"
#endif
#endif //defined(__cplusplus) || defined(FC_STRING_IMPL_C_)



#ifdef __cplusplus
inline FC_CString::FC_CString(int iLenPreAlloc)
{
    FC_StringNewString((FC_StringTYP*)this, iLenPreAlloc);
}
inline FC_CString::~FC_CString()
{
    FC_StringFreeString((FC_StringTYP*)this);
}
inline bool FC_CString::resize(int iNewLen)
{
    return FC_StringResize((FC_StringTYP*)this, iNewLen)!=0;
} 
inline FC_CString& FC_CString::clear()
{
    m_iLen=0; 
    m_psz[0]=0;
    return *this;
}
inline int FC_CString::trim()
{
    int iLen = m_iLen;
    m_iLen = FC_StringTrim(m_psz);
    return iLen - m_iLen;
}
inline int FC_CString::trimLeft()
{
    int iLen = m_iLen;
    m_iLen = FC_StringTrimLeft(m_psz);
    return iLen - m_iLen;
}
inline int FC_CString::trimRight()
{
    int iLen = m_iLen;
    m_iLen = FC_StringTrimRight(m_psz);
    return iLen - m_iLen;
}
inline TCHAR FC_CString::removeLastChar(const TCHAR* pszIfOneOf)
{
    return FC_StringRemoveLastChar(m_psz, pszIfOneOf, &m_iLen);
}

inline FC_CString& FC_CString::load(const TCHAR* psz)
{
    m_iLen=0; 
    m_psz[0]=0;
    FC_StringAppendString((FC_StringTYP*)this, psz);
    return *this;
}
#ifndef UNICODE //you often need to load LPOLECHAR*: 
inline FC_CString& FC_CString::load(const wchar_t* pszw)
{
    m_iLen=0; 
    m_psz[0]=0;
    int   nBytes = wcslen(pszw)*2+1;
    char* pszMul = (TCHAR*)alloca(nBytes);
    FC_StringToMultiByte(pszw, pszMul, nBytes);
    FC_StringAppendString((FC_StringTYP*)this, pszMul);
    return *this;
}
#endif

inline FC_CString& FC_CString::addc(TCHAR c)
{
    FC_StringAppendChar((FC_StringTYP*)this, c);
    return *this;
}
inline FC_CString& FC_CString::addnc(const TCHAR ps[], int nChars)
{
    FC_StringAppendChars((FC_StringTYP*)this, ps, nChars);
    return *this;
}
inline FC_CString& FC_CString::addpsz(const TCHAR* psz)
{
    FC_StringAppendString((FC_StringTYP*)this, psz);
    return *this;
}
inline FC_CString& FC_CString::tab(int pos)
{
    do
    {
        FC_StringAppendChar((FC_StringTYP*)this, _T(' '));
    }while(m_iLen<pos);    	    
    return *this;
}
inline  const TCHAR* FC_CString::appendUpTo(const TCHAR* pszFrom, const TCHAR* pszStopCharSet, const TCHAR** ppEndPtr)
{
    return FC_StringAppendUpTo((FC_StringTYP*)this, pszFrom, pszStopCharSet, ppEndPtr);
}

inline FC_CString& FC_CString::operator<<(char c)
{
    FC_StringAppendChar((FC_StringTYP*)this, c);
    return *this;
}
inline FC_CString& FC_CString::operator<<(const TCHAR* psz)
{
    FC_StringAppendString((FC_StringTYP*)this, psz);
    return *this;
}
#ifndef UNICODE
inline FC_CString& FC_CString::operator<<(const wchar_t* pszw)
{
    int   nBytes = wcslen(pszw)*2+1;
    char* pszMul = (TCHAR*)alloca(nBytes);
    FC_StringToMultiByte(pszw, pszMul, nBytes);
    FC_StringAppendString((FC_StringTYP*)this, pszMul);
    return *this;
}
#endif 
inline FC_CString& FC_CString::operator<<(int i)
{
    TCHAR buffer[64];
    return *this<<_ltot(i,buffer,10);
}
inline FC_CString& FC_CString::operator<<(long l)
{
    TCHAR buffer[64];
    return *this<<_ltot(l,buffer,10);
}
inline FC_CString& FC_CString::operator<<(unsigned int ui)
{
    TCHAR buffer[64];
    return *this<<_ultot(ui,buffer,10);
}
inline FC_CString& FC_CString::operator<<(unsigned long ul)
{
    TCHAR buffer[64];
    return *this<<_ultot(ul,buffer,10);
}
inline FC_CString& FC_CString::operator<<(__int64 l64)
{
    TCHAR buffer[64];
    return *this<<_i64tot(l64,buffer,10);
}
inline FC_CString& FC_CString::operator<<(unsigned __int64 ul64)
{
    TCHAR buffer[64];
    return *this<<_ui64tot(ul64,buffer,10);
}
inline FC_CString& FC_CString::operator<<(const GUID& uuid)
{
   wchar_t wbuf[64];
   StringFromGUID2(uuid, wbuf, FC_ARRAY_LEN(wbuf));
   return *this<<wbuf;
}
inline bool FC_CString::appendWsprintf(const TCHAR* pszFmt, ...)
{
    va_list vp;
    va_start(vp, pszFmt);     
    return FC_StringAppendWsprintf((FC_StringTYP*)this, pszFmt, vp)!=0;
}
inline FC_CString& FC_CString::prepend(const TCHAR* psz1, const TCHAR* psz2)
{
    FC_StringPrepend((FC_StringTYP*)this, psz1, psz2);
    return *this;
}
inline FC_CString& FC_CString::loadRes(unsigned int uiResNum, HINSTANCE hInst)
{
    FC_StringLoadResource((FC_StringTYP*)this, uiResNum,  hInst);
    return *this;
}
inline FC_CString& FC_CString::appendLo(const TCHAR* psz)
{
    int iLenOld=m_iLen;
    FC_StringAppendString((FC_StringTYP*)this, psz);
   _tcslwr(&m_psz[iLenOld]);
   return *this;
}
inline FC_CString& FC_CString::appendUp(const TCHAR* psz)
{  
    int iLenOld=m_iLen;
    FC_StringAppendString((FC_StringTYP*)this, psz);
   _tcsupr(&m_psz[iLenOld]);
   return *this;
}
inline const TCHAR* FC_CString::cutAtLast(const TCHAR* pszSep)
{
    int iLenOld = m_iLen;
    m_iLen = FC_StringCutAtLast(m_psz, pszSep);
    return m_iLen<iLenOld ? &m_psz[m_iLen+1] : &m_psz[m_iLen];
}
inline FC_CString& FC_CString::changeFileExt(const TCHAR* pszNew)
{
    FC_StringChangeFileExt((FC_StringTYP*)this, pszNew);
    return *this;
}
inline const TCHAR* FC_CString::stripFilePart()
{
    int iLenOld = m_iLen;
    m_iLen=FC_StringCutFilePart(m_psz);
    return m_iLen<iLenOld ? &m_psz[m_iLen+1] : &m_psz[m_iLen];
}
inline FC_CString& FC_CString::fileFullPath(const TCHAR* pszRoot)
{
    TCHAR szBuffer[2*MAX_PATH];
    int iRet = FC_StringCanonicalPath(m_psz, szBuffer, FC_ARRAY_LEN(szBuffer), pszRoot, FC_PATHM_FULL);
    m_iLen=0; m_psz[0]=0;
    FC_StringAppendString((FC_StringTYP*)this, szBuffer);
    return *this;
}
inline FC_CString& FC_CString::fileRelPath(const TCHAR* pszRoot /*TODO ,bool bTryRel */)
{
    TCHAR szBuffer[2*MAX_PATH];
    int iRet = FC_StringCanonicalPath(m_psz, szBuffer, FC_ARRAY_LEN(szBuffer), pszRoot, FC_PATHM_KEEPREL);
    m_iLen=0; m_psz[0]=0;
    FC_StringAppendString((FC_StringTYP*)this, szBuffer);
    return *this;
}
inline FC_CString& FC_CString::appendDir(const TCHAR* pszDir)
{
    assert(*pszDir!=_T('\\') && *pszDir!=_T('/'));

    if(m_iLen>0 && m_psz[m_iLen-1] != _T('\\') && m_psz[m_iLen-1] != _T('/'))
        FC_StringAppendChar((FC_StringTYP*)this, _T('\\'));

    FC_StringAppendString((FC_StringTYP*)this, pszDir);

    return *this;
}
inline bool FC_CString::stripBegin(const TCHAR* pszWith)
{
    int i = FC_StringStripBegin(m_psz, pszWith);
    return i>=0 ? m_iLen = i, true : false;
}
inline bool FC_CString::stripBeginI(const TCHAR* pszWith)
{
    int i = FC_StringStripBeginI(m_psz, pszWith);
    return i>=0 ? m_iLen = i, true : false;
}


inline const TCHAR* FC_CString::fmtSysErr(DWORD dwSystemErr)
{
    TCHAR szBuffer[512];
    m_iLen=0; 
    m_psz[0]=0;
    FC_StringFmtSysErr(dwSystemErr, szBuffer, FC_ARRAY_LEN(szBuffer));
    FC_StringAppendString((FC_StringTYP*)this, szBuffer);
    return m_psz;
}
inline const TCHAR* FC_CString::fmtMsgAr(HRESULT hr, HMODULE hModule, const TCHAR* ppsz[])
{
    return FC_StringFmtMsg((FC_StringTYP*)this, hModule, hr, true, (va_list*)ppsz);
}
inline const TCHAR* FC_CString::fmtMsg(HRESULT hr, HMODULE hModule, ...)
{
    va_list vp;
    va_start(vp, hModule);
    return FC_StringFmtMsg((FC_StringTYP*)this, hModule, hr, false, &vp);
}
inline const TCHAR* FC_CString::fmtMsg1(HRESULT hr, HMODULE hModule, const TCHAR* psz1)
{
    const TCHAR* aa[1] = {psz1};
    return FC_StringFmtMsg((FC_StringTYP*)this, hModule, hr, true, (va_list*)aa);
}
inline const TCHAR* FC_CString::fmtMsg2(HRESULT hr, HMODULE hModule, const TCHAR* psz1, const TCHAR* psz2)
{
    const TCHAR* aa[2] = {psz1, psz2};
    return FC_StringFmtMsg((FC_StringTYP*)this, hModule, hr, true, (va_list*)aa);
}
inline const TCHAR* FC_CString::fmtMsg3(HRESULT hr, HMODULE hModule, const TCHAR* psz1, const TCHAR* psz2, 
                                                    const TCHAR* psz3)
{
    const TCHAR* aa[3] = {psz1, psz2, psz3};
    return FC_StringFmtMsg((FC_StringTYP*)this, hModule, hr, true, (va_list*)aa);
}
inline const TCHAR* FC_CString::fmtMsg4(HRESULT hr, HMODULE hModule, const TCHAR* psz1, const TCHAR* psz2, 
                                                    const TCHAR* psz3, const TCHAR* psz4)
{
    const TCHAR* aa[4] = {psz1, psz2, psz3, psz4};
    return FC_StringFmtMsg((FC_StringTYP*)this, hModule, hr, true, (va_list*)aa);
}
inline FC_CString& FC_CString::fmtStr1(const TCHAR* pszFmt, const TCHAR* psz1) 
{
    const TCHAR* aa[1] = {psz1};
    FC_StringFmtMsgStr((FC_StringTYP*)this, pszFmt, true, (va_list*)aa);
    return *this;
}
inline FC_CString& FC_CString::fmtStr2(const TCHAR* pszFmt, const TCHAR* psz1, const TCHAR* psz2) 
{
    const TCHAR* aa[2] = {psz1, psz2};
    FC_StringFmtMsgStr((FC_StringTYP*)this, pszFmt, true, (va_list*)aa);
    return *this;
}
inline FC_CString& FC_CString::fmtStr3(const TCHAR* pszFmt, const TCHAR* psz1, const TCHAR* psz2,
                                       const TCHAR* psz3) 
{
    const TCHAR* aa[3] = {psz1, psz2, psz3};
    FC_StringFmtMsgStr((FC_StringTYP*)this, pszFmt, true, (va_list*)aa);
    return *this;
}
inline FC_CString& FC_CString::fmtStr4(const TCHAR* pszFmt, const TCHAR* psz1, const TCHAR* psz2,
                                       const TCHAR* psz3, const TCHAR* psz4) 
{
    const TCHAR* aa[4] = {psz1, psz2, psz3, psz4};
    FC_StringFmtMsgStr((FC_StringTYP*)this, pszFmt, true, (va_list*)aa);
    return *this;
}


inline BSTR FC_CString::toBSTR()const
{
    return FC_StringToBSTR(m_psz);
}
#endif //__cplusplus


#endif // __FC_CSTRING_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_cstring.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 24  *****************
 * User: Ef           Date: 27.11.02   Time: 12:33
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 23  *****************
 * User: Ef           Date: 26.11.02   Time: 21:28
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_StringCanonicalPath
 * 
 * *****************  Version 22  *****************
 * User: Ef           Date: 24.07.02   Time: 12:30
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * removed AppendUpToTok, fixed bug in prepend, updated comments,  new
 * operator.<<(char)
 * 
 * *****************  Version 21  *****************
 * User: Ef           Date: 11.03.02   Time: 15:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * removed dead code
 * 
 * *****************  Version 20  *****************
 * User: Die          Date: 20.11.01   Time: 17:22
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 19  *****************
 * User: Ef           Date: 15.11.01   Time: 18:45
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 18  *****************
 * User: Ef           Date: 15.11.01   Time: 17:45
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 17  *****************
 * User: Ef           Date: 6.06.01    Time: 15:52
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new function tab
 * 
 * *****************  Version 16  *****************
 * User: Ef           Date: 15.05.01   Time: 17:40
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * loadResource: now has opt. HINSTANCE
 * 
 * *****************  Version 15  *****************
 * User: Ef           Date: 31.03.01   Time: 19:22
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions
 * 
 * *****************  Version 14  *****************
 * User: Ef           Date: 28.03.01   Time: 17:04
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_Str::load  should return void
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 26.03.01   Time: 11:53
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new func appendUpTo, trim retunres number os trimmed chars
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 20.03.01   Time: 11:51
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * reordered member functions, removed makeFullPath
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 12.02.01   Time: 14:21
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 6.02.01    Time: 16:23
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 22.01.01   Time: 11:01
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fc_System.h, some minor changes
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 15.01.01   Time: 16:22
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * some new functions
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 13.01.01   Time: 15:15
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * - new function FC_StringFree to avoid #include "lib mem.h". 
 * - bug fix in FC_CStr
 * - return "" rather than NULL from stripFilePart ...
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 12.01.01   Time: 17:13
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new stupid functions
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 11.01.01   Time: 17:56
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * added comment
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 10.01.01   Time: 18:47
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * pass HMODULE to fmtMsg functions
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 9.01.01    Time: 16:36
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_CString in extra source file
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 9.01.01    Time: 11:23
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 *==
 *----------------------------------------------------------------------------*
*/
