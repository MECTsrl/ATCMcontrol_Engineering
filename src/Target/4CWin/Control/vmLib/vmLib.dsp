# Microsoft Developer Studio Project File - Name="vmLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=vmLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vmLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vmLib.mak" CFG="vmLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vmLib - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "vmLib - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/Target/4CWin/Control/vmLib", VFXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "vmLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f vmLib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "vmLib.exe"
# PROP BASE Bsc_Name "vmLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f vmLib.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "vmLib.exe"
# PROP Bsc_Name "vmLib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "vmLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f vmLib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "vmLib.exe"
# PROP BASE Bsc_Name "vmLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f vmLib.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "vmLib.exe"
# PROP Bsc_Name "vmLib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "vmLib - Win32 Release"
# Name "vmLib - Win32 Debug"

!IF  "$(CFG)" == "vmLib - Win32 Release"

!ELSEIF  "$(CFG)" == "vmLib - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\libDef.c
# End Source File
# Begin Source File

SOURCE=.\libFile.c
# End Source File
# Begin Source File

SOURCE=.\libIec.c
# End Source File
# Begin Source File

SOURCE=.\libMBus2.c
# End Source File
# Begin Source File

SOURCE=.\libSfc.c
# End Source File
# Begin Source File

SOURCE=.\libSys.c
# End Source File
# Begin Source File

SOURCE=.\libSys2.c
# End Source File
# Begin Source File

SOURCE=.\libUtil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\fbFile.h
# End Source File
# Begin Source File

SOURCE=.\fbIec.h
# End Source File
# Begin Source File

SOURCE=.\fbMBus2.h
# End Source File
# Begin Source File

SOURCE=.\fbSys.h
# End Source File
# Begin Source File

SOURCE=.\fbSys2.h
# End Source File
# Begin Source File

SOURCE=.\fbUtil.h
# End Source File
# Begin Source File

SOURCE=.\funBac.h
# End Source File
# Begin Source File

SOURCE=.\funFile.h
# End Source File
# Begin Source File

SOURCE=.\funIec.h
# End Source File
# Begin Source File

SOURCE=.\funMBus2.h
# End Source File
# Begin Source File

SOURCE=.\funSys.h
# End Source File
# Begin Source File

SOURCE=.\funSys2.h
# End Source File
# Begin Source File

SOURCE=.\funSys22.h
# End Source File
# Begin Source File

SOURCE=.\funUtil.h
# End Source File
# Begin Source File

SOURCE=.\libFile.h
# End Source File
# Begin Source File

SOURCE=.\libIec.h
# End Source File
# Begin Source File

SOURCE=..\inc\libMBus2.h
# End Source File
# Begin Source File

SOURCE=.\libSfc.h
# End Source File
# Begin Source File

SOURCE=.\libSys.h
# End Source File
# Begin Source File

SOURCE=.\libSys2.h
# End Source File
# Begin Source File

SOURCE=.\libUtil.h
# End Source File
# Begin Source File

SOURCE=..\inc\osMBus2.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\vmLib.inf
# End Source File
# End Target
# End Project
