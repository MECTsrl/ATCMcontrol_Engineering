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
//  Description:	SECTNWinApp - A WinApp derivative that automatically
//                      uses a thumbnail file dialog.  A very thin class that
//                      really just replaces CWinApp::OnFileOpen with our own
//  Created:		Wed Jul 26 18:48:12 1995
//
//

#ifndef __SECTNWIN_H__
#define __SECTNWIN_H__

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
// AutoDuck tag block block for SECTNWinApp 

//@doc SECTNWinApp
//@mdata BOOL | SECTNWinApp | m_bSECImagesAsTN | Flag that controls display of SECImages as thumbnails.

// adinsert AutoDuck insertion point for SECTNWinApp 
//@doc SECTNWinApp 
//@class A replacement class for CWinApp for thumbnail support.
//@comm
// Replace CWinApp with SECTNWinApp, if you want to support thumbnailing in your application.  
// It is a very thin class that overrides CWinApp::OnFileOpen to create an <c SECTNFileDialog> 
// instead of a CFileDialog.  It does this to retain the same level of error checking provided 
// by the CWinApp class.
//
// To use SECTNWinApp, simply change the base class of your CWinApp derivative to SECTNWinApp.  
// SECTNWinApp::<mf SECTNWinApp::OnFileOpen> will automatically get called instead of CWinApp::OnFileOpen(), 
// and the thumbnailing common dialog will be displayed instead of the default.
//
// The sample THUMBNL in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\THUMBNL directory demonstrates how 
// to use SECTNWinApp and the other Objective Toolkit thumbnailing support classes.
// Take a look at SECTNFileDialog class description for more information.
//
// You can also have thumbnail views of image files in your File/Open dialog. This feature is 
// turned off by default. Set the BOOL member m_bSECImagesAsTN to TRUE in say your 
// derived classe's constructor to turn on the feature.
//
//@devnote
// On MFC 2.52c and lower, there cannot be a CWinApp derivative in an extension DLL.  Therefore, 
// you must link in the sectnwin.cpp source file with your application if you would like to use 
// the 16-bit Objective Toolkit DLL along with thumbnailing support.  The THUMBNL sample 
// demonstrates this build anomaly.
//@xref<c SECTNBitmap> <c SECTNDC> <c SECTNDocument> <c SECTNFileDialog> <c SECTNView> 
//@BASE public|CWinApp
class SECTNWinApp : public CWinApp
{
    
//Construction
public:
//@Access Public Members
	
	//@cmember
	/* Place holder - override for your own thumbnailing-specific construction.*/
    SECTNWinApp(); 

	//@cmember
	/* Turn this flag on (say in your derived app classes's constructor) to see thumbnail views of image files in the Thumbnail enabled File/Open dialog.*/
	BOOL m_bSECImagesAsTN;

// Overrides
    
// Implementation
	//@cmember
	/* Overrides CWinApp::OnFileOpen() to create an SECTNFileDialog instead of CFileDialog.  */
    afx_msg void OnFileOpen();    
};

//snagged from appdlgs.h
static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
			       CDocTemplate* pTemplate, CString* pstrDefaultExt);

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SECTNWIN_H__

