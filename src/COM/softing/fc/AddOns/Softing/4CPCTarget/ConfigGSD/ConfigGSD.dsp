# Microsoft Developer Studio Project File - Name="ConfigGSD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ConfigGSD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ConfigGSD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ConfigGSD.mak" CFG="ConfigGSD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ConfigGSD - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "ConfigGSD - Win32 Release MinDependency" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/AddOns/Softing/4CPCTarget/ConfigGSD", MVHFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "ConfigGSD - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f ConfigGSD.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ConfigGSD.exe"
# PROP BASE Bsc_Name "ConfigGSD.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f ConfigGSD.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ConfigGSD.dll"
# PROP Bsc_Name "ConfigGSD.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\ConfigGSD.dll
InputPath=.\ConfigGSD.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ConfigGSD - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Cmd_Line "NMAKE /f ConfigGSD.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "ConfigGSD.exe"
# PROP BASE Bsc_Name "ConfigGSD.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Cmd_Line "NMAKE /f ConfigGSD.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "ConfigGSD.dll"
# PROP Bsc_Name "ConfigGSD.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseMinDependency
TargetPath=.\ConfigGSD.dll
InputPath=.\ConfigGSD.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ConfigGSD - Win32 Debug"
# Name "ConfigGSD - Win32 Release MinDependency"

!IF  "$(CFG)" == "ConfigGSD - Win32 Debug"

!ELSEIF  "$(CFG)" == "ConfigGSD - Win32 Release MinDependency"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;r;hpj;bat"
# Begin Source File

SOURCE=.\ConfGSD_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigGSD.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigGSD.def
# End Source File
# Begin Source File

SOURCE=.\ConfigGSDBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\DP_Module.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtParam.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtUserParamValues.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PropCfgUserData.cpp
# End Source File
# Begin Source File

SOURCE=.\PropErrors.cpp
# End Source File
# Begin Source File

SOURCE=.\PropGSDModule.cpp
# End Source File
# Begin Source File

SOURCE=.\PropGSDSlaveInfos.cpp
# End Source File
# Begin Source File

SOURCE=.\PropGSDSlaveModule.cpp
# End Source File
# Begin Source File

SOURCE=.\SheetConfigGSD.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConfGSD_Utils.hpp
# End Source File
# Begin Source File

SOURCE=.\ConfigGSDBasic.h
# End Source File
# Begin Source File

SOURCE=.\Defines.h
# End Source File
# Begin Source File

SOURCE=.\DP_Module.h
# End Source File
# Begin Source File

SOURCE=.\ExtParam.h
# End Source File
# Begin Source File

SOURCE=.\ExtUserParamValues.h
# End Source File
# Begin Source File

SOURCE=.\HexEdit.hpp
# End Source File
# Begin Source File

SOURCE=.\PropCfgUserData.h
# End Source File
# Begin Source File

SOURCE=.\PropErrors.h
# End Source File
# Begin Source File

SOURCE=.\PropGSDModule.h
# End Source File
# Begin Source File

SOURCE=.\PropGSDSlaveInfos.h
# End Source File
# Begin Source File

SOURCE=.\PropGSDSlaveModule.h
# End Source File
# Begin Source File

SOURCE=.\SheetConfigGSD.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe;rc;mc"
# Begin Source File

SOURCE=.\ConfigGSD.rc
# End Source File
# Begin Source File

SOURCE=.\ConfigGSDBasic.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VERSION.H
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter "idl;odl"
# Begin Source File

SOURCE=..\Inc\ConfigGSD.idl

!IF  "$(CFG)" == "ConfigGSD - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step
InputPath=..\Inc\ConfigGSD.idl
InputName=ConfigGSD

BuildCmds= \
	midl -Os -I ..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ConfigGSD - Win32 Release MinDependency"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step
InputPath=..\Inc\ConfigGSD.idl
InputName=ConfigGSD

BuildCmds= \
	midl -Os -I ..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ConfigGSD.inf
# End Source File
# End Target
# End Project
