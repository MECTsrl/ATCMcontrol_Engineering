/////////////////////////////////////////////////////////////////////////////
// Calendar.h : header file for the SECCalendar custom control
//
// Stingray Software Extension Classes
// Copyright (C) 1996 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
// CHANGELOG:
//
//    PCB	01/19/96	Started
//
//

// Don's changes:
//  (1) Added SEC_PCS_ALIGNBOTTOMLEFT
//  (2) Added GetSelectedDateValue
//  (3) Removed & from date/time passed into SelectDate

#ifndef __SEC_CALENDAR_H__
#define __SEC_CALENDAR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SEC_CALTITLE_H__
#include "toolkit\CalTitle.h"
#endif //__SEC_CALTITLE_H__

#ifndef __SEC_DAYBAR_H__
#include "toolkit\DayBar.h"
#endif //__SEC_DAYBAR_H__

#ifndef __SEC_DAYBOX_H__
#include "toolkit\DayBox.h"
#endif //__SEC_DAYBOX_H__

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

#define SEC_CAL_SCHEMA					0x0100

class SECCalendar;


// Behavior options. High 16 bits describe overall calendar modes,
// low 16 bits describe component behavior modes.
#define SECBM_AUTOSIZE_FONT				0x00010000L
#define SECBM_AUTOSIZE_RECT				0x00020000L 
#define SECBM_SINGLE_DATE				0x00040000L
#define SECBM_MONTH_BUTTONS				0x00080000L
#define SECBM_YEAR_BUTTONS				0x00100000L
#define SECBM_KEYBOARD_CONTROL			0x00200000L
#define SECBM_MONTH_FROZEN				0x00400000L
#define SECBM_AUTO_HIDE					SECPBM_AUTOCLOSEABLE
#define SECBM_LBUTTONTRACK				0x01000000L

// Not yet implemented.
#define SECBM_RESIZEABLE				0x00800000L

// Draw options. High 16 bits describe overall calendar modes,
// low 16 bits describe component behavior modes.
#define SECDM_FULL_MONTH_NAMES			0x10000000L
#define SECDM_FULL_DAY_NAMES			0x20000000L
  
// Styles for SECPopupCalendar
#define SEC_PCS_ALIGNBOTTOMLEFT			0x0001		// Align calendar at bottom
													// left of supplied rect.

// Calendar-control-to-parent messages.
#define SEC_DATESELECTED				(WM_USER+0x0200)
#define SEC_HIGHLIGHTLISTCHANGED		(WM_USER+0x0201)
#define SEC_OK							(WM_USER+0x0202)
#define SEC_CANCEL						(WM_USER+0x0203)

// Defaults
#define DEFAULT_DAY_BKGD_COLOR			COLOR_WINDOW	
#define DEFAULT_DAY_PRE_POST_BKGD_COLOR	COLOR_BTNSHADOW			
#define DEFAULT_DAYBAR_BKGD_COLOR		COLOR_BTNFACE
#define DEFAULT_DAYBAR_FGD_COLOR		COLOR_BTNTEXT
#define DEFAULT_TITLE_BKGD_COLOR		COLOR_ACTIVECAPTION
#define DEFAULT_TITLE_FGD_COLOR			COLOR_CAPTIONTEXT
#define DEFAULT_LINES_COLOR				COLOR_BTNTEXT
#define DEFAULT_SELECTED_COLOR			COLOR_BKGD
#define DEFAULT_HIGHLIGHTED_COLOR		COLOR_HIGHLIGHT
#define DEFAULT_TEXT_COLOR				COLOR_WINDOWTEXT
#define DEFAULT_GRAYTEXT_COLOR			COLOR_GRAYTEXT

#define SECBM_DEFAULT_VIEW_BEHAVIOR \
			SECPBM_DEFAULT_VIEW_BEHAVIOR | SECBM_AUTOSIZE_FONT | \
			SECBM_YEAR_BUTTONS |SECBM_MONTH_BUTTONS | SECBM_KEYBOARD_CONTROL 

#define SECBM_DEFAULT_DIALOG_BEHAVIOR SECPBM_DEFAULT_DIALOG_BEHAVIOR | \
		SECBM_AUTOSIZE_FONT | SECBM_YEAR_BUTTONS | SECBM_MONTH_BUTTONS | \
		SECBM_KEYBOARD_CONTROL 
									  
#define SECBM_DEFAULT_POPUP_BEHAVIOR SECPBM_DEFAULT_DIALOG_BEHAVIOR | \
			SECBM_AUTOSIZE_FONT | SECBM_YEAR_BUTTONS | SECBM_MONTH_BUTTONS | \
			SECBM_KEYBOARD_CONTROL | SECBM_AUTO_HIDE

#define SECDM_DEFAULT_VIEW_DRAW SECPDM_DEFAULT_VIEW_DRAW | \
			SECDM_FULL_MONTH_NAMES | SECDM_FULL_DAY_NAMES
#define SECDM_DEFAULT_DIALOG_DRAW SECPDM_DEFAULT_DIALOG_DRAW | \
			SECDM_FULL_MONTH_NAMES
#define SECDM_DEFAULT_POPUP_DRAW SECPDM_DEFAULT_POPUP_DRAW

#define SECDM_DEFAULT_VIEW_PPDRAW SECPDM_DEFAULT_VIEW_PPDRAW
#define SECDM_DEFAULT_DIALOG_PPDRAW SECPDM_DEFAULT_DIALOG_PPDRAW
#define SECDM_DEFAULT_POPUP_PPDRAW SECPDM_DEFAULT_POPUP_PPDRAW



#define SECC_NUMDAYS					42				/* 7 days x 6 wks. */
#define SECC_DEFAULT_FONT				_T("Arial")

// Geometry of the Calendar
#define VD_TOP							0.000
#define VD_TITLE						0.175
#define	VD_WK_0							0.250
#define VD_WK_ARRAY						{ 0.250,0.375,0.500,0.625,\
												0.750,0.875,1.000}

#define HD_LEFT							0.000
#define HD_TITLE						1.000
#define HD_DAYBAR						1.000
#define HD_DAY_ARRAY					{0.000, 0.143, 0.286, 0.429,\
										 0.572, 0.715, 0.858, 1.000}

/////////////////////////////////////////////////////////////////////////////
// SECCalendar:
// See Calendar.cpp for the implementation of this class
//

/////////////////////////////
// AutoDuck tag block for SECCalendar 


// adinsert AutoDuck insertion point for SECCalendar 
//@doc SECCalendar 
//@class SECCalendar implements a standard calendar control which can be 
// used for date entry, as well as date representation.  Through the use of 
// member functions, days on the calendar can be highlighted and selected.  
// An SECCalendar instance can be embedded into a dialog, or can be implemented 
// as a popup control through the derived class <c SECPopupCalendar>.  SECCalendar 
// can be embedded into a CView as well as a CDialog.  Since it is derived from 
// CWnd, it can usually be used anywhere a Cwnd can.
// 
//@comm SECCalendar maintains an internal page date as well as a selected date.  The 
// page date corresponds to the day currently being viewed.  So, if the page date 
// was specified as May 13th 1993, the month of May would be shown with the current 
// date.  If the user advanced the calendar by a month, the current page date would 
// also be advanced internally by one month to June 13th 1993.  The only feedback 
// the end user receives is that the month of the current page date is shown in the 
// calendar window.
//
// SECCalendar also maintains a single selected date internally which can be manipulated 
// even if it is not currently being displayed in the current page.  In other words, 
// the current page might be June 1993, while the selected date might be all the way 
// back on May 13th 1975.  The selected date can be advanced through member functions 
// behind the scenes while the current page remains at June 1993.
//
// The overall behavior of an SECCalendar instance is controlled through a number of 
// behavior and drawing options.  The calendar month being drawn consists of carryover 
// days from the previous month as well as days from the next month.  For example, 
// if the first day of the displayed month fell on a Tuesday, that would mean that two 
// carryover days from the previous month would be displayed for Sunday and Monday.  
// Similarly, days for the next month are displayed if space permits.  Behavior for 
// all displayed days is modified at creation time, or dynamically using the following 
// member functions: <mf SECCalendar::SetPrePostDrawMode> <mf SECCalendar::GetPrePostDrawMode>
//  <mf SECCalendar::SetDrawMode>  <mf SECCalendar::GetDrawMode>  <mf SECCalendar::SetBehaviorMode>
//  <mf SECCalendar::GetBehaviorMode>
//
// The CALTEST sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\CALENDAR directory demonstrates how 
// SECCalendar and <c SECPopupCalendar> are used.
//@base public | CWnd
class SECCalendar : public CWnd
{
	DECLARE_SERIAL( SECCalendar ) 


	//@access Creation/Initialization

public:
	
	//@cmember
	/* Creates a SECCalendar object.*/
	SECCalendar( );
	
	//@cmember
	/* Creates a child window and attaches it to the parent window passed in.*/
	virtual BOOL Create(	DWORD dwStyle,
							CRect &Rect, 
							CWnd *pWndParent, 
							UINT nID,
							CCreateContext* pContext = NULL );

	//@access Attributes

public:

	//@cmember
	/* Retrieves the current drawing mode of overlapping days.*/
	long GetPrePostDrawMode( void );
	
	//@cmember
	/* Sets the drawing mode of overlapping days.*/
	BOOL SetPrePostDrawMode( long l );
	
	//@cmember
	/* Retrieves the behavior mode of the calendar.*/
	long GetBehaviorMode( void );
	
	//@cmember
	/* Sets the behavior mode of the calendar.*/
	BOOL SetBehaviorMode( long m );
	
	//@cmember
	/* Gets the drawing mode of the calendar.*/
	long GetDrawMode( void );

	//@cmember
	/* Sets the drawing mode of the calendar.*/
	BOOL FAR SetDrawMode( long m );
	
#ifdef WIN32

	//@cmember
	/* Sets the calendar page to be displayed (32-bit only).*/
	BOOL SetPage( const COleDateTime &calendarPage, BOOL bRedraw = FALSE );

#endif

#ifdef WIN32	

	//@cmember
	/* Retrieves the currently selected date (32-bit only)*/
	const COleDateTime& GetSelectedDateValue( void );
	
#else

	//@cmember
	/* Retrieves the currently selected date (16-bit only)*/
	const CTime& GetSelectedDateValue( void );

#endif

#ifdef WIN32

	//@cmember
	/* Retrieves an array of pointers to all highlighted dates. (32-bit only)*/
	UINT GetAllHighlightedDates( COleDateTime *pDates, UINT nDates );

#endif 

	//@cmember
	/* Retrieves an array of pointers to all highlighted dates.*/
	UINT GetAllHighlightedDates( CTime *pDates, UINT nDates );
	
	//@cmember
	/* Sets the calendar page to be displayed.*/
	BOOL SetPage( const CTime &calendarPage, BOOL bRedraw = FALSE );
		
	//@cmember
	/* Retrieves the currently selected date.*/
	SECDayBox* GetSelectedDate( void );
	
	//@cmember
	/* Retrieves the number of highlighted dates.*/
	UINT GetNumHighlightedDates( void );
	
	//@cmember
	/* Retrieves the status of the automatic size font mode.*/
	BOOL GetAutosizeFont( void );
	
	//@cmember
	/* Sets the status of the automatic size font mode.*/
	void SetAutosizeFont( BOOL bSet );
	
	//@cmember
	/* Retrieves the status of the automatic sizing CRect mode.*/
	BOOL GetAutosizeRect( void );
	
	//@cmember
	/* Sets the status of the automatic sizing CRect mode.*/
	void SetAutosizeRect( BOOL bSet );
	
	//@cmember
	/* Retrieves the CRect associated with the control.*/
	const CRect& GetControlRect( void );
	
	//@cmember
	/* Sets a new CRect for the control.*/
	CRect SetControlRect( CRect &newRect );
	
	//@cmember
	/* Returns the status of single date selection mode.*/
	BOOL GetSingleDate( void );
	
	//@cmember
	/* Sets the status of single date selection mode.*/
	BOOL SetSingleDate( BOOL bTurnOn );
	
	//@cmember
	/* Retrieves the active status of the month buttons.*/
	BOOL GetMonthButtons( void );
	
	//@cmember
	/* Sets the active status of the month buttons.*/
	BOOL SetMonthButtons( BOOL bTurnOn );
	
	//@cmember
	/* Retrieves the active status of the year buttons.*/
	BOOL GetYearButtons( void );
	
	//@cmember
	/* Sets the active status of the year buttons.*/
	BOOL SetYearButtons( BOOL bTurnOn );
	
	//@cmember
	/* Retrieves the active status of keyboard control.*/
	BOOL GetKeyboardControl( void );
	
	//@cmember
	/* Sets the active status of keyboard control.*/
	BOOL SetKeyboardControl( BOOL bTurnOn );
	
	//@cmember
	/* Returns nonzero (true) if the month currently displayed is frozen (can’t be changed).*/
	BOOL GetMonthFrozen( void );
	
	//@cmember
	/* Freezes the month currently being displayed by the calendar control (the month can’t be changed).*/
	BOOL SetMonthFrozen( BOOL bMakeFrozen );
	
	//@cmember
	/* Retrieves the active status of whether the calendar control is automatically hidden.*/
	BOOL GetAutoHide( void );
	
	//@cmember
	/* Sets the active status indicating that the calendar control is automatically hidden.*/
	BOOL SetAutoHide( BOOL bHideAutomatically );
	

	// Retrieves the alignment flag.
	UINT GetAlignmentFlag( void );
	
	// Sets the alignment flag.
	UINT SetAlignmentFlag( UINT lNewFlag );
	
	//
	CWnd *GetNotifyWnd( void );
	
	//
	void SetNotifyWnd( CWnd *pWnd );
	
	//@cmember
	/* Retrieves the color of the calendar text.*/
	COLORREF GetTextColor( void );
	
	//@cmember
	/* Sets the color of the calendar text.*/
	COLORREF SetTextColor( COLORREF newColor );
	
	//@cmember
	/* Retrieves the first day of the week.*/
	int GetFirstDayOfWeek() const;
	
	//@cmember
	/* Sets the first day of the week.*/
	void SetFirstDayOfWeek(int nNewFirstDay, BOOL bRedraw = FALSE);
	

public:
	//@access Operations

	void Serialize( CArchive &ar );

	//@cmember
	/* Advances the calendar page date by one day.*/
	virtual BOOL AdvanceDay( BOOL bSelectionFollows = TRUE );

	//@cmember
	/* Decrements the day being viewed by one.*/
	virtual BOOL RetreatDay(  BOOL bSelectionFollows = TRUE );

	//@cmember
	/* Advances the page being viewed by one week.*/
	virtual BOOL AdvanceWeek( BOOL bSelectionFollows = TRUE );

	//@cmember
	/* Decrements the day being viewed by one week.*/
	virtual BOOL RetreatWeek(  BOOL bSelectionFollows = TRUE );

	//@cmember
	/* Advances the page being viewed by one month.*/
	virtual BOOL AdvanceMonth( BOOL bSelectionFollows = FALSE );

	//@cmember
	/* Decrements the day being viewed by one month.*/
	virtual BOOL RetreatMonth(  BOOL bSelectionFollows = FALSE );

	//@cmember
	/* Advances the page being viewed by one year.*/
	virtual BOOL AdvanceYear(  BOOL bSelectionFollows = FALSE );

	//@cmember
	/* Decrements the day being viewed by one year.*/
	virtual BOOL RetreatYear(  BOOL bSelectionFollows = FALSE );

#ifdef WIN32

	// 
	virtual BOOL DeltaSelection( COleDateTimeSpan &ts );

	//
	virtual BOOL DeltaPage( COleDateTimeSpan &ts );

	//@cmember
	/* Sets the highlighted state for the date passed in.*/
	virtual	BOOL HighlightDate( COleDateTime &date, BOOL bSet );

	//@cmember
	/* Sets the selection state of the date passed in.*/
	virtual	BOOL SelectDate( COleDateTime date, BOOL bSet );

	//@cmember
	/* Toggles the selection status of the date passed in.*/
	virtual	BOOL ToggleSelectDate( COleDateTime &date );

	//@cmember
	/* Toggles the highlighted status of the date passed in. */
	virtual	BOOL ToggleHighlightDate( COleDateTime &date );

	//@cmember
	/* Sets the highlight color for the date passed in.*/
	virtual COLORREF SetHighlightColor( COleDateTime &date, COLORREF newColor );
#endif

	//
	virtual BOOL DeltaSelection( CTimeSpan &ts );

	//
	virtual BOOL DeltaPage( CTimeSpan &ts );

	//@cmember
	/* Sets the highlighted state for the date passed in.*/
	virtual	BOOL HighlightDate( CTime &date, BOOL bSet );

	//@cmember
	/* Sets the selection state of the date passed in.*/
	virtual	BOOL SelectDate( CTime date, BOOL bSet );

	//@cmember
	/* Toggles the selection status of the date passed in.*/
	virtual	BOOL ToggleSelectDate( CTime &date );

	//@cmember
	/* Toggles the highlighted status of the date passed in. */
	virtual	BOOL ToggleHighlightDate( CTime &date );

	//@cmember
	/* Sets the highlight color for the date passed in.*/
	virtual COLORREF SetHighlightColor( CTime &date, COLORREF newColor );
	
protected:

	void PurgeDateList( void );

	//@access Overridable
	
	
	virtual BOOL IsDisplayableDate( int year, int month, int day );


public:

	//@cmember
	/* Initializes the colors for the calendar.*/
	virtual void InitColors();

	// access Overrides

	//{{AFX_MSG(SECCalendar)
	// Called when the calendar windows makes a request to repaint.
	afx_msg void OnPaint();
	
	// Called when the user clicks the OK button (the button with an ID of IDOK).
	afx_msg LONG OnOK(UINT u, long l);

	// Called every time the mouse is moved.
	afx_msg UINT OnNcHitTest(CPoint point);
	// Calls when the user clicks the Cancel button the ESC key dialog box. 
	afx_msg LONG OnCancel(UINT u, long l);
	
	// Called when a nonsystem key is pressed.
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	// Called when the user double-clicks the left mouse button. 
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	// Called when the user presses the left mouse button.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	// Called when the user releases teh left mouse button.
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	// Called when the mouse cursor moves. 
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	// Called to inform CWnd to cancel any internal mode. 
	afx_msg void OnCancelMode();
	
	// Called when the user presses the right mouse button.
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNextYear(); 
	
	afx_msg void OnPrevYear(); 
	
	afx_msg void OnNextMonth();
	
	afx_msg void OnPrevMonth();
	
	// Called when a change is made in the system color setting. 
	afx_msg void OnSysColorChange();	
	//}}AFX_MSG

// Implementation
public:

	virtual	~SECCalendar( );


protected:

	void TextCenteredInRect( CDC *pDC, 
							 CRect theRect, 
							 COLORREF clrFgnd, 
							 COLORREF clrBkgd,
							 CString& strText);


	SECDayBar m_pnlDayBar;
	SECCalTitle m_pnlTitle;

#ifdef WIN32
	virtual void NormalizeDate( COleDateTime &date );
	BOOL DateFoundInList( COleDateTime date );
	BOOL RemoveDateFromList( COleDateTime date );
	BOOL AddDateToList( COleDateTime date );
#else
	virtual void NormalizeDate( CTime &date );
	BOOL DateFoundInList( CTime date );
	BOOL RemoveDateFromList( CTime date );
	BOOL AddDateToList( CTime date );
#endif
	virtual void OnDatePegged( void );
	UINT m_nBevelLines;
	virtual UINT GetClassStyle( void );
	CWnd *m_pNotifyWnd;
	void AdjustFontSizes( CDC *pDC = NULL );
	void AdjustRectSizes( CRect &r, CDC *pDC = NULL );
	CWnd *m_pParent;
	void SizeTitle( void );
	void SizeDayBar( void );
	void SizeDates( void );
	virtual void InitFonts( void );
	virtual void SetUpDateBoxes( void );
	void PaintDates( CDC *pDC );
	enum deltaDate { negYear, negMonth, negDay, Day, Month, Year };

protected:
	BOOL m_bCreated;
#ifdef _DEBUG
	void DumpDateList( void );
#endif

#ifdef WIN32
	COleDateTime m_page;
	COleDateTime m_selection;
	COleDateTime *m_pSelection;
	COleDateTime m_phantomSelection;
#else
	CTime        m_page;
	CTime        m_selection;
	CTime        *m_pSelection;
	CTime        m_phantomSelection;
#endif

	SECDayBox m_day[SECC_NUMDAYS];
	SECDayBox *m_pSelectedDay;

	UINT	m_nOpMode;
	UINT	m_nAlignment;

	BOOL	m_bFullMonthNames;
	BOOL	m_bFullDayNames;

	BOOL	m_bCapture;
	BOOL    m_bDaySaved;
#ifdef WIN32
	COleDateTime m_SaveSelectedDay;
#else
	CTime        m_SaveSelectedDay;
#endif

	CObList m_HltDateList;

	LOGFONT	m_lfTitleFont;
	LOGFONT	m_lfDayFont;
	LOGFONT	m_lfDateFont;
	CFont   m_TitleFont;
	CFont	m_DayFont;
	CFont   m_DateFont;

public:
	CFont* GetTitleFont() { return &m_TitleFont; }
	CFont* GetDayBarFont() { return &m_DayFont; }
	CFont* GetDateFont() { return &m_DateFont; }

protected:
	COLORREF m_clrText;
	COLORREF m_clrLines;
	COLORREF m_clrDayBarText;
	COLORREF m_clrDayBarFace;
	COLORREF m_clrTitleText;
	COLORREF m_clrTitleFace;
	COLORREF m_clrGrayText;

	long m_iPrePostDrawMode;
	long m_iDrawMode;
	long m_iPrePostBehaMode;
	long m_iBehaMode;

	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// SECPopupCalendar window

/////////////////////////////
// AutoDuck tag block block for SECPopupCalendar 

//@doc SECPopupCalendar
//@mdata BOOL  | SECPopupCalendar | m_bHideDoesOK | Determines if calculator 
// send IDOK or IDCANCEL when hidden.
//@xref <c SECPopupCalendar> <mf SECPopupCalendar::SetHideMode>

// adinsert AutoDuck insertion point for SECPopupCalendar 
//@doc SECPopupCalendar 
//@class The SECPopupCalendar provides the function of the <c SECCalendar> class
// in a popup window by extending this <c SECCalendar> class.
//@base public | SECCalendar.
//@xref <c SECCalendar>
class SECPopupCalendar : public SECCalendar
{

	DECLARE_SERIAL(SECPopupCalendar);
	
	//@access Creation/Initialization

public:

	//@cmember
	/* Creates a SECPopupCalendar object*/
	SECPopupCalendar( );

	//@cmember
	/* Creates a popup calendar window.*/
	virtual BOOL Create(DWORD dwStyle, 
						CRect &buttonRect, 
						CWnd *pParentWnd, 
						CCreateContext *pContext = NULL);

	//@access Attributes
public:
	// IDOK or IDCANCEL
	//@cmember
	/* Selects the hiding behavior of the popup window.*/
	long SetHideMode( long okOrCancel );

	// Retrieves the class style for the popup window.
	virtual UINT GetClassStyle( void );

	//@access Operations

public:
	// Serializes this calendar through a CArchive object
	void Serialize( CArchive &ar );


	//@access Overrides
public:

	//{{AFX_MSG(SECPopupCalendar)
	//@cmember
	/* Called when a CWnd object is being activated or deactivated. */
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG

// Implementation
public:
	~SECPopupCalendar( );

protected:
	//@cmember
	/* Determines if calculator send IDOK or IDCANCEL when hidden.*/
	BOOL m_bHideDoesOK;

	DECLARE_MESSAGE_MAP()
};


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

/////////////////////////////////////////////////////////////////////////////
#endif // __SEC_CALENDAR_H__   

