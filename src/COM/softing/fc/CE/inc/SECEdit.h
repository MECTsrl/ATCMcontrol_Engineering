#ifndef __SECEDIT_H__
#define __SECEDIT_H__

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL

// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------

#include "editres.h"
#include "struct.h"
#include <editres.h>
#include <regex.h>
#include <oepageset.h>
#include <secedithint.h>

#include <errno.h>
#include <afxole.h> 

#if _MFC_VER < 0x0420
	#ifndef USES_CONVERSION_T
		#define USES_CONVERSION_T
	#endif

	// Pseudo-Unicode not supported under VC 4.0 and 4.1
	// The A2T macros are used in OE, and are defined in
	// VC 4.0 and 4.1, but are not used at all. They must be
	// included for the libraries to build, though

	#ifndef A2T
		#define A2T(x) (x)
	#endif

	#ifndef T2A
		#define T2A(x) (x)
	#endif

	// These two are not defined in VC 4.0 and 4.1
	// but somebody may have their own impl
	#ifndef A2CT
		#define A2CT(x) (x)
	#endif

	#ifndef T2CA
		#define T2CA(x) (x)
	#endif

	#ifdef _UNICODE
		#error Unicode only supported in VC 4.2 and higher for Objective Edit.
	#endif

#else // _MFC_VER >= 0x0420
	#include <AfxConv.h> // for USES_CONVERSION_T
	#include <malloc.h>  // for USES_CONVERSION_T

	#ifndef USES_CONVERSION_T // Does nothing in ANSI builds
		#ifdef _UNICODE
			#define USES_CONVERSION_T USES_CONVERSION
		#else
			#define USES_CONVERSION_T
		#endif // _UNICODE
	#endif	// USES_CONVERSION_T
#endif

// AUTOFMT 08.08.06 SIS >>
#ifndef ORIGINAL_VERSION

#define OE_REGKEY_EXTRAWORDCHARS    _T("Extra word chars")
#define OE_REGKEY_TERMINATORCHARS   _T("Terminator chars")
#define OE_REGKEY_STARTCHARS        _T("Start chars")
#define OE_REGKEY_KEYWORDFILE       _T("Keywords")
#define OE_REGKEY_COMMENTSTART1     _T("Comment Start 1")
#define OE_REGKEY_COMMENTEND1       _T("Comment End 1")
#define OE_REGKEY_COMMENTSTART2     _T("Comment Start 2")
#define OE_REGKEY_COMMENTEND2       _T("Comment End 2")
#define OE_REGKEY_FONTNAME          _T("Font name")
#define OE_REGKEY_FONTSIZE          _T("Font size")
#define OE_REGKEY_CHARSET           _T("CharSet")
#define OE_REGKEY_FONTWEIGHT        _T("Font weight")
#define OE_REGKEY_FONTITALIC        _T("Font Italic")
#define OE_REGKEY_TEXTCOLDEFAULT    _T("Default Text")
#define OE_REGKEY_TEXTCOLKEYWORD    _T("Keyword")
#define OE_REGKEY_TEXTCOLCOMMENT    _T("Comment")
#define OE_REGKEY_TEXTCOLQUOTE      _T("Quote")
#define OE_REGKEY_TEXTCOLSQUOTE     _T("SQuote")
#define OE_REGKEY_BACKGROUNDCOL     _T("Background")
#define OE_REGKEY_CHROMA            _T("Chroma")
#define OE_REGKEY_AUTOINDENT        _T("Autoindent")
#define OE_REGKEY_CASE              _T("Case")
#define OE_REGKEY_KEEPTABS          _T("Keep tabs")
#define OE_REGKEY_SHOWTABS          _T("Show tabs")
#define OE_REGKEY_TABSIZE           _T("Tab size")
#define OE_REGKEY_WORDWRAP          _T("Word wrap")
#define OE_REGKEY_INSERT            _T("Insert")
#define OE_REGKEY_LOCKFILES         _T("Lock files")

#define OE_FLAG_EXTRAWORDCHARS  0x00000001
#define OE_FLAG_TERMINATORCHARS 0x00000002
#define OE_FLAG_STARTCHARS      0x00000004
#define OE_FLAG_KEYWORDFILE     0x00000008
#define OE_FLAG_COMMENTSTART1   0x00000010
#define OE_FLAG_COMMENTEND1     0x00000020
#define OE_FLAG_COMMENTSTART2   0x00000040
#define OE_FLAG_COMMENTEND2     0x00000080
#define OE_FLAG_FONTNAME        0x00000100
#define OE_FLAG_FONTSIZE        0x00000200
#define OE_FLAG_CHARSET         0x00000400
#define OE_FLAG_FONTWEIGHT      0x00000800
#define OE_FLAG_FONTITALIC      0x00001000
#define OE_FLAG_TEXTCOLDEFAULT  0x00002000
#define OE_FLAG_TEXTCOLKEYWORD  0x00004000
#define OE_FLAG_TEXTCOLCOMMENT  0x00008000
#define OE_FLAG_TEXTCOLQUOTE    0x00010000
#define OE_FLAG_TEXTCOLSQUOTE   0x00020000
#define OE_FLAG_BACKGROUNDCOL   0x00040000
#define OE_FLAG_CHROMA          0x00080000
#define OE_FLAG_AUTOINDENT      0x00100000
#define OE_FLAG_CASE            0x00200000
#define OE_FLAG_KEEPTABS        0x00400000
#define OE_FLAG_SHOWTABS        0x00800000
#define OE_FLAG_TABSIZE         0x01000000
#define OE_FLAG_WORDWRAP        0x02000000
#define OE_FLAG_INSERT          0x04000000
#define OE_FLAG_LOCKFILES       0x08000000

#define UNUSED_STRING           _T("Unused")
#define UNUSED_VALUE            -1

class SECEditSettings
{
public:
    SECEditSettings(const CString& crsFileMappingKey = _T(""));
    ~SECEditSettings();

    void        SetFileMappingKey(const CString& crsFileMappingKey);
    CString     GetFileMappingKey();

    BOOL        IsInitMode();
    void        SetInitMode(BOOL bInitMode);        // if TRUE, changed flags will not be set

    BOOL        LoadFromRegistry();                 // load setting from registry and set used flags
    BOOL        SaveToRegistry();                   // save valid settings to registry

    BOOL        IsUsed(DWORD dwFlag);
    DWORD       GetUsedFlags();
//    void        SetUsedFlags(DWORD dwFlags);

    BOOL        IsChanged(DWORD dwFlag);
    DWORD       GetChangedFlags();
//    void        SetChangedFlags(DWORD dwFlags);

    // general settings
    CString     GetExtraWordChars();
    void        SetExtraWordChars(const CString& crsExtraWordChars);
    CString     GetTerminatorChars();
    void        SetTerminatorChars(const CString& crsTerminatorChars);
    CString     GetStartChars();
    void        SetStartChars(const CString& crsStartChars);
    CString     GetKeywordFile();
    void        SetKeywordFile(const CString& crsKeywordFile);

    // comment settings
    CString     GetCommentStart1();
    void        SetCommentStart1(const CString& crsCommentStart1);
    CString     GetCommentEnd1();
    void        SetCommentEnd1(const CString& crsCommentEnd1);
    CString     GetCommentStart2();
    void        SetCommentStart2(const CString& crsCommentStart2);
    CString     GetCommentEnd2();
    void        SetCommentEnd2(const CString& crsCommentEnd2);
    
    // font settings
    CString     GetFontName();
    void        SetFontName(const CString& crsFontName);
    int         GetFontSize();
    void        SetFontSize(int iFontSize);
    int         GetCharSet();
    void        SetCharSet(int iCharSet);
    int         GetFontWeight();
    void        SetFontWeight(int iFontWeight);
    BOOL        GetFontItalic();
    void        SetFontItalic(BOOL bFontItalic);

    // color settings
    COLORREF    GetTextColDefault();
    void        SetTextColDefault(COLORREF crTextColDefault);
    COLORREF    GetTextColKeyword();
    void        SetTextColKeyword(COLORREF crTextColKeyword);
    COLORREF    GetTextColComment();
    void        SetTextColComment(COLORREF crTextColComment);
    COLORREF    GetTextColQuote();
    void        SetTextColQuote(COLORREF crTextColQuote);
    COLORREF    GetTextColSQuote();
    void        SetTextColSQuote(COLORREF crTextColSQuote);
    COLORREF    GetBackgroundCol();
    void        SetBackgroundCol(COLORREF crBackgroundCol);

    // switches and corresponding settings
    BOOL        GetChroma();
    void        SetChroma(BOOL bChroma);
    BOOL        GetAutoIndent();
    void        SetAutoIndent(BOOL bAutoIndent);
    BOOL        GetCase();
    void        SetCase(BOOL bCase);
    BOOL        GetKeepTabs();
    void        SetKeepTabs(BOOL bKeepTabs);
    BOOL        GetShowTabs();
    void        SetShowTabs(BOOL bShowTabs);
    int         GetTabSize();
    void        SetTabSize(int iTabSize);
    BOOL        GetWordWrap();
    void        SetWordWrap(BOOL bWordWrap);
    BOOL        GetInsert();
    void        SetInsert(BOOL bInsert);
    BOOL        GetLockFiles();
    void        SetLockFiles(BOOL bLockFiles);

protected:
    BOOL        m_bInitMode;            // if TRUE, changed flags will not be set
    CString     m_sFileMappingKey;      // registry section
    
    CString     m_sExtraWordChars;      // extra word characters for chroma
    CString     m_sTerminatorChars;     // terminator character for chroma
    CString     m_sStartChars;          // start characters for chroma
    CString     m_sKeywordFile;         // keyword file name
    CString     m_sCommentStart1;       // comment start sequence 1
    CString     m_sCommentEnd1;         // comment end sequence 1
    CString     m_sCommentStart2;       // comment start sequence 2
    CString     m_sCommentEnd2;         // comment end sequence 2
    CString     m_sFontName;            // font name (see LOGFONT)
    int         m_iFontSize;            // font size (see LOGFONT)
    int         m_iCharSet;             // character set (see LOGFONT)
    int         m_iFontWeight;          // font weight (see LOGFONT)
    BOOL        m_bFontItalic;          // font italic?
    COLORREF    m_crTextColDefault;     // default text color
    COLORREF    m_crTextColKeyword;     // text color for keywords
    COLORREF    m_crTextColComment;     // text color for comments
    COLORREF    m_crTextColQuote;       // text color for quotes
    COLORREF    m_crTextColSQuote;      // text color for s-quotes (?)
    COLORREF    m_crBackgroundCol;      // background color
    BOOL        m_bChroma;              // use syntax coloring?
    BOOL        m_bAutoIndent;          // auto indent active?
    BOOL        m_bCase;                // syntax coloring case sensitive?
    BOOL        m_bKeepTabs;            // keep tabs or fill with spaces?
    BOOL        m_bShowTabs;            // show tabs?
    int         m_iTabSize;             // tab size
    BOOL        m_bWordWrap;            // word wrap mode?
    BOOL        m_bInsert;              // insert mode?
    BOOL        m_bLockFiles;           // lock files?

    DWORD       m_dwUsed;               // used flags: set by LoadFromRegistry
    DWORD       m_dwChanged;            // used flags: set by Set functions, if value changed
};
#endif
// AUTOFMT 08.08.06 SIS <<

//----------------------------------------------------------------------------
//
// general defines
//
//
//----------------------------------------------------------------------------
// It is safe to define this to a lower value.
// DevStudio uses 2084
#define OE_MAXLINELENGTH 16384
#define OE_SUCCESS 0

#define OE_SYNCH_SPLITTERS 49054 // N49054 Plane I 1st soloed in 1995, C-152

//----------------------------------------------------------------------------
//
// defines for Undo
//
//----------------------------------------------------------------------------
#define OE_MAXUNDO 2048

#define OE_UNDO_INSERT       1
#define OE_UNDO_DELETE       2
#define OE_UNDO_CUT          3
#define OE_UNDO_PASTE        4
#define OE_UNDO_REPLACE      5
#define OE_UNDO_NEWLINE      6
#define OE_UNDO_JOINLINE     7
#define OE_UNDO_INS_OVER     8
#define OE_UNDO_COLUMNCUT    9
#define OE_UNDO_COLUMNPASTE 10
#define OE_UNDO_MERGE       11
// 
// global non-class functions
//
LPVOID SECEditAlloc(int iSize);

// KEYWORDARRAYSIZE MUST BE A POWER OF 2
#define KEYWORDARRAYSIZE 2048

#define MYMAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))

#define KEYWORDSIZE 24
//struct to hold keywords
typedef struct tagCKeyWord
   {
   char szWord[KEYWORDSIZE];
   int iLength;
	COLORREF cr;
   } CKEYWORD;
   
typedef CKEYWORD* LPCKEYWORD;

//----------------------------------------------------------------------------
//
// defines for line end types
//
//----------------------------------------------------------------------------
#define OE_CRLF 0
#define OE_LFCR 1
#define OE_LF   2
#define OE_CR   3


//----------------------------------------------------------------------------
//  class SECEdit : interface of the SECEdit class
//  Author:		SteveS
//
//  SECEdit has no visual interface and no position information
//  is maintained, but this is where the text data is maintained.
//
//  SECEdit member functions optionally maintain Undo information and
//  generally return 0 or a pointer for success, nonzero error code or a 
//  NULL pointer for failure.
//
//----------------------------------------------------------------------------

//@doc SECEdit
//@class SECEdit The Objective Edit SECEdit class has no visual interface and no position information
//  is maintained, but this is where the text data is maintained as well as syntax highlighting information.
//
//  SECEdit member functions optionally maintain Undo information and
//  generally return 0 or a pointer for success, nonzero error code or a 
//  NULL pointer for failure.  The majority of the members in this class are implementation
//  specific, but we have documented them for your information in case you need to customize
//  the behavior of Objective Edit.

class SECEdit
{
public:

	//MARK - construction/deletion
	//@cmember The SECEdit constructor.
	SECEdit();

	//@cmember The SECEdit destructor. 
	~SECEdit();

	//@cmember Initializes the SECEdit fields.
	void InitEdit();

	//@cmember Clears all internal SECEdit members.
	void ClearEdit();

	//@cmember Initializes the internal file type settings.
	int InitFileTypeSettings();

	//@cmember Clear the file handles.
	void ClearFileHandle(void);

	//MARK  - Get/Set accessors.
	//@cmember Get the state of the modified flag.
	virtual BOOL GetModified(void);	

	//@cmember Set the state of the modified flag.
	virtual void SetModified(BOOL bOn);


   	//@cmember Set the file locking status
	virtual void SetFileLocking(BOOL bOn);

	//@cmember Get the file locking status
	virtual BOOL GetFileLocking(void);


	//@cmember Set read only flag.
	virtual void SetReadOnly(BOOL bOn);

	//@cmember Get read only flag status.
	virtual BOOL GetReadOnly(void);

	// Not implemented
	virtual void SetWordWrap(BOOL bOn);
	virtual BOOL GetWordWrap(void);
	virtual void SetInsertMode(BOOL bOn);
	virtual BOOL GetInsertMode(void);

	//@cmember Set the tab size.
	virtual void SetTabSize(int iSize);

	//@cmember Get the tab size.
	virtual int GetTabSize(void);


	//MARK - Internal operations - public - line operations.

	//@cmember Gets the length of the largest line.
   int GetLineEndLength(void);


   	//@cmember Internal method for searching.
	BOOL InOrder(PTEXTPOS pFirst, PTEXTPOS pSecond);	
	
	//@cmember Internal method that performs line calculations
    int GetRealOffset(PLINEDESC pLine, int iPos);

   	//@cmember Internal method that calculates tab offsets.
    int GetTabbedOffset(PLINEDESC pLine, int iPos);

	//@cmember Internal method that finds the end of the current line.
	int FindLineEnd(LPSTR lpString, int iLength);

	//@cmember Internal method to convert line ends (andles the OE_CR/OE_LF problem,etc..)
    int ConvertLineEnds(LPSTR* lpText, int iBufferSize, int iLineEndType);

   	//@cmember Internal method that Clears the internal line list.
	int FreeLineList(PLINEDESC pLine);

	//@cmember Internal method. Returns the expanded text offset of an actual position.
	int GetTabbedLength(LPSTR pTextBuffer, int iPos);

	//@cmember Internal method that computes the maximum tabbed characters that are less than OE_MAXLINELENGTH.
	int GetMaxTabbedLength(LPSTR pTextBuffer);

	//@cmember Internal method that creates a line structure from a buffer.
	PLINEDESC LineFromTextBuffer(LPSTR lpTextBuffer);

	//@cmember Internal method for accessing the internal line list.
	PLINEDESC ListGetFirst(PLINEDESC lpSpot, int* i);

	//@cmember Internal method for accessing the internal line list.
   PLINEDESC ListGetLast(PLINEDESC lpSpot);

   	//@cmember Internal method that counds the number of lines currently loaded.
	int CountLines(PLINEDESC pLine);

	//@cmember Internal method that when given a line structure returns the location in the internal line list.
	int GetLineNo(PLINEDESC pLine);

	//@cmember Internal method that goes to a given line in memory.
	PLINEDESC GoToLineNo(int iLineNo);

	//@cmember Internal method that automatically replaces tabs with spaces.
    LPSTR ExpandTabs(LPSTR lpText,int iTextSize, int iTabSize, int iLineEndLength, int *iReturnSize);

	//MARK - internal stuff to do with undo/redo buffers.

	//@cmember Clears the undo buffer.
	void FreeUndoBuffers(void);

	//@cmember Initializes the under buffer.
	void InitUndo(void);

	//@cmember Internal method that saves a transation to the undo buffer.
	BOOL SaveUndoRecord(PUNDORECORD pUndoRecord, LPCSTR lpText);

	//@cmember Begins grouping of undo records for a transaction
	void SetUndoGroup(BOOL bStart = FALSE);

	//@cmember Internal method that retrieves the timestamp of an undo record
	DWORD GetUndoTimestamp();

	//@cmember Performs an UNDO operation.
	BOOL UndoEdit();

	//@cmember Performs a REDO operation.
	BOOL RedoEdit();

	//MARK - internal stuff for cut/copy/paste.

	//@cmember Internal method that inserts a character into a given line structure and position.
	int InsertChar(PTEXTPOS pTextPos, int iChar, BOOL bInsert, BOOL bSaveUndoRecord);

	//@cmember Internal method that inserts a string into a given line structure and position.
	int InsertString(PTEXTPOS pTextPos, LPSTR lpString, BOOL bInsert, BOOL bSaveUndoRecord);

	//@cmember Internal method that joins two lines together.
	int JoinLines(PTEXTPOS pTextPos, BOOL bSaveUndoRecord);

	//@cmember Internal method that deletes a character.
	int DeleteChar(PTEXTPOS pTextPos, BOOL bSaveUndoRecord);

	//@cmember Internal method that deletes a string.
	int DeleteString(PTEXTPOS pTextPos, int iCount, BOOL bSaveUndoRecord);

	//@cmember Internal method used in the implementation of "Cutting".
	int Cut(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord);

	//@cmember Internal method used in the implementation of cutting a column of text.
	int ColumnCut(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord);

	//@cmember Internal method used in the implementation of copy operations.
	LPSTR Copy(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord);

	//@cmember Internal method used in the implementation of copying of columns.
	LPSTR ColumnCopy(PTEXTPOS pStartPos, PTEXTPOS pEndPos, BOOL bSaveUndoRecord);

	//@cmember Internal method used in the implementation of paste operations.
	int Paste(PTEXTPOS, LPSTR lpText, int iSize, BOOL bSaveUndoRecord);

	//@cmember Internal method used in the implementation of column pasting.
	int ColumnPaste(PTEXTPOS, LPSTR lpText, int iSize, BOOL bSaveUndoRecord);

	//MARK - internal file operations are here.

	//@cmember Internal method that sets the type of the end of line.
	void SetLineEndType(int iLineEnd);

	//@cmember Reads a file.
	int ReadFile(LPCTSTR lpName);

	//@cmember Writes a file.
	int WriteFile(LPCTSTR lpName);

	//@cmember Renames a file.
	int RenameFile(LPCTSTR lpOldName, LPCTSTR lpNewName);
		
	//@cmember Internal method that merges two files.
	int MergeFile(PTEXTPOS pTextPos, LPCSTR lpName, BOOL bSaveUndoRecord);
 
	//@cmember Internal function that splits a line into two lines.
	int SplitLine(PTEXTPOS pTextPos, BOOL bSaveUndoRecord);

	//@cmember Internal function that maintains a file mapping table.
	void SetFileMappingKey(CString strSection);

	//@cmember Internal function that maintains a file mapping table.
	void SetFileMappingKey();

// AUTOFMT 11.08.06 SIS >>
#ifndef ORIGINAL_VERSION
    CString GetFileMappingKey();
#endif
// AUTOFMT 11.08.06 SIS <<

	//MARK - internal functions for keywords and stuff.
	
	//@cmember Internal function
	void SetExtraChars();

	//@cmember Internal function used to find the start of a comment.
	virtual int CommentStart(LPSTR s, int iLength, int *iType);

	//@cmember Internal function used to find the end of a comment.
	virtual int CommentEnd(LPSTR s, int iLength, int iType);

	//@cmember Internal function used to determine the type of a character.
	BOOL Myisalnum(int iChar);

	//@cmember Internal function that identifies if the current word is a keyword or not (by looking up the word in the internal keyword table).
	virtual BOOL KeyWord (LPSTR *s, int *iStart, int *iEnd);

	//Internal function that looks up the defined COLORREF for the passed in keyword.
	COLORREF GetColorRef(LPSTR lpWord);

	//@cmember Internal function that loads keywords.
	virtual BOOL GetKeyWords();

	//@cmember Internal function that determins if the given line is in a comment.
	virtual BOOL _InComment(PLINEDESC lpStartLine, LPSTR lpCommentStart, LPSTR lpCommentEnd,
						LPSTR lpSingleLineComment);

	//@cmember Internal function that determins if the given line is in a comment.
	virtual BOOL InComment(PLINEDESC lpStartLine, int *iType);

	//@cmember Internal function that determines if the given string is quoted or not.
	BOOL InQuote(LPSTR lpString, LPSTR lpSpot);

	//@cmember Initializes the internal color syntax highlighting.
	virtual void InitChroma();

	//@cmember Internal function used to find a string in the memory bufffer.
	BOOL Find(PTEXTPOS pStartPos, PTEXTPOS pEndPos, PSEARCHRECORD pSearch);

private:
	//@cmember Internal function used to add a new line.
	PLINEDESC MakeNewLine(int iTextSize);

	//@cmember Internal function frees a line structure.
	int FreeTextLine(PLINEDESC pLine);

	//@cmember Internal function that resizes a line structure.
	BOOL ResizeLine(PLINEDESC pLine, int iSize);

	//@cmember Internal function that inserts data into the internal line list.
   int ListInsertBefore(PLINEDESC lpOne, PLINEDESC lpTwo);

   //@cmember Internal function that inserts data into the internal line list.
   int ListInsertAfter(PLINEDESC lpOne, PLINEDESC lpTwo);

   	//@cmember Internal function that unlinks the internal line list.
   int ListUnlink(PLINEDESC lpSpot);

   	//@cmember Internal function that frees a node in the internal line list.
   int LineListFree(PLINEDESC pLine);

	//@cmember Internal function that frees space on the undo stack.
	BOOL ShrinkUndoStack(void);

	//@cmember Internal function that pads a buffer with spaces.
	int PadWithSpaces(PTEXTPOS pTextPos, BOOL bSaveUndoRecord);

	//@cmember Internal function that checks a buffer for NULL
	int CheckForNull(LPSTR lpBuffer, int iTextSize, int iReplace);

	//@cmember Internal function
	int BuildText(HANDLE fh, LPSTR lpBuffer,int iFileBufferSize);

// AUTOFMT 09.08.06 SIS >>
// made StringFromCOLORREF and COLORREFFromString public
#ifdef ORIGINAL_VERSION
	//@cmember Internal function that gives a color for a given string (syntax highlighting)
	static void StringFromCOLORREF(COLORREF cr, LPSTR lpString);

	//@cmember Internal function that convers a string into a color.
	static COLORREF COLORREFFromString(LPCSTR lpString);
#else
public:
	//@cmember Internal function that gives a color for a given string (syntax highlighting)
	static void StringFromCOLORREF(COLORREF cr, LPSTR lpString);

	//@cmember Internal function that convers a string into a color.
	static COLORREF COLORREFFromString(LPCSTR lpString);
private:
#endif
// AUTOFMT 09.08.06 SIS <<
	//@cmember Internal function for building an internal name table.
	void BuildNameTable(LPCKEYWORD lpNames,LPSTR lpTemp);

	//@cmember Internal function that searches the internal memory buffer.
	LPSTR ScanFwd(LPSTR lpPattern,LPSTR lpText,UINT  iLength);

	//@cmember Internal function that searches the internal memory buffer.
	LPSTR ScanBack(LPSTR lpPattern, LPSTR lpText, int   iLength);
	
	//@cmember Internal function used to implement a hash table.
	int GetHashValue(LPSTR lpWord, int iLength);
	
	//@cmember Internal function used to retrieve the next word in memory.
	LPSTR GetNextWord(LPSTR lpString, int *iWordLength);

	//@cmember Internal function used to get profile information.
	int MyGetPrivateProfileString(LPCSTR lpSectionName,LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize,LPCSTR lpFileName);

	//@cmember Internal function that frees temporary buffers.
	void FreeTempBuffers();

	//@cmember Internal function that parses a pattern and verifies it.
	BOOL IsRegular(unsigned char *pat);

	//@cmember Internal function that scans for a given regex.
	LPSTR FScan(LPSTR lpPattern, LPSTR lpText, int   iLength);

	//@cmember Internal function that scans backwards for a given regex.
	LPSTR FScanBack(LPSTR lpPattern,LPSTR lpText, int   iLength);


// Attributes
public:
	//@cmember LINEDESC linked list
   PLINEDESC    m_pLine;           

   	//@cmember pointer to the undo buffer
   PUNDORECORD  m_pUndoBuffer;     

	//@cmember Internal data member to toggle autoindent on/off
	BOOL         m_bAutoIndent;	

   	//@cmember Internal data member # of bookmarks set
   int          m_iBookmarkCount;       //

   	//@cmember Internal data member that stores  redo position

   int          m_iRedoPos;       
   	//@cmember Internal data member that stores undo position
   int          m_iUndoPos;      
   
   	//@cmember Internal data member that stores undo timestamp
   int          m_dwUndoTimestamp;

   	//@cmember Internal data member that stores total undo records saved
   int          m_iUndoCount;         

   	//@cmember Internal data member that is a circular lists 
   int          m_iUndoStart;         

   	//@cmember Internal data member that stores file handle if locking is on
   HANDLE       m_hFile;              

   	//@cmember Internal data member that stores whether the end of line marker is 1 or 2 bytes
	int          m_iLineEndLength;

	//@cmember Internal data member that stores zero terminated cr/lf or whatever
	char         m_szLineEnd[4];       

	//@cmember Internal data member that stores state between line of a multi-line comment
	BOOL         m_bInComment;		

	//@cmember Internal data member that stores current count of lines in file
	int          m_iLinesInFile;    

   	//@cmember Internal data member that stores if the file needs to be saved.
	BOOL         m_bChanged;       

   	//@cmember Internal data member that stores if the was opened in readonly mode.
	BOOL         m_bReadOnly;     

   	//@cmember Internal data member that stores wordwrap status.
	BOOL         m_bWordWrap;            

   	//@cmember Internal data member that stores this document's tabsize setting
	int          m_iTabSize;    

	//@cmember Internal data member that stores whether to keep tabs or expand to spaces
	BOOL         m_bKeepTabs;	

	//@cmember Internal data member that stores whether or not to show a tab marker.
	BOOL         m_bShowTabs;	

	//@cmember Internal data member that stores insert/overtype state.
	int          m_bInsertMode;			

	//@cmember Internal data member that stores if we need to keep files open to lock them?
	BOOL         m_bFileLocking;			 

	//@cmember Internal data member that stores the line type.
	int          m_iLineEndType;        

   	//@cmember Internal data member that stores the font name, e.g. Courier New.
	CString      m_fontname;       

	//@cmember Internal data member that stores the font size in points
	int          m_fontsize;            

	//@cmember Internal data member that stores the font weight
	int          m_fontweight;            

	//@cmember Internal data member that stores the italic setting
	int          m_fontitalic;            

	//@cmember Internal data member that stores the font character set, ANSI or OEM
	int          m_fontcharset;

	//@cmember Internal data member that stores if chroma coding is enabled.
	BOOL         m_bChroma;         

	//@cmember Internal data member that stores whether chroma coding is case sensitive
	BOOL         m_bCaseSense;		       

	//@cmember Internal data member that stores the color of keywords
	COLORREF		 m_crKeyWord;				 

	//@cmember Internal data member that stores the color of double quote strings.
	COLORREF		 m_crQuote;					

	//@cmember Internal data member that stores color of single quote strings.
	COLORREF		 m_crSQuote;				 

	//@cmember Internal data member that stores the color of comments.
	COLORREF     m_crComment;				 

	//@cmember Internal data member that stores the color of normal text.
	COLORREF		 m_crText;					

	//@cmember Internal data member that stores the background color.
	COLORREF     m_crBackground;			

	//@cmember Internal data member that stores  the registry subkey to use for file settings
	CString      m_strSection;           

	//@cmember Internal data member that stores name of the keyword list INI file.
	CString      m_strKeywordList;		 

   	//@cmember Internal data member that stores a pointer to the active view.
   // A CWnd derivative because it may be a pointer to an SECEditCtrl
	CWnd* m_pActiveView;			 

	//@cmember Internal data member that stores chroma coding extra characters.
	int alnumarray[256];                

	//@cmember Internal data member that stores chroma coding extra characters.
	int terminatorarray[256];

	//@cmember Internal data member that stores chroma coding extra characters.
	int startarray[256];

	//@cmember Internal data member that stores a global for the scan functions

	LPSTR lpScanBuffer; 

	//@cmember Internal data member that stores a hashed table of key words 
	LPCKEYWORD lpNames;	

	//@cmember Internal data member that stores comment delimiters
	char szCommentStart1[16];  

	//@cmember Internal data member that stores comment delimiters
	char szCommentEnd1[16];

	//@cmember Internal data member that stores comment delimiters
	char szCommentStart2[16];
	
	//@cmember Internal data member that stores comment delimiters
	char szCommentEnd2[16];
	
	//@cmember Internal data member that stores the keyword section name.
	CString strKeyWordSection;
		
	//@cmember Internal data member that stores color coding information.
	char szColorString[64];

	//@cmember Internal data member that stores the key section string.
	char szKeySection[64];

	//@cmember Internal data member that stores the ini file buffer.
	LPSTR glpMyIniFileBuffer;

	//@cmember Internal data member that stores the ini file length
	int  giMyIniLength;

	
};  // class SECEdit

//----------------------------------------------------------------------------
//  class SECEditCore : Visual Interface for Objective Edit
//	templatized base classes allow derivation from both CView and CWnd
//  Author:    SteveS
//
//

/////////////////////////////////////////////////////////////////////////////

#define OE_HPAGESIZE 32  // Horizontal scroll bar pagesize
#define OE_STREAM  1     // block marking types
#define OE_COLUMN  2
#define OE_LINE    3

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

class OEFindDlg;	  // forward dec needed for SaveFindDlgSettings()

class COleDropTarget;

struct SEC_DND_STATE: public CNoTrackObject
{
	CWnd* pDropOrigin;

	SEC_DND_STATE() { pDropOrigin = 0; }
};                               

EXTERN_PROCESS_LOCAL(SEC_DND_STATE, _secDndState);


//@doc SECEditCore
//@mdata HACCEL  | SECEditCore | m_hAccelTable | This is the accelerator table that
//Objective Edit uses to route command ID's. The supplied table uses DevStudio
//format. To map a different set of keystrokes to these commands, set this member
//to be equal to your own accelerator table. These are the command ID's that Objective
// These are the Objective Edit Commands that may be mapped in
// your custom accelerator table. The default accelerator table
// provided is DevStudio Emulation.<nl>
// ID_EDIT_DOWN                    <nl>
// ID_EDIT_UP                      <nl>
// ID_EDIT_LEFT                    <nl>
// ID_EDIT_RIGHT                   <nl>
// ID_EDIT_PGUP                    <nl>
// ID_EDIT_PGDN                    <nl>
// ID_EDIT_HOME                    <nl>
// ID_EDIT_END                     <nl>
// ID_EDIT_TOPFILE                 <nl>
// ID_EDIT_BOTTOMFILE              <nl>
// ID_EDIT_WORDLEFT                <nl>
// ID_EDIT_WORDRIGHT               <nl>
// ID_FILE_MERGE                   <nl>
// ID_EDIT_SELECT_UP               <nl>
// ID_EDIT_SELECT_LEFT             <nl>
// ID_EDIT_SELECT_DOWN             <nl>
// ID_EDIT_SELECT_END              <nl>
// ID_EDIT_SELECT_HOME             <nl>
// ID_EDIT_SELECT_PGDN             <nl>
// ID_EDIT_SELECT_WORDLEFT         <nl>
// ID_EDIT_SELECT_PGUP             <nl>
// ID_EDIT_SELECT_RIGHT            <nl>
// ID_EDIT_SELECT_WORDRIGHT        <nl>
// ID_EDIT_BACKSPACE               <nl>
// ID_EDIT_TAB                     <nl>
// ID_EDIT_BACKTAB                 <nl>
// ID_EDIT_FINDNEXT                <nl>
// ID_BOOKMARKSET                  <nl>
// ID_BOOKMARKNEXT                 <nl>
// ID_BOOKMARKPREV                 <nl>
// ID_BOOKMARKDELETE               <nl>
// ID_PAGESETUP                    <nl>
// ID_EDIT_SELECT_BOTTOMFILE       <nl>
// ID_EDIT_SELECT_TOPFILE          <nl>


//@doc SECEditCore
//@mdata CBrush  | SECEditCore | m_brGutter | This is the brush that is used to paint
//the gutter at the left of the Objective Edit window.

//@doc SECEditCore
//@mdata BOOL  | SECEditCore | m_bPasteAfter | This variable controls whether pasted
//text is inserted immediately before or after the cursor. Default: TRUE

//@doc SECEditCore
//@mdata BOOL  | SECEditCore | m_bUseVirtualWhitespace | This sets whether the cursor may be placed anywhere by
//the user, or restricted to areas where text exists. Default: FALSE


//@doc SECEditCore
//@class SECEditCore derives from CView or CWnd. SECEditView and SECEditCtrl
// are the display classes for Objective Edit
// for Objective Edit.  The public operations in this class allow you
// to manipulate the selection, cursor and other aspects of the "view" of
// the syntax highlighted source code file.  All Internal aspects have been
// documented for customizability, but use at your own discretion, these
// aspects of the product may change in the future.
//
// View member functions operate on the current position, maintained here
// as m_iColNo, m_iLineno, m_pCurLine, and m_pTopLine, or the current block,
// maintained in m_BlockStart and m_BlockEnd.  The lower level SECEdit functions
// are called with a specific position.
//
// The screen is invalidated if bUpdate is true and Undo information is saved if
// bSaveUndoRecord is true.
//
// If the function returns an int, 0 is success and non-zero is an error code
// defined in editerr.h
//
// If the function returns a value, 0 or NULL is returned on failure
//

template <class BC>
class SECEditCore : public BC
{

protected: // create from serialization only

	//@cmember SECEditCore constructor
	SECEditCore();

	CSplitterWnd* m_pSplitterParent;
	int m_nSplitRow;
	int m_nSplitCol;
	BOOL m_bScrollMessage;

	//@cmember Returns COleDataSource object used for data sourcing
	virtual COleDataSource* GetNewDataSource();

	//@cmember Initializes the data source.
	virtual BOOL InitializeDataSource(COleDataSource* pDataSource, CLIPFORMAT clipFormat, HGLOBAL hGlobal);

// Attributes
public:

	// static OEPageSetupDlg to replace global OEPageSetupDlg in
	// the old SECEditView.h
	OEPageSetupDlg dlgPageSetup;
	TEXTPOS m_OriginalBlockStart;

// Operations
public:

	// Handle the accelerators
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// GetDocument, returns NULL for SECEditCtrl, and the Doc for SECEditView
	//virtual CDocument* GetDocument()
	//{return NULL;}

	// @cmember GetActiveDoc, returns NULL for SECEditCtrl, and the ActiveDoc for SECEditView
	virtual CDocument* GetActiveDoc();

	// @cmember Set the gutter color
	virtual COLORREF GetGutterColor(COLORREF clrEmpty);

  	//@cmember Make the cursor visible.
   void MakeCursorVisible();

	//@cmember Handles the Find.MarkAll operation.  Called from OnFindReplace().
	void DoFindMarkAll(PSEARCHRECORD pSearch);

	//@cmember Sets the line avoidance parameter from the OEFindDlg object.
   void SetLineAvoidance(OEFindDlg* dlg);

   	//@cmember Saves the settings of an OEFindDlg object.
   void SaveFindDlgSettings(OEFindDlg* dlg);

   	//@cmember Initializes the find dialog.
   virtual void InitFindDialog(OEFindDlg* dlg);

   	//@cmember Adds a find string to the find dialog.
   void AddFindString(CString& newstring);

   	//@cmember Serializes the replace dialog settings.
   void SaveReplaceDlgSettings(OEFindDlg* dlg);

   	//@cmember Adds a string to the replace dialog.
   void AddReplaceString(CString& newstring);

   	//@cmember Internal member function that checks the status of the internal line structures.
   void CheckLines(PLINEDESC pLine);

   	//@cmember Internal member that centers a given line.
   void CenterLine(PLINEDESC pLine);

   	//@cmember Returns the number of bookmarks.
   int CountBookmarks();

   	//@cmember Internal error handler.
   int DoError(int iCode, BOOL bUpdate);

   	//@cmember Internal error handler.
   int DoError(int iCode, LPCTSTR lpExtraText, BOOL bUpdate);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(SECEditView)
   public:

   	//@cmember Virtual function - override this if you want to customize the way that items in the gutter draw.
#ifdef ORIGINAL_VERSION
   virtual void DrawMarks(CPaintDC* dc, RECT* rect, PLINEDESC pLine);
#else
   virtual void DrawMarks(CDC* dc, RECT* rect, PLINEDESC pLine);
#endif

	//@cmember The OnDraw handler for this view.  Override to customize the drawing logic.
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view

   	//@cmember Override this member to initialize anything in your derivatives.
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

   	//@cmember Scroll handler.
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

   protected:

   //}}AFX_VIRTUAL

// Implementation
public:

	//@cmember SECEditCore destructor.
   virtual ~SECEditCore();

#ifdef _DEBUG

   	//AssertValid - verifies that the SECEditView is valid.
   virtual void AssertValid() const;

   	//Dump() - display the values of the object.
   virtual void Dump(CDumpContext& dc) const;

#endif

protected:

	//@cmember Handle to the accelerator table used to route internal commands.
	HACCEL m_hAccelTable;

	//@cmember Brush used to paint the gutter.
	CBrush m_brGutter;

	//@cmember Function to Update the View for CView Derived SECEditCore
	virtual void UpdateViews(CWnd* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);

	// Chance to extend default context menu
	// Return false to not show default menu
	virtual BOOL ExtendContextMenu(CMenu* pMenu);

#ifdef ORIGINAL_VERSION
#else
	int InternalPaint (CDC *dc, CRect drawArea, bool bIsBlank);
	virtual void SubClassDraw (CDC *dc, CRect drawArea);
    virtual bool OverrideBackgroundColor (PLINEDESC pLine, COLORREF &backgroundColor);
    virtual void GetSubclassFlags (PLINEDESC pLine, char *pchFlags);
    virtual bool OverrideTextBackground (char chFlag, COLORREF &backgroundColor);
    virtual bool OverrideTextForeground (char chFlag, COLORREF &foregroundColor);
    virtual bool OnModifyContent ();
	CPoint m_ContextMenuPoint;
#endif

// Generated message map functions
public:
   //{{AFX_MSG(SECEditCore)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !

	//@cmember Internal cursor handler.
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	//@cmember Internal function that creates a context menu.
	afx_msg virtual void OnContextMenu(CWnd* pWnd, CPoint point);

	//@cmember Internal creation helper.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//@cmember Internal button handler.
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

   	//@cmember Internal mouse butt handler.
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

   	//@cmember Internal mouse move handler.
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

   	//@cmember Internal button click handler.
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   	
	//@cmember Paint handler.
    afx_msg void OnPaint();

#if _MFC_VER >= 0x0421
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif


	inline int GetWidth(CDC* dc, LPCSTR lpText, int nCount)
	{
		USES_CONVERSION_T;
		CSize size = dc->GetTextExtent(A2CT(lpText),nCount);
		return size.cx;
	}


	BOOL PaintTextOut( SECEdit* pEdit, 
						   PLINEDESC lpLine, 	// pointer to the current line
							LPSTR str, 			// the full text line, tabs expanded
							int nLength,			// the full length of the text
							int nIndex, 			// first character to draw
							int nCount, 			// max characters to draw
							BOOL bChroma, 			// do it in color
							CDC* dc, 				// printer's dc
							int x, 					// textout coordinates
							int y);					//		 "			 "
   	
	//@cmember Handler for "bottom of file" command.
    afx_msg void OnEditBottomfile();
   	
	//@cmember Handler for "down arrow" command.
   afx_msg void OnEditDown();
   	//@cmember Handler for "end" command.
   afx_msg void OnEditEnd();
   	//@cmember Handler for "home" command.
   afx_msg void OnEditHome();
   	//@cmember Handler for "left cursor key" command.
   afx_msg void OnEditLeft();
   	//@cmember Handler for "page down" command.
   afx_msg void OnEditPgdn();
   	//@cmember Handler for "page up" command.
   afx_msg void OnEditPgup();
   	//@cmember Handler for "right cursor" command.
   afx_msg void OnEditRight();
   	//@cmember Handler for "top of file" command.
   afx_msg void OnEditTopfile();
   	//@cmember Handler for "up cursor key" command.
   afx_msg void OnEditUp();
   	//@cmember Handler for "word left" command".
   afx_msg void OnEditWordleft();
   	//@cmember Size handler.
   afx_msg void OnSize(UINT nType, int cx, int cy);
   	//@cmember Vertical scroll handler.
   afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   	//@cmember WM_CHAR handler.
   virtual afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectDown();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectEnd();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectHome();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectLeft();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectPgdn();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectPgup();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectRight();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectUp();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectWordleft();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectWordright();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectAll();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectTop();
   	//@cmember Selection command handler.
   afx_msg void OnEditSelectBottom();
   	//@cmember Selection command handler.
   afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
   	//@cmember Timer handler.
   afx_msg void OnTimer(UINT nIDEvent);
   	//@cmember Edit/Copy handler.
   afx_msg void OnEditCopy();
   	//@cmember Edit/Copy UI handler.
   afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
   	//@cmember Edit/cut handler.
   afx_msg void OnEditCut();
   	//@cmember Edit/cut UI handler.
   afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
   	//@cmember Edit/paste handler.
   afx_msg void OnEditPaste();
   	//@cmember Edit/paste UI handler.
   afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
   	//@cmember Edit/clear handler.
   afx_msg void OnEditClear();
   	//@cmember Edit/backspace handler.
   afx_msg void OnEditBackspace();
   	//@cmember Edit/backgab handler.
   afx_msg void OnEditBacktab();
   	//@cmember Edit/tab handler.
   afx_msg void OnEditTab();
   	//@cmember Edit/word right handler.
   afx_msg void OnEditWordright();
   	//@cmember Edit/find handler.
   afx_msg void OnEditFind();
   	//@cmember Edit/Find UIhandler.
   afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
   	//@cmember Killfocus handler.
   afx_msg void OnKillFocus(CWnd* pNewWnd);
   	//@cmember Setfocus handler.
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   	//@cmember Edit/find next handler.
   afx_msg void OnEditFindnext();
   	//@cmember Edit/find next UI handler.
   afx_msg void OnUpdateEditFindnext(CCmdUI* pCmdUI);
   	//@cmember Edit/replace handler.
   afx_msg void OnEditReplace();
   	//@cmember Edit/replace UI handler.
   afx_msg void OnUpdateEditReplace( CCmdUI* pCmdUI);
   	//@cmember Edit/undo handler.
   afx_msg void OnEditUndo();
   	//@cmember Edit/undo UI handler.
   afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
   	//@cmember Edit/redo handler.
   afx_msg void OnEditRedo();
   	//@cmember Edit/redo UI handler.
   afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
   //@cmember Toggle Insert/OverStrike mode
   afx_msg void OnToggleInsert();
   afx_msg void OnUpdateToggleInsert(CCmdUI* pCmdUI);
   	//@cmember Next bookmark handler.
   afx_msg void OnBookmarknext();
   	//@cmember Next bookmark UI handler.
   afx_msg void OnUpdateBookmarknext(CCmdUI* pCmdUI);
   	//@cmember Set bookmark handler.
   afx_msg void OnBookmarkset(); 
   	//@cmember Set bookmark UI handler.
   afx_msg void OnUpdateBookmarkset(CCmdUI* pCmdUI);
   	//@cmember Previous bookmark handler.
   afx_msg void OnBookmarkprev();
   	//@cmember Previous bookmark UI handler.
   afx_msg void OnUpdateBookmarkprev(CCmdUI* pCmdUI);
   	//@cmember Delete bookmark handler.
   afx_msg void OnBookmarkdelete();
   	//@cmember Delete bookmark UI handler.
   afx_msg void OnUpdateBookmarkdelete(CCmdUI* pCmdUI);
   	//@cmember Horizontal scroll handler.
   afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   	//@cmember Page setup handler - creates an OEPageSetupDlg.
	afx_msg void OnPageSetup();
	//@cmember Page setup UI handler.
	afx_msg void OnUpdatePageSetup(CCmdUI* pCmdUI);
		//@cmember
   //}}AFX_MSG

	//@cmember Brings up the find/replace dialog when the command is received.
   afx_msg LONG OnFindReplace(WPARAM wParam, LPARAM lParam);
   	

	// Editor information
	//Calculates the number of lines per page.
	int		LinesPerPage	()
	{
		return ( m_rect.bottom / m_iLineHeight );
	}

	//Calculates the last line number in the buffer.
	int		LastLineNo		()
	{
		return m_iFirstLineNo + LinesPerPage() - 1;
	}

	//Calculates if the given line is visible or not.
	BOOL	IsLineVisible	( int Line )
	{
		return ( ( Line >= m_iFirstLineNo ) && ( Line <= LastLineNo() ) );
	}

	//@cmember Calculates if the current point is in the selection area.
	BOOL IsPointInBlock(CPoint point);

	//@cmember OLE drag and drop handler.
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	//@cmember OLE drag and drop handler.
	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	//@cmember OLE drag and drop handler.
	void OnDragLeave();

	//@cmember OLE drag and drop handler.
	void DoDropPaste(LPSTR lpDropText, int iLength);	

	//@cmember OLE drag and drop handler.
	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	//@cmember OLE drag and drop handler.
	DROPEFFECT DoDragCopy();

	//@cmember Get the length of the text.
	long GetTextLength();

	//@cmember TRUE if the next item was found.
	BOOL m_bFoundNext;

	//@cmember FindText - locates the text in the buffer.
	BOOL FindText(LPCTSTR lpszFind, BOOL bCase = TRUE, BOOL bWord = FALSE);

	//@cmember Gets the text from the editor.
	int GetWindowText( LPTSTR lpszStringBuf, int nMaxCount );

	//@cmember Gets the text from the editor.
	void GetWindowText( CString& rString );

	//@cmember Set the editor text.
	void SetWindowText( LPCTSTR lpszString );

	//@cmember Get the current selection.
	BOOL GetSelection( TEXTPOS& Start, TEXTPOS& End, BOOL* pbColumnar = NULL);

	//@cmember Retrieve the current selection.
	LPSTR  GetSelection( BOOL* pbColumnar = NULL );

	//@cmember Set the current selection.
	void SetSelection( PTEXTPOS Start, PTEXTPOS End, BOOL bColumnar = FALSE);

	//@cmember Replace the selected text with new text.
	BOOL ReplaceSelection( LPCTSTR lpNewSel, BOOL bColumnar =FALSE,
														   BOOL bSaveUndoRecord = TRUE);
	//@cmember Insert file into buffer.
	void InsertFile( LPCTSTR FileName );

	//@cmember Update the editor (forces a redraw).
	void UpdateEditor ();

	//@cmember Moves the cursor to a specific line.
	void GotoLine( int Line );

	//@cmember Moves the cursor to a specific line and column.
	void GotoLineCol( int Line, int Col );

	//@cmember Uppercase edit handler.
	void OnEditUppercase();

	//@cmember Lowercase edit handler.
	void OnEditLowercase();

	//@cmember Returns a pointer to the SECEdit object associated with this SECEditView.
   virtual SECEdit* GetEdit();

   	//@cmember Initializes the view.
   virtual int InitView();

   //MARK - move these up.
   	//@cmember Sets the display font.
   void SetFont(LOGFONT& font);

   	//@cmember Sets the display font.
	void SetFont( CFont* pFont, BOOL bRedraw = TRUE );

	//@cmember Retrieves the current display font.
	CFont* GetFont( );

	//@cmember Paints the view.
   int Paint(void);

   	//@cmember Determines if a point is in the current selection region.
   BOOL InBlock(PLINEDESC pstLine, int iLength, POINT *ptBlock);

   	//@cmember Clears a block of text from the display.
   void ClearBlockFromScreen(void);

   	//@cmember Draws with tabs expanded.
   int PaintExpandTabs(LPSTR lpText, LPSTR lpWorkBuffer, int iTextSize, 
                       int iTabSize, int iLineEndLength);


   //MARK internal scrolling ops.

   	//@cmember Scrolls to a given line.
   int ScrollGoToLine(int iPos, BOOL bUpdate);

   	//@cmember Scrolls up a line.
   int ScrollUpLine(BOOL bUpdate);

   	//@cmember Scrolls down a line.
   int ScrollDownLine(BOOL bUpdate);

   	//@cmember Scrolls up a page.
   int ScrollUpPage(BOOL bUpdate);

   	//@cmember Scrolls down a page.
   int ScrollDownPage(BOOL bUpdate);

	//@cmember Internal method that gets a valid column number based on location.
   int GetValidColNo(int iPos);

   	//@cmember Internal method that retrieves the current column number.
   int GetPreviousColNo(int iPos);

   	//@cmember Retrieves the next column number.
   int GetNextColNo(int iPos);

   //MARK - cursor functions.

   	//@cmember Moves the cursor left one column.
   int CursorLeft(BOOL bUpdate);

   	//@cmember Moves the cursor right one column.
   int CursorRight(BOOL bUpdate);

   	//@cmember Moves the cursor up one line.
   int UpLine(BOOL bUpdate);

   	//@cmember Moves the cursor down a line.
   int DownLine(BOOL bUpdate);

   	//@cmember Moves the cursor to the beginning of the line.
   int BeginningOfLine(BOOL bUpdate);

   	//@cmember Moves the cursor to the end of a line.
   int EndOfLine(BOOL bUpdate);

   	//@cmember Moves the cursor up a page.
   int UpPage(BOOL bUpdate);

   	//@cmember Moves the cursor down a page.
   int DownPage(BOOL bUpdate);

   	//@cmember Moves the cursor to the top of the file.
   int TopOfFile(BOOL bUpdate);

   	//@cmember Moves the cursor to the bottom of the file.
   int BottomOfFile(BOOL bUpdate);

   	//@cmember Moves the cursor left one word.
   int WordLeft(BOOL bUpdate);

   	//@cmember Moves the cursor right one word.
   int WordRight(BOOL bUpdate);

   	//@cmember Moves the cursor to the specified location.
   int MouseGoTo(int x, int y);

   	//@cmember Update the status line - override if you want to imlement your custom status line display routines to show the current column/row 
   //information for the editor.
   virtual void UpdateStatusLine(void);

   	//@cmember Returns the number of lines in the file.
   int CountLines();

   	//@cmember Internal function used to manipulate the cursor/caret.
   void MySetCaretPos();

   	//@cmember Internal function used in scrolling.
   virtual void MySetScrollRange(int fnBar, int iMin, int iMax, BOOL bRedraw);

   	//@cmember Internal function used in scrolling.
   virtual void MySetScrollPos(int fnBar, int iPos, BOOL bRedraw);

   	//@cmember Internal function used in scrolling.
   virtual int MyGetScrollPos(int fnBar);

		//@cmember Internal function used in scrolling.
   virtual void MyGetScrollRange(int fnBar, int *iMin, int *iMax);

   	//@cmember Function that splits the current line.
   virtual int SplitLine(BOOL bUpdate, BOOL bSaveUndoRecord);

// AUTOFMT 07.08.06 SIS >>
// original comment is wrong
// AutoIndent indeed does the work
#ifdef ORIGINAL_VERSION
   	//@cmember Turns on and off auto indenting.
	void AutoIndent(BOOL bSaveUndoRecord);
#else
   	//@cmember Performs auto indenting.
	virtual void AutoIndent(BOOL bSaveUndoRecord);
#endif
// AUTOFMT 07.08.06 SIS <<

	//@cmember Internal function to determine if the line structure has text in it.
	BOOL HasText(PLINEDESC pLine, int& iFirst);

	//MARK - insert/delete functions

	//@cmember Inserts a string into the buffer at the current cursor position..
   int InsertString(LPTSTR lpString, BOOL bUpdate, BOOL bSaveUndoRecord);

   	//@cmember Inserts a character into the buffer at the current cursor position.
   int InsertChar(int iChar, BOOL bUpdate, BOOL bSaveUndoRecord);

   	//@cmember Internal function.
	BOOL EndedAComment(SECEdit* pEdit, int iChar);

	//MARK - selection members. 

	//@cmember Clears all selection.
   virtual int ClearSelection(BOOL bUpdate);

   	//@cmember Internal function.
   int ClearInvalidMarkers();

   	//@cmember Starts selection.
   int StartSelection(int iType, BOOL bUpdate);

   	//@cmember Internal function
   BOOL InOrder(PTEXTPOS pFirst, PTEXTPOS pSecond);

   	//@cmember Internal function
   void SetUpInvalidBlock(PTEXTPOS pStart, PTEXTPOS pEnd);

   	//@cmember Internal function
   void ClearInvalidBlockMarks(PTEXTPOS pTextPos);
   
   //@cmember Refreshes during column selection.
   int UpdateColumnSelection(BOOL bUpdate);
   
   //@cmember Updates the screen selection.
   int UpdateSelection(BOOL bUpdate);
   
   //@cmember Selects the entire line.
   int SelectLine(BOOL bUpdate);
   
   //@cmember Selects the entire word.
   int SelectWord(BOOL bUpdate);
   
   //@cmember Selectst he entire file.
   int SelectAll(BOOL bUpdate);
   
   //@cmember Selects up a specified way.
   int SelectUp(int iType, BOOL bUpdate);
   
   //@cmember Selects down a specified way.
   int SelectDown(int iType, BOOL bUpdate);
   
   //@cmember Selects left.
   int SelectLeft(int iType, BOOL bUpdate);
   
   //@cmember Selects right.
   int SelectRight(int iType, BOOL bUpdate);
   
   //@cmember Select all the way to the beginning of the line.
   int SelectHome(int iType, BOOL bUpdate);
   
   //@cmember Select until the end of the line.
   int SelectEnd(int iType, BOOL bUpdate);
   
   //@cmember Select the word to the left.
   int SelectWordLeft(int iType, BOOL bUpdate); 
   
   //@cmember Select the word to the right.
   int SelectWordRight(int iType, BOOL bUpdate);
   
   //@cmember Select up a page.
   int SelectPageUp(int iType, BOOL bUpdate);
   
   //@cmember Select down a page.
   int SelectPageDown(int iType, BOOL bUpdate);
   
   //@cmember Select to the top of the file.
   int SelectTopOfFile(int iType, BOOL bUpdate);
   
   //@cmember Select down to the bottom of the file.
   int SelectEndOfFile(int iType, BOOL bUpdate);

   	//@cmember Internal function to search for a string and select it.
   virtual int SelectFoundText(PTEXTPOS pTextPos, int iLength, BOOL m_bUpdate);

   	//@cmember Copy the clipboard data into the current cursor location.
   int Copy(BOOL bUpdate, BOOL bSaveUndoRecord);
   
   //@cmember Copy the clipboard column data into the current cursor location.
   int ColumnCopy(BOOL bUpdate, BOOL bSaveUndoRecord);

   //@cmember Cut the selected text.
   int Cut(BOOL bUpdate, BOOL bSaveUndoRecord);

   //@cmember Cut the selected column.
   int ColumnCut(BOOL bUpdate, BOOL bSaveUndoRecord);

   	//@cmember Paste the clipboard data.
   int Paste(BOOL bUpdate, BOOL bSaveUndoRecord);

   //@cmember Paste a column of clipboard data.
   int ColumnPaste(BOOL bUpdate, BOOL bSaveUndoRecord);

  	//@cmember Delete a character.
   int DeleteChar(BOOL bUpdate, BOOL bSaveUndoRecord);

   	//@cmember Delete backwards one character.
   int BackSpace(BOOL bUpdate, BOOL bSaveUndoRecord);

   	//@cmember Joint two lines.
   int JoinLines(BOOL bUpdate, BOOL bSaveUndoRecord);

  	//@cmember Insert a tab character.
   int InsertTab(BOOL bUpdate, BOOL bSaveUndoRecord);

   //@cmember Delete a tab character.
   int BackTab(BOOL bUpdate, BOOL bSaveUndoRecord);

   //@cmember Shift a block of text.
   int ShiftBlock(BOOL bBackTab, BOOL bUpdate, BOOL bSaveUndoRecord);

   //@cmember Copies current word or one-line selection to str
	virtual BOOL GetWordOrSelection(CString& str);

public:

	//@cmember Indicates whether the caret may be placed anywhere.
	BOOL m_bUseVirtualWhitespace;

	//@cmember Internal data member 
   PLINEDESC    m_pTopLine;             // first line in view

   	//@cmember Internal data member
   PLINEDESC    m_pCurLine;             // line cursor is on

   	//@cmember Internal data member
   OEFindDlg*   m_finddlg;

   	//@cmember Internal data member
   OEFindDlg*   m_replacedlg;

   	//@cmember Internal data member
   BOOL         m_bFinding;

   	//@cmember Internal data member
   BOOL         m_bReplacing;

   	//@cmember Internal data member
   BOOL         m_bWrapSearch;

   //@cmember Internal data member that stores whether the pasted text goes after
   //or before the cursor
   BOOL			m_bPasteAfter;

   	//@cmember Internal data member 1st document line # in view
   int          m_iFirstLineNo;         //

   	//@cmember Internal data member document line      starts at 1
   int          m_iLineNo;              // line number of cursor

   	//@cmember Internal data member document column    starts at 0
   int          m_iColNo;               // column position of cursor (tabbed offset in current line)
										// Caution: m_iColNo may exceed maximum char index of line, i. e.
										// cursor points behind last character in line.
										// If any character is entered then, the line is filled
										// with empty space up to the cursor position.
										// This is used for auto indent.

   	//@cmember Internal data member offset to 1st visible column
   int          m_iLeftEdge;            //

   	//@cmember Internal data member 1: stream 2: column 3: whole line
   int          m_iBlockType;           //

   	//@cmember Internal data member a block is selected
   int          m_bSelecting;           //

   	//@cmember Internal data member start of block
   TEXTPOS      m_BlockStart;           // used for block selection

   	//@cmember Internal data member end of block
   TEXTPOS      m_BlockEnd;             // used for block selection

   	//@cmember Internal data member client rect
   CRect        m_rect;                 //

   	//@cmember Internal data member textmetric
   int          m_iCharWidth;           //

   	//@cmember Internal data member textmetric
   int          m_iLineHeight;          //

   	//@cmember Internal data member vertical scroll scale
	int          m_iVScale;

   	//@cmember Internal data member turns off drawing if false
   BOOL         m_bUpdate;              //

   	//@cmember Internal data member mouse capture on
   BOOL         m_bTrack;               //

   	//@cmember Internal data member mouse move timer 
   int          m_iTimer;               //

   	//@cmember Internal data member mouse capture, eat the first timer hit
   BOOL         m_bEatIt;               //

   	//@cmember Internal data membermouse capture, for selection update
   POINT        m_ptOldCursor;          // 

   	//@cmember Internal data member CreateCaret called

   BOOL         m_bHaveCaret;           //
   	//@cmember Internal data member for MakeCursorVisible(): don't let m_pCurline end up here
   CRect        m_badrect;              //

   	//@cmember Internal data member max # of chars before wordwrap
   int          m_iMaxChars;            //

   	//@cmember Internal data member screen font
   CFont        m_font;                 //

   	//@cmember Internal data member copy line if no selection
   BOOL         m_bCopyLine;            //

   	//@cmember Internal data member cut line if no selection
   BOOL         m_bCutLine;             // 

   	//@cmember Internal data memberregular expressions enabled
   BOOL         m_bRegular;             // 

   	//Internal data member match case 
   BOOL         m_bMatchCase;           //

   	//@cmember Internal data member search forward
   BOOL         m_bForward;             //

   	//@cmember Internal data member room at left to draw bookmarks etc., in pixels
   int          m_iMarkSpace;           //

	//@cmember Internal data member if NULL, mirror display font
	HFONT			 m_hPrinterFont;			 //

	//@cmember Internal data member font object used when mirroring	
	HFONT			 m_hMirrorFont;			 //

	//@cmember Internal data member print lines per page
	int  			 m_iLinesPerPage;			 //

	//@cmember Internal data member height of one print line
	int			 m_iPrintLineHeight;     //

	//@cmember Internal data member
	CTime			 m_timeHeader;

	//@cmember Internal data member
	CTime			 m_timeFooter;

	//@cmember Internal data member
	BOOL         m_bPrintSelection;

	BOOL	m_4CSpecMouseLeftButtonDown;

};  // class SECEditCore

#ifndef SECEDITCORE_MESSAGE_LIST
#define SECEDITCORE_MESSAGE_LIST()\
	ON_WM_CONTEXTMENU()\
	ON_WM_LBUTTONDOWN()\
	ON_WM_LBUTTONUP()\
	ON_WM_MOUSEMOVE()\
	ON_WM_SETCURSOR()\
	ON_WM_LBUTTONDBLCLK()\
	ON_WM_PAINT()\
	ON_WM_SIZE()\
	ON_WM_CHAR()\
	ON_WM_TIMER()\
	ON_WM_KILLFOCUS()\
	ON_WM_SETFOCUS()\
	ON_WM_VSCROLL()\
	ON_WM_HSCROLL()\
	ON_WM_CREATE()\
	ON_COMMAND(ID_EDIT_BOTTOMFILE, OnEditBottomfile)\
	ON_COMMAND(ID_EDIT_DOWN, OnEditDown)\
	ON_COMMAND(ID_EDIT_END, OnEditEnd)\
	ON_COMMAND(ID_EDIT_HOME, OnEditHome)\
	ON_COMMAND(ID_EDIT_LEFT, OnEditLeft)\
	ON_COMMAND(ID_EDIT_PGDN, OnEditPgdn)\
	ON_COMMAND(ID_EDIT_PGUP, OnEditPgup)\
	ON_COMMAND(ID_EDIT_RIGHT, OnEditRight)\
	ON_COMMAND(ID_EDIT_TOPFILE, OnEditTopfile)\
	ON_COMMAND(ID_EDIT_UP, OnEditUp)\
	ON_COMMAND(ID_EDIT_WORDLEFT, OnEditWordleft)\
	ON_COMMAND(ID_EDIT_SELECT_DOWN, OnEditSelectDown)\
	ON_COMMAND(ID_EDIT_SELECT_END, OnEditSelectEnd)\
	ON_COMMAND(ID_EDIT_SELECT_HOME, OnEditSelectHome)\
	ON_COMMAND(ID_EDIT_SELECT_LEFT, OnEditSelectLeft)\
	ON_COMMAND(ID_EDIT_SELECT_PGDN, OnEditSelectPgdn)\
	ON_COMMAND(ID_EDIT_SELECT_PGUP, OnEditSelectPgup)\
	ON_COMMAND(ID_EDIT_SELECT_RIGHT, OnEditSelectRight)\
	ON_COMMAND(ID_EDIT_SELECT_UP, OnEditSelectUp)\
	ON_COMMAND(ID_EDIT_SELECT_WORDLEFT, OnEditSelectWordleft)\
	ON_COMMAND(ID_EDIT_SELECT_WORDRIGHT, OnEditSelectWordright)\
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)\
	ON_COMMAND(ID_EDIT_SELECT_TOPFILE, OnEditSelectTop)\
	ON_COMMAND(ID_EDIT_SELECT_BOTTOMFILE, OnEditSelectBottom)\
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)\
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)\
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)\
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)\
	ON_COMMAND(ID_EDIT_BACKSPACE, OnEditBackspace)\
	ON_COMMAND(ID_EDIT_BACKTAB, OnEditBacktab)\
	ON_COMMAND(ID_EDIT_TAB, OnEditTab)\
	ON_COMMAND(ID_EDIT_WORDRIGHT, OnEditWordright)\
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)\
	ON_COMMAND(ID_EDIT_FINDNEXT, OnEditFindnext)\
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)\
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)\
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)\
	ON_COMMAND(ID_INDICATOR_OVR, OnToggleInsert)\
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateToggleInsert)\
	ON_COMMAND(ID_BOOKMARKNEXT, OnBookmarknext)\
	ON_COMMAND(ID_BOOKMARKSET, OnBookmarkset)\
	ON_COMMAND(ID_BOOKMARKPREV, OnBookmarkprev)\
	ON_COMMAND(ID_BOOKMARKDELETE, OnBookmarkdelete)\
	ON_COMMAND(ID_PAGESETUP, OnPageSetup)\
	ON_UPDATE_COMMAND_UI(ID_PAGESETUP, OnUpdatePageSetup)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDNEXT, OnUpdateEditFindnext)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)\
	ON_UPDATE_COMMAND_UI(ID_BOOKMARKNEXT, OnUpdateBookmarknext)\
	ON_UPDATE_COMMAND_UI(ID_BOOKMARKSET, OnUpdateBookmarkset)\
	ON_UPDATE_COMMAND_UI(ID_BOOKMARKPREV, OnUpdateBookmarkprev)\
	ON_UPDATE_COMMAND_UI(ID_BOOKMARKDELETE, OnUpdateBookmarkdelete)\
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectAll)\
	ON_REGISTERED_MESSAGE( WM_FINDREPLACE, OnFindReplace)
#endif // SECEDITCORE_MESSAGE_LIST

// These only need to be included when the library is being built
// and not when project that use the libraries are being linked
#ifdef __BUILD_TIME__
	#include "SECEditCore.inl"
	#include "View_Core.inl"
	#include "ViewBackspace.inl"
	#include "ViewCopy.inl"
	#include "ViewCursor.inl"
	#include "ViewCut.inl"
	#include "ViewDelete.inl"
	#include "ViewInsert.inl"
	#include "ViewJoinLines.inl"
	#include "ViewPaint.inl"
	#include "ViewPaste.inl"
	#include "ViewSelect.inl"
	#include "ViewSplitLine.inl"
	#include "ViewTab.inl"
#endif // __BUILD_TIME__
/////////////////////////////////////////////////////////////////////////////

// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA

#endif //__SECEDIT_H__
