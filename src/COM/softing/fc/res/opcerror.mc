;/*++
;
;Module Name:
;
;    OpcError.h
;
;Abstract:
;
;    This file is generated by the MC tool from the OpcError.MC message
;    file.
;
;Author:
;
;    OPC Task Force
;
;Revision History:
;Release 1.0A
;   Removed Unused messages (OPC_E_DUPLICATE, OPC_E_UNKNOWNLCID)
;   Added OPC_S_INUSE, OPC_E_INVALIDCONFIGFILE, OPC_E_NOTFOUND
;
;--*/
;
;#ifndef __OPCERROR_H
;#define __OPCERROR_H
;

MessageIdTypedef=HRESULT
SeverityNames=(
               Success=0x0
               Informational=0x1
               Warning=0x2
               Error=0x3
              )
FacilityNames=(
               System=0x0FF
               Interface=0x004
              )
LanguageNames=(
              English=0x409:OPCErrorMsgENU
              German=0x407:OPCErrorMsgDEU
             )
OutputBase=16

;// First the OPC Error class messages

MessageID=1
Severity=Error
Facility=Interface
SymbolicName=OPC_E_INVALIDHANDLE
Language=English
An invalid handle was passed
.
Language=German
Eine ung�ltige OPC-Zugriffsnummer wurde �bergeben
.


;
;/*------------------------------------- Obsoleted OPC messages 
;// MessageID=
;// SymbolicName=OPC_E_DUPLICATE
;// Language=English
;// A duplicate parameter was passed where one is not allowed
;// .
;// 
;// MessageID=
;// SymbolicName=OPC_E_UNKNOWNLCID
;// Language=English
;// The server does not support the specified locale id
;// .
;--------------------------------------- End of Obsoleted OPC messages 
;*/
;

MessageID=4
SymbolicName=OPC_E_BADTYPE
Language=English
The server cannot convert between the passed or requested data type and the canonical type
.
Language=German
OPC_E_BADTYPE !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_PUBLIC
Language=English
The requested operation cannot be done on a public group
.
Language=German
OPC_E_PUBLIC !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_BADRIGHTS
Language=English
The item's AccessRights do not allow the operation
.
Language=German
OPC_E_BADRIGHTS !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_UNKNOWNITEMID
Language=English
The item definition does not exist within the servers address space
.
Language=German
OPC_E_UNKNOWNITEMID !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_INVALIDITEMID
Language=English
The item definition does not comform to the server's syntax
.
Language=German
OPC_E_INVALIDITEMID !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_INVALIDFILTER
Language=English
The filter string is not valid
.
Language=German
OPC_E_INVALIDFILTER !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_UNKNOWNPATH
Language=English
The item's access path is not known to the server
.
Language=German
!! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_RANGE
Language=English
The value passed to WRITE was out of range
.
Language=German
OPC_E_RANGE !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_DUPLICATE_NAME
Language=English
A group with a duplicate name already exists in the server
.
Language=German
OPC_E_DUPLICATE_NAME !! �bersetzung fehlt !!
.

MessageID=
Severity=Success
SymbolicName=OPC_S_UNSUPPORTEDRATE
Language=English
The server does not support the requested rate, but will use the closest available
.
Language=German
OPC_S_UNSUPPORTEDRATE !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_S_CLAMP
Language=English
A value passed to WRITE was accepted, but was clamped
.
Language=German
OPC_S_CLAMP !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_S_INUSE
Language=English
The operation cannot be completed because the 
object still has references that exist
.
Language=German
OPC_S_INUSE !! �bersetzung fehlt !!
.

MessageID=
Severity=Error
SymbolicName=OPC_E_INVALIDCONFIGFILE
Language=English
The server's configuration file is an invalid format
.
Language=German
OPC_E_INVALIDCONFIGFILE !! �bersetzung fehlt !!
.

MessageID=
SymbolicName=OPC_E_NOTFOUND
Language=English
The server could not locate the requested object
.
Language=German
OPC_E_NOTFOUND !! �bersetzung fehlt !!
.

;#endif // OpcError