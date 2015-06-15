/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/Blob.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Blob.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/Blob.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
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

#if !defined(AFX_BLOB_H__F3D51FE3_28FF_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_BLOB_H__F3D51FE3_28FF_11D5_A05C_0050BF490551__INCLUDED_

#include "FormatException.h"

struct CBlob 
{
    enum {BlobSize = 160};
	unsigned short size;
	unsigned char buffer[BlobSize];

	CBlob() {size = 0;}
	void add(unsigned char ch) 
	{
		if( size >= BlobSize) {
			ASSERT(_ASSERT_);
			throw CFormatException("Blob");
		}
		buffer[size++] = ch;
	}

	void adds4(unsigned long l) 
	{
		if( size >= BlobSize) {
			ASSERT(_ASSERT_);
			throw CFormatException("Blob");
		}
		buffer[size++] = (unsigned char)(l >> 24);
		buffer[size++] = (unsigned char)(l >> 16);
		buffer[size++] = (unsigned char)(l >>  8);
		buffer[size++] = (unsigned char)(l      );
	}

	void add(CBlob& b) 
	{
		if( size + b.size >= BlobSize) {
			ASSERT(_ASSERT_);
			throw CFormatException("Blob");
		}
      memcpy(&buffer[size], b.buffer, b.size);
      size += b.size;
	}
};

#endif

/* ---------------------------------------------------------------------------- */
