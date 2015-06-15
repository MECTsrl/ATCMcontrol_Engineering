
(*[[
    UserURL= "ATCM:COPY_NUMERIC_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)
(* the width parameter specifies how many bytes will be used to extract the value *)

FUNCTION COPY_NUMERIC_FROM_BYTE_STRING : LWORD
VAR_INPUT
    WIDTH : DINT;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR

    COPY_NUMERIC_FROM_BYTE_STRING := 0;
    IF (BSTR.SIZE < POS+WIDTH) THEN 
        POS := -1;
    ELSE 
        NATIVE
        	{
            	// copies a given number of bytes from a bytestring
            	// the bytes to be read are all stored in the parameter value
            	// bytes are copied in big-endian order => most significant byte first
                int c;
                long result,unsignedByte;
                byte byteString[];
        		
                byteString = ((COM.softing.fc.CC.utility.ByteString)_bstr).getByteString();
    			result = 0;
                for (c=0; c<_width; c++) {
        			unsignedByte = (long) byteString[_pos.getInt()+c];
    				result = (result<<8)|(unsignedByte&0xff);
        		}
    			_pos.putInt(_pos.getInt()+_width);
                _copy_numeric_from_byte_string = result;
        	}
        END_NATIVE;
    END_IF;    

END_FUNCTION
