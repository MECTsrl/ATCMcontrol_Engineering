# Microsoft Developer Studio Project File - Name="Deploy" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Deploy - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "deploy.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "deploy.mak" CFG="Deploy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Deploy - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "Deploy - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CTOOL/Deploy", APTFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Deploy - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f deploy.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "deploy.exe"
# PROP BASE Bsc_Name "deploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f deploy.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "deploy.exe"
# PROP Bsc_Name "deploy.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Deploy - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f deploy.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "deploy.exe"
# PROP BASE Bsc_Name "deploy.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f deploy.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "deploy.exe"
# PROP Bsc_Name "deploy.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Deploy - Win32 Release"
# Name "Deploy - Win32 Debug"

!IF  "$(CFG)" == "Deploy - Win32 Release"

!ELSEIF  "$(CFG)" == "Deploy - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Deploy.cpp
# End Source File
# Begin Source File

SOURCE=.\Deploy.rc
# End Source File
# Begin Source File

SOURCE=.\ProjectPatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\defs.h
# End Source File
# Begin Source File

SOURCE=.\Deploy.h
# End Source File
# Begin Source File

SOURCE=.\ProjectPatcher.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\Deploy.inf
# End Source File
# End Target
# End Project
