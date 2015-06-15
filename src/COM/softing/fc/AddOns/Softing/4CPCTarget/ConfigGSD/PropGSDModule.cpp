// PropGSDModule.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigGSD.h"
#include "PropGSDModule.h"
#include "HexEdit.hpp"
#include "ExtParam.h"
#include "ExtUserParamValues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropGSDModule property page

IMPLEMENT_DYNCREATE(PropGSDModule, CPropertyPage)

//---------------------------- CONSTRUCTOR -----------------------------------*
PropGSDModule::PropGSDModule() : CPropertyPage(PropGSDModule::IDD),
m_bExtUserPrms(FALSE),
m_dwUserPrmDataLen(0),
m_pSelModule(NULL)
{
	//{{AFX_DATA_INIT(PropGSDModule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ctrlUserPrm.SetLinePropperties(8, 3);
    m_pModule = NULL;
	m_bUserChanged = FALSE;
}

//---------------------------- CONSTRUCTOR -----------------------------------*
PropGSDModule::PropGSDModule(PropGSDSlaveModule* pModule) : CPropertyPage(PropGSDModule::IDD),
m_bExtUserPrms(FALSE),
m_dwUserPrmDataLen(0),
m_pSelModule(NULL)
{
	m_ctrlUserPrm.SetLinePropperties(8, 3);
	m_pModule = pModule;
	m_bUserChanged = FALSE;
}

//---------------------------- DESTRUCTOR ------------------------------------*
PropGSDModule::~PropGSDModule()
{
	POSITION pos = ExtUserParams.GetHeadPosition();
	while (pos)	{
		ExtParam* pParam = (ExtParam*)ExtUserParams.GetNext(pos);
		delete pParam;
	}
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropGSDModule)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropGSDModule, CPropertyPage)
	//{{AFX_MSG_MAP(PropGSDModule)
	ON_BN_CLICKED(IDC_DEVICE_PRM_ADD, OnDevicePrmAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAILABLE_EXT_USER_PARAM, OnDblclkAvailableExtUserParam)
	ON_BN_CLICKED(IDC_DEVICE_PRM_REMOVE, OnDevicePrmRemove)
	ON_BN_CLICKED(IDC_DEVICE_PRM_REMOVE_ALL, OnDevicePrmRemoveAll)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USED_EXT_USER_PARAMS, OnDblclkListUsedExtUserParams)
	ON_CBN_SELCHANGE(IDC_COMBO_USED_MODULES, OnSelchangeUsedModules)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAIL_MOD_EXTPRM, OnDblclkAvailModExtPrm)
	ON_BN_CLICKED(IDC_MODULE_EXTPRM_ADD, OnModuleExtprmAdd)
	ON_BN_CLICKED(IDC_MODULE_EXTPRM_REMOVE, OnModuleExtprmRemove)
	ON_BN_CLICKED(IDC_MODULE_EXTPRM_REMOVE_ALL, OnModuleExtprmRemoveAll)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USED_MOD_EXTPRM, OnDblclkUsedModExtPrm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropGSDModule message handlers

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDModule::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_USER_PRM_LINECOUNT)->SetWindowText(_T("0.\r\n8.\r\n16."));
	VERIFY(m_ctrlUserPrm.SubclassDlgItem(IDC_DP_USER_PARAM, this));

	InitListCrtls();	// init all listctrls with columnsize and columntext
	FillDeviceExtUserPrm(); // show all editable ExtUserParams
	OnSetActive();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::InitListCrtls()
{
    CRect   rect;
    CString sTitle;
	int iWidth;
	int iThird;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_AVAILABLE_EXT_USER_PARAM);
	if (pList)	{
		pList->GetClientRect(&rect);
		iWidth = rect.Width();
		iThird = iWidth / 3;
		sTitle.LoadString(IDS_EXTUSERPARAM_DESCRIPTION);
		pList->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth, 0 );	// Create 1st Column
	//	sTitle.LoadString(IDS_MODULE_CONFIGBYTES);
	//	pListAvai->InsertColumn(1, sTitle, LVCFMT_LEFT, iThird, 0 );	// Create 2nd Column
	}

	pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_EXT_USER_PARAMS);
	if (pList)	{
		pList->GetClientRect(&rect);
		iWidth = rect.Width();
		iThird = iWidth/3;
		sTitle.LoadString(IDS_EXTUSERPARAM_DESCRIPTION);
		pList->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth, 0 );	// Create 1st Column
	//	sTitle.LoadString(IDS_MODULE_CONFIGBYTES);
	//	pListUsed->InsertColumn(1, sTitle, LVCFMT_LEFT, iThird, 0 );	// Create 2nd Column
	}

	pList = (CListCtrl*)GetDlgItem(IDC_LIST_AVAIL_MOD_EXTPRM);
	if (pList)	{
		pList->GetClientRect(&rect);
		iWidth = rect.Width();
		iThird = iWidth/3;
		sTitle.LoadString(IDS_EXTUSERPARAM_DESCRIPTION);
		pList->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth, 0 );	// Create 1st Column
	}

	pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MOD_EXTPRM);
	if (pList)	{
		pList->GetClientRect(&rect);
		iWidth = rect.Width();
		iThird = iWidth/3;
		sTitle.LoadString(IDS_EXTUSERPARAM_DESCRIPTION);
		pList->InsertColumn(0, sTitle, LVCFMT_LEFT, iWidth, 0 );	// Create 1st Column
	}
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDModule::SetUserPrmData(VARIANT& rvtBytes)
{
	HRESULT hr;
    ASSERT(rvtBytes.vt == (VT_UI1 | VT_ARRAY));
    if (rvtBytes.vt == (VT_UI1 | VT_ARRAY))
    {
        m_DefaultUserPrmData.RemoveAll();

        SAFEARRAY   *psaBytes = rvtBytes.parray;
        long    lBound = 0, uBound = 0;
        hr = ::SafeArrayGetLBound(psaBytes, 1, &lBound);
        _ASSERTE(SUCCEEDED(hr));
        hr = ::SafeArrayGetUBound(psaBytes, 1, &uBound);
        _ASSERTE(SUCCEEDED(hr));
        // loop thru all objects in this level:
        for (long i = lBound; i <= uBound; i++)
        {
            BYTE    ucVal;
            hr = ::SafeArrayGetElement(psaBytes, &i, &ucVal);
            _ASSERTE(SUCCEEDED(hr));
            if (FAILED(hr)) {
                continue;
            }
			m_DefaultUserPrmData.Add(ucVal);
            // next byte
        }
		return TRUE;
    }
	return FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDModule::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CComboBox* pUsedModules = (CComboBox*)GetDlgItem(IDC_COMBO_USED_MODULES);
	if (pUsedModules != NULL)	{
		arModules.RemoveAll();
		pUsedModules->ResetContent();

		m_pModule->GetUsedModules(&arModules);

		for (int i = 0; i < arModules.GetSize(); i++) {
			CDP_Module* pModule = (CDP_Module*)arModules.GetAt(i);
            if (pModule->GetUserPrmLen() != 0) {
			    CString szName;
                szName.Format(_T("%d: "), i+1);
                szName += pModule->GetModuleName();
			    int index = pUsedModules->AddString(szName);
			    pUsedModules->SetItemData(index, (DWORD)pModule);
            }
		}
		pUsedModules->SetCurSel(0);
        m_pSelModule = NULL;
	}
	OnSelchangeUsedModules();
	CreateExtUserParams();
	return CPropertyPage::OnSetActive();
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDModule::GetUserPrmData(CByteArray * pUserPrms)
{
	pUserPrms->RemoveAll();
	CreateExtUserParams();
	if (m_bExtUserPrms || (arModules.GetSize() > 0))	{
		pUserPrms->SetSize(m_ExtUserPrmData.GetSize());
		for (int i = 0; i < m_ExtUserPrmData.GetSize(); i++)	{
			pUserPrms->SetAt(i, m_ExtUserPrmData.GetAt(i));
		}
	}
	else	{
		pUserPrms->SetSize(m_DefaultUserPrmData.GetSize());
		for (int i = 0; i < m_DefaultUserPrmData.GetSize(); i++)	{
			pUserPrms->SetAt(i, m_DefaultUserPrmData.GetAt(i));
		}
	}
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropGSDModule::SetUserChanged(BOOL bNew)
{
	BOOL	bOld = m_bUserChanged;
	m_bUserChanged = bNew;
	return bOld;
}


//------------------------------------------------------------------*
/**
 * Add a ExtParam data object to the slave GSD module.
 * @param		ExtParam* pElement - ExtParam data object.
 *              Shall not be NULL.
*/
void PropGSDModule::AddExtUserParam(ExtParam* pElement)
{
	if (pElement != NULL)
		ExtUserParams.AddTail(pElement);
}


//------------------------------------------------------------------*
/**
 * Adds ExtParam data object to default user param data array. 
 * ExtParam data object describes Ext_User_Prm_Data_Const/Ref from 
 * GSD. The default user param data array will be modified as 
 * appropriate.
 *
 * @param			ExtParam* pExtParam - extended user param block.
 *                  Shall not be NULL.
 * @see             ModifyParamArray()
*/
void PropGSDModule::AddDefaultUserPrmData(ExtParam* pExtParam)
{
    ModifyParamArray(m_DefaultUserPrmData,pExtParam);
}


//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::FillDeviceExtUserPrm()
{
	CListCtrl* pExt = (CListCtrl*)GetDlgItem(IDC_LIST_AVAILABLE_EXT_USER_PARAM);
	CListCtrl* pUsedExt = (CListCtrl*)GetDlgItem(IDC_LIST_USED_EXT_USER_PARAMS);
	if (pExt == NULL || pUsedExt == NULL)
		return;

	pExt->DeleteAllItems();
	pUsedExt->DeleteAllItems();
	LV_ITEM lv;
	POSITION pos = ExtUserParams.GetHeadPosition();
	int index = 0;
	while (pos)	{
		ExtParam* pParam = (ExtParam*)ExtUserParams.GetNext(pos);
		if (!pParam->IsConstantParam())	{
			::memset(&lv, 0, sizeof(LV_ITEM));
			lv.mask = LVIF_TEXT | LVIF_PARAM;
			lv.iItem = index++;
			lv.pszText = (LPTSTR)(LPCTSTR)pParam->GetDescription();
			lv.lParam = (DWORD)pParam;

			if (pParam->IsUsed())
				pUsedExt->InsertItem(&lv);
			else
				pExt->InsertItem(&lv);
		}
	}

	if (pUsedExt->GetItemCount() > 0)
		m_bExtUserPrms = TRUE;
	else
		m_bExtUserPrms = FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::FillModuleExtUserPrm(CDP_Module * pModule)
{
	CListCtrl* pExt = (CListCtrl*)GetDlgItem(IDC_LIST_AVAIL_MOD_EXTPRM);
	CListCtrl* pUsedExt = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MOD_EXTPRM);
	if (pExt == NULL || pUsedExt == NULL)	{
		ASSERT(FALSE);
		return;
	}

	pExt->DeleteAllItems( );
	pUsedExt->DeleteAllItems();
    if (pModule == NULL) {
        return;
    }

	CObList* pExtParams = pModule->GetExtParams();
	POSITION pos = pExtParams->GetHeadPosition();
	LV_ITEM lv;
	int index = 0;
	while (pos)	{
		ExtParam* pParam = (ExtParam*)pExtParams->GetNext(pos);
		if (!pParam->IsConstantParam())	{
			::memset(&lv, 0, sizeof(LV_ITEM));
			lv.mask = LVIF_TEXT | LVIF_PARAM;
			lv.iItem = index++;
			lv.pszText = (LPTSTR)(LPCTSTR)pParam->GetDescription();
			lv.lParam = (DWORD)pParam;

			if (pParam->IsUsed())
				pUsedExt->InsertItem(&lv);
			else
				pExt->InsertItem(&lv);
		}
	}
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDblclkListUsedExtUserParams(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_EXT_USER_PARAMS);
	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_SELECTED);
	if (lv.iItem == -1)
		return;	// exit function if nothing is selected
	lv.mask		= LVIF_PARAM;
	pList->GetItem(&lv);
	ExtParam* pElement = (ExtParam*)lv.lParam;
	ExtUserParamValues ExtUserPrm(pElement);
	if (ExtUserPrm.DoModal() == IDCANCEL)
		return;
	
	CreateExtUserParams();	// Create new UserParamarray
	*pResult = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDblclkAvailableExtUserParam(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnDevicePrmAdd();

	*pResult = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDevicePrmAdd() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_AVAILABLE_EXT_USER_PARAM);
	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_SELECTED);
	if (lv.iItem == -1)
		return;	// exit function if nothing is selected
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;

		ExtUserParamValues ExtUserPrm(pElement);
		if (ExtUserPrm.DoModal() == IDCANCEL)
			return;

		pElement->SetUsed(TRUE);
		//Inc counter for used Modules and view the Number in Dialog
		lv.iItem = pList->GetNextItem(lv.iItem ,LVNI_SELECTED);
	}
	pList->SetFocus();

	FillDeviceExtUserPrm();
	CreateExtUserParams();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDevicePrmRemove() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_EXT_USER_PARAMS);

	if (pList == NULL)
		return;

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_SELECTED);
	if (lv.iItem == -1)
		return;	// exit function if nothing is selected
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;
		pElement->SetUsed(FALSE);
		pElement->SetUsedValue(pElement->GetDefaultValue());
		lv.iItem = pList->GetNextItem(lv.iItem ,LVNI_SELECTED);
	}
	pList->SetFocus();

	FillDeviceExtUserPrm();
	CreateExtUserParams();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDevicePrmRemoveAll()
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_EXT_USER_PARAMS);

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_ALL);
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;
		pElement->SetUsed(FALSE);
		pElement->SetUsedValue(pElement->GetDefaultValue());
		lv.iItem = pList->GetNextItem(lv.iItem, LVNI_ALL);
	}

	FillDeviceExtUserPrm();
	CreateExtUserParams();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDblclkUsedModExtPrm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MOD_EXTPRM);
	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_SELECTED);
	if (lv.iItem == -1)
		return;	// exit function if nothing is selected
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;

		ExtUserParamValues ExtUserPrm(pElement);
		if (ExtUserPrm.DoModal() == IDCANCEL)
			return;

		lv.iItem = pList->GetNextItem(lv.iItem, LVNI_SELECTED);
	}
	pList->SetFocus();

	CreateExtUserParams();
	*pResult = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnDblclkAvailModExtPrm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnModuleExtprmAdd();

	*pResult = 0;
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnModuleExtprmAdd() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_AVAIL_MOD_EXTPRM);
	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_SELECTED);
	if (lv.iItem == -1)
		return;	// exit function if nothing is selected
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;

		ExtUserParamValues ExtUserPrm(pElement);
		if (ExtUserPrm.DoModal() == IDCANCEL)
			return;

		pElement->SetUsed(TRUE);
		//Inc counter for used Modules and view the Number in Dialog
		lv.iItem = pList->GetNextItem(lv.iItem ,LVNI_SELECTED);
	}
	pList->SetFocus();

	FillModuleExtUserPrm(m_pSelModule);
	CreateExtUserParams();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnModuleExtprmRemove() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MOD_EXTPRM);
	if (pList == NULL)
		return;

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_SELECTED);
	if (lv.iItem == -1)
		return;	// exit function if nothing is selected
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;
		pElement->SetUsed(FALSE);
		pElement->SetUsedValue(pElement->GetDefaultValue());
		lv.iItem = pList->GetNextItem(lv.iItem, LVNI_SELECTED);
	}
	pList->SetFocus();

	FillModuleExtUserPrm(m_pSelModule);
	CreateExtUserParams();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnModuleExtprmRemoveAll() 
{
	// TODO: Add your control notification handler code here
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_USED_MOD_EXTPRM);
	if (pList == NULL || m_pSelModule == NULL)
		return;

	LV_ITEM lv;
	::memset(&lv, 0, sizeof(LV_ITEM));
	lv.iItem	= pList->GetNextItem(-1 ,LVNI_ALL);
	lv.mask		= LVIF_PARAM;
	while (lv.iItem != -1)	{
		pList->GetItem(&lv);
		ExtParam* pElement = (ExtParam*)lv.lParam;
		pElement->SetUsed(FALSE);
		pElement->SetUsedValue(pElement->GetDefaultValue());
		lv.iItem = pList->GetNextItem(lv.iItem, LVNI_ALL);
	}
	FillModuleExtUserPrm(m_pSelModule);
	CreateExtUserParams();
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::OnSelchangeUsedModules() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pUsedModules = (CComboBox*)GetDlgItem(IDC_COMBO_USED_MODULES);

    if (pUsedModules != NULL) {
      int iSel = pUsedModules->GetCurSel();
      if (iSel != CB_ERR) {
        CDP_Module* pModule	= (CDP_Module*)pUsedModules->GetItemData(iSel);
        if (((DWORD)pModule) != CB_ERR)
          m_pSelModule = pModule;
      }
    }

    FillModuleExtUserPrm(m_pSelModule);
}

//----------------------------- FUNCTION -------------------------------------*
void PropGSDModule::CreateExtUserParams()
{
	DWORD dwCurrentPos;
	CByteArray	UserParam;
	UserParam.RemoveAll();
	ExtParam* pParam = NULL;
	int offset = 0;

    UserParam.InsertAt(0, &m_DefaultUserPrmData);
    dwCurrentPos = UserParam.GetSize();

    if (m_bExtUserPrms) {
		// This sets all default parameter and of unused ExtUserPrms
		POSITION pos = ExtUserParams.GetHeadPosition();
		while (pos)	{
			pParam = (ExtParam*)ExtUserParams.GetNext(pos);
            ModifyParamArray(UserParam,pParam);
		}// end while of unused UserPrms
	}

    //actualize current position for appending the modules
    dwCurrentPos = UserParam.GetSize();
	// create UserParams and add them to the UserParam Array 
	m_pModule->GetUsedModules(&arModules);
	int index = arModules.GetSize();
	for (int i = 0; i < index; i++)	{
		CDP_Module* pModule = (CDP_Module*)arModules.GetAt(i);
		if (pModule->GetUserPrmLen() == 0)
			continue;

		CByteArray	ModuleParam;
		ModuleParam.RemoveAll();
		ModuleParam.InsertAt(0, 0x00, pModule->GetUserPrmLen());

		CObList* pParamList = pModule->GetExtParams();
		POSITION pos = pParamList->GetHeadPosition();
		while (pos)	{
			pParam = (ExtParam*)pParamList->GetNext(pos);
            ModifyParamArray(ModuleParam,pParam);
		}
		UserParam.InsertAt(dwCurrentPos, &ModuleParam);
		dwCurrentPos += pModule->GetUserPrmLen();
	}

	// set new userprms and show in EditCtrl
	m_ExtUserPrmData.RemoveAll();
    m_ExtUserPrmData.Copy(UserParam);

	if (::IsWindow(m_ctrlUserPrm.m_hWnd))
		m_ctrlUserPrm.SetHexContent(m_ExtUserPrmData.GetData(), m_ExtUserPrmData.GetSize());
}

//-------------------------------  Function  -----------------------------------

//-------------------------------  Function  -----------------------------------
CString PropGSDModule::GetState()
{
    CString retval;
    int iParam = 0;
    int nParams = 0;
    POSITION pos = ExtUserParams.GetHeadPosition();
    while (pos)	{
        ExtParam*pParam = (ExtParam*)ExtUserParams.GetNext(pos);
        if (pParam->IsUsed()) {
            CString val;
            val.Format(_T("%d;"), iParam);
            val += pParam->GetState();
            retval += val;
            nParams++;
        }
        iParam++;
    }
    CString hdr;
    hdr.Format(_T("%d;"), nParams);
    retval = hdr + retval;
    return retval;
}

//-------------------------------  Function  -----------------------------------
BOOL PropGSDModule::SetState(LPCTSTR &buffer)
{
    int nParams = GetNextValue(buffer);
    for (int i=0; i<nParams; i++) {
        int iParam = GetNextValue(buffer);
        POSITION pos = ExtUserParams.GetHeadPosition();
        ExtParam*pParam = NULL;

        for (int j=0; j<=iParam && pos; j++)	{
            ExtParam*pNext = (ExtParam*)ExtUserParams.GetNext(pos);
            if (j == iParam)
                pParam = pNext;
            
        }

        if (pParam == NULL || pParam->IsConstantParam() || pParam->IsUsed()) {
            return FALSE;
        }

        m_bExtUserPrms = TRUE;
        pParam->SetUsed(TRUE);
        if (!pParam->SetState(buffer)) {
            return FALSE;
        }
    }
    return TRUE;
}

BYTE MakeBitFieldInByte(DWORD Start, DWORD Length)
{
	BYTE Field = 1<<Length;
	Field --;
	Field <<= Start;

	return (BYTE)Field;
}


//------------------------------------------------------------------*
/**
 * Modifies a user param byte array with the ExtParam data passed with
 * argument pParam. ExtParam data objects will be build from 
 * Ext_User_Prm_Data_Const/Ref fields within the GSD. Extparam data 
 * describes the datatype of the Ext_User_Prm_Data, the offset within
 * the user param byte array and the value. The passed user param byte
 * array (see argument UserParam) grows as necessary.
 * Existing user param bytes within the array will be or-ed with the
 * new byte value.
 *
 * @param			CByteArray & UserParam - user param byte array to
 *                  modify. The byte array will grow as necessary.
 * @param			ExtParam *pParam - Reference to ExtParam data 
 *                  object describing the Ext_User_Prm_Data. Shall not
 *                  be NULL.
 * @exception		CMemoryException
 * @see			    GSD: Ext_User_Prm_Data_Const/Ref
*/
void ModifyParamArray(CByteArray & UserParam, ExtParam *pParam)
{
	int offset = 0;
    int sizeArray = 0;
    int sizeBytes = 0;


    ASSERT(pParam);

    switch(pParam->GetParamType()) {
    case Array:
        {
		    CByteArray* pBytes = pParam->GetByteArray();

		    sizeBytes = pBytes->GetSize();
            sizeArray = UserParam.GetSize();
		    offset = pParam->GetByteOffset();

            if(offset + sizeBytes > sizeArray) {
                //grow the array
                UserParam.SetSize(offset + sizeBytes);
                BYTE *pb = UserParam.GetData();
                //preset array
                size_t sizeSet = offset + sizeBytes - sizeArray;
                memset(&pb[sizeArray],0,sizeSet);
            }

		    for (int x = 0; x < sizeBytes; x++) {
			    BYTE userbyte = UserParam.GetAt(offset);
			    BYTE setbyte  = pBytes->GetAt(x);
			    userbyte |= setbyte;
			    UserParam.SetAt(offset++, userbyte);
		    }// next x
	    }
        break;
    case Byte:
	    {
		    offset = pParam->GetByteOffset();
            sizeArray = UserParam.GetSize();
            sizeBytes = sizeof(BYTE);

            if(offset + sizeBytes > sizeArray) {
                //grow the array
                UserParam.SetSize(offset + sizeBytes);
                BYTE *pb = UserParam.GetData();
                //preset array
                size_t sizeSet = offset + sizeBytes - sizeArray;
                memset(&pb[sizeArray],0,sizeSet);
            }

			BYTE userbyte = UserParam.GetAt(offset);	// Get current value in Bytearray
			if (pParam->IsBitArea())	{
				userbyte &= ~MakeBitFieldInByte(pParam->GetBitAreaStart(),
											   pParam->GetBitAreaLength());
				userbyte |= (BYTE)(pParam->GetUsedValue() << pParam->GetBitAreaStart());
			}
			else	{
				userbyte = (BYTE)pParam->GetUsedValue();
			}
			UserParam.SetAt(offset, userbyte);		// Set new value in ByteArray

	    }
        break;
    case Word:
	    {
            BYTE setbyte = 0;
		    WORD data = (WORD)pParam->GetUsedValue();

		    offset = pParam->GetByteOffset();
            sizeArray = UserParam.GetSize();
            sizeBytes = sizeof(data);
            
            if(offset + sizeBytes > sizeArray) {
                //grow the array
                UserParam.SetSize(offset + sizeBytes);
                BYTE *pb = UserParam.GetData();
                //preset array
                size_t sizeSet = offset + sizeBytes - sizeArray;
                memset(&pb[sizeArray],0,sizeSet);
            }
            
            for (int i = sizeBytes - 1; i >= 0 ; i--)	{
			    BYTE userbyte = UserParam.GetAt(offset);
			    setbyte = (BYTE)((data  >> (i*8)) & 0x000000FF);
			    userbyte = setbyte;
			    UserParam.SetAt(offset++, userbyte);
		    }
	    }
        break;
    case DWord:
	    {
		    BYTE setbyte = 0;
		    DWORD data = (DWORD)pParam->GetUsedValue();

		    offset = pParam->GetByteOffset();
            sizeArray = UserParam.GetSize();
            sizeBytes = sizeof(data);
            
            if(offset + sizeBytes > sizeArray) {
                //grow the array
                UserParam.SetSize(offset + sizeBytes);
                BYTE *pb = UserParam.GetData();
                //preset array
                size_t sizeSet = offset + sizeBytes - sizeArray;
                memset(&pb[sizeArray],0,sizeSet);
            }

		    for (int i = sizeBytes - 1; i >= 0 ; i--)	{
			    BYTE userbyte = UserParam.GetAt(offset);
			    setbyte = (BYTE)((data  >> (i*8)) & 0x000000FF);
			    userbyte = setbyte;
			    UserParam.SetAt(offset++, userbyte);
		    }
	    }
        break;
    default:
        break;
    }
}
