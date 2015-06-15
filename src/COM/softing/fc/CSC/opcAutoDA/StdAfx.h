//**************************************************************************
//
//       Copyright (c) FactorySoft, Inc.  1997, All Rights Reserved
//
//**************************************************************************
//
//  Filename   :  stdafx.h
//  Author     :  Jim Hansen
//
//  Subsystem  :  OPC Automation Interface
//
//  Description:  Precompiled header
//
//  Revisions  :
//**************************************************************************

#if !defined(AFX_STDAFX_H__09D080DA_F1B3_11D0_8D0A_000000000000__INCLUDED_)
#define AFX_STDAFX_H__09D080DA_F1B3_11D0_8D0A_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT
#pragma warning( disable : 4786 )  // "identifier truncated" warning

#define _WIN32_WINNT 0x0400
//#define _ATL_DEBUG_QI
//#define _ATL_STATIC_REGISTRY

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#endif

#include <List>
using namespace std; // for STL
#include "OPCDA.h"
#include "OPCComn.h"
#include "OPCAutoSofting.h"
#include "resource.h"
#include "OPCAutoServer.h"

// Functions to convert variants to Automation compatible types
HRESULT VariantToAutomation( VARIANT* );
HRESULT ArrayToAutomation( VARIANT*, VARTYPE );

extern LPWSTR empty;
extern CComBSTR emptyString;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__09D080DA_F1B3_11D0_8D0A_000000000000__INCLUDED)
