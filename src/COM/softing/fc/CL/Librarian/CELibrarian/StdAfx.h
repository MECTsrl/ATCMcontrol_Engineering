// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

#if !defined(AFX_STDAFX_H__D56600BB_D42E_11D1_B273_006008736562__INCLUDED_)
#define AFX_STDAFX_H__D56600BB_D42E_11D1_B273_006008736562__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxole.h>         // MFC OLE-Klassen
#include <afxdocob.h>
#include <afxodlgs.h>       // MFC OLE-Dialogfeldklassen
#include <afxdisp.h>        // MFC OLE-Automatisierungsklassen
#include <afxtempl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxadv.h>   // CSharedFile
#include <afxtempl.h>

#include "atlbase.h"
extern CComModule _Module;
#include "atlcom.h"
#include "fc_todebug\fc_assert.h"

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;



#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\IdPathHelp.h"
#include "XMLHelper\XMLNodeIter.h"

#include "CESysDef.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STDAFX_H__D56600BB_D42E_11D1_B273_006008736562__INCLUDED_)
