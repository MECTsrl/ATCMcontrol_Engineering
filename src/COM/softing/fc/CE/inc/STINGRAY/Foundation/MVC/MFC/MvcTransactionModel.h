/////////////////////////////////////////////////////////////////////////////
// MvcTransactionModel.h : Declaration of MvcTransactionModel
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
// Description:  Declaration of MvcTransactionModel class.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcTransactionModel.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcTransactionModel

#pragma once

#ifndef __MFC_MVCTRANSACTIONMODEL_H__
#define __MFC_MVCTRANSACTIONMODEL_H__

// Includes

#include <Foundation\MVC\MvcModel.h>
#include <Foundation\MVC\MvcCommand.h>
#include <Foundation\Patterns\MFC\Collect.h>

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

#ifndef MVC_HISTORY_DEPTH
#define MVC_HISTORY_DEPTH	256
#endif

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////////
//
// MvcTransactionModel
//

//@doc MvcTransactionModel
//@mdata CCStack_T | MvcTransactionModel | m_undoStack | 	Stack of command actions that can be undone.
//@xref <c MvcTransactionModel>

//@doc MvcTransactionModel
//@mdata CCStack_T | MvcTransactionModel | m_redoStack | Stack of command actions that can be redone.
//@xref <c MvcTransactionModel>


//@class
// A transaction model records all commands that have occured
// within an application and is central to the undo/redo, record/playback
// and event logging functionality.  To record a command, the controller
// simply has to call the Log member passing a pointer to the
// MvcCommand object.  Later, the transaction model can undo the logged
// commands.
//
//@base public | CMvcModel

class MvcTransactionModel : public CMvcModel
{
public:
	//@cmember
	/* Constructor*/
	MvcTransactionModel();

	//@cmember
	/* Destructor*/
	virtual ~MvcTransactionModel();

public:

	//@cmember
	/* Reset the state of the transaction model to its initial state*/
	virtual void Reset();

	//@cmember
	/* Tests whether the transaction model has stored new commands since last save*/
	virtual BOOL IsModified() const;

	//@cmember
	/* Records the specified command for later undo or event logging*/
	virtual BOOL Log(CMvcCommand* pCmd);

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
	/* Get the command that will be reversed next time Undo is called*/
	CMvcCommand* PeekUndo();

	//@cmember
	/* Get the command that will be execute next time Redo is called*/
	CMvcCommand* PeekRedo();

	//@cmember
	/* Set the number of commands that can be stored by the transaction model*/
	void SetHistorySize(int m_nHistorySize);

protected:

	//@cmember
	/* Stack of command actions that can be undone.*/
	CCStack_T<CMvcCommand*, CMvcCommand*> m_undoStack;

	//@cmember
	/* Stack of command actions that can be redone.*/
	CCStack_T<CMvcCommand*, CMvcCommand*> m_redoStack;
};

//@doc MvcTransactionModel
//@mfunc Get the command that will be reversed next time Undo is called
//@rdesc Pointer to previous command in buffer
inline CMvcCommand* MvcTransactionModel::PeekUndo()
{
	return m_undoStack.Peek();
}

//@doc MvcTransactionModel
//@mfunc Get the command that will be execute next time Redo is called
//@rdesc Pointer to next command in undo buffer
inline CMvcCommand* MvcTransactionModel::PeekRedo()
{
	return m_redoStack.Peek();
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

#endif  // #ifndef __MFC_MVCTRANSACTIONMODEL_H__
