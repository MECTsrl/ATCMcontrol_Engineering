/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/StdAfx.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: StdAfx.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/StdAfx.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#if !defined(AFX_STDAFX_H__0087E3C1_28F2_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_STDAFX_H__0087E3C1_28F2_11D5_A05C_0050BF490551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Selten benutzte Teile der Windows-Header nicht einbinden

#include <afx.h>
#include <afxwin.h>         // Kern- und Standardkomponenten von MFC
#include <afxtempl.h>		// CArray
#include <afxcoll.h>        // CMap

#include <iostream>
#include <conio.h>

#include "vmSharedDef.h"

// for Debugging us ASSERT
#define _ASSERT_ 0

#define OPC_NCC_BASE 1
#include "intOpcds.h"
#undef  OPC_NCC_BASE

#include "blob.h"
#include "NccCode.h"
#include "Codec.h"
#include "formatexception.h"
#include "xxx_dbp.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt zusätzliche Deklarationen unmittelbar vor der vorherigen Zeile ein.


#endif

/* ---------------------------------------------------------------------------- */
