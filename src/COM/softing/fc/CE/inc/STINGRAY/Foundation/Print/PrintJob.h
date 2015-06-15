/////////////////////////////////////////////////////////////////////////////
// PrintJob.h : SFL Print Preview package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  
//

#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <Foundation\Print\PrintJob.h>
#include <Foundation\Print\Printable.h>
#include <Foundation\Print\PrinterConfig.h>
#include <Foundation\Print\Printer.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc CPrintJob

//@doc CPrintJob
//@class The CPrintJob class encapsulates the task of sending a document to a printer. It takes a printable object, <c IPrintable>, 
// and a document, <c CPrintDoc>, and coordinates the printing to the document.
//@xref <c CPrinter>
//@xref <c CPrintConfig>
//@xref <c CPrintPreviewFrameImpl>
class CPrintJob
{
// Constructors/destructor
public:
	//@cmember,mfunc
	// Constructs the print job.
	//@@syntax CPrintJob(CPrinterConfig* pPrinterConfig = NULL)
	//@@syntax CPrintJob(HANDLE hPrinter, int nJobId)
	//@@parmopt CPrinterConfig* | pPrinterConfig | NULL | Printer configuration. 
	//@@parm HANDLE | hPrinter | Printer handle.
	//@@parm int | nJobId | Print job ID.
	/* Constructs the print job.*/
	CPrintJob(CPrinterConfig* pPrinterConfig = NULL) :
		m_nJobId(SP_ERROR)
	{
		
		// To get rid of warning...
		pPrinterConfig;
	}

	
	CPrintJob(HANDLE hPrinter, int nJobId) :
		m_printer(hPrinter, false),
		m_nJobId(nJobId)
	{
	}

// Attributes
protected:
	//@cmember
	/* The printer object.*/
	CPrinter m_printer;
	//@cmember
	/* Print job ID obtained from the document.*/
	int m_nJobId;

// Operations
public:
	//@cmember,mfunc
	// To begin the print process, invoke the Start method passing in a pointer to the current printable object and a CPrintDoc instance initialized with the printer configuration.  
	//@@rdesc If the printing is successful, returns true; else false.
	//@@parm IPrintable* | pIPrintable | An IPrintable interface pointer to the current printable object.
	//@@parm CPrintDoc* | pPrintDoc | A pointer to a CPrintDoc object that has been initialized with the printer configuration.
	//@@end
	/* Opens the printer specified in CPrintDoc's printer configuration and starts the printing sequence.*/
	virtual bool Start(IPrintable* pIPrintable, CPrintDoc* pPrintDoc)
	{
		bool bSuccess = false;

		if (pPrintDoc != NULL && m_nJobId == SP_ERROR)
		{
			CPrinterConfig* pPrinterConfig = pPrintDoc->GetPrinterConfig();

			if (OpenPrinter(pPrinterConfig))
			{
				HDC hPrintDC = pPrinterConfig->CreateDC();
				pPrintDoc->SetPrintDC(hPrintDC);
				pPrintDoc->SetOutputDC(hPrintDC);

				m_nJobId = pPrintDoc->StartDoc();

				if (m_nJobId != SP_ERROR)
				{
					int nPagesPrinted = 0;
					
					nPagesPrinted = OnPrintDocument(pIPrintable, pPrintDoc);
					pPrintDoc->EndDoc();
				}
			}
		}

		return bSuccess;
	}

	//@cmember
	/* Cancels/deletes the print job.*/
	virtual bool Cancel()
	{
		return SendJobCommand(JOB_CONTROL_CANCEL);
	}

	//@cmember
	/* Deletes the print job.*/
	virtual bool Delete()
	{
		return SendJobCommand(JOB_CONTROL_DELETE);
	}

	//@cmember
	/* Pauses the print job.*/
	virtual bool Pause()
	{
		return SendJobCommand(JOB_CONTROL_PAUSE);
	}

	//@cmember
	/* Restarts an in-progress print job.*/
	virtual bool Restart()
	{
		return SendJobCommand(JOB_CONTROL_RESTART);
	}

	//@cmember
	/* Resumes a paused print job.*/
	virtual bool Resume()
	{
		return SendJobCommand(JOB_CONTROL_RESUME);
	}

	//@cmember
	/* Returns the status of the print job. For status values, please refer to the Win32 documentation on the JOB_INFO_1 structure.*/
	DWORD GetStatus()
	{
		DWORD dwStatus = JOB_STATUS_ERROR;

		DWORD dwSize;
		LPBYTE pbJob = AllocJobInfo(1, dwSize);
		if (pbJob != NULL)
		{
			if (GetJobInfo(pbJob, dwSize, 1))
			{
				dwStatus = reinterpret_cast<JOB_INFO_1*>(pbJob)->Status;
			}
			delete[] pbJob;
		}

		return dwStatus;
	}

	//@cmember
	/* Sets the job priority. For valid priority values, please refer to the Win32 documentation on the JOB_INFO_1 structure.*/
	virtual bool SetPriority(DWORD dwPriority)
	{
		bool bSuccess = false;
		DWORD dwSize;
		LPBYTE pbJob = AllocJobInfo(1, dwSize);
		if (pbJob != NULL)
		{
			JOB_INFO_1* pJobInfo1 = reinterpret_cast<JOB_INFO_1*>(pbJob);
			pJobInfo1->Priority = dwPriority;

			if (SetJobInfo(pbJob, 1))
			{
				bSuccess = true;
			}
			delete[] pbJob;
		}
		return bSuccess;
	}

// Implementation
protected:
	virtual int OnPrintDocument(IPrintable* pIPrintable, CPrintDoc* pPrintDoc)
	{
		int nPagesPrinted = 0;
		int nPageCount = pIPrintable->GetPageCount(pPrintDoc);
		bool bDonePrinting = false;

		while (!bDonePrinting)
		{
			if (nPageCount > 0 && nPagesPrinted >= nPageCount)
			{
				bDonePrinting = true;
			}
			else
			{
				if (pIPrintable->BeginPage(pPrintDoc))
				{
					if (pIPrintable->PrintPage(pPrintDoc))
					{
						nPagesPrinted++;
						pIPrintable->EndPage(pPrintDoc);
						pPrintDoc->IncrementPage();
					}
					else
					{
						bDonePrinting = true;
					}
				}
			}
		}

		return nPagesPrinted;
	}

	//@cmember
	/* Opens the printer identified by the CPrinterConfig parameter.*/
	bool OpenPrinter(CPrinterConfig* pPrinterConfig)
	{
		bool bSuccess = false;

		if (pPrinterConfig != NULL)
		{
			bSuccess = m_printer.Open(pPrinterConfig->GetDeviceName());
		}

		return bSuccess;
	}

	//@cmember
	/* Closes the printer associated with the print job.*/
	bool ClosePrinter()
	{
		return m_printer.Close();
	}

	
	DWORD GetJobInfoSize(DWORD dwLevel)
	{
		DWORD dwSize;

		if (m_printer.IsOpen() && m_nJobId != SP_ERROR)
		{
			if (!::GetJob(m_printer, m_nJobId, dwLevel, NULL, 0, &dwSize) &&
				::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				// succeeded in getting number of bytes required for structure
				return dwSize;
			}
			ClosePrinter();
		}

		return 0;
	}

	LPBYTE AllocJobInfo(DWORD dwLevel, DWORD& dwSize)
	{
		LPBYTE pbJob = NULL;
		dwSize = GetJobInfoSize(dwLevel);

		if (dwSize > 0)
		{
			pbJob = new BYTE[dwSize];
		}

		return pbJob;
	}

	bool GetJobInfo(LPBYTE pbJob, DWORD dwSize, DWORD dwLevel)
	{
		bool bSuccess = false;

		if (m_printer.IsOpen() && m_nJobId != SP_ERROR)
		{
			DWORD dwCopied;
			if (::GetJob(m_printer, m_nJobId, dwLevel, pbJob, dwSize, &dwCopied))
			{
				bSuccess = true;
			}
			ClosePrinter();
		}

		return bSuccess;
	}

	bool SetJobInfo(LPBYTE pbJob, DWORD dwLevel)
	{
		bool bSuccess = false;

		if (m_printer.IsOpen() && m_nJobId != SP_ERROR)
		{
			if (::SetJob(m_printer, m_nJobId, dwLevel, pbJob, 0))
			{
				bSuccess = true;
			}
			ClosePrinter();
		}

		return bSuccess;
	}

	virtual bool SendJobCommand(const DWORD nCommandId)
	{
		bool bSuccess = false;

		if (m_printer.IsOpen() && m_nJobId != SP_ERROR)
		{
			if (::SetJob(m_printer, m_nJobId, 0, NULL, nCommandId))
			{
				bSuccess = true;
			}
			ClosePrinter();
		}

		return bSuccess;
	}
};

};	// namespace stingray::foundation
};	// namespace stingray
