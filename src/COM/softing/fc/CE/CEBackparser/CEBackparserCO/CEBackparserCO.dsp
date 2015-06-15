# Microsoft Developer Studio Project File - Name="CEBackparserCO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEBackparserCO - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEBackparserCO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEBackparserCO.mak" CFG="CEBackparserCO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEBackparserCO - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CEBackparserCO - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEBackparser/CEBackparserCO", ULMFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEBackparserCO - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f cebackparserco.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cebackparserco.exe"
# PROP BASE Bsc_Name "cebackparserco.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f cebackparserco.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEBackparserCO.dll"
# PROP Bsc_Name "cebackparserco.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\CEBackparserCO.dll
InputPath=.\cebackparserco.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CEBackparserCO - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f cebackparserco.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cebackparserco.exe"
# PROP BASE Bsc_Name "cebackparserco.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f cebackparserco.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CEBackparserCO.dll"
# PROP Bsc_Name "cebackparserco.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\CEBackparserCO.dll
InputPath=.\cebackparserco.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CEBackparserCO - Win32 Debug"
# Name "CEBackparserCO - Win32 Release MinDependency"

!IF  "$(CFG)" == "CEBackparserCO - Win32 Debug"

!ELSEIF  "$(CFG)" == "CEBackparserCO - Win32 Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\inc\CEBackparse.idl
# End Source File
# Begin Source File

SOURCE=.\CEBackparseComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CEBackparserCO.cpp
# End Source File
# Begin Source File

SOURCE=.\CEBackparserCO.def
# End Source File
# Begin Source File

SOURCE=.\CEBackparserCO.idl
# End Source File
# Begin Source File

SOURCE=.\CEBackparserCO.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CEBackparseComObj.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CEBackparseComObj.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\CEBackparserCO.INF
# End Source File
# End Target
# End Project
