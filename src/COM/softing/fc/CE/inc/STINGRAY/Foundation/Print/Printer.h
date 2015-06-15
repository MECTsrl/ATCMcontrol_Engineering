/////////////////////////////////////////////////////////////////////////////
// Printer.h : SFL Print Preview package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Class to encapsulate a printer.
//

#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <winspool.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc CPrinter

//@class This class encapsulates a Windows printer. It is basically a
// wrapper class that provides easy access to the printer API functions.
// The class can be passed an existing printer handle or it can create
// one by opening a specified printer. This class can be used in
// conjunction with the <c CPrinterConfig> class to open a printer.
// It is also used in conjunction with the <c CPrintJob> class.
// Using this class you can get a list of print jobs currently
// spooled to a printer. You can also reset the printer or even
// delete the printer.

class CPrinter
{
// Constructors/destructor
public:
	//@cmember
	/* Construct a printer from a handle. */
	CPrinter(HANDLE hPrinter = NULL, const bool bOwnHandle = true) :
		m_hPrinter(hPrinter),
		m_bOwnHandle(bOwnHandle)
	{
	}

	//@cmember
	/* Destroy printer object. */
	virtual ~CPrinter()
	{
		Close();
	}

// Attributes
protected:
	//@cmember
	/* Handle to printer. */
	HANDLE m_hPrinter;
	//@cmember
	/* API structure passed to OpenPrinter to specify desired access level to printer. */
	PRINTER_DEFAULTS m_printerDefaults;
	//@cmember
	/* Flag to determine if this object will close the printer when destroyed. */
	bool m_bOwnHandle;

// Operations
public:
	//@cmember
	/* Return the printer handle associated with this object. */
	operator HANDLE()
	{
		return m_hPrinter;
	}

	//@cmember
	/* Attach an existing printer handle to this object. */
	virtual void Attach(HANDLE hPrinter, bool bOwnHandle = true)
	{
		if (m_bOwnHandle)
		{
			Close();
		}
		m_bOwnHandle = bOwnHandle;
		m_hPrinter = hPrinter;
	}

	//@cmember
	/* Detach the handle associated with this object. */
	virtual HANDLE Detach() 
	{
		HANDLE hPrinter = m_hPrinter;
		m_hPrinter = NULL;
		m_bOwnHandle = true;
		return hPrinter;
	}

	//@cmember
	/* Open a printer by device name. */
	bool Open(LPCTSTR pPrinterName)
	{
		bool bSuccess = false;

		if (m_hPrinter == NULL)
		{
			if (::OpenPrinter(const_cast<LPTSTR>(pPrinterName), &m_hPrinter, NULL /* printer defaults */))
			{
				m_bOwnHandle = true;
				bSuccess = true;
			}
		}

		return bSuccess;
	}

	//@cmember
	/* Determines if this object is associated with an open printer handle. */
	bool IsOpen() const
	{
		return (m_hPrinter != NULL);
	}

	//@cmember
	/* Close the printer handle associated with this object. */
	bool Close()
	{
		bool bSuccess = false;

		if (m_hPrinter != NULL && m_bOwnHandle)
		{
			if (::ClosePrinter(m_hPrinter))
			{
				m_hPrinter = NULL;
				bSuccess = true;
			}
		}

		return bSuccess;
	}

	bool Reset()
	{
		bool bSuccess = false;

		if (m_hPrinter != NULL)
		{
			if (::ResetPrinter(m_hPrinter, NULL /* printer defaults */))
			{
				bSuccess = true;
			}
		}

		return bSuccess;
	}

	//@cmember
	/* Delete the printer configuration associated with this object. */
	bool Delete()
	{
		bool bSuccess = false;

		if (m_hPrinter != NULL)
		{
			if (::DeletePrinter(m_hPrinter))
			{
				m_hPrinter = NULL;
				bSuccess = true;
			}
		}

		return bSuccess;
	}
};

};	// namespace stingray::foundation
};	// namespace stingray
