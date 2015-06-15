#if !defined(AFX_FCIDISPATCH_H__0037C728_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_)
#define AFX_FCIDISPATCH_H__0037C728_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFcIDispatch : public IDispatch
{
    protected:
        ULONG			m_cRef;

	private :
		IUnknown*		m_pUnkownObj;

    public:
        CFcIDispatch(IUnknown *pUnkownObj=NULL);
        ~CFcIDispatch();

        STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

        STDMETHODIMP GetTypeInfoCount(UINT *);
        STDMETHODIMP GetTypeInfo(UINT, LCID, ITypeInfo **);
        STDMETHODIMP GetIDsOfNames(REFIID, OLECHAR **, UINT
            , LCID, DISPID *);
        STDMETHODIMP Invoke(DISPID, REFIID, LCID, USHORT
            , DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *);
};


#endif // !defined(AFX_FCIDISPATCH_H__0037C728_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_)
