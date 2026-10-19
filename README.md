myPreCompiler: C Static Analyzer
A command-line tool written in C that analyzes C source files and reports variable declaration errors, without compiling the code. Built by a team of two for a university course at Sapienza University of Rome.
What it checks
Invalid identifiers: variable names that break C naming rules (e.g. starting with a digit).
Invalid data types: declarations using types that are neither standard C types nor defined earlier with `typedef`, including `typedef struct` with multiple aliases.
Unused variables: global and local variables that are declared but never used.
For every error, the tool reports the line number. It also prints processing statistics: variables checked, total errors, and counts per error type.
Build
```bash
make
```
Or directly:
```bash
gcc -Wall -Wextra -o myPreCompiler main.c controllovar.c strutturedati.c supporto.c
```
Usage
```bash
./myPreCompiler -i <input_file> [-o <output_file>] [-v]
```
Option	Description
`-i`, `--in`	C source file to analyze (required)
`-o`, `--out`	Write the statistics to this file instead of stdout
`-v`, `--verbose`	Also print the statistics to stdout
Example:
```bash
./myPreCompiler -i files/5_test_mixed.c -v
```
Example
Input (`files/5_test_mixed.c`, excerpt):
```c
typedef int 1badAlias;       // line 4: invalid alias name
int 2bad_global;             // line 7: invalid identifier
float unused_global;         // line 8: never used

int main() {
    int 3invalid;            // line 12: invalid identifier
    unknownType wrong;       // line 14: unknown type
    float unused_local;      // line 16: never used
    ...
}
```
Result: 10 variables checked, 4 errors (3 invalid names, 1 invalid type) and 4 unused variables, each reported with its line number. Program output is in Italian.
Test files
The `files/` folder contains test inputs for each case: valid code, bad names, bad types, unused variables, multiple declarations per line, and a mixed file.
Implementation notes
Custom dynamic array: a generic `List` (`void**` with size and capacity) that doubles its capacity with `realloc` when full, giving amortized O(1) appends.
Manual memory management: strings are copied with `strdup` before tokenizing, since `strtok` modifies its input, and every allocation is freed on all exit paths.
Error handling: invalid command-line options, file open/read/write/close failures and empty input files are all reported.
Assumptions
As defined by the assignment, the input contains a single `main` function, with globals declared before `main` and locals declared at the start of it, on contiguous lines.
