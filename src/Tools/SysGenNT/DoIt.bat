@ECHO OFF

setlocal

@ECHO.
@ECHO DoIt.bat
@ECHO -------------------------------------------------------------------------

set DOIT_BUILD=%1

if "%DOIT_BUILD%" == "" goto error
if "%2" == "" goto error

call GenTarget 4CDemo V2.20.00.%DOIT_BUILD% V2.20.00 %2
call GenTarget 4CFC   V2.20.00.%DOIT_BUILD% V2.20.00 %2
call GenTarget 4CBC   V2.20.00.%DOIT_BUILD% V2.20.00 %2
call GenTarget 4CXC   V2.20.00.%DOIT_BUILD% V2.20.00 %2
REM call GenTarget 4CGA   V2.20.00.%DOIT_BUILD% V2.20.00 %2

goto end

@REM --------------------------------------------------------------------------


:error

echo.
echo -------  E R R O R  -------
echo.
echo Doit [Build] [What]
echo.

:end

%SG_DRIVE%
cd %SG_DRIVE%\%SG_DIR%

endlocal
