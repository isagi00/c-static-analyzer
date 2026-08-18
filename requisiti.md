
Traccia
Sviluppare un'applicazione che, dato un file contenente del codice C, lo elabori come segue:
1. Verifichi che i nomi delle variabili rispettino le regole di composizione del nome di un identificatore
2. Controlli che i tipi di dato usati nella dichiarazione delle variabili siano validi dal punto di vista sintattico (ovvero che il nome sia un nome valido), sia per tipi di dato standard C che per i tipi di dato definiti dall'utente mediante typedef
3. Controlli che non vi siano variabili non utilizzate
4. Produca le statistiche di elaborazione riportate nella sezione "Specifiche"

Specifiche
Assunzioni
Si puo' assumere che:
1. il file contenente il codice C fornito come input sia costituito dal solo blocco di codice della funzione main e che non ci siano altre funzioni
2. che tutte le variabili locali siano dichiarate all'inizio della funzione main in righe contigue e che le variabili globali siano dichiarate prima del main in righe contigue;

Input 
Il programma prevede tre parametri di input 
1. -i, --in (notazione doppio trattino) per specificare il file di input
2. -o --out (notazione doppio trattino) per specificare il file di output su cui salvare le statistiche. Se questo parametro non viene usato, l'output e' automaticamente ridirezionato sullo stdout
3. -v, --verbose (notazione doppio trattino) per visualizzare sullo stdout le statistiche di elaborazione




Esempi di esecuzione
- myPreCompiler -i nome_file_input.c \
    memorizza nel file nome_file_output  le statistiche prodotte.
- myPreCompiler -vi nome_file_input.c
    invia su stdout le statistiche di elaborazione
- myPreCompiler -i nome_file_input.c -o nome_file_output -v
    memorizza nel file nome_file_output  le statistiche prodotte e le invia anche sullo stdout

Statistiche di elaborazione
Il programma myPreCompiler deve calcolare le seguenti statistiche di elaborazione:
1. Numero totale di variabili controllate
2. Numero totale di errori rilevati (identificatori non corretti, tipi di dato non  corretti, variabili non utilizzate)
3. Numero di variabili non utilizzate [ok]
4. Numero di nomi di variabili non corretti
5. Numero di tipi di dato non corretti
6. Per ogni errore rilevato, il numero di riga nel file (e il nome del file, qualora consideriate che il programma possa analizzare più file).  [ok]
7. Per ogni variabile non utilizzata, il nome della variabile. [ok]

ERRORI = NOMI + TIPI + NON UTILIZZATI
NON UTILIZZATI
NOMI
TIPI

Errori
L'applicazione sviluppata deve gestire i seguenti errori
1. errore nei parametri di input, opzioni e relativi argomenti specificate da linea di comando
2. errore di apertura dei file di input e output
3. errore di chiusura file
4. errore di lettura da file - ad esempio file vuoto o corrotto
5. errore di scrittura su file

Struttura del programma e uso della memoria
1. Il programma NON deve essere monolitico, ma composto da un main e dalle varie funzioni che sviluppano le funzionalità principali e funzionalità di supporto.
2. Il codice deve essere organizzato in almeno tre file: un file con il main, almeno un file con le funzioni e almeno un file header contenente le dichiarazioni delle strutture dati ed i prototipi delle funzioni.
3. Deve essere privilegiata l'allocazione dinamica della memoria
- In sede di esame il codice deve essere compilato da linea di comando usando il gcc.

Testing
Deve essere effettuato un ampio test funzionale che metta in evidenza l'uso di tutte le opzioni e il corretto controllo delle situazioni di errore (descritte al punto 3)