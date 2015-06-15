/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/FBListCtrl.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FBListCtrl.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/FBListCtrl.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FBLISTCTRL_H_
#define __FBLISTCTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CListCtrlEx.h"
#include "CELibrary.h"	// Hinzugefügt von ClassView //OH: was in 4cdll
#include "KADHelper/KADInfo.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



class CELibrarianView;
class CELibrarianDoc;

/////////////////////////////////////////////////////////////////////////////
// Fenster CFBListCtrl 

class CFBListCtrl : public CListCtrlEx
{
// Konstruktion
public:
	CFBListCtrl(CELibrarianView*);

// Attribute
public:
CELibrarianView* m_pview;
// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFBListCtrl)
	//}}AFX_VIRTUAL

// Implementierung
public:
	void ErrorMessage(long errId);
	void LastErrorMessage();
	static int CALLBACK CompareItem(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort);
	int AddFB(const CString& name, const CString type, const CStringArray& fullFilePaths);
	int AddFBLink(const CString& name, const CString type, const CStringArray& fullFilePaths);
    int AddAddFile(const CStringArray& fullFilePaths);
	CELibrary* GetCELibrary();
	CELibrarianDoc* GetDocument();
	void Refresh();
	void InitColumns();
	void DeleteRow(int row);
	BOOL DoPassWordDlg();
	void OnDeleteSelected();
	virtual ~CFBListCtrl();

	BOOL DoSelectFB(const CString& strName);
	BOOL DoShowFBHeader(const CString& strName);

    CKADInfo* m_pKADInfo;

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	void UpdateViewsWithSelectedItem();
	CELibrarianView* GetView();
	struct
	{
		INT column;
		BOOL ascending;
	} m_SortState;
	UINT m_uiClipFormat;// to hold the Clipboard format Id for this view
	UINT m_uiClipFormatXML;// to hold the Clipboard format Id for this view
	void DoPopupMenu(UINT nMenuID);
	afx_msg void OnBegindragOld(NMHDR* pNMHDR, LRESULT* pResult);

	//{{AFX_MSG(CFBListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCeFbcontextView();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClFbcontextShowhelp();
	afx_msg void OnUpdateClFbcontextShowhelp(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.


#endif // __FBLISTCTRL_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: FBListCtrl.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/10/01    Time: 11:58a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
