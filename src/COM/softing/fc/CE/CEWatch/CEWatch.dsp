# Microsoft Developer Studio Project File - Name="CEWatch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEWatch - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEWatch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEWatch.mak" CFG="CEWatch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEWatch - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CEWatch - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEWatch", JFNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEWatch - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f cewatch.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cewatch.exe"
# PROP BASE Bsc_Name "cewatch.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f cewatch.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cewatch.exe"
# PROP Bsc_Name "cewatch.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "CEWatch - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f cewatch.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cewatch.exe"
# PROP BASE Bsc_Name "cewatch.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f cewatch.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cewatch.exe"
# PROP Bsc_Name "cewatch.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "CEWatch - Win32 Release"
# Name "CEWatch - Win32 Debug"

!IF  "$(CFG)" == "CEWatch - Win32 Release"

!ELSEIF  "$(CFG)" == "CEWatch - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\Watch.rc
# End Source File
# Begin Source File

SOURCE=.\WatchCtrlParent.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchCtrlWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchRenameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WatchCtrlParent.h
# End Source File
# Begin Source File

SOURCE=.\WatchCtrlWrapper.h
# End Source File
# Begin Source File

SOURCE=..\inc\WatchRC.h
# End Source File
# Begin Source File

SOURCE=.\WatchRenameDlg.h
# End Source File
# Begin Source File

SOURCE=.\WatchTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\WatchTabWnd.h
# End Source File
# Begin Source File

SOURCE=..\inc\WatchWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CEWatch.inf
# End Source File
# End Target
# End Project
