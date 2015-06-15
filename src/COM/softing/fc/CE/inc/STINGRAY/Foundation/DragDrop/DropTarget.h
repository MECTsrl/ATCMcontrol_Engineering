// DropTarget.h
// Copyright (C) 1999 Stingray Software Inc.
// All Rights Reserved

#pragma once


namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// IDropTargetImpl
// AutoDuck tag block for IDropTargetImpl
//
//@doc IDropTargetImpl
//
//@mfunc int | IDropTargetImpl| IDropTargetImpl | Constructor
//@comm Constructor performs initialization
//
//@mfunc ULONG | IDropTargetImpl| AddRef | Place a reference count on the object
//@rdesc Returns the current reference count, but you're not supposed to care.
//@comm AddRef is a member of IUnknown 
//
//@mfunc ULONG | IDropTargetImpl| Release | Remove a reference count on the object
//@rdesc Returns the current reference count, but you're not supposed to care.
//@comm Release is a member of IUnknown 
//
//@mfunc HRESULT | IDropTargetImpl| QueryInterface | Get a new interface from the object
//@rdesc Returns S_OK if succeeded, otherwise, returns E_NOINTERFACE
//@parm REFIID | riid | Interface ID to use 
//@parm void** | ppv | place to put the pointer
//@comm QueryInterface is the standard means by which client code acquires
// new interfaces to a COM object. IDropTargetImpl supports IUnknown and IDropTarget.
//
//@mfunc HRESULT | IDropTargetImpl | DragEnter | Notification of drag enter
//@rdesc Returns S_OK if dragging should continue. Returns DRAGDROP_S_CANCELif the dragging should
// stop. Returns DRAGDROP_S_DROP if it's time to drop the data. 
//@parm IDataObject | *pDataObj | Pointer to a data object
//@parm DWORD | grfKeyState | Flag indicating key state 
//@parm POINTL | ptl | current point 
//@parm DWORD* | pdwEffect | Drop target fills this with the desired state based on the keyboard flags. 
//@comm DragEnter calls the virtual function OnDragEnter
//
//@mfunc HRESULT | IDropTargetImpl | DragOver | Notification of drag over 
//@rdesc Returns S_OK if dragging 
//@parm DWORD | grfKeyState | Flag indicating key state 
//@parm POINTL | ptl | current point 
//@parm DWORD* | pdwEffect | Drop target fills this with the desired state based on the keyboard flags. 
//@comm DragOver Calls virtual function OnDragOver
//
//@mfunc HRESULT | IDropTargetImpl | DragLeave | Drag operation is leaving target
//@rdesc Returns S_OK
//@comm DragOver virtual function OnDragLeave
//
//@mfunc HRESULT | IDropTargetImpl | Drop | Notification of dropping data 
//@rdesc Returns S_OK 
//@parm IDataObject* | pDataObj,| Pointer to data being dropped
//@parm DWORD | grfKeyState | Flag indicating key state 
//@parm POINTL | ptl | current point 
//@parm DWORD* | pdwEffect | Drop target fills this with the desired state based on the keyboard flags. 
//@comm Drop calls virtual function OnDrop
//
//@mfunc HRESULT | IDropTargetImpl| RegisterDragDrop | Register a window handle and an IDropTarget pointer 
//@rdesc Returns S_OK if the drop target registered
//@comm Uses m_OTLDThWnd
//
//@mfunc HRESULT | IDropTargetImpl| RegisterDragDrop | Register a window handle and an IDropTarget pointer 
//@rdesc Returns S_OK if the drop target registered
//@parm HWND | hWnd | Window to associate with the drop target. 
//@comm Indicate Which cursors to use
//
//@mfunc HRESULT | IDropTargetImpl| RevokeDragDrop | Revoke registration 
//@rdesc Returns S_OK if the registration was revoked
//@comm Uses m_OTLDThWnd
//
//@mfunc virtual DWORD | IDropTargetImpl | OnDragEnter | Notification of drag enter
//@rdesc Returns the drop effect
//@parm IDataObject | *pDataObj | Pointer to a data object
//@parm DWORD | grfKeyState | Flag indicating key state 
//@parm POINT | pt | current point 
//@comm OnDragOver does nothing-- you need to override this
//
//@mfunc virtual DWORD | IDropTargetImpl | OnDragOver | Notification of drag over 
//@rdesc Returns S_OK if dragging 
//@parm DWORD | grfKeyState | Flag indicating key state 
//@parm POINT | pt | current point 
//@comm OnDragOver does nothing-- you need to override this
//
//@mfunc void | IDropTargetImpl | OnDragLeave | Drag operation is leaving target
//@comm OnDragOver does nothing-- you need to override this
//
//@mfunc DWORD | IDropTargetImpl | OnDrop | Notification of dropping data 
//@rdesc Returns Drop Effect
//@parm IDataObject* | pDataObj,| Pointer to data being dropped
//@parm DWORD | grfKeyState | Flag indicating key state 
//@parm POINT | pt | current point 
//@comm OnDragOver does nothing-- you need to override this
//
//@class IDropTargetImpl | This class is provided as a standard way 
//
// Member functions include:
//  <mf IDropTargetImpl::IDropTargetImpl>, 
//  <mf IDropTargetImpl::AddRef>, 
//  <mf IDropTargetImpl::Release>, 
//  <mf IDropTargetImpl::QueryInterface>, 
//  <mf IDropTargetImpl::DragEnter>,
//  <mf IDropTargetImpl::DragOver>,
//  <mf IDropTargetImpl::DragLeave>,
//  <mf IDropTargetImpl::Drop>,
//  <mf IDropTargetImpl::OnDragEnter>,
//  <mf IDropTargetImpl::OnDragOver>,
//  <mf IDropTargetImpl::OnDragLeave>,
//  <mf IDropTargetImpl::OnDrop>,
//  <mf IDropTargetImpl::RegisterDragDrop>, and 
//  <mf IDropTargetImpl::RevokeDragDrop>
//
//@base public | IDropTarget
//
class IDropTargetImpl :
	public IDropTarget 
{
	LONG m_cRef;
	HWND m_OTLDThWnd;
	IUnknown* m_lpUnknown;

public:
	void Init()
	{
		m_cRef = 0;
		m_OTLDThWnd = 0;
		m_lpUnknown = 0;
		QueryInterface(IID_IUnknown, (void**)&m_lpUnknown);
	}

	IDropTargetImpl()
	{
		Init();
	}

	IDropTargetImpl(HWND hWnd)
	{
		Init();
		m_OTLDThWnd = hWnd;
	}

	~IDropTargetImpl()
	{
	}


public:
	// IDropTarget members

	//@cmember
	/* Increment reference count */
	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	//@cmember
	/* Decrement reference count */
	STDMETHODIMP_(ULONG) Release()
	{
		if (InterlockedDecrement(&m_cRef) == 0) {
			return 0;
		} 
		else {
			return m_cRef;
		}
	}

	//@cmember
	/* Get New interfaces. This object supports IUnknown and IDropTarget */
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		*ppv = NULL;
		if(riid == IID_IUnknown || riid == IID_IDropTarget) {
			*ppv = static_cast<IDropTarget*>(this);
		}

		if(*ppv) {
			((IUnknown*)(*ppv))->AddRef();
			return S_OK;
		} 
		else {
			return E_NOINTERFACE;
		}
	}

	//@cmember
	/* Called when mouse enters drop target */
    STDMETHODIMP DragEnter(IDataObject *pDataObj,
        DWORD grfKeyState,
        POINTL ptl,
        DWORD *pdwEffect)
	{
		POINT pt;
		pt.x = ptl.x; pt.y = ptl.y;
		*pdwEffect = OnDragEnter(pDataObj, grfKeyState, pt);
		return S_OK;
	}

	//@cmember
	/* Called as mouse goes over drop target */
    STDMETHODIMP DragOver(DWORD grfKeyState,
        POINTL ptl,
        DWORD *pdwEffect)
	{
		POINT pt;
		pt.x = ptl.x; pt.y = ptl.y;
		*pdwEffect = OnDragOver(grfKeyState, pt);
		return S_OK;
	}

	//@cmember
	/* Called when mouse leaves drop target */
    STDMETHODIMP DragLeave()
	{
		OnDragLeave();
		return S_OK;
	}

	//@cmember
	/* Called when data is being dropped */
    STDMETHODIMP Drop(IDataObject *pDataObj,
						DWORD grfKeyState,
						POINTL ptl,
						DWORD *pdwEffect)
	{
		POINT pt;
		pt.x = ptl.x; pt.y = ptl.y;
		*pdwEffect = OnDrop(pDataObj, grfKeyState, pt);
		return S_OK;
	}

	//@cmember
	/* Called to register drag drop */
	STDMETHODIMP RegisterDragDrop()
	{
		HRESULT hr = 0;
		if (CoLockObjectExternal(m_lpUnknown, TRUE, FALSE) != S_OK)
		{
			return E_FAIL;
		}

		if (!m_OTLDThWnd) {
			return DRAGDROP_E_INVALIDHWND;
		} 
		else {
			hr = ::RegisterDragDrop(m_OTLDThWnd, static_cast<IDropTarget*>(this));
			if(hr != S_OK)
			{
				CoLockObjectExternal(m_lpUnknown, FALSE, FALSE);
			}
			return hr;
		}
	}

	//@cmember
	/* Called to register drag drop */
	STDMETHODIMP RegisterDragDrop(HWND hWnd)
	{
		m_OTLDThWnd = hWnd;
		HRESULT hr;
		hr = this->RegisterDragDrop();
		return hr;
	}

	//@cmember
	/* Called to revoke drag drop */
	STDMETHODIMP RevokeDragDrop()
	{
		::RevokeDragDrop(m_OTLDThWnd);
		CoLockObjectExternal(m_lpUnknown, FALSE, FALSE);
		return S_OK;
	}


protected: 
	// Overridables

	//@cmember
	/* Called when mouse enters drop target */
    virtual DWORD OnDragEnter(IDataObject *pDataObj,
							        DWORD grfKeyState,
							        POINT pt) = 0;

	//@cmember
	/* Called when mouse is over drop target */
    virtual DWORD OnDragOver(DWORD grfKeyState,
									POINT pt) = 0;

	//@cmember
	/* Called when mouse leaves drop target */
    virtual void OnDragLeave() = 0;

	//@cmember
	/* Called when data is being dropped */
    virtual DWORD OnDrop(IDataObject *pDataObj,
								DWORD grfKeyState,
							    POINT pt) = 0;

}; // IDropTargetImpl


};	// namespace stingray::foundation
};	// namespace stingray

