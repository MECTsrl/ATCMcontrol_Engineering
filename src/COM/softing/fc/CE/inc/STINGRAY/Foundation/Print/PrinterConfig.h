/////////////////////////////////////////////////////////////////////////////
// PrinterConfig.h : SFL Print Preview package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       
// Description:  
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <commdlg.h>
#include <winspool.h>

namespace stingray {
namespace foundation {


//////////////////////////////////////////////////////////////////////////
// CPrinterConfig

//@doc CPrinterConfig
//@class The CPrinterConfig class encapsulates the Windows DEVNAMES and DEVMODE structures used by printing systems for
// identifying and configuring printers. CPrinterConfig takes care of the messy details involved in allocating and manipulating
// these variable length structures and provides convenient accessors and methods that allow you to work with them.
// All physical measurements are in units of 0.1mm (MM_LOMETRIC).
//
// NOTE: CPrinterConfig is an abstract class and cannot be directly instantiated. For configuring the printer, you must use 
// the <c CPrinterConfigImpl> template class. 
// 
//@xref <c IPrintable>
//@xref <c CPrintDoc>
//@xref <c CPrintJob>
//@xref <c CPrintPreviewFrameImpl>	
class CPrinterConfig
{
public:
	//@cmember
	/* Returns a handle to the DEVNAMES structure.*/
	virtual HGLOBAL GetDevNames() const = 0;
	//@cmember
	/* Sets the DEVNAMES structure.*/
	virtual void SetDevNames(const HGLOBAL hSrcDevNames) = 0;
	//@cmember
	/* Returns a handle to the DEVMODE structure.*/
	virtual HGLOBAL GetDevMode() const = 0;
	//@cmember
	/* Sets the DEVMODE structure.*/
	virtual void SetDevMode(const HGLOBAL hSrcDevMode) = 0;

	//@cmember
	/* Creates a device context using the information specified in the DEVNAMES and DEVMODE structures.*/
	virtual HDC CreateDC() const;
	//@cmember
	/* Updates the PRINTDLG parameter with the DEVNAMES and DEVMODE information stored in CPrinterConfig.*/
	virtual void LoadPrintDlg(PRINTDLG& printDlg);
	//@cmember
	/* Updates CPrinterConfig's DEVNAMES and DEVMODE members with the global memory handles specified in the PRINTDLG parameter.*/
	virtual void StorePrintDlg(const PRINTDLG& printDlg);
	//@cmember
	/* Updates the PAGESETUPDLG parameter with the DEVNAMES and DEVMODE handles stored in CPrinterConfig.*/
	virtual void LoadPageSetupDlg(PAGESETUPDLG& psd);
	//@cmember
	/* Updates CPrinterConfig's DEVNAMES and DEVMODE members with the handle information specified in the PAGESETUPDLG parameter.*/
	virtual void StorePageSetupDlg(const PAGESETUPDLG& psd);

	//@cmember
	/* Returns a pointer to a null-terminated string that contains the device driver file name specified in CPrintConfig's DEVNAMES member.*/
	LPTSTR GetDriverName() const;
	//@cmember
	/* Returns a pointer to a null-terminated string that contains the device name specified in CPrintConfig's DEVNAMES member.*/
	LPTSTR GetDeviceName() const;
	//@cmember
	/* Returns a pointer to a null-terminated string that contains the device name for the physical output medium.*/
	LPTSTR GetOutputName() const;

	//@cmember
	/* Sets CPrinterConfig's DEVNAMES and DEVMODE members to handles that have been initialized for the system default printer.*/  
	void SetDefaultPrinter();
	//@cmember
	/* Returns TRUE if the information specified in CPrinterConfig accurately identifies an existing printer. Else FALSE.*/
	bool PrinterExists() const;

	//@cmember
	/* Returns the paper orientation specified in the DEVMODE structure. Valid return values are DMORIENT_PORTRAIT and DMORIENT_LANDSCAPE.*/
	short GetOrientation() const;
	//@cmember
	/* Sets the printer's paper orientation. This can be either DMORIENT_PORTRAIT or DMORIENT_LANDSCAPE.*/
	void SetOrientation(const short nOrientation);

	//@cmember
	/* Returns the paper size specified in CPrinterConfig's DEVMODE structure. For information on paper sizes, please refer to the Win32 documentation on the DEVMODE structure.*/
	short GetPaperSize() const;
	//@cmember
	/* Sets the printer's paper size. For information on paper sizes, please refer to the Win32 documentation on the DEVMODE structure.*/
	void SetPaperSize(const short nPaperSize);

	//@cmember
	/* Returns the paper length specified in the DEVMODE structure's dmPaperLength member.*/
	short GetPaperLength() const;
	//@cmember
	/* Sets the dmPaperLength member in CPrinterConfig's DEVMODE structure.*/
	void SetPaperLength(const short nPaperLength);

	//@cmember
	/* Returns the paper width specified in the DEVMODE structure's dmPaperWidth member.*/
	short GetPaperWidth() const;
	//@cmember
	/* Sets the dmPaperWidth member in CPrinterConfig's DEVMODE structure.*/
	void SetPaperWidth(const short nPaperWidth);

	//@cmember
	/* Returns the number of copies specified in CPrinterConfig's DEVMODE member.*/
	short GetNumCopies() const;
	//@cmember
	/* Sets the number of copies, dmCopies, in CPrinterConfig's DEVMODE structure.*/
	void SetNumCopies(const short nNumCopies);

	//@cmember
	/* Retrieves the logical coordinates identifying the physical page's printable area.*/
	bool GetPrinterMargins(RECT& rcPrinterMargins);
	//@cmember
	/* Retrieves the paper size specified in CPrinterConfig's DEVMODE member.*/
	bool GetPaperMeasurements(SIZE& szPaper);

	static int CalcDevNamesSize(LPDEVNAMES lpDevNames);
	static HGLOBAL DupDevNames(HGLOBAL hSrcDevNames);
	static int CalcDevModeSize(HGLOBAL hDevNames);
	static HGLOBAL DupDevMode(HGLOBAL hSrcDevMode);
};




//////////////////////////////////////////////////////////////////////////
// CPrinterConfigImpl

//@doc CPrinterConfigImpl
//@class The CPrinterConfigImpl template class derives from <c CPrinterConfig> and provides the requisite concretization
// of the CPrinterConfig abstract base class.
//@tcarg bool | bOwnsHandles = true | Default value of true assigns CPrinterConfigImpl ownership of the DEVMODE and DEVNAMES global
// memory handles and frees them when the CPrintConfigImpl object destructs.
//@base public | CPrinterConfig
template <bool bOwnsHandles = true>
class CPrinterConfigImpl : public CPrinterConfig
{
// Constructors/destructor
public:
	//@cmember,mfunc
	// Constructs the printer configuration object.
	//@@syntax CPrinterConfigImpl()
	//@@syntax CPrinterConfigImpl(HANDLE hDevMode, HANDLE hDevNames)
	//@@parm HANDLE | hDevMode | DEVMODE handle. 
	//@@parm HANDLE | hDevNames | DEVNAMES handle.
	/* Constructs the printer configuration object.*/
	CPrinterConfigImpl()
	{
		m_hDevMode = NULL;
		m_hDevNames = NULL;
	}

	CPrinterConfigImpl(HANDLE hDevMode, HANDLE hDevNames)
	{
		m_hDevMode = NULL;
		m_hDevNames = NULL;
		SetDevMode(hDevMode);
		SetDevNames(hDevNames);
	}

	virtual ~CPrinterConfigImpl()
	{
		if (bOwnsHandles)
		{
			if (m_hDevNames != NULL)
			{
				::GlobalFree(m_hDevNames);
			}

			if (m_hDevMode != NULL)
			{
				::GlobalFree(m_hDevMode);
			}
		}
	}

// Attributes
protected:
	//@cmember
	/* Handle to the Win32 DEVMODE structure.*/
	HANDLE m_hDevMode;
	//@cmember
	/* Handle to the Win32 DEVNAMES structure.*/
	HANDLE m_hDevNames;

// Operations
public:
	virtual HGLOBAL GetDevNames() const
	{
		return m_hDevNames;
	}

	virtual void SetDevNames(const HGLOBAL hSrcDevNames)
	{
		if (bOwnsHandles)
		{
			if (m_hDevNames != NULL)
			{
				::GlobalFree(m_hDevNames);
			}
		}

		m_hDevNames = hSrcDevNames;
	}

	virtual HGLOBAL GetDevMode() const
	{
		return m_hDevMode;
	}

	virtual void SetDevMode(const HGLOBAL hSrcDevMode)
	{
		if (bOwnsHandles)
		{
			if (m_hDevMode != NULL)
			{
				::GlobalFree(m_hDevMode);
			}
		}

		m_hDevMode = hSrcDevMode;
	}
};



};	// namespace stingray::foundation
};	// namespace stingray
