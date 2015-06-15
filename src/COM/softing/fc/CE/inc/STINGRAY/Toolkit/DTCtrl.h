/////////////////////////////////////////////////////////////////////////////
// DTCtrl.h : Header file for SECDateTimeCtrl. A date and/or time editting
//            control.
//
// Stingray Software Extension Classes
// Copyright (C) 1996-7 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//		Author:		Don			20/01/97
//

#ifndef __SECDATET_H__
#define __SECDATET_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#ifndef __DTGADGET_H__
#include "toolkit\DTGadget.h"
#endif

#ifndef __AFXDISP_H__
#include "afxdisp.h"
#endif

#ifndef __SEC_CALENDAR_H__
#include "toolkit\calendar.h"
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


// SECDateTimeCtrl styles
#define SEC_DTS_CALENDAR		0x0001L
#define SEC_DTS_UPDOWN			0x0002L
#define SEC_DTS_Y2K_NOFIX		0x0010L		// overrides Y2K fix (backward compatibility)
#define SEC_DTS_Y2K_CONTEXT		0x0020L     // base Y2K fix on current date in control

// SECDateTimeCtrl notifications
//
//  Note we support the following edit control notifications:
//		EN_CHANGE, EN_KILLFOCUS, EN_SETFOCUS.
//  All others are unsupported.
//
#define SECDTN_CHANGED		EN_CHANGE
#define SECDTN_KILLFOCUS	EN_KILLFOCUS
#define SECDTN_SETFOCUS		EN_SETFOCUS

// SECDateTimeCtrl Gadget IDs (bit settings)
#define SECDTID_12HOUR		0x0001
#define SECDTID_24HOUR		0x0002
#define SECDTID_MINUTE		0x0004
#define SECDTID_SECOND		0x0008
#define SECDTID_AMPM		0x0010
#define SECDTID_DAYNO		0x0020
#define SECDTID_DAYNAME		0x0040
#define SECDTID_MONTHNO		0x0080
#define SECDTID_MONTHNAME	0x0100
#define SECDTID_YEAR2		0x0200
#define SECDTID_YEAR4		0x0400
#define SECDTID_CALDROPDOWN	0x4000
#define SECDTID_SPINNER		0x8000
#define SECDTID_ALL			0xffff

/////////////////////////////////////////////////////////////////////////////
// SECDateTimeCtrl window
//
/////////////////////////////
// AutoDuck tag block block for SECDateTimeCtrl 

//@doc SECDateTimeCtrl
//@mfunc void  | SECDateTimeCtrl | SetTabMode | Sets the tab behaviour.
//@comm Sets the tab behaviour, TRUE means tab will move between fields, FALSE
// means move to next control. The default is FALSE, unless fast entry is
// enabled when this is changed to TRUE.  
//@parm BOOL | bEnable | If TRUE, enables tab mode; else, disables tab mode.
//@xref <c SECDateTimeCtrl>

//@doc SECDateTimeCtrl
//@mfunc void  | SECDateTimeCtrl | DisableWMCtlColor | Disables sending of the 
// WM_CTLCOLOR messages.
//@parmopt BOOL | bDisable | TRUE |  If TRUE, disables sending of WM_CTLCOLOR messages.
//@xref <c SECDateTimeCtrl>

//@doc SECDateTimeCtrl
//@mfunc void  | SECDateTimeCtrl | SetBkColor | Sets the background color of the control.
//@parm COLORREF  | cNewColor | parm comment 
//@xref <c SECDateTimeCtrl>

//@doc SECDateTimeCtrl
//@mfunc TCHAR  | SECDateTimeCtrl | GetNullChar | Retrieves the current null mode character. 
//@xref <c SECDateTimeCtrl>

//@doc SECDateTimeCtrl
//@mfunc BOOL  | SECDateTimeCtrl | IsHourPM | Retrieves whether the current hour is PM. 
//@xref <c SECDateTimeCtrl>

// adinsert AutoDuck insertion point for SECDateTimeCtrl 
//@doc SECDateTimeCtrl 
//@class SECDateTimeCtrl is a date/time editing control.  This class works 
// in conjunction with SECDTGadget, an abstract base class for the gadget 
// components used by SECDateTimeCtrl.  The several subclasses that are 
// based on SECDTGadget include:
//
//SECDTStaticGadget		Implements a non-editable text gadget.<nl>
//SECDTNumericGadget	Implements an editable numeric gadget.<nl>
//SECDTListGadget		Implements a text gadget with a defined list of possible strings.<nl>
//SECDTButtonGadget		Implements a simple push button gadget.<nl>
//SECDTSpinGadget		Implements a spin button gadget.<nl>
//
//@comm Before creating/attaching the date/time control, the member 
// function SetFormat must be called.
//
// See the DATETIME sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\DATETIME directory for a 
// demonstration of this class.
//
//@base public | CWnd
class SECDateTimeCtrl : public CWnd
{
	DECLARE_DYNAMIC(SECDateTimeCtrl)

	//@access Creation/Initialization
public:
	//@cmember
	/* Constructs a SECDateTimeCtrl object.*/
	SECDateTimeCtrl();

	//@cmember
	/* Creates the edit control and attaches it to the SECDateTimeCtrl object.*/
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
				CCreateContext* pContext = NULL);

	//@cmember
	/* Creates the edit control with extended style attributes and attaches it */
	// to the SECDateTimeCtrl object.
	BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, 
				  CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	//@cmember
	/* Attaches an SECDateTimeCtrl object to an edit control.*/
	BOOL AttachDateTimeCtrl( UINT uID, CWnd* pParent, DWORD dwDTStyles, 
		                     DWORD dwWinStyles = WS_CHILD|WS_TABSTOP, 
							 DWORD dwExStyle = WS_EX_CLIENTEDGE );
public:

	//@access Attributes

	// Control format type as used by Get/SetFormat.
	//@cmember,menum
	/* Format types */
	enum FormatType
	{
		Time,							//@@emem Locale time format
		ShortDate,						//@@emem Locale short date format 
		LongDate,						//@@emem Locale long date format
		Custom							//@@emem A user supplied date/time format string
	};									

	// The validation types as used by Get/SetMinMax
	//@cmember,menum
	/* Validation types*/
	enum ValidationMode
	{
		NoValidation,					//@@emem No validation is required
		TimeValidation,					//@@emem Validation of time fields only
		DateValidation,					//@@emem Validation of date fields only
		DateTimeValidation				//@@emem Validation of date & time fields
	};

	// Definitions of non-editable fields as used by Get/SetNoEdit
	//@cmember,menum
	/* Non-editable fields*/
	enum NoEdit
	{
		FixedYear   = 0x0001,			//@@emem The year is non-editable
		FixedMonth  = 0x0002,			//@@emem The month is non-editable
		FixedDay    = 0x0004,			//@@emem The day is non-editable
		FixedHour   = 0x0008,			//@@emem The hour is non-editable
		FixedMinute = 0x0010,			//@@emem The minute is non-editable
		FixedSecond = 0x0020,			//@@emem The second is non-editable
		FixedDate   = 0x0007,			//@@emem The date is non-editable
		FixedTime   = 0x0038			//@@emem The time is non-editable
	};

	// Set/Get the current validation mode and the ranges being validated to.
	//@cmember
	/* Sets the current validation mode and the ranges being validated.*/
	BOOL SetMinMax(const COleDateTime& minDateTime, 
				   const COleDateTime& maxDateTime, 
				   ValidationMode mode = DateTimeValidation);

	//@cmember
	/* Gets the current validation mode and the ranges being validated.*/
	ValidationMode GetMinMax(COleDateTime& minDateTime, 
							 COleDateTime& maxDateTime) const;

	// Set/Get the current date/time format
	//@cmember
	/* Sets the current date/time format.*/
	void SetFormat(FormatType format);

	//@cmember
	/* Sets the current date/time format.*/
	void SetFormat(LPCTSTR lpszFormat);

	//@cmember
	/* Gets the current date/time format.*/
	void GetFormat(FormatType& format) const;

	//@cmember
	/* Gets the current date/time format.*/
	void GetFormat(CString& format) const;

	//@cmember
	/* Sets the background color.*/
	void SetBkColor(COLORREF cNewColor) { m_cBkColor=cNewColor; }

	//@cmember
	/* Enables fast input in numeric fields.*/
	void EnableFastEntry(BOOL bEnable = TRUE);

	//@cmember
	/* Enables fast input in numeric fields.*/
	void EnableFastEntry(BOOL bEnable, BOOL bTabEnable);

	//@cmember
	/* Determines whether fast input is enabled.*/
	BOOL IsFastEntryEnabled() const;

	// With fast entry enabled, when a 
	// numeric field has received a valid input (it's maximum number of
	// digits), it advances input focus to the next field.

	//@cmember
	/* Sets the tab behaviour.*/
	void SetTabMode(BOOL bEnable);

	// Sets the tab behaviour, TRUE means tab will move between fields, FALSE
	// means move to next control. The default is FALSE, unless fast entry is
	// enabled when this is changed to TRUE.
	// Set/Determine the current modified state.

	//@cmember
	/* Sets the current modified state.*/
	void SetModified(BOOL bModified);

	//@cmember
	/* Sets the current modified state.*/
	void SetModified(BOOL bDate, BOOL bTime);

	//@cmember
	/* Returns the current modified state.*/
	BOOL IsModified() const;

	//@cmember
	/* Returns the current modified state.*/
	void IsModified(BOOL& bDate, BOOL& bTime) const;

	// Returns TRUE if the user is currently in the middle of editting one
	// of the numeric fields
	//@cmember
	/* Determines if one of the fields is being edited.*/
	BOOL IsEditing() const;

	// Set/Get the current date and time
	//@cmember
	/* Sets the current date and time in the control.*/
	BOOL SetDateTime(const COleDateTime& datetime);

	//@cmember
	/* Sets the current date in the control.*/
	BOOL SetDate(const COleDateTime& date);

	//@cmember
	/* Sets the current time in the control.*/
	BOOL SetTime(const COleDateTime& time);

	//@cmember
	/* Gets the current date and time in the control.*/
	const COleDateTime& GetDateTime() const;

	// Set/Get the edittable fields.
	//@cmember
	/* Gets the non-editable fields.*/
	int  GetNoEdit() const;
	
	//@cmember
	/* Sets the non-editable fields.*/
	void SetNoEdit(int nNoEdit);

	//@cmember
	/* Returns the current null mode character.*/
	TCHAR GetNullChar() const { return m_cNull; }

	// AM/PM indicator
	//@cmember
	/* Returns TRUE if current hour is PM.*/
	BOOL IsHourPM()		{ return m_bHourPM; }

public:

	//@access Operations

	//@cmember
	/* Forces the specified gadget to be redrawn.*/
	void InvalidateGadget(SECDTGadget* pGadget, BOOL bImmediateRedraw = FALSE);
	
	//@cmember
	/* Forces the specified gadget to be redrawn.*/
	void InvalidateGadget(int nIndex, BOOL bImmediateRedraw = FALSE);

	//@cmember
	/* Returns the index of the gadget at the given portion of the window.*/
	int  GadgetFromPoint(CPoint point);

	//@cmember
	/* Brings the specified gadget into the displayable portion of the window.*/
	void BringIntoView(SECDTGadget* pGadget);
	
	//@cmember
	/* Brings the specified gadget into the displayable portion of the window.*/
	void BringIntoView(int nIndex);

	//@cmember
	/* Forces the control to re-size to fit its content exactly.*/
	void SizeToContent();

	//@cmember
	/* Validates the given date/time to the current ranges.*/
	BOOL Validate(const COleDateTime& datetime) const;

	//@cmember
	/* Sets control to null date entry mode.*/
	void SetNull(TCHAR cNull = _T('_'));
	
	//@cmember
	/* Disables sending of the WM_CTLCOLOR messages*/
	void DisableWMCtlColor(BOOL bDisable = TRUE);

	//@access Overrides

	// Function called just before a commit of a new date/time is applied.
	// Return FALSE to abandon the commit.
	//@cmember
	/* Called just before a commit of a new date/time is applied.*/
	virtual BOOL OnChanging(const COleDateTime& newDateTime);

	// Function called after the date/time has been modified. The default
	// implementation passes a SECDTN_CHANGED notification to the parent.
	//@cmember
	/* Called after the date/time has been modified.*/
	virtual void OnChanged();

// Implementation
public:
	virtual ~SECDateTimeCtrl();

protected:
	// Function responsible for parsing the date/time format string and 
	// building the m_gadgets component list.
	virtual void ParseDateTimeFormat();

	// Functions used by ParseDateTimeFormat for parsing individual components
	// of the date/time string
	void ParseHours(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseMinutes(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseSeconds(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseAmPm(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseDay(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseMonth(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseYear(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseEra(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	void ParseQuotedText(LPCTSTR& lpszStart, LPCTSTR& lpsz, UINT uParam);
	BOOL CheckLeadZero(LPCTSTR& lpsz, TCHAR c) const;
	void FlushStatic(LPCTSTR lpszStart, LPCTSTR lpszEnd);

	// Functions used for changing the value of a speicific field 
	BOOL ChangeHour(UINT uNotifyCode, int nID, LPARAM lParam);
	BOOL ChangeMinute(UINT uNotifyCode, int nID, LPARAM lParam);
	BOOL ChangeSecond(UINT uNotifyCode, int nID, LPARAM lParam);
	BOOL ChangeAmPm(UINT uNotifyCode, int nID, LPARAM lParam);
	BOOL ChangeDay(UINT uNotifyCode, int nID, LPARAM lParam);
	BOOL ChangeMonth(UINT uNotifyCode, int nID, LPARAM lParam);
	BOOL ChangeYear(UINT uNotifyCode, int nID, LPARAM lParam);

	// Function used for converting a two-digit year to four-digit year
	virtual int GetCentury(int nTwoDigitYear);

	// Function used in GetCentury. Required because VC 5 does now have a replace member
	// in CString.
	TCHAR* ot_str_replace(const TCHAR* pszInput, const TCHAR* pszOld, const TCHAR* pszNew);

	// Functions for moving forward/backwards a gadget
	void AdvanceGadget();
	void RetreatGadget();

	// Function used to layout the gadgets within the controls client
	virtual void LayoutGadgets();

	// Function used for controlling the drawing of the gadgets.
	virtual void Draw(CDC& dc);

	// Handler for spinner events
	BOOL SpinnerEvent(UINT uNotifyCode, int nID, LPARAM lParam);

	// Handler for creating the popup calendar
	virtual BOOL PopupCalendar(UINT uNotifyCode, int nID, LPARAM lParam);

	// Override this to hook in a SECPopupCalendar derived object.
	virtual void OnCreatePopupCalendar() 	{ m_pPopupCalendar = new SECPopupCalendar; }

	// Functions for updating the specified gadgets with new values and
	// for forcing them to be redisplayed
	void UpdateGadgets(const COleDateTime& datetime);
	void UpdateGadgets(UINT nFlags = SECDTID_ALL);

	// Performs validation of a new date (and can correct certain
	// fields to make it valid)
	virtual BOOL Validate(COleDateTime& datetime, int nPos) const;

	// Handles notify events from gadgets
	virtual BOOL OnNotify(UINT uNotifyCode, int nID, LPARAM lParam);

	// Handles requests from gadgets to capture the mouse.
	void CaptureMouse(SECDTGadget* pGadget, BOOL bSet);

	// Functions for causing the gadget display to be re-built.
	virtual void Tidy();
	void TidyEx();
	void ResetGadgets();
	void ResetCurrentGadget( int nCurGadget );
	BOOL CheckGadgetFocusAbility( int nGadget );
	int  GetFirstFocusGadget();

	// Returns fields which are present (used for NULL dates)
	WORD GetNullFields() const;

	// Checks whether we can leave a NULL edit state.
	void CheckNull(BOOL bUpdate = TRUE);

	// modify day/month format, if desired
	virtual void OnMonthNameInit(CString& strMonthName) { strMonthName; }
	virtual void OnDayNameInit(CString& strDayName) { strDayName; }

	// Sends the notification to our parent.
	void SendNotification(UINT nCode) const;

protected:
// Implementation - data members
	enum								// Internal state flags
	{
		DateModified = 0x0001,			// The date has been modified
		TimeModified = 0x0002,			// The time has been modified
		Editing      = 0x0004			// A numeric field is currently being
	};									// editted.

	int               m_nState;			// Holds state information (see flags)
	int               m_nNoEdit;		// Holds non-editable fields (see NoEdit)
	BOOL			  m_bFastInput;		// TRUE if fast input enabled
	BOOL              m_bTabMode;		// TRUE if tab is to move between fields
	BOOL              m_bCtlColor;		// TRUE if the sending of WM_CTLCOLOR messages is enabled
	COleDateTime      m_datetime;		// Current date/time
	COleDateTime      m_datetimeMin;	// Minimum date/time
	COleDateTime      m_datetimeMax;	// Maximum date/time
	ValidationMode    m_validMode;		// Validation mode (see ValidationMode)
	FormatType        m_formatType;		// Date/Time format type (see FormatType)
	CString           m_strCustomFormat;// Holds custom date/time format string
	HFONT             m_hFont;			// Font used by control
	int               m_nCurGadget;		// Current gadget (gadget with focus)
	int               m_nXOrigin;		// Current offset (scrolled percentage)
										// of window.
	int               m_nRightOffset;	// Amount of space on right reserved for
										// fixed gadgets
	int               m_nFixed;			// No. of fixed gadgets.
	SECDTGadget*      m_pCapture;		// Ptr to gadget which has mouse capture
	SECPopupCalendar* m_pPopupCalendar;	// Handle of popup calendar 
										// (normally NULL)
	TCHAR             m_cNull;			// Char. used to display NULL gadgets.
	BOOL			  m_bHourPM;		// AM/PM toggle
	COLORREF          m_cBkColor;		// Current background color
	COLORREF          m_cTextColor;		// Current text color

public:
	int               m_nBkDisabled;	// System background color when disabled
	int               m_nFgDisabled;	// System foreground color when disabled
	UINT              m_nCalBmp;		// ID of bmp to display on cal. drop down
										// button.
	CRuntimeClass*    m_pListGadClass;	// RTTI info for creating SECDTListGadget
	CRuntimeClass*    m_pNumGadClass;	// RTTI info for creating SECDTNumericGadget
	CRuntimeClass*    m_pStaticGadClass;// RTTI info for creating SECDTStaticGadget
	CRuntimeClass*    m_pButtonGadClass;// RTTI info for creating SECDTButtonGadget
	CRuntimeClass*    m_pSpinGadClass;	// RTTI info for creating SECDTSpinGadget

protected:
	// SECDTGadgetArray would normally be implemented as a template 
	// (CTypedPtrArray). Since templates are not implemented by some Unix
	// compilers, we copy what the template would have done.
	class SECDTGadgetArray : public CObArray
	{
	public:
		// Accessing elements
		SECDTGadget* GetAt(int nIndex) const
			{ return (SECDTGadget*)CObArray::GetAt(nIndex); }
		SECDTGadget*& ElementAt(int nIndex)
			{ return (SECDTGadget*&)CObArray::ElementAt(nIndex); }
		void SetAt(int nIndex, SECDTGadget* ptr)
			{ CObArray::SetAt(nIndex, ptr); }

		// Potentially growing the array
		void SetAtGrow(int nIndex, SECDTGadget* newElement)
		   { CObArray::SetAtGrow(nIndex, newElement); }
		int Add(SECDTGadget* newElement)
		   { return CObArray::Add(newElement); }
		int Append(const SECDTGadgetArray& src)
		   { return CObArray::Append(src); }
		void Copy(const SECDTGadgetArray& src)
			{ CObArray::Copy(src); }

		// Operations that move elements around
		void InsertAt(int nIndex, SECDTGadget* newElement, int nCount = 1)
			{ CObArray::InsertAt(nIndex, newElement, nCount); }
		void InsertAt(int nStartIndex, SECDTGadgetArray* pNewArray)
		   { CObArray::InsertAt(nStartIndex, pNewArray); }

		// overloaded operator helpers
		SECDTGadget* operator[](int nIndex) const
			{ return (SECDTGadget*)CObArray::operator[](nIndex); }
		SECDTGadget*& operator[](int nIndex)
			{ return (SECDTGadget*&)CObArray::operator[](nIndex); }
	};

	// Object maintaining status for null dates.
	class SECNull
	{
	public:
		SECNull();

		// Enables Null edit mode
		void Set();

		// Sets the currently displayed fields
		void SetActualFields(WORD wActualFields);

		// Aborts null edit mode.
		void Clear();
		
		// Returns the bit settings for fields yet to be completed
		WORD GetFields() const { return WORD(m_wNullFields & m_wActualFields); }

		// Returns TRUE if we are currently in NULL date mode
		BOOL IsSet() const     { return m_bSet; }

		// Functions for setting/getting the various date/time fields.
		// Note that the get functions return -1 if the fields has yet to be
		// entered.
		int  GetHour() const;
		void SetHour(int nHour);
		int  GetMinute() const;
		void SetMinute(int nMinute);
		int  GetSecond() const;
		void SetSecond(int nSecond);
		int  GetYear() const;
		void SetYear(int nYear);
		int  GetMonth() const;
		void SetMonth(int nMonth);
		int  GetDay() const;
		void SetDay(int nDay);
		int  GetDayOfWeek() const;
		void SetDayOfWeek(int nDayOfWeek);

		enum							// Bit settings for m_wNullFields
		{
			Hour      = 0x0001,			// Set if hour has yet to be entered
			Minute    = 0x0002,			// Set if minute has yet to be entered
			Second    = 0x0004,			// Set if second has yet to be entered
			Year      = 0x0008,			// Set if year has yet to be entered
			Month     = 0x0010,			// Set if month has yet to be entered
			Day       = 0x0020,			// Set if day has yet to be entered
			DayOfWeek = 0x0040			// Set if day of week has yet to be entered
		};

	protected:
		WORD m_wNullFields;				// Bit settings for incomplete fields
		WORD m_wActualFields;			// Bit settings for fields present
		BOOL m_bSet;					// TRUE if date is currently null
		int  m_nHour;					// Holds hour while in null state
		int  m_nMinute;					// Holds minute while in null state
		int  m_nSecond;					// Holds second while in null state
		int  m_nYear;					// Holds year while in null state
		int  m_nMonth;					// Holds month while in null state
		int  m_nDay;					// Holds day while in null state
		int  m_nDayOfWeek;				// Holds day of week while in null state
	} m_null;

	// m_gadgets is an array of all the gadget components.
	SECDTGadgetArray m_gadgets;

	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnCancelMode();
	afx_msg LONG OnSettingChange(UINT wParam, long lParam);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LONG OnSetFont(UINT wParam, long lParam);
	afx_msg void OnDestroy();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg LONG OnCalendarOK(UINT wParam, LONG lParam);
	afx_msg LONG OnCalendarCancel(UINT wParam, LONG lParam);
	afx_msg LONG OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGetTextLength(WPARAM wParam, LPARAM lParam);

	// A very dear friend of mine ...
	friend class SECDTGadget;
};


/////////////////////////////////////////////////////////////////////////////
// Function for converting a DATE (base type of COleDateTime) to a tm structure
extern BOOL SECTmFromOleDate(DATE dtSrc, struct tm& tmDest);


/////////////////////////////////////////////////////////////////////////////
// Helper function for setting the date/time of a COleDateTime object
extern BOOL SECSetOleDateTime(COleDateTime& dt, int nYear, int nMonth, 
							  int nDay, int nHour, int nMinute, int nSecond);

/////////////////////////////////////////////////////////////////////////////
// inline functions
inline int SECDateTimeCtrl::SECNull::GetHour() const
{ return ((m_wNullFields & m_wActualFields) & Hour)      ? -1 : m_nHour;      }

inline int  SECDateTimeCtrl::SECNull::GetMinute() const 
{ return ((m_wNullFields & m_wActualFields) & Minute)    ? -1 : m_nMinute;    }

inline int SECDateTimeCtrl::SECNull::GetSecond() const 
{ return ((m_wNullFields & m_wActualFields) & Second)    ? -1 : m_nSecond;    }

inline int SECDateTimeCtrl::SECNull::GetYear() const   
{ return ((m_wNullFields & m_wActualFields) & Year)      ? -1 : m_nYear;      }

inline int SECDateTimeCtrl::SECNull::GetMonth() const  
{ return ((m_wNullFields & m_wActualFields) & Month)     ? -1 : m_nMonth;     }

inline int SECDateTimeCtrl::SECNull::GetDay() const    
{ return ((m_wNullFields & m_wActualFields) & Day)	     ? -1 : m_nDay;       }

inline int SECDateTimeCtrl::SECNull::GetDayOfWeek() const
{ return ((m_wNullFields & m_wActualFields) & DayOfWeek) ? -1 : m_nDayOfWeek; }

inline void SECDateTimeCtrl::SetTabMode(BOOL bEnable)
{ m_bTabMode = bEnable; }

inline void SECDateTimeCtrl::DisableWMCtlColor(BOOL bDisable /* = TRUE */)
{ m_bCtlColor = !bDisable; }

inline void SECDateTimeCtrl::SendNotification(UINT nCode) const
{
// Not defined on CE platforms
#ifdef WS_EX_NOPARENTNOTIFY  
	if(GetExStyle() & WS_EX_NOPARENTNOTIFY)
		return;
#endif
	GetParent()->SendMessage(WM_COMMAND,
							 (WPARAM) MAKELONG(GetDlgCtrlID(), nCode),
							 (LPARAM) m_hWnd);
}

inline COLORREF SECDTGadget::GetBkColor() const
{ return m_pCtrl->m_cBkColor; }

inline COLORREF SECDTGadget::GetTextColor() const
{ return m_pCtrl->m_cTextColor; }

/////////////////////////////////////////////////////////////////////////////

#define ON_SECDTN_CHANGED(id, memberFxn) \
		ON_EN_CHANGE(id, memberFxn)

#define ON_SECDTN_SETFOCUS(id, memberFxn) \
		ON_EN_SETFOCUS(id, memberFxn)

#define ON_SECDTN_KILLFOCUS(id, memberFxn) \
		ON_EN_KILLFOCUS(id, memberFxn)

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif 	// WIN32

#endif


