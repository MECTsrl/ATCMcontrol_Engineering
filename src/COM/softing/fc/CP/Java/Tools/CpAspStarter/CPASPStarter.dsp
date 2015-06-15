# Microsoft Developer Studio Project File - Name="CPASPStarter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Java Virtual Machine Java Project" 0x0809

CFG=CPASPStarter - Java Virtual Machine Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit\
 NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den\
 Befehl
!MESSAGE 
!MESSAGE NMAKE /f "CPASPStarter.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "CPASPStarter.mak"\
 CFG="CPASPStarter - Java Virtual Machine Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "CPASPStarter - Java Virtual Machine Release" (basierend auf\
  "Java Virtual Machine Java Project")
!MESSAGE "CPASPStarter - Java Virtual Machine Debug" (basierend auf\
  "Java Virtual Machine Java Project")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Tools/CpAspStarter", BHSFAAAA"
# PROP Scc_LocalPath ""
JAVA=jvc.exe

!IF  "$(CFG)" == "CPASPStarter - Java Virtual Machine Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /O
# ADD JAVA /O

!ELSEIF  "$(CFG)" == "CPASPStarter - Java Virtual Machine Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE JAVA /g
# ADD JAVA /w4 /g

!ENDIF 

# Begin Target

# Name "CPASPStarter - Java Virtual Machine Release"
# Name "CPASPStarter - Java Virtual Machine Debug"
# Begin Group "Source Files"

# PROP Default_Filter "java;html"
# Begin Source File

SOURCE=.\CP_ASPStarter.java
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
