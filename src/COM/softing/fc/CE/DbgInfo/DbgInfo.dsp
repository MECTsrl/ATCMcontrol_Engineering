# Microsoft Developer Studio Project File - Name="DbgInfo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=DbgInfo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DbgInfo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DbgInfo.mak" CFG="DbgInfo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DbgInfo - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "DbgInfo - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "DbgInfo - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "DbgInfo - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/DbgInfo", BONFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "DbgInfo - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f dbginfo.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "dbginfo.exe"
# PROP BASE Bsc_Name "dbginfo.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "DObj"
# PROP Cmd_Line "NMAKE /f dbginfo.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "dbginfo.exe"
# PROP Bsc_Name "dbginfo.bsc"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\dbginfo.exe
InputPath=.\dbginfo.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "DbgInfo - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f dbginfo.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "dbginfo.exe"
# PROP BASE Bsc_Name "dbginfo.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DObjU"
# PROP Cmd_Line "NMAKE /f dbginfo.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "dbginfo.exe"
# PROP Bsc_Name "dbginfo.bsc"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\dbginfo.exe
InputPath=.\dbginfo.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "DbgInfo - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f dbginfo.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "dbginfo.exe"
# PROP BASE Bsc_Name "dbginfo.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "RObj"
# PROP Cmd_Line "NMAKE /f dbginfo.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "dbginfo.exe"
# PROP Bsc_Name "dbginfo.bsc"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\dbginfo.exe
InputPath=.\dbginfo.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "DbgInfo - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f dbginfo.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "dbginfo.exe"
# PROP BASE Bsc_Name "dbginfo.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "RObjU"
# PROP Cmd_Line "NMAKE /f dbginfo.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "dbginfo.exe"
# PROP Bsc_Name "dbginfo.bsc"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseU
TargetPath=.\dbginfo.exe
InputPath=.\dbginfo.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "DbgInfo - Win32 Debug"
# Name "DbgInfo - Win32 Unicode Debug"
# Name "DbgInfo - Win32 Release MinDependency"
# Name "DbgInfo - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "DbgInfo - Win32 Debug"

!ELSEIF  "$(CFG)" == "DbgInfo - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "DbgInfo - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "DbgInfo - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActionDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\AnyDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\BreakpointDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\DbgInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DbgInfo.def
# End Source File
# Begin Source File

SOURCE=.\DBIContext.cpp
# End Source File
# Begin Source File

SOURCE=.\DBIHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugInfoServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GvlFileDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StepDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\TransitionDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\VariableDBI.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActionDBI.h
# End Source File
# Begin Source File

SOURCE=.\AnyDBI.h
# End Source File
# Begin Source File

SOURCE=.\BreakpointDBI.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDBI.h
# End Source File
# Begin Source File

SOURCE=.\DbgInfoCP.h
# End Source File
# Begin Source File

SOURCE=.\DBIContext.h
# End Source File
# Begin Source File

SOURCE=.\DBIHolder.h
# End Source File
# Begin Source File

SOURCE=.\DebugInfoServer.h
# End Source File
# Begin Source File

SOURCE=.\GvlFileDBI.h
# End Source File
# Begin Source File

SOURCE=.\InstanceDBI.h
# End Source File
# Begin Source File

SOURCE=.\ProjectDBI.h
# End Source File
# Begin Source File

SOURCE=.\ResourceDBI.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StepDBI.h
# End Source File
# Begin Source File

SOURCE=.\TransitionDBI.h
# End Source File
# Begin Source File

SOURCE=.\TypeDBI.h
# End Source File
# Begin Source File

SOURCE=.\VariableDBI.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\DbgInfo.rc
# End Source File
# Begin Source File

SOURCE=.\DebugInfoServer.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VERSION.H
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter "*.idl;*.odl"
# End Group
# Begin Source File

SOURCE=.\DbgInfo.inf
# End Source File
# End Target
# End Project
