#include <stddef.h>
#include <stdbool.h>
#include "strutturedati.h"

//include guard
#ifndef SUPPORTO_H
#define SUPPORTO_H

//array utili
extern const char* tipi_base[];	//-> extern permette l'utilizzo di questi array in file .c esterni
extern const char* qualificatori[];
extern const char* storage[];
extern const char* keywords[];


bool check_keyword(char* str);
char** split(char* str,char* separatore, int *numeroP);
char* remove_spazi_dxsx(char* str);
char* remove_spazi_dxsx_v2(char* str);
char** split_variabile(char* str, int *numero);
_Bool ricerca(char* str, char* str_da_ricercare);
char* remove_spazi_sx(char *str);
bool check_riga_commento(char *str);
void remove_commento_inline(char* str);
bool check_riga_include(char *str);
bool check_riga_vuota(char *str);
char *clean_nome_variabile(char *token);
int count_var_inutilizzate(List *vars);
void salva_statistiche_file_esterno(char *nome_file_output, Statistiche *stats, List *var_inutilizzate);
void stampa_statistiche_su_terminale(Statistiche *stats, List *var_inutilizzate);
bool check_intero_valido(char* token);
bool check_var_int_dichiarata(char* token, List* variabili);
bool check_espressione_dopo_return(char* espressione, List* variabili);
bool check_return_valido(char* str, List* variabili);
bool check_assegnazione_valida(char* str, List* vars);
bool check_typedef_struct(char* str);
void print_structdef(StructDef* sd);
void print_array_da_split(char **parole, int numero_parole);
_Bool ricerca_array(char *str, const char *arr[]);
void remove_asterischi(char *str);

#endif
//fine include guard
