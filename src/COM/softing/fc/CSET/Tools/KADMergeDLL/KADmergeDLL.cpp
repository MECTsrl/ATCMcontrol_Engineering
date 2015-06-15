/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmergeDLL.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: KADmergeDLL.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmergeDLL.cpp $
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

// KADmergeDLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "KADmergeDLL.h"
#include "KADmerge.h"

#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CKADmergeDLLApp

BEGIN_MESSAGE_MAP(CKADmergeDLLApp, CWinApp)
	//{{AFX_MSG_MAP(CKADmergeDLLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKADmergeDLLApp construction

CKADmergeDLLApp::CKADmergeDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKADmergeDLLApp object

CKADmergeDLLApp theApp;

LONG Get4CInstPath(string & sFile)
{
    LONG lRes = 0;
    BOOL bRet = FALSE;
    HKEY hPath;
    lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE,                     // handle to open key
                        "SOFTWARE\\softing\\4ControlV2\\2.0",   // subkey name
                        0,                                      // reserved
                        KEY_READ,                               // security access mask
                        &hPath);                                // handle to open key

    if(lRes == ERROR_SUCCESS)
    {
        DWORD dwType = REG_SZ;
        DWORD dwSize = 0;
        lRes = RegQueryValueEx(hPath, "Path", NULL, &dwType, NULL, &dwSize);
        if(lRes == ERROR_SUCCESS)
        {
            // allocate memmory
            unsigned char * szValue = NULL;
            szValue = new unsigned char[dwSize];
            lRes = RegQueryValueEx(hPath, "Path", NULL, &dwType, szValue, &dwSize);
            if(!lRes)
            {
                sFile = (char*)szValue;
                if(sFile.at(sFile.length() - 1) != '\\')
                {
                    sFile = sFile + '\\';
                }
            }
            delete szValue;
            bRet = TRUE;
        }
    }
    RegCloseKey(hPath);

    return bRet;
}

extern "C" __declspec(dllexport) LONG WINAPI Get4CPath(LPSTR pszFile)
{
    if(!pszFile)
    {
        return FALSE;
    }
    LONG lRet = 0;
    string sProjectpath;
    lRet = Get4CInstPath(sProjectpath);
    if(strlen(pszFile) < sProjectpath.length())
    {
        return FALSE;
    }
    strcpy(pszFile, sProjectpath.c_str());

    return lRet;
}

extern "C" __declspec(dllexport) LONG WINAPI GetKernelLanguageAttribute(LPSTR pszProjectPath, LPSTR pszKADFileName, LPSTR pszLanguage, LPSTR pszLogFile)
{
    if(!pszLanguage)
    {
        return FALSE;
    }
    HRESULT hRes = S_FALSE;
    LONG lRet = FALSE;
    string sProjectPath(pszProjectPath);
    string sElementPattern = "/KAD4CE/KERNEL/ADDON[@NAME = \"ATCM Kernel\"]";
    string sAttributePattern = "/KAD4CE/KERNEL/ADDON/@LANGUAGE";
    string sLanguageID;

    try
    {
        CKADmerge aKad(sProjectPath + pszKADFileName);
        hRes = aKad.GetValueByPattern(sElementPattern,
                                      sAttributePattern,
                                      sLanguageID,
                                      sProjectPath + pszLogFile);
        if(hRes == S_OK && strlen(pszLanguage) >= sLanguageID.length())
        {
            lRet = TRUE;
            strcpy(pszLanguage, sLanguageID.c_str());
        }
    }
    catch(string str)
    {
        lRet = FALSE;
    }

    return lRet;
}

extern "C" __declspec(dllexport) LONG WINAPI Installed4CKernel(LPSTR pszProjectPath, LPSTR pszKADFileName, LPSTR pszLogFile)
{
    HRESULT hRes = S_FALSE;
    LONG lRet = TRUE;
    string sProjectPath(pszProjectPath);
    string sElementPattern = "/KAD4CE/KERNEL/ADDON[@NAME = \"ATCM Kernel\"]";
    string sAttributePattern = "@NAME";
    string sValue;

    try
    {
        CKADmerge aKad(sProjectPath + pszKADFileName);
        hRes = aKad.GetValueByPattern(sElementPattern,
                                      sAttributePattern,
                                      sValue,
                                      sProjectPath + pszLogFile);
        if(hRes == S_OK && sValue == "ATCM Kernel")
        {
            lRet = TRUE;
        }
    }
    catch(string str)
    {
        lRet = FALSE;
    }

    return lRet;
}

extern "C" __declspec(dllexport) LONG WINAPI InstalledIECAddOn(LPSTR pszProjectPath, LPSTR pszKADFileName, LPSTR pszLogFile)
{
    HRESULT hRes = S_FALSE;
    LONG lRet = FALSE;
    string sProjectPath(pszProjectPath);
    string sElementPattern = "/KAD4CE/KERNEL/ADDON[@NAME = \"4C IEC AddOn\"]";
    string sAttributePattern = "@NAME";
    string sValue;

    try
    {
        CKADmerge aKad(sProjectPath + pszKADFileName);
        hRes = aKad.GetValueByPattern(sElementPattern,
                                      sAttributePattern,
                                      sValue,
                                      sProjectPath + pszLogFile);
        if(hRes == S_OK && sValue == "4C IEC AddOn")
        {
            lRet = TRUE;
        }
    }
    catch(string str)
    {
        lRet = FALSE;
    }

    return lRet;
}

extern "C" __declspec(dllexport) LONG WINAPI MergeKAD(LPSTR pszProjectPath, LPSTR pszKADFileName, LPSTR pszAddOnFileName, BOOL bOverwrite, LPSTR pszLogFile, BOOL bRemove)
{
    LONG lRet = FALSE;
    string sProjectPath(pszProjectPath);
    string sAddOnFileName = pszAddOnFileName;
    CKADmerge * pKad = NULL;

    try
    {
        pKad = new CKADmerge(sProjectPath + pszKADFileName);

        {
            pKad->OpenLog(sProjectPath + pszLogFile, "MergeKAD()");
            pKad->Log("pszProjectPath:   " + string(pszProjectPath));
            pKad->Log("pszKADFileName:   " + string(pszKADFileName));
            pKad->Log("pszAddOnFileName: " + string(pszAddOnFileName));
            pKad->Log("bOverwrite:       " + string(bOverwrite ? "TRUE" : "FALSE"));
            pKad->Log("pszLogFile:       " + string(pszLogFile));
            pKad->Log("bRemove:          " + string(bRemove ? "TRUE" : "FALSE"));
            pKad->CloseLog();
        }

        if(bRemove)
        {
            // remove addon kad from existing kad
            HRESULT hRes = 0;
            string sKadFile = sProjectPath + pszKADFileName;

            string * psKADFilenames = NULL;
            LONG lLength;
            hRes = pKad->GetKADFilenames(&psKADFilenames, lLength, sProjectPath + pszLogFile);

            // remove kad file
            if(hRes == S_OK)
            {
                hRes = pKad->RemoveKADFile(sKadFile, sProjectPath + pszLogFile);

                CKADmerge * pNewKad = new CKADmerge(sProjectPath + pszKADFileName);

                for(int i = 0; i < (int)lLength; i++)
                {
                    string sDummy = psKADFilenames[i];
                    if(psKADFilenames[i] != string(pszAddOnFileName) )
                    {
                        if(S_OK == pNewKad->Merge(sProjectPath + psKADFilenames[i], bOverwrite, sProjectPath + pszLogFile))
                        {
                            lRet = TRUE;
                        }
                    }
                }
                delete [] psKADFilenames;
                delete pNewKad;
            }
            else
            {
                lRet = FALSE;
            }
        }
        else
        {
            // add addon kad to existing kad
            if(S_OK == pKad->Merge(sProjectPath + sAddOnFileName, bOverwrite, sProjectPath + pszLogFile))
            {
                lRet = TRUE;
            }
        }
    }
    catch(string str)
    {
        lRet = FALSE;
    }

    if(pKad)
        delete pKad;

    return lRet;
}



//EF+JD: Power turbo HACK !
static int  KAD_StringIsAbsPath(const TCHAR* pszPath)
{
    //use isalpha, no multbyte char should be a drive letter.
    if(isalpha(pszPath[0]) && pszPath[1]==_T(':'))
        return 1;
    if(pszPath[0]==_T('\\') && pszPath[1]==_T('\\'))
        return 2;
    return 0;
}
extern "C" __declspec(dllexport) LONG WINAPI MergeKAD_KnieFK(LPSTR pszProjectPath, LPSTR pszDestKAD, LPSTR pszSrcKAD, BOOL bOverwrite, LPSTR pszLogFile, BOOL bRemove)
{
    LONG lRet = FALSE;
    string sProjectPath(pszProjectPath);
    string sSrcKAD;
    string sDestKAD;
    CKADmerge * pKad = NULL;

    if(KAD_StringIsAbsPath(pszDestKAD))
        sDestKAD = pszDestKAD;
    else
        sDestKAD = sProjectPath + pszDestKAD;

    if(KAD_StringIsAbsPath(pszSrcKAD))
        sSrcKAD = pszSrcKAD;
    else
        sSrcKAD = sProjectPath + pszSrcKAD;

    try
    {
        pKad = new CKADmerge(sDestKAD);

        {
            pKad->OpenLog(sProjectPath + pszLogFile, "MergeKAD()");
            pKad->Log("sProjectPath:   " + sProjectPath);
            pKad->Log("sDestKAD:       " + sDestKAD);
            pKad->Log("sSrcKAD:        " + sSrcKAD);
            pKad->Log("bOverwrite:     " + string(bOverwrite ? "TRUE" : "FALSE"));
            pKad->Log("pszLogFile:     " + string(pszLogFile));
            pKad->Log("bRemove:        " + string(bRemove ? "TRUE" : "FALSE"));
            pKad->CloseLog();
        }

        if(bRemove)
        {
            // remove addon kad from existing kad
            HRESULT hRes = 0;

            string * psKADFilenames = NULL;
            LONG lLength;
            hRes = pKad->GetKADFilenames(&psKADFilenames, lLength, sProjectPath + pszLogFile);

            // remove kad file
            if(hRes == S_OK)
            {
                hRes = pKad->RemoveKADFile(sDestKAD, sProjectPath + pszLogFile);

                CKADmerge * pNewKad = new CKADmerge(sDestKAD);

                for(int i = 0; i < (int)lLength; i++)
                {
                    if(psKADFilenames[i] != string(pszSrcKAD) )
                    {
                        string tmphack;
                        if(KAD_StringIsAbsPath(psKADFilenames[i].c_str()))
                            tmphack = psKADFilenames[i];
                        else
                            tmphack = sProjectPath + psKADFilenames[i];


                        if(S_OK == pNewKad->Merge(tmphack, bOverwrite, sProjectPath + pszLogFile))
                        {
                            lRet = TRUE;
                        }
                    }
                }
                delete [] psKADFilenames;
                delete pNewKad;
            }
            else
            {
                lRet = FALSE;
            }
        }
        else
        {
            // add addon kad to existing kad
            if(SUCCEEDED(pKad->Merge(sSrcKAD, bOverwrite, sProjectPath + pszLogFile)))
            {
                lRet = TRUE;
            }
        }
    }
    catch(string str)
    {
        lRet = FALSE;
    }

    if(pKad)
        delete pKad;

    return lRet;
}





extern "C" __declspec(dllexport) LONG WINAPI SetKernelLanguageAttribute(LPSTR pszProjectPath, LPSTR pszKADFileName, LPSTR pszLanguage, LPSTR pszLogFile)
{
    HRESULT hRes = S_FALSE;
    LONG lRet = FALSE;
    string sProjectPath(pszProjectPath);
    /*string sElementPattern = "/KAD4CE/KERNEL/ADDON[@NAME = \"ATCM Kernel\"]";
    string sAttributePattern = "/KAD4CE/KERNEL/ADDON/@LANGUAGE";*/
    string sElementPattern = "/KAD4CE/MAINMENU";
    string sAttributePattern = "";
    string sLanguageID = pszLanguage;

    try
    {
        CKADmerge aKad(sProjectPath + pszKADFileName);
        hRes = aKad.SetValueByPattern(sElementPattern,
                                      sAttributePattern,
                                      sLanguageID,
                                      sProjectPath + pszLogFile);
        if(hRes == S_OK)
        {
            lRet = TRUE;
        }
    }
    catch(string str)
    {
        lRet = FALSE;
    }

    return lRet;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: KADmergeDLL.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * 
 * *****************  Version 6  *****************
 * User: Jd           Date: 24.05.02   Time: 12:35
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * Added remerging.
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 11.04.02   Time: 11:25
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * invented knie fk
 * 
 * *****************  Version 4  *****************
 * User: Kt           Date: 22.03.01   Time: 17:27
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * changed 'sAttributePattern' in 'Installed4CKernel()' and
 * 'InstaledIECAddOn()'
 * 
 * *****************  Version 3  *****************
 * User: Kt           Date: 22.03.01   Time: 17:00
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * included 'Installed4CKernel()' and 'InstalledIECAddOn()'
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 21.03.01   Time: 13:57
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 15.03.01   Time: 14:14
 * Created in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * New Project
 * 
 *----------------------------------------------------------------------------*
*/
