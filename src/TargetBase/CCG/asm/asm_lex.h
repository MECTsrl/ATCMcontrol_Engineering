/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_lex.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asm_lex.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm_lex.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _ASM_LEX_H_
#define _ASM_LEX_H_

#include <tchar.h>


#ifdef	__cplusplus
extern "C" {
#endif


#define ASM_MAX_TOKEN_LENGTH 256



struct ASM_ScannerStruct;/* only foreward declared, used as a HANDLE */

/**
 * ASM_CreateScanner
 * creates a scanner
 *
 * @return       a newly allocated the scanner struct
 */ 
ASM_ScannerStruct* ASM_CreateScanner();

/**
 * ASM_DeleteScanner
 * deletes a scanner
 *
 * @param   ps             the scanner struct to deletes
 */ 
void ASM_DeleteScanner(ASM_ScannerStruct* ps);



/**
 * ASM_SetInputString
 * set the text to scan
 *
 * @param   ps             the scanner struct to work with
 * @param   pszInput       pointer to text, must be valid until 
 *                         ASM_DeleteScanner is called. pszInput will
 *                         not be changed. pszInput may or may not be
 *                         null terminated.
 * @param   ulLen          ulLen: the number of bytes to scan,
 *                         not including a possible null termination
 */ 
void ASM_SetInputString(ASM_ScannerStruct*  ps, 
                        const TCHAR*        pszInput, 
                        unsigned long       ulLen);


/**
 * ASM_NextToken
 * get a token with line and column info
 *
 * @param   ps             the scanner struct to work with
 * @param   ppszToken      output: pointer to token text, 
 *                         valid up to the next call of ASM_NextToken.
 * @param   pnLine         output: the line number 
 * @param   pnColumn       output: the column
 * 
 * @return                 if <=256 ond >0 it's the ascii code of a 
 *                         simple one char token siutable for yacc.
 *                         Otherwise it's one of the enum values 
 *                         of ASM_TOK_ENUM
 */ 
int ASM_NextToken(ASM_ScannerStruct* ps, 
                  const TCHAR**      pszToken, 
                  int*               pnLine,
                  int*               pnColumn);


/**
 * ASM_SkipIECStringLiteral
 * skips an IEC string and stores the skipped text in 
 * the skipped text block buffer. You get the address via ASM_GetSkippedTextBlock.
 * The first character in the text block is the delimitter.
 * The scan stops on 'delim', any character after a '$' is skipped.
 * This function does no further check even the parenthesis may be unbalanced
 * @param   ps             the scanner struct to work with
 * @param   delim          either " or '. 
 * @param   bClearBuffer   if true the skipped text block is cleared first,
 *                         otherwise the string is appended to the buffer.
 * @return                 1 if no error, 0 if string is unterminated (EOF in string)
 */ 
int ASM_SkipIECStringLiteral(ASM_ScannerStruct* ps, 
                             TCHAR              delim, 
                             bool               bClearBuffer);

/**
 * ASM_SkipLine skips the current line not including the '\n', 
 * the \n is "unget".
 *
 * @param   ps             the scanner struct to work with
 * @param   bStore         store the line (not including trailing \r\n) 
 *                         skipped text block buffer
*/ 
void ASM_SkipLine(ASM_ScannerStruct* ps, bool bStore);

bool ASM_SkipBlock(ASM_ScannerStruct* ps, bool bStore, 
                   const TCHAR* pszEnd, bool bIgnoreCase);


/**
 * ASM_GetSkippedTextBlock
 * returns a pointer to a (null terminated) skipped text block, e.g. 
 * from a previous call to ASM_SkipBlock or ASM_SkipIECStringLiteral.
 * The pointer is valid until the next call to a function that 
 * stores text in the buffer.
 * @param   ps             the scanner struct to work with
 * 
 * @return                 pointer to buffer (must not be deleted from caller!) 
 *                         NULL if out of memory
 */ 
const TCHAR* ASM_GetSkippedTextBlock(ASM_ScannerStruct* ps);



typedef enum 
{
    ASM_TOK_TOKEN_TO_LONG = -2,
    ASM_TOK_SYNTAX_ERROR = -1,
    ASM_TOK_EOF = 0,

    //key words
    ASM_TOK_TARGET=257,
    ASM_TOK_FUN,
    ASM_TOK_FB,
    ASM_TOK_PRG,
    ASM_TOK_STRUCT,
    ASM_TOK_GLOBAL,
    ASM_TOK_GLOBAL_RETAIN,
    ASM_TOK_CONFIG,
    ASM_TOK_COPY,
    ASM_TOK_CHANNELS,
    ASM_TOK_CHANNEL,
    ASM_TOK_END_SECTION,
    ASM_TOK_VAR,
    ASM_TOK_IN,
    ASM_TOK_INEXT,
    ASM_TOK_OUT,
    ASM_TOK_INOUT,
    ASM_TOK_ALIGN8,
    ASM_TOK_RESOURCE,
    ASM_TOK_CLSSID,
    ASM_TOK_TASK,

    
    ASM_TOK_BP,
    ASM_TOK_STMTBEG,
    ASM_TOK_ST_INFO,
    ASM_TOK_CALOP,

    ASM_TOK_JMP,
    ASM_TOK_JMPC,
    ASM_TOK_JMPCN,
    
	ASM_TOK_CALL,
	ASM_TOK_READ,
	ASM_TOK_WRITE,

	//tokens
    ASM_TOK_BECOMES,
    ASM_TOK_STRING_LITERAL_BEGIN,
    ASM_TOK_LITERAL,
    ASM_TOK_LITERAL_OFFSET,
    ASM_TOK_LITERAL_OBJID,
    ASM_TOK_ANY_UNSIGNED,
    ASM_TOK_DIRECT_VARIABLE,
    ASM_TOK_IDENTIFIER_EX,

} ASM_TOK_ENUM;


#ifdef	__cplusplus
}
#endif

#endif

/* ---------------------------------------------------------------------------- */
