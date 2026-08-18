#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "supporto.h"

//include guard
#ifndef CONTROLLOVARIABILE_H
#define CONTROLLOVARIABILE_H

// extern int variabili_controllate;
// extern int errori_rilevati;
// extern int variabili_inutilizzate;
// extern int nomi_variabili_non_corretti;
// extern int tipi_dato_scorretti;

// bool check_dichiarazione_main(char* parola);
// bool check_tipo(char* parola);
// bool check_main_args(char* args);
// bool controlloNome(char* parola);
// char*  controlloVariabile(char* filename,Statistiche *stats);
// bool check_tipo_composto(char** tokens_tipo, int n_tipo, char* tipo_base_out);
// bool check_typedef_tipo_e_nome(char* str, int n_riga);
// StructDef* parse_typedef_oneline(char* clean,int n_riga);


// bool controllaStrutturaControllo(char* token, int riga, Statistiche* stats);
// List* check_file(char* filename, Statistiche* stats/*, List* variabili*/);


// //char*  controlloVariabile(char* filename, bool opzione_output, bool opzione_verbose);

// List* controllaUtilizzoVariabili(char *nome_file_in, Statistiche *stats);

void stampa_array_da_split(char **parole, int numero_parole);
bool check_tipo(char* parola, List* struct_definite, Statistiche* stat);
bool check_tipo_base(char* parola);
bool check_nome(char* parola, Statistiche* stat);
bool check_struct_nome_alias(char* nome, Statistiche* stat);
StructDef* parse_struct_typedef(char* buffer, int riga_inizio, Statistiche* stat);
bool check_main_args(char* args, Statistiche* stat);
bool check_tipo_composto(char** tokens_tipo, int n_tipo, char* tipo_base_out);
bool check_dichiarazione_var(char* str, int riga, List* struct_definite, List* var_dichiarate, Statistiche* stat );
bool check_dichiarazione_main(char* riga, Statistiche* stats);
bool check_typedef_tipo_e_nome(char* str, int n_riga, List* struct_definite, Statistiche* stats);
StructDef* parse_typedef_oneline(char* clean, int n_riga, List* struct_definite, Statistiche* stats);
List* check_file(char* filename, Statistiche* stats);

//fine include guard
#endif
