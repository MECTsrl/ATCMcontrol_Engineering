/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_fileIOW.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_fileIOW.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_fileIOW.c $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== contains the unicode implementations for functions in fc_fileIO.h,
 *   the linker should not attract windows w-functions if not ready used.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  26.11.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <windows.h>

#include "fc_tools\fc_fileIO.h"



//RETURN VALUE : TRUE : SubDir exists
FC_TOAPI int FC_TODECL FC_ExistSubDirW(const wchar_t* lpDirectory)
{
    unsigned long dwAttr = GetFileAttributesW(lpDirectory);
    return (dwAttr != 0xFFFFFFFF && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)!=0);
}

//RETURN VALUE : TRUE : SubDir created successfully
FC_TOAPI int FC_TODECL FC_CreateSubDirW(const wchar_t* lpDirectory)
{
    WCHAR szTemp[MAX_PATH];
    WCHAR szDir[MAX_PATH];
    wchar_t* lpStr;

    szDir[0] = L'\0';
    lstrcpyW(szTemp, lpDirectory);

    lpStr = wcstok(szTemp, L"\\");  // get first tok, could be server name
    if (lpStr!=NULL)
    {
        lpStr = szTemp; // keep leading '\\'
        if (lpStr[0] == L'\\' && lpStr[1] == L'\\') 
        {
            lstrcatW(szDir, lpStr);
            lpStr = wcstok(NULL, L"\\");  // UNCpath: get next token
        }
    }

    while (lpStr != NULL)
    {
      if (szDir[0] != 0)
        lstrcatW(szDir, L"\\");
      lstrcatW(szDir, lpStr);
      if (!FC_ExistSubDirW(szDir))
      {
        if (!CreateDirectoryW(szDir, NULL))
          return FALSE;
      }
      lpStr = wcstok(NULL, L"\\");
    }

    return TRUE;
}


//RETURN VALUE : TRUE  : File exists
FC_TOAPI int FC_TODECL FC_ExistW(const wchar_t* lpszFile)
{
    unsigned long dwAttr = GetFileAttributesW(lpszFile);
    return (dwAttr != 0xFFFFFFFF);
}









/*
 *----------------------------------------------------------------------------*
 *  $History: fc_fileIOW.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 3.12.01    Time: 16:56
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * extra file for unicode implem. of fileIO
 *==
 *----------------------------------------------------------------------------*
*/
