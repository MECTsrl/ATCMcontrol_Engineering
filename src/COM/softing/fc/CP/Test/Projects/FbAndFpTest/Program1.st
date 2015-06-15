(* ATCM, 1997, 1998. All Rights Reserved *)
(*[[GUID="{CE4A3FB9-0F1D-11D2-AAB8-00A024399A66}"]]*)

PROGRAM Program1 

VAR
    Mean_Min_Max0:              MEAN_MIN_MAX;

    
    
    Bool16_To_Word0:    BOOL16_TO_WORD;



    ManBoolIn0:         ManBoolIn;
    ManLrealIn0:        ManLrealIn;
    ManDintIn0:         ManDintIn;
    ManByteIn0:         ManByteIn;
    ManWordIn0:         ManWordIn;
    ManDwordIn0:        ManDwordIn;
    ManWstringIn0:      ManWstringIn;
    

	
	three_pos_elem0:			 	THREE_POS_ELEM;

	three_pos_elem1Signal:			SIGNAL;
	three_pos_elem1:			 	THREE_POS_ELEM;



	limit_indicator0:			 	LIMIT_INDICATOR;

	limit_indicator1Signal:			SIGNAL;
	limit_indicator1:			 	LIMIT_INDICATOR;



	Hp10:				HP1;
	Hp10Trend:			Trend;

	Hp11Clock:			CLOCK;
	Hp11:				HP1;
	Hp11Trend:			Trend;

	Hp12Signal1:		SIGNAL;
	Hp12Signal2:		SIGNAL;
	Hp12:				HP1;
	Hp12Trend:			Trend;

	Hp20:				HP2;
	Hp20Trend:			Trend;

	Hp21Clock:			CLOCK;
	Hp21:				HP2;
	Hp21Trend:			Trend;

	Hp22Signal1:		SIGNAL;
	Hp22Signal2:		SIGNAL;
	Hp22:				HP2;
	Hp22Trend:			Trend;



	I0:				I;
	I0Trend:		Trend;

	I1Clock:			CLOCK;
	I1:				I;
	I1Trend:		Trend;



	Pid0:				PID;
	Pid0Trend:			Trend;

	Pid1Clock:			CLOCK;
	Pid1:				PID;
	Pid1Trend:			Trend;



	Pt10:				PT1;
	Pt10Trend:			Trend;

	Pt11Clock:			CLOCK;
	Pt11:				PT1;
	Pt11Trend:			Trend;

	Pt20:				PT2;
	Pt20Trend:			Trend;

	Pt21Clock:			CLOCK;
	Pt21:				PT2;
	Pt21Trend:			Trend;

	Ptt0:				PTT;
	Ptt0Trend:			Trend;

	Ptt1Clock:			CLOCK;
	Ptt1:				PTT;
	Ptt1Trend:			Trend;



	Ramp0:				RAMP;
	Ramp0Trend:			Trend;

	Ramp1Clock:			CLOCK;
	Ramp1:				RAMP;
	Ramp1Trend:			Trend;

	Ramp2Clock:			CLOCK;
	Ramp2:				RAMP;
	Ramp2Trend:			Trend;



	Random0:			RANDOM;
	Random0Trend:		Trend;

	Random1:			RANDOM;
	Random1Trend:		Trend;



	Signal0:			SIGNAL;
	Signal0Trend:		Trend;

	Signal1:			SIGNAL;
	Signal1Trend:		Trend;

	Signal2:			SIGNAL;
	Signal2Trend:		Trend;

	Signal3:			SIGNAL;
	Signal3Trend:		Trend;

	Signal4:			SIGNAL;
	Signal4Trend:		Trend;



	Clock0: 				CLOCK;
	Clock0Trend:			Trend;

	Clock1: 				CLOCK;
	Clock1Trend:			Trend;



	Lp10:				LP1;
	Lp10Trend:			Trend;

	Lp11Clock:			CLOCK;
	Lp11:				LP1;
	Lp11Trend:			Trend;

	Lp12Signal1:		SIGNAL;
	Lp12Signal2:		SIGNAL;
	Lp12:				LP1;
	Lp12Trend:			Trend;

	Lp20:				LP2;
	Lp20Trend:			Trend;

	Lp21Clock:			CLOCK;
	Lp21:				LP2;
	Lp21Trend:			Trend;

	Lp22Signal1:		SIGNAL;
	Lp22Signal2:		SIGNAL;
	Lp22:				LP2;
	Lp22Trend:			Trend;

    TON1:               TON;
    TON1Trend:          Trend;

    TOF1:               TOF;
    TOF1Trend:          Trend;

    TP1:                TP;
    TP1Trend:           Trend;

    Ctud_D_Ex0:         CTUD_D_EX;      
END_VAR
Mean_Min_Max0();
Bool16_To_Word0();

ManBoolIn0();
ManLrealIn0();
ManDintIn0();
ManByteIn0();
ManWordIn0();
ManDwordIn0();
ManWstringIn0();
three_pos_elem0();

three_pos_elem1Signal(FORM := 0, T1 := T#20s, MIN := -1.5, MAX := 1.5);
three_pos_elem1(IN := three_pos_elem1Signal.OUT);

limit_indicator0();

limit_indicator1Signal(FORM := 0, T1 := T#60s, MIN := -15.0, MAX := 15.0);
limit_indicator1(IN := limit_indicator1Signal.OUT,
     H2_V := 10.0,  H2_T := "Text für Überschreitung von H2",
     H1_V := 5.0,   H1_T := "Text für Überschreitung von H1",
                    M_T  := "Text für Medium",
     L1_V := -5.0,  L1_T := "Text für Unterschreitung von L1",
     L2_V := -10.0, L2_T := "Text für Unterschreitung von L2",
	 SCALEMIN := -20.0, SCALEMAX := 20.0);

Hp10();
HP10Trend(IN3 := HP10.IN,
		  IN4 := HP10.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);


Hp11Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Hp11(IN := Hp11Clock.OUT, Ts := T#100ms, F := 0.025);
HP11Trend(IN3 := HP11Clock.OUT,
          IN4 := HP11.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

HP12Signal1(FORM := 0, T1 := T#10s, MIN := -0.5, MAX := 1.5);
HP12Signal2(FORM := 0, T1 := T#120s, MIN := -0.5, MAX := 1.5);
HP12(IN := HP12Signal1.OUT + HP12Signal2.OUT, Ts := T#100ms, F := 0.2);
HP12Trend(IN1 := HP12Signal1.OUT,
          IN2 := HP12Signal2.OUT,
          IN3 := HP12Signal1.OUT + HP22Signal2.OUT,
          IN4 := HP12.OUT,
		  MIN1 := -4.0, MIN2 := -4.0, MIN3 := -4.0, MIN4 := -4.0,
		  MAX1 :=  4.0, MAX2 :=  4.0, MAX3 :=  4.0, MAX4 :=  4.0,
		  TWIN := T#240000ms);

Hp20();
HP20Trend(IN3 := HP20.IN,
		  IN4 := HP20.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Hp21Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Hp21(IN := Hp21Clock.OUT, Ts := T#100ms, F := 0.025);
HP21Trend(IN3 := HP21Clock.OUT,
          IN4 := HP21.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

HP22Signal1(FORM := 0, T1 := T#10s, MIN := -0.5, MAX := 1.5);
HP22Signal2(FORM := 0, T1 := T#120s, MIN := -0.5, MAX := 1.5);
HP22(IN := HP22Signal1.OUT + HP22Signal2.OUT, Ts := T#100ms, F := 0.2);
HP22Trend(IN1 := HP22Signal1.OUT,
          IN2 := HP22Signal2.OUT,
          IN3 := HP22Signal1.OUT + HP22Signal2.OUT,
          IN4 := HP22.OUT,
		  MIN1 := -4.0, MIN2 := -4.0, MIN3 := -4.0, MIN4 := -4.0,
		  MAX1 :=  4.0, MAX2 :=  4.0, MAX3 :=  4.0, MAX4 :=  4.0,
		  TWIN := T#240000ms);

I0();
I0Trend(IN3 := I0.IN,
	 	    IN4 := I0.OUT,
	        MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		    MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		    TWIN := T#240000ms);

I1Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
I1(IN := I1Clock.OUT, C := 0.25, P := 0.05, OMAX := 1.5, OMIN := -0.75);
I1Trend(IN3 := I1Clock.OUT,
	 	    IN4 := I1.OUT,
	        MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		    MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		    TWIN := T#240000ms);


Pid0(PV := Pid0.OUT);
Pid0Trend(IN3 := Pid0.SP,
		  IN4 := Pid0.OUT,
	      MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Pid1Clock(T_ON := T#60s, V_ON := 1.0, T_Off := T#60s, V_OFF := -1.0);
Pid1(SP := Pid1Clock.OUT, PV := Pid1.OUT, P := 0.8, I := 0.5, D := 0.95, Ts := T#100ms, OMIN := -1.75, OMAX := 1.75,
     SCALEMIN1 := -2.0, SCALEMAX1 := 2.0, SCALEMIN2 := -2.5, SCALEMAX2 := 2.5);
Pid1Trend(IN3 := Pid1Clock.OUT,
		  IN4 := Pid1.OUT,
	      MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);


Pt10();
Pt10Trend(IN3 := Pt10.IN,
		  IN4 := Pt10.OUT,
	      MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Pt11Clock(T_ON := T#80s, V_ON := 1.0, T_Off := T#80s, V_OFF := -1.0);
Pt11(IN := Pt11Clock.OUT, Ts := T#100ms, T := T#10s, P := 1.5);
Pt11Trend(IN3 := Pt11Clock.OUT,
          IN4 := Pt11.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Pt20();
Pt20Trend(IN3 := Pt20.IN,
		  IN4 := Pt20.OUT,
	      MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Pt21Clock(T_ON := T#80s, V_ON := 1.0, T_Off := T#80s, V_OFF := -1.0);
Pt21(IN := Pt21Clock.OUT, Ts := T#100ms, T := T#10s, P := 1.5, D := 0.5);
Pt21Trend(IN3 := Pt21Clock.OUT,
          IN4 := Pt21.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Ptt0();
Ptt0Trend(IN3 := Ptt0.IN,
		  IN4 := Ptt0.OUT,
	      MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Ptt1Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Ptt1(IN := Ptt1Clock.OUT, Ts := T#100ms, T := T#10s, P := 1.5);
Ptt1Trend(IN3 := Ptt1Clock.OUT,
          IN4 := Ptt1.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Ramp0();
Ramp0Trend(IN4 := Ramp0.OUT,
		   MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		   MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		   TWIN := T#240000ms);

Ramp1Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Ramp1(IN := Ramp1Clock.OUT, FORM := 0, T_UP := T#40s, T_DOWN := T#20s, OMAX := 1.5, OMIN := -0.75);
Ramp1Trend(IN3 := Ramp1Clock.OUT,
           IN4 := Ramp1.OUT,
		   MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		   MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		   TWIN := T#240000ms);

Ramp2Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Ramp2(IN := Ramp2Clock.OUT, FORM := 3, T_UP := T#40s, T_DOWN := T#20s, OMAX := 1.5, OMIN := -0.75);
Ramp2Trend(IN3 := Ramp2Clock.OUT,
           IN4 := Ramp2.OUT,
		   MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		   MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		   TWIN := T#240000ms);

Random0();
Random0Trend(IN4 := Random0.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);

Random1(LBOUND := -1.5, UBOUND := 1.5);
Random1Trend(IN4 := Random1.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);



Signal0();
Signal0Trend(IN4 := Signal0.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);

Signal1(FORM := 0, T1 := T#20s, MIN := -0.5, MAX := 1.5);
Signal1Trend(IN4 := Signal1.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);

Signal2(FORM := 1, T1 := T#20s, MIN := -0.5, MAX := 1.5);
Signal2Trend(IN4 := Signal2.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);

Signal3(FORM := 2, T1 := T#20s, T2 := T#40s, MIN := -0.5, MAX := 1.5);
Signal3Trend(IN4 := Signal3.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);

Signal4(FORM := 3, T1 := T#20s, T2 := T#60s, MIN := -0.5, MAX := 1.5);
Signal4Trend(IN4 := Signal4.OUT,
		     MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		     MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		     TWIN := T#240000ms);



Clock0();
Clock0Trend(IN4 := Clock0.OUT,
		   MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		   MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		   TWIN := T#240000ms);

Clock1(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Clock1Trend(IN4 := Clock1.OUT,
		   MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		   MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		   TWIN := T#240000ms);



Lp10();
Lp10Trend(IN3 := Lp10.IN,
		  IN4 := Lp10.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Lp11Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Lp11(IN := Lp11Clock.OUT, Ts := T#100ms, F := 0.025);
Lp11Trend(IN3 := Lp11Clock.OUT,
          IN4 := Lp11.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Lp12Signal1(FORM := 0, T1 := T#5s, MIN := -1.0, MAX := 1.0);
Lp12Signal2(FORM := 0, T1 := T#60s, MIN := -2.0, MAX := 2.0);
Lp12(IN := Lp12Signal1.OUT + Lp12Signal2.OUT, Ts := T#100ms, F := 0.02);
Lp12Trend(IN1 := Lp12Signal1.OUT,
          IN2 := Lp12Signal2.OUT,
          IN3 := Lp12Signal1.OUT + Lp12Signal2.OUT,
          IN4 := Lp12.OUT,
		  MIN1 := -4.0, MIN2 := -4.0, MIN3 := -4.0, MIN4 := -4.0,
		  MAX1 :=  4.0, MAX2 :=  4.0, MAX3 :=  4.0, MAX4 :=  4.0,
		  TWIN := T#240000ms);

Lp20();
Lp20Trend(IN3 := Lp20.IN,
		  IN4 := Lp20.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Lp21Clock(T_ON := T#40s, V_ON := 1.0, T_Off := T#40s, V_OFF := -1.0);
Lp21(IN := Lp21Clock.OUT, Ts := T#100ms, F := 0.025);
Lp21Trend(IN3 := Lp21Clock.OUT,
          IN4 := Lp21.OUT,
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#240000ms);

Lp22Signal1(FORM := 0, T1 := T#5s, MIN := -1.0, MAX := 1.0);
Lp22Signal2(FORM := 0, T1 := T#60s, MIN := -2.0, MAX := 2.0);
Lp22(IN := Lp22Signal1.OUT + Lp22Signal2.OUT, Ts := T#100ms, F := 0.02);
Lp22Trend(IN1 := Lp22Signal1.OUT,
          IN2 := Lp22Signal2.OUT,
          IN3 := Lp22Signal1.OUT + Lp22Signal2.OUT,
          IN4 := Lp22.OUT,
		  MIN1 := -4.0, MIN2 := -4.0, MIN3 := -4.0, MIN4 := -4.0,
		  MAX1 :=  4.0, MAX2 :=  4.0, MAX3 :=  4.0, MAX4 :=  4.0,
		  TWIN := T#240000ms);

TON1();
TON1Trend(IN3 := bool_to_lreal(TON1.IN),
          IN4 := bool_to_lreal(TON1.Q),
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#60000ms);

TOF1();
TOF1Trend(IN3 := bool_to_lreal(TOF1.IN),
          IN4 := bool_to_lreal(TOF1.Q),
		  MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		  MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		  TWIN := T#60000ms);

TP1();
TP1Trend(IN3 := bool_to_lreal(TP1.IN),
         IN4 := bool_to_lreal(TP1.Q),
		 MIN1 := -2.0, MIN2 := -2.0, MIN3 := -2.0, MIN4 := -2.0,
		 MAX1 :=  2.0, MAX2 :=  2.0, MAX3 :=  2.0, MAX4 :=  2.0,
		 TWIN := T#60000ms);

Ctud_D_Ex0();
          
END_PROGRAM




