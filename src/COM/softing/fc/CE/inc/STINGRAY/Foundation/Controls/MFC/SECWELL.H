/////////////////////////////////////////////////////////////////////////////
// SecWell.h : header file for the SECColorWell and SECPopupColorWell custom
//             controls
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
//		Author:		Don			04/02/96
//

 
#ifndef __SFL_SECWELL_H__
#define __SFL_SECWELL_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

// CMN Extension DLL
// Initialize declaration context
//

#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

#define CWN_COLOR_CHANGE		(WM_USER+101)
#define CWN_CUSTOM_COLOR		(WM_USER+102)
#define CWS_RAISED_BORDER		0x0001L			// Draw well with raised border

class SECOtherButton;

/////////////////////////////////////////////////////////////////////////////
// SECColorWell window

/////////////////////////////
// AutoDuck tag block block for SECColorWell 

//@doc SECColorWell
//@mfunc virtual void  | SECColorWell | InitAdditionalColors | Called to initialise colors 
// other than the 20 system colors.
//@parm LPPALETTEENTRY  | pPalEntry | parm comment 
//@parm  int  | nCols | parm comment 
//@parm  int  | nSystemCols | parm comment 

//@doc SECColorWell
//@mdata CPalette  | SECColorWell | m_palette | The palette. Holds the colors for cells.
//@xref <c SECColorWell> <mf SECColorWell::CreatePalette>
// <mf SECColorWell::SetPaletteRealization>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nRows | Number of cell rows.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nCols | Number of cell columns.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nIndexOtherCol | Column number of "other" cell.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nIndexOtherRow | Row number of "other" cell.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nCellHeight | Height of cell.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nCellWidth | Width of cell.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nIndentWidth | Width of border.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nIndentHeight | Height of border.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nButtonHeight | Height of other button.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nCurRow | The row of the currently selected cell.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nCurCol | The column of the currently selected cell.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata int  | SECColorWell | m_nPaletteIndexOther | Index of other cell in color palette.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata BOOL  | SECColorWell | m_bHasFocus | True if this currently has focus.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata BOOL  | SECColorWell | m_bMouseTracking | Indicates whether mouse tracking is enabled.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata BOOL  | SECColorWell | m_bHasOther | Indicates whether we have an "other" button.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata BOOL  | SECColorWell | m_bOtherSet | Indicates whether the color for "other" has been set.
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata BOOL  | SECColorWell | m_bRealizePalette | Indicates whether we realize our palette.
//@xref <c SECColorWell> <mf SECColorWell::GetPaletteRealization>

//@doc SECColorWell
//@mdata CBrush  | SECColorWell | m_brushBtnFace | Brush for painting button face
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata SECOtherButton*  | SECColorWell | m_pOtherButton | Object for other button
//@xref <c SECColorWell>

//@doc SECColorWell
//@mdata DWORD  | SECColorWell | m_dwColorDialogFlags | Flags for customizing the CColorDialog displayed in response to clicking the Other button 
//@xref <c SECColorWell> <mf SECColorWell::SetColorDialogFlags> <mf SECColorWell::GetColorDialogFlags>

//@doc SECColorWell
//@mdata COLORREF*  | SECColorWell | m_pCustomColors | Pointer to an array of 16 colors for the custom color boxes in CColorDialog
//@xref <c SECColorWell>

// adinsert AutoDuck insertion point for SECColorWell 
//@doc SECColorWell 
//@class The SECColorWell class implements a color selection window.  This 
// window displays a grid of the 20 system colors, and optionally a button 
// labeled 'Other'. If the 'Other' button is clicked, a common color 
// selection dialog is displayed allowing selection of additional colors.
//
//@ex  You can create a color well window with the following method call: | 
// m_well.Create(WS_VISIBLE|WS_TABSTOP|CWS_RAISED_BORDER,
//                0,
//                0,
//                this,
//                IDC_WELL1);
//@comm The currently selected color can optionally track the mouse 
// (enabled with SetMouseTracking). This means that if the mouse passes 
// over a color cell, then that cell becomes the selected cell.
//
// All displayed colors can optionally be realized in a palette before 
// drawing. This is enabled with SetPaletteRealization.
//
// Any change to the selected color causes a CWN_COLOR_CHANGE message 
// to be sent to the parent of the color well window.
//
// <f DDX_Color> can be used for DDX support with the color well classes.
//
// See the COLOR sample in the \OT\SAMPLES\STANDARD\CONTROLS\COLRWELL 
// directory for a demonstration of this class.
//@base public | CWnd
class SECColorWell : public CWnd
{
	DECLARE_DYNAMIC(SECColorWell);

	//@access Creation/Initialization
public:

	//@cmember,menum
	/* Selection types*/
	enum SelTypes
	{
		ST_Keyboard,		//@@emem Keyboard
		ST_MouseMove,		//@@emem Mouse movement
		ST_MouseButton		//@@emem Mouse click
	};


	//@cmember
	/* Constructs a color well object.*/
	SECColorWell();
	
	//@cmember
	/* Creates a color well window.*/
	virtual BOOL Create(DWORD dwStyle, 
						int x,							// Top left corner (auto sizes)
						int y, 
						CWnd* pParentWnd, 
						UINT nID, 
						BOOL bDialogBaseUnits = FALSE,	// x,y are dialog base units 
														// rather than pixels
						BOOL bOtherButton = TRUE,		// Has an other button
						CCreateContext* pContext = NULL);

	//@access Attributes
public:

	//@cmember
	/* Returns the currently selected color.*/
	COLORREF GetColor() const;

	//@cmember
	/* Gets the currently selected color.*/
	void SetColor(COLORREF color);

	//@cmember
	/* Returns the mouse tracking mode.*/
	BOOL GetMouseTracking() const;

	//@cmember
	/* Sets the mouse tracking mode.*/
	void SetMouseTracking(BOOL bTracking);

	//@cmember
	/* Returns the palette realization mode.*/
	BOOL GetPaletteRealization() const;

	//@cmember
	/* Sets the palette realization mode.*/
	void SetPaletteRealization(BOOL bRealize);

	//@cmember
	/* Sets the flags for customizing the CColorDialog that is display in response to a click on the "Other" button. Default = CC_RGBINIT\|CC_FULLOPEN. */
	virtual void SetColorDialogFlags(DWORD v);

	//@cmember
	/* Returns the flags for the CColorDialog. Default = CC_RGBINIT\|CC_FULLOPEN. */
	DWORD GetColorDialogFlags() const;
	
	//@cmember
	/* Sets the array of 16 colors for the custom color boxes in CColorDialog */
	virtual void SetCustomColors(COLORREF* pColors);
	
	//@cmember
	/* Retrieves the array of 16 colors for the custom color boxes in CColorDialog. Default=NULL */
	COLORREF* GetCustomColors() const;
	
// Operations
public:
	//@cmember
	/* Resets the color well object.*/
	virtual void Reset();

	//@access Overridable
protected:

	//@cmember
	/* Draws an unselected color cell.*/
	virtual void DrawCell(CDC& memDC, CDC& paintDC, const CRect& internalRect, 
						  int nCol, int nRow);
	
	//@cmember
	/* Draws a selected color cell.*/
	virtual void DrawSelectedCell(CDC& memDC, CDC& paintDC, const CRect& internalRect,
								  int nCol, int nRow);

	//@cmember
	/* Notifies the parent window of a change in color.*/
	virtual void NotifyChange();

	//@cmember
	/* Called when the user selects a new color.*/
	virtual void HandleSelection(int nCol, int nRow, SelTypes selType);

	//@cmember
	/* Creates a color palette.*/
	virtual void CreatePalette();

	//@cmember
	/* Sets the number of columns and number of rows in the grid.*/
	virtual void SetGridSize();

	//@cmember
	/* Called to initialise colors other than the 20 system colors.*/
	virtual void InitAdditionalColors(LPPALETTEENTRY pPalEntry, int nCols, int nSystemCols) {pPalEntry; nCols; nSystemCols;}

	//@cmember
	/* Returns the class style used to register the color well window class.*/
	virtual UINT GetClassStyle() const;

	//@cmember
	/* Returns whether the color well has a focus rectangle.*/
	virtual BOOL HasFocusRectangle() const; 

	//@access Overrides

	//@cmember
	/* Called when Windows or an application makes a request to repaint.*/
	afx_msg void OnPaint();
	
	//@cmember
	/* Called after gaining the input focus.*/
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	
	//@cmember
	/* Called immediately before losing the input focus. */
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	
	//@cmember
	/* Override to control the particular types of input to process.*/
	afx_msg UINT OnGetDlgCode();
	
	//@cmember
	/* Called when a nonsystem key is pressed.*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	//@cmember
	/* Called when the user presses the left mouse button.*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	//@cmember
	/* Called when the mouse cursor moves. */
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	//@cmember
	/* Called when the "other" button is clicked (to create CColorDialog).*/
	afx_msg void OnClickedOther();
	
	//@cmember
	/* Called when a change is made in the system color setting.*/
	afx_msg void OnSysColorChange();

#ifdef WIN32
	
	//@cmember
	/* Called when a child control is about to be drawn.*/
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

#endif

	//@access Implementation
public:
	virtual ~SECColorWell();
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

protected:

	//@cmember
	/* Returns the client rectangle of an individual cell.*/
	void GetCellRect(const CRect& internalRect, CRect& rect, int nCol, int nRow);
	
	//@cmember
	/* Returns the client rectangle of an individual cell.*/
	void GetCellRect(CRect& rect, int nCol, int nRow);

	//@cmember
	/* Draws the well.*/
	void Paint(CDC& memDC, CDC& paintDC);
	
	//@cmember
	/* Draws the focus rectangle.*/
	void DrawFocusRect();

	//@cmember
	/* Invalidates an individual cell.*/
	void InvalidateCell(int nCol, int nRow);

	//@cmember
	/* Hit tests which cell a specified point is in.*/
	BOOL HitTest(CPoint point, int& nCol, int& nRow);

	//@cmember
	/* Obtains the palette index associated with an individual cell.*/
	int  GetPaletteIndex( int nCol, int nRow ) const;
	
	//@cmember
	/* Obtains the color associated with an individual cell.*/
	COLORREF GetPaletteColor( int nCol, int nRow ) const;

	//@cmember
	/* Changes the selection to a given cell.*/
	void ChangeSelection(int nCol, int nRow);

	//@cmember
	/* Calculates the internal client rectangle (less 3d borders).*/
	void GetInternalRect(CRect& rect);

	//@cmember
	/* Calculates the required size of the client rectangle.*/
	virtual CRect CalculateSize(CWnd* pParentWnd, int nCol, int nRow, BOOL bDialogBaseUnits);

protected:

	//@access Protected data members

	//@cmember
	/* The palette. Holds the colors for cells.*/
	CPalette m_palette;
	
	//@cmember
	/* Number of cell rows.*/
	int m_nRows;
	
	//@cmember
	/* Number of cell columns.*/
	int m_nCols;
	
	//@cmember
	/* Column number of "other" cell.*/
	int m_nIndexOtherCol;
	
	//@cmember
	/* Row number of "other" cell.*/
	int m_nIndexOtherRow;
	
	//@cmember
	/* Height of cell.*/
	int m_nCellHeight;
	
	//@cmember
	/* Width of cell.*/
	int m_nCellWidth;
	
	//@cmember
	/* Width of border.*/
	int m_nIndentWidth;
	
	//@cmember
	/* Height of border.*/
	int m_nIndentHeight;
	
	//@cmember
	/* Height of other button.*/
	int m_nButtonHeight;
	
	//@cmember
	/* The row of the currently selected cell.*/
	int m_nCurRow;
	
	//@cmember
	/* The column of the currently selected cell.*/
	int m_nCurCol;
	
	//@cmember
	/* Index of other cell in color palette.*/
	int m_nPaletteIndexOther;
	
	//@cmember
	/* True if this currently have focus.*/
	BOOL m_bHasFocus;
	
	//@cmember
	/* Indicates whether mouse tracking is enabled.*/
	BOOL m_bMouseTracking;
	
	//@cmember
	/* Indicates whether we have an "other" button.*/
	BOOL m_bHasOther;
	
	//@cmember
	/* Indicates whether the color for "other" been set.*/
	BOOL m_bOtherSet;
	
	//@cmember
	/* Indicates whether we realize our palette.*/
	BOOL m_bRealizePalette;
	
	//@cmember
	/* Flags for customizing the CColorDialog displayed in response to clicking the Other button */
	DWORD m_dwColorDialogFlags;
	
	//@cmember
	/* Pointer to an array of 16 colors for the custom color boxes in CColorDialog */
	COLORREF* m_pCustomColors;

#ifdef WIN32

	//@cmember
	/* Brush for painting button face*/
	CBrush m_brushBtnFace;

#endif
	
	//@cmember
	/* Object for other button*/
	SECOtherButton* m_pOtherButton;
	
	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// SECPopupColorWell window

/////////////////////////////
// AutoDuck tag block block for SECPopupColorWell 

//@doc SECPopupColorWell
//@mdata HWND  | SECPopupColorWell | m_notifyHwnd | Window to notify of color changes.

//@doc SECPopupColorWell
//@mdata CWnd*  | SECPopupColorWell | m_pNotifyWnd | Window to return focus to on destroying popup.

//@doc SECPopupColorWell
//@mdata BOOL  | SECPopupColorWell | m_bCreated | True when the popup has been created

// adinsert AutoDuck insertion point for SECPopupColorWell 
//@doc SECPopupColorWell 
//@class The SECPopupColorWell class implements a popup color well window 
// (See <c SECColorWell>).  This class is defined primarily for use with 
// the <c SECWellButton> class.  It is popped up when the well button is 
// clicked to allow the user to select a color.  When a color is selected 
// a CWN_COLOR_CHANGE message is sent to the SECWellButton, and the popup 
// window is destroyed (not the SECPopupColorWell object).  Similarly, if 
// the popup window loses focus, it is also destroyed.
//
// If the 'Other' button (displayed on the face of the popup color well 
// window) is clicked, the popup window is destroyed and a CWN_CUSTOM_COLOR 
// message is sent to the button.
//
// <f DDX_Color> can be used for DDX support with the color well classes.
//
// See the COLOR sample in the \OT\SAMPLES\STANDARD\CONTROLS\COLRWELL directory for a 
// demonstration of this class.
//@base public | SECColorWell
class SECPopupColorWell : public SECColorWell
{
	DECLARE_DYNAMIC(SECPopupColorWell);

	//@access Creation/Initialization
public:

	//@cmember
	/* Constructs a popup color well object.*/
	SECPopupColorWell();

	//@cmember
	/* Creates a popup color well window.*/
	virtual BOOL Create(DWORD dwStyle, 
						CRect& buttonRect, 
						CWnd* pParentWnd, 
						BOOL bHasOther = TRUE,		// TRUE if has other button
						CCreateContext* pContext = NULL);

	//@access Attributes
public:
	
	//@cmember
	/* Returns whether the popup color well window is currently created.*/
	BOOL IsCreated() const;

// Operations
public:

	//@cmember
	/* Sets the window to notify of color changes.*/
	void SetNotifyWindow(CWnd* pWnd);

	//@cmember
	/* Resets the popup color well object.*/
	virtual void Reset();

protected:

	//@access Overrides

	//@cmember
	/* Called when the user selects a new color.*/
	virtual void HandleSelection( int x, int y, SelTypes selType );

	//@cmember
	/* Notifies the parent window of a change in color.*/
	virtual void NotifyChange();

	//@cmember
	/* Called by the default OnNcDestroy member function after the */
    // window has been destroyed. 
	virtual void PostNcDestroy();

	//@cmember
	/* Returns the class style used to register the color well window class.*/
	virtual UINT GetClassStyle() const;

	//@cmember
	/* 	Returns whether the color well has a focus rectangle.*/
	virtual BOOL HasFocusRectangle() const; 

	//@cmember
	/* Called when a nonsystem key is pressed.*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//@cmember
	/* Called when a nonsystem key is released.*/
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	//@cmember
	/* Called from OnLButtonDown() when the "other" button is clicked (to create CColorDialog).*/
	virtual void ClickedOther();
	
	//@cmember
	/* Called when the user presses the left mouse button.*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	//@cmember
	/* Override to control the particular types of input to process.*/
	afx_msg UINT OnGetDlgCode();

	//@cmember
	/* Called immediately before losing the input focus.*/
	afx_msg void OnKillFocus(CWnd* pNewWnd);

// Implementation
public:
	virtual ~SECPopupColorWell() {};

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

protected:


protected:
	
	// Protected data members

	//@cmember
	/* Window to notify of color changes.*/
	HWND m_notifyHwnd;

	//@cmember
	/* Window to return focus to on destroying popup.*/
	CWnd* m_pNotifyWnd;

	//@cmember
	/* True when the popup has been created*/
	BOOL m_bCreated;

	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// DDX functions for use with the SECColorWell
//@func DDX function to use with SECColorWell
void DDX_Color(CDataExchange* pDX, SECColorWell& cWell, COLORREF& color);

/////////////////////////////////////////////////////////////////////////////

};  // namespace stingray::foundation
};  // namespace stingray

//
// CMN Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif
            

