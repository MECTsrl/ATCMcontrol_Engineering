/////////////////////////////////////////////////////////////////////////////
//
// IRefCount.h
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
//
// Author:       Dean Hallman
// Description:  Declaration of IRefCount interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/Common/IRefCount.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MISC

#ifndef __IREFCOUNT_H__
#define __IREFCOUNT_H__

//
// CMN Extension DLL
// Initialize declaration context
//
#ifdef _CMNDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    MVC_DATAEXT
#endif //_CMNDLL


/////////////////////////////////////////////////////////////////////////////
//
// IRefCount
//
//@class 
// IRefCount is an interface that can be mixed into any derivation to allow
// the class instances to be referenced counted.  When the reference count
// reaches zero, the class automatically deletes itself.
// 

class IRefCount
{
protected:
	int m_dwRefCount;

public:
	IRefCount() {
		m_dwRefCount = 0;
	};

	virtual ~IRefCount() {}

	virtual void AddRef() {
		m_dwRefCount++;
	};

	virtual void Release() {
		--m_dwRefCount;
		if (!m_dwRefCount)
			delete this;
	};
};

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __IREFCOUNT_H__
