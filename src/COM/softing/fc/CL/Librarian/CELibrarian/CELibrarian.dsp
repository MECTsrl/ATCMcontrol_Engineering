# Microsoft Developer Studio Project File - Name="CELibrarian" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CELibrarian - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CELibrarian.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CELibrarian.mak" CFG="CELibrarian - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CELibrarian - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CELibrarian - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian", CRPFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CELibrarian - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f celibrarian.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "celibrarian.exe"
# PROP BASE Bsc_Name "celibrarian.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f celibrarian.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "celibrarian.exe"
# PROP Bsc_Name "celibrarian.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "CELibrarian - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f celibrarian.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "celibrarian.exe"
# PROP BASE Bsc_Name "celibrarian.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f celibrarian.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "celibrarian.exe"
# PROP Bsc_Name "celibrarian.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "CELibrarian - Win32 Release"
# Name "CELibrarian - Win32 Debug"

!IF  "$(CFG)" == "CELibrarian - Win32 Release"

!ELSEIF  "$(CFG)" == "CELibrarian - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CAboutGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\CEHeaderView.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrarian.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrarian.rc
# End Source File
# Begin Source File

SOURCE=.\CELibrarianDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrarianIF.idl
# End Source File
# Begin Source File

SOURCE=.\CELibrarianPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrarianView.cpp
# End Source File
# Begin Source File

SOURCE=.\CEPassWordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CETextView.cpp
# End Source File
# Begin Source File

SOURCE=.\CETypeId.cpp
# End Source File
# Begin Source File

SOURCE=.\CExEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\CIpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CMainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CSrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\FBListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FBStextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CAboutGeneral.h
# End Source File
# Begin Source File

SOURCE=.\CEHeaderView.h
# End Source File
# Begin Source File

SOURCE=.\CELibrarian.h
# End Source File
# Begin Source File

SOURCE=.\CELibrarianDoc.h
# End Source File
# Begin Source File

SOURCE=.\CELibrarianView.h
# End Source File
# Begin Source File

SOURCE=.\CEPassWordDlg.h
# End Source File
# Begin Source File

SOURCE=.\CETextView.h
# End Source File
# Begin Source File

SOURCE=.\CETypeId.h
# End Source File
# Begin Source File

SOURCE=.\CExEdit.h
# End Source File
# Begin Source File

SOURCE=.\CIpFrame.h
# End Source File
# Begin Source File

SOURCE=.\CListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\CMainFrm.h
# End Source File
# Begin Source File

SOURCE=.\CntrItem.h
# End Source File
# Begin Source File

SOURCE=.\ConstDlg.h
# End Source File
# Begin Source File

SOURCE=.\CSrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\FBListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FBStextDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBox.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CELibrarian.ico
# End Source File
# Begin Source File

SOURCE=.\res\CELibrarian.rc2
# End Source File
# Begin Source File

SOURCE=.\CELibrarian.reg
# End Source File
# Begin Source File

SOURCE=.\res\CELibrarianDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\log4ctr1small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\navmagel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Library"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CELibrary\CEFile.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\CEFile.h
# End Source File
# Begin Source File

SOURCE=..\inc\CEL_Names.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CELibrary.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\CELibrary.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CELibraryException.cpp
# End Source File
# Begin Source File

SOURCE=..\inc\CELibraryException.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CELibraryItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CELibraryItem.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CELibraryList.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CELibraryList.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CEPassWord.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrary\CEPassWord.h
# End Source File
# Begin Source File

SOURCE=..\inc\clsysdef.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\ConstFileParser.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrary\ConstFileParser.h
# End Source File
# Begin Source File

SOURCE=.\CELibrary\ConstList.cpp
# End Source File
# Begin Source File

SOURCE=.\CELibrary\ConstList.h
# End Source File
# Begin Source File

SOURCE=..\inc\resource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CELibrarian.inf
# End Source File
# End Target
# End Project
