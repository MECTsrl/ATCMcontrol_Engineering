The fc_tools.lib library basics
-------------------------------

Goals: 
- Supply a framework for always needed basic utillities.
  Especially for those components that cannot or want not
  use MFC.
- The FC_ should be read as Foundation Code and not as 4 Control,
  therefore fc_tools.lib must not contain any 4CONTOL speciffic 
  stuff (use fcp_utils.lib for this fcp_= 4 Control Product)
  This lib shall re resusable for any development not only
  ATCMControl, that is also the reason way the header files are 
  under ..\inc\fc_tools and not directly ..\inc.
- Memory allocation and assertions are under control of the 
  library user(=calling DLL or EXE). The user may or may
  not use fc_todebug.dll or use standard C-runtime or
  also MFC new/delete.
- UNICODE support via <tchar.h>. All functions use TCHAR*. 
- fc_tools.lib does not throw any exception, if out of memory 
  situation must be handeled you can use your own way by 
  writing your own memory allocation functions.
- fc_tolls.lib can be used together with e.g. MFC framework.
- It is a static link library
- Only Win32 core libs and dlls are attracted by fc_tools.lab, 
  no MFC and no other non system defined fc_xxx.lib. 
  fc_tools.lib is low level.   
- Only the code of called functions will be linked into 
  the calling EXE or DLL (the fc_tools.lib has many objects)
- Inline is not overused, but it's the way how C++ classes work
- The actual functions from the fc_tolls.lib are extern "C"
  C++ is used only for easy to use access to C-routines.

   

The following functional groups are currently defined (please see the 
header files for detailed information):
    fc_define.h       : basic defines like FC_ARRAY_LEN and FC_TODECL
    fc_string.h       : string helper routines not found in c runtime
                         e.g. getFileExtension and the like. 
    fc_registry.h     : Windows registry helper functions
    fc_fileIO.h       : file related helper functions, e.g. creating 
                        directory test if file exists, load a file to mem,
                        parse an ini-file, parse an csv-file, ...
    fc_cstring.h      : string buffer class (needs fc_libmem.h)
    fc_garray.h       : Growing array template class for simple lists (needs fc_libmem.h)
    fc_hash.h         : case sensitive and insensitive hash "void* by string".
                        Several C++ class wrappers using also templates 
                        provide an easy to use hash support (needs fc_libmem.h).
    fc_system.h       : OS-level helper routines. e.g. a UNC aware version of 
                        ANSI 'system' function (FC_System).


If you use one of these headers you must link with fc_tools.lib. 
fc_cstring.h and fc_garray.h needs memory allocation, linking
to fc_garray.obj or fc_cstring.obj will produce a linker error like:
 fc_tools.lib(fc_cstring.obj) : error LNK2001: unresolved external symbol _FC_libassert@12
 fc_tools.lib(fc_cstring.obj) : error LNK2001: unresolved external symbol _FC_LibNewMemory@8
 fc_tools.lib(fc_cstring.obj) : error LNK2001: unresolved external symbol _FC_LibFreeMemory@4
 fc_tools.lib(fc_cstring.obj) : error LNK2001: unresolved external symbol _FC_LibResizeMemory@8


All memory allocation/realloc/free and assertions are done via functions FC_Lib*
that are not defined in the fc_tools.lib itself. To define the FC_Lib* routines
do the following: In one of your  .c or .cpp file of a calling DLL or EXE include
the "fc_tools\fc_libmem.h" header, this header contains the prototypes and two 
optional default implementations:

//Use the fc_tools.lib with the fc_todebug.dll:
#define FC_LIB_USE_TODEBUG
#include "fc_tools\fc_libmem.h"

or 

//Use the fc_tools.lib with the standard runtime:
#define FC_LIB_USE_CRT
#include "fc_tools\fc_libmem.h"

or

//Use only the header and write your own version of the mem alloc and assert routines:
#include "fc_tools\fc_libmem.h"







