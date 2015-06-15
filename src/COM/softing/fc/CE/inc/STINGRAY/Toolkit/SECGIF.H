// Stringray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc,
// All rights reserved.
//
// SEC GIF file format header
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

#ifndef __SECGIF_H__
#define __SECGIF_H__

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

// Typedefs needed by the GIF reading engine.
//
//
typedef struct _GifHeader 
{
	BYTE Signature[3];	// should be "GIF"
	BYTE Version[3];	// should be "87A" or "89A"

	// Logical screen descriptor
	WORD Width;
	WORD Height;
	BYTE ColorInfo;		// Bits 0-2 size of global color table
						// Bit 3 color sorting toggle (for 89A only)
						// Bits 4-6 color resolution; max size of palette
						// Bit 7 toggle for global color table

	BYTE BackColor;		// index into global color table for color to use for background
	BYTE AspectRatio;	// ranges 1 to 255 according to : (AspectRatio + 15)/64
} GifHeader;

typedef struct _GifImageDescriptor 
{
	BYTE Separator;	// should contain value of 0x2c to denote beginning
	WORD Left;
	WORD Top;
	WORD Width;
	WORD Height;
	BYTE ImageInfo;	// Bit 0 local color table flag
					// Bit 1 Interlace flag
					// Bit 2 Sort flag
					// Bits 3-4 Reserved
					// Bits 5-7 size of local color table
} GifImageDescriptor;

typedef struct _GifGraphicsControlExtension
{
	BYTE Introducer;	// always 21h
	BYTE Label;			// always f9h
	BYTE BlockSize;		// always 04h
	BYTE Packed;		// method of graphics disposal to use
	WORD DelayTime;		// hundreths of seconds to wait
	BYTE ColorIndex;	// Transparent Color Index 
	BYTE Terminator;	// Block terminator (always 0)
} GifGraphicControl;

typedef struct _GifPlainTextExtension
{
	BYTE Introducer;	// always 21h
	BYTE Label;			// always 01h
	BYTE BlockSize;		// always 0ch
	WORD TextGridLeft;	// X Position of text grid in pixels
	WORD TextGridTop;	// Y position of text grid in pixels
	WORD TextGridWidth;	// width of text grid in pixels
	WORD TextGridHeight;	// height of text grid in pixels
	BYTE CellWidth;		// Width of a grid cell in pixels
	BYTE CellHeight;	// Height of a grid cell in pixels
	BYTE TextFgColorIndex;	// text fg color index val
	BYTE TextBgColorIndex;	// text bg color index val
	BYTE *PlainTextData;	// the plain text data
	BYTE Terminator;	// Block terminator (always 0)
} GifPlainText;

typedef struct _GifApplicationExtension
{
	BYTE Introducer;	// Extension introducer (0x21)
	BYTE Label;			// always ffh
	BYTE BlockSize;		// always 0bh
	char Identifier[8];	// application identifier
	BYTE AuthentCode[3];	// application authentication
	BYTE *ApplicationData;	// point to application data sub-blocks
	BYTE Terminator;	// block terminator (always 0)
} GifApplication;

typedef struct _GifCommentExtension
{
	BYTE Introducer;	// always 21h
	BYTE Label;			// comment label (always 0xfe)
	BYTE *CommentData;	// Pointer to comment data sub-blocks
	BYTE Terminator;	// always 0
} GifComment;


#define SEC_COLORMAP 0x80
#define READSIZE(y,z)  if ((GetCache((y), (z))) != (z)) return FALSE;
#define EXTFLAG		0x21
#define IMAGESEP	0x2c
#define TRAILER		0x3b
#define INTERLACEMASK 0x40
#define GIF_CACHE_SIZE 60000

/////////////////////////////
// AutoDuck tag block block for SECGif
//@doc SECGif
//@mdata BOOL  | SECGif | m_bInterlaced | Interlaced flag.
//@xref <c SECGif> <mf SECGif::SetInterlaced> <mf SECGif::GetInterlaced>

//@doc SECGif
//@mdata BOOL  | SECGif | m_bUseTransparent | Trasparency flag.
//@xref <c SECGif> <mf SECGif::SetTransparent> <mf SECGif::GetTransparent>

//@doc SECGif
//@mdata BYTE  | SECGif | m_cTransRed | Red transparency color.
//@xref <c SECGif> <mf SECGif::SetTransparentColors> <mf SECGif::SetTransparent> 
// <mf SECGif::GetTransparent>

//@doc SECGif
//@mdata BYTE  | SECGif | m_cTransGreen | Green transparency color.
//@xref <c SECGif> <mf SECGif::SetTransparentColors> <mf SECGif::SetTransparent> 
// <mf SECGif::GetTransparent>

//@doc SECGif
//@mdata BYTE  | SECGif | m_cTransBlue | Blue transparency color.
//@xref <c SECGif> <mf SECGif::SetTransparentColors> <mf SECGif::SetTransparent> 
// <mf SECGif::GetTransparent>

//@doc SECGif
//@mfunc void  | SECGif | SetInterlace | Sets the interlaced state of the GIF image.
//@parm BOOL  | bArg | Interlaced state. 
//@xref <c SECGif> <mf SECGif::GetInterlaced>

//@doc SECGif
//@mfunc void  | SECGif | SetTransparent | Sets the transparency state of the GIF image.
//@parm BOOL  | bArg | Transparency state.
//@xref <c SECGif> <mf SECGif::GetTransparent> 

//@doc SECGif
//@mfunc void  | SECGif | SetTransparentColors | Sets the transparency colors 
// for the GIF image.
//@parm BYTE  | cRed | Red transparency color.
//@parm  BYTE  | cGreen | Green transparency color.
//@parm  BYTE  | cBlue | Blue transparency color.

//@doc SECGif
//@mfunc BOOL  | SECGif | GetTransparent | Returns the transparent state of the GIF image.
//@rdesc Nonzero if the GIF image is transparent; otherwise, 0.
//@xref <c SECGif> <mf SECGif::SetTransparent>

//@doc SECGif
//@mfunc COLORREF  | SECGif | GetTransparentColor | Returns the transparent color for the GIF image.
//@rdesc Returns the current transparent color for the GIF image.
//@xref <c SECGif> <mf SECGif::SetTransparentColor>

//@doc SECGif
//@mfunc BOOL  | SECGif | GetInterlaced | Returns the interlaced state of the GIF image.
//@rdesc Nonzero if the GIF image is interlaced; otherwise, 0.
//@xref <c SECGif> <mf SECGif::SetInterlaced>

// adinsert AutoDuck insertion point for SECGif
//@doc SECGif
//@class SECGif is a <c SECImage> derived class which supports the GIF (Graphics Interchange 
// format) file standard.  
//@comm GIF images are very popular for use on the Internet and other 
// areas.  
//
// SECGif provides support for GIF87 as well as GIF89 formats.
//
// It should be noted that all GIF images use the LZW compression scheme which 
// Unisys owns a patent on.  If you plan on using GIF images in your applications, you 
// will need to contact sales@stingray.com to obtain item number 1024, the GIF/TIFF LZW 
// Unisys unlock pack for a nominal fee. 
//
// The LZW compression algorithm is a patent owned by Unisys. Since Objective Toolkit exposes 
// the algorithm as part of the image classes, we are required to charge a small additional fee 
// to cover the cost of the patent. Since we provide Objective Toolkit in source code format, 
// we expose the LZW compression scheme and users who wish to simply read GIF images are still 
// required to pay the additional fee to unlock the SECGif image libraries.
// 
// See the IMAGETST sample in the \SAMPLES\TOOLKIT\STANDARD\IMAGES\IMAGETST directory for a 
// demonstration of this class.
//@xref <c SECImage>
//@base public | SECImage
class SECGif : public stingray::foundation::SECImage
{
    
    DECLARE_SERIAL(SECGif);
    
	//@access Construction

	//@cmember
	/* Constructs a SECGif object.*/
    SECGif();

	//@cmember
	/* Constructs a SECGif object.*/
    SECGif(LPCTSTR lpszFileName);

public:
	//@access Attributes

	//@cmember
	/* Sets the transparent state of the GIF image.*/
	void SetTransparent(BOOL bArg) {m_bUseTransparent = bArg;}

	//@cmember
	/* Returns the transparent state of the GIF image.*/
	BOOL GetTransparent() 			{ return m_bUseTransparent; }

	//@cmember
	/* Sets the transparent color for the GIF image.*/
	void SetTransparentColors(BYTE cRed, BYTE cGreen, BYTE cBlue) 
		{ m_cTransRed = cRed; m_cTransGreen = cGreen; m_cTransBlue = cBlue; }

	//@cmember
	/* Returns the transparent color for the GIF image.*/
	COLORREF GetTransparentColor() 			
		{ return RGB(m_cTransRed,m_cTransGreen,m_cTransBlue); } 

	//@cmember
	/* Sets the interlaced state of the GIF image.*/
    void SetInterlace(BOOL bArg) {m_bInterlaced = bArg;}

	//@cmember
	/* Returns the interlaced state of the GIF image.*/
	BOOL GetInterlaced() 			{ return m_bInterlaced; } 
    
public:
	// access Operations

    void Serialize(CArchive& ar);
    
	virtual ~SECGif();
	
	// Overridables
protected:
    virtual BOOL DoLoadImage (CFile *pFile); /* Load native format & convert to DIB */
    virtual BOOL DoSaveImage (CFile *pFile); /* Save as native format */
    
// Implementation
protected:

#ifdef WIN32
   BYTE *m_lpPic8, *m_lpRaster, *m_lpRawGIF; 
#else
   BYTE __far *m_lpPic8, *m_lpRaster, *m_lpRawGIF; 
#endif
    HANDLE m_hPic8, m_hRaster, m_hRawGIF;
    WORD m_wXC;
    WORD m_wYC;
    long m_lPass; 
    long m_lBitOffset;
    long m_lColorMapSize;
    long m_lNumCols;
    long m_lBitMask;
    long m_lCodeSize;
    long m_lReadMask;
    int m_nBlockCount;
	int m_nFreeCode;
    // temporary values used in decompress functions
    BYTE m_byte1, m_byte2, m_byte3;     
#ifndef WIN32
    BYTE m_pad; //for 16bit warnings..
#endif //WIN32  
    long m_nByteOffset;
    CPalette m_Palette;
    BOOL	m_b87a;	/*TRUE = old format */

	//@cmember
	/* Interlaced flag.*/
    BOOL m_bInterlaced; 

    GifHeader	m_gifHeader;
    long m_nPixelBits;
    GifImageDescriptor m_gifID;
    GifGraphicControl m_gifGraphicControl;
    GifPlainText m_gifPlainText;
    GifApplication m_gifApplication;
    GifComment m_gifComment;

	//@cmember
	/* Transparency flag.*/
	BOOL m_bUseTransparent;

	BOOL m_bIsTransparent;
	BYTE m_cTransIndex;

	//@cmember
	/* Red transparency color.*/
	BYTE m_cTransRed;
	//@cmember
	/* Green transparency color.*/
	BYTE m_cTransGreen;
	//@cmember
	/* Blue transparency color.*/
	BYTE m_cTransBlue;
    
    DWORD LZWEncodeImage(DWORD data_size);
    BOOL ReadExtension();
    BOOL ReadImageHeader();
    BOOL ReadImage();
    BOOL ReadTrailer();
    BOOL ShiftRasterBytes(long);
    int ReadCode(BOOL);
    BOOL SECCreateColorTable(int nSize);
    int SECReadCode();
    void DoInterlace(int);
    
#ifdef _DEBUG
	void AssertValid() const;
#endif

};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif // __SECGIF_H__

