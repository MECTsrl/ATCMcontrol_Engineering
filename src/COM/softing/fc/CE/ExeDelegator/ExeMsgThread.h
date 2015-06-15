#ifndef __EXEMSGTHREAD_H_
#define __EXEMSGTHREAD_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CEExecute.h"
#include "ThrdTmpl.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CExeMsgThreadData
{
public:
    CExeMsgThreadData()
    {
        m_hStdout = NULL;
        m_hStdin = NULL;
    }

public:
    HANDLE                      m_hStdout;
    HANDLE                      m_hStdin;

    IExeMsgInternal*            m_pIExeMsg;
};


//------------------------------------------------------------------*
/**
 *  class CExeMsgThread
 *
 *  - used by CExeThread to redirect messages from the process to 
 *    the message view.
 *  - reads messages from stdout and calls IExeMsgInternal::NewExeMessage()
 *  - derived from CSigThread<>
 *  - adapts speed of reading automatically not to waste time
 *
 *  @see    CSigThread
 */
class CExeMsgThread : public CSigThread<CExeMsgThreadData>
{
public:

    virtual int OnInitialize();
    virtual int OnExit();
    virtual int OnTimeOut();

protected:
    long    m_lNotReadCount;    // counts how often OnTimeOut was called
                                // without reading a line
};
#endif // __EXEMSGTHREAD_H_