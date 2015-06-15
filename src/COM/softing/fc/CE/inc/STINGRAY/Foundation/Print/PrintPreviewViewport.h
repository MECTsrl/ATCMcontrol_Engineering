/////////////////////////////////////////////////////////////////////////////
// PrintPreviewViewport.h : Print preview viewport class.
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Print preview viewport class.
//

#pragma once

#include <Foundation\Print\PrintPreviewModel.h>
#include <Foundation\Print\PrintPreviewController.h>
#include <Foundation\MVC\MvcLogicalPart.h>
#include <Foundation\MVC\MvcModel.h>
#include <Foundation\MVC\MvcViewport.h>
#include <Foundation\MVC\MvcScrollWrapper.h>
#include <Foundation\Events\EventRouter.h>
#include <Foundation\Debug.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc CPrtPreviewViewport

//@class This class implements a print preview viewport.  It renders the
// IPrintable object contained by the model onto a page.

class CPrtPreviewViewport : public CListeningRouter< CMvcViewport<CMvcLogicalPartImpl, CPrtPreviewModel, CPrtPreviewController> >
{
// Embedded types
public:
	typedef CListeningRouter< CMvcViewport<CMvcLogicalPartImpl, CPrtPreviewModel, CPrtPreviewController> > _BaseViewport;

// Operations
public:
	//@cmember
	/* Get the total virtual size in logical units*/
	virtual CSize GetVirtualSize() const
	{
		CSize szVirtual(GetPaperSize());
		CSize szPageOffset(GetPreviewPageOffset());
		szVirtual.cx += (szPageOffset.cx * 2);
		szVirtual.cy += (szPageOffset.cy * 2);
		return szVirtual;
	}

	//@cmember
	/* Get width and height of printed page in logical units. */
	CSize GetPaperSize() const
	{
		CSize szPaper(0,0);

		CPrtPreviewModel* pModel = GetModel();
		if (pModel != NULL)
		{
			CPrinterConfig* pPrtCfg = pModel->GetPrintDoc()->GetPrinterConfig();
			if (pPrtCfg != NULL)
			{
				pPrtCfg->GetPaperMeasurements(szPaper);
				_BaseViewport::DC dcConvert(const_cast<CPrtPreviewViewport*>(this), TRUE);
				Convert::LPtoLP(dcConvert, MM_LOMETRIC, szPaper);
			}
		}
		return szPaper;
	}

	//@cmember
	/* Get page margins in logical units. */
	CRect GetPrinterMargins() const
	{
		CRect rcMargins(0,0,0,0);
		CPrtPreviewModel* pModel = GetModel();
		if (pModel != NULL)
		{
			CPrinterConfig* pPrtCfg = pModel->GetPrintDoc()->GetPrinterConfig();
			if (pPrtCfg != NULL)
			{
				pPrtCfg->GetPrinterMargins(rcMargins);
			}
		}
		return rcMargins;
	}

	//@cmember
	/* Get minimum margins around page display area. */
	virtual CSize GetPreviewPageOffset() const
	{
		CSize szPad(50,50);  // 5mm
		_BaseViewport::DC dcConvert(const_cast<CPrtPreviewViewport*>(this), TRUE);
		Convert::LPtoLP(dcConvert, MM_LOMETRIC, szPad);
		return szPad;
	}

	//@cmember
	/* Get bounds of a given page in logical units. */
	CRect GetPageBounds(const int nPageNo) const
	{
		// To get rid of warning...
		nPageNo;

		CRect rcPage(0,0,0,0);

		// Make sure that the virtual area is centered
		// within the logical bounds
		CSize szVirtual(GetVirtualSize());
		CSize szLog(GetLogSize());
		if (szVirtual.cx < szLog.cx)
		{
			rcPage.left = (szLog.cx - szVirtual.cx) / 2;
		}
		if (szVirtual.cy < szLog.cy)
		{
			rcPage.top = (szLog.cy - szVirtual.cy) / 2;
		}

		CSize szPage(GetPaperSize());
		CSize szPageOffset(GetPreviewPageOffset());
		rcPage.left += szPageOffset.cx;
		rcPage.right = rcPage.left + szPage.cx;
		rcPage.top += szPageOffset.cy;
		rcPage.bottom = rcPage.top + szPage.cy;

		return rcPage;
	}

	//@cmember
	/* Set logical origin so that virtual bounds are centered in viewport. */
	virtual void CenterPreviewPage()
	{
		CSize szVirtual(GetVirtualSize());
		CSize szLog(GetLogSize());
		SetLogCenter(szVirtual.cx / 2, szVirtual.cy / 2);
	}

	//@cmember
	/* Render to viewport to a device context. */
	virtual void Draw(CDC* pDC)
	{
		CRgn rgnFill;
		rgnFill.CreateRectRgnIndirect(GetLogBounds());

		CPrtPreviewModel* pModel = GetModel();
		if (pModel != NULL)
		{
			CPrintDoc* pPrintDoc = pModel->GetPrintDoc();
			if (pPrintDoc != NULL)
			{
				pPrintDoc->SetOutputDC((HDC)*pDC);

				int nFirstPage = pModel->GetFirstVisiblePage();
				int nLastPage = nFirstPage + pModel->GetNumVisiblePages() - 1;

				// Compute the complex region for the background that excludes the printed pages
				// and paint it.
				CRgn rgnPage, rgnTempFill;
				rgnPage.CreateRectRgn(0,0,0,0);
				rgnTempFill.CreateRectRgn(0,0,0,0);
				for (int nCurPage = nFirstPage; nCurPage <= nLastPage; nCurPage++)
				{
					// Create a RGN for the page rect and exclude this from the logbounds region.					
					rgnPage.SetRectRgn(GetPageBounds(nCurPage));
					rgnTempFill.CopyRgn(rgnFill);
					rgnFill.CombineRgn(rgnTempFill, rgnPage, RGN_DIFF);
				}
				DrawBackgroundRgn(pDC, &rgnFill);

				for (nCurPage = nFirstPage; nCurPage <= nLastPage; nCurPage++)
					DrawPage(pDC, nCurPage);
			}
		}
	}	

	//@cmember
	/* Render a given page to a device context. */
	virtual void DrawPage(CDC* pDC, const int nPageNo)
	{
		CPrtPreviewModel* pModel = GetModel();
		ASSERT(pModel != NULL);
		CPrintDoc* pPrintDoc = pModel->GetPrintDoc();
		ASSERT(pPrintDoc != NULL);

		IPrintable* pIPrintable = pModel->GetPrintable();
		if (pIPrintable == NULL)
			return;
		pPrintDoc->SetCurPage(nPageNo);

		CRect rcMargins(GetPrinterMargins());
		CRect rcPage(GetPageBounds(nPageNo));
		if (pIPrintable->BeginPage(pPrintDoc))
		{		
			OnPrepareDC(pDC);
			// Draw blank page
			CBrush brFill(pModel->GetPageColor());
			pDC->FillRect(&rcPage, &brFill);
			POINT ptPrevVpOrg;

			// Show contents within the margins of current sheet of paper
			pDC->LPtoDP(&rcPage);
			CRgn rgnPage;
			rgnPage.CreateRectRgn(rcPage.left + rcMargins.left,
			                      rcPage.top + rcMargins.top,
			                      rcPage.right + rcMargins.right,
			                      rcPage.bottom + rcMargins.bottom);
			pDC->SelectClipRgn(&rgnPage);

			// Set viewport origin of DC to correspond to top left corner of page
			::SetViewportOrgEx((HDC)*pDC, rcPage.left + rcMargins.left,
			                 rcPage.top + rcMargins.top, &ptPrevVpOrg);

			// Scale the extents of the output DC so that it resembles the resolution of the printer.
			if(!pPrintDoc->IsWYSIWYGEnabled())
				pPrintDoc->ScaleOutputToPrinterResolution();			

			// Have printable object print the page
			pIPrintable->PrintPage(pPrintDoc);
			pIPrintable->EndPage(pPrintDoc);

			::SetViewportOrgEx((HDC)*pDC, ptPrevVpOrg.x, ptPrevVpOrg.y, NULL);
		}
	}	

	virtual void DrawBackgroundRgn(CDC* pDC, CRgn* pRgnFill)
	{
		COLORREF crBackground = RGB(97,97,97); // gray
		CPrtPreviewModel* pModel = GetModel();
		if (pModel != NULL)
			crBackground = pModel->GetBackgroundColor();
		CBrush brFill(crBackground);
		pDC->FillRgn(*pRgnFill, brFill);
	}

	//@cmember
	/* Fill the viewport background. */
	virtual void DrawBackground(CDC* pDC)
    {
		COLORREF crBackground = RGB(97,97,97); // gray
		CPrtPreviewModel* pModel = GetModel();
		if (pModel != NULL)
		{
			crBackground = pModel->GetBackgroundColor();
		}
		CRect rcFill(GetLogBounds());
		CBrush brFill(crBackground);
		pDC->FillRect(&rcFill, &brFill);
	}

	//@cmember
	/* Called by framework before viewport is displayed. */
	virtual void OnInitialUpdate()
	{
		SetMapMode(MM_ANISOTROPIC);
		SetExtents(1000,1000);
		SetLogExtents(1000,1000);
		_BaseViewport::OnInitialUpdate();
		CenterPreviewPage();
	}

	//@cmember
	/* Called when the model is changed. */
	virtual void OnUpdate(ISubject* pSubject, IMessage* pMsg)
	{
		// To get rid of warnings
		pMsg;
		pSubject;

		CenterPreviewPage();
		InvalidateRect(GetBounds(), true);
	}

	// GUID map implements QueryGuid() method
	BEGIN_GUID_MAP(CPrtPreviewViewport)
		GUID_CHAIN_ENTRY(_BaseViewport)
	END_GUID_MAP
};

typedef MvcScrollWrapper_T<CPrtPreviewViewport> CPrtPreviewScrollViewport;

}; // namespace stingray::foundation
}; // namespace stingray

