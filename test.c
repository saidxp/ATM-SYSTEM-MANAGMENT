#include <stdio.h>


int main()
{
    double Amount = 1023.20;


    double tempo = Amount * 7;
    double rate = tempo / 100;
    double t = rate / 12;
    printf("%.2f", t);
}