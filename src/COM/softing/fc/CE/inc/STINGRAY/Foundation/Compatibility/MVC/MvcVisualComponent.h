/////////////////////////////////////////////////////////////////////////////
// MvcVisualComponent.h : Declaration of MvcVisualComponent
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
// Description:  Declaration of MvcVisualComponent interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcVisualComponent.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcVisualComponent

#ifndef __COMPAT_MVCVISUALCOMPONENT_H__
#define __COMPAT_MVCVISUALCOMPONENT_H__

#ifndef _DECORATE_VISUAL_H_NAMES
#define _DECORATE_VISUAL_H_NAMES
#endif

// Includes

#ifndef __COMPAT_VISUAL_H__
#include "Foundation\Compatibility\Common\AGI\Visual.h"
#endif

using rw::IVisual;
using rw::IBounds2D;
using rw::IGraphicsContext;

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _MVCDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    MVC_DATAEXT
#endif //_MVCDLL

///////////////////////////////////////////////////////////////////////////
//
//  MvcAxis
//
//@enum MvcAxis
enum MvcAxis {
	MVCAXIS_X,		//@emem X axis
	MVCAXIS_Y		//@emem Y axis
};

///////////////////////////////////////////////////////////////////////////
//
//  MvcVisualComponent
//
//@doc MvcVisualComponent
//@mdata CRect | MvcVisualComponent | m_rc | Bounds of the visual component with respect to its container. This is
// the "external" perspective of the bounds (i.e. as the container sees it.)
//@xref <c MvcVisualComponent>


//@class
//
// The MvcVisualComponent class provides the basic drawing and positioning
// interfaces required by objects that will be drawn on a device context.
// MvcVisualComponent serves as the ultimate base class for all objects that
// can be rendered to a DC.   A visual component is a light-weight
// base class that gives you the ability to treat all renderable objects
// consistently.  It maintains size and position information, provides support
// for hit testing, and rendering. 
//
// A visual component is a very lightweight object and is the preferred
// base class when you need many, light visual components that are completely
// managed by a more intelligent container.  The container is responsible for
// telling the component when to draw, where to draw, how large it should be
// etc.  The visual component knows nothing about real windows, only drawing
// surfaces passed in via a CDC pointer.  Visual components will draw
// themselves whenever and whereever the container instructs.  This includes
// visual components positioned to partially or fully overlap.
//
// A visual component maintains a bounding rectangle that describes the
// area of its container the component occupies.  There is a important caveat
// here: the size and position of the component is measured in the logical
// unit metrics established by the container.  In other words, if the
// container is measuring its client area in units of inches, then a position
// of (2, 1) will mean the visual component is positioned 2 inches from the
// container's left edge and 1 inch from the containers top edge.  For this
// reason, the units of a visual component's bounds are known as external
// units, denoting that the bound's unit of measure are established
// externally. Also, notice that the subcomponent's origin is (typically)
// measured relative to its container's origin.  Therefore, moving the
// container will automatically move all subcomponents.
//
//@base public | IVisual
//@base public | IBounds2D
//
class MvcVisualComponent : public IVisual, public IBounds2D
{
	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//

public:

	// Construction/Destruction
	MvcVisualComponent();
	virtual ~MvcVisualComponent();

	//////////
	// Origin with respect to container (i.e. in container logical units)


	//@cmember
	/* Get the origin of this component relative to its container and in the container's units*/
	virtual CPoint GetOrigin() const;

	//@cmember
	/* Set the origin of this component relative to the container and in the container's units*/
	virtual CPoint SetOrigin(int x, int y);
	inline CPoint SetOrigin(const CPoint& ptOrg)
			{ return SetOrigin(ptOrg.x, ptOrg.y); }

	//@cmember
	/* Offset the origin of this component by specified deltas*/
	virtual CPoint OffsetOrigin(int xOff, int yOff);

	//@cmember
	/* Move the origin of this component by specified deltas*/
	virtual CPoint MoveOrigin(int xOff,int yOff);

	/////////
	// Size with respect to container (i.e. in container logical units)

	//@cmember
	/* Get the width and height of this component in the container's logical units*/
	virtual CSize GetSize() const;

	//@cmember
	/* Set the width and height of this component in the container's logical units*/
	virtual CSize SetSize(int cx, int cy);
	inline CSize SetSize(const CSize& sz)
			{ return SetSize(sz.cx, sz.cy); }

	//@cmember
	/* Scale the width and height of this component*/
	virtual CSize ScaleBounds(int xNum, int xDenom, int yNum, int yDenom, BOOL bAnchorCenter = FALSE);

	//////////
	// Rectangle & helpers

	//@cmember
	/* Get the component's bounding rectangle in the logical units defined by the container*/
	virtual CRect GetBounds() const;

	//@cmember
	/* Get the origin of this component*/
	inline CPoint TopLeft() const;

	//@cmember
	/* Get the upper, right edge of the component*/
	inline CPoint TopRight() const;

	//@cmember
	/* Get the lower, left edge of the component*/
	inline CPoint BottomLeft() const;

	//@cmember
	/* Get the lower, right edge of the component*/
	inline CPoint BottomRight() const;

	//@cmember
	/* Normalize the bounding rectangle*/
	inline void NormalizeBounds();

	/////////
	// Margins

	//@cmember
	/* Get the margins around the bounds of this visual component. */
	virtual void GetMargins(CRect& rcMargins);

	/////////
	// Hit Testing

	//@cmember
	/* Does the bounding rectangle of this component contain the specified point?*/
	BOOL ContainsPoint(const CPoint& pt) const;

	//@cmember
	/* Does the bounding rectangle of this component overlap the specified rectangle?*/
	BOOL Intersects(const CRect& rc) const;

	/////////
	// Printing

	//@cmember
	/* Print to a string a description of this visual component*/
	virtual void Sprint(CString& strDesc);

	/////////
	// IUnknown interface

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef();

	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release();

	/////////
	// IVisual interface

	//@cmember
	/* Renders the component to a device context. */
	virtual void Draw_(IGraphicsContext* pIGC);

	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//

public:

	///////////
	// Rendering

	//@cmember
	/* Renders the component to a device context. */
	virtual void Draw(CDC* pDC);

	// Initial Update

	//@cmember
	/* The handler for the initial update of this visual component*/
	virtual void OnInitialUpdate();

public:

	//@cmember
	/* Readies the device context for subsequent rendering or logical to device coordinate mapping. */
	virtual void OnPrepareDC(CDC* pDC);

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//

protected:

	// Attributes

	//@cmember
	/* Bounds of the visual component with respect to its container. This is*/
	// the "external" perspective of the bounds (i.e. as the container sees it.)
	CRect m_rc;
};

//////////////////////////////////////////////////////////////////////////////
// Inline functions

//@doc MvcVisualComponent
//@mfunc Get the origin of this component
//@rdesc CPoint
inline CPoint MvcVisualComponent::TopLeft() const
{
	return GetOrigin();
}

//@doc MvcVisualComponent
//@mfunc Get the upper, right edge of the component
//@rdesc CPoint 
inline CPoint MvcVisualComponent::TopRight() const
{
	CPoint pt = GetOrigin();
	pt.Offset(GetSize().cx, 0);
	return pt;
}

//@doc MvcVisualComponent
//@mfunc Get the lower, left edge of the component
//@rdesc CPoint 
inline CPoint MvcVisualComponent::BottomLeft() const
{
	CPoint pt = GetOrigin();
	pt.Offset(0, GetSize().cy);
	return pt;
}

//@doc MvcVisualComponent
//@mfunc Get the lower, right edge of the component
//@rdesc CPoint 
inline CPoint MvcVisualComponent::BottomRight() const
{
	CPoint pt = GetOrigin();
	pt.Offset(GetSize());
	return pt;
}

//@doc MvcVisualComponent
//@mfunc Normalize the bounding rectangle
//@rdesc void
inline void MvcVisualComponent::NormalizeBounds()
{
	m_rc.NormalizeRect();
}

///////////////////////////////////////////////////////////////////////////


//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __COMPAT_MVCVISUALCOMPONENT_H__

