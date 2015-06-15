/////////////////////////////////////////////////////////////////////////////
// GCCore.h : Advanced graphics package
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Bob Powell
// Description:  Advanced Graphics Package
//


#pragma once

#include <Foundation\APIStructs\GCUtil.h>
#include <Foundation\Graphics\GDIObjects.h>
#include <Foundation\Graphics\GraphicContext.h>


namespace stingray {
namespace foundation {


#if !defined(_SFL_MFC_SUPPORT)
	// MFC-compatible alias
	typedef CGraphicsContext CDC;
	typedef CGDIPen CPen;
	typedef CGDIBrush CBrush;
	typedef CGDIBitmap CBitmap;
	typedef CGDIFont CFont;
	typedef CGDIRgn CRgn;
	typedef CGDIPalette CPalette;
#endif


}; // foundation
}; // stingray

