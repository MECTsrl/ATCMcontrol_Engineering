/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianDoc.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarianDoc.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianDoc.h $
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

#ifndef __CELIBRARIANDOC_H_
#define __CELIBRARIANDOC_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CELibrary.h"	// Hinzugefügt von ClassView //OH: was in 4cdll
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CELibrarianIF.h"
#include "StdEditorDoc.h"

class CELibrarianSrvrItem;

class CELibrarianDoc : public CStdEditorDoc
{
protected: // Nur aus Serialisierung erzeugen
	CELibrarianDoc();
	DECLARE_DYNCREATE(CELibrarianDoc)

// Attribute
public:
	CELibrarianSrvrItem* GetEmbeddedItem()
		{ return (CELibrarianSrvrItem*)CStdEditorDoc::GetEmbeddedItem(); }
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pSite);

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CELibrarianDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL
	virtual bool LoadDocument (const CString &strFileName);
	virtual bool SaveDocument (const CString &strFileName);
	virtual HRESULT GotoLocation (const CString &strLocation);
	virtual HRESULT ShowPouHeader (const CString &strPouName);

// Implementierung
public:
	CString GetActualItem();
	void SetActualItem(const CString& actItem);
	void InitDocument();
	CELibrary* GetCELibrary();
	virtual ~CELibrarianDoc();
	BOOL IsReadOnly();
	void SetReadOnlyFlag(BOOL);
	void ErrorMessage(long errId);
	static void DebugHook();
	virtual void OnSerializeDocument (CArchive &ar) ;

    virtual HRESULT ShowHeader(const CString &strPouName);
    virtual HRESULT ShowUserHelp(const CString &strPouName);
    BOOL CanShowUserHelp();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL DoFileDialog(CString& fileName, BOOL bOpenFileDialog, DWORD lFlags);

// Generierte Message-Map-Funktionen
public:
		BOOL m_passWordProtected;
protected:
	typedef enum DOCSTATE { newdoc, loaded, none };
	CString m_actualItem;
	CELibrary m_CELibrary;
	DOCSTATE m_documentState;
	BOOL m_readOnly;

	//{{AFX_MSG(CELibrarianDoc)
	afx_msg void OnFileImport();
	afx_msg void OnLibraryOpen();
	//afx_msg void OnLibrarySave();
	//afx_msg void OnLibrarySaveas();
	//afx_msg void OnLibraryNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECMD_MAP()

	// automation ?
	// Generierte OLE-Dispatch-Map-Funktionen
	//{{AFX_DISPATCH(CELibrarianDoc)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	
	BEGIN_INTERFACE_PART(CELibrarianUserObj, ICELibrarianUser)
	    STDMETHOD(ShowPouHeader)(/*[in]*/ BSTR szPouName);
    END_INTERFACE_PART(CELibrarianUserObj)

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // __CELIBRARIANDOC_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarianDoc.h $
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
 * User: Oh           Date: 7/10/01    Time: 3:59p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
