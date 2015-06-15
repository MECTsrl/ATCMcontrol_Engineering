@ECHO OFF

setlocal

@ECHO.
@ECHO Copy4C_V220.bat
@ECHO -------------------------------------------------------------------------

@if "%1" == ""  goto error
@if "%2" == ""  goto error

@REM SS Settings

set TOOLS_ROOT=C:\Programmi\Microsoft Visual Studio
set MSSSDIR=%TOOLS_ROOT%\Common\VSS\win32
set SSPATH=%MSSSDir%
set SSDIR=c:\Softing\SSDB
set SSUSER=paganonim

path %path%;%MSSSDIR%

@ECHO SSPATH = %SSPATH%
@ECHO SSDIR  = %SSDIR%
@ECHO SSUSER = %SSUSER%

set SRCPATH=C:\Softing\4C
@ECHO SRCPATH = %SRCPATH%

set PROJECT_ROOT=C:\4C_V220
set TARGET_BASE=%PROJECT_ROOT%\TargetBase

@ECHO PROJECT_ROOT = %PROJECT_ROOT%
@ECHO TARGET_BASE  = %TARGET_BASE%


@ECHO.
@ECHO Deinstall 4C
@ECHO -------------------------------------------------------------------------

@ECHO call %SRCPATH%\V2.20.00\Setup.exe -ini "Uninstall.ini" -close -uni_all
      call %SRCPATH%\V2.20.00\Setup.exe -ini "Uninstall.ini" -close -uni_all



@ECHO.
@ECHO Delete old files
@ECHO -------------------------------------------------------------------------

if     "%2" == "KEEP"  goto no_delete
if not "%2" == "DEL"   goto error

echo del %PROJECT_ROOT%\Engineering\*.* /Q /F
     del %PROJECT_ROOT%\Engineering\*.* /Q /F >nul

echo del %PROJECT_ROOT%\Engineering\bin\*.* /S /Q /F
     del %PROJECT_ROOT%\Engineering\bin\*.* /S /Q /F >nul

echo del %PROJECT_ROOT%\Engineering\lib\*.* /S /Q /F
     del %PROJECT_ROOT%\Engineering\lib\*.* /S /Q /F >nul

echo del %PROJECT_ROOT%\Pub\*.* /S /Q /F
     del %PROJECT_ROOT%\Pub\*.* /S /Q /F >nul

echo del %PROJECT_ROOT%\COM\*.* /S /Q /F
     del %PROJECT_ROOT%\COM\*.* /S /Q /F >nul

echo del %TARGET_BASE%\*.* /S /Q /F
     del %TARGET_BASE%\*.* /S /Q /F  >nul

:no_delete



@ECHO.
@ECHO Install 4C Embedded
@ECHO -------------------------------------------------------------------------


echo call %SRCPATH%\V2.20.00\Setup.exe -ini "Osai.ini" -close -all -dir "%PROJECT_ROOT%"
     call %SRCPATH%\V2.20.00\Setup.exe -ini "Osai.ini" -close -all -dir "%PROJECT_ROOT%"



@ECHO.
@ECHO Copy TargetBase
@ECHO -------------------------------------------------------------------------

echo xcopy %SRCPATH%\SysgenV2\V2.20.00.%1\4Control\COM\*.* %PROJECT_ROOT%\COM\*.* /S /Y /Q /K /R /E
     xcopy %SRCPATH%\SysgenV2\V2.20.00.%1\4Control\COM\*.* %PROJECT_ROOT%\COM\*.* /S /Y /Q /K /R /E
if errorlevel 1 goto error

echo xcopy %SRCPATH%\SysgenV2\V2.20.00.%1\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E
     xcopy %SRCPATH%\SysgenV2\V2.20.00.%1\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E
if errorlevel 1 goto error

echo xcopy %SRCPATH%\SysgenV2\V2.20.00.%1\4Control\TargetBase\*.* %TARGET_BASE%\*.* /S /Y /Q /K /R /E
     xcopy %SRCPATH%\SysgenV2\V2.20.00.%1\4Control\TargetBase\*.* %TARGET_BASE%\*.* /S /Y /Q /K /R /E
if errorlevel 1 goto error


@ECHO.
@ECHO Get TargetBase from SS
@ECHO -------------------------------------------------------------------------

cd \
cd %TARGET_BASE%
if errorlevel 1 goto error

echo call ss get $/4CReleased/V2.20.00/TargetBase -R -I-y -Yln -GL%TARGET_BASE%
     call ss get $/4CReleased/V2.20.00/TargetBase -R -I-y -Yln -GL%TARGET_BASE%
if errorlevel 1 goto error



@ECHO.
@ECHO Build TargetBase (DEBUG)
@ECHO -------------------------------------------------------------------------

echo make.pl TargetBase_Prima_NoLib.inf
     make.pl TargetBase_Prima_NoLib.inf
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_Prima_NoLib.mak makemake
     nmake /nologo /f TargetBase_Prima_NoLib.mak makemake
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_Prima_NoLib.mak DEBUG=0 clean
     nmake /nologo /f TargetBase_Prima_NoLib.mak DEBUG=0 clean
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_Prima_NoLib.mak DEBUG=1 clean
     nmake /nologo /f TargetBase_Prima_NoLib.mak DEBUG=1 clean
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_Prima_NoLib.mak DEBUG=1 all
     nmake /nologo /f TargetBase_Prima_NoLib.mak DEBUG=1 all
if errorlevel 1 goto error



@ECHO.
@ECHO Get 4CWin from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4CWin md 4CWin

cd 4CWin

echo call ss get $/4CReleased/V2.20.00/Target/4CWin -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CWin
     call ss get $/4CReleased/V2.20.00/Target/4CWin -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CWin
if errorlevel 1 goto error

echo make.pl 4CWin_ext.inf
     make.pl 4CWin_ext.inf
if errorlevel 1 goto error

echo nmake /nologo /f 4CWin_ext.mak makemake
     nmake /nologo /f 4CWin_ext.mak makemake
if errorlevel 1 goto error


@ECHO.
@ECHO Get 4COsai from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4COsai md 4COsai

cd 4COsai

echo call ss get $/AOReleased/4COsai_V2.20.00 -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4COsai
     call ss get $/AOReleased/4COsai_V2.20.00 -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4COsai
if errorlevel 1 goto error

echo make.pl
     make.pl
if errorlevel 1 goto error

echo nmake /nologo /f 4COsai.mak makemake
     nmake /nologo /f 4COsai.mak makemake
if errorlevel 1 goto error


@ECHO.
@ECHO Get 4COsaiCE from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4COsaiCE md 4COsaiCE

cd 4COsaiCE

echo call ss get $/AOReleased/4COsaiCE_V2.20.00 -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4COsaiCE
     call ss get $/AOReleased/4COsaiCE_V2.20.00 -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4COsaiCE
if errorlevel 1 goto error

echo make.pl
     make.pl
if errorlevel 1 goto error

echo nmake /nologo /f 4COsaiCE.mak makemake
     nmake /nologo /f 4COsaiCE.mak makemake
if errorlevel 1 goto error


@ECHO.
@ECHO Get 4CPrima from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4CPrima md 4CPrima

cd 4CPrima

echo call ss get $/AOReleased/4CPrima_V2.20.00 -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CPrima
     call ss get $/AOReleased/4CPrima_V2.20.00 -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CPrima
if errorlevel 1 goto error

echo make.pl
     make.pl
if errorlevel 1 goto error

echo nmake /nologo /f 4CPrima.mak makemake
     nmake /nologo /f 4CPrima.mak makemake
if errorlevel 1 goto error


goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo Copy4C [BuildNr Targetbase] [Overwrite]
echo [TargetBase]:	   3800.r
echo [Overwrite]:      DEL KEEP
echo.

:end

cd \

endlocal
