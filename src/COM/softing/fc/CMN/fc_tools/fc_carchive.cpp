/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_carchive.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_carchive.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_carchive.cpp $
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
 *==implementaion of archive file
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  22.10.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


//----  Includes:   -------------------------------------------*
#include <stddef.h> //offsetof
#include <stdio.h> //printf
#include "fc_tools\FC_CArchive.h"
#include "fc_tools\fc_crypt.h"



//----  Local Defines:   -------------------------------------------*
#ifndef UNICODE  //not supported for UNICODE rethink and rework this code, at least change the version 


#define FC_AR_PW "FC_AR_PW"
#define AR_IGNORE_CASE true
//bin versions: 1  same binary structure of archive but file data is not DES encoded
//bin versions: 2  same binary structure of archive but file data is     DES encoded
#define FC_AR_BIN_VER  2 //current version is now 2, but older versions can still be read/written
#define FC_AR_SZ_MAGIC _T("ATCM Archive")
struct AR_MagicHdr
{
    TCHAR  szMagicVersion[FC_AR_MAX_VER];
    long   lBinVersionNum;
    TCHAR  szUserVersion[FC_AR_MAX_VER];
    long   nNodes;
};



#define  FC_CArFile_ISDIR   0x00001
#define  FC_CArFile_ALLBITS FC_CArFile_ISDIR
struct FC_CArFileInfo
{
    long     lStrLenId; 
    DWORD    dwFlags;  //FC_CArFile_ISDIR
    long     lData/*Bytes|Files*/;
    long     lOffset;
    TCHAR    szId[2*MAX_PATH];
};
inline DWORD FC_CArFileInfoSize(int lStrLenId)
{
    return offsetof(FC_CArFileInfo, szId) + (lStrLenId+1)*sizeof(TCHAR);
}



static bool MakeTmpAr(FC_CString& tmpFile)
{
    int        i;
    FC_CString help(tmpFile.len()+10);

    if(FC_StringHasPath(tmpFile))
    {
        help.load(tmpFile).stripFilePart();
        if(!FC_ExistSubDir(help))
            FC_CreateSubDir(help);//ignore errors here, will be error later anyway
    }

    tmpFile.addc(_T('~'));
    help.load(tmpFile);

    for(i=1; FC_Exist(tmpFile); i++ )
    {
        if(i>1024)
            return false;
        tmpFile.load(help)<<i;
    }
    
    return true;
}
inline bool WrtFile(HANDLE hFile, const void* pvData, DWORD nBytes)
{
    DWORD dw;
    return WriteFile(hFile, pvData, nBytes, &dw, NULL) && dw==nBytes;
}

static FC_ARSTAT AppendFile(HANDLE hSrc, HANDLE hDest, DWORD dwBytes)
{
    DWORD dw;
    DWORD dw2;
    DWORD dwBytesLeft;
    char  buffer[4096];

      
    dwBytesLeft = dwBytes;
    do
    {
        dw = dwBytesLeft>=sizeof(buffer) ? sizeof(buffer) : dwBytesLeft;

        if(!ReadFile(hSrc, buffer, dw, &dw2, NULL) || dw!=dw2)
            return FC_AR_IO_READ_FILE;
        if(!WrtFile(hDest, buffer, dw2))
            return FC_AR_IO_WRITE_FILE;

        dwBytesLeft -= dw;
    }while(dwBytesLeft);

    return FC_AR_OK;
}






FC_CArNode::FC_CArNode(
    const TCHAR* pszId, 
    FC_CArDir*      pParent
) 
{
    m_pParent = pParent;
    m_lStrLenId = _tcslen(pszId);
    m_pszId = new TCHAR[m_lStrLenId+1];
    memcpy(m_pszId, pszId, (m_lStrLenId+1)*sizeof(TCHAR));
    m_dwFinalOff = 0;
}

FC_CArNode::~FC_CArNode()
{
    delete(m_pszId);
}
FC_CArDir*  FC_CArNode::isDir()
{
    return NULL;
}
FC_CArFile* FC_CArNode::isFile()
{
    return NULL;
}

FC_CString& FC_CArNode::getFullName(FC_CString& path)const
{
    path.clear().resize(MAX_PATH);
    path<<m_pszId;
    for(const FC_CArNode* p = m_pParent; p; p=p->m_pParent)
        path.prepend(p->m_pszId, _T("\\"));
    
    return path;
}




FC_CArFile::FC_CArFile(
    const TCHAR* pszId, 
    FC_CArDir*   parent,
    HANDLE       hData,
    DWORD        dwBytesRaw,
    DWORD        dwOffset,
    FC_ARSTAT*   pRet
)
  : FC_CArNode(pszId, parent)
{
    m_hData         = hData;
    m_dwBytesRaw    = dwBytesRaw;
    m_dwOffset      = dwOffset;

    *pRet = FC_AR_OK;

    if(parent->m_pAr->m_bDoEncodeData)
    {   //HACK: we must get now the m_dwBytesDecoded info, todo this
        //      we read the first 4 byte from m_hData file.
        //      It is a hack because we do assume here how FC_DESEncodeBlock
        //      actualy encodes the origial size in its output (in the first DWORD...)
        DWORD dw;

        DWORD dwOldPos = SetFilePointer(m_hData, 0, NULL, FILE_CURRENT);

        SetFilePointer(m_hData, m_dwOffset, NULL, FILE_BEGIN);
        
        if(!ReadFile(m_hData, &m_dwBytesDecoded, sizeof(m_dwBytesDecoded), &dw, NULL)  || dw!=sizeof(m_dwBytesDecoded))
            *pRet = FC_AR_CORRUPTED;

        SetFilePointer(m_hData, dwOldPos, NULL, FILE_BEGIN);

        //do a consitency check for m_dwBytesDecoded (it may be some giga byte 
        //if the archive is corrupt! Without this check we may crash later when
        //malloc such a blob!
        if(!FC_DESCheckDecodedSize(FC_DES_NORMAL, m_dwBytesDecoded, dwBytesRaw))
        {
            m_dwBytesDecoded = dwBytesRaw;//avoid that m_dwBytesDecoded is a giga byte...
            *pRet = FC_AR_CORRUPTED;
        }
    }
    else
    {
        m_dwBytesDecoded= dwBytesRaw;
    }
}

FC_ARSTAT FC_CArFile::appendToFileRaw(HANDLE hFile)
{
    FC_ARSTAT ret;

    if(m_pParent->m_pAr->m_firstFatalError != FC_AR_OK)
        return FC_AR_ERR_INERROR;

    SetFilePointer(m_hData, m_dwOffset, NULL, FILE_BEGIN);
    ret = AppendFile(m_hData, hFile, m_dwBytesRaw);
    if(ret == FC_AR_IO_READ_FILE)
    {
        m_pParent->m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
        return FC_AR_CORRUPTED;
    }
    return FC_AR_OK;

}

FC_ARSTAT FC_CArFile::writeNode(HANDLE hFile)
{
    //printf("F>%20s: off=%-10d size=%-10d\n",m_pszId, SetFilePointer(hFile, 0, NULL, FILE_CURRENT),m_dwBytes);
    m_dwFinalOff = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
    return appendToFileRaw(hFile);
}

FC_ARSTAT FC_CArFile::writeNodeInfo(HANDLE hFile)
{
    FC_CArFileInfo fhdr;
    DWORD       dwBytes;

    assert(m_dwFinalOff && m_pszId[m_lStrLenId]==0);
    fhdr.lStrLenId = m_lStrLenId;
    fhdr.dwFlags   = 0;
    fhdr.lData     = m_dwBytesRaw;
    fhdr.lOffset   = m_dwFinalOff;
    memcpy(fhdr.szId, m_pszId, (m_lStrLenId+1)*sizeof(TCHAR));

    dwBytes = FC_CArFileInfoSize(m_lStrLenId);
    return WrtFile(hFile, &fhdr, dwBytes) ? FC_AR_OK : FC_AR_IO_WRITE_FILE;
}

FC_CArFile* FC_CArFile::isFile()
{
    return this;
}

DWORD FC_CArFile::getSize()const
{
    return m_dwBytesDecoded;
}
FC_ARSTAT FC_CArFile::getData(DWORD dwBuffer, void* pvData)
{
    DWORD     dw;
   
    if(m_pParent->m_pAr->m_bDoEncodeData)
    {
        SetFilePointer(m_hData, m_dwOffset, NULL, FILE_BEGIN);
        char* pEncoded = new char[m_dwBytesRaw?m_dwBytesRaw:1];//may be file size 0, cannot alloc 0 chars...

        if(!ReadFile(m_hData, pEncoded, m_dwBytesRaw, &dw, NULL)  || dw!=m_dwBytesRaw)
        {
            delete(pEncoded);
            m_pParent->m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }
        
        int iRet = FC_DESDecodeBlock(FC_DES_NORMAL, FC_AR_PW, pEncoded, m_dwBytesRaw, (char*)pvData, dwBuffer);
        delete(pEncoded);

        if(iRet != 0)
            return iRet==2 ? FC_AR_BUFFER_TOO_SMALL : FC_AR_CORRUPTED;
        
    }
    else
    {
        if(dwBuffer<m_dwBytesRaw)
            return FC_AR_BUFFER_TOO_SMALL;

        SetFilePointer(m_hData, m_dwOffset, NULL, FILE_BEGIN);
        if(!ReadFile(m_hData, pvData, m_dwBytesRaw, &dw, NULL)  || dw!=m_dwBytesRaw)
        {
            m_pParent->m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }
    }
    return FC_AR_OK;
}

FC_ARSTAT FC_CArFile::getToDisk(
    const TCHAR* pszDirectory,
    const TCHAR* pszDiskFileName
)
{
    FC_ARSTAT ret;
    bool      bFirstTry;
    HANDLE    hFileDisk;

    if(m_pParent->m_pAr->m_firstFatalError != FC_AR_OK)
        return FC_AR_ERR_INERROR;

    
    m_pParent->m_pAr->m_lastIOErrFile.load(pszDirectory).addc(_T('\\'));
    if(pszDiskFileName)
        m_pParent->m_pAr->m_lastIOErrFile<<pszDiskFileName;
    else
        m_pParent->m_pAr->m_lastIOErrFile<<m_pszId;
    

    //open file and check max file size:
    bFirstTry = true;
TryAgain:
    hFileDisk  = CreateFile(m_pParent->m_pAr->m_lastIOErrFile,GENERIC_WRITE,0,NULL,
                        CREATE_ALWAYS,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

    if(hFileDisk==INVALID_HANDLE_VALUE)
    {
        if(bFirstTry)
        {
            bFirstTry = false;
            FC_CreateSubDir(pszDirectory);
            goto TryAgain;
        }
        return FC_AR_IO_WRITE_FILE;
    }

    if(m_pParent->m_pAr->m_bDoEncodeData)
    {
        DWORD nDecoded = getSize();
        char* pDecoded = new char[nDecoded?nDecoded:1];//may be file size 0, cannot alloc 0 chars...
        ret = this->getData(nDecoded, pDecoded);
    
        if(ret==FC_AR_OK)
            ret = WrtFile(hFileDisk, pDecoded, nDecoded) ? FC_AR_OK : FC_AR_IO_WRITE_FILE;
        
        delete(pDecoded);
    }
    else
    {
        ret = appendToFileRaw(hFileDisk);
    }

    CloseHandle(hFileDisk);
    if(ret!=FC_AR_IO_WRITE_FILE)
       m_pParent->m_pAr->m_lastIOErrFile.clear();

    return ret;
}








FC_CArDir::FC_CArDir(
    FC_CArchive*  pAr,
    const TCHAR* pszId, 
    FC_CArDir*   parent,
    HANDLE       hData,
    long         nNodes,
    DWORD        dwOffset
)
  : FC_CArNode(pszId, parent)
{
    m_pAr       = pAr;
    m_hData     = hData;
    m_nNodes    = nNodes;
    m_nDirs     = -1;
    m_dwOffset  = dwOffset;
    m_pDir      = NULL;
}
FC_CArDir::~FC_CArDir()
{
    if(m_pDir)
    {
        const FC_CHashStr<FC_CArNode*>::Pair* p;
        for(p=m_pDir->firstPair(); p; p=p->pNext)
            delete(p->pVal);
        delete(m_pDir);
    }
}

FC_ARSTAT FC_CArDir::readDir()
{
    DWORD           dw;
    DWORD           dw2;
    long            i;
    FC_CArNode*     pNode;
    FC_CArFileInfo  mhdr;

    if(m_pAr->m_firstFatalError != FC_AR_OK)
        return FC_AR_ERR_INERROR;

    //already loaded:
    if(m_pDir)
        return FC_AR_OK;

    if(m_hData==INVALID_HANDLE_VALUE)
    {
        //nothing to load, was newly empty and now accessed the first time:
        assert(m_nNodes==0 && m_nDirs==-1 && m_dwOffset==0);
        //allocate appropriate hash size:
        m_nDirs = 0;
        m_pDir = new FC_CHashStr<FC_CArNode*>(true, true, 10);
        return FC_AR_OK;
    }


    //load dir info from presistent storage the first time:
    assert(m_nNodes>=0 && m_nDirs==-1 && m_dwOffset!=0);

    m_nDirs = 0;
    SetFilePointer(m_hData, m_dwOffset, NULL, FILE_BEGIN);
    //allocate appropriate hash size:
    m_pDir = new FC_CHashStr<FC_CArNode*>(true, true, m_nNodes*2+3);
    for(i=0; i<m_nNodes; i++)
    {
        //read lstrlenid first:
        if(!ReadFile(m_hData, &mhdr.lStrLenId, sizeof(mhdr.lStrLenId), &dw, NULL))
        {
            m_pAr->m_firstFatalError = FC_AR_IO_READ_FILE;
            return FC_AR_IO_READ_FILE;
        }

        if(dw != sizeof(mhdr.lStrLenId))
        {
            m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }

        if(mhdr.lStrLenId<=0 || mhdr.lStrLenId>MAX_PATH)
        {
            m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }
        
        //read rest of info struct:
        dw2 = FC_CArFileInfoSize(mhdr.lStrLenId) - sizeof(mhdr.lStrLenId);
        if(!ReadFile(m_hData, &mhdr.dwFlags, dw2, &dw, NULL))
        {
            m_pAr->m_firstFatalError = FC_AR_IO_READ_FILE;
            return FC_AR_IO_READ_FILE;
        }
        if(dw != dw2)
        {
            m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }

        //check info:
        if(mhdr.szId[mhdr.lStrLenId] != 0 || mhdr.lData<0 || 
            mhdr.lOffset<0 || (mhdr.dwFlags & ~FC_CArFile_ALLBITS)!=0
          )
        {
            m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }
 
        if(mhdr.dwFlags & FC_CArFile_ISDIR)
        {
            pNode = new FC_CArDir(m_pAr, mhdr.szId, this, m_hData, mhdr.lData, mhdr.lOffset);
        }
        else
        {
            pNode = new FC_CArFile(mhdr.szId, this, m_hData, mhdr.lData, mhdr.lOffset, &m_pAr->m_firstFatalError);
            if(m_pAr->m_firstFatalError != FC_AR_OK)
            {
                delete(pNode);
                return m_pAr->m_firstFatalError;
            }
        }

        if(!m_pDir->insertEx(pNode->getName(), pNode, false, NULL))
        {
            delete(pNode);
            m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }

        if(mhdr.dwFlags & FC_CArFile_ISDIR)
            m_nDirs++;
    }

    //invalid persisten input, one the dir is in mem is remains in mem:
    m_dwOffset = 0;
    m_hData    = INVALID_HANDLE_VALUE;

    return FC_AR_OK;
}

FC_ARSTAT FC_CArDir::writeNode(HANDLE hFile)
{
    FC_ARSTAT ret;
    DWORD     dwTabelSize;
    const FC_CHashStr<FC_CArNode*>::Pair* p;

    ret = FC_AR_OK;
    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    m_dwFinalOff = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

    dwTabelSize = 0;
    for(p=m_pDir->firstPair(); p; p=p->pNext)
        dwTabelSize += FC_CArFileInfoSize(p->pVal->m_lStrLenId);

    //skip the table, will be writeen later:
    SetFilePointer(hFile, dwTabelSize+sizeof(m_nNodes), NULL, FILE_CURRENT);

    //write data for child nodes:
    for(p=m_pDir->firstPair(); p; p=p->pNext)
    {
        if((ret = p->pVal->writeNode(hFile)) != FC_AR_OK)
            return ret;
    }
    
    //seek back to table begin:
    SetFilePointer(hFile, m_dwFinalOff, NULL, FILE_BEGIN);

    //write table
    for(p=m_pDir->firstPair(); p; p=p->pNext)
    {
        if((ret = p->pVal->writeNodeInfo(hFile)) != FC_AR_OK)
            return ret;
    }

    //back to end of file
    SetFilePointer(hFile, 0, NULL, FILE_END);
    return ret;
}
FC_ARSTAT FC_CArDir::writeNodeInfo(HANDLE hFile)
{
    FC_CArFileInfo fhdr;
    DWORD       dwBytes;

    assert(m_dwFinalOff && m_pszId[m_lStrLenId]==0);
    fhdr.lStrLenId = m_lStrLenId;
    fhdr.dwFlags   = FC_CArFile_ISDIR;
    fhdr.lData     = m_nNodes;
    fhdr.lOffset   = m_dwFinalOff;
    memcpy(fhdr.szId, m_pszId, (m_lStrLenId+1)*sizeof(TCHAR));

    dwBytes = FC_CArFileInfoSize(m_lStrLenId);
    return WrtFile(hFile, &fhdr, dwBytes) ? FC_AR_OK : FC_AR_IO_WRITE_FILE;
}

FC_CArDir* FC_CArDir::isDir()
{
    return this;
}

FC_ARSTAT FC_CArDir::createDir(
    const TCHAR*  pszId,
    FC_CArDir**   ppDir   
)
{
    FC_ARSTAT   ret;
    FC_CArNode* pPrev;

    *ppDir = NULL;

    if(!m_pAr->isOpenWrite())
        return FC_AR_NOT_OPENED_WRITE;

    if(!FC_StringIsValidFileName(pszId))
        return FC_AR_BAD_FILE_NAME;

    if(!m_pDir && (ret = readDir())!=FC_AR_OK)
        return ret;

    *ppDir = new FC_CArDir(m_pAr, pszId, this, INVALID_HANDLE_VALUE, 0, 0);
    if(m_pDir->insertEx(pszId, *ppDir, true, &pPrev))
    {
        m_nNodes++;
        m_nDirs++;
    }
    else
    {
        if(pPrev->isDir())
            m_nDirs--;
        delete(pPrev);
    }
    
    m_pAr->setChanged();
    return FC_AR_OK;
}

FC_ARSTAT FC_CArDir::createFile(
    const TCHAR* pszId, 
    const TCHAR* pszDiskFile,
    DWORD        dwBytes,
    const void*  pvData
)
{
    FC_ARSTAT   ret;
    FC_CArFile* pNew;
    FC_CArNode* pPrev;
    DWORD       dwOffset;
    HANDLE      hFileTmp;

    if(m_pAr->m_firstFatalError != FC_AR_OK)
        return FC_AR_ERR_INERROR;

    if(!m_pAr->isOpenWrite())
        return FC_AR_NOT_OPENED_WRITE;

    if(!pszId)
        pszId = FC_StringGetFilePart(pszDiskFile);

    if(!FC_StringIsValidFileName(pszId))
        return FC_AR_BAD_FILE_NAME;

    if(!m_pDir && (ret = readDir())!=FC_AR_OK)
        return ret;

    ret = m_pAr->writeToTmp(pvData, pszDiskFile, &dwBytes, &dwOffset, &hFileTmp);
    if(ret!=FC_AR_OK)
        return ret;

    pNew = new FC_CArFile(pszId, this, hFileTmp, dwBytes, dwOffset, &m_pAr->m_firstFatalError);
    if(m_pAr->m_firstFatalError != FC_AR_OK)
    {
        m_pAr->setChanged();
        delete(pNew);
        return m_pAr->m_firstFatalError;
    }
    if(m_pDir->insertEx(pszId, pNew, true, &pPrev))
        m_nNodes++;
    else
        delete(pPrev);

    m_pAr->setChanged();
    return FC_AR_OK;
}




struct CB_ENUMFARGS
{
    FC_ARSTAT     ret;
    FC_CArDir*    pCurrDir; 
    const TCHAR*  pszFileSpec;
    int           nFilesFound;
};

static int cpEnumDiskFiles(
    void*         pCtx, 
    void*         pCtx2, 
    int           nIsDir,
    int           bOnError,
    const TCHAR*  pszFile
)
{
    CB_ENUMFARGS* pcbea;
    FC_CArDir*    parDir;
    const TCHAR*  pszFileNoPath;

    pcbea = (CB_ENUMFARGS*)pCtx;

    if(bOnError)
    {
        pcbea->ret = FC_AR_IO_READ_FILE;
        return -1; //stop enumeration
    }

    if(nIsDir==0)
    {
        pszFileNoPath = FC_StringGetFilePart(pszFile);
        if(pcbea->pszFileSpec && !FC_StringMatchFilePattern(pszFileNoPath, pcbea->pszFileSpec))
            return 1; //does not match ignore and carry on

        pcbea->ret = pcbea->pCurrDir->createFile(pszFileNoPath, pszFile, 0, NULL);
        pcbea->nFilesFound++;
        if(pcbea->ret != FC_AR_OK)
            return -1;
    }
    else if(nIsDir>0)
    {
        pszFileNoPath = FC_StringGetFilePart(pszFile);
        pcbea->ret = pcbea->pCurrDir->createDir(pszFileNoPath, &parDir);
        if(pcbea->ret != FC_AR_OK)
            return -1;
        pcbea->pCurrDir = parDir;
    }
    else
    {
        assert(nIsDir==-1 && !_tcscmp(pszFile, _T("..")));
        pcbea->pCurrDir = pcbea->pCurrDir->getParent();
    }


    return 1;
} 

FC_ARSTAT FC_CArDir::createFilesFromDisk(
    const TCHAR* pszFileSpec,
    bool         bRecurseDirs
)
{
    const TCHAR* pszFilePart;

    pszFilePart = FC_StringGetFilePart(pszFileSpec);
    
    //no wild cards no recursive? then use simple:
    if(!bRecurseDirs && !FC_StringHasWildCards(pszFilePart))
    {
        return createFile(NULL, pszFileSpec, -1, NULL);
    }
    else
    {
        FC_CString   path;
        long         lFlags;
        CB_ENUMFARGS cbea;

        path.load(pszFileSpec).stripFilePart();
        cbea.ret = FC_AR_OK;
        cbea.pCurrDir = this;
        cbea.pszFileSpec = pszFilePart;
        cbea.nFilesFound = 0;

        if(!_tcscmp(cbea.pszFileSpec, _T("*")))
            cbea.pszFileSpec = NULL;

        lFlags = FC_FILE_ENUM_FULLPATH;
        if(bRecurseDirs)
            lFlags |= FC_FILE_ENUM_SUBDIRS;

        if(!FC_FileEnumFiles(path, lFlags, cpEnumDiskFiles, &cbea, NULL))
            cbea.ret = FC_AR_IO_READ_FILE; 

        assert(cbea.pCurrDir==this || cbea.ret!=FC_AR_OK);
        if(cbea.ret==FC_AR_OK && cbea.nFilesFound==0)
        {
            m_pAr->m_lastIOErrFile.load(pszFileSpec);
            cbea.ret = FC_AR_CIM_NO_SUCH_FILE;
        }

        return cbea.ret;
    }   
}



FC_ARSTAT FC_CArDir::remove(const TCHAR* pszId, int *pnDel)
{
    FC_ARSTAT    ret;
    FC_CArNode*  pNode;
    const FC_CHashStr<FC_CArNode*>::Pair* p;

    if(pnDel)
        *pnDel = 0;

    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    if(pszId && FC_StringHasWildCards(pszId))
    {
        for(p=m_pDir->firstPair(); p; )
        {
            if(!FC_StringMatchFilePattern(p->pszKey, pszId))
            {
                p = p->pNext;
                continue;
            }

            pNode = p->pVal; 
            p = p->pNext;


            if(!m_pDir->remove(pNode->m_pszId))
            {
                assert(!"bad hash");
                m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
                return FC_AR_CORRUPTED;
            }
            m_pAr->setChanged();
            if(pnDel)
                (*pnDel)++;
            m_nNodes--;
            if(pNode->isDir())
                m_nDirs--;
            delete(pNode);
            assert(m_nNodes>=0 && m_nDirs>=0 && m_nDirs<=m_nNodes);
        }
        
        return FC_AR_OK;
    }

    pNode = m_pDir->get(pszId);
    if(pNode)
    {
        if(!m_pDir->remove(pNode->m_pszId))
        {
            assert(!"bad hash");
            m_pAr->m_firstFatalError = FC_AR_CORRUPTED;
            return FC_AR_CORRUPTED;
        }

        m_pAr->setChanged();
        m_nNodes--;
        if(pNode->isDir())
            m_nDirs--;
        delete(pNode);
        assert(m_nNodes>=0 && m_nDirs>=0 && m_nDirs<=m_nNodes);
        if(pnDel)
            *pnDel = 1;
    }
    return FC_AR_OK;
}



FC_ARSTAT FC_CArDir::countSubDirs(int* pnDirs)
{
    FC_ARSTAT ret;
    
    *pnDirs = 0;
    
    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    assert(m_nDirs>=0);
    *pnDirs = m_nDirs;

    return FC_AR_OK;
}


FC_ARSTAT FC_CArDir::getSubDir(const TCHAR* pszId, FC_CArDir** ppDir)
{
    FC_ARSTAT   ret;
    FC_CArNode* pNode;
    
    *ppDir = NULL;

    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    pNode = m_pDir->get(pszId);
    if(pNode)
        *ppDir = pNode->isDir();
    
    return FC_AR_OK;
}
FC_ARSTAT FC_CArDir::getFile(const TCHAR* pszId, FC_CArFile** ppFile)
{
    FC_ARSTAT   ret;
    FC_CArNode* pNode;
    
    *ppFile = NULL;

    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    pNode = m_pDir->get(pszId);
    if(pNode)
        *ppFile = pNode->isFile();
    
    return FC_AR_OK;
}


FC_ARSTAT FC_CArDir::getNodes(
    const TCHAR*            pszFileSpec, 
    bool                    bFiles, 
    bool                    bDirs,
    FC_CGArray<FC_CArNode*>*pNList
)
{
    FC_ARSTAT   ret;
    const FC_CHashStr<FC_CArNode*>::Pair* pp;

    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    pNList->allocSize(m_nNodes+1);
    pNList->resetUsed();

    for(pp=m_pDir->firstPair(); pp; pp=pp->pNext)
    {
        if(pp->pVal->isDir())
        {
            if(!bDirs)
                continue;
        }
        else
        {
            if(!bFiles)
                continue;
        }

        if(pszFileSpec && !FC_StringMatchFilePattern(pp->pszKey, pszFileSpec))
            continue;

        pNList->add(pp->pVal);
    }

    return FC_AR_OK;
}


FC_ARSTAT FC_CArDir::getFilesToDisk(
    const TCHAR*             pszDirectory,
    const TCHAR*             pszFileSpec, //NULL (same as "*") or any *? pattern
    bool                     bRecursive,
    FC_CGArray<FC_CArNode*>* pFileList
)
{
    FC_ARSTAT   ret;
    FC_CArDir*  pDir;
    FC_CString  currDiskPath(MAX_PATH);
    const FC_CHashStr<FC_CArNode*>::Pair* pp;

    if(!m_pDir && (ret = readDir()) != FC_AR_OK)
        return ret;

    currDiskPath.load(pszDirectory);


    for(pp=m_pDir->firstPair(); pp; pp=pp->pNext)
    {
        pDir = pp->pVal->isDir();
        if(pDir)
        {
            if(bRecursive)
            {
                currDiskPath.addc(_T('\\'))<<pp->pszKey;
                ret = pDir->getFilesToDisk(currDiskPath, pszFileSpec, bRecursive, pFileList);
                if(ret != FC_AR_OK)
                    return ret;
                currDiskPath.stripFilePart();
            }
        }
        else
        {
            if(!pszFileSpec || FC_StringMatchFilePattern(pp->pszKey, pszFileSpec))
            {
                if(pFileList)
                    pFileList->add(pp->pVal);

                ret = pp->pVal->isFile()->getToDisk(currDiskPath, NULL);
                if(ret != FC_AR_OK)
                    return ret;
            }
        }
    }

    return FC_AR_OK;
}




FC_CArchive::FC_CArchive() : m_szTmpPutFile(MAX_PATH)
{
    initMembers();
}
FC_CArchive::~FC_CArchive()
{
    if(m_pRoot)
        delete(m_pRoot);
    if(m_hFileArOld != INVALID_HANDLE_VALUE) 
        CloseHandle(m_hFileArOld);
    if(m_hFileTmpPut != INVALID_HANDLE_VALUE) 
        CloseHandle(m_hFileTmpPut);
}

void FC_CArchive::initMembers()
{
    m_pRoot = NULL;
    m_fileName.load(_T(""));
    memset(&m_ftLastModified, 0, sizeof(m_ftLastModified));
    m_hFileArOld = INVALID_HANDLE_VALUE; 
    m_bOpenWrite = false;
    m_bChanged   = false;
    m_bDoEncodeData = false;
    m_BinVersionForWrite = FC_AR_BIN_VER;
    m_szTmpPutFile.load(_T(""));
    m_hFileTmpPut= INVALID_HANDLE_VALUE;
    m_dwTmpPutCurrPos = 0;
    m_firstFatalError = FC_AR_OK;
    m_lastIOErrFile.clear();
}



FC_ARSTAT FC_CArchive::open(
        const TCHAR* pszArchive, 
        bool         bWrite,
        TCHAR        pszVerFound[FC_AR_MAX_VER]
)
{
    HANDLE         hFile;
    FILETIME       ftLastModified;
    FC_CArDir*     pRoot;

    pszVerFound[0] = 0;

    if(!m_fileName.isEmpty())
        close(false, NULL);
    
    assert(m_hFileArOld==INVALID_HANDLE_VALUE && !m_bChanged);

    hFile = INVALID_HANDLE_VALUE;
    if(bWrite)
    {
        if(FC_Exist(pszArchive))
        {
            hFile =  CreateFile(
              pszArchive,                                 // file name
              GENERIC_READ | GENERIC_WRITE,               // access mode
              0,                                          // share mode
              NULL,                                       // SD
              OPEN_ALWAYS,                                // how to create
              FILE_FLAG_RANDOM_ACCESS,                    // file attributes
              NULL                                        // handle to template file
            );
            if(hFile == INVALID_HANDLE_VALUE)
            {
                m_lastIOErrFile.load(pszArchive);
                return FC_AR_IO_WRITE_FILE;
            }
        }
    }
    else
    {
        hFile = CreateFile(
          pszArchive,                                 // file name
          GENERIC_READ,                               // access mode
          FILE_SHARE_READ,                            // share mode
          NULL,                                       // SD
          OPEN_EXISTING,                              // how to create
          FILE_FLAG_RANDOM_ACCESS,                    // file attributes
          NULL                                        // handle to template file
        );
        if(hFile == INVALID_HANDLE_VALUE) 
        {
            m_lastIOErrFile.load(pszArchive);
            return FC_AR_IO_READ_FILE;
        }
    }

    if(hFile != INVALID_HANDLE_VALUE)
    {
        AR_MagicHdr                 mhdr;
        DWORD                       dw;
        BY_HANDLE_FILE_INFORMATION  fileInfo;

        if(!GetFileInformationByHandle(hFile, &fileInfo))
        {
            m_lastIOErrFile.load(pszArchive);
            return FC_AR_IO_READ_FILE;
        }
        else
        {
            ftLastModified  = fileInfo.ftLastWriteTime;
        }
       
        if(!ReadFile(hFile, &mhdr, sizeof(mhdr), &dw, NULL) || dw!=sizeof(mhdr))
            return FC_AR_BADMAGIC;
        //if we read a wrong file the magic header is garbage and maybe the string are not 
        //0-terminated, do it here:
        mhdr.szMagicVersion[FC_AR_MAX_VER-1] = 0;
        mhdr.szUserVersion[FC_AR_MAX_VER-1] = 0;

        if(_tcscmp(mhdr.szMagicVersion, FC_AR_SZ_MAGIC) || mhdr.lBinVersionNum<=0 || mhdr.nNodes<0)
            return FC_AR_BADMAGIC;
        
        if(mhdr.lBinVersionNum != FC_AR_BIN_VER && mhdr.lBinVersionNum != 1)
        {
            _stprintf(pszVerFound, _T("%d,%d or 1"),
                mhdr.lBinVersionNum,FC_AR_BIN_VER);
            return FC_AR_BADMAGIC;
        }
        m_bDoEncodeData = mhdr.lBinVersionNum > 1;
        m_BinVersionForWrite = mhdr.lBinVersionNum; //on write use same bin version as on open

        //copy the user version string after internal version is ok:
        _tcscpy(pszVerFound, mhdr.szUserVersion);

        //force loading the root:
        pRoot = new FC_CArDir(this, _T(""), NULL, hFile, mhdr.nNodes, sizeof(mhdr));
        FC_ARSTAT ret = pRoot->readDir();
        if(ret != FC_AR_OK)
        {
            delete(pRoot);
            CloseHandle(hFile);
            //clear the last fatal error, if open failed we want to be 
            //ready for a next open:
            assert(m_firstFatalError != FC_AR_OK);
            m_firstFatalError = FC_AR_OK;
            return ret;
        }
    }
    else
    {
        //create empty root:
        GetSystemTimeAsFileTime(&ftLastModified);
        m_bDoEncodeData = true; //this is a new (and hence version 2) archive => do encrypt
        m_BinVersionForWrite = FC_AR_BIN_VER;
        pRoot = new FC_CArDir(this, _T(""), NULL, INVALID_HANDLE_VALUE, 0, 0);

        //but if the archive did not exist on open at least
        //create an empty one (if bWrite), to do this in close() set the 
        //changed bit now:
        assert(bWrite);
        m_bChanged = true;
    }


    //set members:
    m_fileName.load(pszArchive);
    m_pRoot          = pRoot;
    m_bOpenWrite     = bWrite;
    m_ftLastModified = ftLastModified;
    m_hFileArOld     = hFile;
    m_lastIOErrFile.clear();
    return FC_AR_OK;
}


FC_ARSTAT FC_CArchive::close(
    bool         bWrite,
    TCHAR        pszUserVer[FC_AR_MAX_VER]
)
{
    DWORD       dw;
    HANDLE      hFileTmp;
    BOOL        bOkSys;
    FC_CString  szTmpAR(MAX_PATH); 
    AR_MagicHdr mhdr;
    FC_ARSTAT   ret;
    DWORD       dwSysIOErr;

    ret        = FC_AR_OK;
    dwSysIOErr = NO_ERROR;

    if(m_fileName.isEmpty())
    {
        assert(m_hFileArOld == INVALID_HANDLE_VALUE);
        goto finalClose;
    }
    if(!bWrite)
    {
        goto finalClose;
    }
    if(!m_bOpenWrite)
    {
        ret = FC_AR_NOT_OPENED_WRITE;
        goto finalClose;
    }
    if(m_firstFatalError!=FC_AR_OK)
    {
        ret = FC_AR_ERR_INERROR;
        goto finalClose;
    }
    if(!m_bChanged)
    {
        goto finalClose;
    }

    //open tmp file:
    szTmpAR.load(m_fileName);
    if(!MakeTmpAr(szTmpAR))
    {
        ret = FC_AR_IO_WRITE_FILE;
        m_lastIOErrFile.load(szTmpAR);
        dwSysIOErr = TYPE_E_CANTCREATETMPFILE;
        goto finalClose;
    }

    hFileTmp =  CreateFile(
      szTmpAR,                     // file name
      GENERIC_WRITE,               // access mode
      FILE_SHARE_READ,             // share mode
      NULL,                        // SD
      CREATE_ALWAYS,               // how to create
      FILE_FLAG_SEQUENTIAL_SCAN,   // file attributes
      NULL                         // handle to template file
    );
    if(hFileTmp == INVALID_HANDLE_VALUE)
    {
        ret = FC_AR_IO_WRITE_FILE;
        m_lastIOErrFile.load(szTmpAR);
        dwSysIOErr = GetLastError();
        goto finalClose;
    }


    //write the magic hdr:
    memset(&mhdr, 0, sizeof(mhdr));

    assert(_tcslen(FC_AR_SZ_MAGIC)<FC_ARRAY_LEN(mhdr.szMagicVersion));
    _tcscpy(mhdr.szMagicVersion, FC_AR_SZ_MAGIC);
    
    assert(_tcslen(pszUserVer)<FC_ARRAY_LEN(mhdr.szUserVersion));
    _tcsncpy(mhdr.szUserVersion, pszUserVer, FC_ARRAY_LEN(mhdr.szUserVersion));

    //m_BinVersionForWrite was set in FC_CArchive::open correctly:
    assert(m_BinVersionForWrite<=FC_AR_BIN_VER);
    mhdr.lBinVersionNum = m_BinVersionForWrite;

    mhdr.nNodes = m_pRoot->countNodes();

    if(!WriteFile(hFileTmp, &mhdr, sizeof(mhdr), &dw, NULL) || dw!=sizeof(mhdr))
    {
        ret = FC_AR_IO_WRITE_FILE;
        m_lastIOErrFile.load(szTmpAR);
        dwSysIOErr = GetLastError();
    }



    //write all data to tmp file:
    if(ret==FC_AR_OK)
    {
        ret = m_pRoot->writeNode(hFileTmp);
        if(ret == FC_AR_IO_READ_FILE || ret == FC_AR_IO_WRITE_FILE)
        {
            dwSysIOErr = GetLastError();
            m_lastIOErrFile.load(szTmpAR);
            assert(dwSysIOErr != NO_ERROR);
        }
    }


    bOkSys = CloseHandle(hFileTmp);
    assert(bOkSys);//ignore this error ... only assert.

    if(ret==FC_AR_OK)
    {
        if(m_hFileArOld != INVALID_HANDLE_VALUE)
        {
            bOkSys = CloseHandle(m_hFileArOld);
            m_hFileArOld = INVALID_HANDLE_VALUE;
            assert(bOkSys);//ignore this error ... only assert.
        }


        //EF win98: hack must delete first:
        DeleteFile(m_fileName);
        if(!MoveFile(
              szTmpAR,                  // pointer to the name of the existing file
              m_fileName                // pointer to the new name for the file
             )
        )

#if 0   //EF MoveFileEx is not ok for win98 !
        if(!MoveFileEx(
              szTmpAR,                  // pointer to the name of the existing file
              m_fileName,               // pointer to the new name for the file
              MOVEFILE_COPY_ALLOWED|
              MOVEFILE_WRITE_THROUGH|
              MOVEFILE_REPLACE_EXISTING // flag that specifies how to move file
             )
        )
#endif
        {
            ret = FC_AR_IO_WRITE_FILE;
            m_lastIOErrFile.load(m_fileName);
            dwSysIOErr = GetLastError();
            bOkSys = DeleteFile(szTmpAR);
            assert(bOkSys);//ignore this error ... only assert.
        }
    }
    else
    {
        bOkSys = DeleteFile(szTmpAR);
        assert(bOkSys);//ignore this error ... only assert.
    }




finalClose:
    if(m_hFileArOld != INVALID_HANDLE_VALUE) 
    {
        bOkSys = CloseHandle(m_hFileArOld);
        assert(bOkSys);//ignore this error ... only assert.
        m_hFileArOld = INVALID_HANDLE_VALUE;
    }
    if(m_hFileTmpPut != INVALID_HANDLE_VALUE) 
    {
        bOkSys = CloseHandle(m_hFileTmpPut);
        assert(bOkSys);//ignore this error ... only assert.
        m_hFileTmpPut = INVALID_HANDLE_VALUE;
    }
    delete(m_pRoot);
    m_pRoot = NULL;
    initMembers();
    if(ret==FC_AR_IO_WRITE_FILE || ret==FC_AR_IO_READ_FILE)
    {
        assert(dwSysIOErr != NO_ERROR);
        SetLastError(dwSysIOErr);
    }
    else
    {
        assert(dwSysIOErr == NO_ERROR);
    }
        
    return ret;
}




FC_ARSTAT FC_CArchive::writeToTmp(
    const void* pvData,
    const TCHAR*pszDiskFile, 
    DWORD*      pdwBytes, 
    DWORD*      pdwOffset, 
    HANDLE*     phFileTmp
)
{

    if(m_firstFatalError != FC_AR_OK) 
        return FC_AR_ERR_INERROR;


    if(m_hFileTmpPut == INVALID_HANDLE_VALUE)
    {
        //open tmp file:
        m_szTmpPutFile.load(m_fileName);
        if(!MakeTmpAr(m_szTmpPutFile))
        {
            SetLastError(TYPE_E_CANTCREATETMPFILE);
            m_lastIOErrFile.load(m_szTmpPutFile);
            m_firstFatalError = FC_AR_IO_WRITE_FILE;
            return FC_AR_IO_WRITE_FILE;
        }

        m_hFileTmpPut =  CreateFile(
          m_szTmpPutFile,              // file name
          GENERIC_WRITE|
          GENERIC_READ,                // access mode
          FILE_SHARE_READ|
          FILE_SHARE_WRITE,            // share mode
          NULL,                        // SD
          CREATE_ALWAYS,               // how to create
          FILE_FLAG_SEQUENTIAL_SCAN|
          FILE_ATTRIBUTE_TEMPORARY |
          FILE_FLAG_DELETE_ON_CLOSE,   // file attributes
          NULL                         // handle to template file
        );

        if(m_hFileTmpPut == INVALID_HANDLE_VALUE)
        {
            m_lastIOErrFile.load(m_szTmpPutFile);
            m_firstFatalError = FC_AR_IO_WRITE_FILE;
            return FC_AR_IO_WRITE_FILE;
        }

        assert(m_dwTmpPutCurrPos==0);
        m_dwTmpPutCurrPos = 0;
    }

    //write to disk and remember the offset in FC_CArFile:
    //aways seek to the eof, file pointer colud have changed
    //if a newly inserted file has been re-read:
    SetFilePointer(m_hFileTmpPut, m_dwTmpPutCurrPos, NULL, FILE_BEGIN);



    if(pszDiskFile)
    {
        HANDLE hFileDisk;

        //open file and check max file size:
        hFileDisk  = CreateFile(pszDiskFile,GENERIC_READ,FILE_SHARE_READ,NULL,
                            OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
        
        if(hFileDisk==INVALID_HANDLE_VALUE)
        {
            m_lastIOErrFile.load(pszDiskFile);
            return FC_AR_IO_READ_FILE;
        }

        *pdwBytes  = GetFileSize(hFileDisk, NULL);
        if (*pdwBytes == 0xFFFFFFFF)
        {
            m_lastIOErrFile.load(pszDiskFile);
            CloseHandle(hFileDisk);
            return FC_AR_IO_READ_FILE;
        }

        if(this->m_bDoEncodeData)
        {
            char* pDecoded = new char[*pdwBytes?*pdwBytes:1];//may be file size 0, cannot alloc 0 chars...
            DWORD dw;
            if(!ReadFile(hFileDisk, pDecoded, *pdwBytes, &dw, NULL) || *pdwBytes!=dw)
            {
                delete(pDecoded);
                CloseHandle(hFileDisk);
                return FC_AR_IO_READ_FILE;
            }
            char* pEncoded = new char[*pdwBytes + 64];
            DWORD nEncoded;
            FC_DESEncodeBlock(FC_DES_NORMAL, FC_AR_PW, pDecoded, *pdwBytes, pEncoded, &nEncoded);
            
            CloseHandle(hFileDisk);
            delete(pDecoded);

            *pdwBytes = nEncoded;
            
            if(!WrtFile(m_hFileTmpPut, pEncoded, nEncoded))
            {
                delete (pEncoded);
                m_firstFatalError = FC_AR_IO_WRITE_FILE;
                m_lastIOErrFile.load(m_szTmpPutFile);
                return FC_AR_IO_WRITE_FILE;
            }
            delete (pEncoded);
        }
        else
        {
            FC_ARSTAT ret = AppendFile(hFileDisk, m_hFileTmpPut, *pdwBytes);
            CloseHandle(hFileDisk);
            if(ret!=FC_AR_OK)
            {
                m_firstFatalError = FC_AR_IO_WRITE_FILE;
                m_lastIOErrFile.load(ret==FC_AR_IO_READ_FILE ? pszDiskFile : m_szTmpPutFile);
                return ret;
            }
        }
    }
    else
    {
        if(this->m_bDoEncodeData)
        {
            char* pEncoded = new char[*pdwBytes + 64];
            DWORD nEncoded;
            FC_DESEncodeBlock(FC_DES_NORMAL, FC_AR_PW, (const char*)pvData, *pdwBytes, pEncoded, &nEncoded);
            
            *pdwBytes = nEncoded;
            
            if(!WrtFile(m_hFileTmpPut, pEncoded, nEncoded))
            {
                delete (pEncoded);
                m_firstFatalError = FC_AR_IO_WRITE_FILE;
                m_lastIOErrFile.load(m_szTmpPutFile);
                return FC_AR_IO_WRITE_FILE;
            }
            delete (pEncoded);
        }
        else if(!WrtFile(m_hFileTmpPut, pvData, *pdwBytes))
        {
            m_firstFatalError = FC_AR_IO_WRITE_FILE;
            m_lastIOErrFile.load(m_szTmpPutFile);
            return FC_AR_IO_WRITE_FILE;
        }
    }


    *pdwOffset = m_dwTmpPutCurrPos;
    *phFileTmp = m_hFileTmpPut;
    m_dwTmpPutCurrPos += *pdwBytes;

    return FC_AR_OK;
}

#endif //UNICODE





/*
 *----------------------------------------------------------------------------*
 *  $History: fc_carchive.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 8.02.05    Time: 13:52
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * fixed d-169
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 12.11.04   Time: 10:53
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new archive format: do DES encode all file data
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
 * User: Ef           Date: 7.11.01    Time: 14:45
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * set last io error file name on open error
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 5.11.01    Time: 19:22
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed call to MoveFileEx (win98!)
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 22.10.01   Time: 23:26
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.10.01   Time: 19:35
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * first version of archive
 *==
 *----------------------------------------------------------------------------*
*/
