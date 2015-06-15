# Microsoft Developer Studio Project File - Name="StdEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=StdEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StdEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StdEditor.mak" CFG="StdEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StdEditor - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "StdEditor - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/StdEditor", FNPFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "StdEditor - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f StdEditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StdEditor.exe"
# PROP BASE Bsc_Name "StdEditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f StdEditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StdEditor.exe"
# PROP Bsc_Name "StdEditor.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "StdEditor - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f StdEditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StdEditor.exe"
# PROP BASE Bsc_Name "StdEditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f StdEditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StdEditor.lib"
# PROP Bsc_Name "StdEditor.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "StdEditor - Win32 Release"
# Name "StdEditor - Win32 Debug"

!IF  "$(CFG)" == "StdEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "StdEditor - Win32 Debug"

!ENDIF 

# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cemaindrivers.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdEdAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\StdEditorDoc.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\cemaindrivers.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\StdEdAttr.h
# End Source File
# Begin Source File

SOURCE=..\inc\StdEditorDoc.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdEditor.inf
# End Source File
# End Target
# End Project
