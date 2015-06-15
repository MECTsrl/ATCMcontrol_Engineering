(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:MSG_Trace.htm"
]]*)

(* MSG_Trace
 * ----------------------------------------------------------------------------
 * Sends a message to the RTS trace device.
 *
 * in   sMsg        Message to trace.
 *)

FUNCTION MSG_Trace

VAR_INPUT
    sMsg : STRING;
END_VAR


inline{STDLIB(MSG_Trace)}

END_FUNCTION
