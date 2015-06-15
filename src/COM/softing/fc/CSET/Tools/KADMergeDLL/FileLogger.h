/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/FileLogger.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FileLogger.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/FileLogger.h $
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

// FileLogger.h: interface for the CFileLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELOGGER_H__164F5D46_178C_11D5_B912_002018641833__INCLUDED_)
#define AFX_FILELOGGER_H__164F5D46_178C_11D5_B912_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include <string>
using namespace std;

class CFileLogger  
{
public:
	CFileLogger();
	CFileLogger(string sFilename, string sDescription);
	virtual ~CFileLogger();

    void CloseLog();
    void OpenLog(string sFilename, string sDescription);
    bool Log(string sString);

private:
    string m_sFilename;
    ofstream aFile;
};

#endif // !defined(AFX_FILELOGGER_H__164F5D46_178C_11D5_B912_002018641833__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 *  $History: FileLogger.h $
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
 * User: Kt           Date: 15.03.01   Time: 14:07
 * Created in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * New Project
 * 
 *----------------------------------------------------------------------------*
*/
