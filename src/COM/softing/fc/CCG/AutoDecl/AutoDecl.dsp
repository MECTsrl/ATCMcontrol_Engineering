# Microsoft Developer Studio Project File - Name="AutoDecl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=AutoDecl - Win32 Unicode Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AutoDecl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AutoDecl.mak" CFG="AutoDecl - Win32 Unicode Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AutoDecl - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "AutoDecl - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "AutoDecl - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "AutoDecl - Win32 Unicode Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/AutoDecl", WLLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "AutoDecl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AutoDecl.exe"
# PROP BASE Bsc_Name "AutoDecl.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AutoDecl.exe"
# PROP Bsc_Name "AutoDecl.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\AutoDecl.exe
InputPath=.\AutoDecl.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "AutoDecl - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AutoDecl.exe"
# PROP BASE Bsc_Name "AutoDecl.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AutoDecl.exe"
# PROP Bsc_Name "AutoDecl.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\AutoDecl.exe
InputPath=.\AutoDecl.exe
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

!ELSEIF  "$(CFG)" == "AutoDecl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AutoDecl___Win32_Release"
# PROP BASE Intermediate_Dir "AutoDecl___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AutoDecl.exe"
# PROP BASE Bsc_Name "AutoDecl.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AutoDecl.exe"
# PROP Bsc_Name "AutoDecl.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\AutoDecl.exe
InputPath=.\AutoDecl.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "AutoDecl - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AutoDecl___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "AutoDecl___Win32_Unicode_Release"
# PROP BASE Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "AutoDecl.exe"
# PROP BASE Bsc_Name "AutoDecl.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Cmd_Line "NMAKE /f AutoDecl.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "AutoDecl.exe"
# PROP Bsc_Name "AutoDecl.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseU
TargetPath=.\AutoDecl.exe
InputPath=.\AutoDecl.exe
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

# Name "AutoDecl - Win32 Debug"
# Name "AutoDecl - Win32 Unicode Debug"
# Name "AutoDecl - Win32 Release"
# Name "AutoDecl - Win32 Unicode Release"

!IF  "$(CFG)" == "AutoDecl - Win32 Debug"

!ELSEIF  "$(CFG)" == "AutoDecl - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "AutoDecl - Win32 Release"

!ELSEIF  "$(CFG)" == "AutoDecl - Win32 Unicode Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoDecl.def
# End Source File
# Begin Source File

SOURCE=.\AutoDecl.rc
# End Source File
# Begin Source File

SOURCE=.\CVMAutoDecl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\VMAutoDecl.idl
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CVMAutoDecl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\VMAutoDecl.h
# End Source File
# Begin Source File

SOURCE=.\VMAutoDeclDef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\VMAutoDecl.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoDecl.inf
# End Source File
# End Target
# End Project
