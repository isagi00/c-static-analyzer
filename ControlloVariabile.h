#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "supporto.h"

//include guard
#ifndef CONTROLLOVARIABILE_H
#define CONTROLLOVARIABILE_H

extern int variabili_controllate;
extern int errori_rilevati;
extern int variabili_inutilizzate;
extern int nomi_variabili_non_corretti;
extern int tipi_dato_scorretti;

bool isMain(char* parola);
bool controlloTipo(char* parola);
bool controlloArgomentiMain(char* args);
bool controlloNome(char* parola);
char*  controlloVariabile(char* filename,Statistiche *stats);
bool check_tipo_composto(char** tokens_tipo, int n_tipo, char* tipo_base_out);
bool controllaTipoENomeTypedef(char* str, int n_riga);
StructDef* parseTypeDef(char* clean,int n_riga);


bool controllaStrutturaControllo(char* token, int riga, Statistiche* stats);
List* check_file(char* filename, Statistiche* stats/*, List* variabili*/);


//char*  controlloVariabile(char* filename, bool opzione_output, bool opzione_verbose);

List* controllaUtilizzoVariabili(char *nome_file_in, Statistiche *stats);

//fine include guard
#endif
