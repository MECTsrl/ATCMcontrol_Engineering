#ifndef __DEFS_H_
#define __DEFS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "ParseGSD.h"   // interfaces
#include "pgsd.h"       // coclasses definition
#include "comcoll.h"    // collection templates

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
/**
* @def  SUPPORTED_GSD_REVISION - defines maximum guaranteed revision of GSD 
* supported by the GSD parser.
*/
#define SUPPORTED_GSD_REVISION  (3)

/**
* @def  GSD_PARSE_MSG_BUFSIZE - defines maximum buffer size for messages 
* issued by the GSD parser during parse process
*/
#define GSD_PARSE_MSG_BUFSIZE   (255)
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
typedef CMap<BAUDRATE, BAUDRATE, long, long&> mapBaudrateToValue;
typedef CMFCObjStore<IParseMessage> ParseMessageList;

typedef CComInterfaceColl<ISlotCollection,&IID_ISlotCollection,ISlot,&LIBID_PARSEGSDLib,2,0> SlotCollection;
typedef CComInterfaceColl<IModuleCollection,&IID_IModuleCollection,IModule,&LIBID_PARSEGSDLib,2,0> ModuleCollection;
typedef CComInterfaceColl<IPhysicalInterfaceCollection,&IID_IPhysicalInterfaceCollection,IPhysicalInterface,&LIBID_PARSEGSDLib,2,0> PhysicalInterfaceCollection;
typedef CComInterfaceColl<IParseMessageCollection,&IID_IParseMessageCollection,IParseMessage,&LIBID_PARSEGSDLib,2,0> ParseMessageCollection;
typedef CComInterfaceColl<IExtUserParamCollection,&IID_IExtUserParamCollection,IExtUserParam,&LIBID_PARSEGSDLib,2,0> ExtUserParamCollection;
typedef CComInterfaceColl<IDiagnosticInfoCollection,&IID_IDiagnosticInfoCollection,IDiagnosticInfo,&LIBID_PARSEGSDLib,2,0> DiagCollection;

HRESULT AllocSysString(BSTR* ps,CString & str);
LANGID  GSD_GetInstallLanguage();
bool    GSD_FormatMsgArg(HINSTANCE hResHandle,LPTSTR pszOut, size_t sizeBuf, HRESULT hr,va_list pArg);
bool    GSD_FormatMessage(CString & str, HRESULT hr, ...);

class CStationCallback
{
public:
    virtual IModule * LookupModule(long lModuleReference) = 0;
    virtual void AddMessage(IParseMessage *) = 0;
    virtual void SetErrorCount(long) = 0;
    virtual void Message(HRESULT, LPCTSTR) = 0;
};


#endif // __DEFS_H_