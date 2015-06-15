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
//  Author:		Scot Wingo
//  Description:	Declarations for SECTNBitmap
//  Created:		Thu Jul 20 21:08:46 1995
//

#ifndef __SECTNBITMAP_H__
#define __SECTNBITMAP_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL



//SECTMBitmap = SECBitmap + Serialization.

/////////////////////////////
// AutoDuck tag block block for SECTNBitmap 
// adinsert AutoDuck insertion point for SECTNBitmap 
//@doc SECTNBitmap 
//@class SECTNBitmap encapsulates a thumbnail bitmap.  A thumbnail bitmap is basically an in-memory 
// DIB that knows how to serialize itself to an archive through the MFC standard Serialize() member 
// function.
//@comm
// When an SECTNDocument is serialized, it first creates an SECTNBitmap from the current view and 
// serializes that.  After the thumbnail, the document continues to serialize application-specific 
// data as normal.  <c SECTNView> takes care of creating the SECTNBitmap and <c SECTNFileDialog>
// handles reading in and displaying the SECTNBitmap in the thumbnail-enabled open file common 
// dialog.
//
// One side effect of Objective Toolkit using the DIB format to store the thumbnail is that the 
// Windows 95 Explorer will automatically display a thumbnail of the thumbnail in the Explorer and 
// common dialogs.  To turn off this feature, change the signature of the first two bytes from DIB 
// (0x4d42) to another non-DIB value.  This signature is written and checked in the 
// <mf SECTNBitmap::Serialize> member funtion.
//
// In most cases the application will never have to deal directly with SECTNBitmap.
//
// The THUMBNL sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\THUMBNL directory demonstrates how to use SECTNBitmap and the other Objective Toolkit
// thumbnail support classes.
//@xref<c SECTNDC> <c SECTNDocument> <c SECTNFileDialog> <c SECTNView> <c SECTNWinApp>
//@BASE public|CBitmap
class SECTNBitmap : public CBitmap
{

protected: //Create from serialization only.
    DECLARE_DYNCREATE(SECTNBitmap) 
    SECTNBitmap();
    
// Implementation
public:
    virtual ~SECTNBitmap();
    virtual void Serialize(CArchive& ar);
#ifdef _DEBUG
    virtual void Dump(CDumpContext& dc) const;    
#endif

// DIB Helper functions
public:
    HANDLE CreateDIB(void);
    HBITMAP CreateDDBFromDIB(HANDLE);
    WORD FAR PaletteSize(LPTSTR lpDIB);
    WORD FAR DIBNumColors(LPTSTR lpDIB);
    LPTSTR FAR FindDIBBits(LPTSTR lpDIB);
    BOOL CreateDIBPalette(HANDLE,CPalette*);
    void PaintDIB(HANDLE);
    
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif // __SECTNBITMAP_H__
