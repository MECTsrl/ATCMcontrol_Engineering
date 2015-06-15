
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ExeMsgThread.h"

//----  Static Initializations:   ----------------------------------*

#define BUFSIZE 1024
#define MIN_TIMEOUT 1
#define MAX_TIMEOUT 1024
#define DECREASE_LEVEL  0.5     // above this level timeout is decreased
#define INCREASE_LEVEL  0.05    // below this level timeout is increased

//------------------------------------------------------------------*
/**
 * on initialize.
 *
 *  this function is called from within the thread to do initialization
 *
 * @param           -
 * @return          thread_noerror: OK
 *                  thread_error: error occurred.
 * @exception       -
 * @see             -
*/
int CExeMsgThread::OnInitialize()
{
    HRESULT hr;
    m_lNotReadCount = 0;

    hr = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
    if(hr != S_OK)
    {
        return thread_error;
    }
    return thread_noerror;
}


//------------------------------------------------------------------*
/**
 * on exit.
 *
 *  this function is called from within the thread when it is to be 
 *  terminated.
 *
 * @param           -
 * @return          thread_noerror: OK
 *                  thread_eror: error occurred.
 * @exception       -
 * @see             -
*/
int CExeMsgThread::OnExit()
{
    CString str;
    CString strItem;
    int     iPos;
    char    chBuf[BUFSIZE+1];
    DWORD   dwRead;

    // read rest from stdout and write to message interface
    while(ReadFile(m_pData->m_hStdout, chBuf, BUFSIZE, &dwRead, NULL) && dwRead != 0)
    {
        chBuf[dwRead] = 0;
        str += chBuf;
        while((iPos = str.Find(_T("\n"))) != -1)
        {
            strItem = str.Left(iPos);
            strItem.TrimRight();

            CComBSTR    sMessage(strItem);
            m_pData->m_pIExeMsg->NewExeMessage(sMessage);
            strItem = str.Mid(iPos + 1);
            str = strItem;
        }
    }
    ::CloseHandle(m_pData->m_hStdout);
    ::CloseHandle(m_pData->m_hStdin);
    ::CoUninitialize();

    m_pData->m_pIExeMsg->Release();
    m_pData = NULL;
    return thread_noerror;
}


//------------------------------------------------------------------*
/**
 * on timeout.
 *
 *  this function is called from within the thread, when a timeout
 *  occurred.
 *  By changing the timout value the speed of the reading process
 *  can be adapted.
 *  If the buffer is filled less than INCREASE_LEVEL the timeout is increased
 *  by a factor of 2 -> reading gets slower.
 *  If the buffer is filled more than DECREASE_LEVEL the timeout is decreased
 *  by a factor of 2 -> reading gets faster.
 *
 * @param           -
 * @return          thread_noerror: OK
 * @exception       -
 * @see             -
*/
int CExeMsgThread::OnTimeOut()
{
    CString str;
    CString strItem;
    int     iPos;
    char    chBuf[BUFSIZE+1];
    DWORD   dwRead;
    float   fBufferFillLevel;

    // read from stdout and write to message interface
    if(ReadFile(m_pData->m_hStdout, chBuf, BUFSIZE, &dwRead, NULL) && dwRead != 0)
    {
        chBuf[dwRead] = 0;
        str += chBuf;
        while((iPos = str.Find(_T("\n"))) != -1)
        {
            strItem = str.Left(iPos);
            strItem.TrimRight();

            CComBSTR    sMessage(strItem);
            m_pData->m_pIExeMsg->NewExeMessage(sMessage);
            strItem = str.Mid(iPos + 1);
            str = strItem;
        }
        fBufferFillLevel = (float)dwRead / BUFSIZE;
        if(fBufferFillLevel > DECREASE_LEVEL)
        {
            if(m_dwMillisec > MIN_TIMEOUT)
            {
                m_dwMillisec >>= 1;
            }
        }
        if(fBufferFillLevel < INCREASE_LEVEL)
        {
            if(m_dwMillisec < MAX_TIMEOUT)
            {
                m_dwMillisec <<= 1;
            }
        }
    }

    return thread_noerror;
}