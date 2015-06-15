FUNCTION ROR : any_bit
VAR_INPUT 
    IN : any_bit; 
    N  : any_int; 
END_VAR

(* we do not support DINT as second param *)
(*
overload bool(bool,dint)   
inline
{
    %1
    %2
    dint_to_int
    ror_bool
}
overload byte(byte,dint)   
inline
{
    %1
    %2
    dint_to_int
    ror_byte
}
overload word(word,dint)   
inline
{
    %1
    %2
    dint_to_int
    ror_word
}
overload dword(dword,dint) 
inline
{
    %1
    %2
    dint_to_int
    ror_dword
}
*)

overload bool(bool,int)   inline{ VMOP(ror_bool) }
overload byte(byte,int)   inline{ VMOP(ror_byte) }
overload word(word,int)   inline{ VMOP(ror_word) }
overload dword(dword,int) inline{ VMOP(ror_dword) }

(*
overload LWORD(LWORD,INT) inline{}
overload LWORD(LWORD,DINT) inline{}
*)
END_FUNCTION
