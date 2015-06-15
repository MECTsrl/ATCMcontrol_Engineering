FUNCTION SHL : any_bit
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
    shl_bool
}
overload byte(byte,dint)   
inline
{
    %1
    %2
    dint_to_int
    shl_byte
}
overload word(word,dint)   
inline
{
    %1
    %2
    dint_to_int
    shl_word
}
overload dword(dword,dint) 
inline
{
    %1
    %2
    dint_to_int
    shl_dword
}
*)

overload bool(bool,int)   inline{ VMOP(shl_bool) }
overload byte(byte,int)   inline{ VMOP(shl_byte) }
overload word(word,int)   inline{ VMOP(shl_word) }
overload dword(dword,int) inline{ VMOP(shl_dword) }

(*
overload LWORD(LWORD,INT) inline{}
overload LWORD(LWORD,DINT) inline{}
*)
END_FUNCTION
