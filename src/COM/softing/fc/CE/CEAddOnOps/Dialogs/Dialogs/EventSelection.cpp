// EventSelection.cpp : implementation file
//

#include "stdafx.h"
#include "ObjPropGenTask.h"
#include "EventSelection.h"

// NFTASKHANDLING 30.05.05 SIS >>
#include <atlbase.h>

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;
// NFTASKHANDLING 30.05.05 SIS <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char* staEvents[] =
{
// System Events
    "EVT_IO_DRIVER_ERROR",      "General IO driver error",                  // 1
    "EVT_POWER_FAIL",           "Power fail",                               // 2
    "EVT_EXCEPTION",            "General IEC task exception",               // 3
    "EVT_BOOTED",               "System has booted",                        // 4
    "EVT_RESERVED_00",          "--- Reserved",                             // 5
    "EVT_RESERVED_01",          "--- Reserved",                             // 6
    "EVT_RESERVED_02",          "--- Reserved",                             // 7
    "EVT_RESERVED_03",          "--- Reserved",                             // 8
    "EVT_RESERVED_04",          "--- Reserved",                             // 9
    "EVT_RESERVED_05",          "--- Reserved",                             // 10
// Engineering command events
    "EVT_DOWNLOAD_BEGIN",       "Download command received",                // 11
    "EVT_DOWNLOAD_END",         "Download finished, resource started",      // 12
    "EVT_ONLCHG_BEGIN",         "Online Change command received",           // 13
    "EVT_ONLCHG_END",           "Online Change finished, resource started", // 14
    "EVT_BEFORE_COLDSTART",     "Coldstart command received",               // 15
    "EVT_BEFORE_WARMSTART",     "Warmstart command received",               // 16
    "EVT_AFTER_COLDSTART",      "Coldstart executed",                       // 17
    "EVT_AFTER_WARMSTART",      "Warmstart executed",                       // 18
    "EVT_START_RESOURCE",       "Start Resource command received",          // 19
    "EVT_STOP_RESOURCE",        "Stop Resource command received",           // 20
    "EVT_START_TASK",           "Start Task command executed",              // 21
    "EVT_STOP_TASK",            "Stop Task command executed",               // 22
    "EVT_INITIALIZE",           "Initialize/Clear command received",        // 23
    "EVT_BACNET_EVENT",         "BACnet event received",                    // 24
    "EVT_BACNET_DEVICE_STATE",  "BACnet device changed state",              // 25
    "EVT_BACNET_CONFIG_RESULT", "BACnet configuration finished",            // 26
    "EVT_RESERVED_10",          "--- Reserved",                             // 27
    "EVT_RESERVED_11",          "--- Reserved",                             // 28
    "EVT_RESERVED_12",          "--- Reserved",                             // 29
    "EVT_RESERVED_13",          "--- Reserved",                             // 30
    "EVT_RESERVED_14",          "--- Reserved",                             // 31
    "EVT_RESERVED_15",          "--- Reserved",                             // 32
// User events (target dependent interpretation)
    "EVT_USER_01",              "--- User Event",                           // 33
    "EVT_USER_02",              "--- User Event",                           // 34
    "EVT_USER_03",              "--- User Event",                           // 35
    "EVT_USER_04",              "--- User Event",                           // 36
    "EVT_USER_05",              "--- User Event",                           // 37
    "EVT_USER_06",              "--- User Event",                           // 38
    "EVT_USER_07",              "--- User Event",                           // 39
    "EVT_USER_08",              "--- User Event",                           // 40
    "EVT_USER_09",              "--- User Event",                           // 41
    "EVT_USER_10",              "--- User Event",                           // 42
    "EVT_USER_11",              "--- User Event",                           // 43
    "EVT_USER_12",              "--- User Event",                           // 44
    "EVT_USER_13",              "--- User Event",                           // 45
    "EVT_USER_14",              "--- User Event",                           // 46
    "EVT_USER_15",              "--- User Event",                           // 47
    "EVT_USER_16",              "--- User Event",                           // 48
    "EVT_USER_17",              "--- User Event",                           // 49
    "EVT_USER_18",              "--- User Event",                           // 50
    "EVT_USER_19",              "--- User Event",                           // 51
    "EVT_USER_20",              "--- User Event",                           // 52
    "EVT_USER_21",              "--- User Event",                           // 53
    "EVT_USER_22",              "--- User Event",                           // 54
    "EVT_USER_23",              "--- User Event",                           // 55
    "EVT_USER_24",              "--- User Event",                           // 56
    "EVT_USER_25",              "--- User Event",                           // 57
    "EVT_USER_26",              "--- User Event",                           // 58
    "EVT_USER_27",              "--- User Event",                           // 59
    "EVT_USER_28",              "--- User Event",                           // 60
    "EVT_USER_29",              "--- User Event",                           // 61
    "EVT_USER_30",              "--- User Event",                           // 62
    "EVT_USER_31",              "--- User Event",                           // 63
    "EVT_USER_32",              "--- User Event",                           // 64
};

/////////////////////////////////////////////////////////////////////////////
// CEventSelection dialog


CEventSelection::CEventSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CEventSelection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventSelection)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// NFTASKHANDLING 30.05.05 SIS >>
	m_bEventListFilled = FALSE;
	m_pICEProjectInfo = NULL;
	// NFTASKHANDLING 30.05.05 SIS <<
}


// NFTASKHANDLING 30.05.05 SIS >>
// calling object handles reference count
// no Addref needed
void CEventSelection::SetProjInfo(ICEProjInfo* pICEProjectInfo)
{
	m_pICEProjectInfo = pICEProjectInfo;
}

void CEventSelection::SetResource(const CString& crstrResource)
{
	m_strResource = crstrResource;
}
// NFTASKHANDLING 30.05.05 SIS <<

void CEventSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventSelection)
	DDX_Control(pDX, IDC_EVENT_LIST, m_tEventList);
	DDX_Control(pDX, IDC_TARGET_COMBO, m_tTargetCombo);		// NFTASKHANDLING 30.05.05 SIS
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventSelection, CDialog)
	//{{AFX_MSG_MAP(CEventSelection)
	ON_NOTIFY(NM_CLICK, IDC_EVENT_LIST, OnClickEventList)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_TARGET_COMBO, OnSelchangeTargetCombo)	// NFTASKHANDLING 30.05.05 SIS
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventSelection message handlers

BOOL CEventSelection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_tEventList.SetImageList(&m_tStateImages, LVSIL_STATE);

    CRect   tRect;
    CString strText;

    m_tEventList.GetClientRect(&tRect);
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = "Event";
	lvc.cx = tRect.Size().cx-16;
    lvc.iImage = 16;
    m_tEventList.InsertColumn(0, &lvc);

	// NFTASKHANDLING 30.05.05 SIS >>
	FillTargetCombo();
	FillEventList();
	// NFTASKHANDLING 30.05.05 SIS <<

    ParseEventString();

    UpdateStates();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// NFTASKHANDLING 30.05.05 SIS >>
void CEventSelection::OnSelchangeTargetCombo() 
{
	FillEventList();
}

void CEventSelection::FillEventList()
{
    int		iEventIdx;
	CString	strText;

    for(int iEvent = MIN_EVENT_NUM; iEvent <= MAX_EVENT_NUM; ++iEvent)
    {
        iEventIdx = iEvent - MIN_EVENT_NUM;
        //strText.Format("%-2d %-20s (%s)", iEvent, staEvents[2*iEventIdx], staEvents[2*iEventIdx+1]);
        strText.Format("%-2d %-20s", iEvent, staEvents[2*iEventIdx]);
		if(m_bEventListFilled)
		{
			m_tEventList.SetItemText(iEventIdx, 0, strText);
		}
		else
		{
			m_tEventList.InsertItem(iEventIdx, strText, 0);
			m_tEventList.SetItemData(iEventIdx,0);
		}
    }
	m_bEventListFilled = TRUE;

	if(m_tTargetCombo.GetCurSel() != 0)
	{
		TEventList	tEventList;
		if(ReadEventDescription(tEventList))
		{
			POSITION	pos = tEventList.GetHeadPosition();
			CEventDescription	tEventDescription;
			while(pos)
			{
				tEventDescription = tEventList.GetNext(pos);
				ASSERT(tEventDescription.m_iIdx >= MIN_EVENT_NUM && tEventDescription.m_iIdx <= MAX_EVENT_NUM);
				if(tEventDescription.m_iIdx >= MIN_EVENT_NUM && tEventDescription.m_iIdx <= MAX_EVENT_NUM)
				{
					strText.Format("%-2d %-20s", tEventDescription.m_iIdx, tEventDescription.m_strName);
					m_tEventList.SetItemText(tEventDescription.m_iIdx - 1, 0, strText);
				}
			}
		}
		else
		{
			m_tTargetCombo.SetCurSel(0);
		}
	}
}

void CEventSelection::SkipWhiteSpace(const CString& crstrLine, int& riChar)
{
	int	iLength = crstrLine.GetLength();
	if(riChar >= iLength)
	{
		return;
	}

	while(riChar < iLength)
	{
		if(!_istspace(crstrLine[riChar]))
		{
			break;
		}
		++riChar;
	}
}

// Line-Format
// Number "Name" "Description"
BOOL CEventSelection::ParseEventDescription(CEventDescription& rtEventDescription, const CString& crstrLine)
{
	int iLength = crstrLine.GetLength();
	int	iStart = 0;
	int	iStop = 0;

	// index
	SkipWhiteSpace(crstrLine, iStart);
	if(iStart >= iLength)
	{
		return FALSE;
	}
	if(crstrLine.Mid(iStart, 2).Compare(_T("//")) == 0)
	{
		return FALSE;
	}
	if(!_istdigit(crstrLine[iStart]))
	{
		return FALSE;
	}

	rtEventDescription.m_iIdx = _ttoi(crstrLine.Mid(iStart));
	ASSERT(rtEventDescription.m_iIdx >= 0 && rtEventDescription.m_iIdx <= MAX_EVENT_NUM);

	// name
	iStart = crstrLine.Find(_T('"'), iStart);
	iStop = crstrLine.Find(_T('"'), iStart+1);
	if((iStart == -1) || (iStop == -1))
	{
		return FALSE;
	}
	rtEventDescription.m_strName = crstrLine.Mid(iStart+1, iStop-iStart-1);

	// description
	iStart = crstrLine.Find(_T('"'), iStop+1);
	iStop = crstrLine.Find(_T('"'), iStart+1);
	if((iStart == -1) || (iStop == -1))
	{
		return FALSE;
	}
	rtEventDescription.m_strDescription = crstrLine.Mid(iStart+1, iStop-iStart-1);

	return TRUE;
}

BOOL CEventSelection::ReadEventDescription(TEventList& rtEventList)
{
	BOOL	bReturn = FALSE;
	CString	strTarget;
	CString strEventDescriptionFile;
	CString strInstallPath = GetInstallationPath();

	m_tTargetCombo.GetWindowText(strTarget);
	CString	strTargetExt = GetTargetExtensionFromType(strTarget);

	strEventDescriptionFile.Format(_T("%sEngineering\\bin\\%s\\EventList.txt"), (LPCTSTR)strInstallPath, (LPCTSTR)strTargetExt);

	CStdioFile	tFile;
	BOOL		bOpen = FALSE;
	try
	{
		if(tFile.Open(strEventDescriptionFile, CFile::modeRead))
		{
			bOpen = TRUE;
		}
	}
	catch(CFileException* pE)
	{
		delete pE;
	}

	if(bOpen)
	{
		CString				strLine;
		CEventDescription	tEventDescription;

		while(tFile.ReadString(strLine))
		{
			if(ParseEventDescription(tEventDescription, strLine))
			{
				rtEventList.AddTail(tEventDescription);
			}
		}
		tFile.Close();
		bReturn = TRUE;
	}
	return bReturn;
}


CString CEventSelection::GetTargetExtensionFromType(const CString& crstrTargetType)
{
	CString	strExtension;

	ASSERT(m_pICEProjectInfo);
	if(m_pICEProjectInfo)
	{
		CComBSTR	sTargetType(crstrTargetType);
		CComBSTR	sTargetExt;

		HRESULT hr = m_pICEProjectInfo->getExtForTargetType(sTargetType, &sTargetExt);
		if(hr == S_OK)
		{
			strExtension = sTargetExt;
		}
	}
	return strExtension;
}

BOOL CEventSelection::CheckEventDescription(const CString& crstrTargetType)
{
	BOOL	bReturn = FALSE;
	CString	strExtension = GetTargetExtensionFromType(crstrTargetType);
	ASSERT(!strExtension.IsEmpty());

	CString	strInstallDir = GetInstallationPath();
	CString strEventDescriptionFile;

	strEventDescriptionFile.Format(_T("%sEngineering\\bin\\%s\\EventList.txt"), (LPCTSTR)strInstallDir, (LPCTSTR)strExtension);

	CFileStatus	tStatus;
	if(CFile::GetStatus(strEventDescriptionFile, tStatus))
	{
		bReturn = TRUE;
	}

	return bReturn;
}

void CEventSelection::FillTargetCombo()
{
	int	iSelect = 0;
	m_tTargetCombo.AddString(_T("Standard"));

	if(m_pICEProjectInfo)
	{
		CString	strInstallDir = GetInstallationPath();

		SAFEARRAY*	pTargetTypes;
		HRESULT		hr = m_pICEProjectInfo->getAllTargetTypes(&pTargetTypes);
		if(hr == S_OK)
		{
			long lLowerBound;
			long lUpperBound;
			::SafeArrayGetLBound(pTargetTypes, 1, &lLowerBound);
			::SafeArrayGetUBound(pTargetTypes, 1, &lUpperBound);

			for(long l = lLowerBound; l <= lUpperBound; l++) 
			{
				CComBSTR elem;
				hr = SafeArrayGetElement(pTargetTypes, &l, &elem);
				if(hr == S_OK)
				{
					if(CheckEventDescription(elem.m_str))
					{
						m_tTargetCombo.AddString(CString(elem.m_str));
					}
				}
			}
			::SafeArrayDestroy(pTargetTypes);    
			pTargetTypes = NULL;
		}

		CString		strTargetSelect;
		SAFEARRAY*	pTargets;
		hr = m_pICEProjectInfo->getAllTargetsAsIdPaths(&pTargets);
		if(hr == S_OK)
		{
			long lLowerBound;
			long lUpperBound;
			::SafeArrayGetLBound(pTargets, 1, &lLowerBound);
			::SafeArrayGetUBound(pTargets, 1, &lUpperBound);

			for(long l = lLowerBound; l <= lUpperBound; l++) 
			{
				CComBSTR elem;
				hr = SafeArrayGetElement(pTargets, &l, &elem);
				if(hr == S_OK)
				{
					if(m_strResource.Compare(GetAttributeOfTarget(elem.m_str, _T("ASSIGNMENT"))) == 0)
					{
						CString strTargetType = GetAttributeOfTarget(elem.m_str, _T("TYPE"));
						int iSelectTmp = m_tTargetCombo.FindString(0, strTargetType);
						if(iSelectTmp != -1)
						{
							iSelect = iSelectTmp;
							break;
						}
					}
				}
			}
			::SafeArrayDestroy(pTargets);    
			pTargets = NULL;
		}
	}
	m_tTargetCombo.SetCurSel(iSelect);
}

CString CEventSelection::GetAttributeOfTarget(const CString& crstrTargetIdPath, const CString& crstrAttribute)
{
	CString	strAttribute;
	if(m_pICEProjectInfo)
	{
		CComBSTR	sTargetIdPath(crstrTargetIdPath);
		CComQIPtr<IXMLDOMElement, &IID_IXMLDOMElement> pXMLPtr;
		CComPtr<IUnknown> pUnkPtr;
		HRESULT hr = m_pICEProjectInfo->getXMLNodePtr(sTargetIdPath, &pUnkPtr);
		if(hr == S_OK)
		{
			pXMLPtr = pUnkPtr;

			CComVariant attrText;
			CComBSTR name(crstrAttribute);

			HRESULT hr = pXMLPtr->getAttribute(name, &attrText);
			if(hr == S_OK)
			{
				strAttribute = attrText.bstrVal;
			}
		}
	}
	return strAttribute;
}
// NFTASKHANDLING 30.05.05 SIS <<

void CEventSelection::UpdateStates()
{
    DWORD dwState;

    for(int iEvent = MIN_EVENT_NUM; iEvent <= MAX_EVENT_NUM; ++iEvent)
    {
        dwState = m_tEventList.GetItemData(iEvent-MIN_EVENT_NUM) + 1;
        m_tEventList.SetItemState(iEvent-MIN_EVENT_NUM, INDEXTOSTATEIMAGEMASK(dwState), TVIS_STATEIMAGEMASK);
    }
}

void CEventSelection::OnOK() 
{
    m_strEvent = BuildEventString();

	CDialog::OnOK();
}

void CEventSelection::SetEventString(const CString& crstrEvent)
{
    m_strEvent = crstrEvent;
}


CString CEventSelection::GetEventString()
{
    return m_strEvent;
}


CString CEventSelection::BuildEventString()
{
    CString strEvent;
    CString strTmp;

    for(int iEvent = MIN_EVENT_NUM; iEvent <= MAX_EVENT_NUM; ++iEvent)
    {
        if(m_tEventList.GetItemData(iEvent-MIN_EVENT_NUM) == 1)
        {
            strTmp.Format("%d,", iEvent);
            strEvent += strTmp;
        }
    }
    if(!strEvent.IsEmpty())
    {
        strEvent.SetAt(strEvent.GetLength()-1, 0);  // strip last comma
    }
    return strEvent;
}

void CEventSelection::ParseEventString()
{
    CString strTmp;
    int     iStrLen = m_strEvent.GetLength();
    int     iEventNum;
    TCHAR   cChar;
    TCHAR   cLastChar(0);   // last character except for '," or space
    
    for(int iChar = 0; iChar < iStrLen; ++iChar)
    {
        cChar = m_strEvent[iChar];
        switch(cChar)
        {
        case _T('\''):      // skip single quotation mark
        case _T('"'):       // skip double quotation mark
            break;
        case _T(' '):
            if(cLastChar == _T(','))
            {
                break;      // skip space after comma
            }
            cLastChar = cChar;
            break;
        case _T(','):
            iEventNum = _ttoi(strTmp);
            if(iEventNum >= MIN_EVENT_NUM && iEventNum <= MAX_EVENT_NUM)
            {
                m_tEventList.SetItemData(iEventNum-MIN_EVENT_NUM, 1);
            }
            strTmp.Empty();
            cLastChar = cChar;
            break;
        default:
            strTmp += cChar;
            cLastChar = cChar;
        }
    }
    if(!strTmp.IsEmpty())
    {
        iEventNum = _ttoi(strTmp);
        if(iEventNum >= MIN_EVENT_NUM && iEventNum <= MAX_EVENT_NUM)
        {
            m_tEventList.SetItemData(iEventNum-MIN_EVENT_NUM, 1);
        }
    }
}


void CEventSelection::OnClickEventList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW    pNmListview = (LPNMLISTVIEW)pNMHDR;
    if(pNmListview->iItem == -1)
    {
        return;
    }
    SHORT   shKeyStateControl = GetKeyState(VK_CONTROL);
    SHORT   shKeyStateShift = GetKeyState(VK_SHIFT);
    if(shKeyStateControl < 0 || shKeyStateShift < 0)
    {
        return;
    }

    UINT uiResult;
    m_tEventList.HitTest(pNmListview->ptAction, &uiResult);
    if(uiResult != LVHT_ONITEMSTATEICON)
    {
        return;
    }

    BOOL    bChangeSelection(FALSE);
    DWORD   dwItemData = m_tEventList.GetItemData(pNmListview->iItem);

    if(m_tEventList.GetItemState(pNmListview->iItem, LVIS_SELECTED) != LVIS_SELECTED)
    {
        bChangeSelection = TRUE;
    }

    dwItemData = (dwItemData) ? 0 : 1;

    m_tEventList.SetItemState(pNmListview->iItem, INDEXTOSTATEIMAGEMASK(dwItemData+1), TVIS_STATEIMAGEMASK);
    m_tEventList.SetItemData(pNmListview->iItem, dwItemData);

    POSITION    pos = m_tEventList.GetFirstSelectedItemPosition();
    int         iItem;
    while(pos)
    {
        iItem = m_tEventList.GetNextSelectedItem(pos);
        if(bChangeSelection)
        {
            m_tEventList.SetItemState(iItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
        }
        else
        {
            m_tEventList.SetItemState(iItem, INDEXTOSTATEIMAGEMASK(dwItemData+1), TVIS_STATEIMAGEMASK);
            m_tEventList.SetItemData(iItem, dwItemData);
        }
    }
    if(bChangeSelection)
    {
        m_tEventList.SetItemState(pNmListview->iItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    }

	*pResult = 0;
}

int CEventSelection::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    m_tStateImages.Create(IDB_SELECTSTATES, 16, 5, RGB(255,255,255));
	
	return 0;
}
