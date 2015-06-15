//////////////////////////////////////////////////////////////////////
// ShortcutBar.h: interface for the SECShortcutBarComp class.
//
// Stingray Software Extension Classes
// Copyright © 1999 Stingray Software Inc,
// All rights reserved.
//
// Authors:      Prakash Surendra and Jay Gopalakrishnan
// Description:  ShortcutBar component class
//

#ifndef __SHORTCUTBARCOMP_H__
#define __SHORTCUTBARCOMP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace stingray;
using namespace foundation;

#include "Foundation\StructuredGraphics\VisualWindow.h"

// The MVC triad for the ShortcutBar
#include "TabBarmodel.h"
#include "TabBarController.h"
#include "BarViewport.h"

namespace stingray {

using namespace foundation;

/////////////////////////////
// AutoDuck tag block block for SECShortcutBarComp

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetHostImpl | Sets the shortcutbar's host implementation. When using the windowed version of the component, the
// host implementation is provided by the control class itself. However, when using the non-windowed version, the parent class hosting the control
// should implement IVisualWindow and invoke SetHostImpl() with a valid pointer to this interface.
//@parm IVisualWindow | pHost | Pointer to the IVisualWindow implementation.
//@xref<mf SECShortcutBarComp::GetHostImpl>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc IVisualWindow*  | SECShortcutBarComp | GetHostImpl | Returns an interface pointer to the host implementation of IVisualWindow.
//@xref<mf SECShortcutBarComp::SetHostImpl>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc int  | SECShortcutBarComp | AddBarVisual | Adds the visual client to the shortcutbar's client list. A visual client can be either an MVC viewport
// or an MVC visual component. 
//@parm TVisual* | pClient | Pointer to the visual client instance.
//@parm LPCTSTR | lpszLabel | The bar label.
//@parm BOOL | bUpdate | If TRUE the shortcutbar is forced to update itself. Default is FALSE.
//@rdesc The return value is the zero-based index of the newly added client.
//@xref<mf SECShortcutBarComp::AddBarWnd>
//@xref<mf SECShortcutBarComp::InsertBarVisual>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc int  | SECShortcutBarComp | AddBarWnd | Adds the window to the shortcutbar's client list. When using the windowed version of the shortcutbar, 
// client windows should be created as children of the shortcutbar. However, when using the non-windowed version of the control, all client windows
// should be made children of the hosting parent.
//@parm HWND& | hWndClient | A reference to the client window's handle.
//@parm LPCTSTR | lpszLabel | The bar label.
//@parm BOOL | bUpdate | If TRUE the shortcutbar is forced to update itself. Default is FALSE.
//@rdesc The return value is the zero-based index of the newly added client.
//@xref<mf SECShortcutBarComp::AddBarVisual>
//@xref<mf SECShortcutBarComp::InsertBarWnd>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc int  | SECShortcutBarComp | InsertBarVisual | Inserts the visual client at the specified index.
//@parm int | nIndex | The zero-based index at which the client is to be inserted.
//@parm TVisual* | pClient | Pointer to the visual client instance.
//@parm LPCTSTR | lpszLabel | The bar label.
//@rdesc The return value is the index of the newly added client.
//@xref<mf SECShortcutBarComp::InsertBarWnd>
//@xref<mf SECShortcutBarComp::AddBarVisual>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc int  | SECShortcutBarComp | InsertWnd | Inserts the client window at the specified index.
//@parm int | nIndex | The zero-based index at which the client is to be inserted.
//@parm HWND& | hWndClient | The window handle of the client.
//@parm LPCTSTR | lpszLabel | The bar label.
//@rdesc The return value is the index of the newly added client.
//@xref<mf SECShortcutBarComp::InsertBarVisual>
//@xref<mf SECShortcutBarComp::AddBarWnd>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | RemoveBar | Removes the bar at the specified index and hides the client.
//@parm int | nIndex | The zero-based index of the client to be removed.
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | ActivateBar | Activates the bar at the specified index. 
//@parm int | nIndex | The zero-based index of the bar to be activated.
//@parm BOOL | bUpdate | If TRUE the shortcutbar will be forced to update itself. Default is fALSE.
//@xref<mf SECShortcutBarComp::GetActiveIndex>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc int  | SECShortcutBarComp | GetActiveIndex | Fetches the index of the active bar.
//@xref<mf SECShortcutBarComp::ActivateBar>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetNotifyWnd | Sets the shortcutbar's notification target. The parent window will be the
// default notification target.
//@parm HWND | hWnd | The window handle of the notification target.
//@xref<mf SECShortcutBarComp::GetNotifyWnd>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc HWND  | SECShortcutBarComp | GetNotifyWnd | Returns the shortcutbar's notification target.
//@xref<mf SECShortcutBarComp::SetNotifyWnd>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc int  | SECShortcutBarComp | GetBarCount | Returns a count of the bars contained by the shortcutbar.
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc DWORD  | SECShortcutBarComp | SetBarStyle | Sets the styles for the shortcutbar.
//@parm DWORD | dwStyle | The shortcutbar styles.
//@rdesc Returns a DWORD of the previously assigned styles.
//@xref<mf SECShortcutBarComp::GetBarStyle>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc DWORD  | SECShortcutBarComp | GetBarStyle | Gets the shortcutbar styles.
//@xref<mf SECShortcutBarComp::SetBarStyle>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetBarIcon | Assigns an icon for the bar at the specified index.
//@parm int | nIndex | The bar index.
//@parm HICON | hIcon | The icon handle.
//@parm UINT | nIDIIcon | Resource ID of the icon.
//@xref<mf SECShortcutBarComp::GetBarIcon>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc HICON  | SECShortcutBarComp | GetBarIcon | Returns a handle to the icon for the bar at nIndex.
//@parm int | nIndex | The bar index.
//@xref<mf SECShortcutBarComp::SetBarIcon>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetBarLabel | Sets the display text for the bar at nIndex.
//@parm int | nIndex | The zero-based index of the bar.
//@parm LPCTSTR | lpszLabel | The bar text.
//@xref<mf SECShortcutBarComp::GetBarLabel>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc LPCTSTR  | SECShortcutBarComp | GetBarLabel | Returns a pointer to the string containing the bar label.
//@parm int | nIndex | The zero-based index of the bar.
//@xref<mf SECShortcutBarComp::SetBarLabel>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc HCURSOR  | SECShortcutBarComp | SetBarCursor | Assigns this cursor to the shortcutbar. The return value is a handle to the previous cursor.
//@parm HCURSOR | hBarCursor | A handle to the new cursor.
//@xref<mf SECShortcutBarComp::GetBarCursor>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc HCURSOR | SECShortcutBarComp | GetBarCursor | Returns a handle to the shortcutbar's current cursor.
//@xref<mf SECShortcutBarComp::SetBarCursor>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetBarFont | Sets the font for the bar at the particular index.
//@parm HFONT | hFont | The font handle to set.
//@parm int | nBarIndex | The zero-based index of the bar. If nBarIndex is -1, then the font will be used for all bars.
//@xref<mf SECShortcutBarComp::GetBarFont>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc HFONT  | SECShortcutBarComp | GetBarFont | Returns a handle to the font used for the bar at this index.
//@parm int | nBarIndex | The bar index.
//@xref<mf SECShortcutBarComp::SetBarFont>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void | SECShortcutBarComp | SetBarBrush | Sets the brush used for drawing the specified bar.
//@parm HBRUSH | hBrush | A handle to the brush.
//@parm int | nBarIndex | The zero-based index of the bar. If this value is -1, then the brush will be used for drawing all bars.
//@xref<mf SECShortcutBarComp::GetBarBrush>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc HBRUSH  | SECShortcutBarComp | GetBarBrush | Returns the brush used for painting the bar at the specified index.
//@parm int | nBarIndex | The zero-based index of the bar.
//@xref<mf SECShortcutBarComp::SetBarBrush>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetBarBkColor | Sets the background color for the bar at the specified index.
//@parm COLORREF | clrBk | The color to be set.
//@parm int | nBarIndex | The zero-based index of the bar. If nBarIndex is -1, then the color will be used for all bars.
//@xref<mf SECShortcutBarComp::GetBarBkColor>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc COLORREF  | SECShortcutBarComp | GetBarBkColor | Returns the background color used for the bar.
//@parm int | nBarIndex | The bar index.
//@xref<mf SECShortcutBarComp::SetBarBkColor>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void | SECShortcutBarComp | SetBarTextColor | Sets the text color for the bar.
//@parm COLORREF | clrText | The text color.
//@parm int | nBarIndex | The zero-based index of the bar. If this value is -1, then the color will be used for all bars.
//@xref<mf SECShortcutBarComp::GetBarTextColor>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc COLORREF  | SECShortcutBarComp | GetBarTextColor | Returns the COLORREF value used for painting the bar text.
//@parm int | nBarIndex | The zero-based index of the bar.
//@xref<mf SECShortcutBarComp::SetBarTextColor>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void  | SECShortcutBarComp | SetLabelAlignment | Sets the label alignment constant for the Shortcutbar. 
//@parm UINT | nAlign | The label alignment. The default value is center.
// One of the following alignments can be used:
// SEC_TABBAR_LBLALIGNLEFT
// SEC_TABBAR_LBLALIGNCENTER
// SEC_TABBAR_LBLALIGNRIGHT
//@xref<mf SECShortcutBarComp::GetLabelAlignment>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc UINT | SECShortcutBarComp | GetLabelAlignment | Returns the label alignment constant.
//@xref<mf SECShortcutBarComp::SetLabelAlignment>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void | SECShortcutBarComp | EditLabel | Starts an in-place edit of the bar text.
//@parm int | nIndex | The index of the bar.
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc void | SECShortcutBarComp | DisableBar | Enables/disables the bar at the specified index.
//@parm int | nIndex | The bar index.
//@parm BOOL | bDisable | If TRUE, the bar is disabled. 
//@xref<mf SECShortcutBarComp::IsBarEnabled>
//@xref<c SECShortcutBarComp>

//@doc SECShortcutBarComp
//@mfunc BOOL | SECShortcutBarComp | IsBarEnabled | Returns TRUE if the bar is enabled. Else FALSE.
//@parm int | nIndex | The bar index.
//@xref<mf SECShortcutBarComp::DisableBar>
//@xref<c SECShortcutBarComp>

// adinsert AutoDuck insertion point for SECShortcutBarComp
//@doc SECShortcutBarComp
//@class The SECShortcutBarComp class is the framework independent implementation for the Shorcutbar 
// component. This class ties up the underlying MVC arhitecture and provides a convenient API for using the control. 
// SECShortcutBarComp is an internal implementation detail and is documented primarily for the common API that it exposes
// through the framework specific controls. You should never directly use the SECShortcutBarComp class. Instead, your application,
// depending on the framework, should use one of the MFC/ATL specialized control classes.
//@comm
// SECShortcutBarComp supports the following styles:
//
// SEC_TABBAR_VERT - Displays the shortcutbar in the vertical mode. This is the default orientation.
//
// SEC_TABBAR_BARCURSOR - Uses a hand cursor for the bar objects. The default is to use the arrow cursor. 
//
// SEC_TABBAR_NOANIMATE - Disables animated switching of bars.
//
// SEC_TABBAR_NOHILIGHT - Disables highlighting of bars.
//
// SEC_TABBAR_CNTXTMENU - Sends the WM_TABBAR_CNTXTMENU message to the shortcutbar's owner window. 
//
//@base public | IQueryGuid
//@xref <c CATLShortcutBarWnd>
//@xref <c CATLShortcutBarHosted>
//@xref <c CMFCShortcutBarWnd>
//@xref <c CMFCShortcutBarHosted>
class SECShortcutBarComp : public IQueryGuid
{
public:
	SECShortcutBarComp()
	{
		m_pViewport = NULL;
		m_pModel = NULL;
		m_pController = NULL;
		m_pHostImpl = NULL;
	};

	virtual ~SECShortcutBarComp()
	{
		if(m_pViewport)
			delete m_pViewport;	
		if(m_pController)
			delete m_pController;
		if(m_pModel)	
			delete m_pModel;	
	};

	// IQueryGuid implementation
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)
	{
		if (guid == __uuidof(IVisualWindow))
			return m_pHostImpl->QueryGuid(guid, ppvObj);
		else 
			*ppvObj = NULL;

		return *ppvObj != NULL;
	};	

public:
	//@Access Public Members
	//@cmember
	/* Sets the host implementation for the shortcutbar. A host can be any class that has a valid HWND tied to it and implements the IVisualWindow interface.*/
	void SetHostImpl(IVisualWindow* pHost)	{ m_pHostImpl = pHost;	};

	//@cmember
	/* Returns a pointer to the host implementation.*/
	IVisualWindow* GetHostImpl()	{ return m_pHostImpl; }

	// Sets up the outlook bar without any window
	virtual BOOL Create(CRect rect)
	{
		IVisualWindow* pHostImpl;
		if(	!QueryGuid(__uuidof(IVisualWindow), (void**)&pHostImpl) )
		{
			// Note: "Host canvas has not been provided. Creation cannot proceed without a valid implementation
			// of the IVisualWindow interface.
			assert(FALSE);
		}	

		m_pViewport = new SECBarViewport;
		assert(m_pViewport);
		m_pViewport->SetHostImpl(pHostImpl);
		m_pViewport->SetOrigin(rect.left, rect.top);
		m_pViewport->SetSize(rect.Width(), rect.Height());

		m_pModel = new SECTabBarPresModel;
		assert(m_pModel);

		m_pViewport->SetModel(m_pModel);
			
		m_pController = new SECTabBarController();
		assert(m_pController);
		m_pController->SetViewport(m_pViewport);
		m_pController->SetModel(m_pModel);

		m_pViewport->SetController(m_pController);	
			
		return TRUE;
	};	

	//@cmember
	/* Adds the visual client, pClient, to the shortcutbar and sets the bar label to be lpszLabel.*/
	template<class TVisual>
		int AddBarVisual(TVisual* pClient, LPCTSTR lpszLabel, BOOL bUpdate = FALSE) { return m_pModel->AddTabBarVisual(pClient, lpszLabel, bUpdate); }
	
	//@cmember
	/* Adds the client window to the shortcutbar.*/
	int AddBarWnd(HWND& hWndClient, LPCTSTR lpszLabel, BOOL bUpdate = FALSE)	{ return m_pModel->AddTabBarWnd(hWndClient, lpszLabel, bUpdate); }
	
	//@cmember
	/* Inserts the visual client at the specified location.*/
	template<class TVisual>
	int InsertBarVisual(int nIndex, TVisual* pClient, LPCTSTR lpszLabel) {
			return m_pModel->InsertTabBarVisual(nIndex, pClient, lpszLabel);}
	
	//@cmember
	/* Inserts the client window at the specified location.*/
	int InsertBarWnd(int nIndex, HWND& hWndClient, LPCTSTR lpszLabel) {
			return m_pModel->InsertTabBarWnd(nIndex, hWndClient, lpszLabel); }

	//@cmember
	/* Removes the bar corresponding to the index.*/
	void RemoveBar(int nIndex) { m_pModel->RemoveTabBar(nIndex); }

	//@cmember
	/* Activates the bar at the particular index. Setting bUpdate to TRUE will force the control to update itself.*/
	void ActivateBar(int nIndex, BOOL bUpdate = FALSE) { m_pModel->ActivateTabBar(nIndex, bUpdate); }

	//@cmember
	/* Returns the zero-based index of the active bar.*/
	int GetActiveIndex() { return m_pModel->GetActiveIndex(); }

	//@cmember
	/* Sets the notification target for the control.*/
	void SetNotifyWnd(HWND hWnd) { m_pModel->SetNotifyWnd(hWnd); }

	//@cmember
	/* Returns a handle to the notification target*/
	HWND GetNotifyWnd() { return m_pModel->GetNotifyWnd(); }

    //@cmember
	/* Gets the number of bars contained in the shorcutbar.*/
	int GetBarCount() { return m_pModel->GetTabBarCount(); }

	//@cmember
	/* Sets the shortcutbar style.*/
	DWORD SetBarStyle(DWORD dwStyle) { return m_pModel->SetTabBarStyle(dwStyle); }

	//@cmember
	/* Returns a DWORD containing the shortcutbar styles.*/
	DWORD GetBarStyle()	{ return m_pModel->GetTabBarStyle(); }

	//@cmember
	/* Sets an icon for the bar at the particular index.*/
	void SetBarIcon(int nIndex, HICON hIcon) { m_pModel->SetTabBarIcon(nIndex, hIcon); }

	//@cmember
	/* Sets an icon for the bar at the particular index.*/
	void SetBarIcon(int nIndex, UINT nIDIcon) { m_pModel->SetTabBarIcon(nIndex, nIDIcon); }

	//@cmember
	/* Returns a handle to the icon associated with the particular bar.*/
	HICON GetBarIcon(int nIndex) { return m_pModel->GetTabBarIcon(nIndex); }

	//@cmember
	/* Sets the display text for the bar at this index.*/
	void SetBarLabel(int nIndex, LPCTSTR lpszLabel) { m_pModel->SetTabBarLabel(nIndex, lpszLabel); }

	//@cmember
	/* Returns a pointer to a string containing the bar's label.*/
	LPCTSTR GetBarLabel(int nIndex) { return m_pModel->GetTabBarLabel(nIndex); }

	//@cmember
	/* Sets a new cursor for the shortcutbar. The function returns a handle to the previous cursor.*/ 
	HCURSOR SetBarCursor(HCURSOR hBarCursor) { return m_pModel->SetTabBarCursor(hBarCursor); }

	//@cmember
	/* Returns a handle to the current cursor.*/
	HCURSOR GetBarCursor() { return m_pModel->GetTabBarCursor(); }

	//@cmember
	/* Sets the font for the bar at nBarIndex. If the index is -1, then the font will be used for all bars.*/
	void SetBarFont(HFONT hFont, int nBarIndex = -1)	{ m_pModel->SetTabBarFont(hFont, nBarIndex); }

	//@cmember
	/* Returns the font used for the bar lying at this index.*/
	HFONT GetBarFont(int nBarIndex)	{ return m_pModel->GetTabBarFont(nBarIndex); }

	//@cmember
	/* Sets the brush used for painting the particular bar. If nBarIndex is -1,the brush is used for all bars.*/
	void SetBarBrush(HBRUSH hBrush, int nBarIndex = -1) { m_pModel->SetTabBarBrush(hBrush, nBarIndex); }

	//@cmember
	/* Returns the brush used by this bar.*/
	HBRUSH GetBarBrush(int nBarIndex) { return m_pModel->GetTabBarBrush(nBarIndex); }

	//@cmember
	/* Sets the background color for the bar at the specified index. If nBarIndex is -1, the back color is set for all the bars.*/
	void SetBarBkColor(COLORREF clrBk, int nBarIndex = -1) { m_pModel->SetTabBarBkColor(clrBk, nBarIndex); }

	//@cmember
	/* Gets the background color of the specified bar.*/
	COLORREF GetBarBkColor(int nBarIndex) { return m_pModel->GetTabBarBkColor(nBarIndex); }

	//@cmember
	/* Sets the text color for the bar at nBarIndex. If the index value is -1, then the text color will be assigned for all bars.*/
	void SetBarTextColor(COLORREF clrText, int nBarIndex = -1) { m_pModel->SetTabBarTextColor(clrText, nBarIndex); }

	//@cmember
	/* Gets the text color used for this bar.*/
	COLORREF GetBarTextColor(int nBarIndex) { return m_pModel->GetTabBarTextColor(nBarIndex); }

	//@cmember
	/* Sets the label alignment for the Shortcutbar.*/
	void SetLabelAlignment(UINT nAlign)	{ m_pModel->SetLabelAlignment(nAlign); }

	//@cmember
	/* Returns the label alignment.*/
	UINT GetLabelAlignment() { return m_pModel->GetLabelAlignment(); }

	//@cmember
	/* Invokes an in-place edit of the specified bar's label.*/
	void EditLabel(int nIndex) { m_pViewport->EditLabel(nIndex); }

	//@cmember
	/* If bDisable is TRUE, then the specified bar is disabled. If not, it is enabled.*/
	void DisableBar(int nIndex, BOOL bDisable = TRUE) { m_pModel->DisableTabBar(nIndex, bDisable); }

	//@cmember
	/* Returns the enabled state for the particular bar.*/
	BOOL IsBarEnabled(int nIndex) { return m_pModel->IsTabBarEnabled(nIndex); }

	// A default route event implementation. This is called by CEventRouterMap_T::ProcessWindowMessage()
	bool RouteEvent(IEvent* pEvent)
	{
		// Handled by the viewport's RouteEvent method.
		if(m_pViewport)
			return m_pViewport->RouteEvent(pEvent);
		else
			return false;
	};
	
protected:
	SECBarViewport*		m_pViewport;
	SECTabBarPresModel*	m_pModel;
	SECTabBarController*  m_pController;
	IVisualWindow*		m_pHostImpl;
};

};  // namespace stingray

#endif //__SHORTCUTBARCOMP_H__
