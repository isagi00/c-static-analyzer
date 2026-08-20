# homeworkC_1
precompilatore, segnali errori di:
- tipo
- nome
- variabili dichiarate ma non utilizzate
- typedef non corretti


compilazione:
gcc -Wall -Wextra -o mypc main.c ControlloVariabile.c strutturedati.c suppporto.c

utilizzo:
./mypc -i path_file_input [-o nome_output] [-v]
