@ECHO OFF
setlocal

SET FILE_SOURCE="%1"
SET FILE_DESTINATION="%2"
SET PROJECT_NAME=%3

if "%FILE_SOURCE%"=="" goto invokerror
if "%FILE_DESTINATION%"=="" goto invokerror

echo.
echo Uncompressing HMI file %PROJECT_NAME%....
echo.

.\7Zip\7z.exe x -y "%FILE_SOURCE%" -o"%FILE_DESTINATION%"  >Uncompress.log 2>&1
if errorlevel 1 goto error
del /s Uncompress.log >nul 2>&1
goto end

:invokerror
echo.
echo Parameters error, see the correct execution below:
echo.
echo 	uncompress ^<destination_file^> ^<source_file^>
echo.
goto end

:error
echo.
echo Error during compression process.
echo See Uncompress.log to more information.
echo.
goto end

:end
endlocal