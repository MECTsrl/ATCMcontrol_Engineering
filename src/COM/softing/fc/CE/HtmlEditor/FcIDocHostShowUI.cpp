

#include "stdafx.h"
#include "fchtmled.h"
#include "FcIDocHostShowUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFcIDocHostShowUI::CFcIDocHostShowUI(IUnknown *pUnkownObj/*=NULL*/)
{
	m_pUnkownObj = pUnkownObj;
    m_cRef = 1;
}

CFcIDocHostShowUI::~CFcIDocHostShowUI()
{
	ASSERT(m_cRef == 0);
}

STDMETHODIMP CFcIDocHostShowUI::QueryInterface(REFIID iid, void **ppvObject)
{
	*ppvObject = NULL;
	if (IID_IDocHostShowUI == iid || IID_IUnknown == iid)
	{
		*ppvObject = this;
		AddRef();
		return NOERROR;
	}

	if (m_pUnkownObj != NULL)
		return m_pUnkownObj->QueryInterface(iid, ppvObject);
	else
		return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CFcIDocHostShowUI::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcIDocHostShowUI::Release()
{
	ASSERT(m_cRef > 0);
    --m_cRef;

	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
    return m_cRef;
}

/*
 * CImpIDocHostShowUI::ShowMessage
 *
 */
STDMETHODIMP CFcIDocHostShowUI::ShowMessage(
            HWND /*hwnd*/,
            LPOLESTR /*lpstrText*/,
            LPOLESTR /*lpstrCaption*/, 
            DWORD /*dwType*/,
            LPOLESTR /*lpstrHelpFile*/,
            DWORD /*dwHelpContext*/,
            LRESULT* /*plResult*/)
{
	return S_FALSE;
}

/*
 * CImpIDocHostShowUI::ShowHelp
 */
STDMETHODIMP CFcIDocHostShowUI::ShowHelp(
            HWND /*hwnd*/,
            LPOLESTR /*pszHelpFile*/,
            UINT /*uCommand*/,
            DWORD /*dwData*/,
            POINT /*ptMouse*/,
            IDispatch* /*pDispatchObjectHit*/)
{
	return S_FALSE;
}

