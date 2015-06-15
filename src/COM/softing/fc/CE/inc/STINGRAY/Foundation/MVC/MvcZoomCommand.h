/////////////////////////////////////////////////////////////////////////////
// MvcZoomCommand.h : Zoom command class
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Zoom command class
//

#pragma once

#ifndef __MVCZOOMCOMMAND_H__
#define __MVCZOOMCOMMAND_H__

#if !defined(_SFL)
#define _SFL
#endif

#include <Foundation\MVC\MvcCommand.h>

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
//@doc CMvcZoomCommand
//
//@class CMvcZoomCommand | Implements a zoom command by querying the command
// receiver object for the <c IZoom> interface and using the interface
// acquired to change the magnification.
//
//@tcarg unsigned int | nTypeID | Type identifier for the command.
//@tcarg typename | _Receiver | Type of object that will receive the command.
//
//@base public | CMvcCommand

template <unsigned int nTypeID, typename _Receiver>
class CMvcZoomCommand : public CMvcCommand
{
// Constructors/destructor
public:
	//@cmember
	/* Construct a zoom command. */
	CMvcZoomCommand(_Receiver* pICmdRecvr = NULL) :
		m_nZoomPctX(100),
		m_nZoomPctY(100)
	{
		SetCommandReceiver(pICmdRecvr);
	}

// Attributes
protected:
	//@cmember
	/* Pointer to the object that will receiver the command. */
	_Receiver* m_pCmdRecvr;
	//@cmember
	/* Magnification for X axis in percent. */
	int m_nZoomPctX;
	//@cmember
	/* Magnification for Y axis in percent. */
	int m_nZoomPctY;

// Operations
public:
	//@cmember
	/* The type ID of the message.  Used for safe typecasting. */
	virtual unsigned int GetTypeID() const
	{
		return nTypeID;
	}

	//@cmember
	/* Print information on the zoom command for logging purposes. */
	virtual void Sprint(CString& strCmd)
	{
		strCmd.Format(_T("Zoom Command: X magnification = %d, Y magnification = %d"), m_nZoomPctX, m_nZoomPctY);
	}

	//@cmember
	/* Assign pointer to the command receiver. */
	void SetCommandReceiver(_Receiver* pCmdRecvr)
	{
		m_pCmdRecvr = pCmdRecvr;
	}

	//@cmember
	/* Set the magnification values. */
	virtual void SetMagnification(const int nZoomPctX, const int nZoomPctY)
	{
		m_nZoomPctX = nZoomPctX;
		m_nZoomPctY = nZoomPctY;
	}

	//@cmember
	/* Return the magnification values. */
	virtual void GetMagnification(int& nZoomPctX, int& nZoomPctY)
	{
		nZoomPctX = m_nZoomPctX;
		nZoomPctY = m_nZoomPctY;
	}

	//@cmember
	/* Execute the zoom command. */
	virtual BOOL Execute()
	{
		IZoom* pIZoom = guid_cast<IZoom*>(m_pCmdRecvr);
		if (pIZoom != NULL)
		{
			pIZoom->SetMagnification(m_nZoomPctX, m_nZoomPctY);
		}
		ISubject* pISubject = guid_cast<ISubject*>(m_pCmdRecvr);
		if (pISubject != NULL)
		{
			pISubject->UpdateAllObservers(NULL, this);
		}
		return TRUE;
	}
};

};	// namespace stingray::foundation
};  // namespace stingray

#endif  // #ifndef __MVCZOOMCOMMAND_H__
