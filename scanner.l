%{

#define YY_NO_UNISTD_H
#define isatty(x) 0

//#include <stdio.h>
#include <string>
#include <stack>
#include "parser.tab.h"

std::string currentLexeme;
std::stack<YY_BUFFER_STATE> openBuffers;

%}

SYMBOLS	[ \t\n./#]
ID		[a-zA-Z_][a-zA-Z0-9_.]*

%%

[ \t\n]+	;

[/]	{
	return SLASH;
}

o/{SYMBOLS}	{
	return OBJECT;
}

g/{SYMBOLS}	{
	return GROUP;
}

v/{SYMBOLS}	{
	return VERTEX;
}

f/{SYMBOLS}	{
	return FACE;
}

vt/{SYMBOLS}	{
	return TEXTURE;
}

vn/{SYMBOLS}	{
	return NORMAL;
}

mtllib/{SYMBOLS}	{
	return MTLLIB;
}

usemtl/{SYMBOLS}	{
	return USEMTL;
}

newmtl/{SYMBOLS}	{
	return NEWMTL;
}

Kd{SYMBOLS}	{
	return KD;
}

map_Kd{SYMBOLS}	{
	return MAP_KD;
}

[#].*	;

{ID}/{SYMBOLS}	{
	currentLexeme = yytext;
	return ID;
}

[+-]?[0-9]*([.][0-9]*)?/{SYMBOLS}	{
	currentLexeme = yytext;
	return REAL;
}

<<EOF>>	{
	if(openBuffers.size() > 0){
		yy_delete_buffer(YY_CURRENT_BUFFER);
		YY_BUFFER_STATE buffer = openBuffers.top();
		openBuffers.pop();
		yy_switch_to_buffer(buffer);
	}else{
		yyterminate();
	}
}

[^ \t\n/#]*	{
	printf("Error on %s.", yytext);
	return 0;
}

%%

int yywrap(){
	return 1;
}

void parseMaterialsFile(std::string path){
	fopen_s(&yyin, (path).c_str(), "r");
	
	openBuffers.push(YY_CURRENT_BUFFER);
	
	yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
	
	BEGIN(INITIAL);
}