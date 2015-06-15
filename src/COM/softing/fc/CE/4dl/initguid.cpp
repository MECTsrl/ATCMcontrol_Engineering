#define FC_LIB_USE_TODEBUG
#include "fc_tools\fc_libmem.h"

#include "CEMsg.h"
#include "CSC_Online.h"
#include "cedl.h"
#include "CSC_OnlineServer.h"
#include "OPCDA.h"


#include "CEMsg_i.c"
#include "CSC_Online_i.c"
#include "cedl_i.c"
#include "CSC_OnlineServer_i.c"
#include "OPCDA_i.c"

#include <atlbase.h>
CComModule _Module;
#include <atlcom.h>


BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


void InitModule()
{
    _Module.Init(ObjectMap, ::GetModuleHandle(NULL));
}

