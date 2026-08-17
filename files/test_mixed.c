typedef unsigned long ulong;
typedef struct { int a; } MyStruct;
typedef MyStruct* MyStructPtr;
typedef int 1badAlias;

int global_ok;
int 2bad_global;
float unused_global;

int main() {
    int valid;
    int 3invalid;
    ulong big;
    unknownType wrong;
    MyStructPtr ptr;
    float unused_local;
    char also_unused;

    valid = 1;
    big = 100;
    ptr = 0;

    return 0;
}
