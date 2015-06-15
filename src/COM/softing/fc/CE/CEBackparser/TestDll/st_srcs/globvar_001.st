VAR_GLOBAL
    a (*[[Alias="b"]]*) : DINT := 5;
END_VAR

VAR_GLOBAL NON_RETAIN
    b (*[[Alias="c"]]*) : DINT := 5;
END_VAR

VAR_GLOBAL CONSTANT
    c (*[[Alias="d"]]*) : LWORD;
END_VAR