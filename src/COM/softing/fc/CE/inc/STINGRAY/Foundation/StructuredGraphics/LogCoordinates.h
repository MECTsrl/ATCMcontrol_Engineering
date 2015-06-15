/////////////////////////////////////////////////////////////////////////////
// LogCoordinates.h : 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Logical coordinate interface
//

#pragma once

#ifndef __LOGCOORDINATES_H__
#define __LOGCOORDINATES_H__

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
// YAxisDirection

//@enum YAxisDirection | Enumeration defining direction positive Y-Axis
enum YAxisDirection
{
	YAXIS_UP = -1,
	YAXIS_DOWN = 1
};

///////////////////////////////////////////////////////////////////////////
//@doc ILogCoordinates

//@class ILogCoordinates | This interface mapping of logical coordinates
// onto container (device) coordinates.

class __declspec(uuid("9EBF6B30-E26A-4cea-BA7F-2C7E8220AA58"))
	ILogCoordinates : public IQueryGuid
{
public:
	//@cmember
	/* Retrieve the origin in logical units (i.e. scroll position)*/
	virtual CPoint GetLogOrigin() const = 0;
	//@cmember
	/* Returns the width and height of the component in logical units*/
	virtual CSize GetLogSize() const = 0;
	//@cmember
	/* Get the origin of the virtual coordinate space in logical units.*/
	virtual CPoint GetVirtualOrigin() const = 0;
	//@cmember
	/* Get the total virtual size in logical units*/
	virtual CSize GetVirtualSize() const = 0;

	//@cmember
	/* Retrieve the mapping mode used for logical to device coordinate translation*/
	virtual int GetMapMode() const = 0;
	//@cmember
	/* Get the container extents.  Note: Extents are not the same as size.*/
	virtual CSize GetExtents() const = 0;
	//@cmember
	/* Get the logical extents.  Note: Extents are not the same as size.*/
	virtual CSize GetLogExtents() const = 0;
	//@cmember
	/* Gets the direction of the Y axis */
	virtual YAxisDirection GetYAxisDirection() const = 0;

	//@cmember
	/* Convert logical units to container units*/
	virtual void LPtoCP(LPPOINT lpPoints, int nCount) const = 0;
	virtual void LPtoCP(LPRECT lpRect) const = 0;
	virtual void LPtoCP(LPSIZE lpSize) const = 0;
	//@cmember
	/* Convert container units to logical units*/
	virtual void CPtoLP(LPPOINT lpPoints, int nCount) const = 0;
	virtual void CPtoLP(LPRECT lpRect) const = 0;
	virtual void CPtoLP(LPSIZE lpSize) const = 0;

	//@cmember
	/* Convert logical units to device units*/
	virtual void LPtoDP(LPPOINT lpPoints, int nCount) const = 0;
	virtual void LPtoDP(LPRECT lpRect) const = 0;
	virtual void LPtoDP(LPSIZE lpSize) const = 0;
	//@cmember
	/* Convert device units to logical units*/
	virtual void DPtoLP(LPPOINT lpPoints, int nCount) const = 0;
	virtual void DPtoLP(LPRECT lpRect) const = 0;
	virtual void DPtoLP(LPSIZE lpSize) const = 0;
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __LOGCOORDINATES_H__
