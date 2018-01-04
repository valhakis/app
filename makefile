cc = gcc

src += src/main.c
src += src/window.c
src += src/temporary.c

obj += obj/main.o
obj += obj/window.o
obj += obj/temporary.o

cflags += -I inc

libs += -lX11
libs += -lGL

all: bin/app

bin/app: $(obj)
	$(cc) -o $@ $^ $(cflags) $(libs)

obj/main.o: src/main.c
	$(cc) -c -o $@ $^ $(cflags)

obj/window.o: src/window.c
	$(cc) -c -o $@ $^ $(cflags)

obj/temporary.o: src/temporary.c
	$(cc) -c -o $@ $^ $(cflags)
