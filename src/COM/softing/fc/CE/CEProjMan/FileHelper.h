#if !defined(AFX_FILEHELPER_H__2509DBDF_176B_4A45_A5B2_06EFDFEDDB54__INCLUDED_)
#define AFX_FILEHELPER_H__2509DBDF_176B_4A45_A5B2_06EFDFEDDB54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"


class CCEProjectManager;


//------------------------------------------------------------------*
/**
 * CFileHelper 
 * a helper calss to switch between absolute and roject relative path,
 * to get file name, extension and path separated and do all macro 
 * replacing stuff
 *
*/
class CFileHelper  
{
public:
    static CCEProjectManager* pProjMan;

    static BOOL    IsAbsolutePath(const CString& strFile);
    static CString GetFilePath(const CString& strFile);
    static CString GetFileName(const CString& strFile);
    static CString GetFileExt(const CString& strFile);
    static CString GetFileNameWithExt(const CString& strFile);
    static CString GetFileAbsolute(const CString& strFile);
    static CString GetFileAbsoluteUnique(const CString& strFile);
    static CString GetFileLower(const CString& strFile);
    static CString ReplaceMacros(const CString& strFile);
    static CString GetFileAbsoluteReplaced(const CString& strFile);
    static CString StripProjectPath(const CString& strFile);
    static CString ReplaceStdLibPath(const CString& strFile);
    static BOOL    ExtractNamedMacro(const CString& crstrName, CString& rstrRest, CString& rstrMacroText);
    static CString ExpandRegistryMacro(const CString& crstrRegExpr);
    static CString GetRegistryString(const CString& crstrBase, const CString& crstrPath, const CString& crstrName);

};

#endif // !defined(AFX_FILEHELPER_H__2509DBDF_176B_4A45_A5B2_06EFDFEDDB54__INCLUDED_)


