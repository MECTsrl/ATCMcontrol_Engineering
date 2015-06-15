/////////////////////////////////////////////////////////////////////////////
// Visual.h : CPP Interfaces for positioning and rendering objects.
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
// Author:       Jeff Boenig
// Description:  CPP Interfaces for positioning and rendering objects.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/Agi/Visual.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#ifndef __COMPAT_VISUAL_H__
#define __COMPAT_VISUAL_H__

#include "Foundation\Compatibility\Common\CppIface.h"
#include "Foundation\Compatibility\Common\CmnGuids.h"

//
// Map names with _ to friendly names if decorated
// names not required.
//
#if !defined(_DECORATE_VISUAL_H_NAMES)
#define Draw_ Draw
#endif

namespace rw
{

///////////////////////////////////////////////////////////////////////////
//@doc IGraphicsContext
//
//@class IGraphicsContext | Interface to a graphics context. Currently
// undefined.
//
class IGraphicsContext;

///////////////////////////////////////////////////////////////////////////
//@doc ISize2D
//
//@class ISize2D | Interface for manipulating the width and height of a
// two-dimensional object.
//
//@mfunc void | ISize2D | GetSize | Get the width and height of an object.
//@rdesc Returns a size object.

//@mfunc void | ISize2D | SetSize | Set the width and height of an object.
//@rdesc Previous size.
//@parm int | cx | Width to set object to.
//@parm int | cy | Height to set object to.
//
//@base public | IUnknown
//
class __declspec(uuid("3D80F542-D51B-11d2-8CAB-0010A4F36466"))ISize2D : public IUnknown
{
public:
	//@cmember
	/* Get the width and height of an object. */
	virtual CSize GetSize() const = 0;
	//@cmember
	/* Set the width and height of an object. */
	virtual CSize SetSize(int cx, int cy) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc IBounds2D
//
//@class IBounds2D | Interface for manipulating the bounding rectangle of
// a two-dimensional object.
//
//@mfunc CRect | IBounds2D | GetBounds | Get the bounding rectangle of
// an object.
//@rdesc CRect that contains the origin and size of the object.
//
//@mfunc CPoint | IBounds2D | GetOrigin | Get the origin of an object.
//@rdesc CPoint that contains the X and Y coordinates of the origin.
//
//@mfunc CPoint | IBounds2D | SetOrigin | Set the origin of an object.
//@rdesc Previous origin.
//@parm int | x | X coordinate of desired origin.
//@parm int | y | Y coordinate of desired origin.
//
//@mfunc CPoint | IBounds2D | MoveOrigin | Move the origin of an object by
// a given X,Y offset
//@rdesc Previous origin.
//@parm int | xOff | Horizontal offset value.
//@parm int | yOff | Vertical offset value.
//
//@base public | ISize2D
//
class __declspec(uuid("3D80F542-D51B-11d2-8CAB-0010A4F36466")) IBounds2D : public ISize2D
{
public:
	//@cmember
	/* Get the bounding rectangle of an object. */
	virtual CRect GetBounds() const = 0;
	//@cmember
	/* Get the origin of an object. */
	virtual CPoint GetOrigin() const = 0;
	//@cmember
	/* Set the origin of an object. */
	virtual CPoint SetOrigin(int x, int y) = 0;
	//@cmember
	/* Move the origin of an object by a given X,Y offset. */
	virtual CPoint MoveOrigin(int xOff,int yOff) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc IVisual
//
//@class IVisual | Interface for drawing a visual object.
//
//@mfunc void | IVisual | Draw_ | Draw the object to a graphics context.
//@rdesc void
//@parm IGraphicsContext* | pIGC | Pointer to graphics context interface.
//
//@base public | IUnknown
//
class __declspec(uuid("3D80F545-D51B-11d2-8CAB-0010A4F36466"))
				IVisual : public IUnknown
{
public:
	//@cmember
	/* Draw the object to a device context. */
	virtual void Draw_(IGraphicsContext* pIGC) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc IVisual
//
//@class IVisual | Implementation of the IVisual and IBounds2D interfaces.
// This class contains a RECT structure and implements IBounds2D and
// ISize2D in terms of that bounding rectangle.
//
//@mfunc HRESULT | Visual2D | QueryInterface | Retrieve a pointer to
// an interface supported by this object.
//@parm REFIID | iid | Identifier of the interface being requested. 
//@parm void ** | ppvObject | Address of pointer variable that receives
// the interface pointer requested in riid.
//@rdesc S_OK if the interface is supported, E_NOINTERFACE if not.
//
//@mfunc ULONG | Visual2D | AddRef | Add a reference to this object.
//@rdesc New reference count value.
//@comm The Visual2D class does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.
//
//@mfunc ULONG | Visual2D | Release | Release a reference to this object.
//@rdesc New reference count value.
//@comm The Visual2D class does not implement reference counting.
// This function does nothing and returns 0. Derived classes can implement
// reference counting by overriding AddRef and Release.
//
//@mfunc void | Visual2D | GetSize | Get the width and height of an object.
//@rdesc Returns a size object.

//@mfunc void | Visual2D | SetSize | Set the width and height of an object.
//@rdesc Previous size.
//@parm int | cx | Width to set object to.
//@parm int | cy | Height to set object to.
//
//@mfunc CRect | Visual2D | GetBounds | Get the bounding rectangle of
// an object.
//@rdesc CRect that contains the origin and size of the object.
//
//@mfunc CPoint | Visual2D | GetOrigin | Get the origin of an object.
//@rdesc CPoint that contains the X and Y coordinates of the origin.
//
//@mfunc CPoint | Visual2D | SetOrigin | Set the origin of an object.
//@rdesc Previous origin.
//@parm int | x | X coordinate of desired origin.
//@parm int | y | Y coordinate of desired origin.
//
//@mfunc CPoint | Visual2D | MoveOrigin | Move the origin of an object by
// a given X,Y offset
//@rdesc Previous origin.
//@parm int | xOff | Horizontal offset value.
//@parm int | yOff | Vertical offset value.
//
//@mfunc void | IVisual | Draw_ | Draw the object to a graphics context.
//@rdesc void
//@parm IGraphicsContext* | pIGC | Pointer to graphics context interface.
//
//@mfunc int | Visual2D | GetWidth | Return the width of the object.
//@rdesc Width in logical units.
//
//@mfunc int | Visual2D | GetHeight | Return the height of the object.
//@rdesc Height in logical units.
//
//@base public | IVisual
//@base public | IBounds2D
//
//@xref <c IVisual>, <c IBounds2D>, <c ISize2D>
//
class Visual2D : public IVisual, public IBounds2D
{
// Constructors/destructor
public:

	//@cmember
	/* Construct a visual 2D object. */
	Visual2D() : m_rc(0,0,0,0)
	{
	}

// Attributes
protected:

	//@cmember
	/* Bounding rectangle for visual object. */
	CRect m_rc;

// Operations
public:

	//////////////////////////////////////////////////
	// IUnknown interface

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject)
	{
		*ppvObject = NULL;

		if (iid == IID_IVisual)
		{
			*ppvObject = static_cast<IVisual*>(this);
		}
		else if (iid == IID_IBounds2D)
		{
			*ppvObject = static_cast<IBounds2D*>(this);
		}
		else if (iid == IID_ISize2D)
		{
			*ppvObject = static_cast<ISize2D*>(this);
		}
		else if (iid == IID_IUnknown)
		{
			*ppvObject = static_cast<ISize2D*>(this);
		}

		if (*ppvObject != NULL)
		{
			this->AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return 0;
	}

	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		return 0;
	}

	//////////////////////////////////////////////////
	// ISize2D interface

	//@cmember
	/* Get the width and height of an object. */
	virtual CSize GetSize() const
	{
		return m_rc.Size();
	}

	//@cmember
	/* Set the width and height of an object. */
	virtual CSize SetSize(int cx, int cy)
	{
		CSize szPrev = m_rc.Size();
		m_rc.right = m_rc.left + cx;
		m_rc.bottom = m_rc.top + cy;
		return szPrev;
	}

	//////////////////////////////////////////////////
	// IBounds2D interface

	//@cmember
	/* Get the bounding rectangle of an object. */
	virtual CRect GetBounds() const
	{
		return m_rc;
	}

	//@cmember
	/* Get the origin of an object. */
	virtual CPoint GetOrigin() const
	{
		CPoint ptOrg;
		ptOrg.x = m_rc.left;
		ptOrg.y = m_rc.top;
		return ptOrg;
	}

	//@cmember
	/* Set the origin of an object. */
	virtual CPoint SetOrigin(int x, int y)
	{
		CPoint ptOld = GetOrigin();
		m_rc.left = x;
		m_rc.top = y;
		return ptOld;
	}

	//@cmember
	/* Move the origin of an object by a given X,Y offset. */
	virtual CPoint MoveOrigin(int xOff,int yOff)
	{
		CPoint ptPrev;
		ptPrev.x = m_rc.left;
		ptPrev.y = m_rc.top;
		m_rc.left += xOff;
		m_rc.right += xOff;
		m_rc.top += yOff;
		m_rc.bottom += yOff;
		return ptPrev;
	}

	//////////////////////////////////////////////////
	// IVisual interface

	//@cmember
	/* Draw the object to a graphics context. */
	virtual void Draw_(IGraphicsContext* pIGC)
	{
		pIGC;
	}

	//////////////////////////////////////////////////
	// Helper functions

	//@cmember
	/* Return the width of the object. */
	int GetWidth() const
	{
		return (m_rc.right - m_rc.left);
	}

	//@cmember
	/* Return the height of the object. */
	int GetHeight() const
	{
		return (m_rc.bottom - m_rc.top);
	}
};

};  // end namespace rw

#endif  // __COMPAT_VISUAL_H__
