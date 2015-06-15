// findfileappDlg.h : Header-Datei
//

#if !defined(AFX_FINDFILEAPPDLG_H__91801327_546A_11D2_925E_00104B4808FB__INCLUDED_)
#define AFX_FINDFILEAPPDLG_H__91801327_546A_11D2_925E_00104B4808FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CESysDef.h"

#define FIF_BUFLEN  1024

// options

#define MRUC_DEFAULT_MRU_SIZE        16
#define FIF_SECTION                  _T("FindFiles")
#define FIF_OPTION_SECTION           _T("FindFilesOptions")
#define FIF_OPTION_ENTRY             _T("Options")

/////////////////////////////////////////////////////////////////////////////
// CRecentFind

class CRecentFindFileList
{
public:
    CRecentFindFileList(UINT nStart, LPCTSTR lpszSection,
	    LPCTSTR lpszEntryFormat, int nSize)
    {
        m_pRecFileList = new CRecentFileList(nStart, lpszSection, lpszEntryFormat, nSize);
        ASSERT(m_pRecFileList);
    }

    ~CRecentFindFileList()
    {
        delete m_pRecFileList;
    }

	int GetSize() const
    {
        return m_pRecFileList->GetSize();
    }

	virtual void Remove(int nIndex)
    {
        m_pRecFileList->Remove(nIndex);
    }

	virtual void ReadList()
    {
        m_pRecFileList->ReadList();
    }

	virtual void WriteList()
    {
        m_pRecFileList->WriteList();
    }

	CString& operator[](int nIndex)
    {
        return (*m_pRecFileList)[nIndex];
    }

    void Add(LPCTSTR lpszPattern);

protected:
    CRecentFileList * m_pRecFileList;
};


/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox window

class CMRUComboBox : public CComboBox
{
// Construction
public:
	CMRUComboBox();

// Attributes & accessor functions
public:
    void           SetMRURegKey ( LPCTSTR szRegKey );
    const CString& GetMRURegKey() const;

    BOOL           SetMRUValueFormat ( LPCTSTR szValueFormat );
    const CString& GetMRUValueFormat() const;

    int            SetMaxMRUSize ( int nMaxSize );
    int            GetMaxMRUSize() const;

    BOOL SetAutoSaveOnDestroy   ( BOOL bAutoSave );
    BOOL SetAutoSaveAfterAdd    ( BOOL bAutoSave );
    BOOL SetAutoRefreshAfterAdd ( BOOL bAutoRefresh );

protected:
    CRecentFindFileList*m_pMRU;
    CString             m_cstrRegKey;
    CString             m_cstrRegValueFormat;
    int                 m_nMaxMRUSize;
    BOOL                m_bSaveOnDestroy;
    BOOL                m_bSaveAfterAdd;
    BOOL                m_bRefreshAfterAdd;

// Operations
public:
    BOOL AddToMRU ( LPCTSTR szNewItem );
    void EmptyMRU();

    void RefreshCtrl();

    BOOL LoadMRU();
    BOOL SaveMRU();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMRUComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMRUComboBox();

protected:
    BOOL AllocNewMRU();
    BOOL VerifyMRUParams() const;

    BOOL m_bParamsChanged;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMRUComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMenuButton window

class CMenuButton : public CButton
{
// Construction
public:
	CMenuButton();

// Attributes
public:

// Operations
public:
    DWORD DoSingleClick(UINT uiIDMenuResource);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Fenster CFindTextCombo 

class CFindTextCombo : public CMRUComboBox
{
// Konstruktion
public:
	CFindTextCombo();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFindTextCombo)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CFindTextCombo();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CFindTextCombo)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFindfileDlg Dialogfeld

class CFindfileDlg : public CDialog
{
// Konstruktion
public:
	void SetFindTextSel(DWORD dwFindTextSel);
	CFindfileDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CFindfileDlg)
	enum { IDD = IDD_FINDFILE_DLG };
	BOOL	m_bCaseSensitive;
	CString	m_strFindText;
	BOOL	m_bRegExp;
	BOOL	m_bWholeWords;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CFindfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung

public:
   	UINT m_uiOption;

protected:
	CFindTextCombo m_FindTextCombo;
	DWORD m_dwFindTextSel;
    CMenuButton m_mbInsertRegEx;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CFindfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnInsertRegExp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_FINDFILEAPPDLG_H__91801327_546A_11D2_925E_00104B4808FB__INCLUDED_)
