@ECHO OFF

setlocal

@ECHO.
@ECHO Copy4C.bat
@ECHO -------------------------------------------------------------------------

@if "%1" == ""  goto error
@if "%2" == ""  goto error


@REM SS Settings

set SSPATH=%MSSSDir%
set SSDIR=\\sfiler\IAD_4C_SSDB\ssdb_4cb
set SSUSER=ln

@ECHO SSPATH = %SSPATH%
@ECHO SSDIR  = %SSDIR%
@ECHO SSUSER = %SSUSER%


set SRCPATH=\\w4cgen\4CVersion

@ECHO SRCPATH = %SRCPATH%

set PROJECT_ROOT=D:\4C
set TARGET_BASE=%PROJECT_ROOT%\TargetBase

@ECHO PROJECT_ROOT = %PROJECT_ROOT%
@ECHO TARGET_BASE  = %TARGET_BASE%



@ECHO.
@ECHO Deinstall 4C
@ECHO -------------------------------------------------------------------------

@ECHO call %SRCPATH%\_Latest\Setup.exe -ini "Uninstall.ini" -close -uni_all
      call %SRCPATH%\_Latest\Setup.exe -ini "Uninstall.ini" -close -uni_all



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


echo call %SRCPATH%\_Latest\Setup.exe -ini "Embedded.ini" -close -all -dir "%PROJECT_ROOT%"
     call %SRCPATH%\_Latest\Setup.exe -ini "Embedded.ini" -close -all -dir "%PROJECT_ROOT%"



@ECHO.
@ECHO Copy TargetBase
@ECHO -------------------------------------------------------------------------

echo xcopy %SRCPATH%\SysgenV2\%1\4Control\COM\*.* %PROJECT_ROOT%\COM\*.* /S /Y /Q /K /R /E
     xcopy %SRCPATH%\SysgenV2\%1\4Control\COM\*.* %PROJECT_ROOT%\COM\*.* /S /Y /Q /K /R /E
if errorlevel 1 goto error

echo xcopy %SRCPATH%\SysgenV2\%1\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E
     xcopy %SRCPATH%\SysgenV2\%1\4Control\Pub\*.* %PROJECT_ROOT%\Pub\*.* /S /Y /Q /K /R /E
if errorlevel 1 goto error

echo xcopy %SRCPATH%\SysgenV2\%1\4Control\TargetBase\*.* %TARGET_BASE%\*.* /S /Y /Q /K /R /E
     xcopy %SRCPATH%\SysgenV2\%1\4Control\TargetBase\*.* %TARGET_BASE%\*.* /S /Y /Q /K /R /E
if errorlevel 1 goto error



@ECHO.
@ECHO Get TargetBase from SS
@ECHO -------------------------------------------------------------------------

cd \
cd %TARGET_BASE%
if errorlevel 1 goto error

echo call %SSPATH%\ss get $/4Control%3/TargetBase -R -I-y -Yln -GL%TARGET_BASE%
     call %SSPATH%\ss get $/4Control%3/TargetBase -R -I-y -Yln -GL%TARGET_BASE%
if errorlevel 1 goto error



@ECHO.
@ECHO Build TargetBase (DEBUG)
@ECHO -------------------------------------------------------------------------

echo make.pl TargetBase_NoLib.inf
     make.pl TargetBase_NoLib.inf
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_NoLib.mak makemake
     nmake /nologo /f TargetBase_NoLib.mak makemake
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_NoLib.mak DEBUG=0 clean
     nmake /nologo /f TargetBase_NoLib.mak DEBUG=0 clean
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_NoLib.mak DEBUG=1 clean
     nmake /nologo /f TargetBase_NoLib.mak DEBUG=1 clean
if errorlevel 1 goto error

echo nmake /nologo /f TargetBase_NoLib.mak DEBUG=1 all
     nmake /nologo /f TargetBase_NoLib.mak DEBUG=1 all
if errorlevel 1 goto error


@ECHO.
@ECHO Get 4CWin from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4CWin md 4CWin

cd 4CWin

echo call %SSPATH%\ss get $/4Control%3/Target/4CWin -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CWin
     call %SSPATH%\ss get $/4Control%3/Target/4CWin -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CWin
if errorlevel 1 goto error

echo make.pl
     make.pl
if errorlevel 1 goto error

echo nmake /nologo /f 4CWin.mak makemake
     nmake /nologo /f 4CWin.mak makemake
if errorlevel 1 goto error



@ECHO.
@ECHO Get 4CFC from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4CFC md 4CFC

cd 4CFC

echo call %SSPATH%\ss get $/Adapt/4CFC -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CFC
     call %SSPATH%\ss get $/Adapt/4CFC -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CFC
if errorlevel 1 goto error

echo make.pl
     make.pl
if errorlevel 1 goto error

echo nmake /nologo /f 4CFC.mak makemake
     nmake /nologo /f 4CFC.mak makemake
if errorlevel 1 goto error


@ECHO.
@ECHO Get 4CBC from SS and Build Makefile
@ECHO -------------------------------------------------------------------------

cd \

cd %PROJECT_ROOT%
cd Target

if not exist 4CBC md 4CBC

cd 4CBC

echo call %SSPATH%\ss get $/Adapt/4CBC -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CBC
     call %SSPATH%\ss get $/Adapt/4CBC -R -I-y -Yln -GL%PROJECT_ROOT%\Target\4CBC
if errorlevel 1 goto error

echo make.pl
     make.pl
if errorlevel 1 goto error

echo nmake /nologo /f 4CBC.mak makemake
     nmake /nologo /f 4CBC.mak makemake
if errorlevel 1 goto error




goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo Copy4C [TargetBase] [Overwrite] [Branch] [Build]
echo [TargetBase]:     V2.10.02.3315
echo [Overwrite]:      DEL KEEP
echo.

:end

cd \

endlocal
