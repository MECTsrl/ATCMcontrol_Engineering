/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/utilsts.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: utilsts.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/utilsts.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           FC_UTIL
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
 *==IEC Scanner functions
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  23.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __UTILSTS_H_
#define __UTILSTS_H_

#ifdef UNICODE
 #error "sorry, the scanner implementaion is not UNICODE aware (lex!)
#endif

#include <tchar.h>

#if	defined	(UTIL_IMP)
#define	UTILIF __declspec(dllexport)
#else
#define	UTILIF __declspec(dllimport)
#endif



#ifdef	__cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////
// Attention: This header file shall be used in a project wide context
// SWKE local definitions shall be kept away.
////////////////////////////////////////////////////////////////////////////


const int UTIL_STS_MAX_TOKEN_LENGTH=255; //maximal token length in chars
struct UTIL_STS_ScannerStruct;          //only a foreward decl, use like a HANDLE


/**
 * UTIL_STS_CreateScanner
 * creates a scanner
 *
 * @return       a newly allocated the scanner struct
 */ 
UTILIF UTIL_STS_ScannerStruct*  UTIL_STS_CreateScanner();

/**
 * UTIL_STS_DeleteScanner
 * deletes a scanner
 *
 * @param   ps             the scanner struct to deletes
 */ 
UTILIF void UTIL_STS_DeleteScanner(
    UTIL_STS_ScannerStruct* ps
);

/**
 * UTIL_STS_SetInputString
 * set the text to scan
 *
 * @param   ps             the scanner struct to work with
 * @param   pszInput       pointer to text, must be valid until 
 *                         UTIL_STS_DeleteScanner is called. pszInput will
 *                         not be changed. pszInput may or may not be
 *                         null terminated.
 * @param   ulLen          ulLen: the number of bytes to scan,
 *                         not including a possible null termination
 */ 
UTILIF void UTIL_STS_SetInputString(
    UTIL_STS_ScannerStruct* ps, 
    const TCHAR*            pszInput, 
    unsigned long           ulLen
);
/**
 * UTIL_STS_SetInputStringEx
 * set the text to scan and seek first to ulBytePos but 
 * update the line, column info during seek.
 *
 * @param   ps             the scanner struct to work with
 * @param   pszInput       pointer to text, must be valid until 
 *                         UTIL_STS_DeleteScanner is called. pszInput will
 *                         not be changed. pszInput may or may not be
 *                         null terminated.
 * @param   ulLen          ulLen: the number of bytes to scan,
 *                         not including a possible null termination
 * @param   ulBytePos      ulBytePos<=ulLen: the number of bytes to skip
 *                         from the beginning of the text.
 */ 
UTILIF void UTIL_STS_SetInputStringEx(
    UTIL_STS_ScannerStruct* ps, 
    const TCHAR*            pszInput, 
    unsigned long           ulLen,
    unsigned long           ulBytePos
);




/**
 * UTIL_STS_NextToken
 * get a token with line and column info
 *
 * @param   ps             the scanner struct to work with
 * @param   ppszToken      output: pointer to token text, 
 *                         valid up to the next call of UTIL_STS_NextToken.
 * @param   pnLine         output: the line number 
 * @param   pnColumn       output: the column
 * 
 * @return                 if <=256 ond >0 it's the ascii code of a 
 *                         simple one char token siutable for yacc.
 *                         Otherwise it's one of the enum values 
 *                         of UTIL_STS_TOK_ENUM
 */ 
UTILIF int  UTIL_STS_NextToken(
    UTIL_STS_ScannerStruct* ps, 
    const TCHAR**           ppszToken, 
    int*                    pnLine,
    int*                    pnColumn
);



/**
 * UTIL_STS_GetFPosOfNextChar same as 
 *   UTIL_STS_GetLengthOfLastToken + UTIL_STS_GetLengthOfLastToken
 *
 * @param   ps             the scanner struct to work with
 * @return                 the byte offset (from the begin of text) of 
 *                         the char right after last token got.
 */ 
UTILIF long UTIL_STS_GetFPosOfNextChar(
    const UTIL_STS_ScannerStruct* ps
);

/**
 * UTIL_STS_GetLineColNextChar returns the (line,col) info 
 * of the input char right after the last token.
 * 
 * @param   ps   the scanner struct to work with
 * @param   pCol receives the next input char's column info
 * @return       >0 the next token's line number
 */ 
UTILIF int UTIL_STS_GetLineColNextChar(
    const UTIL_STS_ScannerStruct* ps,
    int*                          pCol           
);




/**
 * UTIL_STS_GetLengthOfLastToken
 *
 * @param   ps             the scanner struct to work with
 * @return                 the byte offset (from the begin of text) of 
 *                         the last token
 */ 
UTILIF long UTIL_STS_GetOffsetOfLastToken(
    const UTIL_STS_ScannerStruct* ps
);

/**
 * UTIL_STS_GetLengthOfLastToken
 *
 * @param   ps             the scanner struct to work with
 * @return                 the length of the last token in bytes (not chars!)
 */ 
UTILIF long UTIL_STS_GetLengthOfLastToken(
    const UTIL_STS_ScannerStruct* ps
);


/**
 * UTIL_STS_GetLengtOfText simply returns the ulLen value passed
 * to UTIL_STS_SetInputString(Ex)
 *
 * @param   ps             the scanner struct to work with
 * @return                 the byte length of text to parse
 */ 
UTILIF unsigned long UTIL_STS_GetLengthOfText(
    const UTIL_STS_ScannerStruct* ps
);


/**
 * UTIL_STS_GetFPOSFromLineCol
 * gets the byte file pos where to seek to for a token with 
 * (line,col). This function can only be used if the pszFile
 * is the same file used with UTIL_STS_NextToken and the 
 * (line,col) info was also from UTIL_STS_NextToken. 
 * The problem is how the \r is treated in (line,col) info. 
 * UTIL_STS_NextToken does it thisway: every \r adds +1 to
 * a col info, even if not followed by a \n (which is not 
 * treated as syntax error!). The line and col musz both >0
 * even if the file size is 0 bytes.
 *
 * @param   pszFile        full file content
 * @param   nLine          the line number >0 ('>0' is asserted)
 * @param   nColumn        the column      >0 ('>0' is asserted)
 * @param   pulFpos        receives the file pos found is succeeded 
 * @return                 0: if (line,col) points after EOF
 *                         1: position is found
 */ 
UTILIF int UTIL_STS_GetFPOSFromLineCol(
    const TCHAR*   pszFile,
    int            line,
    int            col,
    unsigned long* pulFpos
);


/**
 * UTIL_STS_GetText simply returns the current text to scan
 * this is exactly the pointer passed in UTIL_STS_SetInputString(Ex).
 * UTIL_STS_SetInputString(Ex) must be called first. 
 * 
 * @param   ps   the scanner struct to work with
 * @return       pointer to begin of the current text
 *               or NULL if no text loaded.
 */ 
UTILIF const TCHAR* UTIL_STS_GetText(
    const UTIL_STS_ScannerStruct* ps
);



/**
 * UTIL_STS_GetTextAfterLastToken
 *
 * @param   ps   the scanner struct to work with
 * @param   ppsz 
 *          IN: pointer to buffer of length *pnChars
 *              in TCHARS
 *          OUT:
 *            receives a pointer to the first non white 
 *            and non comment text in the buffer 
 *            There can be multible comments
 *            and even nested ones.
 *
 *          if no token read it's point to the begin 
 *          of the loaded text that is non white space 
 *          If nothing loaded receives NULL.
 * 
 * @param   pnChars: 
 *          IN:  size in TCHARs of buffer
 *          OUT: receives the length in chars 
 *          of where *ppsz points to on exit.
 * 
 * @return  always true except if EOF in comment,
 *          in this case *pnChars,*ppsz is set to 0.
 * 
 */ 
UTILIF int UTIL_STS_BufferSkipComment(
    const TCHAR**                 ppsz,
    long*                         pnChars
);


/**
 * UTIL_STS_GetTextAfterLastToken
 *
 * @param   ps        the scanner struct to work with
 * @param   pnChars:  receives the length in chars 
 *                    of where the returned pointer points to.
 * @return            returns a pointer to the text loaded 
 *                    or NULL if nothing loaded or EOF
 */ 
UTILIF const TCHAR* UTIL_STS_GetTextAfterLastToken(
    const UTIL_STS_ScannerStruct* ps,
    long*                         pnChars
);



/**
 * UTIL_STS_SkipLine
 * skips the current line but not the '\n'
 *
 * @param   ps             the scanner struct to work with
*/ 
UTILIF void UTIL_STS_SkipLine(
    UTIL_STS_ScannerStruct* ps
);


/**
 * UTIL_STS_SkipComment
 * skips an IEC comment, recognizes nested comments
 *
 * @param   ps             the scanner struct to work with
 * @param   pszEnd         the terminating string, normally "*)"
 * @return                 0 if string is unterminated (EOF in string)
 *                         1 if no error
 *                         2 if no error, but comment is nested
 */ 
UTILIF int UTIL_STS_SkipComment(
    UTIL_STS_ScannerStruct* ps, 
    const TCHAR*            pszEnd
);


/**
 * UTIL_STS_GetTokenUpToEOL
 * skips all text up to end of line and stores the skipped text in 
 * the skipped text block buffer. You get the address via UTIL_STS_GetSkippedTextBlock.
 * Leading and trailing whitespace is stripped of.
 * @param   ps             the scanner struct to work with
 */ 
UTILIF void UTIL_STS_GetTokenUpToEOL(
    UTIL_STS_ScannerStruct* ps
);


/**
 * UTIL_STS_SkipIECStringLiteral
 * skips an IEC string and stores the skipped text in 
 * the skipped text block buffer. You get the address via UTIL_STS_GetSkippedTextBlock.
 * The first character in the text block is the delimitter.
 * The scan stops on 'delim', any character after a '$' is skipped.
 * This function does no further check even the parenthesis may be unbalanced
 * @param   ps             the scanner struct to work with
 * @param   delim          either " or '. 
 * @param   bClearBuffer   if true the skipped text block is cleared first,
 *                         otherwise the string is appended to the buffer.
 * @return                 1 if no error, 0 if string is unterminated (EOF in string)
 */ 
UTILIF int UTIL_STS_SkipIECStringLiteral(
    UTIL_STS_ScannerStruct* ps, 
    TCHAR                   delim, 
    bool                    bClearBuffer
);


/**
 * UTIL_STS_SkipBlock
 * skips a block of text and stores the skipped text block 
 * buffer if bStore is true. Comments or strings are not recognized.
 * You get the address via UTIL_STS_GetSkippedTextBlock.
 * Skip stops at pszEnd. pszEnd may be treated ignorecase
 * if bIgnoreCase is true.
 * 
 * @param   ps             the scanner struct to work with
 * @param   bStore         if false to block is not stored  
 * @param   pszEnd         the end of block marker
 * @param   bIgnoreCase    if true pszEnd is treated ignorecase
 * 
 * @return                 1 if no error, 
 *                         0 if block is unterminated (EOF in block)
 */ 
UTILIF int UTIL_STS_SkipBlock(
    UTIL_STS_ScannerStruct* ps, 
    bool                    bStore, 
    const TCHAR*            pszEnd, 
    bool                    bIgnoreCase
);


/**
 * UTIL_STS_SkipMatchingBrace
 * skips a all characters up to and including the closing brace,
 * but the final closing brace will not part of the stored text.
 * Nested (..),[..] or {..} <..> pairs are recognized and also skipped.
 * While skipping the text, braces different from the one brace to search
 * for are treated as any other chars.
 * 
 * @param   ps             the scanner struct to work with
 * @param   bStore         if false to block is not stored  
 * @param   cBrace         one of (,[,{ or < (the token already got).
 * @return                 1 if no error, 
 *                         0 if block is unterminated (EOF in block)
 *                        -1 if cBrace was not one of (,[,{ or <  
 *                         must close to enable C++ editor ..}}>>
 */ 
UTILIF int UTIL_STS_SkipMatchingBrace(
    UTIL_STS_ScannerStruct* ps, 
    bool                    bStore, 
    TCHAR                   cBrace
);

/**
 * UTIL_STS_GetSkippedTextBlock
 * returns a pointer to a (null terminated) skipped text block, e.g. 
 * from a previous call to UTIL_STS_SkipBlock or UTIL_STS_SkipIECStringLiteral.
 * The pointer is valid until the next call to a function that 
 * stores text in the buffer.
 * @param   ps             the scanner struct to work with
 * 
 * @return                 pointer to buffer (must not be deleted from caller!) 
 *                         NULL if out of memory
 */ 
UTILIF const TCHAR* UTIL_STS_GetSkippedTextBlock(
    UTIL_STS_ScannerStruct* ps
);


//tokens:
typedef enum 
{
    UTIL_STS_TOKEN_TO_LONG = -2,
    UTIL_STS_SYNTAX_ERROR = -1,
    UTIL_STS_EOF = 0,

    /* to be used in yacc tokens must have valued >256! */
    UTIL_STS_IDENTIFIER_EX  = 257, 
    UTIL_STS_DIRECT_VARIABLE,
    
    UTIL_STS_POW,
    UTIL_STS_AND,
    UTIL_STS_EQ,
    UTIL_STS_NE,
    UTIL_STS_LT,
    UTIL_STS_LE,
    UTIL_STS_GT,
    UTIL_STS_GE,
    UTIL_STS_BECOMES,
    UTIL_STS_RIGHT_ARROW,
    UTIL_STS_DOTDOT,

    UTIL_STS_INT_LITERAL,
    UTIL_STS_FLOAT_LITERAL,
    UTIL_STS_DATE_LITERAL,
    UTIL_STS_DT_LITERAL,
    UTIL_STS_TOD_LITERAL,
    UTIL_STS_TIME_LITERAL,
    UTIL_STS_TRUE_LITERAL,
    UTIL_STS_FALSE_LITERAL,
    UTIL_STS_STRING_LITERAL_BEGIN,

    UTIL_STS_ST_COMMENT_BEGIN,

    // ATCMControl extensions:
    UTIL_STS_IMPORT_DIRECTIVE,
    UTIL_STS_PRAGMA_DIRECTIVE,
    UTIL_STS_SYS_COMMENT_BEGIN,
    UTIL_STS_SYS_COMMENT_END,
    UTIL_STS_PERIOD,
} UTIL_STS_TOK_ENUM;


#ifdef	__cplusplus
}
#endif	//__cplusplus



#endif // __UTILSTS_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: utilsts.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 9.03.01    Time: 20:43
 * Updated in $/4Control/COM/softing/fc/inc
 * some new functions
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 22.02.01   Time: 21:04
 * Updated in $/4Control/COM/softing/fc/inc
 * inserted first ..Ex function
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 15.02.01   Time: 14:51
 * Updated in $/4Control/COM/softing/fc/inc
 * new functions : get text after last token and skip comments
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:09
 * Created in $/4Control/COM/softing/fc/inc
 * utils and baselib from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
