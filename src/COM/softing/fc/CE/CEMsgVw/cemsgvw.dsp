# Microsoft Developer Studio Project File - Name="CEMsgVw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CEMsgVw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cemsgvw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cemsgvw.mak" CFG="CEMsgVw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEMsgVw - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CEMsgVw - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/CEMsgVw", AANFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CEMsgVw - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f cemsgvw.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cemsgvw.exe"
# PROP BASE Bsc_Name "cemsgvw.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f cemsgvw.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cemsgvw.exe"
# PROP Bsc_Name "cemsgvw.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "CEMsgVw - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f cemsgvw.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "cemsgvw.exe"
# PROP BASE Bsc_Name "cemsgvw.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f cemsgvw.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "cemsgvw.exe"
# PROP Bsc_Name "cemsgvw.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "CEMsgVw - Win32 Release"
# Name "CEMsgVw - Win32 Debug"

!IF  "$(CFG)" == "CEMsgVw - Win32 Release"

!ELSEIF  "$(CFG)" == "CEMsgVw - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CEMsgVw.rc
# End Source File
# Begin Source File

SOURCE=.\ImplMsgSink.cpp
# End Source File
# Begin Source File

SOURCE=.\msgdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgRegex.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTabCtr.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Msgthrd.cpp
# End Source File
# Begin Source File

SOURCE=.\msgview.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\targetmsgview.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\CEMsg.IDL
# End Source File
# Begin Source File

SOURCE=..\..\inc\CEMsgViewer.idl
# End Source File
# Begin Source File

SOURCE=..\inc\ImplMsgSink.h
# End Source File
# Begin Source File

SOURCE=..\inc\msgdoc.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgFrame.h
# End Source File
# Begin Source File

SOURCE=..\inc\msgrc.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgRegex.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgTab.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgTabCtr.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgTabWnd.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgThrd.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgView.h
# End Source File
# Begin Source File

SOURCE=..\inc\MsgWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\inc\TargetMsgView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\res\msg_indi.bmp
# End Source File
# Begin Source File

SOURCE=..\res\MsgConnect.bmp
# End Source File
# Begin Source File

SOURCE=..\res\msgind.bmp
# End Source File
# Begin Source File

SOURCE=..\res\msgstate.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CEMsgVw.inf
# End Source File
# End Target
# End Project
