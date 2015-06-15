(*[[
    UserURL= "ATCMFile:BYTE_STRING_APPEND.htm"
]]*)

(* appends the byte string BSTR2 at the end of the byte string BSTR1 *)

FUNCTION BYTE_STRING_APPEND
VAR_IN_OUT
    BSTR1 : BYTE_STRING;
END_VAR
VAR_INPUT
    BSTR2 : BYTE_STRING;
END_VAR
VAR
    START_POS : DINT;
    COPY_LENGTH : DINT;
END_VAR

    COPY_LENGTH := BSTR2.SIZE;
    START_POS := BSTR1.SIZE;
    BSTR1.SIZE := START_POS+COPY_LENGTH;
    NATIVE
    	{
        	// appends the second bytestring to the first bytestring
    		int c;
            byte byteString1[],byteString2[];

            byteString1 = ((COM.softing.fc.CC.utility.ByteString)_bstr1).getByteString();
            byteString2 = ((COM.softing.fc.CC.utility.ByteString)_bstr2).getByteString();
    		for (c=0; c<_copy_length; c++) {
                byteString1[_start_pos+c] = byteString2[c];
            }
        }
    END_NATIVE;

END_FUNCTION
