@echo off
setlocal

echo.
echo ------------------------------------------------------------------------- 
echo Start HMI build process....
echo -------------------------------------------------------------------------
echo.

:hmi_build_process
set ORIGINAL_PATH=%CD%

cd /D "%ORIGINAL_PATH%\machine"
set MACHINE_DIR=%CD%

echo.
echo Compiling HMI projects, please wait for finish....
echo.

call bin\run.bat

cd /D "%ORIGINAL_PATH%"

echo.
echo HMI projects were built.
echo See each HMI project Build.log for more details.
echo In case of error, read Build_Errors.log.
echo.

:generate_hmi_files
echo.
echo ------------------------------------------------------------------------- 
echo Generating HMI files...
echo.
echo Reading folders in HMI_PROJECTS
echo ------------------------------------------------------------------------- 
echo.

cd /D "%ORIGINAL_PATH%\HMI_PROJECTS"
set HMI_PROJECTS_PATH=%CD%

for /d %%d in (*) do call :process_hmi_project %%d
goto integrate_hmi_files

:process_hmi_project
set ACTUAL_HMI_PROJECT_DIR="%CD%\%1"
set ACTUAL_PROJECT_NAME=%1
set DESTINATION_HMI="%HMI_PROJECTS_PATH%\%ACTUAL_PROJECT_NAME%.hmi"

if not exist %ACTUAL_HMI_PROJECT_DIR%\img ( md %ACTUAL_HMI_PROJECT_DIR%\img )
if not exist %ACTUAL_HMI_PROJECT_DIR%\bin\hmi goto hmi_not_compiled_error
if not exist %ACTUAL_HMI_PROJECT_DIR%\manifest.xml goto error_manifest

:compress_process
cd ..\bins\Compressor\
call Compress %ACTUAL_HMI_PROJECT_DIR% %DESTINATION_HMI% %ACTUAL_PROJECT_NAME%
if errorlevel 1 goto compress_error
cd /D %HMI_PROJECTS_PATH%
rmdir /Q /S %ACTUAL_HMI_PROJECT_DIR%
goto end

:integrate_hmi_files
echo.
echo ------------------------------------------------------------------------- 
echo Integrating HMI files...
echo.
echo Reading compiled HMI files...
echo ------------------------------------------------------------------------- 
echo.

cd /D "%HMI_PROJECTS_PATH%"

for %%a in (*.hmi) do call :proccess_file %%a
goto success

:proccess_file
set ACTUAL_HMI_FILE=%HMI_PROJECTS_PATH%\%1
set HMI_FILE_NAME=%1
set TEMP_HMI_DIR=%ACTUAL_HMI_FILE%_tmp
cd ..\bins\Compressor\
call Uncompress "%ACTUAL_HMI_FILE%" "%TEMP_HMI_DIR%" %HMI_FILE_NAME%
if errorlevel 1 goto uncompress_error
cd /D "%TEMP_HMI_DIR%"
REM ..\..\bins\xml.exe sel -t -v "/hmiproject/targets/target" %HMI_FILE_NAME%\manifest.xml > temp
REM if errorlevel 1 goto error
REM set /p TARGET= < temp
REM del temp
set TARGET=FCTPAC020_01

if not exist ..\..\..\%TARGET% goto target_not_exist

xcopy /Q /S /Y "%ACTUAL_HMI_FILE%" ..\..\..\%TARGET%\Hmi
del /F /S /Q  "%ACTUAL_HMI_FILE%"
cd /D "%HMI_PROJECTS_PATH%"
rd /Q /S "%TEMP_HMI_DIR%"
goto end


:target_not_exist
echo.
echo ERROR on integrate %HMI_FILE_NAME%
echo -------------------------------------------------------------------------
echo The target %TARGET% is not installed. 
echo Please install it before compile this HMI project.
echo -------------------------------------------------------------------------
echo.
cd /D "%HMI_PROJECTS_PATH%"
rd /Q /S "%TEMP_HMI_DIR%"
pause
goto end

:uncompress_error
echo.
echo ERROR
echo -------------------------------------------------------------------------
echo Some error occured during files processing
echo See bins\Compressor\Uncompress.log for more info
echo -------------------------------------------------------------------------
echo.
pause
goto end

:compress_error
echo.
echo ERROR
echo -------------------------------------------------------------------------
echo Some error occured during files processing
echo See bins\Compressor\Compress.log for more info
echo -------------------------------------------------------------------------
echo.
pause
goto end


:error_manifest
echo.
echo ERROR on %ACTUAL_PROJECT_NAME%
echo -------------------------------------------------------------------------
echo The file manifest.xml was not found in HMI project folder %ACTUAL_PROJECT_NAME%
echo Please, copy the base file from %ORIGINAL_PATH%\Template\manifest.xml
echo and edit it for describe your HMI program.
echo -------------------------------------------------------------------------
echo.
cd /D "%HMI_PROJECTS_PATH%"
rd /Q /S "%TEMP_HMI_DIR%"
pause
goto end


:hmi_not_compiled_error
echo.
echo ERROR on %ACTUAL_PROJECT_NAME%
echo -------------------------------------------------------------------------
echo HMI Project %ACTUAL_PROJECT_NAME% was not compiled successful!
echo HMI file for this project will not be generated.
echo See %HMI_PROJECTS_PATH%\%ACTUAL_PROJECT_NAME%\Build_Errors.log
echo for more informations.
echo -------------------------------------------------------------------------
echo.
pause
goto end

:success
echo.
echo -------------------------------------------------------------------------
echo All generated .hmi files are located in properly terget folder. 
echo Now you can upload them from ATCMControl Upload HMI tool.
echo -------------------------------------------------------------------------
echo.
pause
goto end



:end
endlocal

