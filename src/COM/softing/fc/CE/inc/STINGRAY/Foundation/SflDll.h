/////////////////////////////////////////////////////////////////////////////
//@doc SFLDll
//@module SFLDll.h | DLL initialization.
// 
// Stingray Foundation Library<tm>
// <nl>Copyright <cp> 1999 Stingray Software, Inc. All rights reserved.
// 
// This source code is only intended as a supplement to the Stingray 
// Foundation Library User's Guide and related electronic documentation provided with 
// the library. See these sources for detailed information regarding the 
// Stingray Foundation Library product.
// 
// Author: GF
// <nl>Created: 11/99
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef __SFLDLL_H__
#define __SFLDLL_H__

#ifdef _SFLDLL
	#ifdef _SFL_MFC_SUPPORT	
		#ifndef SFL_DATAEXT
			#define SFL_DATAEXT     AFX_DATA_IMPORT
			#define SFL_DATADEF
		#endif
	#else
	#endif
#endif // _SFLDLL

#endif // __SFLDLL_H__

/////////////////////////////////////////////////////////////////////////////
