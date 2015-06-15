# Microsoft Developer Studio Project File - Name="CpSvrTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0102

CFG=CpSvrTools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CpSvrTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CpSvrTools.mak" CFG="CpSvrTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CpSvrTools - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CpSvrTools - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CpSvrTools - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CpSvrTools - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "CpSvrTools - Win32 Unicode Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE "CpSvrTools - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/CpSvrTools", STRFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CpSvrTools - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpSvrTools.exe"
# PROP BASE Bsc_Name "CpSvrTools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpSvrTools.exe"
# PROP Bsc_Name "CpSvrTools.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\Debug
TargetPath=.\Debug\CpSvrTools.dll
InputPath=.\Debug\CpSvrTools.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpSvrTools.exe"
# PROP BASE Bsc_Name "CpSvrTools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpSvrTools.exe"
# PROP Bsc_Name "CpSvrTools.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\DebugU
TargetPath=.\DebugU\CpSvrTools.dll
InputPath=.\DebugU\CpSvrTools.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpSvrTools.exe"
# PROP BASE Bsc_Name "CpSvrTools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpSvrTools.exe"
# PROP Bsc_Name "CpSvrTools.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\CpSvrTools.dll
InputPath=.\ReleaseMinSize\CpSvrTools.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpSvrTools.exe"
# PROP BASE Bsc_Name "CpSvrTools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpSvrTools.exe"
# PROP Bsc_Name "CpSvrTools.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\CpSvrTools.dll
InputPath=.\ReleaseMinDependency\CpSvrTools.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpSvrTools.exe"
# PROP BASE Bsc_Name "CpSvrTools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpSvrTools.exe"
# PROP Bsc_Name "CpSvrTools.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\CpSvrTools.dll
InputPath=.\ReleaseUMinSize\CpSvrTools.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CpSvrTools.exe"
# PROP BASE Bsc_Name "CpSvrTools.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Cmd_Line "NMAKE /f CpSvrTools.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CpSvrTools.exe"
# PROP Bsc_Name "CpSvrTools.bsc"
# PROP Target_Dir ""
# Begin Custom Build - ActiveX-Steuerelemente werden registriert...
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\CpSvrTools.dll
InputPath=.\ReleaseUMinDependency\CpSvrTools.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CpSvrTools - Win32 Debug"
# Name "CpSvrTools - Win32 Unicode Debug"
# Name "CpSvrTools - Win32 Release MinSize"
# Name "CpSvrTools - Win32 Release MinDependency"
# Name "CpSvrTools - Win32 Unicode Release MinSize"
# Name "CpSvrTools - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CpLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\CpSvrTools.cpp
# End Source File
# Begin Source File

SOURCE=.\CpSvrTools.def
# End Source File
# Begin Source File

SOURCE=.\CpSvrTools.idl

!IF  "$(CFG)" == "CpSvrTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\CpSvrTools.idl

BuildCmds= \
	midl /Oicf /h "CpSvrTools.h" /iid "CpSvrTools_i.c" "CpSvrTools.idl"

".\CpSvrTools.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Unicode Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\CpSvrTools.idl

BuildCmds= \
	midl /Oicf /h "CpSvrTools.h" /iid "CpSvrTools_i.c" "CpSvrTools.idl"

".\CpSvrTools.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\CpSvrTools.idl

BuildCmds= \
	midl /Oicf /h "CpSvrTools.h" /iid "CpSvrTools_i.c" "CpSvrTools.idl"

".\CpSvrTools.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Release MinDependency"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\CpSvrTools.idl

BuildCmds= \
	midl /Oicf /h "CpSvrTools.h" /iid "CpSvrTools_i.c" "CpSvrTools.idl"

".\CpSvrTools.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Unicode Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\CpSvrTools.idl

BuildCmds= \
	midl /Oicf /h "CpSvrTools.h" /iid "CpSvrTools_i.c" "CpSvrTools.idl"

".\CpSvrTools.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CpSvrTools - Win32 Unicode Release MinDependency"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\CpSvrTools.idl

BuildCmds= \
	midl /Oicf /h "CpSvrTools.h" /iid "CpSvrTools_i.c" "CpSvrTools.idl"

".\CpSvrTools.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\CpSvrTools_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CpSvrTools.rc
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CpLayout.h
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\CpLayout.rgs
# End Source File
# End Target
# End Project
