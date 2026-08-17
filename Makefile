

CC = gcc
CFLAGS = -Wall -Wextra 

myPreCompiler: main.c ControlloVariabile.c supporto.c strutturedati.c
	$(CC) $(CFLAGS) -o myPreCompiler main.c ControlloVariabile.c supporto.c strutturedati.c


clean: 
	rm -f myPreCompiler