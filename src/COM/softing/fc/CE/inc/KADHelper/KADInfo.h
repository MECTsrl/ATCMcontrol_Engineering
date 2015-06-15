#ifndef __KADINFO_H_
#define __KADINFO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "stdafx.h"
#include "CEKadMan.h"
#include "KadManager.h"
#include "KADTypeInfo.h"
#include <afxtempl.h>
#include <afxcmn.h>
#include <atlbase.h>
#include "KADFilterInfo.h"
#include "KADMainMenu.h"
#include "KADMenu.h"
#include "KADToolbarInfo.h"
#include "KADToolbar.h"
#include "KADDragDropMap.h"

// SIS 22.07.04 >>
#define MAXIMUM_DOCUMENT_COUNT	(100)
// SIS 22.07.04 <<

// SIS 08.11.04 >>
#define MAXIMUM_RECOVER_COUNT	(50)
// SIS 08.11.04 <<

//----  Forward Class Definitions:   -------------------------------*
class CKADOverlayData;

//----  Defines:    ------------------------------------------------*
typedef CMap<CString, LPCTSTR, CKADTypeInfo*, CKADTypeInfo*>        T_TypeInfoMap;
typedef CMap<CString, LPCTSTR, int, int>                            T_StringIntMap;
typedef CMap<CString, LPCTSTR, CKADOverlayData, CKADOverlayData&>   T_OverlayMap;
typedef CMap<int, int, CKADMenuItem*, CKADMenuItem*>                T_MenuItemMap;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CKADOverlayData
{
public:
    CKADOverlayData() {m_iOverlayState = 0; m_iOverlayImage = -1; };

public:
    int m_iOverlayState;    // 1 based overlay state
    int m_iOverlayImage;    // index of overlay image in image list
};

//------------------------------------------------------------------*
/**
 *  class CKADInfo - helper class to provide KAD-type information
 * 
 *  - contains type map to find type information from type name
 *  - reads KAD information using KAD-manager (CEKADMAN)
 *
 *  @see    CKADTypeInfo
 */
class CKADInfo
{
public:
    CKADInfo();
    ~CKADInfo();

    // load data from KAD
    BOOL Load(CComPtr<IUnknown>& pKadMan);


    // general parameters
    // get splash screen file name
    CString GetSplashScreenFile();
    
    // get desktop icon
    CString GetDesktopIcon();
    
    // get application title
    CString GetApplicationTitle();


    // copy image list
    void CopyImageList(CImageList& rtImageList);

    // return image and overlay ids for type
    BOOL GetImageIdsForType(const CString& crstrType, const CString& crstrState, int& riImageId, int& riOverlayId);

    // get pointer to context menu for type (return 0 if not found)
    CKADMenu* GetContextMenuInfoForType(const CString& rstrType);

    // get pointer to main menu object
    CKADMainMenu* GetMainMenu();

    // get pointer to toolbar object
    CKADToolbarInfo* GetToolbarInfo();

    // get pointer to filter info object with certain index
    CKADFilterInfo* GetFilterInfo(int iIndex);

    // is type visible
    BOOL IsTypeVisible(const CString& rstrType);

    // get type text
    CString GetTypeText(const CString& rstrType);

    // is project list view type
    BOOL IsProjectListViewType(const CString& rstrType);

    // get menu item from menu id
    CKADMenuItem* GetMenuItemFromId(int iMenuId);

    // get drag drop map
    CKADDragDropMap* GetDragDropMap();

	// SIS 22.07.04 SIS >>
	// get maximum open document count
	int GetMaximumNumberOfOpenDocuments();
	// SIS 22.07.04 SIS <<

	// SIS 08.11.04 SIS >>
	// get maximum open document count
	int GetMaximumNumberOfRecoverDocuments();
	// SIS 08.11.04 SIS <<

private:

    // load general info
    BOOL LoadGeneralInfo(CComPtr<IGeneral>& pIGeneral);

    // load type information from KAD
    BOOL LoadTypeInfo(CComPtr<IType>& pIKadType);

    // load type menu information from KAD
    BOOL LoadMenuInfo(CComPtr<IMenu>& pIMenu, CKADMenu* pTypeMenu);

    // load menu item info
    BOOL LoadMenuItemInfo(CComPtr<IMenuItem>& pIMenuItem, CKADMenu* pTypeMenu, BOOL bMain = FALSE);

    // load bitmap from file and add it to image list
    int AddBitmapToImageList(const CString& crstrFileName);

    // load state information
    BOOL LoadStates(CComPtr<IEnumStates>& pIEnumStates, CKADTypeInfo* pTypeInfo);

    // add type to existing filter
    void AddTypeToFilterItem(const CString& rstrView, const CString& rstrFilterText, const CString& rstrType);

    // load main menu
    BOOL LoadMainMenu(CComPtr<IMainMenu> &pKadMainMenu);

    // load main submenu
    BOOL LoadMainSubMenu(CComPtr<IMenu>&pIMenu);

    // register main submenu
    void RegisterMainSubMenu(CKADMenu* pMenu);

    // register main menu item
    void RegisterMainMenuItem(const CString& crstrId, CKADMenuItem* pMenuItem);

    // set main menu ids for toolbar
    void SetMainMenuIdsForToolbar(CKADToolbar* pToolbar);

    // load toolbars
    BOOL LoadToolbars(CComPtr<IToolbars>&pIToolbars);

    // load toolbar
    BOOL LoadToolbar(CComPtr<IToolbar>&pIToolbar);

    // get image base id
    int GetImageIdBaseForType(const CString& crstrType);

    // set overlay images
    void SetOverlayImages(CImageList& rtImageList);

    // load drag drop map
    BOOL LoadDragDropMap(CComPtr<IDragDropMap>&pIDragDropMap);

    // load drag drop action
    BOOL LoadDragDropAction(CComPtr<IDragDropAction>&pIDragDropAction);

    BOOL SetAcceleratorData(CKADMenuItem* pItem, const CString& crstrAccel);

    BOOL LoadVirtKeyMap();
    BOOL LoadVirtKeyMapFromFile();

    BOOL StripKey(CString& rstrAccel, CString& rstrKey);

    WORD GetModKeyFlag(const CString& crstrModKey);

private:
    CString             m_strBitmapDir;     // bitmap directory incl. trailing backslash

    T_TypeInfoMap       m_tTypeInfoMap;     // maps type names to type info objects

    CImageList          m_tImageList;       // image list

    T_OverlayMap        m_tOverlayMap;      // map of overlay id string to overlay data object

    int                 m_iOverlayCount;    // counts overlay images (used for states)

    int                 m_iBitmapNo;        // current bitmap number
    
    T_StringIntMap      m_kso_id_map;       // for mapping kso strings to kso-ids

    T_StringIntMap      m_km_id_map;        // for mapping of key / mouse events to menu items

    COLORREF            m_crBackGround;     // background color of images

    CKADFilterInfo      m_tFilterInfo[4];   // category filters for views

    CKADMainMenu        m_tMainMenu;        // main menu description

    int                 m_iMainMenuCount;   // counts main menu items (used for item ids)

    T_MenuItemMap       m_tMenuItemMap;     // map id -> menu item
    T_StringIntMap      m_tMainIdMap;       // map id string -> id

    CKADToolbarInfo     m_tToolbarInfo;     // toolbar info

    CKADDragDropMap     m_tDragDropMap;     // drag drop action map

    T_StringIntMap      m_tVirtKeyMap;      // map for virtual key codes

    CString             m_strSplashScreenFile;  // splash screen file name
    CString             m_strDesktopIcon;       // desktop icon file
    CString             m_strAppTitle;          // application title

	// SIS 22.07.04 SIS >>
	int					m_iMaxDocs;				// maximum count of open documents
	// SIS 22.07.04 SIS <<

	// SIS 08.11.04 SIS >>
	int					m_iMaxDocsRec;			// maximum count of documents to recover on system start
	// SIS 08.11.04 SIS <<
};

#endif // __KADINFO_H_
