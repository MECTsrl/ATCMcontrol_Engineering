/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_fileIO.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_fileIO.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_fileIO.h $
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
 *==Helper functions for file and directory access
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_FILEIO_H_
#define FC_FILEIO_H_

#include <tchar.h>
#include "fc_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNICODE
 #define FC_ExistSubDir  FC_ExistSubDirW
 #define FC_CreateSubDir FC_CreateSubDirW
 #define FC_Exist        FC_ExistW
#else
 #define FC_ExistSubDir  FC_ExistSubDirA
 #define FC_CreateSubDir FC_CreateSubDirA
 #define FC_Exist        FC_ExistA
#endif


// RETURN VALUE : TRUE : SubDir exists
FC_TOAPI int FC_TODECL FC_ExistSubDirA(const char* lpDirectory);
FC_TOAPI int FC_TODECL FC_ExistSubDirW(const wchar_t* lpDirectory);

// RETURN VALUE : TRUE : SubDir created successfully
FC_TOAPI int FC_TODECL FC_CreateSubDirA(const char* lpDirectory);
FC_TOAPI int FC_TODECL FC_CreateSubDirW(const wchar_t* lpDirectory);

// RETURN VALUE : TRUE  : File exists
FC_TOAPI int FC_TODECL FC_ExistA(const char* lpszFile);
FC_TOAPI int FC_TODECL FC_ExistW(const wchar_t* lpszFile);






//------------------------------------------------------------------*
/**
 * FC_FileFullPath creates a full path from pszFile and pszPath.
 * This function is a small wrapper for win32 GetFullPathName but 
 * does not change the process current working directory.
 *
 * If pszFile is absolute or pszPath is NULL pass pszFile to 
 * GetFullPathName, which uses the current  working directory if 
 * pszFile is not absolute.
 * If pszFile is relative pszPath and pszFile are concatinated
 * textually seperated by a \ and the result is passed to win32
 * GetFullPathName. 
 * The result is also the canonical form of the file name, means
 * all ..\///.\  are resolved. 
 * The function cannot handle files longer than 2*MAX_PATH, 
 * even if the \\? syntax used.
 *
 * @param     pszFile: file name with opt. relative or full path
 * @param     pszPath: NULL or an absolute path
 * @param     szFull:  buffer for resulting full canonical path 
 * @param     nBuffer: size of buffer in TCHARs
 * @return    >0   the lstrlen of the final result
 *            =0   the buffer was to short (the buffer may now
 *                 contain invalid and possibly not null terminated 
 *                 text)
 * @see       win32 GetFullPathName
 */
FC_TOAPI int FC_TODECL FC_FileFullPath(
    const TCHAR* pszPath,
    const TCHAR* pszFile, 
    TCHAR        szFull[/*nBuffer*/],
    int          nBuffer
);





//------------------------------------------------------------------*
/**
 * FC_FileExists checks if a file exist. Optionally a path can be 
 * specified that is used if the file is not absolute.
 * If the function returns true the file is a directory too, 
 * it's only checked that win32 GetFileAttributes returns != -1.
 *
 * @param    pszPath: NULL: use current working directory, or it
 *                    should be a full path (if it's not a full
 *                    path pszPath gets appended to the current 
 *                    working directory)
 * @param    pszFile: The file name to test for existence, may 
 *                    be full or relative.
 * @return            0: file does not exist 
 *                    1: file exists
 *                    2: file exists but is directory
*/
FC_TOAPI int FC_TODECL FC_FileExists(
    const TCHAR*   pszPath,
    const TCHAR*   pszFile 
);







//------------------------------------------------------------------*
/**
 * FC_LoadDiskBinFileToMem loads a file binary into memory.
 * The file is opened and read usinf win32 OpenFile and ReadFile flags:
 * CreateFile(pszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
 *                        OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
 *
 * The calling DLL or EXE must define FC_LibNewMemory and free memory 
 * using the compatible FC_LibFreeMemory function. (see inc\fc_tools\readme.txt)
 *
 * @param    pszFileName  : The file name to be opened, must be disk file.     
 * @param    ppByteBuffer : receives a newly allocated  buffer of size *pulLength. 
 *                          Note the special case if the file has size 0: then 
 *                          *ppByteBuffer will also be allocated (with 1 byte) 
 *                          and must therefore be also freed.
 * @param    pulLength    : IN/OUT on input: if *pulLength>0 *pulLength is the 
 *                          maximum file size in bytes allowed, files larger than 
 *                          *pulLength are treated as an error (return code 3)
 *                          On output: receives the size of the file in bytes
 *                          even if return code is 3 (but if the file is larger 
 *                          than 4GB *pulLength is set to 0).
 * @param    pftLastWrite : if not NULL receives the last write file time 
 *                          of the file (usefull if you need to know exactly 
 *                          the last modified time when file was read to mem).
 * @return                0: no error
 *                        1: file does simply not exist.
 *                        2: file cannot be opened or read (possibly missing permission)
 *                           Use win32 GetLastError() for more information.
 *                        3: file is larger than *pulLength (>0)
 *                       -1: Out of memory
*/
FC_TOAPI int FC_TODECL FC_LoadDiskBinFileToMem(
    const TCHAR*   pszFileName, 
    void**         ppByteBuffer, 
    unsigned long* pulLength, 
    __int64*       pftLastWrite
);

//------------------------------------------------------------------*
/**
 * FC_LoadDiskTextFileToMem loads a text file into memory, text file
 * means that the caller will interpret the file content as a text file.
 * Note: the \r\n combinations are not removed.
 * If the file is a UNICODE file (this is assumed only if the byte size 
 * is even and the file starts with 0xFEFF) the starting 0xFEFF is skipped
 * and #ifndef UNICODE the rest is converted to multibyte. 
 * #ifdef UNICODE only the 0xFEFF is skipped the rest is simply copied
 * to the wchar_t* output buffer. The output buffer will in any case
 * be terminated with a 0-TCHAR, so it can be further processed as 
 * a normal null terminated string.
 * The file is opened and read usinf win32 OpenFile and ReadFile flags:
 * CreateFile(pszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,
 *                        OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
 *
 * The calling DLL or EXE must define FC_LibNewMemory and free memory 
 * using the compatible FC_LibFreeMemory function. (see inc\fc_tools\readme.txt)
 *
 * @param    pszFileName  : The file name to be opened, must be disk file.     
 * @param    ppTCHARBuffer: receives a newly allocated TCHAR* buffer of size 
 *                          *pulLength+1. The last TCHAR is not part of the file, 
 *                          it's an always added terminating 0-char (even if 
 *                          file has size 0).
 * @param    pulLength    : IN/OUT on input: if *pulLength>0 *pulLength is the 
 *                          maximum file size in BYTES(!) allowed, files larger than 
 *                          *pulLength are treated as an error (return code 3)
 *                          On output: receives the size of the file in TCHARs
 *                          even if return code is 3 (but if the file is larger 
 *                          than 4GB *pulLength is set to 0).
 * @param    pftLastWrite : if not NULL receives the last write file time 
 *                          of the file (usefull if you need to know exactly 
 *                          the last modified time when file was read to mem).
 * @return                0: no error
 *                        1: file does simply not exist.
 *                        2: file cannot be opened or read (possibly missing permission)
 *                           Use win32 GetLastError() for more information.
 *                        3: file is larger than *pulLength (>0)
 *                        4: Unicode to Multibyte conversion error (error currently not supported)
 *                       -1: Out of memory
*/
FC_TOAPI int FC_TODECL FC_LoadDiskTextFileToMem(
    const TCHAR*   pszFileName, 
    TCHAR**        ppTCHARBuffer, 
    unsigned long* pulLength,    
    __int64*       pftLastWrite
);







//------------------------------------------------------------------*
/**
 * FC_FileGetFileList call back function to be used from FC_FileEnumFiles.
 *
 * @param   pCtx       user defined context information.
 * @param   pCtx2      user defined context information.
 * @param   bIsDir     true if pszFile is a directory.
 * @param   bOnError   true if error reading the directory info
 *                     then pszFile is the directory (always with full path) 
 *                     that could not be read. Use GetLastError() for more info,
 *                     after an error the enumeration stops.
 * @param   pszFile    the file/directory name relative to start directory
 *                     or with full path if FC_FILE_ENUM_FULLPATH was set.
 *
 * @return  >0: continue with next file
 *          =0: if bIsDir was true in this call skip this directory
 *              and continue enumeration with next file or folder if any.
 *          <0: stop enumeration.
 * @see     FC_FileEnumFiles
*/
typedef  int  (*FC_FileEnumCallBack)(
    void*         pCtx, 
    void*         pCtx2, 
    int           nIsDir,  // 0 pszFile is a file name
                           //+1 pszFile is a dir  name
                           //-1 pszFile is ".." to indicate cd ..
    int           bOnError,
    const TCHAR*  pszFile
);


#define FC_FILE_ENUM_SUBDIRS     0x001   //recurse into subdirectories
#define FC_FILE_ENUM_FULLPATH    0x002   //call call back with full path
                                         //otherwise relative to start dir.
//------------------------------------------------------------------*
/**
 * FC_FileEnumFiles can be used to enumerate disk files.
 * Note:   if pszStartDir does not exits the call back is called
 *         with bOnError==true and GetLastError() will be ERROR_PATH_NOT_FOUND.
 * 
 * @param   pszStartDir  !=NULL: where to start the enumeration, 
 *                         NULL: use current working dir
 * @param   lFlags         bit set of FC_FILE_ENUM_* bits (see above)
 * @param   pCallBack      user defined call back
 * @param   pCtx           user defined context information for call back.
 *
 * @return  true:  no IO error
 *          false: any IO error
 *
 * @see     FC_FileEnumFiles
*/
FC_TOAPI int FC_TODECL FC_FileEnumFiles(
    const TCHAR*         pszStartDir,
    long                 lFlags,
    FC_FileEnumCallBack  pCallBack,
    void*                pCtx,
    void*                pCtx2
);


/**
 * FC_FileGetFileList can be used to build a NULL terminated list of
 * disk files.
 * 
 * @param   pszStartDir  !=NULL: where to start the enumeration, 
 *                         NULL: use current working dir
 * @param   pszPattern    NULL or a win32 file pattern using *? wildcards
 *                        if not NULL only files that match the pattern are
 *                        put into the list.
 * @param   lFlags        bit set of FC_FILE_ENUM_* bits (see above)
 *
 * @return  NULL:  no IO error 
 *          not NULL: a NULL terminated list of 0-terminated string.
 *          The list must be freed with FC_FileDeleteFileList
 *
 * @see     FC_FileEnumFiles
*/
FC_TOAPI TCHAR** FC_TODECL FC_FileBuildFileList(
    const TCHAR* pszStartDir,
    const TCHAR* pszPattern,
    long         lFlags
);

FC_TOAPI void FC_TODECL FC_FileDeleteFileList(
    TCHAR** pzpszFList
);



//------------------------------------------------------------------*
/**
 * FC_FileIsReadOnly checks the read only file (or directory) flag. 
 *
 * @param            pszFile: file/directory name including full path
 * 
 * @return           0:  the read only bit is not set. 
 *                   1:  the read only bit is set. 
 *                  -1:  this function failed, typically the file
 *                       does not exist (see GetLastError)
*/
FC_TOAPI int FC_TODECL FC_FileIsReadOnly(const TCHAR* pszFile);

//------------------------------------------------------------------*
/**
 * FC_FileSetReadOnly sets the read only file (or directory) flag
 * regardless of it's previous value.
 *
 * @param            pszFile: file/directory name including full path
 * @param            bReadOnly: TRUE set the flag
 * @param            bReadOnly: FALSE remove the flag
 * @return           0:  this function failed (see GetLastError)
 *                   1:  success
*/
FC_TOAPI int FC_TODECL FC_FileSetReadOnly(
    const TCHAR* pszFile, 
    int          bReadOnly
);


//------------------------------------------------------------------*
/**
 * FC_FileGetSize gets the file size in byte of a named file.
 * TODO: current implementaion cannot handle file larger 2^32 byte
 *
 * @param            pszFile: file name 
 * @return           -1: file does not exist or any other error
 *                   >0: file size.
*/
__int64 FC_FileGetSize(const TCHAR* pszFile);

//------------------------------------------------------------------*
/**
 * FC_FileGetLastWriteTime gets the file's last write access time 
 * stamp as file time using 64 bit integer.
 *
 * @param            pszFile: file name 
 * @return           0 : file does not exist or any other error
 *                   >0: file time of last write access.
*/
__int64 FC_FileGetLastWriteTime(const TCHAR* pszFile);



//------------------------------------------------------------------*
/**
 * FC_FileLastWrt gets the latest file write access time 
 * stamp as file time using 64 bit integer. This functions scans all
 * files that match the pattern pszPattern recursively starting
 * at the directory pszStartDir. 
 * The function does not scan folders that match the pszExcludeDirs pattern 
 * and does not consider files that match the pszExcludeFiles pattern. 
 * The function does only scan folders that match the pszIncludeDirs pattern 
 * and does only consider files that match the pszIncludeFiles pattern. 
 *  
 * The patterns may contain *? wildcards. The patterns may contain alternatives 
 * seperated by '|': Example "*.c|*.h".
 *
 * @param  pszStartDir:      Path to start directory.
 * @param  pszIncludeDirs:   NULL or "*": All subdir names are scaned.
 *                           not NULL:  : Recurse only into directories match this pattern.
 * @param  pszIncludeFiles:  NULL or "*": All files are scaned.
 *                           not NULL:  : Consider only files that match this pattern.
 * @param  pszExcludeDirs:   NULL       : Recurse into all sub directories.
 *                           not NULL   : Recurse only into directories that do NOT match 
 *                                        this pattern. Tip: "*" can be used to specify a 
 *                                        non recursive scan.
 * @param  pszExcludeFiles:  NULL or "*": Consider all files.
 *                           not NULL:  : Consider only files that match this pattern.
 *
 * @param  pszFileName:      If not NULL: Receives the file name (and path) with the highest 
 *                                        (latest) file write time found.
 * @param  nFileName:                     Size in TCHARs of pszFileName. If pszFileName
 *                                        is not NULL nFileName must at least >= _MAX_PATH.
 * @param  pFTLatestWrt:     If not NULL: Receives the latest write file time to any one
 *                                        of the matched files.
 *                                        Special case: if pszStartDir is an empty directory, 
 *                                        or contains only also empty sub directories, *pFTLatestWrt is
 *                                        will be 0 and the return value is TRUE (note: the write file 
 *                                        time of 'direcotries' is platform dependent, and therefore 
 *                                        not at all used by this function.
 *
 * @return FALSE(0): Any error file IO error, use GetLastError() for more information.
 *         TRUE (1): Function completed successfully.
 *
 * @see    FC_StrMatchI
*/
FC_TOAPI int FC_TODECL FC_FileLastWrt(
    const TCHAR* pszStartDir,
    const TCHAR* pszIncludeDirs,
    const TCHAR* pszIncludeFiles,
    const TCHAR* pszExcludeDirs,
    const TCHAR* pszExcludeFiles,
    TCHAR*       pszFileName,
    long         nFileName,
    __int64*     pFTLatestWrt
);


//------------------------------------------------------------------*
/**
 * delete directory.
 *
 *  Delete directory and all contained files and subdirectories.
 *
 *  CAUTION:
 *  Ensure that your path is correct !!! Undo not possible.
 *
 * @param            pszDirectory: directory name including full path
 * @return           successful?
*/
FC_TOAPI int FC_TODECL FC_DeleteDirectory(const TCHAR* pszDirectory);




#define FC_FILE_DEL_MAKEWRT    0x001 //make file writable, this will delete
                                     //also files/folders that are marked 'read only'
#define FC_FILE_DEL_ONLYFILES  0x002 //do not remove the directories

//------------------------------------------------------------------*
/**
 * delete directory ex.
 *
 *  Delete directory and all contained files and subdirectories.
 *  
 *  CAUTION:
 *  Ensure that your path is correct !!! Undo not possible.
 *
 *
 * @param     pszDirectory: directory name including full path
 * @param     lFlags: flags FC_FILE_DEF_* that control the delete action 
 *               FC_FILE_DEL_MAKEWRT:   make file writable, this wil delete
 *                                      also files/folders that are marked 
 *                                      read only.
 *               FC_FILE_DEL_ONLYFILES: do not remove the directories.
 * 
 * @return    1: successful, 0: not successful.
*/
FC_TOAPI int FC_TODECL FC_DeleteDirectoryEx(
    const TCHAR* pszDirectory,
    long         lFlags
);




typedef struct _FC_CSVField
{
    long         lLineNo;
    long         lLength;
    TCHAR*       pszField;
} FC_CSVField;

typedef struct _FC_CSVRecord
{
    long         nFields;  //always >=1, a record has at least one field
    FC_CSVField  fields[1];//size_is(nFields)
} FC_CSVRecord;

typedef struct _FC_CSVMemFile
{
    long           lErrReason;
    long           lErrLine;
    long           nRec;
    FC_CSVRecord** ppRec;
} FC_CSVMemFile;

FC_TOAPI FC_CSVMemFile* FC_TODECL FC_CSVSplitEx(
    TCHAR*         pszFileContent,  //CSV file content
    long           lFileChars,      //number of TCHARs of pszFileContent 
    TCHAR          chSeperator,     //either ';' or ','
    int            bTrimAllFields,  //if TRUE all fields are trimmed on both sides
    int            bHashIsComment   //if TRUE and the first non white 
                                    //space char in the fist field is a '#' 
                                    //then skip this record
); 

FC_TOAPI FC_CSVMemFile* FC_TODECL FC_CSVSplit(
    TCHAR*         pszFileContent, 
    long           lFileChars
); 


FC_TOAPI void FC_TODECL FC_CSVFreeMemFile(FC_CSVMemFile* pMem);









#ifdef __cplusplus
}
#endif

#endif




/*
 *----------------------------------------------------------------------------*
 *  $History: fc_fileIO.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.11.06   Time: 20:44
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 16  *****************
 * User: Ef           Date: 21.01.03   Time: 15:26
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions
 * 
 * *****************  Version 15  *****************
 * User: Ef           Date: 12.06.02   Time: 16:51
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new file functions
 * 
 * *****************  Version 14  *****************
 * User: Ef           Date: 12.03.02   Time: 12:24
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * added build file list function
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 11.03.02   Time: 15:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * removed dead code
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 28.01.02   Time: 17:52
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * some new functions
 * 
 * *****************  Version 11  *****************
 * User: Sis          Date: 9.01.02    Time: 15:46
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_DeleteDirectory()
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 22.10.01   Time: 19:37
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * updated source comments
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 30.08.01   Time: 11:03
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * bOnError
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 29.08.01   Time: 22:11
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * better file enum
 * 
 * *****************  Version 7  *****************
 * User: Die          Date: 27.08.01   Time: 19:33
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 31.03.01   Time: 19:22
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 20.03.01   Time: 11:50
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changed order of  path,file inputs in FC_FileExists and renamed to
 * FC_FileFullPath
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.02.01   Time: 10:27
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions in fc_fileIO.h
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.01.01   Time: 11:01
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fc_System.h, some minor changes
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:08
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * from V1.3 tools.dll, and some new features
 *==
 *----------------------------------------------------------------------------*
*/
