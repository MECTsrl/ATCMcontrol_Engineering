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
//  Description:	Implementation of SECTNView -ThumbNail View Class
//  Created:		Tue Jul 25 23:10:29 1995
//

#ifndef __SECTNVW_H__
#define __SECTNVW_H__

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

class SECTNBitmap;
class SECTNDC;

/////////////////////////////
// AutoDuck tag block block for SECTNView 
//@doc SECTNView
//@mfunc BOOL  | SECTNView | IsThumbNailing |Determines if the SECTNView is in the process of creating a thumbnail. If FALSE, 
// the view is either drawing to the display, printer or another non-thumbnail device.
//@rdesc Nonzero if the view is creating a thumbnail, or 0 if it is not.
//@xref<c SECTNView>

//@doc SECTNView
//@mfunc void | SECTNView | OnDraw | Pure virtual function. As with CView, override to perform 
// your drawing.  OnDraw will be called with an SECTNDC during thumbnail creation. You can call 
// IsThumbNailing in your OnDraw to perform customized thumbnail creation.
//@parm CDC* | pDC | Device Context used.
//@rdesc void
//@xref<c SECTNView>



// adinsert AutoDuck insertion point for SECTNView 
//@doc SECTNView 
//@class SECTNView is a CView replacement that adds automatic thumbnailing to your view.  
//@comm
// To add thumbnailing support to your view, follow these easy steps:
//
//1.	Derive your application view from SECTNView instead of CView.<nl>
//2.	By default, this will automatically create a thumbnail for you.  If you would like to 
// optimize the drawing of the thumbnail (the default is to just redraw the entire client area 
// into the thumbnail and scale it down to be displayed as a miniature), you can call 
// <mf SECTNView::IsThumbNailing> in your OnDraw method to detect the special thumbnailing case.<nl>
//3.	SECTNView also provides some overridables such as OnPrepareThumbnail that let you set the size 
// and other aspects of the thumbnail image before it is created.
//
// The sample THUMBNL in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\THUMBNL directory demonstrates how to add 
// thumbnail support to your MFC application.
//@xref<c SECTNBitmap> <c SECTNDC> <c SECTNDocument> <c SECTNFileDialog> <c SECTNWinApp>
//@BASE public|CView
class SECTNView : public CView
{
    friend class SECTNDocument;
    
protected:
    SECTNView();           // protected constructor used by dynamic creation
    DECLARE_DYNAMIC(SECTNView);
    
// Attributes
//@Access Public Members
public:
    BOOL m_bThumbNailing;
	//@cmember
	/* Determines if the SECTNView is in the process of creating a thumbnail.*/
    BOOL IsThumbNailing() {return m_bThumbNailing;};
    
// Operations
public:
    
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SECTNView)
//@Access Protected Members
protected:
	// pass along pure virtual
	//@cmember
	/* Pure virtual function. OnDraw will be called with an SECTNDC during thumbnail creation.*/
	virtual void OnDraw(CDC* pDC) = 0;      
    
    //}}AFX_VIRTUAL
    
    //Called by SECTNDialog to get thumbnail   
	//@cmember
	/* Called by SECTNDocument to get the thumbnail bitmap.  Override to substantially change the thumbnailing logic.*/
    virtual SECTNBitmap * GetThumbNailBitmap();
    
    //Override OnPrepareThumbNail to set size of thumbnail
	//@cmember
	/* Override to specify a size.  The default is the size of the client window.*/
    virtual void OnPrepareThumbNail(CSize * pSize);
    
    //Override to manipulate TNDC for special thumbnail DC objects.
	//@cmember
	/* Override to manipulate the SECTNDC for any specialized thumbnail drawing to be performed in your OnDraw routine.*/
    virtual void OnThumbNail(SECTNDC * pDC);
    
// Implementation
protected:
    virtual ~SECTNView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    
    // Generated message map functions
protected:
    //{{AFX_MSG(SECTNView)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //__SECTNVW_H__



