# Microsoft Developer Studio Project File - Name="BAR_LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=BAR_LIB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bar_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bar_lib.mak" CFG="BAR_LIB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BAR_LIB - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "BAR_LIB - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB", TKTFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "BAR_LIB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f bar_lib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "bar_lib.exe"
# PROP BASE Bsc_Name "bar_lib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f bar_lib.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "bar_lib.exe"
# PROP Bsc_Name "bar_lib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "BAR_LIB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f bar_lib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "bar_lib.exe"
# PROP BASE Bsc_Name "bar_lib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f bar_lib.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "bar_lib.exe"
# PROP Bsc_Name "bar_lib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "BAR_LIB - Win32 Release"
# Name "BAR_LIB - Win32 Debug"

!IF  "$(CFG)" == "BAR_LIB - Win32 Release"

!ELSEIF  "$(CFG)" == "BAR_LIB - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BAR_Arc.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_ArcBackup.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_ArcRestore.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Callback.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_config.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_CSCOnlineProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_DomainList.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Error.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_FileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Project.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_TypeLib.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Util.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Warning.cpp
# End Source File
# Begin Source File

SOURCE=.\BAR_ZIP.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BAR_Arc.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_ArcBackup.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_ArcRestore.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Callback.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Config.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_CSCOnlineProxy.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_DomainList.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Error.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_FileInfo.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Project.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Trace.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_TypeLib.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Util.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_Warning.hpp
# End Source File
# Begin Source File

SOURCE=.\BAR_ZIP.hpp
# End Source File
# End Group
# Begin Group "libs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lib\bzip2.lib
# End Source File
# Begin Source File

SOURCE=..\lib\BAR.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\BAR_LIB.inf
# End Source File
# End Target
# End Project
