# Microsoft Developer Studio Project File - Name="WinVMM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=WinVMM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinVMM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinVMM.mak" CFG="WinVMM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinVMM - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "WinVMM - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/Target/4CWin/Control/WinVMM", XFXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "WinVMM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f WinVMM.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "WinVMM.exe"
# PROP BASE Bsc_Name "WinVMM.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f WinVMM.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "WinVMM.exe"
# PROP Bsc_Name "WinVMM.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "WinVMM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f WinVMM.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "WinVMM.exe"
# PROP BASE Bsc_Name "WinVMM.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f WinVMM.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "WinVMM.exe"
# PROP Bsc_Name "WinVMM.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "WinVMM - Win32 Release"
# Name "WinVMM - Win32 Debug"

!IF  "$(CFG)" == "WinVMM - Win32 Release"

!ELSEIF  "$(CFG)" == "WinVMM - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\winMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Control.inf
# End Source File
# Begin Source File

SOURCE=.\WinVMM.inf
# End Source File
# End Target
# End Project
