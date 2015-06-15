(*[[
    GUID= "{E2284DED-50ED-40C8-A875-7814E078DBA3}",  
    Alias="alias",
    Created="created",
    Modified="modified",
    UserCreated="usrcreated",
    UserModified="usrmodified",
    UserHelpURL="url",
    UserURL="url",
    UserText1="usertext1",
    UserText2="usertext2",
    UserText3="usertext3",
    KEY_001="VALUE_001",
    KEY_002="VALUE_002",
    KEY_003="VALUE_003",
]]*)

FUNCTION_BLOCK CFuncBlock_001

VAR
    (* normal variables with simple types *)
    x_000                                              : DINT;
    x_001                                              : DINT := 27;
    x_002 (*[[ Alias="alpha" ]]*)                      : DINT;
    x_003 (*[[ Alias="beta" ]]*)                       : DINT := 27;
    x_004, x_005 (*[[ Alias="gamma" ]]*), x_006, x_007 : DINT;
    x_008, x_009 (*[[ Alias="gamma" ]]*), x_010, x_011 : DINT := 27;

    (* normal variables with with simple types and edge detection *)
    x_100                                              : DINT F_EDGE;
    x_101                                              : DINT R_EDGE := 27;
    x_102 (*[[ Alias="alpha" ]]*)                      : DINT F_EDGE;
    x_103 (*[[ Alias="beta" ]]*)                       : DINT R_EDGE := 27;
    x_104, x_105 (*[[ Alias="gamma" ]]*), x_106, x_107 : DINT F_EDGE;
    x_108, x_109 (*[[ Alias="gamma" ]]*), x_110, x_111 : DINT R_EDGE := 27;

    (* direct variables *)
    x_200                         AT %QX0.3.2.1 : DINT;
    x_201                         AT %QX0.3.2.1 : DINT := 27;
    x_202 (*[[ Alias="alpha" ]]*) AT %QX0.3.2.1 : DINT;
    x_203 (*[[ Alias="beta"  ]]*) AT %QX0.3.2.1 : DINT := 27;

(*--------------------------------------------------------------------------------------------------------*)

    (* normal variables with array types *)
    x_300                                              : ARRAY                      OF DINT; 
    x_301                                              : ARRAY [0..6]               OF DINT; 
    x_302                                              : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT;  
    x_303                                              : ARRAY [0..6, 1..7]         OF DINT; 
    x_304 (*[[ Alias="alpha" ]]*)                      : ARRAY                      OF DINT; 
    x_305 (*[[ Alias="beta"  ]]*)                      : ARRAY [0..6]               OF DINT;  
    x_306 (*[[ Alias="gamma" ]]*)                      : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT;  
    x_307 (*[[ Alias="delta" ]]*)                      : ARRAY [0..6, 1..2]         OF DINT; 

    (* normal variables with array types and array initializers *)
    x_300                                              : ARRAY                      OF DINT := [1,2,3]; 
    x_301                                              : ARRAY [0..6]               OF DINT := [1,2,3,4,5,6]; 
    x_302                                              : ARRAY [0..6]               OF DINT := [1,2(3,4),6, 7]; 
    x_303                                              : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT := [1,8(3,4),6, 7]; 
    x_304                                              : ARRAY [0..6, 1..2]         OF DINT := [1,4(3,4), 11, 12, 13, 14, 15, 16 ,17];
    x_305 (*[[ Alias="alpha  " ]]*)                    : ARRAY                      OF DINT := [1,2,3]; 
    x_306 (*[[ Alias="beta   " ]]*)                    : ARRAY [0..6]               OF DINT := [1,2,3,4,5,6]; 
    x_307 (*[[ Alias="gamma  " ]]*)                    : ARRAY [0..6]               OF DINT := [1,2(3,4),6, 7]; 
    x_308 (*[[ Alias="delta  " ]]*)                    : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT := [1,8(3,4),6, 7];  
    x_309 (*[[ Alias="epsilon" ]]*)                    : ARRAY [0..6, 1..2]         OF DINT := [1,4(3,4), 11, 12, 13, 14, 15, 16 ,17];
 

    (* whats about arrays with edge detection *)

    (* direct variables with array type *)
(*    x_500                         AT %QX0.1.0.0 : ARRAY[0..6] OF BOOL; *)


    (* normal variables with structure initializers *)

END_VAR

(*
================================================================================
 local variables
================================================================================
*)
VAR
    local_none : DINT;
END_VAR

VAR NON_RETAIN
    local_non_retain : DINT;
END_VAR

VAR RETAIN
    local_retain : DINT;
END_VAR

VAR CONSTANT
    local_constant : DINT;
END_VAR

(*
================================================================================
 input variables
================================================================================
*)

VAR_INPUT
    input_none : DINT;
END_VAR

VAR_INPUT NON_RETAIN
    input_non_retain : DINT;
END_VAR

VAR_INPUT RETAIN
    input_retain : DINT;
END_VAR

VAR_INPUT CONSTANT
    input_constant : DINT;
END_VAR

(*
================================================================================
 output variables
================================================================================
*)
VAR_OUTPUT
    output_none : DINT;
END_VAR

VAR_OUTPUT NON_RETAIN
    output_non_retain : DINT;
END_VAR

VAR_OUTPUT RETAIN
    output_retain : DINT;
END_VAR

VAR_OUTPUT CONSTANT
    output_constant : DINT;
END_VAR

(*
================================================================================
 inout variables
================================================================================
*)
VAR_IN_OUT
    inout_none : DINT;
END_VAR

VAR_IN_OUT NON_RETAIN
    inout_non_retain : DINT;
END_VAR

VAR_IN_OUT RETAIN
    inout_retain : DINT;
END_VAR

VAR_IN_OUT CONSTANT
    inout_constant : DINT;
END_VAR

(*
================================================================================
 external variables
================================================================================
*)
VAR_EXTERNAL
    external_none : DINT;
END_VAR

VAR_EXTERNAL NON_RETAIN
    external_non_retain : DINT;
END_VAR

VAR_EXTERNAL RETAIN
    external_retain : DINT;
END_VAR

VAR CONSTANT
    external_constant : DINT;
END_VAR

(*
================================================================================
 temp variables
================================================================================
*)
VAR_TEMP
    temp_none : DINT;
END_VAR

VAR_TEMP NON_RETAIN
    temp_non_retain : DINT;
END_VAR

VAR_TEMP RETAIN
    temp_retain : DINT;
END_VAR

VAR_TEMP CONSTANT
    temp_constant : DINT;
END_VAR

(*
================================================================================
 inext variables
================================================================================
*)
VAR_IN_EXT
    inext_none : DINT;
END_VAR

VAR_IN_EXT NON_RETAIN
    inext_non_retain : DINT;
END_VAR

VAR_IN_EXT RETAIN
    inext_retain : DINT;
END_VAR

VAR_IN_EXT CONSTANT
    inext_constant : DINT;
END_VAR


a:= b;

END_FUNCTION_BLOCK

