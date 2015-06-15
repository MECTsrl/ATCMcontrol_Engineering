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
//  Authors:		Don
//  Description:	Declarations for SECToolBar
//  Created:		August 1996
//

#ifdef WIN32

#ifndef __TBTNSTD_H__
#define __TBTNSTD_H__

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

// Forward Declarations
class SECCustomToolBar;
struct SECButtonClass;

// Define the height and width of an individual item in the toolbar bitmap
#define SEC_TOOLBAR_IMAGE_WIDTH		16
#define SEC_TOOLBAR_IMAGE_HEIGHT	15

///////////////////////////////////////////////////////////////////////////////
// SECBtnDrawData struct - contains cached info for drawing toolbars
//
struct SECBtnDrawData
{
	CDC m_monoDC;					// mono DC used during drawing
	CDC m_bmpDC;					// DC holding the toolbar bmp
	CDC m_drawDC;					// mem. DC we use to draw to
	CBrush m_ditherBrush;			// Dither brush used for drawing
									// checked/indeterminate buttons

	SECBtnDrawData();

	// Called when system color scheme changes.
	void SysColorChange();

	// Pre/Post drawing initialization
	void SetBitmap(HBITMAP hBmp);
	BOOL PreDrawButton(CDC& dc, int nMaxBtnWidth, int nMaxBtnHeight, SECCustomToolBar* pToolBar);
	void PostDrawButton();

	static HBRUSH CreateDitherBrush();

protected:
	// Members used by pre/post drawing functions
	CBitmap  m_bmpDraw;
	CBitmap* m_pOldBmpDraw;
	CBitmap  m_bmpMono;
	CBitmap* m_pOldBmpMono;
	HGDIOBJ  m_hOldBmp;
	HBITMAP  m_hCurBmp;
};

///////////////////////////////////////////////////////////////////////////////
// SECStdBtn class

/////////////////////////////
// AutoDuck tag block block for SECStdBtn

// adinsert AutoDuck insertion point for SECStdBtn
//@doc SECStdBtn
//@class The SECStdBtn class includes various operations for drawing buttons on a 
// toolbar. Its member functions are normally called from the Customize dialog 
// (see class SECToolBarsDlg).  
//@comm 
// An end user can use this dialog to create a toolbar 
// with buttons in a style geared toward a particular task.  If the default styles 
// offered in the Customize dialog do not match the user’s requirements, the New 
// Toolbar dialog, reached from within the Customize dialog, allows the user to 
// create a new toolbar from scratch, with a set of buttons in a unique style.
//
// SECStdBtn encapsulates a single toolbar button.  It is used by the 
// SECCustomToolBar class.
//
// See the sample TOOLBAR in the \SAMPLES\TOOLKIT\STANDARD\DOCKING\TOOLBAR directory for a demonstration 
// of this class.
//@xref <c SECToolBarsDlg> <c SECToolBarsPage> <c SECCustomToolBar> <c SECToolBarManager>
class SECStdBtn
{
// Construction
public:
	SECStdBtn();

// Attributes
public:
	enum InformCode			// Codes informing us to take some kind of action
	{
		IBase   = 0x0000,
		Styles  = IBase + 1	// Add/Remove button styles.
	};

	enum BtnType			// List the various button types.
	{
		BtnStandard,		// SECStdBtn
		BtnTwoPart,			// SECTwoPartBtn
		BtnWnd,				// SECWndBtn
		BtnMenu,			// SECTBMenuBtn
		BtnMenuCap,			// SECCapMenuBtn
		BtnCaption,			// SECBtnCaption
		BtnCombo,			// SECComboBtn
		BtnText 			// SECTBTextButton
	};

	struct StyleChange		// Used by InformCode::Styles
	{
		DWORD dwAdd;		// Style bits to add
		DWORD dwRemove;		// Style bits to remove
	};

	UINT m_nID;				// Button ID
	UINT m_nImage;			// Index of image in toolbar bitmap
	UINT m_nStyle;			// Button styles

	int  m_x;				// x position in parent toolbars client rect
	int  m_y;				// y position in parent toolbars client rect
	int  m_cx;				// Width of button
	int  m_cy;				// Height of button
	int  m_nType;			// See BtnType enum

// Operations
public:
	// Sets the parent toolbar
	//@cmember
	/* tSets the parent toolbar.*/
	void SetToolBar(SECCustomToolBar* pToolBar);

	// Returns the window rect. for this button (relative to parent's client)
	//@cmember
	/* Returns the window rect. for this button (relative to parent’s client).*/
	void GetBtnRect(CRect& rect) const;

	// Returns TRUE if this button is hidden
	BOOL IsHidden() const;

	// Returns TRUE if this is a menu button
	BOOL IsMenuButton() const;
	BOOL IsStandardMenuButton() const;

	// SetMode informs the button when the button either enters/leaves a
	// vertically docked state
	//@cmember
	/* Informs the button when the button either enters or leaves a vertically docked state.*/
	virtual void SetMode(BOOL bVertical);

	// SetStyle sets the button style for this button object.
	//@cmember
	/* Sets the button style for this button object.*/
   virtual void SetStyle(UINT nStyle);

	// GetStyle gets the button style for this button object.
	//@cmember
	/* Gets the button style for this button object.*/
   virtual UINT GetStyle() const;

	// TestStyle tests if a specifc style is set for this button object
	//@cmember
	/* Tests if a specifc style is set for this button object*/
   virtual BOOL TestStyle(UINT nStyle) const;

	// Modifies our window position
	//@cmember
	/* Modifies the window position relative to the parent’s client area.*/
	virtual void SetPos(int x, int y);

	// Invalidates the button
	//@cmember
	/* Invalidates the button.*/
	virtual void Invalidate(BOOL bErase = FALSE) const;

// Overrideables
public:
	// Initialises button after creation.
	//@cmember
	/* Called after construction to allow the button to initialize.*/
	virtual void Init(SECCustomToolBar* pToolBar, const UINT* pData);

	// Return FALSE if the button cannot be dragged

	virtual BOOL CanDrag() const { return TRUE; }

	// Draw the button on the given DC.
	//@cmember
	/* Draws the button on the given DC.*/
	virtual void DrawButton(CDC& dc, SECBtnDrawData& data);

	// Handle mouse events in the button's window rect.
	//@cmember
	/* Handles a mouse button press in the toolbar button’s window rect.*/
	virtual BOOL BtnPressDown(CPoint point);
	//@cmember
	/* Handles dragging the mouse while a mouse button is pressed.*/
	virtual void BtnPressMouseMove(CPoint point);
	//@cmember
	/* Handles the WM_CANCELMODE message while tracking a WM_LBUTTONDOWN message.*/
	virtual void BtnPressCancel();
	//@cmember
	/* Handles release of a mouse button press.*/
	virtual UINT BtnPressUp(CPoint point);

	// Called when the button is hidden or unhidden.
	virtual void OnHide() {}

	// Our parent toolbars style has been modified.
	//@cmember
	/* Notifies that parent toolbar’s style has been modified.*/
	virtual void BarStyleChanged(DWORD dwStyle);

	// Tool-tip/flyby help hit-testing
	//@cmember
	/* Tests tooltip/flyby help.*/
	virtual int  OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	// Informs button of some event
	//@cmember
	/* Informs button of some event.  Normally used to reflect a change of */
	// state in all buttons of the same ID.
	virtual void InformBtn(UINT nCode, void* pData);

	// Called when toolbar bitmap has changed ... buttons should now adjust
	// their size.
	//@cmember
	/* Called when toolbar bitmap has changed; button should now adjust their size.*/
	virtual void AdjustSize();

	// Functions called when saving/loading a toolbar state, allowing the
	// button to save state info.
	//@cmember
	/* Called when loading a toolbar state, allowing a toolbar button to save state info.*/
	virtual void GetBtnInfo(BYTE* nSize, LPBYTE* ppInfo) const;
	//@cmember
	/* Called when saving a toolbar state, allowing a  toolbar button to save state information.*/
	virtual void SetBtnInfo(BYTE nSize, const LPBYTE pInfo);

	// Function that sends a WM_NOTIFY for this button.
	virtual LRESULT SendWmNotify(UINT code, DWORD dwMisc1 = 0, DWORD dwMisc2 = 0);

protected:
	// Drawing functions ...
	virtual void DrawFace(SECBtnDrawData& data, BOOL bForce, int& x, int& y, 
						  int& nWidth, int& nHeight, int nImgWidth = -1);
	virtual void DrawDisabled(SECBtnDrawData& data, int x, int y, int nWidth, 
							  int nHeight);
	virtual void DrawChecked(SECBtnDrawData& data, int x, int y, int nWidth, 
							 int nHeight);
	virtual void DrawIndeterminate(SECBtnDrawData& data, int x, int y, 
								   int nWidth, int nHeight);
	virtual void DrawConfigFocus(SECBtnDrawData& data);
	virtual void LoadItemBitmap(SECBtnDrawData& data);

	// DrawButton hooks
	virtual BOOL OnDrawButtonAsSeparator(CDC& dc);
	virtual void OnDrawButtonFace(SECBtnDrawData& data);
	virtual void OnDrawFaceBlt(int xImg,int yImg,int nImgWidth,int nImgHeight,
							   SECBtnDrawData& data,int xDest,int yDest);
	virtual void OnDrawButtonBkgnd(CDC& dc, SECBtnDrawData& data);
	virtual void OnDrawButtonFocus(SECBtnDrawData& data);
	virtual void OnDrawButtonFinalBlt(CDC& dc, SECBtnDrawData& data);

// Implementation
public:
	virtual ~SECStdBtn();

protected:	
	// Drawing helper functions
	void CreateMask(SECBtnDrawData& data, int x, int y, int nWidth, int nHeight);

protected:
	// Implementation, data members.
	SECCustomToolBar* m_pToolBar;			// Our parent toolbar

public:
	// Dynamic creation helpers
	virtual SECButtonClass* GetButtonClass() const;
	static const AFX_DATA SECButtonClass classSECStdBtn;
	static SECStdBtn* PASCAL CreateButton();
};

///////////////////////////////////////////////////////////////////////////////

inline BOOL SECStdBtn::IsHidden() const
{
	return (BOOL) (m_nStyle & TBBS_HIDDEN);
}

inline BOOL SECStdBtn::IsMenuButton() const
{
	return (BOOL) (m_nType == BtnMenu || m_nType == BtnMenuCap);
}

inline BOOL SECStdBtn::IsStandardMenuButton() const
{
	return (BOOL) (m_nType == BtnMenu);
}

///////////////////////////////////////////////////////////////////////////////
// Dynamic button creation helpers
//
#define BUTTON_CLASS(class_name) ((SECButtonClass*)(&class_name::class##class_name))

#define DECLARE_BUTTON(class_name) \
public: \
	static const AFX_DATA SECButtonClass class##class_name; \
	virtual SECButtonClass* GetButtonClass() const; \
	static SECStdBtn* PASCAL CreateButton();

#define IMPLEMENT_BUTTON(class_name) \
	SECStdBtn* PASCAL class_name::CreateButton() \
		{ return new class_name; } \
	const AFX_DATADEF SECButtonClass class_name::class##class_name = { \
		class_name::CreateButton }; \
	SECButtonClass* class_name::GetButtonClass() const \
		{ return BUTTON_CLASS(class_name); } \

struct SECButtonClass 
{
// Attributes
	SECStdBtn* (PASCAL* m_pfnCreateButton)();

// Operations
	SECStdBtn* CreateButton();
};

///////////////////////////////////////////////////////////////////////////////
// Button map definitions and helper macros	 **** DEAN - Should this be moved?
//
#define BTNMAP_DATA_ELEMENTS	5
struct SECBtnMapEntry
{
	SECButtonClass* m_pBtnClass;					// Class for this button
	UINT            m_nID;							// ID for this button
	UINT	 	    m_nStyle;						// Style for this button
	UINT            m_nData[BTNMAP_DATA_ELEMENTS];	// Button specific info.
};

// Define the beginning of a toolbar map.
#define BEGIN_BUTTON_MAP(name) \
	static const SECBtnMapEntry name[] = {

// Define a standard toolbar button
#define STD_BUTTON(id, style) \
	{BUTTON_CLASS(SECStdBtn), id, style|TBBS_BUTTON, 0, 0, 0, 0, 0},

// Define the end of the toolbar map
#define END_BUTTON_MAP() \
	{NULL, 0, 0, 0, 0, 0, 0, 0} \
	};


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TBTNSTD_H__

#endif // WIN32

