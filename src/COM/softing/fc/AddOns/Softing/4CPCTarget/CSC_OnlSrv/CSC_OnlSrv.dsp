# Microsoft Developer Studio Project File - Name="CSC_OnlSrv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CSC_ONLSRV - WIN32 UNICODE RELEASE MINDEPENDENCY
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CSC_OnlSrv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CSC_OnlSrv.mak" CFG="CSC_ONLSRV - WIN32 UNICODE RELEASE MINDEPENDENCY"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CSC_OnlSrv - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CSC_OnlSrv - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/AddOns/Softing/4CPCTarget/CSC_OnlSrv", LAIFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CSC_OnlSrv - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f CSC_OnlSrv.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_OnlSrv.exe"
# PROP BASE Bsc_Name "CSC_OnlSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DObjU"
# PROP Cmd_Line "NMAKE /f CSC_OnlSrv.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "4CSCS.exe"
# PROP Bsc_Name "CSC_OnlSrv.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\4CSCS.exe
InputPath=.\4CSCS.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_OnlSrv - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CSC_OnlSrv.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_OnlSrv.exe"
# PROP BASE Bsc_Name "CSC_OnlSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "RObjU"
# PROP Cmd_Line "NMAKE /f CSC_OnlSrv.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_OnlSrv.exe"
# PROP Bsc_Name "CSC_OnlSrv.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseU
TargetPath=.\CSC_OnlSrv.exe
InputPath=.\CSC_OnlSrv.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CSC_OnlSrv - Win32 Unicode Debug"
# Name "CSC_OnlSrv - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "CSC_OnlSrv - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CSC_OnlSrv - Win32 Unicode Release MinDependency"

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

SOURCE=.\Globals.cpp
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

SOURCE=.\VisTransferFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\VisTransferLocal.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\inc\catids.h
# End Source File
# Begin Source File

SOURCE=.\CommissioningMgt.h
# End Source File
# Begin Source File

SOURCE=.\CommissioningSession.h
# End Source File
# Begin Source File

SOURCE=.\conResource.h
# End Source File
# Begin Source File

SOURCE=.\ControlMgt.h
# End Source File
# Begin Source File

SOURCE=.\CpBreakpointNotification.h
# End Source File
# Begin Source File

SOURCE=.\CpCommissioningMgtNotification.h
# End Source File
# Begin Source File

SOURCE=.\CpSimpleMsgNotification.h
# End Source File
# Begin Source File

SOURCE=.\Creator.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\iecConfiguration.h
# End Source File
# Begin Source File

SOURCE=.\iecResource.h
# End Source File
# Begin Source File

SOURCE=.\iecVar.h
# End Source File
# Begin Source File

SOURCE=.\NSpaceHolder.h
# End Source File
# Begin Source File

SOURCE=.\opcDynGroup.h
# End Source File
# Begin Source File

SOURCE=.\opcDynServer.h
# End Source File
# Begin Source File

SOURCE=.\opcItem.h
# End Source File
# Begin Source File

SOURCE=.\Security.h
# End Source File
# Begin Source File

SOURCE=.\srtResource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysVar.h
# End Source File
# Begin Source File

SOURCE=.\SysVarPool.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\VisTransferFTP.h
# End Source File
# Begin Source File

SOURCE=.\VisTransferLocal.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\..\res\BuildNr.h
# End Source File
# Begin Source File

SOURCE=.\CommissioningMgt.rgs
# End Source File
# Begin Source File

SOURCE=.\ControlMgt.rgs
# End Source File
# Begin Source File

SOURCE=..\..\..\..\res\csc_onlineerr.mc
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.rc
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.rgs
# End Source File
# Begin Source File

SOURCE=..\..\..\..\res\opcerror.mc
# End Source File
# Begin Source File

SOURCE=.\OPCServer.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.idl;*.odl"
# Begin Source File

SOURCE=..\..\..\..\inc\CSC_OnlineServer.idl
# End Source File
# Begin Source File

SOURCE=..\inc\CSC_OnlSrv.idl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\inc\device.odl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\inc\opc\opcDA.idl
# End Source File
# End Group
# Begin Group "Doku"

# PROP Default_Filter "*.htm;*.doc"
# Begin Source File

SOURCE=\\S_4C\4CONTROL\Projekt\SWE\4CSC_OPCSystemVars_Arch.htm
# End Source File
# End Group
# Begin Source File

SOURCE=.\CSC_OnlSrv.doxy
# End Source File
# Begin Source File

SOURCE=.\CSC_OnlSrv.inf
# End Source File
# Begin Source File

SOURCE=.\who.list
# End Source File
# End Target
# End Project
