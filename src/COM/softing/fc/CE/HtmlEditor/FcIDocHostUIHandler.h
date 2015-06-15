#if !defined(AFX_FCIDOCHOSTUIHANDLER_H__0037C723_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_)
#define AFX_FCIDOCHOSTUIHANDLER_H__0037C723_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mshtml.h>
#include <mshtmhst.h>
//#include <triedit.h>

class CFcIDocHostUIHandler : public IDocHostUIHandler
{
    protected:
        ULONG			m_cRef;

	private :
		IUnknown*		m_pUnkownObj;

    public:
        CFcIDocHostUIHandler(IUnknown *pUnkownObj=NULL);
        ~CFcIDocHostUIHandler();

        STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

		STDMETHODIMP GetHostInfo(DOCHOSTUIINFO * pInfo);
		STDMETHODIMP ShowUI(
				DWORD dwID, 
				IOleInPlaceActiveObject * pActiveObject,
				IOleCommandTarget * pCommandTarget,
				IOleInPlaceFrame * pFrame,
				IOleInPlaceUIWindow * pDoc);
		STDMETHODIMP HideUI(void);
		STDMETHODIMP UpdateUI(void);
		STDMETHODIMP EnableModeless(BOOL fEnable);
		STDMETHODIMP OnDocWindowActivate(BOOL fActivate);
		STDMETHODIMP OnFrameWindowActivate(BOOL fActivate);
		STDMETHODIMP ResizeBorder(
				LPCRECT prcBorder, 
				IOleInPlaceUIWindow * pUIWindow, 
				BOOL fRameWindow);
		STDMETHODIMP ShowContextMenu(
				DWORD dwID, 
				POINT * pptPosition,
				IUnknown* pCommandTarget,
				IDispatch * pDispatchObjectHit);
		STDMETHODIMP TranslateAccelerator(
            /* [in] */ LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID);
		STDMETHODIMP GetOptionKeyPath(LPOLESTR *pchKey, DWORD dw);
		STDMETHODIMP GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);

		STDMETHODIMP GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
        
        STDMETHODIMP TranslateUrl( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
        
        STDMETHODIMP FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);


	public :
		inline IOleCommandTarget* GetIOleCommandTarget() { return m_pIOleCommandTarget; }

	private :
		IOleCommandTarget* m_pIOleCommandTarget;
};

/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_FCIDOCHOSTUIHANDLER_H__0037C723_BF3F_11D2_9FE4_00A024363DFC__INCLUDED_)
