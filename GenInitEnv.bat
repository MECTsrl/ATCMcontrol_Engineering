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



@REM MS Java-SDK 4.0:
@REM --------------------------------------------------------------------------
set include=%include%;%programfiles%\Microsoft SDK for Java 4.0\Include
set lib=%lib%;%programfiles%\Microsoft SDK for Java 4.0\Lib\i386
path %path%;%programfiles%\Microsoft SDK for Java 4.0\Bin

@REM MKS-LEX/YACC:
@REM --------------------------------------------------------------------------
set ROOTDIR=C:\MKS
path %path%;C:\MKS\MKSNT

set COPYCMD=/Y
