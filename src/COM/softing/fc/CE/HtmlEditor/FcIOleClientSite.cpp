

#include "stdafx.h"
#include "FcHtmlEd.h"
#include "FcIOleClientSite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFcIOleClientSite::CFcIOleClientSite(IUnknown *pUnkownObj/*=NULL*/)
{
	m_pUnkownObj = pUnkownObj;
    m_cRef = 1;
}

CFcIOleClientSite::~CFcIOleClientSite()
{
	ASSERT(m_cRef == 0);
}

STDMETHODIMP CFcIOleClientSite::QueryInterface(REFIID iid, void **ppvObject)
{
	*ppvObject = NULL;
	if (IID_IOleClientSite == iid || IID_IUnknown == iid)
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

STDMETHODIMP_(ULONG) CFcIOleClientSite::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CFcIOleClientSite::Release()
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


STDMETHODIMP CFcIOleClientSite::SaveObject()
{
    return NOERROR;
}

STDMETHODIMP CFcIOleClientSite::GetMoniker(
	DWORD /*dwAssign*/,
	DWORD /*dwWhich*/, 
	LPMONIKER* /*ppmk*/)
{
    return E_NOTIMPL;
}

STDMETHODIMP CFcIOleClientSite::GetContainer(
	LPOLECONTAINER* /*ppContainer*/)
{
    return E_NOTIMPL;
}

STDMETHODIMP CFcIOleClientSite::RequestNewObjectLayout()
{
    return E_NOTIMPL;
}

STDMETHODIMP CFcIOleClientSite::OnShowWindow(
	BOOL /*fShow*/)
{
    return NOERROR;
}

STDMETHODIMP CFcIOleClientSite::ShowObject()
{
    return NOERROR;
}
