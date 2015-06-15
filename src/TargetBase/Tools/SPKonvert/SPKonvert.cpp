/* $Header: /4CReleased/V2.20.00/TargetBase/TOOLS/SPKonvert/SPKonvert.cpp 1     28.02.07 19:07 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: SPKonvert.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/TOOLS/SPKonvert/SPKonvert.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Tools
 *
 * =COMPONENT			SPKonvert
 *
 * =CURRENT 	 $Date: 28.02.07 19:07 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "SPKonvert.h"
#include "GHFile.h"
#include "utilif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

#define DEFAULT_POU_PROT_REG_KEY  _T("SOFTWARE\\SIEMENS\\POUPROT")


//------------------------------------------------------------------*
/**
 * strip line.
 *
 * @param           rstrLine: stripped line
 * @param           rstrRest: rest
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL StripLine(CString& rstrLine, CString& rstrRest)
{
    if(rstrRest.IsEmpty())
    {
        rstrLine.Empty();
        return FALSE;
    }

    int iFound = rstrRest.Find(_T('\r'));
    if(iFound < 0)
    {
        rstrLine = rstrRest;
        rstrRest.Empty();
        return TRUE;
    }

    rstrLine = rstrRest.Left(iFound);
    rstrRest = rstrRest.Mid(iFound + 2);


    return TRUE;
}


//------------------------------------------------------------------*
/**
 * read key from section.
 *
 * @param           crstrContent: file content
 * @param           crstrSection: section
 * @param           crstrKey: key
 * @param           rstrValue: value of key.
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL ReadKeyFromSection(const CString& crstrContent, const CString& crstrSection, const CString& crstrKey, CString& rstrValue)
{
    int     iFound;
    CString strSectionFind;
    CString strContentWork(crstrContent);
    CString strLine;
    BOOL    bSectionFound(FALSE);
    BOOL    bKeyFound(FALSE);

    strSectionFind.Format("[%s]", crstrSection);
    iFound = crstrContent.Find(strSectionFind);
    if(iFound < 0)
    {
        return FALSE;
    }

    while(StripLine(strLine, strContentWork))
    {
        iFound = strLine.Find(strSectionFind);
        if(iFound >= 0)
        {
            bSectionFound = TRUE;
            break;
        }
    }

    if(!bSectionFound)
    {
        return FALSE;
    }

    while(StripLine(strLine, strContentWork))
    {
        iFound = strLine.Find(_T('['));
        if(iFound >= 0)
        {
            break;
        }
        iFound = strLine.Find(crstrKey);
        if(iFound >= 0)
        {
            iFound = strLine.Find(_T('='));
            if(iFound >= 0)
            {
                bKeyFound = TRUE;
                break;
            }
        }
    }

    if(!bKeyFound)
    {
        return FALSE;
    }

    rstrValue = strLine.Mid(iFound + 1);
    rstrValue.TrimLeft();
    rstrValue.TrimRight();

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * generate guid.
 *
 * @param           -
 * @return          new guid as string including {}
 * @exception       -
 * @see             -
*/
CString GenerateGuid()
{
    CString strGuid;
	GUID m_guid = GUID_NULL;
	::CoCreateGuid(&m_guid);
	if (m_guid == GUID_NULL)
	{
        printf("error creating guid\n");
		return strGuid;
	}

	strGuid.Format( "{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                    m_guid.Data1, m_guid.Data2, m_guid.Data3,
		            m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3],
		            m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);

    return strGuid;
}


//------------------------------------------------------------------*
/**
 * get default password.
 *
 * @param           -
 * @return          default password.
 * @exception       -
 * @see             -
*/
CString GetDefaultPassword()
{
    // read registry key HKLM/SOFTWARE/Siemens/PouProt/DefPD
    CString strPassword(_T("123456"));
    CString strEncrypted;
    long    lResult;
    HKEY    hKey;

    lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, DEFAULT_POU_PROT_REG_KEY, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        return strPassword;
    }

    DWORD   dwBufSize = _MAX_PATH;
    DWORD   dwType;
    lResult = ::RegQueryValueEx(hKey, _T("DefPD"), NULL, &dwType, (unsigned char*)strEncrypted.GetBuffer(_MAX_PATH), &dwBufSize);
    strEncrypted.ReleaseBuffer();

    if (lResult != ERROR_SUCCESS) 
    {
        //close the key
        ::RegCloseKey(hKey);
        return strPassword;
    }

    //close the key
    ::RegCloseKey(hKey);

    // now decrypt it
    CString strDecrypted;
    long    lSize = strEncrypted.GetLength();

    HRESULT hr = UTIL_DecryptString(strEncrypted, strDecrypted.GetBuffer(lSize), NULL);
    strDecrypted.ReleaseBuffer();

    ASSERT(hr == S_OK);
    if(hr != S_OK)
    {
        return strPassword;
    }

    strPassword = strDecrypted;

    return strPassword;
}

//------------------------------------------------------------------*
/**
 * insert project guid.
 *
 * @param           crstrProjectFile: project file incl. path.
 * @param           rstrProjectGuid: project guid without brackets.
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL InsertProjectGuid(const CString& crstrProjectFile, CString& rstrProjectGuid)
{
    CString     strContent;     // file content
    CFileStatus tStatus;

    if(!CFile::GetStatus(crstrProjectFile, tStatus))
    {
        return FALSE;
    }
    long    lSize = tStatus.m_size;

    CFile   tFile;
    try
    {
        tFile.Open(crstrProjectFile, CFile::modeRead);
        tFile.Read(strContent.GetBuffer(lSize+1), lSize);
        strContent.ReleaseBuffer();
        strContent.SetAt(lSize, 0);
        tFile.Close();
    }
    catch(CFileException* pE)
    {
        pE->Delete();
        return FALSE;
    }

    int     iStartPos = 0;
    CString strGuidTmp;

    // if project already contains GUID just read it and return.
    if(ReadKeyFromSection(strContent, _T("Attributes"), _T("GUID"), strGuidTmp))
    {
        // strip quotes and brackets
        strGuidTmp = strGuidTmp.Mid(1);
        strGuidTmp = strGuidTmp.Left(strGuidTmp.GetLength()-1);
        rstrProjectGuid = strGuidTmp;
        return TRUE;
    }

    // search for section [Attributes]
    CString strContentNew;
    CString strLine;
    int     iInsertPos = strContent.Find(_T("[Attributes]"));
    if(iInsertPos < 0)
    {
        strLine.Format("GUID=\"%s\"\r\n", rstrProjectGuid);
        strContentNew.Format("[Attributes]\r\n%s\r\n%s", strLine, strContent);
    }
    else
    {
        iInsertPos = strContent.Find("\r\n", iInsertPos);
        if(iInsertPos >= 0)
        {
            CString strRest;

            iInsertPos += 2;
            strRest = strContent.Mid(iInsertPos);
            strLine.Format("GUID=\"%s\"\r\n", rstrProjectGuid);
            strContentNew.Format("%s%s%s", strContent.Left(iInsertPos), strLine, strRest);
        }
        else
        {
            strLine.Format("\r\nGUID=\"%s\"", rstrProjectGuid);
            strContentNew += strLine;
        }
    }

    // now write file
    try
    {

        if(!tFile.Open(crstrProjectFile, CFile::modeWrite | CFile::modeCreate))
        {
            return FALSE;
        }
        tFile.Write(strContentNew, strContentNew.GetLength());
        tFile.Close();
    }
    catch(CFileException* pE)
    {
        pE->Delete();
        return FALSE;
    }


    return TRUE;
}

//------------------------------------------------------------------*
/**
 * generate gh file.
 *
 * @param           crstrProjectPath: project path
 * @param           crstrPassword: password
 * @param           crstrProjectGuid: project guid
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL GenerateGHFile(const CString& crstrProjectPath, const CString& crstrPassword, const CString& crstrProjectGuid)
{
    CString strFileName;
    strFileName.Format("%s\\Project.4gh", crstrProjectPath);

    CGHFile tGHFile;
    tGHFile.SetFileName(strFileName);
    tGHFile.SetPassword(crstrPassword);
    tGHFile.SetProjectGuid(crstrProjectGuid);

    if(tGHFile.Write())
    {
        return TRUE;
    }
    printf("error: protection data could not be written!\n");
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * ConvertProject
 *
 * @param           crstrProjectFile: project file name including full path
 * @param           crstrProjectPath: project path
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL ConvertProject(const CString& crstrProjectFile, const CString& crstrProjectPath)
{
	// check if file extension is correct
	if(crstrProjectFile.Right(4).CompareNoCase(_T(".4cp")) != 0)
	{
		return FALSE;
	}

    // insert project guid into file
    CString strProjectGuid = GenerateGuid();

    if(!InsertProjectGuid(crstrProjectFile, strProjectGuid))
    {
        CString strMessage;
        strMessage.Format("error: could not open project file %s\n", crstrProjectFile);
        printf(strMessage);
        return FALSE;
    }

    // get default password
    CString strPassword = GetDefaultPassword();

    if(!GenerateGHFile(crstrProjectPath, strPassword, strProjectGuid))
    {
        CString strMessage;
        strMessage.Format("error: error: could convert project file %s\n", crstrProjectFile);
        printf(strMessage);
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * %function%
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL FindProjectFile(CString& rstrProjectFile)
{
    BOOL    bReturn = FALSE;
    CString strCurrentDir;
    CString strFindPath;

    ::GetCurrentDirectory(MAX_PATH, strCurrentDir.GetBuffer(MAX_PATH));
    strCurrentDir.ReleaseBuffer();
    strFindPath.Format("%s\\*.4cp", strCurrentDir);

    WIN32_FIND_DATA   tFindData;
    memset(&tFindData, 0, sizeof(tFindData));

    HANDLE  hFile = ::FindFirstFile(strCurrentDir, &tFindData);
    if(hFile)
    {
        rstrProjectFile.Format("%s.4cp", tFindData.cFileName);
        ::FindClose(hFile);
        bReturn = TRUE;
    }
    return bReturn;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
        CString strProjectFile;
        CString strProjectPath;

        // test arguments
        switch(argc)
        {
        case 1:
            if(!FindProjectFile(strProjectFile))
            {
                printf("error: no project file found in current directory\n");
                return -1;
            }
            break;
        case 2:
            strProjectFile = argv[1];
            break;
        default:
            printf("usage: SPKonvert <project file>\n");
            printf("   or: SPKonvert <project path>\n");
            break;
        }

        // get project path from project file path
        int iFound = strProjectFile.ReverseFind(_T('\\'));
        if(iFound < 0)
        {
            ::GetCurrentDirectory(MAX_PATH, strProjectPath.GetBuffer(MAX_PATH));
            strProjectPath.ReleaseBuffer();
            strProjectFile = strProjectPath + _T("\\") + strProjectFile;
        }
        else
        {
            strProjectPath = strProjectFile.Left(iFound);
        }

        if(!ConvertProject(strProjectFile, strProjectPath))
        {
            printf("project conversion failed\n");
            nRetCode = -1;
        }
        else
        {
            printf("project successfully converted\n");
        }
	}

	return nRetCode;
}

/* ---------------------------------------------------------------------------- */
