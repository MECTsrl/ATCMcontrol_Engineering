/////////////////////////////////////////////////////////////////////////////
// sflall.h : All packages in Stingray Foundation Library
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
#include <Foundation\SmartPtr.h>
#include <Foundation\RefCount.h>
#include <Foundation\CppIface.h>
#include <Foundation\GuidCast.h>
#include <Foundation\Debug.h>

// Individual header files

#include <Foundation\String\StringEx.h>
#include <Foundation\String\sflstring.h>
#include <Foundation\String\sflcoll.h>

#include <Foundation\Util\TransRect.h>
#if defined(_SFL_ATL_SUPPORT)
#include <Foundation\Util\PersistSTL.h>
#endif
#include <Foundation\Util\Convert.h>


// Packages

#include <Foundation\Apps.h>
#include <Foundation\Controls.h>
#include <Foundation\DragDrop.h>
#include <Foundation\Patterns.h>
#include <Foundation\Events.h>
#include <Foundation\Graphics.h>
#include <Foundation\Image.h>
#include <Foundation\Layout.h>
#include <Foundation\MVC.h>
#include <Foundation\Print.h>
#include <Foundation\Properties.h>
#include <Foundation\StructuredGraphics.h>
