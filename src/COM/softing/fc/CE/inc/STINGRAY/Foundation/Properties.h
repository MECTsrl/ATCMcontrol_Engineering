/////////////////////////////////////////////////////////////////////////////
// Properties.h : SFL properties package
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

#if defined(_ATL)
#include <Foundation\Properties\PropIface.h>
#include <Foundation\Properties\Property.h>
#include <Foundation\Properties\Enumeration.h>
#include <Foundation\Properties\PropAccessor.h>
#include <Foundation\Properties\PropContainer.h>
#include <Foundation\Properties\PropEditorBase.h>
#endif

#if defined(_SFL_ATL_SUPPORT)
#include <Foundation\Properties\AxProperty.h>
#include <Foundation\Properties\AxPropContainer.h>
#endif // #if defined(_SFL_ATL_SUPPORT)
