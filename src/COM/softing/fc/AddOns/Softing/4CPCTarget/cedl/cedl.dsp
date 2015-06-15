# Microsoft Developer Studio Project File - Name="cedl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=cedl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cedl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cedl.mak" CFG="cedl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cedl - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "cedl - Win32 Release MinSize" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/AddOns/Softing/4CPCTarget/cedl", QQHFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "cedl - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f cedl.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cpcdl.dll"
# PROP BASE Bsc_Name "4cpcdl.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f cedl.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "4cpcdl.dll"
# PROP Bsc_Name "cedl.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\4cpcdl.dll
InputPath=.\4cpcdl.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "cedl - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Cmd_Line "NMAKE /f cedl.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "4cpcdl.dll"
# PROP BASE Bsc_Name "4cpcdl.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Cmd_Line "NMAKE /f cedl.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "4cpcdl.dll"
# PROP Bsc_Name "cedl.bsc"
# PROP Target_Dir ""
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseMinSize
TargetPath=.\4cpcdl.dll
InputPath=.\4cpcdl.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "cedl - Win32 Debug"
# Name "cedl - Win32 Release MinSize"

!IF  "$(CFG)" == "cedl - Win32 Debug"

!ELSEIF  "$(CFG)" == "cedl - Win32 Release MinSize"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cedl.cpp
# End Source File
# Begin Source File

SOURCE=.\cedl.def
# End Source File
# Begin Source File

SOURCE=.\dlcsc.cpp
# End Source File
# Begin Source File

SOURCE=.\dldb.cpp
# End Source File
# Begin Source File

SOURCE=.\dlevnt.cpp
# End Source File
# Begin Source File

SOURCE=.\dlext.cpp
# End Source File
# Begin Source File

SOURCE=.\dlrs.cpp
# End Source File
# Begin Source File

SOURCE=.\dlstat.cpp
# End Source File
# Begin Source File

SOURCE=.\DLStateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlstfire.cpp
# End Source File
# Begin Source File

SOURCE=.\dlstinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\dlstlist.cpp
# End Source File
# Begin Source File

SOURCE=.\DLUpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Downld.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\inc\comhelper.h
# End Source File
# Begin Source File

SOURCE=.\cpcedl.h
# End Source File
# Begin Source File

SOURCE=.\dlcsc.h
# End Source File
# Begin Source File

SOURCE=.\dldefs.h
# End Source File
# Begin Source File

SOURCE=.\dlevnt.h
# End Source File
# Begin Source File

SOURCE=.\dlext.h
# End Source File
# Begin Source File

SOURCE=.\dlrs.h
# End Source File
# Begin Source File

SOURCE=.\dlstat.h
# End Source File
# Begin Source File

SOURCE=.\DLStateDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlstinfo.h
# End Source File
# Begin Source File

SOURCE=.\dlstlist.h
# End Source File
# Begin Source File

SOURCE=.\DLUpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\Downld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\inc\fc_todebug\fc_assert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\inc\fc_tools\fc_defines.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\inc\refobj.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cedl.rc
# End Source File
# Begin Source File

SOURCE=.\CEDLError.mc
# End Source File
# Begin Source File

SOURCE=..\..\..\..\res\csc_onlineerr.mc
# End Source File
# Begin Source File

SOURCE=.\domimage.bmp
# End Source File
# Begin Source File

SOURCE=.\Download.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=..\..\..\..\inc\cedl.idl
# End Source File
# Begin Source File

SOURCE=.\dl.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\cedl.doxy
# End Source File
# Begin Source File

SOURCE=.\cedl.inf
# End Source File
# End Target
# End Project
