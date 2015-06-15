/////////////////////////////////////////////////////////////////////////////
// MvcIfcs.h : Abstract MVC interfaces.
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
// Author:       Jeff Boenig and Dean Hallman
// Description:  Abstract base classes that define interfaces to the
//               MVC architecture.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcIfcs.h 1     28.02.07 18:58 Ln $

#ifndef __COMPAT_MVCIFCS_H__
#define __COMPAT_MVCIFCS_H__

#ifndef __COMPAT_CPPIFACE_H__
#include "Foundation\Compatibility\Common\CppIface.h"
#endif

#ifndef __COMPAT_VISUAL_H__
#include "Foundation\Compatibility\Common\AGI\Visual.h"
#endif

#ifndef __COMPAT_SUBJECTOBSERVER_H__
#include "Foundation\Compatibility\Common\Patterns\SubjectObserver.h"
#endif

#ifndef __COMPAT_EVENTROUTER_H__
#include "Foundation\Compatibility\Common\Events\EventRouter.h"
#endif

class MvcCommand;

typedef rw::IMessage IMvcMessage;
typedef rw::ISubject IMvcSubject;
typedef rw::IObserver IMvcObserver;

//@doc IMvcUndoRedo
//@mfunc BOOL | IMvcUndoRedo | Do | Execute and log a command
//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc MvcCommand* | IMvcUndoRedo | Undo | Undo a command
//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc  MvcCommand* | IMvcUndoRedo | Redo | Redo a command
//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc BOOL | IMvcUndoRedo | Repeat | Repeat the last command that was executed
//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc MvcCommand* | IMvcUndoRedo | PeekUndo | Returns the next command on the undo stack
//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc MvcCommand* | IMvcUndoRedo | PeekRedo | Returns the next command on the redo stack

//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc MvcTransactionModel* | IMvcUndoRedo | GetTransactionModel | Accessor for the transaction model associated with this model
//@xref <c IMvcUndoRedo>

//@doc IMvcUndoRedo
//@mfunc void | IMvcUndoRedo | SetTransactionModel | Set the transaction model associated with this model
//@xref <c IMvcUndoRedo>

//@class Interface for classes that support command execution and undo/redo
class IMvcUndoRedo
{
public:
	//@cmember
	/* Execute and log a command*/
	virtual BOOL Do(MvcCommand* pCmd) = 0;
	//@cmember
	/* Undo a command*/
	virtual MvcCommand* Undo() = 0;
	//@cmember
	/* Redo a command*/
	virtual MvcCommand* Redo() = 0;
	//@cmember
	/* What is the next command on the undo stack*/
	virtual MvcCommand* PeekUndo() = 0;
	//@cmember
	/* What is the next command on the redo stack*/
	virtual MvcCommand* PeekRedo() = 0;
};

class MvcVisualPart;		// forward declaration

//@doc IMvcVirtualPart
//@mfunc void | IMvcVirtualPart | AddRendering | Adds a visual part to the list of renderings
//@xref <c IMvcVirtualPart>

//@doc IMvcVirtualPart
//@mfunc void | IMvcVirtualPart | RemoveRendering | The visual part identified by the argument is removed from this virtual part's list of renderings.
//@xref <c IMvcVirtualPart>

//@class Interface for a virtual part.  A virtual part is a visual part that
// can render itself on multiple display surfaces simultaneously.  The graphics
// a virtual part draws are virtual because its coordinate space is essentially
// off-screen.  The graphics don't appear on a device until a renderer maps the
// virtual part onto a device. The presentation model implements this interface
// so that its presentation can appear in multiple viewports concurrently.
class IMvcVirtualPart
{
public:
	//@cmember
	/* Adds a visual part to the list of renderings*/
	virtual void AddRendering(MvcVisualPart* pVisualPart) = 0;

	//@cmember
	/* The visual part identified by the argument is removed from this virtual part's list of renderings.*/
	virtual void RemoveRendering(MvcVisualPart* pVisualPart) = 0;
};

//
//@doc IMvcModel
//
//@class IMvcModel | Interface for an MVC model. A model is the subject in a
// subject-observer relationship.
//
//@mfunc BOOL | IMvcModel | IsModified | Test to see if the model has changed
// since the last time it was saved.
//@rdesc Boolean flag indicating if model has been modified.
//
//@mfunc void | IMvcModel | Reset | Clear the model and reset its state.
//@rdesc void
//
//@base public | ISubject
//
class __declspec(uuid("F0526984-E944-11d2-B412-006008AFE059")) 
			IMvcModel : public IMvcSubject
{
public:
	//@cmember
	/* Test to see if the model has changed since the last time it was saved. */
	virtual BOOL IsModified() const = 0;
	//@cmember
	/* Clear the model and reset its state. */
	virtual void Reset() = 0;
};

//@doc IMvcController_T
//@mfunc BOOL  | IMvcController_T | AddListener | Add an event listener to the controller.
//@parm MvcVisualComponent* | pIListener | Points to an MVCVisualComponent listener
//@xref <c IMvcController_T>

//@doc IMvcController_T
//@mfunc BOOL  | IMvcController_T | RemoveListener | Remove an event listener from the controller.
//@parm MvcVisualComponent* | pIListener | Points to an MVCVisualComponent listener
//@xref <c IMvcController_T>

//@doc IMvcController_T
//@mfunc void  | IMvcController_T | SetModel | Assign a model to the controller.
//@rdesc void
//@parm model_t* | pModel | Pointer to model to assign to the viewport.
//@comm Adds the viewport as an observer to the model. Also, pass the
// model to the controller by calling <mf MvcController_T::SetModel>.
//@xref <c IMvcController_T>

//@doc IMvcController_T
//@mfunc model_t*  | IMvcController_T | GetModel | Get a pointer to the model associated with this controller.
//@xref <c IMvcController_T>

//@doc IMvcController_T
//@mfunc void  | IMvcController_T | SetViewport | Assign a viewport to the controller.
//@parm void | pViewport | Points to an viewport to be assigned
//@xref <c IMvcController_T>

//@doc IMvcController_T
//@mfunc viewport_t*  | IMvcController_T | GetViewport | Get a pointer to the viewport associated with this controller.
//@parm viewport_t* | pIListener | Points to an MVCVisualComponent listener
//@xref <c IMvcController_T>

//
//@doc IMvcController_T
//
//@class IMvcController_T | Interface template for an MVC controller. The
// model and viewport classes are parameters to the template. This allows
// a more type-safe controller interface to be instantiated. An instance
// of this template is a pure virtual interface that must be implemented
// by a class.
//
//@tcarg class | model_t | Type of model accessed by the controller.
//@tcarg class | viewport_t | Type of viewport accessed by the controller.
//
//@base public | IEventRouter
//
template<class model_t, class viewport_t>
class IMvcController_T : public rw::IEventRouter
{
public:
	//@cmember
	/* Assign a model to the controller. */
	virtual void SetModel(model_t* pModel) = 0;
	//@cmember
	/* Get a pointer to the model associated with this controller. */
	virtual model_t* GetModel() const = 0;
	//@cmember
	/* Assign a viewport to the controller. */
	virtual void SetViewport(viewport_t* pViewport) = 0;
	//@cmember
	/* Get a pointer to the viewport associated with this controller. */
	virtual viewport_t* GetViewport() const = 0;
};


//@doc IMvcViewport_T
//@mfunc void | IMvcViewport_T | SetModel | Assign a model to the viewport.
//@rdesc void
//@parm model_t* | pModel | Pointer to model to assign to the viewport.
//@comm Adds the viewport as an observer to the model. Also, pass the
// model to the controller by calling <mf MvcController_T::SetModel>.

//@doc IMvcViewport_T
//@mfunc void | IMvcViewport_T | GetModel | Get a pointer to the model associated
// with this viewport.
//@rdesc A pointer to the associated model.

//@doc IMvcViewport_T
//@mfunc BOOL | IMvcViewport_T | CreateController | Create the default controller for this viewport.
//@rdesc FALSE if an error occurs; otherwise TRUE.
//@comm The default implementation does nothing and returns TRUE. Derived
// classes override this function in order to create a default controller.

//@doc IMvcViewport_T
//@mfunc void | IMvcViewport_T | SetController | Set the controller for this viewport.
//@rdesc void
//@parm ctlr_t* | pController | Pointer to controller to assign to the viewport.
//@parmopt const BOOL | bAutoDelCtlr | FALSE | Flag indicating whether the viewport
// should destroy the controller in its destructor.

//@doc IMvcViewport_T
//@mfunc ctlr_t* | IMvcViewport_T | GetController | Get a pointer to the controller for this viewport.
//@rdesc Pointer to controller for this viewport.

//
//@doc IMvcViewport_T
//
//@class IMvcViewport_T | Interface template for an MVC viewport. The
// model and controller classes are parameters to the template. This
// allows a more type-safe viewport interface to be instantiated. An
// instance of this template is a pure virtual interface that must be
// implemented by a class. This interface inherits IVisual and IObserver,
// which implies that a viewport is a visual observer of a model.
//
//@tcarg class | model_t | Type of model accessed by the viewport.
//@tcarg class | ctlr_t | Type of controller for the viewport.
//
//@base public | IVisual
//@base public | IObserver
//
template<class model_t, class ctlr_t>
class IMvcViewport_T : public rw::IVisual, public IMvcObserver
{
public:
	//@cmember
	/* Assign a model to the viewport. */
	virtual void SetModel(model_t* pModel) = 0;
	//@cmember
	/* Get a pointer to the model associated with this viewport. */
	virtual model_t* GetModel() const = 0;
	//@cmember
	/* Create the default controller for this viewport. */
	virtual BOOL CreateController() = 0;
	//@cmember
	/* Set the controller for this viewport. */
	virtual void SetController(ctlr_t* pController, const BOOL bAutoDelCtlr) = 0;
	//@cmember
	/* Get a pointer to the controller for this viewport. */
	virtual ctlr_t* GetController() = 0;
};

#endif  // __COMPAT_MVCIFCS_H__
