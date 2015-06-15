
(* copies the data contained in the structure STR to the byte string BSTR at the position POS *)
(* note that components with non-base types are ignored (arrays and structures) *)
(* string typed structure components are stored with a terminating zero-character *)
(* this is necessary to be able to restore a previously saved string from the byte string *)
FUNCTION_BLOCK COPY_STRUCT_TO_BYTE_STRING
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
    END_VAR
    NATIVE
        {
        	int componentId;
            COM.softing.fc.CC.util.ObjectDirectoryEx stStruct;
        	FcObject componentObject;
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
                    _bool_val.putBool(((FcBool)componentObject).getBool());
    END_NATIVE;
                    COPY_BOOL_TO_BYTE_STRING(BOOL_VAL,POS,BSTR);
    NATIVE
            	} else if (componentObject instanceof FcDouble) {
                    _lreal_val.putDouble(((FcDouble)componentObject).getDouble());
    END_NATIVE;
                    COPY_LREAL_TO_BYTE_STRING(LREAL_VAL,POS,BSTR);
    NATIVE
           		} else if (componentObject instanceof FcInt) {
                    _dint_val.putInt(((FcInt)componentObject).getInt());
    END_NATIVE;
                    COPY_DINT_TO_BYTE_STRING(DINT_VAL,POS,BSTR);
    NATIVE
           		} else if (componentObject instanceof FcLong) {
                    _lint_val.putLong(((FcLong)componentObject).getLong());
    END_NATIVE;
                    COPY_LINT_TO_BYTE_STRING(LINT_VAL,POS,BSTR);
    NATIVE
           		} else if (componentObject instanceof FcString) {
                    _wstring_val.putString(((FcString)componentObject).getString());
    END_NATIVE;
                    COPY_WSTRING_TO_BYTE_STRING(WSTRING_VAL,POS,BSTR);
                    IF (POS <> -1) THEN
                        COPY_BYTE_TO_BYTE_STRING(0,POS,BSTR);
                    END_IF;
    NATIVE
           		}
           		componentId++;
           	}
        }
    END_NATIVE;
END_FUNCTION_BLOCK
