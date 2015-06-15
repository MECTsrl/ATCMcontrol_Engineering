# Microsoft Developer Studio Project File - Name="CSC_Online" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CSC_Online - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CSC_Online.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CSC_Online.mak" CFG="CSC_Online - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CSC_Online - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "CSC_Online - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE", BJSFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CSC_Online - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f CSC_Online.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_Online.exe"
# PROP BASE Bsc_Name "CSC_Online.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "DObj"
# PROP Cmd_Line "NMAKE /f CSC_Online.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_Online.dll"
# PROP Bsc_Name "CSC_Online.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\CSC_Online.dll
InputPath=.\CSC_Online.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_Online - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f CSC_Online.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "CSC_Online.exe"
# PROP BASE Bsc_Name "CSC_Online.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "RObj"
# PROP Cmd_Line "NMAKE /f CSC_Online.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CSC_Online.dll"
# PROP Bsc_Name "CSC_Online.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\CSC_Online.dll
InputPath=.\CSC_Online.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CSC_Online - Win32 Debug"
# Name "CSC_Online - Win32 Release MinSize"

!IF  "$(CFG)" == "CSC_Online - Win32 Debug"

!ELSEIF  "$(CFG)" == "CSC_Online - Win32 Release MinSize"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4CVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\4CVariables.cpp
# End Source File
# Begin Source File

SOURCE=.\CSC_Online.cpp
# End Source File
# Begin Source File

SOURCE=.\CSC_Online.def
# End Source File
# Begin Source File

SOURCE=.\FCSession.cpp
# End Source File
# Begin Source File

SOURCE=.\IBTSession.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSession.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Transfer.cpp
# End Source File
# Begin Source File

SOURCE=.\VarSession.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\4CVariable.h
# End Source File
# Begin Source File

SOURCE=.\4CVariables.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\CatIDs.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\comhelper.h
# End Source File
# Begin Source File

SOURCE=.\CpBreakpointNotification.h
# End Source File
# Begin Source File

SOURCE=.\CpCEMessageEvent.h
# End Source File
# Begin Source File

SOURCE=.\CpSessionNotification.h
# End Source File
# Begin Source File

SOURCE=.\CpTransferNotification.h
# End Source File
# Begin Source File

SOURCE=.\CPtVarNotification.h
# End Source File
# Begin Source File

SOURCE=.\FCSession.h
# End Source File
# Begin Source File

SOURCE=.\IBTSession.h
# End Source File
# Begin Source File

SOURCE=.\MsgSession.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\refobj.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Transfer.h
# End Source File
# Begin Source File

SOURCE=.\VarSession.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\res\BuildNr.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CSC_Online.rc
# End Source File
# Begin Source File

SOURCE=..\..\res\csc_onlineerr.mc
# End Source File
# Begin Source File

SOURCE=.\FCSession.rgs
# End Source File
# Begin Source File

SOURCE=.\FCVariableCollection.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "CAB"

# PROP Default_Filter "*.bat;*inf"
# Begin Source File

SOURCE=..\build_cab.bat
# End Source File
# Begin Source File

SOURCE=..\CSC_Online_cab.inf
# End Source File
# End Group
# Begin Group "INF Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4CSC.inf
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CSC_Online.inf
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=..\..\Inc\CSC_Online.idl

!IF  "$(CFG)" == "CSC_Online - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step
InputPath=..\..\Inc\CSC_Online.idl
InputName=CSC_Online

BuildCmds= \
	midl -Os -I ..\..\inc -h $(InputName).h -iid $(InputName)_i.c $(InputName).idl

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CSC_Online - Win32 Release MinSize"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL Step
InputPath=..\..\Inc\CSC_Online.idl
InputName=CSC_Online

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

SOURCE=..\..\inc\CSC_OnlineServer.idl
# End Source File
# Begin Source File

SOURCE=..\..\inc\CSC_OnlineServerExt.idl
# End Source File
# Begin Source File

SOURCE=..\..\AddOns\Softing\4CPCTarget\inc\CSC_OnlSrv.idl
# End Source File
# End Group
# End Target
# End Project
