

#include "stdafx.h"
#include "FileHelper.h"

#include "CEProjMan.h"
#include "CEProjectManager.h"
#include "CEBackparse.h"
#include "CEMsg.h"
#include "CEProjInfo.h"

#include "CEProjectManager.h"


#include "fc_tools\fc_string.h"
#include "utilif.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CCEProjectManager* CFileHelper::pProjMan = NULL;



//------------------------------------------------------------------*
/**
 * IsAbsolutePath:
 * Determine if the given string containes an absolute or relative path information
 *
 * @param           strFile: the file name with or without path information
 * @return          is this an absolute path or not
 * @exception       -
 * @see             
*/
BOOL CFileHelper::IsAbsolutePath(const CString& strFile)
{
     if (FC_StringIsAbsPath(strFile)>0)
     {
         return TRUE;
     }
     else
     {
         return FALSE;
     }
}



//------------------------------------------------------------------*
/**
 * GetFilePath
 * get the path part of a full file name
 *
 * @param           strFile: the full file name
 * @return          the path part of the file name
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFilePath(const CString& strFile)
{
    CString strFilePath;

    int iPosLastSlash;
    iPosLastSlash = strFile.ReverseFind(_T('\\'));
    if (strFile.ReverseFind(_T('/')) > iPosLastSlash)
    {
        iPosLastSlash = strFile.ReverseFind(_T('/'));
    }
       
    if (iPosLastSlash>0)
    {
        strFilePath = strFile.Left(iPosLastSlash);
    }
    else
    {
        strFilePath = _T("");
    }
    return strFilePath;
}



//------------------------------------------------------------------*
/**
 * GetFileName
 * get the name part of a full file name
 *
 * @param           strFile: the full file name
 * @return          the name of the file
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileName(const CString& strFile)
{
    CString strFileName;
    CString strFileWithExt = GetFileNameWithExt(strFile);

    int iPosDot;
    iPosDot = strFileWithExt.ReverseFind(_T('.'));
    if (iPosDot>0)
    {
        strFileName = strFileWithExt.Left(iPosDot);
    }
    else
    {
        strFileName = strFileWithExt;
    }
    return strFileName;
}



//------------------------------------------------------------------*
/**
 * GetFileExt
 * Get the extension of the file
 *
 * @param           strFile: the file name
 * @return          the extension of the file
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileExt(const CString& strFile)
{
    CString strFileExt;
    CString strFileWithExt = GetFileNameWithExt(strFile);

    int iPosDot;
    iPosDot = strFileWithExt.ReverseFind(_T('.'));
    if (iPosDot>0)
    {
        strFileExt = strFileWithExt.Mid(iPosDot+1);
    }
    else
    {
        strFileExt = _T("");
    }

    return strFileExt;
}



//------------------------------------------------------------------*
/**
 * GetFileNameWithExt
 * Get the filename and extension without path parts
 *
 * @param           strFile: full file name
 * @return          the filename and extension part
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileNameWithExt(const CString& strFile)
{
    CString strFileNameWithExt;

    int iPosLastSlash;
    iPosLastSlash = strFile.ReverseFind(_T('\\'));
    if (strFile.ReverseFind(_T('/')) > iPosLastSlash)
    {
        iPosLastSlash = strFile.ReverseFind(_T('/'));
    }
       
    if (iPosLastSlash>0)
    {
        strFileNameWithExt = strFile.Mid(iPosLastSlash+1);
    }
    else
    {
        strFileNameWithExt = strFile;
    }
    return strFileNameWithExt;
}



//------------------------------------------------------------------*
/**
 * GetFileAbsolute
 * Create an absolute path for a file name with project relative path information
 *
 * @param           strFile: the project relative file information
 * @return          the absolute path information
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileAbsolute(const CString& strFile)
{
    CString strAbs;

    if (!IsAbsolutePath(strFile))
    {
        strAbs = pProjMan->GetProjectPath() + _T("\\") + strFile;
    }
    else
    {
        strAbs = strFile;
    }

    return strAbs;
}



//------------------------------------------------------------------*
/**
 * GetFileAbsoluteUnique
 * Get a unique file information (full path, all slashes are \)
 *
 * @param           strFile: the file information
 * @return          the unique file information
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileAbsoluteUnique(const CString& strFile)
{
    CString strAbs = GetFileAbsolute(strFile);
    strAbs = GetFileLower(strAbs);

    // replace / with \ to get unique path
    for (int i=0; i<strAbs.GetLength(); i++)
    {
        if (strAbs[i] == _T('/'))
        {
            strAbs.SetAt(i, _T('\\'));
        }
    }

    // todo replace .. and .

    return strAbs;
}



//------------------------------------------------------------------*
/**
 * GetFileLower
 * returns the file name and path in lower letters
 *
 * @param           strFile: the file information
 * @return          the lower path infos
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileLower(const CString& strFile)
{
    CString strLower;
    strLower = strFile;
    strLower.MakeLower();
    return strLower;
}



//------------------------------------------------------------------*
/**
 * ReplaceMacros
 * Replace all ATCMControl macros in a string
 *
 * @param           strFile: the string with macros
 * @return          the string with extended macros
 * @exception       -
 * @see             
*/
CString CFileHelper::ReplaceMacros(const CString& strFile)
{
    ASSERT(pProjMan);
    if (!pProjMan)
    {
        return _T("");
    }

    CString strNew;
    CString strRest = strFile;

    int iDollarPos = -1;
    iDollarPos = strRest.Find(_T('$'));

    while(iDollarPos>=0)
    {
        CString strMacro;
        CString strEnv;
        CString strReg;

        strNew += strRest.Left(iDollarPos);
        // leave $ in none of both
        strRest = strRest.Mid(iDollarPos+1);

        if (strRest.GetLength()>=4)
        {
            ExtractNamedMacro(_T("env"), strRest, strEnv);
            ExtractNamedMacro(_T("reg"), strRest, strReg);
        }
        if (strRest.GetLength()>=1)
        {
            CString strLeft = strRest.Left(1);
            if (strLeft.Compare(_T("("))==0)
            {
                int iDollarRight = strRest.Find(_T(")"));
                if (iDollarRight>0)
                {
                    strMacro = strRest.Mid(1,iDollarRight-1);
                    strRest = strRest.Mid(iDollarRight+1);
                }
            }
        }

        // what has been found
        if (!strMacro.IsEmpty())
        {
            CString strReplace;
            if (strMacro.CompareNoCase(_T("PrjDir"))==0)
            {
                strReplace = pProjMan->GetProjectPath();
            }
            else if (strMacro.CompareNoCase(_T("PrjName"))==0)
            {
                strReplace = pProjMan->GetProjectName();
            }
            else if (strMacro.CompareNoCase(_T("LibDir"))==0)
            {
                TCHAR pszLibPath[1000];   
                HRESULT hr = UTIL_GetStdLibPath(pszLibPath,1000);
                if (hr==S_OK)
                {
                    strReplace = pszLibPath;
                }
            }
            else if (strMacro.CompareNoCase(_T("InstDir"))==0)
            {
                TCHAR pszInstPath[1000];   
                size_t sizeChars = 1000;
                HRESULT hr = UTIL_GetInstallPath(pszInstPath, &sizeChars);
                if (hr==S_OK)
                {
                    strReplace = pszInstPath;
                }
            }
            if (!strReplace.IsEmpty())
            {
                strNew += strReplace;
            }
            else 
            {
                strNew += _T("$(")+strMacro+_T(")");
            }
        }
        else if (!strEnv.IsEmpty())
        {
            CString strEnvVar = _tgetenv(strEnv);
            if (!strEnvVar.IsEmpty())
            {
                strNew += strEnvVar;
            }
        }
        else if (!strReg.IsEmpty())
        {
            CString strRegVal = ExpandRegistryMacro(strReg);
            if(!strRegVal.IsEmpty())
            {
                strNew += strRegVal;
            }
        }
        else  // nothing of all possibilities
        {
            strNew += _T("$");
        }

        iDollarPos = strRest.Find(_T('$'));
    }

    strNew += strRest;

    return strNew;  
}


//------------------------------------------------------------------*
/**
 * extract named macro.
 *
 * @param           crstrName: macro name
 * @param           rstrRest: not yet parsed part of macro string (in/out)
 * @param           rstrMacroText: macro text within braces (out)
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CFileHelper::ExtractNamedMacro(const CString& crstrName, CString& rstrRest, CString& rstrMacroText)
{
    CString strTmpName;
    strTmpName.Format("%s(", crstrName);        // temp name = name + "("
    int iTmpNameLen = strTmpName.GetLength();
    CString strLeft = rstrRest.Left(iTmpNameLen);

    if(strLeft.CompareNoCase(strTmpName)==0)
    {
        int iBraceRight = 0;
        int iNumBraceOpen = 1;
        int iLen = rstrRest.GetLength();

        for(int iChar = iTmpNameLen; iChar < iLen; ++iChar)
        {
            if(rstrRest[iChar] == _T('('))
            {
                ++iNumBraceOpen;
            }
            else if (rstrRest[iChar] == _T(')'))
            {
                --iNumBraceOpen;
                if(iNumBraceOpen == 0)
                {
                    iBraceRight = iChar;                        
                }
                break;
            }
        }

        if (iBraceRight>0)
        {
            rstrMacroText = rstrRest.Mid(iTmpNameLen, iBraceRight-iTmpNameLen);
            rstrRest = rstrRest.Mid(iBraceRight+1);
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * expand registry macro.
 *
 * @param           [in] crstrRegExpr: something like:
 *                          "HKEY_LOKAL_MACHINE\Software\Softing, InstallPath"
 *                           ^^^^^^^^^^^^^^ reg path ^^^^^^^^^^^,^^reg name^^
 * @return          string value of this key
*/
CString CFileHelper::ExpandRegistryMacro(const CString& crstrRegExpr)
{
    CString strValue;
    CString strRegBase;
    CString strRegPath;
    CString strRegName;

    int iFound;
    iFound = crstrRegExpr.Find(_T(','));
    if(iFound >= 0)
    {
        strRegPath = crstrRegExpr.Left(iFound);
        strRegName = crstrRegExpr.Mid(iFound + 1);
    }
    else
    {
        strRegPath = crstrRegExpr;
    }
    strRegPath.TrimLeft();
    strRegPath.TrimRight();
    strRegName.TrimLeft();
    strRegName.TrimRight();
    if(!strRegPath.IsEmpty())
    {
        iFound = strRegPath.Find(_T('\\'));
        if(iFound >= 0)
        {
            strRegBase = strRegPath.Left(iFound);
            strRegPath = strRegPath.Mid(iFound + 1);
            strValue = GetRegistryString(strRegBase, strRegPath, strRegName);
        }
    }
    return strValue;
}
     

//------------------------------------------------------------------*
/**
 * get registry string.
 *
 * @param           [in] crstrBase: HKEY_XXX with XXX = CLASSES_ROOT, CURRENT_USER
 *                                                      LOCAL_MACHINE or USERS
 * @param           [in] crstrPath: registry path without preceding or trailing '\' 
 * @param           [in] crstrName: registry key name
 *                          if empty the standard key value is returned
 * @return          string value of registry key
*/
CString CFileHelper::GetRegistryString(const CString& crstrBase, const CString& crstrPath, const CString& crstrName)
{
    CString strValue;
    long    lResult;
    HKEY    hKey;
    if(crstrBase.Compare(_T("HKEY_CLASSES_ROOT"))==0)
    {
        lResult = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, crstrPath, 0, KEY_READ, &hKey);
    }
    else if(crstrBase.Compare(_T("HKEY_CURRENT_USER"))==0)
    {
        lResult = ::RegOpenKeyEx(HKEY_CURRENT_USER, crstrPath, 0, KEY_READ, &hKey);
    }
    else if(crstrBase.Compare(_T("HKEY_LOCAL_MACHINE"))==0)
    {
        lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, crstrPath, 0, KEY_READ, &hKey);
    }
    else if(crstrBase.Compare(_T("HKEY_USERS"))==0)
    {
        lResult = ::RegOpenKeyEx(HKEY_USERS, crstrPath, 0, KEY_READ, &hKey);
    }

    if (lResult != ERROR_SUCCESS)
    {
        return strValue;
    }

    DWORD   dwBufSize = _MAX_PATH;
    DWORD   dwType;
    if(!crstrName.IsEmpty())
    {
        lResult = ::RegQueryValueEx(hKey, crstrName, NULL, &dwType, (unsigned char*)strValue.GetBuffer(_MAX_PATH), &dwBufSize);
    }
    else
    {
        lResult = ::RegQueryValueEx(hKey, NULL, NULL, &dwType, (unsigned char*)strValue.GetBuffer(_MAX_PATH), &dwBufSize);
    }
    strValue.ReleaseBuffer();

    ::RegCloseKey(hKey);
    return strValue;
}

                                    



//------------------------------------------------------------------*
/**
 * GetFileAbsoluteReplaced
 * Get absolute file info with the macros replaced
 *
 * @param           strFile: the file info
 * @return          the expanded file info
 * @exception       -
 * @see             
*/
CString CFileHelper::GetFileAbsoluteReplaced(const CString& strFile)
{
    return CFileHelper::GetFileAbsolute(CFileHelper::ReplaceMacros(strFile));
}



//------------------------------------------------------------------*
/**
 * StripProjectPath
 * Strip the project path from a path info. That means create a project 
 * relative path of an absolute path.
 *
 * @param           strFile: the absolute path
 * @return          the project relative path
 * @exception       -
 * @see             
*/
CString CFileHelper::StripProjectPath(const CString& strFile)
{
    ASSERT(pProjMan);
    CString strProjPath;
    CString strPure;

    if (!IsAbsolutePath(strFile))
    {
        return strFile;
    }
    else
    {
        strProjPath = pProjMan->GetProjectPath();
        if (strFile.GetLength()<=strProjPath.GetLength())
        {
            return strFile;
        }
        CString strLeftOfFile = strFile.Left(strProjPath.GetLength());
        CString strProjPathUnique = CFileHelper::GetFileAbsoluteUnique(strProjPath);
        strLeftOfFile = CFileHelper::GetFileAbsoluteUnique(strLeftOfFile);
        if (strProjPathUnique.CompareNoCase(strLeftOfFile)!=0)
        {
            return strFile;
        }
        else
        {
            strPure = strFile.Mid(strProjPath.GetLength());
            while(!strPure.IsEmpty() 
                  && (strPure.GetAt(0)==_T('/') || strPure.GetAt(0)==_T('\\')))
            {
                strPure = strPure.Mid(1);
            }
            return strPure;
        }
    }
}


//------------------------------------------------------------------*
/**
 * ReplaceStdLibPath
 * Replace the standard lib path in an absolute path info with the
 * $LIB macro
 *
 * @param           strFile: the absolute path info
 * @return          the replaced path info
 * @exception       -
 * @see             
*/
CString CFileHelper::ReplaceStdLibPath(const CString& strFile)
{
    ASSERT(pProjMan);
    CString strLibPath;
    CString strPure;

    if (!IsAbsolutePath(strFile))
    {
        return strFile;
    }
    else
    {
        TCHAR pszLibPath[1000];   
        HRESULT hr = UTIL_GetStdLibPath(pszLibPath,1000);
        if (hr!=S_OK)
        {
            return strFile;
        }
        strLibPath = pszLibPath;
        if (strFile.GetLength()<=strLibPath.GetLength())
        {
            return strFile;
        }
        CString strLeftOfFile = strFile.Left(strLibPath.GetLength());
        CString strLibPathUnique = CFileHelper::GetFileAbsoluteUnique(strLibPath);
        strLeftOfFile = CFileHelper::GetFileAbsoluteUnique(strLeftOfFile);
        if (strLibPathUnique.CompareNoCase(strLeftOfFile)!=0)
        {
            return strFile;
        }
        else
        {
            strPure = strFile.Mid(strLibPath.GetLength());
            while(!strPure.IsEmpty() 
                  && (strPure.GetAt(0)==_T('/') || strPure.GetAt(0)==_T('\\')))
            {
                strPure = strPure.Mid(1);
            }
            return (CString)(_T("$(LIBDIR)")) + _T("\\") + strPure;
        }
    }
}
