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
//  Author:             John Williston
//  Description:        Declarations for SECDirSelectDlg, SECBrowseEditBase
//							SECBrowseDirEdit and SECBrowseFileEdit
//  Created:            August 1996
//
//  Updating Author:	Justin Rudd
//  Update Description: Added support for Win32 common dialog for browsing for directories.
//						Added support for a new directory button ( only to Win16 dialog template ).
//
//  Updating Author:	Robert Oliver
//  Update Description: AutoDuck comments

#ifndef __BROWEDIT_H__
#define __BROWEDIT_H__

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

/////////////////////////////////////////////////////////////////////////////
// SECDirSelectDlg

/////////////////////////////
// AutoDuck tag block block for SECDirSelectDlg 
//@doc SECDirSelectDlg
//@mdata static DWORD  | SECDirSelectDlg | c_dwFlags | The common dialog flags 
// for the dialog.  

//@doc SECDirSelectDlg
//@mdata static const TCHAR*  | SECDirSelectDlg | c_pcszFilter | The file 
// filter string for the dialog.

//@doc SECDirSelectDlg
//@mdata WORD  | SECDirSelectDlg | m_wDialogStyle | The style flags for 
// the dialog.

//@doc SECDirSelectDlg
//@mdata TCHAR  | SECDirSelectDlg | m_szFileBuffer[_MAX_PATH] | A string 
// buffer for the file pathname.

//@doc SECDirSelectDlg
//@mdata CWnd*  | SECDirSelectDlg | m_pParentWnd | A pointer to the parent
// window of the dialog.


// adinsert AutoDuck insertion point for SECDirSelectDlg 
//@doc SECDirSelectDlg 
//@class This helper class provides directory selection under Win16 and
// Win32 by using a customization of the common dialogs.
//@base public | CFileDialog
class SECDirSelectDlg : public CFileDialog
{
public:
	
	//@cmember,menum
	/* Dialog styles*/
    enum DialogStyles
	{
		win16Style = 0x01,	//@@emem 16-bit Windows style
		win32Style = 0x02	//@@emem 32-bit Windows style
	} ;
	
	//@access Creation/initialization

	//@cmember
	/* Constructs a SECDirSelectDlg object.*/
	SECDirSelectDlg(LPCTSTR lpcszCaption, 
		            CWnd* pWndParent = NULL,
			        LPCTSTR lpcszInitialDir = NULL,
					DWORD dwFlags = 0);
	
	//@cmember
	/* Constructs a SECDirSelectDlg object.*/
	SECDirSelectDlg(WORD wDialogStyle = win16Style, 
		            LPCTSTR lpszCaption = NULL, 
					LPCTSTR lpszInitialDir = NULL, 
					CWnd* pParentWnd = NULL,
					DWORD dwFlags = 0);
	
	//@access Attributes

	//@cmember
	/* Sets the style flags for the dialog.*/
	WORD SetDialogStyle( const WORD& wNewStyle );
	
	//@cmember
	/* Retrieves the style flags set for this dialog.*/
	WORD QueryDialogStyle() const;

	//@cmember
	/* Sets the text to use in the directory selection dialog’s caption.*/
	void SetBrowseCaption( LPCTSTR lpszCaption );
	
	//@cmember
	/* Sets the initial directory to be displayed in the directory selection dialog.*/
	void SetInitialDir( LPCTSTR lpszCaption );

	//@cmember
	/* Retrieves the selected path.*/
	void GetPath(CString& strPath) const;
#ifdef WIN32

	//@access Operations

	//@cmember
	/* Invokes the modal dialog box (32-bit only).*/
	virtual int DoModal();

#endif //WIN32

protected:

	//@access Overridable

#ifdef WIN32

	//@cmember
	/* Locates the selected folder if it exists (32-bit only).*/
	virtual BOOL BrowseForFolder( CString& strPath );

#endif //WIN32

    //{{AFX_MSG(SECDirSelectDlg)
	//@cmember
	/* Called to initialize the dialog.*/
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP() 

protected:

	//@access Protected data members
	
	//@cmember
	/* The dialog style flags.*/
	WORD m_wDialogStyle;
	
	//@cmember
	/* A buffer containing the pathname.*/
	TCHAR m_szFileBuffer[_MAX_PATH];
	
	//@cmember
	/* A pointer to this dialog's parent window.*/
	CWnd* m_pParentWnd;

private:

	// Private data members

	// The dialog style flags.
	static DWORD c_dwFlags;

	// The file filter string.
	static const TCHAR* c_pcszFilter;

};



// The following forward reference and abstract base class are
// required for the directory and file browse-edit classes.

class SECBrowseButton;

/////////////////////////////
// AutoDuck tag block block for SECBrowseButton
//@doc SECBrowseEditBase
//@mdata SECBrowseButton*  | SECBrowseEditBase | m_pBrowseBtn | The pointer to the 
// browse button on the dialog.
//@xref <mf SECBrowseEditBase::GetBrowseButton>

//@doc SECBrowseEditBase
//@mdata CString  | SECBrowseEditBase | m_strCaption | The dialog's caption string.
//@xref <mf SECBrowseEditBase::GetBrowseCaption>

//@doc SECBrowseEditBase
//@mfunc void  | SECBrowseEditBase | SetBrowseCaption | Sets the text to be used 
// in the dialog’s caption which comes up in response to a button press.
//@parm LPCTSTR  | lpcszCaption | The new caption text.
//@xref <c SECBrowseEditBase>
//@xref <mf SECBrowseEditBase::GetBrowseCaption>

//@doc SECBrowseEditBase
//@mfunc LPCTSTR  | SECBrowseEditBase | GetBrowseCaption | Returns the text used 
// in the caption of the dialog which comes up in response to a button press. 
//@xref <mf SECBrowseEditBase::SetBrowseCaption>

//@doc SECBrowseEditBase
//@mfunc SECBrowseButton*  | SECBrowseEditBase | GetBrowseButton | Retrieves the 
// browse button.

// adinsert AutoDuck insertion point for SECBrowseEditBase
//@doc SECBrowseEditBase
//@class SECBrowseEditBase is an abstract base class that provides the interface 
// and some of the functionality of a browse edit control.  A browse edit is a 
// Windows edit control with a browse button positioned immediately to its right.  
// A browse button is a push button labeled "..." which pops up a dialog when pressed.  
// This modal dialog presents the user with possible values for the edit field and 
// accepts the user’s choice.  After the user chooses, the selection is entered into 
// the text field automatically.  Of course, the user can still type the value in 
// directly if they prefer.  The browse button is meant to help the user enter a value 
// if they are unsure what values are legal or believe it would be faster to pick the 
// value from a dialog than to type it in.
//
// To use this class, derive your own class from SECBrowseEditBase and override the 
// OnBrowse method.  The text field and browse button will be created and positioned 
// for you by the SECBrowseEditBase base class.  But it is the responsibility of your 
// derived class to define the response to a browse button press.  The OnBrowse method 
// is automatically called when the browse button is pressed, so override this method 
// and code your response.  <c SECBrowseFileEdit> and <c SECBrowseDirEdit> are two classes which 
// derive from SECBrowseEditBase, so refer to their implementations for an example of how 
// this is done.
//
// See the TOOLMENU sample in the ...\SAMPLES\TOOLKIT\STANDARD\UIEXT\TOOLMENU directory 
// for a demonstration of SECBrowseEditBase.
//@base public | CEdit
class SECBrowseEditBase : public CEdit
{
public:
	
	//@access Creation/initialization
	
	//@cmember
	/* Constructs a SECBrowseEditBase object.*/
    SECBrowseEditBase();
	
	// Destructor.
    virtual ~SECBrowseEditBase();
	
	//@cmember
	/* Attaches an SECBrowseEditBase object to an existing */
	// edit control.
	BOOL Initialize(UINT uiID, CWnd* pWndParent);

public:

	//@access Attributes

	//@cmember
	/* Sets dialog caption text*/
	void SetBrowseCaption(LPCTSTR lpcszCaption)	
		{ m_strCaption = lpcszCaption; }
	
	//@cmember
	/* Retrieves the dialog caption text.*/
	LPCTSTR GetBrowseCaption() const
		{ return m_strCaption; }

	//@cmember
	/* Retrieves the browse button.*/
	SECBrowseButton* GetBrowseButton() const		
		{ return m_pBrowseBtn; }

public:

	//@access Overridable

	//@cmember
	/* Override to customize the response to a button press.*/
	virtual void OnBrowse() = 0;
	
protected:
	
	// access Overrides

	// Called when the enable state of the dialog changes.
	afx_msg void OnEnable(BOOL bEnable);
	
	// Called when the dialog is hidden or made visible.
	afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
	
	// Called when the dialog is about to be destroyed.
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	//@cmember
	/* Retrieves the pointer to the window procedure.*/
	virtual WNDPROC* GetSuperWndProcAddr();

protected:

	//@access Protected data members
	
	//@cmember
	/* The browse button pointer.*/
	SECBrowseButton* m_pBrowseBtn;
	
	//@cmember
	/* The dialog's caption string.*/
	CString m_strCaption;

};



// The following provides directory browsing from an edit control.

/////////////////////////////
// AutoDuck tag block block for SECBrowseDirEdit 

//@doc SECBrowseDirEdit
//@mdata WORD  | SECBrowseDirEdit | m_wDialogStyle | The dialog style flags.
// When dialog styles are determined by <md SECBrowseDirEdit::DlgStyles>

// adinsert AutoDuck insertion point for SECBrowseDirEdit 
//@doc SECBrowseDirEdit 
//@class The SECBrowseDirEdit class provides the functionality of a directory 
// edit control.  A directory edit is a browse edit that is specially suited
// for entering a directory pathname.  (See the <c SECBrowseEditBase> class overview
// for a description of a browse edit.)  With a directory edit, the user can type
// in a directory name directly or can pick a directory from a dialog.  When the
// user presses the browse button, a modal directory selection dialog appears.
// After the user picks a directory from the dialog, its pathname is automatically
// entered into the edit field.
//
// SECBrowseDirEdit inherits most of its functionality from <c SECBrowseEditBase>.
// In fact, its only method is an override of OnBrowse, which launches a directory
// selection dialog when the browse button is pressed.
//
// The SECBrowseDirEdit and <c SECListBoxDirEditor> classes are somewhat similar in
// purpose--both allow directory names to be entered by hand or by dialog.  The
// distinction between them is that an SECBrowseDirEdit object is an edit control
// specialized for entering directory pathnames, whereas an <c SECListBoxDirEditor> is 
// a list box specialized for the same purpose.
//
// See the TOOLMENU sample in the ...\SAMPLES\STANDARD\UIEXT\TOOLMENU directory 
// for a demonstration of the SECBrowseDirEdit class.
//@base public | SECBrowseEditBase
class SECBrowseDirEdit : public SECBrowseEditBase
{
public:

	//@access Creation/initialization

	//@cmember
	/* Constructs a SECBrowseDirEdit object.*/
	SECBrowseDirEdit();

	//@cmember,menum
	/* Dialog styles*/
	enum DlgStyles
	{ 
		win16Style = 0x1,	//@@emem 16-bit Windows style.
		win32Style = 0x2	//@@emem 32-bit Windows style.
	} ;

	//@access Attributes

	//@cmember
	/* Sets the dialog style flags*/
	void SetDialogStyle( WORD wStyle );

	//@cmember
	/* Retrieves the dialog style flags*/
	WORD GetDialogStyle() const;

public:

	//@access Operations

	//@cmember
	/* Launches the directory selection dialog*/
	virtual void OnBrowse();

protected:

	//@access Protected data members

	//@cmember
	/* Dialog style flags*/
	WORD m_wDialogStyle;
};



// The following provides file browsing from an edit control.

/////////////////////////////
// AutoDuck tag block block for SECBrowseFileEdit 
//@doc SECBrowseFileEdit
//@mdata CString  | SECBrowseFileEdit | m_strFilters | The file filters for 
// limiting displayed files.
//@xref <mf SECBrowseFileEdit::SetFileFilters>

//@doc SECBrowseFileEdit
//@mdata CString  | SECBrowseFileEdit | m_strInitialFile | The initial filename.
//@xref <mf SECBrowseFileEdit::SetInitialFileName>

//@doc SECBrowseFileEdit
//@mdata CString  | SECBrowseFileEdit | m_strExt | The default file extension.
//@xref <mf SECBrowseFileEdit::SetDefaultExtension>

//@doc SECBrowseFileEdit
//@mdata DWORD    | SECBrowseFileEdit | m_dwDialogStyle | The dialog style flags.
//@xref <mf SECBrowseFileEdit::ModifyDialogStyle>

//@doc SECBrowseFileEdit
//@mfunc void  | SECBrowseFileEdit | SetFileFilters | Sets the file filter string.
//@parm const TCHAR*  | pcszFilters | A string containing the file filters.

//@doc SECBrowseFileEdit
//@mfunc void  | SECBrowseFileEdit | SetDefaultExtension | Sets the default file extension.
//@parm const TCHAR*  | pcszExt | A string containing the file extension.

//@doc SECBrowseFileEdit
//@mfunc void  | SECBrowseFileEdit | SetInitialFileName | Sets the initial filename.
//@parm const TCHAR*  | pcszName | A string containing the initial filename.

// adinsert AutoDuck insertion point for SECBrowseFileEdit 
//@doc SECBrowseFileEdit 
//@class The SECBrowseFileEdit class provides the functionality of a filename 
// edit control.  A filename edit is a browse edit that is specially suited 
// for entering a filename.  (See the <c SECBrowseEditBase> class overview for a 
// description of a browse edit.)  With a filename edit, the user can type a 
// filename in directly or they can pick a filename from a dialog.  When the 
// user presses the browse button, a modal file dialog appears.  After the user 
// picks a filename from the dialog, the full filename is automatically entered 
// into the edit field.
//
// The SECBrowseFileEdit and <c SECListBoxFileEditor> classes are somewhat similar 
// in purpose--both allow filenames to be entered by hand or by dialog.  The 
// distinction between them is that an SECBrowseFileEdit object is an edit 
// control specialized for entering filenames, whereas an <c SECListBoxFileEditor> 
// is a list box specialized for the same purpose.
//
// See the TOOLMENU sample in the ...\SAMPLES\STANDARD\UIEXT\TOOLMENU directory for a 
// demonstration of the SECBrowseFileEdit class.
//@base public | SECBrowseEditBase
class SECBrowseFileEdit : public SECBrowseEditBase
{

public:                           
	
	//@access Creation/initializers
	
	//@cmember
	/* Constructs a SECBrowseFileEdit object.*/
	SECBrowseFileEdit();

	//@access Attributes

	//@cmember
	/* Sets the file filter string.*/
	void SetFileFilters(const TCHAR* pcszFilters)
		{ m_strFilters = pcszFilters; }
	
	//@cmember
	/* Sets the default file extension.*/
	void SetDefaultExtension(const TCHAR* pcszExt)		
		{ m_strExt = pcszExt; }
	
	//@cmember
	/* Sets the initial filename.*/
	void SetInitialFileName(const TCHAR* pcszName)		
		{ m_strInitialFile = pcszName; }
	
	//@cmember
	/* Modifies the current dialog style flags.*/
	void ModifyDialogStyle( DWORD dwRemove, DWORD dwAdd );

	//@access Operations

	//@cmember
	/* Launches the file selection dialog.*/
	virtual void OnBrowse();

protected:

	//@access Protected data members

	//@cmember
	/* The current file filters.*/
	CString m_strFilters;
	
	//@cmember
	/* The initial filename.*/
	CString m_strInitialFile;
	
	//@cmember
	/* The default file extension.*/
	CString m_strExt;
	
	//@cmember
	/* The dialog style.*/
	DWORD   m_dwDialogStyle;

};

/////////////////////////////
// AutoDuck tag block for SECBrowseButton 
//@doc SECBrowseButton
//@mdata SECBrowseEditBase*  | SECBrowseButton | m_pEdtBrowse | Pointer to the
// browse edit control to which this button belongs.

//@doc SECBrowseButton
//@mdata UINT  | SECBrowseButton | m_uiID | Control id for this button.  An
// unused id is scanned for to generate this id.

// adinsert AutoDuck insertion point for SECBrowseButton 
//@doc SECBrowseButton 
//@class The browse button helper class is the class that actually provides
// service when clicked.
//@base public | CButton
class SECBrowseButton : public CButton
{

	//@access Creation/initialization

public:

	//@cmember
	/* Creates a SECBrowseButton object.*/
	SECBrowseButton(SECBrowseEditBase* pEdtBrowse);

	//@cmember
	/* SECBrowseButton destructor.*/
	~SECBrowseButton();

	//@cmember
	/* Creates the browse button control.*/
	BOOL Create();

	//@access Overridables

protected:
    //{{AFX_MSG(SECBrowseButton)
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
	//@cmember
	/* Called by the default OnNcDestroy member function after the */
	// window has been destroyed. 
	virtual void PostNcDestroy();

    //@cmember
    /* Called by this window's parent window when it receives a */
    // notification message that applies to this window.
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);


	// Button width enumeration.
	enum { 
		BTN_WIDTH = 25		// emem Button width.
	} ;

	// Pointer to parent control.
	SECBrowseEditBase* m_pEdtBrowse;
	
	// control ID for this button.
	UINT m_uiID;
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __BROWEDIT_H__

