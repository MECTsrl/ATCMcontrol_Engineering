# Microsoft Developer Studio Project File - Name="visLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=visLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "visLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "visLib.mak" CFG="visLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "visLib - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "visLib - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/Control/visLib", CTXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "visLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f visLib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "visLib.exe"
# PROP BASE Bsc_Name "visLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /nologo /f visLib.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "visLib.exe"
# PROP Bsc_Name "visLib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "visLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f visLib.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "visLib.exe"
# PROP BASE Bsc_Name "visLib.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /nologo /f visLib.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "visLib.exe"
# PROP Bsc_Name "visLib.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "visLib - Win32 Release"
# Name "visLib - Win32 Debug"

!IF  "$(CFG)" == "visLib - Win32 Release"

!ELSEIF  "$(CFG)" == "visLib - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\visAdapt.c
# End Source File
# Begin Source File

SOURCE=.\visCmd.c
# End Source File
# Begin Source File

SOURCE=.\visCom.c
# End Source File
# Begin Source File

SOURCE=.\visExt.c
# End Source File
# Begin Source File

SOURCE=.\visLic.c
# End Source File
# Begin Source File

SOURCE=.\visUtil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc.vis\visDef.h
# End Source File
# Begin Source File

SOURCE=..\inc.vis\visMain.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\visShared.h
# End Source File
# End Group
# Begin Group "Control Inc"

# PROP Default_Filter ""
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
# End Group
# Begin Source File

SOURCE=.\visLib.inf
# End Source File
# End Target
# End Project
