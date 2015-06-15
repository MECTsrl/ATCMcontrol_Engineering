# Microsoft Developer Studio Project File - Name="IDBackparser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=IDBackparser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IDBackparser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IDBackparser.mak" CFG="IDBackparser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IDBackparser - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "IDBackparser - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "IDBackparser - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "IDBackparser - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "IDBackparser - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "IDBackparser - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/IDBackparser", TAOFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "IDBackparser - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "IDBackparser.exe"
# PROP BASE Bsc_Name "IDBackparser.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "IDBackparser.exe"
# PROP Bsc_Name "IDBackparser.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\IDBackparser.exe
InputPath=.\IDBackparser.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "IDBackparser.exe"
# PROP BASE Bsc_Name "IDBackparser.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "IDBackparser.exe"
# PROP Bsc_Name "IDBackparser.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\IDBackparser.exe
InputPath=.\IDBackparser.exe
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

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "IDBackparser.exe"
# PROP BASE Bsc_Name "IDBackparser.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "IDBackparser.exe"
# PROP Bsc_Name "IDBackparser.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\IDBackparser.exe
InputPath=.\IDBackparser.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "IDBackparser.exe"
# PROP BASE Bsc_Name "IDBackparser.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "IDBackparser.exe"
# PROP Bsc_Name "IDBackparser.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\IDBackparser.exe
InputPath=.\IDBackparser.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "IDBackparser.exe"
# PROP BASE Bsc_Name "IDBackparser.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "IDBackparser.exe"
# PROP Bsc_Name "IDBackparser.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\IDBackparser.exe
InputPath=.\IDBackparser.exe
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

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "IDBackparser.exe"
# PROP BASE Bsc_Name "IDBackparser.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f IDBackparser.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "IDBackparser.exe"
# PROP Bsc_Name "IDBackparser.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\IDBackparser.exe
InputPath=.\IDBackparser.exe
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

# Name "IDBackparser - Win32 Debug"
# Name "IDBackparser - Win32 Unicode Debug"
# Name "IDBackparser - Win32 Release MinSize"
# Name "IDBackparser - Win32 Release MinDependency"
# Name "IDBackparser - Win32 Unicode Release MinSize"
# Name "IDBackparser - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "IDBackparser - Win32 Debug"

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "IDBackparser - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CEBackparseImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\IDBackparser.cpp
# End Source File
# Begin Source File

SOURCE=.\IDBackparser.def
# End Source File
# Begin Source File

SOURCE=.\IDBackparser.idl
# End Source File
# Begin Source File

SOURCE=.\IDBackparser.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CEBackparseImpl.h
# End Source File
# Begin Source File

SOURCE=.\IDBackparserCP.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CEBackparseImpl.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\IDBackparser.inf
# End Source File
# End Target
# End Project
