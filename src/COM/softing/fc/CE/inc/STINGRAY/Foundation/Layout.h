/////////////////////////////////////////////////////////////////////////////
// Layout.h : SFL Layout Manager package
//
// Stingray Software Foundation Library
// Copyright (C) 2000 RogueWave Software Inc,
// All rights reserved.
//

#pragma once


#if !defined(_SFL)
#define _SFL
#endif

#include <Foundation\SflCommon.h>

#include <Foundation\Layout\LayoutBase.h>
#include <Foundation\Layout\LayoutNode.h>
#include <Foundation\Layout\LayoutFactory.h>
#include <Foundation\Layout\WindowNode.h>
#include <Foundation\Layout\ScaleLayout.h>
#include <Foundation\Layout\RelativeLayout.h>
#include <Foundation\Layout\BorderClient.h>
#include <Foundation\Layout\DCNode.h>
#include <Foundation\Layout\BorderLayout.h>
#include <Foundation\Layout\BorderGraphic.h>
#include <Foundation\Layout\SplitterLayout.h>
#include <Foundation\Layout\ViewportNode.h>

#ifdef _SFL_ATL_SUPPORT
#include <Foundation\Layout\LayoutManager.h>
#endif
