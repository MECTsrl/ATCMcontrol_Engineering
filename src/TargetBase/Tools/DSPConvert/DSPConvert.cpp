/*
 * PROJECT
 *
 * 	softCONTROL - Tools
 *  
 * COMPONENT / MODULE
 *
 * 	Module: DSPConvert
 *
 * $Archive: /4CReleased/V2.20.00/TargetBase/TOOLS/DSPConvert/DSPConvert.cpp $
 *
 * $Author: Ln $
 * 
 * $Revision: 1 $
 * 
 * $Date: 28.02.07 19:07 $
 */

/*---------------------------------------------------------------------
 * Bug Fixes
 *
 * Bug#		Date	Author	Comment
 *-------------------------------------------------------------------*/

// DSPConvert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DSPConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


CString strSSFile (_T("mssccprj.scc"));

CString strProjEntry_SS  = _T("SCC_Project_Name = ");
CString strProjEntry_DSP = _T("Scc_ProjName");

BOOL DoIt();

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return 1;
	}

	cout << _T("   softCONTROL DSPConvert\n");
	cout << _T("----------------------------\n\n");

	if (argc <= 1)
	{
		cerr << _T("ERROR: Invalid parameters\nUsage: DSPConvert path");
		return 1;
	}

	CString strCurrentDir;
	GetCurrentDirectory (MAX_PATH, strCurrentDir.GetBufferSetLength (MAX_PATH));
	strCurrentDir.ReleaseBuffer();

	// Change directory 
	if (! SetCurrentDirectory (argv[1]))
	{
		cerr <<  _T("Error (") << GetLastError() << _T("): Can't find directory (") << (LPCTSTR)argv[1] << _T(")\n");
		return FALSE;
	}

	DoIt();

	SetCurrentDirectory(strCurrentDir);

	return 0;
}

BOOL DoIt()
{
	CString sCurrentDir;
	GetCurrentDirectory (MAX_PATH, sCurrentDir.GetBufferSetLength (MAX_PATH));
	sCurrentDir.ReleaseBuffer();

	CString sFind(sCurrentDir);
	sFind += "\\*.*";

	CFileFind FileFind;
	if (! FileFind.FindFile(sFind))
	{
		return TRUE;
	}

	BOOL bFound = TRUE;

	while (bFound == TRUE)
	{
		bFound = FileFind.FindNextFile();

		if (FileFind.IsDots())
			continue;

		if (FileFind.IsDirectory())
		{
			if (SetCurrentDirectory(FileFind.GetFileName()))
			{
				cout << (LPCTSTR)FileFind.GetFilePath() << _T("\n");

				if (! DoIt())
				{
					SetCurrentDirectory(sCurrentDir);
					return FALSE;
				}

				SetCurrentDirectory(sCurrentDir);
			}
		}
		else
		{
			CString sCurr = FileFind.GetFileName();
			sCurr.MakeLower();
/*
			if (sCurr.Find(".pl", sCurr.GetLength() - 4) == sCurr.GetLength() - 3)
			{
				cout << (LPCTSTR)FileFind.GetFilePath() << _T("\n");

				if (! ConvertPL (sCurr))
				{
					int i;
					i++;//return FALSE;
				}
			}

*/
			if (sCurr.Find(".dsp", sCurr.GetLength() - 5) == sCurr.GetLength() - 4)
			{
				cout << (LPCTSTR)FileFind.GetFilePath() << _T("\n");

				if (! ConvertDSP (sCurr))
				{
					return FALSE;
				}
			}

		}
	}

	return TRUE;
}

const CString sPLFind("error(s)");

const CString sFail (".$testfails.");
const CString sFailx (".($testfails).");
const CString sFail1("$testfails1");
const CString sFail2("$testfails2");

BOOL ConvertPL (CString const &strComp)
{
	CString strProjectFile = strComp;

	// Open Visual Studio project file
	HANDLE hPL = CreateFile (strProjectFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hPL == INVALID_HANDLE_VALUE)
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't open Perl-File (") << (LPCTSTR)strProjectFile << _T(")\n");
		return FALSE;
	}

	DWORD dwBytesRead = 0;

	// Read Visual Studio project file
	CString sData;

	if (! ReadFile (hPL, sData.GetBufferSetLength(200001), 200000, &dwBytesRead, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't read Perl-File (") << strProjectFile << _T(")\n");
		
		sData.ReleaseBuffer(0);

		CloseHandle (hPL);
		return FALSE;
	}

	sData.SetAt(dwBytesRead, _T('\0'));
	sData.ReleaseBuffer();
	CloseHandle (hPL);

	if (dwBytesRead < 25)
	{
		return TRUE;
	}

	// Find SS project entries
	int iEntry	 = sData.Find (sPLFind, 0);
	if (iEntry == -1)
	{
		return TRUE;
	}

	int iNewLine = sData.Find (_T('\n'), iEntry + 1);
	if (iNewLine == -1)
	{
		cerr << _T("Error: New Line not found.\n");
		return FALSE;
	}

	int iOldLine = -1;
	for (int i = iEntry; i > 0; i--)
	{
		if (sData[i] == '\n')
		{
			iOldLine = i;
			break;
		}
	}

	if (iOldLine == -1)
	{
		cerr << _T("Error: Old Line not found.\n");
		return FALSE;
	}


	CString sLine = sData.Right(sData.GetLength() - iOldLine - 1);
	sLine = sLine.Left(iNewLine - iOldLine - 2);

	int iTF	 = sLine.Find(sFail);
	int iTFx = sLine.Find(sFailx);
	int iTF1 = sLine.Find(sFail1);
	int iTF2 = sLine.Find(sFail1);

	if ((iTF != -1 || iTFx != -1) && iTF1 == -1 && iTF2 == -1)
	{
	}
	else if (iTF == -1 && iTFx == -1 && iTF1 != -1 && iTF2 != -1)
	{
	}
	else
	{
		cerr << _T("Error: 1.\n");
		return FALSE;
	}

	// Create temporary file
	CString strTempPath;
	GetTempPath (MAX_PATH, strTempPath.GetBufferSetLength(MAX_PATH));
	strTempPath.ReleaseBuffer();

	CString strTempFile;
	GetTempFileName(strTempPath, "~4C", 0, strTempFile.GetBufferSetLength(MAX_PATH));
	strTempFile.ReleaseBuffer();

	HANDLE hTemp = CreateFile (strTempFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hTemp == INVALID_HANDLE_VALUE)
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't open temporary file (") << (LPCTSTR)strTempFile << _T(")\n");
		return FALSE;
	}

	// Write copy to temporary file
	DWORD dwBytesWritten = 0;
	if (! WriteFile (hTemp, (LPCTSTR)sData, iOldLine + 1, &dwBytesWritten, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Write to (") << (LPCTSTR)strTempFile << _T(")\n");

		CloseHandle (hTemp);
		DeleteFile (strTempFile);
		return FALSE;
	}

	if ((iTF != -1 || iTFx != -1) && iTF1 == -1 && iTF2 == -1)
	{
		CString sData;

		sData  = "  if ($$ExtOut) {\r\n";
		sData += "      print \"Test result : \".$testfails.\" error(s)\\n\\n\";\r\n";
		sData += "  } else {\r\n";
		sData += "    if ($testfails) {\r\n";
		sData += "      print \"Test result : \".$testfails.\" error(s)\\n\";\r\n";
		sData += "    }\r\n";
		sData += "  }\r\n";

		if (! WriteFile (hTemp, sData, sData.GetLength(), &dwBytesWritten, NULL))
		{
			CloseHandle (hTemp);
			DeleteFile (strTempFile);
			cerr << _T("Error: 2.\n");
			return FALSE;
		}
    }
    else
    {
		CString sData;

		sData  = "  if ($$ExtOut) {\r\n";
		sData += "    print \"Test result : \".($testfails1+$testfails2).\" error(s)\\n\\n\";\r\n";
		sData += "  } else {\r\n";
		sData += "    if ($testfails1) {\r\n";
		sData += "      print \"Test result : \".($testfails1+$testfails2).\" error(s)\\n\";\r\n";
		sData += "    }\r\n";
		sData += "    if ($testfails2) {\r\n";
		sData += "      print \"Test result : \".($testfails1+$testfails2).\" error(s)\\n\";\r\n";
		sData += "    }\r\n";
		sData += "  }\r\n";
		
		if (! WriteFile (hTemp, sData, sData.GetLength(), &dwBytesWritten, NULL))
		{
			CloseHandle (hTemp);
			DeleteFile (strTempFile);
			cerr << _T("Error: 3.\n");
			return FALSE;
		}
	}

	if (! WriteFile (hTemp, ((LPCTSTR)sData) + iNewLine + 1, sData.GetLength() - iNewLine - 1, &dwBytesWritten, NULL))
	{
		CloseHandle (hTemp);
		DeleteFile (strTempFile);
		cerr << _T("Error: 4.\n");
		return FALSE;
	}

	CloseHandle (hTemp);

	// Copy temporary file back to project file
	if (! DeleteFile (strProjectFile))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Delete project file (") << (LPCTSTR)strProjectFile << _T(") ==> Maybe not checked out?\n");
		return FALSE;
	}

	if (! CopyFile (strTempFile, strProjectFile, TRUE))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Copy temporary file (") << (LPCTSTR)strTempFile << _T(")\n");
		return FALSE;
	}

	if (! DeleteFile (strTempFile))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Delete temporary file (") << (LPCTSTR)strTempFile << _T(")\n");
		return FALSE;
	}

	return TRUE;
}

BOOL ConvertDSP (CString const &strComp)
{
	CString strProjectFile = strComp;

	// Open Source Safe project file
	HANDLE hSS = CreateFile (strSSFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hSS == INVALID_HANDLE_VALUE)
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't open SourceSafe-File (") << (LPCTSTR)strSSFile << _T(")\n");
		return FALSE;
	}

	// Open Visual Studio project file
	HANDLE hProject = CreateFile (strProjectFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hProject == INVALID_HANDLE_VALUE)
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't open Project-File (") << (LPCTSTR)strProjectFile << _T(")\n");
		
		CloseHandle (hSS);
		return FALSE;
	}

	// Read Source Safe project file
	CString strSS;
	DWORD dwBytesRead = 0;

	if (! ReadFile (hSS, strSS.GetBufferSetLength(65535), 65534, &dwBytesRead, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't read SourceSafe-File (") << (LPCTSTR)strSSFile << _T(")\n");
		
		strSS.ReleaseBuffer();

		CloseHandle (hSS);
		CloseHandle (hProject);
		return FALSE;
	}
	
	strSS.SetAt(dwBytesRead, _T('\0'));
	strSS.ReleaseBuffer();
	CloseHandle (hSS);

	// Read Visual Studio project file
	CString strProject;

	if (! ReadFile (hProject, strProject.GetBufferSetLength(65535), 65534, &dwBytesRead, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't read SourceSafe-File (") << strProject << _T(")\n");
		
		strSS.ReleaseBuffer();

		CloseHandle (hProject);
		return FALSE;
	}

	strProject.SetAt(dwBytesRead, _T('\0'));
	strProject.ReleaseBuffer();
	CloseHandle (hProject);

	if (dwBytesRead < 25)
	{
		return TRUE;
	}

	// Find SS project entries
	int iProjEntry_SS = strSS.Find (strProjEntry_SS, 0);
	int iNewLine_SS = strSS.Find (_T('\n'), iProjEntry_SS + 1);

	if (iProjEntry_SS == -1)
	{
		cerr << _T("Error: SS project entry ") << (LPCTSTR)strProjEntry_SS << _T(" not found.\n");
		return FALSE;
	}

	int iProjEntry_DSP = strProject.Find (strProjEntry_DSP, 0);
	int iNewLine_DSP = strProject.Find (_T('\n'), iProjEntry_DSP + 1);

	if (iProjEntry_DSP == -1)
	{
		cerr << _T("Error: DSP project entry ") << (LPCTSTR)strProjEntry_DSP << _T(" not found.\n");
		return FALSE;
	}

	// Create temporary file
	CString strTempPath;
	GetTempPath (MAX_PATH, strTempPath.GetBufferSetLength(MAX_PATH));
	strTempPath.ReleaseBuffer();

	CString strTempFile;
	GetTempFileName(strTempPath, "~SC", 0, strTempFile.GetBufferSetLength(MAX_PATH));
	strTempFile.ReleaseBuffer();

	HANDLE hTemp = CreateFile (strTempFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if (hTemp == INVALID_HANDLE_VALUE)
	{
		cerr << _T("Error (") << GetLastError() << _T("): Can't open temporary file (") << (LPCTSTR)strTempFile << _T(")\n");
		return FALSE;
	}

	iProjEntry_DSP += strlen (strProjEntry_DSP) + 2;

	// Write copy to temporary file
	DWORD dwBytesWritten = 0;
	if (! WriteFile (hTemp, (LPCTSTR)strProject, iProjEntry_DSP, &dwBytesWritten, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Write to (") << (LPCTSTR)strTempFile << _T(")\n");

		CloseHandle (hTemp);
		DeleteFile (strTempFile);
		return FALSE;
	}

	if (! WriteFile (hTemp, (LPCTSTR)strSS + iProjEntry_SS + strlen (strProjEntry_SS), iNewLine_SS - iProjEntry_SS - strlen (strProjEntry_SS) - 1, &dwBytesWritten, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Write to (") << (LPCTSTR)strTempFile << _T(")\n");

		CloseHandle (hTemp);
		DeleteFile (strTempFile);
		return FALSE;
	}

	if (! WriteFile (hTemp, _T("\"\r\n"), strlen(_T("\"\r\n")), &dwBytesWritten, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Write to (") << (LPCTSTR)strTempFile << _T(")\n");

		CloseHandle (hTemp);
		DeleteFile (strTempFile);
		return FALSE;
	}

	if (! WriteFile (hTemp, (LPCTSTR)strProject + iNewLine_DSP + 1, strProject.GetLength() - iNewLine_DSP - 1, &dwBytesWritten, NULL))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Write to (") << (LPCTSTR)strTempFile << _T(")\n");

		CloseHandle (hTemp);
		DeleteFile (strTempFile);
		return FALSE;
	}

	CloseHandle (hTemp);

	// Copy temporary file back to project file
	if (! DeleteFile (strProjectFile))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Delete project file (") << (LPCTSTR)strProjectFile << _T(") ==> Maybe not checked out?\n");
		return FALSE;
	}

	if (! CopyFile (strTempFile, strProjectFile, TRUE))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Copy temporary file (") << (LPCTSTR)strTempFile << _T(")\n");
		return FALSE;
	}

	if (! DeleteFile (strTempFile))
	{
		cerr << _T("Error (") << GetLastError() << _T("): Delete temporary file (") << (LPCTSTR)strTempFile << _T(")\n");
		return FALSE;
	}

	return TRUE;
}
