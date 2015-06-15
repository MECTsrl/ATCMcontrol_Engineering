/////////////////////////////////////////////////////////////////////////////
// MvcCompositeViewport.cpp : Implementation of MvcCompositeViewport
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//  Author:       Brad King
//  Description:  Implementation of MvcCompositeViewport
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcCompositeViewport.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcCompositeViewport

#ifndef __COMPAT_MVCCOMPOSITE_VIEWPORT_H__
#define __COMPAT_MVCCOMPOSITE_VIEWPORT_H__


// Header Includes
#ifndef __COMPAT_MVCVIEWPORT_H__
#include "Foundation\Compatibility\MVC\MvcViewport.h"
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

/////////////////////////////////////////////////////////////////////////////
// MvcCompositeViewport

//@doc MvcCompositeViewport
//@mdata ComponentSet | MvcCompositeViewport | m_setComponents | The internal collection declaration
//@xref <c MvcCompositeViewport>

//@doc MvcCompositeViewport
//@mfunc void  | MvcCompositeViewport | AddComponent | Adds a child component to the composite
//@parm MvcVisualComponent* | pComponent | A pointer to the visual component to add.
//@xref <c MvcCompositeViewport>

//@doc MvcCompositeViewport
//@mfunc void  | MvcCompositeViewport | RemoveComponent | Removes a component from the composite
//@parm MvcVisualComponent* | pComponent | A pointer to the visual component to remove. The object itself will not be deleted.
//@xref <c MvcCompositeViewport>

//@doc MvcCompositeViewport
//@mfunc void  | MvcCompositeViewport | CreateComponents | Creates child components. Called by SetModel
//@parm MvcVisualComponent* | pComponent | A pointer to the visual component to remove. The object itself will not be deleted.
//@xref <c MvcCompositeViewport>

//@doc MvcCompositeViewport
//@class This class simplifies the use of child components by providing a default
// collection implementation. Children can be added or removed from the collection,
// and each child will have it's Draw function called during the painting process.
// A function for hit testing child components in the composite helps controllers
// determine appropriate commands for child component actions. 
//
//@base public | MvcViewport

class MvcCompositeViewport : public MvcViewport
{
public:
	typedef CCArray_T<MvcVisualComponent*,MvcVisualComponent*> ComponentSet;

	MvcCompositeViewport();
	virtual ~MvcCompositeViewport();

public:
	// MvcViewport overrides
	//@cmember
	/* Draws this viewport and calls Draw for all children in the composite*/
	virtual void Draw(CDC* pDC);
	//@cmember
	/* Sets the model that this viewport will observe*/
	virtual void SetModel(MvcModel* pModel);
	//@cmember
	/* Determines the child component that contains a point*/
	virtual MvcVisualComponent* HitTestComponent( CPoint pt );
	//@cmember
	/* Adds a child component to the composite*/
	virtual int AddComponent(MvcVisualComponent* pComponent){ return m_setComponents.Add(pComponent);}
	//@cmember
	/* Removes a child component from the composite*/
	virtual void RemoveComponent(MvcVisualComponent* pComponent){ m_setComponents.Remove(pComponent);}
	//@cmember
	/* Returns the collection of children in the composite viewport*/
	inline ICollection_T<MvcVisualComponent*>* GetComponents();

protected:
	//@cmember
	/* Override this function to initially create child components. Called by SetModel()*/
	virtual void CreateComponents(){;}
	//@cmember
	/* Deletes all child components and empties the collection*/
	virtual void DestroyComponents();


protected:
	//@cmember
	/* The internal collection declaration*/
	ComponentSet m_setComponents;
};

//@doc MvcCompositeViewport
//@mfunc Returns the collection of children in the composite viewport
//@rdesc The collection of children
inline ICollection_T<MvcVisualComponent*>* MvcCompositeViewport::GetComponents()
{
	return &m_setComponents;
}

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif	//__MVCCOMPOSITE_VIEWPORT_H__