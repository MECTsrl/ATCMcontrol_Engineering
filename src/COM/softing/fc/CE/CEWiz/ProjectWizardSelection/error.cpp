
#include "stdafx.h"
#include "error.h"
#include "resource.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



TErrorInfo errorInfo[] =
{
    { IDS_ERR_FAILED_TO_EXECUTE_WIZARD }
};


ERROR_MESSAGE_FUNC_IMPL

