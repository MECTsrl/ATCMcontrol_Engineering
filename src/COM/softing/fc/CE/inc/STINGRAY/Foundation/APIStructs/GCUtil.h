/////////////////////////////////////////////////////////////////////////////
// GCUtil.h The Rogue Wave Graphic Context Utility classes.
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Bob Powell
// Description:  
// These classes are intended specifically to replace MFC's CPoint, CRect,
// and CSize classes without the need for other MFC dependancies.
//


#pragma once
#ifndef __GCUTIL_H__
#define __GCUTIL_H__

#include <Foundation\SflCommon.h>


#ifdef NONSTANDARD_GDI
int CEMulDiv(int value, int multiplier, int divisor);
#endif //NONSTANDARD_GDI
// Forward declarations

// Exclude if __AFXWIN_H__ is defined
#if !defined(_SFL_MFC_SUPPORT)

class CSize;
class CRect;
class CPoint;

class CSize : public tagSIZE
{
public:
// Constructors
	// construct an uninitialized size
	CSize(){cx=cy=0;}
	// create from two integers
	CSize(int ncx, int ncy){cx=ncx; cy=ncy;}
	// create from another size
	CSize(SIZE s){cx=s.cx; cy=s.cy;}
	// create from a point
	CSize(POINT pt){cx=pt.x; cy=pt.y;}
	// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
	CSize(DWORD dwSize){cx=(int)(dwSize & 0x0000ffff); cy=(int)((dwSize >> 16) & 0x0000ffff);}

// Operations
	inline bool operator==(SIZE s) const;
	inline bool operator!=(SIZE s) const;
	inline void operator+=(SIZE s);
	inline void operator-=(SIZE s);

// Operators returning CSize values
	inline CSize operator+(SIZE s) const;
	inline CSize operator-(SIZE s) const;
	inline CSize operator-() const;

// Operators returning CPoint values
	inline CPoint operator+(POINT p) const;
	inline CPoint operator-(POINT p) const;

// Operators returning CRect values
	inline CRect operator+(const RECT* pR) const;
	inline CRect operator-(const RECT* pR) const;
};

/////////////////////////////////////////////////////////////////////////////
// CPoint - A 2-D point, similar to Windows POINT structure.

class CPoint : public tagPOINT
{
public:
// Constructors

	// create an uninitialized point
	CPoint(){x=y=0;}
	// create from two integers
	CPoint(int nx, int ny){x=nx; y=ny;}
	// create from another point
	CPoint(POINT pt){x=pt.x, y=pt.y;}
	// create from a size
	CPoint(SIZE s){x=s.cx; y=s.cy;}
	// create from a dword: x = LOWORD(dw) y = HIWORD(dw)
	CPoint(DWORD dwPoint){x=(int)(dwPoint & 0x0000ffff); y=(int)(dwPoint >> 16);}

// Operations

// translate the point
	inline void Offset(int nx, int ny){x+=nx; y+=ny;}
	inline void Offset(POINT p){x+=p.x; y+=p.y;}
	inline void Offset(SIZE s){x+=s.cx; y+=s.cy;}

	inline bool operator==(POINT p) const;
	inline bool operator!=(POINT p) const;
	inline void operator+=(SIZE s);
	inline void operator-=(SIZE s);
	inline void operator+=(POINT p);
	inline void operator-=(POINT p);

	inline bool operator<(POINT p);
	inline bool operator>(POINT p);
	inline bool operator<=(POINT p);
	inline bool operator>=(POINT p);

// Operators returning CPoint values
	inline CPoint operator+(SIZE s) const;
	inline CPoint operator-(SIZE s) const;
	inline CPoint operator-() const;
	inline CPoint operator+(POINT p) const;

// Operators returning CSize values
	inline CSize operator-(POINT p) const;

// Operators returning CRect values
	inline CRect operator+(const RECT* pR) const;
	inline CRect operator-(const RECT* pR) const;
};


/////////////////////////////////////////////////////////////////////////////
// CRect - A 2-D rectangle, similar to Windows RECT structure.

typedef const RECT* LPCRECT;    // pointer to read/only RECT

class CRect : public tagRECT
{
public:

// Constructors

	// uninitialized rectangle
	CRect(){top=left=right=bottom=0;}
	// from left, top, right, and bottom
	CRect(int l, int t, int r, int b){left=l; top=t; right=r; bottom=b;}
	// copy constructor
	CRect(const RECT& src){*this=src;}
	// from a pointer to another rect
	CRect(LPCRECT lpSrcRect){*this=*lpSrcRect;}
	// from a point and size
	CRect(POINT point, SIZE size){left=point.x; top=point.y; right=left+size.cx; bottom=top+size.cy;}
	// from two points
	CRect(POINT topleft, POINT bottomright){left=topleft.x; top=topleft.y; right=bottomright.x; bottom=bottomright.y;}

// Attributes (in addition to RECT members)

	// retrieves the width
	inline int Width() const {return right-left;}
	// returns the height
	inline int Height() const {return bottom-top;}
	// returns the size
	inline CSize Size() const {return CSize(Width(),Height());}
	// reference to the top-left point
	inline CPoint& TopLeft(){return *((CPoint *)this);}
	// reference to the bottom-right point
	inline CPoint& BottomRight(){return *((CPoint *)this+1);}
	// const reference to the top-left point
	inline const CPoint& TopLeft() const {return *((CPoint *)this);}
	// const reference to the bottom-right point
	inline const CPoint& BottomRight() const {return *((CPoint *)this+1);}
	// the geometric center point of the rectangle
	inline CPoint CenterPoint() const {return CPoint(left+(Width()/2), top+(Height()/2));}
	// swap the left and right
	inline void SwapLeftRight(){int t=left; right=left; left=t;}
	static void SwapLeftRight(LPRECT lpRect);

	// convert between CRect and LPRECT/LPCRECT (no need for &)
	operator LPRECT();
	operator LPCRECT() const;

	// returns true if rectangle has no area
	inline bool IsRectEmpty() const {return left==right || top == bottom;}
	// returns true if rectangle is at (0,0) and has no area
	inline bool IsRectNull() const {return left==top==0 && IsRectEmpty();}
	// returns true if point is within rectangle
	inline bool PtInRect(POINT point) const {return point.x>=left && point.x<=right && point.y>=top && point.y<=bottom;}

// Operations

	// set rectangle from left, top, right, and bottom
	inline void SetRect(int x1, int y1, int x2, int y2){left=x1; top=y1; right=x2; bottom=y2;}
	inline void SetRect(POINT topleft, POINT bottomright){*((CPoint *)this)=topleft; *((CPoint *)this+1)=bottomright;}
	// empty the rectangle
	inline void SetRectEmpty(){left=top=right=bottom=0;}
	// copy from another rectangle
	inline void CopyRect(LPCRECT lpSrcRect){*this = *lpSrcRect;}
	// true if exactly the same as another rectangle
	inline bool EqualRect(LPCRECT pR) const {return left==pR->left && top==pR->top && right==pR->right && bottom==pR->bottom;}

	// inflate rectangle's width and height
	
	inline void InflateRect(int x, int y){left -= x; top -= y; right += x; bottom += y;}
	inline void InflateRect(SIZE s){left -= s.cx; top -= s.cy; right += s.cx; bottom += s.cy;}
	inline void InflateRect(LPCRECT pR){left -= pR->left; top -= pR->top; right += pR->right; bottom += pR->bottom;}
	inline void InflateRect(int l, int t, int r, int b){left -= l; top -= t; right += r; bottom += b;}
	
	// deflate the rectangle's width and height

	inline void DeflateRect(int x, int y){left += x; top += y; right -= x; bottom -= y;}
	inline void DeflateRect(SIZE s){left += s.cx; top += s.cy; right -= s.cx; bottom -= s.cy;}
	inline void DeflateRect(LPCRECT pR){left += pR->left; top += pR->top; right -= pR->right; bottom -= pR->bottom;}
	inline void DeflateRect(int l, int t, int r, int b){left += l; top += t; right -= r; bottom -= b;}

	// translate the rectangle by moving its top and left
	inline void OffsetRect(int x, int y){left += x; top += y; right += x; bottom += y;}
	inline void OffsetRect(SIZE s){left += s.cx; top += s.cy; right += s.cx; bottom += s.cy;}
	inline void OffsetRect(POINT p){left += p.x; top += p.y; right += p.x; bottom += p.y;}
	inline void NormalizeRect();

	// set this rectangle to intersection of two others
	inline bool IntersectRect(LPCRECT pR1, LPCRECT pR2){ return ::IntersectRect(this, pR1, pR2)? true : false; }

	// set this rectangle to bounding union of two others
	inline bool UnionRect(LPCRECT pR1, LPCRECT pR2){return ::UnionRect(this, pR1, pR2)? true : false; }

	// set this rectangle to minimum of two others
	inline bool SubtractRect(LPCRECT pR1, LPCRECT pR2){ return ::SubtractRect(this, pR1, pR2)? true : false; }

// Additional Operations
	inline void operator=(const RECT& r);
	inline bool operator==(const RECT& r) const;
	inline bool operator!=(const RECT& r) const;
	inline void operator+=(POINT p);
	inline void operator+=(SIZE s);
	inline void operator+=(LPCRECT pR);
	inline void operator-=(POINT p);
	inline void operator-=(SIZE s);
	inline void operator-=(LPCRECT pR);
	inline void operator&=(const RECT& r);
	inline void operator|=(const RECT& r);

// Operators returning CRect values
	inline CRect operator+(POINT p) const;
	inline CRect operator-(POINT p) const;
	inline CRect operator+(LPCRECT pR) const;
	inline CRect operator+(SIZE s) const;
	inline CRect operator-(SIZE s) const;
	inline CRect operator-(LPCRECT pR) const;
	inline CRect operator&(const RECT& r) const;
	inline CRect operator|(const RECT& r) const;

	inline CRect MulDiv(int nMultiplier, int nDivisor) const;

/////////////////////////////////////////////////////////////////////////////////
// extended functionality
/////////////////////////////////////////////////////////////////////////////////

	inline double Aspect(){ return (double)Width() / Height();}

};

inline void CRect::NormalizeRect()
{
	int t;
	if (left > right) { 
		t = left; 
		left = right; 
		right = t; 
	}
	if (top > bottom) {
		t = top; 
		top = bottom; 
		bottom = t; 
	}
}

inline CRect CRect::MulDiv(int nMultiplier, int nDivisor) const
{
	return CRect(
#ifndef NONSTANDARD_GDI
	::MulDiv(left, nMultiplier, nDivisor),
	::MulDiv(top, nMultiplier, nDivisor),
	::MulDiv(right, nMultiplier, nDivisor),
	::MulDiv(bottom, nMultiplier, nDivisor));
#else // if NONSTANDARD_GDI
	CEMulDiv(left, nMultiplier, nDivisor),
	CEMulDiv(top, nMultiplier, nDivisor),
	CEMulDiv(right, nMultiplier, nDivisor),
	CEMulDiv(bottom, nMultiplier, nDivisor));
#endif //NONSTANDARD_GDI
}

inline void CRect::operator=(const RECT& r) 
{
	left	=r.left;
	top		=r.top;
	right	=r.right;
	bottom	=r.bottom;
}

inline bool CRect::operator==(const RECT& r) const
{
	return	left	==r.left	&&
			top		==r.top		&&
			right	==r.right	&&
			bottom	==r.bottom;
}

inline bool CRect::operator!=(const RECT& r) const
{
	return	left	!=r.left	||
			top		!=r.top		||
			right	!=r.right	||
			bottom	!=r.bottom;
}

inline void CRect::operator+=(POINT p)
{
	OffsetRect(p);
}

inline void CRect::operator+=(SIZE s) 
{
	OffsetRect(s);
}

inline void CRect::operator+=(LPCRECT pR) 
{
	InflateRect(pR);
}

inline void CRect::operator-=(POINT p) 
{
	OffsetRect(-p.x,-p.y);
}

inline void CRect::operator-=(SIZE s) 
{
	OffsetRect(-s.cx,-s.cy);
}

inline void CRect::operator-=(LPCRECT pR) 
{
	DeflateRect(pR);
}

inline void CRect::operator&=(const RECT& r) 
{
	::IntersectRect(this,this,&r);
}

inline void CRect::operator|=(const RECT& r) 
{
	::UnionRect(this,this,&r);
}


// CRect::operators returning CRect values
inline CRect CRect::operator+(POINT p) const 
{
	CRect r(this); r+=p; return r;
}

inline CRect CRect::operator-(POINT p) const 
{
	CRect r(this); r-=p; return r;
}

inline CRect CRect::operator+(LPCRECT pR) const 
{
	CRect r(this); r+=pR; return r;
}

inline CRect CRect::operator+(SIZE s) const 
{
	CRect r(this); r+=s; return r;
}

inline CRect CRect::operator-(SIZE s) const 
{
	CRect r(this); r-=s; return r;
}

inline CRect CRect::operator-(LPCRECT pR) const 
{
	CRect r(this); r-=pR; return r;
}

inline CRect CRect::operator&(const RECT& rc) const 
{
	CRect r(this); r.IntersectRect(this, &rc); return r;
}

inline CRect CRect::operator|(const RECT& rc) const 
{
	CRect r(this); r |= rc; return r;
}

inline CRect::operator LPRECT(void)
{
	return this;
}

inline CRect::operator LPCRECT(void) const
{
	return (const CRect *)this;
}
///////////////////////////////////////////////////////////////////////////////
// CSize implementation

inline bool CSize::operator==(SIZE s) const
{
	return (cx==s.cx && cy==s.cy);
}

inline bool CSize::operator!=(SIZE s) const
{
	return (cx!=s.cx || cy!=s.cy);
}

inline void CSize::operator+=(SIZE s)
{
	cx+=s.cx;
	cy+=s.cy;
}

inline void CSize::operator-=(SIZE s)
{
	cx-=s.cx;
	cy-=s.cy;
}

// CSize::operators returning CSize values
inline CSize CSize::operator+(SIZE s) const
{
	return CSize(cx+s.cx,cy+s.cy);
}

inline CSize CSize::operator-(SIZE s) const
{
	return CSize(cx-s.cx,cy-s.cy);
}

inline CSize CSize::operator-() const
{
	return CSize(-cx,-cy);
}

// CSize::operators returning CPoint values
inline CPoint CSize::operator+(POINT p) const
{
	return CPoint(cx+p.x, cy+p.y);
}

inline CPoint CSize::operator-(POINT p) const
{
	return CPoint(cx-p.x, cy-p.y);
}

// CSize::operators returning CRect values
inline CRect CSize::operator+(const RECT* pR) const
{
	return CRect(pR->left+cx,pR->top+cy,pR->right+cx,pR->bottom+cy);
}

inline CRect CSize::operator-(const RECT* pR) const
{
	return CRect(pR->left-cx,pR->top-cy,pR->right-cx,pR->bottom-cy);
}


inline bool CPoint::operator==(POINT p) const
{
	return (x==p.x && y==p.y);
}

inline bool CPoint::operator!=(POINT p) const
{
	return (x!=p.x || y!=p.y);
}

inline void CPoint::operator+=(SIZE s)
{
	x+=s.cx;
	y+=s.cy;
}

inline void CPoint::operator-=(SIZE s)
{
	x-=s.cx;
	y-=s.cy;
}

inline void CPoint::operator+=(POINT p)
{
	x+=p.x;
	y+=p.y;
}

inline void CPoint::operator-=(POINT p)
{
	x-=p.x;
	y-=p.y;
}

inline bool CPoint::operator<(POINT p)
{
	return ((y < p.y) || (y == p.y && x < p.x));
}

inline bool CPoint::operator>(POINT p)
{
	return ((y > p.y) || (y == p.y && x > p.x));
}

inline bool CPoint::operator<=(POINT p)
{
	return !operator >(p);
}

inline bool CPoint::operator>=(POINT p)
{
	return !operator <(p);
}

// CPoint::operators returning CPoint values
inline CPoint CPoint::operator+(SIZE s) const
{
	return CPoint(x+s.cx, y+s.cy);
}

inline CPoint CPoint::operator-(SIZE s) const
{
	return CPoint(x-s.cx, y-s.cy);
}

inline CPoint CPoint::operator-() const
{
	return CPoint(-x,-y);
}

inline CPoint CPoint::operator+(POINT p) const
{
	return CPoint(x+p.x, y+p.y);
}


// CPoint::operators returning CSize values
inline CSize CPoint::operator-(POINT p) const
{
	return CSize(x-p.x, y-p.y);
}


// CPoint::operators returning CRect values
inline CRect CPoint::operator+(const RECT* pR) const
{
	CRect r(*pR); r += *this; return r;
}

inline CRect CPoint::operator-(const RECT* pR) const
{
	CRect r(*pR); r -= *this; return r;
}

#endif // !defined(_SFL_MFC_SUPPORT)

#endif // __GCUTIL_H__

