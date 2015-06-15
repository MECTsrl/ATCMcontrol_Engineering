/////////////////////////////////////////////////////////////////////////////
// Controls.h : SFL controls implementation package
//
// Stingray Software Foundation Library
// Copyright (C) 2000 RogueWave Software Inc,
// All rights reserved.
//

#pragma once


#if !defined(_SFL)
#define _SFL
#endif


#if defined(_SFL_ATL_SUPPORT)
#include <Foundation\Controls\atlcontrols.h>
#include <Foundation\Controls\StatusBars.h>
#include <Foundation\Controls\Toolbars.h>
#endif

#ifdef _SFL_MFC_SUPPORT
#include <Foundation\Controls\MFC\secbtns.h>
#include <Foundation\Controls\MFC\secwell.h>
#endif
