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
//
//  Author:	      Steve Danielson
//  Description:  Version information for MVC libraries
//  Created:      06/24/98/

#ifndef __MVCVER_H__
#define __MVCVER_H__

/////////////////////////////////////////////////////////////////////////////
// Master version numbers
/////////////////////////////////////////////////////////////////////////////

#define _MVC_VER 0x0FFF

#ifdef _MVCDLL
#ifndef _SFLDLL
#define _SFLDLL
#endif
#endif

#ifndef _MVCNOAUTOLIB

#include "Foundation\SflVersion.h"

#endif //_MVCNOAUTOLIB


#endif // __MVCVER_H__
