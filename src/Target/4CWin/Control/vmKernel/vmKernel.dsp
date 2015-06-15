# Microsoft Developer Studio Project File - Name="vmKernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=vmKernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vmKernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vmKernel.mak" CFG="vmKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vmKernel - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "vmKernel - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/Target/4CWin/Control/vmKernel", UFXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "vmKernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f vmKernel.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "vmKernel.exe"
# PROP BASE Bsc_Name "vmKernel.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /nologo /f vmKernel.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "vmKernel.exe"
# PROP Bsc_Name "vmKernel.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "vmKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f vmKernel.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "vmKernel.exe"
# PROP BASE Bsc_Name "vmKernel.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /nologo /f vmKernel.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "vmKernel.exe"
# PROP Bsc_Name "vmKernel.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "vmKernel - Win32 Release"
# Name "vmKernel - Win32 Debug"

!IF  "$(CFG)" == "vmKernel - Win32 Release"

!ELSEIF  "$(CFG)" == "vmKernel - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\actDebug.c
# End Source File
# Begin Source File

SOURCE=.\actDFull.c
# End Source File
# Begin Source File

SOURCE=.\actDIncr.c
# End Source File
# Begin Source File

SOURCE=.\actDown.c
# End Source File
# Begin Source File

SOURCE=.\actEtc.c
# End Source File
# Begin Source File

SOURCE=.\actFile.c
# End Source File
# Begin Source File

SOURCE=.\dbiMain.c
# End Source File
# Begin Source File

SOURCE=.\fileMain.c
# End Source File
# Begin Source File

SOURCE=.\intInter.c
# End Source File
# Begin Source File

SOURCE=.\intLevel.c
# End Source File
# Begin Source File

SOURCE=.\intMain.c
# End Source File
# Begin Source File

SOURCE=.\ioMain.c
# End Source File
# Begin Source File

SOURCE=.\md5.c
# End Source File
# Begin Source File

SOURCE=.\vmmAct.c
# End Source File
# Begin Source File

SOURCE=.\vmMain.c
# End Source File
# Begin Source File

SOURCE=.\vmmBreak.c
# End Source File
# Begin Source File

SOURCE=.\vmmCmd.c
# End Source File
# Begin Source File

SOURCE=.\vmmCom.c
# End Source File
# Begin Source File

SOURCE=.\vmmLoad.c
# End Source File
# Begin Source File

SOURCE=.\vmmMain.c
# End Source File
# Begin Source File

SOURCE=.\vmmMsg.c
# End Source File
# Begin Source File

SOURCE=.\vmmOnlCh.c
# End Source File
# Begin Source File

SOURCE=.\vmmRet.c
# End Source File
# Begin Source File

SOURCE=.\vmmSys.c
# End Source File
# Begin Source File

SOURCE=.\vmmUtil.c
# End Source File
# Begin Source File

SOURCE=.\vmPrc.c
# End Source File
# Begin Source File

SOURCE=.\vmTimer.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=..\inc\md5.h
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

SOURCE=.\vmmSys.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmShared.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\vmKernel.inf
# End Source File
# End Target
# End Project
