/////////////////////////////////////////////////////////////////////////////
// PropEditorBase.h : Property list class.
//
// Stingray Software Extension Classes
// Copyright (C) 1999 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Brad King and Jeff Boenig
// Description:  Property editor base class.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Properties/PropEditorBase.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef __PROPEDITORBASE_H__
#define __PROPEDITORBASE_H__

#include <Foundation\Properties\PropIFace.h>
#include <Foundation\Util\Convert.h>


namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// CPropertyEditorBase
//
// this class is not a list control, but implements methods that define default
// property browse behavior. Since it does not have the list control HWND as a member,
// it can be used as a base class for a list control, the containing dialog, or any
// other relevant class where it is convenient to handle the list control.

class CPropertyEditorBase
{
// Constructors/destructor
public:
	CPropertyEditorBase() :
		m_nButtonWidth(0),
		m_bShowingButton(TRUE)
	{
	}

// Attributes
public:
	int m_nButtonWidth;
	BOOL m_bShowingButton;
	typedef SmartPtr<IProperty> PropPtr;
	std::vector<PropPtr> m_properties;

// Implementation
protected:
	virtual IPropertyContainer* GetPropertyContainer() = 0;
	virtual HWND GetListBox() = 0;
	virtual HWND GetEditControl() = 0;
	virtual HWND GetMoreButton() = 0;

	virtual CSize GetListBoxOffset()
	{
		return CSize(2,1);
	}

	//@cmember
	/* Creates a two column list in report mode. */
	void InitListControl()
	{
		HWND hwnd = GetListBox();
		ASSERT(hwnd);
		ASSERT(::IsWindow(hwnd));

		// set the style to report mode
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | WS_TABSTOP
						| LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SORTASCENDING;
		::SetWindowLong(hwnd, GWL_STYLE, dwStyle); 

		ListView_SetExtendedListViewStyle(hwnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		// insert two columns for property name and value
		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM; 
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = 10;
		lvc.iSubItem = 0;
		ListView_InsertColumn(hwnd, 0, &lvc);

		lvc.cx = 10;
		lvc.iSubItem = 1;
		ListView_InsertColumn(hwnd, 1, &lvc);
		::SetWindowPos(hwnd, NULL, 0, 0, 0, 0, 
							SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	}

	void ResetColumnWidths()
	{
		HWND hwndList = GetListBox();

		// fits column 0 to its text,
		// column 1 uses the remaining client width

		if(!hwndList || !::IsWindow(hwndList))
			return;
		::UpdateWindow(hwndList);
		ListView_SetColumnWidth(hwndList, 0, LVSCW_AUTOSIZE);
		ListView_SetColumnWidth(hwndList, 1, LVSCW_AUTOSIZE_USEHEADER);
	}

	//@cmember
	/* removes all items from the list. note: IProperty objects are not released */
	HRESULT RemoveAll()
	{
		HWND hwndList = GetListBox();

		if(!hwndList)
			return S_OK;

		if(!::IsWindow(hwndList))
			return S_OK;

		ListView_DeleteAllItems(hwndList);

		return S_OK;
	}

	HRESULT AddItemToList(IProperty* pProp)//, IPropertyContainer* pContainer)
	{
		// creates a new list control item row, with an lParam that
		// points to an IOtlProperty.
		// note: text is only used to set the initial column widths using autosize,
		// the items are all custom draw.
		IPropertyContainer *pContainer = GetPropertyContainer();

		ASSERT(pProp != NULL && pContainer != NULL);

		HWND hwndList = GetListBox();
		_ASSERTE(hwndList);
		_ASSERTE(pProp);
		_ASSERTE(pContainer);

		// don't show nested styles 
		DWORD style = pProp->GetStyleFlags();
		style &= (~PROP_CONTAINER);

		if (style)
		{
			// read the name and value from the property
			BSTR bstrName = NULL;
			pProp->GetName(bstrName);
		

			USES_CONVERSION;
			LVITEM lvItem;
			lvItem.mask = LVIF_TEXT | LVIF_PARAM;
			lvItem.iSubItem = 0;
			lvItem.lParam = reinterpret_cast<DWORD>(pProp);
			lvItem.pszText = OLE2T(bstrName);
			::SysFreeString(bstrName);
			int nItem = ListView_InsertItem(hwndList, &lvItem);
			if(nItem == -1)
				return E_FAIL;

			BSTR bstrVal = NULL;

			// TBD what happens if we don't get a value associated with the
			// property?
			_ASSERTE(pContainer->GetPropertyString(pProp->GetId(), bstrVal));

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = nItem;
			lvItem.iSubItem = 1;
			lvItem.pszText = OLE2T(bstrVal);
			::SysFreeString(bstrVal);
			if(!ListView_SetItem(hwndList, &lvItem))
				return E_FAIL;
		}

		// Recurse this bad boy down to the last container!
		// Check if current property is a container
		IPropertyContainer* subContainer = NULL;
		if (pContainer->GetPropertyContainer(pProp->GetId(), &subContainer))
		{
			// TBD: fill the list with subproperties from the list if style allows it
			if (subContainer != NULL)
			{
				const int propCount = subContainer->GetPropertyCount();
				for(int subIdx = 0; subIdx < propCount; ++subIdx)
				{

					IProperty *pSubProp = subContainer->GetPropertyAt(subIdx);
					if (pSubProp != NULL)
					{
						// The copy should have the id of the subprop + it's containing
						// property so it may be located in the base container.
						IProperty* pCopy = CopyProperty(pSubProp, pProp->GetId());
						if (pCopy != NULL)
						{
							// Adjust id of sub property to use id of it's 
							// property it is nested in. This allows retrieval
							// using the top most container.
							HRESULT hr = AddItemToList(pCopy);//, pContainer);

							if(FAILED(hr))
							{
								pCopy->Release();
								return hr;
							}
							else
							{
								PropPtr spCopy(pCopy);
								m_properties.push_back(spCopy);
							}
							
							pCopy->Release();
						}
					}
				}
			}
		}

		return S_OK;
	}

	HRESULT LoadListControl()
	{
		IPropertyContainer* pContainer = GetPropertyContainer();
		_ASSERTE(pContainer);

		// loads the list control from a populated CPropBrowser
		HWND hwndList = GetListBox();
		_ASSERTE(hwndList);
		_ASSERTE(pContainer);
		HRESULT hr = S_OK;
		int nCount = pContainer->GetPropertyCount();
		for(int i = 0; i < nCount; i++)
		{
			IProperty* pProp = pContainer->GetPropertyAt(i);
			
			IProperty* pCopy = CopyProperty(pProp);
			if (pCopy != NULL)
			{
				hr = AddItemToList(pCopy);//, pContainer);

				if(FAILED(hr))
				{
					pCopy->Release();
					return hr;
				}
				else
				{
					PropPtr spCopy(pCopy);
					m_properties.push_back(spCopy);
				}
				
				pCopy->Release();
			}
			
		}

		return hr;
	}

	// Caller is responsible for deallocating copy.
	// baseId allows you to adjust the id of the property for
	// subproperties in a container.
	IProperty* CopyProperty(IProperty* pProp, int baseId = 0)
	{
		ASSERT(pProp);

		CProperty* pCopy = NULL;
		
		if (pProp)
		{
			BSTR name = NULL;
			BSTR desc = NULL;

			pProp->GetName(name);
			pProp->GetDescription(desc);

			USES_CONVERSION;
			pCopy = new CProperty(baseId + pProp->GetId(), OLE2T(name), OLE2T(desc)); 
			if (pCopy)
			{
				pCopy->SetStyleFlags(pProp->GetStyleFlags());
				pCopy->SetVariantType(pProp->GetVariantType());
				pCopy->SetCategory(pProp->GetCategory());
				pCopy->SetEnumeration(pProp->GetEnumeration());
				pCopy->AddRef();
			}

			// Clean up resources
			::SysFreeString(name);
			::SysFreeString(desc);
		}

		return pCopy;
	}

	HRESULT DoCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
	{
		IPropertyContainer* pContainer = GetPropertyContainer();
		_ASSERTE(pContainer);

		// Track the button when the list is repainted.
		// It's an alternative to creating a derived list view class and handling scroll
		// messages.
		static COLORREF crHL;
		static COLORREF crHLText;
		static COLORREF crWND;
		static COLORREF crWNDText;

		NMLVCUSTOMDRAW* lpcd = (NMLVCUSTOMDRAW*)pNMHDR;
		LRESULT lr = CDRF_DODEFAULT;
		if(lpcd->nmcd.dwDrawStage == CDDS_PREPAINT)
		{
			// cache some system colors for the rest of the drawing stages
			crHL = ::GetSysColor(COLOR_HIGHLIGHT);
			crHLText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			crWND = ::GetSysColor(COLOR_WINDOW);
			crWNDText = ::GetSysColor(COLOR_WINDOWTEXT);

			// color subitems are hand drawn
			lr = CDRF_NOTIFYITEMDRAW;
			// use postpaint to move the drop button or edit control
			lr |= CDRF_NOTIFYPOSTPAINT;
		}
		else if(lpcd->nmcd.dwDrawStage == CDDS_ITEMPREERASE)
		{
			lr = CDRF_SKIPDEFAULT;
		}
		else if(lpcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		{
			// first get the rects
			RECT rcSub0;
			ListView_GetSubItemRect(lpcd->nmcd.hdr.hwndFrom, lpcd->nmcd.dwItemSpec, 0, LVIR_BOUNDS, &rcSub0);
			RECT rcSub1;
			ListView_GetSubItemRect(lpcd->nmcd.hdr.hwndFrom, lpcd->nmcd.dwItemSpec, 1, LVIR_BOUNDS, &rcSub1);

			// fill the backgrounds, based on focus
			COLORREF crBk;
			COLORREF crText;
			if(lpcd->nmcd.uItemState & CDIS_FOCUS)
			{
				crBk = crHL;
				crText = crHLText;
			}
			else
			{
				crBk = crWND;
				crText = crWNDText;
			}
			CBrush br(crBk);
			::FillRect(lpcd->nmcd.hdc, &rcSub0, br);
			CBrush brBk2(crWND);
			::FillRect(lpcd->nmcd.hdc, &rcSub1, brBk2);

			// adjust rects for a small margin
			rcSub0.left += GetListBoxOffset().cx;
			rcSub1.left += GetListBoxOffset().cx;

			// item rect appears to include the bottom grid line
			rcSub0.bottom -= GetListBoxOffset().cy;
			rcSub1.bottom -= GetListBoxOffset().cy;
			
			// get a pointer to the property for this item
			IProperty* pProp = (IProperty*)lpcd->nmcd.lItemlParam;

			_ASSERTE(pProp);

			// get the name and value
			BSTR bstrName	= NULL;
			BSTR bstrVal	= NULL;
			pProp->GetName(bstrName);

			pContainer->GetPropertyString(pProp->GetId(), bstrVal);

			// name (subitem 0)
			USES_CONVERSION;
			COLORREF bkColorPrev = ::SetBkColor(lpcd->nmcd.hdc, crBk);
			COLORREF txtColorPrev = ::SetTextColor(lpcd->nmcd.hdc, crText);
			::DrawText(lpcd->nmcd.hdc, OLE2T(bstrName), -1, &rcSub0, DT_VCENTER | DT_SINGLELINE);
			::SetBkColor(lpcd->nmcd.hdc, bkColorPrev);
			::SetTextColor(lpcd->nmcd.hdc, txtColorPrev);
			
			// value (subitem 1)
			DWORD dwStyle = pProp->GetStyleFlags();
			// use this switch for any extra non-text drawing
			if(dwStyle & PROP_COLOR)
			{
				// draw a colored square on the left edge
				RECT rcColorBox;
				rcColorBox.left = rcSub1.left;
				rcColorBox.top = rcSub1.top + 1;
				rcColorBox.bottom = rcSub1.bottom - 1;
				rcColorBox.right = rcColorBox.left + (rcColorBox.bottom - rcColorBox.top);
				CComVariant var;
				pContainer->GetPropertyValue(pProp->GetId(), var);
				COLORREF cr;
				::OleTranslateColor(var.lVal, NULL, &cr);
				CBrush brImage(cr);
				::FillRect(lpcd->nmcd.hdc, &rcColorBox, brImage);
				CBrush brBlack;
				brBlack.CreateStockObject(BLACK_BRUSH);
				::FrameRect(lpcd->nmcd.hdc, &rcColorBox, brBlack);
				// adjust the text box to the right of the square
				rcSub1.left = rcColorBox.right + 2;
			}

			// draw the text for subitem 1
			::DrawText(lpcd->nmcd.hdc, OLE2T(bstrVal), -1, &rcSub1, DT_VCENTER | DT_SINGLELINE);

			::SysFreeString(bstrName);
			::SysFreeString(bstrVal);

			lr = CDRF_SKIPDEFAULT;

		}
		else if(lpcd->nmcd.dwDrawStage == CDDS_POSTPAINT)
		{
			// the button has to be moved with the subitem, but we don't get a subitem redraw
			// when the list scrolls. So, move the button after the entire control has repainted.

			// get the focused item
			int nItem = ListView_GetNextItem(lpcd->nmcd.hdr.hwndFrom, -1, LVNI_FOCUSED);
			if(nItem != -1)
			{
				RECT rcClient;
				::GetClientRect(lpcd->nmcd.hdr.hwndFrom, &rcClient);
				RECT rcItem;
				ListView_GetSubItemRect(lpcd->nmcd.hdr.hwndFrom, nItem, 1, LVIR_BOUNDS, &rcItem);
				if(rcItem.right > rcClient.right)
					rcItem.right = rcClient.right;

				rcItem.bottom -= GetListBoxOffset().cy;

				if(m_bShowingButton && ::IsWindow(GetMoreButton()))
				{
					// move the button to the right edge of subitem 1 for the focused item
					rcItem.left = rcItem.right - m_nButtonWidth;
					::SetWindowPos(GetMoreButton(), NULL, rcItem.left, rcItem.top, 0, 0,
													SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
				}
				else if(::IsWindow(GetEditControl()))
				{
					rcItem.left += GetListBoxOffset().cx;
					::SetWindowPos(GetEditControl(), NULL, rcItem.left, rcItem.top,
													rcItem.right - rcItem.left,
													rcItem.bottom- rcItem.top,
													SWP_NOZORDER | SWP_NOACTIVATE);
	
				}

			}
		}

		*pResult = lr;
		return S_OK;
	}
};


}; // namespace stingray::foundation
}; // namespace stingray

#endif  // #ifndef __PROPEDITORBASE_H__
