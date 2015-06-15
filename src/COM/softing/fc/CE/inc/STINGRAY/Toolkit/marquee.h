// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			Mark Isham
//  Description:	SECMarquee declaration
//

#ifndef __SECMARQUEE_H
#define __SECMARQUEE_H

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

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

//
// Public interface constants

// Default delay between scrolling successive frames, in milliseconds
#define SEC_MARQUEE_DEF_SCROLL_DELAY 40

// Wrap-around parms. This is a percentage (represented as a decimal)
// of the total available window width to be scrolled with blank space
// following the end of scrolled text before restarting. The following
// constants provided for ease of use
#define SEC_MARQUEE_WRAP_IMMEDIATE  (FLOAT)0       // wrap immediately
#define SEC_MARQUEE_WRAP_ON_HALF    (FLOAT)0.5     // wrap when text clears 1/2
#define SEC_MARQUEE_WRAP_ON_FULL    (FLOAT)1       // wrap when text clears all

// Default foreground/background colors
#define SEC_MARQUEE_DEF_FGND_COLOR  RGB(255,0,0)    // red
#define SEC_MARQUEE_DEF_BGND_COLOR  RGB(0,0,0)      // black

/////////////////////////////////////////////////////////////////////////////
// SECMarquee window

/////////////////////////////
// AutoDuck tag block block for SECMarquee 
//@doc SECMarquee
//@mfunc virtual BOOL  | SECMarquee | IsScrolling | Return current 
// scrolling state.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mfunc virtual void  | SECMarquee | GetWindowText | Get the current 
// marquee message text.
//@parm CString&  | strText | parm comment 
//@xref <c SECMarquee>


//@doc SECMarquee
//@mfunc virtual void  | SECMarquee | GetVisibleText | Get text visible 
// currently visible on the marquee.
//@parm CString&  | strText | parm comment 
//@xref <c SECMarquee>


//@doc SECMarquee
//@mfunc virtual void  | SECMarquee | OnScrollStart | Called when a new 
// message is about to start scrolling.
//@devnote Warning: SECMarquee uses a timer to coordiante text scrolling, 
// yielding in this callback could cause unpredictable behavior.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mfunc virtual void  | SECMarquee | OnScrollComplete | Called when an 
// existing message is about to finish.
//@devnote Warning: SECMarquee uses a timer to coordiante text scrolling, 
// yielding in this callback could cause unpredictable behavior.
//@xref <c SECMarquee>


//@doc SECMarquee
//@mdata CRect  | SECMarquee | m_ClientRect | The client rect of the control.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL   | SECMarquee | m_bWndExists | Indicates whether a hWnd 
// exists for this control.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL   | SECMarquee | m_bIsScrolling | Indicates current scrolling state.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata UINT   | SECMarquee | m_nScrollDelay | The scroll delay in milliseconds.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata UINT   | SECMarquee | m_nTimerID | The internal timer ID.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL   | SECMarquee | m_bTimerActive | Indicates whether the 
// internal timer is active.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata FLOAT  | SECMarquee | m_fWrapModifier | Wrap modifier flags.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata INT    | SECMarquee | m_nWrapPixelPadding | Wrap pixel padding.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata COLORREF  | SECMarquee | m_FgndColor | Current foreground color.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata COLORREF  | SECMarquee | m_BgndColor | Current background color.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CPen  | SECMarquee | m_penWhite | A white pen.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CPen  | SECMarquee | m_penBlack | A black pen.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CPen  | SECMarquee | m_penShadow | A COLOR_BTNSHADOW pen.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CPen  | SECMarquee | m_penFace | A COLOR_BTNFACE pen.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL  | SECMarquee | m_bDrawCtrlBorder | Indicates whether to draw the 3D border.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CFont*  | SECMarquee | m_pFont | The current font.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL  | SECMarquee | m_bShowWindow | Indicates whether the 
// control is currently visible.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL  | SECMarquee | m_bFirstScrollStart | Indicates whether 
// the first scroll has begun.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CString  | SECMarquee | m_strMarqueeText | The entire marquee 
// text to scroll.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata CString  | SECMarquee | m_strVisibleText | Currently displayed text.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata int  | SECMarquee | m_nNextDisplayCharIndex | Moving position pointer.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata BOOL  | SECMarquee | m_bLastCharDisplayed | Indicates that the 
// last character was displayed.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata INT    | SECMarquee | m_nLeftPixelExtent | Starting pixel position 
// of visible marquee.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata INT    | SECMarquee | m_nRightPixelExtent | Ending pixel position 
// of visible marquee.
//@xref <c SECMarquee>

//@doc SECMarquee
//@mdata INT    | SECMarquee | m_nScrollStep | The ScrollStep defines how many pixels the 
//marquee is moved with each timer tick. Negative values scroll backwards.

//@doc SECMarquee
//@mfunc void  | SECMarquee | SetScrollStep | Sets the ScrollStep. 
//@parm INT    | v | Positive values scroll the text towards the left. Negative values scroll to the right.


//@doc SECMarquee
//@mfunc INT   | SECMarquee |  GetScrollStep | Returns the current ScrollStep value.


// adinsert AutoDuck insertion point for SECMarquee 
//@doc SECMarquee 
//@class The SECMarquee class is a scrollable text marquee control class.
// Fonts, colors, scrolling speed and wrapping mode are all configurable.
// The attributes can easily be changed at runtime.  Since it is CWnd derived,
// it can be embedded on a view or dialog just like any other control.
//
//@comm The marquee can be used anywhere a CStatic can be used.  It can be attached
// to an existing CStatic control with the <mf SECMarquee::AttachStatic> method,
// or can be created dynamically with the <mf SECMarquee::Create> method. 
//
// The STATBAR sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\MASKTEST directory demonstrates 
// how to use the SECMarquee class.
//@base public | CStatic
//@devnote When this object is used on Window CE devices the scroll rate is locked by the maximum update
// rate of the screen. This means that the speed cannot go above a slow crawl in some cases. Although the
// preprocessor definition SCROLL_PIXELS_PER_FRAME sets the step rate this sets the rate for all marquees
// globally. The variable m_nScrollStep has been added to allow individual control of the step rate and direction.
class SECMarquee : public CStatic
{
public:
		DECLARE_DYNAMIC(SECMarquee)

        // ************************************************************
        // Public Interface
        // ************************************************************

		//@access Creation/Initialization

		//@cmember
		/* Constructs a SECMarquee object.*/
        SECMarquee();
		
		//@cmember
		/* Dynamically create the marquee control*/
        virtual BOOL Create(RECT rect,CWnd* pParent,LPCTSTR lpszText=_T(""),
                    DWORD dwStyle=WS_VISIBLE|WS_CHILD,UINT nControlID=0xFFFF);

		//@cmember
		/* Attach marquee to an existing static control*/
        virtual BOOL AttachStatic(int nCtlID,CWnd* pParentWnd);


		//@access Attributes

		//@cmember
		/* Set scrolling delay*/
        virtual BOOL SetScrollDelay(UINT uNewDelay=SEC_MARQUEE_DEF_SCROLL_DELAY);

		//@cmember
		/* Set text wrapping mode*/
        virtual BOOL SetTextWrap(FLOAT fraction=SEC_MARQUEE_WRAP_ON_HALF);

		//@cmember
		/* Set foreground and background colors*/
        virtual void SetColors(COLORREF fgnd=SEC_MARQUEE_DEF_FGND_COLOR,
							   COLORREF bgnd=SEC_MARQUEE_DEF_BGND_COLOR);

		//@cmember
		/* Return current scrolling state.*/
        virtual BOOL IsScrolling() { return m_bIsScrolling; }

		//@cmember
		/* Set/Reset marquee message text*/
        virtual void SetWindowText(LPCTSTR lpszNewText=(LPCTSTR)NULL);

		//@cmember
		/* Get the current marquee message text.*/
        virtual void GetWindowText(CString& strText) { strText=m_strMarqueeText; }

		//@cmember
		/* Get text visible currently visible on the marquee.*/
		virtual void GetVisibleText(CString& strText) { strText=m_strVisibleText; }

		//@cmember
		/* Reset the marquee font*/
        virtual void SetFont(CFont* pFont=(CFont *)NULL,BOOL bRedraw=TRUE);

		//@access Operations

		//@cmember
		/* Start/stop the marquee scrolling*/
        BOOL Scroll(BOOL bDoScroll=TRUE);

		//@cmember
		/* Setting the scroll-step sets how many pixels the marquee is moved with each timer tick */
		void SetScrollStep(INT   v){m_nScrollStep=v;}

		//@cmember
		/* Returns the scroll step value */
		INT   GetScrollStep(){return m_nScrollStep;}

protected:
        // ************************************************************
        // Protected Implementation 
        // ************************************************************

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(SECMarquee)
        protected:
        //}}AFX_VIRTUAL

		//@access Overridable

		// Warning: SECMarquee uses a timer to coordiante text scrolling, 
		// yielding during OnScrollStart or OnScrollComplete could cause 
		// unpredictable behavior.

		//@cmember
		/* Called when a new message is about to start scrolling.*/
		virtual void OnScrollStart() {}           

		//@cmember
		/* Called when an existing message is about to finish.*/
        virtual void OnScrollComplete() {}       
	
		//@cmember
		/* Called when the marquee is initializing.*/
        virtual BOOL OnInitMarquee();

		//@cmember
		/* Called when the marquee is scrolling 1 frame.*/
        virtual BOOL OnScrollMarquee();

        // Implementation
		//@cmember
		/* Draws the 3D border around the marquee control.*/
		virtual void Draw3DBorder(CDC* pDC,CRect rect);
		
		//@cmember
		/* Paints the marquee control.*/
		virtual void DoPaint(CPaintDC* pdc);
		
		//@cmember
		/* Resets the marquee control.*/
		virtual void ResetMarquee();
public:
        virtual ~SECMarquee();

        // Generated message map functions
protected:
        //{{AFX_MSG(SECMarquee)
        afx_msg void OnPaint();
        afx_msg void OnTimer(UINT nIDEvent);
        afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		afx_msg void OnSize(UINT nType,int cx,int cy);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

        // protected member vars
		//@access Protected data members

		//@cmember
		/* The client rect of the control.*/
        CRect m_ClientRect;

		//@cmember
		/* Indicates whether a hWnd exists for this control.*/
        BOOL  m_bWndExists;

		//@cmember
		/* Indicates current scrolling state.*/
        BOOL  m_bIsScrolling;

		//@cmember
		/* The scroll delay in milliseconds.*/
        UINT  m_nScrollDelay;

		//@cmember
		/* The internal timer ID.*/
        UINT  m_nTimerID;

		//@cmember
		/* Indicates whether the internal timer is active.*/
        BOOL  m_bTimerActive;

		//@cmember
		/* Wrap modifier flags.*/
        FLOAT m_fWrapModifier;

		//@cmember
		/* Wrap pixel padding.*/
        INT   m_nWrapPixelPadding;

		//@cmember
		/* Current foreground color.*/
        COLORREF m_FgndColor;

		//@cmember
		/* Current background color.*/
        COLORREF m_BgndColor;

		//@cmember
		/* A white pen.*/
        CPen m_penWhite;

		//@cmember
		/* A black pen.*/
        CPen m_penBlack;

		//@cmember
		/* A COLOR_BTNSHADOW pen.*/
        CPen m_penShadow;
		
		//@cmember
		/* A COLOR_BTNFACE pen.*/
        CPen m_penFace;

		//@cmember
		/* Indicates whether to draw the 3D border.*/
        BOOL m_bDrawCtrlBorder;

		//@cmember
		/* The current font.*/
        CFont* m_pFont;

		//@cmember
		/* Indicates whether the control is currently visible.*/
		BOOL m_bShowWindow;

		//@cmember
		/* Indicates whether the first scroll has begun.*/
		BOOL m_bFirstScrollStart;

        // maintain string state to allow redraw at any time

		//@cmember
		/* The entire marquee text to scroll.*/
        CString m_strMarqueeText;
		
		//@cmember
		/* Currently displayed text.*/
        CString m_strVisibleText;

		//@cmember
		/* Moving position pointer.*/
        int m_nNextDisplayCharIndex;

		//@cmember
		/* Indicates that the last character was displayed.*/
        BOOL m_bLastCharDisplayed;

        // starting and ending pixel positions of visible portion
        // of scrollable marquee. Note: start may be less than zero
        // and end may be larger than right margin - this allows
        // graphic effect of "running off the edge"
		
		//@cmember
		/* Starting pixel position of visible marquee.*/
        INT   m_nLeftPixelExtent;
		
		//@cmember
		/* Ending pixel position of visible marquee.*/
        INT   m_nRightPixelExtent;

		//@cmember
		/* Number of pixels to scroll marquee */
		INT   m_nScrollStep;



private:
        // establish unique timer id for each instance
        static int nNextAvailTimerID;
};


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// WIN32

#endif	// __SECMARQUEE_H

/////////////////////////////////////////////////////////////////////////////

