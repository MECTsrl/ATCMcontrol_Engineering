# Microsoft Developer Studio Project File - Name="Setup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Setup - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Setup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Setup.mak" CFG="Setup - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Setup - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "Setup - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup", OGTFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Setup - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f Setup.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "Setup.exe"
# PROP BASE Bsc_Name "Setup.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f Setup.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "Setup.exe"
# PROP Bsc_Name "Setup.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Setup - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f Setup.mak"
# PROP BASE Rebuild_Opt "clean all"
# PROP BASE Target_File "Setup.exe"
# PROP BASE Bsc_Name "Setup.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f Setup.mak /nologo"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "Setup.exe"
# PROP Bsc_Name "Setup.bsc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Setup - Win32 Release"
# Name "Setup - Win32 Debug"

!IF  "$(CFG)" == "Setup - Win32 Release"

!ELSEIF  "$(CFG)" == "Setup - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Install.cpp
# End Source File
# Begin Source File

SOURCE=.\PropBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PropInstall.cpp
# End Source File
# Begin Source File

SOURCE=.\PropLangPath.cpp
# End Source File
# Begin Source File

SOURCE=.\PropLicense.cpp
# End Source File
# Begin Source File

SOURCE=.\PropSelEmb.cpp
# End Source File
# Begin Source File

SOURCE=.\PropSelPC.cpp
# End Source File
# Begin Source File

SOURCE=.\PropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\PropWelcome.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\Setup.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\Install.h
# End Source File
# Begin Source File

SOURCE=.\PropBase.h
# End Source File
# Begin Source File

SOURCE=.\PropInstall.h
# End Source File
# Begin Source File

SOURCE=.\PropLangPath.h
# End Source File
# Begin Source File

SOURCE=.\PropLicense.h
# End Source File
# Begin Source File

SOURCE=.\PropSelEmb.h
# End Source File
# Begin Source File

SOURCE=.\PropSelPC.h
# End Source File
# Begin Source File

SOURCE=.\PropSheet.h
# End Source File
# Begin Source File

SOURCE=.\PropWelcome.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Setup.h
# End Source File
# Begin Source File

SOURCE=.\SetupDef.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\inst_4ca.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_4ck.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_c1.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_c2.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_c3.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_c4.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_c5.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_ie.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_iis.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_j.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_lm.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_os.ico
# End Source File
# Begin Source File

SOURCE=.\res\inst_sp.ico
# End Source File
# Begin Source File

SOURCE=.\res\S1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\S2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\S3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\S4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\S5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Setup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Setup.ico
# End Source File
# Begin Source File

SOURCE=.\res\Setup.rc2
# End Source File
# Begin Source File

SOURCE=.\res\welcome.bmp
# End Source File
# End Group
# Begin Group "License Files"

# PROP Default_Filter "*.rtf"
# Begin Source File

SOURCE=.\License\License.rtf
# End Source File
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter "*.ini"
# Begin Group "de"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ini\de\Language.ini
# End Source File
# End Group
# Begin Group "en"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ini\en\Language.ini
# End Source File
# End Group
# Begin Group "it"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ini\it\Language.ini
# End Source File
# End Group
# Begin Group "pl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ini\pl\Language.ini
# End Source File
# End Group
# Begin Source File

SOURCE=.\ini\CMZ.ini
# End Source File
# Begin Source File

SOURCE=.\ini\DC.ini
# End Source File
# Begin Source File

SOURCE=.\ini\Demo.ini
# End Source File
# Begin Source File

SOURCE=.\ini\EKON.ini
# End Source File
# Begin Source File

SOURCE=.\ini\Embedded.ini
# End Source File
# Begin Source File

SOURCE=.\ini\FCBC.ini
# End Source File
# Begin Source File

SOURCE=.\ini\MBS.ini
# End Source File
# Begin Source File

SOURCE=.\ini\OSAI.ini
# End Source File
# Begin Source File

SOURCE=".\ini\PC-Based.ini"
# End Source File
# Begin Source File

SOURCE=.\ini\PPCCE.ini
# End Source File
# Begin Source File

SOURCE=.\ini\SDK.ini
# End Source File
# Begin Source File

SOURCE=.\ini\Setup.ini
# End Source File
# Begin Source File

SOURCE=.\ini\STS.ini
# End Source File
# Begin Source File

SOURCE=.\ini\Uninstall.ini
# End Source File
# End Group
# Begin Group "Bat Files"

# PROP Default_Filter "*-bat"
# Begin Source File

SOURCE=.\bat\Setup_CMZ.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_DC.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_Demo.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_Embedded.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_FCBC.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_OSAI.bat
# End Source File
# Begin Source File

SOURCE=".\bat\Setup_PC-Based.bat"
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_PPCCE.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Setup_STS.bat
# End Source File
# Begin Source File

SOURCE=.\bat\Uninstall.bat
# End Source File
# End Group
# Begin Source File

SOURCE=.\Setup.inf
# End Source File
# End Target
# End Project
