/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CLIC/RemoveLic/removlic.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: removlic.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CLIC/RemoveLic/removlic.cpp $
 *
 * =PROJECT             CAK1020    ATCMControl V2.0
 *
 * =SWKE                CLIC
 *
 * =COMPONENT           REMOVLIC
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
 *   Implementation if License application for panel PC's dialog
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.02.01  Wi      File created (copied from V1.3)
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


#include "stdafx.h"
#include "resource.h"


int gError = 0;


void RemoveLicKey(HANDLE file, const char* str)
{
    char pszOutput[10000];
    DWORD dwNumberOfBytesWritten;

    char strRegKey[10000];

    strcpy(pszOutput, str);
    strcat(pszOutput, ": ");

    strcpy(strRegKey, "SOFTWARE\\Softing\\License\\");
    strcat(strRegKey, str);

    // checks if a key does exist in the HKEY_LOCAL_MACHINE section
    HKEY hKey;

    if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegKey, 0, KEY_ALL_ACCESS, &hKey)) {
        strcat(pszOutput, "no lic info\r\n");
        WriteFile(file, pszOutput, strlen(pszOutput), &dwNumberOfBytesWritten, NULL);
        return;
    }

    strcat(pszOutput, "\r\n");
    strcat(pszOutput, "lic: ");

    char regValue[10000];
    DWORD dwLen;
    DWORD dwType;
    LONG lret; 
    lret = RegQueryValueEx(hKey, "lic",     // value name
                          NULL,             // reserved
                          &dwType,          // type buffer
                          (LPBYTE)regValue, // data buffer
                          &dwLen);
    if(lret != ERROR_SUCCESS) {
        strcat(pszOutput, "not found\r\n");
    } else {
        strcat(pszOutput, regValue);
        strcat(pszOutput, "  -->  ");
        lret = RegDeleteValue(hKey, "lic");
        if(lret != ERROR_SUCCESS) {
            gError ++;
            strcat(pszOutput, "failed to delete !!!!!!\r\n");
        } else {
            strcat(pszOutput, "deleted\r\n");
        }

    }

    strcat(pszOutput, "tmp: ");

    lret = RegQueryValueEx(hKey, "tmp",     // value name
                          NULL,             // reserved
                          &dwType,          // type buffer
                          (LPBYTE)regValue, // data buffer
                          &dwLen);
    if(lret != ERROR_SUCCESS) {
        strcat(pszOutput, "not found\r\n");
    } else {
        for(unsigned int j=0; j<dwLen; j++) {
            char pszValue[5];
            unsigned char uch = (unsigned char)(regValue[j]);
            _itoa((int)(uch), pszValue, 16);
            strcat(pszOutput, pszValue);
            strcat(pszOutput, " ");
        }
        strcat(pszOutput, "  -->  ");
        lret = RegDeleteValue(hKey, "tmp");
        if(lret != ERROR_SUCCESS) {
            strcat(pszOutput, "failed to delete !!!!!!\r\n");
        } else {
            strcat(pszOutput, "deleted\r\n");
        }
    }

    WriteFile(file, pszOutput, strlen(pszOutput), &dwNumberOfBytesWritten, NULL);

    RegCloseKey(hKey);
}



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
    char pszPath[MAX_PATH];
    char pszOutput[10000];
    DWORD dwNumberOfBytesWritten;

    GetModuleFileName(NULL, pszPath, MAX_PATH);

    pszPath[strlen(pszPath)-3] = '\0';
    strcat(pszPath, "log");

    HANDLE file = NULL;

    file = CreateFile(pszPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if(!file) {
        MessageBox(NULL, "Failed to create log file!", "ATCMControl Remove Lic", MB_OK|MB_ICONERROR);
    }

    strcpy(pszOutput, "RemoveLic\r\n");
    WriteFile(file, pszOutput, strlen(pszOutput), &dwNumberOfBytesWritten, NULL);

    SYSTEMTIME sysTime;

    GetSystemTime(&sysTime);

    sprintf(pszOutput, "%i.%i.%i,%i:%i:%i\r\n", 
            sysTime.wDay, sysTime.wMonth, sysTime.wYear, 
            sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    WriteFile(file, pszOutput, strlen(pszOutput), &dwNumberOfBytesWritten, NULL);

    // rever to the SoftingProd.h file for all products relatet to 4C!
    // remove the V2.0 ATCM Productkeys
    int i;
    for(i=1; i<=6; i++) {
        char strKey[10];
        _itoa(i, strKey, 10);
        RemoveLicKey(file, strKey);
    }
    RemoveLicKey(file, "20");
    RemoveLicKey(file, "21");
    RemoveLicKey(file, "23");
    RemoveLicKey(file, "52");

    // remove the V2.1 ATCM Productkey
    for(i=61; i<=75; i++) {
        char strKey[10];
        _itoa(i, strKey, 10);
        RemoveLicKey(file, strKey);
    }
    RemoveLicKey(file, "79");
    RemoveLicKey(file, "85");
    RemoveLicKey(file, "86");
    
    CloseHandle(file);

    if(gError>0) {
        MessageBox(NULL, "Not all license info could be deleted!", "ATCMControl Remove Lic", MB_OK|MB_ICONERROR);
    } else {
        sprintf(pszOutput, "ATCMControl license information removed successfully.\nLog information stored in %s", pszPath);
        MessageBox(NULL, pszOutput, "ATCMControl Remove Lic", MB_OK);

    }

	return 0;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: removlic.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CLIC/RemoveLic
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 9.12.05    Time: 15:55
 * Updated in $/4Control/COM/softing/fc/CLIC/RemoveLic
 * list of valid product keys extendet
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:32
 * Created in $/4Control/COM/softing/fc/CLIC/RemoveLic
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 19.06.02   Time: 15:38
 * Updated in $/4Control/COM/softing/fc/CLIC/RemoveLic
 * 
 * *****************  Version 1  *****************
 * User: Wi           Date: 2.02.01    Time: 15:55
 * Created in $/4Control/COM/softing/fc/CLIC/RemoveLic
 * moved from V1.3
 * 
 *==
 *----------------------------------------------------------------------------*
*/
