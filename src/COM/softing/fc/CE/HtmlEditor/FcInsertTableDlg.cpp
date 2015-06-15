

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"
#include "FcHtmlColor.h"

#include <TriEdCID.h>

#include "FcInsertTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcDlgInsertTable dialog


CFcDlgInsertTable::CFcDlgInsertTable(CFcHtmlEdView* pParent /*=NULL*/)
	: CDialog(CFcDlgInsertTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFcDlgInsertTable)
	m_nColumns = 2;
	m_nRows = 2;
	m_nWidth = 100;
	m_WidthUnit = 0;
	m_Align = -1;
	m_nPadding = 1;
	m_nSpacing = 1;
	m_CellAlign = -1;
	m_CellBgColor = _T("");
	m_BgImage = _T("");
	m_BgColor = _T("");
	m_ColorBorder = _T("");
	m_CellColorBorder = _T("");
	m_bWrapText = FALSE;
	m_nBorderSize = 1;
	m_Title = _T("");
	//}}AFX_DATA_INIT

	ASSERT_VALID(pParent);
	m_pParent = pParent;
}


void CFcDlgInsertTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFcDlgInsertTable)
	DDX_Control(pDX, IDC_CB_TABLECELL_ALIGN, m_CellAlignCbCtrl);
	DDX_Control(pDX, IDC_CB_TABLE_ALIGN, m_AlignCbCtrl);
	DDX_Control(pDX, IDC_CB_TABLECELL_COLOR_BORDER, m_CbCtrlColorCellBorder);
	DDX_Control(pDX, IDC_CB_TABLECELL_COLOR_BG, m_CbCtrlColorCellBg);
	DDX_Control(pDX, IDC_CB_TABLE_COLOR_BORDER, m_CbCtrlColorBorder);
	DDX_Control(pDX, IDC_CB_TABLE_COLOR_BG, m_CbCtrlColorBg);
	DDX_Control(pDX, IDC_SPIN_TABLE_BORDER_SIZE, m_SpinCtrlBorderSize);
	DDX_Control(pDX, IDC_SPIN_TABLE_CELL_SPACING, m_SpinCtrlSpacing);
	DDX_Control(pDX, IDC_SPIN_TABLE_CELL_PADDING, m_SpinCtrlPadding);
	DDX_Control(pDX, IDC_SPIN_TABLE_WIDTH, m_SpinCtrlWidth);
	DDX_Control(pDX, IDC_SPIN_TABLE_ROWS, m_SpinCtrlRows);
	DDX_Control(pDX, IDC_SPIN_TABLE_COLUMNS, m_SpinCtrlColumns);
	DDX_Text(pDX, IDC_EDIT_TABLE_COLUMNS, m_nColumns);
	DDV_MinMaxInt(pDX, m_nColumns, 1, 100);
	DDX_Text(pDX, IDC_EDIT_TABLE_ROWS, m_nRows);
	DDV_MinMaxInt(pDX, m_nRows, 1, 100);
	DDX_Text(pDX, IDC_EDIT_TABLE_WIDTH, m_nWidth);
	DDX_CBIndex(pDX, IDC_CB_TABLE_WIDTH_UNIT, m_WidthUnit);
	DDX_CBIndex(pDX, IDC_CB_TABLE_ALIGN, m_Align);
	DDX_Text(pDX, IDC_EDIT_TABLE_CELL_PADDING, m_nPadding);
	DDV_MinMaxInt(pDX, m_nPadding, 0, 100);
	DDX_Text(pDX, IDC_EDIT_TABLE_CELL_SPACING, m_nSpacing);
	DDV_MinMaxInt(pDX, m_nSpacing, 0, 100);
	DDX_CBIndex(pDX, IDC_CB_TABLECELL_ALIGN, m_CellAlign);
	DDX_CBString(pDX, IDC_CB_TABLECELL_COLOR_BG, m_CellBgColor);
	DDX_Text(pDX, IDC_EDIT_TABLE_IMAGE_BG, m_BgImage);
	DDX_CBString(pDX, IDC_CB_TABLE_COLOR_BG, m_BgColor);
	DDX_CBString(pDX, IDC_CB_TABLE_COLOR_BORDER, m_ColorBorder);
	DDX_CBString(pDX, IDC_CB_TABLECELL_COLOR_BORDER, m_CellColorBorder);
	DDX_Check(pDX, IDC_CHK_TABLECELL_WRAP_TEXT, m_bWrapText);
	DDX_Text(pDX, IDC_EDIT_TABLE_BORDER_SIZE, m_nBorderSize);
	DDV_MinMaxInt(pDX, m_nBorderSize, 0, 100);
	DDX_Text(pDX, IDC_EDIT_TABLE_TITLE, m_Title);
	//}}AFX_DATA_MAP
	if (m_WidthUnit == 0)
		DDV_MinMaxInt(pDX, m_nWidth, 1, 100);
	else
		DDV_MinMaxInt(pDX, m_nWidth, 1, 4000);
}


BEGIN_MESSAGE_MAP(CFcDlgInsertTable, CDialog)
	//{{AFX_MSG_MAP(CFcDlgInsertTable)
	ON_BN_CLICKED(IDC_BTN_TABLECELL_COLOR_BORDER, OnTableCellColorBorderButton)
	ON_BN_CLICKED(IDC_BTN_TABLECELL_COLOR_BG, OnTableCellBgColorButton)
	ON_BN_CLICKED(IDC_BTN_TABLE_IMAGE_BG, OnTableImageBgButton)
	ON_BN_CLICKED(IDC_BTN_TABLE_COLOR_BORDER, OnTableColorBorderButton)
	ON_BN_CLICKED(IDC_BTN_TABLE_COLOR_BG, OnTableBgColorButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TABLE_COLUMNS, OnDeltaposTableColumnsSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TABLE_ROWS, OnDeltaposTableRowsSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TABLE_WIDTH, OnDeltaposTableWidthSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TABLE_CELL_SPACING, OnDeltaposTableCellSpacingSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TABLE_CELL_PADDING, OnDeltaposTableCellPaddingSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TABLE_BORDER_SIZE, OnDeltaposTableBorderSizeSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcDlgInsertTable message handlers


BOOL CFcDlgInsertTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CbCtrlColorCellBorder.Init();
	m_CbCtrlColorCellBg.Init();
	m_CbCtrlColorBorder.Init();
	m_CbCtrlColorBg.Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFcDlgInsertTable::OnTableCellColorBorderButton() { SelectCustomColor(m_CellColorBorder); }
void CFcDlgInsertTable::OnTableCellBgColorButton()     { SelectCustomColor(m_CellBgColor); }
void CFcDlgInsertTable::OnTableColorBorderButton()     { SelectCustomColor(m_ColorBorder); }
void CFcDlgInsertTable::OnTableBgColorButton()         { SelectCustomColor(m_BgColor); }

void CFcDlgInsertTable::OnTableImageBgButton() 
{
	CString filter;
	filter.LoadString(ID_IMAGE_FILTER);

	CFileDialog dlg (TRUE, "*.gif", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter);

	if (dlg.DoModal () == IDOK)
	{
		m_BgImage = dlg.GetPathName(); 
		if (m_BgImage.GetLength() > 0)
		{
			m_BgImage = "file://" + m_BgImage;
			UpdateData(FALSE);
		}
	}
}

void CFcDlgInsertTable::OnDeltaposTableColumnsSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nColumns, 1, 100, pResult);
}

void CFcDlgInsertTable::OnDeltaposTableRowsSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nRows, 1, 100, pResult);
}

void CFcDlgInsertTable::OnDeltaposTableWidthSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	if (m_WidthUnit == 0)
		DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nWidth, 1, 100, pResult);
	else
		DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nWidth, 1, 4000, pResult);
}

void CFcDlgInsertTable::OnDeltaposTableCellSpacingSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nSpacing, 0, 100, pResult);
}

void CFcDlgInsertTable::OnDeltaposTableCellPaddingSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nPadding, 0, 100, pResult);
}

void CFcDlgInsertTable::OnDeltaposTableBorderSizeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	DoSpin(((NM_UPDOWN*)pNMHDR)->iDelta, m_nBorderSize, 0, 100, pResult);
}

void CFcDlgInsertTable::DoSpin(int delta, int& value, int mini, int maxi, LRESULT* pResult) 
{
	if (delta < 0)
		value++;
	else
		value--;

	if (value < mini) 	value = maxi;
	if (value > maxi) 	value = mini;

	UpdateData(FALSE);
	*pResult = 0;
}

void CFcDlgInsertTable::OnOK() 
{
	UpdateData(TRUE);

	SAFEARRAYBOUND ABound[1];	
	ABound[0].lLbound = 0;
	ABound[0].cElements = 5;
	SAFEARRAY* pA = SafeArrayCreate(VT_VARIANT, 1, ABound);	
	ASSERT(pA);
	if(NULL == pA)
		return;

	LONG ix[1];
	VARIANT varElem;
	HRESULT hr = S_FALSE;

	// elmement 1: number of rows
	ix[0] = 0;
	VariantInit(&varElem);
	V_VT(&varElem) = VT_I4;
	V_I4(&varElem) = m_nRows;
	hr = SafeArrayPutElement(pA, ix, &varElem);
	VariantClear(&varElem);

	// elmement 2: number of columns
	ix[0] = 1;
	VariantInit(&varElem);
	V_VT(&varElem) = VT_I4;
	V_I4(&varElem) = m_nColumns;
	hr = SafeArrayPutElement(pA, ix, &varElem);
	VariantClear(&varElem);


	// elmement 3: table tag attributes
	ix[0] = 2;
	VariantInit(&varElem);
	V_VT(&varElem) = VT_BSTR;
	V_BSTR(&varElem) = GetTableAttributes().AllocSysString();
	hr = SafeArrayPutElement(pA, ix, &varElem);
	VariantClear(&varElem);

	// elmement 4: cell attributes
	ix[0] = 3;
	VariantInit(&varElem);
	V_VT(&varElem) = VT_BSTR;
	V_BSTR(&varElem) = GetCellAttributes().AllocSysString();;
	hr = SafeArrayPutElement(pA, ix, &varElem);
	VariantClear(&varElem);

	// elmement 5: table caption
	if (m_Title.GetLength() > 0)
	{
		ix[0] = 4;
		VariantInit(&varElem);
		V_VT(&varElem) = VT_BSTR;
		V_BSTR(&varElem) = m_Title.AllocSysString();
		hr = SafeArrayPutElement(pA, ix, &varElem);
		VariantClear(&varElem);
	}

	VARIANT va;
	VariantInit(&va);
	V_VT(&va) = VT_ARRAY;
	V_ARRAY(&va) = pA;

	m_pParent->ExecTriEditCommand(IDM_TRIED_INSERTTABLE, &va, NULL, FALSE);
    VariantClear(&va);		

	CDialog::OnOK();
}

CString CFcDlgInsertTable::GetTableAttributes() 
{
	CString Attr;
	CString SingleAttr;	
	
	if (m_nBorderSize > 0)
	{
		SingleAttr.Format(" border=%d", m_nBorderSize);
		Attr += SingleAttr;
	}

	if (m_nWidth > 0)
	{
		SingleAttr.Format(" width=%d", m_nWidth);
		if (m_WidthUnit == 0)
			SingleAttr += CString("%");
		Attr += SingleAttr;
	}

	if (m_nPadding > 0)
	{
		SingleAttr.Format(" cellPadding=%d", m_nPadding);
		Attr += SingleAttr;
	}

	if (m_nSpacing > 0)
	{
		SingleAttr.Format(" cellSpacing=%d", m_nSpacing);
		Attr += SingleAttr;
	}

	CString str = GetColorEntry(m_BgColor); 
	if (str.GetLength() > 0)
	{
		Attr += " bgColor=" + str;
	}

	str = GetColorEntry(m_ColorBorder); 
	if (str.GetLength() > 0)
	{
		Attr += " borderColor=" + str;
	}

	if (m_Align >= 0)
	{
		m_AlignCbCtrl.GetLBText(m_Align, str); 
		Attr += " align=" + str;
	}

	if (m_BgImage.GetLength() > 0)
	{
		Attr += " background=" + m_BgImage;
	}

	return Attr;
}

CString CFcDlgInsertTable::GetCellAttributes() 
{
	CString Attr;
	
	CString str = GetColorEntry(m_CellBgColor); 
	if (str.GetLength() > 0)
	{
		Attr += " bgColor=" + str;
	}

	str = GetColorEntry(m_CellColorBorder); 
	if (str.GetLength() > 0)
	{
		Attr += " borderColor=" + str;
	}

	if (m_CellAlign >= 0)
	{
		m_CellAlignCbCtrl.GetLBText(m_CellAlign, str); 
		Attr += " align=" + str;
	}
	
	if (!m_bWrapText)
	{
		Attr += " noWrap";
	}

	return Attr;
}



CString CFcDlgInsertTable::GetColorEntry(const CString& str) 
{
	if (str.GetLength() > 0)
	{
		CString col = FcGetHtmlColorNameFromString(str);
		if (col.GetLength() > 0)
		{
			return col;
		}
		else
		{
			col = FcGetHtmlColorValueFromString(str);
			if (FcCheckHtmlColor(col))
				return col;
		}
	}
	return CString("");
}

void CFcDlgInsertTable::SelectCustomColor(CString& ColEntry) 
{
	CColorDialog* pColorDlg = new CColorDialog;
	if (pColorDlg->DoModal() == IDOK)
	{
		ColEntry = FcColorRef2HtmlColor(pColorDlg->GetColor()); 
		int ColorIndex = FcGetHtmlColorIndex(ColEntry);
		if (ColorIndex >= 0)
			ColEntry = FcGetHtmlColorNameAndValue(ColorIndex);
		UpdateData(FALSE);
	}
	delete pColorDlg;		
}
