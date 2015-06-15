//
// Stingray Software Extension Classes
// Copyright (C) 1996 Jim Beveridge
// All Rights Reserved
//
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
//
//  Author:			Jim Beveridge
//  Description:	Command description classes - Implementation
//  Created:		Sun Jan 28 10:10:29 1996
//
//

#ifndef __SCUTCMDS_H__
#define __SCUTCMDS_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif // __AFXTEMPL_H__

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
        #undef AFXAPP_DATA
        #define AFXAPP_DATA AFXAPI_DATA
        #undef AFX_DATA
        #define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL


//
// These modifiers always seemed to be defined whenever an accelerator is defined.
// This is empirical, I am not positive what the "real" behavior should be.
const WORD kStandardAcceleratorFlags = FVIRTKEY | FNOINVERT;

namespace SECShortcutUtils {
	void AccelToHotKey(const ACCEL& accel, DWORD* destHotkey);
	void HotKeyToAccel( DWORD destHotkey, ACCEL* accel );
	CString VkToName(UINT nChar, BOOL bExt);
	BOOL GetHotKeyDescription(DWORD hotkey, CString& strKeyName);
	BOOL GetMenuItemString(HMENU hMenu, UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags);
}


/************************************************************************
 *
 *		SECDefaultCommandId
 *
 *	@class	This structure holds a command id, its short name
 *			its description.  To customize which command id's
 *			the user may assign a macro to, create an array
 *			of this structure and fill it in.
 *
 *
 * For example, this declaration gives the system a
 * list of macro-able command id's, but all of the
 * short names and descriptions are figured out from
 * the corresponding string ids.
 *
 *	const SECDefaultCommandId defaultCommands[] =
 *	{
 *		{ ID_VIEW_TOOLBAR },
 *		{ ID_FILE_SAVE },
 *		// etc.
 *		{ ID_FILE_PRINT }
 *	};
 */
 
//@doc SECCommandList 
//@struct SECDefaultCommandId | This structure holds a command id, its short name and its description.  To customize which
// command ID's the user may assign a macro to, create an array of this structure and fill it in.
//@field UINT | m_nID | The ID of the command, such as ID_VIEW_TOOLBAR
//@field UINT | m_nName | String ID that gives the short name of the command. If this is zero then the menu name or tooltip text is used.
//@field UINT | m_nDescription | String ID that gives the description of the command. If zero then the status bar text is used.	
//@ex Creating a list of macro-able command ID's. | This declaration of an array of SECDefaultCommandId structures gives the system
// a list of macro-able command ID's, but overrides some of the default names and descriptions.
//const SECDefaultCommandId defaultCommands[] =
//	{
//		{ ID_VIEW_TOOLBAR, IDS_MAC_VIEW_TOOLBAR, IDS_DESC_VIEW_TOOLBAR },
//		{ ID_FILE_OPEN, 0, IDS_DESC_FILE_OPEN },	// Default name
//		{ ID_FILE_SAVE, IDS_MAC_FILE_SAVE, 0 },		// Default description
//		// etc.
//		{ ID_FILE_PRINT }							// Default both
//	};
//@xref<c SECCommandList>
struct SECDefaultCommandId
{
	// Id of this command, such as ID_VIEW_TOOLBAR
	UINT	m_nID;

	// String ID that gives the short name of the
	// command.  This name appears in the 
	// "Select a macro:" listbox.  If this is
	// zero, then the menu name or toolhelp text is used.
	UINT	m_nName;

	// String ID that gives the description of the
	// command.  This name appears in the 
	// "Description:" listbox.  If this is zero,
	// then the status bar text for this id is used.
	UINT	m_nDescription;
};



struct SECCommand {
	SECCommand();
	SECCommand(UINT nID);

	UINT	m_nID;
	CString	m_strName;
	CString	m_strDescription;
};



//@doc SECCommandList
//@mfunc void  | SECCommandList | SetRules | Call this function to define the invalid 
// combinations and the default modifier combination for a hot key control.The legal values are
// from CHotKeyCtrl::SetRules	.
//@parm WORD  | wInvalidComb | Specifies invalid key combinations.
//@parm WORD | wModifiers | Specifies the key combination to use when an invalid combination is entered. 
//@xref<c SECCommandList>




// adinsert AutoDuck insertion point for SECCommandList 
//@doc SECCommandList 
//@class SECCommandList contains the list of all command IDs that can be assigned, together with a 
//short name and a long description.  
//@comm
// Any and all parts of SECCommandList may be defaulted.
//
// SECCommandList maintains a list of command IDs that may be accelerated.  The accelerator keys 
// themselves are not kept in this structure, because it is a one-to-many relationship.
//
// See the SHORTCUT sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\SHORTCUT directory for a demonstration of this class.
//@xref<t SECDefaultCommandId> <c SECShortcutTable> <c SECShortcutDlg> 
//@BASE public|CArray<lt>SECCommand,SECCommand&<gt>
class SECCommandList : public CArray<SECCommand, SECCommand&>
{
//@access Public Members
public:
	//@cmember
	/* Creates an SECCommandList instance.*/
	SECCommandList(BOOL bInitRules=TRUE);
	virtual ~SECCommandList();

	// Use this to allow various key combinations.  The legal values
	// are from CHotKeyCtrl::SetRules	
	//@cmember
	/* Use this function to allow various key combinations. */
	virtual void SetRules( WORD wInvalidComb, WORD wModifiers )
		{ m_wInvalidComb = wInvalidComb; m_wModifiers = wModifiers; }

	// You automatically get all the menus.  Call this to
	// get rid of them.
	//@cmember
	/* Call this command to get rid of all the default command IDs.*/
	virtual void ClearCommandIds();

	// Takes an array of DefaultCommandId and adds
	// each entry into the list of legal commands.
	// Non-zero names and descriptions are loaded automatically.
	//@cmember
	/* Installs the custom list of IDs.*/
	virtual void AddCommandIds(const SECDefaultCommandId ids[], int count);

	// Returns TRUE if id should be excluded.
	//@cmember
	/* Determines whether an ID should be excluded.*/
	virtual BOOL QueryExcludeId(UINT nID);

	//@cmember
	/* Fills empty strings.*/
	virtual BOOL FillEmpties();

	//@cmember
	/* Automatically fills empty strings from the menu.*/
	virtual BOOL AutoFillEmptiesFromMenus();
	//@cmember
	/* Automatically fills empty strings from the string IDs.*/
	virtual BOOL AutoFillFromStringIds();

	// Fill in reasonable defaults from the menus, tooltips, etc.
	//@cmember
	/* Loads defaults from the menus, tooltips, etc.,*/
	virtual void DeriveDefaults();

	//@cmember
	/* Verifies the validity of the ID.*/
	virtual int Find(UINT nID);

	// The default implementation loads all the menu ID's.
	//@cmember
	/* Loads all the menu IDs.*/
	virtual BOOL AddDefaultIds();

	//@cmember
	/* Fills empty strings from the menu*/
	virtual BOOL FillEmptiesFromMenu(HMENU hMenu, const CString& strPrevious = "");
	
	//@cmember
	/* Loads IDs from the menu*/
	virtual void LoadIdsFromMenu(HMENU hMenu);
	
	//@cmember
	/* Loads Ids from the accelerator table.*/
	virtual void LoadIdsFromAccelTable();

	// Get rid of the accelerator, the &, the (&x), and the ...
	//@cmember
	/* Extracts accelerator characters from the menu string.*/
	virtual void ExtractMenuItemName(LPTSTR str);

protected:

// Attributes
public:
	WORD	m_wInvalidComb;
	WORD	m_wModifiers;

protected:

	// Operations
public:

// Overrides

// Implementation



protected:

};


#if _MFC_VER < 0x0400
// VC 4.0  handles this problem all by itself.
void ConstructElements(SECCommand* pTarget, int iCount);
void DestructElements(SECCommand* pTarget, int iCount);
#endif	//  _MFC_VER < 0x0400

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // WIN32
#endif	// __SCUTCMDS_H__

