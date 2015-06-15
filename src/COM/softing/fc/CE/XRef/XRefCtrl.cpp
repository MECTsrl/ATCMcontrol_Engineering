
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "XRef.h"
#include "XRefCtrl.h"
#include "cemsg_i.c"
#include "CEProjInfo.h"
#include "CEProjInfo_i.c"
#include "CEXref_Error.h"
#include "utilif.h"

/////////////////////////////////////////////////////////////////////////////
// CXRefCtrl

CXRefCtrl::CXRefCtrl()
{
	m_bWindowOnly = TRUE;
	CalcExtent(m_sizeExtent);
	m_XrefData = NULL;

	// XREFPRINT 21.07.05 SIS >>
	m_bLastSearchRead	= TRUE;
	m_bLastSearchWrite	= TRUE;
	// XREFPRINT 21.07.05 SIS <<
}

CXRefCtrl::~CXRefCtrl()
{
	m_CBVariableScope.Detach();
	m_CBDeclScope.Detach();
	m_CBTypeScope.Detach();
	m_CBFileScope.Detach();
	m_LBNameList.Detach();
	m_LBReferenceList.Detach();
	m_STHitText.Detach();
	m_BUAccessRead.Detach();
	m_BUAccessWrite.Detach();
	m_STAccess.Detach();
	m_STFilter.Detach();	// XREFPRINT 21.07.05 SIS
	delete m_XrefData;
}

void CXRefCtrl::ClearDialog()
{
	m_CBVariableScope.ResetContent();
	m_CBDeclScope.ResetContent();
	m_CBTypeScope.ResetContent();
	m_CBFileScope.ResetContent();
	m_LBNameList.ResetContent();
	m_LBReferenceList.ResetContent();

}

LRESULT CXRefCtrl::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_hWnd != NULL)
    {
        HWND hWndDlgItem;

        hWndDlgItem = GetDlgItem(IDC_XREF_NAME_SCOPE);
        m_CBVariableScope.Attach(hWndDlgItem);

        hWndDlgItem = GetDlgItem(IDC_XREF_TYPE_SCOPE);
        m_CBTypeScope.Attach(hWndDlgItem);
        hWndDlgItem = GetDlgItem(IDC_XREF_DECL_SCOPE);
        m_CBDeclScope.Attach(hWndDlgItem);
        hWndDlgItem = GetDlgItem(IDC_XREF_FILE_SCOPE);
        m_CBFileScope.Attach(hWndDlgItem);
        hWndDlgItem = GetDlgItem(IDC_STATIC_INFO);
        m_STHitText.Attach(hWndDlgItem);
        hWndDlgItem = GetDlgItem(IDC_LIST_NAMES);
        m_LBNameList.Attach(hWndDlgItem);
		
		int iTabstops[8] = {60,80,100,120,140,160,180,200};
		m_LBNameList.SetTabStops(8,iTabstops);

        hWndDlgItem = GetDlgItem(IDC_LIST_REFERENCES);
        m_LBReferenceList.Attach(hWndDlgItem);
        hWndDlgItem = GetDlgItem(IDC_ACCESS_READ);
        m_BUAccessRead.Attach(hWndDlgItem);
		m_BUAccessRead.SetCheck(TRUE);
        hWndDlgItem = GetDlgItem(IDC_ACCESS_WRITE);
        m_BUAccessWrite.Attach(hWndDlgItem);
		m_BUAccessWrite.SetCheck(TRUE);
        hWndDlgItem = GetDlgItem(IDC_STATIC_ACCESS);
		m_STAccess.Attach(hWndDlgItem);
		// XREFPRINT 21.07.05 SIS >>
        hWndDlgItem = GetDlgItem(IDC_STATIC_FILTER);
		m_STFilter.Attach(hWndDlgItem);
		// XREFPRINT 21.07.05 SIS <<
		//FillDummy(); Only for test output
	}
	return 0;
}

BOOL CXRefCtrl::Link()
{
	BOOL	fOK = TRUE;
	CString sDirectory;
	GetLinkDirectory(sDirectory);
	delete m_XrefData;
	m_XrefData = new CXref;
	try
	{
		m_XrefData->PerformLink(sDirectory);
		sDirectory += "\\XREF.4LNK";
		CFile theFile;
		if ( !theFile.Open(sDirectory, CFile::modeWrite | CFile::modeCreate ))
		{
			return FALSE;
		}
		CArchive archive(&theFile, CArchive::store);
		archive << m_XrefData ;
		archive.Close();
		theFile.Close();
	}
	catch (CArchiveException *e)
	{
		fOK = FALSE;
		switch (e->m_cause)
		{
			case CArchiveException::badIndex :
			case CArchiveException::badClass  :
			case CArchiveException::badSchema :
				e->ReportError();
				break;
		}

		e->Delete();
	}
	catch (CFileException *e)
	{
		fOK = FALSE;
		e->Delete();
	}

	//m_XrefData->SaveToFile(sPath);
	UpdateXRefData();
	ShowMessage(E_CE_XREF_FINISH_LINK);
	return TRUE;
}

void CXRefCtrl::UpdateXRefData()
{
	int i;
	ClearDialog();
	if (m_XrefData == NULL)
		return ;
	CSortedStringMap *mStringList;
	m_CBVariableScope.AddString(szAny);
	m_CBVariableScope.SetCurSel(0);
	// Types
	mStringList = m_XrefData->GetScopes();
	m_CBDeclScope.AddString(szAll);
	m_CBDeclScope.SetCurSel(0);
	for (i= 0; i < mStringList->GetCount();i++)
	{
		m_CBDeclScope.AddString(mStringList->GetSortedItem(i));
	}

	// Decl Scopes
	mStringList = m_XrefData->GetTypes();
	m_CBTypeScope.AddString(szAll);
	m_CBTypeScope.SetCurSel(0);
	for (i= 0; i < mStringList->GetCount();i++)
	{
		m_CBTypeScope.AddString(mStringList->GetSortedItem(i));
	}
	// File Scopes
	mStringList = m_XrefData->GetFiles();
	m_CBFileScope.AddString(szAll);
	m_CBFileScope.SetCurSel(0);
	for (i= 0; i < mStringList->GetCount();i++)
	{
		m_CBFileScope.AddString(mStringList->GetSortedItem(i));
	}

}

LRESULT CXRefCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CRect rClient;
    rClient.top = rClient.left = 0;
    rClient.right  = LOWORD(lParam); // width of client area
    rClient.bottom = HIWORD(lParam); // height of client area

    LONG dbu = GetDialogBaseUnits();
    WORD dbuh = LOWORD(dbu);
    WORD dbuv = HIWORD(dbu);

	CRect	rControl;
	GetWindowRect(&rControl);
	CRect	rListboxLeft;
	CRect	rListboxRight;
	CRect	rStaticAccess;

	// XREFPRINT 21.07.05 SIS >>
	CRect	rStaticFilter;	
	int		iRightMax;
	// XREFPRINT 21.07.05 SIS <<

	// Move Listboxes
	m_LBNameList.GetWindowRect(&rListboxLeft);
	rListboxLeft.bottom = rControl.bottom;// TODO : Add Code for message handler. Call DefWindowProc if necessary.
	rListboxLeft.OffsetRect(-rControl.left,-rControl.top);
	m_LBNameList.MoveWindow(rListboxLeft.left,rListboxLeft.top,((rControl.Width() * 60) / 100)- dbuh,rListboxLeft.Height());

	m_LBReferenceList.GetWindowRect(&rListboxRight);
	rListboxRight.bottom = rControl.bottom;// TODO : Add Code for message handler. Call DefWindowProc if necessary.
	rListboxRight.OffsetRect(-rControl.left,-rControl.top);
	rListboxRight.left = (rControl.Width() * 60) / 100;
	m_LBReferenceList.MoveWindow(rListboxRight.left,rListboxRight.top,(rControl.Width()* 40) / 100,rListboxRight.Height());
	// XREFPRINT 21.07.05 SIS >>
	m_LBReferenceList.GetWindowRect(&rListboxRight);
	iRightMax = rListboxRight.right - rControl.left;
	// XREFPRINT 21.07.05 SIS <<
	
	// Move Checkboxes
	m_BUAccessRead.GetWindowRect(&rListboxRight);
	rListboxRight.OffsetRect(-rListboxRight.left + (rControl.Width() * 60) / 100,-rControl.top);
	// XREFPRINT 21.07.05 SIS >>
	// Checkboxes are within frame now -> positions change slightly
	m_BUAccessRead.MoveWindow(rListboxRight.left + dbuh,rListboxRight.top,rListboxRight.Width(),rListboxRight.Height());
	m_BUAccessWrite.MoveWindow(rListboxRight.left + 7 * dbuh,rListboxRight.top,rListboxRight.Width(),rListboxRight.Height());
	// XREFPRINT 21.07.05 SIS <<
	
	// HE 30.11.2001 : Error1057 >> : Resize Comboboxes according to new window Size
	CRect	rComboBox;
	m_CBVariableScope.GetClientRect(&rComboBox);
	m_CBVariableScope.ClientToScreen(&rComboBox);
	rComboBox.right = max (rControl.right - dbuh,rComboBox.left + 6 * dbuh);
	rComboBox.OffsetRect(- rControl.TopLeft());
	m_CBVariableScope.MoveWindow(&rComboBox,TRUE);

	m_CBDeclScope.GetClientRect(&rComboBox);
	m_CBDeclScope.ClientToScreen(&rComboBox);
	rComboBox.right = max (rControl.right - dbuh,rComboBox.left + 6 * dbuh);
	rComboBox.OffsetRect(- rControl.TopLeft());
	m_CBDeclScope.MoveWindow(&rComboBox,TRUE);

	m_CBTypeScope.GetClientRect(&rComboBox);
	m_CBTypeScope.ClientToScreen(&rComboBox);
	rComboBox.right = max (rControl.right - dbuh,rComboBox.left + 6 * dbuh);
	rComboBox.OffsetRect(- rControl.TopLeft());
	m_CBTypeScope.MoveWindow(&rComboBox,TRUE);

	m_CBFileScope.GetClientRect(&rComboBox);
	m_CBFileScope.ClientToScreen(&rComboBox);
	rComboBox.right = max (rControl.right - dbuh,rComboBox.left + 6 * dbuh);
	rComboBox.OffsetRect(- rControl.TopLeft());
	m_CBFileScope.MoveWindow(&rComboBox,TRUE);

	// XREFPRINT 21.07.05 SIS >>
	m_STAccess.GetWindowRect(&rStaticAccess);
	rStaticAccess.OffsetRect(-rStaticAccess.left + (rControl.Width() * 60) / 100,-rControl.top);
	rStaticAccess.right = iRightMax;
	m_STAccess.MoveWindow(rStaticAccess.left,rStaticAccess.top,rStaticAccess.Width(),rStaticAccess.Height());

	m_STFilter.GetWindowRect(&rStaticFilter);
	rStaticFilter.OffsetRect(- rControl.TopLeft());
	rStaticFilter.right = iRightMax;
	m_STFilter.MoveWindow(&rStaticFilter,TRUE);
	// XREFPRINT 21.07.05 SIS <<

	// HE 30.11.2001 : Error1057 << : Resize Comboboxes according to new window Size
	return 0;
}

LRESULT CXRefCtrl::OnClickedButton_search(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return SearchDefinitions();
}

LRESULT CXRefCtrl::SearchDefinitions()
{
	int iIndex;
	CString sName;
	CString sScope;
	CString sType;
	CString sFileName;
	CString sHits;
    CString sTmp;
	if (m_XrefData == NULL)
		return 0;

	iIndex = m_CBVariableScope.GetCurSel();
	if (iIndex != CB_ERR)
		m_CBVariableScope.GetLBText(iIndex,sName);
	else
	{
		// HE 30.11.2001 : Error1055 >> : Add last recently used to Combobox
		int iNumEntries;
        int iFound;
		m_CBVariableScope.GetWindowText(sName);
        iFound = m_CBVariableScope.FindStringExact(0, sName);
        if(iFound == -1)
        {
    		m_CBVariableScope.InsertString(1,sName);
		    iNumEntries = m_CBVariableScope.GetCount();
		    if (iNumEntries > 10)
			    m_CBVariableScope.DeleteString(iNumEntries -1);
		    // HE 30.11.2001 : Error1055 << : Add last recently used to Combobox
        }
        else
        {
            m_CBVariableScope.SetCurSel(iFound);
        }
	}

	iIndex = m_CBDeclScope.GetCurSel();
	if (iIndex != CB_ERR)
		m_CBDeclScope.GetLBText(iIndex,sScope);

	iIndex = m_CBTypeScope.GetCurSel();
	if (iIndex != CB_ERR)
		m_CBTypeScope.GetLBText(iIndex,sType);

	iIndex = m_CBFileScope.GetCurSel();
	if (iIndex != CB_ERR)
		m_CBFileScope.GetLBText(iIndex,sFileName);
	CStringArray DefinitionList;
	m_XrefData->FindDefinitions(DefinitionList,sName,sScope,sType,sFileName);
	m_LBNameList.ResetContent();
	int dx=0;
	CSize sz;
	CDC *pDC = m_LBNameList.GetDC();
	for (iIndex= 0; iIndex < DefinitionList.GetSize();iIndex++)
	{
		m_LBNameList.AddString(DefinitionList[iIndex]);
		sz = pDC->GetTextExtent(DefinitionList[iIndex]);
		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_LBNameList.ReleaseDC(pDC);
	if (m_LBNameList.GetHorizontalExtent() < dx)
	{
		m_LBNameList.SetHorizontalExtent(dx);
	}
	m_LBReferenceList.ResetContent();
	m_LBNameList.SetCurSel(0);
	FillReferences();
	sHits.Format(IDS_HIT_COUNT,DefinitionList.GetSize());
	m_STHitText.SetWindowText(sHits);

	// XREFPRINT 21.07.05 SIS >>
	// remember settings
	m_CBVariableScope.GetWindowText(m_strLastSearchName);
	m_CBDeclScope.GetWindowText(m_strLastSearchScope);
	m_CBTypeScope.GetWindowText(m_strLastSearchType);
	m_CBFileScope.GetWindowText(m_strLastSearchFile);
	m_bLastSearchRead = m_BUAccessRead.GetCheck() ? TRUE : FALSE;
	m_bLastSearchWrite = m_BUAccessWrite.GetCheck() ? TRUE : FALSE;
	// XREFPRINT 21.07.05 SIS <<

	return 0;

}

// XREFPRINT 20.07.05 SIS >>
LRESULT CXRefCtrl::OnClickedButton_print(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return PrintSelection();
}

LRESULT CXRefCtrl::PrintSelection()
{
	CWaitCursor	tWait;
	Fire_OnPrint();
	return 0L;
}
// XREFPRINT 20.07.05 SIS <<


LRESULT CXRefCtrl::OnSelchangeList_names(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// if the current selection changes, update the reference Listbox according to the new selected item
	// 
	return FillReferences();
	bHandled = TRUE;
}


LRESULT CXRefCtrl::FillReferences()
{
	int iIndex;
	CString sName;
	if (m_XrefData == NULL)
		return 0;
	iIndex = m_LBNameList.GetCurSel();
	if (iIndex != LB_ERR)
	{
		m_LBNameList.GetText(iIndex,sName);
		CStringArray DeclarationList;
		m_XrefData->FindReferences(DeclarationList,sName,m_BUAccessWrite.GetCheck(),m_BUAccessRead.GetCheck());
		m_LBReferenceList.ResetContent();
		int dx=0;
		CSize sz;
		CDC *pDC = m_LBReferenceList.GetDC();
		for (iIndex= 0; iIndex < DeclarationList.GetSize();iIndex++)
		{
			m_LBReferenceList.AddString(DeclarationList[iIndex]);
			sz = pDC->GetTextExtent(DeclarationList[iIndex]);
			if (sz.cx > dx)
				dx = sz.cx;
		}
		m_LBReferenceList.ReleaseDC(pDC);
		if (m_LBReferenceList.GetHorizontalExtent() < dx)
		{
			m_LBReferenceList.SetHorizontalExtent(dx);
		}
		m_LBReferenceList.SetCurSel(0);

	}
	return 0;

}

LRESULT CXRefCtrl::OnDblclkList_references(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return ShowReference(0);
}


BOOL CXRefCtrl::ShowReference(int iOffset)
{
	CString sEditPosition;
	int iColonPos = -1;
	if (m_XrefData == NULL)
		return 0;
	int iIndex = m_LBReferenceList.GetCurSel();
	if (iIndex != LB_ERR)
	{
		// Change Position if necessary
		if (iOffset != 0)
		{
			if (iIndex + iOffset < 0)
			{
				iIndex	=  m_LBReferenceList.GetCount() -1;
			}
			else if (iIndex + iOffset >= m_LBReferenceList.GetCount())
			{
				iIndex = 0;
			}
			else
			{
				iIndex +=  iOffset;
			}
			m_LBReferenceList.SetCurSel(iIndex);

		}
		m_LBReferenceList.GetText(iIndex,sEditPosition);
		if ((iColonPos = sEditPosition.Find(chDOUBLECOLON)) != -1)
		{
			sEditPosition = sEditPosition.Mid(iColonPos+1);
		}
		CComBSTR sInterface(sEditPosition);
		//Fire_OnNewMessage(sInterface,0,0,sInterface);
		Fire_ShowInEditor(sInterface);
	}
	// on doubleclick, call the Fire_ShowInEditor function with the editor position
	return 0;

}

LRESULT CXRefCtrl::OnClickedAccess_read(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// change the actually shown items in the reference Listbox 
	FillReferences();
	bHandled = TRUE;
	m_bLastSearchRead = m_BUAccessRead.GetCheck();	// XREFPRINT 21.07.05 SIS
	return 0;
}
LRESULT CXRefCtrl::OnClickedAccess_write(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	FillReferences();
	bHandled = TRUE;
	m_bLastSearchWrite = m_BUAccessWrite.GetCheck();	// XREFPRINT 21.07.05 SIS
	return 0;
}


STDMETHODIMP CXRefCtrl::LinkXRef()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	ClearDialog();
	Link();
	return S_OK;
}

STDMETHODIMP CXRefCtrl::ChangeRefPosition(BOOL bForward)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ShowReference	(bForward ? 1 : -1);

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CXRefCtrl::SetActiveQuery(BSTR sVariableName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString sNewQuery = sVariableName;
	// TODO: Add your implementation code here
	if (m_XrefData != NULL)
	{
		int iIndexPipe;
		iIndexPipe = sNewQuery.Find('|');
		if (iIndexPipe != -1)
		{
			CString sName = sNewQuery.Left(iIndexPipe);
			CString sLocation = sNewQuery.Mid(iIndexPipe+1);
			CString sFileScope;
			if (m_XrefData->GetFileScopeByLocation(sLocation,sFileScope))
			{
				// setting actual queries in the dialog
				int iIndexFileScope;
				m_CBDeclScope.SetCurSel(0);
				m_CBTypeScope.SetCurSel(0);
				iIndexFileScope = m_CBFileScope.FindStringExact(0,sFileScope);
				if (iIndexFileScope != CB_ERR)
				{
					m_CBFileScope.SetCurSel(iIndexFileScope);
				}
				else
				{
					m_CBFileScope.SetCurSel(0);
				}
				m_CBVariableScope.ResetContent();
				m_CBVariableScope.AddString(sName);
				m_CBVariableScope.SetCurSel(0);
				// now perform query...
				SearchDefinitions();
			}
		}
	}
	return S_OK;
}


STDMETHODIMP CXRefCtrl::InitXRef()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString sDirectory;
	BOOL	fOK = TRUE;

	ClearDialog();
	delete m_XrefData;
	m_XrefData = NULL;
	try
	{
		GetLinkDirectory(sDirectory);
		sDirectory += "\\XREF.4LNK";
		CFile theFile;
		if ( !theFile.Open(sDirectory, CFile::modeRead ))
		{
			fOK = FALSE;
			return FALSE;
		}
		CArchive archive(&theFile, CArchive::load);
		archive >>m_XrefData;
		archive.Close();
		theFile.Close();
	}
	catch (CArchiveException *e)
	{
		switch (e->m_cause)
		{
			case CArchiveException::badIndex :
			case CArchiveException::badClass  :
			case CArchiveException::badSchema :
				//e->ReportError();
				fOK = FALSE;
				ShowMessage	(E_CE_XREF_WRONG_BINARY_FORMAT);
				break;
		}

		e->Delete();
	}
	catch (CFileException *e)
	{
		fOK = FALSE;
		e->Delete();
	}

	UpdateXRefData();
	if (fOK)
		ShowMessage	(E_CE_XREF_OPEN_XREFDATA);
	return S_OK;
}


// OSAI feature print XRef 16.03.04 SIS >>
// XREFPRINT 21.07.05 SIS >>
// added parameter iMode
// -1: all
//  1: current selection
STDMETHODIMP CXRefCtrl::GetPrintString(/*[out,string]*/ BSTR* psContent, /*[in]*/ int iLineLength, /*[in]*/ int iMode)
// XREFPRINT 21.07.05 SIS <<
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString	strContent;
	if(!m_XrefData)
	{
		return E_FAIL;
	}

	// XREFPRINT 21.07.05 SIS >>
	if(iMode == 1)
	{
		m_XrefData->GetSelectionString(strContent, iLineLength, m_strLastSearchName, m_strLastSearchScope, m_strLastSearchType, m_strLastSearchFile, m_bLastSearchRead, m_bLastSearchWrite);
	}
	else
	{
		m_XrefData->GetContentString(strContent, iLineLength);
	}
	// XREFPRINT 21.07.05 SIS <<
	*psContent = strContent.AllocSysString();

	return S_OK;
}
// OSAI feature print XRef 16.03.04 SIS <<


BOOL CXRefCtrl::GetLinkDirectory(CString &sDirectory)
{
    HRESULT hr;
    IUnknown*       pProjManUnk;

    hr = Fire_GetProjectManager(&pProjManUnk);
    ASSERT(pProjManUnk);
    if (pProjManUnk)
	{
        CComQIPtr<ICEProjInfo, &IID_ICEProjInfo> pProjInfo;
        pProjInfo = pProjManUnk;
		pProjManUnk->Release();
        if (!(!pProjInfo))
		{
			CComBSTR sProjName;
            hr = pProjInfo->getProjectGenPath(&sProjName);
            if (hr == S_OK)
            {
				sDirectory = sProjName;
				sDirectory += "\\__4xr";
				return TRUE;
            }
		}

	}
	return FALSE;
 


}


HRESULT CXRefCtrl::ShowMessage(HRESULT hrMessageCode)
{
	CString str;
    CComBSTR    sMessage;

    UTIL_FormatMessage(AfxGetResourceHandle(),str.GetBuffer(_MAX_PATH),
            _MAX_PATH,hrMessageCode);
    str.ReleaseBuffer();
	sMessage = str;
	return Fire_OnNewMessage(NULL,E_FACILITY_CEXREF,hrMessageCode,sMessage);
}

void CXRefCtrl::FillDummy()
{
	m_CBVariableScope.AddString("Anna");
	m_CBVariableScope.AddString("Berta");
	m_CBVariableScope.AddString("Hugo");
	m_CBVariableScope.AddString("Dumpfbacke");
	m_CBVariableScope.AddString("Karl");
	m_CBVariableScope.AddString("Test");
	m_CBVariableScope.AddString("Schmarrn");
	m_CBVariableScope.SetCurSel(0);

	m_CBDeclScope.AddString("VAR");
	m_CBDeclScope.AddString("VAR_INPUT");
	m_CBDeclScope.AddString("VAR_OUTPUT");
	m_CBDeclScope.AddString("VAR_INOUT");
	m_CBDeclScope.SetCurSel(0);

	m_CBTypeScope.AddString("BOOL");
	m_CBTypeScope.AddString("BYTE");
	m_CBTypeScope.AddString("WORD");
	m_CBTypeScope.AddString("DWORD");
	m_CBTypeScope.AddString("INT");
	m_CBTypeScope.AddString("WSTRING");
	m_CBTypeScope.AddString("XXXX");
	m_CBTypeScope.SetCurSel(0);

	m_LBNameList.AddString("Anna");
	m_LBNameList.AddString("Berta");
	m_LBNameList.AddString("Hugo");
	m_LBNameList.AddString("Dumpfbacke");
	m_LBNameList.AddString("Karl");
	m_LBNameList.AddString("Test");
	m_LBNameList.AddString("Schmarrn");
	m_LBNameList.AddString("hfgkd");
	m_LBNameList.AddString("ghtregh");
	m_LBNameList.AddString("tztgerhtjku");
	m_LBNameList.AddString("gfhdgg");
	m_LBNameList.AddString("klkiluitzhtfg");
	m_LBNameList.AddString("wwfdefgrgt");
	m_LBNameList.AddString("hzjtzrg");
	m_LBNameList.AddString("utkikzeg");
	m_LBNameList.AddString("rgtrteerg");


}

LRESULT CXRefCtrl::OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    IUnknown* pUnk;
    CComQIPtr<ICEProjInfo> pProjMan;

    HRESULT hr = Fire_GetProjectManager(&pUnk);
    if (hr!=S_OK)
    {
        return 0;
    }
    ASSERT(pUnk!=NULL);
    if (pUnk==NULL)
    {
        return 0;
    }
    pProjMan = pUnk;
    pUnk->Release();
    ASSERT(pProjMan!=NULL);
    if (pProjMan==NULL)
    {
        return 0;
    }

    pProjMan->ShowContextHelp(IDD_XREFCTRL);
    return 0;
}
