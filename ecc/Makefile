
all:
	flex ecpp.yy && g++ -o lex lex.yy.c -lfl
	./lex index < index.ecpp > index_template.cpp
	g++ -o render render.cpp
