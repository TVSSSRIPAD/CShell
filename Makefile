run:runcommand.o utils.o shell.o  
	cc -o cshell runcommand.o utils.o shell.o  
shell.o:shell.c 
	cc -c shell.c 
utils.o:utils.c 
	cc -c utils.c 
runcommand.o:runcommand.c 
	cc -c runcommand.c 
clean : 
	rm runcommand.o utils.o shell.o 