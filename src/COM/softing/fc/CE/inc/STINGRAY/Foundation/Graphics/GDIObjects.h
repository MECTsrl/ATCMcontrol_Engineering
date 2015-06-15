/////////////////////////////////////////////////////////////////////////////
// GDIObjects.h : Advanced graphics package
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Bob Powell
// Description:  GDI Objects wrappers
//


#pragma once

#ifndef __GDIOBJECTS_H__
#define __GDIOBJECTS_H__

#include <tchar.h>
#include <foundation\APIStructs\GCUtil.h>
#include <foundation\Debug.h>
#include <foundation\sfldef.h>


#if defined(_SFL_MFC_SUPPORT) && defined(_INC_WINDOWSX)
// The following name from MFC's WINDOWSX.H collide with symbols here
#undef CopyRgn
#endif

namespace stingray {
namespace foundation {


//@doc


template <typename _GDIHandle>
class CHandleWrapper
{
private:
	typedef CHandleWrapper<_GDIHandle> _thisClass;

public:
	typedef _GDIHandle HANDLE;

	// Construction
	CHandleWrapper(_GDIHandle h = NULL, bool bOwn = true): m_hObject(h), m_bOwnHandle(bOwn)
	{}

	CHandleWrapper(const _thisClass& rhs, bool bTransferOwnership = true): m_hObject(rhs.m_hObject), m_bOwnHandle(false)
	{
		if (bTransferOwnership) {
			m_bOwnHandle = rhs.OwnHandle();
			rhs.ReleaseOwnership();
		}
	}

	const _thisClass& operator=(const _thisClass& rhs)
	{
		if (this != &rhs)
		{
			if (m_bOwnHandle) 
			{
				Delete();
			}
			m_hObject = rhs;
			// Always transfer ownership on assignment
			m_bOwnHandle = rhs.OwnHandle();
			rhs.ReleaseOwnership();
		}
		return *this;
	}

	~CHandleWrapper()
	{
		if (m_bOwnHandle) {
			Delete();
		}
	}


	operator _GDIHandle() const
	{
		return this != NULL ? m_hObject : NULL;
	}
	
	bool operator==(const _thisClass& obj) const
	{
		return m_hObject == static_cast<_GDIHandle>(obj);
	}

	bool OwnHandle() const
	{
		return m_bOwnHandle;
	}

public:
	virtual void Attach(_GDIHandle h, bool bOwn = true)
	{
		if (m_bOwnHandle) {
			Delete();
		}
		m_bOwnHandle = bOwn;
		m_hObject = h;
	}

	virtual _GDIHandle Detach() 
	{
		_GDIHandle h = m_hObject;
		m_hObject = NULL;
		m_bOwnHandle = false;
		return h;
	}

	bool Delete()
	{
		bool bRes = true;
		if (m_bOwnHandle && m_hObject) {
			bRes = ReleaseHandle();
			m_hObject = NULL;
		}
		return bRes;
	}

	virtual bool ReleaseHandle()
	{
		return false;
	}


	void ReleaseOwnership() const
	{
		m_bOwnHandle = false;
	}

protected:
	_GDIHandle m_hObject;
	mutable bool m_bOwnHandle;
};


template <typename _GDIHandle>
class CGDIObject:
	public CHandleWrapper<_GDIHandle>
{
	typedef CGDIObject<_GDIHandle> _thisClass;

public:

	/* Object construction */
	CGDIObject(_GDIHandle h = NULL, bool bOwn = true): CHandleWrapper<_GDIHandle>(h, bOwn)
	{}

	CGDIObject(_thisClass& rhs, bool bTransferOwnership = true): CHandleWrapper<_GDIHandle>(rhs, bTransferOwnership)
	{}

	~CGDIObject()
	{
		if (m_bOwnHandle) {
			Delete();
		}
	}

	_thisClass& operator=(const _thisClass& rhs)
	{
		CHandleWrapper<_GDIHandle>::operator=(rhs);
		return *this;
	}


public:
	// CHandleWrapper overrides

	bool DeleteObject()
	{return Delete();}

	virtual bool ReleaseHandle()
	{
		return ::DeleteObject(m_hObject)? true : false;
	}

public:
	// GDI Operations

	// todo Add your Intellisense compatible description here
	int GetObject(int nCount, LPVOID lpObject) const;

	// todo Add your Intellisense compatible description here
	UINT GetObjectType() const;

	// todo Add your Intellisense compatible description here
	bool CreateStockObject(int nIndex) ;

	// todo Add your Intellisense compatible description here
	bool UnrealizeObject() ;

	// todo Add your Intellisense compatible description here
	void UpdateObject();

};

template <typename _GDIHandle>
inline
int CGDIObject<_GDIHandle>::GetObject(int nCount, LPVOID lpObject) const
{
	ASSERT(m_hObject);
	return ::GetObject(m_hObject, nCount, lpObject);
}

template <typename _GDIHandle>
inline
UINT CGDIObject<_GDIHandle>::GetObjectType() const
{
	ASSERT(m_hObject);
	return ::GetObjectType(m_hObject);
}

template <typename _GDIHandle>
inline
bool CGDIObject<_GDIHandle>::CreateStockObject(int nIndex)
{
	ASSERT(!m_hObject);
	Attach(reinterpret_cast<_GDIHandle>(::GetStockObject(nIndex)));
	return m_hObject != NULL;
}

template <typename _GDIHandle>
inline
bool CGDIObject<_GDIHandle>::UnrealizeObject()
{
	ASSERT(m_hObject);
#ifndef NONSTANDARD_GDI
	return ::UnrealizeObject(m_hObject)? true : false;
#else // if NONSTANDARD_GDI
	return false;
#endif //NONSTANDARD_GDI
}


template <typename _GDIHandle>
inline
void CGDIObject<_GDIHandle>::UpdateObject()
{
	// // TODO (just an idea for now!)
}

/* Not using right now since SelectObject has overloads
   to take both a reference and a pointer to a GDI Object
///////////////////////////////////////////////////////////////
// Wrapper class that allows the GDI objects it wraps to look
// like a pointer so code written with these will look like
// and be compatible with MFC GDI code
template<typename GDIObject>
class CGDITempPtrImpl : public GDIObject
{
public:
	CGDITempPtrImpl(GDIObject::HANDLE h = NULL, bool bOwn = true): GDIObject(h, bOwn)
	{}

	CGDITempPtrImpl(CGDITempPtrImpl& rhs, bool bTransferOwnership = true): GDIObject(rhs, bTransferOwnership)
	{}

	operator GDIObject*()
	{ return this; }

	operator const GDIObject*() const
	{ return this; }

	CGDITempPtrImpl* operator->()
	{ return this; }

	const CGDITempPtrImpl* operator->() const
	{ return this; }

	operator GDIObject::HANDLE() const
	{ return this != NULL ? m_hObject : NULL; }
};
*/

// Forward declarations of classes that are wrapped
// by the template
class CGDIPen;
class CGDIBitmap;
class CGDIBrush;
class CGDIFont;
class CGDIRgn;
class CGDIPalette;

// Typedefs for these types. Any GDIObject that is used to
// store the result of a function that returns a temporary
// object in MFC, like SelectObject or FromHandle, should
// be declared as one of these types instead of the explict 
// MFC or SFL GDIObject type for compatibility.
#if defined(_SFL_MFC_SUPPORT)
	typedef CPen*     CGDIPenPtr;
	typedef CBitmap*  CGDIBitmapPtr;
	typedef CBrush*   CGDIBrushPtr;
	typedef CFont*    CGDIFontPtr;
	typedef CRgn*     CGDIRgnPtr;
	typedef CPalette* CGDIPalettePtr;
#else
	/*
	typedef CGDITempPtrImpl<CGDIPen>     CGDIPenPtr;
	typedef CGDITempPtrImpl<CGDIBitmap>  CGDIBitmapPtr;
	typedef CGDITempPtrImpl<CGDIBrush>   CGDIBrushPtr;
	typedef CGDITempPtrImpl<CGDIFont>    CGDIFontPtr;
	typedef CGDITempPtrImpl<CGDIRgn>     CGDIRgnPtr;
	typedef CGDITempPtrImpl<CGDIPalette> CGDIPalettePtr;
	*/
	typedef CGDIPen     CGDIPenPtr;
	typedef CGDIBitmap  CGDIBitmapPtr;
	typedef CGDIBrush   CGDIBrushPtr;
	typedef CGDIFont    CGDIFontPtr;
	typedef CGDIRgn     CGDIRgnPtr;
	typedef CGDIPalette CGDIPalettePtr;
#endif // !defined(_SFL_MFC_SUPPORT)

/////////////////////////////////
// CGDIPen class

//@class CGDIPen |
// Encapsulates a GDI pen handle
//@base public | CGDIObject<lt>HPEN<gt>
class CGDIPen: 
	public CGDIObject<HPEN>
{
public:
	//@cmember,mfunc | CGDIPen | (HPEN h = NULL, bool bOwn = true) |
	// Attach constructor. Initializes a new instance of CGDIPen and 
	// optionally attaches a previously existing pen handle. The ownership
	// of this handle is determined by the bOwn parameter.
	//@@parm HPEN | h | Handle to existing GDI pen, or NULL if the object is being
	// created in detached state.
	//@@parm bool | bOwn | Ownership flag. If true, this instance takes ownership of the
	// handle, otherwise the instance just wraps the handle but does not control its life time.
	CGDIPen(HPEN h = NULL, bool bOwn = true): CGDIObject<HPEN>(h, bOwn)
	{}

	//@cmember,mfunc | CGDIPen | (CGDIPen& rhs, bool bTransferOwnership = true) |
	// Copy constructor. Takes an existing CGDIPen instance and copies its contents to the
	// new instance being constructed. If the object being copied is the owner of the handle,
	// ownership's transference is controlled by the parameter bTransferOwnership.
	//@@parm CGDIPen& | rhs | Object to copy from.
	//@@parm bool | bTransferOwnership | If true and the object reference by rhs has ownership of the
	// handle it wraps, ownership is transferred to the new instance.
	CGDIPen(const CGDIPen& rhs, bool bTransferOwnership = true): CGDIObject<HPEN>(rhs, bTransferOwnership)
	{}

	//@cmember,mfunc | CGDIPen | (int nPenStyle, int nWidth, COLORREF cr) |
	// Creation constructor. Takes the parameters necessary to create a GDI pen
	// object and attaches it to the new instance.
	//@@parm int | nPenStyle | Pen style. 
	//@@parm int | nWidth | Pen width.
	//@@parm COLORREF | cr | Pen color.
	//@@comm
	// For more information on the parameters, see your Windows PlatformSDK documentation.
	CGDIPen(int nPenStyle, int nWidth, COLORREF cr)
	{
		Attach(::CreatePen(nPenStyle, nWidth, cr));
	}

	
	//@cmember,mfunc CGDIPen& | operator = | (CGDIPen& rhs) |
	// Copies the contents of the passed object into this instance. Ownership is always
	// transfered if it belongs to the object being copied.
	//@@parm CGDIPen& | rhs | Object to copy from.
	//@@rdesc Returns this instance.
	CGDIPen& operator=(const CGDIPen& rhs)
	{
		CGDIObject<HPEN>::operator=(rhs);
		return *this;
	}


	//@cmember,mfunc | CGDIPen | (int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL ) |
	// Extended creation constructor. Creates a GDI pen using the parameters passed, and 
	// attaches it to the new instance.
	//@@parm Pen style.
	//@@parm Pen width.
	//@@parm Pen pattern.
	//@@parm Number of entries in lpStyle.
	//@@parm Styles to apply.
	//@@comm
	// For more information on the parameters, see your Windows PlatformSDK documentation.
	CGDIPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL )
	{
	#ifndef NONSTANDARD_GDI
		Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle));
	#else // if NONSTANDARD_GDI
	#endif //NONSTANDARD_GDI
	}

public:
	// GDI Pen operations


	//@cmember
	// Creates a simple, solid color pen
	bool CreatePen(int nPenStyle, int nWidth, COLORREF crColor);

	//@cmember
	// Creates an extended pen 
	bool CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,int nStyleCount = 0, const DWORD* lpStyle = NULL);

	//@cmember
	// Creates a pen indirectly, based on the LOGPEN data struct
	bool CreatePenIndirect(LPLOGPEN lpLogPen);

	//@cmember
	// Returns the LOGPEN structure associated with this pen
	int GetLogPen(LOGPEN* pLogPen);

	//@cmember
	// Returns the EXTLOGPEN structure that describes this pen.
	int GetExtLogPen(EXTLOGPEN* pLogPen);
};

 
inline
//@mfunc Creates a simple pen.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIPen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor) 
{
	ASSERT(!m_hObject);
	Attach(::CreatePen(nPenStyle, nWidth, crColor));
	return m_hObject != NULL;
}

inline
//@mfunc Creates a pattern pen.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIPen::CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,int nStyleCount, const DWORD* lpStyle) 
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle));
	return m_hObject != NULL;
#else // if NONSTANDARD_GDI
	return false;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc Creates a pen from the information in the LOGPEN structure.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIPen::CreatePenIndirect(LPLOGPEN lpLogPen) 
{
	ASSERT(!m_hObject);
	Attach(::CreatePenIndirect(lpLogPen));
	return m_hObject != NULL;
}

inline
//@mfunc Retrieves the LOGPEN structure that describes this pen object.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIPen::GetLogPen(LOGPEN* pLogPen) 
{
	ASSERT(m_hObject);
	return ::GetObject(m_hObject, sizeof(LOGPEN), pLogPen);
}

inline
//@mfunc Retrieves the EXTLOGPEN structure that describes this pen object.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIPen::GetExtLogPen(EXTLOGPEN* pLogPen) 
{
	ASSERT(m_hObject);
	return ::GetObject(m_hObject, sizeof(EXTLOGPEN), pLogPen);
}



/////////////////////////////
// CGDIBitmap class



//@class CGDIBitmap |
// Encapsulates a GDI bitmap object.
//@base public | CGDIObject<lt>HBITMAP<gt>
class CGDIBitmap : 
	public CGDIObject<HBITMAP>
{
public:
	
	//@cmember,mfunc | CGDIBitmap | (HBITMAP h = NULL, bool bOwn = true) |
	// Attach constructor. Initializes a new instance of CGDIBitmap and 
	// optionally attaches a previously existing bitmap handle. The ownership
	// of this handle is determined by the bOwn parameter.
	//@@parm HBITMAP | h | Handle to existing GDI bitmap, or NULL if the object is being
	// created in detached state.
	//@@parm bool | bOwn | Ownership flag. If true, this instance takes ownership of the
	// handle, otherwise the instance just wraps the handle but does not control its life time.
	CGDIBitmap(HBITMAP h = NULL, bool bOwn = true): CGDIObject<HBITMAP>(h, bOwn)
	{}

	//@cmember,mfunc | CGDIBitmap | (CGDIBitmap& rhs, bool bTransferOwnership = true) |
	// Copy constructor. Takes an existing CGDIBitmap instance and copies its contents to the
	// new instance being constructed. If the object being copied is the owner of the handle,
	// ownership's transference is controlled by the parameter bTransferOwnership.
	//@@parm CGDIBitmap& | rhs | Object to copy from.
	//@@parm bool | bTransferOwnership | If true and the object reference by rhs has ownership of the
	// handle it wraps, ownership is transferred to the new instance.
	CGDIBitmap(const CGDIBitmap& rhs, bool bTransferOwnership = true): CGDIObject<HBITMAP>(rhs, bTransferOwnership)
	{}

	//@cmember,mfunc | CGDIBitmap | (int nPenStyle, int nWidth, COLORREF cr) |
	// Creation constructor. Takes the parameters necessary to create a GDI pen
	// object and attaches it to the new instance.
	//@@parm int | nWidth | Bitmap width. 
	//@@parm int | nHeight | Bitmap height.
	//@@parm UINT | nPlanes | Number of planes.
	//@@parm UINT | nBitcount | Number of elements in lpBits
	//@@parm const void* | lpBits | Array of bits.
	//@@comm 
	// For more information on the parameters, see your Windows PlatformSDK documentation.
	CGDIBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits)
	{
		Attach(::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits), true);
	}

	//@cmember,mfunc | CGDIBitmap | (int nPenStyle, int nWidth, COLORREF cr) |
	// Indirect creation constructor. Takes the parameters necessary to create a GDI pen
	// object and attaches it to the new instance.
	//@@parm LPBITMAP | lpBitmap | Bitmap structure. 
	//@@comm 
	// For more information on the parameters, see your Windows PlatformSDK documentation.
	CGDIBitmap(LPBITMAP lpBitmap)
	{
		Attach(::CreateBitmapIndirect(lpBitmap), true);
	}

	//@cmember,mfunc CGDIBitmap& | operator = | (CGDIBitmap& rhs) |
	// Copies the contents of the passed object into this instance. Ownership is always
	// transfered if it belongs to the object being copied.
	//@@parm CGDIBitmap& | rhs | Object to copy from.
	//@@rdesc Returns this instance.
	CGDIBitmap& operator=(const CGDIBitmap& rhs)
	{
		CGDIObject<HBITMAP>::operator=(rhs);
		return *this;
	}

public:
	// GDI Bitmap operations

	//@cmember
	// Loads a bitmap from resources
	bool LoadBitmap(LPCTSTR lpszResourceName, HINSTANCE hInstResource = GetResourceHandle());

	//@cmember
	// Loads a bitmap from resources
	bool LoadBitmap(UINT nIDResource, HINSTANCE hInstResource = GetResourceHandle());

	//@cmember
	// Loads windows stock bitmap
	bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_

	//@cmember
	// Create a bitmap from direct parameters
	bool CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits);

	//@cmember
	// Create a new bitmap from BITMAP structure
	bool CreateBitmapIndirect(LPBITMAP lpBitmap);

	//@cmember
	// Create a bitmap compatible with the DC given
	bool CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight);

	//@cmember
	// Create a discardable bitmap
	bool CreateDiscardableBitmap(HDC hdc, int nWidth, int nHeight);

	//@cmember
	// Retrieve the BITMAP structure that describes this bitmap
	int GetBitmap(BITMAP* pBitMap);

	//@cmember
	// Retrieve the size of the bitmap
	CSize GetBitmapSize() const;

	
	//@cmember
	// Set bits in the bitmap data
	DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);

	//@cmember
	// Get bits from the bitmap data
	DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;

	//@cmember
	// Set the dimensions of the bitmap
	CSize SetBitmapDimension(int nWidth, int nHeight);

	//@cmember
	// Retrieve the dimensions of the bitmap
	CSize GetBitmapDimension() const;
};


//@mfunc bool | CGDIBitmap | LoadBitmap |
// Loads a bitmap from the resource section of the module identified by hInstResource.
//@syntax bmp.LoadBitmap(lpszResourceName[, hInstResource]);
//@syntax bmp.LoadBitmap(nIDResource[, hInstResource]);
//@parm LPCTSTR | lpszResourceName | Resource identifier.
//@parm UINT | nIDResource | Resource identifier.
//@parm HINSTANCE | hInstResource | Handle to the instance containing the resource.
//@rdesc true if function is successful, false otherwise.
//@end
inline
bool CGDIBitmap::LoadBitmap(LPCTSTR lpszResourceName, HINSTANCE hInstResource)
{
	ASSERT(!m_hObject);
	Attach(::LoadBitmap(hInstResource, lpszResourceName));
	return m_hObject != NULL;
}

inline
bool CGDIBitmap::LoadBitmap(UINT nIDResource, HINSTANCE hInstResource)
{
	ASSERT(!m_hObject);
	return LoadBitmap(MAKEINTRESOURCE(nIDResource), hInstResource);
}

inline
//@mfunc
// Loads one of windows stock bitmaps.
//@parm UINT | nIDBitmap | Resource identifier.
//@rdesc true if function is successful, false otherwise.
//@end
bool CGDIBitmap::LoadOEMBitmap(UINT nIDBitmap)
{
	ASSERT(!m_hObject);
	Attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap)));
	return m_hObject != NULL;
}

inline
//@mfunc Creates a bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBitmap::CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits)
{
	ASSERT(!m_hObject);
	Attach(::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits));
	return m_hObject != NULL;
}

inline
//@mfunc Creates a bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBitmap::CreateBitmapIndirect(LPBITMAP lpBitmap)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateBitmapIndirect(lpBitmap));
	return m_hObject != NULL;
#else // if NONSTANDARD_GDI
	return false;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc Creates a bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBitmap::CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight)
{
	ASSERT(!m_hObject);
	ASSERT(hdc);
	Attach(::CreateCompatibleBitmap(hdc, nWidth, nHeight));
	return m_hObject != NULL;
}

inline
//@mfunc Creates a bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBitmap::CreateDiscardableBitmap(HDC hdc, int nWidth, int nHeight)
{
	ASSERT(!m_hObject);
	ASSERT(hdc);
#ifndef NONSTANDARD_GDI
	Attach(::CreateDiscardableBitmap(hdc, nWidth, nHeight));
	return m_hObject != NULL;
#else // if NONSTANDARD_GDI
	return false;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc Retrieves the BITMAP structure associated with this object.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIBitmap::GetBitmap(BITMAP* pBitMap)
{
	ASSERT(m_hObject);
	return ::GetObject(m_hObject, sizeof(BITMAP), pBitMap);
}

inline
//@mfunc Retrieves the size of this bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
CSize CGDIBitmap::GetBitmapSize() const
{
	ASSERT(m_hObject);
	BITMAP bmp;
	int nRes = ::GetObject(m_hObject, sizeof(BITMAP), &bmp);
	CSize szReturn;
	if (nRes != 0) {
		szReturn = CSize(bmp.bmWidth, bmp.bmHeight);
	}
	return szReturn;
}

inline
//@mfunc Modifies the underlying bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
DWORD CGDIBitmap::SetBitmapBits(DWORD dwCount, const void* lpBits)
{
	ASSERT(m_hObject);
#ifndef NONSTANDARD_GDI
	return ::SetBitmapBits((HBITMAP)m_hObject, dwCount, lpBits);
#else // if NONSTANDARD_GDI
	return 0;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc Retrieves bitmap data.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
DWORD CGDIBitmap::GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
{
	ASSERT(m_hObject);
#ifndef NONSTANDARD_GDI
	return ::GetBitmapBits((HBITMAP)m_hObject, dwCount, lpBits);
#else // if NONSTANDARD_GDI
	return 0;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc Sets the dimensions of the bitmap.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
CSize CGDIBitmap::SetBitmapDimension(int nWidth, int nHeight)
{
	ASSERT(m_hObject);
	CSize s;
#ifndef NONSTANDARD_GDI
	::SetBitmapDimensionEx((HBITMAP)m_hObject, nWidth, nHeight, &s);
#else // if NONSTANDARD_GDI
#endif //NONSTANDARD_GDI
	return s;
}

inline
//@mfunc Retrieves the bitmap dimensions
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
CSize CGDIBitmap::GetBitmapDimension() const
{
	ASSERT(m_hObject);
	CSize s;
#ifndef NONSTANDARD_GDI
	::GetBitmapDimensionEx((HBITMAP)m_hObject, &s);
#else // if NONSTANDARD_GDI
#endif //NONSTANDARD_GDI
	return s;
}




/////////////////////////////
// CGDIBrush class

//@class CGDIBrush |
// Encapsulates a GDI brush object
//@base public | CGDIObject<lt>HBRUSH<gt>
class CGDIBrush : 
	public CGDIObject<HBRUSH>
{
public:

	//@cmember,mfunc | CGDIBrush | (HBRUSH h = NULL, bool bOwn = true) |
	// Attach constructor. Initializes a new instance of CGDIBrush and 
	// optionally attaches a previously existing brush handle. The ownership
	// of this handle is determined by the bOwn parameter.
	//@@parm HBRUSH | h | Handle to existing GDI brush, or NULL if the object is being
	// created in detached state.
	//@@parm bool | bOwn | Ownership flag. If true, this instance takes ownership of the
	// handle, otherwise the instance just wraps the handle but does not control its life time.
	CGDIBrush(HBRUSH h = NULL, bool bOwn = true): CGDIObject<HBRUSH>(h, bOwn)
	{}

	//@cmember,mfunc | CGDIBrush | (CGDIBrush& rhs, bool bTransferOwnership = true) |
	// Copy constructor. Takes an existing CGDIBrush instance and copies its contents to the
	// new instance being constructed. If the object being copied is the owner of the handle,
	// ownership's transference is controlled by the parameter bTransferOwnership.
	//@@parm CGDIBrush& | rhs | Object to copy from.
	//@@parm bool | bTransferOwnership | If true and the object reference by rhs has ownership of the
	// handle it wraps, ownership is transferred to the new instance.
	CGDIBrush(const CGDIBrush& rhs, bool bTransferOwnership = true): CGDIObject<HBRUSH>(rhs, bTransferOwnership)
	{}

	
	//@cmember,mfunc | CGDIBitmap | (COLORREF cr) |
	// Creation constructor. Creates a solid brush of the color passed in.
	//@@parm COLORREF | cr | Color of the new brush.
	CGDIBrush(COLORREF cr)
	{
		Attach(::CreateSolidBrush(cr));
	}

	//@cmember,mfunc | CGDIBitmap | (int nIndex, COLORREF cr) |
	// Creation constructor. Creates a hatched pattern brush, with the pattern index given in
	// nIndex and of color cr.
	//@@parm int | nIndex | Pattern index.
	//@@parm COLORREF | cr | Brush color.
	//@@comm 
	// For more information on the parameters, see CreateHatchBrush your Windows PlatformSDK documentation.
	CGDIBrush(int nIndex, COLORREF cr)
	{
	#ifndef NONSTANDARD_GDI
		Attach(::CreateHatchBrush(nIndex, cr));
	#else 
	#endif
	}

	//@cmember,mfunc | CGDIBitmap | (HBITMAP hbmp) |
	// Creation constructor. Creates a bitmap pattern brush, using the bitmap passed in.
	//@@parm HBITMAP | hbmp | Bitmap pattern
	//@@comm 
	// For more information on the parameters, see CreatePatternBrush your Windows PlatformSDK documentation.
	CGDIBrush(HBITMAP hbmp)
	{
		Attach(::CreatePatternBrush(hbmp));
	}


	//@cmember,mfunc CGDIBrush& | operator = | (CGDIBrush& rhs) |
	// Copies the contents of the passed object into this instance. Ownership is always
	// transfered if it belongs to the object being copied.
	//@@parm CGDIBrush& | rhs | Object to copy from.
	//@@rdesc Returns this instance.
	CGDIBrush& operator=(const CGDIBrush& rhs)
	{
		CGDIObject<HBRUSH>::operator=(rhs);
		return *this;
	}


public:
	// GDI Brush operations

	//@cmember
	/* Create a solid brush */
	bool CreateSolidBrush(COLORREF crColor);

	//@cmember
	/* Creates a hatched pattern brush */
	bool CreateHatchBrush(int nIndex, COLORREF crColor);

	//@cmember
	/* Creates a brush using the LOGBRUSH structure */
	bool CreateBrushIndirect(const LOGBRUSH* lpLogBrush);

	//@cmember
	/* Creates a bitmap pattern brush */
	bool CreatePatternBrush(CGDIBitmap& aBitmap);
	//@cmember
	/* Creates a DIB pattern brush */
	bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);

	//@cmember
	/* Creates a DIB pattern brush */
	bool CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage);

	//@cmember
	/* Creates a solid brush using a color in the system palette */
	bool CreateSysColorBrush(int nIndex);

	//@cmember
	/* Retrieves the LOGBRUSH structure associated with this brush */
	int GetLogBrush(LOGBRUSH* pLogBrush);
};


inline
//@mfunc
// Creates a solid brush and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreateSolidBrush(COLORREF crColor)
{
	ASSERT(!m_hObject);
	Attach(::CreateSolidBrush(crColor));
	return m_hObject!=NULL;
}

inline
//@mfunc
// Creates a hatched pattern brush and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreateHatchBrush(int nIndex, COLORREF crColor)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateHatchBrush(nIndex, crColor));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc
// Creates a brush using the LOGBRUSH structure and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateBrushIndirect(lpLogBrush));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc
// Creates a new brush with a bitmap pattern and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreatePatternBrush(CGDIBitmap& aBitmap)
{
	ASSERT(!m_hObject);
	Attach(::CreatePatternBrush(aBitmap));
	return m_hObject != NULL;
}

inline
//@mfunc
// Creates a new brush with a bitmap pattern and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateDIBPatternBrush(hPackedDIB, nUsage));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc
// Creates a new brush with a bitmap pattern and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateDIBPatternBrush((void *)lpPackedDIB, nUsage));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc
// Creates a solid brush using a color in the system palette.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIBrush::CreateSysColorBrush(int nIndex)
{
	ASSERT(!m_hObject);
	Attach(::GetSysColorBrush(nIndex));
	return m_hObject != NULL;
}

inline
//@mfunc
// Retrieves the LOGBRUSH structure associated with this brush.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIBrush::GetLogBrush(LOGBRUSH* pLogBrush)
{
	ASSERT(!m_hObject);
	return ::GetObject((HBRUSH)m_hObject, sizeof(LOGBRUSH), pLogBrush);
}




/////////////////////////////
// CGDIFont class


//@class CGDIFont |
// Encapsulates a GDI font object.
//@base public | CGDIObject<lt>HFONT<gt>
class CGDIFont: 
	public CGDIObject<HFONT>
{
public:

	//@cmember,mfunc | CGDIFont | (HFONT h = NULL, bool bOwn = true) |
	// Attach constructor. Initializes a new instance of CGDIFont and 
	// optionally attaches a previously existing font handle. The ownership
	// of this handle is determined by the bOwn parameter.
	//@@parm HFONT | h | Handle to existing GDI font, or NULL if the object is being
	// created in detached state.
	//@@parm bool | bOwn | Ownership flag. If true, this instance takes ownership of the
	// handle, otherwise the instance just wraps the handle but does not control its life time.
	CGDIFont(HFONT h = NULL, bool bOwn = true): CGDIObject<HFONT>(h, bOwn)
	{}

	//@cmember,mfunc | CGDIFont | (CGDIFont& rhs, bool bTransferOwnership = true) |
	// Copy constructor. Takes an existing CGDIFont instance and copies its contents to the
	// new instance being constructed. If the object being copied is the owner of the handle,
	// ownership's transference is controlled by the parameter bTransferOwnership.
	//@@parm CGDIFont& | rhs | Object to copy from.
	//@@parm bool | bTransferOwnership | If true and the object reference by rhs has ownership of the
	// handle it wraps, ownership is transferred to the new instance.
	CGDIFont(const CGDIFont& rhs, bool bTransferOwnership = true): CGDIObject<HFONT>(rhs, bTransferOwnership)
	{}

	//@cmember,mfunc CGDIFont& | operator = | (CGDIFont& rhs) |
	// Copies the contents of the passed object into this instance. Ownership is always
	// transfered if it belongs to the object being copied.
	//@@parm CGDIFont& | rhs | Object to copy from.
	//@@rdesc Returns this instance.
	CGDIFont& operator=(const CGDIFont& rhs)
	{
		CGDIObject<HFONT>::operator=(rhs);
		return *this;
	}

public:
	// GDI Font operations

	//@cmember
	/* Creates a font with the parameters given in the LOGFONT structure */
	bool CreateFontIndirect(const LOGFONT* lpLogFont);

	//@cmember
	/* Creates a font with the parameters passed in */
	bool CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight, 
		BYTE bItalic, BYTE bUnderline,BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
		BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,LPCTSTR lpszFacename);

	//@cmember
	/* Creates a font using the logical size passed in */
	bool CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hdc = NULL);

	//@cmember
	/* Creates a font using the logical size passed in */
	bool CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdc = NULL);

	//@cmember
	/* Retrieve the LOGFONT structure that describes this font object */
	int GetLogFont(LOGFONT* pLogFont);
};


inline
//@mfunc 
// Creates a font with the parameters given in the LOGFONT structure.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIFont::CreateFontIndirect(const LOGFONT* lpLogFont)
{	
	ASSERT(!m_hObject);
	m_hObject = ::CreateFontIndirect(lpLogFont);
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a font with the parameters passed in.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIFont::CreateFont(int nHeight, int nWidth, int nEscapement,
			 int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
			 BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
			 BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
			 LPCTSTR lpszFacename)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = nHeight;
	lf.lfWidth = nWidth;
	lf.lfEscapement = nEscapement;
	lf.lfOrientation = nOrientation;
	lf.lfWeight = nWeight;
	lf.lfItalic = bItalic;
	lf.lfUnderline = bUnderline;
	lf.lfStrikeOut = cStrikeOut;
	lf.lfCharSet = nCharSet;
	lf.lfOutPrecision = nOutPrecision;
	lf.lfClipPrecision = nClipPrecision;
	lf.lfQuality = nQuality;
	lf.lfPitchAndFamily = nPitchAndFamily;
#ifndef NONSTANDARD_GDI
	_tcsncpy(lf.lfFaceName, lpszFacename, _SFLMIN(31u, _tcslen(lpszFacename)));
#else // if NONSTANDARD_GDI
	wcsncpy(lf.lfFaceName, lpszFacename, _SFLMIN(31, wcslen(lpszFacename)));
#endif //NONSTANDARD_GDI

	Attach(::CreateFontIndirect(&lf));
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a font using the logical size passed in.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIFont::CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hdc)
{
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = nPointSize;
#ifndef NONSTANDARD_GDI
	_tcsncpy(logFont.lfFaceName, lpszFaceName, _tcslen(logFont.lfFaceName));
#else // if NONSTANDARD_GDI
	wcsncpy(logFont.lfFaceName, lpszFaceName, wcslen(logFont.lfFaceName));
#endif //NONSTANDARD_GDI

	return CreatePointFontIndirect(&logFont, hdc);
}

inline
//@mfunc 
// Creates a font using the logical size passed in.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIFont::CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdc)
{
#ifndef NONSTANDARD_GDI
	bool bOwnDC = false;
	if (hdc == NULL) {
		hdc = ::GetDC(NULL);
		bOwnDC = true;
	}

	// convert nPointSize to logical units based on pDC
	LOGFONT logFont = *lpLogFont;
	POINT pt;
	pt.y = ::GetDeviceCaps(hdc, LOGPIXELSY) * logFont.lfHeight;
	pt.y /= 720;    // 72 points/inch, 10 decipoints/point
	::DPtoLP(hdc, &pt, 1);
	POINT ptOrg = { 0, 0 };
	::DPtoLP(hdc, &ptOrg, 1);
	logFont.lfHeight = -abs(pt.y - ptOrg.y);

	if (bOwnDC) {
		ReleaseDC(NULL, hdc);
	}

	return CreateFontIndirect(&logFont);
#else // if NONSTANDARD_GDI
	return false;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc 
// Retrieve the LOGFONT structure that describes this font object.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIFont::GetLogFont(LOGFONT* pLogFont)
{
	ASSERT(m_hObject);
	return ::GetObject((HFONT)m_hObject, sizeof(LOGFONT), pLogFont);
}




/////////////////////////////
// CGDIRgn class

//@class CGDIRgn |
// Encapsulates a GDI region object.
//@base public | CGDIObject<lt>HRGN<gt>
class CGDIRgn: 
	public CGDIObject<HRGN>
{
public:
	
	//@cmember,mfunc | CGDIRgn | (HRGNh = NULL, bool bOwn = true) |
	// Attach constructor. Initializes a new instance of CGDIRgn and 
	// optionally attaches a previously existing bitmap handle. The ownership
	// of this handle is determined by the bOwn parameter.
	//@@parm HRGN | h | Handle to existing GDI region, or NULL if the object is being
	// created in detached state.
	//@@parm bool | bOwn | Ownership flag. If true, this instance takes ownership of the
	// handle, otherwise the instance just wraps the handle but does not control its life time.
	CGDIRgn(HRGN h = NULL, bool bOwn = true): CGDIObject<HRGN>(h, bOwn)
	{}

	//@cmember,mfunc | CGDIRgn | (CGDIRgn& rhs, bool bTransferOwnership = true) |
	// Copy constructor. Takes an existing CGDIRgn instance and copies its contents to the
	// new instance being constructed. If the object being copied is the owner of the handle,
	// ownership's transference is controlled by the parameter bTransferOwnership.
	//@@parm CGDIRgn& | rhs | Object to copy from.
	//@@parm bool | bTransferOwnership | If true and the object reference by rhs has ownership of the
	// handle it wraps, ownership is transferred to the new instance.
	CGDIRgn(const CGDIRgn& rhs, bool bTransferOwnership = true): CGDIObject<HRGN>(rhs, bTransferOwnership)
	{}

	//@cmember,mfunc CGDIRgn& | operator = | (CGDIRgn& rhs) |
	// Copies the contents of the passed object into this instance. Ownership is always
	// transfered if it belongs to the object being copied.
	//@@parm CGDIRgn& | rhs | Object to copy from.
	//@@rdesc Returns this instance.
	CGDIRgn& operator=(const CGDIRgn& rhs)
	{
		CGDIObject<HRGN>::operator=(rhs);
		return *this;
	}


public:
	// GDI Region operations

	//@cmember
	/* Creates a rectangular region */
	bool CreateRectRgn(int x1, int y1, int x2, int y2);

	//@cmember
	/* Creates a rectangular region */
	bool CreateRectRgnIndirect(LPCRECT lpRect);

	//@cmember
	/* Creates an elliptical region */
	bool CreateEllipticRgn(int x1, int y1, int x2, int y2);

	//@cmember
	/* Creates an elliptical region */
	bool CreateEllipticRgnIndirect(LPCRECT lpRect);

	//@cmember
	/* Creates a polygonal region */
	bool CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);

	//@cmember
	/* Creates a polygonal region */
	bool CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,	int nCount, int nPolyFillMode);

	//@cmember
	/* Creates a round rectangular region */
	bool CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);

	//@cmember
	/* Creates a region from the path selected in the given DC */
	bool CreateFromPath(HDC hdc);

	//@cmember
	/* Creates a region from the given region and transformation data */
	bool CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData);

	//@cmember
	/* Changes the region to a rectangular region with the specified coordinates */
	void SetRectRgn(int x1, int y1, int x2, int y2);

	//@cmember
	/* Changes the region to a rectangular region with the specified coordinates */
	void SetRectRgn(LPCRECT lpRect);

	//@cmember
	/* Combines the two given regions using the given combine operation */
	int CombineRgn(CGDIRgn& aRgn1, CGDIRgn& aRgn2, int nCombineMode);

	//@cmember
	/* Copy the region passed in into this region */
	int CopyRgn(CGDIRgn& aRgnSrc);

	//@cmember
	/* Compares the region passed in to this region */
	bool EqualRgn(CGDIRgn& aRgn) const;

	//@cmember
	/* Displaces this region */
	int OffsetRgn(int x, int y);

	//@cmember
	/* Displaces this region */
	int OffsetRgn(POINT point);

	//@cmember
	/* Smallest rectangle that contains the entire region */
	int GetRgnBox(LPRECT lpRect) const;

	//@cmember
	/* Determines whether a given point is contained within this region */
	bool PtInRegion(int x, int y) const;

	//@cmember
	/* Determines whether a given point is contained within this region */
	bool PtInRegion(POINT point) const;

	//@cmember
	/* Determines whether a given rectangle is contained within this region */
	bool RectInRegion(LPCRECT lpRect) const;

	//@cmember
	/* Returns the region information encapsulated in a RGNDATA parameter */
	int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;
};


inline
//@mfunc 
// Creates a rectangular region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateRectRgn(int x1, int y1, int x2, int y2)
{
	ASSERT(!m_hObject);
	Attach(::CreateRectRgn(x1, y1, x2, y2));
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a rectangular region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateRectRgnIndirect(LPCRECT lpRect)
{
	ASSERT(!m_hObject);
	Attach(::CreateRectRgnIndirect(lpRect));
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates an elliptical region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateEllipticRgn(x1, y1, x2, y2));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates an elliptical region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateEllipticRgnIndirect(LPCRECT lpRect)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateEllipticRgnIndirect(lpRect));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a polygonal region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreatePolygonRgn(lpPoints, nCount, nMode));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a polygonal region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,	int nCount, int nPolyFillMode)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a round rectangular region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a region from the path selected in the given DC.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateFromPath(HDC hdc)
{
	ASSERT(!m_hObject);
	ASSERT(hdc);
#ifndef NONSTANDARD_GDI
	Attach(::PathToRegion(hdc));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Creates a region from the given region and transformation data.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	Attach(::ExtCreateRegion(lpXForm, nCount, pRgnData));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc 
// Changes the region to a rectangular region with the specified coordinates.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
void CGDIRgn::SetRectRgn(int x1, int y1, int x2, int y2)
{
	ASSERT(m_hObject);
	::SetRectRgn((HRGN)m_hObject, x1, y1, x2, y2);
}

inline
//@mfunc 
// Changes the region to a rectangular region with the specified coordinates.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
void CGDIRgn::SetRectRgn(LPCRECT lpRect)
{
	ASSERT(m_hObject);
	SetRectRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}

inline
//@mfunc 
// Combines the two given regions using the given combine operation.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIRgn::CombineRgn(CGDIRgn& aRgn1, CGDIRgn& aRgn2, int nCombineMode)
{
	ASSERT(m_hObject);
	return ::CombineRgn(reinterpret_cast<HRGN>(m_hObject), aRgn1, aRgn2, nCombineMode);
}

inline
//@mfunc 
// Copy the region passed in into this region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIRgn::CopyRgn(CGDIRgn& aRgnSrc)
{
	ASSERT(m_hObject);
	return ::CombineRgn(reinterpret_cast<HRGN>(m_hObject), aRgnSrc, NULL, RGN_COPY);
}

inline
//@mfunc 
// Compares the region passed in to this region.
//@rdesc true if both regions are equal, false otherwise.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::EqualRgn(CGDIRgn& aRgn) const
{
	ASSERT(m_hObject);
	return ::EqualRgn(reinterpret_cast<HRGN>(m_hObject), aRgn)? true : false;
}

inline
//@mfunc 
// Displaces this region by the amounts given.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIRgn::OffsetRgn(int x, int y)
{
	ASSERT(m_hObject);
	return ::OffsetRgn(reinterpret_cast<HRGN>(m_hObject), x, y);
}

inline
//@mfunc 
// Displaces this region by the amounts represented in the POINT structure.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIRgn::OffsetRgn(POINT point)
{
	ASSERT(m_hObject);
	return ::OffsetRgn(reinterpret_cast<HRGN>(m_hObject), point.x, point.y);
}

inline
//@mfunc 
// Returns the smallest rectangle that contains the entire region.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIRgn::GetRgnBox(LPRECT lpRect) const
{
	ASSERT(m_hObject);
	return ::GetRgnBox(reinterpret_cast<HRGN>(m_hObject), lpRect);
}

inline
//@mfunc 
// Determines whether a given point is contained within this region.
//@rdesc true if the point is contained in the region, false otherwise.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::PtInRegion(int x, int y) const
{
	ASSERT(m_hObject);
	return ::PtInRegion(reinterpret_cast<HRGN>(m_hObject), x, y)? true : false;
}

inline
//@mfunc 
// Determines whether a given point is contained within this region.
//@rdesc true if the point is contained in the region, false otherwise.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::PtInRegion(POINT point) const
{
	ASSERT(m_hObject);
	return ::PtInRegion(reinterpret_cast<HRGN>(m_hObject), point.x, point.y)? true : false;
}

inline
//@mfunc 
// Determines whether a given rectangle is entirely contained within this region.
//@rdesc true if the point is contained in the region, false otherwise.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIRgn::RectInRegion(LPCRECT lpRect) const
{
	ASSERT(m_hObject);
	return ::RectInRegion(reinterpret_cast<HRGN>(m_hObject), lpRect)? true : false;
}

inline
//@mfunc 
// Returns the region information encapsulated in a RGNDATA parameter.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIRgn::GetRegionData(LPRGNDATA lpRgnData, int nCount) const
{
	ASSERT(m_hObject);
	return ::GetRegionData(reinterpret_cast<HRGN>(m_hObject), nCount, lpRgnData);
}



//@class CGDIPalette |
// Encapsulates a GDI palette object.
//@base public | CGDIObject<lt>HPALETTE<gt>
class CGDIPalette: 
	public CGDIObject<HPALETTE>
{
public:

	//@cmember,mfunc | CGDIPalette | (HPALETTE h = NULL, bool bOwn = true) |
	// Attach constructor. Initializes a new instance of CGDIPalette and 
	// optionally attaches a previously existing palette handle. The ownership
	// of this handle is determined by the bOwn parameter.
	//@@parm HPALETTE | h | Handle to existing GDI palette, or NULL if the object is being
	// created in detached state.
	//@@parm bool | bOwn | Ownership flag. If true, this instance takes ownership of the
	// handle, otherwise the instance just wraps the handle but does not control its life time.
	CGDIPalette(HPALETTE h, bool bOwn = true): CGDIObject<HPALETTE>(h, bOwn)
	{}

	//@cmember,mfunc | CGDIPalette | (CGDIPalette& rhs, bool bTransferOwnership = true) |
	// Copy constructor. Takes an existing CGDIPalette instance and copies its contents to the
	// new instance being constructed. If the object being copied is the owner of the handle,
	// ownership's transference is controlled by the parameter bTransferOwnership.
	//@@parm CGDIPalette& | rhs | Object to copy from.
	//@@parm bool | bTransferOwnership | If true and the object reference by rhs has ownership of the
	// handle it wraps, ownership is transferred to the new instance.
	CGDIPalette(const CGDIPalette& rhs, bool bTransferOwnership = true): CGDIObject<HPALETTE>(rhs, bTransferOwnership)
	{}

	~CGDIPalette()
	{}

	//@cmember,mfunc CGDIPalette& | operator = | (CGDIPalette& rhs) |
	// Copies the contents of the passed object into this instance. Ownership is always
	// transfered if it belongs to the object being copied.
	//@@parm CGDIPalette& | rhs | Object to copy from.
	//@@rdesc Returns this instance.
	CGDIPalette& operator=(const CGDIPalette& rhs)
	{
		CGDIObject<HPALETTE>::operator=(rhs);
		return *this;
	}

public:
	// GDI Palette operations

	//@cmember
	/* Creates a new palette */
	bool CreatePalette(LPLOGPALETTE lpLogPalette) ;

	//@cmember
	/* Creates a new halftone palette */
	bool CreateHalftonePalette(HDC hdc) ;

	//@cmember
	/* Retrieves number of palette entries */
	int GetEntryCount() ;

	//@cmember
	/* Retrieves a specified range of entries in the palette */
	UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const ;

	//@cmember
	/* Set the values of a specified range of entries in the palette */
	UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) ;

	//@cmember
	/* Replaces entries in a specified range of the palette */
	void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) ;

	//@cmember
	/* Retrieves the closest match to the given color  */
	UINT GetNearestPaletteIndex(COLORREF crColor) const ;

	//@cmember
	/* Changes the number of entries in the palette */
	bool ResizePalette(UINT nNumEntries) ;
};


inline
//@mfunc
// Creates a palette using the parameters in the LOGPALETTE structure and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIPalette::CreatePalette(LPLOGPALETTE lpLogPalette) 
{
	ASSERT(!m_hObject);
	Attach(::CreatePalette(lpLogPalette));
	return m_hObject != NULL;
}

inline
//@mfunc
// Creates a halftone palette for the given device context and attaches it to this instance.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIPalette::CreateHalftonePalette(HDC hdc) 
{
	ASSERT(!m_hObject);
#ifndef NONSTANDARD_GDI
	ASSERT(hdc);
	Attach(::CreateHalftonePalette(hdc));
#endif //NONSTANDARD_GDI
	return m_hObject != NULL;
}

inline
//@mfunc
// Retrieves the number of entries in this palette.
//@rdesc Number of entries in this palette.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
int CGDIPalette::GetEntryCount() 
{
	ASSERT(m_hObject);
	int nEntries;
	::GetObject(m_hObject, sizeof(int), &nEntries);
	return nEntries;
}

inline
//@mfunc
// Retrieves a specified range of entries in the palette.
//@rdesc Number of entries retrieved from the palette.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
UINT CGDIPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const 
{
	ASSERT(m_hObject);
	return ::GetPaletteEntries(reinterpret_cast<HPALETTE>(m_hObject), nStartIndex, nNumEntries, lpPaletteColors);
}

inline
//@mfunc
// Set the values of a specified range of entries in the palette.
//@rdesc Number of entries that were set in the palette.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
UINT CGDIPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) 
{
	ASSERT(m_hObject);
	return ::SetPaletteEntries(reinterpret_cast<HPALETTE>(m_hObject), nStartIndex, nNumEntries, lpPaletteColors);
}

inline
//@mfunc
// Replaces entries in a specified range of the palette.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
void CGDIPalette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) 
{
	ASSERT(m_hObject!=NULL);
#ifndef NONSTANDARD_GDI
	::AnimatePalette((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
#else // if NONSTANDARD_GDI
#endif //NONSTANDARD_GDI
}

inline
//@mfunc
// Retrieves the closest match to the given color of all entries in the palette.
//@rdesc Index of closest match entry.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
UINT CGDIPalette::GetNearestPaletteIndex(COLORREF crColor) const 
{
#ifndef NONSTANDARD_GDI
	ASSERT(m_hObject!=NULL);
	return ::GetNearestPaletteIndex(reinterpret_cast<HPALETTE>(m_hObject), crColor);
#else // if NONSTANDARD_GDI
	return 0;
#endif //NONSTANDARD_GDI
}

inline
//@mfunc
// Changes the number of entries in the palette.
//@comm
// For more information on this routine, see your Windows PlatformSDK documentation.
//@end
bool CGDIPalette::ResizePalette(UINT nNumEntries) 
{
#ifndef NONSTANDARD_GDI
	ASSERT(m_hObject!=NULL);
	return ::ResizePalette(reinterpret_cast<HPALETTE>(m_hObject), nNumEntries)? true : false;
#else // if NONSTANDARD_GDI
	return false;
#endif //NONSTANDARD_GDI
}




}; // foundation
}; // stingray

#endif // #ifndef __GDIOBJECTS_H__
