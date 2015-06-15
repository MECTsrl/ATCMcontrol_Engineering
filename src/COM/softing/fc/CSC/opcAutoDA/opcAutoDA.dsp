# Microsoft Developer Studio Project File - Name="opcAutoDA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=opcAutoDA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "opcAutoDA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "opcAutoDA.mak" CFG="opcAutoDA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "opcAutoDA - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "opcAutoDA - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "opcAutoDA - Win32 Unicode Release" (based on "Win32 (x86) External Target")
!MESSAGE "opcAutoDA - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CSC/opcAutoDA", MLSFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "opcAutoDA - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f opcautoda.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "opcautoda.exe"
# PROP BASE Bsc_Name "opcautoda.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "DObj"
# PROP Cmd_Line "NMAKE /f opcautoda.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcautoda.exe"
# PROP Bsc_Name "opcautoda.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\opcautoda.exe
InputPath=.\opcautoda.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "opcAutoDA - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f opcautoda.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "opcautoda.exe"
# PROP BASE Bsc_Name "opcautoda.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DObjU"
# PROP Cmd_Line "NMAKE /f opcautoda.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcautoda.exe"
# PROP Bsc_Name "opcautoda.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\DebugU
TargetPath=.\opcautoda.exe
InputPath=.\opcautoda.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "opcAutoDA - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "RObjU"
# PROP BASE Cmd_Line "NMAKE /f opcautoda.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "opcautoda.exe"
# PROP BASE Bsc_Name "opcautoda.bsc"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "RObjU"
# PROP Cmd_Line "NMAKE /f opcautoda.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcautoda.exe"
# PROP Bsc_Name "opcautoda.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseU
TargetPath=.\opcautoda.exe
InputPath=.\opcautoda.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "opcAutoDA - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "RObj"
# PROP BASE Cmd_Line "NMAKE /f opcautoda.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "opcautoda.exe"
# PROP BASE Bsc_Name "opcautoda.bsc"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "RObj"
# PROP Cmd_Line "NMAKE /f opcautoda.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "opcautoda.exe"
# PROP Bsc_Name "opcautoda.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\opcautoda.exe
InputPath=.\opcautoda.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "opcAutoDA - Win32 Debug"
# Name "opcAutoDA - Win32 Unicode Debug"
# Name "opcAutoDA - Win32 Unicode Release"
# Name "opcAutoDA - Win32 Release"

!IF  "$(CFG)" == "opcAutoDA - Win32 Debug"

!ELSEIF  "$(CFG)" == "opcAutoDA - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "opcAutoDA - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "opcAutoDA - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\opcAutoDA.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCAutoServer.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\opcda_cats.c
# End Source File
# Begin Source File

SOURCE=.\OPCGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCGroupCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCGroups.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCItem.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCItems.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\OPCAutoServer.h
# End Source File
# Begin Source File

SOURCE=.\OPCBrowser.h
# End Source File
# Begin Source File

SOURCE=.\OPCGroup.h
# End Source File
# Begin Source File

SOURCE=.\OPCGroupEvent.h
# End Source File
# Begin Source File

SOURCE=.\OPCItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter "*.rc"
# Begin Source File

SOURCE=.\opcAutoDA.rc
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=..\..\inc\opc\opcAutoSofting.idl
# End Source File
# Begin Source File

SOURCE=..\..\inc\opc\opcComn.idl
# End Source File
# Begin Source File

SOURCE=..\..\inc\opc\opcDA.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\opcAutoDA.def
# End Source File
# Begin Source File

SOURCE=.\opcAutoDA.inf
# End Source File
# End Target
# End Project
