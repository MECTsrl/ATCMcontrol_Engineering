# Microsoft Developer Studio Project File - Name="CEUTIL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEUTIL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEUTIL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEUTIL.mak" CFG="CEUTIL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEUTIL - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CEUTIL - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEUTIL", LDNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEUTIL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f ceutil.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceutil.exe"
# PROP BASE Bsc_Name "ceutil.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f ceutil.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceutil.exe"
# PROP Bsc_Name "ceutil.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "CEUTIL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f ceutil.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceutil.exe"
# PROP BASE Bsc_Name "ceutil.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f ceutil.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceutil.exe"
# PROP Bsc_Name "ceutil.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "CEUTIL - Win32 Release"
# Name "CEUTIL - Win32 Debug"

!IF  "$(CFG)" == "CEUTIL - Win32 Release"

!ELSEIF  "$(CFG)" == "CEUTIL - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CEComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CEEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\CEUTIL.rc
# End Source File
# Begin Source File

SOURCE=.\DirectoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawObject.cpp
# End Source File
# Begin Source File

SOURCE=.\fcdatatip.cpp
# End Source File
# Begin Source File

SOURCE=.\SizedControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SizedDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\CEComboBox.h
# End Source File
# Begin Source File

SOURCE=..\inc\CEEdit.h
# End Source File
# Begin Source File

SOURCE=..\inc\CEUTILrc.h
# End Source File
# Begin Source File

SOURCE=..\inc\DirectoryDlg.h
# End Source File
# Begin Source File

SOURCE=..\inc\DrawObject.h
# End Source File
# Begin Source File

SOURCE=..\inc\fcdatatip.h
# End Source File
# Begin Source File

SOURCE=..\inc\SizedControlBar.h
# End Source File
# Begin Source File

SOURCE=..\inc\SizedDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CEUtil.inf
# End Source File
# End Target
# End Project
