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
//  Description:  Version information for Comon libraries
//  Created:      06/24/98/

#ifndef __CMNVER_H__
#define __CMNVER_H__

/////////////////////////////////////////////////////////////////////////////
// Master version numbers
/////////////////////////////////////////////////////////////////////////////

#define _CMN_VER 0x0FFF

#ifdef _CMNDLL
#ifndef _SFLDLL
#define _SFLDLL
#endif
#endif

//
// Automatic library inclusion macros that use the #pragma/lib feature
//
// Define _CMNNOAUTOLIB when compiling CMN so that it does
// not try and link to itself.
//
#ifdef _CMNNOAUTOLIB
#ifndef _SFLNOAUTOLIB
#define _SFLNOAUTOLIB
#endif
#endif //_CMNNOAUTOLIB

#include <Foundation\SflVersion.h>

#endif // __CMNVER_H__
