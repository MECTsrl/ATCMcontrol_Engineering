/////////////////////////////////////////////////////////////////////////////
// MvcPresentationModel.h : Declaration of MvcPresentationModel_T
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
// Description:  Declaration of MvcPresentationModel_T interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcPresentationModel.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcPresentationModel_T

#pragma once

#ifndef __MFC_MVCPRESENTATIONMODEL_H__
#define __MFC_MVCPRESENTATIONMODEL_H__

#include <Foundation\MVC\MvcModel.h>
#include <Foundation\MVC\MFC\MvcIfcs.h>
#include <Foundation\MVC\MFC\MvcTransactionModel.h>

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//
// MvcPresentationModel_T
//
//@class
// A presentation model is a kind of model that models a presentation. Broadly
// speaking, there are two types of models:
//
// o System Models <nl>
// o Presentation Models.
//
// A system model is an MvcModel-derived class that models some
// non-graphical, real-world system or process.  A presentation model models
// a graphical presentation.  System and presentation models can be used
// exclusively or in combination. Used in combination, a presentation model
// provides the presentation for a system model, essentially mapping the
// system into the graphical realm.
//
// A presentation model is a templatized class, which multiply inherits from
// both MvcModel and MvcVisualPart.  The use of multiple inheritance here
// allows the presentation model to function as both a model and a visual -
// in effect, a visual model.  A good example of where this idea is useful
// is in the implementation of a diagramming application.  It is very natural
// to implement a diagram class as a presentation model.  A diagram would be
// a kind of presentation model, which manages the graphical symbol data,
// font choices, pen widths etc.  Like a model, it manages data, albeit
// graphical data, and exports functionality.  But, like a visual part, a
// diagram can draw itself and even be nested as a symbol inside a parent
// diagram.  Therefore, a diagram is both a model and a visual part.
//
// You may be thinking, "if a presentation model can draw itself, what is the
// viewport left to do?"  Actually, the viewport is offloaded to some degree,
// but it still has a well-defined role.  When using a presentation model, a
// viewport becomes a gateway for a presentation model to render itself through.
// In other words, the viewport becomes a perspective on the presentation,
// defining what subregion of the overall presentation (or diagram) to display.
// The presentation model is essentially a graphics server, serving up whatever
// rectangular portion of the graphics the viewport instructs it to paint.
// This rectangle is measured in logical units and corresponds to the zoom
// factor and scroll positions of the viewport.
//
// The presentation model is also an IMvcVirtualPart which means your viewports
// can receive to types of change notifications from their presentation model:
// change notifications and invalidate reqests.  If you want to use this
// technique, your viewports should register themselves as renders of the
// presentation model.  When the presentation model changes the graphics it is
// serving, the viewports will be told directly by the presentation model to
// invalidate themselves.  The presentation model notifies each render of what
// rectangle in logical coordinates has been invalidated.  How does the virtual
// part send this rectangle?  By simply calling InvalidateRect(), which then
// broadcasts the invalidate call to all renders.
//
// It is the responsibility of the viewport renderer to figure out where that
// logical rectangle is on its display surface and (if it is currently showing
// the invalid region) to perform a true windows invalidation.
//
// This technique is very convenient.  It allows the presentation model to
// draw and erase within its own virtual space using its own logical units,
// blissfully ignorant of how the graphics it produces get mapped onto a device.
//
//
//@base public | MvcModel
//@base public | IMvcVirtualPart
//@base public | IMvcUndoRedo
//

template <class viscomp_t>
class MvcPresentationModel_T : public CMvcModel,
                               public viscomp_t,
							   public stingray::foundation::IMvcVirtualPart,
                               public stingray::foundation::IMvcUndoRedo
{
public:
	typedef CCArray_T<stingray::foundation::MvcVisualPart*, stingray::foundation::MvcVisualPart*> RenderSet;

// Construction/Destruction
public:
	MvcPresentationModel_T(stingray::foundation::MvcTransactionModel* pTransModel = NULL);
	virtual ~MvcPresentationModel_T();

// Queries

	//@cmember
	/* Tests whether the presentation model has been modified*/
	virtual BOOL IsModified() const;

// Undo / Redo Members

	//@cmember
	/* Accessor for the transaction model associated with this model*/
	virtual stingray::foundation::MvcTransactionModel* GetTransactionModel();

	//@cmember
	/* Set for the transaction model associated with this model*/
	virtual void SetTransactionModel(stingray::foundation::MvcTransactionModel* pTransModel);

	//@cmember
	/* Execute and log a command*/
	virtual BOOL Do(CMvcCommand* pCmd);

	//@cmember
	/* Undo a command*/
	virtual CMvcCommand* Undo();

	//@cmember
	/* Redo a command*/
	virtual CMvcCommand* Redo();

	//@cmember
	/* Get the next command on the undo stack*/
	virtual CMvcCommand* PeekUndo();

	//@cmember
	/* Get the next command on the undo stack*/
	virtual CMvcCommand* PeekRedo();

// Support for Multiple Perspective on this Virtual View

	//@cmember
	/* Adds a visual part to the list of renderers of this presentation model.*/
	virtual void AddRendering(stingray::foundation::MvcVisualPart* pVisualPart);

	//@cmember
	/* The visual part identified by the argument is removed from this virtual part's list of renderers.*/
	virtual void RemoveRendering(stingray::foundation::MvcVisualPart* pVisualPart);

// Invalidation

	//@cmember
	/* Invalidate the specified logical rectangle within this virtual part.  The invalidation will be broadcasted to all renderers.*/
	virtual void InvalidateRect(const CRect& rcLog, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	// The GUID_MAP implements QueryGuid
	BEGIN_GUID_MAP(MvcPresentationModel_T<viscomp_t>)
		GUID_CHAIN_ENTRY(CMvcModel)
		GUID_CHAIN_ENTRY(viscomp_t)
	END_GUID_MAP

// Attributes
protected:
	stingray::foundation::MvcTransactionModel* m_pTransModel;
	RenderSet m_setRenderers;
};

///////////////////////////////////////////////////////////////////////////


template<class viscomp_t>
MvcPresentationModel_T<viscomp_t>::MvcPresentationModel_T(stingray::foundation::MvcTransactionModel* pTransModel) : m_pTransModel(pTransModel)
{
}

template<class viscomp_t>
MvcPresentationModel_T<viscomp_t>::~MvcPresentationModel_T()
{
	ASSERT(m_setRenderers.GetSize() == 0);
}

//@doc MvcPresentationModel_T
//@mfunc Adds a visual part to the to the list of dependents
//@rdesc void
//@parm MvcVisualPart* | pVisualPart | A pointer to an object that is dependent on (observing) this object
//@comm
// This function adds any object derived from MvcVisualPart to our list of
// renderers.  When the presentation model changes the graphics, each
// renderer is informed of the invalidated logical rectangle.  The render
// is responsible for mapping the logical rectangle into device coordinate
// and performing the actual windows invalidation.
//@xref <mf MvcPresentationModel_T::RemoveRendering>
template<class viscomp_t>
void MvcPresentationModel_T<viscomp_t>::AddRendering(
	stingray::foundation::MvcVisualPart* pVisualPart)	
{
	m_setRenderers.Add(pVisualPart);
}

//@doc MvcPresentationModel_T
//@mfunc Removes an observer from the list of dependents
//@rdesc void
//@parm MvcVisualPart* | pVisualPart | The dependent to be removed from the list of dependents
//@comm
//  This function removes the specified visual part from this presentation
//  model's list of renders.
//@xref <mf MvcPresentationModel_T::AddRendering>
template<class viscomp_t>
void MvcPresentationModel_T<viscomp_t>::RemoveRendering(
	stingray::foundation::MvcVisualPart* pVisualPart)	
{
	m_setRenderers.Remove(pVisualPart);
}

//@doc MvcPresentationModel_T
//@mfunc Invalidates the specified rectangle specified in logical coordinates
//@rdesc void
//@parm const CRect& | rcLog | Specifies the rectangle to invalidate
//@parmopt BOOL | bErase | TRUE | Should the visual part be completely erased prior to redraw
//@parmopt BOOL | bRepairNow | FALSE | Repair the invalid area immediately
//@comm
// This function iterates through the dependent renderings, invalidating
// the specified logical rectangle.
//@xref <mf MvcVisualPart::InvalidateRect>
template<class viscomp_t>
void MvcPresentationModel_T<viscomp_t>::InvalidateRect(const CRect& rcLog, BOOL bErase, BOOL bRepairNow)
// rcLog is the rectangle to invalidate in logical units
{
	Iterator_T<stingray::foundation::MvcVisualPart*> i(&m_setRenderers);
	stingray::foundation::MvcVisualPart* pVisualPart;
	for (pVisualPart = i.GetFirst(); pVisualPart != NULL; pVisualPart = i.GetNext())
		pVisualPart->InvalidateRect(rcLog, bErase, bRepairNow);
}


//@doc MvcPresentationModel_T
//@mfunc Tests whether this model has been modified since its creation
//@rdesc TRUE if the model has been modified; otherwise FALSE
template<class viscomp_t>
BOOL MvcPresentationModel_T<viscomp_t>::IsModified() const
{
	if (m_pTransModel)
		return m_pTransModel->IsModified();
	else
		return FALSE;
}

//@doc MvcPresentationModel_T
//@mfunc Get the transaction model associated with this model
//@rdesc MvcTransactionModel* A pointer to the current transaction model
template<class viscomp_t>
stingray::foundation::MvcTransactionModel* MvcPresentationModel_T<viscomp_t>::GetTransactionModel()
{
	return m_pTransModel;
}

//@doc MvcPresentationModel_T
//@mfunc Set the transaction model associated with this model
//@parm MvcTransactionModel* | pTransModel | A pointer to the transaction model
template<class viscomp_t>
void MvcPresentationModel_T<viscomp_t>::SetTransactionModel(stingray::foundation::MvcTransactionModel* pTransModel)
{
	m_pTransModel = pTransModel;
}

//@doc MvcPresentationModel_T
//@mfunc Execute and log a command
//@rdesc BOOL TRUE if the call was successful; otherwise FALSE
//@parm CMvcCommand* | pCmd | A  pointer to the command to execute and log.
template<class viscomp_t>
BOOL MvcPresentationModel_T<viscomp_t>::Do(CMvcCommand* pCmd)
{
	ASSERT(m_pTransModel != NULL);
	return m_pTransModel->Do(pCmd);
}


//@doc MvcPresentationModel_T
//@mfunc Undo a command
//@rdesc CMvcCommand* A pointer to the command that was undone
template<class viscomp_t>
CMvcCommand* MvcPresentationModel_T<viscomp_t>::Undo()
{
	ASSERT(m_pTransModel != NULL);
	return m_pTransModel->Undo();
}

//@doc MvcPresentationModel_T
//@mfunc Redo a command
//@rdesc CMvcCommand* A pointer to the command that was redone
template<class viscomp_t>
CMvcCommand* MvcPresentationModel_T<viscomp_t>::Redo()
{
	ASSERT(m_pTransModel != NULL);
	return m_pTransModel->Redo();
}

//@doc MvcPresentationModel_T
//@mfunc Undo a command
//@rdesc CMvcCommand* A pointer to the command that was undone
template<class viscomp_t>
CMvcCommand* MvcPresentationModel_T<viscomp_t>::PeekUndo()
{
	ASSERT(m_pTransModel != NULL);
	return m_pTransModel->PeekUndo();
}

//@doc MvcPresentationModel_T
//@mfunc Redo a command
//@rdesc CMvcCommand* A pointer to the command that was redone
template<class viscomp_t>
CMvcCommand* MvcPresentationModel_T<viscomp_t>::PeekRedo()
{
	ASSERT(m_pTransModel != NULL);
	return m_pTransModel->PeekRedo();
}

};  // namespace stingray::foundation
};  // namespace stingray

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // #ifndef __MFC_MVCPRESENTATIONMODEL_H__
