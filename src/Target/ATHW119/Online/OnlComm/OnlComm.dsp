# Microsoft Developer Studio Project File - Name="OnlComm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OnlComm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OnlComm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OnlComm.mak" CFG="OnlComm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OnlComm - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OnlComm - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/AOReleased/ATHW119_V2.20.00/Online/OnlComm", PGSEAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OnlComm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f onlcomm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "onlcomm.exe"
# PROP BASE Bsc_Name "onlcomm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f onlcomm.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "onlcomm.exe"
# PROP Bsc_Name "onlcomm.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "OnlComm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f onlcomm.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "onlcomm.exe"
# PROP BASE Bsc_Name "onlcomm.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f onlcomm.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "onlcomm.exe"
# PROP Bsc_Name "onlcomm.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "OnlComm - Win32 Release"
# Name "OnlComm - Win32 Debug"

!IF  "$(CFG)" == "OnlComm - Win32 Release"

!ELSEIF  "$(CFG)" == "OnlComm - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OnlComm.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Version"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\res\Product.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=..\..\res\Version.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\TargetBase\inc\4CLayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CommPara.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\LogBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\LogLayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\LogSimple.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Phys232.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\PhysLayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\PhysSocket.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmShared.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\inc\vmSharedDef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\OnlComm.inf
# End Source File
# End Target
# End Project
