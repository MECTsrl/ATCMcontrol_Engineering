/////////////////////////////////////////////////////////////////////////////
// PrintPreviewModel.h : Print preview model class.
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Print preview model class.
//

#pragma once

#include <Foundation\MVC\MvcModel.h>
#include <Foundation\Print\Printable.h>
#include <Foundation\Print\PrintDoc.h>
#include <Foundation\Util\Convert.h>
#include <Foundation\Debug.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc CPrtPreviewModel

//@class This class implements the print preview model. It derives from
// IPrintJobImpl and contains a pointer to the IPrintable interface of the
// object that is being printed.

class CPrtPreviewModel : public CMvcModel
{
// Constructors/destructor
public:
	CPrtPreviewModel(CPrinterConfig* pPrinterConfig = NULL) :
		m_pPrintDoc(NULL),
		m_pIPrintable(NULL),
		m_nFirstVisiblePage(1),
		m_nNumVisiblePages(1)
	{
		// To get rid of warning...
		pPrinterConfig;
	}

// Attributes
protected:
	IPrintable* m_pIPrintable;
	CPrintDoc* m_pPrintDoc;
	int m_nFirstVisiblePage;
	int m_nNumVisiblePages;

// Operations
public:

	//@cmember
	/* Get a pointer to the print document object. */
	CPrintDoc* GetPrintDoc()
	{
		return m_pPrintDoc;
	}

	//@cmember
	/* Get a pointer to the object being printed. */
	virtual IPrintable* GetPrintable()
	{
		return m_pIPrintable;
	}

	//@cmember
	/* Return the number of pages currently visible. */
	int GetNumVisiblePages() const
	{
		return m_nNumVisiblePages;
	}

	//@cmember
	/* Set the number of pages currently visible. */
	void SetNumVisiblePages(const int nNumVisiblePages)
	{
		m_nNumVisiblePages = nNumVisiblePages;
	}

	//@cmember
	/* Return the page number of the first page currently visible. */
	int GetFirstVisiblePage() const
	{
		return m_nFirstVisiblePage;
	}

	//@cmember
	/* Set the page number of the first page currently visible. */
	void SetFirstVisiblePage(const int nFirstVisiblePage)
	{
		m_nFirstVisiblePage = nFirstVisiblePage;
	}

	//@cmember
	/* Preview the next page. */
	virtual void GotoNextPage()
	{
		m_nFirstVisiblePage++;
		UpdateAllObservers(NULL, NULL);
	}

	//@cmember
	/* Preview the next page. */
	virtual void GotoPrevPage()
	{
		m_nFirstVisiblePage--;
		UpdateAllObservers(NULL, NULL);
	}

	//@cmember
	/* Return the print preview background color. */
	virtual COLORREF GetBackgroundColor()
	{
		return RGB(97,97,97);  // gray
	}

	//@cmember
	/* Return the print preview page color. */
	virtual COLORREF GetPageColor()
	{
		return RGB(255,255,255);  // white
	}

	//@cmember
	/* Start the print job. */
	virtual bool Start(IPrintable* pIPrintable, CPrintDoc* pPrintDoc)
	{
		bool bSuccess = false;

		m_pIPrintable = pIPrintable;
		m_pPrintDoc = pPrintDoc;

		if (m_pPrintDoc != NULL)
		{
			CPrinterConfig* pPrinterConfig = m_pPrintDoc->GetPrinterConfig();

			if (pPrinterConfig != NULL)
			{
				HDC hPrintDC = pPrinterConfig->CreateDC();
				if (hPrintDC != NULL)
				{
					m_pPrintDoc->SetPrintDC(hPrintDC);
					UpdateAllObservers(NULL, NULL);
					bSuccess = true;
				}
			}
		}

		return bSuccess;
	}
};

}; // namespace stingray::foundation
}; // namespace stingray

