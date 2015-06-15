# Microsoft Developer Studio Project File - Name="LibAccess" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=LibAccess - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibAccess.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibAccess.mak" CFG="LibAccess - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LibAccess - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "LibAccess - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/LibAccess", ERLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "LibAccess - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f LibAccess.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "LibAccess.exe"
# PROP BASE Bsc_Name "LibAccess.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f LibAccess.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "LibAccess.exe"
# PROP Bsc_Name "LibAccess.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\LibAccess.exe
InputPath=.\LibAccess.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "LibAccess - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LibAccess___Win32_Release"
# PROP BASE Intermediate_Dir "LibAccess___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f LibAccess.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "LibAccess.exe"
# PROP BASE Bsc_Name "LibAccess.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f LibAccess.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "LibAccess.exe"
# PROP Bsc_Name "LibAccess.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\LibAccess.exe
InputPath=.\LibAccess.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "LibAccess - Win32 Debug"
# Name "LibAccess - Win32 Release"

!IF  "$(CFG)" == "LibAccess - Win32 Debug"

!ELSEIF  "$(CFG)" == "LibAccess - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\LibAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\LibAccess.def
# End Source File
# Begin Source File

SOURCE=.\LibAccess.inf
# End Source File
# Begin Source File

SOURCE=.\LowLevelLib.cpp
# End Source File
# Begin Source File

SOURCE=.\newdel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\LowLevelLib.h
# End Source File
# Begin Source File

SOURCE=.\newdel.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CG_4CLAccMsg.mc
# End Source File
# Begin Source File

SOURCE=.\LibAccess.rc
# End Source File
# Begin Source File

SOURCE=.\LowLevelLib.rgs
# End Source File
# End Group
# End Target
# End Project
