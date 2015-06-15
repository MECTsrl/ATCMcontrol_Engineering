# Microsoft Developer Studio Project File - Name="CEProjMan" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEProjMan - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEProjMan.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEProjMan.mak" CFG="CEProjMan - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEProjMan - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEProjMan - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEProjMan - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CEProjMan - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "CEProjMan - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CEProjMan - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEProjMan", RANFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEProjMan - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEProjMan.exe"
# PROP BASE Bsc_Name "CEProjMan.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP Rebuild_Opt "clean all"
# PROP Bsc_Name "CEProjMan.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\CEProjMan.exe
InputPath=.\CEProjMan.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEProjMan.exe"
# PROP BASE Bsc_Name "CEProjMan.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEProjMan.exe"
# PROP Bsc_Name "CEProjMan.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\CEProjMan.exe
InputPath=.\CEProjMan.exe
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

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEProjMan.exe"
# PROP BASE Bsc_Name "CEProjMan.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEProjMan.exe"
# PROP Bsc_Name "CEProjMan.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\CEProjMan.exe
InputPath=.\CEProjMan.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEProjMan.exe"
# PROP BASE Bsc_Name "CEProjMan.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEProjMan.exe"
# PROP Bsc_Name "CEProjMan.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\CEProjMan.exe
InputPath=.\CEProjMan.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEProjMan.exe"
# PROP BASE Bsc_Name "CEProjMan.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEProjMan.exe"
# PROP Bsc_Name "CEProjMan.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\CEProjMan.exe
InputPath=.\CEProjMan.exe
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

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEProjMan.exe"
# PROP BASE Bsc_Name "CEProjMan.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f CEProjMan.mak"
# PROP Rebuild_Opt "clean all"
# PROP Bsc_Name "CEProjMan.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\CEProjMan.exe
InputPath=.\CEProjMan.exe
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

# Name "CEProjMan - Win32 Debug"
# Name "CEProjMan - Win32 Unicode Debug"
# Name "CEProjMan - Win32 Release MinSize"
# Name "CEProjMan - Win32 Release MinDependency"
# Name "CEProjMan - Win32 Unicode Release MinSize"
# Name "CEProjMan - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "CEProjMan - Win32 Debug"

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "CEProjMan - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\inc\cecompmanev.idl
# End Source File
# Begin Source File

SOURCE=.\CEProjectManager.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\CEProjInfo.idl
# End Source File
# Begin Source File

SOURCE=.\CEProjMan.cpp
# End Source File
# Begin Source File

SOURCE=.\CEProjMan.def
# End Source File
# Begin Source File

SOURCE=..\inc\CEProjMan.idl
# End Source File
# Begin Source File

SOURCE=.\CEProjManCP.h
# End Source File
# Begin Source File

SOURCE=.\ClassTree.cpp
# End Source File
# Begin Source File

SOURCE=.\FileHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericTree.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetTree.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CEProjectManager.h
# End Source File
# Begin Source File

SOURCE=.\ClassTree.h
# End Source File
# Begin Source File

SOURCE=.\FileHelper.h
# End Source File
# Begin Source File

SOURCE=.\GenericTree.h
# End Source File
# Begin Source File

SOURCE=.\InstanceTree.h
# End Source File
# Begin Source File

SOURCE=.\ProjectFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\TakeTime.h
# End Source File
# Begin Source File

SOURCE=.\TargetTree.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CEProjectManager.rgs
# End Source File
# Begin Source File

SOURCE=.\CEProjMan.rc
# End Source File
# Begin Source File

SOURCE=.\CEProjMan_Error.mc
# End Source File
# End Group
# Begin Source File

SOURCE=.\CEProjMan.inf
# End Source File
# End Target
# End Project
