// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Author:		Scot Wingo
//  Description:	Declarations for SECZoomView
//  Created:		7/16/95
//
//  Got most logic from ScrollView, plus the tangram sample, which
//  shows how to use ANISTROPIC mode to do zooming.
//
//  Since we're aiming to be a drop-in replacement for scrollview,
//  we override some of the usual scrollview routines plus we
//  always start with zooming off and let the user set it up,
//  instead of zooming by default.
//

#ifndef __SECZMVW_H__
#define __SECZMVW_H__

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
// AutoDuck tag block block for SECZoomView 

//@doc SECZoomView
//@mfunc UINT   | SECZoomView | GetZoomMode | Retrieves the current zoom mode, one of either SEC_ZOOMOFF, SEC_ZOOMNORMAL or SEC_ZOOMFIT.
//@rdesc Returns the current zoom mode. 
//@xref<mf SECZoomView::SetZoomMode>
//@xref<c SECZoomView>

//@doc SECZoomView
//@mfunc float  | SECZoomView | GetZoomLevel | Returns a float which represents the current zoom level. To convert to a percentage
// multiply by 100.
//@rdesc Returns the current zoom level.
//@xref<mf SECZoomView::SetZoomLevel>
//@xref<mf SECZoomView::ZoomIn>
//@xref<mf SECZoomview::ZoomPercent>
//@xref<mf SECZoomView::ZoomOut>
//@xref<c SECZoomView>


//@doc SECZoomView
//@mfunc virtual void  | SECZoomView | ZoomLevelChanged | Override this virtual member function and add a call to GetZoomLevel
// if you want to update any user-interface elements to reflect new zoom levels.
//@xref<mf SECZoomView::GetZoomLevel>
//@xref<c SECZoomView>


// adinsert AutoDuck insertion point for SECZoomView 
//@doc SECZoomView 
//@class SECZoomView is a CScrollView extension that adds automatic zooming to your application views.  
//@comm
// SECZoomView does this by overloading the CScrollView::OnPrepareDC method and performing zooming 
// through standard Windows GDI mapping modes.  MM_ANISOTROPIC is the only supported mapping mode.  
// If you must use a mapping mode other than MM_ANISOTROPIC, SECZoomView cannot be used.  If your 
// current CScrollView changes the mapping modes with an OnPrepareDC override, you will need to 
// turn off these changes as they will nullify the effects of SECZoomView.

//SECZoomView supports three zoom modes:
//@flag	SEC_ZOOMOFF | Zooming is disabled.
//@flag	SEC_ZOOMNORMAL | Zooming is performed based on a floating point zoom factor.  This can be specified as a zoom 
// percentage as well.
//@flag SEC_ZOOMFIT | The SECZoomView automatically changes the zoom level such that all portions of the view are visible 
// inside the view window.
//@normal The developer can specify a min/max zoom which is appropriate for the application and platform.  
// The default is infinite zooming, but this will cause ‘wrap’ as the values which track scrolling 
// overflow.  The ‘wrap’ factor depends on the operating system (Win16 has lower tolerences than 
// Win32, etc.) A SECZoomView can be zoomed by specifying a zoom percentage, a rectangle to be zoomed, 
// or a zoom factor.
//
// SECZoomView provides an overridable callback, <mf SECZoomView::ZoomLevelChanged>, that is called 
// whenever the zoom level changes so that any user interface elements can be updated. The simplicity 
// of this approach allows any number of interesting zoom user interfaces to be used.
//
// The CLOUD sample in the \SAMPLES\TOOLKIT\STANDARD\VIEWS\CLOUD directory shows the SECZoomView and SECPanView classes in action. 
//@xref<c SECPanView>
//@BASE public | CScrollView
class SECZoomView : public CScrollView
{
    DECLARE_DYNAMIC(SECZoomView);
public:
    enum {SEC_ZOOMOFF, SEC_ZOOMPANOVER, SEC_ZOOMNORMAL, SEC_ZOOMFIT};
// Construction    
protected:
	// protected constructor used by dynamic creation
	//@Access Protected Members
	//Constructors
	//@cmember
	/* Creates an SECZoomView object.*/
    SECZoomView();   


// Override this to get zoom scale change notifications.
    //@Access Overridable
	//@cmember
	/* Override ZoomLevelChanged to receive notifications that the zoom level has been modified.  Good for */
	// automatically updating zoom user-interface elements.
    virtual void ZoomLevelChanged(void) {};

	//@cmember
	/* Called when an attempt is made to zoom beyond allowed range.*/
    virtual void ZoomOutOfRange(BOOL bUnderFlow, float fAttempt);

	//@cmember
	/* Sets the current zoom level.*/
    virtual void SetZoomLevel(float);

    
// Operations
public:
    // Overridden CScrollView member functions
    // sizeDefault lives in scrollview?
    void   SetScrollSizes(int nMapMode,
			  SIZE sizeTotal, const SIZE& sizePage = sizeDefault,
			  const SIZE& sizeLine = sizeDefault);
    void   CenterOnLogicalPoint(CPoint ptCenter);
    CPoint GetLogicalCenterPoint(void);
	void   UpdateZoomBars();   // replacement for nonvirtual UpdateBars()
    
	
	//@Access Public Members
    //Zooming Operations
	//@cmember
	/* Gets the current zoom mode setting.*/
    UINT  GetZoomMode() {return m_zoomMode;};

	//@cmember
	/* Retrieves the floating point zoom factor.*/
    float GetZoomLevel() {return m_fZoomScale;};

	//@cmember
	/* Restrict the zoom factor to a minimum and maximum zooming factor.*/
    void  SetZoomMinMax(float fMin, float fMax);

	//@cmember
	/* Restrict the zoom factor to a minimum and maximum zoom percent.*/
    void  SetZoomMinMaxPercent(int nMin, int nMax);

	//@cmember
	/* Sets the zoom mode to either SEC_ZOOMOFF, SEC_ZOOMNORMAL, or SEC_ZOOMFIT.*/
    void  SetZoomMode(UINT zoomMode);
    
	//@cmember
	/* Increases the zoom level.*/
    virtual void ZoomIn(CRect &rect);

	//@cmember
	/* Increases the zoom level.*/
    virtual void ZoomIn(CPoint *point = NULL, float delta = 1.25);

	//@cmember
	/* Decreases the zoom level.*/
    virtual void ZoomOut(CPoint *point = NULL, float delta = 1.25);

	//@cmember
	/* Sets the zoom level to 100%, or no zooming.*/
    virtual void ZoomOriginal(CPoint *point = NULL);

	//@cmember
	/* todo Add a description*/
    virtual void ZoomFit  ();
    
    //Arguments slightly different, so people can just enter percent
    //and have point as default instead of having to specify null point.
	//@cmember
	/* Zooms in or out based on a percent.  For example, 200% will zoom in, where 5% will zoom out.*/
    virtual void ZoomPercent(int nPercent = 25,CPoint *point = NULL);
    
    // Zooming utility functions
    void      ViewDPtoLP (LPPOINT lpPoints, int nCount = 1);
    void      ViewLPtoDP (LPPOINT lpPoints, int nCount = 1);
    void      ClientToDevice(CPoint &point);


// Implementation
protected:
    virtual ~SECZoomView();
#ifdef _DEBUG
    virtual void Dump(CDumpContext&) const;
    virtual void AssertValid() const;
#endif //_DEBUG
    virtual void OnDraw(CDC* pDC) = 0; // Pass on Pure Virtual from CScrollView
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
    // Printing support
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    
protected:
    void      PersistRatio(const CSize &orig, CSize &dest, CPoint &remainder);
    void      ReCalcBars(void);
    virtual	void UpdateViewport(CPoint * ptNewCenter);
    
    // Private member variables
    UINT      m_zoomMode;      // The current zoom mode: normal/off/zoom to fit
    CSize     m_szOrigTotalDev;  // Origional total size in device units
    CSize     m_szOrigPageDev;   // Origional page scroll size in device units
    CSize     m_szOrigLineDev;   // Origional line scroll size in device units
    float     m_fZoomScale;      // Current ratio between device/logical units
    float     m_fZoomMin;
    float     m_fZoomMax;
    
public:
    // Generated message map functions
    //{{AFX_MSG(SECZoomView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    
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

#endif // __SECZMVW_H__

