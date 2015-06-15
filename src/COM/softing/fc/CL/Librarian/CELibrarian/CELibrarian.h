/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarian.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarian.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarian.h $
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

#ifndef __CELIBRARIAN_H_
#define __CELIBRARIAN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"       // Hauptsymbole
#include "objconvert.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif



/////////////////////////////////////////////////////////////////////////////
// CELibrarianApp:
// Siehe CELibrarian.cpp für die Implementierung dieser Klasse
//

class CELibrarianApp : public CWinApp
{
public:
	CELibrarianApp();

	virtual int Run();
	IObjConvert* GetObjConvert();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CELibrarianApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementierung
	COleTemplateServer m_server;
		// Server-Objekt für Dokumenterstellung

	//{{AFX_MSG(CELibrarianApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpFinder();
	afx_msg void OnPrintLibrary();
	afx_msg void OnViewConstants();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
		void ShowHelp(DWORD dwData, UINT nCmd);
		CDocument* GetDocument();

		CSingleDocTemplate* m_pDocTemplate;

		BOOL PrintPOUHeader(LPCTSTR pouName, CDC * pDCPrint, 
                      CPrintInfo * pPrintInfo,CDialog* pDlgPrintStatus);

private:
		IObjConvert* m_pIObjConvert;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.


#endif // __CELIBRARIAN_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarian.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
