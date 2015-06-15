@ECHO OFF

@ECHO.
@ECHO GenInitEnv.bat
@ECHO -------------------------------------------------------------------------

@REM SG_GENDIR     Working directory for compiling.
@REM SG_SETUP      Working directory for setup generation.
@REM SG_TARGET     Target output directory.
@REM SG_SOURCE     Input directory for Kernel/Targetbase sources.
@REM SG_LATEST     Directory to store a copy of the created setups.
@REM SG_SOFTLM     License Manager to be included in the created version.
@REM SG_DRIVE      Drive of the logfile.
@REM SG_DIR        Directory of the logfile.

set PROJECT_ROOT=C:\4C_V220
set TARGET_BASE=%PROJECT_ROOT%\TargetBase
path %path%;%PROJECT_ROOT%\GenTools

if "%SG_GENDIR%" == "" set SG_GENDIR=C:\4Control\GenWork\GenDir.tmp
if "%SG_SETUP%" == ""  set SG_SETUP=C:\4Control\GenWork\SetupDir.tmp

if "%SG_TARGET%" == "" set SG_TARGET=C:\4Control\4C
if "%SG_SOURCE%" == "" set SG_SOURCE=C:\4Control\4C
if "%SG_LATEST%" == "" set SG_LATEST=C:\4Control\4C\V2.20.00

@REM if "%SG_TARGET%" == "" set SG_TARGET=\\sfiler\IA_NO_BACKUP\IAD_4C\4C_VERSION
@REM if "%SG_SOURCE%" == "" set SG_SOURCE=\\sfiler\IA_NO_BACKUP\IAD_4C\4C_VERSION
@REM if "%SG_LATEST%" == "" set SG_LATEST=\\sfiler\IA_NO_BACKUP\IAD_4C\4C_VERSION\_V2.20.00

if "%SG_SOFTLM%" == "" set SG_SOFTLM=C:\4Control\License_Manager\V3.02.0059\AllLanguages
@REM if "%SG_SOFTLM%" == "" set SG_SOFTLM=\\sfiler\iad_Components\License_Manager\V3.02.0059\AllLanguages

@ECHO SG_TARGET      = %SG_TARGET%
@ECHO SG_SOURCE      = %SG_SOURCE%
@ECHO SG_REDIST      = %SG_REDIST%

if "%SG_DRIVE%" == ""   set SG_DRIVE=c:
if "%SG_DIR%" == ""     set SG_DIR=4Control\GenWork\Work


@REM Visual Studio 6.0:
@REM --------------------------------------------------------------------------
@REM Variables out of VCVars32.bat must already be set in the actual Windows 
@REM environment!
@REM path %path%;C:\DevStudio\Common\Tools\WinNT;C:\DevStudio\Common\MSDev98\Bin
@REM path %path%;C:\DevStudio\Common\Tools;C:\DevStudio\VC98\bin
@REM set MSDevDir=C:\DevStudio\Common\MSDev98
@REM set include=C:\DevStudio\VC98\atl\include;C:\DevStudio\VC98\mfc\include;
@REM set include=%include%;C:\DevStudio\VC98\include
@REM set lib=C:\DevStudio\VC98\mfc\lib;C:\DevStudio\VC98\lib

set TOOLS_ROOT=C:\Programmi\Microsoft Visual Studio
set MSSSDIR=%TOOLS_ROOT%\Common\VSS\win32
path %path%;%TOOLS_ROOT%\VB98

@REM Visual Source Safe 6.0
@REM --------------------------------------------------------------------------
rem set ssuser=gen4c
set ssdir=R:\4Control\SSDB
path %path%;C:\Programmi\Microsoft Visual Studio\VSS\win32

@ECHO SSDIR          = %ssdir%

@REM MS Java-SDK 4.0:
@REM --------------------------------------------------------------------------
set include=%include%;C:\Programmi\Microsoft SDK for Java 4.0\Include
set lib=%lib%;C:\Programmi\Microsoft SDK for Java 4.0\Lib\i386
path %path%;C:\Programmi\Microsoft SDK for Java 4.0\Bin

@REM PERL
@REM --------------------------------------------------------------------------
path %path%;C:\Perl\bin\;C:\Perl2Exe

@REM MKS-LEX/YACC:
@REM --------------------------------------------------------------------------
set ROOTDIR=C:\MKS
path %path%;C:\MKS\MKSNT

@REM Install-Shield
@REM --------------------------------------------------------------------------
path %path%;C:\Programmi\File comuni\InstallShield\
path %path%;C:\Programmi\InstallShield\Professional - Windows Installer Edition\System

@REM doxygen
@REM --------------------------------------------------------------------------
path %path%;C:\Programmi\doxygen\bin

set COPYCMD=/Y
