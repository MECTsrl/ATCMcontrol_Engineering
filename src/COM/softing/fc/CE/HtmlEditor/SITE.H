#ifndef _SITE_H_
#define _SITE_H_

#include <mshtml.h>
#include <mshtmhst.h>
#include <triedit.h>


/*
 * The CSite class, a COM object with the interfaces IOleClientSite,
 * IAdviseSink, IOleInPlaceSite, and IOleDocumentSite.
 */

class CFcIDispatch;
class CFcIAdviseSink;
class CFcIOleClientSite;
class CFcIOleDocumentSite;
class CFcIOleInPlaceSite;
class CFcIDocHostUIHandler;
class CFcIDocHostShowUI;

class CSite : public IUnknown
{
private:
    LPUNKNOWN       m_pObj;
    ULONG           m_cRef;

    class CFcHtmlEdView*   m_pFR;

    //Our interfaces
    CFcIOleClientSite*		m_pIOleClientSite;
    CFcIOleDocumentSite*	m_pIOleDocumentSite;
    CFcIOleInPlaceSite*		m_pIOleInPlaceSite;
	CFcIDocHostUIHandler*	m_pIDocHostUIHandler;
    CFcIDocHostShowUI*		m_pIDocHostShowUI;
	CFcIDispatch*			m_pIDispatch;

	DWORD					m_dwDAdviseConnection;	
public:
    CSite(CFcHtmlEdView* pView);
    ~CSite();

    //Gotta have an IUnknown for delegation
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

	// Other functions
    BOOL        Create( TCHAR* pchPath );
    void        Close();
    void        Activate(LONG iVerb);
    void        UpdateDocumentViewRect();

	HRESULT		Load(TCHAR *pchPath);
	HRESULT		Save(TCHAR* pchPath);

	// helper functions
	inline LPUNKNOWN GetObjectUnknown(){ return m_pObj; } 
	IOleCommandTarget* GetCommandTarget();
};

#endif //_SITE_H_
