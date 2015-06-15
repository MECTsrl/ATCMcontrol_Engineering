(*[[
]]*)
FUNCTION_BLOCK ModBusLineReceive

VAR_INPUT
	ExecMode   		: INT;
	slaveAddress    : WORD;
	respTimeOut     : INT;
	Line			: INT;
END_VAR

VAR_OUTPUT
	ExecStatus 		: DWORD;
	RetC			: DWORD;
	Length   		: INT;
	Buffer   		: ARRAY [0..128] OF BYTE;
END_VAR

VAR
timeOutResp     :   BOOL;                           (* Timeout expired *)
responseOK      :   BOOL;
pos             :   DINT := 0;
posRW           :   DINT := 0;
numData         :   INT;
waitResp        :   TIME;
waitETN         :   TIME;
timerWait       :   TON;
bufferLen       :   INT;
tmpLen          :   INT;
getCRC          :   WORD;
readCRC         :   WORD;
retCRC			:	DWORD;
turnArDel       :   INT     := 200;                 (* Value is from 100ms to 200ms *)
startTime       :   BOOL    := TRUE;                (* Enable start timer *)
bufferGet       :   ARRAY [0..128] OF BYTE;         (* Buffer is used to send request *)
bufferSave      :   ARRAY [0..128] OF BYTE;
lenBufSave      :   INT := 0;
indFor          :   INT;
RetCStatus      :   DWORD;
Status			:	SerialLineSTS_struct;
END_VAR

(* When a broadcast request is sent on the serial line, no response is returned from the slaves.
   Nevertheless a delay is respected by the Master in order to allow any slave to process 
   request before sending a new one. This delay is called "Turnaround delay". 
   Value is from 100ms to 200ms. In unicast the response timeout must be set long enough 
   for any slave to process the request and return response. Value is form 1s to several second.
*)
responseOK := false;

if (slaveAddress = 0) then
    waitResp := INT_TO_TIME(turnArDel);     (* Function broadcast *)
else
    waitResp := INT_TO_TIME(respTimeOut);   (* Function unicast *)
end_if;

if (slaveAddress <> 0) then
    (* Start timer wait timeout response *)
    (* Activate timer and check timeout response on rs232 *)
    while (not responseOK) do
        timerWait(IN := startTime, PT := waitResp, Q => timeOutResp, ET => waitETN);

		tmpLen := 128;
        (* Read byte from serial line *)
		RetC := SerialLineReceive(ExecMode, 
							  	     ExecStatus, 
							  	     Line, 
							  	     tmpLen, 
							  	     bufferGet[0]);

		if (RetC = 0) then
			RetCStatus := SerialLineStatus(Line, Status);
			RetC := Status.RXError;
		end_if;

        if ((RetC <> 16#00310032)  and (RetC <> 16#0)) then
			timerWait(IN := FALSE, PT := waitResp, Q => timeOutResp, ET => waitETN);
            goto ERROR_GETRX;
        end_if;

        (* Put byte on bufferSave temp *)
        if (tmpLen <> 0 and (RetC = 16#0)) then
            indFor := tmpLen;
            indFor := indFor - 1;
            for numData := 0 to indFor by 1 do
                if (posRW < 128) then
                    bufferSave[posRW] := bufferGet[numData];
                    posRW := posRW + 1;
                end_if;
            end_for;
        end_if;

        (* Total number of bytes read *)
        lenBufSave := DINT_TO_INT(posRW);
    
        (* Check the slaveAddress *)
        if (bufferSave[0] = WORD_TO_BYTE(slaveAddress)) then
            (* In this case, the timeout serial line isn't error *)
            bufferLen := lenBufSave;        (* Copy tmpLen in bufferLen *)
            bufferLen := SUB(bufferLen, 2); (* Remove last two byte (checksum) *)
            if (bufferLen > 0) then
                (* Calsulate the checksum *)
				retCRC := ModBusCRC16(bufferLen, bufferSave[0], getCRC);
                pos := INT_TO_DINT(bufferLen);  (* Read checksum from response *)
				readCRC := COPY_WORD_FROM_BYTE_ARRAY(pos, bufferSave); (* Returns WORD *)
                (* Check CRC *)
                if (getCRC <> readCRC) then
                    RetC := 0;
                else
                    RetC := 0;
                    bufferLen := SUB(bufferLen, 2);
                    (* Copy data response to output buffer *)
                    for numData := 0 to bufferLen by 1 do
                        Buffer[numData] := bufferSave[numData + 1];
                    end_for;        
        
                    (* Data number read *)
                    Length := numData;
                    bufferSave[0] := 0;
                    posRW := 0;
                    lenBufSave := 0;
                    responseOK := true;
					timerWait(IN := FALSE, PT := waitResp, Q => timeOutResp, ET => waitETN);
                end_if;
            end_if;
        end_if;

        (* If user timeout is zero and no serial data available ....*)
        if ((respTimeOut = 0) and (not responseOK)) then
            (* .... set timeout response *)
            RetC := 16#00310032;
            Length := 0;
            bufferSave[0] := 0;
            posRW := 0;
            lenBufSave := 0;
            responseOK := true;
			timerWait(IN := FALSE, PT := waitResp, Q => timeOutResp, ET => waitETN);
        end_if;

        (* Check user timeout .... *)
        if (respTimeOut <> 0 and timeoutResp) then
            (* .... set timeout response *)
            RetC := 16#00310032;
            Length := 0;
            bufferSave[0] := 0;
            posRW := 0;
            lenBufSave := 0;
            responseOK := true;
			timerWait(IN := FALSE, PT := waitResp, Q => timeOutResp, ET => waitETN);
        end_if;
    end_while;
else
    timeOutResp := false;
    while (not timeOutResp) do
        timerWait(IN := startTime, PT := waitResp, Q => timeOutResp, ET => waitETN);
    end_while;
end_if;

ERROR_GETRX:
;
(* ExecStatus := 0; *)

END_FUNCTION_BLOCK
