CFLAGS=-pthread -Wall -ansi
all: clean prime_client prime_server

prime_client:
	gcc $(CFLAGS) -o $@ client/main.c common/prime.c
prime_server:
	gcc $(CFLAGS) -o $@ server/main.c common/prime.c
example:
	gcc $(CFLAGS) -o $@ client/example.c
install: 
	cp prime_{client,server} /usr/bin
clean:
	rm -f *.o prime_{client,server} example
uninstall:
	rm /usr/bin/prime_{client,server}
debug:

