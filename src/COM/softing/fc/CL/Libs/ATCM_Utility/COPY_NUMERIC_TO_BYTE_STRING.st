(*[[
    UserURL= "ATCMFile:COPY_NUMERIC_TO_BYTE_STRING.htm"
]]*)

(* copies the value VALUE to the byte string BSTR at the position POS *)
(* the width parameter specifies how many bytes will be used to store the value *)

FUNCTION COPY_NUMERIC_TO_BYTE_STRING
VAR_INPUT
    VALUE : LWORD;
    WIDTH : DINT;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR

    IF (BSTR.SIZE < POS+WIDTH) THEN
        BSTR.SIZE := POS+WIDTH;
    END_IF;
    NATIVE
    	{
        	// copies a given number of bytes to a bytestring
        	// the bytes to be stored are all contained in the value parameter
        	// bytes are copied in big-endian order => most significant byte first
    		int c,shiftDistance;
            byte byteValue,byteString[];
    		
    		shiftDistance = 0;
            byteString = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
    		for (c=0; c<_width; c++) {
    			byteString[_pos.getInt()+_width-(c+1)] = 
                    (byte) ((_value&((0xffL)<<shiftDistance))>>>shiftDistance);
    			shiftDistance += 8;
    		}
   			_pos.putInt(_pos.getInt()+_width);
    	}
    END_NATIVE;

END_FUNCTION
