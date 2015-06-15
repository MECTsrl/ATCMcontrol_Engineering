/////////////////////////////////////////////////////////////////////////////
// LayoutMgr.h : SFL Layout Manager 
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager definition and implementation
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <objbase.h>
#include <foundation\GuidCast.h>
#include <foundation\SflCommon.h>
#include <foundation\Graphics\GraphicContext.h>
#include <foundation\Events\WinEvent.h>
#include <foundation\Events\EventRouter.h>
#include <foundation\Layout\LayoutNode.h>
#include <foundation\Layout\LayoutFactory.h>
#include <foundation\Layout\WindowNode.h>


namespace stingray {
namespace foundation {

//@doc

//////////////////////////////////////////////////////
// CLayoutManagerRootBase class


//@class CLayoutManagerRootBase |
// This class integrates the layout management logic into the ATL framework
// by mixing in the layout node tree and providing the plugin for handling
// windows messages and routing them through the layout nodes.
//@base public | ATL::CMessageMap
class CLayoutManagerRootBase:
	public CMessageMap
{
protected:

	BEGIN_MSG_MAP(CLayoutManagerRootBase)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		{
			IEventRouter* pRouter = guid_cast<IEventRouter*>(m_pLayoutRoot);
			if (pRouter) {
				IWinEvent* pWinEvent = new WinEvent(uMsg, wParam, lParam);
				bHandled = pRouter->RouteEvent(pWinEvent);
				lResult = pWinEvent->GetLResult();
				pWinEvent->Release();
				if (bHandled) {
					return TRUE;
				}
			}
		}
	END_MSG_MAP()

public:

	CLayoutManagerRootBase()
		: m_pLayoutRoot(0), m_bUseWindowRect(false), m_bOnSizeRecalc(true)
	{}

	~CLayoutManagerRootBase();

	// Implementation Member Variables
	ILayoutNode* m_pLayoutRoot;
	bool m_bUseWindowRect;
	bool m_bOnSizeRecalc;

	CLayoutFactory m_factory;

public:

	//@cmember void | InitLayout | (ILayoutNode* pRootNode) |
	// Initializes the layout tree.
	virtual void InitLayout(ILayoutNode* pRootNode);
	//@cmember void | AutoPopulate | () |
	// Automatically generates one child node for each child window in the master window.
	virtual void AutoPopulate()	{AutoPopulate(m_pLayoutRoot);}
	//@cmember void | RecalcLayout | (const CRect& rcLayout, bool bRefresh) |
	// Triggers the layout recalculation process.
	void RecalcLayout(const CRect& rcLayout = CRect(0, 0, 0, 0), bool bRefresh = false);

	//@cmember bool | WithinSizeLimits | (DWORD dwDrawAspect, SIZEL *psizel) |
	// Support for layout management in ActiveX controls and OLE objects.
	bool CLayoutManagerRootBase::WithinSizeLimits(DWORD dwDrawAspect, SIZEL *psizel);


protected:
	// Overridables

	//@cmember 
	// Return the window handle of the master window the layout operation is acting on.
	virtual HWND GetHWND() = 0;
	//@cmember
	// Returns the rectangle that should be initially assigned to the layout operation.
	virtual CRect GetInitialRect() = 0;
	//@cmember
	// Returns the rectangle that is going to be assigned to the layout tree after a resize operation.
	virtual CRect GetLayoutRect() = 0;

	// Implementation specific - not overridables
	void InitLayout(ILayoutNode* pRootNode, const CRect& rcInitial);
	void AutoPopulate(ILayoutNode* pSomeNode);

	//@cmember 
	// Returns the factory object utilized for the Node creation/destruction operations.
	CLayoutFactory& GetNodeFactory() 
	{return m_factory;}

	//@cmember,mfunc LayoutInterface | CreateLayoutNode | () |
	// Creates a new layout node of an specific class, and optionally initializes it.
	//@@syntax ISomeLayoutInterface* aNode = CreateLayoutNode(__uuidof(SomeLayoutNodeClass), aNode[, pParentNode[, ID]]);
	//@@syntax ILayoutNode* aNode = CreateLayoutNode(__uuidof(SomeLayoutNodeClass), pParentNode, ID);
	//@@syntax ILayoutNode* aNode = CreateLayoutNode(__uuidof(SomeLayoutNodeClass),pParentNode, hwnd);
	//@@tfarg typename | LayoutInterface | A layout interface derived from ILayoutNode, which will be the type
	// of the interface pointer returned by the templatized version.
	//@@parm GUID& | guid | Unique identifier of the class of the node requested.
	//@@parm LayoutInterface | aNode | Interface pointer that helps define the type of the pointer returned in the templatized version.
	//@@parm ILayoutNode* | pParentNode | Pointer to the node that will occupy the parent position in the tree of the node being created.
	// If NULL (default value) a parent is not assigned, the node needs to be explicitly inserted into the layout tree.
	//@@parm long | ID | Window identifier of a child window that will be associated with the newly create node. Default is -1, indicating no window.
	//@@parm HWND | hwnd | Handle to a child window that will be associated with the newly created node. Valid only for window nodes. Default is null.
	template <typename LayoutInterface>
	LayoutInterface CreateLayoutNode(const GUID& guid, LayoutInterface& pDerived, 
									ILayoutNode* pParentNode = NULL, long nId = -1)
	{
		ILayoutNode* pBasic = m_factory.CreateLayoutNode(guid);
		pDerived = guid_cast<LayoutInterface>(pBasic);
		if (pDerived == 0) {
			m_factory.DestroyLayoutNode(pBasic);
		}
		if (pDerived && pParentNode && nId != -1) {
			InitNode(pBasic, pParentNode, nId);
		}

		return pDerived;
	}

	ILayoutNode* CreateLayoutNode(const GUID& guid, ILayoutNode* pParentNode, long nId)
	{
		ILayoutNode* pNodeCreated = m_factory.CreateLayoutNode(guid);
		if (pParentNode && nId != -1) {
			InitNode(pNodeCreated, pParentNode, nId);
		}
		return pNodeCreated;
	}

	ILayoutNode* CreateLayoutNode(const GUID& guid, ILayoutNode* pParentNode = NULL, HWND hwnd = NULL)
	{
		ILayoutNode* pNodeCreated = m_factory.CreateLayoutNode(guid);
		if (pParentNode && hwnd) {
			InitNode(pNodeCreated, pParentNode, hwnd);
		}
		return pNodeCreated;
	}

	//@cmember,mfunc void | DestroyLayoutNode | (ILayoutNode* pNode) |
	// Destroys the given layout node and all of its descendents.
	//@@parm ILayoutNode* | pNode | Node to destroy and remove of the tree.
	void DestroyLayoutNode(ILayoutNode* pNode)
	{m_factory.DestroyLayoutNode(pNode);}


	// Message handlers

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


protected:

	void InitNode(ILayoutNode* pNodeToInit, ILayoutNode* pParentNode, HWND hwnd = NULL, bool bCallInit = true)
	{
		if (bCallInit) {
			HWND hwndThis = GetHWND();
			pNodeToInit->Init(hwndThis, hwnd);
		}
		if (pParentNode) {
			pParentNode->AddLayoutNode(pNodeToInit);
		}
	}

	void InitNode(ILayoutNode* pNodeToInit, ILayoutNode* pParentNode, long nId)
	{
		HWND hwndThis = GetHWND();
		InitNode(pNodeToInit, pParentNode, (nId > 0)? ::GetDlgItem(hwndThis, nId) : NULL, (nId != -1));
	}

};


inline
CLayoutManagerRootBase::~CLayoutManagerRootBase()
{
	if (m_pLayoutRoot) {
		m_factory.DestroyLayoutNode(m_pLayoutRoot);
	}
}


//@mfunc void | CLayoutManagerRootBase | InitLayout | 
// Called as part of the layout logic initialization process. Overriding this method,
// a descendent class can set up a layout tree following its layout preferences.
//@parm ILayoutNode* | pRootNode | Pointer to the root node in the layout tree. If null, 
//  a root has not been set, so the override can go ahead and create one assigning it to the
//  pRootNode parameter and calling the base class version of the method.
//@comm InitLayout() is called at the moment the layout manager gets initialized. That is as a
// response to the WM_CREATE message for normal windows or WM_INITDIALOG for dialog boxes. <nl>
// A derived class should override InitLayout creating all the layout nodes for the layout algorithms
// the visible elements that should apply, and arranging them in a composite tree according to the layout logic. <nl>
// The pRootNode parameter is the root node of the tree. If it is null, a root node has not been declared, 
// so it should be the first step before any other node can be added to the tree. <nl>
// Derived classes should always call the base class version of the routine, so the call can be routed all the way up
// to the CLayoutManagerRootBase version to finish up the initialization process.
//
/* @ex
  The following example shows a valid override of the InitLayoutFunction |
virtual void CMyDerivedLayoutManager::InitLayout(ILayoutNode* pRootNode)
{
	pRootNode = CreateLayoutNode(__uuidof(foundation::CScaleLayout));
	pRootNode->ModifyNodeStyleEx(0, foundation::OptimizeRedraw, true);
	_LayoutManager::InitLayout(pRootNode); // Always delegate to the base class
}
*/
inline
void CLayoutManagerRootBase::InitLayout (
	ILayoutNode* pRootNode
)
{
	InitLayout(pRootNode, GetInitialRect());
}


inline
void CLayoutManagerRootBase::InitLayout (
	ILayoutNode* pRootNode, 
	const CRect& rcInitial
)
{
	// First default initial rect
	m_pLayoutRoot = pRootNode;
	if (m_pLayoutRoot) {
		m_pLayoutRoot->SetInitialRect(rcInitial);
		m_pLayoutRoot->Init(GetHWND());

		// Recursively recalculate the appropriate Z-Order (necessary to 
		// avoid stepping on child control tab order, while still 
		// allowing nested splitters to coeexist)
		m_pLayoutRoot->RecalcZOrder();

		if (m_pLayoutRoot->GetChildrenCount() == 0) {
			AutoPopulate();
		}

		// Initial recalculation
		RecalcLayout(rcInitial, true);
	}
}


//@mfunc void | CLayoutManagerRootBase | AutoPopulate |
// Automatically creates a child node of the passed node for each child
// window the master window is parent of.
//@parm ILayoutNode* | pRootNode | Layout node to populate automatically.
//@comm AutoPopulate is called implicitly during the initialization process
// on the root node of the tree if this node is empty when the CLayoutManagerRootBase version
// of <mf CLayoutManagerRootBase::InitLayout> is invoked.<nl>
// This automatic mechanism for filling the node is adequate for simple layout algorithms
// that do not require additional settings, like Scale layout.
inline
void CLayoutManagerRootBase::AutoPopulate(ILayoutNode* pRootNode)
{
	if (pRootNode) {
		// First, get all child windows
		HWND hParent = GetHWND();
		HWND hChildWnd = ::GetWindow(GetHWND(), GW_CHILD);
		while (hChildWnd) {
			ILayoutNode* pNode = CWindowLayoutNode::CreateNode();
			pNode->Init(hParent, hChildWnd);
			pRootNode->AddLayoutNode(pNode);
			hChildWnd = ::GetNextWindow(hChildWnd, GW_HWNDNEXT);
		}
	}
}


//@mfunc void | CLayoutManagerRootBase | RecalcLayout |
// Triggers the recalculation process by calling <mf ILayoutNode::RecalcLayout> 
// and <mf ILayoutNode::RealizeNode> on the root node of the tree, using the given
// window rectangle as the new layout area.
//@parm const CRect& | rcLayout | New rectangular area the layout operation is going to 
// take effect on.
//@parm bool | bRefresh | Defines whether an invalidation needs to be performed after the recalculation.
inline
void CLayoutManagerRootBase::RecalcLayout (
	const CRect& rcLayout, 
	bool bRefresh
)
{
	if (m_pLayoutRoot) {
		// Kick off a recalc and refresh
		m_pLayoutRoot->RecalcLayout(rcLayout.IsRectEmpty()? m_pLayoutRoot->GetCurrentRect() : rcLayout, false, true);

		CLayoutTransaction aTransaction;
		m_pLayoutRoot->RealizeNode(aTransaction);

		if (bRefresh) {
			// Manually pump the first invalidation to insure
			// all child nodes initialize properly.
			m_pLayoutRoot->PerformInvalidation(false);
		}
	}
}


inline 
bool CLayoutManagerRootBase::WithinSizeLimits (
	DWORD dwDrawAspect, 
	SIZEL *psizel
)
{
	// Support for ActiveX controls
	if (!m_pLayoutRoot) return true;
	if (dwDrawAspect == DVASPECT_CONTENT ) {
		CSize szMin, szMax;
		DWORD dwFlags;
		m_pLayoutRoot->GetMinMaxSize(szMin, szMax, dwFlags);
		AtlPixelToHiMetric(&szMin, &szMin);
		AtlPixelToHiMetric(&szMax, &szMax);
		if (!(dwFlags & foundation::NoMinSize)) {	// check minimum
			if ((psizel->cx < szMin.cx) || (psizel->cy < szMin.cy)) {
				return false;
			}
		}
		if (!(dwFlags & foundation::NoMaxSize)) {	// check maximum
			if ((psizel->cx > szMax.cx) || (psizel->cy > szMax.cy)) {
				return false;
			}
		}

	}
	return true;
}


inline
LRESULT CLayoutManagerRootBase::OnSize (
	UINT /*uMsg*/, 
	WPARAM /*wParam*/, 
	LPARAM lParam, 
	BOOL& bHandled
)
{
	if (!m_pLayoutRoot) {
		return 0;
	}

	if (!m_bOnSizeRecalc) {
		return 0;
	}

	int nWidth = LOWORD(lParam); 
	int nHeight = HIWORD(lParam);
 
	// New size, peform all necessary size/position recalculations
	CRect rectNode = m_pLayoutRoot->GetCurrentRect();
	if (m_bUseWindowRect) {
		CRect rcWindow = GetLayoutRect();
		rectNode.right = rectNode.left + rcWindow.Width();
		rectNode.bottom = rectNode.top + rcWindow.Height();
	}
	else {
		rectNode.right = rectNode.left + nWidth;
		rectNode.bottom = rectNode.top + nHeight;
	}

	// Parent has sized, force a strict recalc of all children
	// Top level can not allow child negotiation (since window
	// size must be fixed, but children of top level can negotiate
	// size within that top level (and so on down the tree).
	RecalcLayout(rectNode);

	// Keep routing the message
	bHandled = FALSE;
	return 0;
}


inline
LRESULT CLayoutManagerRootBase::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CPaintGraphicsContext dcPaint(GetHWND());

	if (m_pLayoutRoot) {
		m_pLayoutRoot->PaintNode(dcPaint, true);
	}
	bHandled = FALSE;
	return 0;
}


inline
LRESULT CLayoutManagerRootBase::OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	LPMINMAXINFO lpmmi = reinterpret_cast<LPMINMAXINFO>(lParam);

	if (m_pLayoutRoot) {

		// Query configured min/max settings	
		CSize sizeMin;
		CSize sizeMax;
		DWORD dwFlags = 0;

		m_pLayoutRoot->GetMinMaxSize(sizeMin, sizeMax, dwFlags);

		// Min/Max width/height settings
		// Account for extra frame size (if present)
		HWND hwnd = GetHWND();
		CRect clientSize(0, 0, 0, 0), windowSize(0, 0, 0, 0);
		::GetClientRect(hwnd, &clientSize);
		::GetWindowRect(hwnd, &windowSize);
		int frameWidth = windowSize.Width() - clientSize.Width();
		_ASSERTE(frameWidth >= 0);
		int frameHeight = windowSize.Height() - clientSize.Height();
		_ASSERTE(frameHeight >= 0);

		if(!(dwFlags & foundation::NoMinSize)) {
			// Set the min/max
			lpmmi->ptMinTrackSize.x = sizeMin.cx + frameWidth;
			lpmmi->ptMinTrackSize.y = sizeMin.cy + frameHeight;
		}
		if(!(dwFlags & foundation::NoMaxSize)) {
			lpmmi->ptMaxTrackSize.x = sizeMax.cx + frameWidth;
			lpmmi->ptMaxTrackSize.y = sizeMax.cy + frameHeight;
		}
	}
	else {
		bHandled = FALSE;
	}

	return 0;
}


inline
LRESULT CLayoutManagerRootBase::OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bool bResult = false;
	HDC hdc = reinterpret_cast<HDC>(wParam);

	if (m_pLayoutRoot) {
		RECT rcClient = GetInitialRect();
		HRGN hrgnClip = ::CreateRectRgnIndirect(&rcClient);
		::GetClipRgn(hdc, hrgnClip);
		::SetRectRgn(hrgnClip, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		::SelectClipRgn(hdc, hrgnClip);
		::DeleteObject(hrgnClip);

		bResult = m_pLayoutRoot->EraseNodeBkgnd(hdc);
	}

	// Let the default handler paint the background,
	// now that we have set the clipping region.
	bHandled = FALSE;
	return bResult;
}




//////////////////////////////////////////////////////
// CLayoutManagerBase


//@class CLayoutManagerBase |
//  Same as CLayoutManagerRootBase, except that takes as a template parameter
//  the window message that has to be handled for the initiailization process.
//  The reason for this is that such message varies depending on whether the associated
//  window is a standard window or a dialog box.
//@base public | CLayoutManagerRootBase
//@tcarg UINT | initMessage | Message on which response the layout initialization process
// must be performed. Default is WM_CREATE.
template <UINT _initMessage = WM_CREATE>
class CLayoutManagerBase: 
	public CLayoutManagerRootBase
{
protected:
	typedef CLayoutManagerBase<_initMessage> _thisClass;

	BEGIN_MSG_MAP(_thisClass)
		MESSAGE_HANDLER(_initMessage, OnInitLayout)
		CHAIN_MSG_MAP(CLayoutManagerRootBase)
	END_MSG_MAP()


	LRESULT OnInitLayout(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Getting rid of errors.
		uMsg;
		wParam;
		lParam;

		InitLayout(m_pLayoutRoot);
		// Allow further processing of message
		bHandled = FALSE;
		return 1;
	}

};



//@class CLayoutManager |
//  The actual LayoutManager class. Implements the pure virtual functions defined in CLayoutManagerRootBase
//  in terms of the most derived class in the hierarchy, passed as the first template parameter. Mix in this class
//  as a ancestor of your window classes in order to provide layout managemente functionality.<nl>
//  <c CContainerWindow> provides a prebuilt base class for this kind of window, and can be used as base class for 
//  frame windows, dialogs or ActiveX controls. Customized windows with layout logic can include this class directly 
//  in their list of base classes and delegate their messages to it.<nl>
//  It is important to notice that it is essential for the well functioning of the class that the _initMessage passed 
//  as the second template parameter not be absorbed by any derived class in its route here. If any class neeeds to perform 
//  additional process as part of that message handler, it can do so as long as the bHandled flag in ATL is set to false.
//@base public | CLayoutManagerBase<lt>initMessage<gt>
//@tcarg typename | _T | Actual type of the most derived class using CLayoutManager as a mixin.
//@tcarg UINT | initMessage | Message on which response the layout initialization process
// must be performed. Default is WM_CREATE.
template <typename _T, UINT _initMessage = WM_CREATE>
class CLayoutManager:
	public CLayoutManagerBase<_initMessage>
{
public:
	CLayoutManager() {}

protected:
	virtual HWND GetHWND()
	{
		return static_cast<_T*>(this)->m_hWnd;
	}

	virtual CRect GetInitialRect() 
	{
		CRect rc;
		static_cast<_T*>(this)->GetClientRect(&rc);
		return rc;
	}

	virtual CRect GetLayoutRect() 
	{
		CRect rc;
		static_cast<_T*>(this)->GetWindowRect(&rc);
		return rc;
	}

};


};	// namespace stingray::foundation
};	// namespace stingray

