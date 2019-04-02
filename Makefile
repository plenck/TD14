
DIRSDL=/users/prog1a/C/librairie/2011
CFLAGS=-c -g -O2
LFLAGS= -lSDLmain -lSDL -lm


EXE=expression1 expression2 expression3 expression4 expression5

all : $(EXE)

main : fonctions.o main.o arbre.o
	gcc -o $@ $^ $(LDFLAGS)

expression5 : expression5.o fonctions.o arbre.o traceexp.o simplifie.o analyse2.o
	gcc -o $@ $^ $(LDFLAGS)

expression4 : expression4.o fonctions.o arbre.o traceexp.o simplifie.o analyse.o
	gcc -o $@ $^ $(LDFLAGS)

expression4b : expression4b.o fonctions.o arbre.o traceexp.o simplifie.o
		gcc -o $@ $^ $(LDFLAGS)

expression3 : expression3.o fonctions.o arbre.o traceexp.o simplifie.o
	gcc -o $@ $^ $(LDFLAGS)

expression2 : expression2.o fonctions.o arbre.o traceexp.o
	gcc -o $@ $^ $(LDFLAGS)

expression1 : expression1.o fonctions.o arbre.o traceexp.o simplifie.o
	gcc -o $@ $^ $(LDFLAGS)

expression : expression.o fonctions.o arbre.o traceexp.o
	gcc -o $@ $^ $(LDFLAGS)

%.o	:%.c
	gcc  $< $(CFLAGS)

clean:
		rm -f *.o $(EXE)
