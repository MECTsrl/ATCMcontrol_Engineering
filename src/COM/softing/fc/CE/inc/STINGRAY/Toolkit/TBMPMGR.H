// 
// Stingray Software Extension Classes
// Copyright (C) 1998Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detaild information
// regarding using SEC classes.
// 
//  Authors:		Don
//  Description:	Declarations for SECBmpMgr
//  Created:		Jan 1998
//

#ifdef WIN32

#ifndef __TBMPMGR_H__
#define __TBMPMGR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL


class SECBmpMgr : public CObject
{
	DECLARE_DYNAMIC(SECBmpMgr)

public:
// Construction
	SECBmpMgr();

	// Functions for maintaining reference count. Note this object should
	// be Release'd and note deleted!
	void AddRef();
	void Release();

// Attributes
public:

// Operations
public:
	// Note all resources loaded are assumed to be the same size
	// Defines a toolbar resource to add to the list of bitmaps managed
	BOOL AddToolbarResource(LPCTSTR lpszSmall, LPCTSTR lpszLarge, BOOL bSysColor = TRUE);
	BOOL AddToolbarResource(UINT nIDSmall, UINT nIDLarge, BOOL bSysColor = TRUE);

	// Defines a bitmap resource to add to the list of bitmaps managed
	BOOL AddBitmapResource(LPCTSTR lpszSmall, LPCTSTR lpszLarge, const UINT* lpIDArray, 
						   UINT nIDCount, BOOL bSysColor = TRUE);
	BOOL AddBitmapResource(UINT nIDSmall, UINT nIDLarge, const UINT* lpIDArray, 
						   UINT nIDCount, BOOL bSysColor = TRUE);

	// Defines a bitmap handle to add to the list of bitmaps managed
	BOOL AddBitmap(HBITMAP hBmpSmall, HBITMAP hBmpLarge, 
				   const UINT* lpIDArray, UINT nIDCount);

	// Ensures all relevant bitmaps are loaded
	BOOL LoadBitmaps(BOOL bAlwaysLoad = TRUE);

	// Returns the bitmap and handle for the given ID.
	BOOL GetButton(UINT nID, HBITMAP& hBmp, UINT& nIndex, BOOL bAlwaysSmall = FALSE);

	// Changes between small and large bitmap mode
	void ChangeMode(BOOL bSmall);

	// Set's keep small bitmap mode (the small bitmap will always be loaded, even when in
	// large bitmap mode)
	void SetKeepSmall(BOOL bKeep);

	// Set's the bitmap sizes (should not be used with AddToolbarResource).
	void SetSmallSize(int nWidth, int nHeight);
	void SetLargeSize(int nWidth, int nHeight);

	// Methods for returning the various bitmaps sizes
	int  GetSmallWidth() const;
	int  GetSmallHeight() const;
	int  GetLargeWidth() const;
	int  GetLargeHeight() const;
	int  GetImgWidth() const;
	int  GetImgHeight() const;

	// Loads the image information from a toolbar resource
	BOOL LoadToolbarInfo(LPCTSTR lpszResourceName, UINT*& pBmpItems, UINT& nBmpItems, 
						 int& nImgWidth, int& nImgHeight, BOOL bSeparators=FALSE);

// Implementation
public:
	virtual ~SECBmpMgr();

	// Structure detailing a particular bitmap
	struct BmpInfo
	{
		BmpInfo();
		~BmpInfo();

		LPCTSTR m_lpszSmallRsrc;			// The small resource name
		LPCTSTR m_lpszLargeRsrc;			// The large resource name
		HBITMAP m_hBmpSmall;				// The small bitmap handle
		HBITMAP m_hBmpLarge;				// The large bitmap handle
		UINT    m_nCount;					// The no. of images in the bitmap
		BOOL    m_bSysColor;				// TRUE if the bitmap should be modified for
											// the system color scheme when loaded.
		BOOL    m_bSmallHandleOnly;			// TRUE if we don't own the small bmp handle
		BOOL    m_bLargeHandleOnly;			// TRUE if we don't own the large bmp handle
	};

	// Structure detailing an individual image
	struct BmpItem
	{
		UINT m_nIndBmpInfo;					// Index within m_arrInfo of the BmpInfo structure
		UINT m_nBmpOffset;					// Offset within bitmap of image
	};

	// Map of ID's to BmpItem structures
	CMap<UINT, UINT, BmpItem*, BmpItem*> m_mapItem;

	// Array of BmpInfo structures
	CTypedPtrArray<CPtrArray, BmpInfo*>  m_arrInfo;
		
	int m_nSmallWidth;						// Width of small bitmap
	int m_nSmallHeight;						// Height of small bitmap
	int m_nLargeWidth;						// Width of large bitmap
	int m_nLargeHeight;						// Height of large bitmap
	BOOL m_bSmall;							// TRUE if we are in small bitmap mode
	BOOL m_bKeepSmall;						// TRUE if the small bitmaps are to always be kept
	ULONG m_nRef;							// Our reference count

protected:

	// Loads the bitmap for an individual BmpInfo item.
	virtual BOOL LoadBitmap(BmpInfo* pInfo, BOOL bAlwaysLoad = TRUE);

	// Loads the small/large bitmap for an individual BmpInfo item.
	virtual BOOL LoadSmall(BmpInfo* pInfo);
	virtual BOOL LoadLarge(BmpInfo* pInfo);

	// Generates BmpItem's for the given ID array
	void AddBmpInfo(BmpInfo* pInfo, const UINT* lpIDArray, UINT nIDCount);
};

/////////////////////////////////////////////////////////////////////////////

inline BOOL SECBmpMgr::AddToolbarResource(UINT nIDSmall, UINT nIDLarge, 
										  BOOL bSysColor /* = TRUE */)
{
	return AddToolbarResource(MAKEINTRESOURCE(nIDSmall),
							  MAKEINTRESOURCE(nIDLarge),
							  bSysColor);
}

inline BOOL SECBmpMgr::AddBitmapResource(UINT nIDSmall, UINT nIDLarge, 
										 const UINT* lpIDArray, UINT nIDCount, 
										 BOOL bSysColor /* = TRUE */)
{
	return AddBitmapResource(MAKEINTRESOURCE(nIDSmall),
							 MAKEINTRESOURCE(nIDLarge),
							 lpIDArray,
							 nIDCount,
							 bSysColor);
}

inline SECBmpMgr::GetSmallWidth() const
{
	return m_nSmallWidth;
}

inline SECBmpMgr::GetSmallHeight() const
{
	return m_nSmallHeight;
}

inline SECBmpMgr::GetLargeWidth() const
{
	return m_nLargeWidth;
}

inline SECBmpMgr::GetLargeHeight() const
{
	return m_nLargeHeight;
}

inline SECBmpMgr::GetImgWidth() const
{
	return m_bSmall ? GetSmallWidth() : GetLargeWidth();
}

inline SECBmpMgr::GetImgHeight() const
{
	return m_bSmall ? GetSmallHeight() : GetLargeHeight();
}

/////////////////////////////////////////////////////////////////////////////

#endif	// __TBMPMGR_H__

#endif	// WIN32
