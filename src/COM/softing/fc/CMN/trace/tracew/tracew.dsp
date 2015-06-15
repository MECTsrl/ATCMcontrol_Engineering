# Microsoft Developer Studio Project File - Name="tracew" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=tracew - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tracew.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tracew.mak" CFG="tracew - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tracew - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "tracew - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CMN/trace/tracew", WSRFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "tracew - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f tracew.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "tracew.exe"
# PROP BASE Bsc_Name "tracew.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f tracew.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "tracew.exe"
# PROP Bsc_Name "tracew.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "tracew - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f tracew.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "tracew.exe"
# PROP BASE Bsc_Name "tracew.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f tracew.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "tracew.exe"
# PROP Bsc_Name "tracew.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "tracew - Win32 Release"
# Name "tracew - Win32 Debug"

!IF  "$(CFG)" == "tracew - Win32 Release"

!ELSEIF  "$(CFG)" == "tracew - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\trace.cpp
# End Source File
# Begin Source File

SOURCE=..\traceex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\inc\traceif.h
# End Source File
# End Group
# Begin Group "INF"

# PROP Default_Filter "*.inf"
# Begin Source File

SOURCE=.\tracew.inf
# End Source File
# End Group
# End Target
# End Project
