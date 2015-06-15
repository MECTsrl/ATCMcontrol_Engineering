// 
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Description:	Patterns.h - Patterns Header
//  Created:		May 1998
//
//	This file is included via "ProAll.h"
//

#ifndef __COMPAT_SEC_PATTERNS_H
#define __COMPAT_SEC_PATTERNS_H

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>	// templates used extensively
#endif

#ifndef __COMPAT_SECCOLLECT_H__
#include "Foundation\Compatibility\Common\patterns\Collect.h"
#endif

// Factory pattern 
#ifndef __COMPAT_SECFACTORY_PATTERNS_H__
#include "Foundation\Compatibility\Common\patterns\FactoryP.h"
#endif

// PlugIn pattern 
#ifndef __COMPAT_PLUGIN_H__
#include "Foundation\Compatibility\Common\patterns\PlugIn.h"
#endif

#ifndef __COMPAT_SECWndListener_H__
#include "Foundation\Compatibility\Common\patterns\Listen.h"
#endif

// Note: DO NOT INCLUDE composite.h here!
// Composite.h has conditional RTTI dependencies. Since this file
// is entirely inline (i.e. no buildable .cpp implementation),
// it can safely be included directly by any other library
// which requires the composite support.

#endif 		// __COMPAT_SEC_PATTERNS_H
