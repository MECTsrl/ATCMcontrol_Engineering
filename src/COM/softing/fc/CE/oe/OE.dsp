# Microsoft Developer Studio Project File - Name="OE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=OE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OE.mak" CFG="OE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OE - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "OE - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/oe", QIPFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "OE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f oe.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "oe.exe"
# PROP BASE Bsc_Name "oe.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f oe.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "oe.exe"
# PROP Bsc_Name "oe.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "OE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f oe.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "oe.exe"
# PROP BASE Bsc_Name "oe.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f oe.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "oe.exe"
# PROP Bsc_Name "oe.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "OE - Win32 Release"
# Name "OE - Win32 Debug"

!IF  "$(CFG)" == "OE - Win32 Release"

!ELSEIF  "$(CFG)" == "OE - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EditChroma.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCore.cpp
# End Source File
# Begin Source File

SOURCE=.\EditCut.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFile.cpp
# End Source File
# Begin Source File

SOURCE=.\EditFind.cpp
# End Source File
# Begin Source File

SOURCE=.\EditInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\EditJoinLines.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLine.cpp
# End Source File
# Begin Source File

SOURCE=.\EditPaste.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSplitLine.cpp
# End Source File
# Begin Source File

SOURCE=.\EditUndo.cpp
# End Source File
# Begin Source File

SOURCE=.\OEFindDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OEPageset.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEditCore.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEditCtrlOleDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEditHint.cpp
# End Source File
# Begin Source File

SOURCE=.\SECEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# End Group
# Begin Group "Include Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Inc\editres.h
# End Source File
# Begin Source File

SOURCE=..\Inc\OEdit.h
# End Source File
# Begin Source File

SOURCE=..\Inc\OEFindDlg.h
# End Source File
# Begin Source File

SOURCE=..\Inc\OEPageset.h
# End Source File
# Begin Source File

SOURCE=.\OETempl.h
# End Source File
# Begin Source File

SOURCE=.\regex.h
# End Source File
# Begin Source File

SOURCE=.\regex.inl
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEdit.h
# End Source File
# Begin Source File

SOURCE=.\SECEditCore.inl
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEditCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEditCtrlOleDropTarget.h
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEditHint.h
# End Source File
# Begin Source File

SOURCE=..\Inc\SECEditView.h
# End Source File
# Begin Source File

SOURCE=.\Stdafx.h
# End Source File
# Begin Source File

SOURCE=..\Inc\Struct.h
# End Source File
# Begin Source File

SOURCE=.\View_Core.inl
# End Source File
# Begin Source File

SOURCE=.\ViewBackspace.inl
# End Source File
# Begin Source File

SOURCE=.\ViewCopy.inl
# End Source File
# Begin Source File

SOURCE=.\ViewCursor.inl
# End Source File
# Begin Source File

SOURCE=.\ViewCut.inl
# End Source File
# Begin Source File

SOURCE=.\ViewDelete.inl
# End Source File
# Begin Source File

SOURCE=.\ViewInsert.inl
# End Source File
# Begin Source File

SOURCE=.\ViewJoinLines.inl
# End Source File
# Begin Source File

SOURCE=.\ViewPaint.inl
# End Source File
# Begin Source File

SOURCE=.\ViewPaste.inl
# End Source File
# Begin Source File

SOURCE=.\ViewSelect.inl
# End Source File
# Begin Source File

SOURCE=.\ViewSplitLine.inl
# End Source File
# Begin Source File

SOURCE=.\ViewTab.inl
# End Source File
# End Group
# Begin Source File

SOURCE=..\res\idccurso.cur
# End Source File
# Begin Source File

SOURCE=.\OE.inf
# End Source File
# Begin Source File

SOURCE=..\inc\SecEdit.rc
# End Source File
# Begin Source File

SOURCE=..\res\seceditgutter.bmp
# End Source File
# End Target
# End Project
