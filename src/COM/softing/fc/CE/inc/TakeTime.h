#ifndef __TAKETIME_H_
#define __TAKETIME_H_


/* usage:

#define TAKE_TIME_DEFINE
include "TakeTime.h"

  to exclude time measurements comment the line #define TAKE_TIME_DEFINE

  In a program block
TAKE_TIME_INIT;
    to initialize time measurements

TAKE_TIME_START;
    start/restarts time measurements


    take difference between actual time and last TAKE_TIME_START and show some message:
    the message will be appended the following parts:
    prefix: "TT>"
    postfix: " : <time> ms"
#ifdef TAKE_TIME_DEFINE
            CString strMsg;
            strMsg.Format(_T("Fire_UpdateClassTree, connection nr %i : "), nConnectionIndex);
            TAKE_TIME_END(strMsg);
#endif

        

*/




#ifndef DEBUG    
#undef TAKE_TIME_DEFINE
#endif

#ifdef TAKE_TIME_DEFINE

#define TAKE_TIME_INIT     DWORD tt_dwStart = GetTickCount()

#define TAKE_TIME_START     tt_dwStart = GetTickCount()
#define TAKE_TIME_END(MSG)  {\
                                DWORD tt_dwDiff = GetTickCount() - tt_dwStart;\
                                CString tt_strMsg;\
                                tt_strMsg.Format(_T("TT>> %s: %i ms\n"), MSG, tt_dwDiff);\
                                TRACE(tt_strMsg);\
                            }

#define TAKE_TIME_INIT_HR   LARGE_INTEGER liStartCount;\
                            LARGE_INTEGER liEndCount;\
                            LARGE_INTEGER liFrequency;\
                            ::QueryPerformanceFrequency(&liFrequency);

#define TAKE_TIME_START_HR  ::QueryPerformanceCounter(&liStartCount);

#define TAKE_TIME_END_HR(MSG)   {\
                                    ::QueryPerformanceCounter(&liEndCount);\
                                    double dfNewTime;\
                                    dfNewTime = 1000.0 * (liEndCount.QuadPart - liStartCount.QuadPart) / (double)liFrequency.QuadPart;\
                                    TRACE("TT>> %20s: %12.4f ms\n", MSG, dfNewTime);\
                                }

#else

    #define TAKE_TIME_INIT
    #define TAKE_TIME_START
    #define TAKE_TIME_END(MSG)

    #define TAKE_TIME_INIT_HR
    #define TAKE_TIME_START_HR
    #define TAKE_TIME_END_HR(MSG)

#endif



#endif


// TEST_CODE: benchmarking only
    //SYSTEMTIME sysTimeStart;
    //GetSystemTime(&sysTimeStart);

// TEST_CODE: benchmarking only
    /*SYSTEMTIME sysTimeEnd;
    GetSystemTime(&sysTimeEnd);

    FILETIME fTimeStart;
    FILETIME fTimeEnd;

    SystemTimeToFileTime(&sysTimeStart, &fTimeStart);
    SystemTimeToFileTime(&sysTimeEnd, &fTimeEnd);

    ULARGE_INTEGER ulStart;
    ULARGE_INTEGER ulEnd;

    ulStart.HighPart = fTimeStart.dwHighDateTime;
    ulStart.LowPart = fTimeStart.dwLowDateTime;
    ulEnd.HighPart = fTimeEnd.dwHighDateTime;
    ulEnd.LowPart = fTimeEnd.dwLowDateTime;

    ULARGE_INTEGER ulDiff;
    ulDiff.QuadPart= ulEnd.QuadPart - ulStart.QuadPart;
    CString strMsg;
    strMsg.Format(_T("UpdateTime PouSelection: %I64i\n"), ulDiff.QuadPart);

    TRACE(strMsg);
    */