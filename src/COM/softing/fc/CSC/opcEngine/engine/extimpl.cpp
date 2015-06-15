//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : EXTIMPL.CPP                                                 |
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : External implementations                                    |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  11/30/98  RT        changed to OPC V2.0 files                             |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <opcda.h>
#include <opccomn.h>
EXTERN_C const GUID CATID_OPCDAServer10;	// oppcda_cats.c
EXTERN_C const GUID CATID_OPCDAServer20;	// oppcda_cats.c

// ATL code
#include <atlimpl.cpp>
#include <statreg.cpp>

// OPC IIDs
#include "initguid.h"
#define __IID_DEFINED__
#define __GUID_DEFINED__
#define CLSID_DEFINED
#include "opcda_i.c"
#include "opccomn_i.c"
#include "opcda_cats.c"

