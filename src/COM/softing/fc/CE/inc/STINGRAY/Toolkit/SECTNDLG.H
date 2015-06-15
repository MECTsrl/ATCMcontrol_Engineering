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
//  Description:	Declaration for old-style Thumbnail dialog
//  Created:		Fri Jun 30 16:19:42 1995

#ifndef __SECTNFILEDLG_H__
#define __SECTNFILEDLG_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#if !defined(WIN32) && !defined(_INC_TOOLHELP)
#include /* */ "toolhelp.h"
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

#ifndef WIN32
typedef UINT (CALLBACK *LPOFNHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
#endif //WIN32

class SECTNBitmap; //predeclaration
/////////////////////////////
// AutoDuck tag block block for SECTNFileDialog 
// adinsert AutoDuck insertion point for SECTNFileDialog 
//@doc SECTNFileDialog 
//@class SECTNFileDialog adds the thumbnailing area to the common ‘open file’ dialog.  
// In addition, it automatically reads in a saved thumbnail image and displays it in the thumbnail 
// preview area when the user selects the file name.
//@comm
// If no preview is available, it displays ‘no preview’. 
//
// All of the SECTNFileDialog operations happen transparently. In most cases the developer does not 
// ever have to know about SECTNFileDialog, since <c SECTNWinApp> will automatically create,
// display and destroy the dialog.
// 
// If you currently have your own template or hook procedure for CFileDialog, you will
// need to reconcile your specializations with those made by SECTNFileDialog.
// 
//The THUMBNL sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\THUMBNL directory demonstrates how to use SECTNFileDialog and the other 
// Objective Toolkit thumbnail support classes.
//
// The SECTNFileDialog can also display a thumbnail of the image files supported by OT (.bmp, .dib,
// .gif, .jpg, .tif, .tga, .pcx) provided these formats are currently selected in the build-config.
// To turn on this feature call the following static function in say your apps InitInstance:
// SECTNFileDialog::SetDisplaySECImages(TRUE);
// If you need only the thumbnail of the image files to be displayed in the preview area then all
// you have to do is derive your app class from SECTNWinApp, set the handler for ID_FILE_OPEN to be
// SECTNWinApp::OnFileOpen and call SetDisplaySECImages.
//
// The IMAGETST sample in the \SAMPLES\TOOLKIT\STANDARD\IMAGE\IMAGETST directory demonstrates how to use SECTNFileDialog 
// to display thumbnail views of image files in the File/Open dialog.
//
//@xref<c SECTNBitmap> <c SECTNDC> <c SECTNDocument> <c SECTNView> <c SECTNWinApp>
//@BASE public|CFileDialog
class SECTNFileDialog : public CFileDialog
{
    DECLARE_DYNAMIC(SECTNFileDialog);
   
// Construction
public:
    SECTNFileDialog(BOOL bOpenFileDialog,
		    LPCTSTR lpszDefExt = NULL,
		    LPCTSTR lpszFileName= NULL,
		    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		    LPCTSTR lpszFilter = NULL,
		    CWnd * pParentWnd = NULL);
    
// Attributes
// Operations
// Implementation    
public:
    TCHAR m_szCurrentFile[MAX_PATH+1]; //For paints
    CStatic * m_pStatic;  //Pointer to our static where TN displayed.
    BOOL m_bExplorerStyle;
	BOOL m_bDisplaySECImages;	//Display SECImage files in the TN view.

#ifdef WIN32
#ifdef _UNICODE
    void SetCurrentFile(const unsigned short* lpszNewFile)
#else
    void SetCurrentFile(LPCTSTR lpszNewFile)
#endif
		{_tcscpy(m_szCurrentFile,lpszNewFile);};
#else
	// 16-bit
    void SetCurrentFile(const char * lpszNewFile)
    {strcpy(m_szCurrentFile,lpszNewFile);};    
#endif

    // Load the thumbnail from a filename
#ifdef WIN32    
    SECTNBitmap * LoadThumbnail(LPCTSTR);	
	stingray::foundation::SECImage * LoadSECImage(LPCTSTR);
#else //WIN16
    SECTNBitmap * LoadThumbnail(const TCHAR *);
	stingray::foundation::SECImage * LoadSECImage(const TCHAR *);
#endif //WIN32
    
    // Thumbnail drawing routines
    void ClearThumbnail(CDC *,CRect *);
    void DisplayNoThumbnail(CDC *,CRect *);
    void DisplayThumbnail(CDC *,CRect *, SECTNBitmap *);
	void DisplaySECImage(CDC *, CRect *, stingray::foundation::SECImage *);
    void UpdateThumbnail();
    void OnNewFile(LPTSTR lpszFileName);

    // Hooks - only needed/used for Win32 commdlgs.
#ifdef WIN32    
    static BOOL CALLBACK TNFileDlgHookProc(HWND,UINT,WPARAM,LPARAM);
    static SECTNFileDialog * StaticGetPointer(HWND);
#endif //WIN32

//Overrides
public:
    virtual ~SECTNFileDialog(void);
    virtual void OnLBSelChangedNotify(UINT,UINT,UINT);
    virtual BOOL OnInitDialog();
    virtual int DoModal();
    void ResizeDialog();
    
#if _MFC_VER >= 0x400
    void OnFileNameChange();
    void OnFolderChange();
#endif //_MFC_VER >= 0x400
    
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __SECTNFILEDLG_H__
