
#if !defined(AFX_BACNETSIMPLE2_H__3BBBCD98_6263_4A51_8C7A_39A257B29236__INCLUDED_)
#define AFX_BACNETSIMPLE2_H__3BBBCD98_6263_4A51_8C7A_39A257B29236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

/* BACnet Devices
 */
#if 1
#define DEVICE_LOCAL		4711
#define DEVICE_REMOTE		2709
#else
#define DEVICE_LOCAL		2709
#define DEVICE_REMOTE		4711
#endif

#define NO_LOC				1
#define NO_REM              1

/* Number of objects to be created
 */
#define NO_LOC_NC			NO_LOC

#define NO_LOC_AO			NO_LOC
#define NO_LOC_AI			NO_LOC
#define NO_LOC_AV			NO_LOC

#define NO_LOC_BO			NO_LOC
#define NO_LOC_BI			NO_LOC
#define NO_LOC_BV			NO_LOC

#define NO_LOC_MO			NO_LOC
#define NO_LOC_MI			NO_LOC
#define NO_LOC_MV			NO_LOC

#define NO_LOC_TL			NO_LOC
#define NO_LOC_SC			NO_LOC

#define NO_REM_NC			NO_REM

#define NO_REM_AO			NO_REM
#define NO_REM_AI			NO_REM
#define NO_REM_AV			NO_REM

#define NO_REM_BO			NO_REM
#define NO_REM_BI			NO_REM
#define NO_REM_BV			NO_REM

#define NO_REM_MO			NO_REM
#define NO_REM_MI			NO_REM
#define NO_REM_MV			NO_REM

#define NO_REM_TL			NO_REM
#define NO_REM_SC			NO_REM


#endif // !defined(AFX_BACNETSIMPLE2_H__3BBBCD98_6263_4A51_8C7A_39A257B29236__INCLUDED_)
