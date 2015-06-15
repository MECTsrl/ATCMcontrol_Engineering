#if !defined(AFX_FCIOLECLIENTSITE_H__7672CD87_BC18_11D2_9FE3_00A024363DFC__INCLUDED_)
#define AFX_FCIOLECLIENTSITE_H__7672CD87_BC18_11D2_9FE3_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFcIOleClientSite : public IOleClientSite
{
    protected:
        ULONG			m_cRef;

	private :
		IUnknown*		m_pUnkownObj;

    public:
        CFcIOleClientSite(IUnknown *pUnkownObj=NULL);
        ~CFcIOleClientSite();

        STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

        STDMETHODIMP SaveObject();
        STDMETHODIMP GetMoniker(DWORD, DWORD, LPMONIKER *);
        STDMETHODIMP GetContainer(LPOLECONTAINER *);
        STDMETHODIMP ShowObject();
        STDMETHODIMP OnShowWindow(BOOL);
        STDMETHODIMP RequestNewObjectLayout();
};

/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_FCIOLECLIENTSITE_H__7672CD87_BC18_11D2_9FE3_00A024363DFC__INCLUDED_)
