typedef unsigned long ulong;
typedef struct { int x; int y; } Point;
typedef Point* PointPtr;

int main() {
    int counter;
    float salary;
    char name;
    ulong big_number;
    Point p;
    PointPtr pp;

    counter = 10;
    salary = 1500.50;
    name = 'A';
    big_number = 999999;
    p.x = 1;
    pp = &p;

    return 0;
}
