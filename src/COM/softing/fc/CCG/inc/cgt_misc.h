
#ifndef __CGT_MISC_H_
#define __CGT_MISC_H_



//----  Aggregate Includes:   --------------------------------------*
#include "CGT_CDiagMsg.h"

interface I4CLAccess;

//----  Defines:    ------------------------------------------------*
//defined for CGT_WriteFile:
#define CGT_WF_APPEND     0x001   //work in append mode 
#define CGT_WF_TEXTMODE   0x002   //work in text mode, default is bin
#define CGT_WF_CRT_PATH   0x004   //create path if needed
#define CGT_WF_TOLOWER    0x008   //make path and file lower case
//recommended standard flag set:
#define CGT_WF_STD  (CGT_WF_CRT_PATH|CGT_WF_TOLOWER)

//for all 'file to mem' operations this is the max byte size:
#define CGT_MAXMEM_FILE_SIZE 20000000


//use only UTF-8 for all XML dumping:
#define CGT_XML_STANDARD_HDR _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")


//----  Prototypes:    ---------------------------------------------*
//------------------------------------------------------------------*
/**
 * CGT_DeleteFile can be used to delete a single file.
 *
 * @param   pdm        diag message sink
 * @param   pszDir     for convenience only may be NULL
 *                     will be prepended as dir to pszFile
 *                     if pszFile isn't absolute path.
 * @param   pszFile    a [path\]file name
 *
 * @return  true if no IO error or false if file could not be deleted.
 *          It is not an error if the file or path
 *          does simply not exits.
 *
 * @see     CGT_DeleteFiles
*/
extern "C" bool CGT_DeleteFile(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszDir,
    const TCHAR*  pszFile
);




//------------------------------------------------------------------*
/**
 * CGT_CopyFile can be used to copy a file. If pszDst does exist it 
 * is overwritten.
 *
 * @param   pdm        diag message sink
 * @param   pszSrcDir  if not NULL: is used when pszSrc is not absolute
 * @param   pszSrc     source file name with opt. full or rel path
 * @param   pszDstDir  if not NULL: is used when pszDst is not absolute
 * @param   pszDst     source file name with opt. full or rel path
 *
 * @return  true if no IO error or false if file could not be copied.
*/
extern "C" bool CGT_CopyFile(
    CGT_CDiagMsg* pdm,
    const TCHAR*  pszSrcDir,
    const TCHAR*  pszSrc,
    const TCHAR*  pszDstDir,
    const TCHAR*  pszDst
);



//------------------------------------------------------------------*
/**
 * CGT_DeleteFiles can be used to clean up e.g. the gen directory.
 * For safetyness there are two restrictions:
 *  - If bSubDirs is true and the path does not contain a 
 *    substring "\gen" raise an internal compiler error. 
 *  - If pszPattern contains a "*" , and the path does not contain 
 *    a substring "\gen" raise an internal compiler error.  
 * 
 * To delete a single file use CGT_DeleteFile.
 *
 * @param   pdm         diag message sink
 * @param   pszDir      for convenience only may be NULL
 *                      will be prepended as dir to pszPattern
 *                      if pszPattern isn't absolute path.
 *
 * @param   pszPattern  a [path\]file name may have win32 
 *                      wild cards in its file portion.
 *
 * @param   pszExcFiles Exclude file filter, may be NULL.
 *                      If not NULL: Do not delete files 
 *                      that match pszExcFiles, may contain 
 *                      win32 wildcards.
 *
 * @param   bSubDirs    if true also process sub directories.
 *                      The subdirectories itself are not 
 *                      deleted (makes always problems if 
 *                      opened in Explorer)
 *                      BE CAREFULL and read note above.
 *
 * @param   pszExcDirs  Exclude direcory filter, may be NULL.
 *                      If bSubDirs is true and pszExcDirs
 *                      is not NULL: do not delete directories 
 *                      that match pszExcDirs, may contain
 *                      win32 wildcards. Example: use "__*" to
 *                      avoid deletion of the "__cedl" downloder
 *                      directory in gen\<target>\__cedl.
 *
 * @return  true if no IO error or false if at least one
 *          file could not be deleted. The function returns
 *          at the first error. 
 *          It is not an error if pszDir or pszPattern
 *          does simply not exits.
 *
 * @see     CGT_DeleteFile
*/
extern "C" bool CGT_DeleteFiles(
    CGT_CDiagMsg*pdm,
    const TCHAR* pszDir,
    const TCHAR* pszPattern,
    const TCHAR* pszExcFiles,
    bool         bSubDirs,
    const TCHAR* pszExcDirs
);





//------------------------------------------------------------------*
/**
 * CGT_WriteFile can be used to write to a disk file.
 * Dependig on the flags parameter the following will be done:
 *  - create path if doesn't exist
 *  - use lower case path and files names
 *  - append to file or overwrite existing
 *  - binary or text mode
 *
 * @param   pdm        diag message sink
 * @param   pszDir     for convenience only may be NULL
 *                     will be prepended as dir to pszFile
 *                     if pszFile isn't absolute path.
 * @param   pszFile    a [path\]file name
 * @param   pBuffer    buffer containing data to write/apped to file
 * @param   nBytes     number of bytes in pBuffer
 * @param   flags      bitset of CGT_WF* defines in this header.
 *
 * @return  true if no IO error or false if any
 *
 * @see     CGT_WF_* defines in this header.
*/
extern "C" bool CGT_WriteFile(
    CGT_CDiagMsg*pdm,
    const TCHAR* pszDir,  //can be NULL 
    const TCHAR* pszFile,  //must be given
    const void*  pBuffer, 
    long         nBytes, 
    long         flags
);




extern "C" bool CGT_LoadTextFileToMem(
    CGT_CDiagMsg*    pdm,          //for err msg
    const CG_Edipos* pEdp,         //error location usually NULL
    const TCHAR*     pszPath,      //absolute path or NULL
    const TCHAR*     pszFile,      //file name, if pszPath was NULL 
                                   //must contain full path
    TCHAR**          ppCharBuffer, //newly allocated buffer for file content 
    unsigned long*   pulLength,    //NULL or receives the size in TCHARs of
                                   //*ppCharBuffer without the always
                                   //added terminating 0-TCHAR.
    __int64*         pftLastWrite  //NULL or receives the last file write time
);


//If ppCharBuffer is NULL:
//   Only test for file existence, and if does not exist NO error 
//   message is submitted, also the pulLength and pftLastWrite 
//   outputs are set (if not NULL).
//
extern "C" bool CGT_GetLIBFileData(
    CGT_CDiagMsg*    pdm,          //for err msg
    I4CLAccess*      pLibAcc,      //used if locator points to a 4cl lib
    const TCHAR*     pszPOUname,   //must be given 
    const TCHAR*     pszTRGname,   //can be NULL or empty string
    const TCHAR*     pszFILEname,  //must be given
    TCHAR**          ppCharBuffer, //newly allocated buffer for file content 
    unsigned long*   pulLength,    //NULL or receives the size in TCHARs of
                                   //*ppCharBuffer without the always
                                   //added terminating 0-TCHAR.
    __int64*         pftLastWrite  //NULL or receives the last file write time
);



//TODO comment, 
//returns the number of files checked out 
//or 0 if any error
extern "C" long CGT_LIBCheckOutFiles(
    CGT_CDiagMsg*        pdm,           //for err msg
    ICG_IECFront*        pIFront,       //used to get 4cl COM IF by context info
    const CG_LIBContext* pLibCtx,       //4CL context info
    const TCHAR*         pszPOUname,    //must be given 
    const TCHAR*         pszTRGname,    //can be NULL or empty string
    const TCHAR*         pszFILEname,   //must be given
    const TCHAR*         pszCheckOutDir //dest directory
);




//used to get any source file via a locator string 
//even if inside libs (this is the main purpose of the function)
//pszFileLocator is e.g. "myLib.4cl::myPOU\4cpc\myPOU.st"
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
);






typedef struct _CGT_FileLocator
{
    bool   bIsLib;                  //true only if lib location
    TCHAR  pszFile   [2*MAX_PATH]; //disk file or 4cl file, fullpath or relative to project folder
    //if bIsLib next members are set otherwise they are all set empty:  
    TCHAR  pszPOUName[MAX_PATH];   //the POU in the lib, never empty, example: FunctionBlock1.
    TCHAR  pszTrgType[MAX_PATH];   //empty or the target type folder, example: 4cwin
    TCHAR  pszLibFile[MAX_PATH];   //the lib file, never empty, example: FunctionBlock1.4gr
} CGT_FileLocator;

//------------------------------------------------------------------*
/**
 * CGT_SplitFileLocator can be used to classify a CG file locator.
 * If param pSplitted is NULL the function only tests if it's
 * a library location, does not validate syntax.
 * 
 * @param   pszFileLocator  file locator to be classified
 * @param   pSplitted       NULL or receives the individual parts 
 *                          of the input pszFileLocator.
 *
 * @return   0: invalid locator syntax (will be asserted)
 *           1: file is a disk file
 *           2: file is a library file but no target type
 *           3: file is a library file and has a target type
 * 
 * @see      struct CGT_FileLocator, ICG_IECFront::FCPgetPOUDefFileLocator in CG_IECFront.h, 
 *           there is also a generall description of file locator syntax.
*/
extern "C" long CGT_SplitFileLocator(
    const TCHAR*     pszFileLocator, 
    CGT_FileLocator* pSplitted
);


//------------------------------------------------------------------*
/**
 * CGT_IsLibLocation can be used to classify a CG file locator.
 * If param pSplitted is NULL the function only tests if it's
 * a library location, does not validate syntax.
 * 
 * @param   pszFileLocator  file locator to be classified
 * @param   pSplitted       NULL or receives the individual parts 
 *                          of the input pszFileLocator.
 *
 * @return   0: invalid locator syntax (will be asserted)
 *           1: file is a disk file
 *           2: file is a library file
 * @see      struct CGT_FileLocator, ICG_IECFront::FCPgetPOUDefFileLocator in CG_IECFront.h, 
 *           there is also a generall description of file locator syntax.
*/
extern "C" int CGT_IsLibLocation(
    const TCHAR*     pszFileLocator 
);



//------------------------------------------------------------------*
/**
 * CGT_ExistsLocatorFile can be used check if a file exists, the file
 * is identified by it's locator. If the locator points to library 
 * this library must be one of the 4CP [libs] section and 
 * ICG_IECFront::loadLIBs  must have been called already.
 * If ICG_IECFront::loadLIBs was not called or any system error
 * from a library submitt an internal error and return -1.
 * 
 * @param   pdm             error message sink
 * @param   pIFront         used if locator points to a 4cl lib
 * @param   pszFileLocator  file loactore to find
 * @param   ppLibCtx        if not NULL receives a possible lib context,
 *                          this is the same object as pIFront->FCPgetLIBinfo
 *                          would return for the library id.
 * 
 * @return  0: file does not exist 
 *          1: files exist and is a disk file
 *          2: files exist and is a library file
 *         -1: internal error, will be submitted via pdm
 * @see     CGT_SplitFileLocator, ICG_IECFront::FCPgetPOUDefFileLocator, 
 *          there is also a generall description of locator syntax.
 */
extern "C" long CGT_ExistsLocatorFile(
    CGT_CDiagMsg*         pdm,            //error message sink
    ICG_IECFront*         pIFront,        //used if locator points to a 4cl lib
    const TCHAR*          pszFileLocator, //file loactore to find.
    const CG_LIBContext** ppLibCtx        //if not NULL receives a possible lib context
);







//Reading a CSV-File:
class CGT_CSVFile
{
    //never use:
    CGT_CSVFile(CGT_CSVFile&);
    void operator=(CGT_CSVFile&);
    
    TCHAR*        m_pszFileContent;
    unsigned long m_ulLength;
    unsigned long m_ulFpos;
    bool          m_lastFieldWasEOL;
    long          m_nCurrLine;
    bool          m_bDelete;

    TCHAR nextChar()
    {
        TCHAR c = m_ulFpos<m_ulLength ? m_pszFileContent[m_ulFpos++] : 0;
        if(c==_T('\n'))
            m_nCurrLine++;
        return c;
    }

public:
    CGT_CSVFile();
    ~CGT_CSVFile();

    void setStringContent(
        TCHAR* psz,    //pointer to raw text, the buffer will be manipulated
        long   iLen,   //lstrlen(psz) or -1 the function calls lstrlen.
        bool   bDelete //if true ~CGT_CSVFile will delete psz
    );
    
    //------------------------------------------------------------------*
    /**
     * getNextField sets *ppszField to a 0-term string that is the 
     * value of the next field. 
     * The pointer point into the memory passed in setStringContent, 
     * this memory is therefore changed the 0-char is inserted and 
     * possible escapes are processed in place.
     * This function can also be used if the field has embedded 
     * 0-chars then the pLen parameter must be used.
     * 
     *
     * @param   ppszField address of pointer to string to be set to 
     *                    next field's value.
     * @param   pLen      optional long* to receive the number of 
     *                    TCHARs of the field, works also if embedded 
     *                    0-chars.
     *
     * @return  false if any errors, error can be invalid escape syntax
     *          or unexpected EOF or embeded 0-char. To get the line info
     *          use getLineno() after error condition.
     *          TODO supply a last error code ? or return int ?
    */
    bool  getNextField(
        const TCHAR** ppszField,
        long*         pLen = NULL
    );

    //------------------------------------------------------------------*
    /**
     * getRecordFields reads a complete record and stores the values
     * into values into an array of TCHAR* as getNextField would do.
     * If there are more fields as the supplied array can hold the function 
     * returns without reading the rest of the record.
     * This function can also be used if the field has embedded 
     * 0-chars then the pLen parameter must be used.
     * 
     * @param   nMaxFields  >0, the max number of filed expected in 
     *                      record.
     * @param   ppszFields  array of pointer to string to be set to 
     *                      next field's value.
     * @param   pLen        optional array of long* to receive the 
     *                      number of TCHARs of the field, works also 
     *                      if embedded 0-chars.
     *
     * @return  >0: the number of succesfully read and stored fields.
     *          -1: any errors, error can be invalid escape syntax
     *              or unexpected EOF or embeded 0-char. To get the 
     *              line info use getLineno() after error condition.
     * @see      getNextField
    */
    long  getRecordFields(
        long          nMaxFields,
        const TCHAR*  ppszFields[/* nMaxFields*/],
        long*         pLen[/* nMaxFields*/] = NULL
    );

    const TCHAR*      getCurrChars()const{return m_pszFileContent + m_ulFpos;}
    bool  hasMoreFields() {return m_ulFpos<m_ulLength;}
    bool  isEOF()         {return m_ulFpos>=m_ulLength;}
    bool  isLastFieldEOL(){return m_lastFieldWasEOL;}
    long  getLineno()const{return m_nCurrLine;}
};

extern "C"  bool  CGT_LoadCSVFile(
    CGT_CDiagMsg*    pdm,      //error message sink
    CGT_CSVFile*     pReader,  //will be initialised with file
    const TCHAR*     pszPath,  //absolute path or NULL
    const TCHAR*     pszFile   //file name, if pszPath was NULL 
                               //must contain full path
);

extern "C"  bool  CGT_LoadCSVLocatorFile(
    CGT_CDiagMsg*    pdm,          //error message sink
    CGT_CSVFile*     pReader,      //will be initialised with file
    ICG_IECFront*    pIFront,      //used if locator points to a 4cl lib
    const TCHAR*     pszFileLocator//file locator
);


//TODO comment
class CGT_CDmp
{
    //never use:
    CGT_CDmp(CGT_CDmp&);
    void operator=(CGT_CDmp&);

    TCHAR itoa_puff[64];
    enum {nIndentSpaces = 4};
    
    unsigned long m_ulMaxBuffer;
    unsigned long m_ulUsedBuffer;
    TCHAR*        m_pBuffer;
    int           nIdentLevel;
    long          lineNumber;
    int           columnPos;

    inline void putchar(int c)
    {  
        if(m_ulUsedBuffer>=m_ulMaxBuffer)
            growBuffer(0);
        
        m_pBuffer[m_ulUsedBuffer++]=(TCHAR)c;
        columnPos++;
    }
    void growBuffer(unsigned long ulMin);

public:
    CGT_CDmp();
    ~CGT_CDmp();            

    void append(CGT_CDmp& dmp);
    void incrIndent()    {++nIdentLevel;}
    void decrIndent()    {--nIdentLevel;assert(nIdentLevel>=0);}
    int  getIndent()const{return nIdentLevel;}
    void setIndent(int i){assert(i>=0); nIdentLevel=i;}
    long lineNo()const   {return(lineNumber);}
    int  column()const   {return(columnPos);}

    unsigned long ftell()const   {return m_ulUsedBuffer*sizeof(TCHAR);}
    const TCHAR* getPChars()const{return m_pBuffer;}
    long         getNChars()const{return m_ulUsedBuffer;}

    void seek(unsigned long pos)
    {
        assert(pos<=m_ulUsedBuffer);//cannot seek after end
        m_ulUsedBuffer = pos;
    }
    const TCHAR* getSzBuffer()
    {
        if(m_ulUsedBuffer>=m_ulMaxBuffer)
            growBuffer(0);
        
        m_pBuffer[m_ulUsedBuffer]=0;
        return m_pBuffer;
    }
    CGT_CDmp& operator<<(TCHAR dmpMe);
    CGT_CDmp& operator<<(const TCHAR* dmpMe);
#ifndef UNICODE
    CGT_CDmp& operator<<(const wchar_t* dmpMe);
#endif

    CGT_CDmp& operator<<(int dmpMe)
    {
      return *this<<itoa(dmpMe,itoa_puff,10);
    }
    CGT_CDmp& operator<<(unsigned long dmpMe)
    {
      return *this<<ultoa(dmpMe,itoa_puff,10);
    }
    CGT_CDmp& operator<<(long dmpMe)
    { 
      return *this<<ltoa(dmpMe,itoa_puff,10);
    }
    CGT_CDmp& operator<<(__int64 dmpMe)
    { 
        //CAUTION: the runtime function _i64tot does not work with negative input
        //handle negative input by hand!
        if(dmpMe<0)
            return *this<<'-'<<_ui64tot((unsigned __int64)-dmpMe,itoa_puff,10);
        else
            return *this<<_ui64tot((unsigned __int64)dmpMe,itoa_puff,10);
    }
    CGT_CDmp& dumpULONGAsHex(unsigned long ul, int iMinText);
    CGT_CDmp& lo(const TCHAR* dmpMe);  
    CGT_CDmp& up(const TCHAR* dmpMe);  
    CGT_CDmp& sep(int nPos);  

    CGT_CDmp& dumpAsXMLUTF8(const TCHAR* dmpMe, bool bDoquote=false);
    CGT_CDmp& xmlAttrVal(const TCHAR* dmpMe);
    CGT_CDmp& dumpAsCSV(
        const TCHAR* pBinStr, 
        int          iChars,
        bool         bDumpEscapes
    );
};

extern "C" bool CGT_CDmpCSVNeedEscape(
    const TCHAR* pBinStr, 
    int          iChars
);


extern "C" bool CGT_DmpToFile(
    CGT_CDiagMsg*   pdm, 
    const TCHAR*    pszPath,  //optional can be NULL
    const TCHAR*    pszFile,  //must be ful path if pszPath is NULL
    const CGT_CDmp& dmp, 
    bool            bAppend
);



typedef struct _CGT_IPathPos
{
  int depth;
  int strLen;
} CGT_IPathPos;

class CGT_CInstPath
{
    enum{maxPath=5000};
    TCHAR buffer[maxPath+10];
    int iLen;
    int nDepth;
    
public:
    CGT_CInstPath();


    void append(const TCHAR* next);
    void appendLo(const TCHAR* next);
    void appendInt(long l);
    //pszAddr must not have '{' '}' already, will be 
    //inserted by appendTargetAddr:
    void appendTargetAddr(const TCHAR* pszAddr);

    operator const TCHAR *()const {return buffer;}
    void getPos(CGT_IPathPos* pos)const
    {
        pos->depth=nDepth;
        pos->strLen = iLen;
    }
    int getDepth()const{return nDepth;}
    int getStrLen()const{return iLen;}


    const TCHAR* stripFirstN(int n)const;
    void         stripLast();
    void         cutAt(CGT_IPathPos p);
};






//------------------------------------------------------------------*
/**
 * CGT_AppendCMDLFileArg appends a file name to a command line.
 * The function inserts string escapes '"' like "<filename>" if
 * needed. 
 *
 * @param   pCmdl     string object where to append to
 * @param   pszFile   file name, may or may not contain
 *                    embedded blanks, leading/trailing
 *                    blanks also cause '"' to be used.
 *
 * @return          void
*/
extern "C" FC_CString& CGT_AppendCMDLFileArg(
    FC_CString&  cmdl, 
    const TCHAR* pszFile
);



extern "C" typedef struct _CGT_DollarInserts
{
    const TCHAR*  pszId;   //$(<pszId>) must not contain '(' ')'
    const TCHAR*  pszSub;  //substitute string 
} CGT_DollarInserts;


#define CGT_INS_ENV         0x0001
#define CGT_INS_PRINTHELP   0x0002
#define CGT_INS_WARN_UNDEF  0x0004
#define CGT_INS_ESCAPE      0x0008 
//CGT_INS_ESCAPE    : if true assume pszSub is a file name and if contains blanks
//                   insert '"'<pszSub>'"' ss CGT_AppendCMDLFileArg would do.
//CGT_INS_ENV       : enable $env(<environment>)
//CGT_INS_PRINTHELP : If undefined inserts print all available 
//CGT_INS_WARN_UNDEF: Warn undefined inserts 
//------------------------------------------------------------------*
/**
 * CGT_SubsDollarIns can be used to expand $(<id>) macros in a 
 * string, typically used to build command lines. 
 * The syntax "$ (" is not considered as insert nor as warning.
 * The inserted text is not rescanned for $(<id>). The syntax $( name)
 * results in a warning of the form:
 *          warning : '$( name)' not expanded 
 * if bWarn is true
 * White space inside $(...) belongs to <id>.
 *
 * @param   pdm        diag message sink
 * @param   pszOrig    source string where to expand macros
 * @param   pEdpOrig   used as edp for any diags 
 *                     the functions tries to adapt the column
 *                     for every diag if possible (must be ST 
 *                     edipos including column info)
 * @param   inserts    array of insert descriptions
 * @param   nInserts   number of inserts in array
 * @param   flags      combination of CGT_INS_* bits
 * @param   pResult    final result, the pResult->clear()
 *                     is called first, existing string content
 *                     is lost on entry.
 *
 * @return   0  if syntax error: missing ')'
 *          +1  if all ok
 *          -1  if at least one insert was undefined and not expanded
 * @see      CGT_DollarInserts
*/
extern "C" int CGT_SubsDollarIns(
    CGT_CDiagMsg*     pdm, 
    const TCHAR*      pszOrig,
    const CG_Edipos*  pEdpOrig, 
    CGT_DollarInserts inserts[/*nInserts*/], 
    long              nInserts, 
    long              flags,
    FC_CString*       pResult
);
extern "C" int CGT_SubsStdDollarIns(
    CGT_CDiagMsg*     pdm, 
    ICG_IECFront*     pIFront,
    const TCHAR*      pszOrig,
    const CG_Edipos*  pEdpOrig, 
    long              flags,
    FC_CString*       pResult
);

const TCHAR* CGT_GetWinDir();
const TCHAR* CGT_GetSysDir();
const TCHAR* CGT_GetSetupDir(CGT_CDiagMsg* pdm);  //"C:\4c2"
const TCHAR* CGT_GetEngBinDir(CGT_CDiagMsg* pdm); //"C:\4c2\Engineering\bin"
const TCHAR* CGT_GetSTDLibPath(CGT_CDiagMsg* pdm);//"C:\4c2\Engineering\lib"




//------------------------------------------------------------------*
/**
 * CGT_CheckNumOption can be used to chacke option strings of the 
 * form <identifier><digit>.
 * Example: opt=verbose:2
 *
 * @param   pdm        diag message sink
 * @param   pEdp       edipos of pszVal, may be NULL too.
 * @param   pszVal     value string to be tested (must be trimmed)
 * @param   pszOpt     assumed option name: example "verbose:"
 * @param   maxVal     number of inserts in array
 * @param   piVal      receives the value if return code is +1
 *                     otherwise remains unchanged.
 *
 * @return   0  if pszVal does not start with pszOpt (ignore case)
 *          +1  if all ok, pVal is noe changed.
 *          -1  text after pszOpt is not a digit or not in range [0..maxVal]
*/
int CGT_CheckNumOption(
    CGT_CDiagMsg*     pdm,
    const CG_Edipos*  pEdp,
    const TCHAR*      pszVal, 
    const TCHAR*      pszOpt, 
    int               maxVal,
    int*              piVal
);



extern "C" const TCHAR* CGT_PouTypeName(CG_POUTYP pt, bool bEndMaker);
extern "C" const TCHAR* CGT_StoreClassName(CG_DECLSCOPE s);
extern "C" CG_BITSET CGT_DTypesToTBS(CG_DTYPES dt);
// variable type must not be polymorph
extern "C" CG_DTYPES CGT_TBSToDTypes(CG_BITSET type);
extern "C" CG_DTYPES CGT_TypeKeywordToCG_DTYPES(const TCHAR* pszTypeKW);

//Upper case basic type name.
//Note: CG_BOOL_R/F_EDGE returns "BOOL R/F_EDGE", use DT, TOD short names.
extern "C" const TCHAR* CGT_DTYPES_ToString(CG_DTYPES dt);

//Lower case basic type name:
//Note: CG_BOOL_R/F_EDGE returns "bool r/f_edge", use dt, tod short names.
extern "C" const TCHAR* CGT_DTYPES_ToLString(CG_DTYPES dt);

//return a single char short name for each type, see definition of CG_DTYPES for details:  
extern "C" char CGT_DTYPES_ToMangeledChar(CG_DTYPES dt);
//reverse of CGT_DTYPES_ToMangeledChar, but note that R/F edge bool are mapped to 'a' as is bool:
extern "C" CG_DTYPES CGT_MangeledChar_ToDTYPES(char c);

//build mangeled name for CG_DType*:
//syntax is:
// <basic type char> [<string len>|<user type name>|rf] {['['[<range start>],[range length]], ...} ['*']
// 
//VOID                                  =>     0
//BOOL                                  =>     a
//BOOL R_EDGE                           =>     ar
//BOOL F_EDGE                           =>     af
//STRING                                =>     v
//STRING(34)                            =>     v34
//MyStruct                              =>     2MyStruct
//MyFB                                  =>     1MyFB

//ARRAY[0..4]  BOOL                     =>     a[,5
//ARRAY[-1..4] BOOL                     =>     a[-1,5
//ARRAY[-1..4] STRING                   =>     v[-1,5
//ARRAY[-1..4,0..100] STRING(12)        =>     v12[-1,5[,101
//ARRAY[-1..4,1..100] MyStruct          =>     2MyStruct[-1,5[1,100
//TODO:  if pointer type:   '*' follows
#define CGT_MAXMANGELED_TYPE 256
extern "C" const TCHAR* CGT_DTypeToMangeld(const CG_DType* pType, TCHAR buffer[CGT_MAXMANGELED_TYPE]);

//CGT_DTYPES_ToLString2 same as CGT_DTYPES_ToLString but
//CG_BOOL_R/F_EDGE returns "bool"
extern "C" const TCHAR* CGT_DTYPES_ToLString2(CG_DTYPES dt);


//Expr type enum to lowercase lower case string: returns
/* returns:
    CG_NOT="not", CG_UMINUS="-", CG_NOOP="()"
    CG_OR= "or",  CG_XOR="xor",  CG_AND= "and" 
    CG_EQ= "eq",  CG_NE= "ne",   CG_LT=  "lt", 
    CG_LE= "le",  CG_GT= "gt",   CG_GE=  "ge",
    CG_ADD="add", CG_SUB="sub",  CG_MUL= "mul", 
    CG_DIV="div", CG_MOD="mod",  CG_POW= "pow",  
    CG_MOVE=":="
    CG_LITERAL=  "<literal>",    CG_VARREF=    "<variable>"
    CG_STRUCTREF="<struct ref>"  CG_ARRAYREF=  "<array ref>"
    CG_FUNCALL=  "<function>"    CG_FUNMACRO=  "<function>"
    CG_BUILDIN=  "<buildin>",    CG_TMPVAR=    "<tmp>"
    CG_TRIGVAR=  "<trig tmp>"    CG_STEPACTION="<step/action ref>"
    CG_USEREXPR= "<user expr>"   CG_FBCALLEXPR="<FB call>"  //##AC
    CG_LABELREF= "<label ref>"    //##AC
    CG_EXPRTYP_UNDEF: "<undefined>"
*/
extern "C" const TCHAR* CGT_ExprTypToLString(CG_EXPRTYP etyp);


#endif // __CGT_MISC_H_

