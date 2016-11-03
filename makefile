#/*== Stephanie Briere Americo ==*/

CFLAGS = -Wall -g

all: main

clean:
	-rm *.o

purge: clean
	-rm main

# ----

main: main.o tabela.o manipula_tabela.o
	gcc -o main $(CFLAGS) manipula_tabela.o tabela.o main.o

# ----
main.o: main.c
	gcc -c main.c $(CFLAGS)

tabela.o: tabela.c
	gcc -c tabela.c $(CFLAGS)

manipula_tabela.o: manipula_tabela.c
	gcc -c manipula_tabela.c $(CFLAGS)
