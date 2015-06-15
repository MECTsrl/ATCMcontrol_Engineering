
#if !defined __GLOBAL__
#define __GLOBAL__

#pragma warning(disable: 4786)

#if defined _BACKPARSERDLL
#include <fstream>
//extern ofstream DebugOutput;
#endif

#include <string>
using namespace std;

#if defined _BACKPARSERDLL
#define BACKPARSERDLLEXPORT   __declspec( dllexport )
#else
#define BACKPARSERDLLEXPORT
#endif

#define _GVL_FILE "GLOBAL_VAR_FILE"
#define _CST_FILE "CONSTANT_FILE"
#define _STRUCT "STRUCT"
#define _STRUCT_VAR "STRUCT_VAR"
#define _FUNCTION_BLOCK "FUNCTION_BLOCK"
#define _FUNCTION "FUNCTION"
#define _PROGRAM "PROGRAM"
#define _CONFIGURATION "CONFIGURATION"
#define _RESOURCE "RESOURCE"
#define _ERROR_NODE "ERROR_NODE"

#define _END_CONST "END_CONST"
#define _END_STRUCT "END_STRUCT"
#define _END_VAR "END_VAR"

#define _EMPTYFILE "EMPTY_FILE"

#define _EXTENSION_CST "cst"
#define _EXTENSION_GVL "gvl"

//------------------------------------------------------------------*
/**
 *  typedef struct BackparserProjectInfo
 *
 *  @see    ....
*/
typedef struct
{
    string m_strProjectName;
    string m_strProjectPath;
} BackparserProjectInfo;

#endif



