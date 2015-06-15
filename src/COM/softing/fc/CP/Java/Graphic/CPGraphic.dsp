# Microsoft Developer Studio Project File - Name="CPGraphic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Java Virtual Machine Java Project" 0x0809

CFG=CPGraphic - Java Virtual Machine Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit\
 NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den\
 Befehl
!MESSAGE 
!MESSAGE NMAKE /f "CPGraphic.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "CPGraphic.mak" CFG="CPGraphic - Java Virtual Machine Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "CPGraphic - Java Virtual Machine Release" (basierend auf\
  "Java Virtual Machine Java Project")
!MESSAGE "CPGraphic - Java Virtual Machine Debug" (basierend auf\
  "Java Virtual Machine Java Project")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""$/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic", EBSFAAAA"
# PROP Scc_LocalPath ""
JAVA=jvc.exe

!IF  "$(CFG)" == "CPGraphic - Java Virtual Machine Release"

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

!ELSEIF  "$(CFG)" == "CPGraphic - Java Virtual Machine Debug"

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

# Name "CPGraphic - Java Virtual Machine Release"
# Name "CPGraphic - Java Virtual Machine Debug"
# Begin Source File

SOURCE=.\CPGraphic.inf
# End Source File
# End Target
# End Project
