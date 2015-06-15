@echo off
rem lex -al -LC -P lex_yacc\proto\yylex.cpp -D lex_yacc\Scanner.h -o lex_yacc\Scanner.cpp lex_yacc\Scanner.l
echo Warning: Stubbed lex. the "Scanner.h" and "Scanner.cpp" is already done.

rem yacc -l -LC -P lex_yacc\proto\Yyparse.cpp -D lex_yacc\Parser.h -o lex_yacc\Parser.cpp lex_yacc\Parser.y
echo Warning: Stubbed yacc. the "Parser.h" and "Parser.cpp" is already done.
