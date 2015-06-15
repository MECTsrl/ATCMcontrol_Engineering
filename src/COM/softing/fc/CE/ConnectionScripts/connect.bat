@setlocal
@set CURRENT=%~d0%~p0
@set HMI_BIN_PATH=%1
@set HMI_BIN_FILE=%2
@cd /D %CURRENT%

@set CURRENT_IP=192.168.0.24
@echo cd /local/root > cmdftp.txt
@echo lcd %HMI_BIN_PATH% >> cmdftp.txt
@echo put %HMI_BIN_FILE% >> cmdftp.txt
@echo ren %HMI_BIN_FILE% hmi >> cmdftp.txt
@echo bye >> cmdftp.txt
echo yes | plink.exe -m script1.txt %CURRENT_IP% -l root -pw root 
ftp -s:cmdftp.txt -n %CURRENT_IP%
echo yes | plink.exe -m script2.txt %CURRENT_IP% -l root -pw root

@endlocal
