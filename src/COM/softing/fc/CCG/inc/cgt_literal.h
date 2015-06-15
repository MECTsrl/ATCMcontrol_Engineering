
#ifndef __CGT_LITERAL_H_
#define __CGT_LITERAL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "cgt_cdiagmsg.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/**
 * CGT_IECStringToBinary
 *  converts a string to its binary representation 
 *
 * @param           pdm				diagnostic message output
 * @param           pEdp			edipos of string to convert
 * @param           pszIECStr       contains string to convert in folowing format
 *                                  character_string          [']{character_representation}*[']
 *                                  character_representation  ([^$'\n]|"$"({hex_digit}{hex_digit}|[$'LlNnPpRrTt]))
 *                                  hex_digit                 [0-9A-Fa-f]
 *                                  or
 *                                  wcharacter_string         [']{wcharacter_representation}*[']
 *                                  wcharacter_representation ([^$'\n]|"$"({hex_digit}{hex_digit}|[$'LlNnPpRrTt]))
 *                                  hex_digit                 [0-9A-Fa-f]
 * @param           pszBinStr       buffer to store the data, if NULL the the string will be only checked
 *                                  and pIChars contains the nr. of chars of binary representation
 * @param           pIChars         integer in/out: lenght of buffer/length of resulted binary representation in chars 
 *                                  including string terminator '\0'
 * @return          true, false if error ocurred (ex. buffer to small; if pszBinStr == NULL and no errors then true) 
 * @exception       -
 * @see             - 
*/
extern "C" bool CGT_IECStringToBinary(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszIECStr, 
	char*            pszBinStr,
    long*            pIChars);

	
#endif // __CGT_LITERAL_H_


