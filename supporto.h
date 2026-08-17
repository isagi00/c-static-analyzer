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

//funzioni di supporto
bool is_keyword(char* str);
char** split(char* separaS,char* separatore, int *numeroP);
char** split_variabile(char* str, int *numero);
char* eliminaSpaziDxSx(char* str);
char* eliminaSpaziDxSx_v2(char* str);
char* rimuoviSpaziSx(char* str);
bool controllaRigaCommento(char *str);
bool controllaRigaInclude(char *str);
bool controllaRigaVuota(char *str);
char *pulisciNomeVariabile(char *token);
void compatta_stringa(char* dest, const char* src);
void rimuoviCommentoInline(char* str);
int contaRigheFile(const char* filename);
_Bool ricerca(char* str, char* str_da_ricercare);

bool check_typedef_struct(char* str);


int conta_var_inutilizzate(List *vars);
void salva_statistiche_file_esterno(char *nome_file_output, Statistiche *stats, List *var_inutilizzate);
void stampa_statistiche_su_terminale(Statistiche *stats, List *var_inutilizzate);


bool controllaReturnValido(char* str, List* variabili);
bool isInteroValido(char* token);
bool isVariabileIntDichiarata(char* token, List* variabili);

bool isAssegnazioneValida(char* str, List* variabili);
bool isAssegnazioneValida(char* str, List* vars);

void stampaStructDef(StructDef* sd) ;
_Bool ricerca_array(char *str, const char *arr[]);

void elimina_asterischi(char *str);
#endif
//fine include guard
