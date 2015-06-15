# Microsoft Developer Studio Project File - Name="StandardProjectWizard" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=StandardProjectWizard - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StandardProjectWizard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StandardProjectWizard.mak" CFG="StandardProjectWizard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StandardProjectWizard - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "StandardProjectWizard - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "StandardProjectWizard - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "StandardProjectWizard - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "StandardProjectWizard - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "StandardProjectWizard - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEWiz/StandardProjectWizard", YKNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "StandardProjectWizard - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StandardProjectWizard.exe"
# PROP BASE Bsc_Name "StandardProjectWizard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StandardProjectWizard.exe"
# PROP Bsc_Name "StandardProjectWizard.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\StandardProjectWizard.exe
InputPath=.\StandardProjectWizard.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StandardProjectWizard.exe"
# PROP BASE Bsc_Name "StandardProjectWizard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StandardProjectWizard.exe"
# PROP Bsc_Name "StandardProjectWizard.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\StandardProjectWizard.exe
InputPath=.\StandardProjectWizard.exe
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

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StandardProjectWizard.exe"
# PROP BASE Bsc_Name "StandardProjectWizard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StandardProjectWizard.exe"
# PROP Bsc_Name "StandardProjectWizard.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\StandardProjectWizard.exe
InputPath=.\StandardProjectWizard.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StandardProjectWizard.exe"
# PROP BASE Bsc_Name "StandardProjectWizard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StandardProjectWizard.exe"
# PROP Bsc_Name "StandardProjectWizard.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\StandardProjectWizard.exe
InputPath=.\StandardProjectWizard.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StandardProjectWizard.exe"
# PROP BASE Bsc_Name "StandardProjectWizard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StandardProjectWizard.exe"
# PROP Bsc_Name "StandardProjectWizard.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\StandardProjectWizard.exe
InputPath=.\StandardProjectWizard.exe
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

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "StandardProjectWizard.exe"
# PROP BASE Bsc_Name "StandardProjectWizard.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f StandardProjectWizard.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "StandardProjectWizard.exe"
# PROP Bsc_Name "StandardProjectWizard.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\StandardProjectWizard.exe
InputPath=.\StandardProjectWizard.exe
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

# Name "StandardProjectWizard - Win32 Debug"
# Name "StandardProjectWizard - Win32 Unicode Debug"
# Name "StandardProjectWizard - Win32 Release MinSize"
# Name "StandardProjectWizard - Win32 Release MinDependency"
# Name "StandardProjectWizard - Win32 Unicode Release MinSize"
# Name "StandardProjectWizard - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "StandardProjectWizard - Win32 Debug"

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "StandardProjectWizard - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\inc\CEProjWiz.idl
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StandardProjectWizard.def
# End Source File
# Begin Source File

SOURCE=.\StandardProjectWizard.idl
# End Source File
# Begin Source File

SOURCE=.\StandardProjectWizard.rc
# End Source File
# Begin Source File

SOURCE=.\StandardProjectWizardApp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPageNames.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPageResource.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardComObj.h
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StandardProjectWizardApp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdProjectWizardDefs.h
# End Source File
# Begin Source File

SOURCE=..\ProjectWizardSelection\version.h
# End Source File
# Begin Source File

SOURCE=.\WizardPageNames.h
# End Source File
# Begin Source File

SOURCE=.\WizardPageResource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\clvwimag.bmp
# End Source File
# Begin Source File

SOURCE=.\ProjectWizardComObj.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\StandardProjectWizard.inf
# End Source File
# End Target
# End Project
