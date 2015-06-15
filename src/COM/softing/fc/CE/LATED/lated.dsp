# Microsoft Developer Studio Project File - Name="LATED" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=LATED - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lated.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lated.mak" CFG="LATED - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LATED - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "LATED - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/LATED", UFPFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "LATED - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f lated.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "lated.exe"
# PROP BASE Bsc_Name "lated.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f lated.mak DEBUG=0"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "lated.exe"
# PROP Bsc_Name "lated.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "LATED - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f lated.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "lated.exe"
# PROP BASE Bsc_Name "lated.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f lated.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "lated.exe"
# PROP Bsc_Name "lated.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "LATED - Win32 Release"
# Name "LATED - Win32 Debug"

!IF  "$(CFG)" == "LATED - Win32 Release"

!ELSEIF  "$(CFG)" == "LATED - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;r;hpj;bat"
# Begin Source File

SOURCE=.\ChildDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\childfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ipframe.cpp
# End Source File
# Begin Source File

SOURCE=.\LATED.cpp
# End Source File
# Begin Source File

SOURCE=.\lateddoc.cpp
# End Source File
# Begin Source File

SOURCE=.\latedlist.cpp
# End Source File
# Begin Source File

SOURCE=.\latedview.cpp
# End Source File
# Begin Source File

SOURCE=.\lcconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\lcconfiggen.cpp
# End Source File
# Begin Source File

SOURCE=.\lcdatatype.cpp
# End Source File
# Begin Source File

SOURCE=.\lclink.cpp
# End Source File
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\srvritem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\toolbox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\blocksock\blocksock.h
# End Source File
# Begin Source File

SOURCE=.\ChildDialog.h
# End Source File
# Begin Source File

SOURCE=.\childfrm.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\comhelper.h
# End Source File
# Begin Source File

SOURCE=.\ipframe.h
# End Source File
# Begin Source File

SOURCE=.\LATED.h
# End Source File
# Begin Source File

SOURCE=.\lateddefs.h
# End Source File
# Begin Source File

SOURCE=.\lateddoc.h
# End Source File
# Begin Source File

SOURCE=.\latedlist.h
# End Source File
# Begin Source File

SOURCE=.\latedview.h
# End Source File
# Begin Source File

SOURCE=.\lcconfig.h
# End Source File
# Begin Source File

SOURCE=.\lcconfiggen.h
# End Source File
# Begin Source File

SOURCE=.\lcdatatype.h
# End Source File
# Begin Source File

SOURCE=.\lclink.h
# End Source File
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\mfcmap.h
# End Source File
# Begin Source File

SOURCE=.\srvritem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\toolbox.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\XML_Wrapper.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;rc;mc"
# Begin Source File

SOURCE=.\CELATEDError.mc
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LATED.ico
# End Source File
# Begin Source File

SOURCE=.\LATED.rc
# End Source File
# Begin Source File

SOURCE=.\res\LATED.rc2
# End Source File
# Begin Source File

SOURCE=.\LATED.reg
# End Source File
# Begin Source File

SOURCE=.\res\lateddoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\links.bmp
# End Source File
# Begin Source File

SOURCE=.\res\namespac.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\res\wizimage.bmp
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "idl;odl"
# Begin Source File

SOURCE=.\LATEDIF.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\LATED.inf
# End Source File
# End Target
# End Project
