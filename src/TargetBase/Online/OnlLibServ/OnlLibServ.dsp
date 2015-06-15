# Microsoft Developer Studio Project File - Name="OnlLibServ" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OnlLibServ - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OnlLibServ.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OnlLibServ.mak" CFG="OnlLibServ - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OnlLibServ - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OnlLibServ - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/TargetBase/Online/OnlLibServ", KAXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OnlLibServ - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f OnlLibServ.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlLibServ.exe"
# PROP BASE Bsc_Name "OnlLibServ.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f OnlLibServ.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "C:\4c\ENGINE~1\bin\4CWin\winscs.exe"
# PROP Bsc_Name "OnlLibServ.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "OnlLibServ - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f OnlLibServ.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlLibServ.exe"
# PROP BASE Bsc_Name "OnlLibServ.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f OnlLibServ.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Bsc_Name "OnlLibServ.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "OnlLibServ - Win32 Release"
# Name "OnlLibServ - Win32 Debug"

!IF  "$(CFG)" == "OnlLibServ - Win32 Release"

!ELSEIF  "$(CFG)" == "OnlLibServ - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CommissioningMgt.cpp
# End Source File
# Begin Source File

SOURCE=.\CommissioningSession.cpp
# End Source File
# Begin Source File

SOURCE=.\CommObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\conResource.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlMgt.cpp
# End Source File
# Begin Source File

SOURCE=.\Creator.cpp
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\dbiConv.cpp
# End Source File
# Begin Source File

SOURCE=.\iecConfiguration.cpp
# End Source File
# Begin Source File

SOURCE=.\iecResource.cpp
# End Source File
# Begin Source File

SOURCE=.\iecVar.cpp
# End Source File
# Begin Source File

SOURCE=.\NSpaceHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineChangesHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlServError.cpp
# End Source File
# Begin Source File

SOURCE=.\opcDynGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\opcDynServer.cpp
# End Source File
# Begin Source File

SOURCE=.\opcItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Security.cpp
# End Source File
# Begin Source File

SOURCE=.\srtResource.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\SysVar.cpp
# End Source File
# Begin Source File

SOURCE=.\SysVarPool.cpp
# End Source File
# Begin Source File

SOURCE=.\Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\VisTransferLocal.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\CommissioningMgt.h
# End Source File
# Begin Source File

SOURCE=..\inc\CommissioningSession.h
# End Source File
# Begin Source File

SOURCE=..\inc\Config.h
# End Source File
# Begin Source File

SOURCE=..\inc\Connection.h
# End Source File
# Begin Source File

SOURCE=..\inc\conResource.h
# End Source File
# Begin Source File

SOURCE=..\inc\ControlMgt.h
# End Source File
# Begin Source File

SOURCE=..\inc\CpBreakpointNotification.h
# End Source File
# Begin Source File

SOURCE=..\inc\CpCommissioningMgtNotification.h
# End Source File
# Begin Source File

SOURCE=..\inc\CpSimpleMsgNotification.h
# End Source File
# Begin Source File

SOURCE=..\inc\Creator.h
# End Source File
# Begin Source File

SOURCE=..\inc\CSC_OnlineErr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\COM\softing\fc\res\csc_onlineerr.mc
# End Source File
# Begin Source File

SOURCE=..\inc\CustomConnect.h
# End Source File
# Begin Source File

SOURCE=..\inc\dbiConv.h
# End Source File
# Begin Source File

SOURCE=..\inc\ExeModule.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=..\inc\iecConfiguration.h
# End Source File
# Begin Source File

SOURCE=..\inc\iecResource.h
# End Source File
# Begin Source File

SOURCE=..\inc\iecVar.h
# End Source File
# Begin Source File

SOURCE=..\inc\NSpaceHolder.h
# End Source File
# Begin Source File

SOURCE=..\inc\OnlineChangesHelper.h
# End Source File
# Begin Source File

SOURCE=.\OnlServError.h
# End Source File
# Begin Source File

SOURCE=..\inc\opcDynGroup.h
# End Source File
# Begin Source File

SOURCE=..\inc\opcDynServer.h
# End Source File
# Begin Source File

SOURCE=..\inc\opcItem.h
# End Source File
# Begin Source File

SOURCE=..\inc\Resource.h
# End Source File
# Begin Source File

SOURCE=..\inc\Security.h
# End Source File
# Begin Source File

SOURCE=..\inc\srtResource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\SysVar.h
# End Source File
# Begin Source File

SOURCE=..\inc\SysVarPool.h
# End Source File
# Begin Source File

SOURCE=..\inc\Trace.h
# End Source File
# Begin Source File

SOURCE=..\inc\VisTransferLocal.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmShared.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\vmSharedDef.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\COM\softing\fc\inc\CSC_OnlineServer.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\OnlLibServ.inf
# End Source File
# End Target
# End Project
