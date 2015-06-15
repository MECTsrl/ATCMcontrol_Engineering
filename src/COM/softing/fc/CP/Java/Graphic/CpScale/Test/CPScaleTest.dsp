# Microsoft Developer Studio Project File - Name="CPScaleTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Java Virtual Machine Java Project" 0x0809

CFG=CPScaleTest - Java Virtual Machine Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit\
 NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den\
 Befehl
!MESSAGE 
!MESSAGE NMAKE /f "CPScaleTest.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "CPScaleTest.mak"\
 CFG="CPScaleTest - Java Virtual Machine Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "CPScaleTest - Java Virtual Machine Release" (basierend auf\
  "Java Virtual Machine Java Project")
!MESSAGE "CPScaleTest - Java Virtual Machine Debug" (basierend auf\
  "Java Virtual Machine Java Project")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/Test", MFSFAAAA"
# PROP Scc_LocalPath ""
JAVA=jvc.exe

!IF  "$(CFG)" == "CPScaleTest - Java Virtual Machine Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\class"
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /O
# ADD JAVA /O

!ELSEIF  "$(CFG)" == "CPScaleTest - Java Virtual Machine Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\class"
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /g
# ADD JAVA /g

!ENDIF 

# Begin Target

# Name "CPScaleTest - Java Virtual Machine Release"
# Name "CPScaleTest - Java Virtual Machine Debug"
# Begin Source File

SOURCE=.\CP_Debug.bat
# End Source File
# Begin Source File

SOURCE=.\CP_Debug.pl
# End Source File
# Begin Source File

SOURCE=.\CP_GrScaleTest.java
# End Source File
# Begin Source File

SOURCE=.\CP_GrScaleTestFrame.java
# End Source File
# Begin Source File

SOURCE=.\CPScaleTest.html
# End Source File
# End Target
# End Project
