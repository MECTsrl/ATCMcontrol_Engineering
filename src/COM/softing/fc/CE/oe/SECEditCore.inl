#ifndef __SECEDITCORE_INL__
#define __SECEDITCORE_INL__

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


/////////////////////////////////////////////////////////////////////
//
// SECEditCore.inl
//
//
// Description: Implementation of the SECEditCore class
//
	 
#include "SECEdit.h"
#include "oepageset.h"
#include "OEFindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// SECEditCore


/////////////////////////////////////////////////////////////////////////////
// SECEditCore construction/destruction

#if _MFC_VER >= 0x0421
template <class BC>
BOOL SECEditCore<BC>::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// We are doing nothing, but returning TRUE.
	// the reason is that if your are embedded in a Splitter, and the
	// splitter gets this message, it tries to cast the embedded view to
	// a CScrollView, which crashes

	// Unused
	nFlags;
	zDelta;
	pt;

	return TRUE;
}
#endif

//@doc SECEditCore
//@mfunc SECEditCore constructor that initializes the PageSetup dialog member

template <class BC>
SECEditCore<BC>::SECEditCore()
{
	m_iLineHeight = 15;
	m_iFirstLineNo = 1;
	m_pTopLine = m_pCurLine = NULL;
	// Paste in text after the caret by default
	// (so the caret is at the end of the pasted text)
	m_bPasteAfter = FALSE;
	dlgPageSetup.Initialize();

	// This works if it is in a dll
	CBitmap bm;
	HANDLE hBitmap = ::LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_SECEDIT_GUTTER),RT_BITMAP),
		MAKEINTRESOURCE(IDB_SECEDIT_GUTTER), IMAGE_BITMAP, 0, 0,
		LR_DEFAULTCOLOR);

	ASSERT (hBitmap != NULL);
	bm.Attach(hBitmap);
	m_brGutter.CreatePatternBrush(&bm);


	m_hAccelTable = NULL;
	m_bFinding = m_bReplacing = FALSE;
	m_bUseVirtualWhitespace = FALSE; // Set this to TRUE Keep just like it was before.
									// Set FALSE to keep cursor where stuff has been typed
	m_bFinding = FALSE;
	m_bReplacing = FALSE;

	// Used for synchronizing views in a dynamic splitter
	m_pSplitterParent = NULL;
	int m_nSplitRow = 0;
	int m_nSplitCol = 0;
	m_bScrollMessage = FALSE;

//	m_4CSpecMouseLeftButtonDown = FALSE;
}

//@doc SECEditCore 
//@mfunc Saves the dlgPageSetup members in the 
//              Registry
template <class BC>
SECEditCore<BC>::~SECEditCore()
{
	dlgPageSetup.Terminate();
	// destroy any CFindReplace dialogs
	if (m_bFinding)
		m_finddlg->DestroyWindow();
	if (m_bReplacing)
		m_replacedlg->DestroyWindow();
}

template <class BC>
BOOL SECEditCore<BC>::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// finally, translate the message
		//HACCEL hAccel = GetDefaultAccelerator();
		if((m_hAccelTable != NULL) &&  (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)))
		{
			// Handled by the Accel Table
			return TRUE;
		}
	}	
	// Else return Default
	return BC::PreTranslateMessage(pMsg);
}

//@doc SECEditCore
//@mfunc Returns NULL for the ActiveDocument
//       Overriden in SECEditView to return the Active Document
template <class BC>
CDocument* SECEditCore<BC>::GetActiveDoc()
{
	return NULL;
}

//@doc SECEditCore
//@mfunc Returns the color to be used for shading the gutter.
// @rdesc Override this function to change the appearance
// of the gutter. Return clrEmpty for no shading
// or return a colorref to be used to shade the gutter.
template <class BC>
COLORREF SECEditCore<BC>::GetGutterColor(COLORREF clrEmpty)
{
	clrEmpty;	// Unused
	return ::GetSysColor(COLOR_SCROLLBAR);
}

//@doc SECEditCore
//@mfunc Registers the Objective Edit Window class.
//@rdesc BOOL TRUE on success, FALSE on failure.
//@parm CREATESTRUCT& cs An MFC create structure.

template <class BC>
 BOOL SECEditCore<BC>::PreCreateWindow(CREATESTRUCT& cs)
{
   HCURSOR hCursor = ::LoadCursor(NULL,IDC_IBEAM);
	cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_DBLCLKS|CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT|CS_HREDRAW|CS_VREDRAW,
		            hCursor,NULL,NULL);	
	return BC::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// SECEditCore drawing

//@doc SECEditCore
//@mfunc Place holder.
//@rdesc void 
//@parm CDC* pDC
template <class BC>
void SECEditCore<BC>::OnDraw(CDC* pDC)
{
pDC; //UNUSED
	
}





/////////////////////////////////////////////////////////////////////////////
// SECEditCore diagnostics

#ifdef _DEBUG

template <class BC>
void SECEditCore<BC>::AssertValid() const
{
	BC::AssertValid();
}


template <class BC>
void SECEditCore<BC>::Dump(CDumpContext& dc) const
{
	BC::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SECEditCore message handlers

//@doc SECEditCore
//@mfunc Internal function to display an error message defined in
//              the string table
//@rdesc int 
//@parm int iCode
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::DoError(int iCode, BOOL bUpdate)
	{
	CString str;
	TCHAR szBuf[_MAX_PATH+128];

	if (!bUpdate)
		return iCode;

	if (!str.LoadString(iCode))
		{
		TRACE(_T("DoError: Undefined error code %d\r\n"),iCode);
		return iCode;
		}
	else
		_tcscpy(szBuf,str);
	
	AfxMessageBox(szBuf,MB_ICONEXCLAMATION|MB_OK);

	return iCode;
	}


//@doc SECEditCore
//@mfunc Copies the current word or selection to the CString parameter
//@rdesc BOOL 
template <class BC>
BOOL SECEditCore<BC>::GetWordOrSelection(CString& str)
{
	char szWord[OE_MAXLINELENGTH];
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	str.Empty();

	// use the current selection if it's all on one line
	if (m_BlockStart.pLine && m_BlockEnd.pLine)
		{
		if (m_BlockStart.pLine == m_BlockEnd.pLine)
			{
			if (m_BlockEnd.iOffset < OE_MAXLINELENGTH)
				{
				int iStart = pEdit->GetRealOffset(m_BlockStart.pLine,m_BlockStart.iOffset);
				int iEnd = pEdit->GetRealOffset(m_BlockEnd.pLine,m_BlockEnd.iOffset);
				int iCount = iEnd-iStart;
				strncpy(szWord,&(m_BlockStart.pLine->pText[iStart]),
				        iCount);
				szWord[iCount] = 0;
				iCount = strlen(szWord);
				if (szWord[iCount-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
					szWord[iCount-pEdit->m_iLineEndLength] = 0;
				str = szWord;
				return TRUE;
				}
			}
 		}

	PLINEDESC lpLine;
	LPSTR szText;
	int iPos,iLength,iColNo;

	lpLine = m_pCurLine;
	szText = lpLine->pText;
	if (szText)
		iLength = lpLine->iTxtSize;
	else
		iLength = 0;
	if (iLength>1)
		{
		/* (maybe) adjust for OE_CR/OE_LF */
		if (szText[iLength-pEdit->m_iLineEndLength] == pEdit->m_szLineEnd[0])
			iLength -= pEdit->m_iLineEndLength;
		}

	iPos = pEdit->GetRealOffset(lpLine,m_iColNo);

	/*  non-word:
	 *     (!isalnum(szText[iPos]) && (szText[iPos] != '_')
	 */
 
	/*  word:
	 *     (isalnum(szText[iPos]) || (szText[iPos] == '_')
	 */
	if (
		 !iLength ||
		 (iLength < iPos+1) ||
		 ((!pEdit->Myisalnum(szText[iPos]) && (szText[iPos] != '_')))
		)
		{
		return FALSE;
		}

	/* back up to the beginning of the current word */
	while (iPos && (pEdit->Myisalnum(szText[iPos-1]) || (szText[iPos-1] == '_')))
		iPos--;

	iColNo = iPos;

	/* now iPos && iColNo point to the first character of the word */
	while ((iPos <= iLength) && (pEdit->Myisalnum(szText[iPos]) || (szText[iPos] == '_')))
		iPos++;

	//iPos = pEdit->GetTabbedOffset(lpLine,iPos);

	strncpy(szWord,&(m_pCurLine->pText[iColNo]),(iPos-iColNo)+1);
	szWord[(iPos-iColNo)+1] = 0;
	str = szWord;
	return TRUE;
}

//@doc SECEditCore
//@mfunc Internal method to display an error message which includes an extra
//              LPCTSTR parameter - e.g. "Cannot open %s"
//@rdesc int 
//@parm int iCode
//@parm  LPCTSTR lpExtraText
//@parm  BOOL bUpdate
template <class BC>
int SECEditCore<BC>::DoError(int iCode, LPCTSTR lpExtraText, BOOL bUpdate)
   {

	CString str;
	TCHAR szBuf[_MAX_PATH+128];

	if (!bUpdate)
		return iCode;

	if (str.LoadString(iCode))
	   wsprintf(szBuf,str,(LPTSTR)lpExtraText);
	else
		{
		TRACE(_T("DoError: Undefined error code %d\r\n"),iCode);
		return iCode;
		}

	AfxMessageBox(szBuf, MB_ICONEXCLAMATION|MB_OK );

	return iCode;
	}


//@doc SECEditCore
//@mfunc Internal function used to determines if the location at point is inside a
//              selection.
//@rdesc BOOL TRUE if it is in the selection, FALSE if not.
//@parm CPoint point The point to test against the current selection.

template <class BC>
BOOL SECEditCore<BC>::IsPointInBlock(CPoint point) 
   {
	if (!m_bSelecting)
		return FALSE;

   PLINEDESC pLine;
   int nNumLines,nNumChars;
   int nTargetCol,iTargetLine;
	int iLineNo,iColNo;
	CPoint ptBlock;

   nNumChars = m_rect.right / m_iCharWidth;
   nNumLines = m_rect.bottom / m_iLineHeight;

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
   nTargetCol = ( (point.x-m_iMarkSpace) + (m_iCharWidth / 2) ) / m_iCharWidth;

   iTargetLine = point.y / m_iLineHeight;
   if (((iTargetLine*m_iLineHeight)+m_iLineHeight) > m_rect.bottom)
      return FALSE;

	iTargetLine += m_iFirstLineNo;
   pLine = m_pTopLine;
   if (!pLine)
      return FALSE;

	iLineNo = m_iFirstLineNo;
   while (pLine && (iTargetLine != iLineNo))
      {
      pLine = pLine->pNext;
      if (!pLine)
			return FALSE;
		iLineNo++;
      }

	if (InBlock(pLine, pEdit->GetTabbedOffset(pLine,pLine->iTxtSize), &ptBlock))
		{
		if (m_iBlockType == OE_COLUMN)
			{
			if ((nTargetCol >= ptBlock.x) && (nTargetCol <= (ptBlock.x + ptBlock.y)))
				return TRUE;
			}
		else
			{
			iColNo = nTargetCol+m_iLeftEdge;
			if (pLine == m_BlockStart.pLine)
				{
				if ((pLine != m_BlockEnd.pLine) && (iColNo >= ptBlock.x))
					return TRUE;
				else if ((iColNo >= ptBlock.x) && (iColNo <= (ptBlock.x+ptBlock.y)))
					return TRUE;
				}
			else if (pLine == m_BlockEnd.pLine)
				{
				//if (iColNo <= (ptBlock.x+ptBlock.y))
				if (iColNo < (ptBlock.x+ptBlock.y))
					return TRUE;
				}
			else
				return TRUE;

			}
		}

	return FALSE;
	}


//@doc SECEditCore
//@mfunc Internal function to initialize the SECEditCore 
//@rdesc int 0 on success, -1 on failure.
//@parm LPCREATESTRUCT lpCreateStruct
template <class BC>
int SECEditCore<BC>::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (BC::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

//@doc SECEditCore
//@mfunc Virtual method to return a COleDataSource object 
// Overide this method to have your COleDataSource derived class used in Drag and Drop operations.
template <class BC>
COleDataSource* SECEditCore<BC>::GetNewDataSource()
{
	// override this to instantiate your own
	// COleDataSource derived object
	return new COleDataSource;
}


//@doc SECEditCore
//@mfunc Virtual method to initialize a COleDataSource object.
//Overide this method to intiialize the data source used in Drag and Drop operations.
template <class BC>
BOOL SECEditCore<BC>::InitializeDataSource(COleDataSource* pDataSource, CLIPFORMAT clipFormat, HGLOBAL hGlobal)
{
	// Override this function to modify the default data source
	// handling. Return FALSE to indicate no Data Sourcing
	ASSERT_VALID(pDataSource);

	// Cache the usual CF_TEXT into the HGLOBAL
	pDataSource->CacheGlobalData(clipFormat, hGlobal);

	return TRUE;
}


//@doc SECEditCore
//@mfunc Internal method used in OLE drag and drop support.
//@rdesc DROPEFFECT 
//@parm COleDataObject* pDataObject
//@parm  DWORD dwKeyState
//@parm  CPoint point
template <class BC>
DROPEFFECT SECEditCore<BC>::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// Check to see if Data is in a format that we desire...
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

	int nTargetCol = ( (point.x-m_iMarkSpace) + (m_iCharWidth / 2) ) / m_iCharWidth;

	int nTargetLine = point.y / m_iLineHeight;
	if (((nTargetLine*m_iLineHeight)+m_iLineHeight) > m_rect.bottom)
		nTargetLine--;
	
	point.x = (nTargetCol * m_iCharWidth) + m_iMarkSpace;
	point.y = nTargetLine * m_iLineHeight;
	SetCaretPos(point);

   return (dwKeyState & MK_CONTROL) ?
             DROPEFFECT_COPY : DROPEFFECT_MOVE;
}

//@doc SECEditCore
//@mfunc Internal function used to implement OLE drag and drop.
//@rdesc DROPEFFECT 
//@parm COleDataObject* pDataObject
//@parm  DWORD dwKeyState
//@parm  CPoint point
template <class BC>
DROPEFFECT SECEditCore<BC>::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	point;

	// Check to see if Data is in a format that we desire...
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

	::CreateCaret(m_hWnd,(HBITMAP)1,2,m_iLineHeight);
	ShowCaret();
	return (dwKeyState & MK_CONTROL) ?
				DROPEFFECT_COPY : DROPEFFECT_MOVE;
}

//@doc SECEditCore
//@mfunc Internal function used to implement OLE drag and drop.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnDragLeave() 
{
	DestroyCaret();
}

//@doc SECEditCore
//@mfunc Internal function used to implement OLE drag and drop.
//@rdesc void 
//@parm LPTSTR lpDropText
//@parm  int iLength
template <class BC>
void SECEditCore<BC>::DoDropPaste(LPSTR lpDropText, int iLength)
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	int iOffset = 0;
	BOOL bColumn = FALSE;

	TEXTPOS pos;
	pos.pLine = m_pCurLine;
	pos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);

   // if it has our column block header, strip it off
   if ((iLength > 21) && !memcmp(lpDropText,"*SECEdit Column Block*",22))
		{
		bColumn = TRUE;
      iOffset = 24;
		}

   // is there more than one line?
	char search[4] = {0x0D,0x0A,0,0};
	LPSTR lpTemp = strpbrk(&lpDropText[iOffset],search);
	if (!lpTemp)
		pEdit->InsertString(&pos,&lpDropText[iOffset],TRUE,TRUE);
	else
		{
		if (bColumn)
			pEdit->ColumnPaste(&pos,&lpDropText[iOffset], iLength, TRUE);
		else
			pEdit->Paste(&pos, &lpDropText[iOffset], iLength, TRUE);
		}
}

// ((SECEditCore<BC>*)_secDndState->pDropOrigin)
//@doc SECEditCore
//@mfunc Internal function used to implement OLE drag and drop.
//@rdesc BOOL 
//@parm COleDataObject* pDataObject
//@parm  DROPEFFECT dropEffect
//@parm  CPoint point
template <class BC>
BOOL SECEditCore<BC>::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
  if ((dropEffect == DROPEFFECT_COPY || 
       dropEffect == DROPEFFECT_MOVE)) 
	  {

		if (IsPointInBlock(point))
			return FALSE;

		MouseGoTo(point.x, point.y);

		SECEdit* pEdit = GetEdit();
		ASSERT(pEdit != NULL);

		pEdit->SetUndoGroup(TRUE);

		if (_secDndState->pDropOrigin && (dropEffect == DROPEFFECT_MOVE))
			{
			TEXTPOS DropPoint;

			ASSERT_VALID(_secDndState->pDropOrigin);

			// cache the TEXTPOS of the drop point
			DropPoint.pLine = m_pCurLine;
			DropPoint.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);

// #326 08.12.06 SIS >>
#ifdef _ORIGINAL_VERSION
			if (this == _secDndState->pDropOrigin)
				{
				// move to the beginning of the block
				((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_pCurLine = m_BlockStart.pLine;
				((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_iColNo = m_BlockStart.iOffset;
				}

			// If we're dropping to a point on the same line as the marked 
			// block we need to calculate what the offset of the drop point
			// will be after the cut is done.

			if (DropPoint.pLine == ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockEnd.pLine)
				{
				if (((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockStart.pLine == ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockEnd.pLine)
					{
					if (DropPoint.iOffset > ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockEnd.iOffset)
						DropPoint.iOffset -= (((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockEnd.iOffset - ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockStart.iOffset);
					}
				else
					{
					DropPoint.pLine = ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockStart.pLine;
					DropPoint.iOffset = ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockStart.iOffset + 
						                  (DropPoint.iOffset - ((SECEditCore<BC>*)_secDndState->pDropOrigin)->m_BlockEnd.iOffset);
					}
				}
			
			((SECEditCore<BC>*)_secDndState->pDropOrigin)->Cut(FALSE,TRUE);
#else
			SECEditCore<BC>* pSECEdit = (SECEditCore<BC>*)_secDndState->pDropOrigin;

			if (this == _secDndState->pDropOrigin)
			{
				// move to the beginning of the block
				pSECEdit->m_pCurLine = m_BlockStart.pLine;
				pSECEdit->m_iColNo = m_BlockStart.iOffset;
			}
			
			// If we're dropping to a point on the same line as the marked 
			// block we need to calculate what the offset of the drop point
			// will be after the cut is done.
			
			if (DropPoint.pLine == pSECEdit->m_BlockEnd.pLine)
			{
				// SIS: block offset was tabbed offset but drop offset was real offset
				// calculate real block offset to compare
				int iBlockEndOffset = pEdit->GetRealOffset(pSECEdit->m_BlockEnd.pLine, pSECEdit->m_BlockEnd.iOffset);
				int	iBlockStartOffset = pEdit->GetRealOffset(pSECEdit->m_BlockStart.pLine, pSECEdit->m_BlockStart.iOffset);
				if (pSECEdit->m_BlockStart.pLine == pSECEdit->m_BlockEnd.pLine)
				{
					if (DropPoint.iOffset > iBlockEndOffset)
						DropPoint.iOffset -= (iBlockEndOffset - iBlockStartOffset);
				}
				else
				{
					DropPoint.pLine = pSECEdit->m_BlockStart.pLine;
					DropPoint.iOffset = iBlockStartOffset + (DropPoint.iOffset - iBlockEndOffset);
				}
			}

			pSECEdit->Cut(FALSE,TRUE);
#endif
// #326 08.12.06 SIS <<

			// move to the drop point
			m_pCurLine = DropPoint.pLine;
			m_iLineNo = pEdit->GetLineNo(DropPoint.pLine);
			m_iColNo = pEdit->GetTabbedOffset(DropPoint.pLine,DropPoint.iOffset);

			if (this != _secDndState->pDropOrigin)
				_secDndState->pDropOrigin->Invalidate();

			}

		// do the paste
		STGMEDIUM stg;

		FORMATETC fe;
		fe.cfFormat = CF_TEXT;
		fe.tymed = TYMED_HGLOBAL;
		fe.lindex = -1;
		fe.dwAspect = DVASPECT_CONTENT;
		fe.ptd = NULL;

		if (pDataObject->GetData(CF_TEXT, &stg, &fe))
			{
			LPSTR lpDropText = LPSTR(GlobalLock(stg.hGlobal));
			DoDropPaste(lpDropText, strlen(lpDropText));
			GlobalUnlock(stg.hGlobal);
			}
		else
			{
			pEdit->SetUndoGroup(FALSE);
			return FALSE;
			}

		// update the display
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		CountLines();
		MakeCursorVisible();
		UpdateViews(this,NULL,NULL);

		pEdit->SetUndoGroup();
		}
	return TRUE;  
}

//@doc SECEditCore
//@mfunc Internal function used to implement OLE drag and drop.
//@rdesc DROPEFFECT 
template <class BC>
DROPEFFECT SECEditCore<BC>::DoDragCopy()
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	TEXTPOS Start = m_BlockStart;
	TEXTPOS End = m_BlockEnd;
	LPSTR lpSource;
	int iExtra;
	LPSTR lpData;

   if (m_iBlockType == OE_COLUMN)
		{
      Start = m_BlockStart;
      End = m_BlockEnd;
      Start.iOffset = pEdit->GetRealOffset(Start.pLine,Start.iOffset);
      End.iOffset = pEdit->GetRealOffset(Start.pLine,End.iOffset);
      lpSource = pEdit->ColumnCopy(&Start,&End,FALSE);  
		iExtra = 25;
		}
   else if (m_BlockStart.pLine && m_BlockEnd.pLine)
      {
      // BlockStart and BlockEnd positions are tabbed,
      // SECEdit::Copy gets the real offset
      Start = m_BlockStart;
      End = m_BlockEnd;
      Start.iOffset = pEdit->GetRealOffset(Start.pLine,Start.iOffset);
      End.iOffset = pEdit->GetRealOffset(End.pLine,End.iOffset);
      lpSource = pEdit->Copy(&Start,&End,FALSE);
		iExtra = 1;
      }

	HGLOBAL h = GlobalAlloc(GHND | GMEM_SHARE, strlen(lpSource) + iExtra);
	lpData = (LPSTR)GlobalLock(h);
   if (m_iBlockType == OE_COLUMN)
		{
      strcpy(lpData,"*SECEdit Column Block*");
      for (int i=0;i<pEdit->m_iLineEndLength;i++)
         lpData[i+22] = pEdit->m_szLineEnd[i];
      lpData[i+22] = 0;
      strcat(lpData,lpSource);
		}
	else
		strcpy(lpData,lpSource);

	COleDataSource* pDataSource = GetNewDataSource();

	// Virtual method to give user chance to
	// modify the DataSource
	BOOL bDoDragStart = InitializeDataSource(pDataSource, CF_TEXT, h);

	GlobalUnlock(h);
	free(lpSource);

	DROPEFFECT de = DROPEFFECT_NONE;
	
	if(bDoDragStart)
	{
		ASSERT_VALID(pDataSource);
		_secDndState->pDropOrigin = this;
		de = pDataSource->DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);
		_secDndState->pDropOrigin = NULL;
		delete pDataSource;
	}
	return de;
}

//@doc SECEditCore
//@mfunc Internal message handler that moves the caret to the mouse position.  Handles 
//              selection.
//@rdesc void 
//@parm UINT nFlags
//@parm  CPoint point
template <class BC>
void SECEditCore<BC>::OnLButtonDown(UINT nFlags, CPoint point) 
   {
	PLINEDESC pTempLine;
	int iPrevLine,iPrevCol;

//	m_4CSpecMouseLeftButtonDown = TRUE;

	if (m_bSelecting)
		{
		if (nFlags & MK_SHIFT)  // extend selection to here
			{
			iPrevLine = m_iLineNo;
			iPrevCol = m_iColNo;
			pTempLine = m_pCurLine;
			m_pCurLine->bInvalidStart = TRUE;
			m_pCurLine->iInvalidStartCol = m_iColNo;
			MouseGoTo(point.x,point.y);
			if ((iPrevLine > m_iLineNo) ||
				 ((iPrevLine == m_iLineNo) && (iPrevCol > m_iColNo)))
				{
				pTempLine->bInvalidStart = FALSE;
				pTempLine->iInvalidStartCol = 0;
				pTempLine->bInvalidEnd = TRUE;
				pTempLine->iInvalidEndCol = iPrevCol;
				m_pCurLine->bInvalidStart = TRUE;
				m_pCurLine->iInvalidStartCol = m_iColNo;
				}
			else
				{
				m_pCurLine->bInvalidEnd = TRUE;
				m_pCurLine->iInvalidEndCol = m_iColNo;
				}
			UpdateSelection(m_bUpdate);
			}
		else if (IsPointInBlock(point))   // start a drag operation
			{

			if (DoDragCopy() == DROPEFFECT_NONE)
				{
				// kill the selection and start over
   			ClearSelection(m_bUpdate);
		   	MouseGoTo(point.x,point.y);
				}
			}
		else
			{
			// there was a selection, kill it and start over
   		ClearSelection(m_bUpdate);
	   	MouseGoTo(point.x,point.y);
		   StartSelection(m_iBlockType,m_bUpdate);
			GetCursorPos(&m_ptOldCursor);
			ScreenToClient(&m_ptOldCursor);
			if (!m_bTrack)
				{
			   SetCapture();
			   m_bTrack = TRUE;
			   m_iTimer=SetTimer(6,100,NULL);
   			m_bEatIt = TRUE;
				}
			}
		}
	else  // starting a new selection
		{
		BOOL bAlt = GetKeyState(VK_MENU) < 0;
		if ((m_iBlockType == OE_COLUMN) || bAlt)
			m_iBlockType = OE_COLUMN;
		else
			m_iBlockType = OE_STREAM;
		if (nFlags & MK_SHIFT)  // extend selection to here
			{
			StartSelection(m_iBlockType,m_bUpdate);
			iPrevLine = m_iLineNo;
			iPrevCol = m_iColNo;
			pTempLine = m_pCurLine;
			m_pCurLine->bInvalidStart = TRUE;
			m_pCurLine->iInvalidStartCol = m_iColNo;
			MouseGoTo(point.x,point.y);
			if ((iPrevLine > m_iLineNo) ||
				 ((iPrevLine == m_iLineNo) && (iPrevCol > m_iColNo)))
				{
				pTempLine->bInvalidStart = FALSE;
				pTempLine->iInvalidStartCol = 0;
				pTempLine->bInvalidEnd = TRUE;
				pTempLine->iInvalidEndCol = iPrevCol;
				m_pCurLine->bInvalidStart = TRUE;
				m_pCurLine->iInvalidStartCol = m_iColNo;
				}
			else
				{
				m_pCurLine->bInvalidEnd = TRUE;
				m_pCurLine->iInvalidEndCol = m_iColNo;
				}
			UpdateSelection(m_bUpdate);
			}
		else
			{
	      // move, then start selecting
			MouseGoTo(point.x,point.y);
			StartSelection(m_iBlockType,m_bUpdate);
			}
     
		GetCursorPos(&m_ptOldCursor);
		ScreenToClient(&m_ptOldCursor);
		SetCapture();
		m_bTrack = TRUE;
		m_iTimer=SetTimer(6,100,NULL);
		m_bEatIt = TRUE;
		}
   }


//@doc SECEditCore
//@mfunc Internal message handler that ends selection started in OnLButtonDown
//@rdesc void 
//@parm UINT nFlags
//@parm  CPoint point
template <class BC>
void SECEditCore<BC>::OnLButtonUp(UINT nFlags, CPoint point) 
	{
	nFlags; //UNUSED
	point; //UNUSED

	if (m_iTimer)
		{
		KillTimer(6);
		m_iTimer = 0;
		}
	if (m_bTrack)
		{
		m_bTrack = FALSE;
		ReleaseCapture();
		if (m_BlockStart.pLine &&
			 (m_BlockStart.pLine == m_BlockEnd.pLine))
			{
			int iLength = m_BlockStart.pLine->iTxtSize;
			SECEdit* pEdit = GetEdit();
			ASSERT(pEdit != NULL);
			if ((m_BlockStart.iOffset == m_BlockEnd.iOffset) ||
				!iLength ||
				((iLength - pEdit->m_iLineEndLength) < 1) ||
				(pEdit->GetRealOffset(m_BlockStart.pLine,m_BlockStart.iOffset) > (iLength - pEdit->m_iLineEndLength)))
				ClearSelection(m_bUpdate);
			}
		if (!m_bSelecting && (point.x <= (m_iMarkSpace - (m_iCharWidth / 2))))
			SelectLine(m_bUpdate);
		}

//	m_4CSpecMouseLeftButtonDown = FALSE;
   }


//@doc SECEditCore
//@mfunc Internal message handler that updates text selection started in OnLButtonDown
//@rdesc void 
//@parm UINT nFlags
//@parm  CPoint point
template <class BC>
void SECEditCore<BC>::OnMouseMove(UINT nFlags, CPoint point) 
   {
	nFlags;//UNUSED
   PLINEDESC pOldLine;
	int iOldColNo,iOldLineNo;

	if (m_bTrack)
		{
		if (point.y >= m_rect.bottom)
			{
			m_pCurLine->bInvalidStart = TRUE;
			m_pCurLine->iInvalidStartCol = m_iColNo;
			DownLine(m_bUpdate);
			m_pCurLine->bInvalidEnd = TRUE;
			m_pCurLine->iInvalidEndCol = m_iColNo;
			UpdateSelection(m_bUpdate);
			}
		else if (point.y <= m_rect.top)
			{
			m_pCurLine->bInvalidEnd = TRUE;
			m_pCurLine->iInvalidEndCol = m_iColNo;
			UpLine(m_bUpdate);
			m_pCurLine->bInvalidStart = TRUE;
			m_pCurLine->iInvalidStartCol = m_iColNo;
			UpdateSelection(m_bUpdate);
			}
		else       // inside the top and bottom
			{
			static int bReentrant;
			if (bReentrant)
				return;
			bReentrant = TRUE;
			//if (point.x <= m_rect.left)
			//	point.x = m_rect.left-1;
			//else if (point.x >= m_rect.right-1)
			//	point.x = m_rect.right+1;
			pOldLine = m_pCurLine;
			iOldLineNo = m_iLineNo;
			iOldColNo = m_iColNo;
			MouseGoTo(point.x,point.y);
			if (m_iLineNo > iOldLineNo)      // went to next line
				{
				pOldLine->bInvalidStart = TRUE;
				pOldLine->iInvalidStartCol = iOldColNo;
				m_pCurLine->bInvalidEnd = TRUE;
				m_pCurLine->iInvalidEndCol = m_iColNo;
				}
			else if (m_iLineNo < iOldLineNo) // went to prev line
				{
				m_pCurLine->bInvalidStart = TRUE;
				m_pCurLine->iInvalidStartCol = m_iColNo;
				pOldLine->bInvalidEnd = TRUE;
				pOldLine->iInvalidEndCol = iOldColNo;
				}
			else                                  // stayed on same line
				{
				if (m_iColNo > iOldColNo)     // towards end of line
					{
					m_pCurLine->bInvalidStart = TRUE;
					m_pCurLine->iInvalidStartCol = iOldColNo;
					m_pCurLine->bInvalidEnd = TRUE;
					m_pCurLine->iInvalidEndCol = m_iColNo;
					}
				else if (m_iColNo < iOldColNo)// towards beginning of line
					{
					m_pCurLine->bInvalidStart = TRUE;
					m_pCurLine->iInvalidStartCol = m_iColNo;
					m_pCurLine->bInvalidEnd = TRUE;
					m_pCurLine->iInvalidEndCol = iOldColNo;
					}
				else
					{
					// same line and column, forget it
					m_ptOldCursor = point;
					bReentrant = FALSE;
					return;
					}
				}
			bReentrant = FALSE;
			UpdateSelection(m_bUpdate);
			}
		m_ptOldCursor = point;
      }
   }


//@doc SECEditCore
//@mfunc Internal timer handler. Dragging the mouse outside the client area extends
//              the selection in that direction.  OnLButtonDown 
//              creates this timer so we can fake WM_MOUSEMOVE
//              messages.
//@rdesc void 
//@parm UINT nIDEvent
template <class BC>
void SECEditCore<BC>::OnTimer(UINT nIDEvent) 
	{
	POINT ptCursor;

	if (nIDEvent == 6)
		{
		if (m_bEatIt)
			{
			m_bEatIt = FALSE;
			return;
			}
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		if ((ptCursor.y == m_ptOldCursor.y) &&
			 (ptCursor.x == m_ptOldCursor.x) &&
			 (ptCursor.y < m_rect.bottom) &&
			 (ptCursor.y > m_rect.top) &&
			 (ptCursor.x < m_rect.right) &&
			 (ptCursor.x > m_rect.left))
			return;
		else
			{
			m_ptOldCursor = ptCursor;
			OnMouseMove(0,ptCursor);
			}
		}
	}

//@doc SECEditCore
//@mfunc Internal double click message handler.
//@rdesc void 
//@parm UINT nFlags
//@parm  CPoint point
template <class BC>
void SECEditCore<BC>::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	point;  //UNUSED
	nFlags; //UNUSED
	SelectWord(m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal paint handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnPaint() 
{
	Paint();
}


//@doc SECEditCore
//@mfunc Internal context menu handler.
//@rdesc void 
//@parm CWnd* pWnd
//@parm  CPoint point
template <class BC>
void SECEditCore<BC>::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	if (menu.CreatePopupMenu()) 
	{
#ifdef ORIGINAL_VERSION
#else
		m_ContextMenuPoint = point;
		ScreenToClient (&m_ContextMenuPoint);
#endif

		// Set up the menu
		if(ExtendContextMenu(&menu))
		{
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	} 
	else
		BC::OnContextMenu(pWnd, point);
}

//@doc SECEditCore
//@mfunc Virtual method called to initialize the context menu. 
// Override this to set up
// the context menu. Call the base class first to add your items to the end of the menu.
//@rdesc BOOL. Return false to not show the context menu 
//@parm  CMenu* pMenu. Menu to be popped up.
template <class BC>
BOOL SECEditCore<BC>::ExtendContextMenu(CMenu* pMenu)
{
	// Set up the menu
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	DWORD dwStyle;

	dwStyle = pEdit->m_iUndoPos ? MF_STRING : MF_GRAYED|MF_STRING;
	pMenu->AppendMenu(dwStyle, ID_EDIT_UNDO, _T("&Undo"));
	
	dwStyle = (pEdit->m_iUndoCount && (pEdit->m_iUndoPos < pEdit->m_iUndoCount)) ? MF_STRING : MF_GRAYED|MF_STRING;
	pMenu->AppendMenu(dwStyle, ID_EDIT_REDO, _T("&Redo"));
	
	pMenu->AppendMenu(MF_SEPARATOR);

	dwStyle = m_bSelecting ? MF_STRING : MF_GRAYED|MF_STRING;
	pMenu->AppendMenu(dwStyle, ID_EDIT_CUT, _T("Cu&t"));
	pMenu->AppendMenu(dwStyle, ID_EDIT_COPY, _T("&Copy"));
	
	if (OpenClipboard())
		{
		if (IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_OEMTEXT))
			dwStyle = MF_STRING;
		else
			dwStyle = MF_GRAYED|MF_STRING;
		CloseClipboard();
		}
	else
		dwStyle = MF_GRAYED|MF_STRING;
	pMenu->AppendMenu(dwStyle, ID_EDIT_PASTE, _T("&Paste"));
	
	pMenu->AppendMenu(MF_SEPARATOR);
	
	dwStyle = MF_STRING;
	pMenu->AppendMenu(dwStyle, ID_BOOKMARKSET, _T("Toggle &Bookmark"));

	return TRUE;
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditBottomfile() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	BottomOfFile(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler
//@rdesc void
template <class BC>
void SECEditCore<BC>::OnToggleInsert() 
{
	SECEdit* pEdit = GetEdit();
	pEdit->SetInsertMode(!pEdit->GetInsertMode());
}
template <class BC>
void SECEditCore<BC>::OnUpdateToggleInsert(CCmdUI* pCmdUI) 
{
	SECEdit* pEdit = GetEdit();
	pCmdUI->SetText(pEdit->GetInsertMode() ? afxEmptyString : _T("OVR"));
}
//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditDown() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	DownLine(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditEnd() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	EndOfLine(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Inernal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditHome() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	BeginningOfLine(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditLeft() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	CursorLeft(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditPgdn() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	DownPage(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditPgup() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	UpPage(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditRight() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	CursorRight(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditTopfile() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	TopOfFile(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditUp() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	UpLine(m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditWordleft() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	WordLeft(m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditWordright() 
{
   if (m_bSelecting)
		ClearSelection(m_bUpdate);
	WordRight(m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal OnSize handler that caches the view's current size.
//@rdesc void 
//@parm UINT nType
//@parm  int cx
//@parm  int cy
template <class BC>
void SECEditCore<BC>::OnSize(UINT nType, int cx, int cy) 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	m_rect.left = 0;
	m_rect.right = cx;
	m_rect.top = 0;
	m_rect.bottom = cy;
	MySetScrollRange(SB_VERT, 0, pEdit->m_iLinesInFile-1,TRUE); 
	if (m_iFirstLineNo)
		MySetScrollPos(SB_VERT, m_iFirstLineNo-1, TRUE);
	BC::OnSize(nType, cx, cy);
}

//@doc SECEditCore
//@mfunc Internal scroll handler.
//@rdesc BOOL 
//@parm UINT nScrollCode
//@parm  UINT nPos
//@parm  BOOL bDoScroll
template <class BC>
BOOL SECEditCore<BC>::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	BYTE horzcode = LOBYTE(nScrollCode);
	BYTE vertcode = HIBYTE(nScrollCode);
	BOOL bRval = FALSE;

	switch (horzcode)
		{
		case SB_LINEUP:
			bRval = (m_iLeftEdge > 0);
			if (bDoScroll)
				OnHScroll(horzcode, nPos, NULL);
			break;
		case SB_LINEDOWN:
			bRval = (m_iLeftEdge < OE_MAXLINELENGTH);
			if (bDoScroll)
				OnHScroll(horzcode, nPos, NULL);
			break;
		}

	switch (vertcode)
		{
      case SB_LINEUP:
		  if(m_pTopLine == NULL)
			  break;
			bRval = (m_pTopLine->pPrev != NULL);
			if (bDoScroll)
				OnVScroll(vertcode, nPos, NULL);
         break;

      case SB_LINEDOWN:
		  if(m_pCurLine == NULL)
			  break;
			bRval = (m_pCurLine->pNext != NULL);
			if (bDoScroll)
				OnVScroll(vertcode, nPos, NULL);
         break;
		}

	return bRval;
}

//@doc SECEditCore
//@mfunc Internal scroll handler.
//@rdesc void 
//@parm UINT nSBCode
//@parm  UINT nPos
//@parm  CScrollBar* pScrollBar
template <class BC>
void SECEditCore<BC>::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_bScrollMessage = TRUE;

	pScrollBar; //UNUSED
	int iOldLeftEdge = m_iLeftEdge;
	switch (nSBCode)
		{
      case SB_LINEDOWN:
			if (m_iLeftEdge < OE_MAXLINELENGTH)
				m_iLeftEdge++;
         break;

      case SB_LINEUP:
			if (m_iLeftEdge)
				m_iLeftEdge--;
         break;

      case SB_PAGEDOWN:
			m_iLeftEdge += OE_HPAGESIZE;
			if (m_iLeftEdge > OE_MAXLINELENGTH)
				m_iLeftEdge = OE_MAXLINELENGTH;
         break;

      case SB_PAGEUP:
			m_iLeftEdge -= OE_HPAGESIZE;
			if (m_iLeftEdge < 0)
				m_iLeftEdge = 0;
         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         m_iLeftEdge = nPos;
         break;

      case SB_ENDSCROLL:
         MySetScrollPos(SB_HORZ,m_iLeftEdge,m_bUpdate);
      }
	if (iOldLeftEdge != m_iLeftEdge)
		{
      MySetScrollPos(SB_HORZ,m_iLeftEdge,m_bUpdate);
		Invalidate();
		MySetCaretPos();
		}

	m_bScrollMessage = FALSE;
}

//@doc SECEditCore
//@mfunc Internal scroll handler.
//@rdesc void 
//@parm UINT nSBCode
//@parm  UINT nPos
//@parm  CScrollBar* pScrollBar
template <class BC>
void SECEditCore<BC>::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_bScrollMessage = TRUE;

	pScrollBar;//UNUSED
	WORD wPos = (WORD)nPos;
	int iPos;
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	switch (nSBCode)
		{
      case SB_LINEDOWN:
         ScrollDownLine(m_bUpdate);
         break;

      case SB_LINEUP:
         ScrollUpLine(m_bUpdate);
         break;

      case SB_PAGEDOWN:
         ScrollDownPage(TRUE);
         MySetScrollPos(SB_VERT,m_iFirstLineNo-1,m_bUpdate);
         break;

      case SB_PAGEUP:
         ScrollUpPage(TRUE);
         MySetScrollPos(SB_VERT,m_iFirstLineNo-1,m_bUpdate);
         break;

      case SB_THUMBPOSITION:
			iPos = wPos * m_iVScale;
			ScrollGoToLine(iPos+1,m_bUpdate);
			MySetScrollPos(SB_VERT,m_iFirstLineNo-1,m_bUpdate);
         break;

      case SB_THUMBTRACK:
			iPos = wPos * m_iVScale;
			ScrollGoToLine(iPos+1,m_bUpdate);
			MySetScrollPos(SB_VERT,m_iFirstLineNo-1,m_bUpdate);
         break;
	}
	m_bScrollMessage = FALSE;
}



//@doc SECEditCore
//@mfunc Internal keyboard handler.
//@rdesc void 
//@parm UINT nChar
//@parm  UINT nRepCnt
//@parm  UINT nFlags
template <class BC>
void SECEditCore<BC>::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	// Do not allow typing while the left mouse button is down
	/*if (((GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? 
			VK_RBUTTON : VK_LBUTTON))>>8) & 0x0ff)
		return;*/

//	if (m_4CSpecMouseLeftButtonDown)
//		return;

	// Bail Out if read only
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit);
#ifdef ORIGINAL_VERSION
	if(pEdit->GetReadOnly())
        return;
#else
    if (!OnModifyContent () || pEdit->GetReadOnly())
        return;
#endif

	nFlags;//UNUSED
	nRepCnt;//UNUSED
	switch (nChar)
		{
		case VK_RETURN:
			{
			SECEdit* pEdit = GetEdit();
			ASSERT(pEdit != NULL);
			pEdit->SetUndoGroup(TRUE);
			if (m_bSelecting)
				OnEditCut();
			SplitLine(TRUE,TRUE);
			pEdit = GetEdit();
			ASSERT(pEdit != NULL);
			pEdit->SetUndoGroup(FALSE);
			}
			break;
		case VK_ESCAPE:
			if (m_bSelecting)
				ClearSelection(m_bUpdate);

		default:
			// want any char but control char (not only 7 bit ASCII)
#ifdef ORIGINAL_VERSION
			if (nChar > 31 && nChar < 127)
#else
			if (nChar > 31)
#endif
				{
				int iRval = InsertChar(nChar,m_bUpdate,TRUE);
				if (iRval)
					DoError(iRval,m_bUpdate);
				}
			break;
		}
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectDown() 
{
	SelectDown(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectEnd() 
{
	SelectEnd(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectHome() 
{
	SelectHome(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectLeft() 
{
	SelectLeft(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectPgdn() 
{
	SelectPageDown(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectPgup() 
{
	SelectPageUp(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectRight() 
{
	SelectRight(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectUp() 
{
	SelectUp(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectWordleft() 
{
	SelectWordLeft(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectWordright() 
{
	SelectWordRight(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectAll() 
{
	SelectAll(m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable();
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectTop() 
{
	SelectTopOfFile(m_iBlockType,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditSelectBottom() 
{
	SelectEndOfFile(m_iBlockType,m_bUpdate);
}


//@doc SECEditCore
//@mfunc Internal killfocus handler - destroys the caret and hides any selection.
//@rdesc void 
//@parm CWnd* pNewWnd
template <class BC>
void SECEditCore<BC>::OnKillFocus(CWnd* pNewWnd) 
{
	BC::OnKillFocus(pNewWnd);
	
	if (m_bHaveCaret)
		{
	   DestroyCaret();
		m_bHaveCaret = FALSE;
		}
	if (m_bSelecting)
		ClearBlockFromScreen();
}


//@doc SECEditCore
//@mfunc Internal focus handler - creates the caret and shows the selection.
//@rdesc void 
//@parm CWnd* pOldWnd
template <class BC>
void SECEditCore<BC>::OnSetFocus(CWnd* pOldWnd) 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	// m_pActiveView is a CWnd*
	pEdit->m_pActiveView = (CWnd*)this;

	BC::OnSetFocus(pOldWnd);

	if (!m_bHaveCaret && !m_bSelecting)
   	{
	   ::CreateCaret(m_hWnd,NULL,2,m_iLineHeight);
		m_bHaveCaret = TRUE;
   	ShowCaret();
		}
	if (m_bSelecting)
		InvalidateRect(NULL,FALSE);
	MySetCaretPos();

}


//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditCopy() 
{
   int iRval = 0;

	if (m_bSelecting || m_bCopyLine)
   	iRval = Copy(m_bUpdate,TRUE);
	if (iRval)
		DoError(iRval,m_bUpdate);
	
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
   BOOL bEnable = (m_bCopyLine || m_bSelecting);
   pCmdUI->Enable(bEnable);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditCut() 
{
	// If read only do a copy instead
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit);
	if(pEdit->GetReadOnly())
	{
		OnEditCopy();
		return;
	}

   int iRval = 0;

	if (m_bSelecting || m_bCutLine)
		{
		iRval = Copy(m_bUpdate,TRUE);
		if (!iRval)
   	   iRval = Cut(m_bUpdate,TRUE);
		}
	if (iRval)
		DoError(iRval,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
   BOOL bEnable = (m_bCutLine || m_bSelecting);

	// Set False if read only
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit);
	if(pEdit->GetReadOnly())
		bEnable = FALSE;

   pCmdUI->Enable(bEnable);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditPaste() 
{
	// Bail out if read only
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit);
#ifdef ORIGINAL_VERSION
	if(pEdit->GetReadOnly())
		return;
#else
    if (!OnModifyContent () || pEdit->GetReadOnly())
        return;
#endif

	int iRval;
	
   iRval = Paste(m_bUpdate,TRUE);
	if (iRval)
		DoError(iRval,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;

	if (OpenClipboard())
		{
		bEnable =  (IsClipboardFormatAvailable(CF_TEXT)
				|| IsClipboardFormatAvailable(CF_OEMTEXT));
		CloseClipboard();
		}

	// Set False if read only
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit);
	if(pEdit->GetReadOnly())
		bEnable = FALSE;

   pCmdUI->Enable(bEnable);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditClear() 
{
	// Bail out if read only
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit);
#ifdef ORIGINAL_VERSION
	if(pEdit->GetReadOnly())
        return;
#else
    if (!OnModifyContent () || pEdit->GetReadOnly())
        return;
#endif

	int iRval = DeleteChar(m_bUpdate,TRUE);
	if (iRval)
		DoError(iRval,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditBackspace() 
{
#ifndef ORIGINAL_VERSION
    if (!OnModifyContent ())
        return;
#endif

	int iRval = BackSpace(m_bUpdate,TRUE);
	if (iRval)
		DoError(iRval,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditBacktab() 
{
#ifndef ORIGINAL_VERSION
    if (!OnModifyContent ())
        return;
#endif

	int iRval = BackTab(m_bUpdate,TRUE);
	if (iRval)
		DoError(iRval,m_bUpdate);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditTab() 
{
#ifndef ORIGINAL_VERSION
    if (!OnModifyContent ())
        return;
#endif

	int iRval = InsertTab(m_bUpdate,TRUE);
	if (iRval)
		DoError(iRval,m_bUpdate);
}


//@doc SECEditCore
//@mfunc Create or activate the Replace dialog, unless the
//              Find dialog is already active.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditReplace() 
{
   if (m_bFinding)
		{
		m_finddlg->SetFocus();
		return;
		}
   if (!m_bReplacing)
		{
		m_bReplacing = TRUE;
	   m_replacedlg = new OEFindDlg;
	   m_replacedlg->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD_OE_REPLACEDLGORD);
		m_replacedlg->m_fr.hInstance = 
			AfxFindResourceHandle( m_replacedlg->m_fr.lpTemplateName, RT_DIALOG );
		m_replacedlg->m_bReplacing = TRUE;
		InitFindDialog(m_replacedlg);
	   m_replacedlg->Create(FALSE,afxEmptyString,afxEmptyString,FR_DOWN|FR_ENABLETEMPLATE,this);
		m_replacedlg->SetActiveWindow();
		m_replacedlg->ShowWindow(SW_SHOW);
		}
	else
		m_replacedlg->SetFocus();
}

//@doc SECEditCore
//@mfunc Internal command handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


//@doc SECEditCore
//@mfunc Create or activate the Find dialog, unless the 
//              Replace dialog is already active.
//@rdesc void 

template <class BC>
void SECEditCore<BC>::OnEditFind() 
{
   if (m_bReplacing)
		{
		m_replacedlg->SetFocus();
		return;
		}
   if (!m_bFinding)
		{
		m_bFinding = TRUE;
	   m_finddlg = new OEFindDlg;
	   m_finddlg->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD_OE_FINDDLGORD);
		m_finddlg->m_fr.hInstance = 
			AfxFindResourceHandle( m_finddlg->m_fr.lpTemplateName, RT_DIALOG );
		m_finddlg->m_bReplacing = FALSE;
		InitFindDialog(m_finddlg);
	   m_finddlg->Create(TRUE,afxEmptyString,NULL,FR_DOWN|FR_ENABLETEMPLATE,this);
		m_finddlg->SetActiveWindow();
		m_finddlg->ShowWindow(SW_SHOW);
		}
	else
		m_finddlg->SetFocus();
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm OEFindDlg* dlg
template <class BC>
void SECEditCore<BC>::SetLineAvoidance(OEFindDlg* dlg)
   {
	CRect dlgrect;
	dlg->GetWindowRect(&dlgrect);
	m_badrect = dlgrect;
	}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm PSEARCHRECORD pSearch
template <class BC>
void SECEditCore<BC>::DoFindMarkAll(PSEARCHRECORD pSearch)
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	//OnBookmarkdelete();   // clear all existing bookmarks
	TEXTPOS StartPos;
	TEXTPOS EndPos;
	StartPos.pLine = pEdit->ListGetFirst(m_pCurLine,NULL);
	StartPos.iOffset = 0;
	EndPos.pLine = NULL;
	EndPos.iOffset = 0;
	pSearch->fForward = TRUE;

	while (pEdit->Find(&StartPos,&EndPos,pSearch))
		{
		// set a bookmark on this line
		pEdit->m_iBookmarkCount++;
		StartPos.pLine->fBookMark = TRUE;
		StartPos.iOffset += pSearch->iReturnLength;
		}
	if (pEdit->m_iBookmarkCount)
	{
		TopOfFile(FALSE);
		OnBookmarknext();
		Invalidate();
	}
}


//@doc SECEditCore
//@mfunc Internal handler. Handles the registered WM_FINDREPLACE message sent 
//              by the Find/Replace common dialog
//@rdesc LONG 
//@parm WPARAM wParam
//@parm  LPARAM lParam
template <class BC>
LONG SECEditCore<BC>::OnFindReplace(WPARAM wParam, LPARAM lParam)
   {
	USES_CONVERSION_T;

	lParam; //UNUSED
	wParam;//UNUSED

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	TEXTPOS StartPos;
	TEXTPOS EndPos;
	SEARCHRECORD Search;
	static LPSTR lpSaveFoundText = NULL;
	char str[OE_MAXLINELENGTH+1];
	char str2[OE_MAXLINELENGTH+1];
	CString find;
	CString replace;
	OEFindDlg* dlg;
	static BOOL bFoundOnce = FALSE;
	static CString strReplace = afxEmptyString;
	BOOL bFirstFind = FALSE;

	if (m_bFinding)
		dlg = m_finddlg;
	else if (m_bReplacing)
		dlg = m_replacedlg;
	else
		return 0L;

	dlg->UpdateData(TRUE);

	if (dlg->IsTerminating())
		{
		m_bFinding = FALSE;
		m_bReplacing = FALSE;
		m_finddlg = m_replacedlg = dlg = NULL;
		bFoundOnce = FALSE;
		strReplace.Empty();
		if (lpSaveFoundText)
			{
			free(lpSaveFoundText);
			lpSaveFoundText = NULL;
			}
		return 0L;
		}
	else 
		{
		SaveFindDlgSettings(dlg);
		if (m_bReplacing)
			SaveReplaceDlgSettings(dlg);
		Search.fComplain = 0;
#ifdef ORIGINAL_VERSION
		Search.fMatchCase = dlg->MatchCase();
#else
		Search.fMatchCase = dlg->m_bMatchCase;
#endif
		Search.fForward = dlg->m_iUp;
		Search.fRegular = dlg->m_bRegular;
		find = dlg->m_FindWhat;
		Search.lpSearchText = str;
		Search.lpReplaceText = NULL;
		strcpy(Search.lpSearchText,T2CA(find));
		if (m_bReplacing)
			{
			replace = dlg->m_ReplaceWith;
			Search.lpReplaceText = str2;
			strcpy(Search.lpReplaceText,T2CA(replace));
			}
		}
											
	if (dlg->FindNext())
		{
doitagain:
		if (m_bSelecting)
			{
			if (Search.fForward)
				{
				m_pCurLine = m_BlockEnd.pLine;
				m_iLineNo = pEdit->GetLineNo(m_pCurLine);
				m_iColNo = m_BlockEnd.iOffset;
				}
			else
				{
				m_pCurLine = m_BlockStart.pLine;
				m_iLineNo = pEdit->GetLineNo(m_pCurLine);
				m_iColNo = m_BlockStart.iOffset;
				}
			if (!bFoundOnce)
				{
			   ClearSelection(m_bUpdate);
				InvalidateRect(NULL,FALSE);
			   MakeCursorVisible();
				}
			}

		StartPos.pLine = m_pCurLine;
		StartPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
		EndPos.pLine = NULL;
		EndPos.iOffset = 0;

		if (dlg->m_bMarkAll)
			{
			m_bFinding = FALSE;
			m_bReplacing = FALSE;
			m_finddlg = m_replacedlg = NULL;
			dlg->DestroyWindow();
			bFoundOnce = FALSE;
			DoFindMarkAll(&Search);
			strReplace.Empty();
			return 0L;
			}

		if (!pEdit->Find(&StartPos,&EndPos,&Search))
			{
			//strReplace.Empty();
			if (dlg->m_bWrapSearch)
				{
				// wrap around to search the whole file
				EndPos = StartPos;
				if (Search.fForward)
					{
					StartPos.pLine = pEdit->ListGetFirst(m_pCurLine, NULL);
					StartPos.iOffset = 0;
					}
				else
					{
					if (Search.lpReplaceText)
						EndPos.iOffset += strlen(Search.lpReplaceText);
					StartPos.pLine = pEdit->ListGetLast(m_pCurLine);
					StartPos.iOffset = StartPos.pLine->iTxtSize;
					}
				if (!pEdit->Find(&StartPos,&EndPos,&Search))
					{
					strReplace.Empty();
					if (bFoundOnce)
						{
						DoError(IDS_OE_FINISHEDSEARCH,A2CT(str),m_bUpdate);
						m_bFinding = FALSE;
						m_bReplacing = FALSE;
						m_finddlg = m_replacedlg = NULL;
						dlg->DestroyWindow();
						dlg = NULL;
						bFoundOnce = FALSE;
						return 0L;
						}
					else
						DoError(IDS_OE_NOTFOUND,A2CT(str),m_bUpdate);
					dlg->GetDlgItem(IDC_FINDCOMBO1)->SetFocus();
					if (m_bReplacing)
						{
						dlg->GetDlgItem(IDC_OE_PSH1)->EnableWindow(FALSE);
						dlg->GetDlgItem(IDC_OE_PSH2)->EnableWindow(FALSE);
						}
      			return 0L;
					}
				if (bFoundOnce && (StartPos.pLine == m_pCurLine) && (StartPos.iOffset == m_iColNo))
					{
					DoError(IDS_OE_FINISHEDSEARCH,A2CT(str),m_bUpdate);
					m_bFinding = FALSE;
					m_bReplacing = FALSE;
					m_finddlg = m_replacedlg = NULL;
					dlg->DestroyWindow();
					dlg = NULL;
					bFoundOnce = FALSE;
					strReplace.Empty();
					return 0L;
					}
				}
			else
				{
				strReplace.Empty();
				if (bFoundOnce)
					{
					DoError(IDS_OE_FINISHEDSEARCH,A2CT(str),m_bUpdate);
					m_bFinding = FALSE;
					m_bReplacing = FALSE;
					m_finddlg = m_replacedlg = NULL;
					dlg->DestroyWindow();
					dlg = NULL;
					bFoundOnce = FALSE;
					return 0L;
					}
				else
					DoError(IDS_OE_NOTFOUND,A2CT(str),m_bUpdate);
				dlg->GetDlgItem(IDC_FINDCOMBO1)->SetFocus();
				if (m_bReplacing)
					{
					dlg->GetDlgItem(IDC_OE_PSH1)->EnableWindow(FALSE);
					dlg->GetDlgItem(IDC_OE_PSH2)->EnableWindow(FALSE);
					}
      		return 0L;
				}
			}
		else
			{
			if (m_bReplacing)
				{
				dlg->GetDlgItem(IDC_OE_PSH1)->EnableWindow(TRUE);
				dlg->GetDlgItem(IDC_OE_PSH2)->EnableWindow(TRUE);
				}
			strReplace = Search.lpReplaceText;
			bFoundOnce = TRUE;
			}

		if (m_bReplacing)
		   SetLineAvoidance(dlg);
		SelectFoundText(&StartPos,Search.iReturnLength,m_bUpdate);
		// save our selected text.  we do this each time
		// because with regular expressions, the found text
		// doesn't necessarily match the search text
		if (lpSaveFoundText)
			free(lpSaveFoundText);
		lpSaveFoundText = GetSelection();

// SIS: bug fix d-1097
#ifdef ORIGINAL_VERSION
		if (m_bFinding)
			{
			m_bFinding = FALSE;
			m_bReplacing = FALSE;
			m_finddlg = m_replacedlg = NULL;
			dlg->DestroyWindow();
			dlg = NULL;
			bFoundOnce = FALSE;
			}
#endif
		if(bFirstFind && dlg && dlg->ReplaceCurrent())
		{
			bFirstFind = FALSE;
			goto replacefirst;
		}
		}
	else if (dlg->ReplaceCurrent())
		{
		if (!bFoundOnce)
		{
			bFirstFind = TRUE;
			goto doitagain;
		}
replacefirst:
		// since our dialog is modeless, make sure the 
		// selected text is really what we're supposed to
		// be replacing. if not, search again
		{
		LPSTR lpSelected = GetSelection();
		if (!lpSelected || strcmp(lpSelected,lpSaveFoundText))
			{
			bFirstFind = TRUE;
			free(lpSelected);
			goto doitagain;
			}
		free(lpSelected);
		}

		SaveFindDlgSettings(dlg);
		SaveReplaceDlgSettings(dlg);
		strReplace = Search.lpReplaceText;
#ifndef ORIGINAL_VERSION
        int iSavedInsertMode;
        iSavedInsertMode = pEdit->m_bInsertMode;
        pEdit->m_bInsertMode = TRUE;
#endif
      InsertString((LPTSTR)(LPCTSTR)strReplace, m_bUpdate, TRUE);
#ifndef ORIGINAL_VERSION
      pEdit->m_bInsertMode = iSavedInsertMode;
#endif
		if (!Search.fForward)
			m_iColNo -= strReplace.GetLength();
      dlg->GetDlgItem(IDC_OE_PSH1)->EnableWindow(TRUE);
      dlg->GetDlgItem(IDC_OE_PSH2)->EnableWindow(TRUE);
	   dlg->GetDlgItem(IDC_OE_PSH1)->SetFocus();
		bFoundOnce = TRUE;
		goto doitagain;
		}
	else if (dlg->ReplaceAll())
		{
		TEXTPOS TextPos;
		int iCount = 0;
		CWaitCursor wait;
		SaveFindDlgSettings(dlg);
		SaveReplaceDlgSettings(dlg);
		TextPos.pLine = m_pCurLine;
		TextPos.iOffset = m_iColNo;
		m_pCurLine = pEdit->ListGetFirst(m_pCurLine, NULL);
		m_iColNo = 0;
		BOOL bSaveUpdate = m_bUpdate;
		m_bUpdate = FALSE;
		ClearSelection(FALSE);
		StartPos.pLine = m_pCurLine;
		StartPos.iOffset = 0;
		EndPos.pLine = NULL;
		EndPos.iOffset = 0;
		Search.fForward = TRUE;
      dlg->SendDlgItemMessage(IDC_FINDCOMBO1, WM_GETTEXT, sizeof(str),(LPARAM)str);
      dlg->SendDlgItemMessage(IDC_FINDCOMBO2, WM_GETTEXT, sizeof(str),(LPARAM)str2);
		Search.lpSearchText = str;
		Search.lpReplaceText = str2;
		int iReplaceLength = strlen(Search.lpReplaceText);
		while (pEdit->Find(&StartPos,&EndPos,&Search))
			{
			SelectFoundText(&StartPos,Search.iReturnLength,FALSE);
#ifndef ORIGINAL_VERSION
        int iSavedInsertMode;
        iSavedInsertMode = pEdit->m_bInsertMode;
        pEdit->m_bInsertMode = TRUE;
#endif
         InsertString(A2T(Search.lpReplaceText), FALSE, TRUE);
#ifndef ORIGINAL_VERSION
      pEdit->m_bInsertMode = iSavedInsertMode;
#endif
		   iCount++;
			StartPos.iOffset += iReplaceLength;
			Search.lpReplaceText = str2;
			}
		m_bUpdate = bSaveUpdate;
		if (!iCount)
			{
			m_pCurLine = TextPos.pLine;
			m_iColNo = TextPos.iOffset;
		   DoError(IDS_OE_NOTFOUND,A2CT(str),m_bUpdate);
			}
		else
			{
			ClearInvalidMarkers();
			m_bFinding = FALSE;
			m_bReplacing = FALSE;
			m_finddlg = m_replacedlg = NULL;
			dlg->DestroyWindow();
			dlg = NULL;
			MakeCursorVisible();
			InvalidateRect(NULL,FALSE);
			UpdateViews(this, NULL, NULL);
			bFoundOnce = FALSE;
			return 0L;
			}
		strReplace.Empty();
		}
	return 0L;
	}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditFindnext(CCmdUI* pCmdUI) 
{
	CString section; section.LoadString(IDS_OE_SETTINGSFIND);
	CString temp; temp.LoadString(IDS_OE_FINDPERCENT);
	CString entry; entry.Format(temp,1);

	CString str; 
	str = AfxGetApp()->GetProfileString(section,entry,afxEmptyString);
	pCmdUI->Enable(!str.IsEmpty());
}


//@doc SECEditCore
//@mfunc Internal handler. Find Next - if we have enough information to do a Find.
//              Consult the Registry to get our current settings.
//
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditFindnext()
   {
	USES_CONVERSION_T;

	TEXTPOS StartPos;
	TEXTPOS EndPos;
	SEARCHRECORD Search;	
	CString section; section.LoadString(IDS_OE_SETTINGSFIND);
	CString temp; temp.LoadString(IDS_OE_FINDPERCENT);
	CString entry; entry.Format(temp,1);
	CString find;
	char str[OE_MAXLINELENGTH+1];
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	m_bFoundNext = FALSE;
	if (m_bFinding)
		{
      m_finddlg->SetFocus();
		return;
		}
	find =  AfxGetApp()->GetProfileString(section,entry,afxEmptyString);
	if (find.IsEmpty())
		{
		OnEditFind();
		return;
		}
	Search.lpReplaceText = NULL;
	Search.lpSearchText = str;
	strcpy(Search.lpSearchText,T2CA(find));
	temp.LoadString(IDS_OE_REGULAR);
	Search.fRegular = AfxGetApp()->GetProfileInt(section,temp,0);
	temp.LoadString(IDS_OE_MATCHCASE);
	Search.fMatchCase = AfxGetApp()->GetProfileInt(section,temp,0);
	temp.LoadString(IDS_OE_FORWARD);
	Search.fForward = AfxGetApp()->GetProfileInt(section,temp,1);
	temp.LoadString(IDS_OE_WRAPSEARCH);
	m_bWrapSearch = AfxGetApp()->GetProfileInt(section,temp,1);
	Search.fComplain = 0;

	if (m_bSelecting)
		{
		if (Search.fForward)
			{
			m_pCurLine = m_BlockEnd.pLine;
			m_iLineNo = pEdit->GetLineNo(m_pCurLine);
			m_iColNo = m_BlockEnd.iOffset;
			}
		else
			{
			m_pCurLine = m_BlockStart.pLine;
			m_iLineNo = pEdit->GetLineNo(m_pCurLine);
			m_iColNo = m_BlockStart.iOffset;
			}
		ClearSelection(m_bUpdate);
		MakeCursorVisible();
		}
	StartPos.pLine = m_pCurLine;
	StartPos.iOffset = pEdit->GetRealOffset(m_pCurLine,m_iColNo);
	EndPos.pLine = NULL;
	EndPos.iOffset = 0;
	if (!pEdit->Find(&StartPos,&EndPos,&Search))
		{
		if (m_bWrapSearch)
			{
			// wrap around to search the whole file
			EndPos = StartPos;
			if (Search.fForward)
				{
				StartPos.pLine = pEdit->ListGetFirst(m_pCurLine, NULL);
				StartPos.iOffset = 0;
				}
			else
				{
				EndPos.iOffset += strlen(Search.lpSearchText);
				StartPos.pLine = pEdit->ListGetLast(m_pCurLine);
				StartPos.iOffset = StartPos.pLine->iTxtSize;
				}
			if (!pEdit->Find(&StartPos,&EndPos,&Search))
				{
				DoError(IDS_OE_NOTFOUND,A2CT(str),m_bUpdate);
      		return;
				}
			}
		else
			{
			DoError(IDS_OE_NOTFOUND,A2CT(str),m_bUpdate);
			return;
			}
		}

	// else we found it
	SelectFoundText(&StartPos,Search.iReturnLength,m_bUpdate);
	m_bFoundNext = TRUE;
	}


//@doc SECEditCore
//@mfunc Internal handler that saves the current Find settings in the Registry
//@rdesc void 
//@parm OEFindDlg* dlg
template <class BC>
void SECEditCore<BC>::SaveFindDlgSettings(OEFindDlg* dlg)
   {
	CString section; section.LoadString(IDS_OE_SETTINGSFIND);
	CString temp;

	temp.LoadString(IDS_OE_REGULAR);
	AfxGetApp()->WriteProfileInt(section,temp,dlg->m_bRegular);
	temp.LoadString(IDS_OE_MATCHCASE);
	AfxGetApp()->WriteProfileInt(section,temp,dlg->m_bMatchCase);
	temp.LoadString(IDS_OE_FORWARD);
	AfxGetApp()->WriteProfileInt(section,temp,dlg->m_iUp);
	temp.LoadString(IDS_OE_WRAPSEARCH);
	AfxGetApp()->WriteProfileInt(section,temp,dlg->m_bWrapSearch);
	AddFindString(dlg->m_FindWhat);
	}


//@doc SECEditCore
//@mfunc Internal handler that saves the current Replace settings in the Registry.
//@rdesc void 
//@parm OEFindDlg* dlg
template <class BC>
void SECEditCore<BC>::SaveReplaceDlgSettings(OEFindDlg* dlg)
   {
#ifdef ORIGINAL_VERSION
	AddReplaceString(dlg->m_ReplaceWith);
#else
    // AddReplaceString parameter is passed by reference,
    // If parameter is an empty string it is changed to
    // "<OE empty string>" by AddReplaceString. Since
    // SaveReplaceDlgSettings is called prior to replacing
    // all occurences of the search string would be repla-
    // ced by "<OE empty string>" if the entered replace
    // string is empty.
    CString temp=dlg->m_ReplaceWith;
    AddReplaceString (temp);
#endif
	}

//@doc SECEditCore
//@mfunc Internal handler that initializes Find dialog settings from the Registry.
//@rdesc void 
//@parm OEFindDlg* dlg
template <class BC>
void SECEditCore<BC>::InitFindDialog(OEFindDlg* dlg)
   {
	GetWordOrSelection(dlg->m_FindWhat);
	CString section; section.LoadString(IDS_OE_SETTINGSFIND);
	CString temp;

	temp.LoadString(IDS_OE_REGULAR);
	dlg->m_bRegular = AfxGetApp()->GetProfileInt(section,temp,0);
	temp.LoadString(IDS_OE_MATCHCASE);
	dlg->m_bMatchCase = AfxGetApp()->GetProfileInt(section,temp,0);
	temp.LoadString(IDS_OE_FORWARD);
	dlg->m_iUp = AfxGetApp()->GetProfileInt(section,temp,1);
	temp.LoadString(IDS_OE_WRAPSEARCH);
	dlg->m_bWrapSearch = AfxGetApp()->GetProfileInt(section,temp,1);
	}


//@doc SECEditCore
//@mfunc Interal function that saves up to 10 find strings in the Registry.
//@rdesc void 
//@parm CString& newstring
template <class BC>
void SECEditCore<BC>::AddFindString(CString& newstring)
	{
	if (newstring.IsEmpty())
		return;

	// save up to 10 replace strings
	CString oldspot;
	CString oldstring;
	CString nextspot;
	CString nextstring;
	CString temp;
	CString section; section.LoadString(IDS_OE_SETTINGSFIND);

	// move all the old guys down one,
	// stop if we find our newstring
	temp.LoadString(IDS_OE_FINDPERCENT);
	nextspot.Format(temp,1);
	oldstring =  AfxGetApp()->GetProfileString(section,nextspot,afxEmptyString);
	if (oldstring == newstring)
		return;
	AfxGetApp()->WriteProfileString(section,nextspot,newstring);
	if (oldstring.IsEmpty())
		return;
	for (int j = 2; j < 11; j++)
		{
  		nextspot.Format(temp,j);
		nextstring =  AfxGetApp()->GetProfileString(section,nextspot,afxEmptyString);
	   AfxGetApp()->WriteProfileString(section,nextspot,oldstring);
		if (nextstring == newstring)
			break;
		oldstring = nextstring;
		if (nextstring.IsEmpty())
			break;
		}
	}


//@doc SECEditCore
//@mfunc Internal function that saves up to 10 Replace dialog strings in the Registry.
//@rdesc void 
//@parm CString& newstring
template <class BC>
void SECEditCore<BC>::AddReplaceString(CString& newstring)
	{
	if (newstring.IsEmpty())
		newstring = _T("<OE empty string>");

	// save up to 10 replace strings
	CString oldspot;
	CString oldstring;
	CString nextspot;
	CString nextstring;
	CString temp;
	CString section; section.LoadString(IDS_OE_SETTINGSFIND);

	// move all the old guys down one,
	// stop if we find our newstring
	temp.LoadString(IDS_OE_REPLACEPERCENT);
	nextspot.Format(temp,1);
	oldstring =  AfxGetApp()->GetProfileString(section,nextspot,afxEmptyString);
	if (oldstring == newstring)
		return;
	AfxGetApp()->WriteProfileString(section,nextspot,newstring);
	if (oldstring.IsEmpty())
		return;
	for (int j = 2; j < 11; j++)
		{
  		nextspot.Format(temp,j);
		nextstring =  AfxGetApp()->GetProfileString(section,nextspot,afxEmptyString);
	   AfxGetApp()->WriteProfileString(section,nextspot,oldstring);
		if (nextstring == newstring)
			break;
		oldstring = nextstring;
		if (nextstring.IsEmpty())
			break;
		}
	}


//@doc SECEditCore
//@mfunc Internal function. Each view keeps a pointer to the current line and the 
//              first visible line.  Another view could delete either 
//              of those lines.  This functions checks to make sure 
//              they both still exist, and assign new lines if not.
//@rdesc void 
//@parm PLINEDESC pLine
template <class BC>
void SECEditCore<BC>::CheckLines(PLINEDESC pLine)
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	// check for curline
	if (pLine)
		{
		pLine = pEdit->ListGetFirst(pLine, NULL);
		while (pLine)
			{
			if (pLine == m_pCurLine)
				break;
			pLine = pLine->pNext;
			}
		}
	if (!pLine)  // will be NULL if not found
		{
		int iLineNo = m_iLineNo;
		m_pCurLine = m_pTopLine = pEdit->GoToLineNo(iLineNo);
		m_iLineNo = m_iFirstLineNo = pEdit->GetLineNo(m_pCurLine);
		}
	else
		{
		// check for topline
		pLine = m_pCurLine;
		while (pLine && pLine != m_pTopLine)
			pLine = pLine->pPrev;
		if (!pLine)
			{
			m_iFirstLineNo = m_iLineNo;
			m_pTopLine = m_pCurLine;
			}
		}
}


//@doc SECEditCore
//@mfunc Pop one undo record from the list and Undo it.

template <class BC>
void SECEditCore<BC>::OnEditUndo() 
{
   if (!OnModifyContent ())
      return;

	if (m_bSelecting)
		ClearSelection(m_bUpdate);

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	PUNDORECORD pFirst = pEdit->m_pUndoBuffer;
	int iCount = pEdit->m_iUndoPos;
   pFirst += (iCount-1);
	if (!pEdit->UndoEdit())
		return;

	iCount -= pEdit->m_iUndoPos;

	PUNDORECORD pLast = pEdit->m_pUndoBuffer;
   pLast += pEdit->m_iUndoPos;

	m_pCurLine = pEdit->GoToLineNo(pLast->iRow);
	m_iLineNo = pEdit->GetLineNo(m_pCurLine);
	m_iColNo = pEdit->GetTabbedOffset(m_pCurLine,pLast->iCol);

	BOOL bRedraw = FALSE;
	for (int i = 0; i < iCount; i++)
		{
		switch (pLast->iUndoType)
			{
			case OE_UNDO_MERGE:
			case OE_UNDO_CUT:
			case OE_UNDO_PASTE:
			case OE_UNDO_COLUMNCUT:
			case OE_UNDO_COLUMNPASTE:
			case OE_UNDO_JOINLINE:
			case OE_UNDO_NEWLINE:
				bRedraw = TRUE;
				break;
			}
		pLast++;
		} 

	if (bRedraw || iCount > 1)	// #326 11.12.06 SIS
		{
		CheckLines(m_pCurLine);		  // we might have toasted m_pTopLine
		CountLines();
		InvalidateRect(NULL,FALSE);
		}
	else
		{
		CRect rect;
		rect = m_rect;
		rect.top = (int)(m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
		rect.bottom = rect.top+m_iLineHeight;
		InvalidateRect(&rect,FALSE);
		}
	UpdateWindow();
	MakeCursorVisible();
	UpdateViews(this,NULL,NULL);

#ifdef ORIGINAL_VERSION
	if (!pEdit->m_iUndoPos)
		pEdit->SetModified(FALSE);
#endif
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	pCmdUI->Enable(pEdit->m_iUndoPos);
}

//@doc SECEditCore
//@mfunc Internal handler pops one undo record from the list and Redo it.
//@rdesc void 

template <class BC>
void SECEditCore<BC>::OnEditRedo() 
{
	if (m_bSelecting)
		ClearSelection(m_bUpdate);

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	PUNDORECORD pFirst = pEdit->m_pUndoBuffer;
   pFirst += pEdit->m_iUndoPos;
	int iCount = pEdit->m_iUndoPos;
	if (!pEdit->RedoEdit())
		return;
	PUNDORECORD pLast = pEdit->m_pUndoBuffer;
   pLast += (pEdit->m_iUndoPos-1);
	iCount = pEdit->m_iUndoPos - iCount;
	m_pCurLine = pEdit->GoToLineNo(pLast->iRow);
	m_iLineNo = pEdit->GetLineNo(m_pCurLine);
	m_iColNo = pEdit->GetTabbedOffset(m_pCurLine,pLast->iCol);

	if (pLast->iUndoType == OE_UNDO_INSERT)
		m_iColNo++;

	BOOL bRedraw = FALSE;
	for (int i = 0; i < iCount; i++)
		{
		switch (pLast->iUndoType)
			{
			case OE_UNDO_MERGE:
			case OE_UNDO_CUT:
			case OE_UNDO_PASTE:
			case OE_UNDO_COLUMNCUT:
			case OE_UNDO_COLUMNPASTE:
			case OE_UNDO_JOINLINE:
			case OE_UNDO_NEWLINE:
				bRedraw = TRUE;
			}
		pLast--;
		}
	if (bRedraw || iCount > 1)	// #326 11.12.06 SIS
		{
		CheckLines(m_pCurLine);		  // we might have toasted m_pTopLine
		CountLines();
		InvalidateRect(NULL,FALSE);
		}
	else
		{
		CRect rect;
		rect = m_rect;
		rect.top = (int)(m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
		rect.bottom = rect.top+m_iLineHeight;
		InvalidateRect(&rect,FALSE);
		}
	UpdateWindow();
	MakeCursorVisible();
	UpdateViews(this, NULL, NULL);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	pCmdUI->Enable(pEdit->m_iUndoCount && 
		            (pEdit->m_iUndoPos < pEdit->m_iUndoCount));
}


//@doc SECEditCore
//@mfunc Internal handler - If there is a bookmark ahead of the current line, 
//              move to that line.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnBookmarknext() 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	if (pEdit->m_iBookmarkCount)
		{
		PLINEDESC pLine = m_pCurLine->pNext;
		while (pLine && !pLine->fBookMark)
			pLine = pLine->pNext;
		if (!pLine)
			{
			pLine = pEdit->ListGetFirst(m_pCurLine, NULL);
			while (pLine && !pLine->fBookMark)
				pLine = pLine->pNext;
			}
		if (pLine)
			{
			m_iColNo = 0;
			CenterLine(pLine);
			}
		}
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateBookmarknext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetEdit()->m_iBookmarkCount > 0);
}


//@doc SECEditCore
//@mfunc Internal handler that toggles the current line's status as a bookmark.


template <class BC>
void SECEditCore<BC>::OnBookmarkset() 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	if (!m_pCurLine->fBookMark)
		pEdit->m_iBookmarkCount++;
	else
		pEdit->m_iBookmarkCount--;
	m_pCurLine->fBookMark = ~m_pCurLine->fBookMark;
	CRect rect;
	rect = m_rect;
	rect.top = (int)(m_iLineNo - m_iFirstLineNo)*m_iLineHeight;
	rect.bottom = rect.top+m_iLineHeight;
	InvalidateRect(&rect,FALSE);
	UpdateViews(this,NULL,NULL);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI

template <class BC>
void SECEditCore<BC>::OnUpdateBookmarkset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


//@doc SECEditCore
//@mfunc Internal handler that moves to the previous bookmark.

template <class BC>
void SECEditCore<BC>::OnBookmarkprev() 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	if (pEdit->m_iBookmarkCount)
		{
		PLINEDESC pLine = m_pCurLine->pPrev;
		while (pLine && !pLine->fBookMark)
			pLine = pLine->pPrev;
		if (!pLine)
			{
			pLine = pEdit->ListGetLast(m_pCurLine);
			while (pLine && !pLine->fBookMark)
				pLine = pLine->pPrev;
			}
		if (pLine)
			{
			m_iColNo = 0;
			CenterLine(pLine);
			}
		}
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateBookmarkprev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetEdit()->m_iBookmarkCount > 0);
}

//@doc SECEditCore
//@mfunc Internal method that makes pLine the current line, and centers it in the
//              window.
//@rdesc void 
//@parm PLINEDESC pLine

template <class BC>
void SECEditCore<BC>::CenterLine(PLINEDESC pLine)
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	m_pCurLine = pLine;
	m_iLineNo = pEdit->GetLineNo(m_pCurLine);
	// if this line isn't on screen already, try to center it
	int iNumLines = m_rect.bottom / m_iLineHeight;
	int iTargetLine = iNumLines / 2;

	if ((m_iFirstLineNo > m_iLineNo) || ((m_iLineNo-m_iFirstLineNo) >= iNumLines))
		{
		m_pTopLine = m_pCurLine;
		m_iFirstLineNo = m_iLineNo;
		while (m_pTopLine->pPrev && (m_iLineNo-m_iFirstLineNo) < iTargetLine)
			{
			m_pTopLine = m_pTopLine->pPrev;
			m_iFirstLineNo--;
			}
		Invalidate();
		}
	MakeCursorVisible();

}

//@doc SECEditCore
//@mfunc Internal handler that removes the bookmark status from all lines in this document.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnBookmarkdelete() 
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	PLINEDESC pLine = pEdit->ListGetFirst(m_pCurLine, NULL);
	while (pLine)
		{
		pLine->fBookMark = 0;
		pLine = pLine->pNext;
		}
	pEdit->m_iBookmarkCount = 0;
	Invalidate();
	UpdateViews(this,NULL,NULL);
}

//@doc SECEditCore
//@mfunc Internal handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdateBookmarkdelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetEdit()->m_iBookmarkCount > 0);
}


//@doc SECEditCore
//@mfunc Counts the number of lines defined as bookmarks
//@rdesc int The count of bookmarks.
template <class BC>
int SECEditCore<BC>::CountBookmarks()
{
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	int i = 0;
	PLINEDESC pLine = pEdit->ListGetFirst(m_pCurLine, NULL);
	while (pLine)
		{
		if (pLine->fBookMark)
			i++;
		pLine = pLine->pNext;
		}
	return i;
}


//@doc SECEditCore
//@mfunc Internal handler that invokes the page setup dialog.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnPageSetup()
{
	// get the page setup values from the registry
	dlgPageSetup.Initialize();
	if (dlgPageSetup.DoModal() == IDOK)
		// write the new values back out to the registry
		dlgPageSetup.Terminate();
}

//@doc SECEditCore
//@mfunc Internal message handler.
//@rdesc void 
//@parm CCmdUI* pCmdUI
template <class BC>
void SECEditCore<BC>::OnUpdatePageSetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

//@doc SECEditCore
//@mfunc Internal OnSetCursor handler.
//@rdesc BOOL 
//@parm CWnd* pWnd
//@parm  UINT nHitTest
//@parm  UINT message
template <class BC>
BOOL SECEditCore<BC>::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(nHitTest & HTCLIENT)
	{
		CPoint pt;
		if(GetCursorPos(&pt))
		{
			ScreenToClient(&pt);
			if (pt.x <= (m_iMarkSpace - (m_iCharWidth / 2)))
			{
				// Make sure that we find the cursor
				HCURSOR hCursor = ::LoadCursor(AfxFindResourceHandle(MAKEINTRESOURCE(IDC_SECEDIT_GUTTER),
												   RT_GROUP_CURSOR),
												   MAKEINTRESOURCE(IDC_SECEDIT_GUTTER));
				ASSERT(hCursor != NULL);

				::SetCursor(hCursor);

				return 0;
			}
			else if(IsPointInBlock(pt))
			{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				return 0;
			}
		}
	}
	return BC::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////
// CEdit/CRichEdit compatibilty

//@doc SECEditCore
//@mfunc Returns the length of the text (CEdit/CRichEdit compatible)
//@rdesc long  The length of the text in the document.

template <class BC>
long SECEditCore<BC>::GetTextLength()
{
	if (!m_pTopLine)
		return 0;

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	int iTextLength = 0;
	PLINEDESC pLine = pEdit->ListGetFirst(m_pTopLine,NULL);
	while (pLine)
		{
		iTextLength += pLine->iTxtSize;
		pLine = pLine->pNext;
		}
	return (long)iTextLength;
}

//@doc SECEditCore
//@mfunc Finds text in the document - note that the bWord argument is currently ignored.  This function provides CEdit/CRichEdit compatibility.
//@rdesc BOOL Was the text found?
//@parm LPCTSTR lpszFind The string to find.
//@parm  BOOL bCase Case sensitive?
//@parm  BOOL bWord Currently ignored.

template <class BC>
BOOL SECEditCore<BC>::FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord)
{
	bWord;  // we don't currently support whole word searches

	// set up the registry entries that OnEditFindnext() uses
	CString section;	section.LoadString(IDS_OE_SETTINGSFIND);
	CString temp;		temp.LoadString(IDS_OE_FINDPERCENT);
	CString entry;		entry.Format(temp,1);

	AfxGetApp()->WriteProfileString(section,entry,lpszFind);

	temp.LoadString(IDS_OE_MATCHCASE);
	AfxGetApp()->WriteProfileInt(section,temp,bCase);

	OnEditFindnext();
	return m_bFoundNext;
}

//@doc SECEditCore
//@mfunc Loads the text of the document into a buffer.  This function is provided for CEdit/CRichEdit compatibility.
//@rdesc int Number of characters returned in the lpszStringBuf buffer.
//@parm  LPTSTR lpszStringBuf Buffer to copy the document contents into.
//@parm  int nMaxCount The maximum number of characters to copy into the lpszStringBuf argument string buffer.

template <class BC>
int SECEditCore<BC>::GetWindowText( LPTSTR lpszStringBuf, int nMaxCount )
{
	if (!m_pTopLine)
		{
		*lpszStringBuf = 0;
		return 0;
		}

	long iTextLength = GetTextLength();
	iTextLength = min(iTextLength,nMaxCount) * sizeof(TCHAR); // Dealing with a possible UNICODE buffer
	int iRval = iTextLength;

	TCHAR* pBuffer = lpszStringBuf;

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	PLINEDESC pLine = pEdit->ListGetFirst(m_pTopLine,NULL);
	int iCopyLength;
	while (iTextLength)
		{
		iCopyLength = min(pLine->iTxtSize,iTextLength);
		memcpy(pBuffer,pLine->pText,iCopyLength);
		pBuffer += iCopyLength;
		iTextLength -= iCopyLength;
		pLine = pLine->pNext;
		}
	*pBuffer = 0;

	return iRval;
}
  
//@doc SECEditCore
//@mfunc Returns the contents of the buffer in the CString object passed in as an argument.  This function is provided for CEdit/CRichEdit compatibility.
//@rdesc void 
//@parm  CString& rString The CString object to place the contents of the buffer into.

template <class BC>
void SECEditCore<BC>::GetWindowText( CString& rString )
{
	USES_CONVERSION_T;

	if (!m_pTopLine)
		{
		rString.Empty();
		}

	int iTextLength = 0;

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);
	PLINEDESC pLine = pEdit->ListGetFirst(m_pTopLine,NULL);
	while (pLine)
		{
		rString += A2T(pLine->pText);
		iTextLength += pLine->iTxtSize;
		pLine = pLine->pNext;
		}

}
  
//@doc SECEditCore
//@mfunc Replaces the document with the text provided.  This function is provided for CEdit/CRichEdit compatibility.
//@rdesc void 
//@parm  LPCTSTR lpszString The text to put into the buffer.

template <class BC>
void SECEditCore<BC>::SetWindowText( LPCTSTR lpszString )
{
	// need to select all the text first
	SelectAll(FALSE);

	// override the readonly status
	BOOL bSaveReadOnly = GetEdit()->GetReadOnly();
	GetEdit()->SetReadOnly(FALSE);

	ReplaceSelection(lpszString);

	// restore the readonly status
	GetEdit()->SetReadOnly(bSaveReadOnly);

	// Update the scroll bars and line count
	CountLines();
}


//@doc SECEditCore
//@mfunc Gets the currently selected range.   This function is provided for CEdit/CRichEdit compatibility.
//@rdesc BOOL Returns TRUE on success, FALSE on failure (e.g. if there is no selection.
//@parm  TEXTPOS& Start The start of the selection - returned through this argument.
//@parm  TEXTPOS& End The end of the selection - returned through this argument.
//@parm  BOOL* pbColumnar TRUE if we're dealing with columnar selection, FALSE otherwise.

template <class BC>
BOOL  SECEditCore<BC>::GetSelection( TEXTPOS& Start, TEXTPOS& End, BOOL* pbColumnar /* = NULL */ )
{
	SECEdit* pEdit = GetEdit();
	ASSERT( pEdit != NULL );

	if ( pbColumnar )
		*pbColumnar = FALSE;

	if ( m_BlockStart.pLine && m_BlockEnd.pLine )
	{
		// BlockStart and BlockEnd positions are tabbed,
		// WndCopy gets the real offset
		Start = m_BlockStart;
		End	  = m_BlockEnd;
		Start.iOffset = pEdit->GetRealOffset( Start.pLine, Start.iOffset );
		End.iOffset   = pEdit->GetRealOffset( End.pLine,   End.iOffset );

		if ( m_iBlockType == OE_COLUMN )
		{
			if ( pbColumnar )
				*pbColumnar = TRUE;
		}

		return TRUE;
	}
	else
	{
	Start.pLine = m_pCurLine;
	Start.iOffset = pEdit->GetRealOffset( m_pCurLine, m_iColNo );
	End = Start;
	return FALSE;	
	}
}


//@doc SECEditCore
//@mfunc Returns a pointer to the current selection text. This function is provided for CEdit/CRichEdit compatibility.
//@rdesc LPTSTR A pointer to the selected text. 
//@parm  BOOL* pbColumnar Is this columnar selection?

template <class BC>
LPSTR  SECEditCore<BC>::GetSelection( BOOL* pbColumnar /* = NULL */ )
{
	LPSTR  lpText = NULL;

	SECEdit* pEdit = GetEdit();
	ASSERT( pEdit != NULL );

	if ( pbColumnar )
		*pbColumnar = FALSE;

	if ( m_BlockStart.pLine && m_BlockEnd.pLine )
	{
		// BlockStart and BlockEnd positions are tabbed,
		// WndCopy gets the real offset
		TEXTPOS Start = m_BlockStart;
		TEXTPOS End	  = m_BlockEnd;
		Start.iOffset = pEdit->GetRealOffset( Start.pLine, Start.iOffset );
		End.iOffset   = pEdit->GetRealOffset( End.pLine,   End.iOffset );
		if ( m_iBlockType == OE_COLUMN )
		{
			if ( pbColumnar )
				*pbColumnar = TRUE;

			lpText = pEdit->ColumnCopy( &Start, &End, TRUE );
		}
		else
			lpText = pEdit->Copy( &Start, &End, TRUE );
	}

	return lpText;
}

//@doc SECEditCore
//@mfunc Sets the current selection.   This function is provided for CEdit/CRichEdit compatibility.
//@rdesc void
//@parm  PTEXTPOS Start The start of the selection.
//@parm  PTEXTPOS End The end of the selection.
//@parm  BOOL bColumnar TRUE to make a columnar selection, FALSE otherwise.

template <class BC>
void SECEditCore<BC>::SetSelection( PTEXTPOS Start, PTEXTPOS End, BOOL bColumnar /* = FALSE */ )
{

	SECEdit* pEdit = GetEdit();
	ASSERT( pEdit != NULL );

	ClearSelection(FALSE);

	GotoLineCol(pEdit->GetLineNo(Start->pLine),pEdit->GetTabbedOffset(Start->pLine,Start->iOffset));
	StartSelection(bColumnar ? OE_COLUMN : OE_STREAM, FALSE);
	GotoLineCol(pEdit->GetLineNo(End->pLine),pEdit->GetTabbedOffset(End->pLine,End->iOffset));
	UpdateSelection(FALSE);
	UpdateEditor();
}


//@doc SECEditCore
//@mfunc Replaces the currently selected text with the lpNewSel argument. This function is provided for CEdit/CRichEdit compatibility.
//@rdesc BOOL  TRUE if the call succeeds, FALSE if it doesn't.
//@parm  LPCTSTR lpNewSel  Pointer to the text to replace the current selection with.
//@parm  BOOL bColumnar TRUE if the selection is a column, FALSE if not.
//@parm  BOOL bSaveUndoRecord Save this transaction in the undo buffer?
template <class BC>
BOOL  SECEditCore<BC>::ReplaceSelection( LPCTSTR lpNewSel, BOOL bColumnar, BOOL bSaveUndoRecord)
{
	USES_CONVERSION_T;

	SECEdit* pEdit = GetEdit();
	ASSERT( pEdit != NULL );

	// if there's a block marked, cut it
	Cut( FALSE, bSaveUndoRecord );

	TEXTPOS TextPos;
	BOOL	bRet = TRUE;
	char	search[4] = { 0x0D, 0x0A, 0, 0 };

	// is there more than one line?
	LPSTR lpTemp = strpbrk( T2CA(lpNewSel), search );
	if ( !lpTemp )
		bRet = ( InsertString( (LPTSTR) lpNewSel, FALSE, bSaveUndoRecord ) == 0 );
	else
	{
		TextPos.pLine   = m_pCurLine;
		TextPos.iOffset = pEdit->GetRealOffset( m_pCurLine, m_iColNo );
		if ( bColumnar )
			bRet = ( pEdit->ColumnPaste( &TextPos, (LPSTR)T2CA(lpNewSel), _tcslen(lpNewSel), bSaveUndoRecord ) == 0 );
		else
			bRet = ( pEdit->Paste( &TextPos, (LPSTR)T2CA(lpNewSel), _tcslen( lpNewSel ), bSaveUndoRecord ) == 0 );
	}

	UpdateEditor();

	return bRet;
}

//@doc SECEditCore
//@mfunc Inserts a file into the buffer. This function is provided for CEdit/CRichEdit compatibility.
//@rdesc void 
//@parm  LPCTSTR FileName The filename of the file to insert.

template <class BC>
void SECEditCore<BC>::InsertFile( LPCTSTR FileName )
{
	USES_CONVERSION_T;

	// open it
	CFile f;
	try
	{
		f.Open( FileName, CFile::modeRead );
	}
	catch ( CFileException* e )
	{
	  #ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
	  #endif
		e->Delete();
		return;
	}

	int nFileSize = f.GetLength();
	char* pBuff = new char[ nFileSize + 1 ];
	ASSERT( pBuff );
	f.Read( pBuff, nFileSize );

	pBuff[ nFileSize ] = 0;

	ReplaceSelection( A2T(pBuff) );

	delete [] pBuff;
}

//@doc SECEditCore
//@mfunc Internal function that updates all Objective Edit windows.
//@rdesc void  

template <class BC>
void  SECEditCore<BC>::UpdateEditor ()
{
	// update the editor
	InvalidateRect( NULL, FALSE );
	UpdateWindow();
	MakeCursorVisible();
	UpdateViews(this, NULL, NULL);

}



//@doc SECEditCore
//@mfunc Goes to the line specified in the Line argument. This function is provided for CEdit/CRichEdit compatibility.
//@rdesc void 
//@parm  int Line The line to scroll to and display.
template <class BC>
void SECEditCore<BC>::GotoLine( int Line )
{
	GotoLineCol( Line, 0 );
}


//@doc SECEditCore
//@mfunc Move the caret to a specific line and column number. This function is provided for CEdit/CRichEdit compatibility.
//@rdesc void 
//@parm  int Line Line number.
//@parm  int Col  Column number.

template <class BC>
void SECEditCore<BC>::GotoLineCol( int Line, int Col )
{
	// Navigate to new line number
	BOOL Scrolled = FALSE;

	m_iLineNo = Line;

	if ( !IsLineVisible( Line ) )
	{
		// New line not visible; must scroll the view
		if ( Line < m_iFirstLineNo )
		{
			// scrolling up
			ScrollGoToLine ( m_iLineNo, FALSE );
		}
		else
		{
			// scrolling down (position new line as the last visible)
			ScrollGoToLine ( m_iLineNo - LinesPerPage() + 1, FALSE );
		}

		Scrolled = TRUE;
	}

	m_pCurLine = GetEdit()->GoToLineNo( m_iLineNo );

	m_iColNo = Col;
	MakeCursorVisible();

	if ( Scrolled )
	{
		Invalidate();
		UpdateWindow();
	}
}

//@doc SECEditCore
//@mfunc Internal function.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditUppercase() 
{
	BOOL Columnar;
	LPSTR pBuff = GetSelection( &Columnar );

	if ( pBuff )
	{
		CString Buff( pBuff );
		Buff.MakeUpper();

		ReplaceSelection( Buff, Columnar );
	}

	delete pBuff;
}


//@doc SECEditCore
//@mfunc Internal function.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::OnEditLowercase() 
{
	BOOL Columnar;
	LPSTR pBuff = GetSelection( &Columnar );

	if ( pBuff )
	{
		CString Buff( pBuff );
		Buff.MakeLower();

		ReplaceSelection( Buff, Columnar );
	}

	delete pBuff;
}

//@doc SECEditCore
//@mfunc Internal function.
//@rdesc void 
template <class BC>
void SECEditCore<BC>::UpdateViews(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	pSender;  // unused
	SECEditHint* hint;
	if ((lHint == IDS_OE_DELETELINE) && pHint->IsKindOf(RUNTIME_CLASS(SECEditHint)))
		{
		hint = (SECEditHint*)pHint;
		CheckLines(hint->m_pLine);
		}

	if (m_bUpdate)
		InvalidateRect(NULL,FALSE);
}

#ifndef ORIGINAL_VERSION
template<class BC> bool SECEditCore<BC>::OnModifyContent ()
{
    return (true);
}
#endif


#endif //__SECEDITCORE_INL__