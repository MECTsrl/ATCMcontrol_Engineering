/////////////////////////////////////////////////////////////////////////////
// MvcWrapper.h : Declaration of MvcWrapper_T
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Dean Hallman
// Description:  Declaration of MvcWrapper_T template class.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MvcWrapper.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $
#ifndef __MVCWRAPPER_H__
#define __MVCWRAPPER_H__

#pragma once

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc MvcWrapper_T
//@mfunc void  | MvcWrapper_T |  GetMargins | Retrieves the margins around the perimeter of the component
//@parm CRect& | rcMargins | Receives the current margin rectangle
//@xref <c MvcWrapper_T>

//@doc MvcWrapper_T
//@class MvcWrapper_T | A wrapper in MVC is way of nesting visual parts.  The wrapper
// class is equivalent to the Decorator design pattern.
//@tcarg class | base_t | The class of the visual part to derive from
template <class base_t>
class MvcWrapper_T : public base_t
{
// Constructor(s) & destructor
public:
	MvcWrapper_T() 
	{
		m_rcMargins.SetRectEmpty();
	}

// Attributes
protected:
	CRect m_rcMargins;		// margin offsets

// Operations
public:
	//@cmember
	/* Retrieves the margins around the perimeter of the component*/
	virtual void GetMargins(CRect& rcMargins);

	//@cmember
	/* Sets the margins around the perimeter of the component*/
	virtual void SetMargins(const CRect& rcMargins);

// Overrides
public:

	// Device Origin
	virtual CPoint SetOrigin(int x, int y);

	// Device Size
	virtual CSize SetSize(int cx, int cy);

	inline CSize SetSize(const CSize& sz)
	{
		return SetSize(sz.cx, sz.cy);
	}
};

template<class base_t>
//@doc MvcWrapper_T
//@mfunc Retrieves the margins around the perimeter of the component
//@rdesc void 
//@parm CRect& | rcMargins | Output parameter to receive margins
void MvcWrapper_T<base_t>::GetMargins(CRect& rcMargins)
{
	base_t::GetMargins(rcMargins);
	rcMargins.left += m_rcMargins.left;
	rcMargins.top += m_rcMargins.top;
	rcMargins.right += m_rcMargins.right;
	rcMargins.bottom += m_rcMargins.bottom;
}

template<class base_t>
//@doc MvcWrapper_T
//@mfunc Sets the margins around the perimeter of the component
//@rdesc void 
//@parm const CRect& | rcMargins | The new margin rectangle
void MvcWrapper_T<base_t>::SetMargins(const CRect& rcMargins)
{
	CRect rcBaseMargins;
	base_t::GetMargins(rcBaseMargins);
	CRect rc(GetBounds());
	rc.InflateRect(&rcBaseMargins);

	CRect delta;

	// Compute delta between old margin and new
	delta.left = rcMargins.left - m_rcMargins.left;
	delta.top = rcMargins.top - m_rcMargins.top;
	delta.right = rcMargins.right - m_rcMargins.right;
	delta.bottom = rcMargins.bottom - m_rcMargins.bottom;

	// Update the bounds to account for these new margins
	base_t::SetOrigin(rc.left + delta.left, rc.top + delta.top);
	base_t::SetSize(rc.Width() - (delta.left + delta.right),
					rc.Height() - (delta.top + delta.bottom));

	m_rcMargins = rcMargins;
}

template<class base_t>
CPoint MvcWrapper_T<base_t>::SetOrigin(int x, int y)
{
	// set the viewport rect accounting for margins
	return base_t::SetOrigin(x+m_rcMargins.left,y+m_rcMargins.top);
}

template<class base_t>
CSize MvcWrapper_T<base_t>::SetSize(int cx, int cy)
{
	return base_t::SetSize(cx - m_rcMargins.left - m_rcMargins.right,
						   cy - m_rcMargins.top - m_rcMargins.bottom);
}

};	// namespace stingray::foundation
};  // namespace stingray

#endif // #ifndef __MVCWRAPPER_H__