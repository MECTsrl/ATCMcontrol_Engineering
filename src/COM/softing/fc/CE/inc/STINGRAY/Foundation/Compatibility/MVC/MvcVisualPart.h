/////////////////////////////////////////////////////////////////////////////
// MvcVisualPart.h : Declaration of MvcVisualPart
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
// Description:  Declaration of MvcVisualPart interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcVisualPart.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcVisualPart

#ifndef __COMPAT_MVCVISUALPART_H__
#define __COMPAT_MVCVISUALPART_H__

// Includes

//#include "..\Patterns\Collect.h"


#ifndef __COMPAT_MVCVISUALCOMPONENT_H__
#include "Foundation\Compatibility\MVC\MvcVisualComponent.h"
#endif

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
//  MvcVisualPart
//
//@doc MvcVisualPart
//@mdata MvcVisualPart* | MvcVisualPart | m_pContainer | Pointer to the visual part that contains this visual part
//@xref <c MvcVisualPart>

//@class
// An MvcVisualPart is a type of visual component that keeps track of
// its container. Knowledge of its parent allows a visual part to perform 
// invalidation and establish its own size based on the size of its container.
//
// Because of its extra member variables, a visual part 
// is a slightly heavier object than the MvcVisualComponent class.  
// If you require many instantiations of a display
// object, you should probably derive each from MvcVisualComponent and the
// container from MvcVisualPart, MvcLogicalPart or MvcViewport.
//
//@base public | MvcVisualComponent
//

class MvcVisualPart : public MvcVisualComponent
{
// Constructor(s) & destructor
public:
	MvcVisualPart(MvcVisualPart* pContainer = NULL);
	virtual ~MvcVisualPart();

	//@cmember
	/* Create a visual part as a child of the specified container*/
	virtual BOOL Create(MvcVisualPart* pContainer = NULL);

// Attributes
protected:

	//@cmember
	/* Pointer to the visual part that contains this visual part*/
	MvcVisualPart* m_pContainer;

// Accessors
public:

	//@cmember
	/* Retrieve the container of this visual part*/
	MvcVisualPart* GetContainer();

	//@cmember
	/* Set the container of this visual part*/
	virtual void SetContainer(MvcVisualPart* pContainer);

// Operations
public:

	// Invalidation

	//@cmember
	/* Invalidate the entire visual part*/
	virtual void Invalidate(BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	//@cmember
	/* Invalidate the specified component's entire rectangle*/
	virtual void InvalidateVisual(MvcVisualComponent* pVC, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	//@cmember
	/* Invalidate the specified rectangle within this visual part*/
	virtual void InvalidateRect(const CRect& rcLog, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

// Overridables
public:

	//@cmember
	/* An overridable for adding additional creation or initialization logic.*/
	virtual BOOL OnCreate();
};

///////////////////////////////////////////////////////////////////////////

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __COMPAT_MVCVISUALPART_H__

