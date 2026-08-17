#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//strutture dati
//lista
typedef struct{
	void **puntatore;	//puntatore all'array di puntatori che puntano a oggetti di vario tipo
	size_t numero_elementi_attuali;	//numero di elementi attuali (slot occupati)
	size_t spazio_totale_allocato;		//numero di slot nell'array di puntatori  (slot totali)
	//nota: size_t è un tipo senza segno
}List;

//funzione che crea una lista e ritorna puntatore alla struct List.
List* list_create(){
	//allocazione memoria per la struct List
	List* nuova_lista = malloc(sizeof(List));

	//controllo se malloc ha fallito
	if (nuova_lista == NULL) {
		printf("[supporto] creazione lista fallita a causa errore di malloc\n");
		return NULL;
	}

	//impostazione valori iniziali
	nuova_lista -> numero_elementi_attuali = 0;
	nuova_lista -> spazio_totale_allocato = 10;	//10 slot di default 
	nuova_lista -> puntatore = malloc(sizeof(void*) * nuova_lista->spazio_totale_allocato);	//void* è la dimensione di un puntatore

	//controllo se malloc ha fallito per il puntatore
	if (nuova_lista->puntatore == NULL){
		printf("[supporto] creazione puntatore lista fallito a causa errore di malloc\n");
		free(nuova_lista);
		return NULL;
	}

	//printf("[supporto] lista creata correttamente\n");
	return nuova_lista;
}

//funzione aggiunge un elemento alla lista.
void list_append(List* lista, void* el){
	//controllo che la lista sia valida
	if (lista == NULL) {
		printf("[supporto] list_append : lista vuota o non valida\n");
		return;
	}

	//assegna l'elemento nella lista
	if (lista->numero_elementi_attuali < lista->spazio_totale_allocato){
		lista->puntatore[lista->numero_elementi_attuali] = el;
		lista->numero_elementi_attuali += 1;
		//printf("[supporto] list_append: elemento aggiunto correttamente \n");
	}
	//resize se ci sono >= 10 elementi della lista. raddopia la dimensione
	else if (lista->numero_elementi_attuali >= lista->spazio_totale_allocato){
		size_t nuova_capacità = lista->spazio_totale_allocato * 2;
		//ridimensiona l'array di puntatori
		void** nuovo_array = realloc(lista->puntatore, nuova_capacità * sizeof(void*));
		if(nuovo_array == NULL){
			printf("[list_append] realloc fallito\n");
			return;
		}
		lista->puntatore = nuovo_array;
		lista->spazio_totale_allocato = nuova_capacità;
		
		//aggiungi dopo ridimensionamento
		lista->puntatore[lista->numero_elementi_attuali] = el;
		lista->numero_elementi_attuali += 1;
	}
	else{
		printf("[supporto] list_append: lista piena\n");
		return;
	}
}


//funzione ritorna un oggetto della lista
void* list_get(List* list, size_t indice){
	//controllo lista valida
	if (list == NULL){
		printf("[supporto] list_get: lista non valida\n");
		return NULL;
	}

	//controllo out of range index
	if (indice >= list->numero_elementi_attuali){
		printf("[support] list_get: indice out of range.\nindice = %zu\nelementi lista = %zu\n", indice, list->numero_elementi_attuali);
		return NULL;
	}

	//ritorna puntatore all'oggetto
	return list->puntatore[indice];
}


void list_free(List* list){
	if (list == NULL){
		printf("[supporto] list_free: lista non valida. \n");
		return;
	}

	//free array interno
	if (list->puntatore != NULL) {
		free(list->puntatore);
	}

	//free struct stessa
	free(list);

	//printf("[supporto] list_free: lista liberata correttamente\n");
	return;
}



//variabile

typedef struct {
	char* tipo;	//tipo della var
	char* nome; 	//nome della variabile
	int riga_dichiarata; 	//riga in cui è stata dichiarata la variabile
	bool usata; 	//flag, true se la variabile è stata usata	
	char* storage;	//"auto", "static", "extern", "register"
	char* qual; 	//"const", "volatile", "restrict", "_Atomic"
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

StructDef* struct_create(){
    StructDef* s = malloc(sizeof(StructDef));
    if (!s) return NULL;
    s->tag = NULL;
    s->alias = NULL;
    s->numero_alias = 0;
    s->riga_dichiarata = 0;
    s->valida = true;
    return s;
}

void struct_free(StructDef* s){
    if (!s)  return;

    free(s->tag); 

    if (s->alias){
        for (int i = 0; i < s->numero_alias; i++){
            free(s->alias[i]);
        }
        free(s->alias);
    }
    free(s);
}


