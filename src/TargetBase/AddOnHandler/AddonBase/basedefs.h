#ifndef _BASEDEFS_H_
#define _BASEDEFS_H_

// the dispids of the handler functions
#define DISP_NEW                    _T("New")
#define DISP_ASSIGNMENT             _T("Assignment")
#define DISP_RENAME                 _T("Rename")
#define DISP_DELETE                 _T("Delete")
#define DISP_PROPERTIES             _T("Properties")

#define DISP_SETTINGS               _T("Settings")

#define DISP_PROJINFO_UPLOAD        _T("ProjectInfoUpload")
#define DISP_PRE_DLSTEP             _T("PreDLStep")

// some defines for project settings stored in 4cp file
#define TARGET_SET_SECTION          _T("Compiler.")         // Compiler.xxx, where xxx is target extension
#define TARGET_SET_OPTION           _T("opt") 
#define TARGET_SET_TARGET_SECTION   _T("Target.")           // Target.yyy, where yyy is target type
#define TARGET_SET_CONN_KEY         _T("ConnectionTimeout")

#define TARGET_SET_BP_NONE          _T("bp0")
#define TARGET_SET_BP_SIMPLE        _T("bp1")
#define TARGET_SET_BP_ADVANCED      _T("bp2")
#define TARGET_SET_NCC_0            _T("ncc:0")
#define TARGET_SET_NCC_1            _T("ncc:1")

#define TARGET_XMLATTR_CONTROL      _T("CONTROL")
#define TARGET_XMLATTR_RESOURCE     _T("RESOURCE")

#define POU_PROTECTION_FILE         _T("Project.4gh")

// defines to mask a address
#define ADDR_START                  _T("{")
#define ADDR_END                    _T("}")

// object type for the resource assignment
#define TYPE_RESOURCE               _T("RESOURCE")

// maximum user name length
#define MAXUSERNAMELEN              20


#define DEFAULT_MAX_RESOURCE_COUNT  30
#define STD_IP_ADDRESS              _T("localhost")
#define STD_SERIAL_PAR              _T("COM1:19200,n,8,1")

#define FILE_NAME_CUSTOM_BIN    _T("__custdl.bin")

#define E_FACILITY_CEDOWNLOAD	0x283

#endif

/* ---------------------------------------------------------------------------- */
