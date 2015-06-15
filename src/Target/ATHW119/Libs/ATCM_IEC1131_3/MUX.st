FUNCTION MUX : ANY
VAR_INPUT 
   K : ANY_INT;
   IN0: ANY; 
END_VAR
VAR_IN_EXT
   IN1: ANY; 
END_VAR

(* 1 bit *)
overload BOOL  (INT|DINT to INT, BOOL,   ...) inline{ STDLIB(mux_bit) }

(* 8 bit *)
overload BYTE  (INT|DINT to INT, BYTE,   ...) inline{ STDLIB(mux_byte) }

(* 16 bit *)
overload INT   (INT|DINT to INT, INT,    ...) inline{ STDLIB(mux_int) }
overload WORD  (INT|DINT to INT, WORD,   ...) inline{ STDLIB(mux_int) }

(* 32 bit *)
overload DINT  (INT|DINT to INT, DINT,   ...) inline{ STDLIB(mux_dint) }
overload DWORD (INT|DINT to INT, DWORD,  ...) inline{ STDLIB(mux_dint) }
overload TIME  (INT|DINT to INT, TIME,   ...) inline{ STDLIB(mux_dint) }
overload REAL  (INT|DINT to INT, REAL,   ...) inline{ STDLIB(mux_dint) }

(* 64 bit *)
overload LREAL (INT|DINT to INT, LREAL,  ...) inline{ STDLIB(mux_64bit) }

(* string *)
overload STRING(INT|DINT to INT, STRING, ...) inline{ STDLIB(mux_string) }

END_FUNCTION
