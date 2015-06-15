# Microsoft Developer Studio Project File - Name="CeExtract" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CeExtract - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CeExtract.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CeExtract.mak" CFG="CeExtract - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CeExtract - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CeExtract - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CeExtract - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CeExtract - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "CeExtract - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CeExtract - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEExtract", XOMFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CeExtract - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f ceextract.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceextract.exe"
# PROP BASE Bsc_Name "ceextract.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f ceextract.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceextract.exe"
# PROP Bsc_Name "ceextract.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Die Registrierung wird durchgeführt
OutDir=.\Debug
TargetPath=.\ceextract.exe
InputPath=.\ceextract.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f ceextract.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceextract.exe"
# PROP BASE Bsc_Name "ceextract.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f ceextract.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceextract.exe"
# PROP Bsc_Name "ceextract.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Die Registrierung wird durchgeführt
OutDir=.\DebugU
TargetPath=.\ceextract.exe
InputPath=.\ceextract.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warnung : Unicode DLL kann unter Windows 95 nicht registriert werden 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f ceextract.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceextract.exe"
# PROP BASE Bsc_Name "ceextract.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f ceextract.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceextract.exe"
# PROP Bsc_Name "ceextract.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Die Registrierung wird durchgeführt
OutDir=.\ReleaseMinSize
TargetPath=.\ceextract.exe
InputPath=.\ceextract.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f ceextract.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceextract.exe"
# PROP BASE Bsc_Name "ceextract.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f ceextract.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceextract.exe"
# PROP Bsc_Name "ceextract.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Die Registrierung wird durchgeführt
OutDir=.\ReleaseMinDependency
TargetPath=.\ceextract.exe
InputPath=.\ceextract.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f ceextract.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceextract.exe"
# PROP BASE Bsc_Name "ceextract.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f ceextract.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceextract.exe"
# PROP Bsc_Name "ceextract.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Die Registrierung wird durchgeführt
OutDir=.\ReleaseUMinSize
TargetPath=.\ceextract.exe
InputPath=.\ceextract.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warnung : Unicode DLL kann unter Windows 95 nicht registriert werden 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f ceextract.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceextract.exe"
# PROP BASE Bsc_Name "ceextract.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f ceextract.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceextract.exe"
# PROP Bsc_Name "ceextract.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Die Registrierung wird durchgeführt
OutDir=.\ReleaseUMinDependency
TargetPath=.\ceextract.exe
InputPath=.\ceextract.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warnung : Unicode DLL kann unter Windows 95 nicht registriert werden 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CeExtract - Win32 Debug"
# Name "CeExtract - Win32 Unicode Debug"
# Name "CeExtract - Win32 Release MinSize"
# Name "CeExtract - Win32 Release MinDependency"
# Name "CeExtract - Win32 Unicode Release MinSize"
# Name "CeExtract - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "CeExtract - Win32 Debug"

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "CeExtract - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CeExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\CeExtract.def
# End Source File
# Begin Source File

SOURCE=..\..\inc\CeExtract.idl
# End Source File
# Begin Source File

SOURCE=.\CeExtract.rc
# End Source File
# Begin Source File

SOURCE=.\Extract.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\baselib.h
# End Source File
# Begin Source File

SOURCE=.\Extract.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VERSION.H
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Extract.rgs
# End Source File
# End Group
# Begin Group "Libraries"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\fc_tools.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\fc_util.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ceextract.inf
# End Source File
# End Target
# End Project
