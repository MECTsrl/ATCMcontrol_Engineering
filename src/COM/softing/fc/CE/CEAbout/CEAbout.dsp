# Microsoft Developer Studio Project File - Name="CEAbout" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEAbout - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEAbout.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEAbout.mak" CFG="CEAbout - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEAbout - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEAbout - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEAbout - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CEAbout - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "CEAbout - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CEAbout - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEAbout", HULFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEAbout - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f ceabout.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceabout.exe"
# PROP BASE Bsc_Name "ceabout.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f ceabout.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceabout.exe"
# PROP Bsc_Name "ceabout.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\ceabout.exe
InputPath=.\ceabout.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f ceabout.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceabout.exe"
# PROP BASE Bsc_Name "ceabout.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f ceabout.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceabout.exe"
# PROP Bsc_Name "ceabout.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\ceabout.exe
InputPath=.\ceabout.exe
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

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f ceabout.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceabout.exe"
# PROP BASE Bsc_Name "ceabout.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f ceabout.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceabout.exe"
# PROP Bsc_Name "ceabout.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ceabout.exe
InputPath=.\ceabout.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f ceabout.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceabout.exe"
# PROP BASE Bsc_Name "ceabout.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f ceabout.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceabout.exe"
# PROP Bsc_Name "ceabout.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ceabout.exe
InputPath=.\ceabout.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f ceabout.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceabout.exe"
# PROP BASE Bsc_Name "ceabout.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f ceabout.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceabout.exe"
# PROP Bsc_Name "ceabout.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ceabout.exe
InputPath=.\ceabout.exe
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

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f ceabout.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ceabout.exe"
# PROP BASE Bsc_Name "ceabout.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f ceabout.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ceabout.exe"
# PROP Bsc_Name "ceabout.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ceabout.exe
InputPath=.\ceabout.exe
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

# Name "CEAbout - Win32 Debug"
# Name "CEAbout - Win32 Unicode Debug"
# Name "CEAbout - Win32 Release MinSize"
# Name "CEAbout - Win32 Release MinDependency"
# Name "CEAbout - Win32 Unicode Release MinSize"
# Name "CEAbout - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "CEAbout - Win32 Debug"

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "CEAbout - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CEAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\CEAbout.def
# End Source File
# Begin Source File

SOURCE=.\CEAbout.idl
# End Source File
# Begin Source File

SOURCE=.\CEAbout.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\trailer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutBox.h
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\axwin2.h
# End Source File
# Begin Source File

SOURCE=.\BitmapTool.h
# End Source File
# Begin Source File

SOURCE=.\CEAbout.inf
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TRAILER.H
# End Source File
# Begin Source File

SOURCE=.\VERSION.H
# End Source File
# Begin Source File

SOURCE=.\webextdisp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about_box.bmp
# End Source File
# Begin Source File

SOURCE=.\res\about_box.gif
# End Source File
# Begin Source File

SOURCE=.\res\about_en.htm
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.rgs
# End Source File
# Begin Source File

SOURCE=.\res\common.css
# End Source File
# Begin Source File

SOURCE=.\res\cred1.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred2.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred3.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred4.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred5.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred6.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred7.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred8.4cw
# End Source File
# Begin Source File

SOURCE=.\res\cred9.4cw
# End Source File
# Begin Source File

SOURCE=.\res\log4ctr1small.bmp
# End Source File
# End Group
# End Target
# End Project
# Section CEAbout : {00000000-0000-0000-0000-800000800000}
# 	1:12:IDD_ABOUTBOX:103
# End Section
