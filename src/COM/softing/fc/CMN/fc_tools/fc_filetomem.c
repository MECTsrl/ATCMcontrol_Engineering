/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_filetomem.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_filetomem.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_filetomem.c $
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
 *==Implementation of load file routines
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  16.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


//----  Includes:   -------------------------------------------*
#include <windows.h> //DWORD,...
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_libmem.h"




FC_TOAPI int FC_TODECL FC_LoadDiskBinFileToMem(
    const TCHAR*   pszFileName, 
    void**         ppByteBuffer, 
    unsigned long* pulLength, 
    __int64*       pftLastWrite
)
{   
    DWORD                       nBytes;
    HANDLE                      hFile;
    DWORD                       dwSize;
    BY_HANDLE_FILE_INFORMATION  fileInfo;

    FC_LIBASSERT(pszFileName && ppByteBuffer && pulLength);

    //set OUTs to NULL first:
    *ppByteBuffer = NULL;
    if(pftLastWrite)
        *pftLastWrite = 0;


    //open file and check max file size:
    hFile  = CreateFile(pszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
                        OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(hFile==INVALID_HANDLE_VALUE || !GetFileInformationByHandle(hFile, &fileInfo))
    {
        if(hFile!=INVALID_HANDLE_VALUE)
            CloseHandle(hFile);

        if(GetFileAttributes(pszFileName)!=0xFFFFFFFF)
            return 2; //file exists, some non trivial error
        else 
            return 1; //file exists, return 1 to signal the trivial error
    }

    //set last modified time:
    if(pftLastWrite)
    {
        LARGE_INTEGER  lint;
        lint.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
        lint.HighPart= fileInfo.ftLastWriteTime.dwHighDateTime;
        *pftLastWrite = lint.QuadPart;
    }
    
    dwSize = fileInfo.nFileSizeLow;
    if(fileInfo.nFileSizeHigh!=0 || (*pulLength>0 && *pulLength<dwSize))
    {
        if(hFile!=INVALID_HANDLE_VALUE)
            CloseHandle(hFile);
        //large files >4GB !
        if(fileInfo.nFileSizeHigh!=0)
            *pulLength = 0;
        else
            *pulLength = dwSize;
        
        return 3;
    }
    if(!FC_LibNewMemory(ppByteBuffer, dwSize==0 ? 1 : dwSize))
    {
        CloseHandle(hFile);
        return -1;
    }
    if(!ReadFile(hFile, *ppByteBuffer, dwSize, &nBytes, NULL) || nBytes!=dwSize)
    {
        CloseHandle(hFile);
        FC_LibFreeMemory(*ppByteBuffer);
        *ppByteBuffer = NULL;
        return 2; //file exists, some non trivial error
    }
    CloseHandle(hFile);
    *pulLength = dwSize;

    return 0;
}


FC_TOAPI int FC_TODECL FC_LoadDiskTextFileToMem(
    const TCHAR*   pszFileName, 
    TCHAR**        ppTCHARBuffer, 
    unsigned long* pulLength,    
    __int64*       pftLastWrite
)
{   
    DWORD                       nBytes;
    HANDLE                      hFile;
    DWORD                       dwSize;
    BY_HANDLE_FILE_INFORMATION  fileInfo;
    BOOL                        bUnicode;
    DWORD                       dwLastErr;

    FC_LIBASSERT(pszFileName && ppTCHARBuffer && pulLength);

    //set OUTs to NULL first:
    *ppTCHARBuffer = NULL;
    if(pftLastWrite)
        *pftLastWrite = 0;


    //open file and check max file size:
    hFile  = CreateFile(pszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
                        OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if(hFile==INVALID_HANDLE_VALUE || !GetFileInformationByHandle(hFile, &fileInfo))
    {
        dwLastErr = GetLastError();//safe last error
        if(hFile!=INVALID_HANDLE_VALUE)
            CloseHandle(hFile);

        if(GetFileAttributes(pszFileName)!=0xFFFFFFFF)
        {
            SetLastError(dwLastErr);
            return 2; //file exists, some non trivial error
        }
        else 
        {
            SetLastError(dwLastErr);
            return 1; //file exists, return 1 to signal the trivial error
        }
    }

    
    //set last modified time:
    if(pftLastWrite)
    {
        LARGE_INTEGER  lint;
        lint.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
        lint.HighPart= fileInfo.ftLastWriteTime.dwHighDateTime;
        *pftLastWrite = lint.QuadPart;
    }

    //check max file size:
    dwSize = fileInfo.nFileSizeLow;
    if(fileInfo.nFileSizeHigh!=0 || (*pulLength>0 && *pulLength<dwSize))
    {
        if(hFile!=INVALID_HANDLE_VALUE)
            CloseHandle(hFile);
        //large files >4GB !
        if(fileInfo.nFileSizeHigh!=0)
            *pulLength = 0;
        else
            *pulLength = dwSize;
        
        return 3;
    }



    //check if unicode file:
    bUnicode = 0;
    if(dwSize>=2 && (dwSize & 0x1)==0) //must at least be even number of bytes
    {
        //and must start with 0xFEFF:
        WORD wo;
        if(!ReadFile(hFile, &wo, sizeof(WORD), &nBytes, NULL) || nBytes!=2)
        {
            CloseHandle(hFile);
            return 2;//return file IO error
        }

        bUnicode = wo==0xFEFF;
        if(bUnicode)
        {
            dwSize -= 2;
        }
        else
        {
            //reset the file pointer:
            if(SetFilePointer(
                              hFile,     // handle to file
                              0,         // bytes to move pointer
                              NULL,      // bytes to move pointer
                              FILE_BEGIN // starting point
                            ) != 0
            )
            {
                CloseHandle(hFile);
                return 2;//return file IO error
            }
        }

    }

    //special case 0 size file:
    if(dwSize==0)
    {
        if(!FC_LibNewMemory(ppTCHARBuffer, sizeof(wchar_t)))//+1 we will 0 terminate anyway
        {
            CloseHandle(hFile);
            return -1;
        }
        *pulLength = 0;
        CloseHandle(hFile);
        return 0;
    }

#ifdef UNICODE
    if(bUnicode)
#else
    if(!bUnicode)
#endif
    {
        //need not to convert. A possible 0xFEFF is has been already read and dwSize was 
        //decremented by 2. The next code is the same for unicode/non unicode build 
        //only TCHAR changes:
        if(!FC_LibNewMemory(ppTCHARBuffer, dwSize+sizeof(TCHAR)))//+1 we will 0 terminate anyway
        {
            CloseHandle(hFile);
            return -1;
        }
        if(!ReadFile(hFile, *ppTCHARBuffer, dwSize, &nBytes, NULL) || nBytes!=dwSize)
        {
            CloseHandle(hFile);
            FC_LibFreeMemory(*ppTCHARBuffer);
            *ppTCHARBuffer = NULL;
            return 2; //file exists, some non trivial error
        }
        //set length in TCHARs and 0-term:
        *pulLength = dwSize/sizeof(TCHAR);
    }
    else
    {
        int iNeed;
        int iRet;
        
        //must convert, first read file into tmp buffer:
        void* pTmp;//tmp buffer for file content 
        if(!FC_LibNewMemory(&pTmp, dwSize))
        {
            CloseHandle(hFile);
            return -1;
        }
        if(!ReadFile(hFile, pTmp, dwSize, &nBytes, NULL) || nBytes!=dwSize)
        {
            CloseHandle(hFile);
            FC_LibFreeMemory(pTmp);
            return 2; //file exists, some non trivial error
        }

        iNeed =
        #ifdef UNICODE
            MultiByteToWideChar(
              CP_ACP,               // code page
              0,                    // character-type options
              pTmp,                 // string to map
              dwSize,               // number of bytes in string
              NULL,                 // wide-character buffer
              0                     // size of buffer
            );
        #else
            WideCharToMultiByte(
              CP_ACP,                   // code page
              0,                        // performance and mapping flags
              pTmp,                     // wide-character string
              dwSize/sizeof(wchar_t),   // number of chars in string
              NULL,                     // buffer for new string
              0,                        // size of buffer
              NULL,                     // default for unmappable chars
              NULL                      // set when default char used
            );
        #endif

        if(iNeed<=0)
        {
            FC_LIBASSERT(GetLastError()!=ERROR_INSUFFICIENT_BUFFER);//should not happen...
            FC_LibFreeMemory(pTmp);
            CloseHandle(hFile);
            return -1; //TODO better error return 4?
        }

        if(!FC_LibNewMemory(ppTCHARBuffer, (iNeed+1)*sizeof(TCHAR)))//+1 we will 0 terminate anyway
        {
            CloseHandle(hFile);
            return -1;
        }
        
        iRet =
        #ifdef UNICODE
            MultiByteToWideChar(
              CP_ACP,                // code page
              0,                     // character-type options
              pTmp,                  // string to map
              dwSize,                // number of bytes in string
              *ppTCHARBuffer,        // wide-character buffer
              iNeed                  // size of buffer
        );
        #else
            WideCharToMultiByte(
              CP_ACP,                   // code page
              0,                        // performance and mapping flags
              pTmp,                     // wide-character string
              dwSize/sizeof(wchar_t),   // number of chars in string
              *ppTCHARBuffer,           // buffer for new string
              iNeed,                    // size of buffer
              NULL,                     // default for unmappable chars
              NULL                      // set when default char used
            );
        #endif
        FC_LibFreeMemory(pTmp);
        if(iNeed!=iRet)
        {
            FC_LIBASSERT(GetLastError()!=ERROR_INSUFFICIENT_BUFFER);//should not happen...
            FC_LibFreeMemory(ppTCHARBuffer);
            CloseHandle(hFile);
            return -1; //TODO better error return 4?
        }

        //set length in TCHARs:
        *pulLength = iNeed;
    }

    //null term, close file and return ok:
    (*ppTCHARBuffer)[*pulLength] = 0;
    CloseHandle(hFile);
    return 0;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_filetomem.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 22.10.01   Time: 19:35
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * first version of archive
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 26.03.01   Time: 11:45
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * bug fix in load bin file
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.02.01   Time: 10:32
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new functions in fc_fileIO.h, extra impl. c-file for string file name
 * stuff
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.01.01   Time: 11:03
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new fc_system.c
 *==
 *----------------------------------------------------------------------------*
*/
