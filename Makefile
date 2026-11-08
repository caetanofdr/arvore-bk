CC = cc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -Iarv_bk
SANITIZE = -g -fsanitize=address,undefined -fno-omit-frame-pointer

EXEC = demo
OBJS = main.o arv_bk/arv_bk.o

SO := $(shell uname -s)

ifeq ($(SO),Darwin)
    VERIFICA_MEMORIA = leaks --atExit -- ./$(EXEC)
else
    VERIFICA_MEMORIA = valgrind --leak-check=full --error-exitcode=1 ./$(EXEC)
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

main.o: main.c arv_bk/arv_bk.h
	$(CC) $(CFLAGS) -c main.c -o main.o

arv_bk/arv_bk.o: arv_bk/arv_bk.c arv_bk/arv_bk.h
	$(CC) $(CFLAGS) -c arv_bk/arv_bk.c -o arv_bk/arv_bk.o

run: $(EXEC)
	./$(EXEC)

debug: clean
	$(MAKE) $(EXEC) CFLAGS="$(CFLAGS) $(SANITIZE)"
	./$(EXEC)

memoria: clean
	$(MAKE) $(EXEC) CFLAGS="$(CFLAGS) -g"
	$(VERIFICA_MEMORIA)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all run debug memoria clean
