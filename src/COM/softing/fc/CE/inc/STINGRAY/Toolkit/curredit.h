/////////////////////////////////////////////////////////////////////////////
// DropEdit.h : header file for SECDropEdit - subclassed edit with little
//			    drop down button.
//
// Stingray Software Extension Classes
// Copyright (C) 1996-1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//	Author:		John
//

#ifndef __CURREDIT_H__
#define __CURREDIT_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

// The SECCurrencyEdit provides an extensible class for entering
// and displaying custom-formatted currency data.  Input data parsing
// and output display formatting can be customized by descending new
// classes from SECCurrencyEdit::Format and SECCurrencyEdit.
//
// The SECCurrencyEdit supports some of the EM_* edit control message
// interface, and can thus be manipulated to some degree using a CEdit
// object (not recommended, but possible if required for compatibility
// with existing code).  The following lists the messages supported.
//
// Windows Messages		Edit Control Messages
// ----------------		---------------------
// WM_COPY				EM_CANUNDO
// WM_CUT				EM_EMPTYUNDOBUFFER
// WM_GETFONT			EM_GETSEL
// WM_PASTE				EM_SETREADONLY
// WM_SETFONT			EM_SETSEL
// WM_SETREDRAW			EM_UNDO
// WM_SETTEXT
// WM_UNDO


#ifndef __DROPEDIT_H__
#include "toolkit\dropedit.h"
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

// forward declaration;
class SECCurrencyEdit;

//SECCurrencyEdit DDX routines

//@func SECCurrencyEdit DDX routine.
//@rdesc void
//@parm CDataExchange* | pDX | Pointer to a CDataExchange object.
//@parm SECCurrencyEdit& | medit | Reference to a SECCurrencyEdit control.
//@parm double& | dValue | The value to set or retrieve.
void AFXAPI DDX_Currency(CDataExchange* pDX, SECCurrencyEdit& cedit, double& dValue);


/////////////////////////////
// AutoDuck tag block block for SECCurrencyEdit 

//@doc SECCurrencyEdit
//@mdata BOOL  | SECCurrencyEdit | m_bNegative | True if the data value is negative.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata BOOL  | SECCurrencyEdit | m_bCanUndo | True if the Undo buffer is not empty.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata BOOL  | SECCurrencyEdit | m_bParseOnSetText | True if control parses value 
// when WM_SETTEXT messages are received.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata BOOL  | SECCurrencyEdit | m_bRedraw | True if redraw is allowed.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata BOOL  | SECCurrencyEdit | m_bMouseSelect | True while mouse selecting.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata BOOL  | SECCurrencyEdit | m_bOverstrike | True if overstrike mode is enabled.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata UNDODATA  | SECCurrencyEdit | m_Undo | Undo buffer.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata double  | SECCurrencyEdit | m_dValue | Numeric value in the control.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata Format*  | SECCurrencyEdit | m_pFormat | Current format settings object.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata HFONT  | SECCurrencyEdit | m_hFont | Current font.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata int  | SECCurrencyEdit | m_iCaretPos | Caret character position.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata SELDATA  | SECCurrencyEdit | m_Sel | Selection information.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata UINT  | SECCurrencyEdit | m_uiRightMargin | The right margin in pixels.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mdata UINT  | SECCurrencyEdit | m_uiLeftMargin | The left margin in pixels.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc virtual void  | SECCurrencyEdit | OnClicked | Called when the button
// is pressed (to create the popup calculator).
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc DWORD  | SECCurrencyEdit | GetMargins | Retrieves the current margins.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc BOOL  | SECCurrencyEdit | IsNegative | Returns nonzero if the current
// value is negative.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc BOOL  | SECCurrencyEdit | HasSel | Returns nonzero if the control
// currently has selected text.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc BOOL  | SECCurrencyEdit | SetSelNone | Deselects text in the control.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc BOOL  | SECCurrencyEdit | SetSelAll | Selects all text in the control.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc BOOL  | SECCurrencyEdit | IsMouseSelecting | Returns nonzero if the 
// mouse is currently selecting text.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc BOOL  | SECCurrencyEdit | IsOverstrikeEnabled | Returns nonzero if 
// overstrike mode is currently enabled.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc void  | SECCurrencyEdit | DoInsert | Handles inserting of a character.
//@parm TCHAR  | c | Character to insert.
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc void  | SECCurrencyEdit | QueueRedraw | Redraws queued data. 
//@xref <c SECCurrencyEdit>

//@doc SECCurrencyEdit
//@mfunc DWORD  | SECCurrencyEdit | GetMargins | Returns the current margins.
//@xref <c SECCurrencyEdit>


// adinsert AutoDuck insertion point for SECCurrencyEdit 
//@doc SECCurrencyEdit 
//@class The SECCurrencyEdit provides an extensible class for entering and 
// displaying custom-formatted currency data.  Input data parsing and output 
// display formatting can be customized by descending new classes from 
// SECCurrencyEdit::Format and SECCurrencyEdit.
//
// The SECCurrencyEdit class supports some of the EM_* edit control message 
// interface, and it can thus be manipulated to some degree using a Cedit 
// object (not recommended, but possible if required for compatibility with 
// existing code).  Messages supported are listed below.
//
// See the CURRENCY sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\CURRENCY 
// directory for a demonstration of this class.
//
//@flag Windows Messages |
// WM_COPY, WM_CUT, WM_GETFONT, WM_PASTE, WM_SETFONT,
// WM_SETREDRAW, WM_SETTEXT, WM_UNDO, 
//@flag Edit Control Messages |
// EM_CANUNDO, EM_EMPTYUNDOBUFFER, EM_GETSEL, EM_SETREADONLY
// EM_SETSEL, EM_UNDO, EM_SETMARGINS
//
//@base public | SECDropEdit
class SECCurrencyEdit : public SECDropEdit //public CWnd
{
public:
	DECLARE_DYNAMIC(SECCurrencyEdit)

	// The following nested helper class provides the core currency
	// formatting and parsing methods.

	class Format
	{
	public:
		// Construction/destruction.  Note: if the default initialization
		// does not occur, the object will in a valid but decidedly 
		// American state.

		Format(BOOL bInitializeFromSystemDefaults = TRUE);
		virtual ~Format() {}

		// Property retrieval and setting methods for all the currency
		// display options.  Some important notes: 
		//
		// 1) Setting the thousand separator to the null character ('\0') 
		//    will prevent its use.
		// 2) Setting the decimal digits to negative one (-1) will use 
		//    as many digits are necessary to display the number.  If
		//    the number of digits is larger than required, the output
		//    will be padded with the padding character.
		//
		// Negative Format  Positive Format
		//
		// 0	($1)		0	$1 
		// 1	-$1			1	1$ 
		// 2	$-1			2	$ 1
		// 3	$1-			3	1 $
		// 4	(1$)
		// 5	-1$			These formats are taken directly from
		// 6	1-$			Microsoft's documentation regarding
		// 7	1$-			the international section of WIN.INI.
		// 8	-1 $		The SECCurrencyEdit uses them for the
		// 9	-$ 1		positive and negative formats given via
		// 10	1 $-		the formatting object.
		// 11	$ 1-
		// 12   $ -1		Rob (20 Aug 98):  Negative formats 10
		// 13   1- $        through 15 were updated to match those
		// 14   ($ 1)		of Windows NT 4.0.  Format indices 11 
		// 15   (1 $)       through 15 were added.  What was previously 
		//                  format index 10 "$ 1-" is now format index 11.

		BOOL IsLeadingZeroEnabled() const
			{ return m_bLeadingZero; }
		void EnableLeadingZero(BOOL b)
			{ m_bLeadingZero = b; }
		BOOL IsDecimalSeparatorLineEnabled() const
			{ return m_bDecimalSeparatorLine; }
		void EnableDecimalSeparatorLine(BOOL b)
			{ m_bDecimalSeparatorLine = b; }
		LPCTSTR GetMonetarySymbol() const
			{ return m_strMonetarySymbol; }
		void SetMonetarySymbol(LPCTSTR p)
			{ m_strMonetarySymbol = p; }
		LPCTSTR GetGrouping() const
			{ return m_strGrouping; }
		BOOL SetGrouping(LPCTSTR x);
		TCHAR GetThousandSeparator() const
			{ return m_cThousandSeparator; }
		void SetThousandSeparator(TCHAR c)
			{ m_cThousandSeparator = c; }
		TCHAR GetDecimalSeparator() const
			{ return m_cDecimalSeparator; }
		void SetDecimalSeparator(TCHAR c)
			{ m_cDecimalSeparator = c; }
		TCHAR GetPaddingCharacter() const
			{ return m_cPaddingCharacter; }
		BOOL SetPaddingCharacter(TCHAR c);
		BOOL GetBackgroundColor(COLORREF& cr) const;
		void SetBackgroundColor(COLORREF cr);
		BOOL GetNegativeColor(COLORREF& cr) const;
		void SetNegativeColor(COLORREF cr);
		BOOL GetPositiveColor(COLORREF& cr) const;
		void SetPositiveColor(COLORREF cr);
		COLORREF GetDecimalSeparatorLineColor() const
			{ return m_crSeparator; }
		void SetDecimalSeparatorLineColor(COLORREF cr)
			{ m_crSeparator = cr; }
		int GetPositiveFormat() const
			{ return m_iPositiveFormat; }
		void SetPositiveFormat(int i);
		int GetNegativeFormat() const
			{ return m_iNegativeFormat; }
		void SetNegativeFormat(int i);
		int GetDecimalDigits() const
			{ return m_iDecimalDigits; }
		void SetDecimalDigits(int i)
			{ m_iDecimalDigits = i; }
		int GetFractionalDigits() const
			{ return m_iFractionalDigits; }
		void SetFractionalDigits(int i)
			{ m_iFractionalDigits = i; }

		// Reads values as necessary from registry, INI file, etc. to
		// match settings specified by user in Control Panel.

		void InitializeFromSystemDefaults();

		// The following method clears any previously specified custom 
		// color use.

		void ClearCustomColors()
			{ m_iColorUseFlags = 0; }

		// The following methods convert between a numeric value and a
		// string representation.  If customization beyond that required
		// by the basic Format object is requied, derive your own class
		// and override the following methods; then pass your format
		// object to the SECCurrencyEdit in its constructor.

		virtual BOOL ParseValue(LPCTSTR pcsz, double& d, BOOL& bNegative) const;
		virtual void FormatValue(double dAbs, CString& str, BOOL bIsNegative) const;

		// If, for some pathological reason, the user needs to set the thousands
		// separator equivalent to the decimal separator, then he needs to derive
		// his own class and override these methods. Otherwise, it is not at all
		// likely that parsing will ever work consistently and accurately.

		virtual int FindDecimalSeparator(LPCTSTR pcszString, TCHAR cDecimal = '\0') const;
		virtual int FindFirstAndLastDigits(LPCTSTR pcszString, int& iFirst, int& iLast) const;

		// Operators - virtual to allow descendant handling.

		virtual Format& operator=(const Format& rhs);
		virtual BOOL operator==(const Format& rhs) const;
		virtual BOOL operator!=(const Format& rhs) const
			{ return !operator==(rhs); }

	private:
		BOOL m_bLeadingZero;			// leading 0 for numbers -1 < n < 1 ?
		BOOL m_bDecimalSeparatorLine;	// vertical line for decimal point?
		CString m_strMonetarySymbol;	// currency symbol (default: $)
		CString m_strGrouping;			// decimal digit grouping sizes
		TCHAR m_cThousandSeparator;		// groups decimal digits (default: ,)
		TCHAR m_cDecimalSeparator;		// decimal point (default: .)
		TCHAR m_cPaddingCharacter;		// used to pad to left of decimal
		COLORREF m_crBackground;		// background color of control window
		COLORREF m_crNegative;			// color of text when value is negative
		COLORREF m_crPositive;			// color of text when value is negative
		COLORREF m_crSeparator;			// color of decimal separator line
		int m_iColorUseFlags;			// internal color flags variable
		int m_iPositiveFormat;			// WIN.INI format for positive values
		int m_iNegativeFormat;			// WIN.INI format for negative values
		int m_iDecimalDigits;			// number of characters left of decimal
		int m_iFractionalDigits;		// number of digits to right of decimal
	};

	//@access Construction/destruction

	//@cmember
	/* Constructs a SECCurrencyEdit object.*/
	SECCurrencyEdit();

	//@cmember
	/* Constructs a  SECCurrencyEdit object.*/
	SECCurrencyEdit(Format* pFmt);
	
	virtual ~SECCurrencyEdit();

	//@cmember
	/* Replaces an existing edit control.*/
	BOOL Initialize(CWnd* pwndParent, UINT uiControlID);

public:

	//@access Attributes	

	//@cmember
	/* Gets the actual numeric value directly.*/
	void GetValue(double& d) const;

	//@cmember
	/* Sets the actual numeric value directly.*/
	void SetValue(double d, BOOL bRedraw = TRUE);

	//@cmember
	/* Gets the display formatting data.*/
	void GetFormat(Format& f);

	//@cmember
	/* Sets the display formatting data.*/
	void SetFormat(const Format& f);

	//@cmember,menum
	/* Alignments*/
	enum Align 
	{	
		left = 0,		//@@emem Left alignment
		center = 1,		//@@emem Center alignment
		right = 2		//@@emem Right alignment
	};

	//@cmember
	/* The alignment is actually maintained via edit control */
	// ES_* style values; this function is provided for convenience.
	Align GetAlignment() const;

	//@cmember
	/* The alignment is actually maintained via edit control */
	// ES_* style values; this function is provided for convenience.
	void SetAlignment(Align a);

	//@cmember
	/* The read-only feature is actually maintained via the normal */
	// edit control ES_READONLY style; this function is provided for convenience.
	BOOL IsReadOnly() const;

	//@cmember
	/* The read-only feature is actually maintained via the normal */
	// edit control ES_READONLY style; this function is provided for convenience.
	void SetReadOnly(BOOL b);

	//@cmember
	/* Sets the margins for the control.*/
	void SetMargins(UINT nLeft, UINT nRight);

	//@cmember
	/* Retrieves the margins for the control.*/
	DWORD GetMargins() const	
		{ return MAKELONG(m_uiLeftMargin, m_uiRightMargin); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECCurrencyEdit)
	//}}AFX_VIRTUAL

	//@access Overridable

public:
	
	// Determintes whether the given point (in screen or client
	// coordinates) is a hit on the button.
	//@cmember
	/* Performs hit testing for the button.*/
	virtual BOOL HitTestBtn(CPoint point, BOOL bClient = FALSE) const;

	//@cmember
	/* Pre-paste processing.*/
	virtual BOOL PrepareTextForPaste(CString& strText);
	
	//@cmember
	/* Creates the popup menu.*/
	virtual CMenu* CreatePopupMenu() const;
	
	//@cmember
	/* Deletes the popup menu.*/
	virtual void DeletePopupMenu(CMenu* p) const;
	
	//@cmember
	/* Processes backspace key press.*/
	virtual void DoBackspace();
	
	//@cmember
	/* Processes delete key press.*/
	virtual void DoDelete();
	
	//@cmember
	/* Processes insert key press.*/
	virtual void DoInsert(LPCTSTR pcsz);
	
	//@cmember
	/* Erases the control background.*/
	virtual void EraseBackground(CDC* pDC);
	
	//@cmember
	/* Stores data for undo operation.*/
	virtual void SaveDataForUndo(LPCTSTR p = NULL, int iCaret = -1);
	
	//@cmember
	/* Begins edit mode.*/
	virtual void StartEdit(int iCaretPos);
	
	//@cmember
	/* Ends edit mode.*/
	virtual void StopEdit();
	
	//@cmember
	/* Draws a vertical decimal separator on the control.*/
	virtual void DrawDecimalSeparatorLine(CDC& DC, int x);
	
	//@cmember
	/* Handles a minus sign key press.*/
	virtual void HandleNegativeSignKey();
	
	//@cmember
	/* Toggles overstrike mode state.*/
	virtual void ToggleOverstrikeMode();

protected:

	//@cmember
	/* Displays calculator.*/
	virtual BOOL ShowCalculator();
	
	// Override of the SECDropEdit for handling the button click.
	//@cmember
	/* Method called when the button has been clicked.*/
	virtual void OnClicked()
		{ ShowCalculator(); }

public:

	// Handlers provided by ClassWizard.

	//{{AFX_MSG(SECCurrencyEdit)
	
	// access Overrides

	// Called when Windows or an application makes a request to repaint.
	afx_msg void OnPaint();
	
	// Called when the user releases the right mouse button.
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	// Called after gaining the input focus.
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	
	// Called when the user presses the left mouse button.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	// Called immediately before losing the input focus. 
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	
	// Called when a nonsystem key is pressed.
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	// Override to control the particular types of input to process.
	afx_msg UINT OnGetDlgCode();
	
	// Called when a keystroke translates to a nonsystem character. 
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	// Called when a system key is depressed.
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	// Called when the user double-clicks the left mouse button. 
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	// Called when the user releases teh left mouse button.
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	// Called when the mouse cursor moves. 
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	//}}AFX_MSG

	// Edit control message handlers not provided by ClassWizard.

	// Called to process the EM_CANUNDO message.
	afx_msg LRESULT OnCanUndo(WPARAM wParam, LPARAM lParam);
	
	// Called to process the EM_EMPTYUNDOBUFFER message.
	afx_msg LRESULT OnEmptyUndoBuffer(WPARAM wParam, LPARAM lParam);

	/* Called when an application changes the enabled state of */
	// the CWnd object. 
	afx_msg void OnEnable(BOOL bEnable);

	// Called to process the EM_GETSEL message.
	afx_msg LRESULT OnGetSel(WPARAM wParam, LPARAM lParam);
	
	// Called to process the EM_SETREADONLY message.
	afx_msg LRESULT OnSetReadOnly(WPARAM wParam, LPARAM lParam);
	
	// Called to process the EM_SETSEL message.
	afx_msg LRESULT OnSetSel(WPARAM wParam, LPARAM lParam);
	
	// Called to process the EM_SETMARGINS
	afx_msg LRESULT OnSetMargins(WPARAM wParam, LPARAM lParam);

	// Standard message handlers not provided by ClassWizard.

	// Called to process the WM_GETFONT message.
	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_COPY message.
	afx_msg LRESULT OnCopy(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_CUT message.
	afx_msg LRESULT OnCut(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_PASTE message.
	afx_msg LRESULT OnPaste(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_SETFONT message.
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_SETREDRAW message.
	afx_msg LRESULT OnSetRedraw(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_SETTEXT message.
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	
	// Called to process the WM_UNDO message.
	afx_msg LRESULT OnUndo(WPARAM wParam, LPARAM lParam);

	// Command handlers for right mouse button popup menu.

	// Called to process the ID_EDIT_UNDO message.
	afx_msg void OnEditUndo();
	
	// Called to process the ID_EDIT_COPY message.
	afx_msg void OnEditCopy();
	
	// Called to process the ID_EDIT_CUT message.
	afx_msg void OnEditCut();
	
	// Called to process the ID_EDIT_PASTE message.
	afx_msg void OnEditPaste();
	
	// Called to process the ID_EDIT_CLEAR message.
	afx_msg void OnEditClear();
	
	// Called to process the ID_EDIT_SELECT_ALL message.
	afx_msg void OnEditSelectAll();

	

protected:
	// Non-virtual members; descendant classes may utilize these, but should 
	// not override them.

	//@access Implementation

	//@cmember
	/* Notify parent of control state change.*/
	void NotifyParent(UINT uiCode);

	//@cmember
	/* Determines if current value is negative.*/
	BOOL IsNegative() const
		{ return m_bNegative; }
	
	//@cmember
	/* Detemines if edit control has highlighted (selected) text.*/
	BOOL HasSel() const		
		{ return m_Sel.iStart != -1 && m_Sel.iStop != m_Sel.iStart; }
	
	//@cmember
	/* Sets the selected text.*/
	BOOL SetSel(int iStart, int iStop);
	
	//@cmember
	/* Deselects all text in the control.*/
	BOOL SetSelNone()		
		{ return SetSel(-1, 0); }
	
	//@cmember
	/* Selects all text in the control*/
	BOOL SetSelAll()		
		{ return SetSel(0, -1); }
	
	//@cmember
	/* Returns whether text is currently being selected with the mouse.*/
	BOOL IsMouseSelecting() const		
		{ return m_bMouseSelect; }
	
	//@cmember
	/* Returns whether the entire number is selected.*/
	BOOL IsEntireNumberSelected(BOOL* pbMore = NULL) const;
	
	//@cmember
	/* Returns whether overstrike mode is enabled.*/
	BOOL IsOverstrikeEnabled() const		
		{ return m_bOverstrike; }	
	
	//@cmember
	/* Returns the background color of the edit control. */
	COLORREF GetBackgroundColor() const;
	
	//@cmember
	/* Returns the text color for the edit control.*/
	COLORREF GetTextColor() const;
	
	//@cmember
	/* Points to the caret’s position. */
	int PointToCaretPos(const CPoint& pt);
	
	//@cmember
	/* Calculates the rectangle for text in the edit control.*/
	void CalculateTextRect(CRect& rc);
	
	//@cmember
	/* Makes an insertion in the edit control.*/
	void DoInsert(TCHAR c)		{ DoInsert(CString(c)); }
	
	//@cmember
	/* Gets text selected in the edit control.*/
	void GetSelectedText(CString& s) const;
	
	//@cmember
	/* Relocates the caret in the edit control.*/
	void MoveCaret(int iChar, BOOL bUpdateSel = FALSE);
	
	//@cmember
	/* Redraws the core currency formatting on the specified display device context.*/
	void PaintingCore(CDC& DC);
	
	//@cmember
	/* Redraws queued data.*/
	void QueueRedraw(BOOL bEraseBackground = TRUE)		
		{ if (m_bRedraw) InvalidateRect(NULL, bEraseBackground); }
	
	//@cmember
	/* Replaces the currently selected string with the specified text. */
	void ReplaceSelString(LPCTSTR pcszNew, CString& str) const;
	
	//@cmember
	/* Sets text that does not require parsing.*/
	void SetWindowTextNoParse(LPCTSTR p);
	
	//@cmember
	/* Positions the caret near the number.*/
	void MoveCaretToNumber();
	
	//@cmember
	/* Sets the value of the number in the control.*/
	void SetValue(double d, BOOL bNegative, BOOL bRedraw);

	// Data structure used for maintaining selection details.
	// Two special settings should be noted: 0, -1 means select 
	// all and -1, 0 means no selection.

	typedef struct tagSelData
	{
		int iStart, iStop;
		tagSelData() : iStart(-1), iStop(0) {}
		tagSelData(const tagSelData& s) :
			iStart(s.iStart), iStop(s.iStop) {}
		void Set(int i, int j) { iStart = i; iStop = j; }
		void Clear() { iStart = -1; iStop = 0; }
		void Normalize()
		{
			int iTmp;
			if (iStart > iStop)
			{
				iTmp = iStop;
				iStop = iStart;
				iStart = iTmp;
			}
		}
		tagSelData& operator=(const tagSelData& rhs)
		{
			if (this != &rhs)
			{
				iStart = rhs.iStart;
				iStop = rhs.iStop;
			}
			return *this;
		}

	} SELDATA;

	// Data structure used for maintaining undo data.

	typedef struct tagUndoData
	{
		CString strText;
		int iCaretPos;
		SELDATA sel;

		tagUndoData() : iCaretPos(-1) {}
		tagUndoData(const tagUndoData& u) : 
			strText(u.strText), iCaretPos(u.iCaretPos), sel(u.sel) {}
		tagUndoData& operator=(const tagUndoData& rhs)
		{
			if (this != &rhs)
			{
				strText = rhs.strText;
				iCaretPos = rhs.iCaretPos;
				sel = rhs.sel;
			}
			return *this;
		}

	} UNDODATA;

	//@access Protected data members

	//@cmember
	/* Is the data value negative?*/
	BOOL m_bNegative;
	
	//@cmember
	/* Undo buffer has something?*/
	BOOL m_bCanUndo;		
	
	//@cmember
	/* Parse value on WM_SETTEXT?*/
	BOOL m_bParseOnSetText;
	
	//@cmember
	/* Is redraw allowed?*/
	BOOL m_bRedraw;
	
	//@cmember
	/* Are we mouse selecting?*/
	BOOL m_bMouseSelect;
	
	//@cmember
	/* Is overstrike mode enabled?*/
	BOOL m_bOverstrike;
	
	//@cmember
	/* Undo buffer*/
	UNDODATA m_Undo;
	
	//@cmember
	/* Numeric value*/
	double m_dValue;
	
	//@cmember
	/* Current format settings*/
	Format* m_pFormat;
	
	//@cmember
	/* Font set via WM_SETFONT*/
	HFONT m_hFont;
	
	//@cmember
	/* Caret character position*/
	int m_iCaretPos;
	
	//@cmember
	/* 	Selection information*/
	SELDATA m_Sel;
	
	//@cmember
	/* 	The right margin in pixels.*/
	UINT m_uiRightMargin;
	
	//@cmember
	/* 	The left margin in pixels.*/
	UINT m_uiLeftMargin;


	DECLARE_MESSAGE_MAP()
};

// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// WIN32

#endif	// __CURREDIT_H__

