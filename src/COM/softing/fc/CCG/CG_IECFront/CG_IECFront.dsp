# Microsoft Developer Studio Project File - Name="CG_IECFront" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=CG_IECFront - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CG_IECFront.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CG_IECFront.mak" CFG="CG_IECFront - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CG_IECFront - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "CG_IECFront - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CCG/CG_IECFront", BOLFAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "CG_IECFront - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f CG_IECFront.mak "DEBUG=0""
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CG_IECFront.dll"
# PROP Bsc_Name "CG_IECFront.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "CG_IECFront - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f CG_IECFront.mak"
# PROP Rebuild_Opt "clean all"
# PROP Target_File "CG_IECFront.dll"
# PROP Bsc_Name "CG_IECFront.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "CG_IECFront - Win32 Release"
# Name "CG_IECFront - Win32 Debug"

!IF  "$(CFG)" == "CG_IECFront - Win32 Release"

!ELSEIF  "$(CFG)" == "CG_IECFront - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CG_CIECFront.cpp
# End Source File
# Begin Source File

SOURCE=.\CG_IECFront.def
# End Source File
# Begin Source File

SOURCE=.\CG_IECFront.inf
# End Source File
# Begin Source File

SOURCE=.\cgmem.cpp
# End Source File
# Begin Source File

SOURCE=.\COMoverhead.cpp
# End Source File
# Begin Source File

SOURCE=.\configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\declcheck.cpp
# End Source File
# Begin Source File

SOURCE=.\expr.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash4GR.cpp
# End Source File
# Begin Source File

SOURCE=.\helper.cpp
# End Source File
# Begin Source File

SOURCE=.\LDFBD.cpp
# End Source File
# Begin Source File

SOURCE=.\LDFBDTools.cpp
# End Source File
# Begin Source File

SOURCE=.\literals.cpp
# End Source File
# Begin Source File

SOURCE=.\newdel.cpp
# End Source File
# Begin Source File

SOURCE=.\parse4CP.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse4GR.cpp
# End Source File
# Begin Source File

SOURCE=.\parseKAD.cpp
# End Source File
# Begin Source File

SOURCE=.\parsers.cpp
# End Source File
# Begin Source File

SOURCE=.\SFCChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\typeset.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLparse.cpp
# End Source File
# Begin Source File

SOURCE=.\XREFgen.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "global_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\CG_IECFront.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CG_CIECFront.h
# End Source File
# Begin Source File

SOURCE=.\cgmem.h
# End Source File
# Begin Source File

SOURCE=.\configuration.h
# End Source File
# Begin Source File

SOURCE=.\declcheck.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\expr.h
# End Source File
# Begin Source File

SOURCE=.\FeatureSet.h
# End Source File
# Begin Source File

SOURCE=.\Hash4GR.h
# End Source File
# Begin Source File

SOURCE=.\helper.h
# End Source File
# Begin Source File

SOURCE=.\LDFBD.h
# End Source File
# Begin Source File

SOURCE=.\LDFBDTools.h
# End Source File
# Begin Source File

SOURCE=.\literals.h
# End Source File
# Begin Source File

SOURCE=.\parse4CP.h
# End Source File
# Begin Source File

SOURCE=.\Parse4GR.h
# End Source File
# Begin Source File

SOURCE=.\parseKAD.h
# End Source File
# Begin Source File

SOURCE=.\parsers.h
# End Source File
# Begin Source File

SOURCE=.\SFCChecker.h
# End Source File
# Begin Source File

SOURCE=.\typeset.h
# End Source File
# Begin Source File

SOURCE=.\XMLparse.h
# End Source File
# Begin Source File

SOURCE=.\XREFgen.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CG_Messages.h
# End Source File
# Begin Source File

SOURCE=.\CG_Messages.mc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "old"

# PROP Default_Filter ""
# End Group
# Begin Group "yacc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\il_ParseContext.cpp
# End Source File
# Begin Source File

SOURCE=.\il_ParseContext.h
# End Source File
# Begin Source File

SOURCE=.\st.y
# End Source File
# Begin Source File

SOURCE=.\st_ParseContext.cpp
# End Source File
# Begin Source File

SOURCE=.\st_ParseContext.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\tmp.txt
# End Source File
# End Target
# End Project
