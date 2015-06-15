/////////////////////////////////////////////////////////////////////////////
// Patterns.h : SFL Design patterns implementation package
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

#include <Foundation\Patterns\Composite.h>
#include <Foundation\Patterns\Factory.h>
#include <Foundation\Patterns\Iterator.h>
#include <Foundation\Patterns\SubjectObserver.h>

#ifdef _SFL_MFC_SUPPORT
#include <Foundation\Patterns\MFC\Collect.h>
#include <Foundation\Patterns\MFC\FactoryP.h>
#include <Foundation\Patterns\MFC\Listen.h>
#include <Foundation\Patterns\MFC\PlugIn.h>
#endif
