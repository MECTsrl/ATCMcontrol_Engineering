(*[[
    UserURL= "ATCMFile:ColdStartInstance.htm"
]]*)
(* 
 * ATCMControl standard function ColdStartInstance
 * ----------------------------------------------------------------------------
 * PURPOSE: initialises all program, function block or structure instance members 
 *          to their coldstart initial values. This includes also all nested 
 *          function blocks and structs.
 *          The function returns if the initialisation is complete.
 * 
 * EXAMPLE: StopLocalTask("Task1");
 *          ColdStartInstance("Prog1.myFB");
 *
 * NOTE:    It is recommended to stop the task(s) that operates on "Prog1.myFB"
 *          before calling ColdStartInstance.
 * 
 * WARNING: use this function only rarely, it may take a long time to coldstart a 
 *          huge and deeply nested instance, that could cause watchdog error in the 
 *          calling task. Typically this function is used in a task that also 
 *          starts/stops other Tasks and therefore SetSystemTask() was called to 
 *          disable the watchdog anyway.
 *)
FUNCTION ColdStartInstance
VAR_INPUT
    instPath : WSTRING;
END_VAR

;(* add target specific implementaion here *)

END_FUNCTION
