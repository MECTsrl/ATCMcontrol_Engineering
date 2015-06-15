(*[[
    ALIAS="alias",
    CREATED="created",
    MODIFIED="modified",
    USRCREATED="usrcreated",
    USRMODIFIED="usrmodified",
    URL="url",
    USERTEXT1="usertext1",
    USERTEXT2="usertext2",
    USERTEXT3="usertext3",
    KEY_001="VALUE_001",
    KEY_002="VALUE_002",
    KEY_003="VALUE_003",
]]*)

FUNCTION_BLOCK FuncBlock_001

VAR
    (* normal variables with simple types *)
    x_000                                              : DINT;
    x_001                                              : DINT := 27;
    x_002 (*[[ ALIAS="alpha" ]]*)                      : DINT;
    x_003 (*[[ ALIAS="beta" ]]*)                       : DINT := 27;
    x_004, x_005 (*[[ ALIAS="gamma" ]]*), x_006, x_007 : DINT;
    x_008, x_009 (*[[ ALIAS="gamma" ]]*), x_010, x_011 : DINT := 27;

    (* normal variables with with simple types and edge detection *)
    x_100                                              : DINT F_EDGE;
    x_101                                              : DINT R_EDGE := 27;
    x_102 (*[[ ALIAS="alpha" ]]*)                      : DINT F_EDGE;
    x_103 (*[[ ALIAS="beta" ]]*)                       : DINT R_EDGE := 27;
    x_104, x_105 (*[[ ALIAS="gamma" ]]*), x_106, x_107 : DINT F_EDGE;
    x_108, x_109 (*[[ ALIAS="gamma" ]]*), x_110, x_111 : DINT R_EDGE := 27;

    (* direct variables *)
    x_200                         AT %QX0.3.2.1 : DINT;
    x_201                         AT %QX0.3.2.1 : DINT := 27;
    x_202 (*[[ ALIAS="alpha" ]]*) AT %QX0.3.2.1 : DINT;
    x_203 (*[[ ALIAS="beta"  ]]*) AT %QX0.3.2.1 : DINT := 27;

(*--------------------------------------------------------------------------------------------------------*)

    (* normal variables with array types *)
    x_300                                              : ARRAY                      OF DINT; 
    x_301                                              : ARRAY [0..6]               OF DINT; 
    x_302                                              : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT;  
    x_303                                              : ARRAY [0..6, 1..7]         OF DINT; 
    x_304 (*[[ ALIAS="alpha" ]]*)                      : ARRAY                      OF DINT; 
    x_305 (*[[ ALIAS="beta"  ]]*)                      : ARRAY [0..6]               OF DINT;  
    x_306 (*[[ ALIAS="gamma" ]]*)                      : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT;  
    x_307 (*[[ ALIAS="delta" ]]*)                      : ARRAY [0..6, 1..2]         OF DINT; 

    (* normal variables with array types and array initializers *)
    x_300                                              : ARRAY                      OF DINT := [1,2,3]; 
    x_301                                              : ARRAY [0..6]               OF DINT := [1,2,3,4,5,6]; 
    x_302                                              : ARRAY [0..6]               OF DINT := [1,2(3,4),6, 7]; 
    x_303                                              : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT := [1,8(3,4),6, 7]; 
    x_304                                              : ARRAY [0..6, 1..2]         OF DINT := [1,4(3,4), 11, 12, 13, 14, 15, 16 ,17];
    x_305 (*[[ ALIAS="alpha  " ]]*)                    : ARRAY                      OF DINT := [1,2,3]; 
    x_306 (*[[ ALIAS="beta   " ]]*)                    : ARRAY [0..6]               OF DINT := [1,2,3,4,5,6]; 
    x_307 (*[[ ALIAS="gamma  " ]]*)                    : ARRAY [0..6]               OF DINT := [1,2(3,4),6, 7]; 
    x_308 (*[[ ALIAS="delta  " ]]*)                    : ARRAY [1+2..sgn(-6*x)+0.5] OF DINT := [1,8(3,4),6, 7];  
    x_309 (*[[ ALIAS="epsilon" ]]*)                    : ARRAY [0..6, 1..2]         OF DINT := [1,4(3,4), 11, 12, 13, 14, 15, 16 ,17];
 

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
;
END_FUNCTION_BLOCK
