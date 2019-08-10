del /f lex.yy.cpp
del /f parser.tab.cpp
flex scanner.l
bison -d parser.y
ren e:lex.yy.c lex.yy.cpp
ren e:parser.tab.c parser.tab.cpp
PAUSE