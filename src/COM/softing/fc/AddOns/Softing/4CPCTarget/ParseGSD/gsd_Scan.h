#ifndef __GSD_SCAN_H_
#define __GSD_SCAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   ------------------------------------------------*
#include "Scanner.h"
#include "parsemessage.h"
//----  Forward Class Definitions:   -----------------------------------------*
class CStationCallback;
//----  Defines:    ----------------------------------------------------------*
#define _SHOW_TOKENS        // for debugging only !!

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*

//------------------------------------------------------------------*
/**
 *  class CGsdScan - GSD scanner object - derived from yy_scan.
*/
class CGsdScan : public yy_scan
{
public:
    // ctor
    CGsdScan(LPCTSTR pszGsdFile, LPCTSTR pszLogFile = NULL);
    // dtor
    ~CGsdScan();

private:
    // copy ctor
    CGsdScan(CGsdScan const & other);
    // assignment    
    CGsdScan const & operator = (CGsdScan const & other);

// =Access:
public:
    //------------------------------------------------------------------*
    /**
     * Returns TRUE if scanner has file to scan.
     * @return			BOOL  - TRUE scanner has file to scan, FALSE
     *                  otherwise.
    */
    BOOL IsScannerReady() const
    {
        return (m_hGsdFile != NULL);
    }

    LPCTSTR GetGsdFile()
    {
        return m_sGsdFile;
    }

    void Message(HRESULT, LPCTSTR);
    void SetMessages(CStationCallback *);

#ifdef _SHOW_TOKENS
    void ShowToken(int token);
#endif // _SHOW_TOKENS

// =Overridables:
protected:
    virtual void yyerror(char *fmt, ...);
    virtual int yygetc(void);

// =Helpers
private:
    void WriteToLog(LPCTSTR pszLine);

private:
    CString                 m_sGsdFile;
    FILE*                   m_hGsdFile;
    BOOL                    m_bUseTmpFile;
    CString                 m_sLogFile;
    FILE*                   m_hLogFile;
    long                    m_lErrorCount;
    MessageList             m_messages;
    int                     eof_found;
};

#endif // __GSD_SCAN_H_


