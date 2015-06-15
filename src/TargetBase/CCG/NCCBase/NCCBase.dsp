# Microsoft Developer Studio Project File - Name="NCCBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=NCCBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NCCBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NCCBase.mak" CFG="NCCBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NCCBase - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "NCCBase - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/CCG/NCCBase", TWUFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "NCCBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f nccbase.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "nccbase.exe"
# PROP BASE Bsc_Name "nccbase.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f nccbase.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "nccbase.exe"
# PROP Bsc_Name "nccbase.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "NCCBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f nccbase.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "nccbase.exe"
# PROP BASE Bsc_Name "nccbase.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f nccbase.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "nccbase.exe"
# PROP Bsc_Name "nccbase.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "NCCBase - Win32 Release"
# Name "NCCBase - Win32 Debug"

!IF  "$(CFG)" == "NCCBase - Win32 Release"

!ELSEIF  "$(CFG)" == "NCCBase - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FormatException.cpp
# End Source File
# Begin Source File

SOURCE=.\getopt.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash.cpp
# End Source File
# Begin Source File

SOURCE=.\IpCode.cpp
# End Source File
# Begin Source File

SOURCE=.\NCCBase.cpp
# End Source File
# Begin Source File

SOURCE=.\NccCode.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\xxx_dbp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\Blob.h
# End Source File
# Begin Source File

SOURCE=..\inc\Codec.h
# End Source File
# Begin Source File

SOURCE=..\inc\FormatException.h
# End Source File
# Begin Source File

SOURCE=.\getopt.h
# End Source File
# Begin Source File

SOURCE=..\inc\Hash.h
# End Source File
# Begin Source File

SOURCE=..\inc\IpCode.h
# End Source File
# Begin Source File

SOURCE=..\inc\NccCode.h
# End Source File
# Begin Source File

SOURCE=..\inc\Parser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\xxx_dbp.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\NCCBase.inf
# End Source File
# End Target
# End Project
