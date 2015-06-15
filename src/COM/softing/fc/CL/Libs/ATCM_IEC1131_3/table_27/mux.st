(*[[
    UserURL= "ATCMFile:MUX.htm"
]]*)

FUNCTION MUX : ANY
VAR_INPUT 
   K : ANY_INT;
   IN0: ANY; 
END_VAR
VAR_IN_EXT
   IN1: ANY; 
END_VAR


(* optimized multiplexer for 2 values *)
overload BOOL(DINT, BOOL, BOOL)
inline{Utils.mux2_b(tc,%1,%2,%3)}

overload BYTE|WORD|DWORD|DINT|DATE|TOD(
                 DINT,
                 BYTE|WORD|DWORD|DINT|DATE|TOD,
                 BYTE|WORD|DWORD|DINT|DATE|TOD)
inline{Utils.mux2_i(tc,%1,%2,%3)}

overload LREAL(DINT, LREAL, LREAL)
inline{Utils.mux2_d(tc,%1,%2,%3)}

overload WSTRING(DINT, WSTRING, WSTRING)
inline{Utils.mux2_s(tc,%1,%2,%3)}

overload LINT|LWORD|TIME|DT(
                 DINT,
                 LINT|LWORD|TIME|DT,
                 LINT|LWORD|TIME|DT)
inline{Utils.mux2_l(tc,%1,%2,%3)}

overload QVT_BOOL(DINT, QVT_BOOL, QVT_BOOL)
inline{(QvtBoolValue)Utils.mux2_qvt(tc,%1,%2,%3)}

overload QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT(
                 DINT,
                 QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT,
                 QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT)
inline{(QvtIntValue)Utils.mux2_qvt(tc,%1,%2,%3)}

overload QVT_LREAL(DINT, QVT_LREAL, QVT_LREAL)
inline{(QvtDoubleValue)Utils.mux2_qvt(tc,%1,%2,%3)}

overload ANY_DERIVED(DINT, ANY_DERIVED, ANY_DERIVED)
inline{Utils.mux2_od(tc,%1,%2,%3)}



(* optimized multiplexer for 3 values *)
overload BOOL(DINT, BOOL, BOOL, BOOL)
inline{Utils.mux3_b(tc,%1,%2,%3,%4)}

overload BYTE|WORD|DWORD|DINT|DATE|TOD(
         DINT,
         BYTE|WORD|DWORD|DINT|DATE|TOD,
         BYTE|WORD|DWORD|DINT|DATE|TOD,
         BYTE|WORD|DWORD|DINT|DATE|TOD)
inline{Utils.mux3_i(tc,%1,%2,%3,%4)}

overload LREAL(DINT, LREAL, LREAL, LREAL)
inline{Utils.mux3_d(tc,%1,%2,%3,%4)}

overload WSTRING(DINT, WSTRING, WSTRING, WSTRING)
inline{Utils.mux3_s(tc,%1,%2,%3,%4)}

overload LINT|LWORD|TIME|DT(
         DINT,
         LINT|LWORD|TIME|DT,
         LINT|LWORD|TIME|DT,
         LINT|LWORD|TIME|DT)
inline{Utils.mux3_l(tc,%1,%2,%3,%4)}

overload QVT_BOOL(DINT, QVT_BOOL, QVT_BOOL, QVT_BOOL)
inline{(QvtBoolValue)Utils.mux3_qvt(tc,%1,%2,%3,%4)}

overload QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT(
         DINT,
         QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT,
         QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT,
         QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT)
inline{(QvtIntValue)Utils.mux3_qvt(tc,%1,%2,%3,%4)}

overload QVT_LREAL(DINT, QVT_LREAL, QVT_LREAL, QVT_LREAL)
inline{(QvtDoubleValue)Utils.mux3_qvt(tc,%1,%2,%3,%4)}

overload ANY_DERIVED(DINT, ANY_DERIVED, ANY_DERIVED, ANY_DERIVED)
inline{Utils.mux3_od(tc,%1,%2,%3,%4)}



(* non optimized multiplexer with up to 10 values *)
overload BOOL(DINT, BOOL, ...)
inline{Utils.mux_b(tc,%nargs-1,%1,%2,%3(false),%4(false),%5(false),%6(false),%7(false),%8(false),%9(false),%10(false),%11(false))}

overload BYTE|WORD|DWORD|DINT|DATE|TOD(DINT, BYTE|WORD|DWORD|DINT|DATE|TOD, ...)
inline{Utils.mux_i(tc,%nargs-1,%1,%2,%3(0),%4(0),%5(0),%6(0),%7(0),%8(0),%9(0),%10(0),%11(0))}

overload LREAL(DINT, LREAL, ...)
inline{Utils.mux_d(tc,%nargs-1,%1,%2,%3(0.0),%4(0.0),%5(0.0),%6(0.0),%7(0.0),%8(0.0),%9(0.0),%10(0.0),%11(0.0))}

overload WSTRING(DINT, WSTRING, ...)
inline{Utils.mux_s(tc,%nargs-1,%1,%2,%3(null),%4(null),%5(null),%6(null),%7(null),%8(null),%9(null),%10(null),%11(null))}

overload LINT|LWORD|TIME|DT(DINT, LINT|LWORD|TIME|DT, ...)
inline{Utils.mux_l(tc,%nargs-1,%1,%2,%3(0),%4(0),%5(0),%6(0),%7(0),%8(0),%9(0),%10(0),%11(0))}

overload QVT_BOOL(DINT, QVT_BOOL, ...)
inline{(QvtBoolValue)Utils.mux_qvt(tc,%nargs-1,%1,%2,%3(null),%4(null),%5(null),%6(null),%7(null),%8(null),%9(null),%10(null),%11(null))}

overload QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT(DINT, QVT_BYTE|QVT_WORD|QVT_DWORD|QVT_DINT, ...)
inline{(QvtIntValue)Utils.mux_qvt(tc,%nargs-1,%1,%2,%3(null),%4(null),%5(null),%6(null),%7(null),%8(null),%9(null),%10(null),%11(null))}

overload QVT_LREAL(DINT, QVT_LREAL, ...)
inline{(QvtDoubleValue)Utils.mux_qvt(tc,%nargs-1,%1,%2,%3(null),%4(null),%5(null),%6(null),%7(null),%8(null),%9(null),%10(null),%11(null))}

overload ANY_DERIVED(DINT, ANY_DERIVED, ...)
inline{Utils.mux_od(tc,%nargs-1,%1,%2,%3(null),%4(null),%5(null),%6(null),%7(null),%8(null),%9(null),%10(null),%11(null))}

END_FUNCTION
