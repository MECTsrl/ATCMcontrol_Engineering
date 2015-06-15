#if !defined(AFX_FCIOLEINPACESITE_H__7672CD81_BC18_11D2_9FE3_00A224323DFC__INCLUDED_)
#define AFX_FCIOLEINPACESITE_H__7672CD81_BC18_11D2_9FE3_00A224323DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcIOleInPlaceSite.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcIOleInPlaceSite 

class CFcIOleInPlaceSite : public IOleInPlaceSite
{
	// IUnkown interface
    protected:
        ULONG m_cRef;

	public:
        STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

	// IOleInPlaceSite interface
	public : 
        STDMETHODIMP GetWindow(HWND *);
        STDMETHODIMP ContextSensitiveHelp(BOOL);
        STDMETHODIMP CanInPlaceActivate(void);
        STDMETHODIMP OnInPlaceActivate(void);
        STDMETHODIMP OnUIActivate(void);
        STDMETHODIMP GetWindowContext(LPOLEINPLACEFRAME *
                        , LPOLEINPLACEUIWINDOW *, LPRECT, LPRECT
                        , LPOLEINPLACEFRAMEINFO);
        STDMETHODIMP Scroll(SIZE);
        STDMETHODIMP OnUIDeactivate(BOOL);
        STDMETHODIMP OnInPlaceDeactivate(void);
        STDMETHODIMP DiscardUndoState(void);
        STDMETHODIMP DeactivateAndUndo(void);
        STDMETHODIMP OnPosRectChange(LPCRECT);

	// Constructor
    public:
        CFcIOleInPlaceSite(IUnknown *pUnkownObj=NULL);
        ~CFcIOleInPlaceSite();

	public :
		inline void SetWindowContext(CWnd *pSiteWnd) { m_pSiteWnd = pSiteWnd; }

	private :
		IUnknown*			m_pUnkownObj;    
		CWnd*				m_pSiteWnd;
		IOleInPlaceObject*	m_pIOleInPlaceObject;
};


#endif // !defined(AFX_FCIOLEINPACESITE_H__7672CD81_BC18_11D2_9FE3_00A224323DFC__INCLUDED_)
