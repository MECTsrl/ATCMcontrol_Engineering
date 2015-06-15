
(*[[
    UserURL= "ATCM:FA_ASYNC_RENAMEFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Functionblock FA_ASYNC_RENAMEFILE
 *  Purpose: Close file after read, write or getsize operation.
 *)
FUNCTION_BLOCK FA_ASYNC_RENAMEFILE
VAR_INPUT
    EXEC        : BOOL;             (* raising edge starts operation *)
    NEWNAME     : WSTRING;          (* new filename *)
END_VAR
VAR_IN_OUT
    FILE        : FA_ASYNC_FILE;    (* file instance to operate on *)
END_VAR
VAR_OUTPUT
    DONE        : BOOL;             (* operation finished *)
    ERROR       : BOOL;             (* error flag *)
    ERRCD       : DINT;             (* error code *)
    ERRTXT      : WSTRING;          (* error text *)
END_VAR

VAR
    rtrigExec   : R_TRIG;           (* execute *)
    token       : BOOL;             (* instance access token *)
    ireq        : BOOL;             (* execute request *)
END_VAR

rtrigExec(CLK := EXEC);
ireq := rtrigExec.Q;

NATIVE
    _done.putBool( ((COM.softing.fclib.flib.AsyncFLib)_file).renameFile(_exec.getBool(),
                        _ireq.getBool(),
                        _token,
                        this,
                        _newname.getString(),
                        _error,
                        _errcd,
                        _errtxt) );
END_NATIVE;

END_FUNCTION_BLOCK
