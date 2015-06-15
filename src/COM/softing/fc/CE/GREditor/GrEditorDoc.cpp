

#include "stdafx.h"
#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"
#include "GridDlg.h"
#include "SrvrItem.h"
#include "StatDlg.h"

#include "CEGRError.h"

#include "GrEditorIF_i.c"
#include "Watchpoint.h"
#include "STEditorView.h"
#include "GrPrintOptionsDlg.h"

#include "IpFrame.h"

#define  IDD_FINDFILE_DLG 0   // work around .. 
#include "FindFileDlg.h"
#undef   IDD_FINDFILE_DLG 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// print page setup default values ..
#define DEF_PAGE_WIDTH     850      // page width in inch * 100
#define DEF_PAGE_HEIGHT   1100      // page height in inch * 100
#define DEF_MAX_HEIGHT   65000      // max. print height (A2) in mm * 100
#define DEF_MAX_WIDHT    65000      // max. print widht (A2) in mm * 100
#define DEF_MARG_LEFT       63      // left margin in inch * 100
#define DEF_MARG_RIGHT      63      // right margin in inch * 100
#define DEF_MARG_TOP       118      // top margin in inch * 100
#define DEF_MARG_BOTTOM    106      // bottom margin in inch * 100
#define DEF_NP_MARG_LEFT    21      // not printable left margin in inch * 100
#define DEF_NP_MARG_RIGHT   28      // not printable right margin in inch * 100
#define DEF_NP_MARG_TOP     17      // not printable top margin in inch * 100
#define DEF_NP_MARG_BOTTOM  23      // not printable bottom margin in inch * 100


/////////////////////////////////////////////////////////////////////////////
// Helper function

CFont * SelectPrintFont(CDC * pDCPrint, CFont * pFont)
{
    int iPixel = pDCPrint->GetDeviceCaps(LOGPIXELSY);
    pFont->CreateFont(-10 * iPixel / 72, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, _T("Courier New"));
    return pDCPrint->SelectObject(pFont);
}


/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc

const int CGrEditorDoc::UpdateWindowHint     = 10000;
const int CGrEditorDoc::UpdateDrawHint       = 10001;
const int CGrEditorDoc::UpdateDrawElemHint   = 10002;
const int CGrEditorDoc::UpdateSelectionHint  = 10003;
const int CGrEditorDoc::UpdateMarkedElemHint = 10004;
const int CGrEditorDoc::UpdateOleItemsHint   = 10005;
const int CGrEditorDoc::UpdateRubberHint     = 10006;
//nst int CGrEditorDoc::IdleHint             = 10007; (now in PrgEditor)
const int CGrEditorDoc::UpdateLightPinHint   = 10008;
const int CGrEditorDoc::UpdateModifiedHint   = 10009;
const int CGrEditorDoc::UpdateColorHint      = 10010;
const int CGrEditorDoc::EditNameStopHint     = 10011;
const int CGrEditorDoc::UpdateStatusHint     = 10012;
const int CGrEditorDoc::SetPageSizeHint      = 10013;
const int CGrEditorDoc::UpdateFixPointsHint  = 10014;
const int CGrEditorDoc::UpdateSTActionsHint  = 10015;

const int CGrEditorDoc::SyncSTActionsHint    = 10017;
const int CGrEditorDoc::VarSelStopHint       = 10018;
const int CGrEditorDoc::StartLDTracingHint   = 10019;
const int CGrEditorDoc::StopLDTracingHint    = 10020;

bool CGrEditorDoc::m_bFindNoWords = true;
bool CGrEditorDoc::m_bFindNoRegEx = true;

IMPLEMENT_DYNCREATE(CGrEditorDoc, CPrgEditorDoc)

BEGIN_MESSAGE_MAP(CGrEditorDoc, CPrgEditorDoc)
	//{{AFX_MSG_MAP(CGrEditorDoc)
	ON_COMMAND(ID_GR_VIEW_GRID, OnViewGrid)
	ON_COMMAND(ID_GR_OPT_LINERUBB, OnOptLinerubb)
	ON_COMMAND(ID_GR_OPT_COLNORMAL,   OnOptColnormal)
	ON_COMMAND(ID_GR_OPT_COLSELECT,   OnOptColselect)
	ON_COMMAND(ID_GR_OPT_COLERROR,    OnOptColerror)
	ON_COMMAND(ID_GR_OPT_COLCOMMENT,  OnOptColcomment)
	ON_COMMAND(ID_GR_OPT_COLRUBBER,   OnOptColrubber)
	ON_COMMAND(ID_GR_OPT_COLEDITLINE, OnOptColeditline)
	ON_COMMAND(ID_GR_OPT_COLGRID,     OnOptColgrid)
	ON_COMMAND(ID_GR_OPT_COLBACKGD,   OnOptColsheet)
	ON_COMMAND(ID_GR_OPT_COLELEMBK,   OnOptColelembk)
	ON_COMMAND(ID_GR_OPT_COLLADDBK,   OnOptColladdbk)
	ON_COMMAND(ID_GR_OPT_COLFBLKBK,   OnOptColfblkbk)
	ON_COMMAND(ID_GR_OPT_COLEDGE,     OnOptColedge)
	ON_COMMAND(ID_GR_OPT_COLLIGHTPIN, OnOptCollightpin)
	ON_COMMAND(ID_GR_OPT_COLBRKREC    , OnOptColbrkrec)
	ON_COMMAND(ID_GR_OPT_COLBRKDIS    , OnOptColbrkdis)
	ON_COMMAND(ID_GR_OPT_COLBRKACT    , OnOptColbrkact)
	ON_COMMAND(ID_GR_OPT_COLBRKDEC    , OnOptColbrkdec)
	ON_COMMAND(ID_GR_OPT_COLBACKGD_SFC, OnOptColbackgd_sfc)
	ON_COMMAND(ID_GR_OPT_COLELEMBK_SFC, OnOptColelembk_sfc)
	ON_COMMAND(ID_GR_OPT_COLLADDBK_SFC, OnOptColladdbk_sfc)
	ON_COMMAND(ID_GR_OPT_COLFBLKBK_SFC, OnOptColfblkbk_sfc)
	ON_COMMAND(ID_GR_OPT_COLACTIVE_SFC, OnOptColactive_sfc)
	ON_COMMAND(ID_GR_OPT_COLFORCED_SFC, OnOptColforced_sfc)
	ON_COMMAND(ID_GR_OPT_COLBLOCKD_SFC, OnOptColblockd_sfc)
	ON_COMMAND(ID_GR_OPT_FONT, OnOptFont)
	ON_COMMAND(ID_GR_OPT_RECON, OnOptRecon)
	ON_UPDATE_COMMAND_UI(ID_GR_OPT_LINERUBB, OnUpdateOptLinerubb)
	ON_UPDATE_COMMAND_UI(ID_GR_OPT_RECON, OnUpdateOptRecon)
	ON_COMMAND(ID_GR_STATISTIC, OnAppStatistic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_OLECMD_MAP(CGrEditorDoc, CPrgEditorDoc)
//   ON_OLECMD_NEW( )
//   ON_OLECMD_OPEN( )
//   ON_OLECMD_CLEARSELECTION( )
//   ON_OLECMD_SAVE( )
//   ON_OLECMD_SAVE_AS( )
//   ON_OLECMD_SAVE_COPY_AS( )
//   ON_OLECMD_COPY( )
//   ON_OLECMD_CUT( )
//   ON_OLECMD_PASTESPECIAL( )
//   ON_OLECMD_PASTE( )
//   ON_OLECMD_PAGESETUP( )
   ON_OLECMD_PRINT()
   ON_OLECMD_PRINTPREVIEW()
//   ON_OLECMD_REDO( )
//   ON_OLECMD_SELECTALL( )
//   ON_OLECMD_UNDO( )
END_OLECMD_MAP()

BEGIN_DISPATCH_MAP(CGrEditorDoc, CPrgEditorDoc)
	//{{AFX_DISPATCH_MAP(CGrEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IGrEditor to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {59119A15-6422-11D2-9A3D-0000C0D26FBC}
static const IID IID_IGrEditor =
{ 0x59119a15, 0x6422, 0x11d2, { 0x9a, 0x3d, 0x0, 0x0, 0xc0, 0xd2, 0x6f, 0xbc } };

BEGIN_INTERFACE_MAP(CGrEditorDoc, CPrgEditorDoc)
	INTERFACE_PART(CGrEditorDoc, IID_IGrEditor, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc construction/destruction

CGrEditorDoc::CGrEditorDoc()
{
    CString strKey, strSec;

    m_HeaderEditor.SetFileMappingKey(_T("ST"));
    m_HeaderEditor.InitFileTypeSettings();
    m_HeaderEditor.m_crBackground = GDIHelper::GetColor(true, CO_BACKGD);

    // normal initialization
    m_eEditMode = EM_LAST; // preset
    SetEditMode(EM_NORMAL);

    strSec.LoadString(IDS_GR_REG_SECTION);

    strKey.LoadString(IDS_GR_REG_RUBBLINE);
    m_eLineMode   = /*(AfxGetApp()->GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 0) == 0) ?LDM_ORTHO :*/LDM_RUBB;

    strKey.LoadString(IDS_GR_REG_RUBB_RECON);
    m_bRecon = (AfxGetApp()->GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 1) == 1) ? true : false;

    strKey.LoadString(IDS_GR_REG_ZOOMVALUE);
    int i = AfxGetApp()->GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 100);
    m_fZoom       = (float)i / 100.0f;

    SetCurrLang(EL_ALL);
    m_bEnableSFC  = true;

    m_PageSize    = CSize(0,0);
    m_PageGrid    = CSize(0,0);
    m_DocGrid     = CSize(0,0);

    m_bNoDelContents = false;
    m_bStandAloneDoc = true;

    // values for page setup
    m_iMargLeft   = DEF_MARG_LEFT   - DEF_NP_MARG_LEFT; // margins in 0.01 inch
    m_iMargTop    = DEF_MARG_TOP    - DEF_NP_MARG_TOP;
    m_iMargRight  = DEF_MARG_RIGHT  - DEF_NP_MARG_RIGHT;
    m_iMargBottom = DEF_MARG_BOTTOM - DEF_NP_MARG_BOTTOM;

    // load from setup or desktop/workshop
    m_strFont     = _T("Arial");
    m_iFontHeight = 11;

    m_eGrid = GT_POINT;
    m_iGrid = (int)(m_iFontHeight * 100.0 / (float)FontToGrid / 2.0);

    m_bSpecialWPDisplay = false;
    m_bEnableWPChanges = true;
    m_bPrintSTHeader = true;

    InitPageSize();
    SetMarginRect(TRUE);
    ComputePageSize();
    UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);

    ForceReadOnly(false);   // call AFTER the header editor has been created

    m_bEnableWPChanges = true;
    EnableAutomation();
    EnableConnections ();
    AfxOleLockApp();
}


CSECEditor* CGrEditorDoc::GetEditor(UINT id)  
{
   if (id==0)
   {
      return &m_HeaderEditor;
   } else {
      if (id>0 && id<=(UINT)m_Editors.GetSize())
      {
         return m_Editors[id-1];
      }
   }
   return NULL;
}


CGrEditorDoc::~CGrEditorDoc()
{
    DeleteAllEditors();
    AfxOleUnlockApp();
}


UINT CGrEditorDoc::CreateEditor(const CString& FileMappingKey)
{
    UINT id=0;  // invalid

    CSECEditor* pEdit = new CSECEditor;
    if (pEdit)
    {
        pEdit->SetFileMappingKey(FileMappingKey);
        pEdit->InitFileTypeSettings();
        pEdit->m_crBackground = GDIHelper::GetColor(true, CO_BACKGD);
        for (id = 0; id < (UINT)m_Editors.GetSize(); id++)
        {
            if (m_Editors[id]==NULL)
            {
                m_Editors[id] = pEdit;
                return (id+1);
            }
        }
        id = m_Editors.Add(pEdit) + 1;
    } else
    {
        ASSERT(0 && "failed to create editor");
    }
    return id;
}


void CGrEditorDoc::DeleteEditor(UINT id)
{
    ASSERT(id>0);
    ASSERT(id<=(UINT)m_Editors.GetSize());

    if( id>0 && id<=(UINT)m_Editors.GetSize() )
    {
        delete m_Editors[id-1];
        _RPT1(_CRT_WARN,"\n EDITOR %i deleted\t", id);
        m_Editors[id-1] = NULL;
    } else
    {
        ASSERT(0 && "invalid editor ID");
    }
}


void CGrEditorDoc::DeleteAllEditors()
{
    for(int i=0; i<m_Editors.GetSize(); i++)
    {
        CSECEditor* pEdit = m_Editors[i];
        if(pEdit)
        {
           _RPT1(_CRT_WARN,"\n EDITOR %i deleted\t", i+1);
           delete pEdit;
           m_Editors[i] = NULL;
        }
    }
    m_Editors.RemoveAll();
}


void CGrEditorDoc::DeleteContents() 
{
   POSITION       pPosition = GetFirstViewPosition ();
   CGrEditorView *pView;

   m_Elem.UnselectAll();

   while (pPosition)
   {
      pView = dynamic_cast<CGrEditorView *> (GetNextView (pPosition));
      if (pView)
      {
         pView->OnDeleteSTActions(false);  // delete all STActionViews
         break;
      }
   }

   m_Elem.DeleteAllElems();
	
   if( !m_bNoDelContents ) CPrgEditorDoc::DeleteContents();
}


BOOL CGrEditorDoc::OnNewDocument()
{

   CGRTool::ResetIDs();
	if (!CPrgEditorDoc::OnNewDocument())
   {
		return FALSE;
   }
   m_strDocumentFile = GetPathName ();
   m_strBaseFile = GetPathName ();

	// TODO: add reinitialization code here
   m_Action.StoreState(&m_Elem);

	return TRUE;
}


BOOL CGrEditorDoc::OnOpenDocument (LPCTSTR lpszPathName)
{
    CGRTool::ResetIDs();
    if (!CXMLDocument<CPrgEditorDoc>::OnOpenDocument (lpszPathName))
    {
        return (FALSE);
    };
    SetPathName(lpszPathName);
    if (lpszPathName != NULL)
    {
        m_strDocumentFile = lpszPathName;
        m_strBaseFile     = lpszPathName;
    }
    
    m_Action.DeleteAll();
    m_Action.StoreState(&m_Elem);
    SetModifiedFlag(m_Action.StateChangedQ());

    ASSERT(GetEditor(0));   // header editor
/*
    bool bMod = m_Action.StateChangedQ();
    CSECEditor* pEdit;

    for (int i=0; i<m_Editors.GetSize(); i++)
    {
       pEdit = GetEditor(i);
       if (pEdit)
       {
          pEdit->SetModified(bMod);
       } else {
          ASSERT(0); // after loading - all SECEdit should be != NULL
       }
    }
*/
    SetHeaderViewText();
    
    UpdateAllViews(NULL); //, CGrEditorDoc::UpdateSTActionsHint, NULL);
   
    return (TRUE);
}


BOOL CGrEditorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
   
   UpdateAllViews(NULL, SyncSTActionsHint, GetElemList() );
   
   GetSTViewText();         // read text from view -> m_Header
   CSECEditor* pEdit;
   for (int i=0; i<= m_Editors.GetSize(); i++)
   {
      pEdit = GetEditor(i);
      if (pEdit)  // deleted SECEdit's are NULL - is ok
      {
         pEdit->SetModified(FALSE);
      } 
   }
   
   m_Elem.ConnectAllElems();    // prevent zero lines etc.
   
   if( (m_eEditMode != EM_NORMAL) && (m_eEditMode != EM_MONITOR) ) //keep MONITOR-mode if set
   {
      SetEditMode(EM_NORMAL);
   }
   
   if(!CXMLDocument<CPrgEditorDoc>::OnSaveDocument(lpszPathName))
   {
      return (FALSE);
   } else
   {
      SetModified( m_Action.StateChangedQ(true) ); // reset modified flag
      
      if (lpszPathName != NULL)
      {
         m_strDocumentFile = lpszPathName;
         m_strBaseFile = lpszPathName;
      };
   }
   
   return (TRUE);
}

bool CGrEditorDoc::SaveDocument (const CString &strFileName)
{
   if (!OnSaveDocument((LPCTSTR) strFileName))
   {
      return false;
   }
   return true;
}

bool CGrEditorDoc::LoadDocument (const CString &strFileName)
{
    m_bNoDelContents = true;
    if (!OnOpenDocument ((LPCTSTR) strFileName))
    {
        return false;
    }
    m_bNoDelContents = false;
    m_bStandAloneDoc = false;

    UpdateAllViews(NULL, CGrEditorDoc::UpdateSTActionsHint, NULL);

    return true;
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc server implementation

COleServerItem* CGrEditorDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CGrEditorSrvrItem* pItem = new CGrEditorSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc Active Document server implementation

CDocObjectServer *CGrEditorDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}



/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc serialization

void CGrEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
      m_Elem.Serialize(ar);
	} else
	{
      m_Elem.Serialize(ar);
	}
}

void CGrEditorDoc::SerializeXML(CXMLArchive& ar)
{
   m_Elem.RegisterXMLTypes(ar);

   if(ar.IsStoring())    
   {        
      try {
         // set doctype once in document!!
         ar.SetDocType(_T("FCPOU"));            
         ar.SetDTD(_T("FCPOU.dtd"));

         ar.MoveDown( XML_4GR_ELEM_EDITINFO );
         ar.SetAttribute( XML_4GR_ATTR_EDITNAME,    _T(""));
         ar.SetAttribute( XML_4GR_ATTR_EDITVERSION, _T(""));
         ar.SetAttribute( XML_4GR_ATTR_EDITCATID,   _T(""));
         ar.SetAttribute( XML_4GR_ATTR_EDITCLASSID, _T(""));
         ar.MoveUp();

         ar.MoveDown( XML_4GR_ELEM_HEADER );
         ar.SetCData( m_Header );
         ar.MoveUp();

         ar.MoveDown( XML_4GR_ELEM_BODY );

         if(m_sCurrLang.IsEmpty())
         {
            CStatisticData data;
            m_Elem.GetStatisticData(data);
            // count the really pure FBD elements (jump, connector, .. are also in ld!)
            int  iSumFBD = data.m_iFBD_VarFb + data.m_iFBD_VarRd + data.m_iFBD_VarWr;
            m_sCurrLang = XML_4GR_ATTR_VAL_ALL;

            if (data.m_iSUM_SFC > 0) 
            {
               if ((iSumFBD > 0) && (data.m_iSUM_LD == 0))
               {
                  m_sCurrLang = XML_4GR_ATTR_VAL_FBD_SFC;
               } 
               else if((iSumFBD == 0) && (data.m_iSUM_LD > 0))
               {
                  m_sCurrLang = XML_4GR_ATTR_VAL_LD_SFC;
               } 
            }
            else 
            {
               if ((iSumFBD > 0) && (data.m_iSUM_LD == 0))
               {
                  m_sCurrLang = XML_4GR_ATTR_VAL_FBD;
               } 
               else if((iSumFBD == 0) && (data.m_iSUM_LD > 0))
               {
                  m_sCurrLang = XML_4GR_ATTR_VAL_LD;
               } 
            }
         }
         ar.SetAttribute( XML_4GR_ATTR_LANG, m_sCurrLang);

         if (m_bEnableSFC)
         {
            ar.SetAttribute( XML_4GR_ATTR_SFC_MENU, 1L);
         } else  {
            ar.SetAttribute( XML_4GR_ATTR_SFC_MENU, 0L);
         }
         // walk thru list of childs and serialize them
         // this CList-template behind GetNodeList() is appl. specific !!!
         ar << &m_Elem;

         ar.MoveUp();
      }
      catch (HRESULT hr)        
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {
         CString str;
         ar.MoveDown( XML_4GR_ELEM_EDITINFO );
         ar.GetAttribute( XML_4GR_ATTR_EDITNAME,    str);   // TBD!!!
         ar.GetAttribute( XML_4GR_ATTR_EDITVERSION, str);
         ar.GetAttribute( XML_4GR_ATTR_EDITCATID,   str);
         ar.GetAttribute( XML_4GR_ATTR_EDITCLASSID, str);
         ar.MoveUp();
         //  
         ar.MoveDown( XML_4GR_ELEM_HEADER );
         m_Header.Empty();
         ar.GetCData( m_Header );
         ar.MoveUp();
         //
         ar.MoveDown( XML_4GR_ELEM_BODY );

         ar.GetAttribute( XML_4GR_ATTR_LANG, m_sCurrLang);  // only for info ..

         IEC_GR_Lang Lang = LANG_SFC;

         if      (m_sCurrLang == XML_4GR_ATTR_VAL_FBD) 
         { 
            SetCurrLang(EL_FBD);     m_bEnableSFC = FALSE;
            Lang = LANG_FBD;

         }
         else if (m_sCurrLang == XML_4GR_ATTR_VAL_LD) 
         { 
            SetCurrLang(EL_LD);      m_bEnableSFC = FALSE; 
            Lang = LANG_LD;
         }
         else if (m_sCurrLang == XML_4GR_ATTR_VAL_FBD_SFC)
         { 
            SetCurrLang(EL_ALL);/*!*/m_bEnableSFC = TRUE;  
         }
         else if (m_sCurrLang == XML_4GR_ATTR_VAL_LD_SFC)
         { 
            SetCurrLang(EL_ALL);/*!*/m_bEnableSFC = TRUE;  
         }else                                              
         { 
            SetCurrLang(EL_ALL);     m_bEnableSFC = TRUE;
         }

         // slurp in all child element nodes and create objects as necessary!!!
         ar >> &m_Elem;
         ar.MoveUp();

         // check for correct source -> greeting the notepad freaks
         m_Elem.AddAllElemsToPinTable();
         m_Elem.ConnectAllElems();
         m_Elem.RecoverTempVars(); // must be called after language detection

         m_Elem.SetIECLanguage(Lang);  // call AFTER the elements are inserted into the pin table

         m_Elem.bValidPos = !m_Elem.IsOneOverlapped();
         if (!m_Elem.bValidPos)
         {
            SystemMessage (E_CE_GR_INVALID_ELEMENT_POSTION);
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc serialization

void CGrEditorDoc::OnSerializeDocument (CArchive &ar)
{
   Serialize(ar);
}


void CGrEditorDoc::SetModified (bool bModified)
{
	CPrgEditorDoc::SetModified (bModified);
}


void CGrEditorDoc::OnUpdateReadOnly ()
{
   CSECEditor* pEdit;
   
   for (int idx=0; idx <= m_Editors.GetSize(); idx++)
   {
      pEdit = GetEditor(idx);
      if (pEdit)
      {
         pEdit->SetReadOnly(m_bForcedReadOnly);
      }
   }
}


void CGrEditorDoc::OnUpdateModified ()
{
   CSECEditor* pEdit;

   UpdateAllViews(NULL, CGrEditorDoc::UpdateModifiedHint);

   for (int idx=0; idx <= m_Editors.GetSize(); idx++)
   {
      pEdit = GetEditor(idx);
      if (pEdit && pEdit->GetModified())
      {
         SetModifiedFlag(true);
         break;
      }
   }
}

/*func*------------------------------------------------------------------------
  FindFirst: used by FindInFiles to search a string inside the document
  Description = find first occurrence of the given string in the active document
  in :sStringToSearch - string to search for
      uiOption        - defines the search mode; see FIF_OPTION_.. in file 
                        "findfileappDlg.h"
      psLocation      - pointer to the BSTR for the generated location string
  out:hr = S_OK       - first occurrence of string found in document; 
                        psLocation is valid
      hr = S_FALSE    - string not found; psLocation points to empty string
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::FindFirst (const CString &strStringToSearch, UINT uiOption, CString &strLocation)
{
   POSITION       pPosition = GetFirstViewPosition ();
   CGrEditorView *pView     = dynamic_cast<CGrEditorView *> (GetNextView (pPosition));

   while(!pView && pPosition)                 // search in the Header first
   {
      pView = dynamic_cast<CGrEditorView *> (GetNextView (pPosition));
   }

   int   i;
   bool  bWarning = false;

	ASSERT (pView != NULL);
	if (pView != NULL)
	{
      pView->m_SearchDat.m_strFind   = strStringToSearch;
      pView->m_SearchDat.m_bNext     = true; // forward
      pView->m_SearchDat.m_bFindOnly = true; // no replace

      pView->m_SearchDat.m_FindCur   = CPoint(0,0); // header
      pView->m_SearchDat.m_iFindElem = INT_MIN;
      pView->m_SearchDat.m_iFindPos  = 0;
      pView->m_SearchDat.m_iFindLen  = 0;

      // option whole words only ..
      if( uiOption & FIF_OPTION_WHOLEWORD     )  // not implemented  TBD !
      {
         if (m_bFindNoWords)
         {
            m_bFindNoWords = false;
//            AfxMessageBox(IDS_GR_FINDNOWORDS);
            CString str;

            str.LoadString(IDS_GR_FINDNOWORDS);
            strLocation = _T("f,0,0,0,0 : ") + str;
            bWarning = true;
         }
      }
      // option case sensitive ..
      if( uiOption & FIF_OPTION_CASESENSITIVE ) pView->m_SearchDat.m_bCase = true; 
      else                                      pView->m_SearchDat.m_bCase = false;
      // option regular expressions ..
      if( uiOption & FIF_OPTION_REGEXP        )  // not implemented  TBD !
      {
         if (m_bFindNoRegEx)
         {
            m_bFindNoRegEx = false;
//            AfxMessageBox(IDS_GR_FINDNOREGEXP);
            CString str;

            str.LoadString(IDS_GR_FINDNOREGEXP);
            if (!bWarning)
            {
               strLocation = _T("f,0,0,0,0 : ");
            }
            strLocation += str;

            bWarning = true;
         }
      }
      if (bWarning)
      {
         return (S_OK);
      }

      if( pView->FindText(false) )
      {
         i = ((pView->m_SearchDat.m_iFindPos) << 16) | (pView->m_SearchDat.m_iFindLen);
         strLocation.Format(_T("f,%d,%d,%d,%d"), 
                            pView->m_SearchDat.m_FindCur.x, 
                            pView->m_SearchDat.m_FindCur.y, 
                            pView->m_SearchDat.m_iFindElem, 
                            i );
         return (S_OK);
      }
	}

   return (S_FALSE);
}


/*func*------------------------------------------------------------------------
CSTEditorView* CGrEditorDoc::GetSTViewRef(CPoint context)
RET: pointer to the STEditorView that matches the context
-----------------------------------------------------------------------------*/
CView* CGrEditorDoc::GetSTViewRef(CPoint context)
{
    POSITION pPosition = GetFirstViewPosition ();
    CSTEditorView *pView = dynamic_cast<CSTEditorView *> (GetNextView (pPosition));

    while(!pView && pPosition)   // TODO: search in STH-view too
    {
        pView = dynamic_cast<CSTEditorView *> (GetNextView (pPosition));
    }
    return pView;
}


/*func*------------------------------------------------------------------------
  FindNext: used by FindInFiles to search a string inside the document
  Description   = find the next occurrence of the given string in the active document; 
                  FindNext is using all parameter of a prviously called FindFirst()
  in :psLocation    - pointer to the BSTR for the generated location string

  out:hr = S_OK     - first the next occurrence of string found in document; 
                      psLocation is valid
      hr = S_FALSE  - string not found (end of document reached); 
                      psLocation points to empty string
      hr = E_FAIL   - no previously call to FindFirst() detected
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::FindNext (CString &strLocation)
{
    int   i;
    POSITION       pPosition = GetFirstViewPosition ();
    CGrEditorView *pView = dynamic_cast<CGrEditorView *> (GetNextView (pPosition));

    while(!pView && pPosition) 
    {
        pView = dynamic_cast<CGrEditorView *> (GetNextView (pPosition));
    }
       
    ASSERT (pView != NULL);
    if (pView != NULL)
    {
        if( pView->FindText(false, false) )
        {
            i = ((pView->m_SearchDat.m_iFindPos) << 16) | (pView->m_SearchDat.m_iFindLen);
            
            strLocation.Format( _T("f,%d,%d,%d,%d"),
                pView->m_SearchDat.m_FindCur.x, 
                pView->m_SearchDat.m_FindCur.y,
                pView->m_SearchDat.m_iFindElem, 
                i);
            
            return (S_OK);
        }
    }
    
    return (S_FALSE);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnIdle ()
{
//	can't call: UpdateAllViews (NULL, IdleHint, NULL); !!!
// PrintPreview don't know my IdleHint (permanently flicker ..)
    
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
        CView* pView = GetNextView(pos);
        ASSERT_VALID(pView);

        if (pView != NULL)
        {
            CGrEditorView *pGrView = dynamic_cast<CGrEditorView *>(pView);
            if (pGrView != NULL)
            {
                pGrView->OnUpdate(NULL, IdleHint, NULL);
            }else
            {
                CSTEditorView* pSTView = dynamic_cast<CSTEditorView *>(pView);
                if(pSTView)
                {
                    pSTView->OnUpdate(NULL, IdleHint, NULL);
                }
            }
        }
    }
    
    CPrgEditorDoc::OnIdle ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorDoc::ConvertPar( LPTSTR pszLine, long *lValues, int iCount)
{
   LPTSTR  pszEnd;
   int     i;

   pszEnd = pszLine; // only to suppress compiler warning ..

   for ( i=0; i<iCount; i++ )
   {
      if( *pszLine == _T('\0') )
      {
		   return false;
      }
      lValues[i] = _tcstol (pszLine, &pszEnd, 10);
	   if ( i+1 < iCount )
      {
         if( *pszEnd != _T (','))
         {
		      return false;
         }
         pszLine = pszEnd + 1;
      }
   }
   if (*pszEnd != _T ('\0'))
   {
      return false;
   }
   return true;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CString CGrEditorDoc::GetSmartTextGr(const CStdEditorPosition &position, 
                                     CStdEditorPosition &startPosition,
                                     CStdEditorPosition &endPosition,
                                     bool bOnlyAbbrText, 
                                     int iGrid)
{
   CString  str;
   CPoint   gridPos, point;
   CElem   *pE;
   CRect    r;

   point = CPoint (position.GetX(), position.GetY());

   pE = GetElemList()->SearchElem (point, iGrid);
   if (pE != NULL)
   {
      if (pE->SearchTextRect(point, r, iGrid))
      {
         CText &Text=pE->GetTextAt( point, gridPos, iGrid);

         switch (m_eEditMode)
         {
            default:
               if (!bOnlyAbbrText || Text.m_Flag.Chk(TF_ABBR))
               {
                  str = Text.Str();
               }
               break;

            case EM_MONITOR:
               if (!bOnlyAbbrText || Text.m_Flag.Chk(TF_ABBR))
               {
                  switch (pE->GetType())
                  {
                     case ET_ACTION:
                        // today we no access to ST for datatip and watchpoint !
                        str = ((CElemAct*)pE)->GetName() + _T(".A");
                        break;

                     case ET_ACTIONBLOCK:
                        // today we no access to ST for datatip and watchpoint !
                        str = ((CElemAct*)pE)->GetName() + _T(".A");
                        break;

                     case ET_COMMENT:
                     case ET_CONIN:
                     case ET_CONOUT:
                     case ET_CONST:
                     case ET_ALTDIV:
                     case ET_SIMDIV:
                        break;

                     case ET_FB:
                        if (Text != ((CElemFB*)pE)->GetInst())
                        {
                           str = ((CElemFB*)pE)->GetInst() + _T(".") + (CString&)Text;
                        } 
                        break;

                     case ET_FUNCTION:
                     case ET_PROCEDURE:
                        break;

                     case ET_LABEL:
                     case ET_JUMP:
                     case ET_LINE:
                     case ET_RETURN:
                        break;

                     case ET_STEP:
                        str = Text.Str() + _T(".X");
                        break;

                     case ET_TRANSITION:
                     case ET_VARIN:
                     case ET_VAROUT:
                     case ET_VARINOUT:
                        str = Text.Str();
                        break;

                     case ET_POWERRAIL:
                     case ET_CONTACT:
                     case ET_COIL:
                        str = Text.Str();
                        break;
                  }
               }
               break;
         }
         startPosition = CStdEditorPosition (position.GetPosQual(), r.left, r.top, -1, -1);
         endPosition   = CStdEditorPosition (position.GetPosQual(), r.right, r.bottom, -1, -1);
      }
   }
   return str;
}



/*func*------------------------------------------------------------------------
  description: GetSmartText implementation of the PrgEditor method
  in :
  out:
-----------------------------------------------------------------------------*/
CString CGrEditorDoc::GetSmartText(const CStdEditorPosition &position,
                                         CStdEditorPosition &startPosition,
                                         CStdEditorPosition &endPosition)
{
   CString sRet;
   CSECEditor* pEdit=NULL;
   UINT id;
   
   switch (position.GetPosQual())
   {
   case EDP_GRBOX  :
   case EDP_GRLDBOX:
      sRet = GetSmartTextGr(position, startPosition, endPosition);
      break;

   case EDP_STLINE :
   case EDP_GRSTBOX:
      id = position.GetCX();
      if (id == 0 || id == -1)
      {
         pEdit = &m_HeaderEditor;
      } 
      else if (id>0 && id <= (UINT)m_Editors.GetSize())
      {
         pEdit = m_Editors[id-1];
      }
      if (pEdit)
      {
         sRet = pEdit->GetSmartText(position, startPosition, endPosition);
      }
      break;

   case EDP_GRREGION:
   case EDP_GRBEGIN :
   case EDP_GREND   :
   case EDP_GRLINE  :	
   case EDP_GRSIGNAL: // ??
      break;
   }

   return sRet;
}


// d-2155 22.03.05 SIS >>
/*func*------------------------------------------------------------------------
  description: GetTrimmedText implementation of the PrgEditor method
  in :
  out:
-----------------------------------------------------------------------------*/
CString CGrEditorDoc::GetTrimmedText (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, CStdEditorPosition &trimmedStart, CStdEditorPosition &trimmedEnd)
{
   CString sRet;
   CSECEditor* pEdit=NULL;
   UINT id;
   
   switch (startPosition.GetPosQual())
   {
   case EDP_GRBOX  :
   case EDP_GRLDBOX:
      sRet = GetSmartTextGr(startPosition, trimmedStart, trimmedEnd);
      break;

   case EDP_STLINE :
   case EDP_GRSTBOX:
      id = startPosition.GetCX();
      if (id == 0 || id == -1)
      {
         pEdit = &m_HeaderEditor;
      } 
      else if (id>0 && id <= (UINT)m_Editors.GetSize())
      {
         pEdit = m_Editors[id-1];
      }
      if (pEdit)
      {
         sRet = pEdit->GetTrimmedText(startPosition, endPosition, trimmedStart, trimmedEnd);
      }
      break;

   case EDP_GRREGION:
   case EDP_GRBEGIN :
   case EDP_GREND   :
   case EDP_GRLINE  :	
   case EDP_GRSIGNAL: // ??
      break;
   }

   return sRet;
}
// d-2155 22.03.05 SIS <<

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::DecodeLocation (const CString &strLocation, CString &strType,
   CStdEditorPosition &position, CPoint &endPos)
{
   CString strLine;
   LPTSTR  pszLine;
   bool    bResult;
   long    lValues[4];

   strLine = strLocation;

   if( strLine.Left(2) == "l," ||         // line
       strLine.Left(2) == "r," ||         // region
       strLine.Left(2) == "f," )          // find position
   {
      strType = strLine.Left(1);
      pszLine = strLine.GetBuffer (0);
      bResult = ConvertPar( pszLine+2, lValues, 4);
      strLine.ReleaseBuffer ();
      if( !bResult )
      {
		   return (E_FAIL);
      };
      position = CStdEditorPosition(EDP_STLINE, lValues[0], lValues[1] );
      endPos   = CPoint( lValues[2], lValues[3] );
   }
   else if( strLine.Left(2) == "b," ||    // action block, ST-action
            strLine.Left(2) == "B," ||    // action block, ST-action
            strLine.Left(2) == "s," )     // signal
   {
      strType = strLine.Left(1);
      pszLine = strLine.GetBuffer (0);
      bResult = ConvertPar( pszLine+2, lValues, 4);
      if (!bResult)
      {
         bResult = ConvertPar( pszLine+2, lValues, 2);
         lValues[2] = 0; 
         lValues[3] = 0;
      }
      strLine.ReleaseBuffer ();
      if( !bResult )
      {
		   return (E_FAIL);
      };
      position = CStdEditorPosition(EDP_STLINE, lValues[0], lValues[1] );
      endPos   = CPoint(lValues[2], lValues[3]);
   }
   else if( strLine == "begin" )
   {
      strType = strLine;
   }
   else if( strLine == "end" )
   {
      strType = strLine;
   }
   else 
   {
      pszLine = strLine.GetBuffer (0);
      bResult = ConvertPar( pszLine, lValues, 2);
      strLine.ReleaseBuffer ();

      if(!bResult)
      {
         return (E_FAIL);
      }
      strType = _T("");
      position = CStdEditorPosition(EDP_STLINE, 0, 0);
      endPos = CPoint(lValues[0], lValues[1]);      
   }

   return (S_OK);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress)
{
   CPrgEditorDoc::SetInstance (strInstanceName, strMachineName, strMachineAddress);

   if (strInstanceName.IsEmpty())
   {
      SetEditMode(EM_NORMAL);
   }
   else
   {
      SetEditMode(EM_MONITOR);
      // remove emphasizes ..
      m_RegionError.SetRectEmpty();
      m_Elem.ClearFlags(EF_EMPHASIZE);
      // clear all selections
      m_Elem.UnselectAll();
   }
   UpdateAllViews(NULL);
   UpdateAllViews(NULL, CGrEditorDoc::UpdateStatusHint);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::StartLDTracing (void)
{
   UpdateAllViews(NULL, CGrEditorDoc::StartLDTracingHint);

   return (S_OK);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::StopLDTracing (void)
{
   UpdateAllViews(NULL, CGrEditorDoc::StopLDTracingHint);

   return (S_OK);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnShowBreakpoint (CBreakpoint *pBP)
{
	ASSERT (pBP != NULL);
	if (pBP == NULL)
	{
		return;
	};

	CPrgEditorDoc::OnShowBreakpoint (pBP);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnHideBreakpoint (CBreakpoint *pBP)
{
	ASSERT (pBP != NULL);
	if (pBP == NULL)
	{
		return;
	}

	CPrgEditorDoc::OnHideBreakpoint (pBP);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnShowStoppoint ()
{
	CPrgEditorDoc::OnShowStoppoint ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnHideStoppoint ()
{
	CPrgEditorDoc::OnHideStoppoint ();
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc diagnostics

#ifdef _DEBUG
void CGrEditorDoc::AssertValid() const
{
	CPrgEditorDoc::AssertValid();
}

void CGrEditorDoc::Dump(CDumpContext& dc) const
{
	CPrgEditorDoc::Dump(dc);
}
#endif //_DEBUG

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnViewGrid() 
{
   CGridDlg dlg;
   // Initialize dialog data
   dlg.m_iGrid = (int)m_eGrid;
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {   // retrieve the dialog data
       m_eGrid = (GRID_TYPE) dlg.m_iGrid;
       UpdateAllViews(NULL);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnOptLinerubb() 
{
   if( m_eLineMode == LDM_ORTHO )
   {
      m_eLineMode	= LDM_RUBB;
   } else {
      m_eLineMode	= LDM_ORTHO;
   }

   CString strKey, strSec;

   strSec.LoadString(IDS_GR_REG_SECTION);
   strKey.LoadString(IDS_GR_REG_RUBBLINE);
   AfxGetApp()->WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 
      (m_eLineMode == LDM_ORTHO) ? 0 : 1);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnUpdateOptLinerubb(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable  ( m_eEditMode==EM_NORMAL     ||
                     m_eEditMode==EM_LINE_START || 
                     m_eEditMode==EM_LINE  );
	pCmdUI->SetCheck( m_eLineMode==LDM_RUBB );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnUpdateZoom(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable (m_eEditMode != EM_EDITPROP);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SetColor(bool bScreen, COLOR_TYPE ColorType)
{
   ASSERT((ColorType >= 0) || (ColorType < CO_LAST));

   CColorDialog dlg( GDIHelper::GetColor(bScreen, ColorType), CC_RGBINIT );
   COLORREF  cols[16];
   int       i;

   for( i=0; i<min(16, CO_LAST); i++) cols[i] = GDIHelper::GetColor(bScreen, (COLOR_TYPE)i);
   for(    ; i<16;               i++) cols[i] = RGB(255,255,255);
   dlg.m_cc.lpCustColors = cols;
   if( dlg.DoModal() == IDOK )
   {
      GDIHelper::SetColor(bScreen, ColorType, dlg.GetColor());

      UpdateAllViews(NULL, CGrEditorDoc::UpdateColorHint);
   }
}

void CGrEditorDoc::OnOptColnormal()    { SetColor(1, CO_NORMAL    ); }
void CGrEditorDoc::OnOptColselect()    { SetColor(1, CO_SELECT    ); }
void CGrEditorDoc::OnOptColerror()     { SetColor(1, CO_ERROR     ); }
void CGrEditorDoc::OnOptColcomment()   { SetColor(1, CO_COMMENT   ); }
void CGrEditorDoc::OnOptColrubber()    { SetColor(1, CO_RUBBER    ); }
void CGrEditorDoc::OnOptColeditline()  { SetColor(1, CO_EDITLINE  ); }
void CGrEditorDoc::OnOptColgrid()      { SetColor(1, CO_GRID      ); }
void CGrEditorDoc::OnOptColsheet()     { SetColor(1, CO_BACKGD    ); }
void CGrEditorDoc::OnOptColelembk()    { SetColor(1, CO_ELEMBK    ); }
void CGrEditorDoc::OnOptColladdbk()    { SetColor(1, CO_LADDBK    ); }
void CGrEditorDoc::OnOptColfblkbk()    { SetColor(1, CO_FBLKBK    ); }
void CGrEditorDoc::OnOptColedge()      { SetColor(1, CO_EDGE      ); }
void CGrEditorDoc::OnOptCollightpin()  { SetColor(1, CO_EDGE      ); }
void CGrEditorDoc::OnOptColbrkrec()    { SetColor(1, CO_BRKREC    ); }
void CGrEditorDoc::OnOptColbrkdis()    { SetColor(1, CO_BRKDIS    ); }
void CGrEditorDoc::OnOptColbrkact()    { SetColor(1, CO_BRKACT    ); }
void CGrEditorDoc::OnOptColbrkdec()    { SetColor(1, CO_BRKNOC    ); }
void CGrEditorDoc::OnOptColbackgd_sfc(){ SetColor(1, CO_BACKGD_SFC); }
void CGrEditorDoc::OnOptColelembk_sfc(){ SetColor(1, CO_ELEMBK_SFC); }
void CGrEditorDoc::OnOptColladdbk_sfc(){ SetColor(1, CO_LADDBK_SFC); }
void CGrEditorDoc::OnOptColfblkbk_sfc(){ SetColor(1, CO_FBLKBK_SFC); }
void CGrEditorDoc::OnOptColactive_sfc(){ SetColor(1, CO_ACTIVE_SFC); }
void CGrEditorDoc::OnOptColforced_sfc(){ SetColor(1, CO_FORCED_SFC); }
void CGrEditorDoc::OnOptColblockd_sfc(){ SetColor(1, CO_BLOCKD_SFC); }

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnOptFont() 
{
   CClientDC dc(NULL);
   LOGFONT   stLF;

   memset( &stLF, 0, sizeof(stLF));
   strcpy( stLF.lfFaceName, m_strFont);
   stLF.lfHeight = -::MulDiv( -m_iFontHeight, dc.GetDeviceCaps(LOGPIXELSY), 72);
   CFontDialog dlg(&stLF);
   if( dlg.DoModal() == IDOK )
   {
      m_strFont     = stLF.lfFaceName;
      m_iFontHeight = ::MulDiv( -stLF.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
      m_iGrid       = (int)(m_iFontHeight * 100.0 / (float)FontToGrid / 2.0);
      SetMarginRect(TRUE);
      ComputePageSize();
      UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);

//    UpdateAllItems(NULL);
      UpdateAllViews(NULL);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
int CGrEditorDoc::GetGridSize()
{
   return (int)(m_iGrid * m_fZoom);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SetEditMode(EDI_MODE eNewMode)
{
    if (m_eEditMode == EM_EDITPROP && 
        eNewMode    != EM_EDITPROP)
    {
        UpdateAllViews(NULL, CGrEditorDoc::EditNameStopHint);
    }
    
    m_eEditMode = eNewMode;
    
    UpdateAllViews(NULL, CGrEditorDoc::UpdateStatusHint);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SetSfcMode(bool eNewMode)
{
   m_bEnableSFC = eNewMode;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SetCurrLang(EDIT_LANG eCurrLang)
{
   m_eCurrLang = eCurrLang;
   switch (m_eCurrLang)
   {
   case EL_FBD    : m_Elem.SetIECLanguage(LANG_FBD); break;
   case EL_LD     : m_Elem.SetIECLanguage(LANG_LD);  break;
   case EL_ALL    : m_Elem.SetIECLanguage(LANG_SFC); break;

   default        : m_Elem.RecoverIECLang();         break;  // try to detect
   }

//	CInPlaceFrame* pFrame = dynamic_cast <CInPlaceFrame*> m_pInPlaceFrame;
	CInPlaceFrame* pFrame = (CInPlaceFrame*) m_pInPlaceFrame;

   if (IsInPlaceActive () && pFrame)
   {
      pFrame->ViewToolbarFbd(true);

      switch (m_eCurrLang)
      {
      case EL_FBD:      
         pFrame->ViewToolbarSfc(false);
         pFrame->ViewToolbarLd (false);
         break;

      case EL_LD:       
         pFrame->ViewToolbarSfc(false);
         pFrame->ViewToolbarLd (true);
         break;

      case EL_ALL:
         pFrame->ViewToolbarSfc(true);
         pFrame->ViewToolbarLd (true);
         break;
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnOptRecon() 
{
   m_bRecon = !m_bRecon;

   CString strKey, strSec;
   strSec.LoadString(IDS_GR_REG_SECTION);
   strKey.LoadString(IDS_GR_REG_RUBB_RECON);
   AfxGetApp()->WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, m_bRecon ? 1 : 0);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnUpdateOptRecon(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable  ( m_eEditMode==EM_NORMAL);
	pCmdUI->SetCheck( m_bRecon );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
float CGrEditorDoc::SetZoom(float fZoom)
{
   float fOldZoom;

   fOldZoom = m_fZoom;
   m_fZoom = fZoom;

   return fOldZoom;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight)
{
   HRESULT hr;

   if( (lWidth  <= 0) || (lWidth  >= DEF_MAX_WIDHT) ||
       (lHeight <= 0) || (lHeight >= DEF_MAX_HEIGHT))
      return( S_FALSE );

   hr = CStdEditorDoc::SetPrintInfo (strPrinterName, lWidth, lHeight);

   if( hr == S_OK ) 
   {
      SetMarginRect(FALSE);
      hr = ComputePageSize();
      UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);
   }

   return (hr);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::AttachPrinterDC (CDC *pDC)
{
  // prepare printing
   CPrintDialog printDlg(FALSE);  // Instantiate a CPrintDialog object.

   // Retrieve the current printer defaults from the Windows .ini file
   // as necessary, or uses the last printer configuration set by the
   // user in Print Setup.
   CGrEditorApp* pApp = dynamic_cast<CGrEditorApp *>(AfxGetApp());
   ASSERT(pApp);
   ASSERT(pDC);
   if ( !pApp || !pDC)
   {
      return (E_FAIL);
   }

   // select printer
   pApp->DevModeChange ( (LPTSTR)(LPCTSTR)GetPrinterName () );

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
   return S_OK;
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::GetPageCount (long &lCount)
{
   // Calculate max page
   CRect  r;
   CPoint p = GetPageGrid();
   m_Elem.GetAllElemRect(r);    // in grid
   
   lCount = ((r.right + p.x - 1) / p.x) * ((r.bottom + p.y - 1) / p.y);
   if(lCount == 0)
   {
       lCount = 1;
   }
   
   m_bPrintSTHeader = (AfxGetApp()->GetProfileInt("PRINT", "HEADER", 1) == 0) ? false : true;
   if(m_bPrintSTHeader)
   {
       long lCount_ST;
       CDC dcPrint;
       CSECEditor* pEdit;
   
       if (AttachPrinterDC(&dcPrint) == S_OK)
       {
          pEdit = GetEditor(0);
          if (pEdit)
          {
             GetPageCount_ST(*pEdit, dcPrint, lCount_ST);
             lCount += lCount_ST;
          } 
       }
   }
   
   return (S_OK);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::GetPages (CStringArray &pages)
{
   TCHAR    szTempName[_MAX_PATH];
   TCHAR    szPath[_MAX_PATH];
   long     lCount;
   int      i;

#if 0
   // prepare printing
   CPrintDialog printDlg(FALSE);  // Instantiate a CPrintDialog object.

   // Retrieve the current printer defaults from the Windows .ini file
   // as necessary, or uses the last printer configuration set by the
   // user in Print Setup.
   CGrEditorApp* pApp = dynamic_cast<CGrEditorApp *>(AfxGetApp());
   ASSERT(pApp);
   if (!pApp)
   {
      return (E_FAIL);
   }

   // select printer
   pApp->DevModeChange ( (LPTSTR)(LPCTSTR)GetPrinterName () );

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
   CDC dcPrint; 
   dcPrint.Attach (printDlg.GetPrinterDC ()); 
   dcPrint.m_bPrinting = TRUE;
#else
   CDC dcPrint;
   if(AttachPrinterDC(&dcPrint) != S_OK)
   {
       return E_FAIL;
   }
#endif
   dcPrint.m_bPrinting = TRUE;

   // Clear zoom factor
   float fOldZoom = SetZoom(1.0);

   SetMarginRect(FALSE);
   ComputePageSize();
   UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);

   m_bPrintSTHeader = (AfxGetApp()->GetProfileInt("PRINT", "HEADER", 1) == 0) ? false : true;
   if(m_bPrintSTHeader)
   {
       CSECEditor *pEdit = GetEditor(0);
       if (pEdit)
       {
          if (GetPages_ST(*pEdit, dcPrint, pages) != S_OK)
          {
             return E_FAIL;
          }
       } else {
          ASSERT(0);
       }
   }

   // printing body
   CPoint p = GetPageGrid();
   CRect  r;
   m_Elem.GetAllElemRect(r);        // in grid
   // Calculate max page
   lCount = ((r.right + p.x - 1) / p.x) * ((r.bottom + p.y - 1) / p.y);
   if(lCount == 0)
   {
       lCount = 1;
   }
   
   int iPageCol = max(((r.right  + p.x - 1) / p.x), 1);
   CRect rect;
   rect.SetRect( 0, 0, GetPageWidth (), GetPageHeight () );

   for ( i=0; i<lCount; i++)
   {
      VERIFY(GetTempPath(sizeof(szPath), szPath) != 0);
      VERIFY(GetTempFileName(szPath, _T("pgr"), 0, szTempName) != 0);
      pages.Add( szTempName );

      CString strDesc;
      CString strAppl;

      strAppl = GetPathName();
      LPTSTR pszAppl = strAppl.GetBuffer(0);
      strDesc.Format("GrEditor# %s Page %d##", pszAppl, i+1);
      strDesc.Replace(_T('#'), _T('\0'));

      HDC hdcMeta = CreateEnhMetaFile( dcPrint, szTempName, &rect, strDesc);
      if (!hdcMeta) 
      {
         return (E_FAIL);
      }

      CDC dcMeta;
      dcMeta.Attach(hdcMeta);
      dcMeta.m_bPrinting = TRUE;

      POSITION pos = GetFirstViewPosition ();

      while (pos)
      {
         CView *pView = GetNextView(pos);
         if (pView->IsKindOf(RUNTIME_CLASS(CGrEditorView)))
         {
            CGrEditorView *pGrView = dynamic_cast<CGrEditorView *> (pView);
            if (pGrView)
            {
               dcMeta.SetMapMode (MM_ANISOTROPIC);
               dcMeta.SetWindowExt (GetPageSize().cx, GetPageSize().cy);
               dcMeta.SetViewportExt (
                  dcMeta.GetDeviceCaps(HORZRES) * (GetPageWidth () / 100) / dcMeta.GetDeviceCaps(HORZSIZE) , 
                  dcMeta.GetDeviceCaps(VERTRES) * (GetPageHeight() / 100) / dcMeta.GetDeviceCaps(VERTSIZE) );

               // pagination
               // set window origins for desired page (source)
               // n columns x m rows
               //    1    2    3 ...  n
               //  n+1  n+2  n+3 ... 2n
               //  ...  ...  ...     ..
               //  ...  ...  ... ... mn
               int xo = GetPageSize().cx * (i % iPageCol);
               int yo = GetPageSize().cy * (i / iPageCol);
               dcMeta.SetWindowOrg(xo - GetPageSize().cx/2, yo - GetPageSize().cy/2);
               dcMeta.SetViewportOrg(-dcMeta.GetDeviceCaps(HORZRES) * (GetPageWidth () / 100) / dcMeta.GetDeviceCaps(HORZSIZE) / 2,
                                     -dcMeta.GetDeviceCaps(VERTRES) * (GetPageHeight() / 100) / dcMeta.GetDeviceCaps(VERTSIZE) / 2);

               // Create and select the clipping region.
               CRgn rgn;
               r.SetRect (xo, yo, xo + GetPageSize().cx, yo + GetPageSize().cy);
               dcMeta.LPtoDP(&r);
               rgn.CreateRectRgnIndirect (&r); 
               dcMeta.SelectClipRgn (&rgn, RGN_COPY);   
               // draw the page ..
               pGrView->m_DrawTarget = CDbgEditorView::InactiveView;
               pGrView->DrawInto( &dcMeta, GetPageSize());
               pGrView->m_DrawTarget = CDbgEditorView::ActiveView;
            }
         }
      }

      dcMeta.Detach();

      DeleteEnhMetaFile (CloseEnhMetaFile(hdcMeta));
   }

   SetZoom(fOldZoom);               // Restore zoom factore
   SetMarginRect(FALSE);
   ComputePageSize();
   UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);

   return (S_OK);
}


long CGrEditorDoc::GetPrintLinesPerPage_ST(CDC * pDCPrint)
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


HRESULT CGrEditorDoc::GetPageCount_ST(CSECEditor& Edit, CDC &dcPrint, long &lCount)
{
    long lLinesPerPage = GetPrintLinesPerPage_ST(&dcPrint);

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
    for (LINEDESC * pLine = Edit.m_pLine; NULL != pLine; pLine = pLine->pNext)
    {
        bool bSuccess;
        CString strText;

        bSuccess = Edit.ExpandLine (pLine, strText);
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


HRESULT CGrEditorDoc::GetPages_ST(CSECEditor& Edit, CDC &dcPrint, CStringArray &pages)
{
    TCHAR    szTempName[_MAX_PATH];
    TCHAR    szPath[_MAX_PATH];
    long lLinesPerPage;
    long lLineNum = 0;
    TEXTMETRIC tm;
    CFont Font;
    CFont * pOldFont;
    long lCharsPerLine = 80;

    CRect rect;
    rect.SetRect( 0, 0, GetPageWidth (), GetPageHeight () );

    // printing

    lLinesPerPage = GetPrintLinesPerPage_ST(&dcPrint);

    HDC hdcMeta = NULL;
    CDC dcMeta;

    for (LINEDESC * pLine = Edit.m_pLine; NULL != pLine; pLine = pLine->pNext)
    {
        bool bSuccess;
        CString strText;

        bSuccess = Edit.ExpandLine (pLine, strText);
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
                strAppl = GetPathName();
                strDesc.Format("STEditor# %s Page %d##", strAppl, pLine->iLineNo);
                strDesc.Replace(_T('#'), _T('\0'));

                hdcMeta = CreateEnhMetaFile( dcPrint, szTempName, &rect, strDesc);
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

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SetMarginRect(BOOL bReduce)
{
   // calculate size in 0.01 inch
   CSize size;
   size.cx = MulDiv(m_lPageWidth,  10, 254);
	size.cy = MulDiv(m_lPageHeight, 10, 254);

   if( !bReduce ) 
   {  // call from IF for embedded print
      if (size.cx < m_iMargLeft + m_iMargRight) 
      {
         size.cx = 4*m_iGrid;
      }
      if (size.cy < m_iMargTop + m_iMargBottom)
      {
         size.cy = 4*m_iGrid;
      }
      m_Margin0 = CRect(0, 0, size.cx, size.cy);
   } 
   else
   {  // call for stand alone print
      if (size.cx < m_iMargLeft + m_iMargRight) 
      {
         size.cx = m_iMargLeft + m_iMargRight + 4*m_iGrid;
      }
      if (size.cy < m_iMargTop + m_iMargBottom)
      {
         size.cy = m_iMargTop + m_iMargBottom + 4*m_iGrid;
      }
      m_Margin0 = CRect(m_iMargLeft, m_iMargTop,
                       size.cx - m_iMargRight, size.cy - m_iMargBottom);
   }

   // Device metrics for screen
   HDC hDCScreen = GetDC(NULL);
   ASSERT(hDCScreen != NULL);

   // store margin in logical units
   m_Margin0.left   = (int)((float)m_Margin0.left   *
                            (float)GetDeviceCaps(hDCScreen, LOGPIXELSX) / 100.0);
   m_Margin0.top    = (int)((float)m_Margin0.top    *
                            (float)GetDeviceCaps(hDCScreen, LOGPIXELSY) / 100.0);
   m_Margin0.right  = (int)((float)m_Margin0.right  *
                            (float)GetDeviceCaps(hDCScreen, LOGPIXELSX) / 100.0);
   m_Margin0.bottom = (int)((float)m_Margin0.bottom *
                            (float)GetDeviceCaps(hDCScreen, LOGPIXELSY) / 100.0);
   ReleaseDC(NULL, hDCScreen);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::InitPageSize(void)
{
   // default: letter (if dlg failed)
   m_lPageWidth  = MulDiv(DEF_PAGE_WIDTH - (DEF_MARG_LEFT   - DEF_NP_MARG_LEFT  ) -
                                           (DEF_MARG_RIGHT  - DEF_NP_MARG_RIGHT ),
                          254, 10);
   m_lPageHeight = MulDiv(DEF_PAGE_HEIGHT- (DEF_MARG_TOP    - DEF_NP_MARG_TOP   ) -
                                           (DEF_MARG_BOTTOM - DEF_NP_MARG_BOTTOM),
                          254, 10);

   CPrintDialog dlg(FALSE);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{  // GetPrinterDC returns a HDC so attach it
		CDC dc;
		HDC hDC=dlg.CreatePrinterDC();
		ASSERT(hDC != NULL);
		dc.Attach(hDC);
      m_lPageWidth  = dc.GetDeviceCaps(HORZSIZE) * 100;
      m_lPageHeight = dc.GetDeviceCaps(VERTSIZE) * 100;
   } 
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::InitPageSize(CDC* pDC)
{
   // default: letter (if dlg failed)
   m_lPageWidth  = MulDiv(DEF_PAGE_WIDTH - (DEF_MARG_LEFT   - DEF_NP_MARG_LEFT  ) -
                                           (DEF_MARG_RIGHT  - DEF_NP_MARG_RIGHT ),
                          254, 10);
   m_lPageHeight = MulDiv(DEF_PAGE_HEIGHT- (DEF_MARG_TOP    - DEF_NP_MARG_TOP   ) -
                                           (DEF_MARG_BOTTOM - DEF_NP_MARG_BOTTOM),
                          254, 10);

	ASSERT(pDC != NULL);
   if (pDC)
   {
      m_lPageWidth  = pDC->GetDeviceCaps(HORZSIZE) * 100;
      m_lPageHeight = pDC->GetDeviceCaps(VERTSIZE) * 100;
   }
}

/*func*------------------------------------------------------------------------
  ComputePageSize: compute page and doc size in logical units and grids
                   if values are rounded, correct logical margins
  in :
  out:hr = S_OK       - compute successful
      hr = S_FALSE    - error, used internal zoom in and zoom out
                        page size contains wrong values
-----------------------------------------------------------------------------*/
HRESULT CGrEditorDoc::ComputePageSize()
{
   // Device metrics for screen
   HDC hDCScreen = GetDC(NULL);
   ASSERT(hDCScreen != NULL);
   int iFlag = 0;                      // for error check

   // printable page size in logical units
   CSize size = m_Margin0.Size();
   if( (size.cx <= 0) || (size.cy <= 0) )
      return( S_FALSE );

   // compute grids at one page
   m_PageGrid.cx = (int)( (float)size.cx / (float)m_iGrid );
	m_PageGrid.cy = (int)( (float)size.cy / (float)m_iGrid );
   m_PageGrid.cx &= 0xFFFFFFFE;  // round for even number of grid on page
   m_PageGrid.cy &= 0xFFFFFFFE;
   // compute grids at all pages, compute doc size in 100 * inch
   m_DocGrid.cx = ((MAX_XGRID + m_PageGrid.cx-1) / m_PageGrid.cx) * m_PageGrid.cx;
	m_DocGrid.cy = ((MAX_YGRID + m_PageGrid.cy-1) / m_PageGrid.cy) * m_PageGrid.cy;

   do {
      do {
         // round page to grid
         m_PageSize.cx = m_PageGrid.cx * GetGridSize();
         m_PageSize.cy = m_PageGrid.cy * GetGridSize();

         if ((m_PageSize.cx == 0) || (m_PageSize.cy == 0))
         {
            m_fZoom *= 2.0;
            iFlag |= 0x01;             // store zoom in
         }
      } while ((m_PageSize.cx == 0) || (m_PageSize.cy == 0));

      // compute doc size in 100 * inch
      m_DocSize.cx = m_DocGrid.cx * GetGridSize();
      m_DocSize.cy = m_DocGrid.cy * GetGridSize();

      if((m_DocSize.cx < 0) || (m_DocSize.cx >= 0x8000) || 
         (m_DocSize.cy < 0) || (m_DocSize.cy >= 0x8000))
      {
         m_fZoom /= 2.0;
         iFlag |= 0x02;                // store zoom out
      }
   } while ((iFlag != 0x03) &&
            ((m_DocSize.cx < 0) || (m_DocSize.cx >= 0x8000) || 
             (m_DocSize.cy < 0) || (m_DocSize.cy >= 0x8000)));

   // if values rounded, correct margins
   if (((m_PageGrid.cx * GetGridSize()) < size.cx) ||
       ((m_PageGrid.cy * GetGridSize()) < size.cy))
   {
      m_Margin.top    = m_Margin0.top  + (size.cx - m_PageSize.cx) / 2;
      m_Margin.left   = m_Margin0.left + (size.cy - m_PageSize.cy) / 2;
      m_Margin.right  = m_Margin0.left + m_PageSize.cx;
      m_Margin.bottom = m_Margin0.top  + m_PageSize.cy;
   } else {
      m_Margin = m_Margin0;
   }

   ReleaseDC(NULL, hDCScreen);

   return( (iFlag == 0x03) ? S_FALSE : S_OK );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorDoc::PreparePrintDC(CDC* pDC, int iPageNr)
{
   // Clear zoom factor
   float fOldZoom = SetZoom(1.0);
   ComputePageSize();

   // Device metrics for screen
   HDC hDCScreen = ::GetDC(NULL);
   ASSERT(hDCScreen != NULL);
   int cxScreen = GetDeviceCaps(hDCScreen, LOGPIXELSX);
   int cyScreen = GetDeviceCaps(hDCScreen, LOGPIXELSY);

   // Device metrics for given printer
   int cxDevice = pDC->GetDeviceCaps(LOGPIXELSX);
   int cyDevice = pDC->GetDeviceCaps(LOGPIXELSY);

   pDC->SetMapMode(MM_ANISOTROPIC);
   pDC->SetWindowExt  (cxScreen, cyScreen);
   pDC->SetViewportExt(cxDevice, cyDevice);

   // pagination
   // set window origins for desired page (source)
   // n columns x m rows
   //    1    2    3 ...  n
   //  n+1  n+2  n+3 ... 2n
   //  ...  ...  ...     ..
   //  ...  ...  ... ... mn
   CRect  r;
   CPoint p = GetPageGrid();
   m_Elem.GetAllElemRect(r);        // in grid
   int iPageCol = max(((r.right  + p.x - 1) / p.x), 1);
   pDC->SetWindowOrg(GetPageSize().cx * (iPageNr % iPageCol),
                     GetPageSize().cy * (iPageNr / iPageCol));
   pDC->SetViewportOrg(0,0);

   // Restore zoom factore
   SetZoom(fOldZoom);
   ComputePageSize();
}


// read the text from all ST-views back into the elements/header
void CGrEditorDoc::GetSTViewText()
{
   POSITION pos = GetFirstViewPosition();
   while (pos != NULL)
   {
      CSTEditorView* pView = dynamic_cast<CSTEditorView*>( GetNextView(pos) );
      if(pView)
      {
         if (pView->GetEditID() == 0) // Header view
         {
            m_Header.Empty();
            pView->GetWindowText(m_Header);
            break;
         }
      }
   }
}


void CGrEditorDoc::SetHeaderViewText()
{
   POSITION pos = GetFirstViewPosition();
   while (pos != NULL)
   {
      CSTEditorView* pView = dynamic_cast<CSTEditorView*>(GetNextView(pos));
      if (pView && pView->GetEditID() == 0)
      {
         if(pView->m_pCurLine)
         {
            pView->SetWindowText(m_Header);
         }else
         {
            pView->SetDelayedText(m_Header);
         }
         
         // set the CSTEditor features allowed
         pView->m_Features.Clr(CSTEditorView::STF_DEBUGGING);   // -
         pView->m_Features.Set(CSTEditorView::STF_MONITORING);  // +
         pView->m_Features.Clr(CSTEditorView::STF_USERHELP);    // -
         pView->m_Features.Clr(CSTEditorView::STF_POU_INS);     // -
         pView->m_Features.Set(CSTEditorView::STF_DECL_INS);    // +
         pView->m_Features.Clr(CSTEditorView::STF_STMT_INS);    // -
         pView->m_Features.Set(CSTEditorView::STF_SCROLLBAR);   // +
         
         pView->SetCaretPos(CPoint(0,0));
         pView->ClearSelection(true);
         
         CSECEditor *pEdit;
         pEdit = GetEditor(0);
         if (pEdit)
         {
            pEdit->FreeUndoBuffers ();
            pEdit->SetModified(FALSE);
            pEdit->SetReadOnly( CheckReadonly() );
         } else {
            ASSERT(0);
         }
         // we have only one header ..
         break;
      }
   }
}


/////////////////////////////////////////////////////////////////////////////
// CGrEditorDoc commands



BOOL CGrEditorDoc::SaveModified() 
{
	OnUpdateModified ();
	return (CPrgEditorDoc::SaveModified ());
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorDoc::IsModeNormal ()
{
	return (GetEditMode() == EM_NORMAL);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorDoc::IsModeMonitor ()
{
	return (GetEditMode() == EM_MONITOR);
}

/*func*------------------------------------------------------------------------
  update statusbar
  in :strMsg - message for display in statusbar (if access enable)
  out:-
-----------------------------------------------------------------------------*/
void CGrEditorDoc::SystemMessage (HRESULT hr)
{
	CString strMessage;

	MessageBeep (0xFFFFFFFF);

	PrintSystemMessage (E_FACILITY_CEGR, hr);

	if (GetMessageText (hr, strMessage))
	{
		int iPos;

		iPos = strMessage.Find (':');
		if (iPos >= 0)
		{
			iPos = strMessage.Find (':', iPos + 1);
		};
		if (iPos >= 0)
		{
			strMessage = strMessage.Mid (iPos + 1);
			strMessage.TrimLeft ();
		};

		AfxMessageBox (strMessage);
	};
}

/*func*------------------------------------------------------------------------
  display document statistics
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CGrEditorDoc::OnAppStatistic()
{
	CStatDlg  dlg;

   m_Elem.GetStatisticData(dlg.data);

	dlg.DoModal();
}


void CGrEditorDoc::ToggleFixPoint(CPoint P)
{
   UpdateAllViews(NULL, UpdateFixPointsHint);
   if(m_Elem.ToggleFixPoint(P))
   {
      UpdateAllViews(NULL, UpdateFixPointsHint);
   }
}


COleIPFrameWnd* CGrEditorDoc::CreateInPlaceFrame(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
    
    ASSERT_VALID(this);
    ASSERT_VALID(pParentWnd);
    
    // Get run-time class from the doc template.
    CDocTemplate* pTemplate = GetDocTemplate();
    ASSERT_VALID(pTemplate);
    
    // Use existing view if possible.
    CView* pView = NULL;
    CFrameWnd* pFrame = GetFirstFrame();
    if (pFrame != NULL)
    {
        pView = (CView*)pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
        if (pView != NULL)
        {
            ASSERT_KINDOF(CView, pView);
            m_dwOrigStyle = pView->GetStyle();
            m_dwOrigStyleEx = pView->GetExStyle();
        }
    }
    
    // Create the frame from the template, ALWAYS use the CView of the
    // CSplitterWnd.
    COleIPFrameWnd* pFrameWnd = (COleIPFrameWnd*) pTemplate->CreateOleFrame(pParentWnd, this, FALSE);
    if (pFrameWnd == NULL)
        return NULL;
    
    // Connect the splitter window (CSplitterWnd class) to the
    // COleDocIPFrameWnd.
    CWnd* split = pFrame->GetWindow(GW_CHILD);
    VERIFY(pFrame == split->SetParent(pFrameWnd));
    
    // Remember the original parent window for deactivate--uses the
    // CFrameWnd-derived class (that is, CMDIChildWnd for MDI).
    m_pOrigParent = pFrame;
    
    // Set the active view of COleDocIPFrameWnd.
    pFrameWnd->SetActiveView(pView, FALSE);
    pFrameWnd->RecalcLayout();
    pView->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);

    // force activation of the GR-View
    POSITION pos = GetFirstViewPosition();
    while(pos)
    {
       CView *pView = GetNextView(pos);
       if(dynamic_cast<CGrEditorView*>(pView))
       {
          pFrameWnd->SetActiveView(pView);
          pFrameWnd->RecalcLayout();
       }
    }
    
    // Verify the type.
    ASSERT_VALID(pFrameWnd);
    ASSERT_KINDOF(COleIPFrameWnd, pFrameWnd);
    return pFrameWnd;
    //return COleServerDoc::CreateInPlaceFrame(pParentWnd);
}


void CGrEditorDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd) 
{
    // TODO: Add your specialized code here and/or call the base class
    
    ASSERT_VALID(this);
    ASSERT_VALID(pFrameWnd);
    
    // connect view to original, if existing view was used
    if (m_pOrigParent != NULL)
    {
        CView* pView = (CView*)pFrameWnd->GetDescendantWindow(
            AFX_IDW_PANE_FIRST, TRUE);
        ASSERT_VALID(pView);
        
        // Leaving the focus on an MDI child or one of its child
        // windows causes Windows to get confused when the child window
        // is destroyed, not to mention the fact that the focus will be
        // out of sync with activation.
        if (::GetFocus() == pView->m_hWnd)
        {
            // Move focus to somewhere safe.
            HWND hWnd = ::GetParent(pFrameWnd->m_hWnd);
            if (hWnd != NULL)
                ::SetFocus(hWnd);
            
            // Check again.
            if (::GetFocus() == pView->m_hWnd)
                SetFocus(NULL); // last ditch effort
        }
        
        // Set parent of the splitter window (CSplitterWnd class) to be
        // the CFrameWnd-derived class (that is, CMDIChildWnd for MDI
        // application).
        ASSERT_KINDOF(CFrameWnd, m_pOrigParent);
        CFrameWnd* frame = (CFrameWnd*) m_pOrigParent;
        CWnd* split = pFrameWnd->GetWindow(GW_CHILD);
        VERIFY(pFrameWnd == split->SetParent(frame));
        
        // Set the active view of CFrameWnd-derived class (that is,
        // CMDIChildWnd for MDI application).
        frame->SetActiveView(pView, FALSE);
        frame->RecalcLayout();
        
        m_pOrigParent = NULL;
        
        // Remove any scrollbars added because of in-place activation.
        if ((m_dwOrigStyle & (WS_HSCROLL|WS_VSCROLL)) == 0 &&
            (pView->GetStyle() & (WS_HSCROLL|WS_VSCROLL)) != 0)
        {
            ::SetScrollRange(pView->m_hWnd, SB_HORZ, 0, 0, TRUE);
            ::SetScrollRange(pView->m_hWnd, SB_VERT, 0, 0, TRUE);
        }
        
        // Restore old 3D style.
        pView->ModifyStyleEx(0, m_dwOrigStyleEx & WS_EX_CLIENTEDGE,
            SWP_DRAWFRAME);
        
        // Force recalc layout on splitter window.
        CSplitterWnd* pSplitter = CView::GetParentSplitter(pView,
            TRUE);
        if (pSplitter != NULL)
            pSplitter->RecalcLayout();
    }
    
    // No active view or document during destroy.
    pFrameWnd->SetActiveView(NULL);
    
    // Destroy in-place frame window.
    pFrameWnd->DestroyWindow();
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
CString CGrEditorDoc::GetSelectedText()
{
    CString strText;
    POSITION       pPosition = GetFirstViewPosition ();
    CView *pView;

    while(pPosition)
    {
        pView = GetNextView (pPosition);
    }

    CSTEditorView*  pSTView = dynamic_cast<CSTEditorView*> (pView);
    if(pSTView)
    {
        strText = pSTView->GetSelection();
    }
    return strText;
}


//------------------------------------------------------------------*
/**
 * show print options dialog.
 *
 *  print settings are not changed.
 *
 * @param           rstrOptions: options string (input and output)
 * @return          S_OK: ok
 *                  S_FALSE: canceled by user
 * @exception       -
 * @see             -
*/
HRESULT CGrEditorDoc::ShowPrintOptionsDialog(CString& rstrOptions)
{
    CGrPrintOptionsDlg    tDlg;
    CString strValue;
    bool    bPrintSTHeader(true);
    if(ParsePrintOptions(rstrOptions, _T("STHeader"), strValue))
    {
        int iTmp = atoi(strValue);
        bPrintSTHeader = (iTmp == 0) ? false : true;
    }
    if (bPrintSTHeader)
    {
        tDlg.m_bHeader = TRUE;
    }
    else
    {
        tDlg.m_bHeader = FALSE;
    }

    if(tDlg.DoModal() == IDOK)
    {
        rstrOptions.Format("STHeader=%d", tDlg.m_bHeader);
        return S_OK;
    }
    return S_FALSE;
}


//------------------------------------------------------------------*
/**
 * get print options.
 *
 * @param           rstrOptions: options string
 * @return          S_OK: ok
 * @exception       -
 * @see             SetPrintOptions()
*/
HRESULT CGrEditorDoc::GetPrintOptions(CString& rstrOptions)
{
    rstrOptions.Format("STHeader=%d", m_bPrintSTHeader);
	return (S_OK);
}

//------------------------------------------------------------------*
/**
 * set print options.
 *
 * @param           crstrOptions: options string
 * @return          S_OK: ok
 * @exception       -
 * @see             GetPrintOptions()
*/
HRESULT CGrEditorDoc::SetPrintOptions(const CString& crstrOptions)
{
    CString strValue;
    if(ParsePrintOptions(crstrOptions, _T("STHeader"), strValue))
    {
        int iTmp = atoi(strValue);
        m_bPrintSTHeader = (iTmp == 0) ? false : true;
        AfxGetApp()->WriteProfileInt("PRINT", "HEADER", m_bPrintSTHeader);
    }
    return S_OK;
}



