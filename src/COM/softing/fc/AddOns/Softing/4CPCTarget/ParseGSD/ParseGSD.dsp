# Microsoft Developer Studio Project File - Name="ParseGSD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ParseGSD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ParseGSD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ParseGSD.mak" CFG="ParseGSD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ParseGSD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ParseGSD - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/AddOns/Softing/4CPCTarget/ParseGSD", LXKFAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ParseGSD - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "DObj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /i "..\..\res" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\bin\eng\ParseGSD.dll" /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=\su\Projekte\4C2\COM\softing\bin\eng\ParseGSD.dll
InputPath=\su\Projekte\4C2\COM\softing\bin\eng\ParseGSD.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ParseGSD - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /i "..\..\res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"ParseGSD.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\bin\eng\ParseGSD.dll"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseMinDependency
TargetPath=\su\Projekte\4C2\COM\softing\bin\eng\ParseGSD.dll
InputPath=\su\Projekte\4C2\COM\softing\bin\eng\ParseGSD.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ParseGSD - Win32 Debug"
# Name "ParseGSD - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;r;hpj;bat"
# Begin Source File

SOURCE=.\DP_ExtParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DP_Module.cpp
# End Source File
# Begin Source File

SOURCE=.\dp_physicalinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\dp_slot.cpp
# End Source File
# Begin Source File

SOURCE=.\DP_Station.cpp
# End Source File
# Begin Source File

SOURCE=.\dp_unitdiag.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtParamDB.cpp
# End Source File
# Begin Source File

SOURCE=.\gsd_Keyw.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseGSD.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseGSD.def
# End Source File
# Begin Source File

SOURCE=.\ParseGsdFile.cpp
# End Source File
# Begin Source File

SOURCE=.\parsemessage.cpp
# End Source File
# Begin Source File

SOURCE=.\parser.y

!IF  "$(CFG)" == "ParseGSD - Win32 Debug"

# Begin Custom Build - Performing YACC Step
IntDir=.\DObj
InputPath=.\parser.y
InputName=parser

BuildCmds= \
	%ROOTDIR%\mksnt\yacc.exe -m -V $(IntDir)\yystats.txt -LC -o $(InputName).cpp          -D $(InputName).h $(InputName).y

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ParseGSD - Win32 Release MinDependency"

# Begin Custom Build - Performing YACC Step
IntDir=.\ReleaseMinDependency
InputPath=.\parser.y
InputName=parser

BuildCmds= \
	%ROOTDIR%\mksnt\yacc.exe -m -V $(IntDir)\yystats.txt -LC -o $(InputName).cpp          -D $(InputName).h $(InputName).y

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PrmTextDB.cpp
# End Source File
# Begin Source File

SOURCE=.\scanner.l

!IF  "$(CFG)" == "ParseGSD - Win32 Debug"

# Begin Custom Build - Performing LEX Step
InputPath=.\scanner.l
InputName=scanner

BuildCmds= \
	%ROOTDIR%\mksnt\lex.exe -v -a -LC -o $(InputName).cpp -D $(InputName).h          $(InputName).l

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ParseGSD - Win32 Release MinDependency"

# Begin Custom Build - Performing LEX Step
InputPath=.\scanner.l
InputName=scanner

BuildCmds= \
	%ROOTDIR%\mksnt\lex.exe -v -a -LC -o $(InputName).cpp -D $(InputName).h          $(InputName).l

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\comcoll.h
# End Source File
# Begin Source File

SOURCE=.\defs.h
# End Source File
# Begin Source File

SOURCE=.\DP_ExtParam.h
# End Source File
# Begin Source File

SOURCE=.\DP_Module.h
# End Source File
# Begin Source File

SOURCE=.\dp_physicalinterface.h
# End Source File
# Begin Source File

SOURCE=.\dp_slot.h
# End Source File
# Begin Source File

SOURCE=.\DP_Station.h
# End Source File
# Begin Source File

SOURCE=.\dp_unitdiag.h
# End Source File
# Begin Source File

SOURCE=.\ExtParamDB.h
# End Source File
# Begin Source File

SOURCE=.\gsd_Keyw.h
# End Source File
# Begin Source File

SOURCE=.\gsd_Parser.h
# End Source File
# Begin Source File

SOURCE=.\gsd_Scan.h
# End Source File
# Begin Source File

SOURCE=.\ParseGsdFile.h
# End Source File
# Begin Source File

SOURCE=.\parsemessage.h
# End Source File
# Begin Source File

SOURCE=.\PrmTextDB.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe;rc;mc;rgs"
# Begin Source File

SOURCE=.\gsdmessage.mc
# End Source File
# Begin Source File

SOURCE=.\ParseGSD.rc
# End Source File
# Begin Source File

SOURCE=.\ParseGsdFile.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VERSION.H
# End Source File
# End Group
# Begin Group "Intermediates"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\parser.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\scanner.cpp
# ADD CPP /W1
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=.\pgsd.idl
# End Source File
# End Group
# Begin Source File

SOURCE=.\ParseGSD.inf
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
