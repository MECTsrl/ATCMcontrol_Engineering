#include <stdio.h>
#include "cgt_misc.h"
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_registry.h"
#include "utilif.h"



extern "C" bool CGT_DeleteFile(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszDir,
    const TCHAR*  pszFile
)
{
    TCHAR buffer[2*MAX_PATH];

    if(pszDir)
    {
        FC_FileFullPath(pszDir, pszFile, buffer, FC_ARRAY_LEN(buffer));
        pszFile = buffer;
    }


    if(FC_Exist(pszFile) && !DeleteFile(pszFile))
    {
        pdm->msg1(CG_E_FILE_DELETE, NULL, pszFile);
        return false;
    }

    return true;
}


extern "C" bool CGT_CopyFile(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszSrcDir,
    const TCHAR*  pszSrc,
    const TCHAR*  pszDstDir,
    const TCHAR*  pszDst
)
{
    bool  bReTry;
    TCHAR srcBuff[2*MAX_PATH];
    TCHAR dstBuff[2*MAX_PATH];
    TCHAR dirBuff[2*MAX_PATH];

    if(pszSrcDir)
    {
        FC_FileFullPath(pszSrcDir, pszSrc, srcBuff, FC_ARRAY_LEN(srcBuff));
        pszSrc = srcBuff;
    }
    if(pszDstDir)
    {
        FC_FileFullPath(pszDstDir, pszDst, dstBuff, FC_ARRAY_LEN(dstBuff));
        pszDst = dstBuff;
    }


    bReTry = true;
retry:
    if(!CopyFile(pszSrc, pszDst, FALSE))
    {
        if(bReTry)
        {
            _tcscpy(dirBuff, pszDst);
            FC_StringCutFilePart(dirBuff);
            FC_CreateSubDir(dirBuff);
            bReTry = false;
            goto retry;
        }
        pdm->msg2(CG_E_FILE_COPY, NULL, pszSrc, pszDst);
        return false;
    }

    return true;
}


inline bool IsDotDotDir(const TCHAR* pszDir)
{
    return (pszDir[0]==_T('.') && pszDir[1]==0) ||
           (pszDir[0]==_T('.') && pszDir[1]==_T('.') && pszDir[2]==0);
}


extern "C" bool CGT_DeleteFiles(
    CGT_CDiagMsg*pdm,
    const TCHAR* pszDir,
    const TCHAR* pszPattern,
    const TCHAR* pszExcFiles,
    bool         bSubDirs,
    const TCHAR* pszExcDirs
)
{
    bool            bOk;
    HANDLE          hd;
    HRESULT         hr;
    WIN32_FIND_DATA findFileData;    
    FC_CString      dir(MAX_PATH);
    FC_CString      file(MAX_PATH);


    if(pszDir && !FC_StringIsAbsPath(pszPattern))
        file.appendDir(pszDir);

    file.appendDir(pszPattern);

    dir.load(file).stripFilePart();


    //force absolute path:
    if(!FC_StringIsAbsPath(file))
    {
        CGT_IERR(pdm, _T("CGT_DeleteFiles called for relative path")); 
        return false;
    }


    //be carefull !
    //don't allow * pattern unless looks at least like gen dir:
    if(_tcschr(file, _T('*')) && !_tcsstr(file, _T("\\gen\\")))
    {
        CGT_IERR(pdm, _T("CGT_DeleteFiles called with '*' pattern but path contains no '\\gen\\'")); 
        return false;
    }

    //don't allow recursive unless looks at least like gen dir:
    if(bSubDirs && !_tcsstr(file, _T("\\gen\\")))
    {
        CGT_IERR(pdm, _T("CGT_DeleteFiles called with bSubDirs==true but path contains no '\\gen\\'")); 
        return false;
    }

    
    //dirty work:
    hd = FindFirstFile(file, &findFileData);
    if(hd == INVALID_HANDLE_VALUE)
    {
        hr = GetLastError();
        bOk = hr==ERROR_FILE_NOT_FOUND || hr==ERROR_PATH_NOT_FOUND; 
        if(!bOk)
            pdm->sysErr(GetLastError(), NULL, _T("FindFirstFile in folder:"), dir);
        return bOk;
    }
    
    if((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
    {
        if(!pszExcFiles || !FC_StringMatchFilePattern(findFileData.cFileName, pszExcFiles))
        {
            file.load(dir).appendDir(findFileData.cFileName);
            if(!DeleteFile(file))
            {
                pdm->msg1(CG_E_FILE_DELETE, NULL, file);
                goto Error;
            }
        }
    }
    else if(bSubDirs && !IsDotDotDir(findFileData.cFileName))
    {
        if(!pszExcDirs || !FC_StringMatchFilePattern(findFileData.cFileName, pszExcDirs))
        {
            dir.appendDir(findFileData.cFileName);
            bOk = CGT_DeleteFiles(pdm, dir, FC_StringGetFilePart(pszPattern), 
                                  pszExcFiles, bSubDirs, pszExcDirs
                  );
            dir.stripFilePart();
            if(!bOk)
               goto Error;
        }
    }
    
    while(FindNextFile(hd , &findFileData))
    {
        if((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
        {
            if(!pszExcFiles || !FC_StringMatchFilePattern(findFileData.cFileName, pszExcFiles))
            {
                file.load(dir).appendDir(findFileData.cFileName);
                if(!DeleteFile(file))
                {
                    pdm->msg1(CG_E_FILE_DELETE, NULL, file);
                    goto Error;
                }
            }
        }
        else if(bSubDirs && !IsDotDotDir(findFileData.cFileName))
        {
            if(!pszExcDirs || !FC_StringMatchFilePattern(findFileData.cFileName, pszExcDirs))
            {
                dir.appendDir(findFileData.cFileName);
                bOk = CGT_DeleteFiles(pdm, dir, FC_StringGetFilePart(pszPattern), 
                                  pszExcFiles, bSubDirs, pszExcDirs
                           );
                dir.stripFilePart();
                if(!bOk)
                   goto Error;
            }
        }
    }

    bOk = ERROR_NO_MORE_FILES == GetLastError();
    if(!FindClose(hd))
        bOk = false;

    if(!bOk)
        pdm->sysErr(GetLastError(), NULL, _T("delete files in folder:"), dir);

    return bOk;


Error: //must free find handle
    FindClose(hd);
    return false;
}







extern "C" bool CGT_WriteFile(
    CGT_CDiagMsg*pdm,
    const TCHAR* pszPath,
    const TCHAR* pszFile,
    const void*  pBuffer, 
    long         nBytes, 
    long         flags
)
{
    FILE*  pF;
    int    iItems;
    TCHAR  full[2*MAX_PATH];
	bool   bFirstTry;
	TCHAR  mode[3];

    if(pszPath && !FC_StringIsAbsPath(pszFile))
        _tmakepath(full, NULL, pszPath, pszFile, NULL);
    else
        lstrcpy(full, pszFile);

    assert(lstrlen(full) < FC_ARRAY_LEN(full));
    //assert: must now be full path:
    assert(FC_StringIsAbsPath(full));

	if(flags & CGT_WF_TOLOWER)
	    _tcslwr(full);

    bFirstTry = true;

	mode[0] = (flags & CGT_WF_APPEND)   ? _T('a') : _T('w');
	mode[1] = (flags & CGT_WF_TEXTMODE) ? _T('t') : _T('b');
    mode[2] = 0;


tryAgain:
    iItems =0;
    pF = _tfopen(full, mode);
    if(pF)
    {
        iItems = fwrite(pBuffer, 1, nBytes, pF);
        fclose(pF);
        if(iItems!=nBytes)
            pdm->msg1(CG_E_WRITE_FILE_FAILED, NULL, full);
    }
    else if(bFirstTry && (flags & CGT_WF_CRT_PATH))
    {
        FC_CString jot;
        bFirstTry = false;

        //here we create the folder if not exists
        //assume that the error was due to non existent folder...
        jot.load(full).stripFilePart();

        if(FC_CreateSubDir(jot))
            goto tryAgain;
        else
            pdm->msg1(CG_E_MKDIR_FAILED, NULL, jot);//TODO should be fatal error.
    }
    else
    {
        FC_CString jot;
        pdm->msg2(CG_E_FILE_OPEN, NULL, full, 
            jot.fmtSysErr(GetLastError()));
    }

    return iItems==nBytes;
}



bool CGT_DmpToFile(
    CGT_CDiagMsg*   pdm, 
    const TCHAR*    pszPath,  //optional can be NULL
    const TCHAR*    pszFile,  //must be ful path if pszPath is NULL
    const CGT_CDmp& dmp, 
    bool            bAppend
)
{
    const TCHAR* pChars;
    long         nChars;
    long         flags;

    pChars = dmp.getPChars();
    nChars = dmp.getNChars();
    flags  = CGT_WF_STD;
    if(bAppend)
        flags |= CGT_WF_APPEND;

    return CGT_WriteFile(pdm, pszPath, pszFile, pChars, sizeof(TCHAR)*nChars, flags);
}


extern "C" bool CGT_LoadTextFileToMem(
    CGT_CDiagMsg*       pdm,
    const CG_Edipos*    pEdp,
    const TCHAR*        pszPath,
    const TCHAR*        pszFile, 
    TCHAR**             ppCharBuffer, 
    unsigned long*      pulLength, 
    __int64*            pftLastWrite
)
{
    int           iRet;
    unsigned long ulen;
    TCHAR         buffer[2*MAX_PATH];
    const TCHAR*  pszFull;

    if(FC_StringIsAbsPath(pszFile))
    {
        pszFull = pszFile;
    }
    else
    {
        _tmakepath(buffer, NULL, pszPath, pszFile, NULL);
        assert(lstrlen(buffer) < FC_ARRAY_LEN(buffer));
        pszFull = buffer;
    }

    //assert: must now be full path:
    assert(FC_StringIsAbsPath(pszFull));



    //printf("load->%s\r\n",  (const char*)full);

    ulen = CGT_MAXMEM_FILE_SIZE;
    iRet = FC_LoadDiskTextFileToMem(
            pszFull, 
            ppCharBuffer, 
            &ulen,    
            pftLastWrite
           );
    if(pulLength)
        *pulLength = ulen;

    /*
        0: no error
        1: file does simply not exist.
        2: file cannot be opened or read (possibly missing permission)
          Use win32 GetLastError() for more information.
        3: file is larger than *pulLength (>0)
        4: Unicode to Multibyte conversion error (error currently not supported)
        -1: Out of memory
    */
    switch(iRet)
    {
    case  0: 
    break;
    case -1: 
        pdm->msg0(CG_E_OUTOFMEM, pEdp); 
    break;
    case  1: 
        pdm->msg1(CG_E_FILE_NOT_EXIST, pEdp, pszFull); 
    break;
    case  2: 
    {
        FC_CString jot;
        pdm->msg2(CG_E_FILE_OPEN, pEdp, pszFull, jot.fmtSysErr(GetLastError())); 
    }
    break;
    case  3: 
        pdm->msg1(CG_E_FILE_TO_LARGE, pEdp, pszFull); 
    break;
    case  4: 
    default:
        assert(!"unexpected return");
    }
 
    return iRet==0;
}



extern "C" bool CGT_GetLIBFileData(
    CGT_CDiagMsg*    pdm,          //for err msg
    I4CLAccess*      pILib,        //4CL interface 
    const TCHAR*     pszPOUname,   //must be given 
    const TCHAR*     pszTRGname,   //can be NULL or empty string
    const TCHAR*     pszFILEname,  //must be given
    TCHAR**          ppCharBuffer, //NULL or newly allocated buffer for file content
    unsigned long*   pulLength,    //NULL or receives the size in TCHARs of
                                   //*ppCharBuffer without the always
                                   //added terminating 0-TCHAR.
    __int64*         pftLastWrite  //NULL or receives the last file write time
)
{
    BSTR        sPOU;
    BSTR        sTRG;
    BSTR        sFILE;
    HRESULT     hr;
    
   
    sPOU  = FC_StringToBSTR(pszPOUname);
    sTRG  = (pszTRGname && pszTRGname[0]) ? FC_StringToBSTR(pszTRGname) : NULL;
    sFILE = FC_StringToBSTR(pszFILEname);

    if(ppCharBuffer)
        *ppCharBuffer = NULL;
    if(pulLength)
        *pulLength = 0;

    if(pftLastWrite)
    {
        hr = pILib->GetLastModifiedFileTime(pftLastWrite);
        assert(SUCCEEDED(hr));
    }

    hr = pILib->ExistsFile(sPOU, sTRG, sFILE, pulLength);
    if(FAILED(hr) || (hr==S_FALSE && ppCharBuffer))
        goto Error;

    if(!ppCharBuffer)
        goto Ende;//caller wants only to test for existence


#ifdef UNICODE
#error rethink and rework the code
#endif
    *ppCharBuffer = new TCHAR[*pulLength+1];
    hr = pILib->GetFileRaw(sPOU, sTRG, sFILE, (unsigned char*)*ppCharBuffer, *pulLength);//HACK
    if(hr!=S_OK)
        goto Error;
    (*ppCharBuffer)[*pulLength] = 0;

    goto Ende;


Error: 
    {
    BSTR       sLIB;
    FC_CString tmp(MAX_PATH);
    
    tmp<<pszPOUname<<_T("\\");
    if(pszTRGname)
        tmp<<pszTRGname<<_T("\\");
    tmp<<pszFILEname;
    
    pILib->GetLibFileName(&sLIB);

    pdm->msg2(CG_E_READ_LIB_DOMAIN, NULL, tmp, pdm->jot1()<<sLIB);
    FC_FREE_BSTR(sLIB);        
    }

Ende:
    FC_FREE_BSTR(sPOU);        
    FC_FREE_BSTR(sTRG);        
    FC_FREE_BSTR(sFILE);        
    
    return hr==S_OK;
}





extern "C" long CGT_LIBCheckOutFiles(
    CGT_CDiagMsg*        pdm,           //for err msg
    ICG_IECFront*        pIFront,       //used to get 4cl COM IF by context info
    const CG_LIBContext* pLibCtx,       //4CL context info
    const TCHAR*         pszPOUname,    //must be given 
    const TCHAR*         pszTRGname,    //can be NULL or empty string
    const TCHAR*         pszFILEname,   //must be given
    const TCHAR*         pszCheckOutDir //dest directory
)
{
    I4CLAccess* pILib;
    BSTR        sPOU;
    BSTR        sTRG;
    BSTR        sFILE;
    BSTR        sDIR;
    BSTR        sERR;
    HRESULT     hr;
    long        lNum;
    SAFEARRAY*  parrFiles;

  
    pILib = pIFront->get4CLLibIF(pLibCtx);
    if(!pILib)
    {
        CGT_IERR2(pdm, &pLibCtx->edp, pdm->jot1()
          <<_T("CGT_LIBCheckOutFiles: failed to get library interface by context info '")
          <<pLibCtx->pszId<<_T("'")
        );
        return 0;
    }
   
    sPOU  = FC_StringToBSTR(pszPOUname);
    sTRG  = (pszTRGname && pszTRGname[0]) ? FC_StringToBSTR(pszTRGname) : NULL;
    sFILE = FC_StringToBSTR(pszFILEname);
    sDIR  = FC_StringToBSTR(pszCheckOutDir);
    sERR  = NULL;
    
    hr = pILib->CheckOut(sPOU, sTRG, sFILE, sDIR, &parrFiles, &sERR);

    lNum = 0;
    if(parrFiles)
    {
        lNum = parrFiles->rgsabound[0].cElements;
        SafeArrayDestroy(parrFiles);
    }

    if(FAILED(hr) || lNum==0)
    {
        BSTR       sLIB;
        FC_CString tmp(MAX_PATH);
    
        tmp<<pszPOUname<<_T("\\");
        if(pszTRGname)
            tmp<<pszTRGname<<_T("\\");
        tmp<<pszFILEname;
    
        pILib->GetLibFileName(&sLIB);

        pdm->msg2(CG_E_READ_LIB_DOMAIN, NULL, tmp, pdm->jot1()<<sLIB);
        pdm->userMsg("    Reason: ");
        pdm->userMsg(pdm->jot1()<<sERR);
        pdm->userMsg("\r\n");

        FC_FREE_BSTR(sLIB);        
    }


    FC_FREE_BSTR(sPOU);        
    FC_FREE_BSTR(sTRG);        
    FC_FREE_BSTR(sFILE);
    FC_FREE_BSTR(sDIR);
    FC_FREE_BSTR(sERR);
    pILib->Release();

    return lNum;
}


extern "C" long CGT_SplitFileLocator(
    const TCHAR*     pszFileLocator, 
    CGT_FileLocator* pSplitted
)
{
    int          i;
    int          iLen;
    const TCHAR* psz;


    psz = _tcsstr(pszFileLocator, _T("::"));
    if(!psz)
    {
        if(pSplitted)
        {
            iLen = lstrlen(pszFileLocator);
            if(iLen >= 2*MAX_PATH)
                return 0;

            pSplitted->bIsLib = false;
            memcpy(pSplitted->pszFile, pszFileLocator, (iLen+1)*sizeof(TCHAR));
            pSplitted->pszPOUName[0] = 0;
            pSplitted->pszTrgType[0] = 0;
            pSplitted->pszLibFile[0] = 0;
        }
            
        return 1;
    }
    else if(!pSplitted)
    {
        //C:\4c2\Engineering\lib\4C_IEC1131_3.4cl::MUX\[opt trg\]MUX.hdr
        psz = _tcschr(psz, _T('\\'));
        assert(psz);
        if(!psz)
            return 0;

        psz = _tcsinc(psz);
        psz = _tcschr(psz, _T('\\'));
        if(psz)
        {
            assert(!_tcschr(psz+1, _T('\\')));
            return 3; //has target.
        }

        return 2; //has no target.
    }


    //pSplitted not NULL and is lb location, get the individual parts:
    pSplitted->bIsLib = true;
    pSplitted->pszFile   [0] = 0;
    pSplitted->pszPOUName[0] = 0;
    pSplitted->pszTrgType[0] = 0;
    pSplitted->pszLibFile[0] = 0;



    //get the library file name:
    iLen = (psz - pszFileLocator);
    if(iLen >= 2*MAX_PATH)
        return 0;
    memcpy(pSplitted->pszFile, pszFileLocator, iLen*sizeof(TCHAR));
    pSplitted->pszFile[iLen] = 0;

    //asset: mst be ext 4cl:
    assert(!lstrcmpi(FC_StringGetFileExt(pSplitted->pszFile), CG_PREDEF_LIB_EXT));

    //test max length of locator inside lib, should not be longer than MAX_PATH:
    iLen = lstrlen(psz);
    if(iLen >= MAX_PATH)
    {
        assert(!"file locator too long");
        return 0;
    }

    //skip '::' and get POU name
    psz = _tcsinc(psz); 
    psz = _tcsinc(psz);
    for(i=0; *psz && *psz!=_T('\\'); psz = _tcsinc(psz))
        pSplitted->pszPOUName[i++] = *psz;
    pSplitted->pszPOUName[i] = 0;

    if(*psz != _T('\\'))
    {
        assert(!"bad file locator, missing POU name or file name");
        return 0;
    }

    //skip '\' and get target type or file:
    psz = _tcsinc(psz);
    for(i=0; *psz && *psz!=_T('\\'); psz = _tcsinc(psz))
        pSplitted->pszTrgType[i++] = *psz;
    pSplitted->pszTrgType[i] = 0;

    if(*psz != _T('\\'))
    {
        //was not the trg type was the file:
        lstrcpy(pSplitted->pszLibFile, pSplitted->pszTrgType);
        pSplitted->pszTrgType[0] = 0;
        return 2;
    }

    //skip '\' and get file:
    psz = _tcsinc(psz);
    for(i=0; *psz && *psz!=_T('\\'); psz = _tcsinc(psz))
        pSplitted->pszLibFile[i++] = *psz;
    pSplitted->pszLibFile[i] = 0;
    
    assert(!*psz && pSplitted->pszLibFile[0]);

    return 3;
}





extern "C" bool CGT_LoadLocatorFileToMem(
    CGT_CDiagMsg*    pdm,           //for err msg
    const CG_Edipos* pEdp,          //NULL or edp for file IO error, usually NULL
    ICG_IECFront*    pIFront,       //used if locator points to a 4cl lib
    const TCHAR*     pszFileLocator,//file locator
    TCHAR**          ppCharBuffer,  //newly allocated buffer for file content
    unsigned long*   pulLength,     //NULL or receives the size in TCHARs of
                                    //*ppCharBuffer without the always
                                    //added terminating 0-TCHAR.
    __int64*         pftLastWrite   //NULL or receives the last file write time
)
{
    long                 ltyp;
    CGT_FileLocator      splitted;

    assert(ppCharBuffer);

    ltyp = CGT_SplitFileLocator(pszFileLocator, &splitted);
    if(ltyp<=0)
    {
        CGT_IERR(pdm, pdm->jot1()<<_T("invalid locator '")<<pszFileLocator<<_T("'"));
        return false;
    }

    if(ltyp==1)
    {
        return CGT_LoadTextFileToMem(
                pdm,
                pEdp,
                pIFront->FCPgetPrjPath(),
                splitted.pszFile, 
                ppCharBuffer, 
                pulLength, 
                pftLastWrite
        );
    }


    const CG_LIBContext* pInf;
    I4CLAccess*          pLibAcc;

    //get 4cp info and lib access interface:
    pLibAcc = NULL;
    pInf = pIFront->FCPgetLIBinfo(FC_StringGetFilePart(splitted.pszFile));
    if(pInf)
        pLibAcc = pIFront->get4CLLibIF(pInf);

    if(!pLibAcc)
    {
        CGT_IERR(pdm, pdm->jot1()<<_T("invalid lib locator, lib not in project: '")<<pszFileLocator<<_T("'")); 
        return false;
    }

   
    bool bOk = CGT_GetLIBFileData(
        pdm,                 //for err msg
        pLibAcc,             //used if locator points to a 4cl lib
        splitted.pszPOUName, //must be given 
        splitted.pszTrgType, //can be NULL or empty string
        splitted.pszLibFile, //must be given
        ppCharBuffer,        //NULL or newly allocated buffer for file content
        pulLength,           //NULL or receives the size in TCHARs of
                             //*ppCharBuffer without the always
                             //added terminating 0-TCHAR.
        pftLastWrite         //NULL or receives the last file write time
    );
    
    pLibAcc->Release();

    return bOk;
}


extern "C" long CGT_ExistsLocatorFile(
    CGT_CDiagMsg*         pdm,            //error message sink
    ICG_IECFront*         pIFront,        //used if locator points to a 4cl lib
    const TCHAR*          pszFileLocator, //file loactore to find.
    const CG_LIBContext** ppLibCtx        //if not NULL receives a possible lib context
)
{
    long                 ltyp;
    CGT_FileLocator      splitted;
    const CG_LIBContext* pInf;
    I4CLAccess*          pLibAcc;
    bool                 bExists;


    if(ppLibCtx)
        *ppLibCtx = NULL;

    ltyp = CGT_SplitFileLocator(pszFileLocator, &splitted);
    if(ltyp<=0)
    {
        CGT_IERR(pdm, pdm->jot1()<<_T("invalid locator '")<<pszFileLocator<<_T("'"));
        return -1;
    }

    if(ltyp==1)
        return FC_FileExists(pIFront->FCPgetPrjPath(), splitted.pszFile) ? 1 :0; 


    //get 4cp info and lib access interface:
    pLibAcc = NULL;
    pInf = pIFront->FCPgetLIBinfo(FC_StringGetFilePart(splitted.pszFile));
    if(pInf)
        pLibAcc = pIFront->get4CLLibIF(pInf);

    if(!pLibAcc)
    {
        CGT_IERR(pdm, pdm->jot1()<<_T("invalid lib locator, lib not in project: '")<<pszFileLocator<<_T("'")); 
        return -1;
    }

    if(ppLibCtx)
        *ppLibCtx = pInf;

    

    bExists = CGT_GetLIBFileData(
        pdm,                 //for err msg
        pLibAcc,             //used if locator points to a 4cl lib
        splitted.pszPOUName, //must be given 
        splitted.pszTrgType, //can be NULL or empty string
        splitted.pszLibFile, //must be given
        NULL,                //newly allocated buffer for file content or NULL
        NULL,                //NULL or receives the size in TCHARs of
                             //*ppCharBuffer without the always
                             //added terminating 0-TCHAR.
        NULL                 //NULL or receives the last file write time
    );
    
    pLibAcc->Release();

    return bExists;
}







CGT_CInstPath::CGT_CInstPath()
{
    nDepth=0;
    buffer[0] =0;
    iLen=0;
}
void CGT_CInstPath::append(const TCHAR* next)
{
    assert(next && *next && iLen<maxPath);

    if(iLen>0)
        buffer[iLen++] = _T('.');

    while((buffer[iLen++] = *next++) && iLen<maxPath)
        ;

    iLen--;
    assert(buffer[iLen]==0);
    nDepth++;
}
void CGT_CInstPath::appendLo(const TCHAR* next)
{
    assert(next && *next && iLen<maxPath);

    if(iLen>0)
        buffer[iLen++] = _T('.');
    int iBeg = iLen;
    while((buffer[iLen++] = *next++) && iLen<maxPath)
        ;

    iLen--;
    assert(buffer[iLen]==0);
    
    _tcslwr(buffer+iBeg);
    
    nDepth++;
}
void CGT_CInstPath::appendInt(long l)
{
    TCHAR buffer[128];
    ltoa(l, buffer, 10);
    this->append(buffer);
}
void CGT_CInstPath::appendTargetAddr(const TCHAR* pszAddr)
{
    assert(pszAddr && *pszAddr && *pszAddr!=_T('{') && iLen<maxPath);//}
    

    if(iLen>0)
        buffer[iLen++] = _T('.');

    buffer[iLen++] = _T('{');

    while((buffer[iLen++] = *pszAddr++) && iLen<maxPath)
        ;
    assert(buffer[iLen-1]==0);

    buffer[iLen-1] = _T('}');
    buffer[iLen] =0;
    nDepth++;
}

const TCHAR* CGT_CInstPath::stripFirstN(int n)const
{ 
    assert(n>=0);
    if(n==0)
        return buffer;

    for(int i=0; i<iLen; i++)
    {
        if(buffer[i] == _T('{'))
        {
            while(i<iLen && buffer[i] != _T('}'))
                i++;
      
            assert(i<iLen && buffer[i] == _T('}'));
            if(i >= iLen)
                return _T("");
        }

        if(buffer[i]=='.')
        {
            n--;
            if(n<=0) 
                return buffer+i+1;
        }
    }
    return _T("");
}

void CGT_CInstPath::stripLast()
{ 
    int i;
    
    if(nDepth==1)
    {
        buffer[0] = 0;
        iLen=0;
        nDepth =0;
        return;
    }

    for(i = iLen-1;i>=0; i--)
    {
        if(buffer[i]==_T('.'))
        {
            buffer[i] = 0;
            iLen=i;
            nDepth--;
            assert(nDepth>0);
            return;
        }
    }
}

void CGT_CInstPath::cutAt(CGT_IPathPos pos)
{
    assert(pos.depth>=0 && pos.depth>=nDepth && pos.strLen>=0 && pos.strLen<=iLen);
    assert(pos.strLen==0 || buffer[pos.strLen]==_T('.'));
    
    buffer[pos.strLen] = 0;
    nDepth = pos.depth;
}







extern "C" FC_CString& CGT_AppendCMDLFileArg(
    FC_CString&  cmdl, 
    const TCHAR* pszFile
)
{
    if(_tcspbrk(pszFile, _T(" \r\n\t")))
        cmdl<<'"'<<pszFile<<'"';
    else
        cmdl<<pszFile;
    return cmdl;
}




extern "C" int CGT_SubsDollarIns(
    CGT_CDiagMsg*     pdm, 
    const TCHAR*      pszOrig,
    const CG_Edipos*  pEdpOrig, 
    CGT_DollarInserts inserts[/*nInserts*/], 
    long              nInserts, 
    long              flags,
    FC_CString*       pResult
)
{
    int          i;
    bool         bFound;
    const TCHAR* psz;
    const TCHAR* pszIdBeg;
    CG_Edipos    edp; 
    bool         bEnvVar;
    bool         bRegVar;
    const TCHAR* pszEnv;
    TCHAR        szid[MAX_PATH];
    bool         bWasUndef;

    psz = pszOrig;
    edp = *pEdpOrig;
    bWasUndef = false;

    pResult->clear();
    pResult->resize(lstrlen(pszOrig)*2);


Next:
    pResult->appendUpTo(psz, _T("$"), &psz);

    if(FC_StringIncBegin(&psz,  _T("$(")))
    {
        bEnvVar = false;
        bRegVar = false;
    }
    else if((flags & CGT_INS_ENV) && FC_StringIncBeginI(&psz,  _T("$env(")))
    {
        bEnvVar = true;
        bRegVar = false;
    }
    else if((flags & CGT_INS_ENV) && FC_StringIncBeginI(&psz,  _T("$reg(")))
    {
        bEnvVar = false;
        bRegVar = true;
    }
    else if(*psz)
    {   
        pResult->addc(*psz++); //$<something> ??
        goto Next;
    }
    else
    {
        goto Ende;
    }

    pszIdBeg = psz;
    for(i=0, pszIdBeg=psz; *psz && *psz!=')'; )
    {
        if(i>=FC_ARRAY_LEN(szid))
        {
            CG_IncColEdipos(&edp, pszIdBeg - pszOrig);
            pdm->msg0(CG_E_TOKEN_TO_LONG, &edp);
            return 0;
        }
        szid[i++] = *psz++;
    }
    szid[i] = 0;
    FC_StringTrim(szid);

    if(*psz != _T(')'))
    {
        CG_IncColEdipos(&edp, pszIdBeg - pszOrig);
        pdm->msg1(CG_E_SYNTAX_MISSING, &edp, _T("closing )"));
        return 0;
    }
    else
    {
        psz++;
    }

    if(bEnvVar)
    {
        pszEnv = _tgetenv(szid);
        if(pszEnv)
        {
            pResult->addpsz(pszEnv);
        }
    }
    else if(bRegVar)
    {
        TCHAR buffer[2*MAX_PATH];
        if(FC_RegGetStrByPath(szid, 0, buffer, FC_ARRAY_LEN(buffer))==ERROR_SUCCESS)
        {
            pResult->addpsz(buffer);
        }
    }
    else
    {
        bFound = false;
        for(i=0; i<nInserts; i++)
        {
            if(!_tcsicmp(szid, inserts[i].pszId))
            {
                bFound = true;
                if(flags & CGT_INS_ESCAPE)
                    CGT_AppendCMDLFileArg(*pResult, inserts[i].pszSub);
                else
                    pResult->addpsz(inserts[i].pszSub);

                break;
            }
        }

        if(!bFound)
        {
            bWasUndef = true;
            if(flags & CGT_INS_WARN_UNDEF)
            {
                CG_IncColEdipos(&edp, pszIdBeg - pszOrig);
                pdm->msg1(CG_W_UNDEF_INSERT, &edp, szid);
            }
        }
    }
    goto Next;

Ende:

    if(bWasUndef && (flags & CGT_INS_WARN_UNDEF) && (flags & CGT_INS_PRINTHELP))
    {   
        if(!pdm->isDisableWarnings()) //TODO: if no warings also no info???
        {                             //the disableWarning should force also 
                                      //a disable "Info for Warning"
            pdm->userMsg(_T("    known inserts at this location: "));
            for(i=0; i<nInserts; i++)
            {
                if(!_tcsicmp(inserts[i].pszId,  _T("instdir"))) //HACK: don't show deprecated stuff in help
                    continue;
                if(i>0)
                    pdm->userMsg(_T(", "));
                pdm->userMsg(pdm->jot1()<<_T("$(")<<inserts[i].pszId<<')');
            }
            if(flags & CGT_INS_ENV)
            {
                if(i>0)
                    pdm->userMsg(_T(", "));

                pdm->userMsg(_T("$env(...), $reg(...)"));
            }

            pdm->userMsg(_T("\n"));
        }
    }

    return bWasUndef ? -1 : +1;
}

extern "C" int CGT_SubsStdDollarIns(
    CGT_CDiagMsg*     pdm, 
    ICG_IECFront*     pIFront,
    const TCHAR*      pszOrig,
    const CG_Edipos*  pEdpOrig, 
    long              flags,
    FC_CString*       pResult
)
{
    int                      nIns;
    static CGT_DollarInserts inserts[9];

    if(!_tcschr(pszOrig, _T('$')))
    {
        pResult->load(pszOrig);
        return true;
    }

    if(!inserts[0].pszId)
    {
        inserts[0].pszId   = CG_INSERT_INSDIR;
        inserts[0].pszSub  = CGT_GetSetupDir(pdm);

        inserts[1].pszId   = CG_INSERT_LIBDIR;
        inserts[1].pszSub  = CGT_GetSTDLibPath(pdm);

        inserts[2].pszId   = CG_INSERT_BINDIR;
        inserts[2].pszSub  = CGT_GetEngBinDir(pdm);

        inserts[3].pszId   = CG_INSERT_SYSDIR;
        inserts[3].pszSub  = CGT_GetSysDir();

        inserts[4].pszId   = CG_INSERT_WINDIR;
        inserts[4].pszSub  = CGT_GetWinDir();

        //for compatibillity only (tobe removed in future..):
        inserts[5].pszId   = _T("instdir");
        inserts[5].pszSub  = CGT_GetEngBinDir(pdm);
    }

    if(pIFront && pIFront->FCPgetPrjPath()[0])
    {
        inserts[6].pszId   = CG_INSERT_PRJDIR;
        inserts[6].pszSub  = pIFront->FCPgetPrjPath();

        inserts[7].pszId   = CG_INSERT_GENDIR;
        inserts[7].pszSub  = pIFront->FCPgetGenPath();

        inserts[8].pszId   = CG_INSERT_PRJNAME;
        inserts[8].pszSub  = pIFront->FCPgetPrjName();
        nIns = 9;
    }
    else
    {
        nIns = 6;//no project loaded...
    }

    return CGT_SubsDollarIns(pdm, pszOrig, pEdpOrig, inserts, nIns, flags, pResult);
}


const TCHAR* CGT_GetSysDir()
{
    static TCHAR  g_szSysdir[MAX_PATH] = {0};
    if(!g_szSysdir[0])
        GetSystemDirectory(g_szSysdir,  FC_ARRAY_LEN(g_szSysdir));

    return g_szSysdir;
}

const TCHAR* CGT_GetWinDir()
{
    static TCHAR  g_szWindir[MAX_PATH] = {0};
    if(!g_szWindir[0])
        GetWindowsDirectory(g_szWindir,  FC_ARRAY_LEN(g_szWindir));

    return g_szWindir;
}


const TCHAR* CGT_GetSetupDir(CGT_CDiagMsg* pdm)
{
    static TCHAR g_szSetupDir[MAX_PATH] = {0};
    long  ret;
    if(!g_szSetupDir[0])
    {
        ret = FC_RegGetStringVal(HKEY_LOCAL_MACHINE, UTIL_INSTALL_KEY, 
                UTIL_INSTALL_PATH, g_szSetupDir, FC_ARRAY_LEN(g_szSetupDir)); 
        if(ret != ERROR_SUCCESS)
        {
            GetModuleFileName(NULL, g_szSetupDir, FC_ARRAY_LEN(g_szSetupDir));
            //remove up too 'Engineering', we do not know if we are a back end or front end!
            while(_tcsicmp(FC_StringGetFilePart(g_szSetupDir), _T("Engineering")))
                FC_StringCutFilePart(g_szSetupDir);

            if(!_tcsicmp(FC_StringGetFilePart(g_szSetupDir), _T("Engineering")))
                FC_StringCutFilePart(g_szSetupDir); //remove engineering
            else
                g_szSetupDir[0] = 0; //no default possible

            if(pdm && !pdm->isDisableWarnings())
            {
                pdm->msg4(CG_W_REG_READ, NULL, _T("HKLM\\"), UTIL_INSTALL_KEY, 
                    UTIL_INSTALL_PATH, pdm->jot1()<<ret);
                if(g_szSetupDir[0])
                {
                    pdm->userMsg(_T("   Assume module path relative root at '"));
                    pdm->userMsg(g_szSetupDir);
                    pdm->userMsg(_T("'\n"));
                }
            }
        }
    }

    FC_StringRemoveLastChar(g_szSetupDir, _T("\\/"), NULL);
    return g_szSetupDir;
}
const TCHAR* CGT_GetEngBinDir(CGT_CDiagMsg* pdm)
{
    static TCHAR  g_szEngBinDir[MAX_PATH] = {0};
    if(!g_szEngBinDir[0])
    {
        _tcscpy(g_szEngBinDir, CGT_GetSetupDir(pdm));
        _tcscat(g_szEngBinDir, _T("\\Engineering\\bin"));

        if(_tcslen(g_szEngBinDir) >= FC_ARRAY_LEN(g_szEngBinDir))
        {
            _tprintf(_T("fatal error: install dir path name too long, terminate...\n"));
            exit(1);
        }
    }
    return g_szEngBinDir;
}

const TCHAR* CGT_GetSTDLibPath(CGT_CDiagMsg* pdm)
{
    static TCHAR  g_szSTDLibPath[MAX_PATH] = {0};
    long ret;
    if(!g_szSTDLibPath[0])
    {
        //get lib path from registry:
        ret = FC_RegGetStringVal(HKEY_LOCAL_MACHINE, UTIL_INSTALL_KEY, 
                UTIL_INSTALL_LIBPATH, g_szSTDLibPath, FC_ARRAY_LEN(g_szSTDLibPath)); 

        if(ret != ERROR_SUCCESS)
        {
            _tcscpy(g_szSTDLibPath, CGT_GetEngBinDir(pdm));
            FC_StringCutFilePart(g_szSTDLibPath); //remove bin
            _tcscat(g_szSTDLibPath, _T("\\lib")); //add lib

            if(pdm && !pdm->isDisableWarnings())
            {
                pdm->msg4(CG_W_REG_READ, NULL, _T("HKLM\\"), UTIL_INSTALL_KEY, 
                    UTIL_INSTALL_LIBPATH, pdm->jot1()<<ret);
                
                pdm->userMsg(_T("   Assume setup path relative library root at '"));
                pdm->userMsg(g_szSTDLibPath);
                pdm->userMsg(_T("'\n"));
            }
        }
    }

    FC_StringRemoveLastChar(g_szSTDLibPath, _T("\\/"), NULL);
    return g_szSTDLibPath;
}




int CGT_CheckNumOption(
    CGT_CDiagMsg*     pdm,
    const CG_Edipos*  pEdp,
    const TCHAR*      pszVal, 
    const TCHAR*      pszOpt, 
    int               maxVal,
    int*              piVal
)
{
    int             ival;
    CG_Edipos       edp;
    const TCHAR*    psz = pszVal;

    if(!FC_StringIncBeginI(&psz, pszOpt))
        return 0;

    if(isdigit(*psz) && psz[1]==0)
    {
        ival = atoi(psz);
        if(ival>=0 &&  ival<=maxVal)
        {
            *piVal = ival;
            return +1;
        }
    }

    if(pEdp)
    {
        edp = *pEdp;
        CG_IncColEdipos(&edp, _tcslen(pszOpt));
        pEdp = &edp;
    }
    pdm->msg2(CG_E_SYNTAX_EXPECTED_FOUND, pEdp, 
        pdm->jot1()<<_T("[0..")<<maxVal<<']',  psz
    );
    
    return -1;
}
