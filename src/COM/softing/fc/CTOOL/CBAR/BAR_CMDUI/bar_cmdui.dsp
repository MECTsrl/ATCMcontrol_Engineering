# Microsoft Developer Studio Project File - Name="BAR_CMDUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=BAR_CMDUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bar_cmdui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bar_cmdui.mak" CFG="BAR_CMDUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BAR_CMDUI - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "BAR_CMDUI - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_CMDUI", GKTFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "BAR_CMDUI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f bar_cmdui.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "bar_cmdui.exe"
# PROP BASE Bsc_Name "bar_cmdui.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f bar_cmdui.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "bar_cmdui.exe"
# PROP Bsc_Name "bar_cmdui.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "BAR_CMDUI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f bar_cmdui.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "bar_cmdui.exe"
# PROP BASE Bsc_Name "bar_cmdui.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f bar_cmdui.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "bar_cmdui.exe"
# PROP Bsc_Name "bar_cmdui.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "BAR_CMDUI - Win32 Release"
# Name "BAR_CMDUI - Win32 Debug"

!IF  "$(CFG)" == "BAR_CMDUI - Win32 Release"

!ELSEIF  "$(CFG)" == "BAR_CMDUI - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BAR_CMDUI_Args.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_CMDUI_Callback.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_CMDUI_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv_tlb.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BAR_CMDUI_Args.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_CMDUI_Callback.hpp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\BAR_CMDUI.inf
# End Source File
# Begin Source File

SOURCE=..\..\inc\CSC_OnlineServer.tlb
# End Source File
# End Target
# End Project
