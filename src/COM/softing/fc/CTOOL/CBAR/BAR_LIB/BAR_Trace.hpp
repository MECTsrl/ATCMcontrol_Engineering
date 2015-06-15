/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Trace.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Trace.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Trace.hpp $
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

#ifndef _BAR_TRACE_H_
#define _BAR_TRACE_H_

//====  Aggregate Includes:   =================================================*
#include <iostream>
using namespace ::std;

//====  Forward Class Definitions:   ==========================================*

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*
#define BAR_TRACE(lvl, msg)                     BAR_Trace::AWrite(lvl, msg);
#define BAR_TRACE1(lvl, msg, x1)                 BAR_Trace::AWrite(lvl, msg, x1);
#define BAR_TRACE2(lvl, msg, x1, x2)             BAR_Trace::AWrite(lvl, msg, x1, x2);
#define BAR_TRACE3(lvl, msg, x1, x2, x3)         BAR_Trace::AWrite(lvl, msg, x1, x2, x3);
#define BAR_TRACE4(lvl, msg, x1, x2, x3, x4)     BAR_Trace::AWrite(lvl, msg, x1, x2, x3, x4);
#define BAR_TRACE5(lvl, msg, x1, x2, x3, x4, x5) BAR_Trace::AWrite(lvl, msg, x1, x2, x3, x4, x5);

#define BAR_WTRACE(lvl, msg)                      BAR_Trace::WWrite(lvl, msg);
#define BAR_WTRACE1(lvl, msg, x1)                 BAR_Trace::WWrite(lvl, msg, x1);
#define BAR_WTRACE2(lvl, msg, x1, x2)             BAR_Trace::WWrite(lvl, msg, x1, x2);
#define BAR_WTRACE3(lvl, msg, x1, x2, x3)         BAR_Trace::WWrite(lvl, msg, x1, x2, x3);
#define BAR_WTRACE4(lvl, msg, x1, x2, x3, x4)     BAR_Trace::WWrite(lvl, msg, x1, x2, x3, x4);
#define BAR_WTRACE5(lvl, msg, x1, x2, x3, x4, x5) BAR_Trace::WWrite(lvl, msg, x1, x2, x3, x4, x5);


class BAR_Trace
{
public:

    static void AWrite(int ILevel, const    char* sMsg, ...); //=== ASCII
    static void WWrite(int ILevel, const wchar_t* wsMsg, ...); //=== Wide chars

    static void SetTraceLevel(int traceLevel);

private:
    enum { BUFSIZE=8192 };
    static int     m_iTraceLevel;
    static char    m_bufferA[BUFSIZE];
    static wchar_t m_bufferW[BUFSIZE];

    static  ostream& m_out;
    static wostream& m_wout;
};

//=============================================================================*
#endif // _BAR_TRACE_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Trace.hpp $
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
// $Workfile: BAR_Trace.hpp $
