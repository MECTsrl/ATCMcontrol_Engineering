/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_fileIO.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_fileIO.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_fileIO.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
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
 *==File IO helper routines
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/



#include <stdio.h>
#include <tchar.h>
#include <mbstring.h> // _mbstok
#include <windows.h>

#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_string.h" //FC_StringIsAbsPath


//RETURN VALUE : TRUE : SubDir exists
FC_TOAPI int FC_TODECL FC_ExistSubDirA(const char* lpDirectory)
{
    unsigned long dwAttr = GetFileAttributesA(lpDirectory);
    return (dwAttr != 0xFFFFFFFF && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)!=0);
}

//RETURN VALUE : TRUE : SubDir created successfully
FC_TOAPI int FC_TODECL FC_CreateSubDirA(const char* lpDirectory)
{
    CHAR szTemp[MAX_PATH];
    CHAR szDir[MAX_PATH];
    LPSTR lpStr;

    szDir[0] = 0;

    lstrcpyA(szTemp, lpDirectory);

    lpStr = _mbstok(szTemp, "\\");  // get first tok, could be server name
    if (lpStr!=NULL)
    {
        lpStr = szTemp; // keep leading '\\'
        if (lpStr[0] == '\\' && lpStr[1] == '\\') 
        {
            lstrcatA(szDir, lpStr);
            lpStr = _mbstok(NULL, "\\");  // UNCpath: get next token
        }
    }

    while (lpStr != NULL)
    {
      if (szDir[0] != 0)
        lstrcatA(szDir, "\\");
      lstrcatA(szDir, lpStr);
      if (!FC_ExistSubDirA(szDir))
      {
        if (!CreateDirectoryA(szDir, NULL))
          return FALSE;
      }
      lpStr = _mbstok(NULL, "\\");
    }

    return TRUE;
}



//RETURN VALUE : TRUE  : File exists
FC_TOAPI int FC_TODECL FC_ExistA(const char* lpszFile)
{
    unsigned long dwAttr = GetFileAttributesA(lpszFile);
    return (dwAttr != 0xFFFFFFFF);
}



FC_TOAPI int FC_TODECL FC_FileFullPath(
    const TCHAR* pszPath,
    const TCHAR* pszFile, 
    TCHAR        szFull[/*nBuffer*/],
    int          nBuffer
)
{
    int    iLenF;  
    TCHAR* pszFileName;

    if(!pszPath || FC_StringIsAbsPath(pszFile))
    {
        iLenF = GetFullPathName(pszFile, nBuffer, szFull, &pszFileName);
    }
    else
    {
        int   iLenP;  
        TCHAR szTmp[2*MAX_PATH];

        iLenP = _tcslen(pszPath);
        if(iLenP + 1 > FC_ARRAY_LEN(szTmp))
           return 0;
        
        memcpy(szTmp, pszPath, iLenP*sizeof(TCHAR));
        
        //always end in a '\'
        if(szTmp[iLenP-1] == _T('/'))
            szTmp[iLenP-1] = _T('\\');
        else if(szTmp[iLenP-1] != _T('\\'))
            szTmp[iLenP++] = _T('\\');        

        iLenF = _tcslen(pszFile);
        if(iLenP + iLenF + 1 > FC_ARRAY_LEN(szTmp))
           return 0;

        memcpy(szTmp + iLenP, pszFile, (iLenF+1)*sizeof(TCHAR));
        iLenF = GetFullPathName(szTmp, nBuffer, szFull, &pszFileName);
    }

    return (iLenF>0 && iLenF<nBuffer) ? iLenF : 0;
}


FC_TOAPI int FC_TODECL FC_FileExists(
    const TCHAR* pszPath,
    const TCHAR* pszFile 
)
{
    unsigned long dwAttr;

    if(pszPath && !FC_StringIsAbsPath(pszFile))
    {
        TCHAR  szTmp[2*MAX_PATH+1];

        if(!FC_FileFullPath(
            pszPath, pszFile, szTmp, FC_ARRAY_LEN(szTmp))
          )
            return 0; //toooo long will not exist.

        dwAttr = GetFileAttributes(szTmp);
    }
    else
    {
        dwAttr = GetFileAttributes(pszFile);
    }

    return (dwAttr != 0xFFFFFFFF);
}



FC_TOAPI int FC_TODECL FC_FileIsReadOnly(const TCHAR* pszFile)
{
    unsigned long dwAttr;

    dwAttr = GetFileAttributes(pszFile);
    if(dwAttr == 0xFFFFFFFF)
        return -1;
    
    return (dwAttr & FILE_ATTRIBUTE_READONLY) ? 1 : 0;
}
FC_TOAPI int FC_TODECL FC_FileSetReadOnly(const TCHAR* pszFile, int bReadOnly)
{
    unsigned long dwAttr;

    dwAttr = GetFileAttributes(pszFile);
    if(dwAttr == 0xFFFFFFFF)
        return 0;
    
    if(bReadOnly)
    {   
        if((dwAttr & FILE_ATTRIBUTE_READONLY)==0)
            return SetFileAttributes(pszFile, dwAttr & FILE_ATTRIBUTE_READONLY) ? 1 : 0;
    }
    else
    {
        if((dwAttr & FILE_ATTRIBUTE_READONLY)!=0)
            return SetFileAttributes(pszFile, dwAttr & ~FILE_ATTRIBUTE_READONLY) ? 1 : 0;
    }

    return 1;
}


__int64 FC_FileGetSize(const TCHAR* pszFile)
{
    HANDLE hFile  = CreateFile(pszFile,GENERIC_READ,FILE_SHARE_READ,NULL,
                             OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    
    __int64 i64 = -1;
    if(hFile != INVALID_HANDLE_VALUE)
    {
        i64 = GetFileSize(hFile, NULL); //TODO error code and file size high...
        CloseHandle(hFile);
    }

    return i64;
}

__int64 FC_FileGetLastWriteTime(const TCHAR* pszFile)
{
    __int64                     ft = 0;
    HANDLE                      hFile;
    BY_HANDLE_FILE_INFORMATION  fileInfo;

    
    hFile = CreateFile(pszFile,GENERIC_READ,FILE_SHARE_READ,NULL,
                       OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    
    if(hFile!=INVALID_HANDLE_VALUE && GetFileInformationByHandle(hFile, &fileInfo))
    {
        LARGE_INTEGER  lint;
        lint.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
        lint.HighPart= fileInfo.ftLastWriteTime.dwHighDateTime;
        ft = lint.QuadPart;
    }

    if(hFile != INVALID_HANDLE_VALUE)
        CloseHandle(hFile);
    return ft;
}




__inline int IsDotDotDir(const TCHAR* pszDir)
{
    return (pszDir[0]==_T('.') && pszDir[1]==0) ||
           (pszDir[0]==_T('.') && pszDir[1]==_T('.') && pszDir[2]==0);
}



FC_TOAPI int FC_TODECL FC_DeleteDirectory(const TCHAR* pszDirectory)
{
    TCHAR           szFindPattern[2*_MAX_PATH];
    TCHAR           szFilePathName[2*_MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE          hFind;

    if(!pszDirectory || !FC_StringIsAbsPath(pszDirectory))
    {
        return 0;
    }

    
    _stprintf(szFindPattern, _T("%s\\*"), pszDirectory);

    hFind = FindFirstFile(szFindPattern, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    // run over all files
    do
    {
        if(IsDotDotDir(FindFileData.cFileName))
            continue;

        _stprintf(szFilePathName, _T("%s\\%s"), pszDirectory, FindFileData.cFileName);

        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // call delete directory recursively
            FC_DeleteDirectory(szFilePathName);
        }
        else
        {
            // not directory -> try to delete file
            if(!DeleteFile(szFilePathName))
            {
                //stop on first error:
                FindClose(hFind);
                return 0;
            }
        }

    } while(FindNextFile(hFind, &FindFileData));

    FindClose(hFind);

    // now remove (hopefully empty) directory
    return RemoveDirectory(pszDirectory) ? 1 : 0;
}





FC_TOAPI int FC_TODECL FC_DeleteDirectoryEx(
    const TCHAR* pszDirectory,
    long         lFlags
)
{
    TCHAR           szFindPattern[2*_MAX_PATH];
    TCHAR           szFilePathName[2*_MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE          hFind;

    if(!pszDirectory || !FC_StringIsAbsPath(pszDirectory))
    {
        return 0;
    }

    _stprintf(szFindPattern, _T("%s\\*"), pszDirectory);

    hFind = FindFirstFile(szFindPattern, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    // run over all files
    do
    {
        if(IsDotDotDir(FindFileData.cFileName))
            continue;

        _stprintf(szFilePathName, _T("%s\\%s"), pszDirectory, FindFileData.cFileName);

        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // call delete directory recursively
            FC_DeleteDirectory(szFilePathName);
        }
        else 
        {
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) && (lFlags & FC_FILE_DEL_MAKEWRT))
            {
                SetFileAttributes(szFilePathName, FindFileData.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
            }

            // not directory -> try to delete file
            if(!DeleteFile(szFilePathName))
            {
                //stop on first error:
                FindClose(hFind);
                return 0;
            }
        }

    } while(FindNextFile(hFind, &FindFileData));

    FindClose(hFind);

    // now remove (hopefully empty) directory
    if(lFlags & FC_FILE_DEL_ONLYFILES)
    {
        return 1;
    }
    else
    {
        if(lFlags & FC_FILE_DEL_MAKEWRT)
            FC_FileSetReadOnly(pszDirectory, 0);

        return RemoveDirectory(pszDirectory) ? 1 : 0;
    }
}







FC_TOAPI int FC_TODECL FC_FileLastWrt(
    const TCHAR* pszStartDir,
    const TCHAR* pszIncludeDirs,
    const TCHAR* pszIncludeFiles,
    const TCHAR* pszExcludeDirs,
    const TCHAR* pszExcludeFiles,
    TCHAR*       pszFileName,
    long         nFileName,
    __int64*     pFTLatestWrt
)
{
    TCHAR           szFindPattern[2*_MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE          hFind;
    __int64         lwftResult;


    if(pszFileName && nFileName < _MAX_PATH)
        return 0; //TODO set last err
    if(_tcslen(pszStartDir) >= _MAX_PATH)
        return 0; //TODO set last err
    
    lwftResult = 0;

    if(pszFileName)
        *pszFileName = 0;
    if(pFTLatestWrt)
        *pFTLatestWrt = 0;

    _tcscpy(szFindPattern, pszStartDir);
    _tcscat(szFindPattern, _T("\\*"));
    
    hFind = FindFirstFile(szFindPattern, &FindFileData);

    if(hFind == INVALID_HANDLE_VALUE)
        return 1; //empty directory return 0

    do
    {
        if(IsDotDotDir(FindFileData.cFileName))
            continue;

        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(!pszExcludeDirs || !FC_StrMatchI(FindFileData.cFileName, pszExcludeDirs))
            {
                if(!pszIncludeDirs || FC_StrMatchI(FindFileData.cFileName, pszIncludeDirs))
                {
                    TCHAR           szTmp[2*_MAX_PATH];
                    TCHAR           szTmpPath[2*_MAX_PATH];
                    __int64         lwftSubDir;

                    _tcscpy(szTmpPath, pszStartDir);
                    _tcscat(szTmpPath, _T("\\"));
                    _tcscat(szTmpPath, FindFileData.cFileName);

                    FC_FileLastWrt(szTmpPath, 
                        pszIncludeDirs, pszIncludeFiles, pszExcludeDirs, pszExcludeFiles, 
                        pszFileName ? szTmp               : NULL, 
                        pszFileName ? FC_ARRAY_LEN(szTmp) : 0, 
                        &lwftSubDir
                    );

                    if(lwftSubDir > lwftResult)
                    {
                        if(pszFileName)
                        {
                            _tcsncpy(pszFileName, szTmp, nFileName);
                            pszFileName[nFileName-1] = 0;
                        }
                        lwftResult = lwftSubDir;
                    }
                }
            }
        }
        else if(!pszIncludeFiles || FC_StrMatchI(FindFileData.cFileName, pszIncludeFiles))
        {
            LARGE_INTEGER  lint;
            lint.LowPart = FindFileData.ftLastWriteTime.dwLowDateTime;
            lint.HighPart= FindFileData.ftLastWriteTime.dwHighDateTime;
            
            if(lint.QuadPart > lwftResult)
            {
                if(!pszExcludeFiles || !FC_StrMatchI(FindFileData.cFileName, pszExcludeFiles))
                {
                    lwftResult = lint.QuadPart;
                    if(pszFileName)
                    {
                        _tcscpy(pszFileName, pszStartDir);
                        _tcscat(pszFileName, _T("\\"));
                        _tcscat(pszFileName, FindFileData.cFileName);
                    }
                }
            }
        }
    } while(FindNextFile(hFind, &FindFileData));

    FindClose(hFind);

    if(pFTLatestWrt)
        *pFTLatestWrt = lwftResult;

    return 1;
}




/*
 *----------------------------------------------------------------------------*
 *  $History: fc_fileIO.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 21.01.03   Time: 15:27
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new functions
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 12.06.02   Time: 16:51
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new file functions
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 28.01.02   Time: 17:52
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * various bugfixes
 * 
 * *****************  Version 9  *****************
 * User: Sis          Date: 9.01.02    Time: 15:46
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * FC_DeleteDirectory()
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 3.12.01    Time: 16:56
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * extra file for unicode implem. of fileIO
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 29.08.01   Time: 22:11
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * better file enum
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 20.03.01   Time: 11:44
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * changed file,path argument oder
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.02.01   Time: 10:32
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new functions in fc_fileIO.h, extra impl. c-file for string file name
 * stuff
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.01.01    Time: 10:40
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * added some file headers
 *==
 *----------------------------------------------------------------------------*
*/
