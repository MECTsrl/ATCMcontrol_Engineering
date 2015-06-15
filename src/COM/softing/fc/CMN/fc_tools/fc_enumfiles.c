/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_enumfiles.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_enumfiles.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_enumfiles.c $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== Enumerate files callback implemention
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.03.02  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include <windows.h>
#include "fc_tools\fc_libmem.h"
#include "fc_tools\fc_string.h"
#include "fc_tools\fc_fileIO.h"


__inline int IsDotDotDir(const TCHAR* pszDir)
{
    return (pszDir[0]==_T('.') && pszDir[1]==0) ||
           (pszDir[0]==_T('.') && pszDir[1]==_T('.') && pszDir[2]==0);
}


static int FC_FileEnumFilesRec(
    TCHAR                fullPath[3*MAX_PATH],
    const TCHAR*         pszRelPath,
    long                 lFlags,
    FC_FileEnumCallBack  pCallBack,
    void*                pCtx,
    void*                pCtx2,
    int*                 pbCBStop
)
{
    int                iRetCB;
    int                bIsDir;
    HANDLE             hd;
    WIN32_FIND_DATA    findFileData;    
    TCHAR*             pszRest; 
    int                iLenRest;
    int                bRel;
    int                bSubDirs;


    bRel     = (lFlags & FC_FILE_ENUM_FULLPATH) == 0;
    bSubDirs = (lFlags & FC_FILE_ENUM_SUBDIRS) != 0;
    

    iLenRest = _tcslen(fullPath);
    pszRest  = fullPath + iLenRest;
    iLenRest = 3*MAX_PATH - iLenRest - 3 /* -3 to be safe.. */;
    
    if(iLenRest<=0 || !FC_StringCopyToBuffer(pszRest, iLenRest, _T("\\*")))
    {
        SetLastError(ERROR_BUFFER_OVERFLOW);
        return 0;
    }

    hd = FindFirstFile(fullPath, &findFileData);
    //remove the \* again:
    *pszRest = 0;

    if(hd == INVALID_HANDLE_VALUE)
    {
        pCallBack(pCtx, pCtx2, 1, 1, fullPath);
        return 0;
    }

    do
    {
        if(IsDotDotDir(findFileData.cFileName))
            continue;

        *pszRest = _T('\\');
        if(!FC_StringCopyToBuffer(pszRest+1, iLenRest-1, findFileData.cFileName))
        {
            FindClose(hd);
            SetLastError(ERROR_BUFFER_OVERFLOW);
            return 0;
        }
   
        bIsDir = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        
        if(bIsDir && !bSubDirs)
        {
            iRetCB = 1;
        }
        else
        {
            iRetCB = pCallBack(pCtx, pCtx2, bIsDir, 0, bRel ? pszRelPath+1 : fullPath);

            if(iRetCB<0)
            {
                *pbCBStop = 1;
                FindClose(hd);
                return 1;
            }
        }

        if(bIsDir && iRetCB>0 && bSubDirs)
        {
            if(!FC_FileEnumFilesRec(fullPath, pszRelPath, lFlags, pCallBack, pCtx, pCtx2, pbCBStop))
            {
                FindClose(hd);
                return 0;
            }
            if(*pbCBStop)
            {
                FindClose(hd);
                return 1;
            }

            //call cb for cd ..
            iRetCB = pCallBack(pCtx, pCtx2, -1, 0, _T(".."));
            if(iRetCB<0)
            {
                *pbCBStop = 1;
                FindClose(hd);
                return 1;
            }
        }

        //remove the \<file> again:
        *pszRest = 0;

    }while(FindNextFile(hd, &findFileData));

    return ERROR_NO_MORE_FILES==GetLastError() && FindClose(hd);
}


FC_TOAPI int FC_TODECL FC_FileEnumFiles(
    const TCHAR*         pszStartDir,
    long                 lFlags,
    FC_FileEnumCallBack  pCallBack,
    void*                pCtx,
    void*                pCtx2
)
{
    int          iLen;
    TCHAR        fullPath[3*MAX_PATH];
    const TCHAR* pszRelPath;
    int          bCBStop;

    if(pszStartDir)
    {
        iLen = FC_FileFullPath(NULL, pszStartDir, fullPath, 2*MAX_PATH);
    }
    else
    {
        iLen =  GetCurrentDirectory(2*MAX_PATH, fullPath);
    }

    if(iLen<=0)
        return 0;

    pszRelPath = fullPath + iLen;
    bCBStop = 0;
    return FC_FileEnumFilesRec(fullPath, pszRelPath, lFlags, pCallBack, pCtx, pCtx2, &bCBStop);
}





typedef struct _FList
{
    int     nFiles;
    int     nAlloc;
    TCHAR** pzpszList;
} FList;


static TCHAR* StringDup(const TCHAR* psz)
{
    int    size;
    TCHAR* pszCopy;

    size = (_tcslen(psz)+1)*sizeof(TCHAR);
    if(!FC_LibNewMemory((void**)&pszCopy,  size))
        return NULL;
    return (TCHAR*)memcpy(pszCopy, psz, size);
}


static int  MkFileListCallBack(
    void*         pCtx, 
    void*         pCtx2, 
    int           nIsDir,  // 0 pszFile is a file name
                           //+1 pszFile is a dir  name
                           //-1 pszFile is ".." to indicate cd ..
    int           bOnError,
    const TCHAR*  pszFile
)
{
    FList* pLst = (FList*)pCtx2;
    if(bOnError)
        return -1;
 
    if(nIsDir!=0)
        return 1;

    if(pCtx && !FC_StringMatchFilePattern(FC_StringGetFilePart(pszFile), (const TCHAR*)pCtx))
        return 1;

    if(pLst->nFiles+1 >= pLst->nAlloc)
    {
        if(!FC_LibResizeMemory((void**)&pLst->pzpszList, (pLst->nAlloc*2)*sizeof(TCHAR*)))
            return 0;

        pLst->nAlloc *=2;
    }

    pLst->pzpszList[pLst->nFiles++] = StringDup(pszFile);
    pLst->pzpszList[pLst->nFiles] = NULL;
    return 1;
}


FC_TOAPI TCHAR** FC_TODECL FC_FileBuildFileList(
    const TCHAR* pszStartDir,
    const TCHAR* pszPattern,
    long         lFlags
)
{
    FList lst; 
    int   iRet;


    lst.nFiles = 0;
    lst.nAlloc = 128;
    if(!FC_LibNewMemory((void**)&lst.pzpszList, lst.nAlloc*sizeof(TCHAR*)))
        return NULL;


    iRet = FC_FileEnumFiles(pszStartDir, lFlags, MkFileListCallBack, (void*)pszPattern, &lst);
    if(iRet==0)
    {
        lst.pzpszList[lst.nFiles] = NULL;
        FC_FileDeleteFileList(lst.pzpszList);
        lst.pzpszList = NULL;
        
    }

    return lst.pzpszList;
}





FC_TOAPI void FC_TODECL FC_FileDeleteFileList(
    TCHAR** pzpszFList
)
{
    TCHAR** pzpsz;
    if(pzpszFList)
    {
        for(pzpsz = pzpszFList; *pzpsz; pzpsz++)
            FC_LibFreeMemory(*pzpsz);

        FC_LibFreeMemory(pzpszFList);
    }
}





/*
 *----------------------------------------------------------------------------*
 *  $History: fc_enumfiles.c $
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
 * User: Ef           Date: 12.03.02   Time: 12:24
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * added build file list function
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 *==
 *----------------------------------------------------------------------------*
*/
