
#ifndef __WATCHCTRLWRAPPER_H_
#define __WATCHCTRLWRAPPER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CWatchControlWrapper : public CWnd
{
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
            = { 0xD735F1A0, 0xA452, 0x4cde, { 0x83, 0x5F, 0xA8, 0x08, 0xC1, 0x04, 0x7A, 0x6A } };
			//= { 0xfa85c920, 0xe5b6, 0x11d2, { 0xa0, 0xa, 0x0, 0xa0, 0x24, 0x36, 0x3d, 0xfc } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL);

	HRESULT SetProjectPath(LPCTSTR pszPath);
	HRESULT Store(LPCTSTR pszFile);
	HRESULT Load(LPCTSTR pszFile);
    HRESULT ForceResubscribe();
    HRESULT SaveToXMLString(CString& rstrXMLString);
    HRESULT LoadFromXMLString(const CString& crstrXMLString);
    HRESULT Deactivate();

public:
	DECLARE_DYNCREATE(CWatchControlWrapper)
};

//{{AFX_INSERT_LOCATION}}

#endif // __WATCHCTRLWRAPPER_H_

