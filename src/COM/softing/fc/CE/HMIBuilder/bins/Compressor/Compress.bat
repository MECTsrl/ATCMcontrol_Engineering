@ECHO OFF
setlocal

SET FILE_SOURCE="%1"
SET FILE_DESTINATION="%2"
SET PROJECT_NAME=%3

if "%FILE_SOURCE%"=="" goto invokerror
if "%FILE_DESTINATION%"=="" goto invokerror

echo.
echo Compressing HMI file %PROJECT_NAME%....
echo.

.\7Zip\7z.exe a -y -tZip "%FILE_DESTINATION%" "%FILE_SOURCE%" >Compress.log 2>&1
if errorlevel 1 goto error
del /s Compress.log >nul 2>&1
goto end

:invokerror
echo.
echo Parameters error, see the correct execution below:
echo.
echo 	compress ^<source_file^> ^<destination_file^>
echo.
goto end

:error
echo.
echo Error during compression process.
echo See Compress.log to more information.
echo.
goto end

:end
endlocal