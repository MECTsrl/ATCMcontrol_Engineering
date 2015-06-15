/////////////////////////////////////////////////////////////////////////////
//
// MvcCommand.h : Declaration of MvcCommand
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
// Description:  Declaration of MvcCommand class.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcCommand.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcCommand

#ifndef __COMPAT_MVCCOMMAND_H__
#define __COMPAT_MVCCOMMAND_H__

#ifndef __COMPAT_MVCIFCS_H__
#include <Foundation\Compatibility\MVC\MvcIfcs.h>
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

// Forward Declarations
class MvcModel;

//@doc MvcCommand
//@mfunc BOOL  | MvcCommand | IsUndoable | Checks to see if this command can be undone
//@xref <c MvcCommand>

//@doc MvcCommand
//@mfunc BOOL  | MvcCommand | SetUndoable | Sets if this command can be undone
//@parm BOOL | bUndoable | The state to be set.  TRUE indicates the command is undoable.
//@xref <c MvcCommand>


/////////////////////////////////////////////////////////////////////////////
//
// MvcCommand
//
//@class
// An MvcCommand is an abstract base class that defines the interface for
// all command objects.  A command object is essentially a persistent form of
// a verb.  Graphical User Interfaces typically adhere to what is known
// as the "Object-Verb" paradigm, which simply means you select an object
// and then apply a verb to that object.  Fundamentally, each command
// object represents a verb and classes which you derive from MvcCommand
// define specific verbs as appropriate for your application.  Basically,
// you are expected to derive multiple command classes from this class and
// implement the Execute function such that it performs whatever function
// the derived command class encapsulates.  In MVC, the model defines
// a set of verbs, or command objects, that make sense within the triad.
// 
// The MvcCommand class is central to the undo/redo architecture defined
// by MVC.  Typically, a controller will respond to user input by calling
// some operation defined by the model it observes.  That model operation
// should create a command object of the required type, log the command
// and execute it. Logging the commands prior to executing them creates
// a history buffer so that a record exist for undo/redo purposes.  This
// history buffer is maintained internal to the transaction model.
//
//@base public | IMvcMessage

class MvcCommand : public IMvcMessage
{
	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//

public:

	// Initialization/Destruction
	//@cmember
	/* Construction*/
	MvcCommand();
	virtual ~MvcCommand();

	// Queries

	//@cmember
	/* Checks to see if this command can be undone*/
	virtual BOOL IsUndoable() const 	{ return m_bUndoable; }

	//@cmember
	/* Sets if this command can be undone*/
	virtual void SetUndoable(BOOL bUndoable) { m_bUndoable=bUndoable; }

	// Operations

	//@cmember
	/* Perform the action the command represents*/
	virtual BOOL Execute();

	//@cmember
	/* Perform the opposite action of what the command represents*/
	virtual BOOL Unexecute();

	/////////
	// IUnknown interface

	//@cmember
	/* Retrieve a pointer to an interface supported by this object. */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);

	//@cmember
	/* Add a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE AddRef();

	//@cmember
	/* Release a reference to this object. */
	virtual ULONG STDMETHODCALLTYPE Release();

protected:
	BOOL m_bUndoable;
};


//////////////////////////////////////////////////////////////////////////
// Message and command declaration macros
//

#ifndef MVC_NO_COMMAND_DECL_MACROS

#define MVC_DECLARE_COMMAND() \
	private: static const unsigned int m_nID; \
	public: virtual unsigned int GetTypeID() const { return m_nID; } \

#define MVC_IMPLEMENT_COMMAND(class_name, cmd_id) \
	const unsigned int class_name::m_nID = cmd_id; \

#define MVC_DECLARE_MESSAGE() \
	MVC_DECLARE_COMMAND() \

#define MVC_IMPLEMENT_MESSAGE(class_name, cmd_id) \
	MVC_IMPLEMENT_COMMAND(class_name, cmd_id) \

#endif //MVC_NO_COMMAND_DECL_MACROS

/////////////////////////////////////////////////////////////////////////////
//
// MvcCommandSequence_T
//


//@class
// An MvcCommandSequence is a command that contains a sequence of subcommands.
//
//@base public | MvcCommand

template <class base_t>
class MvcCommandSequence_T : public base_t
{
public:
	typedef CCArray_T<MvcCommand*, MvcCommand*> CommandSet;

protected:
	CommandSet m_setCommands;

public:
	// Initialization/Destruction
	virtual ~MvcCommandSequence_T();

	// Operations

	//@cmember
	/* Execute the commands contained by the macro*/
	virtual BOOL Execute();
	//@cmember
	/* Unexecute the commands contained by the macro*/
	virtual BOOL Unexecute();
	//@cmember
	/* Print the action for logging purposes*/
	virtual void Sprint(CString& strCmd);

	//@cmember
	/* Add a command to the macro.*/
	virtual void AddCommand(MvcCommand* pCmd);
	//@cmember
	/* Remove a command from the macro*/
	virtual void RemoveCommand(const int nIndex);
	//@cmember
	/* Remove all commands from the macro*/
	virtual void RemoveAllCommands();

	//@cmember
	/* Return the command list from the macro*/
	inline ICollection_T<MvcCommand*>* GetChildren();
};

////////////////////////////////////////////////////////////////////////
//
// MvcCommandSequence
//

template <class base_t>
MvcCommandSequence_T<base_t>::~MvcCommandSequence_T()
{
	Iterator_T<MvcCommand*> i(&m_setCommands);
	MvcCommand* pCmd = i.GetLast();

	while (pCmd)
	{
		i.Remove();
		delete pCmd;

		pCmd = i.GetLast();
	}
}

//@doc MvcCommandSequence
//@mfunc Returns the collection of commands contained by this command sequence
//@rdesc The collection of commands
template <class base_t>
inline ICollection_T<MvcCommand*>* MvcCommandSequence_T<base_t>::GetChildren()
{
	return &m_setCommands;
}

//@doc MvcCommandSequence
//@mfunc Execute the commands contained by the macro
//@rdesc TRUE if the macro completed successfully, otherwise FALSE
//@comm If any command in the macro fails, the macro will abort
// and FALSE will be returned.
template <class base_t>
BOOL MvcCommandSequence_T<base_t>::Execute()
{
	MvcCommand* pCmd;
	Iterator_T<MvcCommand*> i(&m_setCommands);

	for (pCmd = i.GetFirst(); pCmd != NULL; pCmd = i.GetNext())
	{
		if ( !pCmd->Execute() )
			return FALSE;
	}

	return TRUE;
}

//@doc MvcCommandSequence
//@mfunc Unexecute the commands contained by the macro
//@rdesc TRUE if the macro was Unexecuted successfully, otherwise FALSE
//@comm If any command in the macro fails to Unexecute, the macro will abort
// and FALSE will be returned.
template <class base_t>
BOOL MvcCommandSequence_T<base_t>::Unexecute()
{
	MvcCommand* pCmd;
	Iterator_T<MvcCommand*> i(&m_setCommands);

	for (pCmd = i.GetLast(); pCmd != NULL; pCmd = i.GetPrev())
	{
		if ( !pCmd->Unexecute() )
			return FALSE;
	}

	return TRUE;
}

//@doc MvcCommandSequence
//@mfunc Print the action for logging purposes
//@rdesc void 
//@parm CString& | strCmd | A string to receive the output.
template <class base_t>
void MvcCommandSequence_T<base_t>::Sprint(CString& strCmd)
{
	MvcCommand* pCmd;
	CString strSubCmd;

	strCmd.Format(_T("Macro Command with %d subcommands:\n"), m_setCommands.GetSize());

	Iterator_T<MvcCommand*> i(&m_setCommands);

	for (pCmd = i.GetFirst(); pCmd != NULL; pCmd = i.GetNext())
	{
		pCmd->Sprint(strSubCmd);
		strCmd = strCmd + "  " + strSubCmd + "\n";
	}
}

//@doc MvcCommandSequence
//@mfunc Add a command to the macro.
//@rdesc void 
//@parm MvcCommand* | pCmd | A command to add to the macro
template <class base_t>
void MvcCommandSequence_T<base_t>::AddCommand(MvcCommand* pCmd)
{
	m_setCommands.Add(pCmd);
}

//@doc MvcCommandSequence
//@mfunc Remove a command from the macro
//@rdesc void 
//@parm const int | nIndex | The index of a command to remove. The index is zero based.
template <class base_t>
void MvcCommandSequence_T<base_t>::RemoveCommand(const int nIndex)
{
	m_setCommands.RemoveAt(nIndex);
}

//@doc MvcCommandSequence
//@mfunc Remove all commands from the macro
//@rdesc void 
template <class base_t>
void MvcCommandSequence_T<base_t>::RemoveAllCommands()
{
	m_setCommands.RemoveAll();
}

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __COMPAT_MVCCOMMAND_H__

