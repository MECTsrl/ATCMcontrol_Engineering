// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\KADmerge_exp.h"
#include <crtdbg.h>

/*
-m Engineering\bin\4ceng.kad Engineering\bin\Kernel_4ceng.kad Engineering\bin\IECAddon_4ceng.kad Engineering\bin\4CPC_4ceng.kad Engineering\bin\4CWago_4ceng.kad
-d Engineering\bin\4ceng.kad Engineering\bin\IECAddon_4ceng.kad
-d Engineering\bin\4ceng.kad Engineering\bin\Kernel_4ceng.kad
-d Engineering\bin\4ceng.kad Engineering\bin\4CPC_4ceng.kad
-g Engineering\bin\4ceng.kad
-s Engineering\bin\4ceng.kad 1031
-i Engineering\bin\4ceng.kad
*/

void printUsage()
{
    printf("Test.exe\n");
    printf("KADmerge.dll test program\n\n");
    printf("Usage:\n");
    printf(" test -m 4CEngKAD-File AddOnKAD-File1 AddOnKAD-File2 ... \n");
    printf(" test -d 4CEngKAD-File AddOnKAD-File\n");
    printf(" test -g 4CEngKAD-File\n");
    printf(" test -s 4CEngKAD-File LanguageID\n\n");
    printf(" test -i 4CEngKAD-File\n\n");
    printf("Options:\n");
    printf(" m - Merge KAD Files\n");
    printf(" d - Unmerge KAD File\n");
    printf(" g - Get the 4C Engineering language attribute\n");
    printf(" s - Set the 4C Engineering language attribute\n\n");
    printf(" i - Installed 4C Kernel\n\n");
    printf("Samples:\n");
    printf(" test -m Engineering\\bin\\4ceng.kad Engineering\\bin\\Kernel_4ceng.kad Engineering\\bin\\IECAddon_4ceng.kad\n");
    printf(" Merge the Addon kad files\n\n");
    printf(" test -d Engineering\\bin\\4ceng.kad Engineering\\bin\\Kernel_4ceng.kad\n");
    printf(" Unmerge the Addon kad file\n\n");
    printf(" test -g Engineering\bin\4ceng.kad\n");
    printf(" Get the language attribute from the\n");
    printf(" 'Engineering\\bin\\4ceng.kad' kad file.\n\n");
    printf(" test -s Engineering\bin\4ceng.kad 1031\n");
    printf(" Set the language attribute in the\n");
    printf(" 'Engineering\\bin\\4ceng.kad' kad file to '1031# (english)\n\n");
}

int main(int argc, char* argv[])
{
    //_CrtSetBreakAlloc(264);

    long lRes = 0;
    int i = 0;
    BOOL bOverwrite;
    BOOL bRemove;
    LPSTR pszLogFile = new TCHAR[MAX_PATH];

    if(argc < 2)
    {
        printUsage();
        return lRes;
    }

    int iSwitch = argv[1][1];
    LPSTR pszLanguage = new TCHAR[MAX_PATH];

    LPSTR pszProjectPath = new TCHAR[MAX_PATH];
    Get4CPath(pszProjectPath);

    // set log file
    strcpy(pszLogFile, "KadMerge.log");

    switch(iSwitch)
    {
    case 100:   // d: Unmerge / Delete
        bOverwrite = TRUE;
        bRemove = TRUE;
        if(argc < 4)
        {
            printUsage();
            return lRes;
        }
        printf("Test: KEDmergeDLL\n");
        for(i = 0; i < argc; i++)
        {
            printf("%s ", argv[i]);
        }
        printf("\nmerge kad files ...\n");
        for(i = 3; i < argc; i++)
        {
            lRes = MergeKAD(pszProjectPath, argv[2], argv[i], bOverwrite, pszLogFile, bRemove);
        }
        break;
    case 109:   // m: Merge
        bOverwrite = TRUE;
        bRemove = FALSE;
        if(argc < 4)
        {
            printUsage();
            return lRes;
        }
        printf("Test: KEDmergeDLL\n");
        for(i = 0; i < argc; i++)
        {
            printf("%s ", argv[i]);
        }
        printf("\nmerge kad files ...\n");
        for(i = 3; i < argc; i++)
        {
            lRes = MergeKAD(pszProjectPath, argv[2], argv[i], bOverwrite, pszLogFile, bRemove);
        }
        break;
    case 103:   // g: get language
        if(argc < 3)
        {
            printUsage();
            return lRes;
        }
        lRes = GetKernelLanguageAttribute(pszProjectPath, argv[2], pszLanguage, pszLogFile);
        if(lRes == TRUE)
        {
            printf("language attribute: %s\n", pszLanguage);
        }
        else
        {
            printf("The language attribute is not set.\n");
        }
        break;
    case 115:   // s: set language
        if(argc < 4)
        {
            printUsage();
            return lRes;
        }
        lRes = SetKernelLanguageAttribute(pszProjectPath, argv[2], argv[3], pszLogFile);
        if(lRes == TRUE)
        {
            printf("Set language attribute to %s\n", argv[3]);
        }
        else
        {
            printf("Error. Check if the 'KERNEL' element in the 4CEng kad files exists.\n");
        }
        break;
    case 105:   // i: installed 4c kernel
        if(argc < 3)
        {
            printUsage();
            return lRes;
        }
        //lRes = Installed4CKernel(pszProjectPath, argv[2], pszLogFile);    // 4c kernel
        lRes = InstalledIECAddOn(pszProjectPath, argv[2], pszLogFile);      // iec addon
        if(lRes == TRUE)
        {
            printf("4C Kernel is installed.\n");
        }
        else
        {
            printf("4C Kernel is not installed.\n");
        }
        break;
    default:
    	printUsage();
    }

    return (int)lRes;
}

