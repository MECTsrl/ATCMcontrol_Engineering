# Microsoft Developer Studio Project File - Name="DragDropHelper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=DragDropHelper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DragDropHelper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DragDropHelper.mak" CFG="DragDropHelper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DragDropHelper - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "DragDropHelper - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEUTIL/DragDropHelper", RDNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "DragDropHelper - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f DragDropHelper.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "DragDropHelper.exe"
# PROP BASE Bsc_Name "DragDropHelper.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f DragDropHelper.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "DragDropHelper.exe"
# PROP Bsc_Name "DragDropHelper.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "DragDropHelper - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f DragDropHelper.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "DragDropHelper.exe"
# PROP BASE Bsc_Name "DragDropHelper.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f DragDropHelper.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEUTIL_DragDrop.lib"
# PROP Bsc_Name "DragDropHelper.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "DragDropHelper - Win32 Release"
# Name "DragDropHelper - Win32 Debug"

!IF  "$(CFG)" == "DragDropHelper - Win32 Release"

!ELSEIF  "$(CFG)" == "DragDropHelper - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Dragitem.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PouVarObject.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\inc\Dragitem.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\DragDropHelper\LinkFile.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\DragDropHelper\PouVarDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\DragDropHelper\PouVarObject.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DragDropHelper.inf
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
