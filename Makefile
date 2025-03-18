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

*.o: *.c
	gcc -c $^
