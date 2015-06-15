// KADMerger.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <windows.h>
#include "stdafx.h"


extern "C" __declspec(dllimport) long __stdcall Get4CPath(LPSTR pszFile);
extern "C" __declspec(dllimport) long __stdcall MergeKAD_KnieFK(LPSTR pszProjectPath, 
                                                      LPSTR pszKADFileName, 
                                                      LPSTR pszAddOnFileName, 
                                                      BOOL bOverwrite, 
                                                      LPSTR pszLogFile, 
                                                      BOOL bRemove);


#define KM_MERGE_TO_FILE    0
#define KM_MERGE_TO_4CENG   1
#define KM_MERGE_TO_4CG     2
#define KM_REMERGE          3


void ShowHelpScreen()
{
    printf("KADMerger usage:\n");
    printf("=======================================\n");
    printf("KADMerger /4ceng <MergeKad>\n");
    printf("KADMerger /4cg   <MergeKAD>\n");
    printf("KADMerger /remerge\n");
    printf("\n");
}



int CheckCommandLineParameter(int argc, char* argv[], int& iWhat, char* pszAddonKADName)
{
    
    if (argc<2)
    {
        ShowHelpScreen();
        printf("Error: Command line parameters incorrect!\n");
        return -1;
    }

    if (strcmp(argv[1], "/4ceng")==0)
    {
        iWhat = KM_MERGE_TO_4CENG;
        if (argc!=3)
        {
            ShowHelpScreen();
            printf("Error: Command line parameters incorrect!\n");
            return -1;
        }
        strcpy(pszAddonKADName, argv[2]);
    }
    else if (strcmp(argv[1], "/4cg")==0)
    {
        iWhat = KM_MERGE_TO_4CG;
        if (argc!=3)
        {
            ShowHelpScreen();
            printf("Error: Command line parameters incorrect!\n");
            return -1;
        }
        strcpy(pszAddonKADName, argv[2]);
    }
    else if (stricmp(argv[1], "/remerge")==0)
    {
        iWhat = KM_REMERGE;
        pszAddonKADName = NULL;
    }
    else
    {
        ShowHelpScreen();
        printf("Error: Command line parameters incorrect!\n");
        return -1;
    }

    return 0;
}



int MergeKADs(int iWhat, char* pszAddonKADName)
{
    int  i;
    char pszKadName[MAX_PATH];
    char pszInstallDir[MAX_PATH];
    char pszLogFile[MAX_PATH];
    char pszCurrDir[MAX_PATH];
    char pszAddKADFullPath[MAX_PATH];


    GetModuleFileName(NULL, pszCurrDir, MAX_PATH);
    for(i=strlen(pszCurrDir)-1; i>=0; i--)
    {
        if(pszCurrDir[i]=='\\' || pszCurrDir[i]=='/')
        {
            pszCurrDir[i+1] = 0;
            break;
        }
    }

    strcpy(pszAddKADFullPath, pszCurrDir);
    strcat(pszAddKADFullPath, pszAddonKADName);





    if (iWhat == KM_MERGE_TO_4CENG)
    {
        strcpy(pszKadName, "Engineering\\bin\\4ceng.kad");
        strcpy(pszLogFile, "KadMerge.log");
    }
    else if (iWhat == KM_MERGE_TO_4CG)
    {
        strcpy(pszKadName, "Engineering\\bin\\4cg.kad");
        strcpy(pszLogFile, "KadMerge_CG.log");
    }
    else 
    {
        printf("Error: Wrong command!\n");
        return -1;
    }

    // get installation path

    // hack fill with something: KAD merge Dll test for strlen of parameter
    for(i=0; i<MAX_PATH; i++)
    {
        pszInstallDir[i] = 1;
    }
    pszInstallDir[MAX_PATH-1] = 0;
    if (Get4CPath(pszInstallDir)!=TRUE)
    {
        printf("Error: Looking up ATCMControl installation path!\n");
        return -1;
    }

    if (MergeKAD_KnieFK(pszInstallDir, 
                 pszKadName, 
                 pszAddKADFullPath, 
                 TRUE, 
                 pszLogFile, 
                 FALSE)  != TRUE)
    {
        printf("Error: Merging KADs!\n");
        return -1;
    }

    printf("KAD merged successfully!\n");

    return 0;
}




int Remerge()
{
    int i;
    char pszInstallDir[MAX_PATH];
    char pszKadName[MAX_PATH];
    char pszOrigKadName[MAX_PATH];
    char pszBackKadName[MAX_PATH];
    char pszAddKAD[MAX_PATH];
    char pszAddKADFullPath[MAX_PATH];
    char pszLogFile[MAX_PATH];
    char pszFindFilePattern[MAX_PATH];
    BOOL bError = FALSE;

    // hack fill with something: KAD merge Dll test for strlen of parameter
    for(i=0; i<MAX_PATH; i++)
    {
        pszInstallDir[i] = 1;
    }
    pszInstallDir[MAX_PATH-1] = 0;
    if (Get4CPath(pszInstallDir)!=TRUE)
    {
        printf("Error: Looking up ATCMControl installation path!\n");
        return -1;
    }


    // ************************ 4ceng.kad  *************************
    strcpy(pszOrigKadName, pszInstallDir);
    strcat(pszOrigKadName, "Engineering\\bin\\4ceng.kad");
    
    strcpy(pszBackKadName, pszOrigKadName);
    strcat(pszBackKadName, ".back");

    DeleteFile(pszBackKadName);
    if (!MoveFile(pszOrigKadName, pszBackKadName))
    {
        printf("Could not create backup of 4ceng.kad\n");
    }

    printf("Merging 4ceng.kad\n");

    strcpy(pszKadName, "Engineering\\bin\\4ceng.kad");
    strcpy(pszLogFile, "KadMerge.log");


    HANDLE hFind;
    WIN32_FIND_DATA findFileData;
    BOOL bFound = TRUE;

    strcpy(pszFindFilePattern, pszInstallDir);
    strcat(pszFindFilePattern, "Engineering\\bin\\*_4ceng.kad");


    // start with Kernel_IEC_4ceng.kad and skip this later
    strcpy(pszAddKAD, "Kernel_IEC_4ceng.kad");
    strcpy(pszAddKADFullPath, pszInstallDir);
    strcat(pszAddKADFullPath, "Engineering\\bin\\");
    strcat(pszAddKADFullPath, pszAddKAD);
    printf("Merging KAD: %s\n", pszAddKAD);
    if (MergeKAD_KnieFK(pszInstallDir, 
                         pszKadName, 
                         pszAddKADFullPath, 
                         TRUE, 
                         pszLogFile, 
                         FALSE)  != TRUE)
    {
        printf("Error: Merging KAD %s!\n", pszAddKAD);
        bError = TRUE;
    }
  
    hFind = FindFirstFile(pszFindFilePattern, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (bFound)
        {
            strcpy(pszAddKAD, findFileData.cFileName);

            if (stricmp(pszAddKAD, "Kernel_IEC_4ceng.kad")!=0)
            {
                strcpy(pszAddKADFullPath, pszInstallDir);
                strcat(pszAddKADFullPath, "Engineering\\bin\\");
                strcat(pszAddKADFullPath, pszAddKAD);
                printf("Merging KAD: %s\n", pszAddKAD);
                if (MergeKAD_KnieFK(pszInstallDir, 
                                     pszKadName, 
                                     pszAddKADFullPath, 
                                     TRUE, 
                                     pszLogFile, 
                                     FALSE)  != TRUE)
                {
                    printf("Error: Merging KAD %s!\n", pszAddKAD);
                    bError = TRUE;
                }
            }
            bFound = FindNextFile(hFind, &findFileData);
        }
        FindClose(hFind);
    }



    // ************************ 4cg.kad  *************************
    strcpy(pszOrigKadName, pszInstallDir);
    strcat(pszOrigKadName, "Engineering\\bin\\4cg.kad");
    
    strcpy(pszBackKadName, pszOrigKadName);
    strcat(pszBackKadName, ".back");

    DeleteFile(pszBackKadName);
    if (!MoveFile(pszOrigKadName, pszBackKadName))
    {
        printf("Could not create backup of 4ceng.kad\n");
    }

    printf("\nRemerging 4cg.kad\n");

    strcpy(pszKadName, "Engineering\\bin\\4cg.kad");
    strcpy(pszLogFile, "KadMerge_CG.log");


    bFound = TRUE;

    strcpy(pszFindFilePattern, pszInstallDir);
    strcat(pszFindFilePattern, "Engineering\\bin\\*_4cg.kad");
  
    hFind = FindFirstFile(pszFindFilePattern, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (bFound)
        {
            strcpy(pszAddKAD, findFileData.cFileName);

            strcpy(pszAddKADFullPath, pszInstallDir);
            strcat(pszAddKADFullPath, "Engineering\\bin\\");
            strcat(pszAddKADFullPath, pszAddKAD);
            printf("Merging KAD: %s\n", pszAddKAD);
            if (MergeKAD_KnieFK(pszInstallDir, 
                                 pszKadName, 
                                 pszAddKADFullPath, 
                                 TRUE, 
                                 pszLogFile, 
                                 FALSE)  != TRUE)
            {
                printf("Error: Merging KAD %s!\n", pszAddKAD);
                bError = TRUE;
            }
            bFound = FindNextFile(hFind, &findFileData);
        }
        FindClose(hFind);
    }

    printf("\n");
    if (bError)
    {
        printf("*************\n"
               " Errors during KAD remerging,\n"
               " the old KAD files are still available (4cg|4ceng).kad.back,\n"
               " please rename to original file names.\n\n");
    }
    else
    {
        printf("KADs successfully remerged!\n");
    }
    printf("\n");

    return 0;
}


int main(int argc, char* argv[])
{
    int iWhat;
    char pszAddonKADName[MAX_PATH];

    printf("KADMerger\n");
    printf("*********\n");
    printf("\n");

    if (0 != CheckCommandLineParameter(argc, argv, iWhat, pszAddonKADName))
    {
        return -1;
    }
    
    if (iWhat == KM_REMERGE)
    {
        return Remerge();
    }
    else
    {
        return MergeKADs(iWhat, pszAddonKADName);
    }
}

