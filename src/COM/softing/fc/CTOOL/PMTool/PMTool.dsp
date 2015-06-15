# Microsoft Developer Studio Project File - Name="PMTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=PMTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PMTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PMTool.mak" CFG="PMTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PMTool - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "PMTool - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool", NPTFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "PMTool - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f PMTool.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "PMTool.exe"
# PROP BASE Bsc_Name "PMTool.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f PMTool.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "PMTool.exe"
# PROP Bsc_Name "PMTool.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "PMTool - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f PMTool.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "PMTool.exe"
# PROP BASE Bsc_Name "PMTool.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f PMTool.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "PMTool.exe"
# PROP Bsc_Name "PMTool.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "PMTool - Win32 Release"
# Name "PMTool - Win32 Debug"

!IF  "$(CFG)" == "PMTool - Win32 Release"

!ELSEIF  "$(CFG)" == "PMTool - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CommSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\firmwaredata.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\PMPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PMTool.cpp
# End Source File
# Begin Source File

SOURCE=.\PMTool.rc
# End Source File
# Begin Source File

SOURCE=.\PMToolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageBackup.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageDeploy.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageFileTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPageRestore.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectTargetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\commdefinitions.h
# End Source File
# Begin Source File

SOURCE=.\CommSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\defs.h
# End Source File
# Begin Source File

SOURCE=.\firmwaredata.h
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\PMPropPage.h
# End Source File
# Begin Source File

SOURCE=.\PMTool.h
# End Source File
# Begin Source File

SOURCE=.\PMToolDlg.h
# End Source File
# Begin Source File

SOURCE=.\PropPageBackup.h
# End Source File
# Begin Source File

SOURCE=.\PropPageDeploy.h
# End Source File
# Begin Source File

SOURCE=.\PropPageFileTransfer.h
# End Source File
# Begin Source File

SOURCE=.\PropPageRestore.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectTargetDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\PMTool.ico
# End Source File
# Begin Source File

SOURCE=.\res\PMTool.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\PMTool.inf
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
