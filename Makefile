CC = gcc
CFLAGS = -Wall -Wextra
SRC = main.c controllovar.c strutturedati.c supporto.c

myPreCompiler: $(SRC)
	$(CC) $(CFLAGS) -o myPreCompiler $(SRC)

clean:
	rm -f myPreCompiler
