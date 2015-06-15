/////////////////////////////////////////////////////////////////////////////
// MvcVisualComponent.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Dean Hallman & Jeff Boenig
// Description:  SFL MVC: Declaration of Visual Component class
//

#pragma once

#ifndef __MVCVISUALCOMPONENT_H__
#define __MVCVISUALCOMPONENT_H__

#include <foundation\StructuredGraphics\Visual.h>

namespace stingray {
namespace foundation {

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
// CMvcVisualComponent

//@doc CMvcVisualComponent
//@mdata CRect | CMvcVisualComponent | m_rc | Bounds of the visual component with respect to its container. This is
// the "external" perspective of the bounds (i.e. as the container sees it.)
//@xref <c CMvcVisualComponent>

//@class CMvcVisualComponent |
//
// The CMvcVisualComponent class provides the basic drawing and positioning
// interfaces required by objects that will be drawn on a device context.
// CMvcVisualComponent serves as the ultimate base class for all objects that
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
class CMvcVisualComponent : public IVisual, public IBounds2D
{
// Constructors/destructor
public:

	CMvcVisualComponent()
	{
		m_rc.SetRectEmpty();
	}

	virtual ~CMvcVisualComponent()
	{
	}

// Attributes
protected:
	//@cmember
	/* Bounds of the visual component with respect to its container. This is*/
	// the "external" perspective of the bounds (i.e. as the container sees it.)
	CRect m_rc;

// Operations
public:

	//////////
	// Origin with respect to container (i.e. in container logical units)

	//@cmember,mfunc Get the origin of this component relative to its container and in the container's units
	//@@rdesc CPoint The origin of the component
	//@@comm
	//  Returns the origin of the visual component in the parent's terms.  In other words,
	//  a visual component may be contained by another visual component that has its own
	//  coordinate system and logical to pixel unit mapping.  With or without logical to
	//  device unit scaling, the position of this visual component is in logical units
	//  as defined by its parent.
	//@@xref <mf CMvcVisualComponent::SetOrigin>
	//@@end
	/* Get the origin of this component relative to its container and in the container's units*/
	virtual CPoint GetOrigin() const
	{
		return m_rc.TopLeft();
	}

	//@cmember,mfunc Set the origin of this component relative to its container and in the container's units
	//@@syntax SetOrigin(int x, int y)
	//@@syntax SetOrigin(const CPoint& point)
	//@@rdesc CPoint The previous origin 
	//@@parm int | x |
	//@@parm int | y |
	//@@comm
	//  Call this function to set or move a component's origin in the parent's terms.
	//  In other words, a visual component may be contained by another visual component
	//  that has its own coordinate system and logical to pixel unit mapping.  With or
	//  without logical to device unit scaling, the position of this visual component is
	//  in logical units as defined by its parent.
	//@@xref <mf CMvcVisualComponent::GetOrigin>
	//@@end
	/* Set the origin of this component relative to the container and in the container's units*/
	virtual CPoint SetOrigin(int x, int y)
	{
		CPoint ptOld = GetOrigin();
		m_rc.left = x;
		m_rc.top = y;
		return ptOld;
	}

	inline CPoint SetOrigin(const CPoint& ptOrg)
	{ 
		return SetOrigin(ptOrg.x, ptOrg.y); 
	}

	//@cmember,mfunc Offsets the component's origin by a delta
	//@@rdesc CPoint The new point after the offset
	//@@parm int | xOff | The x offset
	//@@parm int | yOff | The y offset
	//@@comm
	//  Call this function to move the origin of the component by some delta in the
	//  x and y directions.
	//@@xref <mf CMvcVisualComponent::SetOrigin>
	//@@end
	/* Offset the origin of this component by specified deltas*/
	virtual CPoint OffsetOrigin(int xOff, int yOff)
	{
		return SetOrigin(m_rc.left + xOff,
					m_rc.top + yOff);
	}

	//@cmember,mfunc Move the component's origin by a delta
	//@@rdesc CPoint The new point after the move
	//@@parm int | xOff | The x offset
	//@@parm int | yOff | The y offset
	//@@comm
	//  Call this function to move the origin of the component by some delta in the
	//  x and y directions. Unlike OffsetOrigin, this function will move the right
	//  and bottom components of the bounding rect by the specified offset as well.
	//@@xref <mf CMvcVisualComponent::OffsetOrigin>
	//@@end
	/* Move the origin of this component by specified deltas*/
	virtual CPoint MoveOrigin(int xOff,int yOff)
	{
		m_rc.OffsetRect(xOff,yOff);
		return CPoint(m_rc.left,m_rc.top);
	}

	/////////
	// Size with respect to container (i.e. in container logical units)

	//@cmember,mfunc Returns the width and height of the component
	//@@rdesc CSize The size of the component
	//@@comm
	//  Returns the size of the component in the units defined by the parent.
	//@@xref <mf CMvcVisualComponent::SetSize>
	//@@end
	/* Get the width and height of this component in the container's logical units*/
	virtual CSize GetSize() const
	{
		return m_rc.Size();
	}

	//@cmember,mfunc Sets the width and height of the component
	//@@rdesc CSize The previous size of the component
	//@@syntax SetSize(int cx, int cy)
	//@@syntax SetSize(const CSize& sz)
	//@@parm int | cx | 
	//@@parm int | cy |
	//@@comm
	// Sets the size of the component's bounding rectangle in
	// units defined by the parent. The origin is unchanged.
	//@@xref <mf MvcVisualComponent::GetSize>
	//@@end
	/* Set the width and height of this component in the container's logical units*/
	virtual CSize SetSize(int cx, int cy)
	{
		CSize szOld = GetSize();
		m_rc.right = m_rc.left + cx;
		m_rc.bottom = m_rc.top + cy;
		return szOld;
	}

	inline CSize SetSize(const CSize& sz)
	{ 
		return SetSize(sz.cx, sz.cy); 
	}

	//@cmember,mfunc Scales the bounds of the visual component
	//@@rdesc CSize The new size of the component after the scaling is done
	//@@parm int | xNum |
	//@@parm int | xDenom |
	//@@parm int | yNum |
	//@@parm int | yDenom |
	//@@parmopt BOOL | bAnchorCenter | FALSE |
	//@@comm
	//  This function scales the size of the visual component.  The formula
	//  is very simple:
	//
	//        new_width  = (current_width * xNum) / xDenom;
	//        new_height = (current_height * yNum) / yDenom;
	//
	//  To double the width of the component, simply pass the value (int)2
	//  as the xNum parameter and (int)1 as the xDenom parameter.  To half
	//  its width, simply reverse the previous order.  The bAnchorCenter
	//  parameter is useful if you want to zoom about a centerpoint.
	//
	//@@xref <mf MvcVisualComponent::SetSize>
	//@@end
	/* Scale the width and height of this component*/
	virtual CSize ScaleBounds(int xNum, int xDenom, int yNum, int yDenom, bool bAnchorCenter = false)
	{
		CSize szScaled;

		szScaled.cx = ( m_rc.Width() * xNum ) / xDenom;
		szScaled.cy = ( m_rc.Height() * yNum ) / yDenom;

		if (bAnchorCenter) {
			// Offset the origin so our center point stays put
			CSize szDelta;
			szDelta.cx = szScaled.cx - m_rc.Width();
			szDelta.cy = szScaled.cy - m_rc.Height();
			OffsetOrigin(-szDelta.cx/2, -szDelta.cy/2);
		}

		return SetSize(szScaled);
	}

	//////////
	// Rectangle & helpers

	//@cmember,mfunc Get the component's bounding rectangle in the logical units defined by the container
	//@@rdesc CRect 
	//@@end
	/* Get the component's bounding rectangle in the logical units defined by the container*/
	virtual CRect GetBounds() const
	{
		return m_rc;
	}

	//@cmember,mfunc Get the origin of this component
	//@@rdesc CPoint
	//@@end
	/* Get the origin of this component*/
	inline CPoint TopLeft() const
	{
		return GetOrigin();
	}

	//@cmember,mfunc Get the upper, right edge of the component
	//@@rdesc CPoint 
	//@@end
	/* Get the upper, right edge of the component*/
	inline CPoint TopRight() const
	{
		CPoint pt = GetOrigin();
		pt.Offset(GetSize().cx, 0);
		return pt;
	}

	//@cmember,mfunc Get the lower, left edge of the component
	//@@rdesc CPoint 
	//@@end
	/* Get the lower, left edge of the component*/
	inline CPoint BottomLeft() const
	{
		CPoint pt = GetOrigin();
		pt.Offset(0, GetSize().cy);
		return pt;
	}

	//@cmember,mfunc Get the lower, right edge of the component
	//@@rdesc CPoint 
	//@@end
	/* Get the lower, right edge of the component*/
	inline CPoint BottomRight() const
	{
		CPoint pt = GetOrigin();
		pt.Offset(GetSize());
		return pt;
	}

	/////////
	// Margins

	//@cmember,mfunc Get the margins around the bounds of this visual component.
	//@@rdesc void
	//@@parm Output parameter to receive margin values
	//@@end
	/* Get the margins around the bounds of this visual component. */
	virtual void GetMargins(CRect& rcMargins)
	{
		rcMargins = CRect(0,0,0,0);
	}

	/////////
	// Hit Testing

	//@cmember,mfunc Tests whether a point lies within the component's bounds
	//@@rdesc BOOL Returns TRUE if the point lies inside the component; otherwise FALSE
	//@@parm const CPoint& | pt | The point to test
	//@@comm
	//  This function tests whether the specified point lies within the component.
	//@@xref <mf CMvcVisualComponent::Intersects>
	//@@end
	/* Does the bounding rectangle of this component contain the specified point?*/
	BOOL ContainsPoint(const CPoint& pt) const
	{
		return GetBounds().PtInRect(pt);
	}

	//@cmember,mfunc Tests whether the rectangle intersects the component's bounds
	//@@rdesc BOOL Returns TRUE if the rectangle overlaps the component; otherwise FALSE
	//@@parm const CRect& | rc | The rect to test
	//@@comm
	//  This function tests whether the specified rectangle intersects the
	//  component's bounds.
	//@@xref <mf CMvcVisualComponent::ContainsPoint>
	//@@end
	/* Does the bounding rectangle of this component overlap the specified rectangle?*/
	BOOL Intersects(const CRect& rc) const
	{
		CRect rcIntersect;
		return rcIntersect.IntersectRect(GetBounds(), rc);
	}

	//@cmember,mfunc Print a description of this visual part to the specified string
	//@@rdesc void
	//@@parm CString& | strDesc | A string to hold trhe return value
	//@@comm
	// This function just prints an identifier or description of this object
	// to the specified string.  You should override this function and customize
	// the printing.
	//@@end
	/* Print to a string a description of this visual component*/
	virtual void Sprint(CString& strDesc)
	{
		strDesc; // UNUSED
	}

	//@cmember,mfunc
	// Draw the component to a device context.
	//@@rdesc void
	//@@parm CDC* | pDC | Pointer to a device context.
	//@@end
	/* Draw the object to a device context*/
	virtual void Draw(CDC* pDC)
	{
		pDC;
	}

	//@cmember,mfunc
	// Prepare device context for rendering.
	//@@rdesc void
	//@@parm CDC* | pDC | Pointer to a device context.
	//@@end
	/* Prepare device context for rendering*/
	virtual void OnPrepareDC(CDC* pDC)
	{
		pDC;
	}

	//@cmember,mfunc
	// Cleanup device context after rendering.
	//@@rdesc void
	//@@parm CDC* | pDC | Pointer to a device context.
	//@@end
	/* Cleanup device context after rendering*/
	virtual void OnCleanupDC(CDC* pDC)
	{
		pDC;
	}

	//@cmember,mfunc Initializes the visual component
	//@@rdesc void
	//@@comm
	//  This function is called just before the visual component is drawn
	//  for the first time.  Derived classes should override this member
	//  and perform any applicable initialization.
	//@@end
	/* The handler for the initial update of this visual component*/
	virtual void OnInitialUpdate()
	{
	}

	//@cmember,mfunc Normalize the bounding rectangle
	//@@rdesc void
	//@@end
	/* Normalize the bounding rectangle */
	inline void NormalizeBounds()
	{
		m_rc.NormalizeRect();
	}

	// The GUID map implements the QueryGuid method
	BEGIN_GUID_MAP(CMvcVisualComponent)
		GUID_ENTRY(IVisual)
		GUID_ENTRY(IBounds2D)
		GUID_ENTRY(ISize2D)
		GUID_ENTRY2(IQueryGuid,ISize2D)
		GUID_ENTRY2(IRefCount,ISize2D)
	END_GUID_MAP

	//@cmember,mfunc
	// Add a reference to this object.
	//@@rdesc New reference count value.
	//@@end
	/* Add a reference to this object*/
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	//@cmember,mfunc
	// Release a reference to this object.
	//@@rdesc New reference count value.
	//@@end
	/* Release a reference to this object*/
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCVISUALCOMPONENT_H__
