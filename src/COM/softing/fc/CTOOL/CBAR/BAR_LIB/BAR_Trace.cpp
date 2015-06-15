/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Trace.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Trace.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Trace.cpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#include <stdio.h>
#include <stdarg.h>

#include "BAR_Trace.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


char    BAR_Trace::m_bufferA[BUFSIZE] =  "empty buffer";
wchar_t BAR_Trace::m_bufferW[BUFSIZE] = L"empty buffer";

int  BAR_Trace::m_iTraceLevel = 0;

 ostream& BAR_Trace::m_out  =  cout;
wostream& BAR_Trace::m_wout = wcout;

//=============================================================================*
void BAR_Trace::AWrite(int iMsgLevel, const char* sMsg, ...)
//=============================================================================*
{
    if ( iMsgLevel > m_iTraceLevel )
        return;

    va_list args;
    va_start(args, sMsg);

    _vsnprintf(m_bufferA, BUFSIZE, sMsg, args);
    m_bufferA[BUFSIZE-1] = '\0';

    m_out << "#trace-" << iMsgLevel << ": "<< m_bufferA << endl << flush;

}

//=============================================================================*
void BAR_Trace::WWrite(int iMsgLevel, const wchar_t* wsMsg, ...)
//=============================================================================*
{
    if ( iMsgLevel > m_iTraceLevel )
        return;

    va_list args;
    va_start(args, wsMsg);

    _vsnwprintf(m_bufferW, BUFSIZE, wsMsg, args);
    m_bufferW[BUFSIZE-1] = '\0';

    m_wout << L"#trace-" << iMsgLevel << L": "<< m_bufferW << endl << flush;

}

//=============================================================================*
void BAR_Trace::SetTraceLevel(int iTraceLevel)
//=============================================================================*
{
    m_iTraceLevel = iTraceLevel;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Trace.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:37
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Trace.cpp $
