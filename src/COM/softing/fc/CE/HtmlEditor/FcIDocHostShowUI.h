#if !defined(AFX_FCIDOCHOSTSHOWUI_H__0037C727_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_)
#define AFX_FCIDOCHOSTSHOWUI_H__0037C727_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mshtml.h>
#include <mshtmhst.h>
//#include <triedit.h>

class CFcIDocHostShowUI : public IDocHostShowUI
{
    protected:
        ULONG			m_cRef;

	private :
		IUnknown*		m_pUnkownObj;

    public:
        CFcIDocHostShowUI(IUnknown *pUnkownObj=NULL);
        ~CFcIDocHostShowUI();

        STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

		STDMETHODIMP ShowMessage(
								HWND hwnd,
								LPOLESTR lpstrText,
								LPOLESTR lpstrCaption, 
								DWORD dwType,
								LPOLESTR lpstrHelpFile,
								DWORD dwHelpContext,
								LRESULT * plResult);
		STDMETHODIMP ShowHelp(
								HWND hwnd,
								LPOLESTR pszHelpFile,
								UINT uCommand,
								DWORD dwData,
								POINT ptMouse,
								IDispatch * pDispatchObjectHit);

};

/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_FCIDOCHOSTSHOWUI_H__0037C727_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_)
