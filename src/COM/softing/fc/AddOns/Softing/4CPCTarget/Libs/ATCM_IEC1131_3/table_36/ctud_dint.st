
(*[[
    UserURL= "ATCM:CTUD_DINT.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTUD for data type DINT  
 *  Purpose: Up/Down Counter
 *)
FUNCTION_BLOCK CTUD_DINT
VAR_INPUT (* Inputs *)
    CU         : BOOL R_EDGE; (* count up if rising edge   *)
    CD         : BOOL R_EDGE; (* count down if rising edge *)
    R          : BOOL;        (* Reset, set CV to 0        *)
    LD         : BOOL;        (* if true, copy PV to CV    *)
    PV         : DINT;        (* start value               *)
END_VAR
VAR_OUTPUT (* Outputs *)
    QU         : BOOL;        (* TRUE if CV >= PV          *)
    QD         : BOOL;        (* TRUE if CV <= 0           *)
    CV         : DINT;        (* counter output            *)
END_VAR


  IF R THEN 
	CV := 0;
  ELSIF LD THEN 
	CV := PV;
  ELSE 
	IF NOT (CU AND CD) THEN
		IF CU AND (CV < FC_MAX_DINT) THEN 
			CV:=CV+1;
		ELSIF CD AND (CV > FC_MIN_DINT) THEN
			CV := CV-1;
		END_IF;	
	END_IF;	
  END_IF;

  QU := (CV >= PV);
  QD := (CV <= 0); 

END_FUNCTION_BLOCK
