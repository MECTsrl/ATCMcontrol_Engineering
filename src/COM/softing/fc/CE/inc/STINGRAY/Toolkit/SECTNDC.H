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
//  Description:	Declarations for SECTNDC the ThumbNail DC class
//  Created:		Tue Jul 25 23:13:22 1995
//

#ifndef __SECTNDC_H__
#define __SECTNDC_H__

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

/////////////////////////////
// AutoDuck tag block block for SECTNDC 
// adinsert AutoDuck insertion point for SECTNDC 
//@doc SECTNDC 
//@class SECTNDC is a CDC derivative that creates a memory DC with an SECTNBitmap into which an 
// SECTNView draws a thumbnail of the view.  
//@comm
// SECTNView::<mf SECTNView::GetThumbNailBitmap> automatically creates a SECTNDC object and passes 
// it to your <c SECTNView> derivative’s <mf SECTNView::OnThumbNail> member function.
//
// The use of SECTNDC is transparent, and in most cases the user does not need to directly
// call any of the SECTNDC members.
//
// The THUMBNL sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\THUMBNL directory demonstrates how to use SECTNDC and 
// the other Objective Toolkit thumbnail support classes.
//@xref<c SECTNBitmap> <c SECTNDocument> <c SECTNFileDialog> <c SECTNView> <c SECTNWinApp>
//@BASE public|CDC
class SECTNDC : public CDC
{
    DECLARE_DYNAMIC(SECTNDC);
    
// Constructors
public:
    SECTNDC(); 
    void Create(CSize * pSize); //The size of the bitmap
    
// Operations
    BOOL IsThumbNailing() { return TRUE;}; //This DC is a thumbnailing machine.
    
// Attributes
public:
    SECTNBitmap * m_pTNBitmap;   //A serializeable thumbnail bitmap, kicks ass!
    CBitmap     * m_pOldBitmap;  //Store any old bitmap from select
    
// Implementation
public:
    virtual ~SECTNDC();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
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


#endif // __SECTNDC_H__
