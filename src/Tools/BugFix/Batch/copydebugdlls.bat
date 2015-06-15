rem Aufspueren von memory leaks mit Hilfe von fc_todebug
rem kopiert fc_todebug.dll in Engineering\bin

@echo off
if "%PROJECT_ROOT%" == "" goto error

copy fc_todebug.dll %PROJECT_ROOT%\Engineering\bin\fc_todebug.dll
goto finish

:error goto finish

:finish
echo done.