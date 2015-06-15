/////////////////////////////////////////////////////////////////////////////
// Printable.h : SFL Print Preview package 
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Jeff Boenig
// Description:  Declaration of the IPrintable interface.
//

#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#include <Foundation\Print\PrintDoc.h>

namespace stingray {
namespace foundation {

//////////////////////////////////////////////////////////////////////////
//@doc IPrintable

//@class IPrintable | This interface is implemented by objects to support
// printing and print preview. The methods in this interface provide a
// means to print a page at time to a <c CPrintDoc> object and to
// determine the total number of pages the printable object contains.

class __declspec(uuid("9B35CA0F-7A12-401e-8BD5-4330074FF35B"))
	IPrintable
{
public:
	//@cmember
	/* Get number of pages in document. */
	virtual int GetPageCount(CPrintDoc* pPrintDoc) = 0;
	//@cmember
	/* Prepare the next page for printing. */
	virtual bool BeginPage(CPrintDoc* pPrintDoc) = 0;
	//@cmember
	/* Print the current page to the print document. */
	virtual bool PrintPage(CPrintDoc* pPrintDoc) = 0;
	//@cmember
	/* Cleanup after printing a page. */
	virtual bool EndPage(CPrintDoc* pPrintDoc) = 0;
};

};	// namespace stingray::foundation
};	// namespace stingray
