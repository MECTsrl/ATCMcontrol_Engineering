# ATCMcontrol_Engineering
ATCMcontrol Engineering: the PLC SDK

How to install the SDK environment for ATCMcontrol Engineering

- Install Visual Studio 6.0 / Microsoft Visual C++ 2998 x86 9.0.21022
- Install Visual Studio 6.0 service pack 6
- Install Doxygen 1.5.6
- Install Microsoft Java SDK 4.0
- Install com.ms.xml.* classes
- Install ActivePerl 5.8.8 Build 820
- Install IndigoSTAR Perl2Exe V9.100C
- Install the Install Shield Professional 2.03 into C:\InstallShield
- Install the Install Shield LangPackage West Addon
- Install the Install Shield ServicePack
- Copy the dll msxml.dll into c:\windows\system32

To build the Engineer, from an administrator terminal:
	GenKernel.bat 

To build the Target, from an administrator terminal:
	cd C:\AtcmControl_SDK
	GenTarget.bat <target name>

The setup files are created into C:\ATCMControl_SDK\Installer\RELEASE
