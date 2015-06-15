#if !defined(AFX_CEWATCHBACKEND_H__D6848BDA_EC03_11D2_A00E_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHBACKEND_H__D6848BDA_EC03_11D2_A00E_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cewatchbe.h"

class CCeWatchElement;
#define WM_CEWATCH_VARCHANGED	(WM_USER + 10)

interface IDebugInfo;
class CCeWatchType;
class CCeDisplayElement;
class CCeWatchBackEnd : public ICEWatchNotify
{
public:
	CCeWatchBackEnd(LPCTSTR pSymbolFile, CWnd* pWndParent);
	virtual ~CCeWatchBackEnd();
	LONG AddExpression(LPCTSTR pExpr, CCeDisplayElement* pData);
	void RemoveVar(LONG hExpr);
	void UpdateVar(LONG hExpr, CCeWatchElement *pVar);

	bool WriteVar(CCeWatchElement* pVar, LPCTSTR strNewValue);

	bool SetFormat(LONG hExpr, const CString& strFormat);
	CString GetFormat(LONG hExpr);
	bool GetType(LONG hExpr, CCeWatchType& type);
	bool GetType(LPCTSTR pExpr, CCeWatchType& type);
    bool GetTypeAndText(LPCTSTR pExpr, CCeWatchType& type, CString &strText);

	CString ScopeExpression (const CString &strExpr, const CString &strScope);

	IDebugInfo* GetDebugInfo();

public:
	static CString GetStateText (CEExpressionState state);
	static CString GetQualityText(WORD wQuality);
	static CString UTCtoLocalTime (DATE date);
	static CString CleanValue(CCeWatchElement* pVar, LPCTSTR strNewValue) ;
	static bool IsValidValue(CCeWatchElement* pVar, LPCTSTR strNewValue);
	static bool IsValidFormat(CCeWatchElement* pVar, LPCTSTR strNewValue);


public:
	STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP OnExpressionChanged (LONG hExpression);

protected:
	ULONG m_cRef;
	DWORD m_AdviseCookie;

private :
	LPUNKNOWN  m_pBackEnd;
	ICEWatch*  m_pICEWatch;
	CWnd* m_pWndParent;

//	CMap<DWORD, DWORD, void *, void *> m_MapBeH2pVar;

};
BOOL IsValidBackEnd(const CCeWatchBackEnd* pObj);

#endif // !defined(AFX_CEWATCHBACKEND_H__D6848BDA_EC03_11D2_A00E_00A024363DFC__INCLUDED_)
