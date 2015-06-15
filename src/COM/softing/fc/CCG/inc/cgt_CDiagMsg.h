
#ifndef __CGT_CDIAGMSG_H_
#define __CGT_CDIAGMSG_H_


//----  Aggregate Includes:   --------------------------------------*
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cstring.h"
#include "CG_IECFront.h"
#include "CG_Messages.h"

//----  Defines:    ------------------------------------------------*
//internal error macros:
#define CGT_IERR(pdm, reason) (pdm)->intErr(__FILE__, __LINE__, reason)
#define CGT_IERR2(pdm, pEdp, reason) (pdm)->intErr(pEdp, __FILE__, __LINE__, reason)



//----  Prototypes:    ---------------------------------------------*
//functions for HRESULTS
//errorCode is coded as defined by Message Compiler
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//   Sev:  00=Success, 01=Informational, 10=Warning, 11=Error
//   C=Customer code flag,   R=reserved bit
inline long CGT_GetFacilityCode(HRESULT l)         {return 0x0fff & (l>>16);}
inline long CGT_GetSeverity(HRESULT l)             {return 0x3 & (l>>30);}
inline long CGT_GetErrorCode(HRESULT l)            {return 0xffff & l;}
inline bool CGT_GetCustomerBit(HRESULT l)          {return (0x20000000 & l) != 0;}
inline long CGT_SetCustomerBit(HRESULT l, bool bit){return bit?l|0x20000000:l&~0x20000000;}
inline bool CGT_IsInfoSeverity(HRESULT l)          {return (0x3 & (l>>30)) == 1;}
inline bool CGT_IsWarningSeverity(HRESULT l)       {return (0x3 & (l>>30)) == 2;}
inline bool CGT_IsErrorSeverity(HRESULT l)         {return (0x3 & (l>>30)) == 3;}




class CGT_CDiagMsg 
{
    ICG_IECFront* m_pIFront;
    long          m_displayedErrCount;
    long          m_silentErrCount;
    long          m_displayedWarnCount;

    long          m_maxErrors;
    bool          m_bToManyErrors;
    int           m_iDisableWarnings;

    //used as edipos if pEdp is NULL:
    FC_CStr       m_moduleName;
    //if not empty use as the tool name that does not support a feature,
    //see this->errNS:
    FC_CStr       m_NSName;

    //where to get string format resources:
    HMODULE       m_HmoduleRes;
    FC_CString    m_jot1;
    FC_CString    m_jot2;
    FC_CString    m_jot3;
    FC_CString    m_res1;
    FC_CString    m_res2;
    FC_CString    m_res3;
public:
    CGT_CDiagMsg(
        const TCHAR*  pszModule,
        HMODULE       hModule
    );
    //to set it later:
    void setFront(ICG_IECFront* pIFront);

    
    void setModuleName(const TCHAR* pszModule);
    void setNotSupportedName(const TCHAR* pszTrgType);
    void setResHMODULE(HMODULE hModule){m_HmoduleRes = hModule;}
     
    const TCHAR* getModuleName()const{return m_moduleName;}
    const TCHAR* getNotSupportedName()const{return m_NSName;}
    HMODULE      getResHMODULE()const{return m_HmoduleRes;}
 

    bool hasErr()const  {return getTotalErrors() != 0;}
    bool hasNoErr()const{return getTotalErrors() == 0;}
    long getTotalErrors()const;
    long getDisplayedErrors()const;
    long getDisplayedWarnings()const;
    bool toManyErrors()const{return m_bToManyErrors;}

    //------------------------------------------------------------------*
    /**
     * userMsg: All compiler user output (diagnostic messages) are done
     * with this function. userMsg does the following:  
     *  if(CharToOem(pszText, pszOem))
     *   _fputts(pszOem, stdout);
     *  else
     *   _fputts(pszText, stdout);
     *  fflush(stdout);
     *
     * @param           pszText: the text to dump as console output    
    */
    void userMsg(const TCHAR* psz);

    void addIndirectErr();
    void incErrors(long nErr);
    void incWarnings(long nWarn);

    //disable/enable warnings must be called nesed but 
    //in pairs:
    int isDisableWarnings()const{return m_iDisableWarnings;}
    void disableWarnings();
    void enableWarnings();
    void setMaxErrors(int nMax);


    //diag msg for 0..4 arguments:
    void msg0(HRESULT id, const CG_Edipos* pEdp);
    void msg1(HRESULT id, const CG_Edipos* pEdp, const TCHAR* psz);
    void msg2(HRESULT id, const CG_Edipos* pEdp, const TCHAR* psz, const TCHAR* psz2);
    void msg3(HRESULT id, const CG_Edipos* pEdp, 
        const TCHAR* psz, const TCHAR* psz2,  const TCHAR* psz3);
    void msg4(HRESULT id, const CG_Edipos* pEdp, 
        const TCHAR* psz, const TCHAR* psz2,  const TCHAR* psz3, const TCHAR* psz4);
#ifndef UNICODE 
    void msg1(HRESULT id, const CG_Edipos* pEdp, 
        const wchar_t* psz);
    void msg2(HRESULT id, const CG_Edipos* pEdp, 
        const wchar_t* psz, const wchar_t* psz2);
    void msg3(HRESULT id, const CG_Edipos* pEdp, 
        const wchar_t* psz, const wchar_t* psz2, const wchar_t* psz3);
    void msg4(HRESULT id, const CG_Edipos* pEdp, 
        const wchar_t* psz, const wchar_t* psz2, const wchar_t* psz3, const wchar_t* psz4);
#endif
    //not supported error message:
    void errNS1(
        const CG_Edipos* pEdp, 
        const TCHAR*     psz1
    );
    void errNS2(
        const CG_Edipos* pEdp, 
        const TCHAR*     psz1,
        const TCHAR*     psz2
    );
    void errNS3(
        const CG_Edipos* pEdp, 
        const TCHAR*     psz1,
        const TCHAR*     psz2,
        const TCHAR*     psz3
    );

    void sysErr(
        HRESULT          hr, 
        const CG_Edipos* pEdp, 
        const TCHAR*     pszHint,
        const TCHAR*     pszHint2
    );
    void intErr(
        const CG_Edipos* pEdp,
        const TCHAR*     pszCompilerFile, 
        int              nLine, 
        const TCHAR*     pszReason
    );
    void intErr(
        const TCHAR* pszCompilerFile, 
        int          nLine, 
        const TCHAR* pszReason
    );

    void msg( 
        HRESULT           id,   //local or predefined diag message (see CGMessages.mc)
        const CG_Edipos*  pEdp, //error location (used to find POU's symtab)
        const TCHAR*      ppsz[]//NULL or the textual inserts for all %[0-9]
    );


    //------------------------------------------------------------------*
    /**
     * edpToStrBuffer: convert a edipos struct to a string for user display.
     * Example of produced string representation: 
     *    "myProgram.st(23,4)"
     *    "myProgram.4gr(b,56,40)"
     *    "c:\Program Files\4Control\engneering\lib\4C_IEC1131_3.4cl::TON\ton.hdr(2,1)"
     *
     *If bNoFile==true:
     *    "(2,1)"
     *
     * @param    pEdp:    the edipos to convert to a string representation
     * @param    szedp:   buffer for result
     * @param    nBuf:    size of buffer in TCHARs, 2*MAX_PATH is always enough
     * @param    bNoFile: can be true to ommit the file locator. 
     *                    Note: if pEdp is NULL or a command line edp or
     *                    just contains no (line,col) info and bNoFile is true 
     *                    the returned string will be empty. 
     *                    Call with bNoFile==true only for special situations
     *                    like dumping DBI and .link files, not for diagnostics.
     * 
     * @return   false if nBuf to small
     * @see      comments on CG_Edipos definition in CG_IECFront.h
     *           CGT_CDiagMsg::edpToStr, CGT_CDiagMsg::edpToStrNoFile
    */
    const bool edpToStrBuffer(
        const CG_Edipos* pEdp, 
        TCHAR            szedp[/*nBuf*/], 
        int              nBuf,
        bool             bNoFile
    );

    //------------------------------------------------------------------*
    /**
     * edpToStr: convert a edipos struct to a string for user display
     * @param     pEdp:  the edipos to convert
     * @param     szedp: output: a string obj to be loaded with edp string
     * 
     * @return    the input szedp.psz() const TCHAR*
     * @see       edpToStrBuffer
    */
    const TCHAR* edpToStr(
        const CG_Edipos* pEdp, 
        FC_CString&      szedp
    );
    //------------------------------------------------------------------*
    /**
     * edpToStrNoFile: convert a edipos struct to a string without the 
     * the file locator, can be used typically for DBI files and link files.
     * Example of produced edp: "(23,4)"
     *
     * @param     pEdp:  the edipos to convert
     * @param     szedp: output: a string obj to be loaded with edp string.
     * 
     * @return    the input szedp.psz() const TCHAR*
     * @see       edpToStrBuffer
    */
    const TCHAR* edpToStrNoFile(
        const CG_Edipos* pEdp, 
        FC_CString&      szedp 
    );


    //can be used to form text inserts, be carefull: don't
    //store text for later use. jot1..3() should always be used 
    //like this:
    //pdm->msg2(CG_E_LIMIT, &edp, loadRes(RES_ARRAY_LEN), pdm->jot1()<<MAX_ARRAY_LEN) 
    FC_CString& jot1(){return m_jot1.clear();}
    FC_CString& jot2(){return m_jot2.clear();}
    FC_CString& jot3(){return m_jot3.clear();}
    FC_CString& res1(unsigned int uiResNum){ return m_res1.loadRes(uiResNum, m_HmoduleRes); }
    FC_CString& res2(unsigned int uiResNum){ return m_res2.loadRes(uiResNum, m_HmoduleRes); }
    FC_CString& res3(unsigned int uiResNum){ return m_res3.loadRes(uiResNum, m_HmoduleRes); }   
};


#endif // __CGT_CDIAGMSG_H_

