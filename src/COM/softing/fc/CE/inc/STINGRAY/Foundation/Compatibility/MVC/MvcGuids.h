/////////////////////////////////////////////////////////////////////////////
//@doc
//@module MvcGuids.h | This module contains GUID declarations for
// the Stingray MVC library. If initguid.h is included before
// this file, the GUIDs are defined.
// 
// <nl>Copyright <cp> 1998 Stingray Software, Inc. All rights reserved.
// 
// This source code is only intended as a supplement to the Objective 
// Views User's Guide and related electronic documentation provided with 
// the library. See these sources for detailed information regarding the 
// Objective Views product.
// 
// Author: Jeff Boenig
// <nl>Created: 3/99
// 
/////////////////////////////////////////////////////////////////////////////

#include <objbase.h>

#include <Foundation\Compatibility\Common\CmnGuids.h>

const GUID IID_IMvcSubject = IID_ISubject;
const GUID IID_IMvcObserver = IID_IObserver;
const GUID IID_IMvcMessage = IID_IMessage;


// {F0526984-E944-11d2-B412-006008AFE059}
DEFINE_GUID(IID_IMvcModel, 
0xf0526984, 0xe944, 0x11d2, 0xb4, 0x12, 0x0, 0x60, 0x8, 0xaf, 0xe0, 0x59);
