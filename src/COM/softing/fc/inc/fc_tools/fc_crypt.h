/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_crypt.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $RCSFile$
 *
 * =PROJECT             CAK1020
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
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  File created: 08.11.2004
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FC_CRYPT_H_
#define __FC_CRYPT_H_



#include "fc_defines.h" 

#ifdef __cplusplus
extern "C"{
#endif


//-------------- DES (= Data Encryption Standard) Functions ---------
//                  DES Option:   the encoded string is built like this:
#define FC_DES_NORMAL  0x00001    //4byte original size|8 byte magic value|data

#define FC_DES_FAST    0x10000    //fast(er) encryption, less safe(?)
                                  //timing without FC_DES_FAST on a 3 Ghz Pentium 4:
                                  //approx 0.5 Seconds/MB, with FC_DES_FAST 0.3 Seconds/MB
                                  

/**
 * FC_DESEncodeBlock encodes a block of memory 
 * @param           nMode    : a set of FC_DES_* option bits that control how the 
 *                             encoded string looks like, see above.
 * @param           pszPw    : the password to use (only the first 8 chars are used).
 * @param           pData    : pointer to memory block to be encoded.
 * @param           nData    : size of memory block to be encoded.
 * @param           pOut     : pOut is a pointer to an existing 
 *                             memory block to be filled with 
 *                             encoded data, the sizeof this memory 
 *                             block must be at least nData + 64. 
 * @param           pnOut    : receives the sizeof the encoded data, this is 
 *                             the number of bytes in pOut that are actually used.
 * @return          void 
*/
FC_TOAPI void FC_TODECL FC_DESEncodeBlock(
    long            nMode,
    const char*     pszPw, 
    const char*     pData, 
    unsigned long   nData,
    char*           pOut,
    unsigned long*  pnOut
);


/**
 * FC_DESGetDecodedSize get the decoded size of of an encoded block
 * @param           nMode    : a set of FC_DES_* option bits that control how the 
 *                             encoded string looks like, see above. This must be 
 *                             the same as used when encode the data.
 * @param           pszPw    : the password to use (only the first 8 chars are used).
 * @param           pData    : pointer to memory block from where to get the decoded size.
 * @param           nData    : size of memory block from where to get the decoded size.
 * @param           pSize    : receives the decoded size in bytes.
 *                             
 * @return          0        : no error
 *                  1        : invalid password
 *                  3        : pData or nData are invalid
*/
FC_TOAPI int FC_TODECL FC_DESGetDecodedSize(
    long            nMode,
    const char*     pszPw, 
    const char*     pData, 
    unsigned long   nData,
    unsigned long*  pSize
);


/**
 * FC_DESCheckDecodedSize performs a consistency of the encoded and decoded sizes
 *                        
 * @param           nMode    : a set of FC_DES_* option bits that control how the 
 *                             encoded string looks like, see above. This must be 
 *                             the same as used when encode the data.
 * @param   ulDecodedSize    : the size of the block after it is decoded.
 * @param   ulEncodedSize    : size of the encoded block.
 *                             
 * @return          1        : OK
 *                  0        : invalid sizes
*/
FC_TOAPI int FC_TODECL FC_DESCheckDecodedSize(
    long            nMode,
    unsigned long   ulDecodedSize, 
    unsigned long   ulEncodedSize
);


/**
 * FC_DESDecodeBlock decodes a block of memory 
 * @param           nMode    : a set of FC_DES_* option bits that control how the 
 *                             encoded string looks like, see above. This must be 
 *                             the same as used when encode the data.
 * @param           pszPw    : the password to use (only the first 8 chars are used).
 * @param           pData    : pointer to memory block to be decoded.
 * @param           nData    : size of memory block to be decoded.
 * @param           pOut     : pOut is a pointer to an existing 
 *                             memory block to be filled with 
 *                             decoded data, the sizeof this memory 
 *                             block must be at least as returned 
 *                             from FC_DESGetDecodedSize. 
 *                             Only the first FC_DESGetDecodedSize number of 
 *                             bytes are used.
 * @param           nOut     : the sizeof the encoded data memory block pOut, 
 *                             at least as returned from FC_DESGetDecodedSize. 
 *                             
 * @return          0        : no error
 *                  1        : invalid password
 *                  2        : nOut is too small
 *                  3        : pData or nData are invalid
*/
FC_TOAPI int FC_TODECL FC_DESDecodeBlock(
    long            nMode,
    const char*     pszPw, 
    const char*     pData, 
    unsigned long   nData,
    char*           pOut,
    unsigned long   nOut
);

#ifdef __cplusplus
}
#endif


#endif // __FC_CRYPT_H_


/*
 *----------------------------------------------------------------------------*
 *  $Log: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_crypt.h $
 * 
 * 1     28.02.07 19:01 Ln
 * 
 * 2     8.02.05 13:53 Ef
 * fixed d-169
 * 
 * 1     12.11.04 10:49 Ef
 * added DES crypto helper functions
 *==
 *----------------------------------------------------------------------------*
*/
