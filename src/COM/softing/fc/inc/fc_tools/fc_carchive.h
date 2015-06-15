/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_carchive.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_carchive.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_carchive.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
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
 *== archive file access 
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  22.10.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


#ifndef FC_CARCHIVE_H_
#define FC_CARCHIVE_H_

#ifndef UNICODE  //not supported for UNICODE rethink and rework this code, at least change the version 

//----  Aggregate Includes:   --------------------------------------*
#include <tchar.h>
#include <windows.h>

#include "fc_todebug\fc_assert.h"
#include "fc_todebug\fc_mem.h"
#include "fc_tools\fc_cstring.h"
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_fileIO.h"


//----  Forward Class Definitions:   -------------------------------*
class FC_CArDir;
class FC_CArFile;
class FC_CArchive;



//----  Prototypes:    ---------------------------------------------*
typedef enum FC_ARSTAT
{
//ok
    FC_AR_OK
   ,FC_AR_OUTOFMEM
//generall IO errors
   ,FC_AR_IO_READ_FILE     //cannot read file '%file': %sysErr
   ,FC_AR_IO_WRITE_FILE    //cannot write file '%file': %sysErr
   ,FC_AR_CIM_NO_SUCH_FILE //check in from disk multiple: no file found   
//version error:
   ,FC_AR_BADMAGIC         //archive has bad binary format version
//binary format errors while reading:
   ,FC_AR_CORRUPTED        //archive corrupted 
//bad arguments or illegal calls:
   ,FC_AR_NOT_OPENED_WRITE //write archive: was not opened for writing
   ,FC_AR_BAD_FILE_NAME    //create file/folder: name contains invalid characters *?\/
   ,FC_AR_BUFFER_TOO_SMALL //the supplied buffer is too small
   ,FC_AR_ERR_INERROR      //cannot use archive after previous fatal error (leave existing archive unchanged)
};





class FC_CArNode
{
    friend class FC_CArDir;
    friend class FC_CArFile;

    int          m_lStrLenId;
    TCHAR*       m_pszId;
    FC_CArDir*   m_pParent;

    FC_CArNode(
        const TCHAR* pszId, 
        FC_CArDir*   pParent
    );
    virtual ~FC_CArNode();

    DWORD        m_dwFinalOff;
    virtual FC_ARSTAT writeNode(HANDLE hFile) = 0;
    virtual FC_ARSTAT writeNodeInfo(HANDLE hFile) =0;

public:
    virtual FC_CArDir*  isDir();
    virtual FC_CArFile* isFile();
    FC_CArDir* getParent(){return m_pParent;}

    FC_CString&  getFullName(FC_CString& path)const;
    const TCHAR* getName()const{return m_pszId;}
    int          getIdLen()const{return m_lStrLenId;}
};


class FC_CArFile : public FC_CArNode
{
    friend class FC_CArDir;
    HANDLE    m_hData;
    DWORD     m_dwBytesRaw;
    DWORD     m_dwBytesDecoded;
    DWORD     m_dwOffset;

    FC_CArFile(
        const TCHAR* pszId, 
        FC_CArDir*   parent,
        HANDLE       hData,
        DWORD        dwBytesRaw,
        DWORD        dwOffset,
        FC_ARSTAT*   pRet
    );
    ~FC_CArFile(){}
    FC_ARSTAT appendToFileRaw(HANDLE hFile);
    virtual FC_ARSTAT writeNode(HANDLE hFile);
    virtual FC_ARSTAT writeNodeInfo(HANDLE hFile);

public:
    virtual FC_CArFile* isFile();
 
    DWORD      getSize()const;
    FC_ARSTAT  getData(DWORD dwBuffer, void* pvData);
    FC_ARSTAT  getToDisk(const TCHAR* pszDirectory, const TCHAR* pszDiskFileName);
};



class FC_CArDir : public FC_CArNode
{
    friend class FC_CArchive;
    friend class FC_CArFile;
    FC_CArchive*              m_pAr;
    HANDLE                    m_hData;
    long                      m_nNodes;
    long                      m_nDirs;
    DWORD                     m_dwOffset;
    FC_CHashStr<FC_CArNode*>* m_pDir;

    FC_CArDir(
        FC_CArchive*  pAr,
        const TCHAR*  pszId, 
        FC_CArDir*    parent,
        HANDLE        hData,
        long          nFiles,
        DWORD         dwOffset
    );
    ~FC_CArDir();
    FC_ARSTAT readDir();

    virtual FC_ARSTAT writeNode(HANDLE hFile);
    virtual FC_ARSTAT writeNodeInfo(HANDLE hFile);

public:

    virtual FC_CArDir* isDir();

    FC_ARSTAT createDir(
        const TCHAR* pszId,
        FC_CArDir**  ppDir
    );
    FC_ARSTAT createFile(
        const TCHAR* pszId, 
        const TCHAR* pszDiskFile,
        DWORD        dwBytes,
        const void*  pvData
    );


    FC_ARSTAT createFileFromMem(
        const TCHAR* pszId, 
        DWORD        dwBytes,
        const void*  pvData
    )
    {
        return createFile(pszId, NULL, dwBytes, pvData);
    }
    FC_ARSTAT createFileFromDisk(
        const TCHAR* pszDiskFile,
        const TCHAR* pszId 
    )
    {
        return createFile(pszId, pszDiskFile, -1, NULL);
    }
    FC_ARSTAT createFilesFromDisk(
        const TCHAR* pszFileSpec,  //path and file name, file name may have ?*
        bool         bRecurseDirs  //true: recurse into subdirs of pszPath 
    );

    //removes files and recursively directories of this dir.
    //pszId may have ? and * wildcards.
    FC_ARSTAT remove(const TCHAR* pszId, int *pnDel);


    long countNodes()  const{return m_nNodes;}
    FC_ARSTAT countSubDirs(int* pnDirs);

    FC_ARSTAT getSubDir(const TCHAR* pszId, FC_CArDir** ppDir);
    FC_ARSTAT getFile  (const TCHAR* pszId, FC_CArFile** ppFile);


    //creates array of nodes to queery:
    FC_ARSTAT getNodes(
        const TCHAR*            pszFileSpec, //NULL or a filename with opt. *? wildcards
        bool                    bFiles, //want files
        bool                    bDirs,  //want dirs
        FC_CGArray<FC_CArNode*>*pNList  //out receives the result
    );

    FC_ARSTAT getFilesToDisk(
        const TCHAR*             pszDirectory,
        const TCHAR*             pszFileSpec, //NULL (same as "*") or any *? pattern
        bool                     bRecursive,
        FC_CGArray<FC_CArNode*>* pFileList
    );
};


#define FC_AR_MAX_VER 32
class FC_CArchive
{
    friend class FC_CArDir;
    friend class FC_CArFile;

    FILETIME   m_ftLastModified;
    FC_CStr    m_fileName; 
    HANDLE     m_hFileArOld;
    bool       m_bOpenWrite;
    bool       m_bChanged;
    bool       m_bDoEncodeData;
    long       m_BinVersionForWrite;
    FC_CString m_szTmpPutFile;
    HANDLE     m_hFileTmpPut;
    DWORD      m_dwTmpPutCurrPos;
    FC_CArDir* m_pRoot;
    FC_ARSTAT  m_firstFatalError; //set to !=FC_AR_OK if fatal error 
    FC_CString m_lastIOErrFile;   //file name of last file with IO errors

    void initMembers();
    FC_ARSTAT writeToTmp(
        const void* pvData,
        const TCHAR*pszDiskFile, 
        DWORD*      pdwBytes, 
        DWORD*      pdwOffset, 
        HANDLE*     phFileTmp
    );

public:
    FC_CArchive();
    ~FC_CArchive();


    /**
     * Open or create archive. Any currently open archive is closed without writing,
     * in this case all currently used FC_CArNode* pointers gets invalid.
     *
     * If open() fails, regardless of the reason, this object is ready for a
     * next attempt to open an archive without prior close; the getlastArError() will 
     * return FC_AR_OK and the isOpen() will return false.
     *
     * @param    pszArchive  Filename of archive to open or create.
     * @param    bWrite      True if open for writing.
     * @param    pszVerFound If return is FC_AR_OK:
     *                       a) pszArchive exits: 
     *                          pszVerFound = the user version string found. 
     *                          The caller may use it to check the version of the 
     *                          content of the archive.
     *                       b) pszArchive not exit already (only if bWrite was true):
     *                          pszVerFound = ""
     *
     *                       If return is FC_AR_BADMAGIC:
     *                       a) If the file does not look at all like an achive: 
     *                          pszVerFound = ""
     *                       b) Binary version number found does not match the current:
     *                          pszVerFound = "<bin ver num found>,<bin ver num expected>"
     *                          Example:    "1,2"
     *                       
     * @return   one of the following:
     *           FC_AR_OK              succeeded
     *           FC_AR_IO_READ_FILE    use GetLastError() for further system diag.
     *           FC_AR_IO_WRITE_FILE   use GetLastError() for further system diag.
     *           FC_AR_BADMAGIC        see the value of pszVerFound for more info.
     *           FC_AR_CORRUPTED       while reading the root directory of an existing archive.
     */
    FC_ARSTAT open(
        const TCHAR* pszArchive, 
        bool         bWrite,
        TCHAR        pszVerFound[FC_AR_MAX_VER]
    );

    /**
     * Close and optionally writes a succesfully opened archive. 
     * All currently used FC_CArNode* pointers gets invalid and the 
     * archive gets written if:
     * a) there are changes and b) bWrite is true. 
     *
     * Closing an archive resets the internal state as if calling 
     * "new FC_CArchive()". If currently no archive is opened, returns 
     * always FC_ARR_OK.
     *
     * @param    bWrite      True if want to safe changes. To force writing
     *                       even if no changes call setChanged() before.
     * @param    pszUserVer  A user supplied version string for the content
     *                       of the archive. If bWrite is true it must at 
     *                       least be an empty string. If bWrite is false it is
     *                       ignored and can be can also be NULL.
     *
     * @return   one of the folloing: 
     *           FC_AR_OK               succeeded
     *           FC_AR_IO_READ_FILE     use GetLastError() for further system diag.
     *           FC_AR_IO_WRITE_FILE,   use GetLastError() for further system diag.
     *           FC_AR_CORRUPTED        while reading the existing archive for adding changes.
     *           FC_AR_NOT_OPENED_WRITE if bWrite is true but was not opened for writing
     *           FC_AR_ERR_PENDING      if bWrite is true and previous errors would cause a 
     *                                  possibly invalid archive.
     */
    FC_ARSTAT close(
        bool         bWrite,
        TCHAR        pszUserVer[FC_AR_MAX_VER]
    );
 
    FC_CArDir*   getRoot()                    {return m_pRoot;}
    void         setChanged()                 {m_bChanged = true;}
    bool         isOpenWrite()const           {return m_bOpenWrite;}
    const TCHAR* getFileName()const           {return m_fileName;}
    bool         isOpen()const                {return m_pRoot!=NULL;}
    FILETIME     getLastModTime()const        {return m_ftLastModified;}
    const TCHAR* getLastIOErrorFileName()const{return m_lastIOErrFile;}
};



#endif //FC_CARCHIVE_H_

#endif //UNICODE


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_carchive.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 8.02.05    Time: 13:53
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * fixed d-169
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 12.11.04   Time: 10:50
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new archive format: do DES encode all file data
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 11.03.02   Time: 15:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * removed dead code
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 22.10.01   Time: 23:26
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.10.01   Time: 19:34
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * first version of archive
 *==
 *----------------------------------------------------------------------------*
*/
