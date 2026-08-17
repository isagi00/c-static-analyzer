#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "ControlloVariabile.h"
#include "supporto.h"
// #include "strutturedati.h"

int main(int argc, char *argv[]){
    //opzioni : 
    //1. -i, --in
    //2. -o, --out
    //3. -v, --verbose
    char nome_file_input[256];
    char nome_file_output[256];
    bool opzione_input = false;
    bool opzione_output = false;
    bool opzione_verbose = false;

    //controllo argomenti minimi
    if (argc < 3 || argc > 6) {    //1: main, 2: -i, 3:nomefile.c ...
         printf("[main] utilizzo: -i <file_in.c> [-o <file_out>] [-v] \n ");
         printf("opzioni:\n");
         printf("   -i/--in <file.c>  specifica il nome del file .c (obbligatorio)\n");
         printf("   [-o/--out <file>]  specifica il nome del file su cui salvare le statistiche. stampa su terminale di default. (opzionale)\n");
         printf("   [-v/--verbose]  stampa sul terminale le statistiche.\n ");
         return 0;
        }
    
    //controllo opzioni
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--in") == 0){
            if (opzione_input == false && i + 1 < argc){
                strcpy(nome_file_input, argv[i + 1]);
                opzione_input = true;
                i++;
            }
            else{
                printf("[main] utilizzo: -i <file_in.c> [-o <file_out>] [-v] \n ");
                return 0;
            }
        }

        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0){
            if (opzione_output == false && i + 1 < argc) {
                strcpy(nome_file_output, argv[i + 1]);
                opzione_output = true;
                i++;
            }
            else{
                printf("[main] utilizzo: -i <file_in.c> [-o <file_out>] [-v] \n ");
                return 0;
            }
        }

        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0){
            if (opzione_verbose == false) {
                opzione_verbose = true;
            }
            else{
                printf("[main] utilizzo: -i <file_in.c> [-o <file_out>] [-v] \n ");
                return 0;
            }
        }
    }

    // //salva le statistiche su un file esterno o stampa su terminale
    // printf("===========================[inizio analisi]===========================\n \n");

    //qui vanno le funzioni. 

    Statistiche stats = {0};    //inizializza tutto a 0
    stats.righe_errore = list_create();
    // List* variabili;

    //controlloVariabile(nome_file_input,opzione_output, opzione_verbose);
    // List *variabili = controllaUtilizzoVariabili(nome_file_input, &stats);  // ritorna lista delle variabili dichiarate

    List* variabili_dichiarate = check_file(nome_file_input, &stats);
    
    // for (int i = 0; i < variabili_dichiarate->numero_elementi_attuali; i++){
    //     Variabile* var = list_get(variabili_dichiarate, i);
    //     printf("[MAIN] var dichiarata: '%s' di tipo: '%s' con storage: '%s' e qual: '%s' alla riga %i\n",
    //             var->nome, var->tipo, var->storage, var->qual, var->riga_dichiarata);
    // }
    


    if (opzione_output == true && opzione_verbose == true){
        salva_statistiche_file_esterno(nome_file_output, &stats, variabili_dichiarate);
        stampa_statistiche_su_terminale(&stats, variabili_dichiarate);
        // printf("===========================[termine analisi]===========================\n");
        return 0;
    }
    else if (opzione_output == true && opzione_verbose == false){
        salva_statistiche_file_esterno(nome_file_output, &stats, variabili_dichiarate);
        // printf("===========================[termine analisi]===========================\n");
        return 0;
    }
    else if (opzione_output == false){   //senza -o, --out: stampa su terminale di default
        stampa_statistiche_su_terminale(&stats, variabili_dichiarate);
        // printf("===========================[termine analisi]===========================\n");
        return 0;
    }


    
}


