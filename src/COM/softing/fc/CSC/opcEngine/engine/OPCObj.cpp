//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCOBJ.CPP                                                  |
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//                                                                            |
//  Description : IUnknown implementation of COM engine objects               |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "opcobj.h"

void lastCOMObjectActions(void)
{
	// check if released last object
	GenericPointer<OPCEngine> engine = ::getOPCEngine();
	if (engine->canUnloadNow() == S_OK)
	{
		GenericPointer<GenericEventHandler> ev = engine->getEventHandler();
		ev->onReleasedLastOPCObject();
	}
}
