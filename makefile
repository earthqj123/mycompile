main:testmain.o lex.o lock.o
	cc -g testmain.o lex.o lock.o -o main
testmain.o:testmain.c
	cc -c -g testmain.c
lex.o:lex.c
	cc -c -g lex.c 
lock.o:lock.c
	cc -c -g lock.c
clean:
	rm -rf *.o
