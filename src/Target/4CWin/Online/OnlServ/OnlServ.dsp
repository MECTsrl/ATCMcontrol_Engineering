# Microsoft Developer Studio Project File - Name="OnlServ" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OnlServ - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OnlServ.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OnlServ.mak" CFG="OnlServ - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OnlServ - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "OnlServ - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/Target/4CWin/Online/OnlServ", VRXFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OnlServ - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OnlServ___Win32_Debug"
# PROP BASE Intermediate_Dir "OnlServ___Win32_Debug"
# PROP BASE Cmd_Line "NMAKE /f OnlServ.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlServ.exe"
# PROP BASE Bsc_Name "OnlServ.bsc"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "OnlServ___Win32_Debug"
# PROP Intermediate_Dir "OnlServ___Win32_Debug"
# PROP Cmd_Line "NMAKE /f OnlServ.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OnlServ.exe"
# PROP Bsc_Name "OnlServ.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\OnlServ___Win32_Debug
TargetPath=.\OnlServ.exe
InputPath=.\OnlServ.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "OnlServ - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OnlServ___Win32_Release"
# PROP BASE Intermediate_Dir "OnlServ___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f OnlServ.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "OnlServ.exe"
# PROP BASE Bsc_Name "OnlServ.bsc"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "OnlServ___Win32_Release"
# PROP Intermediate_Dir "OnlServ___Win32_Release"
# PROP Cmd_Line "NMAKE /f OnlServ.mak /nologo DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "OnlServ.exe"
# PROP Bsc_Name "OnlServ.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\OnlServ___Win32_Release
TargetPath=.\OnlServ.exe
InputPath=.\OnlServ.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "OnlServ - Win32 Debug"
# Name "OnlServ - Win32 Release"

!IF  "$(CFG)" == "OnlServ - Win32 Debug"

!ELSEIF  "$(CFG)" == "OnlServ - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlServ.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\TargetBase\inc\4CLayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CommissioningMgt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CommissioningSession.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Connection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\conResource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\ControlMgt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CpBreakpointNotification.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CpCommissioningMgtNotification.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CpSimpleMsgNotification.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Creator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CSC_OnlineErr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\CustomConnect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\dbiConv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\ExeModule.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\iecConfiguration.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\iecResource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\iecVar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\NSpaceHolder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\opcDynGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\opcDynServer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\opcItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Security.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\srtResource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\SysVar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\SysVarPool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\Trace.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Online\inc\VisTransferLocal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\inc\vmShared.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\inc\vmSharedDef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CommissioningMgt.rgs
# End Source File
# Begin Source File

SOURCE=.\ControlMgt.rgs
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Kernel\res\csc_onlineerr.mc
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.rc
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.rgs
# End Source File
# Begin Source File

SOURCE=.\CustomConnect.rgs
# End Source File
# Begin Source File

SOURCE=.\dummy.rgs
# End Source File
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Kernel\res\opcerror.mc
# End Source File
# Begin Source File

SOURCE=.\OPCServer.rgs
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.idl;*.odl"
# Begin Source File

SOURCE=..\..\..\..\TargetBase\Kernel\inc\CSC_OnlineServer.idl
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\OnlServ.inf
# End Source File
# Begin Source File

SOURCE=.\who.list
# End Source File
# End Target
# End Project
