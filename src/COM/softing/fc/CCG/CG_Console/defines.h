
#ifndef __DEFINES_H_
#define __DEFINES_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//code of Libs can be reused if Major and minor version is same
//a patch or service pack is not treated as incompatible
//In general the binary code version string is hard coded here
//and not related to the build index.
#define VIS_VERSION			 _T("2.0")

#define VIS_CHILD_OBJ_FILE	 _T("childs.txt")
#define VIS_TASK_FILE		 _T("tasks.txt")
#define VIS_LOADER_LIST_FILE _T("loader.list")
#define VIS_RESOURCE_FILE	 _T("resources.txt")
#define VIS_OBJECTS_FILE     _T("objects.txt")
#define TASK_CONFIG_FILE	 _T("cp_conf")

#define TASK_CONFIG_EXT		 _T(".txt")
#define VIS_DIR_EXT			 _T("__vis")

#define CONSOLE_CONFIG_DIR	 _T("config")  // exported (web) directory, not localy
#define FXR_DIR				 _T("__4xr")   // xref directory
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#endif // __DEFINES_H_


