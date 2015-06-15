
(*[[
    UserURL= "ATCM:BYTE_STRING_SUBSTR.htm"
]]*)

(* extracts a sub- byte string from BSTR *)
(* the sub-string to be extracted is specified by a starting position POS *)
(* and a string length NUM *)

FUNCTION BYTE_STRING_SUBSTR
VAR_INPUT
    BSTR : BYTE_STRING;
    POS  : DINT;
    NUM  : DINT;
END_VAR
VAR_IN_OUT
    BSTR_SUB : BYTE_STRING;
END_VAR
VAR
    COPY_LENGTH : DINT;
END_VAR

    IF (POS < BSTR.SIZE) THEN 
        IF (POS+NUM > BSTR.SIZE) THEN
            COPY_LENGTH := BSTR.SIZE-POS;
        ELSE 
            COPY_LENGTH := NUM;
        END_IF;
    ELSE
        COPY_LENGTH := 0;
    END_IF;
    BSTR_SUB.SIZE := COPY_LENGTH;
    NATIVE
    	{
        	// extracts the given number of bytes at the given position from a bytestring 
    		int c;
            byte sourceByteString[],subByteString[];
            sourceByteString = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
            subByteString = ((COM.softing.fc.CC.utility.ByteString)_bstr_sub).getByteString();
    		for (c=0; c<_copy_length; c++) {
                subByteString[c] = sourceByteString[_pos+c];
            }
        }
    END_NATIVE;

END_FUNCTION
