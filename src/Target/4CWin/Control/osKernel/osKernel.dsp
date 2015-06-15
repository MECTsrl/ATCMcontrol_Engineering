# Microsoft Developer Studio Project File - Name="osKernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=osKernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "osKernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "osKernel.mak" CFG="osKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "osKernel - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "osKernel - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/Target/4CWin/Control/osKernel", RFXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "osKernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f osKernel.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "osKernel.exe"
# PROP BASE Bsc_Name "osKernel.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f osKernel.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "osKernel.exe"
# PROP Bsc_Name "osKernel.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "osKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f osKernel.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "osKernel.exe"
# PROP BASE Bsc_Name "osKernel.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f osKernel.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "osKernel.exe"
# PROP Bsc_Name "osKernel.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "osKernel - Win32 Release"
# Name "osKernel - Win32 Debug"

!IF  "$(CFG)" == "osKernel - Win32 Release"

!ELSEIF  "$(CFG)" == "osKernel - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\osCom.c
# End Source File
# Begin Source File

SOURCE=.\osFieldB.c
# End Source File
# Begin Source File

SOURCE=.\osFile.c
# End Source File
# Begin Source File

SOURCE=.\osFlash.c
# End Source File
# Begin Source File

SOURCE=.\osLib.c
# End Source File
# Begin Source File

SOURCE=.\osMain.c
# End Source File
# Begin Source File

SOURCE=.\osMBus2.c
# End Source File
# Begin Source File

SOURCE=.\osTask.c
# End Source File
# Begin Source File

SOURCE=.\winAdapt.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\res\BuildNr.h
# End Source File
# Begin Source File

SOURCE=..\inc\intDef.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\intOpcds.h
# End Source File
# Begin Source File

SOURCE=..\inc\libDef.h
# End Source File
# Begin Source File

SOURCE=..\inc\osAlign.h
# End Source File
# Begin Source File

SOURCE=..\inc\osDef.h
# End Source File
# Begin Source File

SOURCE=..\inc\osFile.h
# End Source File
# Begin Source File

SOURCE=..\inc\osFirst.h
# End Source File
# Begin Source File

SOURCE=..\inc\osLast.h
# End Source File
# Begin Source File

SOURCE=..\inc\osMBus2.h
# End Source File
# Begin Source File

SOURCE=..\inc\osSocket.h
# End Source File
# Begin Source File

SOURCE=..\inc\osTarget.h
# End Source File
# Begin Source File

SOURCE=..\inc\stdInc.h
# End Source File
# Begin Source File

SOURCE=..\inc\vmmDef.h
# End Source File
# Begin Source File

SOURCE=..\inc\vmmMain.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmShared.h
# End Source File
# Begin Source File

SOURCE=.\winAdapt.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\osKernel.inf
# End Source File
# End Target
# End Project
