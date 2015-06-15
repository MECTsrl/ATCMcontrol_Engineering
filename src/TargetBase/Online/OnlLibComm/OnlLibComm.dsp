# Microsoft Developer Studio Project File - Name="OnlLibComm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OnlLibComm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OnlLibComm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OnlLibComm.mak" CFG="OnlLibComm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OnlLibComm - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OnlLibComm - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/Online/OnlLibComm", YYWFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OnlLibComm - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f OnlLibComm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlLibComm.exe"
# PROP BASE Bsc_Name "OnlLibComm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f OnlLibComm.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OnlLibComm.exe"
# PROP Bsc_Name "OnlLibComm.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "OnlLibComm - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f OnlLibComm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlLibComm.exe"
# PROP BASE Bsc_Name "OnlLibComm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f OnlLibComm.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OnlLibComm.exe"
# PROP Bsc_Name "OnlLibComm.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "OnlLibComm - Win32 Release"
# Name "OnlLibComm - Win32 Debug"

!IF  "$(CFG)" == "OnlLibComm - Win32 Release"

!ELSEIF  "$(CFG)" == "OnlLibComm - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4CLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\LogBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LogLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\LogSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\Phys232.cpp
# End Source File
# Begin Source File

SOURCE=.\PhysLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\PhysSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\4CLayer.h
# End Source File
# Begin Source File

SOURCE=..\inc\CommPara.h
# End Source File
# Begin Source File

SOURCE=..\inc\LogBlock.h
# End Source File
# Begin Source File

SOURCE=..\inc\LogLayer.h
# End Source File
# Begin Source File

SOURCE=..\inc\LogSimple.h
# End Source File
# Begin Source File

SOURCE=..\inc\Phys232.h
# End Source File
# Begin Source File

SOURCE=..\inc\PhysLayer.h
# End Source File
# Begin Source File

SOURCE=..\inc\PhysSocket.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmShared.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmSharedDef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\OnlLibComm.inf
# End Source File
# End Target
# End Project
