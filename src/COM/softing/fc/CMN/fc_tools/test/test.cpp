//#define UNICODE 
//#define _UNICODE
#include <stdio.h>

#include "fc_todebug\fc_assert.h"
 
#include "fc_tools\fc_string.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_registry.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_carchive.h"
#include "fc_tools\fc_crypt.h"
#define FC_LIB_USE_TODEBUG
//#define FC_LIB_USE_CRT
#include "fc_tools\fc_libmem.h"


void* operator new(size_t s)
{
    void* p=NULL;
    FC_LibNewMemory(&p,s);
    return p;
}
void operator delete(void* p)
{
  if(p) 
    FC_LibFreeMemory(p);
}





static void TestHash();
static void TestString();
static void TestGArray(); 
static void TestRegistry();
static void TestString2();
static void TestArchive();
static void TestFileList();
static void TestCrypt();



int main()
{
    TestArchive();
    //return 0;
//    TestFileList();
//    return 0;
//    TestHash();
//    TestString();
#if 0 
    TestString2();
    TestGArray(); 
    TestRegistry();
#endif
    TestCrypt();
    return 0;
}

struct stru1
{
    int p;
    int p2;
};

static void TestHash()
{
    bool bOk;
    const TCHAR* psz;
    FC_CHashStr<const TCHAR*>              sbys(true,true,1);
    FC_CHashStrTable                       bag;
    const FC_CHashStr<const TCHAR*>::Pair* ppair;
    FC_CHashStr<int>                       intHash(false, true);
    FC_CString                             jot;
    FC_CHashPtr<stru1*, int>                ntHash(false, true);
    
    bOk = sbys.insert(_T("a"), _T("test1")); 
    assert(bOk);
    bOk = sbys.insert(_T("a"), _T("test2"));
    assert(!bOk);
    bOk = sbys.remove(_T("a"));
    assert(bOk);
    bOk = sbys.insert(_T("a"), _T("test1")); 
    assert(bOk);


    bOk = sbys.insert(_T("b"), _T("test3"));
    assert(bOk);
    bOk = sbys.insert(_T("c"), _T("test4"));
    assert(bOk);
    bOk = sbys.insert(_T("c"), _T("test5"));
    assert(!bOk);
    
    bOk = sbys.insert(_T(""), _T("test6"));
    assert(bOk);
    
    bOk = sbys.insertEx(_T(""), _T("test7"), false, &psz);  
    assert(!bOk && !lstrcmp(psz, _T("test6")));

    bOk = sbys.insertEx(_T("&"), _T("test7"), true, &psz);  
    assert(bOk && !lstrcmp(psz, _T("test6")));
    psz = NULL;
    bOk = sbys.insertEx(_T(""), _T("test8"), true, &psz);  
    assert(!bOk && !lstrcmp(psz, _T("test6")));
    
    ppair = sbys.getPair(_T(""));  
    assert(ppair && !lstrcmp(ppair->pszKey, _T("")) && 
                    !lstrcmp(ppair->pVal, _T("test8")));

   
    for(ppair = sbys.firstPair(); ppair;ppair = ppair->pNext)
    {
        _tprintf(_T("%s<->%s\n"),ppair->pszKey, ppair->pVal); 
    }


    psz = bag.registerString(_T(""));
    assert(*psz==0);
    psz = bag.registerString(_T("ab"));
    assert(psz[0]==_T('a'));
    assert(psz[1]==_T('b'));
    assert(psz[2]==0);
    psz = bag.registerString(_T(""));
    assert(*psz==0);

    for(int i=0;i<=100000;i++)
    {
        bOk = intHash.insert(jot.clear()<<i, i);
        assert(bOk);
    }

    bOk = intHash.remove(_T("72")) && intHash.remove(_T("172"))
          && intHash.remove(_T("100000"));
    assert(bOk);

    for(i=0;i<=100000;i++)
    {
        bOk = intHash.remove(jot.clear()<<i);
        assert(bOk || i==72 || i==172 || i==100000);
    }

    assert(intHash.countPairs()==0);
}




static void TestString2()
{
    FC_CString buf;
    int iLen;
    const TCHAR* pszTok;
    const TCHAR* pszPat;
    const TCHAR* psz;

    pszTok = "abc";
    pszPat = "a?cd|b*|a*";
    psz = FC_StrMatchI(pszTok, pszPat);
    assert(psz == pszPat+8);

    pszTok = "abc";
    pszPat = "ABC";
    psz = FC_StrMatchI(pszTok, pszPat);
    assert(psz == pszPat);
        
    pszTok = "xyz";
    pszPat = "xyz1";
    psz = FC_StrMatchI(pszTok, pszPat);
    assert(!psz);
    
    pszTok = "xyz";
    pszPat = "xyz1|xyz*";
    psz = FC_StrMatchI(pszTok, pszPat);
    assert(psz == pszPat+5);
    
    
    buf<<2;
    assert(!buf.isEmpty());
    buf.clear();

    iLen = buf.len();
    assert(iLen==0);
    assert(buf.isEmpty());

    buf<<_T("");
    iLen = buf.len();
    assert(iLen==0);
    assert(buf.isEmpty());


    buf<<_T("Frank");
    iLen = buf.len();
    assert(iLen==5);
    assert(!buf.isEmpty());

    buf<<_T("Bernd");
    iLen = buf.len();
    assert(iLen==10);
    assert(!buf.isEmpty());

    buf<<_T("");
    iLen = buf.len();
    assert(iLen==10);
    assert(!buf.isEmpty());

    
    buf.clear();  



}


static void TestString()
{
    FC_CString   jot;
    const TCHAR* pszExt;

    jot.changeFileExt(_T(".txt"));
    pszExt = FC_StringGetFileExt(jot);
    assert(!lstrcmp(pszExt, _T("txt")));
    jot<<_T("\\\\\\S_4C\\scratch/ef\\file.ext");
    pszExt = FC_StringGetFileExt(jot);
    assert(!lstrcmp(pszExt, _T("ext")));

    jot.changeFileExt(NULL);
    assert(!FC_StringGetFileExt(jot)[0] && FC_StringGetFileExt(jot)==&jot[jot.len()]);

    jot.changeFileExt(_T(""));
    pszExt = FC_StringGetFileExt(jot);
    assert(!lstrcmp(pszExt, _T("")));

    jot.changeFileExt(_T(".."));
    pszExt = FC_StringGetFileExt(jot);
    assert(!lstrcmp(pszExt, _T("")));

    jot.changeFileExt(NULL);
    pszExt = FC_StringGetFileExt(jot);
    assert(!lstrcmp(pszExt, _T("")));

    jot.changeFileExt(NULL);
    pszExt = FC_StringGetFileExt(jot);
    assert(!lstrcmp(pszExt, _T("")));

    jot.changeFileExt(NULL);
    pszExt = FC_StringGetFileExt(jot);
    assert(!pszExt[0] && pszExt==&jot[jot.len()]);

    jot.clear()<<_T("\\\\S_4C\\scrat ch/ef\\");
    jot.appendDir(_T("nextDir"));
    assert(!lstrcmp(jot, _T("\\\\S_4C\\scrat ch/ef\\\\nextDir")));

    pszExt = FC_StringGetFilePart(jot);
    assert(!lstrcmp(pszExt, _T("nextDir")));
_tprintf(_T("jot=%s\n"), jot.psz());
    pszExt = jot.stripFilePart();_tprintf(_T("jot=%s\n"), jot.psz());
    assert(!lstrcmp(pszExt, _T("nextDir")));

    pszExt = jot.stripFilePart();
    assert(!lstrcmp(pszExt, _T("")));

    pszExt = jot.stripFilePart();
    assert(!lstrcmp(pszExt, _T("ef")));

    pszExt = jot.stripFilePart();
    assert(!lstrcmp(pszExt, _T("scrat ch")));

    pszExt = jot.stripFilePart();
    assert(!lstrcmp(pszExt, _T("S_4C")));

    assert(!lstrcmp(jot, _T("\\")));
 
    jot<<_T("\\");
    assert(FC_StringIsAbsPath(jot)==2);

    jot.clear()<<_T("x:");
    assert(FC_StringIsAbsPath(jot)==1);

    jot.clear()<<_T("X:\\");
    assert(FC_StringIsAbsPath(jot)==1);

    jot.clear()<<_T("CX:\\");
    assert(FC_StringIsAbsPath(jot)==0);


#if 0  //TODO 
    jot.load(_T("uiou.txt"));
    jot.makeCononicalPath(_T("c:\\dir/dir2"), false);
    assert(!lstrcmp(jot, _T("c:\\dir\\dir2\\uiou.txt")));

    jot.makeCononicalPath(_T("c:\\dir/dir2"), false);
    assert(!lstrcmp(jot, _T("c:\\dir\\dir2\\uiou.txt")));

    jot.load("..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2"), false);
    assert(!lstrcmp(jot, _T("c:\\x")));

    jot.load("..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load("..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2\\"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load("..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2/"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load("..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2/////"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load("..\\..\\.\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2/////"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load("..\\..\\..\\..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/dir2/////"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load("..\\..\\..\\..\\..\\x");
    jot.makeCononicalPath(_T("c:\\dir/.\\dir2/////"), true);
    assert(!lstrcmp(jot, _T("..\\..\\x")));

    jot.load(_T("..\\/////\\\\\\..\\uiou.txt"));
    jot.makeCononicalPath(_T("\\\\s_4c\\scratch/x\\..\\///\\..\\"), false);
    assert(!lstrcmp(jot, _T("\\\\s_4c\\scratch\\uiou.txt")));

    jot.load(_T("c:/x//   //x/./"));
    jot.makeCononicalPath(NULL, false);
    assert(!lstrcmp(jot, _T("c:\\x\\   \\x\\")));

    jot.load(_T("..\\"));
    jot.makeCononicalPath(NULL, true);
    assert(!lstrcmp(jot, _T("..\\")));

    jot.load(_T("..\\../."));
    jot.makeCononicalPath(_T("c:\\x\\y\\z"), true);
    assert(!lstrcmp(jot, _T("..\\..\\..\\x")));

    jot.load(_T("..\\..\\..\\..\\..\\../."));
    jot.makeCononicalPath(_T("c:\\x\\y\\z"), true);
    assert(!lstrcmp(jot, _T("..\\..\\..\\")));

    jot.makeCononicalPath(_T("c:\\x\\y\\z"), false);
    assert(!lstrcmp(jot, _T("c:\\")));

    jot.load(_T("\\..\\../."));
    jot.makeCononicalPath(_T("c:\\x\\y\\z"), true);
    assert(!lstrcmp(jot, _T("..\\..\\..\\x")));
#endif
}


class AC
{
    AC() : i(67){}
    int i;
};

static void TestGArray()
{
    FC_CStrList strlist,strlist2;
    //FC_CGArray<AC> classlist;
    FC_CGArray<long> longList;
    FC_CString jot(230);


    for(int i=0; i<10;i++)
    {
        jot.clear()<<i;
        strlist.add(jot);
        longList.add(i);
    }
    for(i=0; i<longList.getUsed();i++)
    {
        assert(i==longList.get(i));
        //_tprintf(_T("i=%d == %d\n"), i, longList.get(i));
    }
    
    strlist2.addStrList(&strlist);
    strlist2.addStrList(&strlist);

    for(i=0; i<strlist2.getUsed();i++)
    {
        jot.clear()<<i%10;
        assert(!_tcscmp(jot.psz(), strlist2.get(i)));
        //_tprintf(_T("i=%s %s\n"), jot.psz(), strlist2.get(i));
    }
}















static bool g_bRegStopPrint;
static int CALLBACK RegPrintReg(
   HKEY          hKey,
   const TCHAR*  pszSubKey, 
   void*         pUserData
)
{
   FC_CString* pStr = (FC_CString*)pUserData;
   if(pszSubKey)
   {
       (*pStr)<<_T("\\")<<pszSubKey;
       _tprintf(_T("HKCR%s\n"),pStr->psz());

       if(!_tcscmp(pStr->psz(), _T("\\CLSID")))
       {
           _tprintf(_T("      skip HKCR\\CLSID, dump the key after CLSID and then stop enumerating\n"));
           pStr->cutAtLast(_T("\\"));
           g_bRegStopPrint= true;
           return 0;
       }
   }
   else
   {
       pStr->cutAtLast(_T("\\"));
   }
   if(g_bRegStopPrint)
       return -1;

   return 1;
}
static void TestRegistry()
{
    FC_CString jot;
    LONG lret;
    TCHAR buffer[MAX_PATH];
    DWORD dw;    

    lret = FC_RegSetStringVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"),_T("astr"),_T("the value"));
    FC_ASSERT_SYSERR(lret, _T("FC_RegSetStringVal"));

    lret = FC_RegSetDwordVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"),_T("adw"),42);
    FC_ASSERT_SYSERR(lret, _T("FC_RegSetDwordVal"));

    lret = FC_RegSetStringVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"),NULL,_T("the unnamed value"));
    FC_ASSERT_SYSERR(lret, _T("FC_RegSetStringVal"));

    
    lret = FC_RegGetStringVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"),
                         _T("astr"),buffer, FC_ARRAY_LEN(buffer));
    FC_ASSERT_SYSERR(lret, _T("FC_RegGetStringVal"));
    assert(!_tcscmp(buffer, _T("the value")));

    lret = FC_RegGetDwordVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"), _T("adw"), &dw);
    FC_ASSERT_SYSERR(lret, _T("FC_RegGetDwordVal"));
    assert(dw==42);

    lret = FC_RegGetStringVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"),
                         NULL, buffer, FC_ARRAY_LEN(buffer));
    FC_ASSERT_SYSERR(lret, _T("FC_RegGetStringVal"));
    assert(!lstrcmp(buffer, _T("the unnamed value")));


    //test buffer to small:
    lret = FC_RegGetStringVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"),
                         _T("astr"),buffer,  _tcslen(_T("the value")) );
    assert(lret==ERROR_MORE_DATA);

    //test wrong type:
    lret = FC_RegGetDwordVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"), _T("astr"), &dw);
    assert(lret==ERROR_INVALID_DATA);

    lret = FC_RegGetStringVal(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"), _T("adw"),
                                       buffer,  FC_ARRAY_LEN(buffer));
    assert(lret==ERROR_INVALID_DATA);



    //clean up reg:
    //test error case: deleting a top level key that has sub keys is not allowed
    lret = FC_RegRecursiveDelete(HKEY_CLASSES_ROOT, _T("00000aaaaa"));
    assert(lret==E_INVALIDARG);

    lret = FC_RegRecursiveDelete(HKEY_CLASSES_ROOT, _T("00000aaaaa\\test"));
    FC_ASSERT_SYSERR(lret, _T("FC_RegRecursiveDelete"));

    lret = FC_RegRecursiveDelete(HKEY_CLASSES_ROOT, _T("00000aaaaa"));
    FC_ASSERT_SYSERR(lret, _T("FC_RegRecursiveDelete"));


    lret= FC_RegEnumKeys(HKEY_CLASSES_ROOT, NULL,RegPrintReg, &jot);
    //assert that the return value signals the abort from the call back:
    assert(lret==ERROR_OPERATION_ABORTED);
}



static void TestArchive()
{
    FC_CArchive   ar;
    FC_CArDir*    pRoot;
    FC_ARSTAT     ret;
    const TCHAR*  pszAr = "test.ar";
    TCHAR         szVerFound[FC_AR_MAX_VER];
  

    unlink(pszAr);
    szVerFound[0] = 2;
    
    ret = ar.open(pszAr, true, szVerFound);
    assert(ret == FC_AR_OK || szVerFound[0]==0);

    pRoot = ar.getRoot();
     
    ret = pRoot->createFilesFromDisk("C:\\4c\\COM\\softing\\fc\\CCG\\*", true);
    assert(ret == FC_AR_OK);

    ret = pRoot->getFilesToDisk("C:\\4c\\COM\\softing\\fc\\__get_CCG", NULL, true, NULL);
    assert(ret == FC_AR_OK);

    _tcscpy(szVerFound, _T("ef"));
    ret = ar.close(true, szVerFound);
    assert(ret==FC_AR_OK);


    ret = ar.open(pszAr, false, szVerFound);
    assert(ret == FC_AR_OK || !_tcscmp(szVerFound, _T("ef")));

    pRoot = ar.getRoot();

    ret = pRoot->getFilesToDisk("C:\\4c\\COM\\softing\\fc\\__get_CCG2", NULL, true, NULL);
    assert(ret == FC_AR_OK);

    ret = ar.close(false, szVerFound);
    assert(ret==FC_AR_OK);

}


static void TestFileList()
{
    TCHAR** pzpsz;
    TCHAR** pzpszList;
 
    pzpszList = FC_FileBuildFileList("C:\\4c2\\Engineering\\bin", "*.kad", /*FC_FILE_ENUM_SUBDIRS|FC_FILE_ENUM_FULLPATH*/0);

    if(pzpszList)
    {
        for(pzpsz = pzpszList; *pzpsz; pzpsz++)
            _tprintf(_T("%s\n"), *pzpsz); 

        FC_LibFreeMemory(pzpszList);
    }

}

static char* NewTestString(unsigned long ulChars)
{
    char* psz = new char[ulChars+1];

    psz[ulChars] = 0;
    for(unsigned long ul=0; ul<ulChars; ul++)
        psz[ul] = (char)('0' + ul%10);

    return psz;
}

static void TestCryptSingle(const char* pszData)
{
    long            nMode = FC_DES_NORMAL/*|FC_DES_FAST*/;
    const char*     pszPw = "hugo";
    unsigned long   nData;
    char*           pEncoded;
    unsigned long   nEncoded;
    unsigned long   ulDecodedSize;
    char*           pDecoded;
    int             iRet;
    
    nData = strlen(pszData)+1;

    pEncoded = new char[nData+64];
    FC_DESEncodeBlock(nMode,pszPw,pszData,nData,pEncoded,&nEncoded);

    iRet= FC_DESGetDecodedSize(nMode,pszPw,pEncoded,nEncoded,&ulDecodedSize);
    assert(iRet==0);
    assert(ulDecodedSize == nData);

    pDecoded = new char[ulDecodedSize];
    iRet = FC_DESDecodeBlock(nMode,pszPw,pEncoded,nEncoded,pDecoded,ulDecodedSize);
    assert(iRet ==0);
    assert(!strcmp(pDecoded, pszData));
    
    delete(pEncoded);
    delete(pDecoded);
}
static void TestCryptEmpty()
{
    long            nMode = FC_DES_NORMAL;
    const char*     pszPw = "hugo";
    unsigned long   nData;
    char*           pEncoded;
    unsigned long   nEncoded;
    unsigned long   ulDecodedSize;
    char*           pDecoded;
    int             iRet;
    
    nData = 0;
    pEncoded = new char[nData+64];
    FC_DESEncodeBlock(nMode,pszPw,NULL,nData,pEncoded,&nEncoded);

    iRet= FC_DESGetDecodedSize(nMode,pszPw,pEncoded,nEncoded,&ulDecodedSize);
    assert(iRet==0);
    assert(ulDecodedSize == nData);

    pDecoded = NULL;
    iRet = FC_DESDecodeBlock(nMode,pszPw,pEncoded,nEncoded,pDecoded,ulDecodedSize);
    assert(iRet ==0);
    
    delete(pEncoded);
    delete(pDecoded);
}



static void TestCrypt()
{
    char* psz;
    unsigned long ul;
    
    TestCryptEmpty();


    unsigned long dwStart = GetTickCount();
    for(ul=0;ul<1000; ul++)
    {
        psz = NewTestString(ul); 
        TestCryptSingle(psz);
        delete(psz);
    }
    printf("time=%u ms\n", GetTickCount() - dwStart);

    dwStart = GetTickCount();
    psz = NewTestString(1024*1024); 
    for(ul=0;ul<1; ul++)
        TestCryptSingle(psz);

    printf("time=%u ms\n", GetTickCount() - dwStart);
    delete(psz);
}



