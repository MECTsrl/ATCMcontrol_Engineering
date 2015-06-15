// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:			Mark Isham
//  Description:	MVCAll.h - MVC composite header
//  Created:		October 1997
//
//
//	This file is included via "ProAll.h"
//

#ifndef __MODEL_VIEW_CONTROLLER_HEADERS_
#define __MODEL_VIEW_CONTROLLER_HEADERS_

// Common Libraries
// If MVC is being linked as a dll, then we must specify
// to link the common library as a dll
#ifdef _MVCDLL
	#ifndef _CMNDLL
		#define _CMNDLL
	#endif
#endif

#define _SFL_MFC_SUPPORT

#include "Foundation\Compatibility\MVC\mvcver.h"

// Objective Studio Common Library header
// Set up  the components in CMN that MVC depends upon
#ifndef CMN_BLD_PATTERNS
	#define CMN_BLD_PATTERNS
#endif

#ifndef _DECORATE_VISUAL_H_NAMES
#define _DECORATE_VISUAL_H_NAMES
#endif

#include <Foundation\Compatibility\Common\cmnall.h>
#include <Foundation\Compatibility\Common\cmn_cnt.h>

// MVC Libraries
#include "Foundation\Compatibility\Common\IRefCount.h"
#include "Foundation\Compatibility\MVC\MvcIfcs.h"
#include "Foundation\Compatibility\MVC\SmartPtr.h"
#include "Foundation\Compatibility\MVC\MvcMsgHandler.h"
#include "Foundation\Compatibility\MVC\MvcBorderWrapper.h"
#include "Foundation\Compatibility\MVC\MvcBufferedWrapper.h"
#include "Foundation\Compatibility\MVC\MvcCommand.h"
#include "Foundation\Compatibility\MVC\MvcModel.h"
#include "Foundation\Compatibility\MVC\MvcTransactionModel.h"
#include "Foundation\Compatibility\MVC\MvcViewport.h"
#include "Foundation\Compatibility\MVC\MvcVisualComponent.h"
#include "Foundation\Compatibility\MVC\MvcVisualPart.h"
#include "Foundation\Compatibility\MVC\MvcPresentationModel.h"
#include "Foundation\Compatibility\MVC\MvcLogicalPart.h"
#include "Foundation\Compatibility\MVC\MvcWrapper.h"
#include "Foundation\Compatibility\MVC\MvcController.h"
#include "Foundation\Compatibility\MVC\MvcScrollWrapper.h"
#include "Foundation\Compatibility\MVC\MvcScrollView.h"
#include "Foundation\Compatibility\MVC\MvcCompositeViewport.h"
#include "Foundation\Compatibility\MVC\MvcDefSbjObs.h"

#endif 		// __MODEL_VIEW_CONTROLLER_HEADERS_
