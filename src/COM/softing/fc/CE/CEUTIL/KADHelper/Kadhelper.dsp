# Microsoft Developer Studio Project File - Name="KADHelper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=KADHelper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Kadhelper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Kadhelper.mak" CFG="KADHelper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KADHelper - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "KADHelper - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEUTIL/KADHelper", CENFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "KADHelper - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f Kadhelper.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "Kadhelper.exe"
# PROP BASE Bsc_Name "Kadhelper.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f Kadhelper.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "Kadhelper.exe"
# PROP Bsc_Name "Kadhelper.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "KADHelper - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f Kadhelper.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "Kadhelper.exe"
# PROP BASE Bsc_Name "Kadhelper.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f Kadhelper.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "KADHelper.lib"
# PROP Bsc_Name "Kadhelper.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "KADHelper - Win32 Release"
# Name "KADHelper - Win32 Debug"

!IF  "$(CFG)" == "KADHelper - Win32 Release"

!ELSEIF  "$(CFG)" == "KADHelper - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\KADDragDropMap.cpp
# End Source File
# Begin Source File

SOURCE=.\KADFileExtInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KADFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KADFilterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KADFilterItem.cpp
# End Source File
# Begin Source File

SOURCE=.\KADInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KADMainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\KADMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\KADTargetData.cpp
# End Source File
# Begin Source File

SOURCE=.\KADTargetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KADToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\KADToolbarInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KADTypeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADDragDropMap.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADFileExtInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADFileInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADFilterInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADFilterItem.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADInfo.h
# End Source File
# Begin Source File

SOURCE=.\KADKeyCode.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADMainMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADTargetData.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADTargetInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADToolbar.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADToolbarInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\KADTypeInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\KADHelper\kso.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\KADHelper.inf
# End Source File
# End Target
# End Project
