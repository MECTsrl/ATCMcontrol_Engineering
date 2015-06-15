(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:MSG_SendToEng.htm"
]]*)

(* MSG_SendToEng
 * ----------------------------------------------------------------------------
 * Sends a message to the targets message view in the ATCMControl Engineering.
 *
 * in   sMsg        Message to send to the Engineering.
 *)
FUNCTION MSG_SendToEng

VAR_INPUT
    sMsg : STRING;
END_VAR


inline{STDLIB(MSG_SendToEng)}

END_FUNCTION
