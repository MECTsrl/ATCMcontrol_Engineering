# Microsoft Developer Studio Project File - Name="fc_baselib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=fc_baselib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fc_baselib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fc_baselib.mak" CFG="fc_baselib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fc_baselib - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "fc_baselib - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib", SORFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "fc_baselib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f fc_baselib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_baselib.exe"
# PROP BASE Bsc_Name "fc_baselib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f fc_baselib.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_baselib.exe"
# PROP Bsc_Name "fc_baselib.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "fc_baselib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f fc_baselib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_baselib.exe"
# PROP BASE Bsc_Name "fc_baselib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f fc_baselib.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_baselib.exe"
# PROP Bsc_Name "fc_baselib.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "fc_baselib - Win32 Release"
# Name "fc_baselib - Win32 Debug"

!IF  "$(CFG)" == "fc_baselib - Win32 Release"

!ELSEIF  "$(CFG)" == "fc_baselib - Win32 Debug"

!ENDIF 

# Begin Group "Sources"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\baselib.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseLibCOM.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseLibWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\blstring.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\time.cpp
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter "*.h"
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\baselib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BaseLibWrap.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\baselib.rc
# End Source File
# Begin Source File

SOURCE=.\FCBaseLib.rgs
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BaseLib.def
# End Source File
# Begin Source File

SOURCE=.\fc_baselib.inf
# End Source File
# Begin Source File

SOURCE=.\test_BaseLib.htm
# End Source File
# End Target
# End Project
