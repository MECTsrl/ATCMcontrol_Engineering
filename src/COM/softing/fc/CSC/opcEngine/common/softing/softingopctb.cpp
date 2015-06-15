#include "stdafx.h"

#ifdef SOFTING
#include "GenObj.h"
#include "Softing/SoftingOPCTB.h"

DWORD g_toolkitMagic = 0;

void setOPCToolkitMagic(DWORD magic)	
{										
	g_toolkitMagic = magic;				
}

#endif