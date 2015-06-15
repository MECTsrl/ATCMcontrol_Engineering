
(*[[
]]*)

(* Communication Function Blocks, see IEC 1131-5
 * Implements the USTATUS communication function block
 * 
*)

NATIVE
import COM.softing.fc.CC.commFB.*;
END_NATIVE

FUNCTION_BLOCK USTATUS  (* Device verification - receiver *)  
VAR_INPUT                                  
    EN_R   : BOOL;        (* Enable to receive data *)       
    ID     : COMM_CHANNEL;(* Communication channel *)        
END_VAR                                                    
                                                     
VAR_OUTPUT                                                 
    NDR    : BOOL;        (* New user data received *)       
    ERROR  : BOOL;        (* New non-zero STATUS received *) 
    STATUS : DINT;        (* Last detected status *)         
    PHYS   : DINT;        (* Physical status of remote *)              
                          (*   communication partner   *)   
    LOG    : DINT;        (* Logical status of remote *)               
                          (*   communication partner   *)   
    LOCAL  : ARRAY[0..7] OF WORD; (* Local status of remote *)     
                          (*   communication partner   *)   
END_VAR    

    (* is not implemented yet !! *)
    ERROR := TRUE;
    STATUS := CFB_ErrNotImpl;
NATIVE
END_NATIVE;

END_FUNCTION_BLOCK
