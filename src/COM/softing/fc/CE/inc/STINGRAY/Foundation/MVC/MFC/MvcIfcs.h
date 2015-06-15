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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcIfcs.h 1     28.02.07 18:58 Ln $

#pragma once

#ifndef __MFC_MVCIFCS_H__
#define __MFC_MVCIFCS_H__

#include <Foundation\GuidCast.h>
#include <Foundation\StructuredGraphics\Visual.h>
#include <Foundation\Patterns\SubjectObserver.h>
#include <Foundation\Events\EventRouter.h>
#include <Foundation\MVC\MFC\MvcCommand.h>

namespace stingray {
namespace foundation {

typedef stingray::foundation::IMessage IMvcMessage;
typedef stingray::foundation::ISubject IMvcSubject;
typedef stingray::foundation::IObserver IMvcObserver;

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

};  // namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MFC_MVCIFCS_H__
