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
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcCommand.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcCommand

#pragma once

#ifndef __MFC_MVCCOMMAND_H__
#define __MFC_MVCCOMMAND_H__

#include <Foundation\Mvc\MvcCommand.h>
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

namespace stingray {
namespace foundation {

//@type MvcCommand | This is an alias for <c CMvcCommand> used to preserve compability
// with previous versions of MVC.
//@iex typedef stingray::foundation::CMvcCommand MvcCommand;
//@xref <c CMvcCommand>
typedef stingray::foundation::CMvcCommand MvcCommand;

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

#endif  // #ifndef __MFC_MVCCOMMAND_H__
