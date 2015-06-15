(*[[
]]*)
FUNCTION_BLOCK ModBusLineTransmit

VAR_INPUT
	ExecMode        :   INT;
	slaveAddress    :   WORD;
END_VAR

VAR_OUTPUT
	ExecStatus      :   DWORD;
	RetC			:	DWORD;
END_VAR

VAR_INPUT
	Line     : INT;
	Length   : INT;
	Buffer   : ARRAY [0..128] OF BYTE;
END_VAR

VAR
numData         :   INT;
getCRC          :   WORD;
pos             :   DINT;
bufferSend      :   ARRAY [0..128] OF BYTE;         (* Buffer is used to send request *)
tmpBuff			:	ARRAY [0..1] OF BYTE;
tmpLen          :   INT;
END_VAR

(* Slave Address *)
bufferSend[0] := WORD_TO_BYTE(slaveAddress);
tmpLen := SUB(Length, 1);

for numData := 0 to tmpLen by 1 do
    bufferSend[numData + 1] := Buffer[numData];
end_for;

numData := ADD(numData, 1);

(* Call function and get checksum *)
RetC := ModBusCRC16(numData, BufferSend[0], getCRC);

(* Put checksum to bufferSend *)
pos := 0;
COPY_WORD_TO_BYTE_ARRAY(getCRC, pos, tmpBuff);

pos := INT_TO_DINT(numData);
bufferSend[pos] := tmpBuff[0];
bufferSend[pos + 1] := tmpBuff[1];

(* Add checksum len *)
numData := ADD(numData, 2);

RetC := SerialLineTransmit(ExecMode, ExecStatus, Line, numData, bufferSend[0]);

END_FUNCTION_BLOCK
