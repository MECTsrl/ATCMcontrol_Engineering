/* $Header: /4CReleased/V2.20.00/TargetBase/TOOLS/BACnetSimple/StdAfx.h 1     28.02.07 19:07 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: StdAfx.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/TOOLS/BACnetSimple/StdAfx.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Tools
 *
 * =COMPONENT			SPKonvert
 *
 * =CURRENT 	 $Date: 28.02.07 19:07 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _STDAFX_H_
#define _STDAFX_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

#include <iostream>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "BACnetSimple.h"

#include "common_enum.h"
#include "vmSharedDef.h"
#include "bacShared.h"

#endif // _STDAFX_H_

/* ---------------------------------------------------------------------------- */
