#include "header.h"
#include <unistd.h>

void main()
{

    double i = 3.00;
    double Amount = 1001.20;
    double tempo = Amount * 8;
    double rate = tempo / 100;
    double t = rate * i;
    printf("%.2lf\n", t);
}