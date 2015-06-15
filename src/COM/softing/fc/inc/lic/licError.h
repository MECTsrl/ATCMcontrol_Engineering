/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/lic/licError.h 2     12.02.08 19:39 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: licError.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/lic/licError.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                LIC
 *
 * =COMPONENT           liccore
 *
 * =CURRENT      $Date: 12.02.08 19:39 $
 *               $Revision: 2 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 


 

 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==Definition of return codes
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  21.03.02  KK      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __LICERROR_H_
#define __LICERROR_H_


//----  Defines:    ------------------------------------------------*



//----  Prototypes:    ---------------------------------------------*

#ifdef __cplusplus
extern "C" {
#endif



//return values of API functions:
#define SLIC_OK                  0    

//more internal errors:
#define SLIC_ERR_INTERAL         1
#define SLIC_ERR_ILLPARAM        2
#define SLIC_ERR_BUFFER_TO_SMALL 3
#define SLIC_ERR_BAD_TMPTIME     4
#define SLIC_ERR_BAD_PRODNUM     5

//registry read/write erros:
#define SLIC_ERR_REG_READ        8
#define SLIC_ERR_REG_WRITE       9
//registry seems to be manipulated:
#define SLIC_ERR_REG_CORRUPTED   10

//inst/reg keys are invalid:
#define SLIC_ERR_RKEY_FORMAT     13
#define SLIC_ERR_RKEY_INVALID    14
#define SLIC_ERR_IKEY_FORMAT     15

//dongle create errors:
#define SLIC_ERR_DNG_TOOMANYPROD 16  //too many products
#define SLIC_ERR_DNG_WRITE       17  //generall dongle write error

//attempt to reuse a non resuable temp. key
#define SLIC_ERR_RKEY_REUSETMP   18

//"normal" errors:
#define SLIC_ERR_NO_COMP_FOUND   20
#define SLIC_ERR_NO_LIC_FOUND    21
#define SLIC_ERR_TMPLIC_EXPIRED  22

//product enabling errors
#define SLIC_ERR_ENABLE_PROD    25

// file errors
#define SLIC_ERR_FILE_OPEN      30
#define SLIC_ERR_FILE_READ      31
#define SLIC_ERR_FILE_WRITE     32

// library errors
#define SLIC_ERR_LOAD_LIBRARY   40
#define SLIC_ERR_PROC_NOT_FOUND 41

// encoding errors
#define SLIC_ERR_UNKNOWN_ENCODING   45
#define SLIC_ERR_PENDING            46



// error texts
#define SLIC_ERR_TXT_BUF_LEN        100
#define SLIC_ERR_ILLPARAM_TXT       "Wrong parameters"

#define SLIC_ERR_ENABLE_PROD_TXT    "Failed to enable product"

// file error texts
#define SLIC_ERR_FILE_OPEN_TXT      "Failed to open file"
#define SLIC_ERR_FILE_READ_TXT      "Failed to read file"
#define SLIC_ERR_FILE_WRITE_TXT     "Failed to write file"

// library error texts
#define SLIC_ERR_LOAD_LIBRARY_TXT   "Cannot load library"
#define SLIC_ERR_PROC_NOT_FOUND_TXT "Function not found! Old version of library?"

#define SLIC_ERR_UNKNOWN_ENCODING_TXT "Unknown encoding version"




#ifdef __cplusplus
}
#endif


#endif // __LICERROR_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: licError.h $
 * 
 * *****************  Version 2  *****************
 * User: Ln           Date: 12.02.08   Time: 19:39
 * Updated in $/4CReleased/V2.20.00/COM/softing/fc/inc/lic
 * 
 *==
 *----------------------------------------------------------------------------*
*/
