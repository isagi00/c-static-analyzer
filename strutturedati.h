#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//include guard
#ifndef STRUTTUREDATI_H
#define STRUTTUREDATI_H

typedef struct{
	void **puntatore;	//puntatore all'array di puntatori che puntano a oggetti di vario tipo
	size_t numero_elementi_attuali;	//numero di elementi attuali (slot occupati)
	size_t spazio_totale_allocato;		//numero di slot nell'array di puntatori  (slot totali)
	//nota: size_t è un tipo senza segno
}List;

List* list_create();
void list_append(List* lista, void* el);
void* list_get(List* list, size_t indice);
void list_free(List* list);

typedef struct {
	char* tipo;	//tipo della var
	char* nome; 	//nome della variabile
	int riga_dichiarata; 	//riga in cui è stata dichiarata la variabile
	bool usata; 	//flag, true se la variabile è stata usata	
	char* storage;	//"auto", "static", "extern", "register"
	char* qual;		////"const", "volatile", "restrict", "_Atomic"
} Variabile;

//statistiche
typedef struct{
	int variabili_controllate;
	int errori_rilevati;
	int variabili_inutilizzate;
	int nomi_variabili_non_corretti;
	int tipi_dato_scorretti;
	List* righe_errore;
} Statistiche;

typedef struct{
     //typedef struct [tag] { int x; } [alias], [alias];  , ci puo essere un tag, alias multipli
    char* tag; 
    char** alias;    
    int numero_alias;
    int riga_dichiarata;
    bool valida;
} StructDef;

StructDef* struct_create();
void struct_free(StructDef* s);


#endif