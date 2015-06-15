// MainPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "PMTool.h"
#include "MainPropertySheet.h"
#include "resource.h"
#include "PropPageBackup.h"
#include "PropPageRestore.h"
#include "PropPageDeploy.h"
#include "CommSettingsDlg.h"
#include "utilif.h"

#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"

#include "CommDefinitions.h"

#define MIN_CTRL_DISTANCE (10)
#define MIN_BORDER_DISTANCE (15)

// XML TAG and ATTRIBUTES (copied from target base)
#define ADDON_BASE_XML_TAG_ADDON					_T("ADDON")
#define ADDON_BASE_XML_TAG_PROPERTIES				_T("PROPERTIES")
#define ADDON_BASE_XML_ATTR_FILEEXT					_T("FILEEXT")
#define ADDON_BASE_XML_ATTR_FILEPREFIX				_T("FILEPREFIX")
#define ADDON_BASE_XML_ATTR_KADTYPE					_T("KADTYPE")
#define ADDON_BASE_XML_ATTR_DESCRIPTION				_T("DESCRIPTION")
#define ADDON_BASE_XML_ATTR_SUPPORTNCC				_T("SUPPORTNCC")
#define ADDON_BASE_XML_ATTR_SUPPORTFILETRANSFER		_T("SUPPORTFILETRANSFER")	// SUPFILTRANS 24.05.07 SIS
#define ADDON_BASE_XML_ATTR_WIZARDNAME				_T("WIZARDNAME")
#define ADDON_BASE_XML_ATTR_WIZARDDESC				_T("WIZARDDESC")
#define ADDON_BASE_XML_TAG_FIRMWARE					_T("FIRMWARE")
#define ADDON_BASE_XML_ATTR_VERSION					_T("VERSION")
#define ADDON_BASE_XML_ATTR_CUSTOMCONNECT			_T("CUSTOMCONNECT")
#define ADDON_BASE_XML_ATTR_DOWNLOAD				_T("DOWNLOAD")
#define ADDON_BASE_XML_TAG_COMMCHANNEL				_T("COMMCHANNEL")
#define ADDON_BASE_XML_ATTR_ID						_T("ID")
#define ADDON_BASE_XML_ATTR_STDCONNPAR				_T("STDCONNPAR")
#define ADDON_BASE_XML_ATTR_PROTOCOL				_T("PROTOCOL")
#define ADDON_BASE_XML_ATTR_PROTOCOLPAR				_T("PROTOCOLPAR")
#define ADDON_BASE_XML_ATTR_CUSTDL          		_T("CUSTOM_DOWNLOAD")
#define ADDON_BASE_XML_ATTR_CUSTDL_GUI      		_T("CUSTOM_DOWNLOAD_GUI")
#define ADDON_BASE_XML_ATTR_CUSTDL_DIR      		_T("CUSTOM_DOWNLOAD_DIR")
#define ADDON_BASE_XML_ATTR_ENDIAN          		_T("ENDIAN")
#define ADDON_BASE_XML_VALUE_BIG            		_T("BIG")
#define ADDON_BASE_XML_VALUE_LITTLE         		_T("LITTLE")

#define STD_FIRMWARE_VERSION                20000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainPropertySheet

BEGIN_MESSAGE_MAP(CMainPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMainPropertySheet)
	ON_BN_CLICKED(IDC_LAUNCH4C, OnLaunch4C)
	ON_BN_CLICKED(IDC_COMM_SETTINGS, OnCommSettings)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(CMainPropertySheet, CPropertySheet)

CMainPropertySheet::CMainPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    //m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    // load main icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // remove apply and help buttons
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    m_psh.dwFlags &= ~PSH_HASHELP;
    
    m_bStartup = TRUE;
    m_bInit    = FALSE;

    m_bProgressMain = FALSE;
    m_bProgressSub = FALSE;

    GetInstalledTargetTypes();
    InitializeFromRegistry();

    LoadTargetInfo();

	m_iUpperMain = 100;
	m_iUpperSub	 = 100;

	m_iLowerMain = 0;
	m_iLowerSub	 = 0;

	m_bSupportFileTransfer = TRUE;	// SUPFILTRANS 24.05.07 SIS
}

CMainPropertySheet::CMainPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    //m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    
    // load main icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // remove apply and help buttons
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    m_psh.dwFlags &= ~PSH_HASHELP;

    m_bStartup = TRUE;
    m_bInit    = FALSE;

    m_bProgressMain = FALSE;
    m_bProgressSub = FALSE;

	m_bSupportFileTransfer = TRUE;	// SUPFILTRANS 24.05.07 SIS

    GetInstalledTargetTypes();
    InitializeFromRegistry();
    
    LoadTargetInfo();
	
	m_iUpperMain = 100;
	m_iUpperSub	 = 100;

	m_iLowerMain = 0;
	m_iLowerSub	 = 0;
}

BOOL CMainPropertySheet::LoadTargetInfo()
{
    // determine initial target type
    BOOL    bReturn = FALSE;
    TFirmwareArr    arrFirmware;
    if(!m_strTargetType.IsEmpty())
    {
        if(!LoadKADFile(m_strTargetType, arrFirmware))
        {
            CString strMessage;
            strMessage.Format(_T("Could not load target info for target %s.\nRestore and File Transfer will not work.\nPlease check settings."), m_strTargetType);
            AfxMessageBox(strMessage);
            SetStatusText(_T("Could not load target info."));
            return bReturn;
        }
        else
        {
            bReturn = TRUE;
        }
    }
    else
    {
        int iNumTargetTypes = m_astrTargetTypes.GetSize();
        for(int iTargetType = 0; iTargetType < iNumTargetTypes; ++iTargetType)
        {
            if(LoadKADFile(m_astrTargetTypes[iTargetType], arrFirmware))
            {
                m_strTargetType = m_astrTargetTypes[iTargetType];
                bReturn = TRUE;
                break;
            }
        }
    }
    if(!GetConnectParameters(arrFirmware))
    {
        CString strMessage;
        strMessage.Format(_T("Could not load target info for target %s.\nRestore and File Transfer will not work.\nPlease check settings."), m_strTargetType);
        AfxMessageBox(strMessage);
        SetStatusText(_T("Could not load target info."));
    }
    return bReturn;
}


CString CMainPropertySheet::GetCommChannel()
{
	return m_strCommChannel;
}


CMainPropertySheet::~CMainPropertySheet()
{
    // only save active page here
    // use SaveToRegistry only when user opens settings
    AfxGetApp()->WriteProfileInt("Settings", "ActivePage", m_iActivePage);
}


void CMainPropertySheet::InitializeFromRegistry()
{
    m_iActivePage = AfxGetApp()->GetProfileInt("Settings", "ActivePage", 0);

    // m_strTargetType is already initialized by GetInstalledTargetTypes()
    // overwrite this value only if registry entry exists
    CString strTargetType = AfxGetApp()->GetProfileString("Settings", "TargetType");
    if(!strTargetType.IsEmpty())
    {
        m_strCommChannel = AfxGetApp()->GetProfileString("Settings", "CommChannel");
        m_strFirmwareVersion = AfxGetApp()->GetProfileString("Settings", "FirmwareVersion");
        m_strTargetType = strTargetType;
    }
    else
    {
        GetStandardTargetCommInfo(m_strTargetType, m_strCommChannel, m_strFirmwareVersion);
    }
}




void CMainPropertySheet::SaveToRegistry()
{
    AfxGetApp()->WriteProfileInt("Settings", "ActivePage", m_iActivePage);

    AfxGetApp()->WriteProfileString("Settings", "TargetType", m_strTargetType);
    AfxGetApp()->WriteProfileString("Settings", "CommChannel", m_strCommChannel);
    AfxGetApp()->WriteProfileString("Settings", "FirmwareVersion", m_strFirmwareVersion);
}


CString CMainPropertySheet::GetTargetType()
{
    return m_strTargetType;
}


BOOL CMainPropertySheet::GetConnectParameters(TFirmwareArr& rtFWArr)
{
    // search for comm channel
    int iNumFW = rtFWArr.GetSize();
    if(iNumFW == 0)
    {
        ASSERT("Firmware array is empty");
        return FALSE;
    }
    UINT    uiVersion;
    int     iFWFound = -1;

    if(m_strFirmwareVersion.IsEmpty())
    {
        uiVersion = rtFWArr[iNumFW-1].m_uiVersion;
        m_strFirmwareVersion.Format(_T("%u"), uiVersion);
        iFWFound = iNumFW-1;
    }
    else
    {
        uiVersion = (UINT)atoi(m_strFirmwareVersion);
        for(int iFW = 0; iFW < iNumFW; ++iFW)
        {
            if(uiVersion == rtFWArr[iFW].m_uiVersion)
            {
                iFWFound = iFW;
                break;
            }
        }
    }

    if(iFWFound == -1)
    {
        return FALSE;
    }

    m_strConnectGuid = rtFWArr[iFWFound].m_strCustomConnectGuid;

    TCCArray&   rtCCArr = rtFWArr[iFWFound].m_tCommChannelArr;

    int iNumCC = rtCCArr.GetSize();
    if(iNumCC == 0)
    {
        ASSERT("CommChannel List is empty");
        return FALSE;
    }

    int iCCFound = -1;
    if(m_strCommChannel.IsEmpty())
    {
        m_strCommChannel = rtCCArr[0].m_strId; 
        iCCFound = 0;
    }
    else
    {
        for(int iCC = 0; iCC < iNumCC; ++iCC)
        {
            if(m_strCommChannel.CompareNoCase(rtCCArr[iCC].m_strId) == 0)
            {
                iCCFound = iCC;
                break;
            }
        }
    }
    if(iCCFound == -1)
    {
        return FALSE;
    }
    m_strProtocol = rtCCArr[iCCFound].m_strProtocol;
    m_strProtocolPar = rtCCArr[iCCFound].m_strProtocolPar;

    return TRUE;
}


BOOL CMainPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
    // change caption of cancel button to "Exit"
    CWnd*   pWnd = GetDlgItem(IDCANCEL);
    if(pWnd)
    {
        pWnd->SetWindowText(_T("Exit"));
    }
    // hide OK button
    pWnd = GetDlgItem(IDOK);
    if(pWnd)
    {
        CRect   tRect;
        CFont*  pFont;
        pWnd->GetWindowRect(&tRect);
        pFont = pWnd->GetFont();
        ScreenToClient(&tRect);

        m_tCommSettings.Create(_T("Settings"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, tRect, this, IDC_COMM_SETTINGS);
        m_tCommSettings.SetFont(pFont);

        int     iOffset = tRect.right - tRect.left + MIN_CTRL_DISTANCE;
        CPoint  tOffset(-iOffset, 0);

        tRect.OffsetRect(tOffset);

        m_tLaunch4C.Create(_T("ATCMControl"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, tRect, this, IDC_LAUNCH4C);
        m_tLaunch4C.SetFont(pFont);

        pWnd->EnableWindow(FALSE);
        pWnd->ShowWindow(SW_HIDE);

        tRect.right = tRect.left - MIN_CTRL_DISTANCE;
        tRect.left = MIN_BORDER_DISTANCE;
        m_tStatusEdit.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_READONLY, tRect, this, IDC_STATUS);
        m_tStatusEdit.SetFont(pFont);
    }

    SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    SetActivePage(m_iActivePage);   // set active page

    m_bStartup = FALSE;
	
	return bResult;
}


BOOL CMainPropertySheet::IsStartup()
{
    return m_bStartup;
}

void CMainPropertySheet::SetStatusText(const CString& crstrText)
{
    m_strStatusText = crstrText;
    if(m_tStatusEdit.GetSafeHwnd() && !m_bProgressMain && !m_bProgressSub)
    {
        m_tStatusEdit.ShowWindow(SW_SHOW);
        m_tStatusEdit.SetWindowText(crstrText);
        m_tStatusEdit.Invalidate();
        m_tStatusEdit.UpdateWindow();
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMainPropertySheet message handlers

void CMainPropertySheet::OnLaunch4C()
{
    CPMPropPage*    pPage = dynamic_cast<CPMPropPage*>(GetActivePage());
    ASSERT(pPage);
    if(pPage)
    {
        pPage->Launch4C();
    }
}

void CMainPropertySheet::OnCommSettings()
{
    CCommSettingsDlg    tDlg(this);
    tDlg.Initialize(m_astrTargetTypes, m_strTargetType, m_strFirmwareVersion, m_strCommChannel);

    if(tDlg.DoModal() == IDOK)
    {
        tDlg.GetData(m_strTargetType, m_strFirmwareVersion, m_strCommChannel);
        TFirmwareArr    tFWArr;
        if(LoadKADFile(m_strTargetType, tFWArr))
        {
            GetConnectParameters(tFWArr);
        }
        SaveToRegistry();

        InformConnectInfoChanged();
    }
}


void CMainPropertySheet::OnDestroy() 
{
    m_iActivePage = GetActiveIndex();

    CPropertySheet::OnDestroy();
}

void CMainPropertySheet::InitProgress(int iLower /*=0*/, int iUpper /*=25*/, BOOL bSub /*=FALSE*/)
{
    // status edit window must exist
    ASSERT(m_tStatusEdit.GetSafeHwnd());

    // replace status edit by progress control
    if(m_tStatusEdit.GetSafeHwnd())
    {
        CRect   tRect;
        int     iProgressHeight;
        m_tStatusEdit.GetWindowRect(&tRect);
        ScreenToClient(&tRect);

        m_tStatusEdit.ShowWindow(SW_HIDE);
        iProgressHeight = tRect.Height() / 2 - 1;

        if(bSub)
        {
            if(!m_tProgressCtrlSub.GetSafeHwnd())
            {
                // sub rect is below
                CRect   tRectSub(tRect);
                tRectSub.top = tRectSub.bottom - iProgressHeight;
                m_tProgressCtrlSub.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | PBS_SMOOTH, tRectSub, this, IDC_PROGRESS_SUB);
            }
            else
            {
                m_tProgressCtrlSub.ShowWindow(SW_SHOW);
            }
            m_tProgressCtrlSub.SetRange32(iLower, iUpper);
			
			m_iUpperSub = iUpper;
			m_iLowerSub = iLower;
            
			m_bProgressSub = TRUE;
        }
        else
        {
            if(!m_tProgressCtrlMain)
            {
                // main rect is above
                CRect   tRectMain(tRect);
                tRectMain.bottom = tRectMain.top + iProgressHeight;
                m_tProgressCtrlMain.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | PBS_SMOOTH, tRectMain, this, IDC_PROGRESS_MAIN);
            }
            else
            {
                m_tProgressCtrlMain.ShowWindow(SW_SHOW);
            }
            m_tProgressCtrlMain.SetRange32(iLower, iUpper);
			
			m_iUpperMain = iUpper;
            m_iLowerMain = iLower;

			m_bProgressMain = TRUE;
        }
    }
}


void CMainPropertySheet::SetProgress(int iPosition, int iUpper, BOOL bSub /*=FALSE*/)
{
    if(bSub)
    {
        ASSERT(m_tProgressCtrlSub.GetSafeHwnd());
        if(m_tProgressCtrlSub.GetSafeHwnd())
        {
			if (iUpper != -1 && iUpper > m_iUpperSub)
			{
				m_iUpperSub = iUpper;
				m_tProgressCtrlSub.SetRange32(m_iLowerSub, m_iUpperSub);
			}

            m_tProgressCtrlSub.SetPos(iPosition);
        }
    }
    else
    {
        ASSERT(m_tProgressCtrlMain.GetSafeHwnd());
        if(m_tProgressCtrlMain.GetSafeHwnd())
        {
			if (iUpper != -1 && iUpper > m_iUpperMain)
			{
				m_iUpperMain = iUpper;
				m_tProgressCtrlMain.SetRange32(m_iLowerMain, m_iUpperMain);
			}
			
            m_tProgressCtrlMain.SetPos(iPosition);
        }
    }
}


void CMainPropertySheet::StopProgress(BOOL bSub /*=FALSE*/)
{
    if(bSub)
    {
        m_tProgressCtrlSub.ShowWindow(SW_HIDE);
        m_bProgressSub = FALSE;
    }
    else
    {
        m_tProgressCtrlMain.ShowWindow(SW_HIDE);
        m_bProgressMain = FALSE;
    }

    if(!m_bProgressSub && !m_bProgressMain)
    {
        SetStatusText(m_strStatusText);
    }
}



BOOL CMainPropertySheet::GetInstalledTargetTypes()
{
    // read 4cg.kad
    CString strInstallPath;
    size_t sizeInstDir = MAX_PATH;
    if(S_OK != UTIL_GetInstallPath(strInstallPath.GetBuffer(MAX_PATH), &sizeInstDir))
    {
        return FALSE;
    }
    strInstallPath.ReleaseBuffer();
    strInstallPath += _T("Engineering\\bin");

    CString strCGKad;
    strCGKad.Format(_T("%s\\4cg.kad"), strInstallPath);

    CXMLDocument doc;

    // load kad
    if (!doc.Load(strCGKad))
    {
        return FALSE;
    }
    CXMLNode rootNode;
    if (!doc.GetRootNode(rootNode))
    {
        return FALSE;
    }

    CString strTagName;
    if (!rootNode.GetTagName(strTagName))
    {
        return FALSE;
    }

    if (strTagName.Compare(_T("KADCGFRONT"))!=0)
    {
        return FALSE;
    }

    CXMLNodeList childNodeList;

    if (!rootNode.GetChildNodeList(childNodeList))
    {
        return FALSE;
    }

    childNodeList.Reset();
    CXMLNode childNode;

    BOOL    bFound = FALSE;
    while (childNodeList.Next(childNode))
    {
        if (!childNode.GetTagName(strTagName))
        {
            return FALSE;
        }

        if (strTagName.Compare(_T("TARGETS"))==0)
        {
            bFound = TRUE;
            break;
        }
    }
    if(bFound)
    {
        CString strExtension;
        CXMLNodeList targetChildList;
        if (!childNode.GetChildNodeList(targetChildList))
        {
            return FALSE;
        }

        m_astrTargetTypes.RemoveAll();
        while (targetChildList.Next(childNode))
        {
            childNode.GetAttribute(_T("EXT"), strExtension);
            if(!strExtension.IsEmpty() && strExtension.CompareNoCase(_T("4cpc")) != 0)   // do not add pc
            {
                m_astrTargetTypes.Add(strExtension);
            }
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//////////// Communication settings (partly copied from target base) //////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * load KAD file.
 *
 * @param           crstrTargetType: target type (file extension)
 * @return          successful?
 * @exception       -
 * @see             ReadProperties(), ReadFirmware()
*/
BOOL CMainPropertySheet::LoadKADFile(const CString crstrTargetType, TFirmwareArr& rtFirmwareArr)
{
    CString strKADFileName;
    CString strTargetType(crstrTargetType);
    if(strTargetType.IsEmpty())
    {
        strTargetType = _T("4CWin");
    }
    strKADFileName.Format(_T("%s\\%s_Addon.kad"), strTargetType, strTargetType);

    CXMLDocument doc;

    BOOL bProperties = FALSE;

    CString strPath;
    LPSTR pszPath;

    // Get the setup path
    try
    {
        pszPath = strPath.GetBufferSetLength(MAX_PATH);
    }

    catch (CMemoryException * e)
    {
        e->Delete();
        return FALSE;
    }

    size_t sizeChars = MAX_PATH;
    if (UTIL_GetInstallPath(pszPath, &sizeChars)!=S_OK)
    {
        return FALSE;
    }

    strPath.ReleaseBuffer();

    int pos = strPath.ReverseFind('\\');
    if (pos > 0)
    {
        strPath = strPath.Left(pos + 1);
        strPath += _T("Engineering\\bin\\");
        strPath += strKADFileName;
    }
    else
    {
        return FALSE;
    }

    // load kad
    if (!doc.Load(strPath))
    {
        return FALSE;
    }

    CXMLNode rootNode;
    if (!doc.GetRootNode(rootNode))
    {
        return FALSE;
    }

    CString strTagName;
    if (!rootNode.GetTagName(strTagName))
    {
        return FALSE;
    }

    if (strTagName.Compare(ADDON_BASE_XML_TAG_ADDON)!=0)
    {
        return FALSE;
    }

    CXMLNodeList childNodeList;

    if (!rootNode.GetChildNodeList(childNodeList))
    {
        return FALSE;
    }

    childNodeList.Reset();
    CXMLNode childNode;

    rtFirmwareArr.RemoveAll();

    while (childNodeList.Next(childNode))
    {
        if (!childNode.GetTagName(strTagName))
        {
            return FALSE;
        }

        if (strTagName.Compare(ADDON_BASE_XML_TAG_PROPERTIES)==0)
        {
            if (bProperties)
            {
                return FALSE;
            }

            if (!ReadProperties(childNode))
            {
                return FALSE;
            }
            bProperties = TRUE;
        }
        else if (strTagName.Compare(ADDON_BASE_XML_TAG_FIRMWARE)==0)
        {
            CFirmwareData tFirmware;
            if (!ReadFirmware(childNode, tFirmware))
            {
                return FALSE;
            }
            rtFirmwareArr.Add(tFirmware);
        }
    }

    if (bProperties && rtFirmwareArr.GetSize()>0)
    {
        m_bInit = TRUE;
    }
    else
    {
        return FALSE;
    }
    return m_bInit;
}
//------------------------------------------------------------------*
/**
 * read properties.
 *
 * @param           node: xml root node.
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMainPropertySheet::ReadProperties(CXMLNode& node)
{
    node.GetAttribute(ADDON_BASE_XML_ATTR_DESCRIPTION, m_strDescription);

	// SUPFILTRANS 24.05.07 SIS >>
	CString	strTmp;
	node.GetAttribute(ADDON_BASE_XML_ATTR_SUPPORTFILETRANSFER, strTmp);
	if(!strTmp.IsEmpty())
	{
		m_bSupportFileTransfer = _ttoi(strTmp);
	}
	else
	{
		m_bSupportFileTransfer = TRUE;
	}
	// SUPFILTRANS 24.05.07 SIS <<

    return TRUE;
}



//------------------------------------------------------------------*
/**
 * read firmware.
 *
 * @param           tFirmware: firmware data object
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CMainPropertySheet::ReadFirmware(CXMLNode& node, CFirmwareData& tFirmware)
{
    CString strTmp;
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_VERSION, strTmp))
    {
        return FALSE;
    }
    tFirmware.m_uiVersion = atoi(strTmp);
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_DESCRIPTION, tFirmware.m_strVersionDescription))
    {
        return FALSE;
    }
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_CUSTOMCONNECT, tFirmware.m_strCustomConnectGuid))
    {
        return FALSE;
    }
    if (!node.GetAttribute(ADDON_BASE_XML_ATTR_DOWNLOAD, tFirmware.m_strDownloadGuid))
    {
        return FALSE;
    }

    // run over all children and get comm channel data
    CXMLNodeList childNodeList;

    if (!node.GetChildNodeList(childNodeList))
    {
        return FALSE;
    }

    CXMLNode    childNode;
    CString     strTagName;

    while (childNodeList.Next(childNode))
    {
        if (!childNode.GetTagName(strTagName))
        {
            return FALSE;
        }

        if(strTagName.Compare(ADDON_BASE_XML_TAG_COMMCHANNEL)==0)
        {
            CCommChannelData    tCommChannelData;

            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_ID, tCommChannelData.m_strId))
            {
                return FALSE;
            }
            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_PROTOCOL, tCommChannelData.m_strProtocol))
            {
                tCommChannelData.m_strProtocol = GetStandardProtocol(tCommChannelData.m_strId);
            }

            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_PROTOCOLPAR, tCommChannelData.m_strProtocolPar))
            {
                tCommChannelData.m_strProtocolPar = GetStandardProtocolParameter(tCommChannelData.m_strId);
            }

            if(!childNode.GetAttribute(ADDON_BASE_XML_ATTR_STDCONNPAR, tCommChannelData.m_strStdConnPar))
            {
                tCommChannelData.m_strStdConnPar = GetStandardConnectParameter(tCommChannelData.m_strId);
            }

            tFirmware.m_tCommChannelArr.Add(tCommChannelData);
        }
    }
    return TRUE;
}



//------------------------------------------------------------------*
/**
 * build control string.
 *
 *  control string is built up like this:
 *  "<OPCServer>,<CustomConnectGuid>,<CommChannel>(<ConnectParameters>),VERSION(<FirmwareVersion>)"
 *      where:
 *      <OPCServer>: OPC-Server address
 *      <CustomConnectGuid>: GUID of custom connect interface
 *      <CommChannel>: communication channel "TCP" or "RS232" ...
 *      <ConnectParameters>: connect parameter string depending on communication channel
 *      <FirmwareVersion>: internal firmware version number
 *  
 * @param           crstrOPCServer: OPC-Server address
 * @param           crstrCommChannel: communication channel string
 * @param           crstrConnectPar: connect parameter string
 * @param           uiFirmwareIndex: index of firmware data in firmware array
 * @return          control string.
 * @exception       -
 * @see             ParseControlString()
*/
CString CMainPropertySheet::GetControlString( const CString& crstrAddress)
{
    HRESULT hr;
    CString strControl;
    CString strOPCInstName;
    
    strOPCInstName = BuildOPCInstName(m_strCommChannel, crstrAddress);

    hr = UTIL_BuildControlString(_T(""), _T(""), strOPCInstName, m_strCommChannel, crstrAddress, m_strProtocol, m_strProtocolPar, m_strFirmwareVersion, strControl.GetBuffer(UTIL_MAX_CONTROL_STRING_LEN));
    ASSERT(hr == S_OK);

    strControl.ReleaseBuffer();
    strControl = strControl.Mid(2);     // strip empty opc-server and connect guid

    return strControl;
}


//------------------------------------------------------------------*
/**
 * get communication channel description.
 *
 * @param           crstrCommChannel: communication channel
 * @return          communication channel description (as used in combo boxes)
 * @exception       -
 * @see             -
*/
CString CMainPropertySheet::GetCommChannelDescription(const CString& crstrCommChannel)
{
    CString strCommChannelDesc(crstrCommChannel);
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_TCP) == 0)
    {
        strCommChannelDesc = _T("TCP/IP");
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        strCommChannelDesc = _T("RS232");
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_NETBEUI) == 0)
    {
        strCommChannelDesc = _T("NETBEUI");
    }
    return strCommChannelDesc;
}

//------------------------------------------------------------------*
/**
 * get communication channel from description.
 *
 *  overwrite this function if extension regarding comm channels is required
 *
 * @param           crstrCommChannelDesc: communication channel description
 * @return          communication channel
 * @exception       -
 * @see             -
*/
CString CMainPropertySheet::GetCommChannelFromDescription(const CString& crstrCommChannelDesc)
{
    if(crstrCommChannelDesc.CompareNoCase(_T("TCP/IP")) == 0)
    {
        return COMM_CHANNEL_TCP;
    }
    if(crstrCommChannelDesc.CompareNoCase(_T("RS232")) == 0)
    {
        return COMM_CHANNEL_RS232;
    }
    if(crstrCommChannelDesc.CompareNoCase(_T("NETBEUI")) == 0)
    {
        return COMM_CHANNEL_NETBEUI;
    }
    return _T("");
}


//------------------------------------------------------------------*
/**
 * get standard connect parameter for comm channel.
 *
 * @param           crstrCommChannel: communication channel
 * @return          standard connect parameter
 * @exception       -
 * @see             -
*/
CString CMainPropertySheet::GetStandardConnectParameter(const CString& crstrCommChannel)
{
    CString strStdConnPar;
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        strStdConnPar = STD_RS232_PAR;
    }
    else
    {
        if(m_strTargetType.CompareNoCase(_T("4Win")) == 0)
        {
            strStdConnPar = _T("localhost");
        }
        else
        {
            strStdConnPar = STD_TCP_PAR;
        }
    }
    return strStdConnPar;
}


//------------------------------------------------------------------*
/**
 * get standard protocol.
 *
 * @param           crstrCommChannel: communication channel
 * @return          standard protocol
 * @exception       -
 * @see             -
*/
CString CMainPropertySheet::GetStandardProtocol(const CString& crstrCommChannel)
{
    return STD_PROTOCOL;
}


//------------------------------------------------------------------*
/**
 * get standard protocol parameter.
 *
 * @param           crstrCommChannel: communication channel
 * @return          standard protocol parameter
 * @exception       -
 * @see             -
*/
CString CMainPropertySheet::GetStandardProtocolParameter(const CString& crstrCommChannel)
{
    return STD_PROTOCOL_PARAMETER;
}


//------------------------------------------------------------------*
/**
 * build OPC server instance name.
 *
 *  in case of TCP/IP this is target IP address
 *  in case of RS232 this is COM-Port
 *
 * @param           crstrCommChannel: communication channel
 * @param           crstrConnectPar: connection parameter string
 * @return          OPC server instance name
 * @exception       -
 * @see             -
*/
CString CMainPropertySheet::BuildOPCInstName(const CString& crstrCommChannel, const CString& crstrConnectPar)
{
    CString strOPCInstName(crstrConnectPar);
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        int iFound = crstrConnectPar.Find(_T(':'));
        if(iFound >= 0)
        {
            strOPCInstName = crstrConnectPar.Left(iFound);
        }
    }
    else
    {
        strOPCInstName = crstrConnectPar;
    }
    return strOPCInstName;
}


BOOL CMainPropertySheet::GetStandardTargetCommInfo(CString& rstrTargetType, CString& rstrCommChannel, CString& rstrFirmwareVersion)
{
    HKEY    hKeyPMTool;
    LONG    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Softing\\PMTool" ,0 , KEY_READ, &hKeyPMTool);
    BOOL    bReturn = FALSE;
    if(lResult == ERROR_SUCCESS)
    {
        CString strTmp;
        DWORD  dwSize = _MAX_PATH;
        DWORD  dwType;
        lResult = RegQueryValueEx(hKeyPMTool, "StandardTargetType", NULL, &dwType, (unsigned char*)strTmp.GetBuffer(_MAX_PATH), &dwSize);
        strTmp.ReleaseBuffer();
        if(lResult == ERROR_SUCCESS)
        {
            bReturn = TRUE;

            rstrTargetType = strTmp;
            rstrCommChannel.Empty();        // avoid inconsistencies
            rstrFirmwareVersion.Empty();

            lResult = RegQueryValueEx(hKeyPMTool, "StandardCommChannel", NULL, &dwType, (unsigned char*)strTmp.GetBuffer(_MAX_PATH), &dwSize);
            strTmp.ReleaseBuffer();
            if(lResult == ERROR_SUCCESS)
            {
                rstrCommChannel = strTmp;
                lResult = RegQueryValueEx(hKeyPMTool, "StandardFirmwareVersion", NULL, &dwType, (unsigned char*)strTmp.GetBuffer(_MAX_PATH), &dwSize);
                strTmp.ReleaseBuffer();
                if(lResult == ERROR_SUCCESS)
                {
                    rstrFirmwareVersion = strTmp;
                }
            }
        }
    }
    RegCloseKey(hKeyPMTool);

    return bReturn;
}


void CMainPropertySheet::InformConnectInfoChanged()
{
    int iPageCount = GetPageCount();

    CPMPropPage*    pPage;
    for(int iPage = 0; iPage < iPageCount; ++iPage)
    {
        pPage = dynamic_cast<CPMPropPage*>(GetPage(iPage));
        if(pPage)
        {
            pPage->OnCommSettingsChanged();
        }
    }
}

CString CMainPropertySheet::GetConnectInfo()
{
    CString strConnectInfo;
    strConnectInfo.Format(_T("Target-Type: %s, Firmware-Version: %s, Comm-Channel: %s"), m_strTargetType, m_strFirmwareVersion, m_strCommChannel);
    return strConnectInfo;
}

// SUPFILTRANS 24.05.07 SIS >>
BOOL CMainPropertySheet::GetSupportFileTransfer()
{
	return m_bSupportFileTransfer;
}
// SUPFILTRANS 24.05.07 SIS <<





///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// End Communication settings ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


