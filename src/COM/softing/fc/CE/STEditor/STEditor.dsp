# Microsoft Developer Studio Project File - Name="STEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=STEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "STEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "STEditor.mak" CFG="STEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "STEditor - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "STEditor - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/STEditor", NNPFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "STEditor - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f STEditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "STEditor.exe"
# PROP BASE Bsc_Name "STEditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f STEditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "STEditor.exe"
# PROP Bsc_Name "STEditor.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "STEditor - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f STEditor.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "STEditor.exe"
# PROP BASE Bsc_Name "STEditor.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f STEditor.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "STEditor.exe"
# PROP Bsc_Name "STEditor.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "STEditor - Win32 Release"
# Name "STEditor - Win32 Debug"

!IF  "$(CFG)" == "STEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "STEditor - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\STEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\STEditor.rc
# End Source File
# Begin Source File

SOURCE=.\STEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\STEditorIF.idl

!IF  "$(CFG)" == "STEditor - Win32 Release"

!ELSEIF  "$(CFG)" == "STEditor - Win32 Debug"

# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\STEditorView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\EditorSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=..\inc\key.rc
# End Source File
# Begin Source File

SOURCE=..\inc\keyrc.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PrintOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STEditor.h
# End Source File
# Begin Source File

SOURCE=.\STEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\STEditorView.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\res\idccurso.cur
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\res\seceditgutter.bmp
# End Source File
# Begin Source File

SOURCE=.\res\STEditor.ico
# End Source File
# Begin Source File

SOURCE=.\res\STEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\STEditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\4chkeywords.ini
# End Source File
# Begin Source File

SOURCE=..\..\res\CEBPManError.mc
# End Source File
# Begin Source File

SOURCE=..\..\res\CESTError.mc
# End Source File
# Begin Source File

SOURCE=.\ilkeywords.ini
# End Source File
# Begin Source File

SOURCE=.\STEditor.inf
# End Source File
# Begin Source File

SOURCE=.\STEditor.reg
# End Source File
# Begin Source File

SOURCE=.\STEditorIF.tlb
# End Source File
# Begin Source File

SOURCE=.\stkeywords.ini
# End Source File
# End Target
# End Project
