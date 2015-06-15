#ifndef __PROJECTWIZARDDEFS_H_
#define __PROJECTWIZARDDEFS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#define FILE_EXT_PROJECT          _T("4cp")
#define FILE_EXT_CONFIGURATION    _T("con")
#define FILE_EXT_RESOURCE         _T("res")
#define FILE_EXT_GLOBAL_VARIABLES _T("gvl")
#define FILE_EXT_CONSTANTS        _T("cst")
#define FILE_EXT_TARGETS          _T("4cpc")
#define FILE_EXT_LIBRARY          _T("4cl")
#define FILE_EXT_LATCOMM          _T("4clc")

#define CONFIGURATION_FILE_NAME   _T("Configuration1")


typedef int TSubSystems;

enum
{
    SUBSYSTEM_NONE = 0x0,
    SUBSYSTEM_ANY = 0x1,
    SUBSYSTEM_PROFIBUS = 0x2,
    SUBSYSTEM_OPC = 0x4,
    SUBSYSTEM_SERIAL_COMMUNICATION = 0x8
};


enum TIecLanguage
{
    IEC_LANGUAGE_ST,
    IEC_LANGUAGE_FBD,    
    IEC_LANGUAGE_SFC,
    IEC_LANGUAGE_LD,
    IEC_LANGUAGE_IL
};

#endif // __PROJECTWIZARDDEFS_H_

