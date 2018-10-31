all:add.cgi
add.cgi: add.o 
	gcc add.o -o add.cgi 
add.o: add.c 
	gcc -c add.c -g
clean:
	rm -rf *.o add.cgi
