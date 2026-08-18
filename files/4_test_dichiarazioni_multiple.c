typedef struct { int x; } Punto;
typedef unsigned long ulong;

int main() {
    int a, b, c;                   
    float x, y;                    
    Punto p1, p2;                  
    ulong u1, u2, u3;              
    int d, 2invalid, e;            
    float ok, bad@name, fine;      
    int used_var;                  
    int unused_var;                

    a = 10;
    b = a + 5;
    c = 20;
    x = 3.14;
    y = x * 2;
    p1.x = 1;
    p2 = p1;
    u1 = 42;
    u2 = u1 + 1;
    used_var = a + b;
    return 0;
}