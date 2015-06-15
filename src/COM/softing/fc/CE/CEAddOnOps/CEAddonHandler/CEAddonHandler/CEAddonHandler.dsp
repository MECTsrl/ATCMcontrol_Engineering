# Microsoft Developer Studio Project File - Name="CEAddonHandler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEAddonHandler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEAddonHandler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEAddonHandler.mak" CFG="CEAddonHandler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEAddonHandler - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEAddonHandler - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEAddonHandler - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CEAddonHandler - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "CEAddonHandler - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CEAddonHandler - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEAddOnOps/CEAddonHandler/CEAddonHandler", WVLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEAddonHandler - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEAddonHandler.exe"
# PROP BASE Bsc_Name "CEAddonHandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Dialogs"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEAddonHandler.exe"
# PROP Bsc_Name "CEAddonHandler.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\..\..\Dialogs
TargetPath=.\CEAddonHandler.exe
InputPath=.\CEAddonHandler.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEAddonHandler.exe"
# PROP BASE Bsc_Name "CEAddonHandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEAddonHandler.exe"
# PROP Bsc_Name "CEAddonHandler.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\CEAddonHandler.exe
InputPath=.\CEAddonHandler.exe
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

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEAddonHandler.exe"
# PROP BASE Bsc_Name "CEAddonHandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEAddonHandler.exe"
# PROP Bsc_Name "CEAddonHandler.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\CEAddonHandler.exe
InputPath=.\CEAddonHandler.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEAddonHandler.exe"
# PROP BASE Bsc_Name "CEAddonHandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEAddonHandler.exe"
# PROP Bsc_Name "CEAddonHandler.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\CEAddonHandler.exe
InputPath=.\CEAddonHandler.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEAddonHandler.exe"
# PROP BASE Bsc_Name "CEAddonHandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEAddonHandler.exe"
# PROP Bsc_Name "CEAddonHandler.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\CEAddonHandler.exe
InputPath=.\CEAddonHandler.exe
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

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CEAddonHandler.exe"
# PROP BASE Bsc_Name "CEAddonHandler.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f CEAddonHandler.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEAddonHandler.exe"
# PROP Bsc_Name "CEAddonHandler.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\CEAddonHandler.exe
InputPath=.\CEAddonHandler.exe
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

# Name "CEAddonHandler - Win32 Debug"
# Name "CEAddonHandler - Win32 Unicode Debug"
# Name "CEAddonHandler - Win32 Release MinSize"
# Name "CEAddonHandler - Win32 Release MinDependency"
# Name "CEAddonHandler - Win32 Unicode Release MinSize"
# Name "CEAddonHandler - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "CEAddonHandler - Win32 Debug"

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "CEAddonHandler - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CEAddonActionComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandler.def
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandler.idl
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandler.rc
# End Source File
# Begin Source File

SOURCE=.\deletegvlimport.cpp
# End Source File
# Begin Source File

SOURCE=.\DeleteObject.cpp
# End Source File
# Begin Source File

SOURCE=.\deleteprograminst.cpp
# End Source File
# Begin Source File

SOURCE=.\DeleteResource.cpp
# End Source File
# Begin Source File

SOURCE=.\DeleteTask.cpp
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\FileSelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertConstant.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertConstantFile.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertDataType.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertFunctionBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertGlobVar.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertGlobVarFile.cpp
# End Source File
# Begin Source File

SOURCE=.\insertgvlimport.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\insertprograminst.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertResource.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Methods.cpp
# End Source File
# Begin Source File

SOURCE=.\Operation.cpp
# End Source File
# Begin Source File

SOURCE=.\PropDataType.cpp
# End Source File
# Begin Source File

SOURCE=.\PropFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\PropFunctionBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\PropProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\propprograminst.cpp
# End Source File
# Begin Source File

SOURCE=.\propproject.cpp
# End Source File
# Begin Source File

SOURCE=.\PropResource.cpp
# End Source File
# Begin Source File

SOURCE=.\PropTask.cpp
# End Source File
# Begin Source File

SOURCE=.\RenameObject1.cpp
# End Source File
# Begin Source File

SOURCE=.\renameprograminst.cpp
# End Source File
# Begin Source File

SOURCE=.\RenameResource.cpp
# End Source File
# Begin Source File

SOURCE=.\RenameTask.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Var.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CEAddonActionComObj.h
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandlerCP.h
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandlerGlobal.h
# End Source File
# Begin Source File

SOURCE=.\deletegvlimport.h
# End Source File
# Begin Source File

SOURCE=.\DeleteObject.h
# End Source File
# Begin Source File

SOURCE=.\deleteprograminst.h
# End Source File
# Begin Source File

SOURCE=.\DeleteResource.h
# End Source File
# Begin Source File

SOURCE=.\DeleteTask.h
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\FileSelDlg.h
# End Source File
# Begin Source File

SOURCE=.\InsertConstant.h
# End Source File
# Begin Source File

SOURCE=.\InsertConstantFile.h
# End Source File
# Begin Source File

SOURCE=.\InsertDataType.h
# End Source File
# Begin Source File

SOURCE=.\InsertFunction.h
# End Source File
# Begin Source File

SOURCE=.\InsertFunctionBlock.h
# End Source File
# Begin Source File

SOURCE=.\InsertGlobVar.h
# End Source File
# Begin Source File

SOURCE=.\InsertGlobVarFile.h
# End Source File
# Begin Source File

SOURCE=.\insertgvlimport.h
# End Source File
# Begin Source File

SOURCE=.\InsertProgram.h
# End Source File
# Begin Source File

SOURCE=.\insertprograminst.h
# End Source File
# Begin Source File

SOURCE=.\InsertResource.h
# End Source File
# Begin Source File

SOURCE=.\InsertTask.h
# End Source File
# Begin Source File

SOURCE=.\Methods.h
# End Source File
# Begin Source File

SOURCE=.\Operation.h
# End Source File
# Begin Source File

SOURCE=.\PropDataType.h
# End Source File
# Begin Source File

SOURCE=.\PropFunction.h
# End Source File
# Begin Source File

SOURCE=.\PropFunctionBlock.h
# End Source File
# Begin Source File

SOURCE=.\PropProgram.h
# End Source File
# Begin Source File

SOURCE=.\propprograminst.h
# End Source File
# Begin Source File

SOURCE=.\propproject.h
# End Source File
# Begin Source File

SOURCE=.\PropResource.h
# End Source File
# Begin Source File

SOURCE=.\PropTask.h
# End Source File
# Begin Source File

SOURCE=.\RenameObject1.h
# End Source File
# Begin Source File

SOURCE=.\renameprograminst.h
# End Source File
# Begin Source File

SOURCE=.\RenameResource.h
# End Source File
# Begin Source File

SOURCE=.\RenameTask.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Var.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CEAddonActionComObj.rgs
# End Source File
# Begin Source File

SOURCE=.\CEAddonHandler.INF
# End Source File
# End Group
# End Target
# End Project
