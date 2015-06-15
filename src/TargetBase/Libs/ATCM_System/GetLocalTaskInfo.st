(*[[
    UserURL= "ATCMfile:GetLocalTaskInfo.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetLocalTaskInfo
 *)
FUNCTION GetLocalTaskInfo
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

VAR_IN_OUT
    INFO : TASKINFO;
END_VAR

overload (STRING,TASKINFO)  inline{STDLIB(GetLocalTaskInfo)}

END_FUNCTION
