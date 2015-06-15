# Microsoft Developer Studio Project File - Name="opcComn_PS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=opcComn_PS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OPCCOMN_PS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OPCCOMN_PS.mak" CFG="opcComn_PS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "opcComn_PS - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "opcComn_PS - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CSC/OPCCOMN_PS", JNSFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "opcComn_PS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f OPCCOMN_PS.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OPCCOMN_PS.exe"
# PROP BASE Bsc_Name "OPCCOMN_PS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake /f OPCCOMN_PS.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OPCCOMN_PS.DLL"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "opcComn_PS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f OPCCOMN_PS.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "OPCCOMN_PS.exe"
# PROP BASE Bsc_Name "OPCCOMN_PS.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake /f OPCCOMN_PS.mak DEBUG=1"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcComn_PS.DLL"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "opcComn_PS - Win32 Release"
# Name "opcComn_PS - Win32 Debug"

!IF  "$(CFG)" == "opcComn_PS - Win32 Release"

!ELSEIF  "$(CFG)" == "opcComn_PS - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=.\dlldata.c
# End Source File
# Begin Source File

SOURCE=..\..\inc\opc\opcComn.idl
# End Source File
# Begin Source File

SOURCE=.\opcComn.tlb
# End Source File
# Begin Source File

SOURCE=.\opccomn_i.c
# End Source File
# Begin Source File

SOURCE=.\opccomn_p.c
# End Source File
# Begin Source File

SOURCE=.\OPCCOMN_PS.def
# End Source File
# Begin Source File

SOURCE=.\OPCCOMN_PS.inf
# End Source File
# Begin Source File

SOURCE=.\OPCCOMN_PS.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Target
# End Project
