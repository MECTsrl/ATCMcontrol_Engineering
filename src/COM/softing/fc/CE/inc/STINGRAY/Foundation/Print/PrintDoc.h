/////////////////////////////////////////////////////////////////////////////
// PrintDoc.h : SFL Print Preview package 
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

#include <commdlg.h>
#include <tchar.h>

#include <Foundation\Print\PrinterConfig.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
// CPrintDoc

//@doc CPrintDoc
//@class The CPrintDoc class encapsulates the Windows DOCINFO structure, printer configuration data, printer device context
// and exposes an interface for rendering output to a printer or file. CPrintDoc provides the necessary support for
// print preview by maintaining two device context - a printer device context and an output device context. 
// The printer dc always refers to a device context that represents the printer. The output dc, however, will represent
// the display screen during a print preview operation and the printer dc while printing.
// 
// During a printing operation, CPrintDoc's public accessors can be used to retrieve information 
// such as the current page number, document info and printer configuration data. 
//
//@xref <c IPrintable>
//@xref <c CPrinterConfig>
//@xref <c CPrintJob>
//@xref <c CPrintPreviewFrameImpl>
class CPrintDoc
{
// Constructors/destructor
public:
	CPrintDoc() :
		m_dcPrint(NULL),
		m_dcOutput(NULL),
		m_nCurPage(1),
		m_pPrinterConfig(NULL)
	{
		memset(&m_docInfo, 0, sizeof(DOCINFO));
		m_docInfo.cbSize = sizeof(DOCINFO);
		SetDocumentName(_T("Document"));
		m_bWYSIWYG = FALSE;
	}

	virtual ~CPrintDoc()
	{
		SetDocumentName(NULL);
		SetOutputFileName(NULL);
		SetDataType(NULL);
	}

// Attributes
protected:
	//@cmember
	/* Win32 structure containing the input and output file names and other printing related information.*/
	DOCINFO m_docInfo;
	//@cmember
	/* Handle to the printer device context.*/
	HDC m_dcPrint;
	//@cmember
	/* Handle to the output device context. In preview mode, this refers to the screen DC.*/
	HDC m_dcOutput;
	//@cmember
	/* The current page number.*/
	int m_nCurPage;
	//@cmember
	/* Printer configuration information.*/
	CPrinterConfig* m_pPrinterConfig;
	BOOL m_bWYSIWYG;

// Operations
public:

	//@cmember
	/* Retrieves the document name from the DOCINFO structure. */
	LPCTSTR GetDocumentName() const
	{
		return m_docInfo.lpszDocName;
	}

	//@cmember
	/* Sets the document name in the DOCINFO structure.*/
	void SetDocumentName(LPCTSTR lpszDocName)
	{
		if (m_docInfo.lpszDocName != NULL)
		{
			free((void*) m_docInfo.lpszDocName);
			m_docInfo.lpszDocName = NULL;
		}

		if (lpszDocName != NULL)
		{
			m_docInfo.lpszDocName = _tcsdup(lpszDocName);
		}
	}

	//@cmember
	/* Returns the output file name.*/
	LPCTSTR GetOutputFileName() const
	{
		return m_docInfo.lpszOutput;
	}

	//@cmember
	/* Sets the output file name.*/
	void SetOutputFileName(LPCTSTR lpszOutput)
	{
		if (m_docInfo.lpszOutput != NULL)
		{
			free((void*) m_docInfo.lpszOutput);
			m_docInfo.lpszOutput = NULL;
		}

		if (lpszOutput != NULL)
		{
			m_docInfo.lpszOutput = _tcsdup(lpszOutput);
		}
	}

	//@cmember
	/* Returns the data type specified for the print job.*/
	LPCTSTR GetDataType() const
	{
		return m_docInfo.lpszDatatype;
	}

	//@cmember
	/* Sets the data type to be used for the print job.*/
	void SetDataType(LPCTSTR lpszDatatype)
	{
		if (m_docInfo.lpszDatatype != NULL)
		{
			free((void*) m_docInfo.lpszDatatype);
			m_docInfo.lpszDatatype = NULL;
		}

		if (lpszDatatype != NULL)
		{
			m_docInfo.lpszDatatype = _tcsdup(lpszDatatype);
		}
	}

	//@cmember
	/* Returns any additional information specified about the print job.*/
	DWORD GetType() const
	{
		return m_docInfo.fwType;
	}

	//@cmember
	/* Sets additional information about the print job.*/
	void SetType(const DWORD fwType)
	{
		m_docInfo.fwType = fwType;
	}

	//@cmember
	/* Sets the output device context.*/
	void SetOutputDC(const HDC hDC)
	{
		m_dcOutput = hDC;
	}

	//@cmember
	/* Returns a pointer to the printer configuration data that CPrintDoc maintains.*/
	virtual CPrinterConfig* GetPrinterConfig()
	{
		return m_pPrinterConfig;
	}

	//@cmember
	/* Sets the printer configuration data for the document.*/
	void SetPrinterConfig(CPrinterConfig* pPrinterConfig)
	{
		m_pPrinterConfig = pPrinterConfig;
	}

	//@cmember
	/* Returns the output device context.*/
	HDC GetOutputDC() const
	{
		return m_dcOutput;
	}

	//@cmember
	/* Sets the device context handle used for printing.*/
	void SetPrintDC(const HDC hDC)
	{
		m_dcPrint = hDC;
	}

	//@cmember
	/* Returns the printer device context.*/
	HDC GetPrintDC() const
	{
		return m_dcPrint;
	}

	//@cmember
	/* Starts the print job by invoking the Win32 ::StartDoc() API.*/  
	virtual int StartDoc()
	{
		return ::StartDoc(m_dcPrint, &m_docInfo);
	}

	//@cmember
	/* Signals termination of the print job.*/
	virtual int EndDoc()
	{
		return ::EndDoc(m_dcPrint);
	}

	//@cmember
	/* Stops the current print job.*/
	virtual int AbortDoc()
	{
		return ::AbortDoc(m_dcPrint);
	}

	int GetFromPage() const
	{
		return 0;
	}

	int GetToPage() const
	{
		return 0;
	}

	//@cmember
	/* Retrieves the current page number.*/
	int GetCurPage() const
	{
		return m_nCurPage;
	}

	//@cmember
	/* Sets the current page number.*/
	void SetCurPage(const int nCurPage)
	{
		m_nCurPage = nCurPage;
	}

	//@cmember
	/* Increments the current page number.*/
	int IncrementPage()
	{
		return ++m_nCurPage;
	}

	BOOL IsWYSIWYGEnabled()
	{
		return m_bWYSIWYG;
	}


	virtual void ScaleOutputToScreenResolution()
	{
		int nMapMode = ::GetMapMode(m_dcOutput);
		if ( nMapMode == MM_ANISOTROPIC || nMapMode == MM_ISOTROPIC || nMapMode == MM_TEXT )
		{
			m_bWYSIWYG = TRUE;
			HDC hScreenDC = ::CreateCompatibleDC(0);
			SIZE szViewportExt; 
			GetViewportExtEx(m_dcOutput, &szViewportExt);

			int baseDpiX = ::GetDeviceCaps(hScreenDC, LOGPIXELSX);
			int baseDpiY = ::GetDeviceCaps(hScreenDC, LOGPIXELSY);
			int destDpiX = ::GetDeviceCaps(m_dcOutput, LOGPIXELSX);
			int destDpiY = ::GetDeviceCaps(m_dcOutput, LOGPIXELSY);

			if (baseDpiX != destDpiX || baseDpiY != destDpiY)
			{
				//
				// Resolution of destination device differs from the
				// screen resolution. Scale the viewport extents so
				// that logical units on the destination device are
				// equivalent to logical units on the screen.
				//
				szViewportExt.cx = (szViewportExt.cx * destDpiX) / baseDpiX;
				szViewportExt.cy = (szViewportExt.cy * destDpiY) / baseDpiY;

				SIZE szPrevViewportExt;
				::SetMapMode(m_dcOutput, MM_ANISOTROPIC);
				::SetViewportExtEx(m_dcOutput, szViewportExt.cx, szViewportExt.cy, &szPrevViewportExt);
			}

			::DeleteDC(hScreenDC);
		}
	}

	virtual void ScaleOutputToPrinterResolution()
	{
		int nMapMode = ::GetMapMode(m_dcOutput);
		if ( nMapMode == MM_ANISOTROPIC || nMapMode == MM_ISOTROPIC || nMapMode == MM_TEXT )
		{
			// WYSIWYG flag is set to false. CMvcPrintable::OnPrepareDC() may or may not enable it the next time.
			m_bWYSIWYG = FALSE;
			if (m_dcPrint == m_dcOutput || m_dcPrint == 0 || m_dcOutput == 0)
				return;

			int outDpiX = ::GetDeviceCaps(m_dcOutput, LOGPIXELSX);
			int outDpiY = ::GetDeviceCaps(m_dcOutput, LOGPIXELSY);
			int prtDpiX = ::GetDeviceCaps(m_dcPrint, LOGPIXELSX);
			int prtDpiY = ::GetDeviceCaps(m_dcPrint, LOGPIXELSY);

			if (outDpiX != prtDpiX || outDpiY != prtDpiY)
			{
				//
				// Resolution of printer differs from resolution of the
				// output device. Scale the window extents of the output
				// device so that logical units are equivalent to logical
				// units on the printer.
				//
				SIZE szWndExt;
				SIZE szPrevWndExt;

				::GetWindowExtEx(m_dcOutput, &szWndExt);

				szWndExt.cx = (szWndExt.cx * prtDpiX) / outDpiX;
				szWndExt.cy = (szWndExt.cy * prtDpiY) / outDpiY;

				::SetMapMode(m_dcOutput, MM_ANISOTROPIC);
				::SetWindowExtEx(m_dcOutput, szWndExt.cx, szWndExt.cy, &szPrevWndExt);
			}
		}
	}
};

};	// namespace stingray::foundation
};	// namespace stingray
