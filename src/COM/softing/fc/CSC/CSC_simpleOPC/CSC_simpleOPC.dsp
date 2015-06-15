# Microsoft Developer Studio Project File - Name="CSC_simpleOPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CSC_simpleOPC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CSC_simpleOPC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CSC_simpleOPC.mak" CFG="CSC_simpleOPC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CSC_simpleOPC - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CSC_simpleOPC - Win32 Unicode Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CSC_simpleOPC - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE "CSC_simpleOPC - Win32 Unicode Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC", IKSFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CSC_simpleOPC - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_simpleOPC.exe"
# PROP BASE Bsc_Name "CSC_simpleOPC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "DObj"
# PROP Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_simpleOPC.exe"
# PROP Bsc_Name "CSC_simpleOPC.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\CSC_simpleOPC.exe
InputPath=.\CSC_simpleOPC.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_simpleOPC.exe"
# PROP BASE Bsc_Name "CSC_simpleOPC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DObjU"
# PROP Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_simpleOPC.exe"
# PROP Bsc_Name "CSC_simpleOPC.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\CSC_simpleOPC.exe
InputPath=.\CSC_simpleOPC.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /l "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_simpleOPC.exe"
# PROP BASE Bsc_Name "CSC_simpleOPC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "RObj"
# PROP Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_simpleOPC.exe"
# PROP Bsc_Name "CSC_simpleOPC.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\CSC_simpleOPC.exe
InputPath=.\CSC_simpleOPC.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_simpleOPC.exe"
# PROP BASE Bsc_Name "CSC_simpleOPC.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "RObjU"
# PROP Cmd_Line "NMAKE /f CSC_simpleOPC.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_simpleOPC.exe"
# PROP Bsc_Name "CSC_simpleOPC.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Performing registration
OutDir=.\ReleaseU
TargetPath=.\CSC_simpleOPC.exe
InputPath=.\CSC_simpleOPC.exe
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

# Name "CSC_simpleOPC - Win32 Debug"
# Name "CSC_simpleOPC - Win32 Unicode Debug"
# Name "CSC_simpleOPC - Win32 Release MinDependency"
# Name "CSC_simpleOPC - Win32 Unicode Release MinDependency"

!IF  "$(CFG)" == "CSC_simpleOPC - Win32 Debug"

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Unicode Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CSC_simpleOPC.cpp
# End Source File
# Begin Source File

SOURCE=.\CSC_simpleOPC.def
# End Source File
# Begin Source File

SOURCE=.\CSC_simpleOPC.idl

!IF  "$(CFG)" == "CSC_simpleOPC - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step on $(InputPath)
InputPath=.\CSC_simpleOPC.idl
InputName=CSC_simpleOPC

BuildCmds= \
	midl -Os -I ..\..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Unicode Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step on $(InputPath)
InputPath=.\CSC_simpleOPC.idl
InputName=CSC_simpleOPC

BuildCmds= \
	midl -Os -I ..\..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Release MinDependency"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step on $(InputPath)
InputPath=.\CSC_simpleOPC.idl
InputName=CSC_simpleOPC

BuildCmds= \
	midl -Os -I ..\..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_simpleOPC - Win32 Unicode Release MinDependency"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step on $(InputPath)
InputPath=.\CSC_simpleOPC.idl
InputName=CSC_simpleOPC

BuildCmds= \
	midl -Os -I ..\..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ItemContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncAccessGlobals.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFormatter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ItemContainer.h
# End Source File
# Begin Source File

SOURCE=.\Items.h
# End Source File
# Begin Source File

SOURCE=.\ItemStringList.h
# End Source File
# Begin Source File

SOURCE=.\MsgHistory.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SyncAccess.h
# End Source File
# Begin Source File

SOURCE=.\SyncAccessGlobals.h
# End Source File
# Begin Source File

SOURCE=.\ValueFormatter.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CSC_simpleOPC.rc
# End Source File
# Begin Source File

SOURCE=.\FCMessageHistory.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SyncAccess.rgs
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter "*.idl"
# End Group
# Begin Source File

SOURCE=.\csc_simpleOPC.inf
# End Source File
# Begin Source File

SOURCE=.\test_readOPC.htm
# End Source File
# Begin Source File

SOURCE=.\testSimpleOPC.asp
# End Source File
# End Target
# End Project
