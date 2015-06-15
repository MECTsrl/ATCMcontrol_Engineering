


// msgView.cpp : implementation of the CCompileView class
//

#include "stdafx.h"

#include "MsgView.h"
#include "msgrc.h" 
#include "MsgFrame.h"
#include "msgdoc.h"
#include "MsgRegex.h"
#include "MsgWnd.h"
#include "fc_todebug\fc_mem.h"
#include "fc_tools\fc_string.h"
#include "cemain.h"
#include "MainFrm.h"
#include "CEFileHandler.h"
#include "GenEdCont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Helper functions
/*CMainFrame * GetMainFrame(void)
{
    CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
    ASSERT(pMainFrm);
    return pMainFrm;
}*/
//line must begin (ignore case) with cd<whitespace>:
//assumes that pszLine is trimmed left & right
//return ""   if special case 'cd $(prjdir)'
//return NULL if does not look like a cd info line
static const TCHAR* IsLineCDInfo(const TCHAR* pszLine)
{
    if((pszLine[0]=='c' || pszLine[0]=='C') && (pszLine[1]=='d' || pszLine[1]=='D'))
    {
        pszLine += 2;
        if(*pszLine==' ' || *pszLine=='\t')
        {
            pszLine++;
            while(*pszLine==' ' || *pszLine=='\t')
                pszLine++;

            if(*pszLine)
            {
                //cd $(prjdir) returns always back to prj default dir
                if(!_tcscmp(pszLine, _T("$(prjdir)")))
                    return _T("");
                else
                    return pszLine;
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------*
/**
 * Gets change directory info from previous lines, feature only 
 * for cmdl and compiler tab, all other tabs return sFile unchanged.
 *
 * @param           iLine: zero based line index from where to 
 *                  search cd [path] commands backward until
 *                  first line or cd <fullpath> or special cd $(prjdir) 
 *                  is found
 * @param           editCtrl: holds the text data
 * @param           sFile: in/out. gets updated to point to final path
 * @param  (HACK)   pMsgFrame: CMsgFrame, used to check if cmdl, compiler tab
 * @param  (HACK)   pDoc:      CMsgDoc, used to check if cmdl, compiler tab
 *                  may still be a (project) relative path on exit.
 * @see             -
*/
static void GetCDInfo(int iLine, CEdit& editCtrl, CString& sFile, CMsgFrame* pMsgFrame, CMsgDoc* pDoc)
{
    int          nLen;
    const TCHAR* pszDir;
    TCHAR        buffer[2*MAX_PATH];


    if(FC_StringIsAbsPath(sFile))
        return;
    if(iLine >= editCtrl.GetLineCount())
        return;

    //feature only for compiler tab (HACK):
    ASSERT (pMsgFrame != NULL);
    if (!pMsgFrame)
        return;
    int iIndex=-1;
    pMsgFrame->m_tabWnd.GetActiveTab(iIndex);
    if(!pDoc  || iIndex<0 || iIndex >= pDoc->m_arrMsgData.GetSize())
    {
        ASSERT (!"bad pDoc, iIndex");
        return;
    }
    MSG_VIEWER_TYPE msgTyp = pDoc->m_arrMsgData[iIndex].m_MsgViewerType;
    if(msgTyp != MSG_CmdLineMsgViewer && msgTyp != MSG_CompileMsgViewer)
        return;


    while(iLine>=0)
    {
        nLen = editCtrl.GetLine(iLine, buffer, FC_ARRAY_LEN(buffer)-10);
        iLine--;
        if(nLen<=0)
            continue;
        buffer[nLen] = 0;//<-needed ?
        FC_StringTrim(buffer);
        if((pszDir = IsLineCDInfo(buffer)) != NULL)
        {
            if(!*pszDir)
                break;//cd $(prjdir): don't prepend default prj dir as an abs path to sFile

            //all other rel or abs cd commands must be prepended:
            sFile.Insert(0, '\\');
            sFile.Insert(0, pszDir);
            //if is abs must stop here:
            if(FC_StringIsAbsPath(pszDir))
                break; 
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
// CMsgView

IMPLEMENT_DYNCREATE(CMsgView, CEditView)

BEGIN_MESSAGE_MAP(CMsgView, CEditView)
	//{{AFX_MSG_MAP(CMsgView)
	ON_WM_CREATE()
    ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDBLCLK()
    ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_WM_SIZE()
    ON_WM_SETFOCUS()
	ON_COMMAND(IDM_MSG_CONTEXT_MENU, OnContextMenuCommand)
	ON_COMMAND(IDM_MSG_COPY, OnEditCopy)
	ON_COMMAND(IDM_MSG_CLEAR, OnEditClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgView construction/destruction

CMsgView::CMsgView()
{
    m_phbr = new CBrush(0x00ffffff);
    ASSERT(m_phbr);
    m_pRegex = NULL;
    m_pRegex = new CMsgRegex;
    ASSERT(m_pRegex);
}

CMsgView::~CMsgView()
{
    delete m_phbr;
    if (m_pRegex!=NULL)
    {
        delete m_pRegex;
    }
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgView::CtlColor
 * Description			
 * @param			CDC* pDC
 * @param			UINT nCtlColor
 * @return			HBRUSH 
 * @exception			
 * @see			
*/
HBRUSH CMsgView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor(0x00ffffff);
	return (HBRUSH) m_phbr->GetSafeHandle();
}

/////////////////////////////////////////////////////////////////////////////
// CMsgView diagnostics

#ifdef _DEBUG
void CMsgView::AssertValid() const
{
    CView::AssertValid();
}

void CMsgView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}


#endif //_DEBUG

void CMsgView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	/*ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this.

	if (pInfo->m_nCurPage > (UINT)m_aPageStart.GetSize() &&
		!PaginateTo(pDC, pInfo))
	{
		// can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}
	ASSERT_VALID(this);*/
}

/////////////////////////////////////////////////////////////////////////////
// CMsgView message handlers



//------------------------------------------------------------------*
/**
 * Function name	CMsgView::OnInitialUpdate
 * Description			
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgView::PreCreateWindow
 * Description			
 * @param			CREATESTRUCT& cs
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_HSCROLL | WS_VSCROLL |
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_WANTRETURN;

    return CEditView::PreCreateWindow(cs);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgView::OnActivateView
 * Description			
 * @param			BOOL bActivate
 * @param			CView* pActivateView
 * @param			CView* pDeactiveView
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
    #if 0
	SECTabWnd* pTabWnd = (SECTabWnd*)GetParent();
	if (pTabWnd != NULL)
		pTabWnd->ShowScrollBar(SB_BOTH, !bActivate);
    #endif
    
    if (bActivate)
    {
        UpdateEditText(true);
    }
	
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgView::OnCreate
 * Description			
 * @param			LPCREATESTRUCT lpCreateStruct
 * @return			int 
 * @exception			
 * @see			
*/
int CMsgView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate (lpCreateStruct) == -1)
    {
        return (-1);
    };

    CFont *pFont;
    // Set fixed font
    pFont = CFont::FromHandle((HFONT)GetStockObject (ANSI_FIXED_FONT));
    GetEditCtrl().SetFont(pFont);
    GetEditCtrl().SetReadOnly();
    // set text limit to maximum of Windows 95/98
    GetEditCtrl().SetLimitText(MSGWND_MAXTEXTLEN);
    UpdateEditText ();
   
    return (0);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgView::OnUpdate
 * Description			
 * @param			CView *pSender
 * @param			LPARAM lHint
 * @param			CObject *pHint
 * @return			void 
 * @exception			
 * @see			
*/

void CMsgView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
    if (lHint == CMsgDoc::TextModifiedHint)
    {
        CMsgFrame *pMessageFrame;
        
        pMessageFrame = dynamic_cast<CMsgFrame *> (GetParentFrame ());
        ASSERT (pMessageFrame != NULL);
        if (pMessageFrame == NULL)
            return;
        
        int uiIndex;
        if (pMessageFrame->m_tabWnd.GetActiveTab(uiIndex) && (int)pHint == uiIndex)
        {
            UpdateEditText ();
        };

        return;
    };

    CView::OnUpdate (pSender, lHint, pHint);
    
}


//------------------------------------------------------------------*
/**
 * Updates text in the edit field from the message data associated 
 * with this Tab respectively view.
 * @param       bFocus - true: set focus on edit control, false
 *              do not set focus (default is false).
 * @see			OnActivateView()
*/
void CMsgView::UpdateEditText(bool bFocus)
{
    CMsgDoc *pDoc;
    CMsgFrame *pMessageFrame;
    int uiIndex;

    pDoc = GetDocument ();
    ASSERT (pDoc != NULL);
    if (pDoc == NULL)
        return;

    pMessageFrame = dynamic_cast<CMsgFrame *> (GetParentFrame ());
    ASSERT (pMessageFrame != NULL);
    if (pMessageFrame == NULL)
        return;
    
    pMessageFrame->m_tabWnd.GetActiveTab (uiIndex);
    CMsgData & MsgData = pDoc->m_arrMsgData[uiIndex];

    if(bFocus) {
        GetEditCtrl().SetFocus();
    }
    
    // Save caret position and scroll position
    MsgData.m_dwEditPos = GetEditCtrl().GetSel();
    MsgData.m_bEndPos = (GetEditCtrl().LineFromChar(-1) == GetEditCtrl().GetLineCount() - 1);
    int nFirstVisible = GetEditCtrl().GetFirstVisibleLine();
    
    GetEditCtrl().SetWindowText(pDoc->m_arrMsgData[uiIndex].m_strText);

    if (MsgData.m_bEndPos)
    {
        // set position to end of text;
        int iLen = MsgData.m_strText.GetLength();
        int iLine = GetEditCtrl().LineFromChar(iLen);
        iLen = GetEditCtrl().LineIndex(iLine);
        GetEditCtrl().SetSel(iLen, iLen);
    }
    else
    {
        // Scroll the edit control so that the first visible line
        // is the first line of text.
        int nowFirstVisible = GetEditCtrl().GetFirstVisibleLine();
        
        GetEditCtrl().LineScroll(nFirstVisible-nowFirstVisible, 0);
        GetEditCtrl().SetSel(MsgData.m_dwEditPos);
    }

}

#ifdef _DEBUG 
CMsgDoc* CMsgView::GetDocument() // non-debug version is inline
{
    if (m_pDocument)
    {
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMsgDoc)));
    }

    return (CMsgDoc *)m_pDocument;
}
#endif


//------------------------------------------------------------------*
/**
 * Function name	CMsgView::AppendText
 * Description			
 * @param			LPCTSTR szMsgText
 * @return			int 
 * @exception			
 * @see			
*/
int CMsgView::AppendText(LPCTSTR szMsgText)
{
    CString str(_T(""));
    CString strAppendText;
    int Length;
    int nStart;
    int nEnd;
    BOOL bScroll;
    int iLineCount;
    int iCurrentLine;
    int iMaxTextLen;
    CEdit& rtEdit = GetEditCtrl();
    iLineCount = rtEdit.GetLineCount();
    iCurrentLine = rtEdit.LineFromChar(-1);
    iMaxTextLen = rtEdit.GetLimitText();

    rtEdit.GetWindowText(str);
    Length = str.GetLength();

    bScroll = (iCurrentLine == iLineCount - 1);
    if(iLineCount > MSGWND_MAXLINES || 
       Length + lstrlen(szMsgText) + 2 > iMaxTextLen) 
    {
        rtEdit.SetRedraw(FALSE);
        rtEdit.GetSel(nStart, nEnd);
        int nNewFirst = rtEdit.LineIndex(MSGWND_DELLINES);
        rtEdit.GetWindowText(str);
        str = str.Mid(nNewFirst);
        rtEdit.SetWindowText(str);
        nStart = max(0, nStart - nNewFirst);
        nEnd = max(0, nEnd - nNewFirst);
        rtEdit.SetRedraw(TRUE);
        rtEdit.SetSel(nStart, nEnd);
    }

    rtEdit.GetWindowText(str);
    Length = str.GetLength();
    if (Length == 0)
    {
        strAppendText = szMsgText;
    }
    else
    {
        strAppendText.Format(_T("\r\n%s"), szMsgText);
    }

    if (!bScroll)
    {
        rtEdit.SetRedraw(FALSE);
        rtEdit.GetSel(nStart, nEnd);
        Length = str.GetLength();
        rtEdit.SetSel(Length, Length);
        rtEdit.ReplaceSel(strAppendText);
        rtEdit.SetSel(nStart, nEnd);
        rtEdit.SetRedraw(TRUE);
    }
    else
    {
        rtEdit.SetRedraw(FALSE);
        Length = str.GetLength();
        rtEdit.SetSel(Length, Length);
        rtEdit.ReplaceSel(strAppendText);
        int iLine = rtEdit.LineFromChar(Length + strAppendText.GetLength());
        Length = rtEdit.LineIndex(iLine);
        rtEdit.SetSel(Length, Length);
        rtEdit.SetRedraw(TRUE);
        rtEdit.LineScroll(1, 0);
    }

    return 0;
} 



//------------------------------------------------------------------*
/**
 * double click handler
 *
 * @param           see MSDN
 * @return          -
 * @exception       -
 * @see             -
*/
void CMsgView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    ShowCurrentError();
}


//------------------------------------------------------------------*
/**
 * show current error.
 *
 *  selects current line and shows error location in source if possible.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             GetLine(), SelectLine(), EvaluateErrorMessage(), ShowSourceLocation()
*/
void CMsgView::ShowCurrentError()
{
    int     iLine;          // line number of current line
    CString strLine;        // current line as string
    CString strSourceFile;  // source file name without path
    CString strLocation;    // location in source file

    CEdit& editCtrl = GetEditCtrl();

    // get current line
    iLine = editCtrl.LineFromChar();
    if(!SelectLine(iLine))
    {
        return;
    }

    GetLine(iLine, strLine);

    if(EvaluateErrorMessage(strLine, strSourceFile, strLocation))
    {
        GetCDInfo(iLine-1, editCtrl, strSourceFile, dynamic_cast<CMsgFrame*>(GetParentFrame()), GetDocument());
        ShowSourceLocation(strSourceFile, strLocation);
    }
    else
    {
        MessageBeep(-1);
    }
}


//------------------------------------------------------------------*
/**
 * show next error.
 *
 *  selects the next error message and displays the source location.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             GetLine(), SelectLine(), EvaluateErrorMessage(), ShowSourceLocation()
*/
void CMsgView::ShowNextError()
{
    int     iLine;
    BOOL    bFound = FALSE;
    CString strLine;
    CString strSourceFile;
    CString strLocation;
    CEdit&  editCtrl = GetEditCtrl();

    iLine = editCtrl.LineFromChar();
    ++iLine;
    if(iLine >= editCtrl.GetLineCount())
    {
        iLine = 0;
    }

    while(GetLine(iLine, strLine))
    {
        if(EvaluateErrorMessage(strLine, strSourceFile, strLocation))
        {
            SelectLine(iLine);
            GetCDInfo(iLine-1, editCtrl, strSourceFile, dynamic_cast<CMsgFrame*>(GetParentFrame()), GetDocument());
            ShowSourceLocation(strSourceFile, strLocation);
            bFound = TRUE;
            break;
        }
        ++iLine;
    }
    if(!bFound)
    {
        MessageBeep(-1);
    }
}

//------------------------------------------------------------------*
/**
 * show previous error.
 *
 *  selects the previous error message and displays the source location.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             GetLine(), SelectLine(), EvaluateErrorMessage(), ShowSourceLocation()
*/
void CMsgView::ShowPreviousError()
{
    int     iLine;
    BOOL    bFound = FALSE;
    CString strLine;
    CString strSourceFile;
    CString strLocation;
    CEdit&  editCtrl = GetEditCtrl();

    iLine = editCtrl.LineFromChar();
    --iLine;
    if(iLine < 0)
    {
        iLine = editCtrl.GetLineCount() - 1;
    }

    while(GetLine(iLine, strLine))
    {
        if(EvaluateErrorMessage(strLine, strSourceFile, strLocation))
        {
            SelectLine(iLine);
            GetCDInfo(iLine-1, editCtrl, strSourceFile, dynamic_cast<CMsgFrame*>(GetParentFrame()), GetDocument());
            ShowSourceLocation(strSourceFile, strLocation);
            bFound = TRUE;
            break;
        }
        --iLine;
    }
    if(!bFound)
    {
        MessageBeep(-1);
    }
}

//------------------------------------------------------------------*
/**
 * select line.
 *
 * @param           iLine: zero based line index
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMsgView::SelectLine(int iLine)
{
    int iStart;
    int iLength;
    CString strLine;

    CEdit& editCtrl = GetEditCtrl();

    iStart = editCtrl.LineIndex(iLine);
    if(iStart == -1)
    {
        return FALSE;
    }

    iLength = editCtrl.LineLength(iStart);
    editCtrl.SetSel(iStart, iStart+iLength);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get line from line index
 *
 * @param           iLine: zero based line index
 * @param           rstrLine: line as string.
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMsgView::GetLine(int iLine, CString& rstrLine)
{
    CEdit& editCtrl = GetEditCtrl();
    if(iLine < 0 || iLine >= editCtrl.GetLineCount())
    {
        return FALSE;
    }
    int iLineIndex = editCtrl.LineIndex(iLine);
    int iLength = editCtrl.LineLength(iLineIndex);
    editCtrl.GetLine(iLine, rstrLine.GetBuffer(iLength), iLength);
    rstrLine.ReleaseBuffer();
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * evaluate error message.
 *
 * @param           crstrMessage: message text
 * @param           rstrSourceFile: source file name without extension
 * @param           rstrLocation: location within source file
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMsgView::EvaluateErrorMessage(const CString& crstrMessage, CString& rstrSourceFile, CString& rstrLocation)
{
    CString strFileName;
    int iMsgType;

    if (!m_pRegex->EvaluateMessage(crstrMessage))
    {
        iMsgType = m_pRegex->GetMessageType();
        m_pRegex->GetSourcePos(rstrLocation);
        if(rstrLocation.IsEmpty() || iMsgType == CEMSG_TYPE_STANDARD || iMsgType == CEMSG_TYPE_DONTKNOW)
        {
            return FALSE;
        }
        m_pRegex->GetSourceName(rstrSourceFile);
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * show source location for (error) message
 *
 * @param           crstrSourceFile: source file name
 * @param           crstrLocation: location within source file
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMsgView::ShowSourceLocation(const CString& crstrSourceFile, const CString& crstrLocation)
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp*>(AfxGetApp());
    ASSERT(pApp);
    if (pApp == NULL)
    {
        return FALSE;
    }

    CCEFileHandler *pFileHandler;
    CGenEdDoc *pDoc;

    pFileHandler = pApp->GetFileHandler();
    if (pFileHandler == NULL)
    {
        return FALSE;
    }

    HRESULT hr;
    hr = pFileHandler->ShowFile(crstrSourceFile, pDoc);
    if (FAILED (hr) || (pDoc==NULL))
    {
        // Null can be returned now 
        // ==> the file could not be opend in 4C, 
        // if possible an extern editor has been opened
        return FALSE;
    }
    
    CGenEdCont *pContainer;
    pContainer = pApp->GetContainer();
    if (pContainer != NULL)
    {
        pContainer->ShowLocation (pDoc, crstrLocation);
        return TRUE;
    }
    return FALSE;
}

void CMsgView::OnContextMenuCommand ()
{
    CPoint point (0, 0);

    ClientToScreen (&point);
    OnContextMenu (this, point);
}

void CMsgView::OnContextMenu(CWnd *pWnd, CPoint point) 
{
    CFrameWnd *pFrame;
    CMenu menu;
    CMenu *pPopupMenu;
    int nStart;
    int nEnd;
    UINT uiEnable;

    // make sure window is active

    pFrame = GetParentFrame ();
    ASSERT (pFrame != NULL);
    if (pFrame != NULL)
    {
        pFrame->ActivateFrame ();
    };

    if (!menu.LoadMenu (IDR_COMPILEVW_POPUP))
    {
        return;
    }
    pPopupMenu = menu.GetSubMenu (0);
    ASSERT (pPopupMenu != NULL);
    if (pPopupMenu == NULL)
    {
        return;
    }
    GetEditCtrl().GetSel(nStart, nEnd);
    uiEnable = (nStart == nEnd) ? MF_DISABLED | MF_GRAYED : MF_ENABLED;
    pPopupMenu->EnableMenuItem(IDM_MSG_COPY, uiEnable);
    pPopupMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CMsgView::OnEditCopy () 
{
    GetEditCtrl().Copy ();
}

void CMsgView::OnEditClear ()
{
    Clear();
}


void CMsgView::Clear(int iIndex) 
{
    GetEditCtrl().SetWindowText (_T(""));

    CMsgFrame* pMessageFrame = dynamic_cast<CMsgFrame *> (GetParentFrame ());
    ASSERT (pMessageFrame != NULL);
    if (NULL == pMessageFrame)
    {
        return;
    }
    if (iIndex<0)
    {
        pMessageFrame->m_tabWnd.GetActiveTab(iIndex);
    }

    CMsgDoc *pDoc;
    pDoc = GetDocument ();
    ASSERT (pDoc != NULL);
    if (pDoc == NULL)
    {
        return;
    }
    pDoc->m_arrMsgData[iIndex].m_strText = _T("");
}

