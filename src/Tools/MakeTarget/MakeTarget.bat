@ECHO OFF

setlocal

@ECHO.
@ECHO MakeTarget.bat
@ECHO -------------------------------------------------------------------------


set PRODUCTNAME=%1
set REPLACEFILE=%2

set TARGET_DIR=%PROJECT_ROOT%\Target
set NEW_TARGET_DIR=%TARGET_DIR%\%PRODUCTNAME%
set TEMPLATE_DIR=%PROJECT_ROOT%\Target\Templates

if "%REPLACEFILE%" == "" goto error
if "%PRODUCTNAME%" == "" goto error

@ECHO NEW_TARGET_DIR = %NEW_TARGET_DIR%
@ECHO TEMPLATE_DIR   = %TEMPLATE_DIR


@ECHO.
@ECHO Copy templates ...
@ECHO -------------------------------------------------------------------------
md %NEW_TARGET_DIR%

echo xcopy %TEMPLATE_DIR%\AddOnHandler\*.*  %NEW_TARGET_DIR%\AddOnHandler\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\AddOnHandler\*.*  %NEW_TARGET_DIR%\AddOnHandler\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\CCG\*.*           %NEW_TARGET_DIR%\CCG\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\CCG\*.*           %NEW_TARGET_DIR%\CCG\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\inc\*.*           %NEW_TARGET_DIR%\inc\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\inc\*.*           %NEW_TARGET_DIR%\inc\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\KAD\*.*           %NEW_TARGET_DIR%\KAD\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\KAD\*.*           %NEW_TARGET_DIR%\KAD\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\Libs\*.*          %NEW_TARGET_DIR%\Libs\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\Libs\*.*          %NEW_TARGET_DIR%\Libs\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\Online\*.*        %NEW_TARGET_DIR%\Online\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\Online\*.*        %NEW_TARGET_DIR%\Online\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\Res\*.*           %NEW_TARGET_DIR%\Res\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\Res\*.*           %NEW_TARGET_DIR%\Res\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\Setup\*.*         %NEW_TARGET_DIR%\Setup\*.* /S /Y /Q
     xcopy %TEMPLATE_DIR%\Setup\*.*         %NEW_TARGET_DIR%\Setup\*.* /S /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\[PR].*         %NEW_TARGET_DIR%\%PRODUCTNAME%.* /Y /Q
     xcopy %TEMPLATE_DIR%\[PR].*         %NEW_TARGET_DIR%\%PRODUCTNAME%.* /Y /Q
if errorlevel 1 goto error

echo xcopy %TEMPLATE_DIR%\[PR]_SDK.*     %NEW_TARGET_DIR%\%PRODUCTNAME%_SDK.* /Y /Q
     xcopy %TEMPLATE_DIR%\[PR]_SDK.*     %NEW_TARGET_DIR%\%PRODUCTNAME%_SDK.* /Y /Q
if errorlevel 1 goto error


@ECHO.
@ECHO Create Setup ...
@ECHO -------------------------------------------------------------------------
TAGuidgen.pl %NEW_TARGET_DIR%\Setup\[PR]AddOn.isv > %NEW_TARGET_DIR%\Setup\%PRODUCTNAME%AddOn.isv
if errorlevel 1 goto error

del %NEW_TARGET_DIR%\Setup\[PR]AddOn.isv
if errorlevel 1 goto error


@ECHO.
@ECHO Rename Files ...
@ECHO -------------------------------------------------------------------------
TARename.pl ppp %PRODUCTNAME% %NEW_TARGET_DIR%
if errorlevel 1 goto error

TARename.pl eee %PRODUCTNAME% %NEW_TARGET_DIR%
if errorlevel 1 goto error


@ECHO.
@ECHO Replace Fields ...
@ECHO -------------------------------------------------------------------------
TAReplace.pl %REPLACEFILE% %NEW_TARGET_DIR%
if errorlevel 1 goto error

@ECHO.
@ECHO Clean Up ...
@ECHO -------------------------------------------------------------------------

TADeleteSSFiles.pl %NEW_TARGET_DIR%
if errorlevel 1 goto error

copy %REPLACEFILE% %NEW_TARGET_DIR%
if errorlevel 1 goto error

@ECHO.
@ECHO Done...
@ECHO.

goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo MakeTarget [Target] [ReplaceFile]
echo [Target]:      4CWin 4COsai 4CFC  ...
echo [ReplaceFile]: any filename
echo.

:end

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

endlocal



