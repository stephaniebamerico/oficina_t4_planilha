#/*== Stephanie Briere Americo ==*/

CFLAGS = -Wall -g

all: myxcel

clean:
	-rm *.o

purge: clean
	-rm myxcel

# ----

myxcel: myxcel.o table.o csv_file.o window.o
	gcc -o myxcel $(CFLAGS) *.o -lncurses #csv_file.o table.o window.o myxcel.o

# ----
myxcel.o: myxcel.c
	gcc -c myxcel.c $(CFLAGS)

table.o: table.h table.c
	gcc -c table.c $(CFLAGS)

csv_file.o: table.h csv_file.h csv_file.c
	gcc -c csv_file.c $(CFLAGS)

window.o: window.h window.c
	gcc -c window.c $(CFLAGS)
