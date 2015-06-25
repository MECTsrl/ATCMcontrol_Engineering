# ATCMcontrol_Engineering
ATCMcontrol Engineering: the PLC SDK

How to install the SDK environment for ATCMcontrol Engineering on Win7 32bit

- Install Visual Studio 6.0 / Microsoft Visual C++ 2998 x86 9.0.21022
- Install Visual Studio 6.0 service pack 6
- Install Doxygen 1.5.6
- create a zero byte "C:\Windows\MSJAVA.DLL" file (DOS command "type nul >> C:\Windows\msjava.dll") 
- Install Microsoft Java SDK 4.0
- Fix Java installation  (add it to PATH and run DOS command "clspack -auto")
- Install MSXML 2.5
- Install ActivePerl 5.8.8 Build 820
- Install IndigoSTAR Perl2Exe V9.100C
- Install the Install Shield Professional 2.03 into C:\InstallShield
- Install the Install Shield LangPackage West Addon
- Install the Install Shield ServicePack

To build the Engineer, from an administrator terminal:
- cd C:\AtcmControl_SDK
- GenKernel.bat 

To build the Target, from an administrator terminal:
- cd C:\AtcmControl_SDK
- GenTarget.bat ATHW119

The setup files are created into C:\ATCMControl_SDK\Installer\RELEASE
