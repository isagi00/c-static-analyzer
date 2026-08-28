#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "supporto.h"

//include guard
#ifndef CONTROLLOVARIABILE_H
#define CONTROLLOVARIABILE_H

bool check_tipo(char* parola, List* struct_definite, Statistiche* stat);
bool check_nome(char* parola, Statistiche* stat);
bool check_struct_nome_alias(char* nome, Statistiche* stat);
StructDef* parse_struct_typedef(char* buffer, int riga_inizio, Statistiche* stat);
bool check_main_args(char* args, Statistiche* stat);
bool check_tipo_composto(char** tokens_tipo, int n_tipo, char* tipo_base_out);
bool check_dichiarazione_var(char* str, int riga, List* struct_definite, List* var_dichiarate, Statistiche* stat );
bool check_dichiarazione_main(char* riga, Statistiche* stats);
bool check_typedef_tipo_e_nome(char* str, List* struct_definite, Statistiche* stats);
StructDef* parse_typedef_oneline(char* clean, int n_riga, List* struct_definite, Statistiche* stats);
List* check_file(char* filename, Statistiche* stats);

//fine include guard
#endif
