// Stringray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc,
// All rights reserved.
//
// SEC DIB file format header
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
// CHANGELOG: 
//
//    AAB	7/22/95	Started
//    MSW       9/30/95 Code review
//
//

#ifndef __SFL_SECDIB_H__
#define __SFL_SECDIB_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SFL_SECIMAGE_H__
#include "Foundation\Image\MFC\secimage.h"
#endif

//
// CMN Extension DLL
// Initialize declaration context
//

#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL


namespace stingray {
namespace foundation {

/////////////////////////////
// AutoDuck tag block block for SECDib
// adinsert AutoDuck insertion point for SECDib
//@doc SECDib
//@class SECDib derives from <c SECImage> and supports the Windows DIB (device 
// independent bitmap) format, often stored as ".dib" or ".bmp" files.   
//@comm Currently, SECDib only supports Windows version 3 bitmaps which do not include encoding.
// Numerous Windows reference manuals document the format of DIB files, as well as 
// APIs used to manipulate DIB images.
//
// See the IMAGETST sample in the \OT\SAMPLES\STANDARD\IMAGES\IMAGETST directory for a 
// demonstration of this class.
//@base public | SECImage
class SECDib : public SECImage
{
    DECLARE_SERIAL(SECDib)
	
// Construction
public:
	//@cmember
	/* Constructs a SECDib object.*/
    SECDib();

// Implementation
protected:
    LPBITMAPFILEHEADER  m_lpbmfHdr;

    virtual BOOL DoSaveImage(CFile* pFile);
    virtual BOOL DoLoadImage(CFile* pFile);

public:    
    void Serialize(CArchive& ar);
    virtual  ~SECDib();
#ifdef _DEBUG
    void AssertValid() const;
#endif
};

};  // namespace stingray::foundation
};  // namespace stingray

//////////////////////////////////////////////////////
// Promote the operator>> symbol declared by the
// DECLARE_SERIAL macro to the global namespace.
using stingray::foundation::operator>>;
//////////////////////////////////////////////////////

//
// CMN Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SFL_SECDIB_H__


