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
//  Author:			John Williston
//  Description:	Declarations for SECListBoxEdit, SECListBoxEditorHelper,
//                  SECListBoxEditor, SECListBoxDirEditor and SECListBoxFileEditor
//  Created:		August 1996
//
// Change History:
// 2/26/97 Mark Isham - Added some much needed virtual callbacks for significant events
// 8/97 - Added dev studio style "blank line" edit, numerous message notifications, and
//		  various bug fixes courtesy of H. Scott Buckwalter
//

#ifndef __LSTBXEDT_H__
#define __LSTBXEDT_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#if defined(WIN32) && !defined(__AFXCMN_H__)
#include "afxcmn.h"
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

// Use the following flags when initializing a SECListBoxEditor object
// to configure it.  Note:  the LBE_TOOLTIPS flag is only valid when
// the LBE_BUTTONS flag is in use.

const int LBE_BROWSE       = 0x0001;	// edit has a browse button?
const int LBE_AUTOEDIT     = 0x0002;	// alphanumeric key starts edit?
const int LBE_BUTTONS      = 0x0004; 	// supply command buttons?
const int LBE_TOOLTIPS     = 0x0008;	// supply tooltips for buttons ?
const int LBE_SHORTCUTS    = 0x0010;	// add, delete, and move from keyboard?
const int LBE_DRAGNDROP    = 0x0020;	// drag and drop items in list?
const int LBE_NOTRAILBLANK = 0x0040;    // do not add the trailing blank item
const int LBE_NEWBUTTON    = 0x0100;	// display "new" pushbutton
const int LBE_DELBUTTON    = 0x0200;	// display "delete" pushbutton
const int LBE_UPBUTTON     = 0x0400;	// display "Up" pushbutton
const int LBE_DOWNBUTTON   = 0x0800;	// display "Down" pushbutton

const int LBE_DEFAULT = LBE_AUTOEDIT | LBE_BUTTONS | 
                        LBE_TOOLTIPS | LBE_SHORTCUTS | LBE_DRAGNDROP |
						LBE_NEWBUTTON | LBE_DELBUTTON | 
						LBE_UPBUTTON | LBE_DOWNBUTTON;

const UINT NM_LBE_DONE = 1000;

#define LBN_LINEDELETED		10
#define LBN_LINEMOVED		11
#define WM_SECLSTBXEDIT_ESCAPE	WM_USER+0x0100	// VK_ESCAPE notification

// Forward declarations to avoid header #include's.
class SECListBoxEdit;
class SECListBoxEditorHelper;

/////////////////////////////
// AutoDuck tag block block for SECListBoxEditor 


//@doc SECListBoxEditor
//@mfunc BOOL  | SECListBoxEditor | IsEditing | Determines whether 
// in-place editing is occurring at the moment (i.e., floating edit 
// control is present and user can change contents of the listbox).
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mfunc void  | SECListBoxEditor | SetEscape | Sets escape key handling.
//@parm BOOL  | bSet | If TRUE, control handles the escape key.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mfunc BOOL  | SECListBoxEditor | GetEscape | Retrieves escape key handling state.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mfunc void  | SECListBoxEditor | ForcePaint | Forces a repaint. 
//@devnote This method is
// provided for NT3.51 painting workaround.   NT3.51 does not always properly 
// receive WM_PAINT notifications (manifests as invisible buttons). You can 
// workaround by directly firing the paint in response to the parent window's 
// OnPaint handler. 
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata CButton  | SECListBoxEditor | m_btnBrowse | Browse button (if used).
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata CToolTipCtrl  | SECListBoxEditor | m_ToolTip | Tooltip control.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata SECListBoxEdit*  | SECListBoxEditor | m_pEdit | Pointer to editing control.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata SECListBoxEditorHelper*  | SECListBoxEditor | m_pHelper | Pointer to helper object.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata CRect  | SECListBoxEditor | m_rcButtons[NUM_BUTTONS] | Button rectangles
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata BOOL   | SECListBoxEditor | m_bUseButton[NUM_BUTTONS] | Button 
// existance boolean flags.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata int  | SECListBoxEditor | m_iFlags | data member comment
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata BOOL  | SECListBoxEditor | m_bInitialized | Initialization flag.  
// TRUE if object is initialized.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata int  | SECListBoxEditor | m_calcdHeight | Calculated height.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata BOOL  | SECListBoxEditor | m_bAddInProgress | TRUE if a new item is being created.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata CString  | SECListBoxEditor | m_strRenameOldName | Item text name before rename occurred.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata BOOL  | SECListBoxEditor | m_bEscape | TRUE if handling escape key
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mfunc virtual void  | SECListBoxEditor | OnBrowse | Called when the browse button is clicked.
//@comm Descendants should override this method to provide browse functionality.  
// Access to the edit control is available through the protected class members.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata int   | SECListBoxEditor | mnChangedLine | The index of an item before it is moved.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata int   | SECListBoxEditor | mnNewLine | The index of an item after it is moved.
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata BOOL  | SECListBoxEditor | mbQuick | "Quick mode" flag. (See <mf SECListBoxEditor::SetQuick>.)
//@xref <c SECListBoxEditor>

//@doc SECListBoxEditor
//@mdata int   | SECListBoxEditor | mnSpaces | Number of spaces treated as a tab (quick mode). (See <mf SECListBoxEditor::SetQuick>.)
//@xref <c SECListBoxEditor>

// adinsert AutoDuck insertion point for SECListBoxEditor 
//@doc SECListBoxEditor 
//@class The SECListBoxEditor class provides the functionality of an 
// editable list box.  An editable list box is a Windows list box that 
// enables the end user to interactively modify the items it contains.
//@comm The user can modify the ordering of list box items, create and delete 
// items and edit the content of items.  There are two ways to edit a 
// list box item.  First, you can in-place edit the text of the item by 
// simply selecting the item and typing.  Second, you can browse for a 
// value by pressing the browse button located to the right of the item.  
// A browse button is a push button labeled "..." which is only visible 
// after the user double-clicks on an item.  When pressed, the browse 
// button pops up a dialog to allow the user to choose from a list 
// (or tree) of possible values.  After the user picks a new value and 
// dismisses the dialog, the selected item is automatically replaced.
//
// To support the creation, deletion and reordering of list box items, 
// SECListBoxEditor adds several child controls, called command buttons, 
// above the list box.  The command buttons and their actions when 
// pressed are shown below:
//
// Command Button		Action When Pressed<nl>
// New Button			A new list box item is created at the end of the list.<nl>
// Delete Button		The selected list box item is removed.<nl>
// Move-Up Button		The selected list box item is moved up one.<nl>
// Move-Down Button		The selected list box item is moved down one.
//
// The command buttons are optional and their creation can be individually
// controlled by using style flag passed into the initialization of the 
// editable list box (See SECListBoxEditor::<mf SECListBoxEditor::Initialize>.)
//
// Several shortcut keys are also available: "Insert" to add an entry to end; 
// "Delete" to remove currently selected item; "Escape" to discard changes 
// for in-place edit; and "Return" to accept in-place edit changes.
//
// The SECListBoxEditor can be used directly, or it can serve as a base 
// class.  Used directly, an SECListBoxEditor object supports item creation, 
// deletion, reordering and in-place text editing.  However, it cannot 
// support browsing.  If browsing is required, you must derive a class 
// from SECListBoxEditor and override its OnBrowse method.  Typically, an 
// overridden OnBrowse method will pop up an application-specific modal 
// dialog, accept the user’s choice and write the new value back to the 
// selected list box item.  Refer to the implementations of OnBrowse in 
// <c SECListBoxDirEditor> and <c SECListBoxFileEditor> for examples of how 
// this is done.
//
// SECListBoxEditor now supports list boxes with the LBS_MULTIPLESEL style. Items
// can only be moved using the buttons or keyboard when a single item is selected
// (moving multiple items this way does not make sense).  You can drag an item
// with the mouse with multiple items selected, and you the delete operation will
// delete all selected items.
// 
// See the LBEDIT sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\LBEDIT directory for a 
// demonstration of the SECListBoxEditor class.
//
//@base public | CWnd
//@xref <c SECListBoxDirEditor> <c SECListBoxFileEditor>
class SECListBoxEditor : public CWnd
{
public:

	//@access Creation/Initialization

	//@cmember
	/* Constructs an SECListBoxEditor object.*/
	SECListBoxEditor();

	// SECListBoxEditor destructor.
	virtual ~SECListBoxEditor();

	//@cmember
	/* Attaches an SECListBoxEditor object to an existing list box control.*/
	virtual BOOL Initialize(CWnd* pwndParent, UINT uiID, int iFlags = LBE_DEFAULT);

	virtual BOOL Initialize(UINT uiID, CWnd* pwndParent, int iFlags = LBE_DEFAULT);

	//@access Attributes

	//@cmember
	/* Determines whether in-place editing is occurring at the moment.*/
	BOOL IsEditing() const		
		{ return m_pEdit != NULL; }

	//@cmember
	/* Sets escape key handling.*/
	void SetEscape(BOOL bSet) 
		{ m_bEscape=bSet; }
	
	//@cmember
	/* Retrieves escape key handling state.*/
	BOOL GetEscape() 
		{ return m_bEscape; }

	//@cmember
	/* Returns a pointer to the listbox control bound to this control.*/
	CListBox* GetListBoxPtr() const;
	
	BOOL IsFlagSet(int i)   { return m_iFlags & i; }
	
	BOOL AreFlagsSet(int i) { return (m_iFlags & i) == i; }

	//@cmember
	/* Sets "quick mode".*/
	virtual void SetQuick (BOOL bQuick = TRUE);
	
	//@cmember
	/* Retrieves the "quick mode" state.*/
	BOOL GetQuick() const;
	
	//@access Operations	

	//@cmember
	/* Forces an end to active in-place editing.*/
	virtual void StopEditing();

	//@cmember
	/* Forces a repaint*/
	void ForcePaint() { OnPaint(); }

protected:


public:

	//@access Overridable

	//@cmember
	/* Called when editing has started.*/
	virtual void OnEditingJustStarted();

	//@cmember
	/* Called when editing has stopped.*/
	virtual void OnEditingStopped();

	//@cmember
	/* Called after an item has been renamed.*/
	virtual void OnItemRenamed(int nIndexRenamed,CString strOldName,CString strNewName);

	//@cmember
	/* Called after an item has been added.*/
	virtual void OnItemAdded(int nIndexNewItem,CString strTextInserted);

	//@cmember
	/* Called after an item has been moved.*/
	virtual void OnItemMoved(int nOldIndex,int nNewIndex,CString strTextMoved,BOOL WasDragged);

	//@cmember
	/* Called before an item is deleted.*/
	virtual BOOL OnItemDelete(int nIndexToBeDeleted,CString strTextDeleted);

	// The following methods may be overridden in descendant classes.
	//@cmember
	/* Adds an item to the listbox.*/
	virtual BOOL DoAdd();

	//@cmember
	/* Deletes the currently selected item from the listbox.*/
	virtual BOOL DoDelete();

	//@cmember
	/* Moves an item down in the listbox.*/
	virtual BOOL DoMoveDown();
	
	//@cmember
	/* Moves an item up in the listbox.*/
	virtual BOOL DoMoveUp();
	
	//@cmember
	/* Begins editing mode.*/
	virtual BOOL StartEditing(TCHAR c = NULL);
	
	//@cmember
	/* Calculates the height of the control.*/
	virtual int CalculateHeight();
	
	//@cmember
	/* Calculates the button placements.*/
	virtual void CalculateButtonRectangles();
	
	//@cmember
	/* Resets tooltips for the buttons.*/
	virtual void RecycleTooltips();
	
	//@cmember
	/* Resets the edit control size and position.*/
	virtual void ResetEditorSizePos();
	
	//@cmember
	/* Changes the visibility status of the buttons.*/
	virtual void SetButtonVisibility(int iFlags);
	
	//@cmember
	/* Creates the edit control for editing*/
	virtual BOOL EditCore(int iIndex, BOOL bDelOnCancel, LPCTSTR p = NULL);
	
	//@cmember
	/* Moves an item in the list box up one position.*/
	virtual void MoveUpCore(int iIndex);
	
	//@cmember
	/* Deletes an item in the listbox.*/
	virtual BOOL DeleteItem (int nItem);
	
	//@cmember
	/* Sets the number of spaces to treat as a tab for "quick mode".*/
	virtual void SetSpacesToTab (int nSpaces = 1);

	//@cmember
	/* Called when the browse button is clicked.*/
	virtual void OnBrowse() { }
	
	// access Overrides
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECListBoxEditor)

	//Used to translate window messages before they are dispatched.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	//}}AFX_VIRTUAL

	//{{AFX_MSG(SECListBoxEditor)

	// Called when Windows or an application makes a request to repaint.
	afx_msg void OnPaint();

	// Called when the user presses the left mouse button.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	// Called every time the mouse is moved.
	afx_msg UINT OnNcHitTest(CPoint point);


	// Called when an application changes the enabled state
	afx_msg void OnEnable( BOOL bEnable );

	// Called after the window's size has changed. 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	// Called after the CWnd object has been moved.
	afx_msg void OnMove(int x, int y);
	
	//}}AFX_MSG

	//@access Implementation

	// Useful constants -- hidden in class scope.
	enum { NUM_BUTTONS = 4, IDC_EDIT = 100, IDC_BUTTON = 101 };

	//@cmember
	/* Returns the index of an item before it changed.*/
	int	 GetChangedLine() const;
	
	//@cmember
	/* Returns the index of an item after it moved.*/
	int	 GetNewLine() const;
	
	//@cmember
	/* Gets the number of spaces to be treated as a tab for "quick mode".*/
	int	 GetSpacesToTab() const;


	// Grant friendship to a class used in the implementation.
	friend class SECListBoxEditorHelper;

protected:

	//@access Protected data members.

	//@cmember
	/* Browse button (if used).*/
	CButton m_btnBrowse;
				
#if defined(WIN32) && !defined(_WIN32_WCE_NO_TOOLTIPS)

	//@cmember
	/* Tooltip control.*/
	CToolTipCtrl m_ToolTip;

#endif


	//@cmember
	/* Pointer to editing control.*/
	SECListBoxEdit* m_pEdit;

	//@cmember
	/* Pointer to helper object.*/
	SECListBoxEditorHelper* m_pHelper;

	//@cmember
	/* Button rectangles*/
	CRect m_rcButtons[NUM_BUTTONS];
	
	//@cmember
	/* Button existance boolean flags*/
	BOOL  m_bUseButton[NUM_BUTTONS];

	//@cmember
	/* Flags controlling operation.*/
	int m_iFlags;

	//@cmember
	/* Initialization flag.*/
	BOOL m_bInitialized;

	//@cmember
	/* Calculated height.*/
	int m_calcdHeight;

	//@cmember
	/* TRUE if a new item is being created.*/
	BOOL m_bAddInProgress;

	//@cmember
	/* Item text before rename occurred.*/
	CString m_strRenameOldName;

	//@cmember
	/* TRUE if handling escape key*/
	BOOL m_bEscape;

	//@cmember
	/* The index of an item before it is moved.*/
	int		mnChangedLine;

	//@cmember
	/* The index of an item after it is moved.*/
	int		mnNewLine;
	
	//@cmember
	/* "Quick mode" flag.*/
	BOOL	mbQuick;
	
	//@cmember
	/* Number of spaces treated as a tab (quick mode).*/
	int		mnSpaces;

	
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////
// AutoDuck tag block block for SECListBoxDirEditor 

//@doc SECListBoxDirEditor
//@mfunc virtual BOOL  | SECListBoxDirEditor | Initialize | Initializes 
// the SECListBoxEditor object by attaching to the list box control 
// identified by uiID.  This is done by subclassing the list box control 
// and attaching it to the instance of SECListBoxEditor for which 
// Initialize was called.
//
// See SECListBoxEditor::<mf SECListBoxEditor::Initialize> for available styles.
//
//@parm CWnd*  | pwndParent | A pointer to the parent of the list box 
// control.
//@parm  UINT  | uiID | The ID of the list box control to attach to.
//@parmopt  int | iFlags | LBE_DEFAULT | Flags indicating edit features 
// the list box should have.
//@xref <c SECListBoxDirEditor>

// adinsert AutoDuck insertion point for SECListBoxDirEditor 
//@doc SECListBoxDirEditor 
//@class The SECListBoxDirEditor class is an editable list box that is 
// customized for entering a list of directories.  An SECListBoxDirEditor 
// object enables users to enter directory pathnames by typing them or by 
// picking them from a directory selection dialog.  (The look and features 
// are similar to the Include and Library path editors accessible from 
// Visual C++’s "Tools->Options..." menu item.)  Most of the implementation, 
// look and behavior is inherited from its <c SECListBoxEditor> base class.  
// So, refer to the documentation for <c SECListBoxEditor> for a more complete 
// explanation of the features programming interface of an editable list box.
//
// SECListBoxDirEditor inherits most of its functionality from <c SECListBoxEditor>.  
// In fact, its only method is an override of OnBrowse, which launches a directory 
// selection dialog when the browse button is pressed.
//
// The SECListBoxDirEditor and <c SECBrowseDirEdit> classes are somewhat similar in 
// purpose: both allow directory names to be entered by hand or by dialog.  
// The distinction between them is that an SECListBoxDirEditor object is a list 
// box specialized for entering directory pathnames, whereas an <c SECBrowseDirEdit> 
// is an edit control specialized for the same purpose.
//
// See the LBEDIT sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\LBEDIT directory for a 
// demonstration of the SECListBoxDirEditor class.
//
//@base public | SECListBoxEditor
//@xref <c SECListBoxEditor> <c SECListBoxFileEditor> <c SECBrowseDirEdit>
class SECListBoxDirEditor : public SECListBoxEditor
{
protected:
	//@cmember
	/* Called when the browse button is clicked.*/
	virtual void OnBrowse();
public:

	//@cmember
	/* Attaches an SECListBoxDirEditor object to an existing list box control.*/
	virtual BOOL Initialize(CWnd* pwndParent, UINT uiID, int iFlags = LBE_DEFAULT);
	virtual BOOL Initialize(UINT uiID, CWnd* pwndParent, int iFlags = LBE_DEFAULT);

};

/////////////////////////////
// AutoDuck tag block block for SECListBoxFileEditor 
//@doc SECListBoxFileEditor
//@mdata CString  | SECListBoxFileEditor | m_strCaption | The selection 
// dialog’s caption text.
//@xref <c SECListBoxDirEditor>

//@doc SECListBoxFileEditor
//@mdata CString  | SECListBoxFileEditor | m_strFilters | The file filters.
//@xref <c SECListBoxDirEditor>

//@doc SECListBoxFileEditor
//@mdata CString  | SECListBoxFileEditor | m_strInitialFile | The initially 
// displayed filename.
//@xref <c SECListBoxDirEditor>

//@doc SECListBoxFileEditor
//@mdata CString  | SECListBoxFileEditor | m_strExt | The default filename 
// extension.
//@xref <c SECListBoxDirEditor>

//@doc SECListBoxFileEditor
//@mfunc void  | SECListBoxFileEditor | SetBrowseCaption | Modifies the 
// text used in the caption of the file selection dialog.  Note that the 
// new caption does not take effect until the next time the file selection 
// dialog is shown.
//@parm const TCHAR*  | pcszCaption | The file selection dialog’s caption.
//@xref <c SECListBoxDirEditor>

//@doc SECListBoxFileEditor
//@mfunc void  | SECListBoxFileEditor | SetFileFilters | A series of string 
// pairs that specify filters you can apply to the file.  If you specify 
// file filters, only selected files will appear in the files lib box.
//@parm const TCHAR*  | pcszFilters | The file filters to use.
//@xref <c SECListBoxDirEditor>


//@doc SECListBoxFileEditor
//@mfunc void  | SECListBoxFileEditor | SetDefaultExtension | Call this 
// member function to set the default filename extension to be used in the 
// file selection dialog. 
//@parm const TCHAR*  | pcszExt | The default filename extension.  If 
// the user does not include an extension in the filename edit box, the 
// extension specified by pcszExt is automatically appended to the filename.  If 
// this parameter is NULL, no file extension is appended.
//@xref <c SECListBoxDirEditor>

//@doc SECListBoxFileEditor
//@mfunc void  | SECListBoxFileEditor | SetInitialFileName | Call this 
// member function to set the initial filename display in the file selection 
// dialog box’s edit field. 
//@parm const TCHAR*  | pcszName | parm comment 
//@xref <c SECListBoxDirEditor>


//@doc SECListBoxFileEditor
//@mfunc virtual BOOL  | SECListBoxFileEditor | Initialize | Initializes 
// the SECListBoxFileEditor object by attaching to the list box control 
// identified by uiID.  This is done by subclassing the list box control 
// and attaching it to the instance of SECListBoxEditor for which 
// Initialize was called.<nl>
//@parm CWnd*  | pwndParent | Pointer to the parent of the list box 
// control to attach to.
//@parm  UINT  | uiID | ID of the list box control to attach to.
//@parmopt  int | iFlags | LBE_BROWSE | Flags indicating edit features 
// the list box should have.
//@xref <c SECListBoxDirEditor>


// adinsert AutoDuck insertion point for SECListBoxFileEditor 
//@doc SECListBoxFileEditor 
//@class The SECListBoxFileEditor class is an editable list box that is 
// customized for entering a list of filenames.  An SECListBoxFileEditor 
// object enables users to enter filenames by typing them or by picking 
// them from a file selection dialog.  Most of the implementation, look 
// and behavior is inherited from its <c SECListBoxEditor> base class.  So, 
// refer to the documentation for <c SECListBoxEditor> for a more complete 
// explanation of the features and programming interface of an editable 
// list box.
//
// The SECListBoxFileEditor and <c SECBrowseFileEdit> classes are somewhat 
// similar in purpose: both allow filenames to be entered by hand or by 
// dialog.  The distinction between them is that an SECListBoxFileEditor 
// object is a list box specialized for entering filenames, whereas an 
// <c SECBrowseFileEdit> is an edit control specialized for the same purpose.
//
// See the LBEDIT sample in the \SAMPLES\TOOLKIT\STANDARD\CONTROLS\LBEDIT directory for a 
// demonstration of the SECListBoxFileEditor class.
//
//@base public | SECListBoxEditor
//@xref <c SECListBoxEditor> <c SECListBoxDirEditor> <c SECBrowseFileEdit>
class SECListBoxFileEditor : public SECListBoxEditor
{

public:

	//@access Creation/Initialization
	
	//@cmember
	/* Attaches an SECListBoxFileEditor object to an existing list box control.*/
	virtual BOOL Initialize(CWnd* pwndParent, UINT uiID, int iFlags = LBE_DEFAULT);

	virtual BOOL Initialize(UINT uiID, CWnd* pwndParent, int iFlags = LBE_DEFAULT);

	//@access Attributes

	//@cmember
	/* Sets the text to use in the file selection dialog’s caption.*/
	void SetBrowseCaption(const TCHAR* pcszCaption)		
		{ m_strCaption = pcszCaption; }
	
	//@cmember
	/* Sets the file filters to be used in the file selection dialog.*/
	void SetFileFilters(const TCHAR* pcszFilters)		
		{ m_strFilters = pcszFilters; }
	
	//@cmember
	/* Sets the default filename extension to be used in the file */
	// selection dialog.
	void SetDefaultExtension(const TCHAR* pcszExt)		
		{ m_strExt = pcszExt; }
	
	//@cmember
	/* Sets the initial filename to be displayed in the file selection */
	// dialog.
	void SetInitialFileName(const TCHAR* pcszName)		
		{ m_strInitialFile = pcszName; }
	
protected:

	//@access Overridable

	//@cmember
	/* Called when the browse button is clicked.*/
	virtual void OnBrowse();

	//@access Protected data members

	//@cmember
	/* The selection dialog’s caption text.*/
	CString m_strCaption;

	//@cmember
	/* The file filters.*/
	CString m_strFilters;

	//@cmember
	/* The initially displayed filename.*/
	CString m_strInitialFile;

	//@cmember
	/* The default filename extension.*/
	CString m_strExt;

};

// windowsx.h defines SubclassWindow as a macro.  This
// definition conflicts with CWnd::SubclassWindow().
#ifdef SubclassWindow
#undef SubclassWindow
#endif

class SECListBoxEditorHelper : public CListBox
{
public:
	SECListBoxEditorHelper(SECListBoxEditor& lbe, CListBox* p);

protected:
	SECListBoxEditor& m_LBE;
	void OnBrowse()		{ m_LBE.OnBrowse(); }
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	BOOL m_bFocusLost;
protected:

	//{{AFX_MSG(SECListBoxEditorHelper)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT uiFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnEscapeState(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

class SECListBoxEdit : public CEdit
{
protected:
	enum {IDC_BUTTON = 100};
	BOOL m_bDelOnCancel;
	int m_iIndex;
	int m_iOldSel;
	UINT m_uiBrowseID;
	void DoNotify(BOOL bChanged);
	void OnKeyDownBase(UINT nChar, UINT nRepCnt, UINT nFlags);

	//{{AFX_MSG(SECListBoxEdit)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	SECListBoxEdit (CListBox* pLB, UINT uiID, int iIndex,
				 BOOL bDelOnCancel, UINT uiBrowseID,
				 LPCTSTR p = NULL, 
				 BOOL bQuick = FALSE, int nSpaces = 0);

protected:
	BOOL	mbQuick;
	int		mnSpaces;
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif

#endif // __LSTBXEDT_H__

