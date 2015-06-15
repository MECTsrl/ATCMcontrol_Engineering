// PropGSDSlaveModule.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigGSD.h"
#include "PropGSDSlaveModule.h"

#include "DP_Module.h"
#include "SheetConfigGSD.h"
#include "ConfGSD_Utils.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropGSDSlaveModule property page

IMPLEMENT_DYNCREATE(PropGSDSlaveModule, CPropertyPage)

//----------------------------- Constructor ---------------------------------*
PropGSDSlaveModule::PropGSDSlaveModule() : CPropertyPage(PropGSDSlaveModule::IDD)
{
	//{{AFX_DATA_INIT(PropGSDSlaveModule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Modules.RemoveAll();
	m_lMaxDataLen = 0;
	m_lMaxOutputLen = 0;
	m_lMaxInputLen = 0;
	m_lMaxModule = 0;
	m_lUsedModules = 0;
	m_lInputLen = 0;
	m_lOutputLen = 0;
	m_lIOLen = 0;
}

//----------------------------- Destructor ----------------------------------*
PropGSDSlaveModule::~PropGSDSlaveModule()
{
	// delete all Moduledefinitions
	for (int i = 0; i < m_Modules.GetSize(); i++)	{
		CDP_Module* pModule = (CDP_Module*)m_Modules.GetAt(i);
		delete pModule;
	}
	for (i = 0; i < m_UsedModules.GetSize(); i++)	{
		CDP_Module* pModule = (CDP_Module*)m_UsedModules.GetAt(i);
		delete pModule;
	}
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropGSDSlaveModule)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	if (	pDX->m_bSaveAndValidate
		&&	((CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES))->GetItemCount() < 1)
	{
		if (MessageBox(_T("At least one Module should be defined!\nDo you want to continue?"),
				   _T("Warning!"),
				   MB_ICONEXCLAMATION | MB_ICONWARNING | MB_YESNO) == IDNO)	{
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(PropGSDSlaveModule, CPropertyPage)
	//{{AFX_MSG_MAP(PropGSDSlaveModule)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAILABLE_MODULES, OnDblclkListAvailableModules)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AVAILABLE_MODULES, OnClkListModules)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USED_MODULES, OnClkListModules)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_ALL, OnButtonRemoveAll)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_ONE_DOWN, OnOneDown)
	ON_BN_CLICKED(IDC_ONE_UP, OnOneUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropGSDSlaveModule message handlers

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CListCtrl* pListAvai = (CListCtrl*)GetDlgItem(IDC_LIST_AVAILABLE_MODULES);
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);

	// TODO: Add extra initialization here
	
    CRect   rect;
    pListAvai->GetClientRect(&rect);
    int iWidth = rect.Width();
    int iThird = iWidth / 3;
    CString sTitle;
	sTitle.LoadString(IDS_MODULE_DESCRIPTION);
    pListAvai->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth-iThird, 0 );	// Create 1st Column
	sTitle.LoadString(IDS_MODULE_CONFIGBYTES);
    pListAvai->InsertColumn(1, sTitle, LVCFMT_LEFT, iThird+iWidth, 0 );	// Create 2nd Column

	pListUsed->GetClientRect(&rect);
	iWidth = rect.Width();
	iThird = iWidth/3;
	sTitle.LoadString(IDS_MODULE_DESCRIPTION);
    pListUsed->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth-iThird, 0 );	// Create 1st Column
	sTitle.LoadString(IDS_MODULE_CONFIGBYTES);
    pListUsed->InsertColumn(1, sTitle, LVCFMT_LEFT, iThird+iWidth, 0 );	// Create 2nd Column

	LV_ITEM lv;
	lv.iImage = I_IMAGECALLBACK;

	for (int i = 0; i < m_Modules.GetSize(); i++) {
		CString szModuleName;
		CDP_Module* pModule = (CDP_Module*)m_Modules.GetAt(i);
		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lv.iSubItem = 0;
		lv.iItem = i;
		lv.lParam = (DWORD) pModule;
		szModuleName = pModule->GetModuleName();
		lv.pszText = szModuleName.GetBuffer(szModuleName.GetLength());
		pListAvai->InsertItem(&lv);
		szModuleName.ReleaseBuffer();

		szModuleName.Empty();
		lv.mask = LVIF_TEXT;
		lv.iSubItem = 1;
		CByteArray* pCfg = pModule->GetConfigArray();
		
		CString szCfg;
		CStringArray saText;
		CUIntArray uiaInputs;
		CUIntArray uiaOutputs;
		int iInputs, iOutputs, iIOputs, iSeq;
		::DecodeCfgData(pCfg, saText, uiaInputs, uiaOutputs, iInputs, iOutputs, iIOputs, iSeq);
		for (int j = 0; j < saText.GetSize(); j++)	{
			szModuleName += saText[j];
		}
		szModuleName += _T(" (");
		BuildHexString(pCfg->GetData(), pCfg->GetSize(), szModuleName);
		szModuleName += _T(")");
		lv.pszText = szModuleName.GetBuffer(szModuleName.GetLength());
		pListAvai->SetItem(&lv);
		szModuleName.ReleaseBuffer();
	}

	for (i = 0; i < m_UsedModules.GetSize(); i++) {
			CDP_Module* pModule = (CDP_Module*)m_UsedModules.GetAt(i);

			AddToUsedModules(pModule);
    }

	if (m_bModular)	{
		CString szValue;
		szValue.Format(_T("%d"), m_lMaxModule);
		GetDlgItem(IDC_MAX_MODULES)->SetWindowText(szValue);
		szValue.Format(_T("%d"), m_lUsedModules);
		GetDlgItem(IDC_USED_MODULES)->SetWindowText(szValue);
		szValue.Format(_T("%d"), m_lMaxInputLen);
		GetDlgItem(IDC_MAX_INPUT_LENGTH)->SetWindowText(szValue);
		szValue.Format(_T("%d"), m_lMaxOutputLen);
		GetDlgItem(IDC_MAX_OUTPUT_LENGTH)->SetWindowText(szValue);
		szValue.Format(_T("%d"), m_lMaxDataLen);
		GetDlgItem(IDC_MAX_IN_OUTPUT_LENGTH)->SetWindowText(szValue);
	}
	else	{
//		pListAvai->EnableWindow(FALSE);
//		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(FALSE);
//		GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_ONE_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_ONE_DOWN)->EnableWindow(FALSE);
//		GetDlgItem(IDC_LIST_USED_MODULES)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAX_MODULES)->SetWindowText(_T(""));
		GetDlgItem(IDC_USED_MODULES)->SetWindowText(m_lUsedModules ? _T("1") : _T("0"));
		GetDlgItem(IDC_MAX_INPUT_LENGTH)->SetWindowText(_T(""));
		GetDlgItem(IDC_MAX_OUTPUT_LENGTH)->SetWindowText(_T(""));
		GetDlgItem(IDC_MAX_IN_OUTPUT_LENGTH)->SetWindowText(_T(""));
		
	}

	UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::AddNewModule(CDP_Module* pModule)
{
	if (m_Modules.Add(pModule) >= 0)
		return TRUE;
	else
		return FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::SetMaxInputLen(LONG lLen)
{
	m_lMaxInputLen = lLen;
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::SetMaxOutputLen(LONG lLen)
{
	m_lMaxOutputLen = lLen;
	return TRUE;

}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::SetMaxModule(LONG lMaxModule)
{
	m_lMaxModule = lMaxModule;
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::SetMaxDataLen(LONG lLen)
{
	m_lMaxDataLen = lLen;
	return TRUE;

}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnClkListModules(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UpdateUI();
	*pResult = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnDblclkListAvailableModules(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	ASSERT(pListUsed != NULL);
	if (pListUsed == NULL)
		return;
	if (!m_bModular && (pListUsed->GetItemCount() > 0))	{
		return;
	}
	else	{
		OnButtonAdd();
	}
	*pResult = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	if (m_lUsedModules < m_lMaxModule)
	{
		CListCtrl* pListAvai = (CListCtrl*)GetDlgItem(IDC_LIST_AVAILABLE_MODULES);
		ASSERT(pListAvai != NULL);
		if (pListAvai == NULL)
			return;
		LV_ITEM lv;
		::memset(&lv, 0, sizeof(LV_ITEM));
		lv.iItem	= pListAvai->GetNextItem(-1 ,LVNI_SELECTED);
		lv.mask		= LVIF_PARAM;
		while (lv.iItem != -1 && m_lUsedModules < m_lMaxModule)	{
			pListAvai->GetItem(&lv);
			CDP_Module* pModule = new CDP_Module(*((CDP_Module*)lv.lParam));

			if (!AddToUsedModules(pModule))	{
				delete pModule;
			}
			else	{
				m_lUsedModules++;
			}

			//Inc counter for used Modules and view the Number in Dialog
			CString szValue;
			szValue.Format(_T("%d"), m_lUsedModules);
			GetDlgItem(IDC_USED_MODULES)->SetWindowText(szValue);

			lv.iItem = pListAvai->GetNextItem(lv.iItem ,LVNI_SELECTED);
		}
		pListAvai->SetFocus();
	}
	CreateUsedModuleArray();
	UpdateUI();

    GetDlgItem(IDC_LIST_AVAILABLE_MODULES)->SetFocus();
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::AddToUsedModules(CDP_Module* pModule, int iItempos/* = -1*/)
{
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	ASSERT(pListUsed != NULL);
	if (pListUsed == NULL)
		return FALSE;
	CByteArray* pCfg = pModule->GetConfigArray();	// Get configbyte-array
	CStringArray Strings;
	CUIntArray	 Offsets, Lengths;
	int iInput = 0, iOutput = 0, iIOput = 0, iSeq = 0;

	::DecodeCfgData(pCfg, Strings, Offsets, Lengths, iInput, iOutput, iIOput, iSeq);
	if (iItempos < 0 && m_bModular)	{
		CString szError(_T("Datalength error"));
		if (m_lInputLen + iInput > m_lMaxInputLen)	{
			MessageBox(_T("Length of Inputs is too long!"), szError, MB_ICONEXCLAMATION | MB_ICONWARNING | MB_OK);
			return FALSE;
		}
		if (m_lOutputLen + iOutput > m_lMaxOutputLen)	{
			MessageBox(_T("Length of Outputs is too long!"), szError, MB_ICONEXCLAMATION | MB_ICONWARNING | MB_OK);
			return FALSE;
		}
		if (m_lIOLen + iIOput > m_lMaxDataLen)	{
			MessageBox(_T("Datalength (Inputs + Outputs) is too long!"), szError, MB_ICONEXCLAMATION | MB_ICONWARNING | MB_OK);
			return FALSE;
		}
		m_lInputLen  += iInput;
		m_lOutputLen += iOutput;
		m_lIOLen	 += iIOput;
	}
	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	CString szModuleName;
	lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lv.iSubItem = 0;

	if (iItempos < 0)
		lv.iItem = pListUsed->GetItemCount();	// Set Item to last position of List
	else
		lv.iItem = iItempos;			// Set Item to given position
	lv.lParam = (DWORD) pModule;			// Modulepointer as parameter
	szModuleName = pModule->GetModuleName();
	lv.pszText = szModuleName.GetBuffer(szModuleName.GetLength());
	if (pListUsed->InsertItem(&lv) < 0)	{	// write moduledescription in view
		szModuleName.ReleaseBuffer();
		return FALSE;
	}
	szModuleName.ReleaseBuffer();

	szModuleName.Empty();
	lv.mask = LVIF_TEXT;
	lv.iSubItem = 1;
	for (int j = 0; j < Strings.GetSize(); j++)	{
		szModuleName += Strings[j];
	}
	szModuleName += _T(" (");
	BuildHexString(pCfg->GetData(), pCfg->GetSize(), szModuleName); // create hexstring
	szModuleName += _T(")");
	lv.pszText = szModuleName.GetBuffer(szModuleName.GetLength());
	if (!pListUsed->SetItem(&lv))	{	// write Conf. Bytes in view
		szModuleName.ReleaseBuffer();
		return FALSE;
	}
	szModuleName.ReleaseBuffer();

	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnButtonRemoveAll() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	ASSERT(pListUsed != NULL);
	if (pListUsed == NULL)
		return;

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pListUsed->GetNextItem(-1 ,LVNI_ALL);
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pListUsed->GetItem(&lv);
		CDP_Module* pModule = (CDP_Module*)lv.lParam;
		delete pModule;
		lv.iItem = pListUsed->GetNextItem(lv.iItem, LVNI_ALL);
	}

	if(pListUsed->DeleteAllItems())	{	// Remove all Items from Used Moduleview
		CString szValue;
		m_lUsedModules = 0;
		szValue.Format(_T("%d"), m_lUsedModules);
		GetDlgItem(IDC_USED_MODULES)->SetWindowText(szValue);
	}
	CreateUsedModuleArray();
	UpdateUI();
    GetDlgItem(IDC_LIST_AVAILABLE_MODULES)->SetFocus();

	m_lInputLen = 0;
	m_lOutputLen = 0;
	m_lIOLen = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	ASSERT(pListUsed != NULL);
	if (pListUsed == NULL)
		return;
	int iItem = pListUsed->GetNextItem(-1 ,LVNI_SELECTED);
	if (iItem == -1)
		return;

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.mask		= LVIF_PARAM;
	lv.iItem	= iItem;
	pListUsed->GetItem(&lv);

	CDP_Module* pModule = (CDP_Module*)lv.lParam;
	CStringArray Strings;
	CUIntArray	 Offsets, Lengths;
	int iInput = 0, iOutput = 0, iIOput = 0, iSeq = 0;
	::DecodeCfgData(pModule->GetConfigArray(), Strings, Offsets, Lengths, iInput, iOutput, iIOput, iSeq);
	m_lInputLen -= iInput;
	m_lOutputLen -= iOutput;
	m_lIOLen -= iIOput;

	if (pListUsed->DeleteItem(iItem))	{
		//Inc counter for used Modules and view the Number in Dialog
		CString szValue;
		m_lUsedModules--;
		szValue.Format(_T("%d"), m_lUsedModules);
		GetDlgItem(IDC_USED_MODULES)->SetWindowText(szValue);
		if (iItem >= pListUsed->GetItemCount())
			iItem = pListUsed->GetItemCount() - 1;
		pListUsed->SetFocus();
		pListUsed->SetItem(iItem, 0, LVIF_STATE, NULL, 0,
		         LVIS_FOCUSED | LVIS_SELECTED,
				 LVIS_FOCUSED | LVIS_SELECTED, NULL);
		delete pModule;
	}
	CreateUsedModuleArray();
	UpdateUI();
    GetDlgItem(IDC_LIST_USED_MODULES)->SetFocus();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnOneDown() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	ASSERT(pListUsed != NULL);
	if (pListUsed == NULL)
		return;
	int iItem = pListUsed->GetNextItem(-1 ,LVNI_SELECTED);
	if (iItem < 0 || iItem >= pListUsed->GetItemCount()-1)
		return;		// return if no Item is selected

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem = iItem;
	lv.mask = LVIF_PARAM;
	pListUsed->GetItem(&lv);

	if (pListUsed->DeleteItem(iItem))	{
		CDP_Module* pModule = (CDP_Module*)lv.lParam;	// save Modulepointer for futur adding
		AddToUsedModules(pModule, iItem + 1);

		// Set Focus to ListView and select moved Item
		pListUsed->SetFocus();	
		pListUsed->SetItem(iItem+1, 0, LVIF_STATE, NULL, 0,
		         LVIS_FOCUSED | LVIS_SELECTED,
				 LVIS_FOCUSED | LVIS_SELECTED, NULL);

		CreateUsedModuleArray();	// Updates the Array with used modules
	}
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::OnOneUp() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	ASSERT(pListUsed != NULL);
	if (pListUsed == NULL)
		return;
	int iItem = pListUsed->GetNextItem(-1 ,LVNI_SELECTED);
	if (iItem <= 0)
		return;		// return if no Item is selected

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem = iItem;
	lv.mask = LVIF_PARAM;
	pListUsed->GetItem(&lv);

	if (pListUsed->DeleteItem(iItem))	{
		CDP_Module* pModule = (CDP_Module*)lv.lParam;	// save Modulepointer for futur adding
		AddToUsedModules(pModule, iItem - 1);

		// Set Focus to ListView and select moved Item
		pListUsed->SetFocus();
		pListUsed->SetItem(iItem-1, 0, LVIF_STATE, NULL, 0,
		         LVIS_FOCUSED | LVIS_SELECTED,
				 LVIS_FOCUSED | LVIS_SELECTED, NULL);

		CreateUsedModuleArray();	// Updates the Array with used modules
	}
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::UpdateUI()
{
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	CListCtrl* pListAvai = (CListCtrl*)GetDlgItem(IDC_LIST_AVAILABLE_MODULES);
	ASSERT(pListUsed != NULL);
	ASSERT(pListAvai != NULL);
	if (pListUsed == NULL || pListAvai == NULL)
		return;

	int iSelAvai = pListAvai->GetNextItem(-1 ,LVNI_SELECTED);
	int iSelUsed = pListUsed->GetNextItem(-1 ,LVNI_SELECTED);

	if (!m_bModular)	{
		if (pListUsed->GetItemCount() > 0)	{
			GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(iSelAvai >= 0);
		}
	}
    else
    {
	    GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(iSelAvai >= 0);
    }

	GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(pListUsed->GetItemCount() > 0);
	GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(iSelUsed >= 0);
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::BuildHexString(BYTE const * pCfgData,
										int icntBytes,
										CString & sCfgTxt)
{
/*>>>> 
build a hex string from <cntBytes> bytes in <pCfgData>

I   pCfgData:   ptr to special format byte
I   cntBytes:   number of bytes to display
O   sCfgTxt:    string to receive hex string

Result
  void
<<<<*/
    CString sNum;
    for (int i = 0; i < icntBytes; i++)
    {
        if (i == 0) {
            sNum.Format(_T("0x%02X"), *pCfgData);
        } else {
            sNum.Format(_T(", 0x%02X"), *pCfgData);
        }
        sCfgTxt += sNum;
        pCfgData++;
    }
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::GetUsedModules(CObArray * pModules)
{
	pModules->RemoveAll();
	for (int i = 0; i < m_UsedModules.GetSize(); i++){
		CDP_Module* pModule = (CDP_Module*)m_UsedModules.GetAt(i);
		pModules->Add(pModule);
	}

  return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::CreateCfgByteArray(CByteArray* pCfgBytes)
{
	for (int iCount = 0; iCount < m_UsedModules.GetSize(); iCount++)	{
		CDP_Module* pModule	= (CDP_Module*)m_UsedModules.GetAt(iCount);
		CByteArray* pArray	= pModule->GetConfigArray();
		for( int i = 0; i <= pArray->GetUpperBound(); i++)	{
			pCfgBytes->Add(pArray->GetAt(i));
		}
	}
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::CreateUsedModuleArray()
{
	m_UsedModules.RemoveAll();
	CListCtrl* pListUsed = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MODULES);
	if (pListUsed == NULL)
		return FALSE;

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.mask		= LVIF_PARAM;

	for (int i = 0; i < pListUsed->GetItemCount(); i++)	{
		lv.iItem	= i;
		pListUsed->GetItem(&lv);
		CDP_Module* pModule = (CDP_Module*)lv.lParam;

		m_UsedModules.Add(pModule);
	}
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnKillActive();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDSlaveModule::SetModularType(BOOL bModular)
{
	m_bModular = bModular;
}

//----------------------------- FUNCTION -------------------------------------*
CString PropGSDSlaveModule::GetState()
{
    int nModules = m_UsedModules.GetSize();
    CString retval;
    retval.Format(_T("%d;"), nModules);
    for (int i=0; i<nModules; i++) {
        CDP_Module* pModule = (CDP_Module*)m_UsedModules.GetAt(i);
        CString val;
        val.Format(_T("%d;"), pModule->GetModuleId());
        val += pModule->GetState();
        retval += val;
    }
    return retval;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDSlaveModule::SetState(LPCTSTR &buffer)
{
    int nModules = GetNextValue(buffer);
    for (int i=0; i<nModules; i++) {
        int moduleID = GetNextValue(buffer);
        if (moduleID<0 && moduleID>=m_Modules.GetSize())
            return FALSE;

        CDP_Module* pModule = new CDP_Module(*((CDP_Module*)m_Modules[moduleID]));
        if (!pModule)
            return FALSE;

        _ASSERTE(pModule->GetModuleId() == moduleID);
        if (!pModule->SetState(buffer))
            return FALSE;

        m_UsedModules.Add(pModule);
        m_lUsedModules++;
    }
    return TRUE;
}
