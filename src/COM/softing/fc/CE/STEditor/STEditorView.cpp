

// STEditorView.cpp : implementation of the CSTEditorView class
//

#include "stdafx.h"
#include "fc_todebug\fc_assert.h"
#include "STEditor.h"

#include "STEditorDoc.h"
#include "STEditorView.h"
#include "utilif.h"
#include "cedb.h"
#include "keyrc.h"
#include "XML_Wrapper.h"
#include "CESTError.h"
#include "CEBPManError.h"
#include "EditorSettingsDlg.h"      // AUTOFMT 14.08.06 SIS



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_EXT_ST "ST"
#define FILE_EXT_SDT "SDT"
#define FILE_EXT_HDR "HDR"
#define FILE_EXT_GVL "GVL"

/////////////////////////////////////////////////////////////////////////////
// CGotoLineDlg dialog

class CGotoLineDlg : public CDialog
{
// Construction
public:
    CGotoLineDlg(UINT uiLineCount, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CGotoLineDlg)
    enum { IDD = IDD_GOTOLINE };
    UINT	m_uiLineNumber;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGotoLineDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    UINT m_uiLineCount;

    // Generated message map functions
    //{{AFX_MSG(CGotoLineDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


class CEditVarDlg : public CDialog
{
// Construction
public:
	CEditVarDlg (CPrgEditorDoc *pDoc, const CString &strNamePreset, const CString &strValuePreset, CWnd *pParent=NULL);

// Dialog Data
	//{{AFX_DATA(CEditVarDlg)
	enum { IDD = IDD_EDITVARIABLE };
	CEdit	m_VarNameCtrl;
	CComboBox	m_VarValueCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditVarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CPrgEditorDoc *m_pDoc;
    CString       m_strNamePreset;
    CString       m_strValuePreset;

	// Generated message map functions
	//{{AFX_MSG(CEditVarDlg)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokVarvalue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool Apply ();
};

CEditVarDlg::CEditVarDlg(CPrgEditorDoc *pDoc, const CString &strNamePreset, const CString &strValuePreset, CWnd *pParent)
: CDialog (CEditVarDlg::IDD, pParent),
  m_pDoc (pDoc),
  m_strNamePreset (strNamePreset),
  m_strValuePreset (strValuePreset)
{
	//{{AFX_DATA_INIT(CEditVarDlg)
	//}}AFX_DATA_INIT
}


void CEditVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVarDlg)
	DDX_Control(pDX, IDC_VARNAME, m_VarNameCtrl);
	DDX_Control(pDX, IDC_VARVALUE, m_VarValueCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditVarDlg, CDialog)
	//{{AFX_MSG_MAP(CEditVarDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_CBN_SELENDOK(IDC_VARVALUE, OnSelendokVarvalue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVarDlg message handlers

void CEditVarDlg::OnApply ()
{
	Apply ();
}


bool CEditVarDlg::Apply ()
{
    CString strVariableName;
    CString strVariableValue;
    CComVariant varValue;
    HRESULT hr;
    LONG lHandle;
    BL_IEC_TYP iecType;
    USES_CONVERSION;

    m_VarNameCtrl.GetWindowText (strVariableName);

    m_VarValueCtrl.GetWindowText (strVariableValue);

    if (strVariableName.IsEmpty ())
        return (true);

    hr = m_pDoc->AddWatchItem (strVariableName, lHandle);
    if (FAILED (hr))
    {
        CString str;
        str.LoadString(IDS_ERRMSG_WRONG_VAR_SYNTAX);
        AfxMessageBox (str);
        return (false);
    }
    ASSERT (lHandle != 0);
    if (lHandle == 0)
        return (false);

    if (m_pDoc->IsSimpleWatchItemType (lHandle, iecType))
    {
        if (iecType == BL_LREAL && !strVariableValue.IsEmpty()
            && strVariableValue.Find(_T('.'))<0)
            strVariableValue += _T(".0");
        hr = BL_StringToVariant (T2COLE (strVariableValue), iecType, NULL, BL_USE_IEC_FORMAT, &varValue);
        if (FAILED (hr)) 
        {
            CString str;
            str.LoadString(IDS_ERRMSG_WRONG_VALUE_SYNTAX);
            AfxMessageBox (str);
            return (false);
        }
    }
    else
    {
        varValue = strVariableValue;
    }

    hr = m_pDoc->ConnectWatchItem (lHandle);
    if (FAILED (hr))
    {
        m_pDoc->RemoveWatchItem (lHandle);
        CString str;
        str.LoadString(IDS_ERRMSG_FAILED_TO_WRITE_VAR);
        AfxMessageBox (str);
        return (false);
    }

    hr = m_pDoc->WriteWatchItem (lHandle, varValue);
    if (FAILED (hr))
    {
        m_pDoc->DisconnectWatchItem (lHandle);
        m_pDoc->RemoveWatchItem (lHandle);
        CString str;
        str.LoadString(IDS_ERRMSG_FAILED_TO_WRITE_VAR);
        AfxMessageBox (str);
        return (false);
    }

    m_pDoc->DisconnectWatchItem (lHandle);
    m_pDoc->RemoveWatchItem (lHandle);

    OnSelendokVarvalue ();
    return (true);
}


void CEditVarDlg::OnOK ()
{
	if (Apply ())
	{
		CDialog::OnOK ();
	}
}


BOOL CEditVarDlg::OnInitDialog ()
{
	if (!CDialog::OnInitDialog ())
	{
		return (FALSE);
	}

	m_VarNameCtrl.SetWindowText (m_strNamePreset);

	m_VarValueCtrl.AddString (m_strValuePreset);
	m_VarValueCtrl.SetWindowText (m_strValuePreset);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEditVarDlg::OnSelendokVarvalue ()
{
	CString strValue;

	m_VarValueCtrl.GetWindowText (strValue);

	if (m_VarValueCtrl.FindString (0, strValue) < 0)
	{
		m_VarValueCtrl.AddString (strValue);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSTEditorView

static const int WPHorizontalMargin=3;
static const int WPVerticalMargin=2;

static const int SUSPEND_RESUME_WATCHPOINTS = 10000;



IMPLEMENT_DYNCREATE(CSTEditorView, SECEditView )

BEGIN_MESSAGE_MAP(CSTEditorView, SECEditView)
	//{{AFX_MSG_MAP(CSTEditorView)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_GOTO_LINE, OnGotoLine)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_SCROLL_DOWN, OnEditScrollDown)
	ON_COMMAND(ID_EDIT_SCROLL_UP, OnEditScrollUp)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEdit)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xffff, OnToolTipNfy)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, SECEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, SECEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, SECEditView::OnFilePrintPreview)
	ON_COMMAND(IDM_SET_WATCHPOINT, OnSetWatchpoint)
	ON_UPDATE_COMMAND_UI(IDM_SET_WATCHPOINT, OnUpdateMonitor)
	ON_COMMAND(IDM_CLEAR_WATCHPOINT, OnClearWatchpoint)
	ON_UPDATE_COMMAND_UI(IDM_CLEAR_WATCHPOINT, OnUpdateMonitor)
	ON_COMMAND(IDM_CLEAR_ALL_WATCHPOINTS, OnClearAllWatchpoints)
	ON_UPDATE_COMMAND_UI(IDM_CLEAR_ALL_WATCHPOINTS, OnUpdateMonitor)
	ON_COMMAND(IDM_WRITE_WATCHPOINT, OnWriteWatchpoint)
	ON_UPDATE_COMMAND_UI(IDM_WRITE_WATCHPOINT, OnUpdateMonitor)
	ON_COMMAND(IDM_TOGGLE_BREAKPOINT, OnToggleBreakpoint)
	ON_UPDATE_COMMAND_UI(IDM_TOGGLE_BREAKPOINT, OnUpdateDebug)
	ON_COMMAND(IDM_REMOVE_ALL_BREAKPOINTS, OnRemoveAllBreakpoints)
	ON_UPDATE_COMMAND_UI(IDM_REMOVE_ALL_BREAKPOINTS, OnUpdateDebug)
	ON_COMMAND(IDM_TOGGLE_BREAKPOINT_ENABLED, OnToggleBreakpointEnabled)
	ON_UPDATE_COMMAND_UI(IDM_TOGGLE_BREAKPOINT_ENABLED, OnUpdateDebug)
	ON_COMMAND(IDM_INSERT_FB_CALL, OnInsertFBCall)
    ON_COMMAND(IDM_SHOW_VAR_SELECTION, OnShowVarSelection)
	ON_COMMAND(IDM_CONTINUE_TASK, OnContinueTask)
	ON_UPDATE_COMMAND_UI(IDM_CONTINUE_TASK, OnUpdateDebug)
    ON_COMMAND(IDM_STEP_INTO, OnStepInto)
	ON_UPDATE_COMMAND_UI(IDM_STEP_INTO, OnUpdateDebug)
    ON_COMMAND(IDM_STEP_OVER, OnStepOver)
	ON_UPDATE_COMMAND_UI(IDM_STEP_OVER, OnUpdateDebug)
    ON_COMMAND(IDM_STEP_OUT, OnStepOut)
	ON_UPDATE_COMMAND_UI(IDM_STEP_OUT, OnUpdateDebug)
    ON_COMMAND(IDM_INSERT_IF, OnInsertIF)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_IF, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_CASE, OnInsertCASE)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_CASE, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_FOR, OnInsertFOR)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_FOR, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_WHILE, OnInsertWHILE)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_WHILE, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_REPEAT, OnInsertREPEAT)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_REPEAT, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_ASSIGNMENT, OnInsertAssignment)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_ASSIGNMENT, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_RETURN, OnInsertRETURN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_RETURN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_EXIT, OnInsertEXIT)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_EXIT, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_VAR, OnInsertVAR)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_VAR, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_CONST, OnInsertCONST)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_CONST, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_VAR_GLOBAL, OnInsertVARGLOBAL)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_VAR_GLOBAL, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_VAR_EXTERNAL, OnInsertVAREXTERNAL)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_VAR_EXTERNAL, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_INITIALSTEP, OnInsertINITIALSTEP)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_INITIALSTEP, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_STEP, OnInsertSTEP)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_STEP, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_ACTION, OnInsertACTION)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_ACTION, OnUpdateEdit)
    ON_COMMAND(IDM_INSERT_TRANSITION, OnInsertTRANSITION)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_TRANSITION, OnUpdateEdit)
	ON_COMMAND(IDM_CONTEXT_MENU, OnContextMenuKey)
    ON_MESSAGE(SUSPEND_RESUME_WATCHPOINTS, OnSuspendResumeWatchpoints)
    ON_COMMAND(IDM_TOGGLE_MANUAL_MODE, OnToggleManualMode)
    ON_UPDATE_COMMAND_UI(IDM_TOGGLE_MANUAL_MODE, OnUpdateSFCMenu)
    ON_COMMAND(IDM_PERFORM_CYCLE, OnPerformCycle)
    ON_UPDATE_COMMAND_UI(IDM_PERFORM_CYCLE, OnUpdateSFCMenu)
    ON_COMMAND(IDM_FORCE_TRANSITION_ONCE, OnForceTransitionOnce)
    ON_UPDATE_COMMAND_UI(IDM_FORCE_TRANSITION_ONCE, OnUpdateSFCMenu)
    ON_COMMAND(IDM_FORCE_TRANSITION, OnForceTransition)
    ON_UPDATE_COMMAND_UI(IDM_FORCE_TRANSITION, OnUpdateSFCMenu)
    ON_COMMAND(IDM_BLOCK_TRANSITION, OnBlockTransition)
    ON_UPDATE_COMMAND_UI(IDM_BLOCK_TRANSITION, OnUpdateSFCMenu)
    ON_COMMAND(IDM_GLOBALLY_FORCE_ALL_TRANSITIONS, OnGloballyForceAllTransitions)
    ON_COMMAND(IDM_FORCE_ALL_TRANSITIONS, OnForceAllTransitions)
    ON_UPDATE_COMMAND_UI(IDM_FORCE_ALL_TRANSITIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_BLOCK_ALL_TRANSITIONS, OnBlockAllTransitions)
    ON_UPDATE_COMMAND_UI(IDM_BLOCK_ALL_TRANSITIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_REVERT_ALL_TRANSITIONS, OnRevertAllTransitions)
    ON_UPDATE_COMMAND_UI(IDM_REVERT_ALL_TRANSITIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_GLOBALLY_FORCE_ALL_TRANSITIONS, OnGloballyForceAllTransitions)
    ON_UPDATE_COMMAND_UI(IDM_GLOBALLY_FORCE_ALL_TRANSITIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_FORCE_ACTION_ONCE, OnForceActionOnce)
    ON_UPDATE_COMMAND_UI(IDM_FORCE_ACTION_ONCE, OnUpdateSFCMenu)
    ON_COMMAND(IDM_FORCE_ACTION, OnForceAction)
    ON_UPDATE_COMMAND_UI(IDM_FORCE_ACTION, OnUpdateSFCMenu)
    ON_COMMAND(IDM_BLOCK_ACTION, OnBlockAction)
    ON_UPDATE_COMMAND_UI(IDM_BLOCK_ACTION, OnUpdateSFCMenu)
    ON_COMMAND(IDM_FORCE_ALL_ACTIONS, OnForceAllActions)
    ON_UPDATE_COMMAND_UI(IDM_FORCE_ALL_ACTIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_BLOCK_ALL_ACTIONS, OnBlockAllActions)
    ON_UPDATE_COMMAND_UI(IDM_BLOCK_ALL_ACTIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_REVERT_ALL_ACTIONS, OnRevertAllActions)
    ON_UPDATE_COMMAND_UI(IDM_REVERT_ALL_ACTIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_GLOBALLY_BLOCK_ALL_ACTIONS, OnGloballyBlockAllActions)
    ON_UPDATE_COMMAND_UI(IDM_GLOBALLY_BLOCK_ALL_ACTIONS, OnUpdateSFCMenu)
    ON_COMMAND(IDM_VIEW_OBJHELP, OnViewObjHelp)
    ON_MESSAGE(WM_EDITOR_POSITION_CHANGED, OnEditorPositionChanged)
    ON_COMMAND_RANGE(IDM_INSERT_LRU_FIRST, IDM_INSERT_LRU_LAST, OnInsertLruPou)
    ON_UPDATE_COMMAND_UI_RANGE(IDM_INSERT_LRU_FIRST, IDM_INSERT_LRU_LAST, OnUpdateEdit)
    ON_COMMAND(IDM_XREF_LAST, OnXRefChangeRefPositionLast)
    ON_COMMAND(IDM_XREF_NEXT, OnXRefChangeRefPositionNext)
    ON_COMMAND(IDM_XREF_QUERY, OnXRefSetActiveQuery)
    ON_COMMAND(IDM_SHOW_VAR_SEL_POPUP, OnShowVarSelPopup)
    ON_UPDATE_COMMAND_UI(IDM_SHOW_VAR_SEL_POPUP, OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEdit)
    
    // context menu handler for IL editor
	ON_COMMAND(IDM_INSERT_LD, OnInsertLD)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_LD, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_LDN, OnInsertLDN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_LDN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_ST, OnInsertST)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_ST, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_STN, OnInsertSTN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_STN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_S, OnInsertS)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_S, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_R, OnInsertR)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_R, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_AND, OnInsertAND)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_AND, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_ANDN, OnInsertANDN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_ANDN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_OR, OnInsertOR)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_OR, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_ORN, OnInsertORN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_ORN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_XOR, OnInsertXOR)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_XOR, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_XORN, OnInsertXORN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_XORN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_ADD, OnInsertADD)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_ADD, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_SUB, OnInsertSUB)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_SUB, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_MUL, OnInsertMUL)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_MUL, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_DIV, OnInsertDIV)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_DIV, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_GT, OnInsertGT)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_GT, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_GE, OnInsertGE)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_GE, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_EQ, OnInsertEQ)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_EQ, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_NE, OnInsertNE)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_NE, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_LE, OnInsertLE)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_LE, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_LT, OnInsertLT)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_LT, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_JMP, OnInsertJMP)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_JMP, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_JMPC, OnInsertJMPC)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_JMPC, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_JMPCN, OnInsertJMPCN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_JMPCN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_CAL, OnInsertCAL)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_CAL, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_CALC, OnInsertCALC)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_CALC, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_CALCN, OnInsertCALCN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_CALCN, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_RET, OnInsertRET)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_RET, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_RETC, OnInsertRETC)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_RETC, OnUpdateEdit)
	ON_COMMAND(IDM_INSERT_RETCN, OnInsertRETCN)
	ON_UPDATE_COMMAND_UI(IDM_INSERT_RETCN, OnUpdateEdit)

    // AUTOFMT 10.08.06 SIS >>
    ON_COMMAND(IDM_EDIT_FORMATSEL, OnFormatSelection)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_FORMATSEL, OnUpdateFormatSelection)
    ON_COMMAND(IDM_EDIT_SETTINGS, OnEditorSettings)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_SETTINGS, OnUpdateEditorSettings)
    // AUTOFMT 10.08.06 SIS <<

END_MESSAGE_MAP()

IMPLEMENT_SERIAL(CDragItem, CObject,CB_FORMAT_4CLIB_VERSION)
IMPLEMENT_SERIAL(CXMLDragItem, CObject,CB_FORMAT_4CXML_VERSION)


/////////////////////////////////////////////////////////////////////////////
// CSTEditorView construction/destruction

CSTEditorView::CSTEditorView ()
{
    m_bIsInitiallyUpdated = FALSE;
    m_uiClipFormatId = RegisterClipboardFormat (CB_FORMAT_4CLIB);
    m_uiCBFormatXMLId = RegisterClipboardFormat (CB_FORMAT_4CXML);
    m_iLastLine = -1;
    m_DrawTarget = ActiveView;
    m_bHaveWPFont = m_WatchpointFont.CreateStockObject (DEFAULT_GUI_FONT) == TRUE;
    m_pTrackedWP = NULL;
    m_bEnteredSuspending = false;
    m_bRedrawAfterSFCUpdate = false;

    // These are SECEditView attributes. SECEditView does not initialize
    // them in its constructor but in OnInitialUpdate. Our OnIdle method
    // uses them before OnInitialUpdate is called so we initialize them
    // ourselves.

    m_iLineNo = 1;
    m_iColNo = 0;
    m_Features.Set( CSTEditorView::STF_DEBUGGING  );
    m_Features.Set( CSTEditorView::STF_MONITORING );
    m_Features.Set( CSTEditorView::STF_AUTOFMT    );    // AUTOFMT 10.08.06 SIS
    m_Features.Set( CSTEditorView::STF_USERHELP   );
    m_Features.Set( CSTEditorView::STF_POU_INS    );
    m_Features.Set( CSTEditorView::STF_DECL_INS   );
    m_Features.Set( CSTEditorView::STF_STMT_INS   );

    m_EditorID = 0;         // header by default
}


CSTEditorView::~CSTEditorView ()
{
    if (m_bHaveWPFont)
    {
        m_WatchpointFont.DeleteObject ();
    }

    m_DataTip.SetPopupCB (NULL);
    m_DataTip.SetTextRequestCB (NULL);
    m_DataTip.SetActionQueryCB (NULL);

    RemoveDataTip ();
}


BOOL CSTEditorView::PreCreateWindow(CREATESTRUCT& cs)
{

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return SECEditView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSTEditorView drawing

void CSTEditorView::OnDraw(CDC* pDC)
{
    CPrgEditorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // TODO: add draw code for native data here
}

void CSTEditorView::DrawInto (CDC *dc, CSize drawAreaSize)
{
    CRect drawArea;

    m_DrawTarget = InactiveView;

    drawArea.left = 0;
    drawArea.top = 0;
    drawArea.right = drawAreaSize.cx;
    drawArea.bottom = drawAreaSize.cy;

    InternalPaint (dc, drawArea, true);

    m_DrawTarget = ActiveView;
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support


/////////////////////////////////////////////////////////////////////////////
// CSTEditorView diagnostics

#ifdef _DEBUG
void CSTEditorView::AssertValid() const
{
    SECEditView::AssertValid();
}

void CSTEditorView::Dump(CDumpContext& dc) const
{
    SECEditView::Dump(dc);
}

CPrgEditorDoc* CSTEditorView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrgEditorDoc)));
    return (CPrgEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTEditorView message handlers


void CSTEditorView::SerializeText (CArchive &ar)
{

    // Just the way SECEditView TypeAhead sample does it

    if (ar.IsStoring ())
    {
        CString strText;

        GetWindowText (strText);
        ar.WriteString (strText);
    }
    else
    {
        CString strTemp;
        char *pszText=NULL;
        unsigned int uiLength=0;
        unsigned int uiSize=0;

        while (ar.ReadString (strTemp))
        {
            unsigned int uiLineLength;
            USES_CONVERSION;

            strTemp += "\n";
            uiLineLength = strTemp.GetLength ();


            // Size overflow. Is it possible that anybody
            // really waits until the overflow happens.
            // Maybe with a GHz CPU.

            if (uiLineLength + uiSize < uiLineLength)
            {
                break;
            }


            // Don't forget about the 0 byte at the end.
            // Thats why >= is used instead of >.

            if (uiLength + uiLineLength >= uiSize)
            {
                char *pszNewText;

                uiSize = uiSize == 0 ? 100 * uiLineLength : 5 * uiSize + uiLineLength;
                pszNewText = new char[uiSize];

                if (pszText != NULL)
                {
                    strcpy (pszNewText, pszText);
                    delete[] pszText;
                }

                pszText = pszNewText;
            }

            strcpy (&pszText[uiLength], T2CA (strTemp));
            uiLength += uiLineLength;
        }

        if (m_bIsInitiallyUpdated)
        {
            SetWindowText (pszText == NULL ? "" : pszText);

            // There is an error in SECEditCore. SetWindowText selects the
            // current contents and then cuts the selection before inserting
            // the new text. However cutting the contents clears the selection
            // only if there was at least one character of text selected. If
            // there was nothing selected the selected state remains in effect
            // and no caret will be displayed. Therefore we give it a little
            // kick here.

            ClearSelection (m_bUpdate);
        }
        else
        {
            m_strDelayedText = pszText == NULL ? "" : pszText;
        }

        delete[] pszText;
    }
}


void CSTEditorView::SetDelayedText(const CString& s)
{
    m_strDelayedText = s;
}


int CSTEditorView::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    if (SECEditView::OnCreate (lpCreateStruct) == -1)
        return (-1);

    if (!m_DataTip.IsCreated ())
    {
       m_DataTip.Create (this, TTS_ALWAYSTIP | TTS_NOPREFIX, this);
    }
    
    if (m_DataTip.IsCreated ())
    {
       m_DataTip.Activate ();
    }

    return (0);
}


DROPEFFECT CSTEditorView::OnDragOver (COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    CFrameWnd* pParent = GetParentFrame();
    CPrgEditorDoc *pDoc;
//    TCHAR ext[_MAX_EXT+1];

    DROPEFFECT dropeffect = SECEditView::OnDragOver(pDataObject, dwKeyState, point);

    if(!pParent) 
    {
        return DROPEFFECT_NONE;
    }

    pDoc = GetDocument();
    if(!pDoc) 
    {
        return DROPEFFECT_NONE;
    }
    if(pDoc->CheckReadonly())
    {
        return DROPEFFECT_NONE;
    }

    GetDocument()->ActivateInPlace();
    
//    if (SUCCEEDED(UTIL_GetFileExt(pDoc->GetDocumentFile(),ext,
//            sizeof(ext)/sizeof(*ext)))) {
//        if ((_stricmp(FILE_EXT_ST,ext) != 0) &&
//                (_stricmp(FILE_EXT_SDT,ext) != 0) &&
//                (_stricmp(FILE_EXT_HDR,ext) != 0) &&
//                (_stricmp(FILE_EXT_GVL,ext) != 0)) {
//            return DROPEFFECT_NONE;
//        }
//    } else {
//        return DROPEFFECT_NONE;
//    }
    if(pDataObject->IsDataAvailable(m_uiCBFormatXMLId)) {
        HGLOBAL hGlob = pDataObject->GetGlobalData(m_uiCBFormatXMLId);

        if(!hGlob)
            return DROPEFFECT_NONE;

        CSharedFile GlobFile;
        GlobFile.SetHandle(hGlob, FALSE);

        try {
            CArchive ar(&GlobFile, CArchive::load);
            CXMLDragItem DragItem;
            DragItem.Serialize(ar);

            if (DragItem.m_lObjClass == CEDB_FBObj)
            {
                return (dwKeyState & MK_CONTROL ? DROPEFFECT_COPY : DROPEFFECT_LINK);
            }
            else if (DragItem.m_lObjClass == CEDB_DUTObj)
            {
                return (DROPEFFECT_COPY);
            }
            else if (DragItem.m_lObjClass == CEDB_FncObj)
            {
                return (DROPEFFECT_LINK);
            }
            else
            {
                return DROPEFFECT_NONE;
            }
        }
        catch(CException *e) {
            e->Delete();
            return DROPEFFECT_NONE;
		
        }
    }
    else if(pDataObject->IsDataAvailable(m_uiClipFormatId)) {
        HGLOBAL hGlob = pDataObject->GetGlobalData(m_uiClipFormatId);

        if(!hGlob)
            return DROPEFFECT_NONE;

        CSharedFile GlobFile;
        GlobFile.SetHandle(hGlob, FALSE);

        try {
            CArchive ar(&GlobFile, CArchive::load);
            CDragItem DragItem;
            DragItem.Serialize(ar);
            if (DragItem.m_lObjClass == CEDB_FBObj || DragItem.m_lObjClass == CEDB_DUTObj)
            {
                return (DROPEFFECT_COPY);
            }
            else if (DragItem.m_lObjClass == CEDB_FncObj || DragItem.m_lObjClass == CEDB_ConstObj)
            {
                return (DROPEFFECT_LINK);
            }
            else
            {
                return DROPEFFECT_NONE;
            }
        }
        catch(CException *e) {
            e->Delete();
            return DROPEFFECT_NONE;
		
        }
    }
	
    return dropeffect;
}


BOOL CSTEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pDataObject->IsDataAvailable(m_uiCBFormatXMLId)) {
		HGLOBAL hGlob = pDataObject->GetGlobalData(m_uiCBFormatXMLId);

		if(!hGlob)
			return FALSE;

		CSharedFile GlobFile;
		GlobFile.SetHandle(hGlob, FALSE);

		try {
			CArchive ar(&GlobFile, CArchive::load);
			CXMLDragItem DragItem;

			DragItem.Serialize(ar);

		    SendMessage(WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(point.x,point.y));
		    SendMessage(WM_LBUTTONUP,MK_LBUTTON,MAKELPARAM(point.x,point.y));

			if (DragItem.m_lObjClass == CEDB_FBObj)
			{
				if (dropEffect == DROPEFFECT_COPY)
				{
					InsertDeclaration (DragItem.m_strSource);
				}
				else
				{
					OnInsertFBCallHint (DragItem.m_strSource);
				}
			}
			else if (DragItem.m_lObjClass == CEDB_DUTObj)
			{
				InsertDeclaration (DragItem.m_strSource);
			}
			else if (DragItem.m_lObjClass == CEDB_FncObj || DragItem.m_lObjClass == CEDB_ConstObj)
			{
				OnInsertFBCallHint (DragItem.m_strSource);
			}
		}
		catch(CException *e) {
			e->Delete();
			return FALSE;
		
		}
	}
	else if(pDataObject->IsDataAvailable(m_uiClipFormatId)) {
		HGLOBAL hGlob = pDataObject->GetGlobalData(m_uiClipFormatId);

		if(!hGlob)
			return FALSE;

		CSharedFile GlobFile;
		GlobFile.SetHandle(hGlob, FALSE);

		try {
            CString strItem,strHelp;
			CArchive ar(&GlobFile, CArchive::load);
			CDragItem DragItem;
			DragItem.Serialize(ar);

            switch(DragItem.m_lObjClass) {
                case CEDB_FBObj:
                case CEDB_DUTObj:
				    strItem.Empty();
				    strHelp.LoadString(IDS_KEY_DEFINSTANCE);
				    strItem += DragItem.m_strObjName;
				    strItem += strHelp;
				    strHelp.LoadString(IDS_KEY_COLON);
				    strItem += _T(' ');
				    strItem += strHelp;
				    strItem += _T(' ');
				    strItem += DragItem.m_strObjName;
				    strHelp.LoadString(IDS_KEY_SEMICOL);
				    strItem += strHelp;
				    
				    SendMessage(WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(point.x,point.y));
				    SendMessage(WM_LBUTTONUP,MK_LBUTTON,MAKELPARAM(point.x,point.y));
				    InsertString (strItem.GetBuffer (0), TRUE, TRUE);
                    break;
                case CEDB_FncObj:
				    strItem.Empty();
				    strItem += DragItem.m_strObjName;
                    strHelp.Format(IDS_KEY_IDENT,_T(""));
                    strItem += strHelp;
				    strHelp.LoadString(IDS_KEY_SEMICOL);
				    strItem += strHelp;

				    SendMessage(WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(point.x,point.y));
				    SendMessage(WM_LBUTTONUP,MK_LBUTTON,MAKELPARAM(point.x,point.y));
				    InsertString (strItem.GetBuffer (0), TRUE, TRUE);
                    break;
                default:
                    return FALSE;
			}
		}
		catch(CException *e) {
			e->Delete();
			return FALSE;
		
		}
	}
	
	return SECEditView::OnDrop(pDataObject, dropEffect, point);
}


void CSTEditorView::OnInitialUpdate()
{
   CSECEditor *pEdit;
   
   SECEditView::OnInitialUpdate ();
   
   SetWindowText (m_strDelayedText);
   m_strDelayedText = "";
   
   // There is an error in SECEditCore. SetWindowText selects the
   // current contents and then cuts the selection before inserting
   // the new text. However cutting the contents clears the selection
   // only if there was at least one character of text selected. If
   // there was nothing selected the selected state remains in effect
   // and no caret will be displayed. Therefore we give it a little
   // kick here.
   
   ClearSelection (m_bUpdate);
   
   pEdit = GetEditor( m_EditorID );
   pEdit->SetModified (FALSE);
   pEdit->InitUndo();
   
   ModifyStyle(0,WS_VSCROLL);  // add WS_VSCROLL
   
   SECEditView::SetScrollRange(SB_VERT,0,32000);
   SECEditView::SetScrollPos(SB_VERT,32000);
   SECEditView::ShowScrollBar(SB_VERT);
   
   m_bIsInitiallyUpdated = TRUE;
}


void CSTEditorView::OnSize(UINT nType, int cx, int cy)
{
    CPrgEditorDoc *pDoc;
    CFrameWnd *pFrame;

    // If the editor document is a doc object (better condition would be:
    // if the editor is running inside CEMAIN) and the parent frame is not
    // a COleDocIPFrameWnd (i.e. the editor is not inplace active) we al-
    // ways use the client area extent passed by CEMAIN as the view size.
    // Using (cx,cy) is wrong because it is the MDI child frame size de-
    // creased by border space which is in no way related to the required
    // size. Using a wrong size leads to strange behaviour (see scenario
    // described below). Note that we cannot use pDoc->IsInplaceActive to
    // determine whether to use (cx,cy) or the client area extent because
    // during inplace deactivation OnSize is called with the MDI child
    // frame size while pDoc->IsInplaceActive still returns TRUE. Instead
    // we use the kind of parent frame to decide.
    // Scenario: Client area height (as passed by CEMAIN) is smaller than
    // the height needed to display the whole contents. The MDI child frame
    // height (decreased by border space) is bigger than the height needed
    // to display the whole contents. The editor window is scrolled so that
    // the contents first line is not visible. As soon as OnSize is called
    // (with the height of the MDI child frame decreased by border space)
    // OE believes there is enough room to display all of the content and
    // sets the first visible line to 1. This is obviously not what we wan-
    // ted since the actual visible size (as passed by CEMAIN) is too small
    // to display all of the content.

    pDoc = GetDocument ();
    pFrame = GetParentFrame ();
    if (pDoc != NULL && pDoc->IsDocObject () &&
        pFrame != NULL && dynamic_cast<COleDocIPFrameWnd *> (pFrame) == NULL)
    {
        CSize size;

        size = pDoc->GetClientExtent ();
        cx = size.cx;
        cy = size.cy;
    }

	SECEditView::OnSize(nType, cx, cy);
}



void CSTEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWatchpoint *pWP;
	CPoint grabPosition;
    CPrgEditorDoc *pDoc;

	pWP = HitWatchpoint (point, grabPosition);
	if (pWP == NULL)
	{
		SECEditView::OnLButtonDown(nFlags, point);
		return;
	}

    pDoc = GetDocument ();
    if (pDoc != NULL)
    {
        if (pDoc->RaiseWatchpoint (pWP))
        {
            Invalidate ();
            UpdateWindow ();
        }
    }

	SetCapture ();
	m_pTrackedWP = pWP;

	m_WPGrabPosition = grabPosition;
}


void CSTEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPrgWPClientData *pClientData;

	if (m_pTrackedWP == NULL)
	{
		SECEditView::OnLButtonUp(nFlags, point);
		return;
	}

	pClientData = (CPrgWPClientData *)m_pTrackedWP->GetClientData ();
	assert (pClientData != NULL);
	if (pClientData != NULL)
	{
		CRect rect;
		long lDisplayX;
		long lDisplayY;
		long lDisplayOffsetX;
		long lDisplayOffsetY;

		rect = pClientData->GetArea ();

		lDisplayX = rect.left / m_iCharWidth;
		lDisplayY = rect.top / m_iLineHeight;

		lDisplayOffsetX = rect.left - lDisplayX * m_iCharWidth;
		lDisplayOffsetY = rect.top - lDisplayY * m_iLineHeight;

		pClientData->SetDisplay (lDisplayX, lDisplayY + 1, lDisplayOffsetX, lDisplayOffsetY);
	}

	ReleaseCapture ();
	m_pTrackedWP = NULL;
}


void CSTEditorView::OnMouseMove (UINT nFlags, CPoint point)
{
	if (m_pTrackedWP != NULL)
	{
		TrackWP (point);
		return;
	}

	SECEditView::OnMouseMove (nFlags, point);
}


CWatchpoint *CSTEditorView::HitWatchpoint (CPoint point, CPoint &grabPosition)
{
    CPrgEditorDoc *pDoc;
    POSITION pPosition;
    CWatchpoint *pBestWP=NULL;
    CPrgWPClientData *pBestClientData=NULL;
    CRect rect;

    pDoc = GetDocument ();

    pPosition = pDoc->GetFirstWPPosition ();
    while (pPosition != NULL)
    {
        CWatchpoint *pWP;
        CPrgWPClientData *pClientData;

        pWP = pDoc->GetNextWP (pPosition);
        ASSERT (pWP != NULL);
        if (pWP == NULL)
            continue;

        pClientData = (CPrgWPClientData *)pWP->GetClientData ();
        ASSERT (pClientData != NULL);
        if (pClientData == NULL)
            continue;

        rect = pClientData->GetArea ();
        WPAreaToClientArea (rect);

        if (point.x >= rect.left && point.x < rect.right &&
            point.y >= rect.top && point.y < rect.bottom)
        {
            if (pBestWP == NULL || pClientData->GetZ () < pBestClientData->GetZ ())
            {
                pBestWP = pWP;
                pBestClientData = pClientData;
            }
        }
    }

    if (pBestWP == NULL)
    {
        return (NULL);
    }

    rect = pBestClientData->GetArea ();
    WPAreaToClientArea (rect);

    grabPosition.x = point.x - rect.left;
    grabPosition.y = point.y - rect.top;

    return (pBestWP);
}


void CSTEditorView::TrackWP (CPoint point)
{
	CPrgWPClientData *pClientData;
	CRect rect;
	CRect wpArea;
	int xDelta;
	int yDelta;

	GetClientRect (&rect);

	if (point.x < rect.left || point.x >= rect.right ||
	    point.y < rect.top || point.y >= rect.bottom)
	{
		return;
	}

	pClientData = (CPrgWPClientData *)m_pTrackedWP->GetClientData ();
	assert (pClientData != NULL);
	if (pClientData == NULL)
		return;

	wpArea = pClientData->GetArea ();
	WPAreaToClientArea (wpArea);

	InvalidateRect (&wpArea);

	xDelta = point.x - (wpArea.left + m_WPGrabPosition.x);
	yDelta = point.y - (wpArea.top + m_WPGrabPosition.y);

	wpArea.left += xDelta;
	wpArea.right += xDelta;
	wpArea.top += yDelta;
	wpArea.bottom += yDelta;

	InvalidateRect (&wpArea);

	ClientAreaToWPArea (wpArea);
	pClientData->SetArea (wpArea);

	UpdateWindow ();
}


CString CSTEditorView::GetCursorContextExpression (CStdEditorPosition &startPosition, CStdEditorPosition &endPosition)
{
   return (GetContextExpression (GetCursorPoint (), startPosition, endPosition));
}


CString CSTEditorView::GetContextExpression (CPoint point, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition)
{
   CPrgEditorDoc *pDoc;
   CStdEditorPosition position;
   
   pDoc = GetDocument ();
   
   if (IsPointInBlock (point))
   {
      TEXTPOS startTextPos;
      TEXTPOS endTextPos;
      CStdEditorPosition selectionStart;
      CStdEditorPosition selectionEnd;
      
      if (!GetSelection (startTextPos, endTextPos) || !pDoc->CanMonitor())
      {
         return ("");
      }
      
      selectionStart = CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo);
      selectionEnd = CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset, endTextPos.pLine->iLineNo);
      
      return (GetEditor(m_EditorID)->GetTrimmedText (selectionStart, selectionEnd, startPosition, endPosition));
   }
   
   if (!ConvertMousePosition (point, position))
   {
      return ("");
   }
   
   return (pDoc->GetSmartText (position, startPosition, endPosition));
}


CPoint CSTEditorView::GetCursorPoint ()
{
    CPoint point;

    point.x = m_iMarkSpace + (m_iColNo - m_iLeftEdge) * m_iCharWidth + m_iCharWidth / 2;
    point.y = (m_iLineNo - m_iFirstLineNo) * m_iLineHeight + m_iLineHeight / 2;

    return (point);
}


bool CSTEditorView::ConvertMousePosition (const CPoint &mousePoint, CStdEditorPosition &position)
{
	int iMouseLine;
	int iMouseColumn;

	if (mousePoint.x < m_iMarkSpace || mousePoint.y > m_rect.bottom)
	{
		return (false);
	}

	iMouseColumn = m_iLeftEdge + (mousePoint.x - m_iMarkSpace) / m_iCharWidth;

	iMouseLine = m_iFirstLineNo + mousePoint.y / m_iLineHeight;

	position = CStdEditorPosition (EDP_STLINE, iMouseColumn + 1, iMouseLine);

	return (true);
}

bool CSTEditorView::ConvertPositionMouse (const CStdEditorPosition &position, CPoint &mousePoint)
{
    int iPosCol = position.GetX() - 1;
    int iPosLine = position.GetY();

    int xMouse;
    int yMouse;

    xMouse = (iPosCol - m_iLeftEdge) * m_iCharWidth + m_iMarkSpace;
    yMouse = (iPosLine - m_iFirstLineNo) * m_iLineHeight;

    mousePoint.x = xMouse;
    mousePoint.y = yMouse;

    return TRUE;
}



void CSTEditorView::OnUpdate (CView *pSender, LPARAM lHint, CObject *pHint)
{
    if (lHint == CPrgEditorDoc::IdleHint)
    {
        OnIdle ();
        return;
    }

    if (lHint == CPrgEditorDoc::ToolTipNfyHint)
    {
       OnUpdateToolTipNotify( (TTNfyData*)pHint );
       return;
    }
   
    if (lHint == CPrgEditorDoc::PreDisconnectHint)
    {
        RemoveDataTip ();
        return;
    }

    if (lHint == CPrgEditorDoc::ShowWatchpointHint)
    {
        if( ((CWatchpoint*)pHint)->GetPosition().GetPosQual() == EDP_STLINE)
        {
           OnShowWatchpoint ((CWatchpoint *)pHint);
        }
        return;
    }

    if (lHint == CPrgEditorDoc::HideWatchpointHint)
    {
        if( ((CWatchpoint*)pHint)->GetPosition().GetPosQual() == EDP_STLINE)
        {
           OnHideWatchpoint ((CWatchpoint *)pHint);
        }
        return;
    }

    if (lHint == CPrgEditorDoc::WatchpointDataHint)
    {
        if( ((CWatchpoint*)pHint)->GetPosition().GetPosQual() == EDP_STLINE)
        {
           UpdateWatchpoint ((CWatchpoint *)pHint);
        }
        return;
    }

    if (lHint == CPrgEditorDoc::ShowBreakpointHint)
    {
        OnShowBreakpoint ((CBreakpoint *)pHint);
        return;
    }

    if (lHint == CPrgEditorDoc::HideBreakpointHint)
    {
        OnHideBreakpoint ((CBreakpoint *)pHint);
        return;
    }

    if (lHint == CPrgEditorDoc::ShowStoppointHint)
    {
        OnShowStoppoint ();
        return;
    }

    if (lHint == CPrgEditorDoc::HideStoppointHint)
    {
        OnHideStoppoint ();
        return;
    }

    if (lHint == CStdEditorDoc::GotoPositionHint)
    {
        GotoPosition (*(CStdEditorPosition *)pHint);
        return;
    }

    if (lHint == CStdEditorDoc::GetPositionHint)
    {
        GetPosition (*(CStdEditorPosition *)pHint);
        return;
    }

    if (lHint == CPrgEditorDoc::InsertFBCallHint)
    {
        OnInsertFBCallHint (*(CString *)pHint);
        return;
    }

    if (lHint == CPrgEditorDoc::InsertVariableHint)
    {
        OnInsertVariable(*(CString *)pHint);
        return;
    }

    if (lHint == CStdEditorDoc::GotoLocationHint)
    {
        GotoLocation (*(CString *)pHint);
        return;
    }

    if (lHint == CPrgEditorDoc::ExpressionChangedHint)
    {
        if ((LONG)pHint == m_lDataTipHandle)
        {
            m_DataTip.Follow (true);
        }

        return;
    }

    if (lHint == CPrgEditorDoc::ManualModeToggledHint)
    {
        OnManualModeToggled ();
        return;
    }

    if (lHint == CPrgEditorDoc::StepAttributeChangedHint)
    {
        OnStepAttributeChanged ((CPrgEdStep *)pHint);
        return;
    }
    
    if (lHint == CPrgEditorDoc::ActionAttributeChangedHint)
    {
        OnActionAttributeChanged ((CPrgEdAction *)pHint);
        return;
    }

    if (lHint == CPrgEditorDoc::AllActionsOffToggledHint)
    {
        OnAllActionsOffToggled ();
        return;
    }

    if (lHint == CPrgEditorDoc::TransitionAttributeChangedHint)
    {
        OnTransitionAttributeChanged ((CPrgEdTransition *)pHint);
        return;
    }
    
    if (lHint == CPrgEditorDoc::AllTransitionsOnToggledHint)
    {
        OnAllTransitionsOnToggled ();
        return;
    }

    if (lHint == CPrgEditorDoc::EndSFCUpdateHint)
    {
        OnEndSFCUpdate ();
        return;
    }

    // AUTOFMT 14.08.06 SIS >>
    if(lHint == CSTEditorDoc::SettingsChangedHint)
    {
        SECEdit*    pEdit = GetEdit();
        ASSERT(pEdit);
        if(pEdit)
        {
            pEdit->InitFileTypeSettings();
            Invalidate();
            UpdateWindow();
        }
    }
    // AUTOFMT 14.08.06 SIS <<

    SECEditView::OnUpdate (pSender, lHint, pHint);
}


void CSTEditorView::OnShowWatchpoint (CWatchpoint *pWP)
{
    CClientDC dc (this);

    BuildWatchpointText (pWP);
    UpdateWatchpointArea (pWP, &dc);

    if (GetDocument ()->IsInPlaceActive ())
    {
        RemoveDataTip();
        Invalidate ();
        UpdateWindow ();
    }
    else
    {
        COleServerItem *pItem;

        pItem = GetDocument ()->GetEmbeddedItem ();
        ASSERT (pItem != NULL);
        if (pItem != NULL)
        {
            pItem->NotifyChanged ();
        }
    }
}


void CSTEditorView::BuildWatchpointText (CWatchpoint *pWP)
{
    CPrgWPClientData *pClientData;
    CString strText;
    COLORREF backgroundColor;
    bool bCrossedOut;

    ASSERT (pWP != NULL);
    if (pWP == NULL)
    {
        return;
    }

    pClientData = (CPrgWPClientData *)pWP->GetClientData ();
    ASSERT (pClientData != NULL);
    if (pClientData == NULL)
    {
        return;
    }

    strText = BuildWatchText (pClientData->GetVariableName (), pWP->GetWatchItem (), pWP->GetIECType (), true, backgroundColor, bCrossedOut);

    pClientData->SetText (strText);
    pClientData->SetBackgroundColor (backgroundColor);
    pClientData->SetCrossedOut (bCrossedOut);
}


LPCTSTR CSTEditorView::GetDataTypeName(UINT uiIECType)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>GetDataTypeName</B>.
*
*	Description = returns a string constant containing the data type name
*	<p>
*	Specification =
*	<p>
*	@param uiIECType - id of the data type (as defined in baselib.h)
*	<p>
*	@param
*	<p>
*
*	@return - String constant containing the data type name
*	<p>
*	@return
*	<p>
*
*
*	@exception
*	<p>
*	@see
*	<p>
*/
{

    switch (uiIECType)
    {
        case BL_ANY:
            return _T("BOOL");
            break;

        case BL_BOOL:
            return _T("BOOL");
            break;

        case BL_BYTE:
            return _T("BYTE");
            break;

        case BL_WORD:
            return _T("WORD");
            break;

        case BL_DWORD:
            return _T("DWORD");
            break;

        case BL_LWORD:
            return _T("LWORD");
            break;

        case BL_DINT:
            return _T("DINT");
            break;

        case BL_LINT:
            return _T("LINT");
            break;

        case BL_LREAL:
            return _T("LREAL");
            break;

        case BL_TIME:
            return _T("TIME");
            break;

        case BL_DT:
            return _T("DT");
            break;

        case BL_DATE:
            return _T("DATE");
            break;

        case BL_TOD:
            return _T("TOD");
            break;

        case BL_WSTRING:
            return _T("WSTRING");
            break;

		case BL_UINT:
            return _T("UINT");
            break;

		case BL_UDINT:
            return _T("UDINT");
            break;

		case BL_ULINT:
            return _T("ULINT");
            break;

        default:
            return _T("");
            break;
    }
}


void CSTEditorView::OnShowBreakpoint (CBreakpoint *pBP)
{
	if (GetDocument ()->IsInPlaceActive ())
	{
		Invalidate ();
		UpdateWindow ();
	}
	else
	{
		COleServerItem *pItem;

		pItem = GetDocument ()->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		}
	}
}


void CSTEditorView::OnHideBreakpoint (CBreakpoint *pBP)
{
	if (GetDocument ()->IsInPlaceActive ())
	{
		Invalidate ();
		UpdateWindow ();
	}
	else
	{
		COleServerItem *pItem;

		pItem = GetDocument ()->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		}
	}
}


void CSTEditorView::OnShowStoppoint ()
{
	if (GetDocument ()->IsInPlaceActive ())
	{
		Invalidate ();
		UpdateWindow ();
	}
	else
	{
		COleServerItem *pItem;

		pItem = GetDocument ()->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		}
	}
}


void CSTEditorView::OnHideStoppoint ()
{
	if (GetDocument ()->IsInPlaceActive ())
	{
		Invalidate ();
		UpdateWindow ();
	}
	else
	{
		COleServerItem *pItem;

		pItem = GetDocument ()->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		}
	}
}


void CSTEditorView::OnManualModeToggled ()
{
    m_bRedrawAfterSFCUpdate = true;
}


void CSTEditorView::OnStepAttributeChanged (CPrgEdStep *pStep)
{
    long lY;

    lY = pStep->GetY ();

    if (lY >= m_iFirstLineNo && lY <= LastLineNo ())
    {
        m_bRedrawAfterSFCUpdate = true;
    }
}


void CSTEditorView::OnActionAttributeChanged (CPrgEdAction *pAction)
{
    long lY;

    lY = pAction->GetY ();

    if (lY >= m_iFirstLineNo && lY <= LastLineNo ())
    {
        m_bRedrawAfterSFCUpdate = true;
    }
}


void CSTEditorView::OnAllActionsOffToggled ()
{
    m_bRedrawAfterSFCUpdate = true;
}


void CSTEditorView::OnTransitionAttributeChanged (CPrgEdTransition *pTransition)
{
    long lY;

    lY = pTransition->GetY ();

    if (lY >= m_iFirstLineNo && lY <= LastLineNo ())
    {
        m_bRedrawAfterSFCUpdate = true;
    }
}


void CSTEditorView::OnAllTransitionsOnToggled ()
{
    m_bRedrawAfterSFCUpdate = true;
}


void CSTEditorView::OnEndSFCUpdate ()
{
    if (!m_bRedrawAfterSFCUpdate)
    {
        return;
    }

    m_bRedrawAfterSFCUpdate = false;

	if (GetDocument ()->IsInPlaceActive ())
	{
		Invalidate ();
		UpdateWindow ();
	}
	else
	{
		COleServerItem *pItem;

		pItem = GetDocument ()->GetEmbeddedItem ();
		ASSERT (pItem != NULL);
		if (pItem != NULL)
		{
			pItem->NotifyChanged ();
		}
	}
}


void CSTEditorView::GotoPosition (const CStdEditorPosition &position)
{
	CStdEditorPosition displayPosition;
	int iX;
	int iY;
	SECEdit *pEdit;
	int iLines;

	displayPosition = GetDocument()->GetEditor(m_EditorID)->ToDisplayPosition (position);
	displayPosition.GetXYPosition (iX, iY);
	// If column is not specified use column 1
	if (iX <= 0)
	{
		iX = 1;
	}

	// Always clear a selection when jumping to a position.
	ClearSelection (m_bUpdate);

	pEdit = GetEditor (m_EditorID);
	assert (pEdit != NULL);
	if (pEdit == NULL)
		return;
	iLines = pEdit->m_iLinesInFile;

	GotoLine(iY > iLines ? iLines : iY);
	if(iX > 1)
	{
		for (int i = 1; i < iX-1; i++)
			CursorRight(false);
		CursorRight(true);
	}
}


void CSTEditorView::GetPosition (CStdEditorPosition &position)
{
	position = CStdEditorPosition (EDP_STLINE, m_iColNo + 1, m_iLineNo);
}


void CSTEditorView::GotoLocation (const CString &strLocation)
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;
	CStdEditorPosition position;

	pDoc = GetDocument ();

	hr = GetEditor(m_EditorID)->DecodeLocation (strLocation, position);
	if (FAILED (hr))
	{
		return;
	}

	GotoPosition (position);
}

void CSTEditorView::OnInsertVariable(const CString &strVar)
{
    InsertTextLines (strVar);
}

void CSTEditorView::OnInsertFBCallHint (const CString &strXML)
{
   CPrgEditorDoc* pDoc = GetDocument();
   if(pDoc && pDoc->GetLastActiveView()==this)
   {
      InsertFBCall(strXML);
   }
}

void CSTEditorView::InsertFBCall(const CString &strXML)
{
	CString strTemp=strXML;
	CMemFile file;
	LPCTSTR pszXML;
	HRESULT hr;

	pszXML = strTemp.GetBuffer (0);
	file.Attach ((BYTE *)pszXML, strTemp.GetLength () * sizeof (TCHAR));

	try
	{
		CXMLArchive ar (&file, CArchive::load);
		CString strCall;

		ar.Load ();

		if (!ar.MoveDown (_T ("BODY")))
		{
			hr = E_FAIL;
		}
		else
		{
			if (ar.MoveDown (_T ("FB")))
			{
				hr = BuildFBCallFromXML (ar, strCall);
			}
			else if (ar.MoveDown (_T ("FNC")))
			{
				hr = BuildFunctionCallFromXML (ar, strCall);
			}
			else if (ar.MoveDown (_T ("CONST")))
			{
				hr = BuildConstantValueFromXML (ar, strCall);
			}
			else
			{
				FC_DebugPrint ("Cannot insert this kind of XML");
				hr = E_FAIL;
			}
		}

		if (hr == S_OK)
		{
			InsertTextLines (strCall);
		}
	}
	catch (HRESULT hrX)
	{
		FC_DebugPrint ("Error while deserializing POU XML (hr=%08lx)\n", hrX);
		hr = hrX;
	}

	if (FAILED (hr))
	{
		FC_DebugPrint ("Failed to insert XML (hr=%08lx)", hr);
	}

	strTemp.ReleaseBuffer ();
}

HRESULT CSTEditorView::BuildFBCallFromXML (CXMLArchive &ar, CString &strCall)
{
	CString strName;
	CString strTypeName;
	unsigned int uiIndentation;

	strCall.Empty ();

	// If we have a name in XML description we will use it.
	// Otherwise we construct an instance name from the type
	// name.

	(void)ar.GetAttribute (_T ("NAME"), strName);

	if (!ar.GetAttribute (_T ("TYPENAME"), strTypeName))
	{
		FC_DebugPrint ("Attribute TYPENAME missing in FB tag");
		return (E_FAIL);
	}

	if (strName.IsEmpty ())
	{
		strCall.Format ("%s_Inst(", (LPCTSTR)strTypeName);
	}
	else
	{
		strCall.Format ("%s(", (LPCTSTR)strName);
	}

	uiIndentation = strCall.GetLength ();

	if (ar.MoveDown (_T ("PINLIST")))
	{
		bool bFirstArgument=true;

		if (ar.MoveDown (_T ("PIN")))
		{
			do
			{
				CString strArgumentClass;

				if (!ar.GetAttribute (_T ("CLASS"), strArgumentClass))
				{
					FC_DebugPrint ("Attribute CLASS missing in PIN tag");
					continue;
				}

				if (strArgumentClass.CompareNoCase ("input") == 0 ||
				    strArgumentClass.CompareNoCase ("inoutput") == 0 ||
					strArgumentClass.CompareNoCase ("inext") == 0)
				{
					CString strArgumentName;
					CString strArgumentType;

					if (!ar.GetAttribute (_T ("NAME"), strArgumentName))
					{
						FC_DebugPrint ("Attribute NAME missing in PIN tag");
						continue;
					}

					if (!ar.GetAttribute (_T ("TYPE"), strArgumentType))
					{
						FC_DebugPrint ("Attribute TYPE missing in PIN tag");
						continue;
					}

					if (!bFirstArgument)
					{
						strCall += _T (",\n");
						strCall += CString (_T (' '), uiIndentation);
					}
					else
					{
						bFirstArgument = false;
					}

					strCall += strArgumentName;
					strCall += " := (*";
					strCall += strArgumentType;
                    if (strArgumentClass.CompareNoCase("inoutput") == 0)
                        strCall += ", INOUT";
					strCall += "*)";
				}
			} while (ar.MoveNext ());

			ar.MoveUp ();
		}

		if (ar.MoveDown (_T ("PIN")))
		{
			bool bHaveComment=false;

			do
			{
				CString strArgumentClass;

				if (!ar.GetAttribute (_T ("CLASS"), strArgumentClass))
				{
					FC_DebugPrint ("Attribute CLASS missing in PIN tag");
					continue;
				}

				if (strArgumentClass.CompareNoCase ("output") == 0 ||
					strArgumentClass.CompareNoCase ("outext") == 0)
				{
					CString strArgumentName;
					CString strArgumentType;

					if (!ar.GetAttribute (_T ("NAME"), strArgumentName))
					{
						FC_DebugPrint ("Attribute NAME missing in PIN tag");
						continue;
					}

					if (!ar.GetAttribute (_T ("TYPE"), strArgumentType))
					{
						FC_DebugPrint ("Attribute TYPE missing in PIN tag");
						continue;
					}

					if (!bFirstArgument)
					{
						strCall += _T ("\n");
						strCall += CString (_T (' '), uiIndentation);
					}
					else
					{
						bFirstArgument = false;
					}

					if (!bHaveComment)
					{
						strCall += _T ("(*");
						uiIndentation += 2;
						bHaveComment = true;
					}

					strCall += strArgumentName;
					strCall += " => ";
					strCall += strArgumentType;
				}
			} while (ar.MoveNext ());

			if (bHaveComment)
			{
				strCall += _T ("*)");
			}
		}
	}

	strCall += _T (");\n");

	return (S_OK);
}


HRESULT CSTEditorView::BuildFunctionCallFromXML (CXMLArchive &ar, CString &strCall)
{
	CString strName;
	bool bHaveReturnType=false;
	CString strReturnType;

	strCall.Empty ();

	if (!ar.GetAttribute (_T ("TYPENAME"), strName))
	{
		FC_DebugPrint ("Attribute TYPENAME missing in FNC tag");
		return (E_FAIL);
	}

	strCall.Format ("%s(", (LPCTSTR)strName);

	if (ar.MoveDown (_T ("PINLIST")))
	{
		if (ar.MoveDown (_T ("PIN")))
		{
			CString strArgumentClass;
			CString strArgumentName;
			CString strArgumentType;
			bool bFirstArgument=true;

			do
			{
				if (!ar.GetAttribute (_T ("CLASS"), strArgumentClass))
				{
					FC_DebugPrint ("Attribute CLASS missing in PIN tag");
					continue;
				}

				if (!ar.GetAttribute (_T ("NAME"), strArgumentName))
				{
					FC_DebugPrint ("Attribute NAME missing in PIN tag");
					continue;
				}

				if (!ar.GetAttribute (_T ("TYPE"), strArgumentType))
				{
					FC_DebugPrint ("Attribute TYPE missing in PIN tag");
					continue;
				}

				if (strArgumentClass.CompareNoCase ("input") == 0 ||
				    strArgumentClass.CompareNoCase ("inoutput") == 0 ||
				    strArgumentClass.CompareNoCase ("inext") == 0)
				{
					if (!bFirstArgument)
					{
						strCall += _T (", ");
					}
					else
					{
						bFirstArgument = false;
					}

					strCall += strArgumentName;
					strCall += _T (" (*");
					strCall += strArgumentType;
				    if (strArgumentClass.CompareNoCase ("inoutput") == 0)
                        strCall += _T(", INOUT");
					strCall += _T ("*)");
				}
				else if (strArgumentClass.CompareNoCase ("output") == 0 ||
				         strArgumentClass.CompareNoCase ("outext") == 0)
				{
                    if (!strArgumentName.IsEmpty())
                    {
					    if (!bFirstArgument)
					    {
						    strCall += _T (", ");
					    }
					    else
					    {
						    bFirstArgument = false;
					    }

					    strCall += strArgumentName;
					    strCall += _T (" (*");
					    strCall += strArgumentType;
					    strCall += _T (", OUTPUT*)");
                    }
                    else if (bHaveReturnType)
					{
						FC_DebugPrint ("Duplicate output PIN tag in function XML");
					}
					else
					{
						strReturnType = strArgumentType;
						bHaveReturnType = true;
					}
				}
			} while (ar.MoveNext ());
		}
	}

	strCall += _T(");");

	if (bHaveReturnType)
	{
		strCall += _T (" (* Returns ");
		strCall += strReturnType;
		strCall += _T(" *)");
	}

	return (S_OK);
}


HRESULT CSTEditorView::BuildConstantValueFromXML (CXMLArchive &ar, CString &strCall)
{
	CString strName;

	strCall.Empty ();

	if (!ar.GetAttribute (_T ("NAME"), strName))
	{
		FC_DebugPrint ("Attribute NAME missing in CONST tag");
		return (E_FAIL);
	}

	strCall = strName;
	return (S_OK);
}


HRESULT CSTEditorView::InsertDeclaration (const CString &strXML)
{
	CString strTemp=strXML;
	CMemFile file;
	LPCTSTR pszXML;
	HRESULT hr;

	pszXML = strTemp.GetBuffer (0);
	file.Attach ((BYTE *)pszXML, strTemp.GetLength () * sizeof (TCHAR));

	try
	{
		CXMLArchive ar (&file, CArchive::load);
		CString strDecl;

		ar.Load ();

		if (!ar.MoveDown (_T ("BODY")))
		{
			hr = E_FAIL;
		}
		else
		{
			if (ar.MoveDown (_T ("FB")))
			{
				hr = BuildFBDeclaration (ar, strDecl);
			}
			else
			{
                if(ar.MoveDown (_T ("SDT")))
                {
                    hr = BuildSDTDeclaration (ar, strDecl);
                }
                else
                {
				    FC_DebugPrint ("Cannot insert this kind of XML");
				    hr = E_FAIL;
                }
			}
		}

		if (hr == S_OK)
		{
			InsertTextLines (strDecl);
		}
	}
	catch (HRESULT hrX)
	{
		FC_DebugPrint ("Error while deserializing POU XML (hr=%08lx)\n", hrX);
		hr = hrX;
	}

	if (FAILED (hr))
	{
		FC_DebugPrint ("Failed to insert XML (hr=%08lx)", hr);
	}

	strTemp.ReleaseBuffer ();

	return (hr);
}


HRESULT CSTEditorView::BuildFBDeclaration (CXMLArchive &ar, CString &strDecl)
{
	CString strName;
	CString strTypeName;

	strDecl.Empty ();

	// If we have a name in XML description we will use it.
	// Otherwise we construct an instance name from the type
	// name.

	(void)ar.GetAttribute (_T ("NAME"), strName);

	if (!ar.GetAttribute (_T ("TYPENAME"), strTypeName))
	{
		FC_DebugPrint ("Attribute TYPENAME missing in FB tag");
		return (E_FAIL);
	}

	if (strName.IsEmpty ())
	{
		strDecl.Format ("%s_Inst: %s;", (LPCTSTR)strTypeName, (LPCTSTR)strTypeName);
	}
	else
	{
		strDecl.Format ("%s: %s;", (LPCTSTR)strName, (LPCTSTR)strTypeName);
	}

	return (S_OK);
}


HRESULT CSTEditorView::BuildSDTDeclaration (CXMLArchive &ar, CString &strDecl)
{
	CString strName;
	CString strTypeName;

	strDecl.Empty ();

	// If we have a name in XML description we will use it.
	// Otherwise we construct an instance name from the type
	// name.

	(void)ar.GetAttribute (_T ("NAME"), strName);

	if (!ar.GetAttribute (_T ("TYPENAME"), strTypeName))
	{
		FC_DebugPrint ("Attribute TYPENAME missing in SDT tag");
		return (E_FAIL);
	}

	if (strName.IsEmpty ())
	{
		strDecl.Format ("%s_Inst: %s;", (LPCTSTR)strTypeName, (LPCTSTR)strTypeName);
	}
	else
	{
		strDecl.Format ("%s: %s;", (LPCTSTR)strName, (LPCTSTR)strTypeName);
	}

	return (S_OK);
}



void CSTEditorView::OnInsertIF ()
{
	InsertTextLines ("IF Expression THEN\n\tThenStatementList;\nELSE\n\tElseStatementList;\nEND_IF;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertCASE ()
{
	InsertTextLines ("CASE Expression OF\n\tElement: StatementList;\nELSE\n\tElseStatementList;\nEND_CASE;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertFOR ()
{
	InsertTextLines ("FOR Variable := StartValue TO EndValue BY Increment DO\n\tStatementList;\nEND_FOR;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertWHILE ()
{
	InsertTextLines ("WHILE Condition DO\n\tStatementList;\nEND_WHILE;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertREPEAT ()
{
	InsertTextLines ("REPEAT\n\tStatementList;\nUNTIL Condition\nEND_REPEAT;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertAssignment ()
{
	InsertTextLines ("Variable := Value;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertRETURN ()
{
	InsertTextLines ("RETURN;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertEXIT ()
{
	InsertTextLines ("EXIT;");
    RecreateCaret ();
}


void CSTEditorView::OnInsertVAR ()
{
	InsertTextLines ("VAR\nEND_VAR");
    RecreateCaret ();
}

void CSTEditorView::OnInsertCONST()
{
    InsertTextLines ("CONST\nEND_CONST");
    RecreateCaret ();
}

void CSTEditorView::OnInsertVARGLOBAL()
{
    InsertTextLines ("VAR_GLOBAL\nEND_VAR");
    RecreateCaret ();
}

void CSTEditorView::OnInsertVAREXTERNAL()
{
    InsertTextLines ("VAR_EXTERNAL\nEND_VAR");
    RecreateCaret ();
}


void CSTEditorView::OnInsertINITIALSTEP ()
{
    InsertTextLines ("INITIAL_STEP StepName :\n\tActionList;\nEND_STEP");
    RecreateCaret ();
}


void CSTEditorView::OnInsertSTEP ()
{
    InsertTextLines ("STEP StepName :\n\tActionList;\nEND_STEP");
    RecreateCaret ();
}


void CSTEditorView::OnInsertACTION ()
{
    InsertTextLines ("ACTION ActionName :\n\tStatementList;\nEND_ACTION");
    RecreateCaret ();
}


void CSTEditorView::OnInsertTRANSITION ()
{
    InsertTextLines ("TRANSITION TransitionName FROM StepList TO StepList\n\t:= Expression;\nEND_TRANSITION");
    RecreateCaret ();
}


// menu handler for IL editor
void CSTEditorView::OnInsertLD ()
{
	InsertTextLines ("\tLD \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertLDN ()
{
	InsertTextLines ("\tLDN \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertST ()
{
	InsertTextLines ("\tST \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertSTN ()
{
	InsertTextLines ("\tSTN \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertS ()
{
	InsertTextLines ("\tS \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertR ()
{
	InsertTextLines ("\tR \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertAND ()
{
	InsertTextLines ("\tAND \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertANDN ()
{
	InsertTextLines ("\tANDN \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertOR ()
{
	InsertTextLines ("\tOR \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertORN ()
{
	InsertTextLines ("\tORN \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertXOR ()
{
	InsertTextLines ("\tXOR \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertXORN ()
{
	InsertTextLines ("\tXORN \tBoolVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertADD ()
{
	InsertTextLines ("\tADD \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertSUB ()
{
	InsertTextLines ("\tSUB \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertMUL ()
{
	InsertTextLines ("\tMUL \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertDIV ()
{
	InsertTextLines ("\tDIV \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertGT ()
{
	InsertTextLines ("\tGT \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertGE ()
{
	InsertTextLines ("\tGE \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertEQ ()
{
	InsertTextLines ("\tEQ \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertNE ()
{
	InsertTextLines ("\tNE \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertLE ()
{
	InsertTextLines ("\tLE \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertLT ()
{
	InsertTextLines ("\tLT \tVariable");
    RecreateCaret ();
}


void CSTEditorView::OnInsertJMP ()
{
	InsertTextLines ("\tJMP \tLabel");
    RecreateCaret ();
}


void CSTEditorView::OnInsertJMPC ()
{
	InsertTextLines ("\tJMPC \tLabel");
    RecreateCaret ();
}


void CSTEditorView::OnInsertJMPCN ()
{
	InsertTextLines ("\tJMPCN \tLabel");
    RecreateCaret ();
}


void CSTEditorView::OnInsertCAL ()
{
	InsertTextLines ("\tCAL \tFunctionBlock");
    RecreateCaret ();
}


void CSTEditorView::OnInsertCALC ()
{
	InsertTextLines ("\tCALC \tFunctionBlock");
    RecreateCaret ();
}


void CSTEditorView::OnInsertCALCN ()
{
	InsertTextLines ("\tCALCN \tFunctionBlock");
    RecreateCaret ();
}


void CSTEditorView::OnInsertRET ()
{
	InsertTextLines ("\tRET");
    RecreateCaret ();
}


void CSTEditorView::OnInsertRETC ()
{
	InsertTextLines ("\tRETC");
    RecreateCaret ();
}


void CSTEditorView::OnInsertRETCN ()
{
	InsertTextLines ("\tRETCN");
    RecreateCaret ();
}

// AUTOFMT 10.08.06 SIS >>
PLINEDESC CSTEditorView::GetNextNonEmptyLine(PLINEDESC pLine, BOOL bIncludeCurrentLine /*=FALSE*/)
{
    int         iFirstChar;

    if(!bIncludeCurrentLine)
    {
        pLine = pLine->pNext;
    }
    while(pLine && !HasText(pLine, iFirstChar))
    {
        pLine = pLine->pNext;
    }
    return pLine;

}

void CSTEditorView::OnFormatSelection()
{
    FormatSelection();
}

BOOL CSTEditorView::FormatSelection()
{
    TEXTPOS tpStart;
    TEXTPOS tpEnd;
    GetSelection(tpStart, tpEnd);
    ASSERT(tpStart.pLine && tpEnd.pLine);
    if(!tpStart.pLine || !tpEnd.pLine)
    {
        return FALSE;
    }

    // if selection empty or only one line selected -> do nothing
    int iStartLine = tpStart.pLine->iLineNo;
    int iEndLine = tpEnd.pLine->iLineNo;
    if(iStartLine == iEndLine)
    {
        return FALSE;
    }

    SECEdit* pEdit = GetEdit();

    PLINEDESC   pLine = tpStart.pLine;
    PLINEDESC   pLineTemplate = NULL;
    int         iTargetColChars;
    int         iTargetCol;
    CString     sKeyword;
    CString     sKeywordTemplate;
    int         iIndent;
    
    pLine = GetNextNonEmptyLine(pLine, TRUE);
    if(!pLine || (pLine->iLineNo >= iEndLine))
    {
        return FALSE;
    }
    pLineTemplate = pLine;

    pEdit->SetUndoGroup(TRUE);
    if(pEdit->m_bKeepTabs)
    {
        HasText(pLineTemplate, iTargetColChars);
        iTargetCol = pEdit->GetTabbedOffset(pLineTemplate, iTargetColChars);
        AdaptIndent(pLineTemplate, iTargetCol, TRUE, TRUE);
    }

    while(pLine && (pLine->iLineNo < iEndLine))
    {
        iIndent = 0;

        pLine = GetNextNonEmptyLine(pLineTemplate);
        if(!pLine || (pLine->iLineNo >= iEndLine))
        {
            break;
        }
        
        HasText(pLineTemplate, iTargetColChars);
        iTargetCol = pEdit->GetTabbedOffset(pLineTemplate, iTargetColChars);

        // test for increase or decrease
        sKeywordTemplate = GetFirstKeyword(pLineTemplate);
        sKeyword = GetFirstKeyword(pLine);
        if(!IsCorrespondingStartKeyword(sKeyword, sKeywordTemplate))
        {
            if(IsIndentDecreaseKeyword(sKeyword))
            {
                iIndent--;
            }
            if(IsIndentIncreaseKeyword(sKeywordTemplate))
            {
                iIndent++;
            }
        }
        iTargetCol += (pEdit->m_iTabSize) * iIndent;

        AdaptIndent(pLine, iTargetCol, TRUE, TRUE);

        pLineTemplate = pLine;
    }
    pEdit->SetUndoGroup(FALSE);

    Invalidate();
	UpdateWindow();

    return TRUE;
}

void CSTEditorView::OnEditorSettings()
{
    CEditorSettingsDlg tDlg;
    SECEdit*    pEdit = GetEdit();
    CString     sExtension = pEdit->GetFileMappingKey();
    if(sExtension.IsEmpty())
    {
        return;
    }

    SECEditSettings tSettings(sExtension);
    CSTEditorApp* pApp = (CSTEditorApp*)AfxGetApp();

    pApp->SetSettingDefaults(tSettings);
    tDlg.SetDefaultEditSettings(tSettings);

    if(tDlg.DoModal() == IDOK)
    {
        pEdit->InitFileTypeSettings();
        Invalidate();
        UpdateWindow();

        CSTEditorDoc* pDoc = (CSTEditorDoc*)GetDocument();
        CDocTemplate* pDocTemplate = pDoc->GetDocTemplate();
        POSITION pos = pDocTemplate->GetFirstDocPosition();
        while(pos)
        {
            pDoc = (CSTEditorDoc*)pDocTemplate->GetNextDoc(pos);
            pDoc->UpdateAllViews(this, CSTEditorDoc::SettingsChangedHint);
        }
    }
}
// AUTOFMT 10.08.06 SIS <<

void CSTEditorView::InsertTextLines (CString strLines)
{
	SECEdit *pEdit;
	BOOL bSavedAutoIndent;
	bool bRequireIndent=false;
	int iIndentation;

    CPrgEditorDoc*  pDoc = GetDocument();
    pDoc->PrepareEdit();

	pEdit = GetEditor (m_EditorID);
	assert (pEdit != NULL);
	if (pEdit == NULL)
		return;

	pEdit->SetUndoGroup (TRUE);

    // caret is not positioned correctly
    int iColNo = pEdit->GetRealOffset(m_pCurLine, m_iColNo);

	bSavedAutoIndent = pEdit->m_bAutoIndent;
	pEdit->m_bAutoIndent = FALSE;

	iIndentation = m_iColNo;

	while (!strLines.IsEmpty ())
	{
		int iPos;
		CString strTabbedPart;

		iPos = strLines.Find (_T ('\n'));
		if (iPos == -1)
		{
			strTabbedPart = strLines;
			strLines.Empty ();
		}
		else
		{
			strTabbedPart = strLines.Left (iPos);
			strLines = strLines.Mid (iPos + 1);
		}

		if (bRequireIndent)
		{
			if (!strTabbedPart.IsEmpty () && iIndentation != 0)
			{
				CString strSpaces (_T (' '), iIndentation);

				InsertString ((LPTSTR)(LPCTSTR)strSpaces, TRUE, TRUE);
                iColNo += strSpaces.GetLength();
			}

			bRequireIndent = false;
		}

        if(pEdit->m_bKeepTabs)
        {
            InsertString((LPTSTR)(LPCTSTR)strTabbedPart, TRUE, TRUE);
            iColNo += strTabbedPart.GetLength();
        }
        else
        {
		    while (!strTabbedPart.IsEmpty ())
		    {
			    int iTabPos;
			    CString strPart;

			    iTabPos = strTabbedPart.Find (_T ('\t'));
			    if (iTabPos == -1)
			    {
				    strPart = strTabbedPart;
				    strTabbedPart.Empty ();
			    }
			    else
			    {
				    strPart = strTabbedPart.Left (iTabPos);
				    strTabbedPart = strTabbedPart.Mid (iTabPos + 1);
			    }

			    if (!strPart.IsEmpty ())
			    {
				    InsertString ((LPTSTR)(LPCTSTR)strPart, TRUE, TRUE);
                    iColNo += strPart.GetLength();
			    }

			    if (iTabPos != -1)
			    {
				    CString strSpaces (_T (' '), pEdit->m_iTabSize);

				    InsertString ((LPTSTR)(LPCTSTR)strSpaces, TRUE, TRUE);
                    iColNo += strSpaces.GetLength();
			    }
		    }
        }

        m_iColNo = pEdit->GetTabbedOffset(m_pCurLine, iColNo);

		if (iPos != -1)
		{
			SplitLine (TRUE, TRUE);
            iColNo = 0;
			bRequireIndent = true;
		}
	}

	pEdit->m_bAutoIndent = bSavedAutoIndent;
	pEdit->SetUndoGroup (FALSE);
}


void CSTEditorView::RecreateCaret ()
{

    // For strange reason the caret gets lost under some circumstances
    // (e.g. inserting text as a reaction to context menu selection).
    // This method is used to destroy and then again create the caret
    // which normally helps showing the caret where the caret should
    // already be visible.

    if (m_bHaveCaret)
    {
        DestroyCaret ();
        m_bHaveCaret = FALSE;
    }

    ::CreateCaret (m_hWnd, NULL, 2, m_iLineHeight);
    m_bHaveCaret = TRUE;

    ShowCaret ();

	MySetCaretPos();
}


BOOL CSTEditorView::ExtendContextMenu (CMenu *pMenu)
{
    CPrgEditorDoc *pDoc;
    DWORD dwStyle;
    SECEdit *pEdit;
    CStdEditorPosition position;
    bool bNeedSeparator=false;
    CString strConMenuText;

    MouseGoTo (m_ContextMenuPoint.x, m_ContextMenuPoint.y);

    pDoc = GetDocument ();
    pEdit = GetEditor (m_EditorID);

    if (!pDoc->CheckReadonly () && !pEdit->GetReadOnly())
    {
        strConMenuText.LoadString(IDS_CONMENU_EDIT_CUT);
        dwStyle = m_bSelecting ? MF_STRING : MF_GRAYED | MF_STRING;
        pMenu->AppendMenu (dwStyle, ID_EDIT_CUT, strConMenuText); 

        strConMenuText.LoadString(IDS_CONMENU_EDIT_COPY);
        dwStyle = m_bSelecting ? MF_STRING : MF_GRAYED | MF_STRING;
        pMenu->AppendMenu (dwStyle, ID_EDIT_COPY, strConMenuText); 

        if (OpenClipboard ())
        {
            if (IsClipboardFormatAvailable (CF_TEXT) || IsClipboardFormatAvailable (CF_OEMTEXT))
            {
                dwStyle = MF_STRING;
            }
            else
            {
                dwStyle = MF_GRAYED|MF_STRING;
            }

            CloseClipboard ();
        }
        else
        {
            dwStyle = MF_GRAYED | MF_STRING;
        }
        strConMenuText.LoadString(IDS_CONMENU_EDIT_PASTE);
        pMenu->AppendMenu (dwStyle, ID_EDIT_PASTE, strConMenuText); 

        strConMenuText.LoadString(IDS_CONMENU_EDIT_CLEAR);
        pMenu->AppendMenu (MF_STRING, ID_EDIT_CLEAR, strConMenuText); 

        bNeedSeparator = true;
    }

    if (!pDoc->CheckReadonly () && !pEdit->GetReadOnly())
    {
        CMenu subMenu;

        if (bNeedSeparator)
        {
            pMenu->AppendMenu (MF_SEPARATOR);
            bNeedSeparator = false;
        }

        if(m_Features.Chk(STF_POU_INS))
        {
            strConMenuText.LoadString(IDS_CONMENU_INSERT_FB_CALL);
            pMenu->AppendMenu (MF_STRING, IDM_INSERT_FB_CALL   , strConMenuText); 
            // last recently used pous
            CMenu subMenuLRU;
            CPrgEditorDoc *pDoc;
	        HRESULT hr;

	        pDoc = GetDocument ();
	        if (pDoc != NULL)  
            {   
                
	            hr = pDoc->GetLRUPous(m_listLruPous);
                if (hr!=S_OK || m_listLruPous.GetCount()==0)
                {
                    strConMenuText.LoadString(IDS_CONMENU_INSERT_LRU_FIRST);
                    pMenu->AppendMenu(MF_STRING | MF_GRAYED, IDM_INSERT_LRU_FIRST, strConMenuText); 
                }
                else
                {
                    if (subMenuLRU.CreatePopupMenu())
                    {
                        POSITION pos;
                        pos = m_listLruPous.GetHeadPosition();
                        int count = 0;
                        while(pos)
                        {
                            CString str = m_listLruPous.GetNext(pos);
                            subMenuLRU.AppendMenu(MF_STRING, IDM_INSERT_LRU_FIRST+count, str);
                            count++;
                            if (IDM_INSERT_LRU_FIRST+count>IDM_INSERT_LRU_LAST)
                            {
                                break;
                            }
                        }
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_LRU_FIRST);
                        pMenu->AppendMenu (MF_POPUP, (UINT)subMenuLRU.Detach (), strConMenuText);
                    }
                }
            }

            strConMenuText.LoadString(IDS_CONMENU_SHOW_VAR_SELECTION);
            pMenu->AppendMenu (MF_STRING, IDM_SHOW_VAR_SELECTION, strConMenuText); 
         
            strConMenuText.LoadString(IDS_CONMENU_SHOW_VAR_SEL_POPUP);
            pMenu->AppendMenu (MF_STRING, IDM_SHOW_VAR_SEL_POPUP, strConMenuText); 

            strConMenuText.LoadString(IDS_CONMENU_XREF_QUERY);
            pMenu->AppendMenu (MF_STRING, IDM_XREF_QUERY, strConMenuText); 
            strConMenuText.LoadString(IDS_CONMENU_XREF_LAST);
            pMenu->AppendMenu (MF_STRING, IDM_XREF_LAST, strConMenuText); 
            strConMenuText.LoadString(IDS_CONMENU_XREF_NEXT);
            pMenu->AppendMenu (MF_STRING, IDM_XREF_NEXT, strConMenuText); 
            
            bNeedSeparator = true;
        }

        


        if(m_Features.Chk(STF_DECL_INS) || m_Features.Chk(STF_STMT_INS))
        {
            if (subMenu.CreatePopupMenu ())
            {
                if(m_Features.Chk(STF_DECL_INS)) 
                {
                    strConMenuText.LoadString(IDS_CONMENU_INSERT_VAR);
                    subMenu.AppendMenu (MF_STRING, IDM_INSERT_VAR        , strConMenuText);
                    strConMenuText.LoadString(IDS_CONMENU_INSERT_CONST);
                    subMenu.AppendMenu (MF_STRING, IDM_INSERT_CONST      , strConMenuText);
                    strConMenuText.LoadString(IDS_CONMENU_INSERT_VAR_GLOBAL);
                    subMenu.AppendMenu (MF_STRING, IDM_INSERT_VAR_GLOBAL , strConMenuText); 
                    strConMenuText.LoadString(IDS_CONMENU_INSERT_VAR_EXTERNAL);
                    subMenu.AppendMenu (MF_STRING, IDM_INSERT_VAR_EXTERNAL,strConMenuText); 
                    strConMenuText.LoadString(IDS_CONMENU_INSERT_ASSIGNMENT);
                    subMenu.AppendMenu (MF_STRING, IDM_INSERT_ASSIGNMENT , strConMenuText); 
                }
                if(m_Features.Chk(STF_STMT_INS))
                {
                    CString strFile;
                    BOOL    bIsIL(FALSE);
                    strFile = pDoc->GetDocumentFile();      // AUTOFMT 08.08.06 SIS: GetPathName() returned empty string
                    if(strFile.GetLength() >= 3)
                    {
                        if (strFile.Right(3).CompareNoCase(_T(".il")) == 0)   // Instruction List Editor
                            bIsIL = TRUE;
                    }

                    if(bIsIL)
                    {
/***/
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_LD);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_LD    , strConMenuText);
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_LDN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_LDN   , strConMenuText);
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_ST);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_ST    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_STN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_STN   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_S);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_S     , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_R);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_R     , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_AND);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_AND   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_ANDN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_ANDN  , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_OR);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_OR    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_ORN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_ORN   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_XOR);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_XOR   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_XORN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_XORN  , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_ADD);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_ADD   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_SUB);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_SUB   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_MUL);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_MUL   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_DIV);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_DIV   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_GT);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_GT    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_GE);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_GE    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_EQ);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_EQ    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_NE);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_NE    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_LE);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_LE    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_LT);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_LT    , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_CAL);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_CAL   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_CALC);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_CALC  , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_CALCN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_CALCN , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_JMP);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_JMP   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_JMPC);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_JMPC  , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_JMPCN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_JMPCN , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_RET);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_RET   , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_RETC);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_RETC  , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_RETCN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_RETCN , strConMenuText); 

                        strConMenuText.LoadString(IDS_CONMENU_INSERT_IL_SYNTAX);
                    }
                    else
                    {
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_IF);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_IF         , strConMenuText);
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_CASE);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_CASE       , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_FOR);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_FOR        , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_WHILE);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_WHILE      , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_REPEAT);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_REPEAT     , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_RETURN);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_RETURN     , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_EXIT);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_EXIT       , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_INITIALSTEP);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_INITIALSTEP, strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_STEP);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_STEP       , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_ACTION);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_ACTION     , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_TRANSITION);
                        subMenu.AppendMenu (MF_STRING, IDM_INSERT_TRANSITION , strConMenuText); 
                        strConMenuText.LoadString(IDS_CONMENU_INSERT_ST_SYNTAX);
                    }
                }
                pMenu->AppendMenu (MF_POPUP, (UINT)subMenu.Detach (), strConMenuText); 
                bNeedSeparator = true;
            }
        }
    }

    // AUTOFMT 10.08.06 SIS >>
    if(m_Features.Chk(STF_AUTOFMT))
    {
        if (bNeedSeparator)
        {
            pMenu->AppendMenu (MF_SEPARATOR);
        }

        DWORD   dwStyle = MF_STRING;
        SECEdit*    pEdit = GetEdit();
        CString     sExtension = pEdit->GetFileMappingKey();
        if(sExtension.IsEmpty())
        {
            dwStyle |= MF_GRAYED;
        }

        strConMenuText.LoadString(IDS_CONMENU_EDITOR_SETTINGS);
        pMenu->AppendMenu (dwStyle, IDM_EDIT_SETTINGS, strConMenuText); 
        dwStyle = MF_STRING;
        CString sText = GetSelection();
        if(sText.IsEmpty())
        {
            dwStyle |= MF_GRAYED;
        }
        strConMenuText.LoadString(IDS_CONMENU_FORMATSEL);
        pMenu->AppendMenu (dwStyle, IDM_EDIT_FORMATSEL, strConMenuText); 

        bNeedSeparator = true;
    }
    // AUTOFMT 10.08.06 SIS <<

    if(m_Features.Chk(STF_USERHELP))
    {
        if (bNeedSeparator)
        {
            pMenu->AppendMenu (MF_SEPARATOR);
        }

        strConMenuText.LoadString(IDS_CONMENU_VIEW_OBJHELP);
        pMenu->AppendMenu (MF_STRING, IDM_VIEW_OBJHELP, strConMenuText); 

        bNeedSeparator = true;
    }

    if (m_Features.Chk(STF_MONITORING) && pDoc->CanMonitor ())
    {
        DWORD dwStyle1;
        DWORD dwStyle2;
        DWORD dwStyle3;

        if (bNeedSeparator)
        {
            pMenu->AppendMenu (MF_SEPARATOR);
        }

        dwStyle1 = FindWatchpointByPoint (m_ContextMenuPoint) != NULL ? MF_STRING : MF_GRAYED | MF_STRING;
        dwStyle2 = pDoc->GetFirstWPPosition () != NULL ? MF_STRING : MF_GRAYED | MF_STRING;
        dwStyle3 = pDoc->IsConnected () ? MF_STRING : MF_GRAYED | MF_STRING;

        strConMenuText.LoadString(IDS_CONMENU_SET_WATCHPOINT);
        pMenu->AppendMenu (MF_STRING, IDM_SET_WATCHPOINT       , strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_CLEAR_WATCHPOINT);
        pMenu->AppendMenu (dwStyle1 , IDM_CLEAR_WATCHPOINT     , strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_CLEAR_ALL_WATCHPOINTS);
        pMenu->AppendMenu (dwStyle2 , IDM_CLEAR_ALL_WATCHPOINTS, strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_WRITE_WATCHPOINT);
        pMenu->AppendMenu (dwStyle3 , IDM_WRITE_WATCHPOINT     , strConMenuText); 

        bNeedSeparator = true;
    }
        

    if (m_Features.Chk(STF_DEBUGGING) && pDoc->CanDebug () )
    {
        DWORD dwStyle1;
        DWORD dwStyle2;
        DWORD dwStyle3;

        if (bNeedSeparator)
        {
            pMenu->AppendMenu (MF_SEPARATOR);
        }

        CStdEditorPosition position(EDP_STLINE, -1, m_iLineNo);
        dwStyle1 = pDoc->LookupBreakpoint (position) != NULL ? MF_STRING : MF_GRAYED | MF_STRING;

        dwStyle2 = pDoc->GetFirstBPPosition () != NULL ? MF_STRING : MF_GRAYED | MF_STRING;

        dwStyle3 = pDoc->IsDebugConnected () ? MF_STRING : MF_GRAYED | MF_STRING;

        strConMenuText.LoadString(IDS_CONMENU_TOGGLE_BREAKPOINT);
        pMenu->AppendMenu (MF_STRING, IDM_TOGGLE_BREAKPOINT        , strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_REMOVE_ALL_BREAKPOINTS);
        pMenu->AppendMenu (dwStyle2 , IDM_REMOVE_ALL_BREAKPOINTS   , strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_TOGGLE_BREAKPOINT_ENABLED);
        pMenu->AppendMenu (dwStyle1 , IDM_TOGGLE_BREAKPOINT_ENABLED, strConMenuText);

        pMenu->AppendMenu (MF_SEPARATOR);

        dwStyle = GetSFCMenuStyle (IDM_TOGGLE_MANUAL_MODE);
        strConMenuText.LoadString(IDS_CONMENU_TOGGLE_MANUAL_MODE);
        pMenu->AppendMenu (dwStyle, IDM_TOGGLE_MANUAL_MODE, strConMenuText); 

        pMenu->AppendMenu (MF_SEPARATOR);

        strConMenuText.LoadString(IDS_CONMENU_CONTINUE_TASK);
        pMenu->AppendMenu (dwStyle3, IDM_CONTINUE_TASK, strConMenuText); 

        strConMenuText.LoadString(IDS_CONMENU_STEP_INTO);
        pMenu->AppendMenu (dwStyle3, IDM_STEP_INTO, strConMenuText); 

        strConMenuText.LoadString(IDS_CONMENU_STEP_OVER);
        pMenu->AppendMenu (dwStyle3, IDM_STEP_OVER, strConMenuText); 

        strConMenuText.LoadString(IDS_CONMENU_STEP_OUT);
        pMenu->AppendMenu (dwStyle3, IDM_STEP_OUT, strConMenuText); 

        strConMenuText.LoadString(IDS_CONMENU_PERFORM_CYCLE);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_PERFORM_CYCLE), IDM_PERFORM_CYCLE, strConMenuText); 

        ExtendMenuBySFCItems (pMenu);

        bNeedSeparator = true;
    }

    if (!pDoc->CheckReadonly () && !pEdit->GetReadOnly())
    {
        if (bNeedSeparator)
        {
            pMenu->AppendMenu (MF_SEPARATOR);
        }

        strConMenuText.LoadString(IDS_CONMENU_BOOKMARKSET);
        pMenu->AppendMenu (MF_STRING, ID_BOOKMARKSET , strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_BOOKMARKNEXT);
        pMenu->AppendMenu (MF_STRING, ID_BOOKMARKNEXT, strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_BOOKMARKPREV);
        pMenu->AppendMenu (MF_STRING, ID_BOOKMARKPREV, strConMenuText);

        bNeedSeparator = true;
    }

    return (TRUE);
}


void CSTEditorView::ExtendMenuBySFCItems (CMenu *pMenu)
{
    CPrgEditorDoc *pDoc;
    CString strConMenuText;

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return;
    }

    pMenu->AppendMenu (MF_SEPARATOR);

    if (pDoc->IsTransitionPosition (-(m_iColNo + 1), m_iLineNo))
    {
        strConMenuText.LoadString(IDS_CONMENU_FORCE_TRANSITION_ONCE);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_FORCE_TRANSITION_ONCE), IDM_FORCE_TRANSITION_ONCE, strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_FORCE_TRANSITION);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_FORCE_TRANSITION)     , IDM_FORCE_TRANSITION     , strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_BLOCK_TRANSITION);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_BLOCK_TRANSITION)     , IDM_BLOCK_TRANSITION     , strConMenuText); 
    }

    strConMenuText.LoadString(IDS_CONMENU_FORCE_ALL_TRANSITIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_FORCE_ALL_TRANSITIONS) , IDM_FORCE_ALL_TRANSITIONS , strConMenuText); 
    strConMenuText.LoadString(IDS_CONMENU_BLOCK_ALL_TRANSITIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_BLOCK_ALL_TRANSITIONS) , IDM_BLOCK_ALL_TRANSITIONS , strConMenuText); 
    strConMenuText.LoadString(IDS_CONMENU_REVERT_ALL_TRANSITIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_REVERT_ALL_TRANSITIONS), IDM_REVERT_ALL_TRANSITIONS, strConMenuText);

#if 0
    strConMenuText.LoadString(IDS_CONMENU_GLOBALLY_FORCE_ALL_TRANSITIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_GLOBALLY_FORCE_ALL_TRANSITIONS), IDM_GLOBALLY_FORCE_ALL_TRANSITIONS, strConMenuText); 
#endif

    pMenu->AppendMenu (MF_SEPARATOR);

    if (pDoc->IsActionPosition (-(m_iColNo + 1), m_iLineNo))
    {
        strConMenuText.LoadString(IDS_CONMENU_FORCE_ACTION_ONCE);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_FORCE_ACTION_ONCE), IDM_FORCE_ACTION_ONCE, strConMenuText); 
        strConMenuText.LoadString(IDS_CONMENU_FORCE_ACTION);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_FORCE_ACTION)     , IDM_FORCE_ACTION     , strConMenuText);
        strConMenuText.LoadString(IDS_CONMENU_BLOCK_ACTION);
        pMenu->AppendMenu (GetSFCMenuStyle (IDM_BLOCK_ACTION)     , IDM_BLOCK_ACTION     , strConMenuText); 
    }

    strConMenuText.LoadString(IDS_CONMENU_FORCE_ALL_ACTIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_FORCE_ALL_ACTIONS) , IDM_FORCE_ALL_ACTIONS , strConMenuText); 
    strConMenuText.LoadString(IDS_CONMENU_BLOCK_ALL_ACTIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_BLOCK_ALL_ACTIONS) , IDM_BLOCK_ALL_ACTIONS , strConMenuText); 
    strConMenuText.LoadString(IDS_CONMENU_REVERT_ALL_ACTIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_REVERT_ALL_ACTIONS), IDM_REVERT_ALL_ACTIONS, strConMenuText); 

#if 0
    strConMenuText.LoadString(IDS_CONMENU_GOBALLY_BLOCK_ALL_ACTIONS);
    pMenu->AppendMenu (GetSFCMenuStyle (IDM_GLOBALLY_BLOCK_ALL_ACTIONS), IDM_GLOBALLY_BLOCK_ALL_ACTIONS, strConMenuText); 
#endif
}

// AUTOFMT 07.08.06 SIS >>
BOOL CSTEditorView::IsIndentIncreaseKeyword(const CString& crsKeyword)
{
    if(crsKeyword.CompareNoCase(_T("IF")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("ELSE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("ELSIF")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("FOR")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("WHILE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("REPEAT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("CASE")) == 0)
    {
        return TRUE;
    }
	// #326 13.12.06 SIS >>
    if(crsKeyword.CompareNoCase(_T("STEP")) == 0)
    {
        return TRUE;
    }
	// #326 13.12.06 SIS <<
    if(crsKeyword.CompareNoCase(_T("INITIAL_STEP")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("ACTION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("TRANSITION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_INPUT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_OUTPUT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_IN_OUT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_GLOBAL")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_EXTERNAL")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_ACCESS")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_TEMP")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_IN_EXT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("VAR_CONFIG")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("CONFIGURATION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("CONST")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("FIELDBUS")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("FUNCTION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("FUNCTION_BLOCK")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("HW_DEFINITION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("NATIVE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("OPC_CONFIG")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("PC")) == 0)
    {
        return TRUE;
    }
// PROGRAM is also used in resource as one line statement
//    if(crsKeyword.CompareNoCase(_T("PROGRAM")) == 0)
//    {
//        return TRUE;
//    }
    if(crsKeyword.CompareNoCase(_T("RESOURCE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("STRUCT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("SYNCHRONIZED")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("TYPE")) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL CSTEditorView::IsIndentDecreaseKeyword(const CString& crsKeyword)
{
    if(crsKeyword.CompareNoCase(_T("END_IF")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("ELSE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("ELSIF")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_FOR")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_WHILE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("UNTIL")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_REPEAT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_CASE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_STEP")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_ACTION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_TRANSITION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_VAR")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_CONFIGURATION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_CONST")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_FIELDBUS")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_FUNCTION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_FUNCTION_BLOCK")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_HW_DEFINITION")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_NATIVE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_OPC_CONFIG")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_PC")) == 0)
    {
        return TRUE;
    }
// PROGRAM is also used in resource as one line statement
//    if(crsKeyword.CompareNoCase(_T("END_PROGRAM")) == 0)
//    {
//        return TRUE;
//    }
    if(crsKeyword.CompareNoCase(_T("END_RESOURCE")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_STRUCT")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_SYNCHRONIZED")) == 0)
    {
        return TRUE;
    }
    if(crsKeyword.CompareNoCase(_T("END_TYPE")) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * IsCorrespondingStartKeyword
 *
 *  is keyword start keyword to some other keyword
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CSTEditorView::IsCorrespondingStartKeyword(const CString& crsKeyword, const CString& crsKeywordTest)
{
    // general rule
    // END_XXX always corresponds to XXX
    if(crsKeyword.GetLength() > 4 && crsKeyword.Left(4).CompareNoCase(_T("END_")) == 0)
    {
        CString sTest = crsKeyword.Mid(4);
        if(sTest.CompareNoCase(crsKeywordTest) == 0)
        {
            return TRUE;
        }
    }
    if(crsKeyword.CompareNoCase(_T("END_IF")) == 0)
    {
        if(crsKeywordTest.CompareNoCase(_T("ELSE")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("ELSIF")) == 0)
        {
            return TRUE;
        }
    }
    if(crsKeyword.CompareNoCase(_T("ELSE")) == 0)
    {
        return (crsKeywordTest.CompareNoCase(_T("IF")) == 0);
    }
    if(crsKeyword.CompareNoCase(_T("UNTIL")) == 0)
    {
        return (crsKeywordTest.CompareNoCase(_T("REPEAT")) == 0);
    }
    if(crsKeyword.CompareNoCase(_T("END_REPEAT")) == 0)
    {
        return (crsKeywordTest.CompareNoCase(_T("UNTIL")) == 0);
    }
    if(crsKeyword.CompareNoCase(_T("END_CASE")) == 0)
    {
        return (crsKeywordTest.CompareNoCase(_T("ELSE")) == 0);
    }
    if(crsKeyword.CompareNoCase(_T("END_STEP")) == 0)
    {
        return (crsKeywordTest.CompareNoCase(_T("INITIAL_STEP")) == 0);
    }
    if(crsKeyword.CompareNoCase(_T("END_VAR")) == 0)
    {
        if(crsKeywordTest.CompareNoCase(_T("VAR_INPUT")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_OUTPUT")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_IN_OUT")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_GLOBAL")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_EXTERNAL")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_ACCESS")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_TEMP")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_IN_EXT")) == 0)
        {
            return TRUE;
        }
        else if(crsKeywordTest.CompareNoCase(_T("VAR_CONFIG")) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// AUTOFMT 14.08.06 SIS >>
//------------------------------------------------------------------*
/**
 * GetFirstKeyword
 *
 *  get first keyword in line
 *
 * @param           pLine: line descriptor
 * @exception       -
 * @see             -
*/
CString CSTEditorView::GetFirstKeyword(PLINEDESC pLine)
{
    CString sKeyWord;
    int     iStart, iEnd;

    SECEdit* pEdit = GetEdit();
    ASSERT(pEdit != NULL);
	LPSTR	pszKeyword = pLine->pText;

    if(pEdit->KeyWord(&pszKeyword, &iStart, &iEnd))
    {
        LPTSTR lpszBuf = sKeyWord.GetBuffer(iEnd - iStart + 1);
        _tcsncpy(lpszBuf, pLine->pText+iStart, iEnd - iStart);
		lpszBuf[iEnd - iStart] = 0;
        sKeyWord.ReleaseBuffer();
		sKeyWord.TrimRight();
    }

    return sKeyWord;
}

//------------------------------------------------------------------*
/**
 * CheckDecreaseIndent
 *
 *  auto format: check line for decreasing indentation
 *
 * @param           pLine: line to be checked
 * @param           bSaveUndoRecord: save undo record?
 * @return          -
 * @exception       -
 * @see             AutoIndent()
*/
void CSTEditorView::CheckDecreaseIndent(PLINEDESC pLine, BOOL bSaveUndoRecord)
{
    // run backwards to get last line with text = template line
    PLINEDESC   pLineTemplate = pLine->pPrev;
    int         iTargetColChars = 0;
    while (pLineTemplate && !HasText(pLineTemplate,iTargetColChars))
    {
        pLineTemplate = pLineTemplate->pPrev;
    }

    if (!pLineTemplate)
    {
        return;
    }

    CString sKeyword = GetFirstKeyword(pLine);
    if(!IsIndentDecreaseKeyword(sKeyword))
    {
        return;
    }

    SECEdit* pEdit = GetEdit();
    ASSERT(pEdit != NULL);
    
    int     iIndent = -1;
    CString sKeywordTest = GetFirstKeyword(pLineTemplate);
    if(IsCorrespondingStartKeyword(sKeyword, sKeywordTest))
    {
        iIndent = 0;
    }

    // get column index of first non space character of template line
    int iTargetCol = pEdit->GetTabbedOffset(pLineTemplate, iTargetColChars);

    iTargetCol += (pEdit->m_iTabSize) * iIndent;

    AdaptIndent(pLine, iTargetCol, bSaveUndoRecord);
}

//------------------------------------------------------------------*
/**
 * CheckIncreaseIndent
 *
 *  auto format: check line for increasing indentation
 *
 * @param           pLine: line to be checked
 * @param           bSaveUndoRecord: save undo record?
 * @return          -
 * @exception       -
 * @see             AutoIndent()
*/
void CSTEditorView::CheckIncreaseIndent(PLINEDESC pLine, BOOL bSaveUndoRecord)
{
    // run backwards to get last line with text = template line
    PLINEDESC   pLineTemplate = pLine->pPrev;
    int         iTargetColChars = 0;
    while (pLineTemplate && !HasText(pLineTemplate,iTargetColChars))
    {
        pLineTemplate = pLineTemplate->pPrev;
    }

    if (!pLineTemplate)
    {
        return;
    }

	int	iIndent = 0;
    CString sKeyword = GetFirstKeyword(pLineTemplate);
    if(IsIndentIncreaseKeyword(sKeyword))
    {
	    iIndent = 1;
    }

    SECEdit* pEdit = GetEdit();
    ASSERT(pEdit != NULL);

    // get column index of first non space character of template line
    int iTargetCol = pEdit->GetTabbedOffset(pLineTemplate, iTargetColChars);

    iTargetCol += (pEdit->m_iTabSize) * iIndent;

    AdaptIndent(pLine, iTargetCol, bSaveUndoRecord);
}


void CSTEditorView::AdaptIndent(PLINEDESC pLine, int iTargetCol, BOOL bSaveUndoRecord, BOOL bFormatMode /*=FALSE*/)
{
    if(pLine == NULL)
    {
        return;
    }
    
    SECEdit* pEdit = GetEdit();
    ASSERT(pEdit != NULL);

	if(iTargetCol < 0)
	{
		iTargetCol = 0;
	}

    // insert spaces into current line
    int     iFirstChars;
    int     iFirst;
    BOOL    bConvertIndent = FALSE;
    if (HasText(pLine, iFirstChars))
    {
        iFirst = pEdit->GetTabbedOffset(pLine,iFirstChars);

        if(bFormatMode)
        {
            if(pEdit->m_bKeepTabs)
            {
                // if format mode -> set target column to multiple of tab size
                // this avoids inserting of spaces
                if(pEdit->m_iTabSize > 0)
                {
                    int iMinChars = iFirst / pEdit->m_iTabSize + iFirst % pEdit->m_iTabSize;
                    if(iMinChars != iFirstChars)
                    {
                        bConvertIndent = TRUE;
                    }
                    iTargetCol = iTargetCol / pEdit->m_iTabSize * pEdit->m_iTabSize;
                }
            }
            else
            {
                if(iFirstChars != iFirst)
                {
                    bConvertIndent = TRUE;
                }
            }
        }

        if(iFirst != iTargetCol || bConvertIndent)
        {
            TEXTPOS pos;
            pos.pLine = pLine;
            pos.iOffset = 0;

            pEdit->DeleteString(&pos, iFirstChars, bSaveUndoRecord);
        
            CString str;
            LPTSTR p = str.GetBuffer(iTargetCol);
            if (pEdit->m_bKeepTabs)
            {
                int iNumTabs = iTargetCol / pEdit->m_iTabSize;
                int iNumSpaces = iTargetCol % pEdit->m_iTabSize;
                memset(p,'\t',iNumTabs);
                memset(&p[iNumTabs],' ',iNumSpaces);
                p[iNumTabs+iNumSpaces] = 0;
            }
            else
            {
                memset(p,' ',iTargetCol);
                p[iTargetCol] = 0;
            }
            if (p[0])
            {
                pEdit->InsertString(&pos,T2A(p),TRUE,bSaveUndoRecord);
            }
            str.ReleaseBuffer();
        }
    }
    m_iColNo = iTargetCol;
}

void CSTEditorView::AutoIndent(BOOL bSaveUndoRecord)
{
    USES_CONVERSION_T;
    
    SECEdit* pEdit = GetEdit();
    ASSERT(pEdit != NULL);
    
    // first check indentation of previous line
    CheckDecreaseIndent(m_pCurLine->pPrev, bSaveUndoRecord);
    // then check indentation of current line
    CheckIncreaseIndent(m_pCurLine, bSaveUndoRecord);
}


// AUTOFMT 07.08.06 SIS <<



void CSTEditorView::GetSFCMenuProperties (UINT uiId, bool &bEnabled, bool &bChecked)
{
    CPrgEditorDoc *pDoc;
    bool bInManualMode;
    bool bInCycle;

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        bEnabled = false;
        bChecked = false;
        return;
    }

    if (!pDoc->CanDebug () || !pDoc->IsConnected ())
    {
        bEnabled = false;
        bChecked = false;
        return;
    }

    switch (uiId)
    {
    case IDM_TOGGLE_MANUAL_MODE:
        bEnabled = pDoc->InManualMode (bInManualMode);
        bChecked = bEnabled && bInManualMode;
        break;

    case IDM_PERFORM_CYCLE:
        bEnabled = pDoc->InManualMode (bInManualMode) && bInManualMode &&
                   pDoc->InSFCCycle (bInCycle) && !bInCycle;
        bChecked = false;
        break;

    case IDM_FORCE_TRANSITION_ONCE:
        bEnabled = true;
        bChecked = pDoc->IsTransitionForcedOnce (-(m_iColNo + 1), m_iLineNo);
        break;

    case IDM_FORCE_TRANSITION:
        bEnabled = true;
        bChecked = pDoc->IsTransitionForcedPermanently (-(m_iColNo + 1), m_iLineNo);
        break;

    case IDM_BLOCK_TRANSITION:
        bEnabled = true;
        bChecked = pDoc->IsTransitionBlocked (-(m_iColNo + 1), m_iLineNo);
        break;

    case IDM_FORCE_ALL_TRANSITIONS:
        bEnabled = true;
        bChecked = false;
        break;

    case IDM_BLOCK_ALL_TRANSITIONS:
        bEnabled = true;
        bChecked = false;
        break;

    case IDM_REVERT_ALL_TRANSITIONS:
        bEnabled = true;
        bChecked = false;
        break;

    case IDM_GLOBALLY_FORCE_ALL_TRANSITIONS:
        bEnabled = true;
        bChecked = pDoc->AreAllTransitionsForcedGlobally ();
        break;

    case IDM_FORCE_ACTION_ONCE:
        bEnabled = true;
        bChecked = pDoc->IsActionForcedOnce (-(m_iColNo + 1), m_iLineNo);
        break;

    case IDM_FORCE_ACTION:
        bEnabled = true;
        bChecked = pDoc->IsActionForcedPermanently (-(m_iColNo + 1), m_iLineNo);
        break;

    case IDM_BLOCK_ACTION:
        bEnabled = true;
        bChecked = pDoc->IsActionBlocked (-(m_iColNo + 1), m_iLineNo);
        break;

    case IDM_FORCE_ALL_ACTIONS:
        bEnabled = true;
        bChecked = false;
        break;

    case IDM_BLOCK_ALL_ACTIONS:
        bEnabled = true;
        bChecked = false;
        break;

    case IDM_REVERT_ALL_ACTIONS:
        bEnabled = true;
        bChecked = false;
        break;

    case IDM_GLOBALLY_BLOCK_ALL_ACTIONS:
        bEnabled = true;
        bChecked = pDoc->AreAllActionsBlockedGlobally ();
        break;
    }
}


DWORD CSTEditorView::GetSFCMenuStyle (UINT uiId)
{
    bool bEnabled;
    bool bChecked;
    DWORD dwStyle;

    GetSFCMenuProperties (uiId, bEnabled, bChecked);

    dwStyle = MF_STRING;

    if (!bEnabled)
    {
        dwStyle |= MF_GRAYED;
    }

    if (bChecked)
    {
        dwStyle |= MF_CHECKED;
    }

    return (dwStyle);
}


void CSTEditorView::OnUpdateSFCMenu (CCmdUI *pCmdUI)
{
    bool bEnabled;
    bool bChecked;

    GetSFCMenuProperties (pCmdUI->m_nID, bEnabled, bChecked);

    pCmdUI->Enable (bEnabled);
    pCmdUI->SetCheck (bChecked);
}


void CSTEditorView::SubClassDraw (CDC *pDC, CRect drawArea)
{
    DrawActionInfos (pDC, drawArea);
    DrawTransitionInfos (pDC, drawArea);
    DrawWatchpoints (pDC, drawArea);
}


void CSTEditorView::GetSubclassFlags (PLINEDESC pLine, char *pchFlags)
{
    CPrgEditorDoc *pDoc;

    if (pLine == NULL)
    {
        SECEditView::GetSubclassFlags (pLine, pchFlags);
        return;
    }

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        SECEditView::GetSubclassFlags (pLine, pchFlags);
        return;
    }

    if (pLine->bStep)
    {
        CList<CPrgEdStep *, CPrgEdStep *> steps;
        POSITION pPosition;

        pDoc->GetStepsAtLine (steps, pLine->iLineNo);

        pPosition = steps.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdStep *pStep;
            bool bActive=false;
            CSTStepClientData *pClientData;
            int iKeywordColumn;
            int iColumn;

            pStep = steps.GetNext (pPosition);
            ASSERT (pStep != NULL);
            if (pStep == NULL)
                continue;

            if (!pStep->IsActive ())
                continue;

            pClientData = (CSTStepClientData *)pStep->GetClientData ();
            ASSERT (pClientData != NULL);
            if (pClientData == NULL)
                continue;

            iKeywordColumn = -GetEditor(m_EditorID)->ToOffsetPosition (pClientData->GetKeywordPosition ()).GetX ();
            for (iColumn = iKeywordColumn; iColumn < pLine->iTxtSize - 2; ++iColumn)
            {
                if (!isalpha (pLine->pText[iColumn]) && pLine->pText[iColumn] != '_')
                    break;

                pchFlags[iColumn] = 0x10;
            }
        }
    }

    if (pLine->bAction)
    {
        CList<CPrgEdAction *, CPrgEdAction *> actions;
        POSITION pPosition;

        pDoc->GetActionsAtLine (actions, pLine->iLineNo);

        pPosition = actions.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdAction *pAction;
            bool bActive=false;
            bool bForced=false;
            bool bBlocked=false;
            CSTActionClientData *pClientData;
            int iKeywordColumn;
            int iColumn;

            pAction = actions.GetNext (pPosition);
            ASSERT (pAction != NULL);
            if (pAction == NULL)
                continue;

            if (pAction->IsActive ())
            {
                bActive = true;
            }

            if (pAction->IsForced ())
            {
                bForced = true;
            }
            else if (pDoc->AreAllActionsBlockedGlobally () || pAction->IsBlocked ())
            {
                bBlocked = true;
            }

            if (!bActive && !bForced && !bBlocked)
                continue;

            pClientData = (CSTActionClientData *)pAction->GetClientData ();
            ASSERT (pClientData != NULL);
            if (pClientData == NULL)
                continue;

            iKeywordColumn = -GetEditor(m_EditorID)->ToOffsetPosition (pClientData->GetKeywordPosition ()).GetX ();
            for (iColumn = iKeywordColumn; iColumn < pLine->iTxtSize - 2; ++iColumn)
            {
                if (!isalpha (pLine->pText[iColumn]) && pLine->pText[iColumn] != '_')
                    break;

                if (bForced)
                {
                    pchFlags[iColumn] = 1;
                }
                else if (bBlocked)
                {
                    pchFlags[iColumn] = 2;
                }

                if (bActive)
                {
                    pchFlags[iColumn] |= 0x10;
                }
            }
        }
    }

    if (pLine->bTransition)
    {
        CList<CPrgEdTransition *, CPrgEdTransition *> transitions;
        POSITION pPosition;

        pDoc->GetTransitionsAtLine (transitions, pLine->iLineNo);

        pPosition = transitions.GetHeadPosition ();
        while (pPosition != NULL)
        {
            CPrgEdTransition *pTransition;
            bool bForced=false;
            bool bBlocked=false;
            CSTTransitionClientData *pClientData;
            int iKeywordColumn;
            int iColumn;

            pTransition = transitions.GetNext (pPosition);
            ASSERT (pTransition != NULL);
            if (pTransition == NULL)
                continue;

            if (pDoc->AreAllTransitionsForcedGlobally () || pTransition->IsForced ())
            {
                bForced = true;
            }
            else if (pTransition->IsBlocked ())
            {
                bBlocked = true;
            }

            if (!bForced && !bBlocked)
                continue;

            pClientData = (CSTTransitionClientData *)pTransition->GetClientData ();
            ASSERT (pClientData != NULL);
            if (pClientData == NULL)
                continue;

            iKeywordColumn = -GetEditor(m_EditorID)->ToOffsetPosition (pClientData->GetKeywordPosition ()).GetX ();
            for (iColumn = iKeywordColumn; iColumn < pLine->iTxtSize - 2; ++iColumn)
            {
                if (!isalpha (pLine->pText[iColumn]) && pLine->pText[iColumn] != '_')
                    break;

                if (bForced)
                {
                    pchFlags[iColumn] = 1;
                }
                else if (bBlocked)
                {
                    pchFlags[iColumn] = 2;
                }
            }
        }
    }

    SECEditView::GetSubclassFlags (pLine, pchFlags);
}


bool CSTEditorView::OverrideTextForeground (char chFlag, COLORREF &foregroundColor)
{
    if ((chFlag & 0xf) == 1)
    {
        foregroundColor = RGB (0, 128, 0);
        return (true);
    }

    if ((chFlag & 0xf) == 2)
    {
        foregroundColor = RGB (255, 0, 0);
        return (true);
    }

    return (SECEditView::OverrideTextForeground (chFlag, foregroundColor));
}


bool CSTEditorView::OverrideTextBackground (char chFlag, COLORREF &backgroundColor)
{
    if ((chFlag & 0xf0) == 0x10)
    {
        backgroundColor = RGB (200, 200, 200);
        return (true);
    }

    return (SECEditView::OverrideTextBackground (chFlag, backgroundColor));
}


COLORREF CSTEditorView::GetGutterColor (COLORREF clrEmpty)
{
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return (SECEditView::GetGutterColor (clrEmpty));
    }

    if (!pDoc->IsSFCDebugging ())
    {
        return (SECEditView::GetGutterColor (clrEmpty));
    }

    return (RGB (165, 206, 247));
}


void CSTEditorView::DrawWatchpoints (CDC *pDC, CRect drawArea)
{
   CPrgEditorDoc *pDoc;
   CArray<CWatchpoint *, CWatchpoint *> watchpoints;
   int iIndex;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }
   
   pDoc->ZOrderWatchpoints (watchpoints);
   
   for (iIndex = watchpoints.GetSize () - 1; iIndex >= 0; --iIndex)
   {
      DrawWatchpoint (pDC, drawArea, watchpoints.GetAt (iIndex));
   }
   
   PostMessage (SUSPEND_RESUME_WATCHPOINTS);
}


void CSTEditorView::DrawWatchpoint (CDC *pDC, CRect drawArea, CWatchpoint *pWP)
{
	CPrgWPClientData *pClientData;
	CBrush brush (RGB (0, 0, 0));
	CFont *pOldFont;
	CRect clipRect;
	CRect rect;
	CRect varRect;

	pClientData = (CPrgWPClientData *)pWP->GetClientData ();
	assert (pClientData != NULL);
	if (pClientData == NULL)
	{
		return;
	}

	if(pWP->GetPosition().GetPosQual() != EDP_STLINE)  // skip watchpoints of different context
	{
		return;
	}

	UpdateWatchpointExtent (pWP, pDC);

	if (!UpdateWatchpointVisibility (pClientData, drawArea))
	{
		return;
	}

	if (m_bHaveWPFont)
	{
		pOldFont = pDC->SelectObject (&m_WatchpointFont);
	}

	rect = pClientData->GetArea ();
	WPAreaToClientArea (rect);

	clipRect = drawArea;
	clipRect.left += m_iMarkSpace;
	pDC->IntersectClipRect (&clipRect);

	pDC->FillSolidRect (&rect, pClientData->GetBackgroundColor ());

	pDC->Rectangle (&rect);

	pDC->SetTextColor (RGB (0, 0, 0));
	pDC->TextOut (rect.left + WPHorizontalMargin, rect.top + WPVerticalMargin, pClientData->GetText ());

    if (pClientData->GetCrossedOut ())
    {
        CPen pen (PS_SOLID, 1, RGB (255, 0, 0));
        CPen *pOldPen;

        pOldPen = pDC->SelectObject (&pen);

        pDC->MoveTo (rect.left, rect.top);
        pDC->LineTo (rect.right - 1, rect.bottom - 1);

        pDC->MoveTo (rect.right - 1, rect.top);
        pDC->LineTo (rect.left, rect.bottom - 1);

        pDC->SelectObject (pOldPen);
    }

	::SelectClipRgn (pDC->m_hDC, NULL);

	if (m_bHaveWPFont)
	{
		pDC->SelectObject (pOldFont);
	}
}


void CSTEditorView::DrawActionInfos (CDC *pDC, CRect drawArea)
{
	CPrgEditorDoc *pDoc;
    CList<CPrgEdAction *, CPrgEdAction *> actions;
    POSITION pPosition;

	pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return;
    }

    if (pDoc->AreAllActionsBlockedGlobally ())
    {
        pDoc->GetAllActions (actions);
    }
    else
    {
        pDoc->GetActions (actions, CPrgEditorDoc::ActionForcedOnce);
        pDoc->GetActions (actions, CPrgEditorDoc::ActionForcedPermanently);
        pDoc->GetActions (actions, CPrgEditorDoc::ActionBlocked);
    }

    pPosition = actions.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdAction *pAction;

        pAction = actions.GetNext (pPosition);
        ASSERT (pAction != NULL);
        if (pAction == NULL)
            continue;

        DrawActionInfo (pDC, drawArea, pAction);
    }
}


void CSTEditorView::DrawActionInfo (CDC *pDC, CRect drawArea, CPrgEdAction *pAction)
{
    CSTActionClientData *pClientData;
    CStdEditorPosition position;
    CString strText;
    CPrgEditorDoc *pDoc;

    pClientData = (CSTActionClientData *)pAction->GetClientData ();
    ASSERT (pClientData != NULL);
    if (pClientData == NULL)
        return;

    pDoc = GetDocument ();
    if (pDoc == NULL)
        return;

    position = pClientData->GetBehindIdPosition ();

    if (pDoc->AreAllActionsBlocked ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_BLOCKED); 
    }
    else if (pAction->IsForcedOnce ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_FORCED); 
    }
    else if (pAction->IsForcedPermanently ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_FORCED_PERMANENTLY); 
    }
    else if (pAction->IsBlocked ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_BLOCKED); 
    }

    DrawSFCInfo (pDC, drawArea, position, strText);
}


void CSTEditorView::DrawTransitionInfos (CDC *pDC, CRect drawArea)
{
	CPrgEditorDoc *pDoc;
    CList<CPrgEdTransition *, CPrgEdTransition *> transitions;
    POSITION pPosition;

	pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return;
    }

    if (pDoc->AreAllTransitionsForcedGlobally ())
    {
        pDoc->GetAllTransitions (transitions);
    }
    else
    {
        pDoc->GetTransitions (transitions, CPrgEditorDoc::TransitionForcedOnce);
        pDoc->GetTransitions (transitions, CPrgEditorDoc::TransitionForcedPermanently);
        pDoc->GetTransitions (transitions, CPrgEditorDoc::TransitionBlocked);
    }

    pPosition = transitions.GetHeadPosition ();
    while (pPosition != NULL)
    {
        CPrgEdTransition *pTransition;

        pTransition = transitions.GetNext (pPosition);
        ASSERT (pTransition != NULL);
        if (pTransition == NULL)
            continue;

        DrawTransitionInfo (pDC, drawArea, pTransition);
    }
}


void CSTEditorView::DrawTransitionInfo (CDC *pDC, CRect drawArea, CPrgEdTransition *pTransition)
{
    CSTTransitionClientData *pClientData;
    CStdEditorPosition position;
    CString strText;
    CPrgEditorDoc *pDoc;

    pClientData = (CSTTransitionClientData *)pTransition->GetClientData ();
    ASSERT (pClientData != NULL);
    if (pClientData == NULL)
        return;

    pDoc = GetDocument ();
    if (pDoc == NULL)
        return;

    position = pClientData->GetBehindIdPosition ();

    if (pDoc->AreAllTransitionsForcedGlobally ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_FORCED_PERMANENTLY); 
    }
    if (pTransition->IsForcedOnce ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_FORCED); 
    }
    else if (pTransition->IsForcedPermanently ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_FORCED_PERMANENTLY);
    }
    else if (pTransition->IsBlocked ())
    {
        strText.LoadString(IDS_SFC_ACTION_TEXT_BLOCKED);
    }

    DrawSFCInfo (pDC, drawArea, position, strText);
}


void CSTEditorView::DrawSFCInfo (CDC *pDC, CRect drawArea, const CStdEditorPosition &position, const CString &strText)
{
    CPrgEditorDoc *pDoc;
    int iColumn;
    int iLine;
    CRect rect;
	CFont *pOldFont;
	CSize extent;
	CRect clientRect;
	int iTopOffset;
	bool bVisible;

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return;
    }

    GetEditor(m_EditorID)->ToDisplayPosition (position).GetXYPosition (iColumn, iLine);
    ++iColumn;

	if (m_bHaveWPFont)
	{
		pOldFont = pDC->SelectObject (&m_WatchpointFont);
	}

	extent = pDC->GetTextExtent (strText);
	extent.cx += 2 * WPHorizontalMargin;
	extent.cy += 2 * WPVerticalMargin;

    rect.left = (iColumn - 1) * m_iCharWidth;
    rect.top = (iLine - 1) * m_iLineHeight + (m_iLineHeight - extent.cy) / 2;
	rect.right = rect.left + extent.cx;
	rect.bottom = rect.top + extent.cy;

    if (m_DrawTarget == ActiveView)
    {
        GetClientRect (&clientRect);
    }
    else
    {
        clientRect = drawArea;
    }

    iTopOffset = (m_iFirstLineNo - 1) * m_iLineHeight;

    bVisible = rect.bottom > iTopOffset + clientRect.top && rect.top < iTopOffset + clientRect.bottom &&
               rect.right > clientRect.left && rect.left < clientRect.right;

    if (bVisible)
    {
    	CRect clipRect;

	    WPAreaToClientArea (rect);

	    clipRect = drawArea;
	    clipRect.left += m_iMarkSpace;
	    pDC->IntersectClipRect (&clipRect);

	    pDC->FillSolidRect (&rect, RGB (255, 255, 230));
	    pDC->Rectangle (&rect);
	    pDC->SetTextColor (RGB (0, 0, 0));
	    pDC->TextOut (rect.left + WPHorizontalMargin, rect.top + WPVerticalMargin, strText);

	    ::SelectClipRgn (pDC->m_hDC, NULL);
    }

	if (m_bHaveWPFont)
	{
		pDC->SelectObject (pOldFont);
	}
}


void CSTEditorView::UpdateWatchpointArea (CWatchpoint *pWP, CDC *pDC)
{
    CPrgWPClientData *pClientData;
    long lDisplayX;
    long lDisplayY;
    long lDisplayOffsetX;
    long lDisplayOffsetY;
    CRect rect;

    pClientData = (CPrgWPClientData *)pWP->GetClientData ();
    ASSERT (pClientData != NULL);
    if (pClientData == NULL)
    {
        return;
    }

    pClientData->GetDisplay (lDisplayX, lDisplayY, lDisplayOffsetX, lDisplayOffsetY);

    rect.left = rect.right = (lDisplayX - 1) * m_iCharWidth + lDisplayOffsetX;
    rect.top = rect.bottom = (lDisplayY - 1) * m_iLineHeight + lDisplayOffsetY;

    pClientData->SetArea (rect);

    UpdateWatchpointExtent (pWP, pDC);
}


void CSTEditorView::UpdateWatchpointExtent (CWatchpoint *pWP, CDC *pDC)
{
    CPrgWPClientData *pClientData;
    CFont *pOldFont;
    CSize extent;
    CRect rect;

    pClientData = (CPrgWPClientData *)pWP->GetClientData ();
    ASSERT (pClientData != NULL);
    if (pClientData == NULL)
    {
        return;
    }

    rect = pClientData->GetArea ();

    if (m_bHaveWPFont)
    {
        pOldFont = pDC->SelectObject (&m_WatchpointFont);
    }

    extent = pDC->GetTextExtent (pClientData->GetText ());
    extent.cx += 2 * WPHorizontalMargin;
    extent.cy += 2 * WPVerticalMargin;

    rect.right = rect.left + extent.cx;
    rect.bottom = rect.top + extent.cy;

    pClientData->SetArea (rect);

    if (m_bHaveWPFont)
    {
        pDC->SelectObject (pOldFont);
    }
}


bool CSTEditorView::UpdateWatchpointVisibility (CPrgWPClientData *pClientData, CRect drawArea)
{
    CRect rect;
    CRect clientRect;
    bool bVisible;
    int xOffset;
    int yOffset;

    rect = pClientData->GetLargestArea ();

    if (m_DrawTarget == ActiveView)
    {
        GetClientRect (&clientRect);
    }
    else
    {
        clientRect = drawArea;
    }

    xOffset = m_iLeftEdge * m_iCharWidth;
    yOffset = (m_iFirstLineNo - 1) * m_iLineHeight;

    bVisible = rect.bottom > yOffset && rect.top < yOffset + (clientRect.bottom - clientRect.top) &&
               rect.right > xOffset && rect.left < yOffset + (clientRect.right - clientRect.left);

    if (m_DrawTarget == ActiveView)
    {
        pClientData->SetActiveVisibility (bVisible);
    }
    else
    {
        pClientData->SetInactiveVisibility (bVisible);
    }

    return (bVisible);
}


void CSTEditorView::OnSuspendResumeWatchpoints (WPARAM, LPARAM)
{
    SuspendInvisibleWatchpoints ();
}


void CSTEditorView::SuspendInvisibleWatchpoints ()
{
	CPrgEditorDoc *pDoc;
	POSITION pPosition;

    if (m_bEnteredSuspending)
    {
        return;
    }
    m_bEnteredSuspending = true;

	pDoc = GetDocument ();

	pPosition = pDoc->GetFirstWPPosition ();
	while (pPosition != NULL)
	{
		CWatchpoint *pWP;
		CPrgWPClientData *pClientData;
		bool bVisible;

		pWP = pDoc->GetNextWP (pPosition);
		assert (pWP != NULL);
		if (pWP == NULL)
			continue;

		pClientData = (CPrgWPClientData *)pWP->GetClientData ();
		assert (pClientData != NULL);
		if (pClientData == NULL)
			continue;

		if (pDoc->IsInPlaceActive ())
		{
			bVisible = pClientData->GetActiveVisibility ();
		}
		else
		{
			bVisible = pClientData->GetInactiveVisibility ();
		}

		if (bVisible)
		{
			pDoc->ResumeWatchpoint (pWP);
		}
		else
		{
			pDoc->SuspendWatchpoint (pWP);
		}
	}

    m_bEnteredSuspending = false;
}


void CSTEditorView::DrawMarks (CDC *pDC, RECT *rect, PLINEDESC pLine)
{
	SECEditView::DrawMarks (pDC, rect, pLine);

	if (pLine->bBreakpoint)
	{
		DrawBreakpoint (pDC, rect, pLine);
	}

	if (pLine->bStoppoint)
	{
		DrawStoppoint (pDC, rect, pLine);
	}
}


void CSTEditorView::DrawBreakpoint (CDC *pDC, RECT *rect, PLINEDESC pLine)
{
	POSITION pPosition;
	CPrgEditorDoc *pDoc;
	bool bFound=false;
	CBreakpoint *pBP;
	SECEdit *pEdit;
	CRect bpRect;
	CBrush subscribedBrush (RGB (255, 0, 0));
	CBrush unsubscribedBrush (RGB (255, 160, 160));
	CBrush inactiveBrush (RGB (255, 255, 255));
	CBrush* pOldBrush;

	pDoc = GetDocument ();

	pPosition = pDoc->GetFirstBPPosition ();
	while (pPosition != NULL)
	{
		pBP = pDoc->GetNextBP (pPosition);
		assert (pBP != NULL);
		if (pBP == NULL)
			continue;

		if (pLine->iLineNo == pBP->GetPosition().GetY())
		{
			bFound = true;
			break;
		}
	}

	assert (bFound);
	if (!bFound)
	{
		return;
	}

	pEdit = GetEditor (m_EditorID);
	assert (pEdit != NULL);
	if (pEdit == NULL)
		return;

	bpRect.top = rect->top + 2;
	bpRect.bottom = rect->bottom - 2;
	bpRect.left = 2;
	bpRect.right = m_iMarkSpace - 2;
	if (bpRect.top - bpRect.bottom < bpRect.right - bpRect.left)
	{
		bpRect.right = bpRect.left + (bpRect.bottom - bpRect.top);
	}
	else
	{
		bpRect.bottom = bpRect.top + (bpRect.right - bpRect.left);
	}

	if (pBP->IsActive ())
	{
		if (pBP->IsSubscribed ())
		{
			pOldBrush = pDC->SelectObject (&subscribedBrush);
		}
		else
		{
			pOldBrush = pDC->SelectObject (&unsubscribedBrush);
		}
	}
	else
	{
		pOldBrush = pDC->SelectObject (&inactiveBrush);
	}

	pDC->Ellipse (&bpRect);
	pDC->SelectObject (pOldBrush);
	pDC->SetBkColor (pEdit->m_crBackground);
}


void CSTEditorView::DrawStoppoint (CDC *pDC, RECT *rect, PLINEDESC pLine)
{
	CPrgEditorDoc *pDoc;
	CStdEditorPosition stopPosition;
	int iWidth;
	int iHeight;
	int iLeft;
	int iTop;
	CPoint points[7];
	CBrush brush (RGB (255, 255, 0));
	CBrush *pOldBrush;

	pDoc = GetDocument ();

	if (!pDoc->GetStopPosition (stopPosition))
	{
		return;
	}

	pOldBrush = pDC->SelectObject (&brush);

	iHeight = rect->bottom - rect->top - 4;
	iTop = rect->top + (rect->bottom - rect->top - iHeight) / 2;
	iWidth = iHeight;
	iLeft = (m_iMarkSpace - iWidth) / 2;

	points[0].x = iLeft;              points[0].y = iTop + iHeight / 3;
	points[1].x = iLeft + iWidth / 2; points[1].y = iTop + iHeight / 3;
	points[2].x = iLeft + iWidth / 2; points[2].y = iTop;
	points[3].x = iLeft + iWidth - 1; points[3].y = iTop + iHeight / 2;
	points[4].x = iLeft + iWidth / 2; points[4].y = iTop + iHeight - 1;
	points[5].x = iLeft + iWidth / 2; points[5].y = iTop + (iHeight - 1) - iHeight / 3;
	points[6].x = iLeft;              points[6].y = iTop + (iHeight - 1) - iHeight / 3;

	pDC->Polygon (points, 7);

	pDC->SelectObject (pOldBrush);
}


void CSTEditorView::OnIdle ()
{
    if (m_iLineNo != m_iLastLine || m_iColNo != m_iLastColumn)
    {
	    m_iLastLine = m_iLineNo;
	    m_iLastColumn = m_iColNo;

	    PostMessage (WM_EDITOR_POSITION_CHANGED, m_iLastColumn + 1, m_iLastLine);
    }
}

void CSTEditorView::OnEditorPositionChanged(WPARAM wParam, LPARAM lParam)
{
    if (GetDocument() != NULL)
        GetDocument()->PositionChanged (CStdEditorPosition (EDP_STLINE, wParam, lParam));
}



void CSTEditorView::OnGotoLine ()
{
	SECEdit *pEdit;

	pEdit = GetEditor(m_EditorID);
	assert (pEdit != NULL);
	if (pEdit == NULL)
		return;

	CGotoLineDlg dialog (pEdit->m_iLinesInFile);

	if (dialog.DoModal () == IDOK)
	{
		GotoPosition (CStdEditorPosition (EDP_STLINE, 1, dialog.m_uiLineNumber));
	}

	GetParentFrame ()->SetFocus ();
}


void CSTEditorView::OnSetWatchpoint ()
{
	CPrgEditorDoc *pDoc;
	CPoint point;

	pDoc = GetDocument ();
	if (pDoc == NULL || !m_Features.Chk(STF_MONITORING))
		return;

    point = GetCursorPoint ();

    if (IsPointInBlock (point))
    {
        TEXTPOS startTextPos;
        TEXTPOS endTextPos;
        CStdEditorPosition startPosition;
        CStdEditorPosition endPosition;

        if (!GetSelection (startTextPos, endTextPos))
        {
            return;
        }

        startPosition = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo));
        endPosition   = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset,   endTextPos.pLine->iLineNo));

        (void)pDoc->SetExpressionWatchpoint (startPosition, endPosition, true);

        return;
    }

    (void)pDoc->SetWatchpoint(CStdEditorPosition(EDP_STLINE, m_iColNo + 1, m_iLineNo), true);
}


void CSTEditorView::OnClearWatchpoint ()
{
	CWatchpoint *pWP;

	pWP = FindWatchpointByPoint (m_ContextMenuPoint);

	if (pWP != NULL && m_Features.Chk(STF_MONITORING) )
	{
		CPrgEditorDoc *pDoc;
		HRESULT hr;

		pDoc = GetDocument ();
		if (pDoc != NULL)
        {
            hr = pDoc->ClearWatchpoint (pWP);
        }
	}
}


void CSTEditorView::OnClearAllWatchpoints ()
{
	CPrgEditorDoc *pDoc;

	pDoc = GetDocument ();
	if (pDoc != NULL  && m_Features.Chk(STF_MONITORING))
    {
	    pDoc->ClearAllWatchpoints ();
    }
}


void CSTEditorView::OnWriteWatchpoint ()
{
    CPrgEditorDoc *pDoc;
    CWatchpoint *pWP;
    CString strExpression;
    CString strValue;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        
        pWP = FindWatchpointByPoint (m_ContextMenuPoint);
        
        if (pWP != NULL)
        {
            CPrgWPClientData *pClientData;
            
            pClientData = (CPrgWPClientData *)pWP->GetClientData ();
            if (pClientData == NULL)
            {
                return;
            }
            strExpression = pClientData->GetVariableName ();
            
            strValue = pDoc->GetWatchExpressionValue (pWP->GetWatchItem ());
        }
        else
        {
            HRESULT hr;
            CStdEditorPosition startPosition;
            CStdEditorPosition endPosition;
            long lHandle;
            CEExpressionState state;
            
            strExpression = GetCursorContextExpression (startPosition, endPosition);
            
            hr = pDoc->AddWatchItem (strExpression, lHandle);
            if (hr != S_OK)
            {
                ::MessageBeep (-1);
                return;
            }
            
            hr = pDoc->ConnectWatchItem (lHandle);
            if (hr != S_OK)
            {
                ::MessageBeep (-1);
                pDoc->RemoveWatchItem (lHandle);
                return;
            }
            
            state = pDoc->GetWatchExpressionState (lHandle);
            
            if (state == CEUnknownSubscription || state == CENotConnected || state == CEEmptyValue)
            {
                pDoc->DisconnectWatchItem (lHandle);
                pDoc->RemoveWatchItem (lHandle);
                ::MessageBeep (-1);
                return;
            }
            
            if (state == CEGoodExpression || state == CEReloadingValue)
            {
                strValue = pDoc->GetWatchExpressionValue (lHandle);
            }
            
            pDoc->DisconnectWatchItem (lHandle);
            pDoc->RemoveWatchItem (lHandle);
        }
        
        CEditVarDlg dialog (pDoc, strExpression, strValue, this);
        
        dialog.DoModal ();
    }
}


void CSTEditorView::OnToggleBreakpoint ()
{
	CStdEditorPosition position;
	CPrgEditorDoc *pDoc;
	HRESULT hr;
	int iDistance=1;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        position = CStdEditorPosition(EDP_STLINE, -1, m_iLineNo);
        
        if (pDoc->LookupBreakpoint(position) == NULL)
        {
            hr = pDoc->SetBreakpoint(position);
            if (hr == E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
            {
                while (m_iLineNo - iDistance >= m_iFirstLineNo || m_iLineNo + iDistance <= LastLineNo ())
                {
                    if (m_iLineNo + iDistance <= LastLineNo ())
                    {
                        position = CStdEditorPosition (EDP_STLINE, -1, m_iLineNo + iDistance);
                        hr = pDoc->SetBreakpoint (position);
                        if (hr != E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
                        {
                            break;
                        }
                    }
                    
                    if (m_iLineNo - iDistance >= m_iFirstLineNo)
                    {
                        position = CStdEditorPosition (EDP_STLINE, -1, m_iLineNo - iDistance);
                        hr = pDoc->SetBreakpoint (position);
                        if (hr != E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
                        {
                            break;
                        }
                    }                    
                    ++iDistance;
                }
            }
            
            if (FAILED (hr))
            {
                if (hr == E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
                {
                    SystemMessage (E_CE_ST_NO_VALID_BREAKPOINT_POSITION);
                }
                else
                {
                    SystemMessage (E_CE_ST_CANNOT_SET_BREAKPOINT);
                }
            }
        }
        else
        {
            hr = pDoc->ClearBreakpoint (position);
            
            if (FAILED (hr))
            {
                SystemMessage (E_CE_ST_CANNOT_CLEAR_BREAKPOINT);
            }
        }
    }
}


void CSTEditorView::OnRemoveAllBreakpoints ()
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING) )
    {
        hr = pDoc->ClearInstanceBreakpoints ();
    }
}


void CSTEditorView::OnToggleBreakpointEnabled ()
{
	CStdEditorPosition position;
	CPrgEditorDoc *pDoc;
	HRESULT hr;
	CBreakpoint *pBP;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        position = CStdEditorPosition(EDP_STLINE, -1, m_iLineNo);
        
        pBP = pDoc->LookupBreakpoint(position);
        if (pBP == NULL)
        {
            MessageBeep (-1);
            return;
        }
        
        if (pBP->IsActive ())
        {
            hr = pDoc->DeactivateBreakpoint(position);
            
            if (FAILED (hr))
            {
                SystemMessage (E_CE_ST_CANNOT_DEACTIVATE_BREAKPOINT);
            }
        }
        else
        {
            hr = pDoc->ActivateBreakpoint(position);
            
            if (FAILED (hr))
            {
                if (hr == E_CE_BPMAN_INVALID_BREAKPOINT_POSITION)
                {
                    SystemMessage (E_CE_ST_INVALID_BREAKPOINT_POSITION);
                }
                else
                {
                    SystemMessage (E_CE_ST_CANNOT_ACTIVATE_BREAKPOINT);
                }
            }
        }
    }
}



void CSTEditorView::OnUpdateEdit(CCmdUI *pCmdUI)
{
	CStdEditorDoc *pDoc;

	pDoc = GetDocument();
    if(pDoc->CheckReadonly())
    {
		pCmdUI->Enable(FALSE);
		return;
	}
    pCmdUI->Enable(TRUE);
}


// AUTOFMT 10.08.06 SIS >>
void CSTEditorView::OnUpdateEditorSettings(CCmdUI *pCmdUI)
{
    BOOL    bEnable = TRUE;
    SECEdit*    pEdit = GetEdit();
    if(pEdit)
    {
        CString sExtension = pEdit->GetFileMappingKey();
        if(sExtension.IsEmpty())
        {
            bEnable = FALSE;
        }
    }
    pCmdUI->Enable(bEnable);
}

void CSTEditorView::OnUpdateFormatSelection(CCmdUI *pCmdUI)
{
    BOOL    bEnable = TRUE;
	CStdEditorDoc *pDoc;

	pDoc = GetDocument();
    if(pDoc->CheckReadonly())
    {
        bEnable = FALSE;
	}
    else
    {
        CString sText = GetSelection();
        if(sText.IsEmpty())
        {
            bEnable = FALSE;
        }
    }
    pCmdUI->Enable(bEnable);
}
// AUTOFMT 10.08.06 SIS <<



void CSTEditorView::OnUpdateMonitor (CCmdUI *pCmdUI)
{
	CPrgEditorDoc *pDoc;

	pDoc = GetDocument ();
	if (pDoc == NULL || !m_Features.Chk(STF_MONITORING))
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	if (!pDoc->CanMonitor ())
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	switch (pCmdUI->m_nID)
	{
	case IDM_CLEAR_ALL_WATCHPOINTS:
		pCmdUI->Enable (pDoc->GetFirstWPPosition () != NULL);
		break;

	default:
		pCmdUI->Enable (TRUE);
	}
}


void CSTEditorView::OnUpdateDebug (CCmdUI *pCmdUI)
{
	CPrgEditorDoc *pDoc;

	pDoc = GetDocument ();
	if (pDoc == NULL || !m_Features.Chk(STF_DEBUGGING))
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	if (!pDoc->CanDebug ())
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	switch (pCmdUI->m_nID)
	{
	case IDM_TOGGLE_BREAKPOINT_ENABLED:
		{
			CStdEditorPosition position(EDP_STLINE, -1, m_iLineNo);
			pCmdUI->Enable (pDoc->LookupBreakpoint(position) != NULL);
		}
		break;

	case IDM_REMOVE_ALL_BREAKPOINTS:
		pCmdUI->Enable (pDoc->GetFirstBPPosition () != NULL);
		break;

	default:
		pCmdUI->Enable (TRUE);
	}
}


void CSTEditorView::OnInsertFBCall ()
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_STMT_INS) )  // statement insertion enable?
    {
        hr = pDoc->SelectFBForCall ();
    }
}


void CSTEditorView::OnShowVarSelection()
{
	CPoint          point;
    CString         strText;
    CPrgEditorDoc*  pDoc;
	HRESULT         hr;

	pDoc = GetDocument ();
	if (pDoc == NULL)
		return;

    CString strPouName = pDoc->GetDocumentFile();
    CFile file;
    file.SetFilePath(strPouName);
    strPouName = file.GetFileTitle();
    
    point = GetCursorPoint ();
        
    if (IsPointInBlock (point))
    {
        TEXTPOS startTextPos;
        TEXTPOS endTextPos;
        CStdEditorPosition startPosition;
        CStdEditorPosition endPosition;

        if (!GetSelection (startTextPos, endTextPos))
        {
            return;
        }
        
        startPosition = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo));
        endPosition   = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset,   endTextPos.pLine->iLineNo));
        
        strText = pDoc->GetContextStringSelection(startPosition, endPosition);
        if (!strText.IsEmpty())
        {
            strPouName += _T('|');
            strPouName += strText;
        }
        hr = pDoc->ShowVarSelection(strPouName);
        return;
    }
    
    strText = pDoc->GetContextStringPosition(CStdEditorPosition (EDP_STLINE, m_iColNo + 1, m_iLineNo));
    if (!strText.IsEmpty())
    {
        strPouName += _T('|');
        strPouName += strText;
    }      
    hr = pDoc->ShowVarSelection(strPouName);
}


void CSTEditorView::OnViewObjHelp ()
{
	CPrgEditorDoc *pDoc;
	CPoint point;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_USERHELP))
    {
        point = GetCursorPoint ();
        
        if (IsPointInBlock (point))
        {
            TEXTPOS startTextPos;
            TEXTPOS endTextPos;
            CStdEditorPosition startPosition;
            CStdEditorPosition endPosition;

            if (!GetSelection (startTextPos, endTextPos))
            {
                return;
            }
            
            startPosition = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo));
            endPosition   = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset,   endTextPos.pLine->iLineNo));
            
            (void)pDoc->OnViewObjHelpSelection(startPosition, endPosition);
            return;
        }
        
        (void)pDoc->OnViewObjHelpPosition(CStdEditorPosition (EDP_STLINE, m_iColNo + 1, m_iLineNo));
    }
}



void CSTEditorView::OnContinueTask ()
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ContinueTask ();
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_CONTINUE_TASK);
        }
    }
}

void CSTEditorView::OnStepInto ()
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->StepTask (1);
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_STEP_INTO);
        }
    }
}

void CSTEditorView::OnStepOver ()
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->StepTask (2);
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_STEP_OVER);
        }
    }
}

void CSTEditorView::OnStepOut ()
{
	CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->StepTask (3);
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_STEP_OUT);
        }
    }
}

void CSTEditorView::OnContextMenuKey ()
{
	CRect rect;
	CPoint point;

	GetClientRect (rect);
	ClientToScreen (&rect);
	point.x = rect.left + 50;
	point.y = rect.top + 30;
	SendMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM (point.x, point.y));
}


void CSTEditorView::OnToggleManualMode ()
{
    HRESULT hr;
    CPrgEditorDoc *pDoc;
    bool bInManualMode;

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return;
    }

    if (!pDoc->InManualMode (bInManualMode))
    {
        return;
    }

    hr = pDoc->SetManualMode (!bInManualMode);
}


void CSTEditorView::OnPerformCycle ()
{
    HRESULT hr;
    CPrgEditorDoc *pDoc;
    bool bInManualMode;
    bool bInCycle;

    pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        if (!pDoc->InManualMode (bInManualMode))
        {
            AfxMessageBox (IDS_CANNOT_PERFORM_CYCLE);
            return;
        }
        
        if (!bInManualMode)
        {
            AfxMessageBox (IDS_CANNOT_PERFORM_CYCLE);
            return;
        }
        
        if (!pDoc->InSFCCycle (bInCycle))
        {
            AfxMessageBox (IDS_CANNOT_PERFORM_CYCLE);
            return;
        }
        
        if (bInCycle)
        {
            AfxMessageBox (IDS_CANNOT_PERFORM_CYCLE);
            return;
        }
        
        hr = pDoc->PerformSFCCycle ();
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_PERFORM_CYCLE);
            return;
        }
    }
}


void CSTEditorView::OnForceTransitionOnce ()
{
    HRESULT hr;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return;
    }

    hr = pDoc->ForceTransitionOnce (!pDoc->IsTransitionForcedOnce (-(m_iColNo + 1), m_iLineNo), -(m_iColNo + 1), m_iLineNo);

    if (FAILED (hr))
    {
        AfxMessageBox (IDS_CANNOT_FORCE_TRANSITION);
    }
}


void CSTEditorView::OnForceTransition ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {

        hr = pDoc->ForceTransitionOnce (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        hr = pDoc->BlockTransition (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        hr = pDoc->ForceTransition (!pDoc->IsTransitionForcedPermanently (-(m_iColNo + 1), m_iLineNo), -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_TRANSITION);
        }
    }
}


void CSTEditorView::OnBlockTransition ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
	if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceTransitionOnce (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        hr = pDoc->ForceTransition (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        hr = pDoc->BlockTransition (!pDoc->IsTransitionBlocked (-(m_iColNo + 1), m_iLineNo), -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_BLOCK_TRANSITION);
        }
    }
}


void CSTEditorView::OnForceAllTransitions ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceAllTransitionsOnce (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        hr = pDoc->BlockAllTransitions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        hr = pDoc->ForceAllTransitions (true);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }

        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_TRANSITION);
        }
    }
}


void CSTEditorView::OnBlockAllTransitions ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceAllTransitionsOnce (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAllTransitions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAllTransitions (true);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_TRANSITION);
        }
    }
}


void CSTEditorView::OnRevertAllTransitions ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceAllTransitionsOnce (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAllTransitions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAllTransitions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_TRANSITION);
        }
    }
}


void CSTEditorView::OnGloballyForceAllTransitions ()
{
    HRESULT hr;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->GloballyForceAllTransitions (!pDoc->AreAllTransitionsForcedGlobally ());
        
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_TRANSITIONS);
        }
    }
}


void CSTEditorView::OnForceActionOnce ()
{
    HRESULT hr;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceActionOnce (!pDoc->IsActionForcedOnce (-(m_iColNo + 1), m_iLineNo), -(m_iColNo + 1), m_iLineNo);
        
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_ACTION);
        }
    }
}


void CSTEditorView::OnForceAction ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceActionOnce (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAction (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAction (!pDoc->IsActionForcedPermanently (-(m_iColNo + 1), m_iLineNo), -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_ACTION);
        }
    }
}


void CSTEditorView::OnBlockAction ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceActionOnce (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAction (false, -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAction (!pDoc->IsActionBlocked (-(m_iColNo + 1), m_iLineNo), -(m_iColNo + 1), m_iLineNo);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_BLOCK_ACTION);
        }
    }
}


void CSTEditorView::OnForceAllActions ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceAllActionsOnce (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAllActions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAllActions (true);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_ACTION);
        }
    }
}


void CSTEditorView::OnBlockAllActions ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceAllActionsOnce (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAllActions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAllActions (true);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_ACTION);
        }
    }
}


void CSTEditorView::OnRevertAllActions ()
{
    HRESULT hr;
    HRESULT hrResult=S_OK;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->ForceAllActionsOnce (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->ForceAllActions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        hr = pDoc->BlockAllActions (false);
        if (FAILED (hr) && hrResult == S_OK)
        {
            hrResult = hr;
        }
        
        if (FAILED (hrResult))
        {
            AfxMessageBox (IDS_CANNOT_FORCE_ACTION);
        }
    }
}


void CSTEditorView::OnGloballyBlockAllActions ()
{
    HRESULT hr;
    CPrgEditorDoc *pDoc;

    pDoc = GetDocument ();
    if (pDoc != NULL && m_Features.Chk(STF_DEBUGGING))
    {
        hr = pDoc->BlockAllActions (!pDoc->AreAllActionsBlockedGlobally ());
        
        if (FAILED (hr))
        {
            AfxMessageBox (IDS_CANNOT_BLOCK_ACTIONS);
        }
    }
}


CWatchpoint *CSTEditorView::FindWatchpointByPoint (CPoint point)
{
    CPrgEditorDoc *pDoc;
    POSITION pPosition;
    unsigned int uiBestZ;
    CWatchpoint *pBestWP=NULL;

    pDoc = GetDocument ();

    pPosition = pDoc->GetFirstWPPosition ();
    while (pPosition != NULL)
    {
        CWatchpoint *pWP;
        CPrgWPClientData *pClientData;
        CRect area;

        pWP = pDoc->GetNextWP (pPosition);
        assert (pWP != NULL);
        if (pWP == NULL)
            continue;

        pClientData = (CPrgWPClientData *)pWP->GetClientData ();
        assert (pClientData != NULL);
        if (pClientData == NULL)
            continue;

        area = pClientData->GetArea ();
        WPAreaToClientArea (area);

        if (point.x >= area.left && point.x < area.right &&
            point.y >= area.top && point.y < area.bottom)
        {
            unsigned int uiZ;

            uiZ = pClientData->GetZ ();

            if (pBestWP == NULL || uiZ < uiBestZ)
            {
                pBestWP = pWP;
                uiBestZ = uiZ;
            }
        }
    }

    return (pBestWP);
}


void CSTEditorView::WPAreaToClientArea (CRect &area)
{
	int xOffset;
	int yOffset;

	xOffset = m_iLeftEdge * m_iCharWidth;
	area.left -= xOffset;
	area.left += m_iMarkSpace;
	area.right -= xOffset;
	area.right += m_iMarkSpace;

	yOffset = (m_iFirstLineNo - 1) * m_iLineHeight;
	area.top -= yOffset;
	area.bottom -= yOffset;
}


void CSTEditorView::ClientAreaToWPArea (CRect &area)
{
	int xOffset;
	int yOffset;

	xOffset = m_iLeftEdge * m_iCharWidth;
	area.left += xOffset;
	area.left -= m_iMarkSpace;
	area.right += xOffset;
	area.right -= m_iMarkSpace;

	yOffset = (m_iFirstLineNo - 1) * m_iLineHeight;
	area.top += yOffset;
	area.bottom += yOffset;
}


void CSTEditorView::SystemMessage (HRESULT hr)
{
	CPrgEditorDoc *pDoc;
	CString strMessage;

	MessageBeep (-1);

	pDoc = GetDocument ();
	if (pDoc == NULL)
		return;

	pDoc->PrintSystemMessage (E_FACILITY_CEST, hr);

	if (pDoc->GetMessageText (hr, strMessage))
	{
		int iPos;

		iPos = strMessage.Find (':');
		if (iPos >= 0)
		{
			iPos = strMessage.Find (':', iPos + 1);
		}
		if (iPos >= 0)
		{
			strMessage = strMessage.Mid (iPos + 1);
			strMessage.TrimLeft ();
		}

		AfxMessageBox (strMessage);
	}
}


BOOL CSTEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (zDelta < 0)
    {
        while (-zDelta >= WHEEL_DELTA / 3)
        {
            if (nFlags & MK_CONTROL)
            {
                ScrollRightColumn ();
            }
            else
            {
                ScrollDownLine (TRUE);
            }

            zDelta += WHEEL_DELTA / 3;
        }
    }
    else
    {
        while (zDelta >= WHEEL_DELTA / 3)
        {
            if (nFlags & MK_CONTROL)
            {
                ScrollLeftColumn ();
            }
            else
            {
                ScrollUpLine (TRUE);
            }

            zDelta -= WHEEL_DELTA / 3;
        }
    }

    if (m_pTrackedWP != NULL)
    {
        ScreenToClient (&pt);
        TrackWP (pt);
    }

    return (TRUE);
}


void CSTEditorView::OnEditScrollDown()
{
    ScrollDownLine (TRUE);

    if (!IsLineVisible (m_iLineNo))
    {
        GotoLine (m_iFirstLineNo);
    }
}


void CSTEditorView::OnEditScrollUp()
{
    ScrollUpLine (TRUE);

    if (!IsLineVisible (m_iLineNo))
    {
        GotoLine (LastLineNo ());
    }
}


void CSTEditorView::ScrollLeftColumn ()
{
    int iOldLeftEdge;

    iOldLeftEdge = m_iLeftEdge;

    if (m_iLeftEdge != 0)
    {
        --m_iLeftEdge;
    }

    if (iOldLeftEdge != m_iLeftEdge)
    {
        MySetScrollPos (SB_HORZ, m_iLeftEdge, m_bUpdate);
        Invalidate ();
        MySetCaretPos ();
    }
}


void CSTEditorView::ScrollRightColumn ()
{
    int iOldLeftEdge;

    iOldLeftEdge = m_iLeftEdge;

    if (m_iLeftEdge < OE_MAXLINELENGTH)
    {
        ++m_iLeftEdge;
    }

    if (iOldLeftEdge != m_iLeftEdge)
    {
        MySetScrollPos (SB_HORZ, m_iLeftEdge, m_bUpdate);
        Invalidate ();
        MySetCaretPos ();
    }
}

void CSTEditorView::OnEditCut()
{
    CPrgEditorDoc*  pDoc = GetDocument();
    pDoc->PrepareEdit();

    SECEditView::OnEditCut();
}

void CSTEditorView::OnEditPaste()
{
    CPrgEditorDoc*  pDoc = GetDocument();
	SECEdit *pEdit;

	pEdit = GetEditor(m_EditorID);
	if (pEdit == NULL)
		return;

    if(pDoc->PrepareEdit() && !pEdit->GetReadOnly())
    {
        SECEditView::OnEditPaste();
    }
    else
    {
        CString strMessage;
        strMessage.LoadString(IDS_FILE_READ_ONLY);
        AfxMessageBox(strMessage);
    }
}

BOOL CSTEditorView::InitializeDataSource (COleDataSource *pDataSource, CLIPFORMAT clipFormat, HGLOBAL hGlobal)
{
    CPrgEditorDoc *pDoc;
    TEXTPOS startTextPos;
    TEXTPOS endTextPos;
    CStdEditorPosition selectionStart;
    CStdEditorPosition selectionEnd;
    CString strText;
    CStdEditorPosition startPosition;
    CStdEditorPosition endPosition;

    if (!SECEditView::InitializeDataSource (pDataSource, clipFormat, hGlobal))
    {
        return (FALSE);
    }

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return (TRUE);
    }

    if (!GetSelection (startTextPos, endTextPos))
    {
        return (TRUE);
    }

    selectionStart = CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo);
    selectionEnd   = CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset,   endTextPos.pLine->iLineNo);

    strText = GetEditor(m_EditorID)->GetTrimmedText (selectionStart, selectionEnd, startPosition, endPosition);

    pDoc->CacheWatchExpression (strText, pDataSource);

    return (TRUE);
}


bool CSTEditorView::OnModifyContent ()
{
    SECEdit *pEdit;
    CPrgEditorDoc *pDoc;

    pEdit = GetEditor(m_EditorID);
    if (pEdit == NULL)
    {
        return (true);
    }

    pDoc = GetDocument ();
    if (pDoc == NULL)
    {
        return (true);
    }

    if (pDoc->CheckReadonly ())
    {
        ::MessageBeep(-1);
        return (false);
    }

    // change to class tab
    pDoc->PrepareEdit();

//    if (pDoc->HadReadOnlyWarning())
//    {
//        return (true);
//    }
//
//    if (AfxMessageBox (IDS_READONLY_WARNING, MB_OKCANCEL) == IDCANCEL)
//    {
//        return (false);
//    }

//    pDoc->HadReadOnlyWarning(true);
    return (true);
}


void CSTEditorView::OnInsertLruPou(UINT nID)
{
    CPrgEditorDoc *pDoc;
	HRESULT hr;

    UINT nIDZero = nID - IDM_INSERT_LRU_FIRST;
    UINT count = 0;
    CString strPouName;
    CString strPouDef;

    POSITION pos = m_listLruPous.GetHeadPosition();
    while(pos)
    {
        CString str = m_listLruPous.GetNext(pos);
        if (count == nIDZero)
        {
            strPouName = str;
            break;
        }
        count++;
    }
    
    if (strPouName.IsEmpty())
    {
        return;
    }

	pDoc = GetDocument ();
	if (pDoc != NULL)  
    {   
        hr = pDoc->GetPouXML(strPouName, strPouDef);

        if (hr==S_OK)
        {
            InsertFBCall(strPouDef);
        }
    }
}


void CSTEditorView::OnXRefChangeRefPositionLast()
{
    CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL)  
    {   
        hr = pDoc->XRefChangeRefPosition(FALSE);
    }
}

void CSTEditorView::OnXRefChangeRefPositionNext()
{
    CPrgEditorDoc *pDoc;
	HRESULT hr;

	pDoc = GetDocument ();
	if (pDoc != NULL)  
    {   
        hr = pDoc->XRefChangeRefPosition(TRUE);
    }
}

void CSTEditorView::OnXRefSetActiveQuery()
{
	CPrgEditorDoc *pDoc;
	CPoint point;
    CString strText;

	pDoc = GetDocument ();
	if (pDoc != NULL)
    {
        CString parentName = pDoc->GetDocumentFile();
        int     lbs;
        lbs = parentName.ReverseFind(_T('\\'))+1;
        parentName = parentName.Mid(lbs, parentName.GetLength() - lbs);

        parentName =  _T("|") + parentName;

        point = GetCursorPoint ();
        
        if (IsPointInBlock (point))
        {
            TEXTPOS startTextPos;
            TEXTPOS endTextPos;
            CStdEditorPosition startPosition;
            CStdEditorPosition endPosition;

            if (!GetSelection (startTextPos, endTextPos))
            {
                return;
            }
            
            startPosition = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -startTextPos.iOffset, startTextPos.pLine->iLineNo));
            endPosition   = GetEditor(m_EditorID)->ToDisplayPosition (CStdEditorPosition (EDP_STLINE, -endTextPos.iOffset,   endTextPos.pLine->iLineNo));
            
            strText = pDoc->GetContextStringSelection(startPosition, endPosition);
            if (strText.IsEmpty())
            {
                return;
            }
            strText += parentName;
            (void)pDoc->XRefSetActiveQuery(strText);
            return;
        }
        
        strText = pDoc->GetContextStringPosition(CStdEditorPosition (EDP_STLINE, m_iColNo + 1, m_iLineNo));
        if (!strText.IsEmpty())
        {
            strText += parentName;
        }      
        (void)pDoc->XRefSetActiveQuery(strText);
    }
}


void CSTEditorView::OnShowVarSelPopup()
{
    HRESULT hr;

    CPrgEditorDoc *pDoc;
    
    pDoc = GetDocument ();
	if (pDoc != NULL)  
    {
        LONG xPos;
        LONG yPos;

		// 11.12.06 SIS >>
        CSECEditor* pEdit = pDoc->GetEditor(m_EditorID);
        if (!pEdit)
        {
            return;
        }
		int	iColNoReal = pEdit->GetRealOffset(m_pCurLine, m_iColNo);
		// 11.12.06 SIS <<

        CStdEditorPosition posCont (EDP_STLINE, m_iColNo, m_iLineNo);
        CPoint mousePoint;      
        ConvertPositionMouse (CStdEditorPosition(EDP_STLINE, posCont.GetX(), posCont.GetY()+1), mousePoint);
        ClientToScreen(&mousePoint);
        xPos = mousePoint.x;
        yPos = mousePoint.y;

        CString strPouName = pDoc->GetDocumentFile();
        CFile file;
        file.SetFilePath(strPouName);
        strPouName = file.GetFileTitle();

        CString strTextLine = pEdit->GetTextLine(m_iLineNo);

        LONG lColStart;
        LONG lColEnd;
        CString strSelection;

        hr = pDoc->ShowVarSelectionBox(xPos, yPos, 
                                       strTextLine, iColNoReal, 
                                       _T(""),
                                       strPouName,
                                       strSelection, lColStart, lColEnd);
            
        if (hr == S_OK)
        {
			// 11.12.06 SIS >>
            TEXTPOS startTextPos;
            startTextPos.iOffset = lColStart;
            startTextPos.pLine = m_pCurLine;
            TEXTPOS endTextPos;
            endTextPos.iOffset = lColEnd;
            endTextPos.pLine = m_pCurLine;
			// 11.12.06 SIS <<
            SetSelection(&startTextPos, &endTextPos);

            ReplaceSelection(strSelection);
            ClearSelection(m_bUpdate);

            RecreateCaret();
        }
        SetFocus();
    }
    
}


/////////////////////////////////////////////////////////////////////////////
// CGotoLineDlg dialog


CGotoLineDlg::CGotoLineDlg(UINT uiLineCount, CWnd* pParent /*=NULL*/)
: CDialog(CGotoLineDlg::IDD, pParent),
  m_uiLineCount (uiLineCount)
{
	//{{AFX_DATA_INIT(CGotoLineDlg)
	m_uiLineNumber = 1;
	//}}AFX_DATA_INIT
}


void CGotoLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Use INT_MAX for maximum line number because line
	// number will be converted to a signed integer.

	//{{AFX_DATA_MAP(CGotoLineDlg)
	DDX_Text(pDX, IDC_LINE_NUMBER, m_uiLineNumber);
	DDV_MinMaxUInt(pDX, m_uiLineNumber, 1, m_uiLineCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoLineDlg, CDialog)
	//{{AFX_MSG_MAP(CGotoLineDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGotoLineDlg message handlers
