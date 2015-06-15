//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : GLOBALS.H                                                   |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : Declaration of global variables                             |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// resource id of registar script
extern UINT g_registerId;

// COM class id of OPC-Server object
extern CLSID g_clsid;

// OPC data formats
extern const UINT OPCSTMFORMATDATA;
extern const UINT OPCSTMFORMATDATATIME;
extern const UINT OPCSTMFORMATWRITECOMPLETE;

#endif
