# Microsoft Developer Studio Project File - Name="fc_todebug" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=fc_todebug - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fc_todebug.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fc_todebug.mak" CFG="fc_todebug - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fc_todebug - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "fc_todebug - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug", NPRFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "fc_todebug - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f fc_todebug.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_todebug.exe"
# PROP BASE Bsc_Name "fc_todebug.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f fc_todebug.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_todebug.exe"
# PROP Bsc_Name "fc_todebug.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "fc_todebug - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f fc_todebug.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "fc_todebug.exe"
# PROP BASE Bsc_Name "fc_todebug.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f fc_todebug.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "fc_todebug.exe"
# PROP Bsc_Name "fc_todebug.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "fc_todebug - Win32 Release"
# Name "fc_todebug - Win32 Debug"

!IF  "$(CFG)" == "fc_todebug - Win32 Release"

!ELSEIF  "$(CFG)" == "fc_todebug - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fc_assert.c
# End Source File
# Begin Source File

SOURCE=.\fc_mem.c
# End Source File
# Begin Source File

SOURCE=.\fc_todebug.inf
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "inc\fc_todebug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\fc_todebug\fc_assert.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\fc_todebug\fc_mem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\fc_mem_l.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\fc_todebug.rc
# End Source File
# End Group
# End Target
# End Project
