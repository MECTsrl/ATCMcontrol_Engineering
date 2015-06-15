/////////////////////////////////////////////////////////////////////////////
// LogCoordinatesImpl.h : 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Logical coordinate interface
//

#pragma once

#ifndef __LOGCOORDINATESIMPL_H__
#define __LOGCOORDINATESIMPL_H__

#include <Foundation\StructuredGraphics\LogCoordinates.h>
#include <Foundation\StructuredGraphics\Zoom.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc ILogCoordinatesImpl
//
//@class ILogCoordinatesImpl |
// This class implements the <c ILogCoordinates> interface. The
// ILogCoordinates interface provides a mapping of logical units onto
// container units.
//
//@base public | ILogCoordinates

template <class T>
class ILogCoordinatesImpl : public ILogCoordinates
{
// Constructor(s) & destructor
public:

	//@cmember
	/* Default constructor for ILogCoordinatesImpl */
	ILogCoordinatesImpl() :
		m_nMapMode(MM_ANISOTROPIC),
		m_ptLogOrg(0,0)
	{
		m_ext.Logical.cx = 1;
		m_ext.Logical.cy = 1;
		m_ext.Container.cx = 1;
		m_ext.Container.cy = 1;
	}

// Attributes
protected:

	//@cmember
	/* The mapping mode as defined by the device context*/
	int m_nMapMode;

	//@cmember
	/* The logical origin of this visual part*/
	CPoint m_ptLogOrg;

	// @cmember,mstruct
	/* The extents struct maps logical units to container units*/
	struct Extents
	{
		CSize Logical;		// @@field  Extents are not the same as size! The logical extents in 
							// combination with the container extents define a ratio for mapping
		CSize Container;	// @@field The container extents in combination with the logical extents
							// define a ratio for mapping logical units to container units
	};

	//@cmember
	/* The extents struct maps logical units to container units*/
	struct Extents m_ext;

// Operations
public:

	//@cmember,mfunc
	// Retrieve the origin in logical units (i.e. scroll position)
	//@@rdesc Logical origin
	//@@end
	/* Retrieve the origin in logical units*/
	virtual CPoint GetLogOrigin() const
	{
		return m_ptLogOrg;
	}

	//@cmember,mfunc Sets the logical origin of the contents of this visual part
	//@@syntax SetLogOrigin(int x, int y)
	//@@syntax SetLogOrigin(const CPoint& point)
	//@@rdesc The previous logical origin of the visual part
	//@@parm const CPoint& | ptLog | The new logical origin in logical units
	//@@xref <mf ILogCoordinatesImpl::GetLogOrigin>
	//@@end
	/* Sets the logical origin of the contents of this visual part*/
	virtual CPoint SetLogOrigin(int x, int y)
	{
		CPoint oldOrigin = GetLogOrigin();
		m_ptLogOrg.x = x;
		m_ptLogOrg.y = y;
		return oldOrigin;
	}

	CPoint SetLogOrigin(const CPoint& ptLog)
	{
		return SetLogOrigin(ptLog.x, ptLog.y);
	}

	//@cmember,mfunc Offsets the logical origin by the specified deltas
	//@@rdesc The origin after the translation
	//@@parm int | xOff | The offset in the X direction in logical units
	//@@parm int | yOff | The offset in the Y direction in logical units
	//@@xref <mf ILogCoordinatesImpl::SetLogOrigin>
	//@@end
	/* Offsets the logical origin by the specified deltas*/
	CPoint OffsetLogOrigin(int xOff, int yOff)
	{
		if (xOff != 0 || yOff != 0)
		{
			SetLogOrigin(m_ptLogOrg.x + xOff, m_ptLogOrg.y + yOff);
		}
		return GetLogOrigin();
	}

	//@cmember,mfunc Returns the width and height of the component in logical units.
	//@@rdesc CSize The logical size of the component
	//@@comm
	// The value returned by this function represents the number of logical
	// units that fit in the component's bounding rectangle (defined in container
	// units). The size of the bounding rectangle is converted based on the ratio
	// defined by the container extents and logical extents.
	// 
	//@@xref <mf ILogCoordinatesImpl::SetLogSize>
	//@@end
	/* Returns the width and height of the component in logical units*/
	virtual CSize GetLogSize() const
	{
		ILogCoordinatesImpl<T>* pThis = const_cast< ILogCoordinatesImpl<T>* >(this);
		T* pT = static_cast<T*>(pThis);
		CSize szLog = pT->GetSize();
		CPtoLP(&szLog);
		szLog.cy *= GetYAxisDirection();
		return szLog;
	}

	//@cmember,mfunc
	// Returns the logical bounds as a rectangle
	//@@rdesc CRect
	//@@end
	/* Returns the logical bounds as a rectangle*/
	CRect GetLogBounds() const
	{
		CRect rcLog(GetLogOrigin(), GetLogSize());
		// A rect may be denormalized if we have an upward Y-Axis.
		// So, fix it.
		rcLog.NormalizeRect();
		return rcLog;
	}

	//@cmember,mfunc Sets the width and height of the component in logical units.
	//@@syntax SetLogSize(int cx, int cy)
	//@@syntax SetLogSize(const CSize& szLog)
	//@@parm  int | cx | the horizontal size
	//@@parm  int | cy | the vertical size
	//@@parm  const CSize& | szExt | the logical size
	//@@rdesc CSize The previous logical size of the component
	//@@comm
	// Sizes the component so that it has covers enough area to display
	// the specified logical size.  Note, the number of logical units
	// per pixel is not changed, rather the on-screen size of the
	// component is changed to accommodate the desired total number of
	// logical units.
	//@@xref <mf ILogCoordinatesImpl::GetLogSize>
	//@@end
	/* Sets the width and height of the component in logical units*/
	virtual CSize SetLogSize(int cx, int cy)
	{
		CSize szPrev(GetLogSize());
		CSize szCtnr(cx, cy);

		LPtoCP(&szCtnr);
		ILogCoordinatesImpl<T>* pThis = const_cast< ILogCoordinatesImpl<T>* >(this);
		T* pT = static_cast<T*>(pThis);
		pT->SetSize(szCtnr);

		return szPrev;
	}

	CSize SetLogSize(const CSize& szLog)
	{
		return SetLogSize(szLog.cx, szLog.cy);
	}

	//@cmember,mfunc Returns the size of the virtual space shown by this logical part in logical units.
	//@@rdesc CPoint
	//@@end
	/* Get the origin of the virtual coordinate space in logical units.*/
	virtual CPoint GetVirtualOrigin() const
	{
		return CPoint(0,0);
	}

	//@cmember,mfunc Sets the virtual origin of the logical part.
	//@@syntax SetVirtualOrigin(int x, int y)
	//@@syntax SetVirtualOrigin(const CPoint& ptVirtual)
	//@@rdesc void
	//@@parm int | x | The x component of the virtual origin in logical units
	//@@parm int | y | The y component of the virtual origin in logical units
	//@@parm const CPoint& | ptVirtualOrg | The new virtual origin
	//@@end
	/* Sets the virtual origin of the logical part*/
	virtual void SetVirtualOrigin(int x, int y)
	{
		x; // UNUSED
		y; // UNUSED
	}

	void SetVirtualOrigin(const CPoint& ptVirtualOrg)
	{
		SetVirtualSize(ptVirtualOrg.x, ptVirtualOrg.y);
	}

	//@cmember,mfunc Returns the size of the virtual space shown by this logical part in logical units.
	//@@rdesc CSize
	//@@comm
	// This function must be overriden, because by default its
	// return value is the same as <mf MvcLogicalPart::GetLogSize>.  So, why
	// doesn't the logical part store the virtual size and this function return
	// said value?  Because there is more then one place you may choose to
	// store the virtual size value.  For example, you could have one component
	// which contains another.  In this scenerio, the size of the contained
	// component is the virtual size of the container.  Alternatively, you may
	// want to store the virtual size directly in the logical part itself.  To
	// accommodate both possibilities, this function is virtual and must
	// be overriden.
	//@@xref <mf ILogCoordinatesImpl::SetVirtualSize>
	//@@end
	/* Get the total virtual size in logical units*/
	virtual CSize GetVirtualSize() const
	{
		return GetLogSize();
	}

	//@cmember,mfunc Sets the virtual size of the logical part.
	//@@syntax SetVirtualSize(int cx, int cy)
	//@@syntax SetVirtualSize(const CSize& szVirtual)
	//@@rdesc void
	//@@parm int | cx | The virtual width in logical units
	//@@parm int | cy | The virtual height in logical units
	//@@parm const CSize& | szVirtual | The new virtual size
	//@@comm
	// This function must be overriden.  It's default implementation does
	// nothing.  So, why doesn't the logical part store the virtual size
	// and this function set said value?  Because there is more then one place
	// you may choose to store the virtual size value.  For example, you could
	// have one component which contains another.  In this scenerio, the size
	// of the contained component is the virtual size of the container.
	// Alternatively, you may want to store the virtual size directly in the
	// logical part itself.  To accommodate both possibilities, this function
	// is virtual and must be overriden.
	//@@xref <mf ILogCoordinatesImpl::GetVirtualSize>
	//@@end
	/* Set the total virtual size in logical units*/
	virtual void SetVirtualSize(int cx, int cy)
	{
		cx; // UNUSED
		cy; // UNUSED
	}

	virtual void SetVirtualSize(const CSize& size)
	{
		size; // UNUSED
	}

	//@cmember,mfunc Reports the current mapping mode
	//@@rdesc The mapping mode used by this visual part
	//@@comm
	// The mapping modes are defined by the CDC class and the
	// visual part class makes use of these definitions.  Each
	// visual part can have its own local mapping mode which is
	// used in the logical to device coordinate mapping algorithm.
	//@@xref <mf CDC::GetMapMode>
	//@@end
	/* Retrieve the mapping mode used for logical to device coordinate translation*/
	virtual int GetMapMode() const
	{
		return m_nMapMode;
	}

	//@cmember,mfunc Sets the mapping mode to be used by this visual part
	//@@rdesc The previous mapping mode
	//@@parm int | nMapMode | The new mapping mode to be used by this visual part
	//@@comm
	// This function sets the mapping mode as defined by the CDC class.
	//@@xref <mf CDC::SetMapMode>
	//@@end
	/* Set the mapping mode used for logical to device coordinate translation*/
	virtual int SetMapMode(int nMapMode)
	{
		int prvMode = m_nMapMode;
		m_nMapMode = nMapMode;
		return prvMode;
	}

	//@cmember,mfunc Reports the extents of this visual part in container units.
	//@@rdesc Extent sizes of the visual part
	//@@xref <mf ILogCoordinatesImpl::SetExtents>
	//@@end
	/* Get the container extents.  Note: Extents are not the same as size.*/
	virtual CSize GetExtents() const
	{
		return m_ext.Container;
	}

	//@cmember,mfunc Sets the container extents of this visual part.
	//@@syntax SetExtents(int cx, int cy)
	//@@syntax SetExtents(const CSize& szExt)
	//@@rdesc The previous container extents.
	//@@parm int | cx | The new extents on the X axis
	//@@parm int | cy | The new extents of the Y axis
	//@@parm const CSize& | szExt | The new extents of the logical part
	//@@comm
	// Extents define how logical units are mapped to container units. The
	// logical extents and container extents define ratios along the
	// horizontal and vertical axes that are used to convert logical
	// units to container units. The extents are closely tied to the mapping
	// mode, and extents are read-only for all mapping modes except
	// MM_ANISOTROPIC and MM_ISOTROPIC.
	//@@xref <mf ILogCoordinatesImpl::GetExtents>
	//@@end
	/* Sets the container extents*/
	virtual CSize SetExtents(int cx, int cy)
	{
		CSize szPrev(m_ext.Container);

		if (!ExtentsFixed())
		{
			m_ext.Container.cx = cx;
			m_ext.Container.cy = cy;
		}

		return szPrev;
	}

	CSize SetExtents(const CSize& szExt)
	{
		return SetExtents(szExt.cx, szExt.cy);
	}

	//@cmember,mfunc Reports the logical extents of this visual part.
	//@@rdesc The logical extents
	//@@xref <mf ILogCoordinatesImpl::SetLogExtents>
	//@@end
	/* Get the logical extents*/
	virtual CSize GetLogExtents() const
	{
		return m_ext.Logical;
	}

	//@cmember,mfunc Sets the logical extents of this visual part
	//@@syntax SetLogExtents(int cx, int cy)
	//@@syntax SetLogExtents(const CSize& szLogExt)
	//@@rdesc The previous logical size of the visual part
	//@@parm int | cx | The new logical extents of this logical part on the X axis
	//@@parm int | cy | The new logical extents of this logical part on the Y axis
	//@@parm const CSize& | szLogExt | The new logical extents of this logical part
	//@@comm
	// Extents define how logical units are mapped to container units. The
	// logical extents and container extents define ratios along the
	// horizontal and vertical axes that are used to convert logical
	// units to container units. The extents are closely tied to the mapping
	// mode, and extents are read-only for all mapping modes except
	// MM_ANISOTROPIC and MM_ISOTROPIC.
	//@@xref <mf ILogCoordinatesImpl::GetLogExtents>
	//@@end
	/* Sets the logical extents of this visual part*/
	virtual CSize SetLogExtents(int cx, int cy)
	{
		CSize szPrev(m_ext.Logical);

		if (!ExtentsFixed())
		{
			m_ext.Logical.cx = cx;
			m_ext.Logical.cy = cy;
		}

		return szPrev;
	}

	//@cmember,mfunc Indicates if extents can be modified or not.
	//@@rdesc TRUE if extents can be modified, FALSE if extents are read-only.
	//@@comm This function returns TRUE for all mapping modes except MM_ANISOTROPIC
	// and MM_ISOTROPIC.
	//@@end
	/* Indicates if extents can be modified*/
	virtual BOOL ExtentsFixed() const
	{
		if ((m_nMapMode == MM_ANISOTROPIC) || (m_nMapMode == MM_ISOTROPIC))
		{
			return FALSE;
		}
		return TRUE;
	}


	//@cmember,mfunc
	// Determines the direction of the Y axis.
	//@@rdesc Enumeration indicating the direction of the Y axis
	//@@comm
	// This function examines the mapping mode and extents to determine
	// if a Cartesian-type coordinate system is being used
	//@@end
	/* Determines the direction of the Y axis*/
	virtual YAxisDirection GetYAxisDirection() const
	{
		YAxisDirection nDirection = YAXIS_DOWN;

		if (m_nMapMode == MM_LOMETRIC || m_nMapMode == MM_HIMETRIC ||
			m_nMapMode == MM_LOENGLISH || m_nMapMode == MM_HIENGLISH)
		{
			nDirection = YAXIS_UP;
		}
		else if ((m_ext.Logical.cy < 0 && m_ext.Container.cy > 0) ||
				 (m_ext.Logical.cy > 0 && m_ext.Container.cy < 0))
		{
			nDirection = YAXIS_UP;
		}

		return nDirection;
	}

	//@cmember,mfunc Convert logical units to container units
	//@@syntax LPtoCP( LPPOINT lpPoints, int nCount)
	//@@syntax LPtoCP( LPRECT lpRect )
	//@@syntax LPtoCP( LPSIZE lpSize )
	//@@rdesc void 
	//@@parm  LPPOINT | lpPoints | A pointer to an array of points to convert.
	//@@parmopt int | nCount | 1 | The number of points in the array
	//@@parm	 LPRECT | lpRect | A logical rectangle to convert
	//@@parm	 LPSIZE | lpSize | A logical size to convert
	//@@comm Use this function to convert an array of points, a rectangle, or a size 
	// from logical units to container units.  There is one very important subtlety
	// that you should be aware of.  The following two calls may yield entirely
	// different results:
	//
	//   LPtoCP(CPoint(5, 5))
	//   LPtoCP(CSize(5, 5))
	//
	// The first call determines which point in the parents coordinate
	// space the logical unit 5, 5 is located at.  The second call determines
	// the number of container units in 5 logical units along the X axis and
	// 5 units along the Y axis.  In other words, the first call takes the
	// logical origin into account.  The second does not, as size is independent
	// of the origin.
	//@@end
	/* Convert logical units to container units*/
	virtual void LPtoCP(LPPOINT lpPoints, int nCount = 1) const
	{
		int i;
		float xFactor, yFactor;
		CalcConversionFactors(xFactor, yFactor);
		ILogCoordinatesImpl<T>* pThis = const_cast< ILogCoordinatesImpl<T>* >(this);
		T* pT = static_cast<T*>(pThis);
		CSize szLog = pT->GetSize();
		CPoint ptOrg(pT->GetOrigin());
		for (i=0; i<nCount; i++) {
			lpPoints[i].x = (int)((lpPoints[i].x - m_ptLogOrg.x) / xFactor) + ptOrg.x;
			lpPoints[i].y = (int)((lpPoints[i].y - m_ptLogOrg.y) / yFactor) + ptOrg.y;
		}
	}

	virtual void LPtoCP(LPRECT lpRect) const
	{
		float xFactor, yFactor;
		CalcConversionFactors(xFactor, yFactor);
		ILogCoordinatesImpl<T>* pThis = const_cast< ILogCoordinatesImpl<T>* >(this);
		T* pT = static_cast<T*>(pThis);
		CPoint ptOrg(pT->GetOrigin());
		lpRect->left = (int)((lpRect->left - m_ptLogOrg.x) / xFactor) + ptOrg.x;
		lpRect->top = (int)((lpRect->top - m_ptLogOrg.y) / yFactor) + ptOrg.y;
		lpRect->right = (int)((lpRect->right - m_ptLogOrg.x) / xFactor) + ptOrg.x;
		lpRect->bottom = (int)((lpRect->bottom - m_ptLogOrg.y) / yFactor) + ptOrg.y;
	}

	virtual void LPtoCP(LPSIZE lpSize) const
	{
		float xFactor, yFactor;
		CalcConversionFactors(xFactor, yFactor);
		lpSize->cx = (int)(lpSize->cx / xFactor);
		lpSize->cy = (int)(lpSize->cy / yFactor);
	}

	//@cmember,mfunc Convert container units to logical units
	//@@syntax CPtoLP( LPPOINT lpPoints, int nCount)
	//@@syntax CPtoLP( LPRECT lpRect )
	//@@syntax CPtoLP( LPSIZE lpSize )
	//@@rdesc void 
	//@@parm  LPPOINT | lpPoints | A pointer to an array of points to convert.
	//@@parmopt int | nCount | 1 | The number of points in the array
	//@@parm	 LPRECT | lpRect | A container rectangle to convert
	//@@parm	 LPSIZE | lpSize | A container size to convert
	//@@comm Use this function to convert an array of points, a rectangle, or a size 
	// from container units to logical units.
	//@@end
	/* Convert container units to logical units*/
	virtual void CPtoLP(LPPOINT lpPoints, int nCount = 1) const
	{
		int i;
		float xFactor, yFactor;
		CalcConversionFactors(xFactor, yFactor);
		ILogCoordinatesImpl<T>* pThis = const_cast< ILogCoordinatesImpl<T>* >(this);
		T* pT = static_cast<T*>(pThis);
		CPoint ptOrg(pT->GetOrigin());
		for (i=0; i<nCount; i++) {
			lpPoints[i].x = m_ptLogOrg.x + (int)((lpPoints[i].x - ptOrg.x) * xFactor);
			lpPoints[i].y = m_ptLogOrg.y + (int)((lpPoints[i].y - ptOrg.y) * yFactor);
		}
	}

	virtual void CPtoLP(LPRECT lpRect) const
	{
		float xFactor, yFactor;
		CalcConversionFactors(xFactor, yFactor);
		ILogCoordinatesImpl<T>* pThis = const_cast< ILogCoordinatesImpl<T>* >(this);
		T* pT = static_cast<T*>(pThis);
		CPoint ptOrg(pT->GetOrigin());
		lpRect->left = m_ptLogOrg.x + (int)((lpRect->left - ptOrg.x) * xFactor);
		lpRect->top = m_ptLogOrg.y + (int)((lpRect->top - ptOrg.y) * yFactor);
		lpRect->right = m_ptLogOrg.x + (int)((lpRect->right - ptOrg.x) * xFactor);
		lpRect->bottom = m_ptLogOrg.y + (int)((lpRect->bottom - ptOrg.y) * yFactor);
	}

	virtual void CPtoLP(LPSIZE lpSize) const
	{
		float xFactor, yFactor;
		CalcConversionFactors(xFactor, yFactor);
		lpSize->cx = (int)(lpSize->cx * xFactor);
		lpSize->cy = (int)(lpSize->cy * yFactor);
	}

	//@cmember,mfunc Convert logical units to device units
	//@@syntax void LPtoDP(LPPOINT lpPoints, int nCount) const
	//@@syntax void LPtoDP(LPRECT lpRect) const
	//@@syntax void LPtoDP(LPSIZE lpSize) const
	//@@rdesc void 
	//@@parm  LPPOINT | lpPoints | A pointer to an array of points to convert.
	//@@parmopt int | nCount | 1 | The number of points in the array
	//@@parm	 LPRECT | lpRect | A logical rectangle to convert
	//@@parm	 LPSIZE | lpSize | A logical size to convert
	//@@comm Use this function to convert an array of points, a rectangle, or a size 
	// from logical units to device units. The implementation of this function in
	// this class simply calls <mf ILogCoordinatesImpl::LPtoCP>. However, derived
	// classes may define container and device units differently.
	//@@end
	/* Convert logical units to container units*/
	virtual void LPtoDP(LPPOINT lpPoints, int nCount) const
	{
		LPtoCP(lpPoints, nCount);
	}

	virtual void LPtoDP(LPRECT lpRect) const
	{
		LPtoCP(lpRect);
	}

	virtual void LPtoDP(LPSIZE lpSize) const
	{
		LPtoCP(lpSize);
	}

	//@cmember,mfunc Convert device units to logical units
	//@@syntax void DPtoLP(LPPOINT lpPoints, int nCount) const
	//@@syntax void DPtoLP(LPRECT lpRect) const
	//@@syntax void DPtoLP(LPSIZE lpSize) const
	//@@rdesc void 
	//@@parm  LPPOINT | lpPoints | A pointer to an array of points to convert.
	//@@parmopt int | nCount | 1 | The number of points in the array
	//@@parm	 LPRECT | lpRect | A container rectangle to convert
	//@@parm	 LPSIZE | lpSize | A container size to convert
	//@@comm Use this function to convert an array of points, a rectangle, or a size 
	// from device units to logical units. The implementation of this function in
	// this class simply calls <mf ILogCoordinatesImpl::CPtoLP>. However, derived
	// classes may define container and device units differently.
	//@@end
	/* Convert container units to logical units*/
	virtual void DPtoLP(LPPOINT lpPoints, int nCount) const
	{
		CPtoLP(lpPoints, nCount);
	}

	virtual void DPtoLP(LPRECT lpRect) const
	{
		CPtoLP(lpRect);
	}

	virtual void DPtoLP(LPSIZE lpSize) const
	{
		CPtoLP(lpSize);
	}

	BEGIN_GUID_MAP(ThisClass)
		GUID_ENTRY(ILogCoordinates)
	END_GUID_MAP

// Implementation
protected:
	virtual void CalcConversionFactors(float& xFactor, float& yFactor) const
	{
		CSize szWndExt(m_ext.Logical);
		CSize szVpExt(m_ext.Container);

		IZoom* pZoom = guid_cast<IZoom*>(const_cast< ILogCoordinatesImpl<T>* >(this));
		if (pZoom != NULL)
		{
			pZoom->ZoomExtents(szWndExt, szVpExt);
		}

		xFactor = (szVpExt.cx != 0) ? (float)szWndExt.cx / (float)szVpExt.cx : (float)1.0;
		yFactor = (szVpExt.cy != 0) ? (float)szWndExt.cy / (float)szVpExt.cy : (float)1.0;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __LOGCOORDINATESIMPL_H__
