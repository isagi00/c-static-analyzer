#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

#include "supporto.h"


/*
data una parola, ritorna se la parola è equivalente a un tipo valido, che sia base oppure definita da utente
- parola: stringa da controllare. es: "hello world"
ritorna true se la parola data è una dichiarazione di tipo corretto.
*/
bool check_tipo(char* parola, List* struct_definite, Statistiche* stat){
	//FORSE: ELIMINA * e [] dalla parola
	char* copia = strdup(parola);
	remove_asterischi(copia);
	char* pulito = remove_spazi_dxsx_v2(copia);
	// printf("[controllloTipo] check sul tipo '%s'\n", pulito);

	//caso: 'char'
	if(strcmp(pulito,"char")==0){
		return true;
	}
	//casi: 'int' 'long' 'short
	else if(strcmp(pulito,"int")==0 || strcmp(pulito,"long")==0 || strcmp(pulito,"short")==0){
        return true;
    }
	//casi: 'float', 'double'
	else if(strcmp(pulito,"float")==0 || strcmp(pulito,"double")==0){
        return true;
    }
	//caso: 'bool'
	else if(strcmp(pulito,"bool")==0){
        return true;
    }

	//gestisci casi per tipi definiti dall'utente
	if (struct_definite){
		for (size_t i = 0; i < struct_definite->numero_elementi_attuali; i++){
			StructDef* sd = list_get(struct_definite,i);
			for(int j = 0; j< sd->numero_alias; j++){
				if(strcmp(pulito, sd->alias[j]) == 0){
					printf("[controllotipo] variabile di tipo definito dall'utente '%s'\n", sd->alias[j]);
					return true;
				}
			}
		}
	}

	stat->tipi_dato_scorretti++;
	// stat->errori_rilevati++;
	printf("[check_tipo] errore di tipo");
    return false;
}

// /*
// controlla se parola data è di tipi base.
// */
// bool check_tipo_base(char* parola){
// 	//caso: 'char'
// 	if(strcmp(parola,"char")==0){
// 		return true;
// 	}
// 	//casi: 'int' 'long' 'short
// 	else if(strcmp(parola,"int")==0 || strcmp(parola,"long")==0 || strcmp(parola,"short")==0){
//         return true;
//     }
// 	//casi: 'float', 'double'
// 	else if(strcmp(parola,"float")==0 || strcmp(parola,"double")==0){
//         return true;
//     }
// 	//caso: 'bool'
// 	else if(strcmp(parola,"bool")==0){
//         return true;
//     }
// 	printf("[check_tipo] errore di tipo");
//     return false;
// }


/*
controlla se il nome della variabile dichirata è consentita.
- parola: nome della variabile : es. 'var'
ritorna true se il nome della variabile dichiarata è valida.
*/
bool check_nome(char* parola, Statistiche* stat){
	if (parola == NULL || strlen(parola) == 0){
		printf("[controlloNome] nome vuoto o NULL. \n");
		return false;
	}

	char* copia = strdup(parola);
	if (copia == NULL) return false;
	char* pulito = remove_spazi_dxsx(copia);
    int n = strlen(pulito);

	if(n == 0){
		printf("[check_nome] nome vuoto dopo pulizia spazi dx,sx. \n");
		return false;
	}
    
	for (int i = 0; i<n; i++){
		//primo carattere puo essere solo lettera o '_'
		if (i == 0){
			if(!isalpha(pulito[i])  && pulito[i] != '_'){ 
				printf("[check_nome] il nome della variabile non può iniziare con '%c'. \n", pulito[i]);
				free(copia);
				stat->nomi_variabili_non_corretti++;
				return false;
			}
		}
		//gli altri caratteri possono essere numeri, lettere o underscore
		else {
			if(!isalnum(pulito[i]) && pulito[i] != '_'){
				printf("[check_nome] carattere '%c' non può essere nome variabile. \n", pulito[i]);
				free(copia);
				stat->nomi_variabili_non_corretti++;
				return false;
			}
		}
	}
	
	//controlla se è keyword
	if (check_keyword(pulito)) {
		printf("[check_nome] '%s' è una parola chiave riservata.\n", pulito);
        free(copia);
		stat->nomi_variabili_non_corretti++;
        return false;
	}

	free(copia);
	return true;

}

/*
controlla se l'alias di una struct è valida o non.
ritorna il nome dell'alias valido
*/
bool check_struct_nome_alias(char* nome, Statistiche* stat){
	if (!nome || strlen(nome) == 0) return false;

	char* copia = strdup(nome);
	char* pulito = remove_spazi_dxsx_v2 (copia);

	char alias[128] = {0};	//buffer per copiare alias
	//copia nel buffer il nome dell'alias, saltando spazi. 
	//se l'alias è un array viene trocata la parte dell'array.
	for (size_t ptr = 0; ptr < strlen(pulito); ptr++){
		if (pulito[ptr] == '['){	//alias[1024] termina su '['
			break;
		}

		if (isalnum(pulito[ptr]) || pulito[ptr] == '_'){
			alias[ptr] = pulito[ptr];
		}
		else{
			alias[ptr] = ' ';
		}
	}
	char* alias_pulito = remove_spazi_dxsx_v2(alias);
	alias_pulito[strlen(alias_pulito)] = '\0';
	bool risultato = check_nome(alias_pulito, stat);
	free(copia);
	if(risultato){
		// printf("check_struct_nome_alias: nome valido: '%s' -> alias: '%s'\n", pulito, alias_pulito);
	}
	else{
		printf("check_struct_nome_alias: nome non valido: '%s' -> alias: '%s'\n", pulito, alias_pulito);
	}
	return risultato;
}

/*
	fa parsing su una dichirazine typedef struct e restituisce una StructDef* se valida.
	gestisce: 
	- 'typedef struct { ... } alias;
	- 'typedef struct [tag] { ... } alias;
	- 'typedef struct { ... } alias1, *alias2;
	- typedef multilinea tramite buffer
 */
StructDef* parse_struct_typedef(char* buffer, int riga_inizio, Statistiche* stat){
	if (!buffer) return NULL;

	char* str = remove_spazi_dxsx_v2(buffer);
	if (!str) return NULL;
	//check su 'typedef struct'
	if (strncmp(str, "typedef ", 8) != 0){
		free(str);
		return NULL;
	}
	char* dopo_typedef = str + 7;	//salta 'typedef'
	while (*dopo_typedef == ' ' || *dopo_typedef == '\t') dopo_typedef++; //salta spazi
	if (strncmp(dopo_typedef, "struct ", 7) != 0){
		free(str);
		return NULL;
	}
	printf("[parsetypedefstruct]: ok 'typedef struct' alla riga: %i\n", riga_inizio);
	
	//crea il StructDef, con [tag] e alias
	StructDef* sd = struct_create();
	if (!sd) { free(str); return NULL; };

	char* ptr = dopo_typedef + 6; //salta 'struct'
	while (*ptr == ' ' || *ptr == '\t') ptr++; //salta spazi

	//caso [opzionale]: 'typedef struct [tag] { ... } alias';, con [tag]
	if (*ptr != '{'){
		//estrai tag
		int inizio_tag = ptr - str;
		while (*ptr && *ptr != '{' && *ptr != ' ' && *ptr != '\t') ptr++; //accumula solo lettere
		int len_tag = ptr - (str + inizio_tag); 
		if (len_tag > 0){
			sd->tag = strndup(str+inizio_tag, len_tag);
		}
		while (*ptr == ' ' || *ptr == '\t') ptr++; //salta spazi dopo il tag
	}

	//deve esserci '{'
	if(*ptr != '{'){
		printf("[parsetypedefstruct] manca '{' alla riga %i\n", riga_inizio);
		struct_free(sd);
		free(str);
		return NULL;
	}

	//trova '}'
	char* close_brace = strchr(ptr, '}');
	if (!close_brace){
		printf("[parsetypedefstruct] manca '}' alla riga %i\n", riga_inizio);
		struct_free(sd);
		free(str);
		return NULL;
	}
	if (close_brace < ptr){
		printf("[parsetypedefstruct] '}' prima di '{' durante la dichiarazione del typedef alla riga:  %i\n", riga_inizio);
		struct_free(sd);
		free(str);
		return NULL;
	}

	//dopo '}' ci devono essere alias
	ptr = close_brace + 1;
	while (*ptr == ' ' || *ptr == '\t') ptr++; //salta spazi

	//deve finire con ';'
	char* semi = strchr(ptr, ';');
	if (!semi){
		printf("[parsetypedefstruct] manca ';' alla riga %i\n", riga_inizio);
		struct_free(sd);
		free(str);
		return NULL;
	}
	*semi = '\0';

	//parsing alias
	char* alias_str = strdup(ptr);	//copia sulla parte degli alias
	int n_token;
	char** tokens = split(alias_str, ",", &n_token);
	if (n_token == 0){
		printf("[parsetypedefstruct] nessun alias per la struct alla riga: %i\n", riga_inizio);
		free(tokens);
		free(alias_str);
		struct_free(sd);
		free(str);
		return NULL;
	}

	//primo passaggio sugli alias per contare alias validi
	int validi = 0;
	for (int i = 0; i<n_token; i++){
		char* pulito = remove_spazi_dxsx_v2(tokens[i]);
		if (pulito && strlen(pulito) > 0 && check_struct_nome_alias(pulito,stat)){
			validi++;
		}
		free(pulito);
	}
	if(validi == 0){
		printf("[parsetypedefstruct] nessun alias valido per la struct alla riga: %i\n", riga_inizio);
		free(tokens);
		free(alias_str);
		struct_free(sd);
		free(str);
		return NULL;
	}

	sd->numero_alias = validi;
	sd->alias = malloc(sizeof(char*) * validi);
	sd->riga_dichiarata = riga_inizio;

	//secondo passaggio per popolare il nome degli alias
	int idx = 0;
	for (int i = 0; i < validi && idx < validi; i++){
		char* pulito = remove_spazi_dxsx_v2(tokens[i]);
		if(pulito && strlen(pulito) > 0 && check_struct_nome_alias(pulito,stat)){
			sd->alias[idx] = strdup(pulito);
			idx++;
		}
		free(pulito);
	}


	//free dopo tutta sta roba
	free(tokens);
	free(alias_str);
	free(str);

	//debug
	printf("[parsetypdefstruct] OK alla riga %i\n", riga_inizio);

	return sd;

}

/*
funzione controlla la dichiarazione corretta degli argomenti del main().
gestisce anche il caso in cui si ha come argomento 'void'
- args: argomenti del main. es: "int argc, char*argv[]"
ritorna true se gliu argomenti sono corretti.
*/

bool check_main_args(char* args, Statistiche* stat){
	if (args == NULL) return false;

	//copia modificabile di args
	char* copia = strdup(args);		//free() prima di return
	if (!copia) return false;

	//elimina spazi avanti e dietro
	char* copia_pulita = remove_spazi_dxsx(copia);

	//caso: argomenti vuoti, ovvero 'int main()'
	if(strlen(copia_pulita) == 0){
		free(copia);
		return true;
	}

	//caso: 'void'
	if(strcmp(copia_pulita, "void") == 0){
		free(copia);
		return true;
	}

	//caso: 'int <nome>, char [*][*] <nome>[]'
	int numero_parti;
	char** parti = split(copia_pulita, ",", &numero_parti);	//free() prima di return
	if (numero_parti != 2){
		free(copia);
		free(parti);
		printf("[check_main_args] numero di argomenti non valido.\n");
		return false;
	}

	//verifica di 'int <nome>'
	char* prima_parte = remove_spazi_dxsx(parti[0]);
	// int len_prima_parte = strlen(prima_parte);
	int numero_token_prima_parte;
	char** tokens_prima_parte = split(prima_parte, " \t", &numero_token_prima_parte);
	//deve avere esattamente 2 token: 'int' + 'nome_valido'
	bool ok_primaparte = (numero_token_prima_parte == 2 &&
						  strcmp(tokens_prima_parte[0],"int") == 0 &&
						  check_nome(tokens_prima_parte[1],stat));
	free(tokens_prima_parte);
	if (!ok_primaparte){
		free(copia);
		free(parti);
		printf("[check_main_args] dichiarazione 'int <nome>' non valido. \n");
		return false;
	}

	//verifica di 'char[*][*] <nome>[]
	char* seconda_parte = remove_spazi_dxsx(parti[1]);
	int len_seconda_parte = strlen(seconda_parte);
	int pos = 0;
	//il primo token deve essere necessariamente 'char'
	while(pos<len_seconda_parte && isspace(seconda_parte[pos])) pos++;//salta spazi iniziali
	if(strncmp(&seconda_parte[pos], "char", 4) != 0){
		free(copia);
		free(parti);
		printf("[check_main_args] dichiarazione 'char' invalido. \n");
		return false;
	}
	pos += 4;

	//conta asterischi
	int asterischi = 0;
	while (pos < len_seconda_parte){
		if (seconda_parte[pos] == '*'){
			asterischi++;
			pos++;
		}
		else if(isspace(seconda_parte[pos])){
			pos++;
		}
		else break;	//fine asterischi
	}
	//deve esserci almeno un asterisco, massimo 2
	if (asterischi < 1 || asterischi > 2){
		free(parti);
		free(copia);
		printf("[check_main_args] asterischi mancanti oppure numero di asterischi eccessivo. \n");
		return false;
	}


	//leggi il nome della variabile
	while (pos<len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salta spazi

	int inizio_nome = pos;
	while (pos < len_seconda_parte && (isalnum(seconda_parte[pos]) || seconda_parte[pos] == '_')){
		pos++;
	}
	int len_nome = pos - inizio_nome;
	if (len_nome == 0){
		free(parti);
		free(copia);
		printf("[check_main_args] specifica un nome 'char <nome>'.\n");
		return false;
	}
	//estrai e valida nome
	char* nome_var = strndup(&seconda_parte[inizio_nome], len_nome);
	if (!nome_var || !check_nome(nome_var, stat)){
		free(nome_var);
		free(copia);
		free(parti);
		printf("[check_main_args] nome inserito in 'char <nome' invalido. \n");
		return false;
	}
	free(nome_var);

	//verifica del '[]' finale
	while (pos < len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salti spazi
	if (pos >= len_seconda_parte || seconda_parte[pos] != '['){	//carattere in secondaparte[pos] deve essere '['
		free(copia);
		free(parti);
		printf("[check_main_args] manca il '[' dopo argv. \n");
		return false;
	}
	pos++;

	while (pos < len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salta spazi
	if (pos >= len_seconda_parte || seconda_parte[pos] != ']'){	//carattere in secondaparte[pos] deve essere ']'
		free(copia);
		free(parti);
		printf("[check_main_args] manca il ']' dopo argv. \n");
		return false;
	}
	pos++;

	//non deve rimanere nulla
	while(pos<len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salta spazi

	bool ok_secondaparte = (pos == len_seconda_parte);

	free(copia);
	free(parti);
	return ok_secondaparte;	//ok_primaparte verificato prima.
}




// _Bool isInt(char* valore, bool isSigned){
//     int i = 0;
//     // salta il segno se c'è
//     if( (valore[0] == '-') || (valore[0] == '+' && isSigned) ) {
//         i = 1;
//     }
// 	else{
// 		return 0;
// 	}
    
//     // se è solo "+" o "-" 
//     if(valore[i] == '\0'){
// 		return 0;
// 	}
    
//     // controlla che tutti i caratteri restanti siano cifre
//     while(valore[i] != '\0') {
//         if(!isdigit(valore[i])){
// 			return 0;
// 		}
//         i++;
//     }
    
//     return 1;
// }

// bool isChar(char* valore, bool isSigned){
// 	bool char_lettera_ok = false;
// 	bool char_numero = false; 
// 	bool char_numero_ok =false;
// 	if(strlen(valore)<=3 && strlen(valore)>=2 && valore[0]=='\'' && valore[strlen(valore)-1]=='\''){
// 		char_lettera_ok = true;
// 	}

//     if(isInt(valore, isSigned)){
// 		char_numero = true;
// 	}

// 	if(char_numero){
// 		int n=atoi(valore);	//cast "42" -> 42 ?
// 		if(isSigned){
// 			if(n >= -128 && n <= 127){
// 				char_numero_ok = true;
// 			}
// 		}
// 		else {
// 			if(n >= 0 && n <= 255){
// 				char_numero_ok = true;
// 			}
// 		}
// 	}
// 	return(char_lettera_ok || char_numero_ok);
// }


// /*
// controlla se il valore assegnato alla variabile è valido o non.
// -valore: stringa che rappresente il valore assegnato alla variabile. es:"10"
// -tipo: tipo di dato dichiarato per la variabile. es: "int"
// ritorna true se il valore assegnato è valido
// @TODO:
// unsigned int x = 42;
// unsigned long x = 100000UL;
// unsigned long long x = 9000000000ULL;
// long long x = -9000000000LL;
// long double x = 3.14159265358979323846L;
// L è opzionale
// */
// bool controlloValoreAssegnato(char* valore,char* tipo){
// 	// printf("[controlloValoreAssegnato] valore : '%s'\n", valore);
// 	//char
// 	if(strcmp(tipo,"char")==0){      
// 		return isChar(valore, true);
//     }
// 	//int, long, short
// 	else if(strcmp(tipo,"int")==0 || strcmp(tipo,"long")==0 || strcmp(tipo,"short")==0){ 

//         for(size_t i = 0; i <strlen(valore); i++){
// 			if(i== 0 && valore[i] == '"') return false;	//se inizia con " sicuramente è stringa, salta
// 			if(i == 0 && (valore[i] == '+' || valore[i] == '-')) continue; // segno ok
//        	 	if(valore[i] == ' ' || valore[i] == '\t') continue;	//salta spazi
// 			/*
// 			l'assegnazione 'a = b + c' deve ritornare corretto, se b e c sono variabili interi. 
// 			altrimenti ritornerebbe false scorrettaemnte
// 			*/
//        		// if(!digit(valore[i])) return false;	//non è numero
//        		// if(isalpha(valore[i])) return false; // no lettere, solo cifre

//             // if( !(isalnum(valore[i]) ||valore[i]=='+'||valore[i]=='-'||valore[i]==' '|| valore[i]=='\t') ){
//             //     return false;
//             // }
//         }
//         return true;
//     }
// 	//float, double
// 	else if(strcmp(tipo,"float")==0 || strcmp(tipo,"double")==0){
//     	bool haPunto = false;
//     	bool haE = false;
//     	int len = strlen(valore);

//     	for(int i = 0; i < len; i++){
//         	if(valore[i] == '.'){
//             	if(haPunto || haE) return false;
//             	haPunto = true;
//         	}
//         	else if(valore[i] == 'e' || valore[i] == 'E'){
//             	if(haE) return false; 
//             	haE = true;
//         	}
//         	else if(valore[i] == '+' || valore[i] == '-'){
//             	if(i != 0){
// 					return false; 
// 				} 
//         	}	
//         	else if(valore[i] == ' ' || valore[i] == '\t'){
//             	continue; 
//         	}
//         	else if(!isdigit(valore[i])){
//             	return false;
//         	}
// 			//tutto ok ritorna true
// 			return true;
//     	}
// 	//bool
// 	}
// 	else if(strcmp(tipo,"bool")==0){     
// 		// usa dirretamente funzione int
//         return false;
//     }
// 	// free(valore);
//     return true;
// }



bool check_tipo_composto(char** tokens_tipo, int n_tipo, char* tipo_base_out){
	if(n_tipo <= 0) return false;

	int base_count = 0;
	int long_count = 0;
	int short_count = 0;
	int sign_count = 0;
	char* base_type = NULL;

	for (int i = 0; i< n_tipo; i++){
		char* t = tokens_tipo[i];
		if (strcmp(t, "int") == 0 || strcmp(t, "char") == 0 || strcmp(t, "float") == 0 || 
			strcmp(t, "double") == 0 || strcmp(t, "bool") == 0 || strcmp(t, "_Bool") == 0){
				base_count ++;
				base_type = t;
			}
		else if (strcmp(t, "long") == 0) long_count++;
		else if (strcmp(t, "short") == 0) short_count++;
		else if (strcmp(t, "unsigned") == 0 || strcmp(t, "signed") == 0) sign_count++;
		else return false;
	}


	//regole
	if (base_count > 1) return false;
	if (long_count > 2) return false;
	if (short_count > 1) return false;
	if (sign_count > 1) return false;
	if (short_count && long_count) return false;

	if (base_type){
		if (strcmp(base_type,"float")==0  && (long_count||short_count||sign_count)) return false;
		if (strcmp(base_type,"double")==0 && (short_count||sign_count||long_count>1)) return false; // "long double" ok
		if ((strcmp(base_type,"bool")==0 || strcmp(base_type,"_Bool")==0) && (long_count||short_count||sign_count)) return false;
		if (strcmp(base_type,"char")==0   && (long_count||short_count)) return false; // signed/unsigned char ok
	}

	if (tipo_base_out){
		if (base_type){
			strcpy(tipo_base_out, base_type);
		}
		else{
			strcpy(tipo_base_out, "int");
		}
	}
	return true;
}


bool check_dichiarazione_var(char* str, int riga, List* struct_definite, List* var_dichiarate, Statistiche* stat ){
	if (!str) return false;
	//duplica e splitta
	char* copia_str = strdup(str);	//"int a = 10"

	//spezza sulle virgole: ogni pezzo è una dichiarazione
	//	il tipo dichiarato (+ storage/qual) appare nel primo
	int n_decl;
	char** dichiarazioni = split(copia_str, ",", &n_decl);
	if (!dichiarazioni || n_decl < 1){
		free(copia_str);
		return false;
	}

	//dal primo dichiaratore estrai [storage][qual][tipo...][nome][=valore]
	int n_parti;
	char** parti_var = split_variabile(dichiarazioni[0], &n_parti);	// "int", "a", "10"
	char** parti_var_copia = parti_var;
	//	rileva keyword di storage e qualificatori
	_Bool ha_storage = 0;
	char* stor = NULL;
	_Bool ha_qual = 0;
	char* qual = NULL;
	while ((!ha_storage && ricerca_array(parti_var[0], storage)) || (!ha_qual && ricerca_array(parti_var[0], qualificatori)))
	{
		if (!ha_storage && ricerca_array(parti_var[0], storage)){
			ha_storage = 1;
			stor = strdup(parti_var[0]);
			parti_var++;
			n_parti--;
		}
		if (!ha_qual && ricerca_array(parti_var[0], qualificatori)){
			ha_qual = 1;
			qual = strdup(parti_var[0]);
			parti_var++;
			n_parti--;
		}
	}
	if (!parti_var || n_parti < 1){ 
		free(parti_var_copia);
		free(dichiarazioni);
		free(copia_str);
		if (stor) free(stor);
		if (qual) free(qual);
		return false;
	}

	//separa : [tipo...] [nome] [valore]
	//valore è ultimo token solo se c'è '='
	bool primo_ha_valore = (strchr(dichiarazioni[0], '=') != NULL);
	int n_tipo = primo_ha_valore ? n_parti -2: n_parti -1; //se ha valore togli 2, '[nome][valore]'. se non ha valore togli [nome]

	if (n_tipo < 1){
		free(parti_var_copia);
		free(copia_str);
		free(dichiarazioni);
		if (stor) free(stor);
		if (qual) free(qual);
		return false;
	}

	// int idx_nome = n_tipo;
	// int idx_valore = n_tipo + 1;

	//-- controllo tipo
	// il tipo è condiviso tra tutte le dichiarazioni
	char tipo_base[32] = {0};
	bool tipo_ok;
	if (n_tipo == 1){
		tipo_ok = check_tipo(parti_var[0], struct_definite, stat);
		//check_tipo incrementa stat.tipo_dato_scorretti internamente
		if(tipo_ok){
			strncpy(tipo_base, parti_var[0], sizeof(tipo_base)-1);
		}
	}
	else{
		tipo_ok = check_tipo_composto(parti_var, n_tipo, tipo_base);
	}
	if (!tipo_ok){
		if(n_tipo>1){
			stat->tipi_dato_scorretti++;
		}
		free(parti_var_copia);
		free(dichiarazioni);
		free(copia_str);
		if(stor) free(stor);
		if(qual) free(qual);
		return false;
	}

	// //-- controllo nome
	// bool nome_ok = check_nome(parti_var[idx_nome], stat);
	// //controllo valore assegnato?
	// // bool valore_ok = true;
	// // if(primo_ha_valore && nome_ok){
	// // 	valore_ok = controlloValoreAssegnato(parti_var[idx_valore], tipo_base);
	// // 	if (!valore_ok){
	// // 		printf("[controllaDichiarazioneVariabile] valore '%s' non valido per tipo '%s'\n", parti_var[idx_valore], tipo_base);
	// // 	}
	// // }

	// bool risultato = tipo_ok && nome_ok;

	//ricostruisci il tipo completo come stringa unica
	// if (risultato){
	// 	char buffer_tipo[256] = {0};
	// 	for (int i = 0; i < n_tipo; i++){
	// 		if (i > 0) strcat(buffer_tipo, " "); //concatena spazio
	// 		strcat(buffer_tipo, parti_var[i]);	//concatena [tipo]
	// 	}

	// 	Variabile* var = malloc(sizeof(Variabile));
	// 	var->nome = strdup(parti_var[idx_nome]);
	// 	var->tipo = strdup(buffer_tipo);
	// 	var->riga_dichiarata = riga;
	// 	var->usata = false;
	// 	var->storage = ha_storage ? strdup(stor) : NULL;
	// 	var->qual = ha_qual ? strdup(qual) : NULL;
	// 	list_append(var_dichiarate, var);
	// }

	//ricostruisci il tipo come stringa unica
	char buffer_tipo[256] = {0};
	for(int i = 0; i < n_tipo; i++){
		if(i > 0){
			strcat(buffer_tipo, " ");
		}
		strcat(buffer_tipo,parti_var[i]);
	}

	//per ogni dichiarazione estrai nome e crea Variabile
	for (int d = 0; d < n_decl; d++){
		char* nome;
		if (d == 0){
			nome = strdup(parti_var[n_tipo]);
		}
		else{
			char* seg = strdup(dichiarazioni[d]);
			char* eq = strchr(seg,'=');
			if(eq){
				*eq = '\0';
			}
			char* nome_pulito = remove_spazi_dxsx_v2(seg);
			nome = strdup(nome_pulito);
			free(nome_pulito);
			free(seg);
		}
		stat->variabili_controllate++;
		//check sul nome
		if(!check_nome(nome, stat)){
			stat->errori_rilevati++;
			list_append(stat->righe_errore, (void*)(intptr_t) riga);
			printf("[check_dichiarazione_var] nome '%s' non valido alla riga %i\n", nome, riga);
			free(nome);
			continue;
		}

		//creazione Var
		Variabile* var = malloc(sizeof(Variabile));
		var->nome = nome;
		var->tipo = strdup(buffer_tipo);
		var->riga_dichiarata = riga;
		var->usata = false;
		var->storage = ha_storage ? strdup(stor) : NULL;
		var->qual = ha_qual ? strdup(qual) : NULL;
		list_append(var_dichiarate, var);
	}

	free(parti_var_copia);
	free(copia_str);
	if (stor) free(stor);
	if (qual) free(qual);
	return true;
}



/*
controlla se la riga corrente è una valida dichiarazione del main().
-riga: riga attuale
ritorna true se è una valida dichiarazione di main. es: "int main (int argc, char* argv[])"
*/
bool check_dichiarazione_main(char* riga, Statistiche* stats) {
	if(riga == NULL) return false;

	//trova parentesi e valida che sono nell'ordine corretto
	char* p_open = strchr(riga, '(');
	char* p_close = strchr(riga, ')');
	if (p_open == NULL || p_close == NULL || p_open >= p_close){
		return false;
	}

	//estrai parte prima di '(': "int main"
	int len_prima = p_open - riga;
	char* prima_parte_str = strndup(riga, len_prima);
	if (!prima_parte_str) return false;

	//processa prima parte
	char* prima_parte_pulito = remove_spazi_dxsx(prima_parte_str);
	int parole_prima_parte;
	char** tokens_primaparte = split(prima_parte_pulito, " \t", &parole_prima_parte);
	bool ok_segnatura = (parole_prima_parte == 2 &&
							strcmp(tokens_primaparte[0],"int") == 0 &&
							strcmp(tokens_primaparte[1], "main") == 0);
	free(tokens_primaparte);
	free(prima_parte_str);
	if(!ok_segnatura) return false;

	//estrai gli argomenti/contenuto tra le parentesi
	int len_args = p_close - p_open - 1;
	char* args= NULL;
	if (len_args>0) args = strndup(p_open+1, len_args);	//copia argomenti
	//se len_args==0, significa che è stato dichiarato 'int main()'
	bool ok_args=false;

	if (args==NULL || strlen(args) == 0){		//strlen considera 0 una serie di spazi bianchi(?)
		//caso: 'main()' oppure 'main(				)'
		ok_args = true; 
	}
	else{
		//casi: 'int main(int <nome>, char* <nome>[])'	... 
		ok_args = check_main_args(args, stats);
	}

	free(args);
	return ok_args;
}

/*
controlla se il tipo del typedef è valido: 
es: typedef [tipo] [alias1, alias2, alias3 ...]
ritorna true.
*/
bool check_typedef_tipo_e_nome(char* str, List* struct_definite, Statistiche* stats){
    if (!str) return false;

    char* copia = strdup(str);
	//TEST: elimina completamente gli asterischi
	remove_asterischi(copia);
	// printf("[check_typedef_tipo_e_nome] copia senza asterischi: '%s'\n", copia);

    if (!copia) return false;

	//1. tipo + alias principale
	//typedef tipo alias1,' alias2
    int n_parti;
    char** parti = split(copia, ",;", &n_parti);
    if (n_parti == 0){
        free(copia);
        return false;
    }

    int n_tokens;
    char** tokens = split(parti[0], " \t", &n_tokens);	
    if (n_tokens < 2){
        free(copia);
        free(tokens);
        return false;
    }
    // ultimo token = alias
    char* alias_principale = tokens[n_tokens - 1];

    if (!check_struct_nome_alias(alias_principale, stats)){
        free(copia);
        free(tokens);
        return false;
    }

	bool tipo_valido = false;

    // if (n_tokens == 2){ //int alias
    //     tipo_valido = check_tipo(tokens[0], struct_definite) && check_struct_nome_alias(tokens[1], stats);
    // }
    // else if (n_tokens == 3){	//long long alias
    //     tipo_valido = controllaLongShort_tre_sessione(tokens, stats);
    // }
    // else if (n_tokens == 4){	//unsigned long long alias
    //     tipo_valido = controllaLongShort_quattro_sessione(tokens, stats);
    // }
    // else if (n_tokens == 5){	//unsigned long long int alias
    //     tipo_valido = controllaLongShort_cinque_sessione(tokens, stats);
    // }

	if (n_tokens == 2){
		tipo_valido = check_tipo(tokens[0], struct_definite, stats) && check_struct_nome_alias(tokens[1], stats);
	}
	else{ //tipo composto
		char tipo_base_alias[32] = {0};
		tipo_valido = check_tipo_composto(tokens, n_tokens-1, tipo_base_alias) && check_struct_nome_alias(tokens[n_tokens-1], stats);
	}


    if (!tipo_valido){
        free(copia);
        free(tokens);
        return false;
    }

    //2. altri alias
	//typedef tipo alias1, 'alias2'
    for (int i = 1; i < n_parti; i++){
        char* alias = remove_spazi_dxsx_v2(parti[i]);
        if (!check_struct_nome_alias(alias, stats)){
            free(copia); free(tokens);
            return false;
        }
    }

    free(copia); free(tokens);
    return true;
}

/*
parsing per 'typedef [tipo] [alias]' singola linea, senza struct
ritorna una StructDef*
*/
StructDef* parse_typedef_oneline(char* clean, int n_riga, List* struct_definite, Statistiche* stats){
	//verifica che il primo token sia 'typedef' e non 'typedefsss'
	int n_token;
	char** tokens = split(clean, " \t;", &n_token);
	if (n_token == 0 || !tokens){
		free(tokens);
		return NULL;
	}
	if (strcmp(tokens[0], "typedef") != 0){
		printf("[parse_typedef_oneline] dichiarazione typedef non corretto alla riga: %i", n_riga);
		return NULL;
	}
	//se entra scorrettamente per typedef struct
	if (strcmp(tokens[1], "struct") == 0){
		return NULL;
	}

	//ricostruisci tutto dopo "typedef"
	char buffer[1024] = {0};
	
	for (int i = 1; i < n_token; i++){
		strcat(buffer, tokens[i]);
		strcat(buffer, " ");
	}
	if (!check_typedef_tipo_e_nome(buffer, struct_definite, stats)){
		printf("[parse_typedef_oneline] typedef invalido alla riga: %i", n_riga);
		return NULL;
	}

	//split sugli alias
	//'typedef tipo alias1, alias2, alias3 ...
	int n_parti;
	char** parti = split(buffer, ",;", &n_parti);
	if (n_parti == 0) return NULL;
	//'typedef tipo alias1 | alias2| alias3
	StructDef* sd = struct_create();
	sd->alias = malloc(sizeof(char*) * n_parti);
	sd->numero_alias = 0;
	sd->riga_dichiarata = n_riga;

	for (int i = 0; i < n_parti; i++){
		char* p = remove_spazi_dxsx_v2(parti[i]);

		char* alias = NULL;
		//'typedef tipo alias1| alias2| alias3 
		if (i == 0){
			// primo contiene 'tipo alias1' -> prendi ultimo token
			int n_tok;
			char** tok = split(p, " \t", &n_tok);
			// char* alias_principale = tok[n_tok-1];
			if (n_tok > 0){
				alias = strdup(tok[n_tok-1]);
			}
		}
		//altri alias
		else{
			alias = strdup(p);
		}
		if (alias && check_struct_nome_alias(alias, stats)){
			sd->alias[sd->numero_alias++] = alias;
		}
	}

	printf("[parse_typedef_oneline] typedef valido (%d alias) riga %d\n",
		   sd->numero_alias, n_riga);

	return sd;
}


/*
la funzione controlla la correttezza di:
1. dichiarazione del main + argomenti
2. dichiarazione delle strutture di controllo, es: if, else, for, while
3. dichiarazione delle variabili

*/
List* check_file(char* filename, Statistiche* stats){
	//apertura file e gestione errore
	FILE* file = fopen(filename, "r");
	if (!file){
		printf("[ControlloVariabile] errore apertura file: %s\n", filename);
		return NULL;
	}

	//init variabili utili
	char riga[1024];
	int n_riga = 1;
	int n_graffe = 0;
	bool main_dichiarato = false;

	bool in_struct = false;
	List* struct_definite = list_create();
	char struct_buffer[4028] = {0}; //buffer per accumulare righe se struct righe multiple
	int riga_inizio_struct = 0;

	List* var_dichiarate = list_create();

	//scorre le righe del file
	while (fgets(riga, sizeof(riga), file) != NULL){
		//rimpiazza \n con \0
		riga[strcspn(riga, "\n")] = '\0';
		char* clean = remove_spazi_dxsx_v2(riga);
		
		//salta righe vuote
		if (strlen(clean) == 0){
			n_riga ++;
			continue;
		}

		//conta graffe della riga
		for(size_t k = 0; k < strlen(clean); k++){
			if (clean[k] == '{') n_graffe++;
			else if(clean[k] == '}') n_graffe--;
		}


		//rileva inizio typedef struct
		if (!in_struct && check_typedef_struct(clean)){
			printf("[checkfile]inizio 'typedef struct' alla riga %i \n", n_riga);
			//caso in cui il typedef struct avviene su unica riga, quindi ci sta '}'
			if (strchr(clean, '}')){
				StructDef* sd = parse_struct_typedef(clean, n_riga, stats);
				if (sd) list_append(struct_definite, sd);
				else stats->errori_rilevati++;
			}
			//caso 'typedef struct' multiriga
			else{
				in_struct = true;
				riga_inizio_struct = n_riga;
				strcpy(struct_buffer, clean);
				strcat(struct_buffer, " ");	//spazi tra righe della struct
			}
			free(clean);
			n_riga++;
			continue;
		}


		//buffering struct multiriga
		if (in_struct){
			strcat(struct_buffer, clean);	//aggiungi la riga nel buffer
			strcat(struct_buffer, " ");		//spazio dopo la riga
			//controlla se è terminato 
			if (strchr(clean, '}') && strchr(clean, ';')){
				StructDef* sd = parse_struct_typedef(struct_buffer, riga_inizio_struct, stats);
				if (sd){
					list_append(struct_definite, sd);
					printf("[checkfile] strcut multiriga completato alla riga %i\n", n_riga);
				}
				else{
					stats->errori_rilevati++;
				}
				in_struct = false;
				struct_buffer[0] = '\0';
			}
			free(clean);
			n_riga++;
			continue;
		}

		//rileva typedef [tipo] [nome];
		if (strncmp(clean, "typedef", 7) == 0){
			StructDef* sd = parse_typedef_oneline(clean, n_riga, struct_definite, stats);
			if (sd){
				list_append(struct_definite, sd);
				printf("[checkfile]typedef valido alla riga %i\n", n_riga);
				n_riga++;
				continue;
			} 
			else {
				printf("[checkfile] typedef non valido alla riga %i\n ", n_riga);
				stats->errori_rilevati++;
				// int* ptr_riga = malloc(sizeof(int));
				// *ptr_riga = n_riga;
				// list_append(stats->righe_errore, ptr_riga);
				list_append(stats->righe_errore, (void*) (intptr_t) n_riga);
				n_riga++;
				continue;
			}
		}
		

		//split su '{' '}' e ';', controllo split vuoto.
		//tokens ha: "int test = 0", "char hello, ...", array di stringhe.
		int n_tokens;
		char** tokens = split(clean, "{};", &n_tokens);
		if(!tokens || n_tokens == 0){
			free(clean);
			free(tokens);
			n_riga++;
			continue;
		}

		//processa ogni token
		for(int i = 0; i < n_tokens; i++){
			// un token è del tipo: "int test = 0"
			char* token = tokens[i];
			if(!token) continue;
			char* pulito = remove_spazi_dxsx_v2(token);
			if (!pulito || strlen(pulito) == 0){	//salta se pulito = ""
				free(pulito);
			 	continue;
			}
		
			//controllo dichiarazione main() e gestione main() duplicato
			if (!main_dichiarato && check_dichiarazione_main(pulito, stats)){
				printf("[checkfile] dichiarazione main() corretta alla riga: %i\n", n_riga);
				main_dichiarato = true;
				free(pulito);
				continue;
			}
			else if(main_dichiarato && check_dichiarazione_main(pulito, stats)){
				stats -> errori_rilevati++;
				// int* ptr_riga = malloc(sizeof(int));
				// *ptr_riga = n_riga;
				// list_append(stats->righe_errore, ptr_riga);
				list_append(stats->righe_errore, (void*) (intptr_t) n_riga);
				printf("[checkfile] dichiarato un altro main alla riga: %i\n", n_riga);
				free(pulito);
				continue;
			}

			//controlla se la dichiarazione del return è valido
			if(strncmp(pulito, "return", 6) == 0){
				// printf("[ControlloVariabile] attualmente nel controllo del return, pulito = '%s' \n", pulito);
				if (!main_dichiarato){
					printf("[checkfile] return fuori dal main alla riga %i \n", n_riga);
					int* ptr_riga = malloc(sizeof(int));
					*ptr_riga = n_riga;
					list_append(stats->righe_errore, ptr_riga);
					stats->errori_rilevati++;
				}
				else if (check_return_valido(pulito, var_dichiarate)){
					printf("[checkfile] return ok alla riga: %i \n", n_riga);
				}
				else{
					printf("[checkfile] return non valido alla riga: %i \n", n_riga);
					// int* ptr_riga = malloc(sizeof(int));
					// *ptr_riga = n_riga;
					// list_append(stats->righe_errore, ptr_riga);
					list_append(stats->righe_errore, (void*) (intptr_t) n_riga);
				}
				free(pulito);
				continue;
			}


			// //controllo dichiarazione delle variabili
			// if(controllaDichiarazioneVariabile(pulito, n_riga, struct_definite, var_dichiarate, stats)){
			// 	// Variabile
			// 	stats->variabili_controllate++;	//conta solo dichiarazione valida di var
			// 	printf("[checkfile] variabile valida '%s' alla riga %i\n", pulito, n_riga);
			// 	//adrebbe aggiunto alla lista delle variabili dichiarate
			// }

			if(check_dichiarazione_var(pulito, n_riga, struct_definite, var_dichiarate, stats)){
				// stats-> variabili_controllate ++;
				printf("[checkfile] variabile valida '%s' alla riga %i\n", pulito, n_riga);
			}

			// se non è dichiarazione, controlla assegnazione
			else if (check_assegnazione_valida(pulito,var_dichiarate)){
				//controllo assegnazione, es: 'a = b + 10;
				printf("[checkfile] assegnazione valida alla riga: %i \n", n_riga);
				free(pulito);
 				continue;
			}
			//altrimenti errore
			else{
				stats->variabili_controllate++;
				stats->errori_rilevati++;

				// int* ptr_riga = malloc(sizeof(int));
				// *ptr_riga = n_riga;
				// list_append(stats->righe_errore, ptr_riga);
				list_append(stats->righe_errore, (void*) (intptr_t) n_riga);

				printf("[checkfile] dichiarazione/assegnazione non valida alla riga: %i \n", n_riga);
			}
			

			//altri controlli andranno qui

			free(pulito);
		}

		free(tokens);
		free(clean);
		n_riga++;
	}

	fclose(file);

	//controlla che il numero delle graffe sia corretto
	if (n_graffe != 0) {
		printf("[checkfile] ATTENZIONE: parentesi graffe non bilanciate (differenza: %i)\n", n_graffe);
		stats->errori_rilevati++;
	}
	
	if(!main_dichiarato){
		printf("[checkfile] ATTENZIONE: manca la dichiarazione del main()\n");
		stats->errori_rilevati++;
	}

	for (size_t s = 0; s < struct_definite->numero_elementi_attuali; s++){
		StructDef* sd = list_get(struct_definite, s);
		for (int j = 0; j < sd->numero_alias; j++){
			char* alias = sd->alias[j];
			printf("STRUCT: alias : %s, numero alias: %d", alias, sd->numero_alias);
		}
		printf("\n");
	}

	return var_dichiarate;
}








