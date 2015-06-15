/////////////////////////////////////////////////////////////////////////////
// MvcVisualPart.h : SFL Model-View-Controller architecture 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  SFL MVC: Declaration of Visual Part class
//

#pragma once

#ifndef __MVCVISUALPART_H__
#define __MVCVISUALPART_H__

#include <Foundation\Mvc\MvcVisualComponent.h>
#include <Foundation\StructuredGraphics\VisualHost.h>

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc CMvcVisualPart
//
//@class
// A CMvcVisualPart is a type of visual component that keeps track of
// its container. Knowledge of its parent allows a visual part to perform 
// invalidation and establish its own size based on the size of its container.
//
// Because of its extra member variables, a visual part 
// is a slightly heavier object than the CMvcVisualComponent class.  
// If you require many instantiations of a display
// object, you should probably derive each from CMvcVisualComponent and the
// container from CMvcVisualPart, CMvcLogicalPart or CMvcViewport.
//
//@tcarg class | _Base | Base visual class.
//@tcarg class | _Container | Container type.
//
//@base public | _Base
//

template <class _Base, class _Container>
class CMvcVisualPart : public _Base
{
// Embedded types
public:
	typedef CMvcVisualPart<_Base, _Container> _ThisClass;
	typedef _Base _BaseClass;
	typedef _Container _ContainerClass;

// Constructor(s) & destructor
public:

	//@cmember,mfunc Construct a visual part object
	//@@parmopt MvcVisualPart* | pContainer | NULL | Pointer to the container for the object
	//@@end
	/* Construct a visual part object*/
	CMvcVisualPart(_ContainerClass* pContainer = NULL) :
		m_pContainer(pContainer)
	{
	}

// Attributes
protected:
	//@cmember
	/* Pointer to the object that contains the visual part*/
	_ContainerClass* m_pContainer;

// Operations
public:

	//@cmember,mfunc Create a visual part as a child of the specified container
	//@@rdesc Returns TRUE if the call was successful.
	//@@parmopt MvcVisualPart* | pContainer | NULL | A pointer to the container of this part.
	//@@comm 
	// Initializes the the container property of this visual part
	// by calling SetContainer.
	//@@xref <mf CMvcVisualPart::SetContainer>
	//@@end
	/* Create a visual part as a child of the specified container*/
	virtual BOOL Create(_ContainerClass* pContainer = NULL)
	{
		SetContainer(pContainer);
		return OnCreate();
	}

	//@cmember,mfunc Returns a pointer to the visual object that contains the visual part
	//@@rdesc A pointer to the container object
	//@@comm 
	// A visual part is a visual component that maintains a back pointer to
	// its containing visual object.  This function reports that back pointer.
	//@@end
	/* Retrieve the container of this visual part*/
	_ContainerClass* GetContainer()
	{
		return m_pContainer;
	}

	//@cmember,mfunc Sets a pointer to the visual part that contains the visual part
	//@@parmopt MvcVisualPart* | pContainer | NULL |  A pointer to the container of this part.
	//@@comm
	// A visual part is a visual component that maintains a back pointer to
	// its containing visual object.  This function modifies that back pointer.
	//@@end
	/* Set the container of this visual part*/
	virtual void SetContainer(_ContainerClass* pContainer)
	{
		m_pContainer = pContainer;
	}

	//@cmember,mfunc Invalidates the specified rectangle of this component in logical coordinates
	//@@rdesc void
	//@@parm const CRect& | rect | Specifies the rectangle to invalidate, in logical coordinates
	//@@parmopt BOOL | bErase | TRUE | Should the visual part be completely erased prior to redraw
	//@@parmopt BOOL | bRepairNow | FALSE | Repair the invalid area immediately
	//@@comm
	// This function delegates to it's container, if it has one. If this component does not
	// have a container, then the call has no effect.
	//@@xref <mf CMvcVisualPart::Invalidate>
	//@@end
	/* Invalidate the specified rectangle within this visual part*/
	virtual void InvalidateRect(const CRect& rect, BOOL bErase = TRUE, BOOL bRepairNow = FALSE)
	{
		if (m_pContainer)
		{
			m_pContainer->InvalidateRect(rect, bErase, bRepairNow);
		}
	}

	//@cmember,mfunc Validates the specified rectangle of this component in logical coordinates
	//@@rdesc void
	//@@parm const CRect& | rect | Specifies the rectangle to validate, in logical coordinates
	//@@comm
	// This function delegates to it's container, if it has one. If this component does not
	// have a container, then the call has no effect.
	//@@xref <mf CMvcVisualPart::InvalidateRect>
	//@@end
	/* Validate the specified rectangle within this visual part*/
	virtual void ValidateRect(const CRect& rect)
	{
		if (m_pContainer)
		{
			m_pContainer->ValidateRect(rect);
		}
	}

	//@cmember,mfunc Invalidates this component's entire rectangle.
	//@@rdesc void
	//@@parmopt BOOL | bErase | TRUE | Should the visual part be completely erased prior to redraw
	//@@parmopt BOOL | bRepairNow | FALSE | Repair the invalid area immediately
	//@@comm
	// This function iterates up to the topmost parent in order to
	// map the visual parts bounds to device coordinates and then
	// it invalidates the rectangle occupied by the visual part.
	//@@xref <mf CMvcVisualPart::InvalidateRect>
	//@@end
	/* Invalidate the entire visual part*/
	virtual void Invalidate(BOOL bErase = TRUE, BOOL bRepairNow = FALSE)
	{
		InvalidateVisual(this, bErase, bRepairNow);
	}

	//@cmember,mfunc Invalidates the specified visual component's entire rectangle.
	//@@rdesc void
	//@@parm MvcVisualComponent* | pVisual | A pointer to a component to invalidate
	//@@parmopt BOOL | bErase | TRUE | Should the visual part be completely erased prior to redraw
	//@@parmopt BOOL | bRepairNow | FALSE | Repair the invalid area immediately
	//@@comm
	// This function invalidates the entire visual component rectangle of the
	// component pointed to by pVisual.
	//@@xref <mf CMvcVisualPart::InvalidateRect>
	//@@end
	/* Invalidate the specified component's entire rectangle*/
	virtual void InvalidateVisual(IVisual* pVisual, BOOL bErase = TRUE, BOOL bRepairNow = FALSE)
	{
		IBounds2D* pIBounds = guid_cast<IBounds2D*>(pVisual);
		if (pIBounds != NULL)
		{
			InvalidateRect(pIBounds->GetBounds(), bErase, bRepairNow);
		}
	}

// Overridables
public:
	//@cmember,mfunc Called after the visual part has been created to allow
	// the creation and/or initialization of additional children
	//@@rdesc Returns TRUE if the call was successful
	//@@comm
	// This function is called by <mf CMvcVisualPart::Create> after it
	// has finished creating this visual part.  If you have additional
	// members to create or initialize, you can do so by overriding this
	// member in your derived classes.  The difference between this
	// function and OnInitialUpdate is the latter is called later
	// in the construction and initialization process.  By the time
	// OnInitialUpdate is called, all children should be created
	// and, if this is a viewport, the model will be set.  This
	// function is called just after creation and cannot depend on
	// the model being ready.  But, if you need to create addition
	// children or sibilings (i.e. scroll bars), override this
	// member and do so there.  You could accomplish the same thing
	// by overriding Create and calling the base class implementation
	// first.  The advantage of overriding this function versus Create
	// is the signature of OnCreate doesn't change whereas Create
	// has a different number of arguments depending on the level of
	// derivation.  Therefore, a wrapper class can't override Create
	// because a wrapper class wouldn't know which prototype to
	// override.
	//@@xref <mf CMvcVisualPart::Create>
	//@@end
	/* An overridable for adding additional creation or initialization logic*/
	virtual BOOL OnCreate()
	{
		return TRUE;
	}
};

//@type CMvcVisualPartImpl | Default instantiation of the <c CMvcVisualPart> template class.
//@iex typedef CMvcVisualPart<stingray::foundation::CMvcVisualComponent,stingray::foundation::IVisualHost> CMvcVisualPartImpl;
//@xref <c CMvcVisualPart>, <c CMvcVisualComponent>, <c IVisualHost>
typedef CMvcVisualPart<stingray::foundation::CMvcVisualComponent,stingray::foundation::IVisualHost> CMvcVisualPartImpl;

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCVISUALPART_H__
