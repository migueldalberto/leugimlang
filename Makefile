OBJS := scanner.o token.o stmt.o expr.o parser.o transpiler.o
OUT := leugimlang

all: main.c $(OBJS)
	gcc -o $(OUT) main.c $(OBJS)

debug: main.c $(OBJS)
	gcc -o $(OUT) main.c $(OBJS)

test: test.c $(OBJS)
	gcc -o test test.c $(OBJS) -g

clean:
	rm -f $(OUT) $(OBJS)

scanner.o: scanner.h scanner.c
	gcc -c scanner.c

token.o: token.h token.c
	gcc -c token.c

parser.o: parser.h parser.c
	gcc -c parser.c

stmt.o: stmt.h stmt.c
	gcc -c stmt.c

expr.o: expr.h expr.c
	gcc -c expr.c

interpreter.o: interpreter.h interpreter.c
	gcc -c interpreter.c

environment.o: environment.h environment.c
	gcc -c environment.c

transpiler.o: transpiler.h transpiler.c 
	gcc -c transpiler.c
