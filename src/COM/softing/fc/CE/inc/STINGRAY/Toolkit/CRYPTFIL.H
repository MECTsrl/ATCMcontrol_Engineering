// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:		    PCB
//  Description:	Declarations for Crypto-File (SECCryptoFile)
//  Created:		Apr 4, 1995
//
//  CHANGELOG:
//     PCB          12/1/95    SEC++ Accomodations
//
//


#ifndef __CRYPTOFIL_H__
#define __CRYPTOFIL_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL


#ifndef __BLACKBOX_H__
#include "toolkit\blackbox.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// SECCryptoFile

/////////////////////////////
// AutoDuck tag block block for SECCryptoFile 


//@doc SECCryptoFile
//@mdata SECCryptoBlackBox  | SECCryptoFile | m_bb | En/decryption engine object.
//@xref <c SECCryptoFile>

// adinsert AutoDuck insertion point for SECCryptoFile 
//@doc SECCryptoFile 
//@class SECCryptoFile is derived from CFile, and provides encryption and 
// decryption services for data written to and from the file.
//
//@comm Two modes are provided, Electronic Codebook (ECB) and Output Feedback (OFB).
//
// ECB is moderately secure, and allows random access reads and writes.  
// However, application of sophisticated cryptographic methods allows much 
// easier recovery of the contents of each of two or more files encrypted 
// with the same password.  Encourage your users to change their passwords 
// frequently, or implement another layer of key management.
// 
// OFB is much more secure, but file access is unidirectional and sequential 
// only.  Therefore, an SECCryptoFile opened in OFB may be CFile::modeRead 
// or CFile::modeWrite, but not both.  The seek functions are also not 
// available.
//
// See the TEST32 sample in the \SAMPLES\TOOLKIT\STANDARD\UTILITY\CRYPT directory for a 
// demonstration of this class.
//@base public | CFile
class SECCryptoFile : public CFile 
{
public:
	
	//@cmember,menum
	/* Encryption directions*/
	enum eDir { 
		ENC,	//@@emem Encode
		DEC		//@@emem Decode
	} ;

	//@cmember,menum
	/* Encryption methods*/
	enum eMode { 
		ECB,	//@@emem Electronic Codebook encoding
		OFB		//@@emem Output Feedback encoding
	} ;

	//@access Creation/Initialization

	//@cmember
	/* Constructs a SECCryptoFile object.*/
	SECCryptoFile( const TCHAR 	*pszFileName, 
					const TCHAR 	*pszPassword, 
					UINT 		nOpenFlags,
					eMode		CipherMode );
	
	//@cmember
	/* Constructs a SECCryptoFile object.*/
	SECCryptoFile( int hFile, const TCHAR *pszPassword, eMode CipherMode );
	
	//@cmember
	/* Constructs a SECCryptoFile object.*/
	SECCryptoFile();
	
	//@access Operations

	//@cmember
	/* Opens an SECCryptoFile with a path, password, direction and */
	//en/decryption mode.
	BOOL Open( 	const TCHAR *pszFileName,
				const TCHAR *pszPassword, 
				UINT nOpenFlags,
	 			eMode CipherMode,
				CFileException *p = NULL );
	
	//@cmember
	/* Reads a number of bytes from the file and decrypts them. */
	UINT Read( void FAR *pBuf, UINT nCount );

	//@cmember
	/* Reads a number of bytes from the file and decrypts them.  */
	UINT ReadHuge( void FAR *pBuf, DWORD nCount );

	//@cmember
	/* Encrypts and then writes a number of characters to the file.*/
	void Write( void FAR *pBuf, UINT nCount );

	//@cmember
	/* Encrypts and then writes a number of characters to the file.  */
	void WriteHuge( void FAR *pBuf, DWORD nCount );
	
	//@cmember
	/* Moves to a specified location in the file.  */
	long Seek( long lOff, UINT nFrom );

	//@cmember
	/* Moves to the beginning of the file.*/
	void SeekToBegin( void );

	//@cmember
	/* Moves to the end of the file.  Only usable in ECB mode.*/
	void SeekToEnd( void );

protected:

	//@access Protected data members

	//@cmember
	/* En/decryption engine object.*/
	SECCryptoBlackBox m_bb;
};	


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif // __CRYPTOFIL_H__

