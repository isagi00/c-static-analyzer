#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "strutturedati.h"

const char* tipi_base[] = {"int", "float", "double", "char", "long", "bool",	//bool mi sa che non è tipo base
						   "signed", "unsigned",
						   "short", "void", NULL};

const char* qualificatori[] = {"const", "volatile", "restrict", "_Atomic", NULL};

const char* storage[] = {"auto", "static", "extern", "register", NULL};

const char* keywords[] = {
        // tipi
        "int","char","float","double","long","short","unsigned","signed","void",
        // storage
        "extern","static","auto","register",
        // qualificatori
        "const","volatile","restrict",
        // controllo flusso
        "if","else","while","for","do","return","break","continue",
        "switch","case","default","goto",
        // altri
        "struct","union","enum","typedef","sizeof",
        NULL
    };

/*
controlla se la stringa corrente è una keyword.
-str: stringa
ritorna true se è una keyword.
*/
bool is_keyword(char* str){
	//check tipi
	for (int i = 0; tipi_base[i] != NULL; i++){
		if (strcmp(str,tipi_base[i]) == 0){
			return true;
		}
	}
	//check su altri keyword
	for (int j = 0; keywords[j] != NULL; j++){
		if(strcmp(str, keywords[j]) == 0){
			return true;
		}
	}
	return false;
}

/*funzione splitta str in un array di token.
str: stringa in input. es: ("hello world")
separatore: caratteri su cui splittare. es: (" \t;")
numeroP: numero di token all'interno dell'array.
ritorna una lista di stringhe splittate sui separatori specificati.
es: ["hello", "world"]
*/
char** split(char* str,char* separatore, int *numeroP){
	char **vettore = malloc(64*sizeof(char*));
	char *token=strtok(str,separatore);
	int i=0;
	while(token!=NULL){
		vettore[i]=token;
		i++;
		token=strtok(NULL,separatore);
	}
	*numeroP=i;
	return vettore;
}

/*
elimina spazi a destra e sinistra della stringa
- str: stringa. es: "		hello 	world		"
ritorna la stringa con spazi eliminati. 
es: "hello world"
*/
char* eliminaSpaziDxSx(char* str){
	if (!str) return NULL;

    int n = strlen(str);
	//elimina spazi dx
    for(int i = 1; i < n; i++){
        if(str[n-i] == ' ' || str[n-i] == '\t'){
            str[n-i] = '\0';
        } else {
            break;
        }
    }
	//elimina spazi sx
    while(*str == ' ' || *str == '\t'){
        str++;
    }
    return str;
}

/*
ritorna una copia della stringa con spazi sx e dx rimossi.
da fare free() al termine dell'utilizzo
*/
char* eliminaSpaziDxSx_v2(char* str){
	if (!str) return NULL;
	//gestione stringa vuota
	int len = strlen(str);
	if (len== 0) return strdup("");

	//scorre spazi a sx
	int inizio = 0;	
	while (str[inizio] == ' ' || str[inizio] == '\t') inizio ++;

	//se stringa tutti spazi ritorna ""
	if (inizio == len) return strdup("");

	//scorre spazi a dx
	int fine = strlen(str) - 1; // -1 per non partire dal '\0'
	while (fine > inizio && (str[fine] == ' ' || str[fine] == '\t')) fine --;

	//copia della stringa
	int lenstr = fine - inizio + 1;
	char* pulito = strndup(str + inizio, lenstr);

	return pulito;
}

//strtok hen nb bu bi genshin nb
char** split_variabile(char* str, int *numero) {
    if (!str || !numero) return NULL;
    
    char **vettore = calloc(64, sizeof(char*));  //  calloc per inizializzare a NULL
    if (!vettore) return NULL;
    
    int i = 0;
    char* str_copy = strdup(str);  // copia per strtok (che modifica)
    if (!str_copy) { free(vettore); return NULL; }

	char* eq = strchr(str_copy, '=');
	char* parte_sinistra = NULL;
	char* parte_destra = NULL;

	if (eq) {
		*eq = '\0';               // taglia su '='
		parte_sinistra = str_copy;
		parte_destra = eq + 1;
	} else {
		parte_sinistra = str_copy;
	}

	// tokenizza sinistra
	char* t = strtok(parte_sinistra, " \t");
	while (t != NULL && i < 63) {
		char* trimmed = eliminaSpaziDxSx_v2(t);
		if (trimmed) vettore[i++] = trimmed;
		t = strtok(NULL, " \t");
	}

	// valore a destra (se c'era '=')
	if (parte_destra) {
		char* trimmed = eliminaSpaziDxSx_v2(parte_destra);
		if (trimmed && strlen(trimmed) > 0) vettore[i++] = trimmed;
	}
	free(str_copy);		
    *numero = i;

    return vettore;
}

_Bool ricerca(char* str, char* str_da_ricercare) {
    int len = strlen(str);
    int len_da_ricercare = strlen(str_da_ricercare);

    for(int i = 0; i < len - len_da_ricercare; i++) {
        if(strncmp(str + i, str_da_ricercare, len_da_ricercare) == 0) {
            return 1;
        }
    }
	
    return 0;
}

/*funzione ha come argomento un array di caratteri.
restituisce il puntatore alla prima posizione non spazio.
- str: stringa in input. es: "		hello world"
ritorna: "hello world"
*/
char *rimuoviSpaziSx(char *str){
	//str = puntatore
	while(*str == ' ' || *str == '\t'){
		str++;
	}
	return str;
}

/*controlla se la riga attuale è un commento.
restituisce true se è commento.
str: stringa in input
*/
bool controllaRigaCommento(char *str){
	char *rigapulita = rimuoviSpaziSx(str);
	
	if (rigapulita[0] == '/' && rigapulita[1] == '/'){
		return true;
	}
	return false;
}

/*
rimuove i commenti inline. non molto robusto eh dovrebbe gestire casi base 
modifica la stringa in place
*/
void rimuoviCommentoInline(char* str){
	if(!str) return;
	int len = strlen(str);

	//caso: "int a = 10 //commento "
	for (int i = 0; i < len; i++){
		if (i+1 >= len) break;
		if(str[i] == '/' && str[i+1] == '/'){
			str[i] = '\0';
			return;
		}
	}

	//caso: "int /* commento */ a = 10"
	//per adesso mette spazi al posto del commento
	int start = -1;	//-1 indica non trovato
	int end = -1;
	for (int j = 0; j < len - 1; j++){
		if (str[j] == '/' && str[j+1] == '*'){
			start = j;
		}
		else if (str[j] == '*' && str[j+1] == '/'){
			end = j + 1;
			break;	//termina dopo averlo trovato
		}
	}
	if(start != -1 && end != -1){
		while (start <= end){
			str[start] = ' ';
			start++;
		}
	}

	
	return;
}

/*
conta il numero di righe del file
*/
int contaRigheFile(const char* filename){
	if (!filename) return -1;
	FILE* file = fopen(filename, "r");
	if(!file){
		printf("[contaRigheFile] errore apertura file");
		return -1;
	}

	int righe = 0;
	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), file) != NULL){
		righe++;
	}

	fclose(file);
	return righe;
}



/*controlla se la riga attuale è un #include valido o non.
restituisce true se è valido.
str: stringa in input
*/
bool controllaRigaInclude(char *str){
	char *riga_pulita = rimuoviSpaziSx(str);
	char copia[256];
	strncpy(copia, riga_pulita, sizeof(copia));

	char *token = strtok(copia, " \t");	//inserisce \0 su spazio e \t

	if (token == NULL) return false;

	//caso #include <?>
	if (strcmp(token, "#include") == 0) return true;

	//caso # 			include <?>
	if (strcmp(token, "#") == 0){
		token = strtok(NULL, " \t");	//NULL: continua dalla stringa precedente, quindi '#'
		if (token != NULL && strcmp(token, "include") == 0) return true;
	}
	return false;
}

/*
controlla se la riga attuale è una riga vuota (con solo \n).
restituisce true se è vuoto.
str: stringa in input
*/
bool controllaRigaVuota(char *str){
	char *riga_pulita = rimuoviSpaziSx(str);	//rimuove spazi davanti
	riga_pulita[strcspn(riga_pulita, "\n")] = '\0';	//sostituisce \n con \0 (per sicurezza)

	if (riga_pulita[0] == '\0'){
		return true;
	}
	return false;
}


char *pulisciNomeVariabile(char *token){
	//x=0;
	char *copia = strdup(token);

	for (int i=0; copia[i] != '\0'; i++){
		if (!isalnum(copia[i]) && copia[i] != '_'){
			copia[i] = '\0';
			break;
		}
	}
	return copia;
}

/* ricompatta una stringa rimuovendo spazi e tab.
ad esempio: "int argc,     char **      argv    [    ]"
diventa:	"intargc,char**argv[]"
- dest: destinazione della stringa compattata
- src: stringa da compattare
*/
void compatta_stringa(char* dest, const char* src){
	int j = 0;
	for(int i = 0; src[i] != '\0'; i++){
		if(src[i] != ' ' && src[i] != '\t'){
			dest[j] = src[i];
			j++;
		}
	}
	dest[j] = '\0';	//aggiungi terminatore di stringa
}




int conta_var_inutilizzate(List *vars){
	//lista vuota
	if (vars->numero_elementi_attuali==0){
		return 0;
	}
	//conta var inutilizzate
	int count = 0;
	for (size_t i=0; i < vars->numero_elementi_attuali; i++){
		Variabile *var = list_get(vars, i);
		if (var->usata == false){
			count++;
		}
	}
	return count;
}



//salva statistiche su file esterno
void  salva_statistiche_file_esterno(char *nome_file_output, Statistiche *stats, List *var_inutilizzate){
	FILE *file_pointer = fopen(nome_file_output, "w"); //puntatore a file esterno

	if (file_pointer == NULL) {
		printf("[STAT] salva_statistiche_file_esterno : Errore apertura file output\n");
		return;
	}

	stats->variabili_inutilizzate = conta_var_inutilizzate(var_inutilizzate);

	//scrittura su file output
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di variabili controllate: %i\n", stats->variabili_controllate);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di errori rilevati: %i\n", stats->errori_rilevati);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di variabili non utilizzati: %i\n", stats->variabili_inutilizzate);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di nomi delle variabili non corretti: %i\n", stats->nomi_variabili_non_corretti);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di tipi di dato scorretti:  %i\n", stats->tipi_dato_scorretti);

	//scrittura nome variabili non usate
	for (size_t i = 0; i < var_inutilizzate->numero_elementi_attuali; i++){
		Variabile *var = list_get(var_inutilizzate, i);
		if (var->usata == false){
			fprintf(file_pointer,"[STAT] salva_statistiche_file_esterno: Variabile '%s' inutilizzata alla riga %i\n", var->nome, var->riga_dichiarata);
		}
	}

	fclose(file_pointer);
	printf("[STAT] salva_statistiche_file_esterno: statistiche salvate in '%s'\n", nome_file_output);
	return;
}


//stampa statistiche su terminale
void stampa_statistiche_su_terminale(Statistiche *stats, List *var_inutilizzate){

	stats->variabili_inutilizzate = conta_var_inutilizzate(var_inutilizzate);

	// printf("[STAT] stampa_statistiche_su_terminale: numero di variabili controllate: %i\n", stats->variabili_controllate);
	// printf("[STAT] stampa_statistiche_su_terminale: numero di errori rilevati: %i\n", stats->errori_rilevati);
	// printf("[STAT] stampa_statistiche_su_terminale: numero di variabilio non utilizzati: %i\n", stats->variabili_inutilizzate);
	// printf("[STAT] stampa_statistiche_su_terminale: numero di nomi delle variabili non corretti: %i\n", stats->nomi_variabili_non_corretti);
	// printf("[STAT] stampa_statistiche_su_terminale: numero di tipi di dato scorretti: %i:\n", stats->tipi_dato_scorretti);
	printf("------------------ STATISTICHE DI ELABORAZIONE ------------------\n");
	printf("numero di variabili controllate: %i\n", stats->variabili_controllate);
	printf("numero di errori rilevati: %i\n", stats->errori_rilevati);
	printf("numero di variabili non utilizzati: %i\n", stats->variabili_inutilizzate);
	printf("numero di nomi delle variabili non corretti: %i\n", stats->nomi_variabili_non_corretti);
	printf("numero di tipi di dato scorretti: %i:\n", stats->tipi_dato_scorretti);
	printf("-----------------------------------------------------------------\n");

	printf("\n");

	if (stats->errori_rilevati != 0){
		printf("----------------------------- ERRORI ----------------------------\n");

		int total = stats->righe_errore->numero_elementi_attuali;
		for (int i = 0; i < total; i++){
			int* ptr_riga = (int*) list_get(stats-> righe_errore, i);
			printf("errore alla riga: %i\n", *ptr_riga);
		}
		printf("-----------------------------------------------------------------\n");


		printf("\n");

	}
	
	if (stats->variabili_inutilizzate != 0){
		printf("------------------ VARIABILI NON UTILIZZATE ---------------------\n");
	//scrittura nome variabili non usate
	for (size_t i = 0; i < var_inutilizzate->numero_elementi_attuali; i++){
		Variabile *var = list_get(var_inutilizzate, i);
		if (var->usata == false){
			printf("[STAT]stampa_statistiche_su_terminale: variabile '%s' inutilizzata alla riga %i\n", var->nome, var->riga_dichiarata);
		}
	}
	printf("-----------------------------------------------------------------\n");
	}
	
	return;
}

/**
 * controlla se il token corrente è un intero valido
 * token: token da controllare
 * ritorna true se è valido.
 */
bool isInteroValido(char* token){
	if (!token) return false;
	size_t i=0;

	//controlla se ci sta un segno '+' o '-', salta se ci sta
	if (token[0] == '+' || token[0] == '-'){
		i = 1;
	}

	//salta spazi e altri segni. es: '+ - +    - 420' è espressione valida....
	while (i < strlen(token) && (isspace(token[i] || token[i] == '+' || token[i] == '-'))) i++;

	//ci deve essere una cifra dopo il segno
	if (token[i] == '\0'){
		// printf("[isInteroValido] intero '%s' non valido \n", token);
 		return false;
	}

	//resto devono esere cifre
	while (token[i]){
		if (!isdigit(token[i])){
			// printf("[isInteroValido] intero '%s' non valido \n", token);
			return false;
		}
		i++;
	}

	return true;
}

/*
controlla se il token corrente è una variabile di tipo int dichiarato.
token: token da controllare
varibili: List, con Variabile
ritorna true se il token è una variabile int.
*/
bool isVariabileIntDichiarata(char* token, List* variabili){
	if (!token || !variabili) return false;
	char* pulito = eliminaSpaziDxSx_v2(token);
	for (size_t i = 0; i < variabili->numero_elementi_attuali; i++){
		Variabile* var = list_get(variabili,i);
		if (var && strcmp(pulito, var->nome) == 0 && strcmp(var->tipo, "int") == 0){
			free(pulito);
			return true;
		}
	}
	printf("[isVariabileIntDichiarata] variabile '%s' non dichiarata o di tipo non int \n", pulito);
	free(pulito);
	return false;
}


/*
convalida se l'espressione dopo il return è valido. controlla solo se ci sono interi e variabili di tipo intero.
espressione: stringa del tipo 'a + b + 10'
variabili: List con Variabile al suo interno
ritorna true se l'espressione è valida
*/
bool convalidaEspressioneReturn(char* espressione, List* variabili){
	if (!espressione || !variabili) return false;
	//espressione sarà del tipo : 'a + zhgey - 69'

	int i = 0;
	int len = strlen(espressione);

	while (i < len){
		//salta spazi
		while (i<len && isspace(espressione[i])) i++;
		if (i>= len) break;

		//salta operatori
		if (espressione[i] == '+' || espressione[i] == '-' || espressione[i] == '*' || 
            espressione[i] == '/' || espressione[i] == '%' || espressione[i] == '(' || espressione[i] == ')') {
            i++;
            continue;
        }

		//estrai token: numero o nome var
		int start = i;
		while (i < len && (isalnum(espressione[i]) || espressione[i] == '_' || espressione[i] == '.')){	//il '.' per numero come '3.14'
			i++;
		}
		if(start == i){ //carattere non riconosciuto
			printf("[ConvalidaEspressioneReturn] carattere '%c' non permesso / non riconosciuto come espressione di return \n", espressione[i]);
			return false;
		} 
		 

		//copia token trovato
		char copia[256];
		int len_copia = i - start;
		strncpy(copia, espressione + start, len_copia);
		copia[len_copia] = '\0';

		//controlla se è un numero
		if (isInteroValido(copia)) continue;

		//controlla se è una variabile int dichiarata
		if (isVariabileIntDichiarata(copia, variabili)) continue;

		return false; //token non valido.
	}
	return true; //se tutti i token sono validi
}



/*
controlla la correttezza del return del main(). controlla casi del tipo:
	return;
	return 10;	
	return b; -> controlla che b sia una var con tipo int.
	return a + b + 10 - 420 ...
ritorna true se il return è dichiarato correttamente.
*/
bool controllaReturnValido(char* str, List* variabili){
	if (!str) return false;
	if (!variabili) return false;

	// printf("controllo return su: '%s'\n", str);
	//splitta su spazi
	int n_parti;
	char** tokens = split(str, " \t;", &n_parti);
	if (!tokens) return false;
	
	//controlla il tipo di ritorno
	//caso: 'return;'
	if(n_parti == 1){
		if (strcmp(tokens[0], "return") == 0){
			free(tokens);
			return true;
		}
	}
	//casi: 'return b' dove b è un 'int' oppure 'return 10'
	else if (n_parti == 2 && strcmp(tokens[0], "return") == 0){
		//caso: 'return b'
		//controlla se la variabile sia dichiarata in precedenza, e che il tipo sia un 'int'
		bool risultato = false;
		for (size_t j = 0; j < variabili->numero_elementi_attuali; j++){
			Variabile* var = list_get(variabili, j);
			if (strcmp(tokens[1], var->nome) == 0 && strcmp(var->tipo, "int") == 0){
				risultato = true;
				break;
			}
		}
		//caso: 'return 10'
		if(!risultato){
			for (size_t i = 0; i < strlen(tokens[1]); i++){
				if (!isdigit(tokens[1][i])){
					printf("[controllaReturnValido] espressione di return non valido: '%s' \n", tokens[1]);
					risultato = false;
					break;
				} 
				risultato = true;	//se loop finisce senza break, è valido
			}
		}
		free(tokens);
		return risultato;
	}
	//casi: 'return a + b + 10 + 20 + ciao...'
	else if (n_parti > 2 && strcmp(tokens[0], "return") == 0 ){
		//ricostruisci espressione dopo 'return'
		char espressione[512] = {0};
		for (int i = 1; i < n_parti; i++){
			strcat(espressione, tokens[i]);	//praticamente .join sui token
			if (i < n_parti - 1) strcat(espressione, " ");	//aggiunge spazio tra i token tranne sull'ultimo
		}	

		if (convalidaEspressioneReturn(espressione, variabili)){
			free(tokens);
			return true;
		} 
	}
	free(tokens);
	printf("[controlloReturnValido] espressione return non valido!\n");
	return false;
}




bool isAssegnazioneValida(char* str, List* vars){
	//controllo sulla riga, controlla che non inizi con una kw.
	char* copia = strdup(str);
	int n_parole;
	char** parole = split(copia, " \t", &n_parole);

	if (n_parole == 0 || !parole[0]){
		free(copia); free(parole);
		return false;
	}
	//controlla se il primo token è una kw
	if (is_keyword(parole[0])){
		free(copia); free(parole);
		return false;
	}

	//se primo token non kw, split su = per separare su left e right
	char* copia2 = strdup(str);
	int n_token;
	char** tokens = split(copia2, "=", &n_token);
	if (n_token < 2){
		free(copia2); free(tokens);
		return false;
	}

	char* lhs = strdup(tokens[0]); //left hand side, tokens a sx
	char* rhs = strdup(tokens[1]); //right hand size, tokens a dx

	//lhs
	bool lhs_valido = false;
	int n_lhs;
	char** left = split(lhs, " \t,.", &n_lhs);	//. per caso: 'struct.var = 10'
	for (int l = 0; l < n_lhs; l++){
		char* attuale = left[l];
		for (size_t i = 0; i<vars->numero_elementi_attuali; i++){
			Variabile* var = list_get(vars,i);
			if (strcmp(attuale, var->nome) == 0 ){
				var->usata = true;
				lhs_valido = true;
			}
		}
	}
	free(left);

    if (!lhs_valido){
        free(copia2); free(tokens);
        free(lhs); free(rhs);
        free(copia); free(parole);
        return false;
    }

	//rhs: marca le variabili che compaiono come usate
	int n_rhs;
	char** right = split(rhs, " \t,+-*/", &n_rhs);
	for (int r = 0; r < n_lhs; r++){
		char* attuale = right[r];
		if (attuale == NULL) {
			continue;
		}
		for (size_t i = 0; i<vars->numero_elementi_attuali; i++){
			Variabile* var = list_get(vars,i);
			if (strcmp(attuale, var->nome) == 0 ){
				var->usata = true;
			}
		}
	}
	free(copia2); free(tokens);
	free(lhs); free(rhs);
	free(right);
	return true;
}

char* trovaTipoVar(char* dichiarazione){
	int n_tokens;
	char* copia = strdup(dichiarazione);
	char** tokens = split_variabile(copia, &n_tokens);

	for (int i = 0; i< n_tokens; i++){
		char* token = tokens[i];
		for (int j = 0; tipi_base[j] != NULL; j++){
			if (strcmp(token, tipi_base[j]) == 0){
				free(tokens);
				return token;
			}
		}
	}
	free(tokens);
	return NULL;
}

/*
controlla se in 'str' ci sta una dichiarazione corretta di 'typedef struct'
*/
bool check_typedef_struct(char* str){
	if(!str) return false;
	//typedef struct{ int x; } Point;
	char* copia = strdup(str);
	if(!copia) return false;
	int n_token;
	char** tokens = split(copia, " \t{};", &n_token); //typedef, struct,  int, x, Point

	bool result = false;
	if (n_token >= 2){
		result = strcmp(tokens[0],"typedef") == 0 && strcmp(tokens[1],"struct") == 0;
	}

	free(copia);
	free(tokens);
	return result;
}


/*
Stampa le informazioni di uno StructDef in formato leggibile.
Esempio output:
[Struct] riga 5: tag='Point' alias=[Point]
[Struct] riga 10: tag='Tag' alias=[Vec, *VecPtr]
*/
void stampaStructDef(StructDef* sd) {
    if (!sd) {
        printf("[Struct] NULL\n");
        return;
    }
    
    printf("[Struct] riga %d: ", sd->riga_dichiarata);
    
    // Tag opzionale
    if (sd->tag) {
        printf("tag='%s' ", sd->tag);
    } else {
        printf("tag=(anonimo) ");
    }
    
    // Alias
    printf("alias=[");
    for (int i = 0; i < sd->numero_alias; i++) {
        printf("%s", sd->alias[i]);
        if (i < sd->numero_alias - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

_Bool ricerca_array(char *str, const char *arr[]){
	if(!str||!arr){return 	0;}


	for (int i = 0; arr[i] != NULL; i++){
		if (strcmp(arr[i], str) == 0){
			return 1;
		}
	}
	return 0;
	
}

/*
rimpiazza asterischi con spazi in-place
*/
void elimina_asterischi(char *str){
	for (size_t i = 0; i < strlen(str); i++){
		if (str[i] == '*'){
			str[i] = ' ';
		}
	}
}