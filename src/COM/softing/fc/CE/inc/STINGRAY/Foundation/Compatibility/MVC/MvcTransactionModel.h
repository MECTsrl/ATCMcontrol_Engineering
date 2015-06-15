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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcTransactionModel.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

//@doc MvcTransactionModel

#ifndef __COMPAT_MVCTRANSACTIONMODEL_H__
#define __COMPAT_MVCTRANSACTIONMODEL_H__

// Includes

#ifndef __COMPAT_MVCCOMMAND_H__
#include "Foundation\Compatibility\MVC\MvcCommand.h"
#endif

#ifndef __COMPAT_MVCMODEL_H__
#include "Foundation\Compatibility\MVC\MvcModel.h"
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

#ifndef MVC_HISTORY_DEPTH
#define MVC_HISTORY_DEPTH	256
#endif

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
//@base public | MvcModel

class MvcTransactionModel : public MvcModel
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
	virtual BOOL Log(MvcCommand* pCmd);

	//@cmember
	/* Execute and log a command*/
	virtual BOOL Do(MvcCommand* pCmd);

	//@cmember
	/* Undo a command*/
	virtual MvcCommand* Undo();

	//@cmember
	/* Redo a command*/
	virtual MvcCommand* Redo();

	//@cmember
	/* Get the command that will be reversed next time Undo is called*/
	MvcCommand* PeekUndo();

	//@cmember
	/* Get the command that will be execute next time Redo is called*/
	MvcCommand* PeekRedo();

	//@cmember
	/* Set the number of commands that can be stored by the transaction model*/
	void SetHistorySize(int m_nHistorySize);

protected:

	//@cmember
	/* Stack of command actions that can be undone.*/
	CCStack_T<MvcCommand*, MvcCommand*> m_undoStack;

	//@cmember
	/* Stack of command actions that can be redone.*/
	CCStack_T<MvcCommand*, MvcCommand*> m_redoStack;
};

//@doc MvcTransactionModel
//@mfunc Get the command that will be reversed next time Undo is called
//@rdesc Pointer to previous command in buffer
inline MvcCommand* MvcTransactionModel::PeekUndo()
{
	return m_undoStack.Peek();
}

//@doc MvcTransactionModel
//@mfunc Get the command that will be execute next time Redo is called
//@rdesc Pointer to next command in undo buffer
inline MvcCommand* MvcTransactionModel::PeekRedo()
{
	return m_redoStack.Peek();
}

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif __COMPAT_MVCTRANSACTIONMODEL_H__

