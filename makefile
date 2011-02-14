CFLAGS=-pthread -Wall -ansi
all: clean primegen

primegen :
	gcc $(CFLAGS) -o $@ main.c
install : 
	cp primegen /usr/bin
clean:
	rm -f *.o primegen
uninstall:
	rm /usr/bin/primegen
