(*[[
]]*)
(* copies the data contained in the bytes string at the position POS to the structure STR *)
(* note that components with non-base types are ignored (arrays and structures) *)
(* string typed structure components are stored with a terminating zero-character *)
(* this is necessary to be able to restore a previously saved string from the byte string *)
FUNCTION_BLOCK COPY_STRUCT_FROM_BYTE_STRING
    VAR_IN_OUT
        STR : ANY_DERIVED;
        POS : DINT;
        BSTR : BYTE_STRING;
    END_VAR
    VAR
        BOOL_VAL : BOOL;
        LREAL_VAL : LREAL;
        DINT_VAL : DINT;
        LINT_VAL : LINT;
        WSTRING_VAL : WSTRING;
        WSTRING_LENGTH : DINT;
        BSTR_SIZE : DINT;
    END_VAR

    BSTR_SIZE := BSTR.SIZE;

    NATIVE
        {
            boolean stringEndPosFound;
        	int componentId,stringEndPos;
            byte byteString[];
        	FcObject componentObject;
            COM.softing.fc.CC.util.ObjectDirectoryEx stStruct;
        	ObjectDescriptor componentDescriptor,structureDescriptor;
        	java.util.Enumeration componentNameEnumerator;

            stStruct = (COM.softing.fc.CC.util.ObjectDirectoryEx) _str;
        	structureDescriptor = stStruct.getOD();
        	componentId = 0;
        	componentNameEnumerator = stStruct.getNameEnum();
            while (componentNameEnumerator.hasMoreElements() && (_pos.getInt() != -1)) {
                componentNameEnumerator.nextElement();
        		componentObject = stStruct.getObjectByID(componentId);
        		componentDescriptor = componentObject.getOD();
        		if(componentObject instanceof FcBool) {
    END_NATIVE;
                    BOOL_VAL := COPY_BOOL_FROM_BYTE_STRING(POS,BSTR);
    NATIVE
                    if (_pos.getInt() != -1) {
                        ((FcBool)componentObject).putBool(_bool_val.getBool());
                    }
            	} else if (componentObject instanceof FcDouble) {
    END_NATIVE;
                    LREAL_VAL := COPY_LREAL_FROM_BYTE_STRING(POS,BSTR);
    NATIVE
                    if (_pos.getInt() != -1) {
                        ((FcDouble)componentObject).putDouble(_lreal_val.getDouble());
                    }
           		} else if (componentObject instanceof FcInt) {
    END_NATIVE;
                    DINT_VAL := COPY_DINT_FROM_BYTE_STRING(POS,BSTR);
    NATIVE
                    if (_pos.getInt() != -1) {
                        ((FcInt)componentObject).putInt(_dint_val.getInt());
                    }
           		} else if (componentObject instanceof FcLong) {
    END_NATIVE;
                    LINT_VAL := COPY_LINT_FROM_BYTE_STRING(POS,BSTR);
    NATIVE
                    if (_pos.getInt() != -1) {
                        ((FcLong)componentObject).putLong(_lint_val.getLong());
                    }
           		} else if (componentObject instanceof FcString) {
                    byteString = ((COM.softing.fc.CC.commFB.ByteString)_bstr).getByteString();
                    stringEndPosFound = false;
                    stringEndPos = _pos.getInt();
                    while ((stringEndPos < _bstr_size.getInt()) && (! stringEndPosFound)) {
                        if (byteString[stringEndPos] == 0) {
                            stringEndPosFound = true;
                        } else {
                            stringEndPos++;
                        }
                    }
                    if (stringEndPosFound) { 
                        _wstring_length.putInt(stringEndPos-_pos.getInt());    
    END_NATIVE;
                        WSTRING_VAL := COPY_WSTRING_FROM_BYTE_STRING(WSTRING_LENGTH,POS,BSTR);
                        IF (POS <> -1) THEN
                            POS := POS+1;
                        END_IF;
    NATIVE
                    } else {
                        _pos.putInt(-1);
                    }
                    if (_pos.getInt() != -1) {
                        ((FcString)componentObject).putString(_wstring_val.getString());
                    }
           		}
           		componentId++;
           	}
        }
    END_NATIVE;
END_FUNCTION_BLOCK
