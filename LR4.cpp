// LR4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "BMPpicture.h"
int main(int argc,char *argv[])
{
    if (argc != 4)
        return -1;
    double n;
    string s(argv[3]);
    n=stod(s);
    BMPpicture test;
    test.copyBigger(argv[1],argv[2],n);
    return 0;
}
