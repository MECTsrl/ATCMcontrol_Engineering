# Microsoft Developer Studio Project File - Name="osShared" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=osShared - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "osShared.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "osShared.mak" CFG="osShared - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "osShared - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "osShared - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/Control/osShared", BTXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "osShared - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f osShared.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "osShared.exe"
# PROP BASE Bsc_Name "osShared.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f osShared.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "osShared.exe"
# PROP Bsc_Name "osShared.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "osShared - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f osShared.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "osShared.exe"
# PROP BASE Bsc_Name "osShared.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f osShared.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "osShared.exe"
# PROP Bsc_Name "osShared.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "osShared - Win32 Release"
# Name "osShared - Win32 Debug"

!IF  "$(CFG)" == "osShared - Win32 Release"

!ELSEIF  "$(CFG)" == "osShared - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\osMsg.c
# End Source File
# Begin Source File

SOURCE=.\osUtil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Source File

SOURCE=.\osShared.inf
# End Source File
# End Target
# End Project
