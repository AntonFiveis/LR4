// LR4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "BMPpicture.h"
#include <iostream>
using namespace std;
int main(int argc,char *argv[])
{
    int n;
    string s(argv[3]);
    n=stoi(s);
    BMPpicture test(argv[1]);
    test.copyBigger(argv[2],n);
   
}
