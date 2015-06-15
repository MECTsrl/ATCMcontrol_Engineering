/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/FileLogger.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FileLogger.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/FileLogger.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
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
 *  15.03.2000  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

// FileLogger.cpp: implementation of the CFileLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KADmergeDLL.h"
#include "FileLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileLogger::CFileLogger()
{

}

CFileLogger::CFileLogger(string sFilename, string sDescription)
{
    OpenLog(sFilename, sDescription);
}

CFileLogger::~CFileLogger()
{
    CloseLog();
}

void CFileLogger::CloseLog()
{
    if(aFile.is_open())
    {
        aFile << "End Log" << endl << endl;
        aFile.flush();
        aFile.close();
    }
}

bool CFileLogger::Log(string sString)
{
    if(aFile.is_open())
    {
        aFile << sString << endl;
        aFile.flush();
        return true;
    }
    else
    {
        return false;
    }
}

void CFileLogger::OpenLog(string sFilename, string sDescription)
{
    CloseLog();
    m_sFilename = sFilename;
    aFile.open(m_sFilename.c_str(), ios_base::app);

    time_t ltime;
    time(&ltime);

    aFile << sDescription << endl;
    aFile << "Start Log at : " << ctime(&ltime);
}

/*
 *----------------------------------------------------------------------------*
 *  $History: FileLogger.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 21.03.01   Time: 13:45
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 15.03.01   Time: 14:06
 * Created in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * New Project
 * 
 *----------------------------------------------------------------------------*
*/
