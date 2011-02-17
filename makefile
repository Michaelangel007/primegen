CFLAGS=-pthread -Wall -ansi
all: clean client server

client:
	gcc $(CFLAGS) -o primegen_$@ client/main.c common/prime.c
server:
	gcc $(CFLAGS) -o primegen_$@ server/main.c common/prime.c
install: 
	cp primegen_client primegen_server /usr/bin
clean:
	rm -f *.o primegen_client primegen_server
uninstall:
	rm /usr/bin/primegen_{client,server}
debug:

