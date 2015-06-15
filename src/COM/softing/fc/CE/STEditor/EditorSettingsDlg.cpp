// EditorSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "steditor.h"
#include "EditorSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorSettingsDlg dialog


CEditorSettingsDlg::CEditorSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditorSettingsDlg)
	m_bKeepTabs = FALSE;
	m_iTabSize = 4;
	//}}AFX_DATA_INIT
    m_hBgBrush = NULL;
    m_hFont = NULL;
    m_crDefTextColor = 0;
    m_crKeywordColor = RGB(0,0,128);
    m_crQuoteColor = RGB(128,0,0);
    m_crCommentColor = RGB(0,128,0);
    m_crBgColor = RGB(255,255,255);
}

CEditorSettingsDlg::~CEditorSettingsDlg()
{
    if(m_hBgBrush)
    {
        DeleteObject(m_hBgBrush);
    }
    if(m_hFont)
    {
        DeleteObject(m_hFont);
    }
}

void CEditorSettingsDlg::SetDefaultEditSettings(SECEditSettings& rtDefaultSettings)
{
    m_tDefaultSettings = rtDefaultSettings;
    m_tSettings = m_tDefaultSettings;

    m_tSettings.LoadFromRegistry();
}

void CEditorSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorSettingsDlg)
	DDX_Check(pDX, IDC_ES_KEEPTABS_CHK, m_bKeepTabs);
	DDX_Text(pDX, IDC_ES_TABSIZE_EDT, m_iTabSize);
	DDV_MinMaxInt(pDX, m_iTabSize, 1, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditorSettingsDlg)
	ON_BN_CLICKED(IDC_ES_DEFTEXTCOL_BTN, OnEsDeftextcolBtn)
	ON_BN_CLICKED(IDC_ES_QUOTECOL_BTN, OnEsQuotecolBtn)
	ON_BN_CLICKED(IDC_ES_COMMENTCOL_BTN, OnEsCommentcolBtn)
	ON_BN_CLICKED(IDC_ES_BGCOL_BTN, OnEsBgcolBtn)
	ON_BN_CLICKED(IDC_ES_KEYWORDCOL_BTN, OnEsKeywordcolBtn)
	ON_BN_CLICKED(IDC_ES_DEFAULT_BTN, OnEsDefault)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorSettingsDlg message handlers

void CEditorSettingsDlg::OnEsDeftextcolBtn() 
{
    if(SelectColor(m_crDefTextColor))
    {
        Invalidate();
    }
}

void CEditorSettingsDlg::OnEsQuotecolBtn() 
{
    if(SelectColor(m_crQuoteColor))
    {
        Invalidate();
    }
}

void CEditorSettingsDlg::OnEsCommentcolBtn() 
{
    if(SelectColor(m_crCommentColor))
    {
        Invalidate();
    }
}

void CEditorSettingsDlg::OnEsBgcolBtn() 
{
    if(SelectColor(m_crBgColor))
    {
        CreateBgBrush();
        Invalidate();
    }
}

void CEditorSettingsDlg::OnEsKeywordcolBtn() 
{
    if(SelectColor(m_crKeywordColor))
    {
        Invalidate();
    }
}

BOOL CEditorSettingsDlg::OnInitDialog() 
{
    SetValues(FALSE);
    
	CDialog::OnInitDialog();

    CreateBgBrush();
    CreateTextFont();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditorSettingsDlg::CreateBgBrush()
{
    LOGBRUSH    logBrush;
    logBrush.lbStyle = BS_SOLID;
    logBrush.lbColor = m_crBgColor;
    logBrush.lbHatch = 0;

    HBRUSH hBgBrushNew = CreateBrushIndirect(&logBrush);

    if(hBgBrushNew)
    {
        if(m_hBgBrush)
        {
            DeleteObject(m_hBgBrush);
        }
        m_hBgBrush = hBgBrushNew;
    }
}

void CEditorSettingsDlg::CreateTextFont()
{
    LOGFONT logFont;
    HFONT   hFont = (HFONT)GetStockObject (ANSI_FIXED_FONT);
    CGdiObject* pGdiObject;
    pGdiObject = CGdiObject::FromHandle (hFont);
    if (pGdiObject != NULL && pGdiObject->GetObject (sizeof (LOGFONT), &logFont) != 0)
    {
        // TODO: settings
        HFONT hFontNew = CreateFontIndirect(&logFont);
        if(hFontNew)
        {
            if(m_hFont)
            {
                DeleteObject(m_hFont);
            }
            m_hFont = hFontNew;
        }
    }
}

void CEditorSettingsDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    HDC     hDC = lpDrawItemStruct->hDC;
    HWND    hwndItem = lpDrawItemStruct->hwndItem;
    UINT    uiCtlID = lpDrawItemStruct->CtlID;

    RECT    rcItem = lpDrawItemStruct->rcItem;
    HBRUSH  hBrushOld = (HBRUSH)SelectObject(hDC, m_hBgBrush);

    ::FillRect(hDC, &rcItem, m_hBgBrush);
    ::DrawEdge(hDC, &rcItem, EDGE_ETCHED, BF_RECT);

    COLORREF    crCol = m_crDefTextColor;
    switch(uiCtlID)
    {
    case IDC_ES_KEYWORDCOL_BTN:
        crCol = m_crKeywordColor;
        break;
    case IDC_ES_QUOTECOL_BTN:
        crCol = m_crQuoteColor;
        break;
    case IDC_ES_COMMENTCOL_BTN:
        crCol = m_crCommentColor;
        break;
    }
    COLORREF    crOldTextCol = ::SetTextColor(hDC, crCol);
    COLORREF    crOldBkCol = ::SetBkColor(hDC, m_crBgColor);

    ASSERT(m_hFont);
    HFONT   hFontOld = NULL;
    if(m_hFont)
    {
        hFontOld = (HFONT)::SelectObject(hDC, m_hFont);
    }

    CString sText(_T("Click to Change"));
    ::ExtTextOut(hDC, 5, 5, ETO_CLIPPED, &rcItem, (LPCTSTR)sText, sText.GetLength(), NULL);

    ::SetTextColor(hDC, crOldTextCol);
    ::SetBkColor(hDC, crOldBkCol);
    if(m_hFont)
    {
        ::SelectObject(hDC, hFontOld);
    }
    ::SelectObject(hDC, hBrushOld);

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


BOOL CEditorSettingsDlg::SelectColor(COLORREF& crCol)
{
    BOOL            bReturn = FALSE;
    CColorDialog	tColDlg(crCol, CC_FULLOPEN);

    if(tColDlg.DoModal() == IDOK)
    {
        crCol = tColDlg.GetColor();
        bReturn = TRUE;
    }
    return bReturn;
}

void CEditorSettingsDlg::OnEsDefault() 
{
    m_tSettings = m_tDefaultSettings;
    
    SetValues(FALSE);
    CreateBgBrush();
    
    Invalidate();
}

void CEditorSettingsDlg::SetValues(BOOL bSave /*=TRUE*/)
{
    if(bSave)
    {
        UpdateData(TRUE);

        m_tSettings.SetTabSize(m_iTabSize);
        m_tSettings.SetKeepTabs(m_bKeepTabs);
        m_tSettings.SetBackgroundCol(m_crBgColor);
        m_tSettings.SetTextColDefault(m_crDefTextColor);
        m_tSettings.SetTextColKeyword(m_crKeywordColor);
        m_tSettings.SetTextColQuote(m_crQuoteColor);
        m_tSettings.SetTextColComment(m_crCommentColor);
    }
    else
    {
        m_iTabSize          = m_tSettings.GetTabSize();
        m_bKeepTabs         = m_tSettings.GetKeepTabs();
        m_crBgColor         = m_tSettings.GetBackgroundCol();
        m_crDefTextColor    = m_tSettings.GetTextColDefault();
        m_crKeywordColor    = m_tSettings.GetTextColKeyword();
        m_crQuoteColor      = m_tSettings.GetTextColQuote();
        m_crCommentColor    = m_tSettings.GetTextColComment();

        UpdateData(FALSE);
    }
}

void CEditorSettingsDlg::OnOK() 
{
    SetValues();

    m_tSettings.SaveToRegistry();
	
	CDialog::OnOK();
}
