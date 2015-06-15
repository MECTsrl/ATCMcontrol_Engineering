
#ifndef __DEFINES_H_
#define __DEFINES_H_

#define TARGET_EXT _T("4cpc")  //the target type name of this back end





#define PC_GEN_CLASSES_DIR     _T(".4cpc\\.classes")
#define PC_GEN_DBI_DIR         _T(".4cpc\\.dbi")
#define PC_GEN_LINK_DIR        _T(".4cpc\\.link")

#define PC_TRG_LOADER_LIST     _T("loader.list")
#define PC_RETAIN_ITEM_FILE    _T("__retain.list")
#define PC_TARGET_INIT_FILE    _T("__.init")
#define PC_TASK_FILE_PRFIX     _T("__")

#define PC_INIT_EXT            _T("init")
#define PC_INIT_JIF            _T("jif")


const int  PC_MIN_TASK_PRIORITY = 0;//as defined in iec11-3 2.7.2 
                                    //this is the highest IEC Prio
const int  PC_MAX_TASK_PRIORITY = 9;//this is the lowest IEC Prio 
const int  PC_MIN_TASK_INTERVAL_NT_MS = 20; //smallest task interval for resource typ NT
const int  PC_MIN_TASK_INTERVAL_MS = 5;
const int  PC_MAX_TASK_INTERVAL_MS = 3600*1000*24;
const long PC_MAX_RETAIN_ITEMS     = 1000;
const long PC_MAX_VARIABLE_OBJECTS = 1000*100;

//sys comments for 4CPC java code:
#define PC_ATTR_DECLID      _T("declid")
#define PC_ATTR_JAVACLASS   _T("javaclass")


#define PACKAGE_BASE_PATH _T("COM.softing.fc.CC")


//code of Libs can be reused if Major and minor version is same
//a patch or service pack is not treated as incompatible
//In general the binary code version string is hard coded here
//and not related to the build index.
#define BIN_CODE_VERSION _T("2.00")



                             



#endif // __DEFINES_H_

