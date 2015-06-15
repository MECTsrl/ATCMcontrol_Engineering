# Microsoft Developer Studio Project File - Name="ProjectWizardBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ProjectWizardBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProjectWizardBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProjectWizardBase.mak" CFG="ProjectWizardBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProjectWizardBase - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "ProjectWizardBase - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/ProjectWizardBase", OCUFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "ProjectWizardBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f ProjectWizardBase.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ProjectWizardBase.exe"
# PROP BASE Bsc_Name "ProjectWizardBase.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /nologo /f ProjectWizardBase.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ProjectWizardBase.exe"
# PROP Bsc_Name "ProjectWizardBase.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "ProjectWizardBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f ProjectWizardBase.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ProjectWizardBase.exe"
# PROP BASE Bsc_Name "ProjectWizardBase.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /nologo /f ProjectWizardBase.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ProjectWizardBase.exe"
# PROP Bsc_Name "ProjectWizardBase.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "ProjectWizardBase - Win32 Release"
# Name "ProjectWizardBase - Win32 Debug"

!IF  "$(CFG)" == "ProjectWizardBase - Win32 Release"

!ELSEIF  "$(CFG)" == "ProjectWizardBase - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ProjectWizardData.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardLibsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardPropsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardResource.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardTask.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ProjectWizardData.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardDefs.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardLibrary.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardLibsPage.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardPage.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardPropsPage.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardRes.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardResource.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardTask.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Res"

# PROP Default_Filter "*.bmp;*.rc"
# Begin Source File

SOURCE=.\libspage.bmp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardRes.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\ProjectWizardBase.inf
# End Source File
# End Target
# End Project
