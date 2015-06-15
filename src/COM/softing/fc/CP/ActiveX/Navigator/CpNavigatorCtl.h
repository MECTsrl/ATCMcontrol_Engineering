#if !defined(AFX_CPNAVIGATORCTL_H__B3380731_7157_11D1_A4A8_00A024363DFC__INCLUDED_)
#define AFX_CPNAVIGATORCTL_H__B3380731_7157_11D1_A4A8_00A024363DFC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <OBJSAFE.H>

// CpNavigatorCtl.h: Deklaration der CCpNavigatorCtrl-ActiveX-Steuerelementeklasse.

/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorCtrl: Siehe CpNavigatorCtl.cpp für Implementierung.

class CCpNavTab;
class CCpNavTreeCtrl;
class CCpObjectList;

class CCpNavigatorCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCpNavigatorCtrl)

	// Interface to declare control save for sripting
	DECLARE_INTERFACE_MAP()   
	BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)
		STDMETHOD_(HRESULT, GetInterfaceSafetyOptions) ( 
			/* [in] */ REFIID riid,
			/* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,
			/* [out] */ DWORD __RPC_FAR *pdwEnabledOptions      );        
		STDMETHOD_(HRESULT, SetInterfaceSafetyOptions) ( 
			/* [in] */ REFIID riid,            
			/* [in] */ DWORD dwOptionSetMask,
			/* [in] */ DWORD dwEnabledOptions      );
	END_INTERFACE_PART(ObjSafe);

// Konstruktor
public:
	CCpNavigatorCtrl();

public :
// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CCpNavigatorCtrl)
	public:
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementierung
//protected:
	~CCpNavigatorCtrl();

	DECLARE_OLECREATE_EX(CCpNavigatorCtrl)    // Klassenerzeugung und GUID
	DECLARE_OLETYPELIB(CCpNavigatorCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCpNavigatorCtrl)     // Eigenschaftenseiten-IDs
	DECLARE_OLECTLTYPE(CCpNavigatorCtrl)		// Typname und versch. Status

// Nachrichtenzuordnungstabellen
	//{{AFX_MSG(CCpNavigatorCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT OnNewURL(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLoadStatus(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch-Tabellen
	//{{AFX_DISPATCH(CCpNavigatorCtrl)
	afx_msg BSTR GetSelectedUrl();
	afx_msg BSTR GetSelectedServer();
	afx_msg SCODE LoadObjectList(LPCTSTR sUrl);
	afx_msg SCODE ShowObjectList(short iPage, long iFilter);
	afx_msg BSTR GetObjectUrl(LPCTSTR sName);
	afx_msg BSTR GetLoaderUrl(LPCTSTR sName);
	afx_msg BOOL IsObjectListLoaded();
	afx_msg BSTR GetObjetUrl2(LPCTSTR sName);
	afx_msg void SetFocus();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Ereignistabellen
	//{{AFX_EVENT(CCpNavigatorCtrl)
	void FireNewUrl(LPCTSTR sUrl, BOOL bNewServer, BOOL bNewWindow)
		{FireEvent(eventidNewUrl,EVENT_PARAM(VTS_BSTR  VTS_BOOL  VTS_BOOL), sUrl, bNewServer, bNewWindow);}
	void FireLoadingReady()
		{FireEvent(eventidLoadingReady,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch- und Ereignis-IDs
public:
	enum {
	//{{AFX_DISP_ID(CCpNavigatorCtrl)
	dispidGetSelectedUrl = 1L,
	dispidGetSelectedServer = 2L,
	dispidLoadObjectList = 3L,
	dispidShowObjectList = 4L,
	dispidGetObjectUrl = 5L,
	dispidGetLoaderUrl = 6L,
	dispidIsObjectListLoaded = 7L,
	dispidGetObjetUrl2 = 8L,
	dispidSetFocus = 9L,
	eventidNewUrl = 1L,
	eventidLoadingReady = 2L,
	//}}AFX_DISP_ID
	};


private :
    CCpObjectList*	m_pObjectList;
	
	CStatic*		m_pStatusLine;
	CCpNavTab*		m_pSheet;
	CCpNavTreeCtrl*	m_pTreeCtrl[3];

	BOOL			m_bServerUrl;
	CString			m_sUrl;

	BOOL			m_bObjectListLoaded;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_CPNAVIGATORCTL_H__B3380731_7157_11D1_A4A8_00A024363DFC__INCLUDED)
