# Microsoft Developer Studio Project File - Name="PrgEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=PrgEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PrgEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrgEditor.mak" CFG="PrgEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrgEditor - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "PrgEditor - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/PrgEditor", ILPFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "PrgEditor - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f PrgEditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "PrgEditor.exe"
# PROP BASE Bsc_Name "PrgEditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f PrgEditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "PrgEditor.exe"
# PROP Bsc_Name "PrgEditor.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "PrgEditor - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f PrgEditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "PrgEditor.exe"
# PROP BASE Bsc_Name "PrgEditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f PrgEditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "PrgEditor.lib"
# PROP Bsc_Name "PrgEditor.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "PrgEditor - Win32 Release"
# Name "PrgEditor - Win32 Debug"

!IF  "$(CFG)" == "PrgEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "PrgEditor - Win32 Debug"

!ENDIF 

# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PrgEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PrgEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Symbols.cpp
# End Source File
# Begin Source File

SOURCE=.\Watchpoint.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\Breakpoint.h
# End Source File
# Begin Source File

SOURCE=..\inc\PrgEditorDoc.h
# End Source File
# Begin Source File

SOURCE=..\inc\PrgEditorView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\Symbols.h
# End Source File
# Begin Source File

SOURCE=..\inc\Watchpoint.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PrgEditor.inf
# End Source File
# End Target
# End Project
