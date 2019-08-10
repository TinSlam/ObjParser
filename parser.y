%{
#include "ObjParser.h"

#include "ObjParserModel.h"

ObjParserModel* model = nullptr;

extern std::string currentLexeme;
extern void yyerror(char const* error);
extern int yylex();
extern FILE* yyin;
extern int yylineno;

%}

%start entries

%token SLASH
%token OBJECT
%token GROUP
%token VERTEX
%token FACE
%token TEXTURE
%token NORMAL
%token MTLLIB
%token USEMTL
%token REAL
%token ID

%token NEWMTL
%token KD
%token MAP_KD

%%

entries : entries entry
	|

entry : vertex { model->vertexFound(); }
	| textCoords { model->textureCoordsFound(); }
	| normal { model->normalsFound(); }
	| face { model->faceFound(); }
	| object
	| group
	| includeLib
	| useMat
	| newMat
	| Kd
	| mapKd

vertex : VERTEX REAL firstNumberFound REAL secondNumberFound REAL thirdNumberFound defaultFourthOne
	| VERTEX REAL firstNumberFound REAL secondNumberFound REAL thirdNumberFound REAL fourthNumberFound

textCoords : TEXTURE REAL firstNumberFound defaultSecondZero defaultThirdZero
	| TEXTURE REAL firstNumberFound REAL secondNumberFound defaultThirdZero
	| TEXTURE REAL firstNumberFound REAL secondNumberFound REAL thirdNumberFound

normal : NORMAL REAL firstNumberFound REAL secondNumberFound REAL thirdNumberFound

face : FACE point firstPointFound point secondPointFound point thirdPointFound morePoints

morePoints : morePoints point
	|

point : REAL firstNumberFound secondNumberNotAvailable thirdNumberNotAvailable
	| REAL firstNumberFound SLASH REAL secondNumberFound thirdNumberNotAvailable
	| REAL firstNumberFound SLASH REAL secondNumberFound SLASH REAL thirdNumberFound
	| REAL firstNumberFound SLASH secondNumberNotAvailable SLASH REAL thirdNumberFound

object : OBJECT ID

group : GROUP ID

includeLib : MTLLIB ID { model->parseMtlFile(currentLexeme); }

useMat : USEMTL ID { model->setCurrentMaterial(currentLexeme); }

newMat : NEWMTL ID { model->newMaterial(currentLexeme); }

Kd	: KD REAL REAL REAL

mapKd : MAP_KD ID { model->mapKd(currentLexeme); }

firstNumberFound : { model->strList[0] = currentLexeme; }

secondNumberFound : { model->strList[1] = currentLexeme; }

thirdNumberFound : { model->strList[2] = currentLexeme; }

fourthNumberFound : { model->strList[3] = currentLexeme; }

firstPointFound : { for(int i = 0; i < 3; i++) model->vertex1List[i] = model->strList[i]; }

secondPointFound : { for(int i = 0; i < 3; i++) model->vertex2List[i] = model->strList[i]; }

thirdPointFound : { for(int i = 0; i < 3; i++) model->vertex3List[i] = model->strList[i]; }

defaultFourthOne : { model->strList[3] = "1"; }

defaultSecondZero : { model->strList[1] = "0"; }

defaultThirdZero : { model->strList[2] = "0"; }

secondNumberNotAvailable : { model->strList[1] = "0"; }

thirdNumberNotAvailable : { model->strList[2] = "0"; }

%%

/*
int main(){
	parseFile("input.txt", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}
*/

void parseFile(const char* path, float** vbo, int* vboLength, short** ibo, int* iboLength, float* width, float* height, float* depth, std::map<int, std::string>** materialsMap, int** materials, int** materialsIndices, int* materialsNumber){
	std::string temp = path;
	model = new ObjParserModel(temp);
	model->materialsPath = new std::map<int, std::string>();

	fopen_s(&yyin, path, "r");
	
	yyparse();
	model->wrapUp();
	
	fclose(yyin);
	
	*vbo = model->vbo;
	*vboLength = model->vboLength;
	*ibo = model->ibo;
	*iboLength = model->iboLength;
	*width = model->width;
	*height = model->height;
	*depth = model->depth;
	*materialsMap = model->materialsPath;
	*materials = model->materials;
	*materialsIndices = model->materialsIndices;
	*materialsNumber = model->materialsCount;
	
	delete(model);
}

void yyerror(char const* error){
	printf("Error on line %d : %s\n", yylineno, error);
}