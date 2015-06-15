# Microsoft Developer Studio Project File - Name="CeWatchView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CeWatchView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CeWatchView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CeWatchView.mak" CFG="CeWatchView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CeWatchView - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CeWatchView - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEWatchView", UGNFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CeWatchView - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Cmd_Line "NMAKE /f cewatchview.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cewatchview.exe"
# PROP BASE Bsc_Name "cewatchview.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Cmd_Line "NMAKE /f cewatchview.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cewatchview.ocx"
# PROP Bsc_Name "cewatchview.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\cewatchview.ocx
InputPath=.\cewatchview.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CeWatchView - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Cmd_Line "NMAKE /f cewatchview.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cewatchview.exe"
# PROP BASE Bsc_Name "cewatchview.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Cmd_Line "NMAKE /f cewatchview.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cewatchview.dll"
# PROP Bsc_Name "cewatchview.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\cewatchview.dll
InputPath=.\cewatchview.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CeWatchView - Win32 Release"
# Name "CeWatchView - Win32 Debug"

!IF  "$(CFG)" == "CeWatchView - Win32 Release"

!ELSEIF  "$(CFG)" == "CeWatchView - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ButtonIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\CeDisplayElement.cpp
# End Source File
# Begin Source File

SOURCE=.\CeSymbol.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchBackEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchElement.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchIndices.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchList.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchList2.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchSheetInsVar.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchType.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchView.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchView.def
# End Source File
# Begin Source File

SOURCE=.\CeWatchView.odl
# End Source File
# Begin Source File

SOURCE=.\CeWatchViewCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\CeWatchViewPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpandArrayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PageInsVarList.cpp
# End Source File
# Begin Source File

SOURCE=.\PageInsVarTree.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ButtonIcon.h
# End Source File
# Begin Source File

SOURCE=.\CeDisplayElement.h
# End Source File
# Begin Source File

SOURCE=.\CeSymbol.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchBackEnd.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchDlg.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchEdit.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchElement.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchIndices.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchList.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchSheetInsVar.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchType.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchView.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchViewCtl.h
# End Source File
# Begin Source File

SOURCE=.\CeWatchViewPpg.h
# End Source File
# Begin Source File

SOURCE=.\ExpandArrayDlg.h
# End Source File
# Begin Source File

SOURCE=.\PageInsVarList.h
# End Source File
# Begin Source File

SOURCE=.\PageInsVarTree.h
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

SOURCE=.\CeWatchView.rc
# End Source File
# Begin Source File

SOURCE=.\CeWatchViewCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\del.ico
# End Source File
# Begin Source File

SOURCE=.\del_d.ico
# End Source File
# Begin Source File

SOURCE=.\delall.ico
# End Source File
# Begin Source File

SOURCE=.\delall_d.ico
# End Source File
# Begin Source File

SOURCE=.\insert.ico
# End Source File
# Begin Source File

SOURCE=.\insert_d.ico
# End Source File
# Begin Source File

SOURCE=.\ObjectTypes.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VarState.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\cewatchview.inf
# End Source File
# Begin Source File

SOURCE=.\WatchFormats.4cw
# End Source File
# End Target
# End Project
