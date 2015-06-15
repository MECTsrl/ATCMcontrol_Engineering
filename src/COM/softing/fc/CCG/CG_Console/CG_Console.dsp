# Microsoft Developer Studio Project File - Name="CG_Console" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CG_Console - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CG_Console.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CG_Console.mak" CFG="CG_Console - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CG_Console - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CG_Console - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/CG_Console", ONLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CG_Console - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f CG_Console.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CG_Console.exe"
# PROP BASE Bsc_Name "CG_Console.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f CG_Console.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CG_Console.exe"
# PROP Bsc_Name "CG_Console.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "CG_Console - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f CG_Console.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CG_Console.exe"
# PROP BASE Bsc_Name "CG_Console.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f CG_Console.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cg_console.dll"
# PROP Bsc_Name "cg_console.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "CG_Console - Win32 Release"
# Name "CG_Console - Win32 Debug"

!IF  "$(CFG)" == "CG_Console - Win32 Release"

!ELSEIF  "$(CFG)" == "CG_Console - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CG_CConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\CG_Console.def
# End Source File
# Begin Source File

SOURCE=.\CG_Console.inf
# End Source File
# Begin Source File

SOURCE=.\LoaderList.cpp
# End Source File
# Begin Source File

SOURCE=.\newdel.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "global_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\CG_Console.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CG_CConsole.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\LoaderList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# End Target
# End Project
