#ifndef __STRUCT_H__
#define __STRUCT_H__

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

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL


/*-------------------------------------------------------------------------*
 *                              STRUCT.H                                   *
 *  declarations of global structures                                      *
 *                                                                         *
 *-------------------------------------------------------------------------*/

typedef struct tagUNDORECORD
   {
   int      iUndoType;
   int      iRow;          // starting position   
   int      iCol;          //    "        "       

   int      iEndRow;       // used in cut & paste
   int      iEndCol;       //    "        "      

	char     cInsertChar;   //	character inserted
	char     cOvertypeChar;	// character overtyped

	DWORD    dwTimestamp;   // time stamp of record

   LPSTR    lpBuffer;		// buffer inserted

   } UNDORECORD;
typedef UNDORECORD *PUNDORECORD;

typedef struct tagLINEDESC
   {
   struct tagLINEDESC *pNext;     /* next line in list                 4  */
   struct tagLINEDESC *pPrev;     /* prev line in list                 4  */
   LPSTR            pText;        /* actual text                       4  */
   int              iTxtSize;     /* bytes of text                     4  */
   int              iBuffLen;     /* size of block allocated for txt   4  */

	int              iLineNo;		 /* 1-based line number               4  */  
   
   unsigned int      iInvalidStartCol:16;  /* 0-based offset                   */
   unsigned int      iInvalidEndCol:16;    /* 0-based offset                   */
   
   unsigned int              fInBlock:  1; /* part or all of the text is marked 4  */
   unsigned int              fBlockWholeLine: 1;
	unsigned int				  fBookMark:   1;
	unsigned int				  fFlag1:      1;  
	unsigned int				  fFlag2:      1;  
	unsigned int				  fFlag3:      1;  
	unsigned int				  fFlag4:      1;  
	unsigned int				  fFlag5:      1;  
	unsigned int				  fFlag6:      1;  
	unsigned int				  fFlag7:      1;  
	unsigned int				  fFlag8:      1;  
   unsigned int              bInvalidStart:1;      /* PatBlt starts here               */
   unsigned int              bInvalidEnd:1;        /* PatBlt stops here                */

   // fUnused??? Don't like boring names like this!!!
#ifdef ORIGINAL_VERSION
   unsigned int              fUnused:    19;
#else
   unsigned int bBreakpoint:1;
   unsigned int bStoppoint:1;
   unsigned int bStep:1;
   unsigned int bAction:1;
   unsigned int bTransition:1;
#endif

   } LINEDESC;
typedef LINEDESC *PLINEDESC;

//  Structure to hold a specific text position.  Used for block markers
typedef struct tagTEXTPOS
   {
   PLINEDESC pLine;              /*  ptr to TEXTBUFFER              4  */
   int       iOffset;            /*  offset from beginning of text  4  */
								 // SIS: this is real offset and not tabbed offset!
   } TEXTPOS;
typedef TEXTPOS *PTEXTPOS;


// struct used to pass the search and replace parameters


typedef struct tagSEARCHRECORD
   {
   unsigned int      fComplain:    1;    /* show 'not found' etc. to user       */
   unsigned int      fPrompt:      1;    /* bring up search/replace dialog box  */
   unsigned int      fMatchCase:   1;    /* exact case match only               */
   unsigned int      fForward:     1;    /* forward direction                   */
   unsigned int      fChangeAll:   1;    /* change all                          */
   unsigned int      fConfirm:     1;    /* ask user to confirm changes         */
   unsigned int      fRegular:     1;    /* use regular expression              */
   unsigned int      fUnused:     25;    /*                                   4 */
	int               iReturnLength;		  // length of found text
   LPSTR     lpSearchText;      /* string to search for              4 */
   LPSTR     lpReplaceText;     /* string to replace found text with 4 */
   } SEARCHRECORD;
typedef SEARCHRECORD *PSEARCHRECORD;

// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA

#endif //__STRUCT_H__
