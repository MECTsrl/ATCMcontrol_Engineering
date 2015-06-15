////////////////////////////////////////////////////////////////////////////////
// TabBarModel.h: interfaces for the SECTabBarSysModel, SECTabBarPresModel classes.
//
// Stingray Software Extension Classes
// Copyright © 1999 Stingray Software Inc,
// All rights reserved.
//
// Author:      Prakash Surendra and Jay Gopalakrishnan
// Description:  ShortcutBar - MVCModel implementation
//

#ifndef __TABBAR_MODEL_H__
#define __TABBAR_MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Foundation\Mvc\MvcModel.h"
#include "Foundation\Mvc\MvcController.h"
#include "Foundation\Mvc\MvcVisualComponent.h"

#include <string.h>
#include <vector>

namespace stingray {

using namespace foundation;

class SECTabBarBase : public IQueryGuid
{
public:
	// IQueryGuid implementation
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)	{ return false;	}

	SECTabBarBase(LPCTSTR lpszLabel) {
		m_pszLabel = NULL;
		m_hIcon = NULL;
		m_bVisualComp = FALSE;
		SetLabel(lpszLabel);
	}

	virtual ~SECTabBarBase() {
		if(m_pszLabel)
			delete m_pszLabel;
		if(m_hIcon)
			::DestroyIcon(m_hIcon);
	}

	TCHAR* GetLabel() {	return m_pszLabel; }
	
	void SetLabel(LPCTSTR lpszLabel) {
		assert(lpszLabel);
		if(m_pszLabel)
			delete m_pszLabel;
		m_pszLabel = new TCHAR[_tcslen(lpszLabel)+1];
		_tcscpy(m_pszLabel, lpszLabel);
//		memcpy(m_pszLabel, lpszLabel, _tcslen(lpszLabel)+1);
	}
	
	HICON GetIcon() { return m_hIcon; }
	void SetIcon(HICON hIcon) {	m_hIcon = hIcon; }
	BOOL IsVisualComp() {	return m_bVisualComp;	}

protected:
// Attributes
	TCHAR*	m_pszLabel;
	HICON	m_hIcon;
	BOOL	m_bVisualComp;

friend class SECTabBarSysModel;
};

template<class T>
class SECTabBar : public SECTabBarBase
{
	typedef T _client;

public:
	// IQueryGuid implementation
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)	{ return false; }	

	SECTabBar()	{}
	SECTabBar(_client& client, LPCTSTR lpszLabel) : SECTabBarBase(lpszLabel), m_Client(client)	{}
	virtual ~SECTabBar()	{}

	_client& GetClient()	{ return m_Client; }	
	
protected:
	_client& m_Client;

friend class SECTabBarSysModel;
};

typedef SECTabBar<HWND> CTabBarWnd;

// The SECTabBarMvc class serves as an adapter for the common API between the MVC visual component and viewport types
template<class T>
class SECTabBarMvc : public SECTabBar<T>, public IVisual, public IBounds2D
{
public:
	SECTabBarMvc()	{}
	SECTabBarMvc(T& client) : SECTabBar<T>(client, _T(" ")) {}
	SECTabBarMvc(T& client, LPCTSTR lpszLabel) : SECTabBar<T>(client, lpszLabel) {}
	virtual ~SECTabBarMvc()	{}

	// IQueryGuid implementation
	virtual bool QueryGuid(REFGUID guid, void **ppvObj) { return m_Client.QueryGuid(guid, ppvObj); }

	// IRefCount implementation
	virtual ULONG STDMETHODCALLTYPE AddRef() { return m_Client.AddRef(); }
	virtual ULONG STDMETHODCALLTYPE Release() { return m_Client.Release(); }

	// IVisual implementation
	virtual void Draw(CDC* pCDC)	{ m_Client.Draw(pCDC); }
	virtual void OnPrepareDC(CDC* pCDC)	{ m_Client.OnPrepareDC(pCDC); }
	virtual void OnCleanupDC(CDC* pCDC)	{ m_Client.OnCleanupDC(pCDC); }

	// IBounds2D implementation
	virtual CSize GetSize() const { return m_Client.GetSize(); }
	virtual CSize SetSize(int cx, int cy) { return m_Client.SetSize(cx, cy); }
	virtual CRect GetBounds() const { return m_Client.GetBounds(); }
	virtual CPoint GetOrigin() const { return m_Client.GetOrigin();	}
	virtual CPoint SetOrigin(int x, int y) { return m_Client.SetOrigin(x,y); }
	virtual CPoint MoveOrigin(int xOff,int yOff) { return m_Client.MoveOrigin(xOff, yOff); }
};


// SECTabBarMessage - Updates observers with info related to changes in the tab bar model
class SECTabBarMessage : public CRefCountImpl<IMessage>
{
// IRefCount implementation
	IMPLEMENTS_REFCOUNTING();

public:
// IQueryGuid implementation
	virtual bool QueryGuid(REFGUID guid, void **ppvObj)	{
		if(guid == __uuidof(IMessage))
		{
			*ppvObj=static_cast<IMessage*>(this);
			AddRef();
			return true;
		}
		*ppvObj=NULL;
		return false;
	};

	// IMessage implementation
	// No other message type is currently being used. 
	virtual unsigned int GetTypeID() const		{ return 0; };	
	virtual void Sprint(CString& /*strCmd*/)	{};

public:
	SECTabBarMessage(UINT nTabBarVerb, SECTabBarBase* pTabBar, DWORD dwExtra = 0)	{
		m_nTabBarVerb = nTabBarVerb;
		m_pTabBar = pTabBar;
		m_dwExData = dwExtra;
		m_ulRefCount=0;
	};		
	virtual ~SECTabBarMessage()	{};

	enum { TABBAR_ACTIVATE = 0, TABBAR_ADD, TABBAR_INSERT, TABBAR_REMOVE, TABBAR_UPDATE };

	UINT m_nTabBarVerb;
	SECTabBarBase* m_pTabBar;
	DWORD m_dwExData;
};


// SECTabBarSysModel - System model for the ShortcutBar/TabWindow components
class SECTabBarSysModel  : public CMvcModel
{
typedef std::vector<SECTabBarBase*>	TabBarVector;
typedef CMvcController<CMvcModel, Visual2D> CMvcControllerInst;

public:
	SECTabBarSysModel()
	{
		m_nActiveIndex = -1;
	}

	virtual ~SECTabBarSysModel()
	{
		// Iterate collection and destroy all tabbar objects
		TabBarVector::iterator iter = m_vectTabBar.begin();
		while(iter != m_vectTabBar.end())
		{
			delete *iter;
			iter++;
		}
	}

	int GetTabBarCount()	{ return m_vectTabBar.size(); }

	SECTabBarBase* GetTabBarAt(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < GetTabBarCount());
		return m_vectTabBar.at(nIndex);
	}

	int GetTabBarIndex(SECTabBarBase* pTabBar)
	{
		assert(pTabBar);
		int nSz = GetTabBarCount();
		for(int i=0; i<nSz; i++)
		{
			if(m_vectTabBar.at(i) == pTabBar)
				return i;
		}
		return -1;
	}

	int GetActiveIndex()	{ return m_nActiveIndex; };

	template<class TVisual>
	int AddTabBarVisual(TVisual* pClient, LPCTSTR lpszLabel)
	{
		assert(pClient != NULL);
		SECTabBarBase* pTabBar = new SECTabBarMvc<TVisual> (*pClient, lpszLabel);
		assert(pTabBar);
		pTabBar->m_bVisualComp = TRUE;
		m_vectTabBar.push_back(pTabBar);
		if(m_nActiveIndex == -1)
			m_nActiveIndex = 0;
		return GetTabBarCount();
	}

	int AddTabBarWnd(HWND& hWndClient, LPCTSTR lpszLabel)
	{
		assert(::IsWindow(hWndClient));
		SECTabBarBase* pTabBar = new SECTabBar<HWND> (hWndClient, lpszLabel);
		assert(pTabBar);
		m_vectTabBar.push_back(pTabBar);
		if(m_nActiveIndex == -1)
			m_nActiveIndex = 0;
		return GetTabBarCount();
	}

	template<class TVisual>
	int InsertTabBarVisual(int nIndex, TVisual* pClient, LPCTSTR lpszLabel)
	{
		assert(pClient != NULL);
		SECTabBarBase* pTabBar = new SECTabBarMvc<TVisual>(*pClient, lpszLabel);
		assert(pTabBar);
		pTabBar->m_bVisualComp = TRUE;
		if (nIndex < GetTabBarCount()) 
			m_vectTabBar.insert(&m_vectTabBar[nIndex], pTabBar);
		else
		{
			m_vectTabBar.push_back(pTabBar);
			nIndex = GetTabBarCount();
		}
		if(nIndex <= m_nActiveIndex)
			m_nActiveIndex++;
		return 0;
	}

	int InsertTabBarWnd(int nIndex, HWND& hWndClient, LPCTSTR lpszLabel)
	{
		assert(::IsWindow(hWndClient));
		SECTabBarBase* pTabBar = new CTabBarWnd(hWndClient, lpszLabel);
		assert(pTabBar);

		if (nIndex < GetTabBarCount()) 
			m_vectTabBar.insert(&m_vectTabBar[nIndex], pTabBar);
		else
		{
			m_vectTabBar.push_back(pTabBar);
			nIndex = GetTabBarCount();
		}

		if(nIndex <= m_nActiveIndex)
			m_nActiveIndex++;

		return nIndex;
	}

	void RemoveTabBar(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < GetTabBarCount());

		// If nIndex is the currently active tabbar, then activate the previous/next tabbar
		if(nIndex == m_nActiveIndex)
		{
			if(nIndex-1 >= 0)
				m_nActiveIndex = nIndex-1;
			else if(nIndex+1 < GetTabBarCount())
					m_nActiveIndex = nIndex+1;
			else m_nActiveIndex = -1;

			NotifyObservers(SECTabBarMessage::TABBAR_REMOVE, GetTabBarAt(nIndex), m_nActiveIndex);	
		}
		
		// Remove the tab from the collection
		SECTabBarBase* pTabBar = GetTabBarAt(nIndex);
		m_vectTabBar.erase(&m_vectTabBar[nIndex]);
		delete pTabBar;

		if(nIndex <= m_nActiveIndex)
			m_nActiveIndex--;
	}

	void ActivateTabBar(int nActivate, BOOL bUpdate = FALSE)
	{
		assert(nActivate >= 0 && nActivate < GetTabBarCount());
		int nPrev = m_nActiveIndex;
		m_nActiveIndex = nActivate;
		
		NotifyObservers(SECTabBarMessage::TABBAR_ACTIVATE, GetTabBarAt(nActivate), nPrev);
		if(bUpdate)
			NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);	
	}

protected:
	void NotifyObservers(UINT nMsg, SECTabBarBase* pTabBar, DWORD dwExtra = 0)
	{
		SECTabBarMessage* pMsg = new SECTabBarMessage(nMsg, pTabBar, dwExtra);
		pMsg->AddRef();
	 	UpdateAllObservers(NULL, pMsg);
		pMsg->Release();
	}

protected:
	TabBarVector m_vectTabBar;
	int m_nActiveIndex;
};


class SECTabBarVisual
{
public:
	SECTabBarVisual()	{ m_hBrush = NULL; }
	virtual ~SECTabBarVisual()	{};

protected:
	HFONT		m_hFont;			// Font used for the tabbar label
	HBRUSH		m_hBrush;			// Brush used for painting the tabbar background
	COLORREF	m_clrText;			// Text color of the tabbar
	COLORREF	m_clrBkgnd;			// Background color of the tabbar
	BOOL		m_bDisabled;		// Disabled if true

friend class SECTabBarPresModel;
};


// Label Alignment constants
#define SEC_TABBAR_LBLALIGNLEFT			0
#define SEC_TABBAR_LBLALIGNCENTER		1
#define SEC_TABBAR_LBLALIGNRIGHT		2

// TabBar styles
#define SEC_TABBAR_VERT				0x00000001		// Default alignment
#define SEC_TABBAR_HORZ				0x00000002		// To be implemented
#define SEC_TABBAR_BARCURSOR		0x00000004
#define SEC_TABBAR_NOANIMATE		0x00000080
#define SEC_TABBAR_NOHILIGHT		0x00000200
#define SEC_TABBAR_CNTXTMENU		0x00000400

#define SEC_TABBAR_STYLES			(SEC_TABBAR_VERT		|\
									 SEC_TABBAR_HORZ		|\
									 SEC_TABBAR_BARCURSOR   |\
									 SEC_TABBAR_NOHILIGHT   |\
									 SEC_TABBAR_NOANIMATE   |\
									 SEC_TABBAR_CNTXTMENU) 	
								

// SECTabBarPresModel - Presentation model for the ShortcutBar/TabWindow components. The presentation model
// observes the system model.
class SECTabBarPresModel  : public CMvcModel, public IObserver
{
typedef std::vector<SECTabBarVisual*>	VectTabBarVisual;

public:
	SECTabBarPresModel()
	{
		// Create the system model and add this instance as an observer
		m_pSysModel = new SECTabBarSysModel;
		m_pSysModel->AddObserver(this);
		
		m_nLblAlgn = SEC_TABBAR_LBLALIGNCENTER;
		
		ConstructGDIObjects();
	}

	virtual ~SECTabBarPresModel()
	{
		DestroyGDIObjects();

		// Iterate collection and destroy all SECTabBarVisual objects
		VectTabBarVisual::iterator iter = m_vectVisual.begin();
		while(iter != m_vectVisual.end())
		{
			delete *iter;
			iter++;
		}

		if(m_pSysModel)
			delete m_pSysModel;
	}

	// IQueryGuid, IRefCount 
	bool QueryGuid(REFGUID guid, void **ppvObj)	{ return CMvcModel::QueryGuid(guid, ppvObj); }
	ULONG STDMETHODCALLTYPE AddRef()	{ return CMvcModel::AddRef(); }
	ULONG STDMETHODCALLTYPE Release()	{ return CMvcModel::Release();	}

	virtual void OnUpdate(ISubject* pSubject, IMessage* pMsg)
	{
		// Pass on the notification to all observers of this subject
		pMsg->AddRef();
		UpdateAllObservers(NULL, pMsg);
		pMsg->Release();
	}

	template<class TVisual>
	int AddTabBarVisual(TVisual* pClient, LPCTSTR lpszLabel, BOOL bUpdate)
	{
		int nTab = m_pSysModel->AddTabBarVisual(pClient, lpszLabel);		
		m_vectVisual.push_back(CreateTabBarVisual());
		NotifyObservers(SECTabBarMessage::TABBAR_ADD, GetTabBarAt(nTab-1));
		if(bUpdate)
			NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);
		return nTab;
	}

	virtual int AddTabBarWnd(HWND& hWndClient, LPCTSTR lpszLabel, BOOL bUpdate)
	{
		int nTab = m_pSysModel->AddTabBarWnd(hWndClient, lpszLabel);	
		m_vectVisual.push_back(CreateTabBarVisual());

		NotifyObservers(SECTabBarMessage::TABBAR_ADD, GetTabBarAt(nTab-1));
		if(bUpdate)
			NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);

		return nTab;
	}

	template<class TVisual>
	int InsertTabBarVisual(int nIndex, TVisual* pClient, LPCTSTR lpszLabel)
	{
		m_pSysModel->InsertTabBarVisual(nIndex, pClient, lpszLabel);
		if (nIndex < m_vectVisual.size()) 
			m_vectVisual.insert(&m_vectVisual[nIndex], CreateTabBarVisual());
		else
			m_vectVisual.push_back(CreateTabBarVisual());
		NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);
		return nIndex;
	}
	virtual int InsertTabBarWnd(int nIndex, HWND& hWndClient, LPCTSTR lpszLabel)
	{
		m_pSysModel->InsertTabBarWnd(nIndex, hWndClient, lpszLabel);	
		if (nIndex < m_vectVisual.size())
			m_vectVisual.insert(&m_vectVisual[nIndex], CreateTabBarVisual());	
		else
			m_vectVisual.push_back(CreateTabBarVisual());

		NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);

		return nIndex;
	}

	virtual void RemoveTabBar(int nIndex)
	{
		m_pSysModel->RemoveTabBar(nIndex);

		// Remove the SECTabBarVisual associated with this tabbar from the visual vector and delete it.	
		SECTabBarVisual* pVisual = GetTabBarVisualAt(nIndex);
		m_vectVisual.erase(&m_vectVisual[nIndex]);
		delete pVisual;

		NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);	
	}

	virtual void ActivateTabBar(int nActivate, BOOL bUpdate = FALSE)	{ m_pSysModel->ActivateTabBar(nActivate, bUpdate); }

	void SetNotifyWnd(HWND hWnd)
	{
		assert(::IsWindow(hWnd));
		m_hWndNotify = hWnd;
	}

	HWND GetNotifyWnd()	{ return m_hWndNotify; }

	DWORD SetTabBarStyle(DWORD dwStyle)
	{
		DWORD dwOld = m_dwStyle;
		m_dwStyle = dwStyle;
		return dwOld;
	}

	DWORD GetTabBarStyle()	{ return m_dwStyle; }

	void SetTabBarIcon(int nIndex, HICON hIcon)
	{
		SECTabBarBase* pTabBar = GetTabBarAt(nIndex);
		pTabBar->SetIcon( ::CopyIcon(hIcon) );
	}

	void SetTabBarIcon(int nIndex, UINT nIDIcon)
	{
		HICON hIcon = (HICON)::LoadImage(hResInstance, MAKEINTRESOURCE(nIDIcon), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
		SECTabBarBase* pTabBar = GetTabBarAt(nIndex);
		pTabBar->SetIcon(hIcon);
	}

	HICON GetTabBarIcon(int nIndex)
	{
		SECTabBarBase* pTabBar = GetTabBarAt(nIndex);
		return pTabBar->GetIcon();
	}

	void SetTabBarLabel(int nIndex, LPCTSTR lpszLabel)
	{
		SECTabBarBase* pTabBar = GetTabBarAt(nIndex);
		pTabBar->SetLabel(lpszLabel);
	}

	LPCTSTR GetTabBarLabel(int nIndex)	{ return GetTabBarAt(nIndex)->GetLabel(); }

	void SetTabBarFont(HFONT hFont, int nBarIndex)
	{
		// if nBarIndex = -1, then all tabbars have the same font
		if(nBarIndex == -1)
			SetAttribHelper(hFont, offsetof(SECTabBarVisual, m_hFont));		
		else
			GetTabBarVisualAt(nBarIndex)->m_hFont = hFont;
	}

	HFONT GetTabBarFont(int nBarIndex)
	{
		SECTabBarVisual* pTabBarVisual = GetTabBarVisualAt(nBarIndex);
		return pTabBarVisual->m_hFont;
	}

	void SetTabBarBrush(HBRUSH hBrush, int nBarIndex)
	{
		if(nBarIndex == -1)
			SetAttribHelper(hBrush, offsetof(SECTabBarVisual, m_hBrush));
		else
			GetTabBarVisualAt(nBarIndex)->m_hBrush = hBrush;	
	}

	HBRUSH GetTabBarBrush(int nBarIndex)
	{
		SECTabBarVisual* pTabBarVisual = GetTabBarVisualAt(nBarIndex);
		return pTabBarVisual->m_hBrush;
	}

	void SetTabBarBkColor(COLORREF clrBk, int nBarIndex)
	{
		if(nBarIndex == -1)
			SetAttribHelper(clrBk, offsetof(SECTabBarVisual, m_clrBkgnd));
		else
			GetTabBarVisualAt(nBarIndex)->m_clrBkgnd = clrBk;
	}

	COLORREF GetTabBarBkColor(int nBarIndex)
	{
		SECTabBarVisual* pTabBarVisual = GetTabBarVisualAt(nBarIndex);
		return pTabBarVisual->m_clrBkgnd;
	}

	void SetTabBarTextColor(COLORREF clrText, int nBarIndex)
	{
		if(nBarIndex == -1)
			SetAttribHelper(clrText, offsetof(SECTabBarVisual, m_clrText));
		else
			GetTabBarVisualAt(nBarIndex)->m_clrText = clrText;
	}

	COLORREF GetTabBarTextColor(int nBarIndex)
	{
		SECTabBarVisual* pVisual = GetTabBarVisualAt(nBarIndex);
		return pVisual->m_clrText;
	}

	HCURSOR SetTabBarCursor(HCURSOR hCursor)
	{
		HCURSOR hCurOld = m_hBarCursor;
		m_hBarCursor = hCursor;
		return hCurOld;
	}

	HCURSOR GetTabBarCursor()	{ return m_hBarCursor; }

	void SetLabelAlignment(UINT nAlign)
	{
		if(m_nLblAlgn != nAlign)
		{
			m_nLblAlgn = nAlign;
			NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);
		}
	}

	UINT GetLabelAlignment()	{ return m_nLblAlgn; }

	void DisableTabBar(int nIndex, BOOL bDisable)
	{
		SECTabBarVisual* pVisual = GetTabBarVisualAt(nIndex);
		if(pVisual->m_bDisabled != bDisable)
		{
			pVisual->m_bDisabled = bDisable;
			NotifyObservers(SECTabBarMessage::TABBAR_UPDATE, NULL);
		}
	}

	BOOL IsTabBarEnabled(int nIndex)	{ return !GetTabBarVisualAt(nIndex)->m_bDisabled; }

	int GetTabBarCount()	{ return m_pSysModel->GetTabBarCount(); }

	SECTabBarBase* GetTabBarAt(int nIndex)	{ return m_pSysModel->GetTabBarAt(nIndex); }
	
	int GetTabBarIndex(SECTabBarBase* pTabBar)	{ return m_pSysModel->GetTabBarIndex(pTabBar); }

	int GetActiveIndex()	{ return m_pSysModel->GetActiveIndex(); }	

protected:
	SECTabBarVisual* CreateTabBarVisual()
	{
		SECTabBarVisual* pVisual = new SECTabBarVisual;
		assert(pVisual);
		pVisual->m_hFont = HFONT(m_defFont);
		pVisual->m_clrBkgnd = ::GetSysColor(COLOR_BTNFACE);
		pVisual->m_clrText = ::GetSysColor(COLOR_BTNTEXT);
		pVisual->m_bDisabled = FALSE;
		
		return pVisual;
	}

	SECTabBarVisual* GetTabBarVisualAt(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_vectVisual.size());
		return m_vectVisual.at(nIndex);
	}

	void ConstructGDIObjects()
	{
		// Construct the default font object
		LOGFONT lf;
		lf.lfHeight = 15;
		lf.lfWidth = 0;
		lf.lfEscapement = 0;	
		lf.lfOrientation = 0;
		lf.lfWeight = FW_NORMAL;
		lf.lfItalic = FALSE;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = 0;
		lf.lfCharSet = ANSI_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
		_tcsncpy(lf.lfFaceName, _T("Arial"), LF_FACESIZE);
		m_defFont.CreateFontIndirect( &lf );

		// Load the 'hand' cursor from the SFL resources.
		m_hBarCursor = ::LoadCursor(hResInstance, MAKEINTRESOURCE(IDC_BARS_HAND));
		assert(m_hBarCursor);
	}

	void DestroyGDIObjects()
	{
		if(HFONT(m_defFont) != NULL)
			m_defFont.DeleteObject();

		::DestroyCursor(m_hBarCursor);
	}

	void NotifyObservers(UINT nMsg, SECTabBarBase* pTabBar, DWORD dwExtra = 0)
	{
		SECTabBarMessage* pMsg = new SECTabBarMessage(nMsg, pTabBar, dwExtra);
		pMsg->AddRef();
 		UpdateAllObservers(NULL, pMsg);
		pMsg->Release();
	}

	// SetAttribHelper iterates the tabbar collection and sets the value for the SECTabBarVisual 
	// attribute existing at the BYTE offset specified by the 'nOffset' parameter.
	template<class TAttribType>
	void SetAttribHelper(TAttribType attribVal, size_t nOffset)
	{
		int nSz = m_vectVisual.size();
		for(int i=0; i<nSz; i++)
			*(TAttribType*)((BYTE*)GetTabBarVisualAt(i) + nOffset) = attribVal;
	}

protected:
	SECTabBarSysModel* m_pSysModel;
	// Vector of SECTabBarVisual objects
	VectTabBarVisual m_vectVisual;

	CGDIFont	m_defFont;		// Default font for the tabbars.
	HCURSOR     m_hBarCursor;	// Default 'Hand' cursor for the tabbars
	UINT		m_nLblAlgn;
	DWORD		m_dwStyle;
	HWND        m_hWndNotify;
};

}; // namespace stingray


#endif // __TABBAR_MODEL_H__
