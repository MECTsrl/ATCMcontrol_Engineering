/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/stdafx.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: stdafx.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy/stdafx.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CDEPLOY
 *
 * =COMPONENT           Deploy
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  09.07.2002  JD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


#if !defined(AFX_STDAFX_H__CD315A7C_A62C_4E0F_8C8E_7C07BFC14D09__INCLUDED_)
#define AFX_STDAFX_H__CD315A7C_A62C_4E0F_8C8E_7C07BFC14D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>


#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;


#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\IdPathHelp.h"
#include "XMLHelper\XMLNodeIter.h"

#include "CESysDef.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CD315A7C_A62C_4E0F_8C8E_7C07BFC14D09__INCLUDED_)




/*
 *----------------------------------------------------------------------------*
 * $History: stdafx.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 17.09.02   Time: 11:08
 * Created in $/4Control/COM/softing/fc/CTOOL/Deploy
 * Moved from Bruneck Target to product.
 * 
 *==
 *----------------------------------------------------------------------------*
*/
