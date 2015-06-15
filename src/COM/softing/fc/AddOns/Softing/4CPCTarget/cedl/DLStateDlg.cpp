
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cedl.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"
#include "dlstlist.h"
#include "DLStateDlg.h"
#include "DLUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//----  Local Defines:   -------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CDLStateDlg dialog
CDLStateDlg::CDLStateDlg (CStationList *pStationList, CComObject<CImpDLEvents> *pEvents, CWnd *pParent)
: CDialog (CDLStateDlg::IDD, pParent),
  m_pStationList (pStationList),
  m_pEvents (pEvents)
{
	ASSERT (pStationList != NULL);
    m_iSelection = -1;
	//{{AFX_DATA_INIT(CDLStateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLStateDlg)
	DDX_Control(pDX, IDC_CONTROL_DETAILS, m_Details);
	DDX_Control(pDX, IDC_CONTROLS, m_ControlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLStateDlg, CDialog)
	//{{AFX_MSG_MAP(CDLStateDlg)
	ON_BN_CLICKED(IDC_CONTROL_DETAILS, OnDetails)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CONTROLS, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_CONTROLS, OnDoubleClick)
    ON_MESSAGE(CEDL_MSG_VAR_CHANGED, OnVarChanged)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLStateDlg message handlers

BOOL CDLStateDlg::OnInitDialog () 
{
	CDialog::OnInitDialog ();
    //set the help id for context sensitive help
    SetWindowContextHelpId(IDD);
	PrepareList ();
	m_Details.EnableWindow (m_iSelection != -1);
	return (TRUE);  // return TRUE unless you set the focus to a control
	                // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLStateDlg::OnDetails () 
{
	ShowDetails ();
}


void CDLStateDlg::OnItemChanged (NMHDR *pNMHDR, LRESULT *pResult) 
{
	//m_Details.EnableWindow (GetSelectedStation () != NULL);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)) {
		m_iSelection = pNMListView->iItem;
    }
    else {
        m_iSelection = -1;
    }
	
	if((m_iSelection != -1) && IsConnected()) {
		m_Details.EnableWindow (TRUE);
    }
	else {
		m_Details.EnableWindow (FALSE);
	}

    *pResult = 0;
}


void CDLStateDlg::OnDoubleClick (NMHDR *pNMHDR, LRESULT *pResult) 
{
    ShowDetails ();
	*pResult = 0;
}

void CDLStateDlg::OnCancel () 
{

    POSITION pPosition = m_pStationList->GetHeadPosition ();
	while (pPosition != NULL)
	{
		CStation *pStation;
		pStation = m_pStationList->GetNext (pPosition);
        (pStation->GetStationInfo()).UnRegisterWnd();
    }

	CDialog::OnCancel ();
}

void CDLStateDlg::PrepareList(void)
{
	CString strHeadings;
	POSITION pPosition;
    int iPos=0;
	int iColumn=0;
	int iIndex=0;

	m_ControlList.SetExtendedStyle (m_ControlList.GetExtendedStyle () | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	strHeadings.LoadString (IDS_COLUMNS);

	do
	{
		CString strHeading;

		strHeadings = strHeadings.Mid (iPos);

		iPos = strHeadings.Find (_T ('|'));
		strHeading = iPos == -1 ? strHeadings : strHeadings.Left (iPos);
		AddColumn (strHeading, iColumn++);
	} while (iPos++ != -1);

	pPosition = m_pStationList->GetHeadPosition ();
	while (pPosition != NULL)
	{
		CStation *pStation;
		pStation = m_pStationList->GetNext (pPosition);
		m_ControlList.InsertItem (iIndex, pStation->GetAddress());

        (pStation->GetStationInfo()).RegisterWnd(GetSafeHwnd());
        OnVarChanged((UINT)-1,(LONG)pStation);
        iIndex++;
    }

}


LONG CDLStateDlg::OnVarChanged(UINT wParam, LONG lParam)
{
    CStation* pStation = reinterpret_cast<CStation *>(lParam);
    int iVarIndex;
    
    if(!pStation) {
        return 1L;
    }

    int iIndex = m_pStationList->FindStationIndex(pStation);

    if(iIndex == -1) {
        return 1L;
    }
    //found .. display data
    
    CStationInfo & rInfo = pStation->GetStationInfo();
    int iVars = rInfo.GetNumberOfVars();
    CString str;

    iVarIndex = static_cast<int>(wParam);

    if(iVarIndex == -1) {//update all values
        for(int i = 0;i < iVars;i++) {
            if(i != DloadGUID &&
               i != ProjectGUID) {
                str = rInfo.GetVar(i);
                m_ControlList.SetItemText(iIndex, i+1,str);
            }
            else {
                CheckDownloadState(iIndex,pStation);
            }
        }

        return 1L;
    }
    else { //single update
        if(iVarIndex != DloadGUID && 
           iVarIndex != ProjectGUID) {
            str = rInfo.GetVar(iVarIndex);
            m_ControlList.SetItemText(iIndex, iVarIndex+1,str);
            return 1L;
        }

        CheckDownloadState(iIndex,pStation);
    }
    
    return 1L;
}

void CDLStateDlg::CheckDownloadState(int iIndex, CStation *pStation)
{
    CStationInfo & rInfo = pStation->GetStationInfo();
    CString str;
    int iIDS = -1;
    int iIDSArray[] = {-1,IDS_NOT_CONNECTED,IDS_DIFF_CLEARED,IDS_DIFF_PRJ,IDS_DIFF_UNSYNC,IDS_DIFF_SYNC};

    try{
        int state = rInfo.State();
        iIDS = iIDSArray[state];

        if(iIDS != -1) {
            str.LoadString(iIDS);
        }

    }
    catch(CDaoException* e) {
		e->Delete(); 
	}
	catch(CMemoryException * e) {
		e->Delete();
	}

    m_ControlList.SetItemText(iIndex,(DloadGUID)+1,str);
}

void CDLStateDlg::AddColumn (const CString &strHeading, int iColumn)
{
	int iWidth;

	iWidth = m_ControlList.GetStringWidth (strHeading) + 15;
	m_ControlList.InsertColumn (iColumn, strHeading, LVCFMT_LEFT, iWidth);
}

void CDLStateDlg::ShowDetails(void)
{
	CStation *pStation;
	CDLUpdateDlg dialog(AfxGetMainWnd());

    if(m_iSelection == -1) {
        return;
    }

    pStation = m_pStationList->FindStationByIndex(m_iSelection);
    if(!pStation){
        return;
    }

	if(!pStation->IsConnected()) {
		return;
	}

    dialog.SetStation (pStation);
    dialog.SetEventPtr (m_pEvents);
	dialog.DoModal ();
}

bool CDLStateDlg::IsConnected(void)
{
	CStation *pStation;

    if(m_iSelection == -1) {
        return false;
    }

    pStation = m_pStationList->FindStationByIndex(m_iSelection);
    if(!pStation){
        return false;
    }

	return pStation->IsConnected();
}



BOOL CDLStateDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
    if (pHelpInfo->iContextType == HELPINFO_WINDOW){
        return ShowHelp(GetSafeHwnd(),pHelpInfo->dwContextId);
    }
    return TRUE;
}

