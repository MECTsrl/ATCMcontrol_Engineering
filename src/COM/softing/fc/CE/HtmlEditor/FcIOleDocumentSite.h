#if !defined(AFX_FCIOLEDOCUMENTSITE_H__7672CD88_BC18_11D2_9FE3_00A024363DFC__INCLUDED_)
#define AFX_FCIOLEDOCUMENTSITE_H__7672CD88_BC18_11D2_9FE3_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcIOleDocumentSite.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcIOleDocumentSite 

class CFcIOleDocumentSite : public IOleDocumentSite
{
	// IUnkown interface
    protected:
        ULONG m_cRef;

	public:
        STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

	// IOleDocumentSite interface
	public : 
		STDMETHODIMP ActivateMe(IOleDocumentView *pView);

	// Constructor
    public:
        CFcIOleDocumentSite(IUnknown *pUnkownObj=NULL);
        ~CFcIOleDocumentSite();

    public:
		inline IOleDocumentView * GetDocumentView() { return m_pDocView; }

	private :
		IUnknown*			m_pUnkownObj;    
		IOleDocumentView*	m_pDocView;
};


#endif // !defined(AFX_FCIOLEDOCUMENTSITE_H__7672CD88_BC18_11D2_9FE3_00A024363DFC__INCLUDED_)
