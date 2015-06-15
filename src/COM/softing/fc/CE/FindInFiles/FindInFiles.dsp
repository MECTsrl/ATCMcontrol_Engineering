# Microsoft Developer Studio Project File - Name="FindInFiles" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=FindInFiles - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FindInFiles.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FindInFiles.mak" CFG="FindInFiles - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FindInFiles - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CE/FindInFiles", NQNFAAAA"
# PROP Scc_LocalPath "."
# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f FindInFiles.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "FindInFiles.exe"
# PROP BASE Bsc_Name "FindInFiles.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f FindInFiles.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "FindInFiles.exe"
# PROP Bsc_Name "FindInFiles.bsc"
# PROP Target_Dir ""
LIB32=link.exe -lib
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\FindInFiles.exe
InputPath=.\FindInFiles.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build
# Begin Target

# Name "FindInFiles - Win32 Debug"

!IF  "$(CFG)" == "FindInFiles - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CEFIFComObj.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFThread.cpp
# End Source File
# Begin Source File

SOURCE=.\FindInFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\FindInFiles.def
# End Source File
# Begin Source File

SOURCE=.\FindInFiles.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CEFIFComObj.h
# End Source File
# Begin Source File

SOURCE=.\FIFThread.h
# End Source File
# Begin Source File

SOURCE=.\FindInFilesCP.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CEFIFComObj.rgs
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\FindInFiles.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\FindInFiles.inf
# End Source File
# End Target
# End Project
