/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CEFile.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CEFile.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary/CEFile.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "StdAfx.h"
#include "CEFile.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEFile::CEFile()
{

}

CEFile::~CEFile()
{

}

/////////////////////////////////////////////////////////////////////
/**
 * get the path from full file path
 *
 * @param fullFilePath		the full file path of the file
 * @return					the path of the file only
 * @exception  
 * @see        
 */
LPCTSTR CEFile::GetFilePath(LPCTSTR pfullFilePath)
{
	static CString path = "";
	CString fullFilePath = pfullFilePath;
	for (int ii=fullFilePath.GetLength()-1; ii>=0; ii--)
	{
		if (fullFilePath[ii] == '\\' || fullFilePath[ii] == '/' || fullFilePath[ii]== ':')
		{
			path = fullFilePath.Left(ii);
			break;
		}
	}
	return path;
}

/////////////////////////////////////////////////////////////////////
/**
 * get the file name from full file path
 *
 * @param fullFilePath		the full file path of the file
 * @return					the file name of the file only
 * @exception  
 * @see        
 */
LPCTSTR CEFile::GetFileName(LPCTSTR pfullFilePath)
{
	static CString fileName = "";
	fileName = pfullFilePath;
	CString fullFilePath = pfullFilePath;
	for (int ii=fullFilePath.GetLength()-1; ii>=0; ii--)
	{
		if (fullFilePath[ii] == '\\' || fullFilePath[ii] == '/' || fullFilePath[ii]== ':')
		{
			fileName = fullFilePath.Right(fullFilePath.GetLength()-ii-1);
			break;
		}
	}
	return fileName;
}

/////////////////////////////////////////////////////////////////////
/**
 * get the file name from full file path
 *
 * @param fullFileName		the full file name (with extension)
 * @return					the file extension
 * @exception  
 * @see        
 */
LPCTSTR CEFile::GetFileExt(LPCTSTR pfullFileName)
{
	static CString fileExt;
	CString fullFileName = pfullFileName;
	for (int ii=fullFileName.GetLength()-1; ii>=0; ii--)
	{
		if (fullFileName[ii] == '.')
		{
			fileExt = fullFileName.Right(fullFileName.GetLength()-ii-1);
			break;
		}
		fileExt="";
	}
	return fileExt;
}

/////////////////////////////////////////////////////////////////////
/**
 * test if given directory exists
 *
 * @param directory		the full directory name
 * @return				TRUE directory exists, else FALSE 
 * @exception  
 * @see        
 */
BOOL CEFile::IsDirectory(LPCTSTR directory)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(directory);
	if (bWorking)
	{
		bWorking = finder.FindNextFile();
		return finder.IsDirectory();
	}
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////
/**
 * test if given file exists
 *
 * @param fileName		the file name
 * @return				TRUE if file exists, else FALSE 
 * @exception  
 * @see        
 */
BOOL CEFile::IsFile(LPCTSTR fileName)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(fileName);
	if (bWorking)
	{
		bWorking = finder.FindNextFile();
		return (!finder.IsDirectory());
	}
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////
/**
 * test if given file has read-only flag
 *
 * @param fileName		the file name
 * @return				TRUE if file exists and has read only flag,
 *						else FALSE 
 * @exception  
 * @see        
 */
BOOL CEFile::IsReadOnly(LPCTSTR fileName)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(fileName);
	if (bWorking)
	{
		bWorking = finder.FindNextFile();
		return (finder.IsReadOnly());
	}
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////
/**
 * get the modification date of the file
 *
 * @param fileName		the file name
 * @param timeStamp		the timestamp
 * @return				TRUE if file exists,
 *						else FALSE 
 * @exception  
 * @see        
 */
BOOL CEFile::GetTimeStamp(LPCTSTR fileName, time_t* pTimeStamp)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(fileName);
	if (bWorking)
	{
		CTime refTime;
		bWorking = finder.FindNextFile();
		finder.GetLastWriteTime( refTime ) ;
		*pTimeStamp = refTime.GetTime();
		return TRUE;
	}
	else
		return FALSE;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: CEFile.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian/CELibrary
 *==
 *----------------------------------------------------------------------------*
*/
