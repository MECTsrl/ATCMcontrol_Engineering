/////////////////////////////////////////////////////////////////////////////
// Mvc.h : SFL Model View Controller package
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

#include <Foundation\MVC\MvcVisualComponent.h>
#include <Foundation\MVC\MvcVisualPart.h>
#include <Foundation\MVC\MvcLogicalPart.h>
#include <Foundation\MVC\MvcCommand.h>
#include <Foundation\MVC\MvcModel.h>
#include <Foundation\MVC\MvcController.h>
#include <Foundation\MVC\MvcViewport.h>
#include <Foundation\MVC\MvcComponent.h>
#include <Foundation\MVC\MvcBorderWrapper.h>
#include <Foundation\MVC\MvcPrintable.h>
#include <Foundation\MVC\MvcWrapper.h>
#include <Foundation\MVC\MvcZoomCommand.h>

#ifdef _SFL_MFC_SUPPORT
#include <Foundation\MVC\MFC\MvcMsgHandler.h>
#include <Foundation\MVC\MFC\MvcBufferedWrapper.h>
#include <Foundation\MVC\MFC\MvcCommand.h>
#include <Foundation\MVC\MFC\MvcCompositeViewport.h>
#include <Foundation\MVC\MFC\MvcController.h>
#include <Foundation\MVC\MFC\MvcDefSbjObs.h>
#include <Foundation\MVC\MFC\MvcIfcs.h>
#include <Foundation\MVC\MFC\MvcLogicalPart.h>
#include <Foundation\MVC\MFC\MvcModel.h>
#include <Foundation\MVC\MFC\MvcBufferedVp.h>
#include <Foundation\MVC\MFC\MvcPresentationModel.h>
#include <Foundation\MVC\MFC\MvcScrollView.h>
#include <Foundation\MVC\MFC\MvcScrollWrapper.h>
#include <Foundation\MVC\MFC\MvcTransactionModel.h>
#include <Foundation\MVC\MFC\MvcViewport.h>
#include <Foundation\MVC\MFC\MvcVisualComponent.h>
#include <Foundation\MVC\MFC\MvcVisualPart.h>
#endif

#ifdef _SFL_ATL_SUPPORT
#include <Foundation\MVC\MvcAtl.h>
#include <Foundation\MVC\MvcClientViewport.h>
#include <Foundation\MVC\MvcScrollWrapper.h>

#if defined(_WIN32_IE) && (_WIN32_IE >= 0x5000)
#include <Foundation\MVC\MvcPersist.h>
#endif

#endif
