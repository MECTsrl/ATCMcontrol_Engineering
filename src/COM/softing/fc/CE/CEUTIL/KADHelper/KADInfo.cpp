
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "KADInfo.h"
#include "stdafx.h"

#include "KADTypeInfo.h"
#include "kso.h"

#include "utilif.h"
#include "cesysdef.h"

#include "KADKeyCode.h"
#include "ComHelper.h"

//----  Static Initializations:   ----------------------------------*
#define VIRT_KEY_FILENAME		_T("virtkeycode.txt")
#define MOD_KEY_ALT				_T("ALT")
#define MOD_KEY_CONTROL			_T("CTRL")
#define MOD_KEY_SHIFT			_T("SHIFT")
#define MAX_VIRTKEY_STRLEN		(32)

// kso strings
const char* con_csa_kso_strings[] = 
{
    "open",
    "openuserhelp",
    "openurl",
    "goto",
    "gotodefinition",
    "delete",
    "openconsole",
    "openinstance",

    "connect",
    "disconnect",
    "connectex",
    "disconnectex",

    "downloadfull",
    "downloadchanges",
    "downloadselection",
    "downloadfullall",
    "downloadchangesall",
    "downloadfullselect",
    "downloadchangesselect",

    "buildall",
    "buildselect",
    "buildtarget",
    "compilefile",
    "buildlibrary",

    "startcontrol",
    "stopcontrol",
    "startalltasks",
    "stopalltasks",
    "coldstartreset",
    "warmstartreset",
    "initializeclear",
    "starttask",
    "stoptask",

    "settingsresetdocking",
    "settingslibrary",
    "settingspassword",
    "settingsengineering",
    "settingsprint",

    "downloadflash",
    "updateflash",
    "clearflash",
};

const char* con_csa_mouse_key_strings[] =
{
    "open",
    "delete",
    "insert",
    "rename",
    "cut",
    "copy",
    "paste"
};

const char* con_csa_view_strings[] =
{
    "file",
    "class",
    "target",
    "instance"
};


CKADInfo::CKADInfo()
{
    //m_pKadManager = NULL;
    m_iBitmapNo         = 0;
    m_iOverlayCount     = 0;
    m_iMainMenuCount    = 0;
    m_crBackGround      = RGB(255,0,255);
	// SIS 22.07.04 >>
	m_iMaxDocs			= MAXIMUM_DOCUMENT_COUNT;
	// SIS 22.07.04 <<
	// SIS 08.11.04 >>
	m_iMaxDocsRec		= MAXIMUM_RECOVER_COUNT;
	// SIS 08.11.04 <<

    // initialize m_kso_id_map
    m_kso_id_map.SetAt(con_csa_kso_strings[0], KSO_OPEN);
    m_kso_id_map.SetAt(con_csa_kso_strings[1], KSO_OPEN_USER_HELP);
    m_kso_id_map.SetAt(con_csa_kso_strings[2], KSO_OPEN_URL);
    m_kso_id_map.SetAt(con_csa_kso_strings[3], KSO_GOTO);
    m_kso_id_map.SetAt(con_csa_kso_strings[4], KSO_GOTO_DEFINITION);
    m_kso_id_map.SetAt(con_csa_kso_strings[5], KSO_DELETE);
    m_kso_id_map.SetAt(con_csa_kso_strings[6], KSO_OPEN_CONSOLE);
    m_kso_id_map.SetAt(con_csa_kso_strings[7], KSO_OPEN_INSTANCE);

    m_kso_id_map.SetAt(con_csa_kso_strings[8],  KSO_CONNECT);
    m_kso_id_map.SetAt(con_csa_kso_strings[9],  KSO_DISCONNECT);
    m_kso_id_map.SetAt(con_csa_kso_strings[10], KSO_CONNECT_EX);
    m_kso_id_map.SetAt(con_csa_kso_strings[11], KSO_DISCONNECT_EX);

    m_kso_id_map.SetAt(con_csa_kso_strings[12], KSO_DOWNLOAD_FULL);
    m_kso_id_map.SetAt(con_csa_kso_strings[13], KSO_DOWNLOAD_CHANGES);
    m_kso_id_map.SetAt(con_csa_kso_strings[14], KSO_DOWNLOAD_SELECTION);
    m_kso_id_map.SetAt(con_csa_kso_strings[15], KSO_DOWNLOAD_FULL_ALL);
    m_kso_id_map.SetAt(con_csa_kso_strings[16], KSO_DOWNLOAD_CHANGES_ALL);
    m_kso_id_map.SetAt(con_csa_kso_strings[17], KSO_DOWNLOAD_FULL_SELECT);
    m_kso_id_map.SetAt(con_csa_kso_strings[18], KSO_DOWNLOAD_CHANGES_SELECT);

    m_kso_id_map.SetAt(con_csa_kso_strings[19], KSO_BUILD_ALL);
    m_kso_id_map.SetAt(con_csa_kso_strings[20], KSO_BUILD_SELECT);
    m_kso_id_map.SetAt(con_csa_kso_strings[21], KSO_BUILD_TARGET);
    m_kso_id_map.SetAt(con_csa_kso_strings[22], KSO_COMPILE_FILE);
    m_kso_id_map.SetAt(con_csa_kso_strings[23], KSO_BUILD_LIBRARY);
    
    m_kso_id_map.SetAt(con_csa_kso_strings[24], KSO_START_CONTROL);
    m_kso_id_map.SetAt(con_csa_kso_strings[25], KSO_STOP_CONTROL);
    m_kso_id_map.SetAt(con_csa_kso_strings[26], KSO_START_ALL_TASKS);
    m_kso_id_map.SetAt(con_csa_kso_strings[27], KSO_STOP_ALL_TASKS);
    m_kso_id_map.SetAt(con_csa_kso_strings[28], KSO_COLDSTART_RESET);
    m_kso_id_map.SetAt(con_csa_kso_strings[29], KSO_WARMSTART_RESET);
    m_kso_id_map.SetAt(con_csa_kso_strings[30], KSO_INITIALIZE_CLEAR);
    m_kso_id_map.SetAt(con_csa_kso_strings[31], KSO_START_TASK);
    m_kso_id_map.SetAt(con_csa_kso_strings[32], KSO_STOP_TASK);

    m_kso_id_map.SetAt(con_csa_kso_strings[33], KSO_SETTINGS_RESET_DOCKING);
    m_kso_id_map.SetAt(con_csa_kso_strings[34], KSO_SETTINGS_LIBRARY);
    m_kso_id_map.SetAt(con_csa_kso_strings[35], KSO_SETTINGS_PASSWORD);
    m_kso_id_map.SetAt(con_csa_kso_strings[36], KSO_SETTINGS_ENGINEERING);
    m_kso_id_map.SetAt(con_csa_kso_strings[37], KSO_SETTINGS_PRINT);

    m_kso_id_map.SetAt(con_csa_kso_strings[38], KSO_DOWNLOAD_FLASH);
    m_kso_id_map.SetAt(con_csa_kso_strings[39], KSO_UPDATE_FLASH);
    m_kso_id_map.SetAt(con_csa_kso_strings[40], KSO_CLEAR_FLASH);

    m_km_id_map.SetAt(con_csa_mouse_key_strings[0], KEY_MOUSE_OPEN);
    m_km_id_map.SetAt(con_csa_mouse_key_strings[1], KEY_MOUSE_DELETE);
    m_km_id_map.SetAt(con_csa_mouse_key_strings[2], KEY_MOUSE_INSERT);
    m_km_id_map.SetAt(con_csa_mouse_key_strings[3], KEY_MOUSE_RENAME);
    m_km_id_map.SetAt(con_csa_mouse_key_strings[4], KEY_MOUSE_CUT);
    m_km_id_map.SetAt(con_csa_mouse_key_strings[5], KEY_MOUSE_COPY);
    m_km_id_map.SetAt(con_csa_mouse_key_strings[6], KEY_MOUSE_PASTE);

    LoadVirtKeyMap();
}


CKADInfo::~CKADInfo()
{
    // delete type info
    POSITION        tPosition;
    CString         cs_type_name;
    CKADTypeInfo*   pTypeInfo;

    tPosition = m_tTypeInfoMap.GetStartPosition();
    while(tPosition)
    {
        m_tTypeInfoMap.GetNextAssoc(tPosition, cs_type_name, pTypeInfo);
        delete pTypeInfo;
    }
}

// SIS 22.07.04 >>
//------------------------------------------------------------------*
/**
 * get maximum number of open documents
 *
 * @param			-
 * @return			maximum number of open documents
 * @exception		-
 * @see				-
*/
int CKADInfo::GetMaximumNumberOfOpenDocuments()
{
	return m_iMaxDocs;
}
// SIS 22.07.04 <<

// SIS 08.11.04 >>
//------------------------------------------------------------------*
/**
 * get maximum number of documents to recover on system start
 *
 * @param			-
 * @return			maximum number of documents to recover
 * @exception		-
 * @see				-
*/
int CKADInfo::GetMaximumNumberOfRecoverDocuments()
{
	return m_iMaxDocsRec;
}
// SIS 08.11.04 <<

//------------------------------------------------------------------*
/**
 * load virtual key map.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CKADInfo::LoadVirtKeyMap()
{
    CStdioFile  tFile;
    CString     strPath;

    // read virtual key codes
    CString strVirtKey;
    DWORD   dwKeyCode;
    CString strLine;
    int     iTmp;
    int     iRet;

    for(int iCode = 0; iCode < NUM_KEY_CODES; ++iCode)
    {
        iTmp = iCode*2;
        strVirtKey = con_caKeyCode[iTmp];
        iRet = sscanf(con_caKeyCode[iTmp+1], "%x", &dwKeyCode);
        ASSERT(iRet == 1);
        if(iRet == 1)
        {
            m_tVirtKeyMap.SetAt(strVirtKey, dwKeyCode);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * load virtual key map from file.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CKADInfo::LoadVirtKeyMapFromFile()
{
    CStdioFile  tFile;
    CString     strPath;
    CString     strVirtKeyFile;
    UINT        nStrLen = _MAX_PATH;
    HRESULT     hr;

    hr = UTIL_GetInstallPath(strPath.GetBuffer(_MAX_PATH), &nStrLen);
    strPath.ReleaseBuffer();
    strPath = strPath + _T("Engineering\\bin\\");

    strVirtKeyFile = strPath + VIRT_KEY_FILENAME;

    if(!tFile.Open(strVirtKeyFile, CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // read virtual key codes
    CString strVirtKey;
    DWORD   dwKeyCode;
    CString strLine;
    int     iRet;

    tFile.ReadString(strLine);  // header
    while(tFile.ReadString(strLine))
    {
        iRet = sscanf(strLine, "%s %x", strVirtKey.GetBuffer(MAX_VIRTKEY_STRLEN), &dwKeyCode);
        strVirtKey.ReleaseBuffer();
        ASSERT(iRet == 2);
        if(iRet == 2)
        {
            m_tVirtKeyMap.SetAt(strVirtKey, dwKeyCode);
        }
    }
    return TRUE;
}


// get splash screen file name
CString CKADInfo::GetSplashScreenFile()
{
    return m_strSplashScreenFile;
}

// get desktop icon
CString CKADInfo::GetDesktopIcon()
{
    return m_strDesktopIcon;
}

// get application title
CString CKADInfo::GetApplicationTitle()
{
    return m_strAppTitle;
}

//------------------------------------------------------------------*
/**
 * load data from KAD
 *
 *  caution: this function should only be called once!
 *  no reset is provided.
 *
 * @return          - TRUE, if method was successful, else FALSE
 * @exception       -
*/
BOOL CKADInfo::Load(CComPtr<IUnknown>& pKadMan)
{
    // only temporary: create own KAD-manager object
    HRESULT hr; 

    UINT    nStrLen = _MAX_PATH;

    hr = UTIL_GetInstallPath(m_strBitmapDir.GetBuffer(_MAX_PATH), &nStrLen);
    m_strBitmapDir.ReleaseBuffer();
    m_strBitmapDir = m_strBitmapDir + CE_IMAGE_PATH + "\\";

    CComQIPtr<IGeneral> pIGeneralTmp = pKadMan;
    CComPtr<IGeneral>   pIGeneral = pIGeneralTmp;
    if(pIGeneral)
    {
        LoadGeneralInfo(pIGeneral);
    }
    else
    {
        return FALSE;
    }

    CComQIPtr<IMainMenu> pMainMenuTmp = pKadMan;
    CComPtr<IMainMenu> pMainMenu = pMainMenuTmp;
    if(!pMainMenu)
    {
        return FALSE;
    }

    LoadMainMenu(pMainMenu);

    CComQIPtr<IToolbars>   pIToolbarsTmp = pKadMan;
    CComPtr<IToolbars>   pIToolbars = pIToolbarsTmp;
    if(!pIToolbars)
    {
        return FALSE;
    }

    LoadToolbars(pIToolbars);

    CComQIPtr<ITypes>   pKadManager = pKadMan;
    if(!pKadManager)
    {
        return FALSE;
    }

    CComPtr<IEnumTypes> kadEnumTypes;
    hr = pKadManager->GetTypeEnumerator(&kadEnumTypes);
    if (hr != S_OK || !kadEnumTypes)
        return FALSE;

    CComQIPtr<IDragDrop>    kadDragDrop = pKadMan;
    CComPtr<IDragDropMap>   kadDragDropMap;

    hr = kadDragDrop->GetDragDropMap(&kadDragDropMap);
    if(hr == S_OK)
    {
        LoadDragDropMap(kadDragDropMap);
    }

    // create image list
    m_tImageList.Create(20, 15, ILC_COLOR | ILC_MASK, 200, 10);
    m_tImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

    kadEnumTypes->Reset();

    ULONG           n;
    CComPtr<IType>  pIKadType;

    hr = kadEnumTypes->Next(1, &pIKadType, &n);
    while (hr == S_OK && pIKadType)
    {
        LoadTypeInfo(pIKadType);
        pIKadType.Release();
        hr = kadEnumTypes->Next(1, &pIKadType, &n);
    }

    // now set overlay images
    SetOverlayImages(m_tImageList);

    return(TRUE);
}

BOOL CKADInfo::LoadGeneralInfo(CComPtr<IGeneral>& pIGeneral)
{
    BOOL        bReturn = TRUE;
    HRESULT     hr;
    CComBSTR    sSplashScreenFile;
    CComBSTR    sDesktopIcon;
    CComBSTR    sAppTitle;
    CComBSTR    sMaxDocs;		// SIS 22.07.04
    CComBSTR    sMaxDocsRec;	// SIS 08.11.04

    hr = pIGeneral->GetSplashScreenFilename(&sSplashScreenFile);
    if(hr == S_OK)
    {
        m_strSplashScreenFile = sSplashScreenFile;
    }
    else
    {
        bReturn = FALSE;
    }

    hr = pIGeneral->GetDesktopIcon(&sDesktopIcon);
    if(hr == S_OK)
    {
        m_strDesktopIcon = sDesktopIcon;
    }
    else
    {
        bReturn = FALSE;
    }

    hr = pIGeneral->GetApplicationTitle(&sAppTitle);
    if(hr == S_OK)
    {
        m_strAppTitle = sAppTitle;
    }
    else
    {
        bReturn = FALSE;
    }

	// SIS 22.07.04 >>
    hr = pIGeneral->GetMaxDocs(&sMaxDocs);
    if(hr == S_OK)
    {
		CString	strMaxDocs = sMaxDocs;
		m_iMaxDocs = _ttoi(strMaxDocs);
    }
	// SIS 22.07.04 <<

	// SIS 08.11.04 >>
    hr = pIGeneral->GetMaxDocsRec(&sMaxDocsRec);
    if(hr == S_OK)
    {
		CString	strMaxDocsRec = sMaxDocsRec;
		m_iMaxDocsRec = _ttoi(strMaxDocsRec);
    }
	// SIS 08.11.04 <<

    return bReturn;
}


//------------------------------------------------------------------*
/**
 * load type information from KAD
 *
 * @param           - IType* pIKadType - pointer to type interface           
 * @return          - TRUE, if method was successful, else FALSE
*/
BOOL CKADInfo::LoadTypeInfo(CComPtr<IType>& pIKadType)
{
    HRESULT         hr;
    CComPtr<IMenu>  pIMenu;
    CComPtr<IEnumStates>    pIEnumStates;
    CComBSTR        bstrTmp;
    CKADTypeInfo*   pTypeInfo;
    CString         strType;
    BOOL            bReturn = TRUE;
    BOOL            bVisible = TRUE;
    BOOL            bTmp;

    hr = pIKadType->GetId(&bstrTmp);
    if(hr != S_OK)
    {
        return FALSE;
    }

    strType = bstrTmp;
    // create new type-info object
    pTypeInfo = new CKADTypeInfo;
    pTypeInfo->m_tData.m_strID = strType;
    bstrTmp.Empty();
    hr = pIKadType->GetIcon(&bstrTmp);
    if(hr == S_OK)
    {
        pTypeInfo->m_tData.m_strBitmap = bstrTmp;
        pTypeInfo->m_tData.m_iImageIdx = AddBitmapToImageList(pTypeInfo->m_tData.m_strBitmap); // add bitmap
    }
    else
    {
        bReturn = FALSE;
    }
    hr = pIKadType->GetVisibilityFlag(&bTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        bVisible = bTmp;
        pTypeInfo->m_tData.m_bVisible = bVisible;
    }
    bstrTmp.Empty();

    CString strView;
    CString strFilterText;

    hr = pIKadType->GetView(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        strView = bstrTmp;
    }
    bstrTmp.Empty();
    hr = pIKadType->GetFilterText(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        strFilterText = bstrTmp;
    }
    if(bVisible && strView != "" && strFilterText != "")
    {
        AddTypeToFilterItem(strView, strFilterText, strType);
    }

    hr = pIKadType->GetProjectListViewFlag(&bTmp);
    if(hr == S_OK)
    {
        pTypeInfo->m_tData.m_bListView = bTmp;
    }

    bstrTmp.Empty();
    hr = pIKadType->GetText(&bstrTmp);
    if(hr == S_OK)
    {
        pTypeInfo->m_tData.m_strText = bstrTmp;
    }

    hr = pIKadType->GetMenu(&pIMenu);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    if(pIMenu)
    {
        if(!LoadMenuInfo(pIMenu, &(pTypeInfo->m_tMenu)))
            bReturn = FALSE;
    }

    hr = pIKadType->GetStateEnumerator(&pIEnumStates);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    if(pIEnumStates)
    {
        if(!LoadStates(pIEnumStates, pTypeInfo))
        {
            bReturn = FALSE;
        }
    }

    m_tTypeInfoMap.SetAt(strType, pTypeInfo);
    return(bReturn);
}

//------------------------------------------------------------------*
/**
 * add type to existing filter
 *
 * @param           [in] rstrView: view, where filter item should be added
 * @param           [in] rstrFilterText: for identification of filter
 * @param           [in] rstrType: type to be added to filter
 * @see             LoadTypeInfo()
*/
void CKADInfo::AddTypeToFilterItem(const CString& rstrView, const CString& rstrFilterText, const CString& rstrType)
{
    int iViewIndex = -1;
    int iFilterIndex;
    CString strView = rstrView;
    strView.MakeLower();

    if(strView == con_csa_view_strings[0])
    {
        iViewIndex = 0;
    }
    if(strView == con_csa_view_strings[1])
    {
        iViewIndex = 1;
    }
    if(strView == con_csa_view_strings[2])
    {
        iViewIndex = 2;
    }
    if(strView == con_csa_view_strings[3])
    {
        iViewIndex = 3;
    }

    if(iViewIndex >= 0)
    {
        iFilterIndex = m_tFilterInfo[iViewIndex].GetFilterItemIndex(rstrFilterText);
        if(iFilterIndex == -1)  // filter with this text does not exist -> add new
        {
            iFilterIndex = m_tFilterInfo[iViewIndex].AddNewFilterItem(rstrFilterText);
        }
        m_tFilterInfo[iViewIndex].AddTypeToFilterItem(rstrType, iFilterIndex);
    }
}

//------------------------------------------------------------------*
/**
 * load type menu information from KAD
 *
 * @param           - IType* pIKadType - pointer to type interface           
 * @param           - CKADMenu* pMenu - pointer to menu data object           
 * @return          - TRUE, if method was successful, else FALSE
*/
BOOL CKADInfo::LoadMenuInfo(CComPtr<IMenu>& pIMenu, CKADMenu* pMenu)
{
    HRESULT         hr;
    CComBSTR        bstrTmp;
    CComPtr<IEnumMenuItems> pIMenuItemEnum;
    CComPtr<IMenuItem>      pIMenuItem;
    ULONG           n;
    BOOL            bReturn = TRUE;

    hr = pIMenu->GetId(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pMenu->m_strID = bstrTmp;
    }
    bstrTmp.Empty();
    hr = pIMenu->GetTitle(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pMenu->m_strTitle = bstrTmp;
    }
    pMenu->m_pHandler = new GUID;
    hr = pIMenu->GetHandler(pMenu->m_pHandler);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    hr = pIMenu->GetMenuItemEnumerator(&pIMenuItemEnum);
    if(hr != S_OK)
    {
        return FALSE;
    }

    pIMenuItemEnum->Reset();

    if(pIMenuItemEnum)
    {
        hr = pIMenuItemEnum->Next(1, &pIMenuItem, &n);
        while (hr == S_OK && pIMenuItem)
        {
            if(!LoadMenuItemInfo(pIMenuItem, pMenu))
            {
                bReturn = FALSE;
            }
            pIMenuItem.Release();
            hr = pIMenuItemEnum->Next(1, &pIMenuItem, &n);
        }
    }
    pMenu->SortMenu();
    return(bReturn);
}

//------------------------------------------------------------------*
/**
 * load type menu item information from KAD
 *      releases pIMenuItem at the end
 *
 * @param           [in] pIMenuItem: pointer to menu item interface           
 * @param           [in] pMenu: pointer to menu data object           
 * @param           [in] bMain: main menu item to be loaded -> register it
 * @return          all loaded?
*/
BOOL CKADInfo::LoadMenuItemInfo(CComPtr<IMenuItem>& pIMenuItem, CKADMenu* pMenu, BOOL bMain /*=FALSE*/)
{
    HRESULT             hr;
    CComBSTR            bstrTmp;
    CComBSTR            sOrderKey;

    GUID                tGUID;
//    BOOL                bDisabled;
    BOOL                bSeparator = FALSE;
    BOOL                bReturn = TRUE;
    CKADMenuItem*       pItemNew;
    int                 iIdTmp;
    CString             strTmp;

    hr = pIMenuItem->GetOrderKey(&sOrderKey);
//    ASSERT(hr == S_OK);     // each menu item has to have an order key
    if(hr != S_OK)
    {
        return FALSE;
    }

    // create new item
    pItemNew = new CKADMenuItem;
    pItemNew->m_strOrderKey = sOrderKey;

    hr = pIMenuItem->GetText(&bstrTmp); 
    if(hr == S_OK)
    {
        pItemNew->m_strText = bstrTmp;
    }
    else
    {
        bReturn = FALSE;
    }
    hr = pIMenuItem->GetHandler(&tGUID);
    if(hr == S_OK)
    {
        pItemNew->m_pHandler = new GUID;
        *(pItemNew->m_pHandler) = tGUID;
    }
    else
    {
        bReturn = FALSE;
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetExeHandler(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pItemNew->m_strExe = bstrTmp;
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetDispatchId(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pItemNew->m_strCommand = bstrTmp;
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetStandardOperation(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        strTmp = bstrTmp;
        strTmp.MakeLower();
        if(m_kso_id_map.Lookup(strTmp, iIdTmp))
        {
            pItemNew->m_iKSO = iIdTmp;
        }
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetStatusBarText(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pItemNew->m_strStatusText = bstrTmp;
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetEnableExpression(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pItemNew->m_strEnableExpr = bstrTmp;
        pItemNew->m_strEnableExpr.MakeLower();
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetShowExpression(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pItemNew->m_strShowExpr = bstrTmp;
        pItemNew->m_strShowExpr.MakeLower();
    }
    bstrTmp.Empty();
    hr = pIMenuItem->GetKeyMouseEvent(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        strTmp = bstrTmp;
        strTmp.MakeLower();
        if(m_km_id_map.Lookup(strTmp, iIdTmp))
        {
            pItemNew->m_iKeyMouse = iIdTmp;
        }
    }
    bstrTmp.Empty();

    hr = pIMenuItem->GetAccelerator(&bstrTmp);
    if(hr == S_OK)
    {
        CString strAccel = bstrTmp;
        SetAcceleratorData(pItemNew, strAccel);
    }

    pMenu->AddMenuItem(pItemNew);

    if(bMain)
    {
        CComBSTR sId;
        // read unique identifier
        hr = pIMenuItem->GetId(&sId);
        if(hr == S_OK)
        {
            pItemNew->m_strId = sId;
        }
        else
        {
            // if this assertion fires, the main menu item has no identifier
            ASSERT(0);
        }
    }

    return(bReturn);
}


//------------------------------------------------------------------*
/**
 * set accelerator data.
 *
 * @param           [in] pItem: pointer to kad menu item data object
 * @param           [in] crstrAccel: accelerator string
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CKADInfo::SetAcceleratorData(CKADMenuItem* pItem, const CString& crstrAccel)
{
    // accel strings look like this:
    // "VK_DEL", "ALT+1", "ALT+VK_F1"
    // in general: [<ModKey>] [<ModKey>] [<ModKey>] <Key>
    // key may be a virtual key (starting with VK) or a real key like "1"
    CString strAccel = crstrAccel;
    CString strKey;
    WORD    fModKey;
    strAccel.MakeUpper();

    for(int i=0; i<3; ++i)
    {
        if(!StripKey(strAccel, strKey))
        {
            break;
        }

        fModKey = GetModKeyFlag(strKey);
        if(fModKey == 0)
        {
            break;
        }
        pItem->m_wAccelVirt |= fModKey;
    }

    // get key
    // first look in virtual key code map
    int iKey;
    pItem->m_bAccel = TRUE;
    if(m_tVirtKeyMap.Lookup(strKey, iKey))
    {
        pItem->m_wAccelKey = iKey;
        pItem->m_wAccelVirt |= FVIRTKEY;
    }
    else
    {
        if(!strKey.IsEmpty())
        {
            pItem->m_wAccelKey = WORD(strKey[0]);
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}


BOOL CKADInfo::StripKey(CString& rstrAccel, CString& rstrKey)
{
    int iFound = rstrAccel.Find(_T('+'));
    if(iFound >= 0)
    {
        rstrKey = rstrAccel.Left(iFound);
        rstrAccel = rstrAccel.Mid(iFound+1);
        return TRUE;
    }
    rstrKey = rstrAccel;
    return FALSE;
}

WORD CKADInfo::GetModKeyFlag(const CString& crstrModKey)
{
    if(crstrModKey.Compare(MOD_KEY_ALT) == 0)
    {
        return FALT;
    }
    if(crstrModKey.Compare(MOD_KEY_CONTROL) == 0)
    {
        return FCONTROL;
    }
    if(crstrModKey.Compare(MOD_KEY_SHIFT) == 0)
    {
        return FSHIFT;
    }
    return 0;
}

//------------------------------------------------------------------*
/**
 * register main menu item.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CKADInfo::RegisterMainMenuItem(const CString& crstrId, CKADMenuItem* pMenuItem)
{
    m_tMainIdMap.SetAt(crstrId, pMenuItem->m_iItemNo);
    m_tMenuItemMap.SetAt(pMenuItem->m_iItemNo, pMenuItem);
}

//------------------------------------------------------------------*
/**
 * load states
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CKADInfo::LoadStates(CComPtr<IEnumStates>& pIEnumStates, CKADTypeInfo* pTypeInfo)
{
    HRESULT         hr;
    CComBSTR        sIcon;
    CComBSTR        sStateVal;
    CComBSTR        sOverlay;
//    CComBSTR        sText;
    CComPtr<IState> pIState;
    ULONG           n;
    BOOL            bReturn = TRUE;
    CKADStatusData* pState;
    int             iStateIdx = 0;
    CString         strTmp;
    CString         strOverlay;
    int             iImageIndex = 0;
    BOOL            bOverlay;
    CKADOverlayData tOverlayData;


    if(pIEnumStates)
    {
        hr = pIEnumStates->Next(1, &pIState, &n);
        while (hr == S_OK && pIState)
        {
            pState = new CKADStatusData;
            pState->m_iImageId = pTypeInfo->m_tData.m_iImageIdx;

            bOverlay = FALSE;

            hr = pIState->GetIcon(&sIcon);
            if(hr == S_OK)
            {
                strTmp = sIcon;

                hr = pIState->GetOverlay(&sOverlay);
                if(hr == S_OK)
                {
                    bOverlay = TRUE;
                    strOverlay = sOverlay;
                    strOverlay.MakeLower();
                    if(m_tOverlayMap.Lookup(strOverlay, tOverlayData))
                    {
                        iImageIndex = tOverlayData.m_iOverlayImage;
                    }
                    else
                    {
                        iImageIndex = AddBitmapToImageList(strTmp);  // add bitmap
                        tOverlayData.m_iOverlayImage = iImageIndex;
                        tOverlayData.m_iOverlayState = ++m_iOverlayCount;
                        m_tOverlayMap.SetAt(strOverlay, tOverlayData);
                        ASSERT(iImageIndex >= 0);
                    }
                    pState->m_iOverlayState = tOverlayData.m_iOverlayState;
                }
                else
                {
                    iImageIndex = AddBitmapToImageList(strTmp);  // add bitmap
                    pState->m_iImageId = iImageIndex;
                    ASSERT(iImageIndex >= 0);
                }
            }
            else
            {
                if(bOverlay && pState->m_iOverlayState == -1)
                {
                    ASSERT(0);
                }
            }

            hr = pIState->GetStateValue(&sStateVal);
            if(hr == S_OK)
            {
                strTmp = sStateVal;
                pTypeInfo->InsertStateValues(strTmp, iStateIdx);
            }

            pTypeInfo->m_tStateArray.Add(pState);
            pIState.Release();
            hr = pIEnumStates->Next(1, &pIState, &n);
            ++iStateIdx;
        }
    }
    return(bReturn);
}


//------------------------------------------------------------------*
/**
 * set overlay images.
 *
 * @param           rtImageList: image list
 * @return          
 * @exception       -
 * @see             
*/
void CKADInfo::SetOverlayImages(CImageList& rtImageList)
{
    POSITION        pos = m_tOverlayMap.GetStartPosition();
    CKADOverlayData tKADOverlayData;
    CString         strOverlay;
    while(pos)
    {
        m_tOverlayMap.GetNextAssoc(pos, strOverlay, tKADOverlayData);
        rtImageList.SetOverlayImage(tKADOverlayData.m_iOverlayImage, tKADOverlayData.m_iOverlayState);
    }
}

//------------------------------------------------------------------*
/**
 * load bitmap from file and add it to image list
 *
 * @param           [in] strFileName: bitmap file name
 * @return          image index or -1 if failed
*/
int CKADInfo::AddBitmapToImageList(const CString& crstrFileName)
{
    int    iImageIdx = -1;

    CString strFileName = m_strBitmapDir + crstrFileName;

    HBITMAP     h_bmpImage;
    HIMAGELIST  hImageList = m_tImageList.GetSafeHandle();
    int         iCount;

    h_bmpImage = (HBITMAP)LoadImage(0, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE /*| LR_CREATEDIBSECTION*/);
    if(h_bmpImage)
    {
        iImageIdx = ::ImageList_AddMasked(hImageList, h_bmpImage, m_crBackGround);
        iCount = m_tImageList.GetImageCount();

        Sleep(10);
        if(iImageIdx != -1)
        {
            ASSERT(iImageIdx == m_iBitmapNo);
            m_iBitmapNo = iImageIdx + 1;
        }
        else
        {
            iImageIdx = 0;
            TRACE("Image %s not added\n", crstrFileName);
        }
        ::DeleteObject(h_bmpImage);
    }
    return(iImageIdx);
}

//------------------------------------------------------------------*
/**
 * return image list
 *
*/
void CKADInfo::CopyImageList(CImageList& rtImageList)
{
    rtImageList.Create(&m_tImageList);
    SetOverlayImages(rtImageList);
}


//------------------------------------------------------------------*
/**
 * get image ids for type.
 *
 * @param           [in] crstrType:type id string
 * @param           [in] crstrType:state as string
 * @return          
 * @exception       -
 * @see             
*/
BOOL CKADInfo::GetImageIdsForType(const CString& crstrType, const CString& crstrState, int& riImageId, int& riOverlayState)
{
    CKADTypeInfo*   pTypeInfo;
    int             iStateVal = 0;

    riOverlayState = -1;
    riImageId = 0;

    if(!m_tTypeInfoMap.Lookup(crstrType, pTypeInfo))
    {
        return(FALSE);
    }

    if(!crstrState.IsEmpty())
    {
        iStateVal = atoi(crstrState);
    }
    pTypeInfo->GetStateInfoFromStateVal(iStateVal, riImageId, riOverlayState);
    return(TRUE);
}


//------------------------------------------------------------------*
/**
 * get image id base for type.
 *
 * @param           [in] crstrType: type id string
 * @return          image base id
 * @see             
*/
int CKADInfo::GetImageIdBaseForType(const CString& crstrType)
{
    CKADTypeInfo*   pTypeInfo;
    int             iImageBase;

    if(!m_tTypeInfoMap.Lookup(crstrType, pTypeInfo))
    {
        return(FALSE);
    }
    iImageBase = pTypeInfo->m_tData.m_iImageIdx;
    return iImageBase;
}

//------------------------------------------------------------------*
/**
 * get pointer to context menu for type
 *
 * @param           [in] rstrType: type id string
 * @return          pointer to context menu or 0 if not found
*/
CKADMenu* CKADInfo::GetContextMenuInfoForType(const CString& rstrType)
{
    CKADTypeInfo*   pTypeInfo;

    if(!m_tTypeInfoMap.Lookup(rstrType, pTypeInfo))
        return 0;
    return(&(pTypeInfo->m_tMenu));
}


//------------------------------------------------------------------*
/**
 * get pointer to filter info object with certain index
 *
 * @param           [in] iIndex: index of filter object = view index
 * @return          pointer to filter object or 0 if index not valid
*/
CKADFilterInfo* CKADInfo::GetFilterInfo(int iIndex)
{
    if(iIndex >= 0 && iIndex < 4)
    {
        return(&m_tFilterInfo[iIndex]);
    }
    return(0);
}

//------------------------------------------------------------------*
/**
 * is type visible
 *
 * @param           [in] rstrType: type id string
 * @return          visible?
 * @see             
*/
BOOL CKADInfo::IsTypeVisible(const CString& rstrType)
{
    CKADTypeInfo*   pTypeInfo;

    // unknown types are always invisible
    if(!m_tTypeInfoMap.Lookup(rstrType, pTypeInfo))
    {
        return(FALSE);
    }
    return(pTypeInfo->m_tData.m_bVisible);
}

//------------------------------------------------------------------*
/**
 * is project list view type
 *
 * @param           [in] rstrType: type id string
 * @return          is to be shown in project list view?
*/
BOOL CKADInfo::IsProjectListViewType(const CString& rstrType)
{
    CKADTypeInfo*   pTypeInfo;

    // unknown types are always invisible
    if(!m_tTypeInfoMap.Lookup(rstrType, pTypeInfo))
    {
        return(FALSE);
    }
    return(pTypeInfo->m_tData.m_bListView);
}

    // 
//------------------------------------------------------------------*
/**
 * get type text
 *
 * @param           [in] rstrType: type id string
 * @return          is to be shown in project list view?
*/
CString CKADInfo::GetTypeText(const CString& rstrType)
{
    CKADTypeInfo*   pTypeInfo;

    // unknown types are always invisible
    if(!m_tTypeInfoMap.Lookup(rstrType, pTypeInfo))
    {
        return("");
    }
    return(pTypeInfo->m_tData.m_strText);
}


//------------------------------------------------------------------*
/**
 * load main menu information.
 *
 * @param           [in] pKadMan: interface pointer to KAD manager
 * @return          successful?
 *
*/
BOOL CKADInfo::LoadMainMenu(CComPtr<IMainMenu> &pKadMainMenu)
{
    HRESULT             hr;
    BOOL                bReturn = TRUE;
    CComPtr<IEnumMenus> pIEnumMenus;
    CComPtr<IMenu>      pIMenu;
    ULONG               n;

	hr = pKadMainMenu->GetMenuEnumerator(&pIEnumMenus);
    if(hr == S_OK)
    {
        hr = pIEnumMenus->Next(1, &pIMenu, &n);
        while (hr == S_OK && pIMenu)
        {
            if(!LoadMainSubMenu(pIMenu))
            {
                bReturn = FALSE;
            }
            pIMenu.Release();
            hr = pIEnumMenus->Next(1, &pIMenu, &n);
        }
    }
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * load main sub menu.
 *
 * @param           [in] pIMenu: interface pointer to menu
 * @param           [in/out] rtMenu: menu description
 * @return          completely loaded?
*/
BOOL CKADInfo::LoadMainSubMenu(CComPtr<IMenu>&pIMenu)
{
    HRESULT         hr;
    CComBSTR        bstrTmp;
    CComPtr<IEnumMenuItems> pIMenuItemEnum;
    CComPtr<IMenuItem>      pIMenuItem;
    ULONG           n;
    BOOL            bReturn = TRUE;
    CKADMenu*       pMenu = new CKADMenu;

    hr = pIMenu->GetId(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pMenu->m_strID = bstrTmp;
    }
    bstrTmp.Empty();
    hr = pIMenu->GetTitle(&bstrTmp);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }
    else
    {
        pMenu->m_strTitle = bstrTmp;
    }
    pMenu->m_pHandler = new GUID;
    hr = pIMenu->GetHandler(pMenu->m_pHandler);

    hr = pIMenu->GetMenuItemEnumerator(&pIMenuItemEnum);
    if(hr != S_OK)
    {
        bReturn = FALSE;
    }

    if(pIMenuItemEnum)
    {
        hr = pIMenuItemEnum->Next(1, &pIMenuItem, &n);
        while (hr == S_OK && pIMenuItem)
        {
            if(!LoadMenuItemInfo(pIMenuItem, pMenu, TRUE))
                bReturn = FALSE;
            pIMenuItem.Release();
            hr = pIMenuItemEnum->Next(1, &pIMenuItem, &n);
        }
    }
    pMenu->SortMenu();
    RegisterMainSubMenu(pMenu);
    return(bReturn);
}


//------------------------------------------------------------------*
/**
 * register main submenu.
 *
 *  register all items of main sub menu and add menu to main menu list.
 *
 * @param           [in] pMenu: pointer to main submenu
 * @see             RegisterMainMenuItem()
*/
void CKADInfo::RegisterMainSubMenu(CKADMenu* pMenu)
{
    CKADMenuItem*   pItem;

    int iNumItems = pMenu->m_tItems.GetSize();
    for(int iItem = 0; iItem < iNumItems; ++iItem)
    {
        pItem = pMenu->m_tItems[iItem];
        pItem->m_iItemNo = m_iMainMenuCount++;
        RegisterMainMenuItem(pItem->m_strId, pItem);
    }
    m_tMainMenu.AddMenu(pMenu);
}

// load toolbars
BOOL CKADInfo::LoadToolbars(CComPtr<IToolbars>&pIToolbars)
{
    HRESULT hr;
    BOOL    bReturn = TRUE;
    ULONG   n;

    CComPtr<IEnumToolbars> pIEnumToolbars;

    hr = pIToolbars->GetToolbarEnumerator(&pIEnumToolbars);
    if(hr == S_OK)
    {
        CComPtr<IToolbar>   pIToolbar;
        hr = pIEnumToolbars->Next(1, &pIToolbar, &n);
        while (hr == S_OK && pIToolbar)
        {
            if(!LoadToolbar(pIToolbar))
            {
                bReturn = FALSE;
            }
            pIToolbar.Release();
            hr = pIEnumToolbars->Next(1, &pIToolbar, &n);
        }
    }
    else
    {
        bReturn = FALSE;
    }
    return bReturn;
}

//------------------------------------------------------------------*
/**
 * load toolbar.
 *
 *  precondition: main sub menu with same ID is already loaded
 *
 * @param           [in] pIToolbar: interface pointer to KAD toolbar
 * @return          successful?
*/
BOOL CKADInfo::LoadToolbar(CComPtr<IToolbar>&pIToolbar)
{
    HRESULT         hr;
    BOOL            bReturn = TRUE;
    BOOL            bSeparator = FALSE;
    CComBSTR        sId;
    CComBSTR        sMainMenuId;
    CComBSTR        sToolTipText;
    CComBSTR        sIcon;
    ULONG           n;
    CComPtr<IEnumToolbarItems>  pIEnumToolbarItems;
    CComPtr<IToolbarItem>       pIToolbarItem;
    CKADMenu*       pMainSubMenu;
    CString         strId;

    // first get id
    hr = pIToolbar->GetId(&sId);
    if(hr == S_OK)
    {
        strId = sId;
        pMainSubMenu = m_tMainMenu.GetMenuById(strId);
        if(!pMainSubMenu)
        {
            return FALSE;
        }
    }

    // create new toolbar object
    CKADToolbar*    pToolbar = new CKADToolbar;

    pToolbar->m_strId = strId;

    hr = pIToolbar->GetToolbarItemEnumerator(&pIEnumToolbarItems);
    if(hr != S_OK || !pIEnumToolbarItems)
    {
        return FALSE;
    }

    hr = pIEnumToolbarItems->Next(1, &pIToolbarItem, &n);
    while(hr == S_OK && pIToolbarItem)
    {
        CString         strMainMenuId;
        CString         strIcon;
        CString         strToolTipText;

        hr = pIToolbarItem->GetToolTip(&sToolTipText);
        if(hr == S_OK)
        {
            strToolTipText = sToolTipText;
            sToolTipText.Empty();
        }

        hr = pIToolbarItem->GetIcon(&sIcon);
        ASSERT(hr == S_OK);
        if(hr == S_OK)
        {
            strIcon = sIcon;
            sIcon.Empty();
        }

        hr = pIToolbarItem->GetMainMenuId(&sMainMenuId);
        if(hr == S_OK)
        {
            strMainMenuId = sMainMenuId;   
            sMainMenuId.Empty();
        }

        // now add item
        pToolbar->AddItem(strMainMenuId, strIcon, strToolTipText);

        pIToolbarItem.Release();
        hr = pIEnumToolbarItems->Next(1, &pIToolbarItem, &n);
    }

    SetMainMenuIdsForToolbar(pToolbar);
    pToolbar->SortToolbar(pMainSubMenu);

    m_tToolbarInfo.AddToolbar(pToolbar);
    return bReturn;
}

//------------------------------------------------------------------*
/**
 * set main menu ids for toolbar.
 *
 *  tool tip text is copied from toolbar item to menu item
 *
 * @param           [in] pToolbar: pointer to KAD toolbar
*/
void CKADInfo::SetMainMenuIdsForToolbar(CKADToolbar* pToolbar)
{
    CKADToolbarItem*    pToolbarItem;
    CKADMenuItem*       pMenuItem;
    int                 iMainId;

    // run over all toolbar items
    pToolbarItem = pToolbar->GetFirstItem();
    while(pToolbarItem)
    {
        if(m_tMainIdMap.Lookup(pToolbarItem->m_strMainMenuId, iMainId))
        {
            pToolbarItem->m_iMainMenuItemNo = iMainId;
            m_tMenuItemMap.Lookup(iMainId, pMenuItem);
            if(pMenuItem)
            {
                pMenuItem->m_strToolTipText = pToolbarItem->m_strToolTipText;
            }
        }
        else
        {
            TRACE("Toolbar item %s could not be mapped to main menu entry!\n", pToolbarItem->m_strMainMenuId);
        }
        pToolbarItem = pToolbar->GetNextItem();
    }
}

// get pointer to main menu object
CKADMainMenu* CKADInfo::GetMainMenu()
{
    return(&m_tMainMenu);
}

// get pointer to toolbar object
CKADToolbarInfo* CKADInfo::GetToolbarInfo()
{
    return(&m_tToolbarInfo);
}

// get drag drop map
CKADDragDropMap* CKADInfo::GetDragDropMap()
{
    return(&m_tDragDropMap);
}


// load drag drop map
BOOL CKADInfo::LoadDragDropMap(CComPtr<IDragDropMap>&pIDragDropMap)
{
    HRESULT                         hr;
    BOOL                            bReturn = TRUE;
    CComPtr<IEnumDragDropActions>   pIEnumDragDropActions;
    CComPtr<IDragDropAction>        pIDragDropAction;
    ULONG                           n;

	hr = pIDragDropMap->GetDragDropActionEnumerator(&pIEnumDragDropActions);
    if(hr == S_OK)
    {
        hr = pIEnumDragDropActions->Next(1, &pIDragDropAction, &n);
        while (hr == S_OK && pIDragDropAction)
        {
            if(!LoadDragDropAction(pIDragDropAction))
            {
                bReturn = FALSE;
            }
            pIDragDropAction.Release();
            hr = pIEnumDragDropActions->Next(1, &pIDragDropAction, &n);
        }
    }
    return bReturn;
}

// load drag drop action
BOOL CKADInfo::LoadDragDropAction(CComPtr<IDragDropAction>&pIDragDropAction)
{
    HRESULT         hr;
    BOOL            bReturn = TRUE;
    CComBSTR        sDragType;
    CComBSTR        sDropType;
    CComBSTR        sEnableExpression;
    CGuid           tGUID;
    GUID            sGUID;

    hr = pIDragDropAction->GetDragType(&sDragType);
    if(hr != S_OK)
    {
        return FALSE;
    }
    hr = pIDragDropAction->GetDropType(&sDropType);
    hr = pIDragDropAction->GetHandler(&sGUID);
    if(hr == S_OK)
    {
        tGUID = sGUID;
    }
    hr = pIDragDropAction->GetEnableExpression(&sEnableExpression);
    m_tDragDropMap.AddDragDropAction((CString)sDragType, (CString)sDropType, tGUID, (CString)sEnableExpression);
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * get menu item from id.
 *
 *  lookup in menu item id map for menu item with given id.
 *   
 * @param           [in] lMenuId: menu item id.
 * @return          pointer to menu item or NULL if not found
 *
*/
CKADMenuItem* CKADInfo::GetMenuItemFromId(int iMenuId)
{
    CKADMenuItem*    pMenuItem = NULL;
    m_tMenuItemMap.Lookup(iMenuId, pMenuItem);
    return(pMenuItem);
}

