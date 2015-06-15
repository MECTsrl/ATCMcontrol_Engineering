/////////////////////////////////////////////////////////////////////////////
// @doc CMNDll
// @module CMNDll.h | Extension DLL initialization.
// 
// Objective Studio<tm>
// <nl>Copyright <cp> 1998 Stingray Software, Inc. All rights reserved.
// 
// This source code is only intended as a supplement to the Objective 
// Studio User's Guide and related electronic documentation provided with 
// the library. See these sources for detailed information regarding the 
// Objective Studio product.
// 
// Author: Stephen A. Danielson
// <nl>Created: 06/98
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef __CMNDLL_H__
#define __CMNDLL_H__

#ifdef _CMNDLL
extern "C" void WINAPI CMNInitDll();

#ifndef CMN_DATAEXT
	#define CMN_DATAEXT     AFX_DATA_IMPORT
	#define CMN_DATADEF
#endif


#endif // _CMNDLL

#endif // __CMNDLL_H__

/////////////////////////////////////////////////////////////////////////////

