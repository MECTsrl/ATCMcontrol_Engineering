# Microsoft Developer Studio Project File - Name="CpNavigator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0102

CFG=CpNavigator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CpNavigator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CpNavigator.mak" CFG="CpNavigator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CpNavigator - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CpNavigator - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CpNavigator - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CpNavigator - Win32 Unicode Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator", JURFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CpNavigator - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpNavigator.exe"
# PROP BASE Bsc_Name "CpNavigator.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpNavigator.exe"
# PROP Bsc_Name "CpNavigator.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\Release
TargetPath=.\Release\CpNavigator.ocx
InputPath=.\Release\CpNavigator.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpNavigator - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpNavigator.exe"
# PROP BASE Bsc_Name "CpNavigator.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpNavigator.exe"
# PROP Bsc_Name "CpNavigator.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\Debug
TargetPath=.\Debug\CpNavigator.ocx
InputPath=.\Debug\CpNavigator.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpNavigator - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpNavigator.exe"
# PROP BASE Bsc_Name "CpNavigator.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Ext "ocx"
# PROP Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpNavigator.exe"
# PROP Bsc_Name "CpNavigator.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\DebugU
TargetPath=.\DebugU\CpNavigator.ocx
InputPath=.\DebugU\CpNavigator.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpNavigator - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpNavigator.exe"
# PROP BASE Bsc_Name "CpNavigator.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Ext "ocx"
# PROP Cmd_Line "NMAKE /f CpNavigator.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpNavigator.exe"
# PROP Bsc_Name "CpNavigator.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\CpNavigator.ocx
InputPath=.\ReleaseU\CpNavigator.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CpNavigator - Win32 Release"
# Name "CpNavigator - Win32 Debug"
# Name "CpNavigator - Win32 Unicode Debug"
# Name "CpNavigator - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CpInternet.cpp
# End Source File
# Begin Source File

SOURCE=.\CpNavigator.cpp
# End Source File
# Begin Source File

SOURCE=.\CpNavigator.def
# End Source File
# Begin Source File

SOURCE=.\CpNavigator.odl
# End Source File
# Begin Source File

SOURCE=.\CpNavigator.rc
# End Source File
# Begin Source File

SOURCE=.\CpNavigatorCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\CpNavigatorPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\CpNavPageFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CpNavTab.cpp
# End Source File
# Begin Source File

SOURCE=.\CpNavTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CpObjectList.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\VariablePath.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CpInternet.H
# End Source File
# Begin Source File

SOURCE=.\CpNavigator.h
# End Source File
# Begin Source File

SOURCE=.\CpNavigatorCtl.h
# End Source File
# Begin Source File

SOURCE=.\CpNavigatorPpg.h
# End Source File
# Begin Source File

SOURCE=.\CpNavPageFrame.h
# End Source File
# Begin Source File

SOURCE=.\CpNavTab.h
# End Source File
# Begin Source File

SOURCE=.\CpNavTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CpObjectList.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VariablePath.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CpNavigatorCtl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CpNavTreeIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
