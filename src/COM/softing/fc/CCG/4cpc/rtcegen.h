
#ifndef __RTCEGEN_H_
#define __RTCEGEN_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "parse4ch.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define RTC_FILE_EXT _T("rtc")

#define ST_RTC_SUPPORTED_TYPES     (CG_TBS_BOOL|CG_TBS_BYTE|CG_TBS_WORD|CG_TBS_DWORD|CG_TBS_DINT|CG_TBS_TIME|CG_TBS_INSTANCE)
#define ST_RTC_SUPPORTED_TYPES_GVL (CG_TBS_BOOL|CG_TBS_BYTE|CG_TBS_WORD|CG_TBS_DWORD|CG_TBS_DINT)

#define ST_MAX_RTC_ADDR_LEN  256
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//compile the RTCE programs of the resource:
bool CG_GenRTCECodeForTarget(CGT_CDiagMsg* pdm, ICG_IECFront* pIF, CTarget* pTrg);




#endif // __RTCEGEN_H_


