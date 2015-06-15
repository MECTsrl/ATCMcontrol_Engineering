# Microsoft Developer Studio Project File - Name="KadManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=KadManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "kadmanager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "kadmanager.mak" CFG="KadManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KadManager - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "KadManager - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "KadManager - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "KadManager - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "KadManager - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "KadManager - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEKADMAN", KPMFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "KadManager - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f kadmanager.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "kadmanager.exe"
# PROP BASE Bsc_Name "kadmanager.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f kadmanager.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "kadmanager.exe"
# PROP Bsc_Name "kadmanager.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\kadmanager.exe
InputPath=.\kadmanager.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "KadManager - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f kadmanager.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "kadmanager.exe"
# PROP BASE Bsc_Name "kadmanager.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f kadmanager.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "kadmanager.exe"
# PROP Bsc_Name "kadmanager.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\kadmanager.exe
InputPath=.\kadmanager.exe
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

!ELSEIF  "$(CFG)" == "KadManager - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f kadmanager.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "kadmanager.exe"
# PROP BASE Bsc_Name "kadmanager.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f kadmanager.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "kadmanager.exe"
# PROP Bsc_Name "kadmanager.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\kadmanager.exe
InputPath=.\kadmanager.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "KadManager - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f kadmanager.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "kadmanager.exe"
# PROP BASE Bsc_Name "kadmanager.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f kadmanager.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "kadmanager.exe"
# PROP Bsc_Name "kadmanager.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\kadmanager.exe
InputPath=.\kadmanager.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "KadManager - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f kadmanager.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "kadmanager.exe"
# PROP BASE Bsc_Name "kadmanager.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f kadmanager.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "kadmanager.exe"
# PROP Bsc_Name "kadmanager.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\kadmanager.exe
InputPath=.\kadmanager.exe
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

!ELSEIF  "$(CFG)" == "KadManager - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f kadmanager.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "kadmanager.exe"
# PROP BASE Bsc_Name "kadmanager.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f kadmanager.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "kadmanager.exe"
# PROP Bsc_Name "kadmanager.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\ 
TargetPath=.\kadmanager.exe
InputPath=.\kadmanager.exe
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

# Name "KadManager - Win32 Debug"
# Name "KadManager - Win32 Unicode Debug"
# Name "KadManager - Win32 Release MinSize"
# Name "KadManager - Win32 Release MinDependency"
# Name "KadManager - Win32 Unicode Release MinSize"
# Name "KadManager - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "KadManager - Win32 Debug"

!ELSEIF  "$(CFG)" == "KadManager - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "KadManager - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "KadManager - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "KadManager - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "KadManager - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\inc\CEKadMan.idl
# End Source File
# Begin Source File

SOURCE=.\ClassComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\DragDropActionComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DragDropMapComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumData.cpp
# End Source File
# Begin Source File

SOURCE=.\FileComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\IECFeaturesComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\KadAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\KadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\KadManager.def
# End Source File
# Begin Source File

SOURCE=..\inc\KadManager.idl
# End Source File
# Begin Source File

SOURCE=.\KadManager.rc
# End Source File
# Begin Source File

SOURCE=.\KadManagerComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuItemComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\StateComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\TargetOnlineComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolbarComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolbarItemComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\VarDeclComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlNodeRef.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClassComObj.h
# End Source File
# Begin Source File

SOURCE=.\ComObj.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DragDropActionComObj.h
# End Source File
# Begin Source File

SOURCE=.\DragDropMapComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumClassesComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumData.h
# End Source File
# Begin Source File

SOURCE=.\EnumDragDropActionsComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumFilesComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumMenuItemsComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumMenusComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumStatesComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumTargetsComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumToolbarItemsComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumToolbarsComObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumTypesComObj.h
# End Source File
# Begin Source File

SOURCE=.\ErrorCodes.h
# End Source File
# Begin Source File

SOURCE=.\FileComObj.h
# End Source File
# Begin Source File

SOURCE=.\IECFeaturesComObj.h
# End Source File
# Begin Source File

SOURCE=.\KadAccess.h
# End Source File
# Begin Source File

SOURCE=.\KadManagerComObj.h
# End Source File
# Begin Source File

SOURCE=.\MenuComObj.h
# End Source File
# Begin Source File

SOURCE=.\MenuItemComObj.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StateComObj.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TargetComObj.h
# End Source File
# Begin Source File

SOURCE=.\TargetOnlineComObj.h
# End Source File
# Begin Source File

SOURCE=.\ToolbarComObj.h
# End Source File
# Begin Source File

SOURCE=.\ToolbarItemComObj.h
# End Source File
# Begin Source File

SOURCE=.\TypeComObj.h
# End Source File
# Begin Source File

SOURCE=.\VarDeclComObj.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\XmlNodeRef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\KadManagerComObj.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\KadManager.inf
# End Source File
# Begin Source File

SOURCE=.\KadManagerErrors.mc
# End Source File
# End Target
# End Project
