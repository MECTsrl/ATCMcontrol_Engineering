(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
]]*)

(* SYS_TraceStat
 * ----------------------------------------------------------------------------
 * Traces the actual load statistics to the Engineering.
 * Warning: This function block is not thread safe and creates a significant
 * amount of processor load itself.
 *
 * in  tCycle       Cycle time.
 *)
FUNCTION_BLOCK SYS_TraceStat

VAR_INPUT
    tCycle  : TIME := t#5s;
END_VAR
VAR
    tLast   : TIME;
    tNow    : TIME;

    wRes    : WORD;
    str     : STRING;
END_VAR

    tNow := Tim_Get();

    if tNow < tLast + tCycle then
        return;
    end_if;

    tLast := tNow;
    
    wRes := LD_Get(LOAD_LOAD_AVERAGE, 0, str);
    if wRes = OK then MSG_SendToEng(str); end_if;

    wRes := LD_Get(LOAD_CPU_SYSTEM, 0, str);
    if wRes = OK then MSG_SendToEng(str); end_if;

    wRes := LD_Get(LOAD_CPU_IEC, 0, str);
    if wRes = OK then MSG_SendToEng(str); end_if;

    wRes := LD_Get(LOAD_CPU_RTS, 0, str); 
    if wRes = OK then MSG_SendToEng(str); end_if;

    MSG_SendToEng('--------------------------------------------------------------');

END_FUNCTION_BLOCK
