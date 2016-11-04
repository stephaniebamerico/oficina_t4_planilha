#/*== Stephanie Briere Americo ==*/

CFLAGS = -Wall -g

all: myxcel

clean:
	-rm *.o

purge: clean
	-rm myxcel

# ----

myxcel: myxcel.o table.o csv_file.o
	gcc -o myxcel $(CFLAGS) csv_file.o table.o myxcel.o

# ----
myxcel.o: myxcel.c
	gcc -c myxcel.c $(CFLAGS)

table.o: table.c
	gcc -c table.c $(CFLAGS)

manipula_tabela.o: csv_file.c
	gcc -c csv_file.c $(CFLAGS)
