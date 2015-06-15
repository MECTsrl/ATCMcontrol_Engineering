
// STEditorDoc.cpp : implementation of the CSTEditorDoc class
//

#include "stdafx.h"
#include "fc_todebug\fc_assert.h"
#include "STEditor.h"

#include "STEditorDoc.h"
#include "STEditorView.h"
#include "SrvrItem.h"
#include "utilif.h"
#include "cesys.h"
#include "STEditorIF_i.c"
#include "Watchpoint.h"
#include "DbgInfo.h"
#include "PrintOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Helper functions

CFont * SelectPrintFont(CDC * pDCPrint, CFont * pFont)
{
	int iPixel = pDCPrint->GetDeviceCaps(LOGPIXELSY);
	pFont->CreateFont(-10 * iPixel / 72, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, _T("Courier New"));
    return pDCPrint->SelectObject(pFont);
}

/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc

const int CSTEditorDoc::IdleHint                = 10000;
const int CSTEditorDoc::SettingsChangedHint     = 10001;    // AUTOFMT 14.08.06 SIS


IMPLEMENT_DYNCREATE(CSTEditorDoc, CPrgEditorDoc)

BEGIN_MESSAGE_MAP(CSTEditorDoc, CPrgEditorDoc)
	//{{AFX_MSG_MAP(CSTEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSTEditorDoc, CPrgEditorDoc)
	//{{AFX_DISPATCH_MAP(CSTEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ISTEditor to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {9F791437-09AC-11D2-B9A5-006008749B3D}
static const IID IID_ISTEditor =
{ 0x9f791437, 0x9ac, 0x11d2, { 0xb9, 0xa5, 0x0, 0x60, 0x8, 0x74, 0x9b, 0x3d } };

BEGIN_INTERFACE_MAP(CSTEditorDoc, CPrgEditorDoc)
	INTERFACE_PART(CSTEditorDoc, IID_ISTEditor, Dispatch)
END_INTERFACE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc construction/destruction

CSTEditorDoc::CSTEditorDoc ()
//: m_bSpecialWPDisplay (false)
{
    m_bEnableWPChanges = true;
    EnableAutomation ();
    EnableConnections ();
    AfxOleLockApp ();
}


CSTEditorDoc::~CSTEditorDoc ()
{
	AfxOleUnlockApp ();
}


BOOL CSTEditorDoc::OnNewDocument ()
{
	if (!CPrgEditorDoc::OnNewDocument ())
	{
		return FALSE;
	};

	InstallSyntaxColoring ();

	return TRUE;
}


BOOL CSTEditorDoc::OnOpenDocument (LPCTSTR lpszPathName)
{
	if (!CPrgEditorDoc::OnOpenDocument (lpszPathName))
	{
		return (FALSE);
	};

	InstallSyntaxColoring ();

	return (TRUE);
}


bool CSTEditorDoc::LoadDocument (const CString &strFileName)
{
	bool bResult;

	bResult = CPrgEditorDoc::LoadDocument (strFileName);
	InstallSyntaxColoring ();
	return (bResult);
}


bool CSTEditorDoc::SaveDocument (const CString &strFileName)
{
	bool bResult;

	bResult = CPrgEditorDoc::SaveDocument (strFileName);
	InstallSyntaxColoring ();
	return (bResult);
}


void CSTEditorDoc::InstallSyntaxColoring ()
{
	TCHAR szExtension[_MAX_EXT + 1];

	UTIL_GetFileExt (m_strDocumentFile, szExtension, ARRAY_LEN (szExtension));

	if (szExtension[0] == 0)
	{
		m_Edit.SetFileMappingKey (_T ("default"));
	}
	else
	{
		m_Edit.SetFileMappingKey (szExtension);
	};

	m_Edit.InitFileTypeSettings ();
}

/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc server implementation

COleServerItem* CSTEditorDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CSTEditorSrvrItem* pItem = new CSTEditorSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc ActiveX Document server implementation

CDocObjectServer *CSTEditorDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}

/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc serialization

CSECEditor *CSTEditorDoc::GetEditor (UINT id)
{
	return (&m_Edit);
}

void CSTEditorDoc::OnSerializeDocument (CArchive &ar)
{
	CSTEditorView *pView;

	pView = GetEditView ();
	ASSERT (pView != NULL);

	if (pView == NULL)
	{
		return;
	};

	pView->SerializeText (ar);
	m_Edit.SetModified (FALSE);
	if (!ar.IsStoring ())
	{
		m_Edit.FreeUndoBuffers ();
	};
}


CSTEditorView *CSTEditorDoc::GetEditView ()
{
	POSITION pPosition;
	CView *pView;

	pPosition = GetFirstViewPosition ();

	if (pPosition == NULL)
	{
		return (NULL);
	};

	pView = GetNextView (pPosition);

	if (pView == NULL)
	{
		return (NULL);
	};

	ASSERT (pView->IsKindOf (RUNTIME_CLASS (CSTEditorView)));

	return ((CSTEditorView *)pView);
}


void CSTEditorDoc::SetModified (bool bModified)
{
	CPrgEditorDoc::SetModified (bModified);
	m_Edit.SetModified (bModified);
}


void CSTEditorDoc::OnUpdateReadOnly ()
{
	m_Edit.SetReadOnly (m_bForcedReadOnly);
}


void CSTEditorDoc::OnUpdateModified ()
{
	SetModifiedFlag (m_Edit.GetModified ());
}


void CSTEditorDoc::OnIdle ()
{
    UpdateAllViews (NULL, CPrgEditorDoc::IdleHint, NULL);
    CPrgEditorDoc::OnIdle ();
}


CString CSTEditorDoc::GetSmartText (const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition)
{
      return m_Edit.GetSmartText(position, startPosition, endPosition);
}



CString CSTEditorDoc::GetTrimmedText (const CStdEditorPosition &startPosition,
                                      const CStdEditorPosition &endPosition,
                                      CStdEditorPosition &trimmedStart,
                                      CStdEditorPosition &trimmedEnd)
{
      return m_Edit.GetTrimmedText(startPosition,endPosition, trimmedStart, trimmedEnd);
}


CStdEditorPosition CSTEditorDoc::ToOffsetPosition (const CStdEditorPosition &position)
{
    return m_Edit.ToOffsetPosition(position);
}

CStdEditorPosition CSTEditorDoc::ToDisplayPosition (const CStdEditorPosition &position)
{
    return m_Edit.ToDisplayPosition(position);
}


void CSTEditorDoc::OnShowBreakpoint (CBreakpoint *pBP)
{
    if(pBP && m_Edit.ShowBreakpoint(pBP->GetPosition ().GetY (), true))  // true: show
    {
        CPrgEditorDoc::OnShowBreakpoint (pBP);  // UpdateAllViews...
    }
}


void CSTEditorDoc::OnHideBreakpoint (CBreakpoint *pBP)
{
    if(pBP && m_Edit.ShowBreakpoint(pBP->GetPosition ().GetY (), false))  // false: hide
    {
        CPrgEditorDoc::OnHideBreakpoint (pBP);  // UpdateAllViews...
    }

}


void CSTEditorDoc::OnShowStoppoint ()
{
    if( m_Edit.ShowStoppoint(m_StopPosition.GetY(), true))
    {
        CPrgEditorDoc::OnShowStoppoint ();
    }
}


void CSTEditorDoc::OnHideStoppoint ()
{
    if( m_Edit.ShowStoppoint(m_StopPosition.GetY(), false))
    {
        CPrgEditorDoc::OnHideStoppoint ();
    }

}


void CSTEditorDoc::OnCreateStep (CPrgEdStep *pStep)
{
    CStdEditorPosition idPosition;
    CStdEditorPosition keywordPosition;
    CStdEditorPosition behindIdPosition;
    CSTStepClientData *pClientData;
//    int iLine;

    CPrgEditorDoc::OnCreateStep (pStep);

    idPosition = CStdEditorPosition (EDP_STLINE, -(pStep->GetX () - 1), pStep->GetY ());

    if (!m_Edit.FindKeywordPosition (idPosition, keywordPosition, behindIdPosition))
    {
        keywordPosition = idPosition;
    };

    pClientData = new CSTStepClientData (keywordPosition, behindIdPosition);
    pStep->SetClientData (pClientData);

    m_Edit.ShowStep( pStep->GetY(), true);
}


void CSTEditorDoc::OnDestroyStep (CPrgEdStep *pStep)
{
    CSTStepClientData *pClientData;
    POSITION pPosition;
    bool bFound=false;

    pClientData = (CSTStepClientData *)pStep->GetClientData ();
    if (pClientData != NULL)
    {
        delete pClientData;
    };

    pPosition = m_StepList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdStep *pStep2;
        CSTStepClientData *pClientData2;

        pStep2 = m_StepList.GetNext (pPosition);
        ASSERT (pStep2 != NULL);
        if (pStep2 == NULL)
            continue;

        if (pStep2 == pStep)
            continue;

        pClientData2 = (CSTStepClientData *)pStep2->GetClientData ();
        if (pClientData2->GetKeywordPosition ().GetY () == pClientData->GetKeywordPosition ().GetY ())
        {
            bFound = true;
            break;
        };
    };

    if (!bFound)
    {
        m_Edit.ShowStep( pStep->GetY(), false);
    };
}


void CSTEditorDoc::OnCreateAction (CPrgEdAction *pAction)
{
    CStdEditorPosition idPosition;
    CStdEditorPosition keywordPosition;
    CStdEditorPosition behindIdPosition;
    CSTActionClientData *pClientData;
//    int iLine;

    CPrgEditorDoc::OnCreateAction (pAction);

    idPosition = CStdEditorPosition (EDP_STLINE, -(pAction->GetX () - 1), pAction->GetY ());

    if (!m_Edit.FindKeywordPosition (idPosition, keywordPosition, behindIdPosition))
    {
        keywordPosition = idPosition;
    };

    pClientData = new CSTActionClientData (keywordPosition, behindIdPosition);
    pAction->SetClientData (pClientData);

    m_Edit.ShowAction( pAction->GetY(), true);
}


void CSTEditorDoc::OnDestroyAction (CPrgEdAction *pAction)
{
    CSTActionClientData *pClientData;
    POSITION pPosition;
    bool bFound=false;

    pClientData = (CSTActionClientData *)pAction->GetClientData ();
    if (pClientData != NULL)
    {
        delete pClientData;
    };

    pPosition = m_ActionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction2;
        CSTActionClientData *pClientData2;

        pAction2 = m_ActionList.GetNext (pPosition);
        ASSERT (pAction2 != NULL);
        if (pAction2 == NULL)
            continue;

        if (pAction2 == pAction)
            continue;

        pClientData2 = (CSTActionClientData *)pAction2->GetClientData ();
        if (pClientData2->GetKeywordPosition ().GetY () == pClientData->GetKeywordPosition ().GetY ())
        {
            bFound = true;
            break;
        };
    };

    if (!bFound)
    {
        m_Edit.ShowAction( pAction->GetY(), false);
    };
}


void CSTEditorDoc::OnCreateTransition (CPrgEdTransition *pTransition)
{
    CStdEditorPosition idPosition;
    CStdEditorPosition keywordPosition;
    CStdEditorPosition behindIdPosition;
    CSTTransitionClientData *pClientData;
//    int iLine;

    CPrgEditorDoc::OnCreateTransition (pTransition);

    idPosition = CStdEditorPosition (EDP_STLINE, -(pTransition->GetX () - 1), pTransition->GetY ());

    if (!m_Edit.FindKeywordPosition (idPosition, keywordPosition, behindIdPosition))
    {
        keywordPosition = idPosition;
    };

    pClientData = new CSTTransitionClientData (keywordPosition, behindIdPosition);
    pTransition->SetClientData (pClientData);

    m_Edit.ShowTransition( pTransition->GetY(), true);
}


void CSTEditorDoc::OnDestroyTransition (CPrgEdTransition *pTransition)
{
    CSTTransitionClientData *pClientData;
    POSITION pPosition;
    bool bFound=false;

    pClientData = (CSTTransitionClientData *)pTransition->GetClientData ();
    if (pClientData != NULL)
    {
        delete pClientData;
    };

    pPosition = m_TransitionList.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition2;
        CSTTransitionClientData *pClientData2;

        pTransition2 = m_TransitionList.GetNext (pPosition);
        ASSERT (pTransition2 != NULL);
        if (pTransition2 == NULL)
            continue;

        if (pTransition2 == pTransition)
            continue;

        pClientData2 = (CSTTransitionClientData *)pTransition2->GetClientData ();
        if (pClientData2->GetKeywordPosition ().GetY () == pClientData->GetKeywordPosition ().GetY ())
        {
            bFound = true;
            break;
        };
    };

    if (!bFound)
    {
        m_Edit.ShowTransition( pTransition->GetY(), false);
    };
}


void CSTEditorDoc::OnInstancePathModified ()
{
    HRESULT hr;
    DBILanguage language;

    if (m_pTypeDBI == NULL)
    {
        EnableSFC (false);
        CPrgEditorDoc::OnInstancePathModified ();
        return;
    };

    hr = m_pTypeDBI->get_Language (&language);
    ASSERT (hr == S_OK);
    if (hr != S_OK)
    {
        EnableSFC (false);
        CPrgEditorDoc::OnInstancePathModified ();
        return;
    };

    if (language != DBIST)
    {
        EnableSFC (false);
        CPrgEditorDoc::OnInstancePathModified ();
        return;
    };

    EnableSFC (true);
    CPrgEditorDoc::OnInstancePathModified ();
}



/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc diagnostics

#ifdef _DEBUG
void CSTEditorDoc::AssertValid() const
{
	CPrgEditorDoc::AssertValid();
}

void CSTEditorDoc::Dump(CDumpContext& dc) const
{
	CPrgEditorDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTEditorDoc commands

BOOL CSTEditorDoc::SaveModified() 
{
	OnUpdateModified ();
	return (CPrgEditorDoc::SaveModified ());
}

HRESULT CSTEditorDoc::AttachPrinterDC (CDC * pDC)
{
    // prepare printing
    CPrintDialog printDlg(FALSE);  // Instantiate a CPrintDialog object.

    // Retrieve the current printer defaults from the Windows .ini file
    // as necessary, or uses the last printer configuration set by the
    // user in Print Setup.
    CSTEditorApp* pApp = dynamic_cast<CSTEditorApp *>(AfxGetApp());

    pApp->DevModeChange((LPTSTR)(LPCTSTR)GetPrinterName());

    if (!pApp->GetPrinterDeviceDefaults (&printDlg.m_pd)) 
	{ 
        // Fails to create a printer device context for printing because
        // no printer is being installed. Bring up dialog box to alert the
        // users that they need to install a printer.  This step is optional.
        if (pApp->DoPrintDialog (&printDlg) != IDOK)
		{
            return (E_FAIL);
        }
    }

    // Creates a printer DC from the DEVMODE and DEVNAMES structures.
    if (printDlg.CreatePrinterDC () == NULL) 
    {
        return (E_FAIL);
    }

    // A printer device context (DC) has been created.
    // Attach it to a CDC object. The DC is deleted when the CDC
    // object goes out of scope.
    pDC->Attach (printDlg.GetPrinterDC ()); 
    return (S_OK);
}

long CSTEditorDoc::GetPrintLinesPerPage (CDC * pDCPrint)
{
    CFont Font;
    TEXTMETRIC tm;

    CFont * pOldFont = SelectPrintFont(pDCPrint, &Font);
    pDCPrint->GetTextMetrics(&tm);
    pDCPrint->SelectObject(pOldFont);

    CSize Size(tm.tmAveCharWidth, tm.tmHeight);
    pDCPrint->LPtoHIMETRIC(&Size);
    return (Size.cy > 0 ? GetPageHeight() / Size.cy : 0);
}

HRESULT CSTEditorDoc::GetPageCount (long &lCount)
{
    CDC dcPrint;
    int iLines = m_Edit.GetLinesInFile();
	if (0 == iLines)
	{
		lCount = 0;
        return S_OK;
	}

    if (FAILED(AttachPrinterDC(&dcPrint)))
    {
        return (E_FAIL);
    }

    long lLinesPerPage = GetPrintLinesPerPage(&dcPrint);

    if (lLinesPerPage <= 0)
    {
        return (E_FAIL);
    }

    CFont Font;
    CFont * pOldFont;

    pOldFont = SelectPrintFont(&dcPrint, &Font);
    TEXTMETRIC tm;
    dcPrint.GetTextMetrics(&tm);
    CSize tmSize(tm.tmAveCharWidth, tm.tmHeight);
    dcPrint.LPtoHIMETRIC(&tmSize);

    long lCharsPerLine = GetPageWidth() / tmSize.cx;
    long lLineNum = 0;
    for (LINEDESC * pLine = m_Edit.m_pLine; NULL != pLine; pLine = pLine->pNext)
    {
        bool bSuccess;
        CString strText;

        bSuccess = m_Edit.ExpandLine (pLine, strText);
        ASSERT (bSuccess);

        strText.TrimRight ();

        if (strText.IsEmpty())
        {
            lLineNum++;
        }else
        {
            lLineNum += (((strText.GetLength() - 1) / lCharsPerLine) + 1);
        }
    }

    dcPrint.SelectObject(pOldFont);
    lCount = (lLineNum - 1) / lLinesPerPage + 1;
	return (S_OK);
}

HRESULT CSTEditorDoc::GetPages (CStringArray &pages)
{
    TCHAR    szTempName[_MAX_PATH];
    TCHAR    szPath[_MAX_PATH];
    long lLinesPerPage;
    long lLineNum = 0;
    CDC      dcPrint;
    TEXTMETRIC tm;
    CFont Font;
    CFont * pOldFont;
    long lCharsPerLine = 80;

    if (FAILED(AttachPrinterDC(&dcPrint)))
    {
        return (E_FAIL);
    }

    dcPrint.m_bPrinting = TRUE;

    CRect rect;
    rect.SetRect( 0, 0, GetPageWidth (), GetPageHeight () );

    // printing

    lLinesPerPage = GetPrintLinesPerPage(&dcPrint);

    HDC hdcMeta = NULL;
    CDC dcMeta;

    for (LINEDESC * pLine = m_Edit.m_pLine; NULL != pLine; pLine = pLine->pNext)
    {
        bool bSuccess;
        CString strText;

        bSuccess = m_Edit.ExpandLine (pLine, strText);
        ASSERT (bSuccess);

        strText.TrimRight ();

        // if text line is empty make it a line with a single
        // blank character
        if (strText.IsEmpty())
        {
            strText = _T(" ");
        }

        while (!strText.IsEmpty())
        {

            if (hdcMeta == NULL)
            {
                VERIFY(GetTempPath(sizeof(szPath), szPath) != 0);
                VERIFY(GetTempFileName(szPath, _T("pst"), 0, szTempName) != 0);
                pages.Add( szTempName );

                CString strDesc;
                CString strAppl;
                TCHAR*   pBuf;

                strAppl = GetPathName();

                // read documentation of CreateEnhMetaFile() to understand the
                // formatting of this string
                strDesc.Format("STEditor %s Page %d ", strAppl, pLine->iLineNo);
                pBuf = strDesc.GetBuffer(0);
                pBuf[8] = 0;
                pBuf[strDesc.GetLength() - 1] = 0;
                strDesc.ReleaseBuffer();

                hdcMeta = ::CreateEnhMetaFile( dcPrint, szTempName, &rect, strDesc);

                if (!hdcMeta) 
                {
                    return (E_FAIL);
                }

                dcMeta.Attach(hdcMeta);
                dcMeta.m_bPrinting = TRUE;
                dcMeta.SetMapMode (MM_TEXT);

                pOldFont = SelectPrintFont(&dcMeta, &Font);
                dcMeta.GetTextMetrics(&tm);
                CSize tmSize(tm.tmAveCharWidth, tm.tmHeight);
                dcMeta.LPtoHIMETRIC(&tmSize);
                lCharsPerLine = GetPageWidth() / tmSize.cx;
            }

            dcMeta.TextOut(0, lLineNum * tm.tmHeight, strText.Left(lCharsPerLine));
            lLineNum++;
            if (strText.GetLength() > lCharsPerLine)
            {
                strText = strText.Mid(lCharsPerLine);
            }
            else
            {
                strText = _T("");
            }

            if (lLineNum >= lLinesPerPage)
            {
                lLineNum = 0;
                dcMeta.SelectObject(pOldFont);
                Font.DeleteObject();
                dcMeta.Detach();
                DeleteEnhMetaFile (CloseEnhMetaFile(hdcMeta));
                hdcMeta = NULL;
            }
        }
    }

    if (NULL != hdcMeta)
    {
        dcMeta.SelectObject(pOldFont);
        dcMeta.Detach();
        DeleteEnhMetaFile (CloseEnhMetaFile(hdcMeta));
        hdcMeta = NULL;
    }

    return (S_OK);
}

HRESULT CSTEditorDoc::ShowPrintOptionsDialog(CString& rstrOptions)
{
//    CPrintOptionsDlg    tDlg;
//    if(tDlg.DoModal() == IDOK)
//    {
//        TRACE("IDOK\n");
//    }
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
/**
 * get selected text.
 *
 * @param           -
 * @return          selected text
 * @exception       -
 * @see             
*/
CString CSTEditorDoc::GetSelectedText()
{
    CString strText;
    CSTEditorView*  pView = GetEditView();
    if(pView)
    {
        strText = pView->GetSelection();
    }
    return strText;
}


void CSTEditorDoc::RecreateToolTip()
{
    CSTEditorView*  pView = GetEditView();
    ASSERT(pView);
    if(pView)
    {
        pView->RecreateToolTip();
    }
}
