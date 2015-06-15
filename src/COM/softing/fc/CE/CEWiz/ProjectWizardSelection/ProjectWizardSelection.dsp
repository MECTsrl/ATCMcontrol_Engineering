# Microsoft Developer Studio Project File - Name="ProjectWizardSelection" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ProjectWizardSelection - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProjectWizardSelection.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProjectWizardSelection.mak" CFG="ProjectWizardSelection - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProjectWizardSelection - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "ProjectWizardSelection - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEWiz/ProjectWizardSelection", AKNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "ProjectWizardSelection - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f ProjectWizardSelection.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ProjectWizardSelection.exe"
# PROP BASE Bsc_Name "ProjectWizardSelection.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f ProjectWizardSelection.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ProjectWizardSelection.exe"
# PROP Bsc_Name "ProjectWizardSelection.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "ProjectWizardSelection - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f ProjectWizardSelection.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ProjectWizardSelection.exe"
# PROP BASE Bsc_Name "ProjectWizardSelection.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f ProjectWizardSelection.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ProjectWizardSelection.exe"
# PROP Bsc_Name "ProjectWizardSelection.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "ProjectWizardSelection - Win32 Release"
# Name "ProjectWizardSelection - Win32 Debug"

!IF  "$(CFG)" == "ProjectWizardSelection - Win32 Release"

!ELSEIF  "$(CFG)" == "ProjectWizardSelection - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\inc\CEProjWiz.idl
# End Source File
# Begin Source File

SOURCE=.\error.cpp
# End Source File
# Begin Source File

SOURCE=.\extern.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardSelection.def
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardSelection.rc
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardSelectionApp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=..\Inc\ErrorMessage.h
# End Source File
# Begin Source File

SOURCE=.\extern.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardSelectionApp.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\WizardList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\clvwimag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ProjectWizardSelection.rc2
# End Source File
# Begin Source File

SOURCE=.\res\wizimage.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ProjectWizardSelection.inf
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
