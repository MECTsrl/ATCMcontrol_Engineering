

#include "stdafx.h"
#include "CeWatchView.h"
#include "CeWatchList.h"
#include "CeSymbol.h"
#include "CeWatchSheetInsVar.h"
#include "PageInsVarList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageInsVarList property page

IMPLEMENT_DYNCREATE(CPageInsVarList, CPropertyPage)

CPageInsVarList::CPageInsVarList(
	CCeWatchList* pWatchList/*=NULL*/, 
	CCeWatchSheetInsVar* pParent/*=NULL*/, 
	CCeSymbol* pSymbolTable/*=NULL*/) : 
		CPropertyPage(CPageInsVarList::IDD)
{
	//{{AFX_DATA_INIT(CPageInsVarList)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pWatchList = pWatchList;
	m_pSymbolTable = pSymbolTable;
}

CPageInsVarList::~CPageInsVarList()
{
}

void CPageInsVarList::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageInsVarList)
	DDX_Control(pDX, IDC_LIST_VARNAME, m_VarListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageInsVarList, CPropertyPage)
	//{{AFX_MSG_MAP(CPageInsVarList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VARNAME, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VARNAME, OnDblClick)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_VARNAME, OnGetDispInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageInsVarList message handlers


BOOL CPageInsVarList::OnInitDialog() 
{
	CWaitCursor wait;   // display wait cursor

	CPropertyPage::OnInitDialog();
	m_VarListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect r;
	m_VarListCtrl.GetClientRect(&r);
	
	// create header
	CString strHeader;

	// type column
	strHeader.LoadString(IDS_HEADER_TYPE);
	m_VarListCtrl.InsertColumn(1, strHeader, LVCFMT_LEFT, 60, -1);

	// name column
	strHeader.LoadString(IDS_HEADER_NAME);
	m_VarListCtrl.InsertColumn(1, strHeader, LVCFMT_LEFT, r.Width() - m_VarListCtrl.GetColumnWidth(0), -1);

	// fill list
	m_SymArray.SetSize(0, 1000);
	InsertItems(m_pSymbolTable);
	m_SymArray.FreeExtra();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CPageInsVarList::InsertItems(CCeSymbol* pVar) 
{
	const CCeWatchType type = pVar->GetType();

	// skip externals
    if (CCeWatchElement::IsVarKindVisible(type.GetVarKind()))
	{
		// show variables only
		if (type.IsVarType())
		{
			m_VarListCtrl.InsertItem(m_SymArray.GetSize(), "");
			m_SymArray.Add(pVar);
		}

		// show children, but do not show array
		if (!type.IsArray()) 
		{
			int iCount = pVar->GetChildrenCount();
			for (int ii = 0; ii < iCount; ii++)
			{
				CCeSymbol* pChildVar = pVar->GetChild(ii);
				ASSERT(pChildVar != NULL);
				if (pChildVar != NULL)
                {
					InsertItems(pChildVar);
                }
                else
                {
                    //m_VarListCtrl.SetItemImage(hItem, 11, 11);
	                CString strMessage;
	                strMessage.FormatMessage(IDS_ERROR_DBI_NOT_FOUND, pVar->GetFullName());
                    ::AfxMessageBox(strMessage, MB_ICONSTOP);
                    break;
                }
			}
		}
	}
	return true; 
}

void CPageInsVarList::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	ASSERT(pItem->iItem < m_SymArray.GetSize());
	if (pItem->iItem < m_SymArray.GetSize())
	{
		CCeSymbol* pVar = m_SymArray.GetAt(pItem->iItem);
		if (pItem->mask & LVIF_TEXT) //valid text buffer?
		{    
			switch(pItem->iSubItem)
			{
			case 0: //fill in main text            
				lstrcpy(pItem->pszText, pVar->GetType().GetName());            
				break;
			case 1: //fill in sub item 1 text            
				lstrcpy(pItem->pszText, pVar->GetFullName());            
				break;
			}
		}
	}

	*pResult = 0;
}


void CPageInsVarList::Add() 
{
	for (int ii = 0; ii < m_VarListCtrl.GetItemCount(); ii++)
	{
		if (m_VarListCtrl.GetItemState(ii, LVIS_SELECTED))
		{
			CCeSymbol* pVar = m_SymArray.GetAt(ii);
			if (pVar != NULL && pVar->GetType().IsVarType())
				m_pWatchList->InsertVariable(pVar);
		}
	}
	m_pWatchList->UpdateConnections();	
}

bool CPageInsVarList::CanAdd() 
{
	for (int ii = 0; ii < m_VarListCtrl.GetItemCount(); ii++)
	{
		if (m_VarListCtrl.GetItemState(ii, LVIS_SELECTED))
			return true;
	}
	return false;
}


void CPageInsVarList::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_pParent->SetModified(CanAdd());
	*pResult = 0;
}

void CPageInsVarList::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Add();
	*pResult = 0;
}


BOOL CPageInsVarList::OnApply() 
{
	if (m_pParent->GetActivePage() == this)
		Add();	
	return CPropertyPage::OnApply();
}


BOOL CPageInsVarList::OnSetActive() 
{
	m_pParent->SetModified(CanAdd());
	return CPropertyPage::OnSetActive();
}
