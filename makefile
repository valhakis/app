cc = gcc

src = src/main.c
obj = obj/main.o

all: bin/app

bin/app: $(obj)
	$(cc) -o $@ $^

obj/main.o: src/main.c
	$(cc) -c -o $@ $^
