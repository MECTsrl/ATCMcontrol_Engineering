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
//  Author:		    AAB
//  Description:	Declarations for Compress-File (SECCompress)
//  Created:		Jan 15, 1996
//
//  CHANGELOG:
//     AAB	1/15/96		Original creation based on CCryptoFile architecture
//
//

#ifndef __SECCOMPRESS_FILE_H__
#define __SECCOMPRESS_FILE_H__

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


#ifndef __SECCOMP_H__
#include "toolkit\compeng.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// SECCompressFile

/////////////////////////////
// AutoDuck tag block block for SECCompressFile 
//@doc SECCompressFile
//@mdata SECComp  | SECCompressFile | m_Comp | Compression engine object.
//@xref <c SECCompressFile>

//@doc SECCompressFile
//@mdata gzFile  | SECCompressFile | m_gzFile | 'zlib' compression object.
//@xref <c SECCompressFile>

//@doc SECCompressFile
//@mdata BOOL  | SECCompressFile | m_bReadHuge | Flag when reading huge 
// vals (WIN3.1 only)
//@xref <c SECCompressFile>

//@doc SECCompressFile
//@mdata BOOL  | SECCompressFile | m_bCompressMode | Indicates whether 
// compression is enabled or disabled.
//@xref <c SECCompressFile> <mf SECComp::SetCompressMode>

// adinsert AutoDuck insertion point for SECCompressFile 
//@doc SECCompressFile 
//@class SECCompressFile is derived from CFile, and provides compression 
// and decompression services for data written to and from the file.
//
//@comm An instance of SECCompressFile can be in compression mode, where a 
// limited number of CFile access functions are available; and regular 
// mode, where normal CFile behavior is exhibited.
//
// Since SECCompressFile compresses its data as a whole, it is not 
// possible to seek to the middle of a compressed data block to partially 
// decompress a block.
//
// However, by switching back and forth between compressed mode and normal 
// CFile mode, compressed data can be written to a file in different 
// locations, and a jump table might be placed at the beginning of the 
// file to store seek locations of the different compressed blocks.
//
// See the COMPDEMO sample in the \SAMPLES\TOOLKIT\STANDARD\UTILITY\CFILES directory for a 
// demonstration of this class.
//@base public | CFile
class SECCompressFile : public CFile 
{
public:

	//@access Creation/Initialization

	//@cmember
	/* Constructs a SECCompressFile object.*/
	SECCompressFile();

	//@cmember
	/* Constructs a SECCompressFile object.*/
	SECCompressFile(  LPCTSTR lpszFileName, 
					UINT 		nOpenFlags );
	
	//@cmember
	/* Constructs a SECCompressFile object.*/
	SECCompressFile( int hFile );

	~SECCompressFile() { Close(); }
	
	//@access Operations
	
	//@cmember
	/* Opens an SECCompressFile with a given pathname.*/
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
		CFileException* pError = NULL, int nCompLevel=-1);

	//@cmember
	/* Reads a number of bytes from the file and decompresses them. */
	UINT Read( void FAR *pBuf, UINT nCount );
	
	//@cmember
	/* Reads a number of bytes from the file and decompresses them.  */
	// Semantically equivalent to Read, in Win32.
	DWORD ReadHuge( void FAR *pBuf, DWORD nCount );
	
	//@cmember
	/* Compresses and then writes a number of characters to the file.*/
	void Write( const void FAR *pBuf, UINT nCount );
	
	//@cmember
	/* Compresses and then writes a number of characters to the file.  */
	// Semantically equivalent to Write, in Win32.
	void WriteHuge( void FAR *pBuf, DWORD nCount );
	
	//@cmember
	/* Seeks to the specified location.*/
	long Seek( long lOff, UINT nFrom );
	
	//@cmember
	/* Seeks to the beginning of file.*/
	void SeekToBegin( void );
	
	//@cmember
	/* Seeks to end of file.*/
	void SeekToEnd( void );
	
	//@cmember
	/* Returns the compressed length of the file.*/
	DWORD GetLength();
	
	//@cmember
	/* Closes the file.*/
	void Close( void );
	
	//@cmember
	/* Turns the compression mode on or off.*/
	void SetCompressMode(BOOL bState);
	
	//@cmember
	/* Retrieves whether compression is enabled or disabled.*/
	BOOL GetCompressMode();

public: 

	//@access Public data members
	
	//@cmember
	/* Compression engine object.*/
	SECComp m_Comp;

	//@cmember
	/* 'zlib' compression object.*/
	gzFile	m_gzFile; 

#ifndef WIN32
	//@cmember
	/* Flag when reading huge vals (WIN3.1 only)*/
	BOOL	m_bReadHuge;
#endif

protected:
	
	//@access Protected data members

	//@cmember
	/* Indicates whether compression is enabled or disabled.*/
	BOOL m_bCompressMode;

};	

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SECCOMPRESS_FILE_H__

